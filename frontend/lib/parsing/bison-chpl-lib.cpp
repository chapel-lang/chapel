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
#line 326 "chpl.ypp"

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
#define YYLAST   17878

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  182
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  203
/* YYNRULES -- Number of rules.  */
#define YYNRULES  729
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1257

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
    1135,  1139,  1143,  1148,  1152,  1156,  1164,  1169,  1175,  1180,
    1189,  1190,  1194,  1199,  1207,  1216,  1224,  1235,  1243,  1244,
    1245,  1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,
    1255,  1256,  1257,  1258,  1259,  1271,  1275,  1280,  1285,  1291,
    1300,  1301,  1305,  1314,  1318,  1322,  1326,  1330,  1334,  1343,
    1344,  1345,  1349,  1350,  1351,  1352,  1353,  1357,  1358,  1359,
    1371,  1372,  1373,  1378,  1379,  1380,  1381,  1382,  1383,  1384,
    1385,  1386,  1387,  1388,  1389,  1390,  1391,  1392,  1393,  1394,
    1395,  1396,  1400,  1401,  1402,  1403,  1404,  1405,  1406,  1407,
    1408,  1409,  1410,  1411,  1418,  1419,  1420,  1421,  1425,  1426,
    1430,  1437,  1447,  1451,  1457,  1457,  1463,  1463,  1472,  1473,
    1474,  1475,  1476,  1477,  1478,  1482,  1487,  1492,  1497,  1504,
    1512,  1513,  1517,  1526,  1533,  1542,  1561,  1560,  1570,  1569,
    1582,  1588,  1591,  1592,  1601,  1602,  1603,  1607,  1630,  1649,
    1668,  1673,  1678,  1683,  1708,  1713,  1718,  1723,  1728,  1733,
    1738,  1743,  1748,  1753,  1758,  1763,  1768,  1773,  1778,  1784,
    1793,  1802,  1806,  1810,  1814,  1818,  1822,  1826,  1830,  1834,
    1839,  1844,  1849,  1857,  1873,  1892,  1896,  1903,  1904,  1909,
    1914,  1915,  1916,  1917,  1918,  1919,  1920,  1921,  1922,  1923,
    1924,  1925,  1930,  1935,  1936,  1937,  1938,  1947,  1948,  1952,
    1956,  1960,  1967,  1971,  1975,  1982,  1986,  1990,  1994,  2001,
    2002,  2006,  2010,  2014,  2021,  2035,  2052,  2060,  2065,  2075,
    2076,  2080,  2084,  2089,  2098,  2103,  2107,  2114,  2115,  2119,
    2129,  2135,  2147,  2154,  2155,  2156,  2160,  2161,  2165,  2169,
    2173,  2177,  2181,  2189,  2208,  2221,  2228,  2233,  2240,  2239,
    2249,  2255,  2254,  2269,  2276,  2288,  2306,  2303,  2331,  2335,
    2336,  2338,  2343,  2344,  2348,  2349,  2353,  2356,  2358,  2363,
    2364,  2375,  2395,  2394,  2410,  2409,  2427,  2437,  2434,  2461,
    2469,  2477,  2488,  2499,  2508,  2523,  2524,  2528,  2529,  2530,
    2539,  2540,  2541,  2542,  2543,  2544,  2545,  2546,  2547,  2548,
    2549,  2550,  2551,  2552,  2553,  2554,  2555,  2556,  2557,  2558,
    2559,  2560,  2561,  2562,  2563,  2567,  2568,  2569,  2570,  2571,
    2572,  2573,  2574,  2575,  2576,  2577,  2578,  2583,  2584,  2588,
    2589,  2593,  2594,  2598,  2599,  2603,  2604,  2608,  2609,  2613,
    2617,  2621,  2626,  2631,  2636,  2641,  2649,  2653,  2661,  2662,
    2663,  2664,  2665,  2666,  2667,  2668,  2669,  2673,  2674,  2675,
    2676,  2677,  2678,  2682,  2683,  2684,  2688,  2689,  2690,  2691,
    2692,  2693,  2694,  2698,  2699,  2702,  2703,  2707,  2708,  2712,
    2717,  2718,  2720,  2722,  2724,  2729,  2731,  2736,  2738,  2740,
    2742,  2744,  2746,  2748,  2753,  2754,  2755,  2759,  2768,  2772,
    2780,  2784,  2791,  2814,  2815,  2820,  2821,  2822,  2823,  2824,
    2829,  2828,  2838,  2837,  2847,  2852,  2860,  2864,  2871,  2891,
    2910,  2914,  2921,  2923,  2925,  2932,  2933,  2934,  2938,  2942,
    2943,  2944,  2948,  2949,  2953,  2954,  2958,  2959,  2960,  2961,
    2965,  2966,  2970,  2974,  2975,  2981,  2982,  2986,  2990,  2994,
    3001,  3002,  3006,  3011,  3020,  3021,  3025,  3026,  3033,  3034,
    3035,  3036,  3037,  3038,  3040,  3042,  3044,  3046,  3052,  3054,
    3057,  3059,  3061,  3063,  3065,  3067,  3069,  3071,  3073,  3075,
    3080,  3084,  3088,  3092,  3096,  3105,  3114,  3126,  3130,  3134,
    3138,  3142,  3146,  3150,  3154,  3161,  3179,  3187,  3188,  3189,
    3190,  3191,  3192,  3193,  3197,  3198,  3202,  3206,  3213,  3220,
    3230,  3231,  3235,  3250,  3254,  3261,  3262,  3263,  3264,  3265,
    3266,  3270,  3272,  3274,  3276,  3278,  3284,  3291,  3303,  3315,
    3328,  3345,  3352,  3357,  3362,  3367,  3373,  3379,  3409,  3416,
    3423,  3424,  3428,  3430,  3431,  3433,  3435,  3436,  3437,  3438,
    3441,  3442,  3443,  3444,  3445,  3446,  3447,  3448,  3449,  3453,
    3454,  3458,  3459,  3460,  3464,  3465,  3466,  3467,  3476,  3477,
    3480,  3481,  3482,  3486,  3498,  3510,  3517,  3519,  3521,  3523,
    3525,  3527,  3536,  3542,  3546,  3550,  3557,  3558,  3562,  3563,
    3567,  3568,  3569,  3570,  3571,  3572,  3573,  3574,  3579,  3584,
    3589,  3594,  3599,  3608,  3613,  3622,  3623,  3624,  3625,  3626,
    3627,  3628,  3629,  3630,  3631,  3632,  3633,  3634,  3635,  3636,
    3637,  3638,  3639,  3640,  3641,  3642,  3643,  3644,  3648,  3649,
    3650,  3651,  3652,  3653,  3656,  3660,  3664,  3668,  3672,  3679,
    3683,  3687,  3691,  3699,  3700,  3701,  3702,  3703,  3704,  3705
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

#define YYPACT_NINF (-894)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-730)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -894,    65,  3756,  -894,   -81,    53,  -894,  -894,  -894,  -894,
    -894,  -894,  -894,  -894, 11934,    14,    90,   103, 13795,   192,
   17649,    14,   278,    37,   286,    90,  5324, 11934,   220, 17705,
    -894,   403,   392,  -894, 10019,   407,   396,   300,  -894,   424,
     429, 17705, 17705, 17705,  -894,  2638, 10887,   444, 11934, 11934,
     222,  -894,   460,   497, 11934,  -894, 13795,  -894, 11934,   477,
     384,   297,   381, 13145,   503, 17761,  -894, 11934,  8113, 11934,
   10887, 13795, 11934,   465,   517,   406,  5324,   523, 11934,   524,
    7075,  7075,  -894,   526,  -894, 13795,  -894,   530, 10192,  -894,
   11934,  -894, 11934,  -894,  -894, 13622, 11934,  -894, 10365,  -894,
    -894,  -894,  4268,  8286, 11934,  -894,  4972,  -894,  -894,  -894,
    -894, 17367,   590,  -894,   455,   435,  -894,    25,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  7248,  -894,  7421,  -894,  -894,    80,  -894,  -894,  -894,
     551,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,   463,
   17705,  -894,   449,   436,   350,  -894,   204,  -894,  -894,  -894,
     361,   374,  -894,  -894, 17705,  1794,  -894,   461,   447,  -894,
    -894,  -894,   470,   475, 11934,   476,   478,  -894,  -894,  -894,
   17184,  1558,    87,   480,   481,  -894,  -894,   230,  -894,  -894,
    -894,  -894,  -894,   282,  -894,  -894,  -894, 11934,  -894, 17705,
    -894, 11934, 11934,    54,   558,   262,  -894,  -894,  -894,  -894,
   17184,   333,  -894,  -894,    42,  5324,  -894,  -894,  -894,   482,
     168,   450,  -894,   225,  -894,   491,  -894,   142, 17184,  8459,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894, 17705,
   17705,   -28, 14467,  -894,  -894,   538,  8459,   496,   494,   366,
    5324,  3914,    91,    29,  -894,  5324,  -894,  -894, 15297,  -894,
      11, 15828,   597,  -894,   495,   498,  -894, 15297,   168,   597,
    -894,  8459,  2211,  2211,    59,     3,  -894,     5,  -894,  -894,
    -894,  -894,  7767,  7767,  -894,   521,  -894,   457,   527,  -894,
    -894, 14018,   525,  -894,  -894, 17184,   233, 11934, 11934,  -894,
     -40,  -894,  -894, 17184,   508, 15868,  -894, 15297,   168, 17184,
     510,  8459,  -894, 17184, 15908, 11934,  -894,  -894,  -894,  -894,
    -894,   168,   515,   355,   355,  1183,   597,   597,   210,  -894,
    -894,  4444,   130, 11934,   536,   -73,   511,  1183,   673,  -894,
    -894, 17705,  -894, 11934,  -894,  4268,  -894,   122,  -894,   538,
    -894,  -894,   683,   516,  4620, 11934,  -894, 11934,   623,  -894,
    -894, 14967,   196,   463, 17184,   645, 11934,  5324,   611,  -894,
    -894, 10019, 10538, 11060,  -894,  -894,  -894, 17705, 11934,   522,
    -894, 17445,   314,   361,  -894,  -894,  -894,  -894,   602,  -894,
    -894,  -894,  -894,   128,   605,  -894,  -894,  -894, 13454,   575,
     244,  -894,   539,   260,   365,  -894,   557, 11934, 11934, 11934,
   11060, 11060, 11934,   378, 11934, 11934, 11934, 11934, 11934,   249,
   13622, 11934, 11934, 11934, 11934, 11934, 11934, 11934, 11934, 11934,
   11934, 11934, 11934, 11934, 11934, 11934, 11934,   639,  -894,  -894,
    -894,  -894,  -894, 10192, 10192,  -894,  -894,  -894,  -894, 10192,
    -894,  -894, 10192, 10192,  8459,  8632, 11060, 11060, 15988,   542,
    -894, 11233, 11934, 17705,  -894,  6377,  -894,  -894,  -894,  2638,
    -894, 11934,  -894,   586,   547,   568,  -894,  -894,   596,   598,
    5324,   690,  5324,  -894,   693, 11934,   662,   560,  -894, 10192,
   17705,  -894,  -894,  3914,  -894,  -894,    76,  -894, 12107,   606,
   11934,  2638,  -894,  -894, 11934,  -894, 17501, 11934, 11934,  -894,
     225,   561,  -894,  -894,  -894,  -894, 17705,  -894,   361, 13318,
    3914,   584, 12107,  -894, 17184, 17184,  -894,   450,  -894,    38,
    -894,  8459,   562,  -894, 15297,   711,   711,  -894,  -894,  -894,
    -894, 11409,  -894, 16028,  8808,  8981,  -894,  -894,  -894, 17184,
   11060, 11060,  -894,   455,  9154,  -894,   232,  -894,  4796,  -894,
     276, 16068,   299, 15058, 17705,  6899,  6726,  -894,   463,   566,
    -894,   225,  -894, 15378,   622, 17705,    18, 15297,   570, 13912,
     -38,     9, 16148,   -61,    21,  1273,   -67,   612,  1825,   573,
   17593,   624,   167,  -894,  -894,    89,  -894,  -894,    47,  -894,
    -894,  6553,  -894,   647,  -894,   595,   613,  -894,   619,  -894,
     620,   621,   625,   626,   627,  -894,   628,   629,   630,   631,
     632,   633,   634,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,  -894,  -894,  -894, 11934,   643,   637,   595,  -894,
     595,  -894, 11934,  -894, 17705,  -894,  -894,  -894,  3065,  3065,
     483,  -894,   483,  -894,   483, 13016,  1502,  1435,  1463,   168,
     355,  -894,   391,  -894,  -894,  -894,  -894,  -894,  1183,  3228,
     483,   509,   509,  3065,   509,   509,  1310,   355,  3228,  3904,
    1310,   597,   597,   355,  1183,   635,   636,   638,   640,   648,
     649,   615,   614,  -894,   483,  -894,   483, 11934, 10192,     4,
   15457,   599,   618,   172,  -894,  -894,  -894,  -894,   113,  -894,
     458, 17224,   341,   303, 17184, 10192,  -894, 10192, 11934, 11934,
     758,  5324,   759, 16227,  5324, 15537, 17705,  -894,   117,  -894,
     118,  -894,   124,  3914,    91,  -894, 17184,  9327,  -894, 17184,
    -894,  -894,  -894, 17705, 16268, 16308,  2638,  -894,   584,   652,
    -894,   126, 12107,  -894,   102,  -894, 11934,  -894,   644,    68,
     646,  -894,  2072,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,    60,  3574,  -894,  -894,   133,  -894,  -894,  -894,  -894,
   11934,  -894,    64, 14121, 11934,  -894, 11934,  6899,  6726,  -894,
    9500,   367,  9673,   412,   438,  9846,  7594,   441,   304,  -894,
    -894,  -894, 16388,   655,   651,   650,  -894, 17705,  2638, 11060,
      18, 11934,   748,  -894,  -894, 11934, 17184,  5324,   642, 11060,
   11582,  4092,   656, 11934, 11758,  -894,  -894,    18,  5500, 11060,
    -894,    18,  -894, 10192,   653,  2193,  -894,  -894, 11934,    50,
    -894,   802,  -894,   647,  -894,  -894,  -894,  -894,  -894,    55,
     534,  -894, 16428,  -894, 14281,  -894,  -894, 17184,  -894,   657,
     659,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,   148,
   11060, 13912, 12280, 11060, 10192,  -894,  -894,  -894,  7940,  -894,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,    49, 11060, 11060,  -894,  -894,  -894, 17184, 17184,  5324,
    -894,  5324, 11934,  -894,  5324,   765,   658,   660, 17705,   450,
    -894,  -894,   606,  -894,  -894,  -894,   663,   664,   312,   102,
   11934,   647,   584,  -894, 10714,  -894,  -894, 17184,  -894,  5324,
   11934,  -894,  -894,  -894, 17705,  -894,   676,   450, 11060,  5324,
   11060, 10192,  -894,  -894,  -894,  -894,  -894,   391,  -894,  -894,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,  6899,  -894,  -894, 16474, 15629,   323,   667,  6899,
    -894, 11934,  -894,  -894,  2995,   326,   327,    14, 16580,  -894,
   16626, 11934,   173,  -894,  5324,   -23, 15708,  -894,  -894, 17184,
    5324, 17184,  -894, 16672,  -894, 10019, 17184,    28, 15137,  -894,
     179,  -894,  -894,  -894,  -894,   675,  1004,  -894, 17184, 17705,
     590,  -894,    55,   240, 11934,  -894,   661,   665,  -894,   595,
     595,  -894,  -894,  -894,   666, 16778, 11934,   772, 16824,   184,
    -894,   606,   185,   195,  -894,  -894, 17184,   787,  5324,  -894,
    -894,  -894,  -894,  -894,  8459,  8459,  -894,  -894,  -894,  -894,
    -894,   102, 17184,  -894,   162,    85,   669, 11934,  -894,    58,
   15788,    21, 15218,  -894,   176, 11934,  6899,  6726,  -894,  -894,
    -894, 16864, 17184, 14439,  -894,  -894,  -894,    18,    18,  -894,
   15297,  -894,  -894,  5676,   668,  5852,   670,  -894, 11934, 14546,
    6028,  -894,    18,  6204,  -894,    18,   672,  1004,  -894,  -894,
    -894, 17593,  -894,  -894,  -894,    78,  -894,   -46,  -894,   212,
   16984,    61,  -894,  -894,  -894, 12453, 12626, 16944, 12799, 12972,
    -894,  -894,  5324,  -894,   674,   684,  -894,  -894,  5324,  -894,
     450, 17184,  5324,  5324,  -894, 17184,   329,   694, 11934,  -894,
    -894,  -894,  -894, 10019,  -894, 17184,  5324, 10019,  -894, 17184,
    5324, 17184,  5324, 10019, 17184,  -894, 10019, 17184,  -894,  -894,
    -894,  -894,    36,  -894, 11934,   240,    78,    78,    78,    78,
      78,    78,   240,  5148,  -894,  -894,  -894,  -894, 11934, 11934,
   11934, 11934, 11934,  -894,   697,   698,   622,  -894,  -894,  -894,
   17184, 14627,  -894, 14706,  -894, 16227, 14787, 14866,  -894, 17184,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,   692,  -894, 17024,
   17064, 17104, 17144,  -894,  -894,  5324,  5324,  5324,  5324, 11934,
   11934, 11934, 11934, 16227, 16227, 16227, 16227
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   140,   459,    55,   672,   673,
     674,   668,   669,   675,   211,   584,   129,   162,   556,   169,
     558,   584,   168,   490,   488,   129,     0,   211,   277,   163,
     201,   200,   666,   199,   211,   167,    68,   278,   340,   164,
     141,     0,     0,     0,   335,     0,   211,   171,   211,   211,
     601,   576,   676,   172,   211,   341,   550,   485,   211,     0,
       0,   186,   184,   426,   166,   559,   487,   211,   211,   211,
     211,   554,   211,     0,   170,     0,     0,   142,   211,   667,
     211,   211,   478,   165,   315,   552,   489,   173,   211,   725,
     211,   727,   211,   728,   729,   617,   211,   726,   211,   625,
     182,   724,     0,   211,   211,     4,     0,     5,    10,    11,
      12,    48,    51,    52,    56,     0,    46,    71,    13,    77,
      14,    15,    16,    17,    30,   536,   537,    23,    47,   183,
     193,   211,   202,   639,   194,    18,     0,    32,    31,    20,
       0,   272,    19,   635,    22,    36,    33,    34,   192,   316,
       0,   190,     0,     0,   632,   339,     0,   629,   188,   356,
     446,   437,   627,   191,     0,     0,   189,   644,   623,   622,
     626,   540,   538,     0,   211,   633,   634,   638,   637,   636,
       0,   539,     0,   645,   646,   647,   670,   671,   628,   542,
     541,   630,   631,     0,    29,   558,   163,   211,   141,     0,
     559,   211,   211,     0,     0,   632,   644,   538,   633,   634,
     548,   539,   645,   646,     0,     0,   585,   130,   131,     0,
     557,     0,   479,     0,   486,     0,    21,     0,   525,   211,
     137,   143,   155,   149,   148,   157,   134,   147,   158,   144,
     138,   159,   132,   160,   153,   146,   154,   152,   150,   151,
     133,   135,   145,   156,   161,   139,   325,   136,   210,     0,
       0,     0,     0,    70,    69,    71,   211,     0,     0,     0,
       0,     0,     0,     0,   496,     0,   179,    41,     0,   307,
       0,   306,   710,   605,   602,   603,   604,   492,   551,   711,
       7,   211,   338,   338,   433,   176,   428,   177,   429,   430,
     434,   435,   175,   174,   436,   426,   521,     0,   343,   344,
     346,     0,   427,   520,   348,   508,     0,   211,   211,   180,
     629,   620,   643,   621,     0,     0,    44,     0,   555,   543,
       0,   211,    45,   549,     0,   211,   285,    48,   289,   286,
     289,   553,     0,   712,   714,   615,   709,   708,     0,    74,
      78,     0,     0,   211,     0,     0,   527,   616,     0,     6,
     313,     0,   200,   211,   314,     0,    49,     0,     9,    71,
      50,    53,     0,    60,     0,   211,    72,   211,     0,   490,
     198,     0,   668,   316,   640,   206,   211,     0,   214,   215,
     216,   211,   211,   211,   213,   560,   567,     0,   211,     0,
     312,     0,   426,   446,   444,   445,   443,   364,   448,   447,
     451,   450,   452,     0,   441,   438,   439,   442,     0,   483,
       0,   480,     0,     0,   624,    35,   606,   211,   211,   211,
     211,   211,   211,   713,   211,   211,   211,   211,   211,     0,
     614,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   395,   402,   403,
     404,   399,   401,   211,   211,   397,   400,   398,   396,   211,
     406,   405,   211,   211,   211,   211,   211,   211,     0,     0,
     214,   211,   211,     0,   589,     0,    37,    25,    38,     0,
      26,   211,    39,   536,     0,   531,   532,   535,     0,     0,
       0,   246,     0,   395,   242,   211,     0,     0,    24,   211,
       0,   256,    27,     0,   500,   502,     0,   519,   211,   505,
     211,     0,   178,    40,   211,   309,     0,   211,   211,    42,
       0,     0,   187,   185,   431,   432,     0,   427,   446,   426,
       0,   523,   211,   122,   641,   642,   354,     0,   181,     0,
      43,   211,     0,   296,     0,   287,   288,    28,    76,    75,
      79,   211,   677,     0,   211,   211,   664,   662,     8,   492,
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
     411,   365,   211,   482,     0,   477,   495,   494,   705,   704,
     707,   716,   715,   720,   719,   701,   698,   699,   700,   618,
     688,   140,     0,   658,   659,   142,   657,   656,   612,   692,
     703,   697,   695,   706,   696,   694,   686,   691,   693,   702,
     685,   689,   690,   687,   613,     0,     0,     0,     0,     0,
       0,     0,     0,   718,   717,   722,   721,   211,   211,     0,
       0,   283,     0,   595,   596,   598,   600,   587,     0,   590,
       0,     0,     0,     0,   526,   211,   545,   211,   211,   211,
     245,     0,   241,     0,     0,     0,     0,   544,     0,   259,
       0,   257,     0,   503,     0,   518,   517,   211,   498,   611,
     497,   308,   305,     0,     0,     0,     0,   655,   523,   349,
     345,     0,   211,   524,   505,   347,   211,   353,     0,     0,
       0,   546,     0,   290,   678,   619,   663,   528,   527,   665,
     529,     0,     0,    57,    58,     0,    61,    63,    67,    66,
     211,    97,     0,     0,   211,    92,   211,   211,   211,    64,
     211,   370,   211,   371,   372,   211,   211,   382,     0,   407,
     408,    81,    80,    91,     0,     0,   318,     0,     0,   211,
       0,   211,     0,   219,   218,   211,   569,     0,     0,   211,
     211,     0,     0,   211,   211,   681,   230,     0,     0,   211,
     228,     0,   280,   211,     0,   338,   324,   330,   211,   328,
     323,   426,   414,   453,   511,   510,   509,   512,   454,   460,
     426,   363,     0,   369,     0,   359,   360,   484,   481,     0,
       0,   128,   126,   127,   125,   124,   123,   653,   654,     0,
     211,   679,   211,   211,   211,   588,   597,   599,   211,   586,
     162,   169,   168,   167,   164,   171,   172,   166,   170,   165,
     173,     0,   211,   211,   491,   534,   533,   254,   253,     0,
     248,     0,   211,   244,     0,   250,     0,   282,     0,     0,
     501,   504,   505,   506,   507,   304,     0,     0,     0,   505,
     211,   453,   523,   522,   211,   422,   420,   355,   298,     0,
     211,   297,   300,   547,     0,   291,   294,     0,   211,     0,
     211,   211,    59,   121,   118,    98,   110,   104,   103,   102,
     112,    99,   113,   108,   101,   109,   107,   105,   106,   100,
     111,   114,   211,   117,   116,    88,    87,     0,     0,   211,
     196,   211,   197,   318,   338,     0,     0,   584,   584,   222,
       0,   211,     0,   238,     0,     0,     0,   680,   236,   570,
       0,   683,   682,     0,   231,   211,   563,     0,     0,   229,
       0,   311,   186,   184,   310,   338,   338,   319,   334,     0,
       0,   416,   460,     0,   211,   357,   418,     0,   366,   411,
     411,   660,   661,   282,     0,     0,   211,     0,     0,     0,
     591,   505,   644,   644,   247,   243,   575,   249,     0,    73,
     279,   258,   255,   499,   211,   211,   493,   423,   421,   350,
     351,   505,   410,   302,     0,     0,     0,   211,   292,     0,
       0,     0,     0,    62,     0,   211,   211,   211,    93,    95,
      84,    83,    82,   338,   203,   209,   207,     0,     0,   217,
       0,   240,   239,     0,     0,     0,     0,   237,   211,     0,
       0,   226,     0,     0,   224,     0,   284,   338,   321,   320,
     329,     0,   336,   474,   475,     0,   476,   462,   465,     0,
     461,     0,   412,   361,   362,   211,   211,     0,   211,   211,
     284,   592,     0,   252,     0,     0,   425,   424,     0,   301,
       0,   295,     0,     0,   119,    89,     0,     0,   211,   204,
     221,   220,   223,   211,   234,   572,     0,   211,   232,   571,
       0,   684,     0,   211,   562,   227,   211,   561,   225,   281,
     322,   332,     0,   473,   211,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   455,   457,   358,   456,   211,   211,
     211,   211,   211,   251,   607,   608,   303,   293,    94,    96,
      85,     0,   235,     0,   233,   566,     0,     0,   337,   464,
     466,   467,   470,   471,   472,   468,   469,   463,   458,     0,
       0,     0,     0,   609,   610,     0,     0,     0,     0,   211,
     211,   211,   211,   574,   573,   565,   564
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -894,  -894,  -894,     0,   -52,  2337,  -894,   446,   753,  -894,
    -894,   462,  -397,  -111,  -894,   292,  -894,  -894,  -112,  -894,
    -894,    23,   604,  -894,  -539,  2865,   502,  -572,  -894,  -791,
    -894,  -894,  -894,    62,  -894,  -894,  -894,   845,  -894,  2530,
    -163,  -894,  -894,  -454,   270,  -894,  -834,  -894,  -894,   279,
    -894,  -894,  -894,  -894,  -894,  -894,  -894,  -894,    10,  -894,
    -158,   504,  -304,  -894,   487,  -894,  -894,   -58,  1468,  -894,
    -894,  -894,  -894,   541,  -894,   -79,  -894,  -894,  -894,  -894,
     359,  -894,  -894,  -894,   -98,  -894,  -342,  -789,  -894,  -894,
    -894,  -894,  -894,  -600,  -894,    44,  -894,  -894,  -894,  -894,
    -894,   347,  -894,  1520,  -894,  -894,  -894,  -894,   505,  -894,
    -894,  -894,  -894,     8,  -402,  -174,  -773,  -893,  -649,  -894,
       7,  -894,    17,   -49,   588,   -48,  -894,  -894,  -332,  -810,
    -894,  -303,  -894,  -146,  -305,  -297,  -607,  -894,  -894,    63,
     227,  -894,  -201,   768,  -894,  -894,  -159,   383,   143,  -292,
    -758,  -598,  -894,  -894,  -894,  -593,  -505,  -894,  -736,   -18,
    -208,  -894,  -263,  -477,  -554,    -2,  -894,  -894,  -894,  -894,
    -894,  -894,    56,  -894,   -16,  -571,  -894,    -3,  -894,  -894,
     353,   559,  -894,  -894,  -894,  -894,  2431,  -894,   -50,   771,
    -894,  1173,  1294,  -894,  -894,  -894,  -894,  -894,  -894,  -894,
    -894,  -894,  -468
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   350,   106,   107,   108,   109,   110,   367,
     368,   111,   112,   113,   114,   373,   575,   805,   806,   115,
     116,   117,   378,   118,   119,   120,   351,   833,   582,   834,
     121,   122,   993,   579,   580,   123,   124,   219,   255,   272,
     125,   257,   126,   306,   529,   127,   128,   293,   292,   129,
     130,   131,   132,   133,   134,   591,   837,   135,   203,   137,
     393,   394,   600,   138,   261,   139,   760,   761,   204,   141,
     142,   143,   144,   555,   793,   967,   145,   146,   789,   962,
     279,   280,   147,   148,   149,   150,   399,   865,   151,   152,
     612,  1039,  1040,   613,   153,   205,  1192,   155,   156,   307,
     308,   309,   951,   157,   321,   547,   786,   158,   159,  1141,
     160,   161,   667,   668,   829,   830,   831,   955,   881,   403,
     615,  1047,   616,   617,   311,   537,   418,   407,   413,   879,
    1206,  1207,   162,  1045,  1137,  1138,  1139,   163,   164,   420,
     421,   673,   165,   166,   223,   530,   273,   274,   515,   516,
     768,   313,   877,   622,   623,   519,   314,   783,   784,   352,
     354,   355,   494,   495,   496,   206,   168,   169,   395,   170,
     396,   171,   207,   173,   215,   216,   738,   739,   740,   174,
     208,   209,   177,   178,   179,   322,   180,   385,   497,   211,
     182,   212,   213,   185,   186,   187,   188,   603,   189,   190,
     191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   371,   105,   507,   610,   221,   423,   473,   787,   227,
     867,   305,   136,   828,   310,   312,   669,   742,   324,   885,
     807,   886,   489,   876,   167,   873,   956,   998,   531,   848,
     852,  1037,   758,   857,   861,   383,   136,   785,   342,   788,
     994,   589,   949,   484,   275,   482,   154,  1004,   517,   316,
     517,   275,   900,   356,   275,  -331,  1077,   849,   172,   265,
    -277,  1120,  -278,  1042,   765,     3,   493,   691,   552,  1203,
     154,   618,   375,  1194,   167,   565,   500,   520,   167,   167,
    -516,  1133,   172,   214,   546,   194,   136,   854,   505,   604,
     136,   136,   517,   217,  1203,   480,   389,   390,   214,   862,
     167,   566,  1195,   493,   167,   386,   863,   534,   968,  -516,
    -260,   376,   136,   387,  -299,   855,   136,  1043,   275,   198,
     154,   388,   389,   390,   154,   154,   681,   683,   493,   620,
     214,  -352,   172,  1134,   369,   214,   172,   172,   847,  -516,
     377,  1080,   214,   365,   959,   218,   154,   386,   222,   214,
     154,   535,   850,  1113,  -516,   387,  -299,   850,   172,   524,
    -261,  -514,   172,   388,   389,   390,  -516,   875,   493,   695,
    1037,   942,   723,   725,  1044,  -516,   229,   521,   266,   214,
     901,   214,   365,  1136,  1073,   851,   960,   386,  1156,   365,
    -514,  1078,   365,   926,   518,  1158,   518,  -516,  -327,   365,
     391,  1128,  1129,   388,   389,   390,   779,   365,   491,   571,
    -299,   721,   722,   167,  1103,   485,  1081,  -516,  -516,   360,
     906,   762,  -327,  -516,   763,   136,  1100,  1204,   767,   481,
     852,   861,   365,   491,  1162,  -514,   969,   871,   518,  -516,
     961,   283,   391,  1133,   670,   997,   835,  -514,   781,  -267,
     764,   899,   691,    24,   954,   392,   365,  -516,   404,   154,
     474,   908,   475,   872,   907,   747,   938,   604,   167,  1037,
     692,   172,   763,   167,   763,   621,   697,   953,   561,   405,
     136,   971,   391,   693,   588,   136,   364,   909,   790,   406,
     491,   937,   939,  1190,  -514,  1134,   747,   570,   940,  -514,
     952,   284,   562,  1151,   198,    57,  1167,   972,   492,    84,
     433,   493,   493,   694,   154,   869,   277,    66,   439,   154,
     285,   491,  1053,  1157,   999,   773,   172,   747,  1061,   776,
     743,   172,   747,  -594,  1135,  -266,   286,   392,  1196,   870,
     326,    86,   294,  -593,   -70,  1136,   493,  1111,  1164,   167,
     305,   560,  1079,  1126,   695,   175,   797,   800,  1150,  -594,
     696,   136,   296,   167,   -70,  1197,   298,  1198,  1199,  -593,
    1200,  1201,   167,   476,   601,   136,   194,   477,   224,   175,
     607,   491,   558,   -70,   136,   167,  1030,   429,   493,   408,
     838,   299,   674,   229,   300,   154,    59,   136,   194,   543,
    1143,  1144,   414,  -652,   808,  -652,   301,   172,   521,   154,
     675,   493,   -70,   716,   717,   258,   671,   807,   154,   718,
    1094,   172,   719,   720,   810,   304,   677,  1059,   -69,   175,
     172,   154,   923,   175,   175,  -651,   477,  -651,   409,  1130,
     742,   410,   811,   172,  1114,  1116,   430,   814,   -69,  -275,
     431,   521,   999,   411,   415,   175,  1122,  1125,  -727,   175,
     521,   691,  -727,   729,  -265,   815,   416,   -69,  1036,   924,
    1000,   999,   412,   266,   674,   521,   910,   999,  1076,   911,
     263,  -262,   669,   264,   912,   417,   290,   305,   614,  1098,
     310,   312,  1105,  1106,   196,  1218,   -69,   433,   167,   371,
     167,  -269,   437,  -728,   913,   439,  -648,  -728,  -648,   442,
     136,   914,   136,   198,   798,   798,  -581,  -273,  1114,  1116,
    1122,  1125,   915,  -651,  1166,  -651,   336,   339,  1193,  -729,
     916,  1191,  -724,  -729,   267,  1007,  -724,   365,  -650,   510,
    -650,   429,   337,   337,   154,  1015,   154,   917,   523,   294,
     525,  -649,   801,  -649,  -270,  1027,   172,   291,   172,   918,
    -264,   176,   294,   695,   889,   493,   890,   330,   175,   296,
     919,   532,   533,   298,  -268,   920,   167,   840,   560,   331,
    -274,  -276,   296,  -263,   493,   176,   298,  -271,   136,  1231,
    1232,  1233,  1234,  1235,  1236,     7,  1054,   550,   299,  1057,
     430,   300,  1205,   358,   431,   372,   374,  1036,   397,   402,
     398,   299,   424,   301,   300,   483,    59,   948,   681,   723,
     401,   365,   154,   175,   376,   433,   301,  -578,   175,   429,
     437,   538,   304,   439,   172,   176,  -577,   442,  1127,   176,
     176,   425,  -582,  1131,  -583,   304,  -580,  -579,   487,   974,
     697,   433,   434,  1205,   435,   436,   437,   490,   438,   439,
     440,   176,   508,   442,  1089,   176,  1091,   509,   527,   448,
     449,   528,   542,   452,   548,   539,   453,   454,   455,  1006,
       7,   557,   568,   551,   564,   567,   573,   456,   430,   574,
     584,   595,   431,   608,   619,   925,    38,   624,   360,  -338,
     493,   672,    23,    24,   175,   676,  1036,   439,  -417,   715,
    1050,   361,   745,    30,   362,   728,   747,   944,   175,    33,
    -338,   746,   748,    55,   749,   751,    38,   175,   754,   756,
    -338,   782,   767,   792,   757,   777,   791,   836,   921,   433,
     175,   493,   841,   845,   437,   866,   438,   439,   363,   167,
     868,   442,   167,    55,   878,    57,   590,   358,   449,  1032,
     491,   136,  1033,  -140,   136,   364,   455,    66,   880,  -162,
    -169,  -168,   904,   181,   176,  -167,  -164,  -141,  -171,  -172,
    -166,  -170,  -142,  -165,  -173,   883,    82,   884,    84,   897,
     898,    86,   905,   929,   931,   154,  1011,   181,   154,   950,
    1068,   891,   892,   999,   893,  1148,   894,   172,   493,   871,
     172,  1154,  1155,  1035,   895,   896,   958,  1002,  1014,   305,
     963,  1003,  1152,  1087,  1069,  1031,  1070,  1012,   305,   176,
     294,  1051,  1020,  1099,   176,  1052,  1074,  1075,  1189,  1142,
    1195,   100,  1145,  1160,  1176,   167,  1180,   181,  1214,   167,
     296,   181,   181,   175,   298,   175,   167,   136,  1215,  1093,
    1219,   136,  1243,  1244,   366,   803,   843,   844,   136,   506,
     225,   572,   973,   181,   856,   860,   578,   181,   596,   299,
    1071,   556,   300,   771,    59,  1086,   780,  1046,  1041,  1228,
     592,   154,  1049,   536,   301,   154,  1132,  1237,  1230,   380,
    1005,   888,   154,   172,   770,  1060,   941,   172,     0,     0,
     176,   493,   493,   304,   172,     0,     0,     0,     0,     0,
    1062,  1063,     0,     0,   176,   371,     0,   167,     0,   167,
       0,   175,   167,   176,     0,     0,     0,     0,     0,   136,
       0,   136,  1085,     0,   136,   505,   176,     0,     0,     0,
       0,     0,  1035,     0,     0,     0,     0,   167,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   167,     0,  1084,
       0,     0,     0,   154,     0,   154,     0,     0,   154,   136,
       0,     0,     0,     0,     0,   172,   181,   172,     0,     0,
     172,  1107,  1108,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   154,     0,     0,     0,     0,     0,     7,
       0,     0,   167,   154,     0,   172,   371,     0,   167,     0,
     371,     0,     0,     0,   136,   172,     0,   360,     0,     0,
     136,    23,    24,     0,     0,     0,     0,     0,     0,     0,
     361,   181,    30,   362,     0,     0,   181,   505,    33,   505,
       0,  1035,   505,   505,     0,    38,     0,     0,   154,   176,
       0,   176,     0,     0,   154,     0,   167,     0,     0,     0,
     172,     0,   860,     0,     0,     0,   172,   363,   136,     0,
       0,     0,    55,     0,    57,     0,     0,     0,  1032,     0,
       0,  1033,     0,     0,   364,     0,    66,     0,     0,     0,
       0,     0,     0,     0,   175,     0,     0,   175,     0,     0,
    1009,   167,   154,   167,     0,    82,     0,    84,   167,     0,
      86,   167,   181,   136,   172,   136,     0,  1024,     0,     0,
     136,  1029,     0,   136,     0,     0,   181,   176,     0,     0,
       0,     0,     0,     0,     0,   181,     0,     0,     0,     0,
     167,     0,     0,     0,     0,     0,   167,   154,   181,   154,
     167,   167,   136,     0,   154,     0,     0,   154,   136,   172,
     100,   172,   136,   136,   167,   183,   172,     0,   167,   172,
     167,     0,     0,     0,     0,     0,   136,     0,     0,     0,
     136,     0,   136,     0,     0,     0,   154,     0,     0,   183,
     175,   167,   154,  1238,   175,     0,   154,   154,   172,     0,
       0,   175,     0,   136,   172,   429,     0,     0,   172,   172,
     154,     0,     0,     0,   154,     0,   154,     0,     0,     0,
       0,     0,   172,     0,     0,     0,   172,     0,   172,     0,
       0,     0,     0,   167,   167,   167,   167,   154,     0,   183,
       0,     0,     0,   183,   183,   136,   136,   136,   136,   172,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   181,     0,   181,   430,   183,     0,     0,   431,   183,
       0,     0,   175,     0,   175,     0,   427,   175,     0,   154,
     154,   154,   154,   428,     0,     0,   184,  1121,  1124,     0,
       0,   172,   172,   172,   172,   429,   858,     0,     0,     0,
     176,     0,   175,   176,     0,     0,     0,     0,     0,     0,
     184,   859,   175,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,     0,     0,     0,   442,     0,     0,
       0,     0,   429,   448,   449,     0,     0,   452,     0,   181,
     453,   454,   455,     0,     0,  1159,     0,     0,     0,     0,
       0,  1121,  1124,     0,   430,     0,     0,   175,   431,     0,
     184,     0,     0,   175,   184,   184,     0,  1170,  1171,     0,
    1172,     0,     0,     0,     0,     0,     0,     0,   183,     0,
       0,     0,  1185,     0,   214,  1188,   184,     0,   432,     0,
     184,   430,     0,     0,     0,   431,   176,     0,     0,     0,
     176,     0,     0,     0,     0,   433,   434,   176,   435,   436,
     437,   175,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,     0,
     453,   454,   455,   183,   365,     0,     0,     0,   183,     0,
       0,   456,   433,   434,     0,   435,   436,   437,     0,   438,
     439,     0,     0,     0,   442,     0,   175,   429,   175,     0,
     140,   449,     0,   175,     0,     0,   175,   453,   454,   455,
       0,     0,     0,     0,     0,     0,     0,     0,   176,     0,
     176,     0,     0,   176,   140,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   175,     0,     0,     0,   184,
       0,   175,     0,     0,     0,   175,   175,     0,   176,     0,
       0,     0,   181,     0,   183,   181,   430,     0,   176,   175,
     431,     0,     0,   175,   429,   175,     0,     0,   183,     0,
       0,     0,     0,     0,   140,     0,     0,   183,     0,     0,
       0,     0,     0,     0,   430,     0,   175,     0,   431,     0,
     183,     0,     0,     0,   184,     0,     0,     0,     0,   184,
     140,     0,     0,   176,   140,     0,     0,   433,   434,   176,
       0,   436,   437,     0,   438,   439,     0,     0,   320,   442,
       0,     0,     0,   430,     0,     0,   449,   431,   175,   175,
     175,   175,   453,   454,   455,   433,   434,     0,   320,     0,
     437,     0,   438,   439,     0,     0,     0,   442,   181,     0,
       0,     0,   181,   320,   449,     0,     0,   176,     0,   181,
     453,   454,   455,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   184,     0,     0,     0,   437,
       0,   438,   439,     0,     0,     0,   442,     0,     0,   184,
       0,     0,     0,   449,     0,     0,     0,     0,   184,   453,
     454,   455,   176,   183,   176,   183,     0,     0,     0,   176,
       0,   184,   176,   140,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,     0,
     181,     0,   181,     0,     0,   181,     0,     0,     0,     0,
       0,   176,     0,     0,     0,     0,     0,   176,     0,     0,
       0,   176,   176,     0,     0,     0,     0,     0,   472,     0,
     181,  -648,     0,  -648,     0,   176,     0,     0,   140,   176,
     181,   176,     0,   140,     0,     0,     0,     0,     0,   320,
       0,   183,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   176,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   181,   320,     0,     0,     0,
       0,   181,     0,     0,   184,   422,   184,   230,     0,     0,
       0,     0,     0,     0,   176,   176,   176,   176,     0,     0,
       0,   320,   231,   232,     0,   233,     0,     0,     0,   140,
     234,     0,     0,     0,     0,     0,   864,     0,   235,     0,
    -318,     0,     0,   140,   236,     0,     0,     0,     0,   181,
     237,     0,   140,     0,   238,     0,     0,   239,  -318,   240,
       0,   320,  -318,  -318,     0,   140,     0,     0,   241,     0,
       0,  -318,     0,  -318,  -318,   242,   243,     0,     0,  -318,
       0,     0,   184,   244,     0,     0,  -318,     0,     0,  -318,
       0,     0,     0,   245,   181,     0,   181,     0,     0,     0,
       0,   181,   246,   247,   181,   248,     0,   249,  -318,   250,
    -318,     0,   251,  -318,     0,  -318,   252,  -318,   253,  -318,
    -318,   254,  -318,     0,     0,  -318,     0,  -318,     0,     0,
       0,     0,     0,   181,   183,     0,     0,   183,     0,   181,
       0,     0,     0,   181,   181,     0,  -318,     0,  -318,     0,
       0,  -318,     0,     0,     0,     0,     0,   181,     0,     0,
       0,   181,     0,   181,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   271,   140,     0,
     140,     0,     0,     0,   181,     0,     0,     0,     0,     0,
       0,     0,     0,   320,   320,     0,     0,     0,     0,   320,
       0,  -318,   320,   320,   320,   320,     0,  -318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   181,   181,   181,   181,
     183,     0,     0,     0,   183,     0,     0,     0,     0,   320,
       0,   183,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   184,   140,     0,   184,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   320,     0,     0,     0,   230,     0,     0,     0,     0,
       0,     0,     0,     0,   320,   320,     0,     0,     0,     0,
     231,   232,     0,   233,   320,     0,     0,     0,   234,     0,
       0,     0,   183,     0,   183,     0,   235,   183,     0,     0,
       0,     0,   236,     0,     0,     0,     0,     0,   237,     0,
       0,     0,   238,     0,     0,   239,     0,   240,     0,     0,
       0,     0,   183,     0,     0,     0,   241,     0,     0,     0,
       0,   184,   183,   242,   243,   184,     0,     0,     0,     0,
       0,   244,   184,     0,     0,     0,     0,     0,     0,     0,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
     246,   247,     0,   248,     0,   249,     0,   250,     0,     0,
     251,     0,     0,     0,   252,     0,   253,   183,     0,   254,
       0,     0,     0,   183,     0,     0,     0,     0,     7,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   360,     0,     0,   140,
      23,    24,   140,   184,     0,   184,     0,     0,   184,   361,
       0,    30,   362,     0,   360,     0,     0,    33,    23,    24,
       0,   183,     0,   365,    38,   964,     0,   361,   320,    30,
     362,     0,     0,   184,     0,    33,     0,     0,     0,     0,
       0,     0,    38,   184,     0,   320,   363,   320,     0,     0,
       0,    55,     0,    57,     0,    59,     0,  1032,     0,     0,
    1033,     0,     0,   364,   363,    66,   183,   320,   183,    55,
       0,    57,     0,   183,     0,     0,   183,     0,     0,     0,
       0,   364,     0,    66,    82,     0,    84,     0,   184,    86,
       0,     0,     0,     0,   184,   140,     0,     0,     0,   140,
       0,     0,    82,     0,    84,   183,   140,    86,     0,     0,
       0,   183,     0,     0,     0,   183,   183,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   183,
       0,     0,     0,   183,     0,   183,     0,     0,     0,   100,
       0,     0,   184,   226,     0,  1034,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   183,     0,     0,     0,
       0,     0,     0,   320,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   140,     0,   140,
       0,     0,   140,     0,     0,     0,     0,   184,     0,   184,
       0,     0,     0,   332,   184,     0,     0,   184,   183,   183,
     183,   183,     0,     0,   320,     0,     0,   140,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   140,     0,     0,
       0,     0,     0,   359,     0,   210,   184,     0,     0,   220,
       0,     0,   184,     0,     0,     0,   184,   184,   228,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
     184,     0,     0,     0,   184,     0,   184,   278,     0,   281,
     282,     0,   140,     0,     0,   287,     0,   288,   140,   289,
       0,   320,     0,     0,   315,     0,     0,   184,   228,   323,
     325,   327,   328,   329,     0,     0,     0,   333,     0,   334,
       0,     0,     0,     0,     0,     0,   341,     0,     0,   323,
       0,   343,     0,   344,     0,     0,   345,   346,     0,   347,
       0,     0,     0,   228,   323,   357,   140,     0,     0,   184,
     184,   184,   184,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   486,     0,     0,     0,     0,     0,     0,   256,
       0,     0,   381,     0,   384,     0,     0,     0,     0,     0,
       0,   268,   269,   270,     0,     0,     0,     0,     0,     0,
       0,   140,     0,   140,     0,     0,     0,     0,   140,     0,
       0,   140,     0,     0,   320,   320,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   426,     0,   512,     0,     0,
       0,     0,   522,     0,     0,     0,     0,     0,     0,     0,
     140,     0,     0,     0,     0,     0,   140,     0,   478,     0,
     140,   140,   333,   228,     0,     0,     0,     0,     0,     0,
       0,   230,     0,     0,   140,     0,     0,     0,   140,     0,
     140,     0,     0,     0,     0,     0,   231,   232,     0,   233,
     323,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,   140,   235,     0,     0,     0,     0,     0,   236,     0,
     400,     0,     0,     0,   237,     0,     0,     0,   238,     0,
       0,   239,     0,   240,   419,     0,     0,   323,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,   242,
     243,     0,     0,   140,   140,   140,   140,   244,     0,     0,
       0,     0,   323,     0,   594,     0,     0,   245,     0,   479,
       0,     0,     0,   329,   333,     0,   246,   247,     0,   248,
       0,   249,     0,   250,     0,     0,   251,     0,   544,   545,
     252,     0,   253,     0,     0,   254,     0,     0,     0,     0,
       0,     0,   323,     0,     0,     0,   554,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   563,     0,     0,     0,     0,   498,
     499,     0,     0,     0,   569,     0,     0,     0,     0,     0,
       0,   514,     0,     0,     0,     0,   581,     0,   583,     0,
       0,   271,     0,     0,     0,     0,     0,   593,     0,     0,
       0,     0,   597,   602,   605,     0,     0,     0,     0,   228,
       0,     0,     0,     0,     0,     0,     0,   750,     0,   752,
       0,   541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   678,   679,
     680,   682,   684,   685,     0,   686,   687,   688,   689,   690,
       0,   698,   699,   700,   701,   702,   703,   704,   705,   706,
     707,   708,   709,   710,   711,   712,   713,   714,     0,     0,
       0,   256,     0,     0,   323,   323,     0,     0,     0,     0,
     323,     0,     0,   323,   323,   323,   323,   724,   726,     0,
       0,   276,   602,   730,     0,     0,   741,     0,     0,     0,
       0,     0,   744,     0,     0,     0,     0,   606,     0,     0,
       0,   611,     0,   753,     0,   276,   755,     0,     0,     0,
     323,     0,     0,     0,     0,   338,   340,     0,   666,   766,
       0,   769,     0,     0,     0,   281,     0,     0,   774,   775,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,   315,     0,     0,   370,     0,     0,     0,
       0,     0,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   744,     0,     0,   323,   323,     0,     0,     0,
       7,   228,   802,     0,     0,   323,     0,     0,     0,     0,
       0,     0,     0,   731,     0,     0,   832,   832,   360,     0,
       0,     0,    23,    24,     0,     0,     0,     0,     0,     0,
     846,   361,     0,    30,   362,     0,     0,     0,     0,    33,
     759,     0,     0,   514,     0,     0,    38,     0,     0,     0,
       0,     0,   315,     0,     0,     0,   772,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   778,     0,   363,     0,
     514,     0,     0,    55,     0,    57,     0,    59,     0,  1032,
       0,     0,  1033,     0,     0,   364,   488,    66,   930,     0,
       0,   933,     0,     0,     0,     0,   882,   429,     0,     0,
       0,     0,     0,   887,     0,     0,    82,     0,    84,     0,
       0,    86,     0,     0,   819,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   842,   501,   504,     0,     0,
       0,     0,     0,     0,   511,     0,     0,     0,     0,     0,
     611,     0,     0,   276,     0,   276,     0,     0,     0,     0,
       0,     0,   276,     0,     0,     0,   430,     0,   753,   323,
     431,   100,     0,     0,     0,     0,     0,  1104,     0,     0,
       0,     0,     0,     0,     0,     0,   323,     0,   323,   927,
     928,     0,     0,     0,  1013,     0,     0,     0,  1018,     0,
     432,     0,   276,     0,     0,   522,     0,     0,   323,     0,
       0,     0,     0,     0,   419,     0,     0,   433,   434,     0,
     435,   436,   437,   315,   438,   439,   440,   957,   441,   442,
     443,   444,     0,   446,   447,   448,   449,   450,   451,   452,
       0,     0,   453,   454,   455,     0,     0,     0,     0,     0,
       0,   581,     0,   456,     0,   995,     0,   996,   832,   832,
       0,   343,     0,   344,     0,     0,   346,   347,     0,     0,
     429,     0,     0,     0,     0,     0,  1064,     0,  1065,     0,
    1008,  1067,  1010,     0,     0,     0,   228,     0,     0,     0,
    1016,   744,  1019,     0,  1021,  1023,   936,     0,     0,  1026,
    1028,     0,     0,   514,   323,     0,  1083,     0,     0,  1038,
       0,     0,     0,   945,     0,     0,  1018,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,   966,   431,     0,     0,     0,     0,     0,     0,
       0,  1055,  1019,  1026,  1058,   323,     0,     0,     0,   741,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1112,     0,   682,   724,     0,     0,  1117,     0,     0,
       0,     0,     0,  1066,     0,     0,     0,   419,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   315,   442,   443,   444,  1082,   446,   447,   448,   449,
       0,   228,   452,     0,     0,   453,   454,   455,     0,  1090,
       0,  1092,   323,     0,     0,  1153,   456,     0,     0,     0,
       0,     0,     0,     0,   666,     0,     0,     0,     0,   276,
       0,     0,     0,   832,     0,     0,     0,     0,     0,     0,
    1101,     0,  1102,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1110,     0,     0,     0,     0,     0,     0,     0,
    1174,     0,  1178,     0,     0,     0,  1119,   522,     0,     0,
     522,   276,   276,     0,     0,     0,     0,     0,   759,   276,
     276,     0,     0,     0,     0,  1140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1147,     0,  1213,
       0,     0,     0,     0,   966,  1216,     0,     0,     0,  1174,
    1178,     0,     0,     0,     0,   323,   323,     0,     0,     0,
       0,     0,     0,  1222,     0,     0,     0,  1224,  1161,   752,
       0,     0,     0,     0,     0,     0,  1165,   832,   832,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1175,     0,  1179,     0,     0,  1181,
       0,  1184,     0,     0,  1187,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   611,
       0,     0,     0,     0,     0,     0,  1175,  1179,     0,  1184,
    1187,     0,   752,   752,   752,   752,     0,   427,     0,     0,
       0,     0,     0,     0,   428,     0,     0,     0,     0,  1220,
       0,     0,     0,     0,  1221,     0,   429,   275,  1223,     0,
       0,     0,     0,  1225,  1226,     0,     0,  1227,     0,     0,
     935,     0,   970,     0,     0,  1229,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1239,
    1240,  1225,  1241,  1242,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   965,     0,     0,
       0,   611,     0,     0,     0,   430,     0,   276,     0,   431,
       0,     0,     0,     0,     0,     0,  1253,  1254,  1255,  1256,
    1253,  1254,  1255,  1256,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   214,     0,     0,     0,   432,
       0,     0,     0,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   434,     0,   435,
     436,   437,   276,   438,   439,   440,   276,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
       0,   453,   454,   455,     0,   365,     0,     0,     0,     0,
       0,     0,   456,     0,     0,     0,    -2,     4,     0,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,    15,    16,    17,    18,     0,    19,     0,    20,
      21,  -211,    22,    23,    24,    25,    26,    27,     0,  -211,
      28,     0,    29,     0,    30,    31,    32,  -211,  -211,  -211,
      33,    34,    35,   -68,  1072,    36,    37,    38,     0,    39,
    -338,    40,     0,    41,    42,    43,    44,    45,     0,    46,
      47,    48,    49,   -68,    50,    51,     0,    52,    53,    54,
       0,  -338,  1088,     0,    55,    56,    57,    58,    59,    60,
      61,  -338,   -68,    62,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,    77,    78,     0,    79,    80,    81,    82,    83,    84,
      85,   -68,    86,    87,     0,     0,  -211,     0,    88,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   276,   276,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,   230,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,     0,   103,
       0,  -211,   231,   232,   104,   233,   429,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,   235,     0,
     276,     0,     0,     0,   236,     0,   276,   276,     0,     0,
     237,     0,     0,     0,   238,     0,     0,   239,     0,   240,
       0,     0,   276,   276,     0,   276,     0,     0,   241,     0,
       0,     0,     0,     0,   504,   242,   243,   276,     0,     0,
     276,     0,     0,   244,     0,   430,     0,     0,     0,   431,
       0,     0,     0,   245,     0,     0,     0,     0,     0,     0,
       0,     0,   246,   247,     0,   248,     0,   249,     0,   250,
       0,     0,   251,     0,     0,  1217,   252,     0,   253,   432,
       0,   254,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,     0,   446,   447,   448,   449,   450,     0,   452,     0,
       0,   453,   454,   455,     0,     0,     0,     0,     0,     0,
       0,     0,   456,     0,     0,     0,   504,   513,   504,     0,
       0,   504,   504,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,  -679,     0,    14,    15,    16,
      17,    18,  -679,    19,     0,    20,    21,  -211,    22,    23,
      24,    25,    26,    27,  -679,  -211,    28,  -679,    29,     0,
      30,    31,    32,  -211,  -211,  -211,    33,    34,    35,   -68,
       0,    36,    37,    38,     0,    39,  -338,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -338,     0,     0,
      55,    56,    57,    58,     0,    60,    61,  -338,   -68,    62,
      63,    64,    65,  -679,    66,    67,    68,  -679,    69,    70,
      71,    72,    73,    74,    75,    76,     0,    77,    78,     0,
      79,    80,    81,    82,    83,    84,    85,   -68,    86,    87,
       0,     0,  -211,     0,    88,     0,     0,  -679,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,  -679,    92,  -679,  -679,  -679,
    -679,  -679,  -679,  -679,     0,  -679,  -679,  -679,  -679,  -679,
    -679,  -679,  -679,  -679,  -679,  -679,  -679,    99,   100,  -679,
    -679,  -679,     0,   102,  -679,   103,     0,  -211,     0,   348,
    -679,     5,     6,     7,     8,     9,    10,    11,    12,    13,
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
     349,   103,     0,  -211,     0,     4,   104,     5,     6,     7,
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
     100,     0,     0,   101,     0,   102,   559,   103,     0,  -211,
       0,   576,   104,     5,     6,     7,     8,     9,    10,    11,
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
       0,   102,   577,   103,     0,  -211,     0,     4,   104,     5,
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
      98,    99,   100,     0,     0,   101,     0,   102,   809,   103,
       0,  -211,     0,     4,   104,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,    16,
      17,    18,     0,    19,     0,    20,    21,  -211,    22,    23,
      24,    25,    26,    27,     0,  -211,    28,     0,    29,     0,
      30,    31,    32,  -211,  -211,  -211,    33,    34,    35,   -68,
       0,    36,    37,    38,     0,    39,  -338,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -338,     0,     0,
      55,    56,    57,    58,   358,    60,    61,  -338,   -68,    62,
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
       0,   103,     0,  -211,     0,     4,   104,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
      15,    16,    17,    18,     0,    19,     0,    20,    21,  -211,
      22,    23,    24,    25,    26,    27,     0,  -211,    28,     0,
      29,     0,    30,    31,    32,  -211,  -211,  -211,    33,    34,
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
      32,  -211,  -211,  -211,    33,  1025,    35,   -68,     0,    36,
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
      33,  1173,    35,   -68,     0,    36,    37,    38,     0,    39,
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
      30,    31,    32,  -211,  -211,  -211,    33,  1177,    35,   -68,
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
    -211,  -211,    33,  1183,    35,   -68,     0,    36,    37,    38,
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
      29,     0,    30,    31,    32,  -211,  -211,  -211,    33,  1186,
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
     100,     0,     0,   101,     0,   102,     0,   103,   732,  -211,
       5,     6,   104,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,   733,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,  -211,  -211,
    -211,     0,   197,    35,     0,   734,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,   735,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     201,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,    85,     0,   736,    87,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,   737,  -211,     0,   874,   104,     5,     6,     0,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   195,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,   295,     0,   196,
       0,     0,     0,    32,  -211,  -211,  -211,     0,   197,    35,
       0,     0,     0,   297,     0,     0,    39,     0,   198,     0,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   200,     0,     0,     0,     0,     0,     0,
       0,    71,   302,    73,    74,    75,   303,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,     0,  -211,     5,
       6,   104,     8,     9,    10,    11,    12,    13,     0,   627,
       0,    14,     0,     0,    17,    18,   629,    19,     0,   195,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   196,     0,     0,     0,    32,     0,     0,     0,
       0,   197,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   198,   635,   199,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   200,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   201,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    89,   503,   458,   459,   460,   461,   462,     0,     0,
     465,   466,   467,   468,     0,   470,   471,     0,   820,   821,
     822,   823,   824,   648,     0,   649,     0,    95,     0,   650,
     651,   652,   653,   654,   655,   656,   825,   658,   659,    97,
     826,    99,   -90,   661,   662,   827,   664,   202,     0,   103,
       0,     0,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,   627,     0,    14,     0,     0,    17,    18,   629,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,   635,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   201,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    89,   503,   458,   459,   460,   461,
     462,     0,     0,   465,   466,   467,   468,     0,   470,   471,
       0,   820,   821,   822,   823,   824,   648,     0,   649,     0,
      95,     0,   650,   651,   652,   653,   654,   655,   656,   825,
     658,   659,    97,   826,    99,     0,   661,   662,   827,   664,
     202,     0,   103,     0,     0,     0,     0,   104,     5,     6,
       7,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,    15,     0,    17,    18,     0,    19,     0,   195,    21,
       0,    22,     0,     0,     0,     0,    27,     0,     0,    28,
       0,   196,     0,     0,     0,    32,     0,     0,     0,     0,
      34,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     198,     0,   199,     0,     0,    44,    45,     0,    46,    47,
      48,    49,     0,    50,    51,     0,    52,    53,   335,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   200,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,    74,    75,    76,     0,
      77,    78,     0,    79,     0,     0,     0,    83,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   102,     0,   103,     0,
       0,     5,     6,   104,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   195,     0,     0,    22,   379,    24,     0,     0,     0,
       0,     0,    28,     0,   196,     0,     0,     0,    32,     0,
       0,     0,     0,   197,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   198,     0,   199,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,   363,     0,     0,     0,     0,     0,    56,    57,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   200,     0,
      66,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   201,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,    85,     0,    86,    87,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,     0,     5,     6,   104,     8,     9,    10,
     382,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,    20,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,  -211,  -211,  -211,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,    65,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   201,     0,    77,     0,     0,    79,
       0,     0,     0,    83,    84,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   202,     0,   103,     0,  -211,     5,     6,   104,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   195,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     196,     0,     0,     0,    32,     0,     0,     0,     0,   197,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   198,
       0,   199,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   200,  -723,     0,     0,     0,  -723,
       0,     0,    71,    72,    73,    74,    75,   201,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,  -380,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
    -380,     0,     0,   101,     0,   202,  -380,   103,     0,     0,
       5,     6,   104,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,  -211,  -211,
    -211,     0,   197,    35,     0,     0,     0,    37,     0,     0,
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
     103,     0,  -211,     5,     6,   104,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   195,     0,     0,    22,     0,   733,     0,
       0,     0,     0,     0,    28,     0,   196,     0,     0,     0,
      32,     0,     0,     0,     0,   197,    35,     0,   734,     0,
      37,     0,     0,    39,     0,   198,     0,   199,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     200,     0,   735,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   201,     0,    77,     0,     0,    79,     0,
       0,     0,    83,     0,    85,     0,   736,    87,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
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
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,   319,
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
      92,    93,    94,     0,     0,     0,     0,    95,   353,     0,
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
     202,     0,   103,  -530,     0,     5,     6,   104,     8,     9,
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
      79,   317,   318,     0,    83,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   202,     0,   103,     0,     0,  -530,     0,
     104,     5,     6,     0,     8,     9,    10,    11,    12,    13,
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
       0,   103,   796,     0,     5,     6,   104,     8,     9,    10,
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
     101,     0,   202,     0,   103,   799,     0,     5,     6,   104,
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
       0,     0,     0,   101,     0,   202,     0,   103,   804,     0,
       5,     6,   104,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,   197,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,   943,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     201,     0,    77,     0,     0,    79,   317,   318,     0,    83,
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
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,  -388,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,  -388,     0,     0,   101,
       0,   202,  -388,   103,     0,     0,     5,     6,   104,     8,
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
       0,  -373,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,  -373,
       0,     0,   101,     0,   202,  -373,   103,     0,     0,     5,
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
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,  -381,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,  -381,     0,     0,   101,     0,   202,  -381,   103,
       0,     0,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,   259,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   201,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,    85,     0,   260,    87,     0,     0,     0,
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
      79,   317,   318,     0,    83,     0,    85,     0,     0,    87,
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
       0,     0,     0,    63,    64,   200,  -723,     0,     0,     0,
    -723,     0,     0,    71,    72,    73,    74,    75,   201,     0,
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
       0,     0,     0,   197,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   198,     0,   199,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   200,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   201,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,   598,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,     0,   599,     0,   104,     5,     6,     0,
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
      94,     0,  -409,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,  -409,     0,
       5,     6,   104,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
     275,    28,     0,   196,     0,     0,     0,    32,     0,     0,
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
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   102,     0,
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
       0,    79,     0,     0,     0,    83,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,     0,     0,   599,
       0,   104,     5,     6,     0,     8,     9,    10,    11,    12,
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
     202,   794,   103,     0,     0,     5,     6,   104,     8,     9,
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
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   202,     0,   103,     0,     0,  1017,     0,
     104,     5,     6,     0,     8,     9,    10,    11,    12,    13,
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
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,     0,  1022,     0,   104,     5,     6,     0,
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
       5,     6,   104,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,   295,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,   197,    35,     0,     0,     0,   297,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,   302,    73,    74,    75,
     303,     0,    77,     0,     0,    79,     0,     0,     0,    83,
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
      32,     0,     0,     0,     0,  1056,    35,     0,     0,     0,
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
       0,     0,     0,    32,     0,     0,     0,     0,  1208,    35,
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
       0,  1209,    35,     0,     0,     0,    37,     0,     0,    39,
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
       0,     0,     0,     0,  1211,    35,     0,     0,     0,    37,
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
     202,     0,   103,     0,     0,     5,     6,   104,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   195,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   196,     0,
       0,     0,    32,     0,     0,     0,     0,  1212,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   198,     0,   199,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,   429,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   200,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   201,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,   431,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   202,     0,   103,     0,     0,     0,     6,
     104,     8,     9,    10,    11,    12,    13,     0,   433,   434,
      14,   435,   436,   437,    18,   438,   439,   440,   195,   441,
     442,   443,   444,   294,   446,   447,   448,   449,   450,     0,
     452,   196,     0,   453,   454,   455,  -211,  -211,  -211,     0,
     197,     0,     0,   296,   456,     0,     0,   298,     0,     0,
       0,     0,   199,     0,     0,    44,    45,     0,     0,     0,
       0,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,     0,   299,     0,    56,   300,    58,    59,    60,     0,
       0,     0,     0,    63,     0,   200,     0,   301,     0,     0,
       0,     0,     0,    71,     0,    73,     0,    75,     0,     0,
       0,     0,     0,     0,     0,     0,   304,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,  -342,
    -211,     0,     6,   104,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,     0,    18,     0,     0,
       0,   195,     0,     0,     0,     0,   294,     0,     0,     0,
       0,     0,     0,     0,   196,     0,     0,     0,     0,  -211,
    -211,  -211,     0,   197,     0,     0,   296,     0,     0,     0,
     298,     0,     0,     0,     0,   199,     0,     0,    44,    45,
       0,     0,     0,     0,    49,     0,    50,    51,     0,     0,
       0,     0,     0,     0,     0,   299,     0,    56,   300,    58,
      59,    60,     0,     0,     0,     0,    63,     0,   200,     0,
     301,     0,     0,     0,     0,     0,    71,     0,    73,     0,
      75,     0,     0,     0,     0,     0,     0,     0,     0,   304,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   625,     0,   626,     0,     0,
      90,    91,    92,    93,    94,     0,     0,   627,     0,    95,
       0,     0,   628,   232,   629,   630,     0,     0,    96,     0,
     631,    97,    98,    99,     0,     0,     0,   101,   235,   202,
     196,   103,     0,  -211,   236,     0,   104,     0,     0,     0,
     632,     0,     0,     0,   238,     0,     0,   633,     0,   634,
     635,     0,     0,     0,     0,     0,     0,     0,   636,     0,
       0,     0,     0,     0,     0,   242,   637,     0,     0,     0,
       0,     0,     0,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   638,     0,     0,     0,     0,     0,     0,
       0,     0,   246,   247,     0,   639,     0,   249,     0,   640,
       0,     0,   251,     0,     0,     0,   641,     0,   253,     0,
       0,   642,     0,     0,     0,     0,     0,     0,     0,     0,
     503,   458,   459,   460,   461,   462,     0,     0,   465,   466,
     467,   468,     0,   470,   471,     0,   643,   644,   645,   646,
     647,   648,     0,   649,     0,     0,     0,   650,   651,   652,
     653,   654,   655,   656,   657,   658,   659,     0,   660,     0,
       0,   661,   662,   663,   664,     5,     6,   665,     8,     9,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,     0,    98,    99,     0,     0,
       0,   101,     0,   202,     0,   103,     0,  -211,     5,     6,
       0,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   195,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   196,     0,     0,     0,    32,  -211,  -211,  -211,     0,
     197,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     198,     0,   199,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   200,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   201,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,    85,
       0,     0,    87,     0,     0,     5,     6,     0,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   195,     0,    90,    22,    92,
       0,     0,     0,     0,     0,     0,    28,     0,   196,     0,
       0,     0,    32,  -211,  -211,  -211,     0,   197,    35,     0,
      99,     0,    37,     0,     0,    39,   202,   198,   103,   199,
    -211,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   200,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   201,     0,    77,     0,     0,
      79,   230,     0,     0,    83,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,   231,   232,     0,   233,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,     0,    90,     0,    92,     0,   236,     0,
       0,     0,     0,     0,   237,     0,     0,     0,   238,     0,
       0,   239,     0,   240,     0,     0,     0,    99,     0,     0,
       0,     0,   241,     0,     0,   103,     0,  -211,     0,   242,
     243,     0,     0,     0,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,   246,   247,     0,   248,
       0,   249,     0,   250,   691,     0,   251,     0,     0,     0,
     252,     0,   253,     0,   627,   254,     0,     0,     0,   975,
     976,   629,   977,     0,     0,     0,     0,   978,     0,     0,
       0,     0,     0,     0,     0,   693,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   979,     0,     0,
       0,   980,     0,     0,   981,     0,   198,   635,     0,     0,
       0,     0,     0,     0,     0,   694,     0,     0,     0,     0,
       0,   540,     0,   982,     0,     0,     0,     0,     0,     0,
     983,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     984,     0,     0,     0,     0,     0,     0,     0,     0,   985,
     986,     0,   987,     0,   988,     0,   695,     0,     0,     0,
       0,     0,   696,   989,     0,   990,     0,     0,   991,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   458,   459,
     460,   461,   462,     0,     0,   465,   466,   467,   468,     0,
     470,   471,     0,   643,   644,   645,   646,   647,   648,     0,
     649,     0,     0,     0,   650,   651,   652,   653,   654,   655,
     656,   657,   658,   659,   230,   660,     0,     0,   661,   662,
     663,   664,   992,     0,   627,     0,     0,     0,     0,   231,
     232,   629,   233,     0,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,   235,     0,     0,     0,     0,
       0,   236,     0,     0,     0,     0,     0,   237,     0,     0,
       0,   238,     0,     0,   239,     0,   240,   635,     0,     0,
       0,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,   242,   243,     0,     0,     0,     0,     0,     0,
     244,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,     0,     0,     0,     0,     0,     0,     0,     0,   246,
     247,     0,   248,     0,   249,     0,   250,     0,     0,   251,
       0,     0,     0,   252,     0,   253,     0,     0,   254,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   458,   459,
     460,   461,   462,     0,     0,   465,   466,   467,   468,     0,
     470,   471,     0,   643,   644,   645,   646,   647,   648,     0,
     649,     0,     0,     0,   650,   651,   652,   653,   654,   655,
     656,   657,   658,   659,     7,   660,     0,     0,   661,   662,
     663,   664,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   360,     0,     0,     0,    23,    24,     0,     0,
       0,     0,     0,     0,     0,   361,     0,    30,   362,     0,
     427,     0,     0,    33,     0,     0,     0,   428,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   363,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,  1032,     0,     0,  1033,     0,     0,   364,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,    84,     0,     0,    86,     0,     0,   430,   427,
       0,     0,   431,     0,     0,     0,   428,     0,     0,     0,
       0,   502,     0,     0,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,   503,   458,   459,   460,   461,   462,     0,
       0,   465,   466,   467,   468,   100,   470,   471,     0,   433,
     434,  1169,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,     0,   453,   454,   455,   430,   365,     0,
     427,   431,     0,     0,     0,   456,     0,   428,     0,     0,
    1182,     0,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,   503,   458,   459,   460,   461,   462,     0,     0,
     465,   466,   467,   468,     0,   470,   471,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,     0,     0,   453,   454,   455,     0,   365,   430,   427,
       0,     0,   431,     0,   456,     0,   428,     0,     0,     0,
       0,  1245,     0,     0,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,   503,   458,   459,   460,   461,   462,     0,
       0,   465,   466,   467,   468,     0,   470,   471,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,     0,   453,   454,   455,   430,   365,     0,
     427,   431,     0,     0,     0,   456,     0,   428,     0,     0,
    1246,     0,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,   503,   458,   459,   460,   461,   462,     0,     0,
     465,   466,   467,   468,     0,   470,   471,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,     0,     0,   453,   454,   455,     0,   365,   430,   427,
       0,     0,   431,     0,   456,     0,   428,     0,     0,     0,
       0,  1247,     0,     0,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,   503,   458,   459,   460,   461,   462,     0,
       0,   465,   466,   467,   468,     0,   470,   471,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,     0,   453,   454,   455,   430,   365,     0,
       0,   431,     0,     0,     0,   456,     0,     0,     0,     0,
    1248,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,     0,   428,     0,     0,
       0,   432,   503,   458,   459,   460,   461,   462,     0,   429,
     465,   466,   467,   468,   585,   470,   471,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,     0,     0,   453,   454,   455,     0,   365,     0,     0,
       0,   586,     0,     0,   456,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,   427,   816,     0,     0,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,     0,   432,     0,     0,   817,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   818,   587,   453,   454,   455,     0,     0,     0,
       0,     0,     0,     0,     0,   456,     0,     0,     0,   430,
     427,     0,     0,   431,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
    1123,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,     0,   453,   454,   455,   430,     0,
       0,   427,   431,     0,     0,     0,   456,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,   275,     0,     0,     0,     0,     0,     0,   214,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,     0,   453,   454,   455,     0,   365,   430,
     427,     0,     0,   431,     0,   456,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
     275,     0,     0,     0,     0,     0,     0,     0,     0,   214,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,     0,   453,   454,   455,   430,   365,
       0,   427,   431,     0,     0,     0,   456,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,   839,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,     0,   453,   454,   455,     0,   365,   430,
     427,     0,     0,   431,     0,   456,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
     902,     0,     0,     0,     0,     0,     0,     0,     0,   214,
       0,     0,     0,   432,     0,   903,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,     0,   453,   454,   455,   430,     0,
     427,     0,   431,     0,     0,     0,   456,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,     0,   453,   454,   455,     0,   430,     0,
       0,     0,   431,     0,     0,   456,     0,     0,     0,     0,
       0,   934,   427,     0,     0,     0,     0,     0,     0,   428,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   429,   432,     0,     0,     0,  1096,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,  1097,   453,   454,   455,     0,   365,     0,
       0,     0,     0,     0,     0,   456,     0,     0,     0,     0,
     430,   427,     0,     0,   431,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,     0,     0,   453,   454,   455,   430,
       0,   427,     0,   431,     0,     0,     0,   456,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,   214,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,   427,   526,     0,     0,     0,     0,     0,   428,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
     429,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,     0,   453,   454,   455,     0,   430,
       0,   427,     0,   431,  1115,     0,   456,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,   214,
       0,     0,     0,   432,     0,     0,     0,     0,     0,   430,
       0,   427,     0,   431,     0,     0,     0,     0,   428,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
     429,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   432,     0,   453,   454,   455,     0,   430,
       0,     0,     0,   431,  1163,     0,   456,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   432,     0,   453,   454,   455,     0,   430,
       0,   427,     0,   431,     0,     0,   456,     0,   428,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
     429,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   432,     0,   453,   454,   455,     0,   549,
       0,   427,     0,     0,     0,     0,   456,     0,   428,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
     429,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,   553,   453,   454,   455,     0,   430,
       0,   427,   812,   431,     0,     0,   456,     0,   428,     0,
       0,     0,   727,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,   430,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   432,     0,   453,   454,   455,     0,   430,
       0,   427,     0,   431,     0,     0,   456,     0,   428,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
     429,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   432,     0,   453,   454,   455,     0,     0,
       0,     0,   795,     0,     0,     0,   456,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   813,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,     0,   453,   454,   455,     0,   430,
     427,     0,     0,   431,     0,     0,   456,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,   932,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   853,   432,     0,     0,     0,     0,     0,     0,
       0,   427,     0,     0,     0,     0,     0,     0,   428,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
     429,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,     0,   453,   454,   455,   430,     0,
       0,   427,   431,     0,     0,     0,   456,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,   431,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,   432,   453,   454,   455,     0,     0,   430,
       0,   427,  1001,   431,     0,   456,     0,     0,   428,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
     429,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   432,     0,   453,   454,   455,     0,     0,
       0,   427,   946,     0,     0,     0,   456,     0,   428,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
     429,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,     0,   453,   454,   455,     0,   430,
       0,     0,   947,   431,     0,     0,   456,   427,  1095,     0,
       0,     0,     0,     0,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,   430,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   432,     0,   453,   454,   455,     0,     0,
       0,     0,     0,     0,     0,   430,   456,     0,     0,   431,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   427,     0,   453,   454,   455,     0,   432,
     428,     0,  1048,     0,     0,     0,   456,     0,     0,     0,
       0,     0,   429,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   427,
       0,   453,   454,   455,     0,     0,   428,     0,     0,     0,
       0,     0,   456,     0,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   427,     0,     0,     0,     0,
       0,     0,   428,     0,     0,     0,     0,     0,     0,     0,
       0,   214,     0,     0,   429,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,   431,   433,   434,     0,   435,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,     0,     0,   453,   454,   455,
       0,   432,     0,     0,     0,     0,     0,     0,   456,     0,
       0,     0,     0,   430,     0,     0,     0,   431,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   427,  1109,   453,   454,   455,  1118,   432,   428,     0,
       0,     0,     0,     0,   456,     0,     0,     0,     0,     0,
     429,     0,     0,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   427,     0,   453,
     454,   455,     0,     0,   428,     0,     0,     0,     0,     0,
     456,     0,     0,     0,     0,     0,   429,  1149,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,   431,     0,     0,     0,   427,  1168,     0,
       0,     0,     0,     0,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,   431,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,     0,   453,   454,   455,     0,   432,
       0,     0,     0,     0,  1146,   430,   456,   427,     0,   431,
       0,     0,     0,     0,   428,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,     0,     0,   427,     0,     0,
       0,     0,   456,     0,   428,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
       0,   453,   454,   455,     0,   430,     0,   427,     0,   431,
       0,     0,   456,     0,   428,     0,  1202,     0,  1210,     0,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,   430,     0,   427,     0,   431,
       0,     0,     0,     0,   428,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,   430,     0,   427,     0,   431,
       0,     0,   456,     0,   428,     0,   433,   434,  1249,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,   430,     0,   427,     0,   431,
       0,     0,   456,     0,   428,     0,   433,   434,  1250,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,   430,     0,   427,     0,   431,
       0,     0,   456,     0,   428,     0,   433,   434,  1251,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,   430,     0,   427,     0,   431,
       0,     0,   456,     0,   428,     0,   433,   434,  1252,   435,
     436,   437,     0,   438,   439,   440,   429,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,   430,     0,     0,     0,   431,
       0,     0,   456,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,   922,     0,     0,     0,   431,
       0,     0,   456,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   432,
       0,   453,   454,   455,     0,     0,     0,     0,     0,     0,
       0,     0,   456,     0,     0,     0,   433,   434,     0,   435,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,     0,
     360,   453,   454,   455,    23,    24,     0,     0,     0,     0,
       0,     0,   456,   361,     0,    30,   362,     0,     0,     0,
       0,    33,     0,     0,     0,     0,     0,     0,    38,     0,
       0,  -338,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   -68,     0,     0,     0,     0,     0,
     363,     0,  -338,     0,     0,    55,   609,    57,   230,     0,
       7,    61,  -338,   -68,    62,     0,     0,   364,     0,    66,
       0,     0,     0,   231,   232,     0,   233,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,     0,    82,   235,
      84,     0,     0,    86,     0,   236,     0,     0,     0,     0,
       0,   237,     0,     0,     0,   238,     0,     0,   239,     0,
     240,     0,     0,     0,   230,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,     0,   231,
     232,     0,   233,     0,   244,     0,     0,   234,     0,    24,
       0,     0,     0,   100,   245,   235,     0,     0,   365,     0,
       0,   236,     0,   246,   247,     0,   248,   237,   249,     0,
     250,   238,     0,   251,   239,     0,   240,   252,     0,   253,
       0,     0,   254,     0,     0,   241,     0,     0,     0,     0,
       0,     0,   242,   243,     0,     0,     0,     0,     0,     0,
     244,    57,     0,     0,     0,     0,     0,     0,     0,     0,
     245,     0,     0,    66,     0,     0,   230,     0,     7,   246,
     247,     0,   248,     0,   249,     0,   250,     0,     0,   251,
       0,   231,   232,   252,   233,   253,     0,    86,   254,   234,
       0,     0,     0,     0,     0,     0,     0,   235,     0,     0,
       0,     0,     0,   236,     0,     0,     0,     0,     0,   237,
       0,     0,     0,   238,     0,     0,   239,     0,   240,     0,
       0,     0,  -313,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,   242,   243,     0,  -313,  -313,     0,
    -313,     0,   244,     0,     0,  -313,     0,     0,     0,     0,
       0,     0,   245,  -313,     0,     0,     0,     0,     0,  -313,
       0,   246,   247,     0,   248,  -313,   249,     0,   250,  -313,
       0,   251,  -313,     0,  -313,   252,     0,   253,   230,     0,
     254,     0,     0,  -313,     0,     0,     0,     0,     0,     0,
    -313,  -313,     0,   231,   232,     0,   233,     0,  -313,     0,
       0,   234,     0,     0,     0,     0,     0,     0,  -313,   235,
       0,     0,     0,     0,     0,   236,     0,  -313,  -313,     0,
    -313,   237,  -313,     0,  -313,   238,     0,  -313,   239,     0,
     240,  -313,     0,  -313,  -314,     0,  -313,     0,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,     0,  -314,
    -314,     0,  -314,     0,   244,     0,     0,  -314,     0,     0,
       0,     0,     0,     0,   245,  -314,     0,     0,     0,     0,
       0,  -314,     0,   246,   247,     0,   248,  -314,   249,     0,
     250,  -314,     0,   251,  -314,     0,  -314,   252,     0,   253,
       0,     0,   254,     0,     0,  -314,     0,     0,     0,     0,
       0,     0,  -314,  -314,     0,     0,     0,     0,     0,     0,
    -314,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -314,     0,     0,     0,     0,     0,     0,     0,     0,  -314,
    -314,     0,  -314,     0,  -314,     0,  -314,     0,     0,  -314,
       0,     0,     0,  -314,     0,  -314,     0,     0,  -314
};

static const yytype_int16 yycheck[] =
{
       2,   112,     2,   266,   401,    21,   165,   181,   547,    27,
     610,    63,     2,   585,    63,    63,   418,   485,    68,   668,
     574,   670,   223,   621,    26,   618,   784,   818,   291,   600,
     601,   865,   509,   604,   605,   133,    26,   542,    88,     1,
     813,   383,   778,     1,    33,   203,     2,   836,     1,    67,
       1,    33,    48,   103,    33,     5,   949,    48,     2,    36,
      57,    33,    57,   873,   518,     0,   229,     3,   331,    33,
      26,   403,    47,   119,    76,   148,   104,    48,    80,    81,
      33,     3,    26,   121,   124,   166,    76,   148,   262,   393,
      80,    81,     1,     3,    33,    41,    42,    43,   121,   166,
     102,   174,   148,   266,   106,    25,   173,    48,    48,    62,
      57,    86,   102,    33,    76,   176,   106,    62,    33,    55,
      76,    41,    42,    43,    80,    81,   430,   431,   291,     1,
     121,   171,    76,    55,   111,   121,    80,    81,   176,    48,
     115,   951,   121,   171,    76,    55,   102,    25,   111,   121,
     106,    92,   148,   176,   107,    33,   118,   148,   102,   148,
      57,    33,   106,    41,    42,    43,   119,   621,   331,   105,
    1004,   764,   476,   477,   119,   126,   173,   148,   173,   121,
     176,   121,   171,   105,   942,   176,   118,    25,  1081,   171,
      62,   949,   171,   747,   147,    33,   147,   148,   148,   171,
     120,  1035,  1036,    41,    42,    43,   538,   171,   148,   367,
     172,   474,   475,   215,  1003,   173,   952,   126,   171,    23,
      48,   513,   172,   174,   148,   215,   999,   166,   126,   175,
     801,   802,   171,   148,   176,   107,   176,   148,   147,   148,
     172,    19,   120,     3,   418,   817,   588,   119,   540,    57,
     174,   728,     3,    28,   152,   175,   171,   166,    54,   215,
     173,   148,   175,   174,    92,   148,   148,   571,   270,  1103,
      21,   215,   148,   275,   148,   147,   439,   782,   148,    75,
     270,   148,   120,    34,   382,   275,    90,   174,   551,    85,
     148,   174,   174,  1127,   166,    55,   148,   175,   174,   171,
     174,    79,   172,  1061,    55,    80,  1097,   174,   166,   113,
     142,   474,   475,    64,   270,   148,    46,    92,   150,   275,
      98,   148,   174,  1081,   148,   526,   270,   148,   921,   530,
     489,   275,   148,   148,    94,    57,   114,   175,   126,   172,
      70,   116,    28,   148,    47,   105,   509,   174,   172,   351,
     402,   351,   950,   174,   105,     2,   564,   565,   174,   174,
     111,   351,    48,   365,    67,   153,    52,   155,   156,   174,
     158,   159,   374,    91,   392,   365,   166,    95,    92,    26,
     398,   148,   172,    86,   374,   387,   863,    32,   551,    28,
     591,    77,   148,   173,    80,   351,    82,   387,   166,   166,
    1049,  1050,    28,   173,   172,   175,    92,   351,   148,   365,
     166,   574,   115,   463,   464,    12,   418,   971,   374,   469,
     992,   365,   472,   473,   148,   111,   166,   904,    47,    76,
     374,   387,    91,    80,    81,   173,    95,   175,    77,  1039,
     908,    80,   166,   387,  1015,  1016,    91,   148,    67,    57,
      95,   148,   148,    92,    80,   102,  1027,  1028,    91,   106,
     148,     3,    95,   481,    57,   166,    92,    86,   865,   166,
     166,   148,   111,   173,   148,   148,    18,   148,   166,    21,
      84,    57,   884,    87,    26,   111,     9,   539,   174,   166,
     539,   539,   166,   166,    36,   166,   115,   142,   500,   610,
     502,    57,   147,    91,    46,   150,   173,    95,   175,   154,
     500,    53,   502,    55,   564,   565,   166,    57,  1089,  1090,
    1091,  1092,    64,   173,  1096,   175,    80,    81,  1135,    91,
      72,  1131,    91,    95,   105,   839,    95,   171,   173,   173,
     175,    32,    80,    81,   500,   849,   502,    89,   278,    28,
     280,   173,   570,   175,    57,   859,   500,   173,   502,   101,
      57,     2,    28,   105,   173,   728,   175,   102,   215,    48,
     112,   292,   293,    52,    57,   117,   578,   593,   578,   173,
      57,    57,    48,    57,   747,    26,    52,    57,   578,  1196,
    1197,  1198,  1199,  1200,  1201,     5,   900,   327,    77,   903,
      91,    80,  1141,    82,    95,   150,   171,  1004,    57,   173,
     147,    77,   165,    92,    80,    57,    82,   776,   922,   923,
     171,   171,   578,   270,    86,   142,    92,   166,   275,    32,
     147,   174,   111,   150,   578,    76,   166,   154,  1035,    80,
      81,   166,   166,  1040,   166,   111,   166,   166,   166,   812,
     813,   142,   143,  1192,   145,   146,   147,   166,   149,   150,
     151,   102,   166,   154,   968,   106,   970,   173,   173,   160,
     161,   173,   147,   164,   166,   148,   167,   168,   169,   838,
       5,   166,     9,   173,   148,   174,     3,   178,    91,   173,
      67,    80,    95,   171,    92,   745,    51,    92,    23,    54,
     863,   126,    27,    28,   351,   166,  1103,   150,   174,    70,
     884,    36,   126,    38,    39,   173,   148,   767,   365,    44,
      75,   174,   126,    78,   126,    35,    51,   374,    35,    67,
      85,   147,   126,    22,   174,   174,   174,   171,   740,   142,
     387,   904,   120,   173,   147,   172,   149,   150,    73,   751,
     126,   154,   754,    78,   107,    80,   111,    82,   161,    84,
     148,   751,    87,   150,   754,    90,   169,    92,   173,   150,
     150,   150,   173,     2,   215,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   142,   111,   150,   113,   174,
     176,   116,   174,    35,    35,   751,    48,    26,   754,   147,
      35,   166,   166,   148,   166,    33,   166,   751,   971,   148,
     754,  1074,  1075,   865,   166,   166,   172,   166,   176,   871,
     174,   171,    35,   147,   166,   172,   166,   845,   880,   270,
      28,   174,   176,   166,   275,   176,   173,   173,   166,   174,
     148,   166,   176,   174,   176,   847,   176,    76,   174,   851,
      48,    80,    81,   500,    52,   502,   858,   847,   174,   971,
     166,   851,   165,   165,   111,   573,   596,   597,   858,   265,
      25,   367,   810,   102,   604,   605,   374,   106,   391,    77,
     938,   340,    80,   524,    82,   964,   539,   880,   871,  1192,
     385,   847,   884,   305,    92,   851,  1042,  1202,  1195,   131,
     837,   674,   858,   847,   521,   908,   763,   851,    -1,    -1,
     351,  1074,  1075,   111,   858,    -1,    -1,    -1,    -1,    -1,
     922,   923,    -1,    -1,   365,  1036,    -1,   929,    -1,   931,
      -1,   578,   934,   374,    -1,    -1,    -1,    -1,    -1,   929,
      -1,   931,   960,    -1,   934,  1119,   387,    -1,    -1,    -1,
      -1,    -1,  1004,    -1,    -1,    -1,    -1,   959,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   969,    -1,   959,
      -1,    -1,    -1,   929,    -1,   931,    -1,    -1,   934,   969,
      -1,    -1,    -1,    -1,    -1,   929,   215,   931,    -1,    -1,
     934,  1007,  1008,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   959,    -1,    -1,    -1,    -1,    -1,     5,
      -1,    -1,  1014,   969,    -1,   959,  1127,    -1,  1020,    -1,
    1131,    -1,    -1,    -1,  1014,   969,    -1,    23,    -1,    -1,
    1020,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      36,   270,    38,    39,    -1,    -1,   275,  1221,    44,  1223,
      -1,  1103,  1226,  1227,    -1,    51,    -1,    -1,  1014,   500,
      -1,   502,    -1,    -1,  1020,    -1,  1068,    -1,    -1,    -1,
    1014,    -1,   802,    -1,    -1,    -1,  1020,    73,  1068,    -1,
      -1,    -1,    78,    -1,    80,    -1,    -1,    -1,    84,    -1,
      -1,    87,    -1,    -1,    90,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   751,    -1,    -1,   754,    -1,    -1,
     840,  1113,  1068,  1115,    -1,   111,    -1,   113,  1120,    -1,
     116,  1123,   351,  1113,  1068,  1115,    -1,   857,    -1,    -1,
    1120,   861,    -1,  1123,    -1,    -1,   365,   578,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   374,    -1,    -1,    -1,    -1,
    1152,    -1,    -1,    -1,    -1,    -1,  1158,  1113,   387,  1115,
    1162,  1163,  1152,    -1,  1120,    -1,    -1,  1123,  1158,  1113,
     166,  1115,  1162,  1163,  1176,     2,  1120,    -1,  1180,  1123,
    1182,    -1,    -1,    -1,    -1,    -1,  1176,    -1,    -1,    -1,
    1180,    -1,  1182,    -1,    -1,    -1,  1152,    -1,    -1,    26,
     847,  1203,  1158,  1203,   851,    -1,  1162,  1163,  1152,    -1,
      -1,   858,    -1,  1203,  1158,    32,    -1,    -1,  1162,  1163,
    1176,    -1,    -1,    -1,  1180,    -1,  1182,    -1,    -1,    -1,
      -1,    -1,  1176,    -1,    -1,    -1,  1180,    -1,  1182,    -1,
      -1,    -1,    -1,  1245,  1246,  1247,  1248,  1203,    -1,    76,
      -1,    -1,    -1,    80,    81,  1245,  1246,  1247,  1248,  1203,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   500,    -1,   502,    91,   102,    -1,    -1,    95,   106,
      -1,    -1,   929,    -1,   931,    -1,    13,   934,    -1,  1245,
    1246,  1247,  1248,    20,    -1,    -1,     2,  1027,  1028,    -1,
      -1,  1245,  1246,  1247,  1248,    32,    33,    -1,    -1,    -1,
     751,    -1,   959,   754,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    48,   969,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,    -1,    -1,    -1,   154,    -1,    -1,
      -1,    -1,    32,   160,   161,    -1,    -1,   164,    -1,   578,
     167,   168,   169,    -1,    -1,  1085,    -1,    -1,    -1,    -1,
      -1,  1091,  1092,    -1,    91,    -1,    -1,  1014,    95,    -1,
      76,    -1,    -1,  1020,    80,    81,    -1,  1107,  1108,    -1,
    1110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   215,    -1,
      -1,    -1,  1122,    -1,   121,  1125,   102,    -1,   125,    -1,
     106,    91,    -1,    -1,    -1,    95,   847,    -1,    -1,    -1,
     851,    -1,    -1,    -1,    -1,   142,   143,   858,   145,   146,
     147,  1068,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,   270,   171,    -1,    -1,    -1,   275,    -1,
      -1,   178,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,    -1,    -1,    -1,   154,    -1,  1113,    32,  1115,    -1,
       2,   161,    -1,  1120,    -1,    -1,  1123,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   929,    -1,
     931,    -1,    -1,   934,    26,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1152,    -1,    -1,    -1,   215,
      -1,  1158,    -1,    -1,    -1,  1162,  1163,    -1,   959,    -1,
      -1,    -1,   751,    -1,   351,   754,    91,    -1,   969,  1176,
      95,    -1,    -1,  1180,    32,  1182,    -1,    -1,   365,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    -1,   374,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,  1203,    -1,    95,    -1,
     387,    -1,    -1,    -1,   270,    -1,    -1,    -1,    -1,   275,
     102,    -1,    -1,  1014,   106,    -1,    -1,   142,   143,  1020,
      -1,   146,   147,    -1,   149,   150,    -1,    -1,    68,   154,
      -1,    -1,    -1,    91,    -1,    -1,   161,    95,  1245,  1246,
    1247,  1248,   167,   168,   169,   142,   143,    -1,    88,    -1,
     147,    -1,   149,   150,    -1,    -1,    -1,   154,   847,    -1,
      -1,    -1,   851,   103,   161,    -1,    -1,  1068,    -1,   858,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   351,    -1,    -1,    -1,   147,
      -1,   149,   150,    -1,    -1,    -1,   154,    -1,    -1,   365,
      -1,    -1,    -1,   161,    -1,    -1,    -1,    -1,   374,   167,
     168,   169,  1113,   500,  1115,   502,    -1,    -1,    -1,  1120,
      -1,   387,  1123,   215,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,    -1,
     929,    -1,   931,    -1,    -1,   934,    -1,    -1,    -1,    -1,
      -1,  1152,    -1,    -1,    -1,    -1,    -1,  1158,    -1,    -1,
      -1,  1162,  1163,    -1,    -1,    -1,    -1,    -1,   170,    -1,
     959,   173,    -1,   175,    -1,  1176,    -1,    -1,   270,  1180,
     969,  1182,    -1,   275,    -1,    -1,    -1,    -1,    -1,   229,
      -1,   578,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1203,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1014,   266,    -1,    -1,    -1,
      -1,  1020,    -1,    -1,   500,     1,   502,     3,    -1,    -1,
      -1,    -1,    -1,    -1,  1245,  1246,  1247,  1248,    -1,    -1,
      -1,   291,    18,    19,    -1,    21,    -1,    -1,    -1,   351,
      26,    -1,    -1,    -1,    -1,    -1,     1,    -1,    34,    -1,
       5,    -1,    -1,   365,    40,    -1,    -1,    -1,    -1,  1068,
      46,    -1,   374,    -1,    50,    -1,    -1,    53,    23,    55,
      -1,   331,    27,    28,    -1,   387,    -1,    -1,    64,    -1,
      -1,    36,    -1,    38,    39,    71,    72,    -1,    -1,    44,
      -1,    -1,   578,    79,    -1,    -1,    51,    -1,    -1,    54,
      -1,    -1,    -1,    89,  1113,    -1,  1115,    -1,    -1,    -1,
      -1,  1120,    98,    99,  1123,   101,    -1,   103,    73,   105,
      75,    -1,   108,    78,    -1,    80,   112,    82,   114,    84,
      85,   117,    87,    -1,    -1,    90,    -1,    92,    -1,    -1,
      -1,    -1,    -1,  1152,   751,    -1,    -1,   754,    -1,  1158,
      -1,    -1,    -1,  1162,  1163,    -1,   111,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,  1176,    -1,    -1,
      -1,  1180,    -1,  1182,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,   500,    -1,
     502,    -1,    -1,    -1,  1203,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   463,   464,    -1,    -1,    -1,    -1,   469,
      -1,   166,   472,   473,   474,   475,    -1,   172,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1245,  1246,  1247,  1248,
     847,    -1,    -1,    -1,   851,    -1,    -1,    -1,    -1,   509,
      -1,   858,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   751,   578,    -1,   754,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   551,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   564,   565,    -1,    -1,    -1,    -1,
      18,    19,    -1,    21,   574,    -1,    -1,    -1,    26,    -1,
      -1,    -1,   929,    -1,   931,    -1,    34,   934,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,   959,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,   847,   969,    71,    72,   851,    -1,    -1,    -1,    -1,
      -1,    79,   858,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,   101,    -1,   103,    -1,   105,    -1,    -1,
     108,    -1,    -1,    -1,   112,    -1,   114,  1014,    -1,   117,
      -1,    -1,    -1,  1020,    -1,    -1,    -1,    -1,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,   751,
      27,    28,   754,   929,    -1,   931,    -1,    -1,   934,    36,
      -1,    38,    39,    -1,    23,    -1,    -1,    44,    27,    28,
      -1,  1068,    -1,   171,    51,   173,    -1,    36,   728,    38,
      39,    -1,    -1,   959,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    51,   969,    -1,   745,    73,   747,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    84,    -1,    -1,
      87,    -1,    -1,    90,    73,    92,  1113,   767,  1115,    78,
      -1,    80,    -1,  1120,    -1,    -1,  1123,    -1,    -1,    -1,
      -1,    90,    -1,    92,   111,    -1,   113,    -1,  1014,   116,
      -1,    -1,    -1,    -1,  1020,   847,    -1,    -1,    -1,   851,
      -1,    -1,   111,    -1,   113,  1152,   858,   116,    -1,    -1,
      -1,  1158,    -1,    -1,    -1,  1162,  1163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1176,
      -1,    -1,    -1,  1180,    -1,  1182,    -1,    -1,    -1,   166,
      -1,    -1,  1068,    26,    -1,   172,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1203,    -1,    -1,    -1,
      -1,    -1,    -1,   863,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   929,    -1,   931,
      -1,    -1,   934,    -1,    -1,    -1,    -1,  1113,    -1,  1115,
      -1,    -1,    -1,    76,  1120,    -1,    -1,  1123,  1245,  1246,
    1247,  1248,    -1,    -1,   904,    -1,    -1,   959,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   969,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    14,  1152,    -1,    -1,    18,
      -1,    -1,  1158,    -1,    -1,    -1,  1162,  1163,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
    1176,    -1,    -1,    -1,  1180,    -1,  1182,    46,    -1,    48,
      49,    -1,  1014,    -1,    -1,    54,    -1,    56,  1020,    58,
      -1,   971,    -1,    -1,    63,    -1,    -1,  1203,    67,    68,
      69,    70,    71,    72,    -1,    -1,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    88,
      -1,    90,    -1,    92,    -1,    -1,    95,    96,    -1,    98,
      -1,    -1,    -1,   102,   103,   104,  1068,    -1,    -1,  1245,
    1246,  1247,  1248,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      -1,    -1,   131,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    42,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1113,    -1,  1115,    -1,    -1,    -1,    -1,  1120,    -1,
      -1,  1123,    -1,    -1,  1074,  1075,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,    -1,   270,    -1,    -1,
      -1,    -1,   275,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1152,    -1,    -1,    -1,    -1,    -1,  1158,    -1,   197,    -1,
    1162,  1163,   201,   202,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    -1,  1176,    -1,    -1,    -1,  1180,    -1,
    1182,    -1,    -1,    -1,    -1,    -1,    18,    19,    -1,    21,
     229,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,  1203,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
     150,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,   164,    -1,    -1,   266,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,  1245,  1246,  1247,  1248,    79,    -1,    -1,
      -1,    -1,   291,    -1,   387,    -1,    -1,    89,    -1,   199,
      -1,    -1,    -1,   302,   303,    -1,    98,    99,    -1,   101,
      -1,   103,    -1,   105,    -1,    -1,   108,    -1,   317,   318,
     112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,   331,    -1,    -1,    -1,   335,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   353,    -1,    -1,    -1,    -1,   259,
     260,    -1,    -1,    -1,   363,    -1,    -1,    -1,    -1,    -1,
      -1,   271,    -1,    -1,    -1,    -1,   375,    -1,   377,    -1,
      -1,   173,    -1,    -1,    -1,    -1,    -1,   386,    -1,    -1,
      -1,    -1,   391,   392,   393,    -1,    -1,    -1,    -1,   398,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   500,    -1,   502,
      -1,   311,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   427,   428,
     429,   430,   431,   432,    -1,   434,   435,   436,   437,   438,
      -1,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,    -1,    -1,
      -1,   361,    -1,    -1,   463,   464,    -1,    -1,    -1,    -1,
     469,    -1,    -1,   472,   473,   474,   475,   476,   477,    -1,
      -1,    46,   481,   482,    -1,    -1,   485,    -1,    -1,    -1,
      -1,    -1,   491,    -1,    -1,    -1,    -1,   397,    -1,    -1,
      -1,   401,    -1,   502,    -1,    70,   505,    -1,    -1,    -1,
     509,    -1,    -1,    -1,    -1,    80,    81,    -1,   418,   518,
      -1,   520,    -1,    -1,    -1,   524,    -1,    -1,   527,   528,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     539,    -1,    -1,   542,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,   551,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   561,    -1,    -1,   564,   565,    -1,    -1,    -1,
       5,   570,   571,    -1,    -1,   574,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   483,    -1,    -1,   585,   586,    23,    -1,
      -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
     599,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,
     510,    -1,    -1,   513,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,   621,    -1,    -1,    -1,   526,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   536,    -1,    73,    -1,
     540,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    84,
      -1,    -1,    87,    -1,    -1,    90,   221,    92,   751,    -1,
      -1,   754,    -1,    -1,    -1,    -1,   665,    32,    -1,    -1,
      -1,    -1,    -1,   672,    -1,    -1,   111,    -1,   113,    -1,
      -1,   116,    -1,    -1,   584,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   595,   261,   262,    -1,    -1,
      -1,    -1,    -1,    -1,   269,    -1,    -1,    -1,    -1,    -1,
     610,    -1,    -1,   278,    -1,   280,    -1,    -1,    -1,    -1,
      -1,    -1,   287,    -1,    -1,    -1,    91,    -1,   727,   728,
      95,   166,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   745,    -1,   747,   748,
     749,    -1,    -1,    -1,   847,    -1,    -1,    -1,   851,    -1,
     125,    -1,   327,    -1,    -1,   858,    -1,    -1,   767,    -1,
      -1,    -1,    -1,    -1,   674,    -1,    -1,   142,   143,    -1,
     145,   146,   147,   782,   149,   150,   151,   786,   153,   154,
     155,   156,    -1,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,   810,    -1,   178,    -1,   814,    -1,   816,   817,   818,
      -1,   820,    -1,   822,    -1,    -1,   825,   826,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,   929,    -1,   931,    -1,
     839,   934,   841,    -1,    -1,    -1,   845,    -1,    -1,    -1,
     849,   850,   851,    -1,   853,   854,   756,    -1,    -1,   858,
     859,    -1,    -1,   763,   863,    -1,   959,    -1,    -1,   868,
      -1,    -1,    -1,   773,    -1,    -1,   969,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,   792,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   900,   901,   902,   903,   904,    -1,    -1,    -1,   908,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1014,    -1,   922,   923,    -1,    -1,  1020,    -1,    -1,
      -1,    -1,    -1,   932,    -1,    -1,    -1,   837,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   950,   154,   155,   156,   954,   158,   159,   160,   161,
      -1,   960,   164,    -1,    -1,   167,   168,   169,    -1,   968,
      -1,   970,   971,    -1,    -1,  1068,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   884,    -1,    -1,    -1,    -1,   554,
      -1,    -1,    -1,   992,    -1,    -1,    -1,    -1,    -1,    -1,
     999,    -1,  1001,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1011,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1113,    -1,  1115,    -1,    -1,    -1,  1025,  1120,    -1,    -1,
    1123,   596,   597,    -1,    -1,    -1,    -1,    -1,   938,   604,
     605,    -1,    -1,    -1,    -1,  1044,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1056,    -1,  1152,
      -1,    -1,    -1,    -1,   964,  1158,    -1,    -1,    -1,  1162,
    1163,    -1,    -1,    -1,    -1,  1074,  1075,    -1,    -1,    -1,
      -1,    -1,    -1,  1176,    -1,    -1,    -1,  1180,  1087,  1182,
      -1,    -1,    -1,    -1,    -1,    -1,  1095,  1096,  1097,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1113,    -1,  1115,    -1,    -1,  1118,
      -1,  1120,    -1,    -1,  1123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1039,
      -1,    -1,    -1,    -1,    -1,    -1,  1145,  1146,    -1,  1148,
    1149,    -1,  1245,  1246,  1247,  1248,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,  1168,
      -1,    -1,    -1,    -1,  1173,    -1,    32,    33,  1177,    -1,
      -1,    -1,    -1,  1182,  1183,    -1,    -1,  1186,    -1,    -1,
     755,    -1,    48,    -1,    -1,  1194,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1208,
    1209,  1210,  1211,  1212,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   792,    -1,    -1,
      -1,  1131,    -1,    -1,    -1,    91,    -1,   802,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,  1245,  1246,  1247,  1248,
    1249,  1250,  1251,  1252,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,   840,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,   857,   149,   150,   151,   861,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,     0,     1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    17,    18,    19,    -1,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,   939,    49,    50,    51,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    69,    -1,    71,    72,    73,
      -1,    75,   967,    -1,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,   105,   106,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,    -1,   120,    -1,   122,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1027,  1028,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,     3,    -1,   163,
     164,   165,   166,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,   175,    18,    19,   178,    21,    32,    -1,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
    1085,    -1,    -1,    -1,    40,    -1,  1091,  1092,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    -1,  1107,  1108,    -1,  1110,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,  1119,    71,    72,  1122,    -1,    -1,
    1125,    -1,    -1,    79,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,    -1,   101,    -1,   103,    -1,   105,
      -1,    -1,   108,    -1,    -1,  1160,   112,    -1,   114,   125,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,    -1,   158,   159,   160,   161,   162,    -1,   164,    -1,
      -1,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,  1221,   173,  1223,    -1,
      -1,  1226,  1227,     1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    13,    -1,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    -1,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,   105,   106,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,    -1,   120,    -1,   122,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,    -1,   171,   172,   173,    -1,   175,    -1,     1,
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
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,     1,
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
     166,    -1,    -1,   169,    -1,   171,    -1,   173,     1,   175,
       3,     4,   178,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      43,    -1,    45,    46,    -1,    48,    -1,    50,    -1,    -1,
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
     173,   174,   175,    -1,     1,   178,     3,     4,    -1,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    43,    -1,    45,    46,
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
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,     3,
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
     164,   165,   166,   167,   168,   169,   170,   171,    -1,   173,
      -1,    -1,     3,     4,   178,     6,     7,     8,     9,    10,
      11,    -1,    13,    -1,    15,    -1,    -1,    18,    19,    20,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    56,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,   134,   135,   136,   137,    -1,   139,   140,
      -1,   142,   143,   144,   145,   146,   147,    -1,   149,    -1,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,   167,   168,   169,   170,
     171,    -1,   173,    -1,    -1,    -1,    -1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    16,    -1,    18,    19,    -1,    21,    -1,    23,    24,
      -1,    26,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    57,    -1,    -1,    60,    61,    -1,    63,    64,
      65,    66,    -1,    68,    69,    -1,    71,    72,    73,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,
      -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
     105,   106,    -1,   108,    -1,    -1,    -1,   112,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
      -1,     3,     4,   178,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    -1,    26,    27,    28,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,
      -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
     112,    -1,   114,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,    -1,     3,     4,   178,     6,     7,     8,
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
      -1,    -1,    -1,   112,   113,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,     3,     4,   178,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,
      66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    88,    89,    90,    91,    -1,    -1,    -1,    95,
      -1,    -1,    98,    99,   100,   101,   102,   103,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,   148,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
     166,    -1,    -1,   169,    -1,   171,   172,   173,    -1,    -1,
       3,     4,   178,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      43,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
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
     173,    -1,   175,     3,     4,   178,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,
      -1,    21,    -1,    23,    -1,    -1,    26,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    45,    46,    -1,    48,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,
      60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,
      90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,   103,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,   112,    -1,   114,    -1,   116,   117,    -1,    -1,
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
      -1,   108,   109,   110,    -1,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,
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
     144,   145,   146,    -1,    -1,    -1,    -1,   151,   152,    -1,
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
     171,    -1,   173,   174,    -1,     3,     4,   178,     6,     7,
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
     108,   109,   110,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,    -1,   176,    -1,
     178,     3,     4,    -1,     6,     7,     8,     9,    10,    11,
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
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,   174,    -1,
       3,     4,   178,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,    -1,   105,    -1,    -1,   108,   109,   110,    -1,   112,
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
     164,   165,   166,    -1,    -1,   169,    -1,   171,   172,   173,
      -1,    -1,     3,     4,   178,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,   112,    -1,   114,    -1,   116,   117,    -1,    -1,    -1,
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
     108,   109,   110,    -1,   112,    -1,   114,    -1,    -1,   117,
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
      -1,    -1,    -1,    88,    89,    90,    91,    -1,    -1,    -1,
      95,    -1,    -1,    98,    99,   100,   101,   102,   103,    -1,
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
      -1,   123,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,    -1,   176,    -1,   178,     3,     4,    -1,
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
     146,    -1,   148,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,   174,    -1,
       3,     4,   178,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
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
      -1,    -1,    -1,   123,    -1,   125,    -1,    -1,    -1,    -1,
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
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,   176,
      -1,   178,     3,     4,    -1,     6,     7,     8,     9,    10,
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
     171,   172,   173,    -1,    -1,     3,     4,   178,     6,     7,
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
      -1,   169,    -1,   171,    -1,   173,    -1,    -1,   176,    -1,
     178,     3,     4,    -1,     6,     7,     8,     9,    10,    11,
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
      -1,   173,    -1,    -1,   176,    -1,   178,     3,     4,    -1,
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
      68,    69,    -1,    71,    72,    -1,    -1,    -1,    32,    -1,
      -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,    -1,   105,    -1,    -1,
     108,    -1,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    95,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,    -1,    -1,     4,
     178,     6,     7,     8,     9,    10,    11,    -1,   142,   143,
      15,   145,   146,   147,    19,   149,   150,   151,    23,   153,
     154,   155,   156,    28,   158,   159,   160,   161,   162,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,   174,
     175,    -1,     4,   178,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    41,
      42,    43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    57,    -1,    -1,    60,    61,
      -1,    -1,    -1,    -1,    66,    -1,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    80,    81,
      82,    83,    -1,    -1,    -1,    -1,    88,    -1,    90,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    13,    -1,   151,
      -1,    -1,    18,    19,    20,    21,    -1,    -1,   160,    -1,
      26,   163,   164,   165,    -1,    -1,    -1,   169,    34,   171,
      36,   173,    -1,   175,    40,    -1,   178,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,    -1,   101,    -1,   103,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,   129,   130,   131,    -1,    -1,   134,   135,
     136,   137,    -1,   139,   140,    -1,   142,   143,   144,   145,
     146,   147,    -1,   149,    -1,    -1,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,   164,    -1,
      -1,   167,   168,   169,   170,     3,     4,   173,     6,     7,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,    -1,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,     3,     4,
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
      -1,    -1,   117,    -1,    -1,     3,     4,    -1,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      18,    19,    -1,    21,    -1,    23,    -1,   142,    26,   144,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    43,    -1,    45,    46,    -1,
     165,    -1,    50,    -1,    -1,    53,   171,    55,   173,    57,
     175,    -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,
      68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,    -1,   105,    -1,    -1,
     108,     3,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    -1,    21,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,   142,    -1,   144,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    64,    -1,    -1,   173,    -1,   175,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,   101,
      -1,   103,    -1,   105,     3,    -1,   108,    -1,    -1,    -1,
     112,    -1,   114,    -1,    13,   117,    -1,    -1,    -1,    18,
      19,    20,    21,    -1,    -1,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,   173,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,    -1,   101,    -1,   103,    -1,   105,    -1,    -1,    -1,
      -1,    -1,   111,   112,    -1,   114,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
     129,   130,   131,    -1,    -1,   134,   135,   136,   137,    -1,
     139,   140,    -1,   142,   143,   144,   145,   146,   147,    -1,
     149,    -1,    -1,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,     3,   164,    -1,    -1,   167,   168,
     169,   170,   171,    -1,    13,    -1,    -1,    -1,    -1,    18,
      19,    20,    21,    -1,    -1,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
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
     159,   160,   161,   162,     5,   164,    -1,    -1,   167,   168,
     169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,
      13,    -1,    -1,    44,    -1,    -1,    -1,    20,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    84,    -1,    -1,    87,    -1,    -1,    90,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    91,    13,
      -1,    -1,    95,    -1,    -1,    -1,    20,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,   166,   139,   140,    -1,   142,
     143,   172,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    91,   171,    -1,
      13,    95,    -1,    -1,    -1,   178,    -1,    20,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,    -1,    -1,
     134,   135,   136,   137,    -1,   139,   140,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,   171,    91,    13,
      -1,    -1,    95,    -1,   178,    -1,    20,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,    -1,   139,   140,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    91,   171,    -1,
      13,    95,    -1,    -1,    -1,   178,    -1,    20,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,    -1,    -1,
     134,   135,   136,   137,    -1,   139,   140,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,   171,    91,    13,
      -1,    -1,    95,    -1,   178,    -1,    20,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,    -1,   139,   140,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    91,   171,    -1,
      -1,    95,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,    -1,    32,
     134,   135,   136,   137,    37,   139,   140,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,   171,    -1,    -1,
      -1,    74,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
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
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      23,   167,   168,   169,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      73,    -1,    75,    -1,    -1,    78,     1,    80,     3,    -1,
       5,    84,    85,    86,    87,    -1,    -1,    90,    -1,    92,
      -1,    -1,    -1,    18,    19,    -1,    21,    -1,    -1,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,   111,    34,
     113,    -1,    -1,   116,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    18,
      19,    -1,    21,    -1,    79,    -1,    -1,    26,    -1,    28,
      -1,    -1,    -1,   166,    89,    34,    -1,    -1,   171,    -1,
      -1,    40,    -1,    98,    99,    -1,   101,    46,   103,    -1,
     105,    50,    -1,   108,    53,    -1,    55,   112,    -1,   114,
      -1,    -1,   117,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    92,    -1,    -1,     3,    -1,     5,    98,
      99,    -1,   101,    -1,   103,    -1,   105,    -1,    -1,   108,
      -1,    18,    19,   112,    21,   114,    -1,   116,   117,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    64,    -1,    -1,
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
      55,   112,    -1,   114,     3,    -1,   117,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    18,
      19,    -1,    21,    -1,    79,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    98,    99,    -1,   101,    46,   103,    -1,
     105,    50,    -1,   108,    53,    -1,    55,   112,    -1,   114,
      -1,    -1,   117,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,    -1,   101,    -1,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,   112,    -1,   114,    -1,    -1,   117
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
     368,   221,     1,    28,    48,    92,   116,   174,   358,   359,
     360,   368,   384,   328,   368,   126,   174,   148,   126,   126,
     187,    35,   187,   368,    35,   368,    67,   174,   345,   221,
     248,   249,   331,   148,   174,   225,   368,   126,   332,   368,
     329,   262,   221,   324,   368,   368,   324,   174,   221,   310,
     283,   331,   147,   339,   340,   338,   288,   206,     1,   260,
     344,   174,    22,   256,   172,   174,   174,   342,   370,   174,
     342,   341,   368,   197,   174,   199,   200,   346,   172,   172,
     148,   166,    14,   150,   148,   166,    14,    37,    74,   221,
     142,   143,   144,   145,   146,   160,   164,   169,   209,   296,
     297,   298,   368,   209,   211,   268,   171,   238,   324,    48,
     356,   120,   221,   226,   226,   173,   368,   176,   357,    48,
     148,   176,   357,   124,   148,   176,   226,   357,    33,    48,
     226,   357,   166,   173,     1,   269,   172,   275,   126,   148,
     172,   148,   174,   337,     1,   225,   333,   334,   107,   311,
     173,   300,   368,   142,   150,   300,   300,   368,   322,   173,
     175,   166,   166,   166,   166,   166,   166,   174,   176,   345,
      48,   176,    33,    48,   173,   174,    48,    92,   148,   174,
      18,    21,    26,    46,    53,    64,    72,    89,   101,   112,
     117,   347,    91,    91,   166,   370,   346,   368,   368,    35,
     187,    35,    35,   187,   104,   207,   221,   174,   148,   174,
     174,   330,   337,    70,   370,   221,   174,   174,   328,   340,
     147,   284,   174,   338,   152,   299,   332,   368,   172,    76,
     118,   172,   261,   174,   173,   207,   221,   257,    48,   176,
      48,   148,   174,   215,   222,    18,    19,    21,    26,    46,
      50,    53,    72,    79,    89,    98,    99,   101,   103,   112,
     114,   117,   171,   214,   298,   368,   368,   209,   211,   148,
     166,    14,   166,   171,   269,   321,   328,   244,   368,   226,
     368,    48,   341,   187,   176,   244,   368,   176,   187,   368,
     176,   368,   176,   368,   226,    45,   368,   244,   368,   226,
     345,   172,    84,    87,   172,   186,   194,   228,   368,   273,
     274,   304,   311,    62,   119,   315,   302,   303,   174,   295,
     297,   174,   176,   174,   244,   368,    45,   244,   368,   345,
     359,   337,   347,   347,   187,   187,   368,   187,    35,   166,
     166,   249,   207,   332,   173,   173,   166,   299,   332,   333,
     311,   340,   368,   187,   240,   341,   257,   147,   207,   244,
     368,   244,   368,   200,   209,    14,    37,    74,   166,   166,
     298,   368,   368,   269,   172,   166,   166,   356,   356,   166,
     368,   174,   187,   176,   357,   176,   357,   187,   124,   368,
      33,   226,   357,    33,   226,   357,   174,   194,   228,   228,
     275,   194,   315,     3,    55,    94,   105,   316,   317,   318,
     368,   291,   174,   300,   300,   176,   176,   368,    33,    33,
     174,   332,    35,   187,   344,   344,   299,   332,    33,   226,
     174,   368,   176,   176,   172,   368,   209,   211,    14,   172,
     226,   226,   226,    45,   187,   368,   176,    45,   187,   368,
     176,   368,   104,    45,   368,   226,    45,   368,   226,   166,
     228,   275,   278,   318,   119,   148,   126,   153,   155,   156,
     158,   159,    62,    33,   166,   206,   312,   313,    45,    45,
     104,    45,    45,   187,   174,   174,   187,   207,   166,   166,
     368,   368,   187,   368,   187,   368,   368,   368,   313,   368,
     317,   318,   318,   318,   318,   318,   318,   316,   185,   368,
     368,   368,   368,   165,   165,   104,   104,   104,   104,   104,
     104,   104,   104,   368,   368,   368,   368
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
     360,   361,   361,   361,   361,   361,   362,   362,   362,   362,
     362,   363,   364,   364,   364,   364,   364,   364,   365,   366,
     367,   367,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   369,
     369,   370,   370,   370,   371,   371,   371,   371,   372,   372,
     372,   372,   372,   373,   373,   373,   374,   374,   374,   374,
     374,   374,   375,   375,   375,   375,   376,   376,   377,   377,
     378,   378,   378,   378,   378,   378,   378,   378,   378,   378,
     378,   378,   378,   379,   379,   380,   380,   380,   380,   380,
     380,   380,   380,   380,   380,   380,   380,   380,   380,   380,
     380,   380,   380,   380,   380,   380,   380,   380,   381,   381,
     381,   381,   381,   381,   381,   382,   382,   382,   382,   383,
     383,   383,   383,   384,   384,   384,   384,   384,   384,   384
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
       1,     1,     2,     2,     2,     2,     2,     8,     8,     9,
       9,     4,     3,     3,     2,     2,     2,     1,     3,     4,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     4,     4,     4,     3,     3,     3,     3,
       5,     5,     3,     4,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     4,     4,
       5,     4,     5,     3,     5,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1
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
#line 5954 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 639 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 5960 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 640 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 5966 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 5974 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 655 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 5982 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 659 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 5991 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 669 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 5999 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 674 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6007 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 681 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6013 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6019 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6025 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6031 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6037 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6043 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6049 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6055 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 689 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6061 "bison-chpl-lib.cpp"
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
#line 6083 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 708 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6089 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 709 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6095 "bison-chpl-lib.cpp"
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
#line 6107 "bison-chpl-lib.cpp"
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
#line 6120 "bison-chpl-lib.cpp"
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
#line 6133 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TLABEL ident_def stmt  */
#line 737 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6141 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 741 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6152 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt_base: error TSEMI  */
#line 748 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6161 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: assignment_stmt  */
#line 755 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6167 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: if_stmt  */
#line 756 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6173 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: loop_stmt  */
#line 757 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6179 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: select_stmt  */
#line 758 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6185 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: manage_stmt  */
#line 759 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6191 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 760 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6197 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: throw_stmt  */
#line 761 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6203 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 763 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6209 "bison-chpl-lib.cpp"
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
#line 6233 "bison-chpl-lib.cpp"
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
#line 6245 "bison-chpl-lib.cpp"
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
#line 6266 "bison-chpl-lib.cpp"
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
#line 6286 "bison-chpl-lib.cpp"
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
#line 6307 "bison-chpl-lib.cpp"
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
#line 6328 "bison-chpl-lib.cpp"
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
#line 6348 "bison-chpl-lib.cpp"
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
#line 6370 "bison-chpl-lib.cpp"
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
#line 6382 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 926 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6390 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 933 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6399 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 940 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6407 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 944 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6418 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 954 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6424 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 956 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6432 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 960 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6440 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 967 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6449 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 972 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6459 "bison-chpl-lib.cpp"
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
#line 6481 "bison-chpl-lib.cpp"
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
#line 6502 "bison-chpl-lib.cpp"
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
#line 6523 "bison-chpl-lib.cpp"
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
#line 6542 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1057 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6548 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1058 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6555 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1060 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6562 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1065 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6568 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1066 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6575 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1072 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6585 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1090 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6594 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1095 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6603 "bison-chpl-lib.cpp"
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
#line 6616 "bison-chpl-lib.cpp"
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
#line 6635 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1130 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6641 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1131 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6647 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1136 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6655 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1140 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6663 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1144 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6672 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1149 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6680 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1153 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6688 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1157 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6697 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1165 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6706 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1170 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6716 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1176 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6725 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1181 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6736 "bison-chpl-lib.cpp"
    break;

  case 90: /* opt_only_ls: %empty  */
#line 1189 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6742 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: renames_ls  */
#line 1190 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6748 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1195 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6757 "bison-chpl-lib.cpp"
    break;

  case 93: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1200 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6769 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1208 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6782 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1217 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6794 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1225 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6807 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1236 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6816 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr  */
#line 1272 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6824 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT all_op_name  */
#line 1276 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6833 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1281 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6842 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TAS ident_use  */
#line 1286 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6852 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1292 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6862 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_expr  */
#line 1300 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6868 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_ls: import_ls TCOMMA import_expr  */
#line 1301 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6874 "bison-chpl-lib.cpp"
    break;

  case 122: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1306 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6884 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1315 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6892 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1319 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6900 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1323 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6908 "bison-chpl-lib.cpp"
    break;

  case 126: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1327 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6916 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1331 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6924 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1335 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6932 "bison-chpl-lib.cpp"
    break;

  case 129: /* opt_label_ident: %empty  */
#line 1343 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6938 "bison-chpl-lib.cpp"
    break;

  case 130: /* opt_label_ident: TIDENT  */
#line 1344 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6944 "bison-chpl-lib.cpp"
    break;

  case 131: /* opt_label_ident: TINIT  */
#line 1345 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6950 "bison-chpl-lib.cpp"
    break;

  case 140: /* ident_use: TIDENT  */
#line 1371 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6956 "bison-chpl-lib.cpp"
    break;

  case 141: /* ident_use: TINIT  */
#line 1372 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6962 "bison-chpl-lib.cpp"
    break;

  case 142: /* ident_use: TTHIS  */
#line 1373 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6968 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TBOOL  */
#line 1400 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6974 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TENUM  */
#line 1401 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6980 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TINT  */
#line 1402 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6986 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TUINT  */
#line 1403 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6992 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TREAL  */
#line 1404 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6998 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TIMAG  */
#line 1405 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7004 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TCOMPLEX  */
#line 1406 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7010 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TBYTES  */
#line 1407 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7016 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TSTRING  */
#line 1408 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7022 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TLOCALE  */
#line 1409 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7028 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TNOTHING  */
#line 1410 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7034 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TVOID  */
#line 1411 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7040 "bison-chpl-lib.cpp"
    break;

  case 178: /* do_stmt: TDO stmt  */
#line 1425 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7046 "bison-chpl-lib.cpp"
    break;

  case 179: /* do_stmt: block_stmt  */
#line 1426 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7052 "bison-chpl-lib.cpp"
    break;

  case 180: /* return_stmt: TRETURN TSEMI  */
#line 1431 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7063 "bison-chpl-lib.cpp"
    break;

  case 181: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1438 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7074 "bison-chpl-lib.cpp"
    break;

  case 182: /* class_level_stmt: TSEMI  */
#line 1448 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7082 "bison-chpl-lib.cpp"
    break;

  case 183: /* class_level_stmt: inner_class_level_stmt  */
#line 1452 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7092 "bison-chpl-lib.cpp"
    break;

  case 184: /* $@1: %empty  */
#line 1457 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7099 "bison-chpl-lib.cpp"
    break;

  case 185: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1459 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7108 "bison-chpl-lib.cpp"
    break;

  case 186: /* $@2: %empty  */
#line 1463 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7115 "bison-chpl-lib.cpp"
    break;

  case 187: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1465 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7124 "bison-chpl-lib.cpp"
    break;

  case 195: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1483 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7133 "bison-chpl-lib.cpp"
    break;

  case 196: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1488 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7142 "bison-chpl-lib.cpp"
    break;

  case 197: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1493 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7151 "bison-chpl-lib.cpp"
    break;

  case 198: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1498 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7159 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_start: TFORWARDING  */
#line 1505 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7168 "bison-chpl-lib.cpp"
    break;

  case 200: /* extern_or_export: TEXTERN  */
#line 1512 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7174 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_or_export: TEXPORT  */
#line 1513 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7180 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1518 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7190 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1528 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7200 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1535 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7212 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
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
#line 7234 "bison-chpl-lib.cpp"
    break;

  case 206: /* $@3: %empty  */
#line 1561 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7242 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1565 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7251 "bison-chpl-lib.cpp"
    break;

  case 208: /* $@4: %empty  */
#line 1570 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7260 "bison-chpl-lib.cpp"
    break;

  case 209: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1575 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7269 "bison-chpl-lib.cpp"
    break;

  case 210: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1583 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7277 "bison-chpl-lib.cpp"
    break;

  case 211: /* no_loop_attributes: %empty  */
#line 1588 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7283 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1591 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7289 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1592 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7295 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
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
    if (context->hasCurlyBracesLoc()) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC(context->curlyBracesLoc()));
      context->resetCurlyBracesLoc();
    }
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7322 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TWHILE expr do_stmt  */
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
    if (context->hasCurlyBracesLoc()) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC(context->curlyBracesLoc()));
      context->resetCurlyBracesLoc();
    }
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7345 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: TWHILE ifvar do_stmt  */
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
#line 7368 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1669 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7377 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1674 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7386 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1679 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7395 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1684 "chpl.ypp"
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
#line 7424 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1709 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7433 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw expr TIN expr task_intent_clause do_stmt  */
#line 1714 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7442 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1719 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7451 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator task_intent_clause do_stmt  */
#line 1724 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7460 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1729 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7469 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: for_loop_kw expr task_intent_clause do_stmt  */
#line 1734 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7478 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1739 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7487 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: for_loop_kw zippered_iterator task_intent_clause do_stmt  */
#line 1744 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7496 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1749 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7505 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR expr_ls TIN expr task_intent_clause TRSBR stmt  */
#line 1754 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7514 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1759 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7523 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator task_intent_clause TRSBR stmt  */
#line 1764 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7532 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1769 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7541 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt_base: TLSBR expr_ls task_intent_clause TRSBR stmt  */
#line 1774 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7550 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1779 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7560 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt_base: TLSBR zippered_iterator task_intent_clause TRSBR stmt  */
#line 1785 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7570 "bison-chpl-lib.cpp"
    break;

  case 240: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1794 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7580 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF expr TTHEN stmt  */
#line 1803 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7588 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF expr block_stmt  */
#line 1807 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7596 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1811 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7604 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1815 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7612 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1819 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7620 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF ifvar block_stmt  */
#line 1823 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7628 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1827 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7636 "bison-chpl-lib.cpp"
    break;

  case 248: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1831 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7644 "bison-chpl-lib.cpp"
    break;

  case 249: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1835 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7653 "bison-chpl-lib.cpp"
    break;

  case 250: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1840 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7662 "bison-chpl-lib.cpp"
    break;

  case 251: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1845 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7671 "bison-chpl-lib.cpp"
    break;

  case 252: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1850 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7680 "bison-chpl-lib.cpp"
    break;

  case 253: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1858 "chpl.ypp"
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
#line 7700 "bison-chpl-lib.cpp"
    break;

  case 254: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1874 "chpl.ypp"
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
#line 7720 "bison-chpl-lib.cpp"
    break;

  case 255: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1893 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7728 "bison-chpl-lib.cpp"
    break;

  case 256: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1897 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7736 "bison-chpl-lib.cpp"
    break;

  case 257: /* ifc_formal_ls: ifc_formal  */
#line 1903 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7742 "bison-chpl-lib.cpp"
    break;

  case 258: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1904 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7748 "bison-chpl-lib.cpp"
    break;

  case 259: /* ifc_formal: ident_def  */
#line 1909 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7754 "bison-chpl-lib.cpp"
    break;

  case 279: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1953 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7762 "bison-chpl-lib.cpp"
    break;

  case 280: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1957 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7770 "bison-chpl-lib.cpp"
    break;

  case 281: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1961 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7778 "bison-chpl-lib.cpp"
    break;

  case 282: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1968 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7786 "bison-chpl-lib.cpp"
    break;

  case 283: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1972 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7794 "bison-chpl-lib.cpp"
    break;

  case 284: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1976 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7802 "bison-chpl-lib.cpp"
    break;

  case 285: /* try_stmt: TTRY tryable_stmt  */
#line 1983 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7810 "bison-chpl-lib.cpp"
    break;

  case 286: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1987 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7818 "bison-chpl-lib.cpp"
    break;

  case 287: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1991 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7826 "bison-chpl-lib.cpp"
    break;

  case 288: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1995 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7834 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr_ls: %empty  */
#line 2001 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7840 "bison-chpl-lib.cpp"
    break;

  case 290: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2002 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7846 "bison-chpl-lib.cpp"
    break;

  case 291: /* catch_expr: TCATCH block_stmt  */
#line 2007 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7854 "bison-chpl-lib.cpp"
    break;

  case 292: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2011 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7862 "bison-chpl-lib.cpp"
    break;

  case 293: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2015 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7870 "bison-chpl-lib.cpp"
    break;

  case 294: /* catch_expr_inner: ident_def  */
#line 2022 "chpl.ypp"
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
#line 7888 "bison-chpl-lib.cpp"
    break;

  case 295: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2036 "chpl.ypp"
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
#line 7906 "bison-chpl-lib.cpp"
    break;

  case 296: /* throw_stmt: TTHROW expr TSEMI  */
#line 2053 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7916 "bison-chpl-lib.cpp"
    break;

  case 297: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2061 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7925 "bison-chpl-lib.cpp"
    break;

  case 298: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2066 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7936 "bison-chpl-lib.cpp"
    break;

  case 299: /* when_stmt_ls: %empty  */
#line 2075 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7942 "bison-chpl-lib.cpp"
    break;

  case 300: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2076 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7948 "bison-chpl-lib.cpp"
    break;

  case 301: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2081 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7956 "bison-chpl-lib.cpp"
    break;

  case 302: /* when_stmt: TOTHERWISE stmt  */
#line 2085 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7965 "bison-chpl-lib.cpp"
    break;

  case 303: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2090 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 7975 "bison-chpl-lib.cpp"
    break;

  case 304: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2099 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 7984 "bison-chpl-lib.cpp"
    break;

  case 305: /* manager_expr: expr TAS ident_def  */
#line 2104 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 7992 "bison-chpl-lib.cpp"
    break;

  case 306: /* manager_expr: expr  */
#line 2108 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8000 "bison-chpl-lib.cpp"
    break;

  case 307: /* manager_expr_ls: manager_expr  */
#line 2114 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8006 "bison-chpl-lib.cpp"
    break;

  case 308: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2115 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8012 "bison-chpl-lib.cpp"
    break;

  case 309: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2120 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8021 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2130 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8031 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2136 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8043 "bison-chpl-lib.cpp"
    break;

  case 312: /* class_start: class_tag ident_def  */
#line 2148 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8051 "bison-chpl-lib.cpp"
    break;

  case 313: /* class_tag: TCLASS  */
#line 2154 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8057 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_tag: TRECORD  */
#line 2155 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8063 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_tag: TUNION  */
#line 2156 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8069 "bison-chpl-lib.cpp"
    break;

  case 316: /* opt_inherit: %empty  */
#line 2160 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8075 "bison-chpl-lib.cpp"
    break;

  case 317: /* opt_inherit: TCOLON expr_ls  */
#line 2161 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8081 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_level_stmt_ls: %empty  */
#line 2165 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8090 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2170 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8098 "bison-chpl-lib.cpp"
    break;

  case 320: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2174 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8106 "bison-chpl-lib.cpp"
    break;

  case 321: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2178 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8114 "bison-chpl-lib.cpp"
    break;

  case 322: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2182 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8122 "bison-chpl-lib.cpp"
    break;

  case 323: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
#line 2190 "chpl.ypp"
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
#line 8145 "bison-chpl-lib.cpp"
    break;

  case 324: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
#line 2209 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8159 "bison-chpl-lib.cpp"
    break;

  case 325: /* enum_header: TENUM ident_def  */
#line 2222 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8167 "bison-chpl-lib.cpp"
    break;

  case 326: /* enum_ls: enum_item  */
#line 2229 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8176 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_ls: enum_ls TCOMMA  */
#line 2234 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8186 "bison-chpl-lib.cpp"
    break;

  case 328: /* $@5: %empty  */
#line 2240 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8195 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2245 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8204 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2250 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8213 "bison-chpl-lib.cpp"
    break;

  case 331: /* $@6: %empty  */
#line 2255 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8222 "bison-chpl-lib.cpp"
    break;

  case 332: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2260 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8231 "bison-chpl-lib.cpp"
    break;

  case 333: /* enum_item: ident_def  */
#line 2270 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8242 "bison-chpl-lib.cpp"
    break;

  case 334: /* enum_item: ident_def TASSIGN expr  */
#line 2277 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8255 "bison-chpl-lib.cpp"
    break;

  case 335: /* lambda_decl_start: TLAMBDA  */
#line 2289 "chpl.ypp"
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
#line 8271 "bison-chpl-lib.cpp"
    break;

  case 336: /* $@7: %empty  */
#line 2306 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8281 "bison-chpl-lib.cpp"
    break;

  case 337: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2312 "chpl.ypp"
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
#line 8303 "bison-chpl-lib.cpp"
    break;

  case 339: /* linkage_spec: linkage_spec_empty  */
#line 2335 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8309 "bison-chpl-lib.cpp"
    break;

  case 340: /* linkage_spec: TINLINE  */
#line 2336 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8316 "bison-chpl-lib.cpp"
    break;

  case 341: /* linkage_spec: TOVERRIDE  */
#line 2338 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8323 "bison-chpl-lib.cpp"
    break;

  case 342: /* opt_fn_type_formal_ls: %empty  */
#line 2343 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8329 "bison-chpl-lib.cpp"
    break;

  case 343: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2344 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8335 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_type_formal_ls: fn_type_formal  */
#line 2348 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8341 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2349 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8347 "bison-chpl-lib.cpp"
    break;

  case 346: /* fn_type_formal: named_formal  */
#line 2354 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8353 "bison-chpl-lib.cpp"
    break;

  case 347: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2357 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8359 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_type_formal: formal_type  */
#line 2359 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8365 "bison-chpl-lib.cpp"
    break;

  case 349: /* opt_fn_type_ret_type: %empty  */
#line 2363 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8371 "bison-chpl-lib.cpp"
    break;

  case 350: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2364 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8377 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2377 "chpl.ypp"
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
#line 8396 "bison-chpl-lib.cpp"
    break;

  case 352: /* $@8: %empty  */
#line 2395 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8406 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2401 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8419 "bison-chpl-lib.cpp"
    break;

  case 354: /* $@9: %empty  */
#line 2410 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8429 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2416 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8442 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2428 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8450 "bison-chpl-lib.cpp"
    break;

  case 357: /* $@10: %empty  */
#line 2437 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8458 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2441 "chpl.ypp"
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
#line 8480 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2462 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8492 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2470 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8504 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2478 "chpl.ypp"
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
#line 8519 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2489 "chpl.ypp"
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
#line 8534 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2500 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8544 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2509 "chpl.ypp"
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
#line 8560 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2524 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8566 "bison-chpl-lib.cpp"
    break;

  case 369: /* fn_ident: ident_def TBANG  */
#line 2531 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8576 "bison-chpl-lib.cpp"
    break;

  case 409: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2588 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8582 "bison-chpl-lib.cpp"
    break;

  case 410: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2589 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8588 "bison-chpl-lib.cpp"
    break;

  case 411: /* opt_formal_ls: %empty  */
#line 2593 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8594 "bison-chpl-lib.cpp"
    break;

  case 412: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2594 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8600 "bison-chpl-lib.cpp"
    break;

  case 413: /* req_formal_ls: TLP TRP  */
#line 2598 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8606 "bison-chpl-lib.cpp"
    break;

  case 414: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2599 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8612 "bison-chpl-lib.cpp"
    break;

  case 415: /* formal_ls_inner: formal  */
#line 2603 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8618 "bison-chpl-lib.cpp"
    break;

  case 416: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2604 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8624 "bison-chpl-lib.cpp"
    break;

  case 417: /* formal_ls: %empty  */
#line 2608 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8630 "bison-chpl-lib.cpp"
    break;

  case 418: /* formal_ls: formal_ls_inner  */
#line 2609 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8636 "bison-chpl-lib.cpp"
    break;

  case 420: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2618 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8644 "bison-chpl-lib.cpp"
    break;

  case 421: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2623 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8652 "bison-chpl-lib.cpp"
    break;

  case 422: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2628 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8660 "bison-chpl-lib.cpp"
    break;

  case 423: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2633 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8668 "bison-chpl-lib.cpp"
    break;

  case 424: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2638 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8676 "bison-chpl-lib.cpp"
    break;

  case 425: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2643 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8684 "bison-chpl-lib.cpp"
    break;

  case 426: /* opt_formal_intent_tag: %empty  */
#line 2649 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8693 "bison-chpl-lib.cpp"
    break;

  case 427: /* opt_formal_intent_tag: required_intent_tag  */
#line 2654 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8702 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TIN  */
#line 2661 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8708 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TINOUT  */
#line 2662 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8714 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TOUT  */
#line 2663 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8720 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TCONST TIN  */
#line 2664 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8726 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TCONST TREF  */
#line 2665 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 8732 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TCONST  */
#line 2666 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 8738 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TPARAM  */
#line 2667 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 8744 "bison-chpl-lib.cpp"
    break;

  case 435: /* required_intent_tag: TREF  */
#line 2668 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 8750 "bison-chpl-lib.cpp"
    break;

  case 436: /* required_intent_tag: TTYPE  */
#line 2669 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 8756 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: %empty  */
#line 2673 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8762 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TPARAM  */
#line 2674 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 8768 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TREF  */
#line 2675 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 8774 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TCONST TREF  */
#line 2676 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 8780 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TCONST  */
#line 2677 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 8786 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TTYPE  */
#line 2678 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 8792 "bison-chpl-lib.cpp"
    break;

  case 443: /* proc_iter_or_op: TPROC  */
#line 2682 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8798 "bison-chpl-lib.cpp"
    break;

  case 444: /* proc_iter_or_op: TITER  */
#line 2683 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8804 "bison-chpl-lib.cpp"
    break;

  case 445: /* proc_iter_or_op: TOPERATOR  */
#line 2684 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8810 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_ret_tag: %empty  */
#line 2688 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 8816 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_ret_tag: TOUT  */
#line 2689 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 8822 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_ret_tag: TCONST  */
#line 2690 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 8828 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: TCONST TREF  */
#line 2691 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 8834 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TREF  */
#line 2692 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 8840 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TPARAM  */
#line 2693 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 8846 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TTYPE  */
#line 2694 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 8852 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_throws_error: %empty  */
#line 2698 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8858 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_throws_error: TTHROWS  */
#line 2699 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8864 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_function_body_stmt: TSEMI  */
#line 2702 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8870 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_function_body_stmt: function_body_stmt  */
#line 2703 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8876 "bison-chpl-lib.cpp"
    break;

  case 457: /* function_body_stmt: block_stmt_body  */
#line 2707 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8882 "bison-chpl-lib.cpp"
    break;

  case 458: /* function_body_stmt: TDO toplevel_stmt  */
#line 2708 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 8888 "bison-chpl-lib.cpp"
    break;

  case 459: /* query_expr: TQUERIEDIDENT  */
#line 2712 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8894 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_lifetime_where: %empty  */
#line 2717 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8900 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_lifetime_where: TWHERE expr  */
#line 2719 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8906 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2721 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8912 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2723 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 8918 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2725 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 8924 "bison-chpl-lib.cpp"
    break;

  case 465: /* lifetime_components_expr: lifetime_expr  */
#line 2730 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8930 "bison-chpl-lib.cpp"
    break;

  case 466: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2732 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8936 "bison-chpl-lib.cpp"
    break;

  case 467: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2737 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8942 "bison-chpl-lib.cpp"
    break;

  case 468: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2739 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8948 "bison-chpl-lib.cpp"
    break;

  case 469: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2741 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8954 "bison-chpl-lib.cpp"
    break;

  case 470: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2743 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8960 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2745 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8966 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2747 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8972 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2749 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 8978 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_ident: TIDENT  */
#line 2753 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8984 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_ident: TINIT  */
#line 2754 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8990 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_ident: TTHIS  */
#line 2755 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8996 "bison-chpl-lib.cpp"
    break;

  case 477: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2760 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9005 "bison-chpl-lib.cpp"
    break;

  case 478: /* type_alias_decl_stmt_start: TTYPE  */
#line 2769 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9013 "bison-chpl-lib.cpp"
    break;

  case 479: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2773 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9022 "bison-chpl-lib.cpp"
    break;

  case 480: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2781 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 481: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2785 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9038 "bison-chpl-lib.cpp"
    break;

  case 482: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2792 "chpl.ypp"
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
#line 9062 "bison-chpl-lib.cpp"
    break;

  case 483: /* opt_init_type: %empty  */
#line 2814 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9068 "bison-chpl-lib.cpp"
    break;

  case 484: /* opt_init_type: TASSIGN expr  */
#line 2816 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9074 "bison-chpl-lib.cpp"
    break;

  case 485: /* var_decl_type: TPARAM  */
#line 2820 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9080 "bison-chpl-lib.cpp"
    break;

  case 486: /* var_decl_type: TCONST TREF  */
#line 2821 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9086 "bison-chpl-lib.cpp"
    break;

  case 487: /* var_decl_type: TREF  */
#line 2822 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9092 "bison-chpl-lib.cpp"
    break;

  case 488: /* var_decl_type: TCONST  */
#line 2823 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9098 "bison-chpl-lib.cpp"
    break;

  case 489: /* var_decl_type: TVAR  */
#line 2824 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9104 "bison-chpl-lib.cpp"
    break;

  case 490: /* $@11: %empty  */
#line 2829 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9113 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2833 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9122 "bison-chpl-lib.cpp"
    break;

  case 492: /* $@12: %empty  */
#line 2838 "chpl.ypp"
  {
    // Use a mid-rule action to preserve the 'on' expr
    context->noteVarDestinationExpr((yyvsp[0].expr));
  }
#line 9131 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_stmt: TON expr $@12 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2843 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9140 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2848 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9149 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_stmt: var_decl_type error TSEMI  */
#line 2853 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    context->resetDeclStateOnError();
  }
#line 9158 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2861 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9166 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2865 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9174 "bison-chpl-lib.cpp"
    break;

  case 498: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2872 "chpl.ypp"
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
#line 9198 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2892 "chpl.ypp"
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
#line 9218 "bison-chpl-lib.cpp"
    break;

  case 500: /* tuple_var_decl_component: ident_def  */
#line 2911 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9226 "bison-chpl-lib.cpp"
    break;

  case 501: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2915 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9234 "bison-chpl-lib.cpp"
    break;

  case 502: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2922 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9240 "bison-chpl-lib.cpp"
    break;

  case 503: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2924 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9246 "bison-chpl-lib.cpp"
    break;

  case 504: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2926 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9252 "bison-chpl-lib.cpp"
    break;

  case 505: /* opt_init_expr: %empty  */
#line 2932 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9258 "bison-chpl-lib.cpp"
    break;

  case 506: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2933 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9264 "bison-chpl-lib.cpp"
    break;

  case 507: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2934 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9270 "bison-chpl-lib.cpp"
    break;

  case 508: /* formal_or_ret_type_expr: expr  */
#line 2938 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9276 "bison-chpl-lib.cpp"
    break;

  case 509: /* ret_type: formal_or_ret_type_expr  */
#line 2942 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9282 "bison-chpl-lib.cpp"
    break;

  case 510: /* ret_type: reserved_type_ident_use  */
#line 2943 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9288 "bison-chpl-lib.cpp"
    break;

  case 511: /* ret_type: error  */
#line 2944 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9294 "bison-chpl-lib.cpp"
    break;

  case 512: /* colon_ret_type: TCOLON ret_type  */
#line 2948 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9300 "bison-chpl-lib.cpp"
    break;

  case 513: /* colon_ret_type: error  */
#line 2949 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9306 "bison-chpl-lib.cpp"
    break;

  case 514: /* opt_ret_type: %empty  */
#line 2953 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9312 "bison-chpl-lib.cpp"
    break;

  case 516: /* opt_type: %empty  */
#line 2958 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9318 "bison-chpl-lib.cpp"
    break;

  case 517: /* opt_type: TCOLON expr  */
#line 2959 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9324 "bison-chpl-lib.cpp"
    break;

  case 518: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2960 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9330 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_type: error  */
#line 2961 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9336 "bison-chpl-lib.cpp"
    break;

  case 520: /* formal_type: formal_or_ret_type_expr  */
#line 2965 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9342 "bison-chpl-lib.cpp"
    break;

  case 521: /* formal_type: reserved_type_ident_use  */
#line 2966 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9348 "bison-chpl-lib.cpp"
    break;

  case 522: /* colon_formal_type: TCOLON formal_type  */
#line 2970 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9354 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_colon_formal_type: %empty  */
#line 2974 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9360 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_colon_formal_type: colon_formal_type  */
#line 2975 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9366 "bison-chpl-lib.cpp"
    break;

  case 525: /* expr_ls: expr  */
#line 2981 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9372 "bison-chpl-lib.cpp"
    break;

  case 526: /* expr_ls: expr_ls TCOMMA expr  */
#line 2982 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9378 "bison-chpl-lib.cpp"
    break;

  case 527: /* tuple_component: opt_try_expr  */
#line 2986 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9384 "bison-chpl-lib.cpp"
    break;

  case 528: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2991 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9392 "bison-chpl-lib.cpp"
    break;

  case 529: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2995 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9400 "bison-chpl-lib.cpp"
    break;

  case 530: /* opt_actual_ls: %empty  */
#line 3001 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9406 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_actual_ls: actual_ls  */
#line 3002 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9412 "bison-chpl-lib.cpp"
    break;

  case 532: /* actual_ls: actual_expr  */
#line 3007 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9421 "bison-chpl-lib.cpp"
    break;

  case 533: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3012 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9431 "bison-chpl-lib.cpp"
    break;

  case 534: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3020 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9437 "bison-chpl-lib.cpp"
    break;

  case 535: /* actual_expr: opt_try_expr  */
#line 3021 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9443 "bison-chpl-lib.cpp"
    break;

  case 536: /* ident_expr: ident_use  */
#line 3025 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9449 "bison-chpl-lib.cpp"
    break;

  case 537: /* ident_expr: scalar_type  */
#line 3026 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9455 "bison-chpl-lib.cpp"
    break;

  case 543: /* sub_type_level_expr: TSINGLE expr  */
#line 3039 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9461 "bison-chpl-lib.cpp"
    break;

  case 544: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3041 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9467 "bison-chpl-lib.cpp"
    break;

  case 545: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3043 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9473 "bison-chpl-lib.cpp"
    break;

  case 546: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3045 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9479 "bison-chpl-lib.cpp"
    break;

  case 547: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3047 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9489 "bison-chpl-lib.cpp"
    break;

  case 548: /* sub_type_level_expr: TATOMIC expr  */
#line 3053 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9495 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TSYNC expr  */
#line 3055 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9501 "bison-chpl-lib.cpp"
    break;

  case 550: /* sub_type_level_expr: TOWNED  */
#line 3058 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9507 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TOWNED expr  */
#line 3060 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9513 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TUNMANAGED  */
#line 3062 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9519 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3064 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9525 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TSHARED  */
#line 3066 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9531 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TSHARED expr  */
#line 3068 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9537 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TBORROWED  */
#line 3070 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9543 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TBORROWED expr  */
#line 3072 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9549 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TCLASS  */
#line 3074 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9555 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TRECORD  */
#line 3076 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9561 "bison-chpl-lib.cpp"
    break;

  case 560: /* for_expr: no_loop_attributes for_expr_base  */
#line 3080 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9567 "bison-chpl-lib.cpp"
    break;

  case 561: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3085 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9575 "bison-chpl-lib.cpp"
    break;

  case 562: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3089 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9583 "bison-chpl-lib.cpp"
    break;

  case 563: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3093 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9591 "bison-chpl-lib.cpp"
    break;

  case 564: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3097 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9604 "bison-chpl-lib.cpp"
    break;

  case 565: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3106 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9617 "bison-chpl-lib.cpp"
    break;

  case 566: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3115 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9630 "bison-chpl-lib.cpp"
    break;

  case 567: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3126 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9636 "bison-chpl-lib.cpp"
    break;

  case 568: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3131 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9644 "bison-chpl-lib.cpp"
    break;

  case 569: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3135 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9652 "bison-chpl-lib.cpp"
    break;

  case 570: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3139 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9660 "bison-chpl-lib.cpp"
    break;

  case 571: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3143 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9668 "bison-chpl-lib.cpp"
    break;

  case 572: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3147 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9676 "bison-chpl-lib.cpp"
    break;

  case 573: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3151 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9684 "bison-chpl-lib.cpp"
    break;

  case 574: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3155 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9692 "bison-chpl-lib.cpp"
    break;

  case 575: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3162 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9706 "bison-chpl-lib.cpp"
    break;

  case 576: /* nil_expr: TNIL  */
#line 3179 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9712 "bison-chpl-lib.cpp"
    break;

  case 584: /* opt_task_intent_ls: %empty  */
#line 3197 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9718 "bison-chpl-lib.cpp"
    break;

  case 585: /* opt_task_intent_ls: task_intent_clause  */
#line 3198 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9724 "bison-chpl-lib.cpp"
    break;

  case 586: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3203 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9732 "bison-chpl-lib.cpp"
    break;

  case 587: /* task_intent_clause: TWITH TLP TRP  */
#line 3207 "chpl.ypp"
  {
    auto err = context->syntax((yyloc), "'with' clause cannot be empty.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-2]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9743 "bison-chpl-lib.cpp"
    break;

  case 588: /* task_intent_clause: TWITH TLP error TRP  */
#line 3214 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[-2]), "invalid intent expression in 'with' clause.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9754 "bison-chpl-lib.cpp"
    break;

  case 589: /* task_intent_clause: TWITH error  */
#line 3221 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[0]), "missing parentheses after 'with' clause.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-1]), dummyLoc, dummyLoc, dummyLoc, exprs);
  }
#line 9765 "bison-chpl-lib.cpp"
    break;

  case 590: /* task_intent_ls: intent_expr  */
#line 3230 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9771 "bison-chpl-lib.cpp"
    break;

  case 591: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3231 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9777 "bison-chpl-lib.cpp"
    break;

  case 592: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3236 "chpl.ypp"
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
#line 9796 "bison-chpl-lib.cpp"
    break;

  case 593: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3251 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9804 "bison-chpl-lib.cpp"
    break;

  case 594: /* intent_expr: expr TREDUCE ident_expr  */
#line 3255 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9812 "bison-chpl-lib.cpp"
    break;

  case 595: /* task_var_prefix: TCONST  */
#line 3261 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 9818 "bison-chpl-lib.cpp"
    break;

  case 596: /* task_var_prefix: TIN  */
#line 3262 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 9824 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_var_prefix: TCONST TIN  */
#line 3263 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 9830 "bison-chpl-lib.cpp"
    break;

  case 598: /* task_var_prefix: TREF  */
#line 3264 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 9836 "bison-chpl-lib.cpp"
    break;

  case 599: /* task_var_prefix: TCONST TREF  */
#line 3265 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 9842 "bison-chpl-lib.cpp"
    break;

  case 600: /* task_var_prefix: TVAR  */
#line 3266 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 9848 "bison-chpl-lib.cpp"
    break;

  case 601: /* new_maybe_decorated: TNEW  */
#line 3271 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9854 "bison-chpl-lib.cpp"
    break;

  case 602: /* new_maybe_decorated: TNEW TOWNED  */
#line 3273 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9860 "bison-chpl-lib.cpp"
    break;

  case 603: /* new_maybe_decorated: TNEW TSHARED  */
#line 3275 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9866 "bison-chpl-lib.cpp"
    break;

  case 604: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3277 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9872 "bison-chpl-lib.cpp"
    break;

  case 605: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3279 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9878 "bison-chpl-lib.cpp"
    break;

  case 606: /* new_expr: new_maybe_decorated expr  */
#line 3285 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 9886 "bison-chpl-lib.cpp"
    break;

  case 607: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3292 "chpl.ypp"
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
#line 9902 "bison-chpl-lib.cpp"
    break;

  case 608: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
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
    (yyval.expr) = context->buildNewExpr((yyloc), New::SHARED, call.release());
  }
#line 9918 "bison-chpl-lib.cpp"
    break;

  case 609: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3316 "chpl.ypp"
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
#line 9935 "bison-chpl-lib.cpp"
    break;

  case 610: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3329 "chpl.ypp"
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
#line 9953 "bison-chpl-lib.cpp"
    break;

  case 611: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3346 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9961 "bison-chpl-lib.cpp"
    break;

  case 612: /* range_literal_expr: expr TDOTDOT expr  */
#line 3353 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 9970 "bison-chpl-lib.cpp"
    break;

  case 613: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3358 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 9979 "bison-chpl-lib.cpp"
    break;

  case 614: /* range_literal_expr: expr TDOTDOT  */
#line 3363 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 9988 "bison-chpl-lib.cpp"
    break;

  case 615: /* range_literal_expr: TDOTDOT expr  */
#line 3368 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 9998 "bison-chpl-lib.cpp"
    break;

  case 616: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3374 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10008 "bison-chpl-lib.cpp"
    break;

  case 617: /* range_literal_expr: TDOTDOT  */
#line 3380 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10018 "bison-chpl-lib.cpp"
    break;

  case 618: /* cast_expr: expr TCOLON expr  */
#line 3410 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10026 "bison-chpl-lib.cpp"
    break;

  case 619: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3417 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10034 "bison-chpl-lib.cpp"
    break;

  case 620: /* super_expr: fn_expr  */
#line 3423 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10040 "bison-chpl-lib.cpp"
    break;

  case 624: /* expr: sub_type_level_expr TQUESTION  */
#line 3432 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10046 "bison-chpl-lib.cpp"
    break;

  case 625: /* expr: TQUESTION  */
#line 3434 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10052 "bison-chpl-lib.cpp"
    break;

  case 629: /* expr: fn_type  */
#line 3439 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10058 "bison-chpl-lib.cpp"
    break;

  case 639: /* opt_expr: %empty  */
#line 3453 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10064 "bison-chpl-lib.cpp"
    break;

  case 640: /* opt_expr: expr  */
#line 3454 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10070 "bison-chpl-lib.cpp"
    break;

  case 641: /* opt_try_expr: TTRY expr  */
#line 3458 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10076 "bison-chpl-lib.cpp"
    break;

  case 642: /* opt_try_expr: TTRYBANG expr  */
#line 3459 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10082 "bison-chpl-lib.cpp"
    break;

  case 643: /* opt_try_expr: super_expr  */
#line 3460 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10088 "bison-chpl-lib.cpp"
    break;

  case 649: /* call_base_expr: expr TBANG  */
#line 3477 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10096 "bison-chpl-lib.cpp"
    break;

  case 650: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3480 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10102 "bison-chpl-lib.cpp"
    break;

  case 653: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3487 "chpl.ypp"
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
#line 10118 "bison-chpl-lib.cpp"
    break;

  case 654: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3499 "chpl.ypp"
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
#line 10134 "bison-chpl-lib.cpp"
    break;

  case 655: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3511 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10142 "bison-chpl-lib.cpp"
    break;

  case 656: /* dot_expr: expr TDOT ident_use  */
#line 3518 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10148 "bison-chpl-lib.cpp"
    break;

  case 657: /* dot_expr: expr TDOT TTYPE  */
#line 3520 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10154 "bison-chpl-lib.cpp"
    break;

  case 658: /* dot_expr: expr TDOT TDOMAIN  */
#line 3522 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10160 "bison-chpl-lib.cpp"
    break;

  case 659: /* dot_expr: expr TDOT TLOCALE  */
#line 3524 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10166 "bison-chpl-lib.cpp"
    break;

  case 660: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3526 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10172 "bison-chpl-lib.cpp"
    break;

  case 661: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3528 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10178 "bison-chpl-lib.cpp"
    break;

  case 662: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3537 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenthLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10188 "bison-chpl-lib.cpp"
    break;

  case 663: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3543 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10196 "bison-chpl-lib.cpp"
    break;

  case 664: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3547 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10204 "bison-chpl-lib.cpp"
    break;

  case 665: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3551 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10212 "bison-chpl-lib.cpp"
    break;

  case 666: /* bool_literal: TFALSE  */
#line 3557 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10218 "bison-chpl-lib.cpp"
    break;

  case 667: /* bool_literal: TTRUE  */
#line 3558 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10224 "bison-chpl-lib.cpp"
    break;

  case 668: /* str_bytes_literal: STRINGLITERAL  */
#line 3562 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10230 "bison-chpl-lib.cpp"
    break;

  case 669: /* str_bytes_literal: BYTESLITERAL  */
#line 3563 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10236 "bison-chpl-lib.cpp"
    break;

  case 672: /* literal_expr: INTLITERAL  */
#line 3569 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10242 "bison-chpl-lib.cpp"
    break;

  case 673: /* literal_expr: REALLITERAL  */
#line 3570 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10248 "bison-chpl-lib.cpp"
    break;

  case 674: /* literal_expr: IMAGLITERAL  */
#line 3571 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10254 "bison-chpl-lib.cpp"
    break;

  case 675: /* literal_expr: CSTRINGLITERAL  */
#line 3572 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10260 "bison-chpl-lib.cpp"
    break;

  case 676: /* literal_expr: TNONE  */
#line 3573 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10266 "bison-chpl-lib.cpp"
    break;

  case 677: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3575 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10275 "bison-chpl-lib.cpp"
    break;

  case 678: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3580 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10284 "bison-chpl-lib.cpp"
    break;

  case 679: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3585 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10293 "bison-chpl-lib.cpp"
    break;

  case 680: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3590 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10302 "bison-chpl-lib.cpp"
    break;

  case 681: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3595 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10311 "bison-chpl-lib.cpp"
    break;

  case 682: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3600 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10320 "bison-chpl-lib.cpp"
    break;

  case 683: /* assoc_expr_ls: expr TALIAS expr  */
#line 3609 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10329 "bison-chpl-lib.cpp"
    break;

  case 684: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3614 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10339 "bison-chpl-lib.cpp"
    break;

  case 685: /* binary_op_expr: expr TPLUS expr  */
#line 3622 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10345 "bison-chpl-lib.cpp"
    break;

  case 686: /* binary_op_expr: expr TMINUS expr  */
#line 3623 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10351 "bison-chpl-lib.cpp"
    break;

  case 687: /* binary_op_expr: expr TSTAR expr  */
#line 3624 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10357 "bison-chpl-lib.cpp"
    break;

  case 688: /* binary_op_expr: expr TDIVIDE expr  */
#line 3625 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10363 "bison-chpl-lib.cpp"
    break;

  case 689: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3626 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10369 "bison-chpl-lib.cpp"
    break;

  case 690: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3627 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10375 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TMOD expr  */
#line 3628 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10381 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TEQUAL expr  */
#line 3629 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10387 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3630 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10393 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3631 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10399 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3632 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10405 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TLESS expr  */
#line 3633 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10411 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TGREATER expr  */
#line 3634 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10417 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TBAND expr  */
#line 3635 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10423 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TBOR expr  */
#line 3636 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10429 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TBXOR expr  */
#line 3637 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10435 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TAND expr  */
#line 3638 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10441 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TOR expr  */
#line 3639 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10447 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TEXP expr  */
#line 3640 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10453 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TBY expr  */
#line 3641 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10459 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TALIGN expr  */
#line 3642 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10465 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr THASH expr  */
#line 3643 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10471 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TDMAPPED expr  */
#line 3644 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10477 "bison-chpl-lib.cpp"
    break;

  case 708: /* unary_op_expr: TPLUS expr  */
#line 3648 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10483 "bison-chpl-lib.cpp"
    break;

  case 709: /* unary_op_expr: TMINUS expr  */
#line 3649 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10489 "bison-chpl-lib.cpp"
    break;

  case 710: /* unary_op_expr: TMINUSMINUS expr  */
#line 3650 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10495 "bison-chpl-lib.cpp"
    break;

  case 711: /* unary_op_expr: TPLUSPLUS expr  */
#line 3651 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10501 "bison-chpl-lib.cpp"
    break;

  case 712: /* unary_op_expr: TBANG expr  */
#line 3652 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10507 "bison-chpl-lib.cpp"
    break;

  case 713: /* unary_op_expr: expr TBANG  */
#line 3653 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10515 "bison-chpl-lib.cpp"
    break;

  case 714: /* unary_op_expr: TBNOT expr  */
#line 3656 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10521 "bison-chpl-lib.cpp"
    break;

  case 715: /* reduce_expr: expr TREDUCE expr  */
#line 3661 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10529 "bison-chpl-lib.cpp"
    break;

  case 716: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3665 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10537 "bison-chpl-lib.cpp"
    break;

  case 717: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3669 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10545 "bison-chpl-lib.cpp"
    break;

  case 718: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3673 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10553 "bison-chpl-lib.cpp"
    break;

  case 719: /* scan_expr: expr TSCAN expr  */
#line 3680 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10561 "bison-chpl-lib.cpp"
    break;

  case 720: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3684 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10569 "bison-chpl-lib.cpp"
    break;

  case 721: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3688 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10577 "bison-chpl-lib.cpp"
    break;

  case 722: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3692 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10585 "bison-chpl-lib.cpp"
    break;


#line 10589 "bison-chpl-lib.cpp"

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
