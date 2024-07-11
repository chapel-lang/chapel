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
#define YYLAST   17615

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  182
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  205
/* YYNRULES -- Number of rules.  */
#define YYNRULES  729
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1260

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
    1840,  1845,  1850,  1858,  1874,  1893,  1897,  1904,  1905,  1910,
    1915,  1916,  1917,  1918,  1919,  1920,  1921,  1922,  1923,  1924,
    1925,  1926,  1931,  1936,  1937,  1938,  1939,  1948,  1949,  1953,
    1957,  1961,  1968,  1972,  1976,  1983,  1987,  1991,  1995,  2002,
    2003,  2007,  2011,  2015,  2022,  2036,  2053,  2061,  2066,  2076,
    2077,  2081,  2085,  2090,  2099,  2104,  2108,  2115,  2116,  2120,
    2130,  2136,  2148,  2155,  2156,  2157,  2161,  2162,  2166,  2170,
    2174,  2178,  2182,  2190,  2209,  2222,  2229,  2234,  2241,  2240,
    2250,  2256,  2255,  2270,  2277,  2289,  2307,  2304,  2332,  2336,
    2337,  2339,  2344,  2345,  2349,  2350,  2354,  2357,  2359,  2364,
    2365,  2376,  2396,  2395,  2411,  2410,  2428,  2438,  2435,  2462,
    2470,  2478,  2489,  2500,  2509,  2524,  2525,  2529,  2530,  2531,
    2540,  2541,  2542,  2543,  2544,  2545,  2546,  2547,  2548,  2549,
    2550,  2551,  2552,  2553,  2554,  2555,  2556,  2557,  2558,  2559,
    2560,  2561,  2562,  2563,  2564,  2568,  2569,  2570,  2571,  2572,
    2573,  2574,  2575,  2576,  2577,  2578,  2579,  2584,  2585,  2589,
    2590,  2594,  2595,  2599,  2600,  2604,  2605,  2609,  2610,  2614,
    2618,  2622,  2627,  2632,  2637,  2642,  2650,  2654,  2662,  2663,
    2664,  2665,  2666,  2667,  2668,  2669,  2670,  2674,  2675,  2676,
    2677,  2678,  2679,  2683,  2684,  2685,  2689,  2690,  2691,  2692,
    2693,  2694,  2695,  2699,  2700,  2703,  2704,  2708,  2709,  2713,
    2718,  2719,  2721,  2723,  2725,  2730,  2732,  2737,  2739,  2741,
    2743,  2745,  2747,  2749,  2754,  2755,  2756,  2760,  2769,  2773,
    2781,  2785,  2792,  2815,  2816,  2821,  2822,  2823,  2824,  2825,
    2830,  2829,  2839,  2838,  2848,  2853,  2861,  2865,  2872,  2892,
    2911,  2915,  2922,  2924,  2926,  2933,  2934,  2935,  2939,  2943,
    2944,  2945,  2949,  2950,  2954,  2955,  2959,  2960,  2961,  2962,
    2966,  2967,  2971,  2975,  2976,  2982,  2983,  2987,  2991,  2995,
    3002,  3003,  3007,  3012,  3021,  3022,  3026,  3027,  3034,  3035,
    3036,  3037,  3038,  3039,  3041,  3043,  3045,  3047,  3053,  3055,
    3058,  3060,  3062,  3064,  3066,  3068,  3070,  3072,  3074,  3076,
    3081,  3085,  3089,  3093,  3097,  3106,  3115,  3127,  3131,  3135,
    3139,  3143,  3147,  3151,  3155,  3162,  3180,  3188,  3189,  3190,
    3191,  3192,  3193,  3194,  3198,  3199,  3203,  3210,  3211,  3215,
    3224,  3225,  3229,  3244,  3248,  3255,  3256,  3257,  3258,  3259,
    3260,  3264,  3266,  3268,  3270,  3272,  3278,  3285,  3297,  3309,
    3322,  3339,  3346,  3351,  3356,  3361,  3367,  3373,  3403,  3410,
    3417,  3418,  3422,  3424,  3425,  3427,  3429,  3430,  3431,  3432,
    3435,  3436,  3437,  3438,  3439,  3440,  3441,  3442,  3443,  3447,
    3448,  3452,  3453,  3454,  3458,  3459,  3460,  3461,  3470,  3471,
    3474,  3475,  3476,  3480,  3492,  3504,  3511,  3513,  3515,  3517,
    3519,  3521,  3530,  3536,  3540,  3544,  3551,  3552,  3556,  3557,
    3561,  3562,  3563,  3564,  3565,  3566,  3567,  3568,  3573,  3578,
    3583,  3588,  3593,  3602,  3607,  3616,  3617,  3618,  3619,  3620,
    3621,  3622,  3623,  3624,  3625,  3626,  3627,  3628,  3629,  3630,
    3631,  3632,  3633,  3634,  3635,  3636,  3637,  3638,  3642,  3643,
    3644,  3645,  3646,  3647,  3650,  3654,  3658,  3662,  3666,  3673,
    3677,  3681,  3685,  3693,  3694,  3695,  3696,  3697,  3698,  3699
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

#define YYTABLE_NINF (-730)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -900,    75,  3856,  -900,   -81,    69,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900, 11864,     0,    89,    73, 13725,   101,
   17386,     0,   195,    24,   141,    89,  5430, 11864,    99, 17442,
    -900,   308,   206,  -900,  9949,   258,   426,   175,  -900,   301,
     268, 17442, 17442, 17442,  -900,  2279, 10817,   331, 11864, 11864,
      64,  -900,   350,   390, 11864,  -900, 13725,  -900, 11864,   463,
     312,   251,   295, 13075,   448, 17498,  -900, 11864,  8043, 11864,
   10817, 13725, 11864,   414,   475,   379,  5430,   511, 11864,   514,
    7005,  7005,  -900,   517,  -900, 13725,  -900,   522, 10122,  -900,
   11864,  -900, 11864,  -900,  -900, 13552, 11864,  -900, 10295,  -900,
    -900,  -900,  4374,  8216, 11864,  -900,  5078,  -900,  -900,  -900,
    -900,  3200,   584,  -900,   454,   441,  -900,   181,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  7178,  -900,  7351,  -900,  -900,    27,  -900,  -900,  -900,
     559,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,   479,
   17442,  -900,   459,   460,   424,  -900,   328,  -900,  -900,  -900,
     230,   259,  -900,  -900, 17442,  1760,  -900,   480,   486,  -900,
    -900,  -900,   488,   504, 11864,   506,   513,  -900,  -900,  -900,
   16893,   701,   223,   516,   521,  -900,  -900,   250,  -900,  -900,
    -900,  -900,  -900,    76,  -900,  -900,  -900, 11864,  -900, 17442,
    -900, 11864, 11864,    22,   588,   411,  -900,  -900,  -900,  -900,
   16893,   449,  -900,  -900,   487,  5430,  -900,  -900,  -900,   525,
      -4,   500,  -900,   201,  -900,   527,  -900,   187, 16893,  8389,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900, 17442,
   17442,   -22,  3976,  -900,  -900,   598,  8389,   529,   512,   465,
    5430,  2708,    94,     7,  -900,  5430,  -900,  -900, 15006,  -900,
      11, 15537,   245,  -900,   515,   523,  -900, 15006,    -4,   245,
    -900,  8389,  1818,  1818,    28,    15,  -900,    74,  -900,  -900,
    -900,  -900,  7697,  7697,  -900,   490,  -900,   530,   551,  -900,
    -900,  2824,   554,  -900,  -900, 16893,   302, 11864, 11864,  -900,
     -19,  -900,  -900, 16893,   537, 15577,  -900, 15006,    -4, 16893,
     533,  8389,  -900, 16893, 15617, 11864,  -900,  -900,  -900,  -900,
    -900,    -4,   542,   412,   412,   583,   245,   245,   139,  -900,
    -900,  4550,   149, 11864,   561,   118,   536,   583,   703,  -900,
    -900, 17442,  -900, 11864,  -900,  4374,  -900,    56,  -900,   598,
    -900,  -900,   710,   544,  4726, 11864,  -900, 11864,   653,  -900,
    -900,  3483,    35,   479, 16893,   605, 11864,  5430,   644,  -900,
    -900,  9949, 10468, 10990,  -900,  -900,  -900, 17442, 11864,   560,
    -900, 17182,   202,   230,  -900,  -900,  -900,  -900,   646,  -900,
    -900,  -900,  -900,    34,   649,  -900,  -900,  -900, 13384,   610,
     311,  -900,   576,   318,   469,  -900,   603, 11864, 11864, 11864,
   10990, 10990, 11864,   477, 11864, 11864, 11864, 11864, 11864,   313,
   13552, 11864, 11864, 11864, 11864, 11864, 11864, 11864, 11864, 11864,
   11864, 11864, 11864, 11864, 11864, 11864, 11864,   675,  -900,  -900,
    -900,  -900,  -900, 10122, 10122,  -900,  -900,  -900,  -900, 10122,
    -900,  -900, 10122, 10122,  8389,  8562, 10990, 10990, 15697,   575,
    -900, 11163, 11864, 17442,  7870,  -900,  -900,  -900,  2279,  -900,
   11864,  -900,   628,   581,   608,  -900,  -900,   633,   634,  5430,
     732,  5430,  -900,   740, 11864,   709,   604,  -900, 10122, 17442,
    -900,  -900,  2708,  -900,  -900,   135,  -900, 12037,   655, 11864,
    2279,  -900,  -900, 11864,  -900, 17238, 11864, 11864,  -900,   201,
     609,  -900,  -900,  -900,  -900, 17442,  -900,   230, 13248,  2708,
     632, 12037,  -900, 16893, 16893,  -900,   500,  -900,    52,  -900,
    8389,   611,  -900, 15006,   760,   760,  -900,  -900,  -900,  -900,
   11339,  -900, 15737,  8738,  8911,  -900,  -900,  -900, 16893, 10990,
   10990,  -900,   454,  9084,  -900,   334,  -900,  4902,  -900,   322,
   15777,   330, 14767, 17442,  6829,  6656,  -900,   479,   615,  -900,
     201,  -900, 15087,   667, 17442,    78, 15006,   621, 13842,   -37,
     113, 15857,   126,    77, 14597,   303,   640,  1315,   624, 17330,
     669,   256,  -900,  -900,   140,  -900,  -900,    47,  -900,  -900,
    6483,  -900,   690,  -900,   625,   650,  -900,   651,  -900,   652,
     656,   657,   658,   659,  -900,   661,   664,   665,   666,   668,
     670,   671,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900, 11864,   663,   673,   625,  -900,   625,
    -900, 11864,  -900, 17442,  -900,  -900,  -900, 16973, 16973,   438,
    -900,   438,  -900,   438, 12946,   905,   813,   997,    -4,   412,
    -900,   494,  -900,  -900,  -900,  -900,  -900,   583, 13119,   438,
    2116,  2116, 16973,  2116,  2116,   623,   412, 13119, 17013,   623,
     245,   245,   412,   583,   637,   660,   676,   678,   680,   682,
     677,   674,  -900,   438,  -900,   438, 11864, 10122,     3, 15166,
     626,   214,  -900,  -900,  -900,   151,  -900,   622, 16933,   265,
     332, 16893, 10122,  -900, 10122, 11864, 11864,   790,  5430,   814,
   15936,  5430, 15246, 17442,  -900,   185,  -900,   190,  -900,   217,
    2708,    94,  -900, 16893,  9257,  -900, 16893,  -900,  -900,  -900,
   17442, 15977, 16017,  2279,  -900,   632,   705,  -900,   235, 12037,
    -900,   267,  -900, 11864,  -900,   684,     1,   683,  -900,  1898,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,   143, 14676,
    -900,  -900,   269,  -900,  -900,  -900,  -900, 11864,  -900,    63,
   13948, 11864,  -900, 11864,  6829,  6656,  -900,  9430,   474,  9603,
     505,   518,  9776,  7524,   519,   353,  -900,  -900,  -900, 16097,
     712,   692,   691,  -900, 17442,  2279, 10990,    78, 11864,   815,
    -900,  -900, 11864, 16893,   688,  5430,   689, 10990, 11512,  4198,
     694, 11864, 11688,  -900,  -900,    78,  5606, 10990,  -900,    78,
    -900, 10122,   695,  1417,  -900,  -900, 11864,    45,  -900,   555,
    -900,   690,  -900,  -900,  -900,  -900,  -900,   176,   275,  -900,
   16137,  -900, 14108,  -900,  -900, 16893,  -900,   698,   704,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,   272, 10990, 13842,
   12210, 10990, 10122,  -900,  -900,  7870,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,    38, 10990,
   10990,  -900,  -900,  -900, 16893, 16893,  5430,  -900,  5430, 11864,
    -900,  5430,   829,   711,   713, 17442,   500,  -900,  -900,   655,
    -900,  -900,  -900,   714,   715,   367,   267, 11864,   690,   632,
    -900, 10644,  -900,  -900, 16893,  -900,  5430, 11864,  -900,  -900,
    -900, 17442,  -900,   735,   500, 10990,  5430, 10990, 10122,  -900,
    -900,  -900,  -900,  -900,   494,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  6829,
    -900,  -900, 16183, 15338,   381,   717,  6829,  -900, 11864,  -900,
    -900,  1558,   382,   387,     0, 16289,  -900, 16335, 11864,   278,
    7870,  -900,  5430,   -13, 15417,  -900,  -900, 16893,  5430, 16893,
    -900, 16381,  -900,  9949, 16893,    82, 14846,  -900,   286,  -900,
    -900,  -900,  -900,  2102,  2438,  -900, 16893, 17442,   584,  -900,
     176,    90, 11864,  -900,   736,   716,  -900,   625,   625,  -900,
    -900,  -900,   718, 16487, 11864,   852, 16533,   287,  -900,   655,
     293,   297,  -900,  -900, 16893,   851,  5430,  -900,  -900,  -900,
    -900,  -900,  8389,  8389,  -900,  -900,  -900,  -900,  -900,   267,
   16893,  -900,    81,    12,   719, 11864,  -900,    51, 15497,    77,
   14927,  -900,   317, 11864,  6829,  6656,  -900,  -900,  -900, 16573,
   16893,  1986,  -900,  -900,  -900,    78,    78,  -900, 15006,  -900,
     300,  -900,  -900,  5782,   720,  5958,   721,  -900, 11864, 14192,
    6134,  -900,    78,  6310,  -900,    78,   723,  2438,  -900,  -900,
    -900, 17330,  -900,  -900,  -900,    85,  -900,   -12,  -900,   405,
   16693,    80,  -900,  -900,  -900, 12383, 12556, 16653, 12729, 12902,
    -900,  -900,  5430,  -900,   725,   726,  -900,  -900,  5430,  -900,
     500, 16893,  5430,  5430,  -900, 16893,   391,   729, 11864,  -900,
    -900,  -900,  -900,  7870,  -900,  9949,  -900, 16893,  5430,  9949,
    -900, 16893,  5430, 16893,  5430,  9949, 16893,  -900,  9949, 16893,
    -900,  -900,  -900,  -900,    84,  -900, 11864,    90,    85,    85,
      85,    85,    85,    85,    90,  5254,  -900,  -900,  -900,  -900,
   11864, 11864, 11864, 11864, 11864,  -900,   727,   737,   667,  -900,
    -900,  -900, 16893,  -900, 14271,  -900, 14352,  -900, 15936, 14431,
   14512,  -900, 16893,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
     743,  -900, 16733, 16773, 16813, 16853,  -900,  -900,  5430,  5430,
    5430,  5430, 11864, 11864, 11864, 11864, 15936, 15936, 15936, 15936
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
     214,   211,   211,     0,   211,    37,    25,    38,     0,    26,
     211,    39,   536,     0,   531,   532,   535,     0,     0,     0,
     246,     0,   395,   242,   211,     0,     0,    24,   211,     0,
     256,    27,     0,   500,   502,     0,   519,   211,   505,   211,
       0,   178,    40,   211,   309,     0,   211,   211,    42,     0,
       0,   187,   185,   431,   432,     0,   427,   446,   426,     0,
     523,   211,   122,   641,   642,   354,     0,   181,     0,    43,
     211,     0,   296,     0,   287,   288,    28,    76,    75,    79,
     211,   677,     0,   211,   211,   664,   662,     8,   492,   211,
     211,   212,    56,   211,    54,     0,    65,     0,   120,     0,
     115,     0,    86,     0,   211,   211,   195,   316,     0,   208,
       0,   205,   584,     0,     0,     0,     0,     0,   568,     0,
       0,   525,     0,     0,     0,   283,   317,     0,     0,     0,
     333,     0,   326,   413,     0,   415,   419,     0,   449,   513,
       0,   515,   453,   440,   411,   137,   391,   143,   389,   149,
     148,   147,   144,   138,   393,   159,   160,   146,   150,   133,
     145,   161,   388,   370,   373,   371,   372,   394,   383,   374,
     387,   379,   377,   390,   378,   376,   381,   386,   375,   380,
     384,   385,   382,   392,   211,   367,     0,   411,   368,   411,
     365,   211,   482,     0,   477,   495,   494,   705,   704,   707,
     716,   715,   720,   719,   701,   698,   699,   700,   618,   688,
     140,     0,   658,   659,   142,   657,   656,   612,   692,   703,
     697,   695,   706,   696,   694,   686,   691,   693,   702,   685,
     689,   690,   687,   613,     0,     0,     0,     0,     0,     0,
       0,     0,   718,   717,   722,   721,   211,   211,     0,     0,
     283,   595,   596,   598,   600,     0,   587,     0,     0,     0,
       0,   526,   211,   545,   211,   211,   211,   245,     0,   241,
       0,     0,     0,     0,   544,     0,   259,     0,   257,     0,
     503,     0,   518,   517,   211,   498,   611,   497,   308,   305,
       0,     0,     0,     0,   655,   523,   349,   345,     0,   211,
     524,   505,   347,   211,   353,     0,     0,     0,   546,     0,
     290,   678,   619,   663,   528,   527,   665,   529,     0,     0,
      57,    58,     0,    61,    63,    67,    66,   211,    97,     0,
       0,   211,    92,   211,   211,   211,    64,   211,   370,   211,
     371,   372,   211,   211,   382,     0,   407,   408,    81,    80,
      91,     0,     0,   318,     0,     0,   211,     0,   211,     0,
     219,   218,   211,   569,     0,     0,     0,   211,   211,     0,
       0,   211,   211,   681,   230,     0,     0,   211,   228,     0,
     280,   211,     0,   338,   324,   330,   211,   328,   323,   426,
     414,   453,   511,   510,   509,   512,   454,   460,   426,   363,
       0,   369,     0,   359,   360,   484,   481,     0,     0,   128,
     126,   127,   125,   124,   123,   653,   654,     0,   211,   679,
     211,   211,   211,   597,   599,   211,   586,   162,   169,   168,
     167,   164,   171,   172,   166,   170,   165,   173,     0,   211,
     211,   491,   534,   533,   254,   253,     0,   248,     0,   211,
     244,     0,   250,     0,   282,     0,     0,   501,   504,   505,
     506,   507,   304,     0,     0,     0,   505,   211,   453,   523,
     522,   211,   422,   420,   355,   298,     0,   211,   297,   300,
     547,     0,   291,   294,     0,   211,     0,   211,   211,    59,
     121,   118,    98,   110,   104,   103,   102,   112,    99,   113,
     108,   101,   109,   107,   105,   106,   100,   111,   114,   211,
     117,   116,    88,    87,     0,     0,   211,   196,   211,   197,
     318,   338,     0,     0,   584,   584,   222,     0,   211,     0,
     211,   238,     0,     0,     0,   680,   236,   570,     0,   683,
     682,     0,   231,   211,   563,     0,     0,   229,     0,   311,
     186,   184,   310,   338,   338,   319,   334,     0,     0,   416,
     460,     0,   211,   357,   418,     0,   366,   411,   411,   660,
     661,   282,     0,     0,   211,     0,     0,     0,   588,   505,
     644,   644,   247,   243,   575,   249,     0,    73,   279,   258,
     255,   499,   211,   211,   493,   423,   421,   350,   351,   505,
     410,   302,     0,     0,     0,   211,   292,     0,     0,     0,
       0,    62,     0,   211,   211,   211,    93,    95,    84,    83,
      82,   338,   203,   209,   207,     0,     0,   217,     0,   240,
       0,   590,   239,     0,     0,     0,     0,   237,   211,     0,
       0,   226,     0,     0,   224,     0,   284,   338,   321,   320,
     329,     0,   336,   474,   475,     0,   476,   462,   465,     0,
     461,     0,   412,   361,   362,   211,   211,     0,   211,   211,
     284,   592,     0,   252,     0,     0,   425,   424,     0,   301,
       0,   295,     0,     0,   119,    89,     0,     0,   211,   204,
     221,   220,   223,   211,   589,   211,   234,   572,     0,   211,
     232,   571,     0,   684,     0,   211,   562,   227,   211,   561,
     225,   281,   322,   332,     0,   473,   211,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   455,   457,   358,   456,
     211,   211,   211,   211,   211,   251,   607,   608,   303,   293,
      94,    96,    85,   591,     0,   235,     0,   233,   566,     0,
       0,   337,   464,   466,   467,   470,   471,   472,   468,   469,
     463,   458,     0,     0,     0,     0,   609,   610,     0,     0,
       0,     0,   211,   211,   211,   211,   574,   573,   565,   564
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -900,  -900,  -900,    -1,   -59,  2335,  -900,   356,   792,  -900,
    -900,   413,  -399,  -107,  -900,   329,  -900,  -900,   -62,  -900,
    -900,    58,   645,  -900,  -533,  2968,   535,  -574,  -900,  -787,
    -900,  -900,  -900,   106,  -900,  -900,  -900,   890,  -900,   180,
    -204,  -900,  -900,  -463,   595,  -900,  -826,  -900,  -900,   233,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,    10,  -900,
    -163,   549,  -374,  -900,   528,  -900,  -900,   -15,  1541,  -900,
    -900,  -900,  -900,   585,  -900,   -38,  -900,  -900,  -900,  -900,
     407,  -900,  -900,  -900,  -104,  -900,  -342,  -774,  -900,  -900,
    -900,  -900,  -900,  -595,  -900,   130,  -900,  -900,  -900,  -900,
    -900,   393,  -900,  1511,  -900,  -900,  -900,  -900,   543,  -900,
    -900,  -900,  -900,    50,  -407,  -161,  -777,  -899,  -646,  -900,
      57,  -900,    79,   -48,   638,   -46,  -900,  -900,  -360,  -798,
    -900,  -250,  -900,   -95,  -258,  -248,  -745,  -900,  -900,   117,
     274,  -900,  -201,   821,  -900,  -900,  -162,   433,   197,  -253,
    -747,  -593,  -900,  -900,  -900,  -587,  -503,  -900,  -733,   -18,
      31,  -900,  -260,  -490,  -557,    -2,  -900,  -900,  -900,  -900,
    -900,  -900,   163,  -900,   -14,  -900,  -900,  -470,  -900,  -873,
    -900,  -900,   406,   547,  -900,  -900,  -900,  -900,  2581,  -900,
     -42,   999,  -900,  1242,  1383,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -476
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   350,   106,   107,   108,   109,   110,   367,
     368,   111,   112,   113,   114,   373,   574,   802,   803,   115,
     116,   117,   378,   118,   119,   120,   351,   830,   581,   831,
     121,   122,   990,   578,   579,   123,   124,   219,   255,   272,
     125,   257,   126,   306,   528,   127,   128,   293,   292,   129,
     130,   131,   132,   133,   134,   590,   834,   135,   203,   137,
     393,   394,   599,   138,   261,   139,   757,   758,   204,   141,
     142,   143,   144,   554,   790,   964,   145,   146,   786,   959,
     279,   280,   147,   148,   149,   150,   399,   863,   151,   152,
     611,  1037,  1038,   612,   153,   205,  1194,   155,   156,   307,
     308,   309,   948,   157,   321,   546,   783,   158,   159,  1141,
     160,   161,   666,   667,   826,   827,   828,   952,   879,   403,
     614,  1045,   615,   616,   311,   536,   418,   407,   413,   877,
    1208,  1209,   162,  1043,  1137,  1138,  1139,   163,   164,   420,
     421,   672,   165,   166,   223,   529,   273,   274,   514,   515,
     765,   313,   875,   621,   622,   518,   314,   780,   781,   352,
     354,   355,   493,   494,   495,   206,   168,   169,   395,   170,
     396,   171,   207,   173,   215,   216,   735,   850,  1110,   736,
     737,   174,   208,   209,   177,   178,   179,   322,   180,   385,
     496,   211,   182,   212,   213,   185,   186,   187,   188,   602,
     189,   190,   191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   105,   609,   423,   305,   371,   506,   221,   739,   227,
     825,   668,   136,   784,   865,   310,   804,   312,   755,   603,
     473,   883,   488,   884,   167,   492,   324,   874,   995,   383,
     871,   530,  1058,   991,   953,   619,   136,  1035,   782,   516,
     482,   588,   946,   617,   275,   275,   342,  1075,   516,   316,
    -331,   898,   386,   785,   762,   519,   680,   682,   360,  1001,
     387,   356,   492,   480,   389,   390,   690,  -514,   388,   389,
     390,   551,  -277,  1040,   167,     3,   533,   956,   167,   167,
    -516,   386,   499,   283,   844,   194,   136,   492,  1133,   387,
     136,   136,   217,  1133,   265,   516,  -514,   388,   389,   390,
     167,   504,   722,   724,   167,   545,   386,  1196,   844,  -516,
     275,   275,   136,  1205,  1158,  1120,   136,  1205,   198,   957,
     534,   214,   388,   389,   390,   364,  -260,   492,  -299,   846,
    -261,  -278,   154,   855,   859,   222,  1197,  1111,   433,   845,
    1134,  -514,  -516,   284,   218,  1134,   439,   391,    84,   365,
    1078,   848,  -352,  -514,  -516,   520,   154,   873,  -267,   523,
     490,   847,   285,  1113,  -516,   172,  -516,   476,   694,   369,
    -299,   477,   844,   958,   939,  1035,   391,   776,   286,   899,
    1156,   620,   365,   365,  1135,   517,  -516,   923,   229,   172,
    1136,   965,  1071,  -327,   517,  1136,   603,   481,   844,  1076,
    -514,   391,   392,   844,   570,  -514,   154,  1128,  1129,   256,
     154,   154,  -516,   167,   720,   721,  1079,  -327,  -516,  1098,
    -516,   268,   269,   270,  -299,   136,  1101,  1162,   375,    24,
     294,   569,   154,   224,   844,   696,   154,   897,  1041,   172,
     994,   517,  -516,   172,   172,   832,  1206,   266,   365,   365,
     296,   365,  -266,   365,   298,   365,   392,   669,   408,   759,
    -516,   848,   903,  -275,   844,   172,   564,   376,   167,   172,
     492,   492,   229,   167,   852,  1035,   950,   429,   587,   299,
     136,    57,   300,   760,    59,   136,   778,   414,   869,   849,
     787,   490,   565,    66,   301,  1042,   377,   560,   -70,   905,
    1223,  1192,   853,   294,   492,   194,   904,   409,  1167,   761,
     410,   557,  1151,   304,   870,  -265,   690,    86,   -70,   966,
     258,   561,   411,   296,   770,   906,   740,   298,   773,   859,
     400,  1059,  1157,   744,   691,   490,   430,   -70,   935,   415,
     431,   412,   -69,   305,   419,   154,   492,   692,   266,   167,
     559,   416,   299,   491,  1077,   300,   920,    59,  -262,   934,
     477,   136,   -69,   167,   936,   760,   -70,   301,   198,   492,
     417,  1028,   167,   267,   600,   136,   613,   693,   172,   479,
     606,   -69,   404,   760,   136,   167,   304,   433,  -269,   835,
    1195,   937,   437,   764,   438,   439,   474,   136,   475,   442,
     154,  1143,  1144,   405,   867,   154,   449,  -273,   175,   949,
     -69,   804,  1057,   406,   455,  1092,   670,   968,   694,   951,
     744,   715,   716,  -652,   695,  -652,   490,   717,   868,   739,
     718,   719,   175,   172,   744,   744,   336,   339,   172,   497,
     498,  -594,  1130,   969,   429,  -593,  1051,  -270,  1173,  -417,
     490,   513,  1109,  1234,  1235,  1236,  1237,  1238,  1239,   673,
    1126,  1150,  1004,   728,  1034,   996,   520,  -594,   542,   860,
     807,  -593,   290,  1013,  1174,   668,   861,   674,   811,   305,
     520,   154,   175,  1025,   676,   291,   175,   175,   808,  1164,
     310,   540,   312,   337,   337,   154,   812,   167,   921,   167,
     194,   996,   371,   430,   154,  -264,   805,   431,   175,   136,
     263,   136,   175,   264,   172,   520,   330,   154,   294,   997,
    1166,   795,   795,   492,  1052,   531,   532,  1055,   172,   996,
     673,  1198,  -268,  1074,   739,   520,  1193,   172,   296,   996,
     492,   256,   298,  1114,  1116,   680,   722,  1096,  1103,   176,
     172,   798,   331,  1104,   433,  1122,  1125,  1220,  1199,   437,
    1200,  1201,   439,  1202,  1203,  -727,   442,   299,  -274,  -727,
     300,  -276,   358,   176,  -263,   167,   559,   605,   837,  -271,
     433,   610,   301,   294,  -651,   437,  -651,   136,   439,     7,
    -581,  1087,   442,  1089,   794,   797,  -728,  -651,   665,  -651,
    -728,   304,  1034,   296,   372,   971,   696,   298,  1207,  -729,
    -724,   945,   374,  -729,  -724,   429,   397,  1114,  1116,  1122,
    1125,   175,  -648,   176,  -648,   690,   398,   176,   176,   154,
     401,   154,   299,   402,  1127,   300,   365,    59,   509,  1131,
     907,   277,  -650,   908,  -650,   483,  -578,   301,   909,   176,
    -649,   424,  -649,   176,  -577,   429,    38,   492,   196,  -338,
     484,  1207,   172,   730,   172,   326,   304,   887,   910,   888,
     425,   365,  -582,  1003,   430,   911,   175,   198,   431,  -583,
    -338,   175,  -580,    55,   376,   508,   912,  -579,   526,   756,
    -338,   486,   513,   489,   913,   507,   527,   739,   492,   538,
     922,   541,  1034,   547,   537,   769,   550,   154,   556,   563,
     566,   914,   567,   572,   430,   775,   589,   573,   431,   513,
     583,  1048,   941,   915,   594,   433,   434,   694,   435,   436,
     437,   607,   438,   439,   916,   918,   671,   442,   618,   917,
     172,   623,   675,   448,   449,   714,   167,   452,   727,   167,
     453,   454,   455,   439,   742,   743,   744,   175,   136,   745,
     746,   136,   176,   816,   492,   433,   434,   748,   435,   436,
     437,   175,   438,   439,   839,   751,   753,   442,   754,   779,
     175,   764,   789,   774,   449,   788,   833,   838,   490,   610,
     453,   454,   455,   175,   842,   866,   864,   876,   878,   902,
    -140,  -162,  -169,   889,  1033,   881,  -168,  -167,  -164,  -141,
     305,  -171,  1154,  1155,  -172,  -166,  -170,   176,  -142,   305,
    -165,  -173,   176,   882,  1009,   926,   890,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   471,   891,   167,   892,   429,   893,   167,   894,   928,
     896,   895,   947,   419,   167,   136,   955,   960,   999,   136,
     996,  1010,  1000,  1008,  1066,  1012,   136,  1029,   492,   492,
    1018,   472,  1049,   522,  -648,   524,  -648,  1067,   154,  1068,
    1050,   154,  1085,  1097,   869,  1148,  1152,  1072,  1073,  1191,
    1142,  1197,  1246,  1160,  1145,  1221,  1178,  1182,   176,  1216,
    1217,   800,  1247,   366,   430,   175,  1091,   175,   431,   577,
     505,   172,   176,   970,   172,   225,   571,  1060,  1061,   595,
    1069,   176,   549,  1084,   167,   555,   167,   371,   591,   167,
     768,   777,  1047,   933,   176,  1044,   136,   429,   136,  1083,
     513,   136,  1033,   535,  1231,  1132,  1240,   886,  1039,  1233,
     942,  1002,   380,   767,   167,   433,   434,   938,   504,   436,
     437,     0,   438,   439,   167,     0,  1082,   442,     0,   963,
       0,     0,     0,     0,   449,   154,   136,     0,     0,   154,
     453,   454,   455,   175,     0,     0,   154,     0,     0,     0,
    1105,  1106,     0,     0,     0,     0,   430,     0,     0,     0,
     431,   181,     0,     0,     0,     0,     0,     0,   172,     0,
     167,     0,   172,     0,   419,     0,   167,     0,     0,   172,
     371,     0,   136,     0,   371,   181,     0,     0,   136,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1033,     0,     0,     0,   176,   433,   176,     0,
       0,     0,   437,     0,   438,   439,   154,     0,   154,   442,
       0,   154,   665,   504,   167,   504,   449,     0,   504,   504,
       0,     0,   453,   454,   455,   181,   136,     0,     0,   181,
     181,     0,     0,     0,     0,     0,   154,     0,   430,   172,
       0,   172,   431,     0,   172,     0,   154,     0,     0,     0,
       0,   181,     0,     0,     0,   181,     0,     0,     0,     0,
       0,   167,     0,   167,     0,   756,     0,     0,   167,   172,
       0,   167,     0,   136,   176,   136,     0,     0,     0,   172,
     136,     0,     0,   136,     0,     0,     0,     0,     0,   433,
     434,   963,   154,     0,   437,     0,   438,   439,   154,     0,
     167,   442,     0,     0,   175,     0,   167,   175,   449,     0,
     167,   167,   136,     0,   453,   454,   455,     0,   136,     0,
       0,     0,   136,   136,     0,   172,   167,     0,     0,     0,
     167,   172,   167,     0,     0,     0,     0,     0,   136,     0,
     840,   841,   136,     0,   136,     0,   154,     0,   854,   858,
       0,     0,     0,   167,  1241,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   181,   136,     0,   610,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   172,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   154,   183,   154,   167,   167,   167,   167,
     154,   175,     0,   154,     0,   175,     0,     0,   136,   136,
     136,   136,   175,     0,     0,     0,     0,     0,   183,   181,
       0,     0,     0,     0,   181,     0,   172,     0,   172,     0,
       0,     0,   154,   172,     0,     0,   172,     0,   154,     0,
       0,     0,   154,   154,     0,   176,     0,     0,   176,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   154,     0,
       0,   610,   154,     0,   154,   172,   862,     0,   183,     0,
    -318,   172,   183,   183,     0,   172,   172,     0,     0,     0,
       0,     0,   175,     0,   175,   154,     0,   175,  -318,     0,
       0,   172,  -318,  -318,   183,   172,     0,   172,   183,     0,
     181,  -318,     0,  -318,  -318,     0,     0,     0,     0,  -318,
       0,     0,   175,     0,   181,     0,  -318,     0,   172,  -318,
       0,     0,   175,   181,     0,     0,     0,     0,   154,   154,
     154,   154,     0,     0,     0,   184,   181,     0,  -318,     0,
    -318,     0,   176,  -318,   858,  -318,   176,  -318,     0,  -318,
    -318,     0,  -318,   176,     0,  -318,     0,  -318,     0,   184,
       0,   172,   172,   172,   172,     0,     0,     0,   175,     0,
       0,     0,     7,     0,   175,     0,  -318,     0,  -318,     0,
       0,  -318,  1006,     0,     0,     0,     0,     0,     0,     0,
     360,     0,     0,     0,    23,    24,     0,     0,     0,     0,
    1022,     0,     0,   361,  1027,    30,   362,   183,     0,   184,
       0,    33,     0,   184,   184,     0,     0,     0,    38,     0,
       0,     0,   175,   176,     0,   176,     0,     0,   176,     0,
       0,  -318,     0,     0,     0,   184,     0,  -318,     0,   184,
     363,     0,     0,     0,     0,    55,     0,    57,   181,    59,
     181,  1030,     0,   176,  1031,     0,     0,   364,     0,    66,
       0,     0,   183,   176,     0,     0,     0,   183,     0,   175,
       0,   175,     0,     0,     0,     0,   175,     0,    82,   175,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   175,   176,
       0,     0,     0,     7,   175,   176,     0,   140,   175,   175,
       0,     0,     0,     0,     0,     0,   181,     0,     0,   320,
       0,   360,     0,   100,   175,    23,    24,     0,   175,  1032,
     175,     0,     0,   183,   361,     0,    30,   362,   184,   320,
       0,     0,    33,     0,     0,     0,     0,   183,     0,    38,
       0,   175,     0,   176,   320,     0,   183,   140,     0,     0,
    1121,  1124,     0,     0,     0,     0,     0,     0,     0,   183,
       0,   363,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,  1030,   140,     0,  1031,     0,   140,   364,     0,
      66,     0,     0,   184,   175,   175,   175,   175,   184,     0,
     176,     0,   176,     0,     0,     0,     0,   176,     0,    82,
     176,    84,     0,     0,    86,     0,     0,     0,  1159,     0,
       0,     0,     0,     0,  1121,  1124,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   176,
    1170,  1171,     0,  1172,     0,   176,     0,     0,     0,   176,
     176,     0,     0,     0,     0,     0,     0,  1187,     0,     0,
    1190,     0,     0,     0,   100,   176,     0,     0,     0,   176,
    1102,   176,     0,     0,   184,     0,     0,     0,     0,     0,
     320,   183,     0,   183,     0,     0,     0,   181,   184,     0,
     181,     0,   176,     0,     0,     0,   140,   184,     0,     0,
       0,   422,     0,   230,     0,     0,     0,     0,     0,     0,
     184,     0,     0,     0,     0,     0,     0,   320,   231,   232,
       0,   233,     0,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,   235,   176,   176,   176,   176,     0,
     236,     0,   320,     0,     0,     0,   237,     0,     0,     0,
     238,   140,     0,   239,     0,   240,   140,     0,     0,   183,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,     0,     0,     0,     0,     0,     0,   244,
       0,   360,   320,     0,   181,    23,    24,     0,   181,   245,
       0,     0,     0,     0,   361,   181,    30,   362,   246,   247,
       0,   248,    33,   249,     0,   250,     0,     0,   251,    38,
       0,     0,   252,     0,   253,     0,     0,   254,     0,     0,
       0,     0,   184,     0,   184,     0,     0,     0,     0,     0,
       0,   363,   140,     0,     0,     0,    55,     0,    57,     0,
       0,   230,     0,     0,     0,     0,   140,     0,   364,     0,
      66,     0,     0,     0,     0,   140,   231,   232,     0,   233,
       0,     0,     0,     0,   234,   181,     0,   181,   140,    82,
     181,    84,   235,   271,    86,     0,     0,     0,   236,     0,
       0,     0,     0,     0,   237,     0,     0,     0,   238,     0,
       0,   239,     0,   240,     0,   181,     0,     0,     0,     0,
     184,     0,   241,     0,     0,   181,     0,     0,     0,   242,
     243,     0,     0,     0,   320,   320,     0,   244,     0,     0,
     320,     0,     0,   320,   320,   320,   320,   245,     0,     0,
     183,     7,     0,   183,     0,     0,   246,   247,     0,   248,
       0,   249,     0,   250,     0,     0,   251,     0,     0,   360,
     252,   181,   253,    23,    24,   254,     0,   181,     0,   320,
       0,     0,   361,     0,    30,   362,     0,     0,     0,     0,
      33,     0,     0,     0,     0,     0,     0,    38,     0,     0,
     140,     0,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
       0,   320,     0,     0,    55,   181,    57,     0,    59,   365,
    1030,   961,     0,  1031,   320,   320,   364,     0,    66,     0,
       0,     0,     0,     0,   320,     0,     0,   183,     0,     0,
       0,   183,     0,     0,     0,     0,     0,    82,   183,    84,
       0,     0,    86,     0,     0,     0,     0,     7,     0,     0,
       0,     0,   181,     0,   181,     0,     0,     0,   140,   181,
       0,     0,   181,     0,     0,   360,     0,     0,     0,    23,
      24,   184,     0,     0,   184,     0,     0,     0,   361,     0,
      30,   362,     0,     0,     0,     0,    33,     0,   429,     0,
       0,   181,   100,    38,     0,     0,     0,   181,  1169,     0,
       0,   181,   181,     0,     0,     0,     0,     0,   183,     0,
     183,     0,     0,   183,     0,   363,     0,   181,     0,     0,
      55,   181,    57,   181,   358,     0,  1030,     0,     0,  1031,
       0,     0,   364,     0,    66,     0,     0,     0,   183,     0,
       0,     0,     0,     0,   181,     0,     0,   430,   183,     0,
       0,   431,     0,    82,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   184,     0,
       0,     0,   184,     0,     0,     0,     0,     0,   320,   184,
       0,     0,     0,     0,     0,     0,     0,   181,   181,   181,
     181,     0,     0,   320,   183,   320,     0,     0,   433,   434,
     183,   435,   436,   437,     0,   438,   439,   440,   100,     0,
     442,     0,     0,     0,     0,   320,   448,   449,     0,     0,
     452,     0,   230,   453,   454,   455,     0,     0,     0,   140,
       0,     0,   140,     0,   456,     0,     0,   231,   232,     0,
     233,     0,     0,     0,     0,   234,     0,     0,   183,   184,
       0,   184,     0,   235,   184,     0,     0,     0,     0,   236,
       0,     0,     0,     0,     0,   237,     0,     0,     0,   238,
       0,     0,   239,     0,   240,     0,     0,     0,     0,   184,
       0,     0,     0,   241,     0,     0,     0,     0,     0,   184,
     242,   243,     0,     0,     0,   183,     0,   183,   244,     0,
       0,   226,   183,     0,     0,   183,     0,     0,   245,     0,
       0,     0,   320,     0,     0,     0,     0,   246,   247,     0,
     248,     0,   249,     0,   250,     0,   140,   251,     0,     0,
     140,   252,     0,   253,   183,   184,   254,   140,     0,     0,
     183,   184,     0,     0,   183,   183,     0,     0,     0,     0,
       0,   332,     0,   320,     0,     0,     0,     0,     0,     0,
     183,     0,     0,     0,   183,     0,   183,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   359,     0,     7,     0,     0,     0,   183,     0,   184,
       0,     0,   271,     0,     0,     0,     0,     0,     0,     0,
       0,   360,     0,     0,     0,    23,    24,   140,     0,   140,
       0,     0,   140,     0,   361,     0,    30,   362,     0,   320,
       0,     0,    33,     0,     0,     0,     0,     0,     0,    38,
     183,   183,   183,   183,     0,     0,   184,   140,   184,     0,
       0,     0,     0,   184,     0,     0,   184,   140,     0,     0,
       0,   363,     0,     0,     0,     0,    55,     0,    57,     0,
       0,     0,  1030,     0,     0,  1031,     0,     0,   364,     0,
      66,     0,     0,     0,     0,   184,     0,     0,     0,     0,
       0,   184,     0,     0,     0,   184,   184,     0,     0,    82,
     485,    84,     0,   140,    86,     0,     0,     0,     0,   140,
       0,   184,     0,     0,     0,   184,     0,   184,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   320,   320,     0,     0,     0,   184,     0,
       0,     0,     0,     0,     0,   210,     0,     0,     0,   220,
       0,     0,     0,     0,   100,   511,     0,   140,   228,     0,
     521,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   278,     0,   281,
     282,   184,   184,   184,   184,   287,     0,   288,     0,   289,
       0,     0,     0,     0,   315,     0,     0,     0,   228,   323,
     325,   327,   328,   329,   140,     0,   140,   333,     0,   334,
       0,   140,     0,     0,   140,     0,   341,     0,     0,   323,
       0,   343,     0,   344,     0,     0,   345,   346,     0,   347,
       0,     0,     0,   228,   323,   357,     0,     0,     0,     0,
       0,     0,     0,   140,     0,     0,     0,     0,     0,   140,
       0,     0,     0,   140,   140,     0,     0,     0,     0,     0,
       0,   230,   381,     0,   384,     0,     0,     0,     0,   140,
       0,     0,   593,   140,     0,   140,   231,   232,     0,   233,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,     0,     0,     0,   140,     0,   236,     0,
       0,     0,     0,     0,   237,   426,     0,     0,   238,     0,
       0,   239,     0,   240,     0,     0,     0,     0,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,   478,   242,
     243,     0,   333,   228,     0,     0,     0,   244,     0,   140,
     140,   140,   140,     0,     0,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,   246,   247,     0,   248,
     323,   249,     0,   250,     0,     0,   251,     0,     0,     0,
     252,     0,   253,     0,     0,   254,     0,   230,     0,     0,
       0,     0,     0,     0,   747,     0,   749,     0,     0,     0,
       0,     0,   231,   232,     0,   233,     0,   323,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,   235,     0,
       0,     0,     0,     0,   236,     0,     0,     0,     0,     0,
     237,     0,   323,     0,   238,     0,     0,   239,     0,   240,
       0,   512,     0,   329,   333,     0,     0,     0,   241,     0,
       0,     0,     0,     0,     0,   242,   243,     0,   543,   544,
       0,     0,     0,   244,     0,     0,     0,     0,     0,     0,
       0,     0,   323,   245,     0,     0,   553,     0,     0,     0,
       0,     0,   246,   247,     0,   248,     0,   249,     0,   250,
       0,     0,   251,     0,   562,     0,   252,     0,   253,     0,
       0,   254,     0,     0,   568,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   580,     0,   582,     0,
       0,     0,     0,     0,     0,     0,     0,   592,     0,     0,
       0,     0,   596,   601,   604,     0,     0,     0,     0,   228,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   539,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   677,   678,
     679,   681,   683,   684,   276,   685,   686,   687,   688,   689,
       0,   697,   698,   699,   700,   701,   702,   703,   704,   705,
     706,   707,   708,   709,   710,   711,   712,   713,   276,     0,
       0,     0,     0,     0,   323,   323,     0,     0,   338,   340,
     323,     0,     0,   323,   323,   323,   323,   723,   725,     0,
       0,     0,   601,   729,     0,   738,     0,     0,     0,     0,
       0,   741,     0,     0,     0,     0,     0,     0,     0,   370,
       0,     0,   750,   927,     0,   752,   930,     0,     0,   323,
       0,     0,     0,     0,     0,     0,     0,     0,   763,     0,
     766,     0,     0,     0,   281,     0,     0,   771,   772,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,   323,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   741,     0,     0,   323,   323,     0,     0,     0,     0,
     228,   799,     0,     0,   323,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   829,   829,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   843,
    1011,     0,     0,     0,  1016,     0,     0,     0,     0,   487,
       0,   521,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   360,     0,     0,     0,    23,    24,   500,
     503,     0,     0,     0,     0,     0,   361,   510,    30,   362,
       0,     0,     0,     0,    33,   880,   276,     0,   276,     0,
       0,    38,   885,     0,  -338,   276,     0,     0,     0,     0,
       0,  1062,     0,  1063,     0,     0,  1065,   -68,     0,     0,
       0,     0,     0,   363,     0,  -338,     0,     0,    55,     0,
      57,     0,     0,     0,    61,  -338,   -68,    62,     0,     0,
     364,  1081,    66,     0,     0,   276,     0,     0,     0,     0,
       0,  1016,     0,     0,     0,     0,     0,   750,   323,     0,
       0,    82,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,   323,     0,   323,   924,   925,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   323,     0,  1112,     0,     0,
       0,     0,     0,  1117,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,   954,     0,   100,     0,     0,     0,
       0,   365,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   580,     0,
       0,     0,   992,     0,   993,   829,   829,     0,   343,     0,
     344,  1153,     0,   346,   347,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1005,     0,  1007,
       0,     0,     0,   228,     0,     0,     0,     0,  1014,   741,
    1017,     0,  1019,  1021,     0,     0,     0,  1024,  1026,     0,
       0,     0,   323,     0,     0,     0,     0,  1036,  1176,     0,
    1180,     0,     0,     0,     0,   521,     0,     0,   521,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1053,
    1017,  1024,  1056,   323,     0,     0,   738,  1215,     0,     0,
       0,     0,     0,  1218,     0,     0,   427,  1176,  1180,     0,
     681,   723,     0,   428,     0,     0,     0,     0,     0,     0,
    1064,     0,     0,  1225,     0,   429,     0,  1227,     0,   749,
     584,   276,     0,     0,     0,     0,     0,     0,   315,     0,
       0,     0,  1080,     0,     0,     0,     0,     0,   228,     0,
       0,     0,     0,     0,     0,     0,  1088,     0,  1090,   323,
       0,     0,     0,     0,     0,     0,     0,   585,     0,     0,
       0,     0,     0,   276,   276,     0,     0,     0,     0,     0,
     829,   276,   276,     0,   430,     0,     0,  1099,   431,  1100,
       0,     0,     0,   749,   749,   749,   749,     0,     0,  1108,
       0,   738,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1119,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1140,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,  1147,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,   586,
     453,   454,   455,   323,   323,     0,     0,     0,     0,     0,
       0,   456,     0,     0,     0,     0,  1161,     0,     0,     0,
       0,     0,     0,     0,  1165,   829,   829,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1177,     0,  1181,     0,     0,  1183,
       0,  1186,     0,     0,  1189,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     932,     0,     0,     0,     0,     0,  1177,  1181,     0,  1186,
    1189,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1222,
       0,     0,     0,     0,   738,     0,  1224,   962,     0,     0,
    1226,     0,     0,     0,     0,  1228,  1229,   276,     0,  1230,
       0,     0,     0,     0,     0,     0,     0,  1232,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1242,  1243,  1228,  1244,  1245,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   276,     0,     0,     0,   276,     0,  1256,
    1257,  1258,  1259,  1256,  1257,  1258,  1259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    -2,     4,     0,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,    15,    16,    17,    18,     0,    19,     0,    20,
      21,  -211,    22,    23,    24,    25,    26,    27,     0,  -211,
      28,     0,    29,     0,    30,    31,    32,  -211,  -211,  -211,
      33,    34,    35,   -68,  1070,    36,    37,    38,     0,    39,
    -338,    40,     0,    41,    42,    43,    44,    45,     0,    46,
      47,    48,    49,   -68,    50,    51,     0,    52,    53,    54,
       0,  -338,  1086,     0,    55,    56,    57,    58,    59,    60,
      61,  -338,   -68,    62,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,    77,    78,     0,    79,    80,    81,    82,    83,    84,
      85,   -68,    86,    87,     0,     0,  -211,     0,    88,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,   427,
       0,     0,     0,   276,   276,     0,   428,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,   429,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,     0,   103,
       0,  -211,     0,     0,   104,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   276,     0,     0,     0,     0,     0,   276,   276,     0,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,   431,     0,   276,   276,     0,   276,     0,     0,     0,
     501,     0,     0,     0,     0,     0,     0,   503,     0,     0,
     276,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,   432,   502,   458,   459,   460,   461,   462,     0,     0,
     465,   466,   467,   468,     0,   470,   471,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,  1219,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,     0,     0,   453,   454,   455,     0,   365,     0,     0,
       0,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   503,     0,   503,     0,     0,   503,   503,     4,
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
       0,  -679,     0,    14,    15,    16,    17,    18,  -679,    19,
       0,    20,    21,  -211,    22,    23,    24,    25,    26,    27,
    -679,  -211,    28,  -679,    29,     0,    30,    31,    32,  -211,
    -211,  -211,    33,    34,    35,   -68,     0,    36,    37,    38,
       0,    39,  -338,    40,     0,    41,    42,    43,    44,    45,
       0,    46,    47,    48,    49,   -68,    50,    51,     0,    52,
      53,    54,     0,  -338,     0,     0,    55,    56,    57,    58,
       0,    60,    61,  -338,   -68,    62,    63,    64,    65,  -679,
      66,    67,    68,  -679,    69,    70,    71,    72,    73,    74,
      75,    76,     0,    77,    78,     0,    79,    80,    81,    82,
      83,    84,    85,   -68,    86,    87,     0,     0,  -211,     0,
      88,     0,     0,  -679,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,  -679,    92,  -679,  -679,  -679,  -679,  -679,  -679,  -679,
       0,  -679,  -679,  -679,  -679,  -679,  -679,  -679,  -679,  -679,
    -679,  -679,  -679,    99,   100,  -679,  -679,  -679,     0,   102,
    -679,   103,     0,  -211,     0,   348,  -679,     5,     6,     7,
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
     100,     0,     0,   101,     0,   102,   349,   103,     0,  -211,
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
       0,   102,   558,   103,     0,  -211,     0,   575,   104,     5,
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
      98,    99,   100,     0,     0,   101,     0,   102,   576,   103,
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
       0,   101,     0,   102,   806,   103,     0,  -211,     0,     4,
     104,     5,     6,     7,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,    15,    16,    17,    18,     0,    19,
       0,    20,    21,  -211,    22,    23,    24,    25,    26,    27,
       0,  -211,    28,     0,    29,     0,    30,    31,    32,  -211,
    -211,  -211,    33,    34,    35,   -68,     0,    36,    37,    38,
       0,    39,  -338,    40,     0,    41,    42,    43,    44,    45,
       0,    46,    47,    48,    49,   -68,    50,    51,     0,    52,
      53,    54,     0,  -338,     0,     0,    55,    56,    57,    58,
     358,    60,    61,  -338,   -68,    62,    63,    64,    65,     0,
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
      33,  1023,    35,   -68,     0,    36,    37,    38,     0,    39,
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
      30,    31,    32,  -211,  -211,  -211,    33,  1175,    35,   -68,
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
    -211,  -211,    33,  1179,    35,   -68,     0,    36,    37,    38,
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
      29,     0,    30,    31,    32,  -211,  -211,  -211,    33,  1185,
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
      32,  -211,  -211,  -211,    33,  1188,    35,   -68,     0,    36,
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
       0,   102,     0,   103,   872,  -211,     5,     6,   104,     8,
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
       6,   104,     8,     9,    10,    11,    12,    13,     0,   626,
       0,    14,     0,     0,    17,    18,   628,    19,     0,   195,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   196,     0,     0,     0,    32,     0,     0,     0,
       0,   197,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   198,   634,   199,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   200,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   201,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    89,   502,   458,   459,   460,   461,   462,     0,     0,
     465,   466,   467,   468,     0,   470,   471,     0,   817,   818,
     819,   820,   821,   647,     0,   648,     0,    95,     0,   649,
     650,   651,   652,   653,   654,   655,   822,   657,   658,    97,
     823,    99,   -90,   660,   661,   824,   663,   202,     0,   103,
       0,     0,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,   626,     0,    14,     0,     0,    17,    18,   628,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,   634,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   201,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    89,   502,   458,   459,   460,   461,
     462,     0,     0,   465,   466,   467,   468,     0,   470,   471,
       0,   817,   818,   819,   820,   821,   647,     0,   648,     0,
      95,     0,   649,   650,   651,   652,   653,   654,   655,   822,
     657,   658,    97,   823,    99,     0,   660,   661,   824,   663,
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
       0,    19,     0,   195,     0,     0,    22,     0,   731,     0,
       0,     0,     0,     0,    28,     0,   196,     0,     0,     0,
      32,     0,     0,     0,     0,   197,    35,     0,   732,     0,
      37,     0,     0,    39,     0,   198,     0,   199,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     200,     0,   733,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   201,     0,    77,     0,     0,    79,     0,
       0,     0,    83,     0,    85,     0,   734,    87,     0,     0,
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
       0,   103,   793,     0,     5,     6,   104,     8,     9,    10,
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
     101,     0,   202,     0,   103,   796,     0,     5,     6,   104,
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
       0,     0,     0,   101,     0,   202,     0,   103,   801,     0,
       5,     6,   104,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,   197,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,   940,    52,    53,
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
       0,   597,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,     0,   598,     0,   104,     5,     6,     0,
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
       0,     0,     0,   597,     0,    89,     0,     0,     0,     0,
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
       0,     0,   101,     0,   202,     0,   103,     0,     0,   598,
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
     202,   791,   103,     0,     0,     5,     6,   104,     8,     9,
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
       0,   101,     0,   202,     0,   103,     0,     0,  1015,     0,
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
       0,   103,     0,     0,  1020,     0,   104,     5,     6,     0,
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
      32,     0,     0,     0,     0,  1054,    35,     0,     0,     0,
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
       0,     0,     0,    32,     0,     0,     0,     0,  1210,    35,
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
       0,  1211,    35,     0,     0,     0,    37,     0,     0,    39,
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
       0,     0,     0,     0,  1213,    35,     0,     0,     0,    37,
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
       0,     0,    32,     0,     0,     0,     0,  1214,    35,     0,
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
       0,   429,   299,     0,    56,   300,    58,    59,    60,     0,
       0,     0,     0,    63,     0,   200,     0,   301,     0,     0,
       0,     0,     0,    71,     0,    73,     0,    75,     0,     0,
       0,     0,     0,     0,     0,     0,   304,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,   431,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,  -342,
    -211,     0,     6,   104,     8,     9,    10,    11,    12,    13,
       0,   433,   434,    14,   435,   436,   437,    18,   438,   439,
     440,   195,     0,   442,   443,   444,   294,   446,   447,   448,
     449,     0,     0,   452,   196,     0,   453,   454,   455,  -211,
    -211,  -211,     0,   197,     0,     0,   296,   456,     0,     0,
     298,     0,     0,     0,     0,   199,     0,     0,    44,    45,
       0,     0,     0,     0,    49,     0,    50,    51,     0,     0,
       0,     0,     0,     0,     0,   299,     0,    56,   300,    58,
      59,    60,     0,     0,     0,     0,    63,     0,   200,     0,
     301,     0,     0,     0,     0,     0,    71,     0,    73,     0,
      75,     0,     0,     0,     0,     0,     0,     0,     0,   304,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   624,     0,   625,     0,     0,
      90,    91,    92,    93,    94,     0,     0,   626,     0,    95,
       0,     0,   627,   232,   628,   629,     0,     0,    96,     0,
     630,    97,    98,    99,     0,     0,     0,   101,   235,   202,
     196,   103,     0,  -211,   236,     0,   104,     0,     0,     0,
     631,     0,     0,     0,   238,     0,     0,   632,     0,   633,
     634,     0,     0,     0,     0,     0,     0,     0,   635,     0,
       0,     0,     0,     0,     0,   242,   636,     0,     0,     0,
       0,     0,     0,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   637,     0,     0,     0,     0,     0,     0,
       0,     0,   246,   247,     0,   638,     0,   249,     0,   639,
       0,     0,   251,     0,     0,     0,   640,     0,   253,     0,
       0,   641,     0,     0,     0,     0,     0,     0,     0,     0,
     502,   458,   459,   460,   461,   462,     0,     0,   465,   466,
     467,   468,     0,   470,   471,     0,   642,   643,   644,   645,
     646,   647,     0,   648,     0,     0,     0,   649,   650,   651,
     652,   653,   654,   655,   656,   657,   658,     0,   659,     0,
       0,   660,   661,   662,   663,     5,     6,   664,     8,     9,
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
      79,   690,     0,     0,    83,     0,    85,     0,     0,    87,
       0,   626,     0,     0,     0,     0,   972,   973,   628,   974,
       0,     0,     0,     0,   975,     0,     0,     0,     0,     0,
       0,     0,   692,     0,    90,     0,    92,     0,     0,     0,
       0,     0,     0,     0,   976,     0,     0,     0,   977,     0,
       0,   978,     0,   198,   634,     0,     0,    99,     0,     0,
       0,     0,   693,     0,     0,   103,     0,  -211,     0,     0,
     979,     0,     0,     0,     0,     0,     0,   980,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   981,     0,     0,
       0,     0,     0,     0,     0,     0,   982,   983,     0,   984,
       0,   985,     0,   694,     0,     0,     0,     0,     0,   695,
     986,     0,   987,     0,     0,   988,     0,     0,     0,     0,
       0,     0,     0,     0,   502,   458,   459,   460,   461,   462,
       0,     0,   465,   466,   467,   468,     0,   470,   471,     0,
     642,   643,   644,   645,   646,   647,     0,   648,     0,     0,
       0,   649,   650,   651,   652,   653,   654,   655,   656,   657,
     658,   230,   659,     0,     0,   660,   661,   662,   663,   989,
       0,   626,     0,     0,     0,     0,   231,   232,   628,   233,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,     0,     0,     0,     0,     0,   236,     0,
       0,     0,     0,     0,   237,     0,     0,     0,   238,     0,
       0,   239,     0,   240,   634,     0,     0,     0,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,   242,
     243,     0,     0,     0,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,   427,   246,   247,     0,   248,
       0,   249,   428,   250,     0,     0,   251,     0,     0,     0,
     252,     0,   253,     0,   429,   254,     0,     0,     0,     0,
       0,     0,     0,     0,   502,   458,   459,   460,   461,   462,
       0,     0,   465,   466,   467,   468,     0,   470,   471,     0,
     642,   643,   644,   645,   646,   647,     0,   648,     0,     0,
       0,   649,   650,   651,   652,   653,   654,   655,   656,   657,
     658,     0,   659,     0,     0,   660,   661,   662,   663,     0,
       0,     0,     0,   430,   427,     0,     0,   431,     0,     0,
       0,   428,     0,     0,     0,     0,  1184,     0,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   502,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,   430,   365,     0,   427,   431,     0,     0,     0,
     456,     0,   428,     0,     0,  1248,     0,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   432,   502,   458,   459,
     460,   461,   462,     0,     0,   465,   466,   467,   468,     0,
     470,   471,     0,   433,   434,     0,   435,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,     0,     0,   453,   454,
     455,     0,   365,   430,   427,     0,     0,   431,     0,   456,
       0,   428,     0,     0,     0,     0,  1249,     0,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   502,   458,
     459,   460,   461,   462,     0,     0,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,   430,   365,     0,   427,   431,     0,     0,     0,
     456,     0,   428,     0,     0,  1250,     0,     0,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   432,   502,   458,   459,
     460,   461,   462,     0,     0,   465,   466,   467,   468,     0,
     470,   471,     0,   433,   434,     0,   435,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,     0,     0,   453,   454,
     455,     0,   365,   430,     0,     0,     0,   431,     0,   456,
     427,     0,     0,     0,     0,     0,  1251,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
     856,     0,     0,     0,     0,     0,     0,   432,   502,   458,
     459,   460,   461,   462,     0,   857,   465,   466,   467,   468,
       0,   470,   471,     0,   433,   434,     0,   435,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,     0,   453,
     454,   455,     0,   365,     0,     0,     0,     0,   430,   427,
     456,     0,   431,     0,     0,     0,   428,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   429,   275,
       0,     0,     0,     0,     0,     0,     0,     0,   844,     0,
       0,     0,   432,     0,   967,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,     0,   453,   454,   455,   430,   365,     0,
       0,   431,     0,     0,     0,   456,     0,     0,     0,     0,
     427,   813,     0,     0,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   844,     0,   429,
       0,   432,     0,     0,   814,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   815,     0,   453,   454,   455,     0,   365,     0,     0,
       0,     0,     0,     0,   456,     0,     0,     0,   430,   427,
       0,     0,   431,     0,     0,     0,   428,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   429,  1123,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,     0,   453,   454,   455,   430,     0,     0,
     427,   431,     0,     0,     0,   456,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
     275,     0,     0,     0,     0,     0,     0,   844,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,     0,     0,   453,   454,   455,     0,   365,   430,   427,
       0,     0,   431,     0,   456,     0,   428,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   429,   275,
       0,     0,     0,     0,     0,     0,     0,     0,   844,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,     0,   453,   454,   455,   430,   365,     0,
     427,   431,     0,     0,     0,   456,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,     0,   836,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,     0,     0,   453,   454,   455,     0,   365,   430,   427,
       0,     0,   431,     0,   456,     0,   428,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   429,   900,
       0,     0,     0,     0,     0,     0,     0,     0,   214,     0,
       0,     0,   432,     0,   901,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,     0,   453,   454,   455,   430,     0,   427,
       0,   431,     0,     0,     0,   456,   428,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,     0,     0,   453,   454,   455,     0,   430,     0,     0,
       0,   431,     0,     0,   456,     0,     0,     0,     0,     0,
     931,   427,     0,     0,     0,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,   432,     0,     0,     0,  1094,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,     0,  1095,   453,   454,   455,     0,   365,     0,     0,
       0,     0,     0,     0,   456,     0,     0,     0,     0,   430,
     427,     0,     0,   431,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,   434,     0,   435,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,     0,     0,   453,   454,   455,   430,     0,
     427,     0,   431,     0,     0,     0,   456,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,   844,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
     427,   525,     0,     0,     0,     0,     0,   428,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,   429,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,     0,   453,   454,   455,     0,   430,     0,
     427,     0,   431,  1115,     0,   456,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,   844,     0,
       0,     0,   432,     0,     0,     0,     0,     0,   430,     0,
     427,     0,   431,     0,     0,     0,     0,   428,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,   429,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   432,     0,   453,   454,   455,     0,   430,     0,
       0,     0,   431,  1163,     0,   456,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   432,     0,   453,   454,   455,     0,   430,     0,
     427,     0,   431,     0,     0,   456,     0,   428,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,   429,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   432,     0,   453,   454,   455,     0,   548,     0,
     427,     0,     0,     0,     0,   456,     0,   428,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,   429,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,   552,   453,   454,   455,     0,   430,     0,
     427,   809,   431,     0,     0,   456,     0,   428,     0,     0,
       0,   726,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,   430,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   432,     0,   453,   454,   455,     0,   430,     0,
     427,     0,   431,     0,     0,   456,     0,   428,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,   429,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   432,     0,   453,   454,   455,     0,     0,     0,
       0,   792,     0,     0,     0,   456,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   810,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,     0,   453,   454,   455,     0,   430,   427,
       0,     0,   431,     0,     0,   456,   428,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   429,     0,
       0,   929,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   851,   432,     0,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,     0,   428,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,   429,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,     0,   453,   454,   455,   430,     0,     0,
     427,   431,     0,     0,     0,   456,     0,   428,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,   431,     0,     0,     0,     0,     0,   433,   434,
       0,   435,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,     0,   432,   453,   454,   455,     0,     0,   430,     0,
     427,   998,   431,     0,   456,     0,     0,   428,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,   429,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   432,     0,   453,   454,   455,     0,     0,     0,
     427,   943,     0,     0,     0,   456,     0,   428,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,   429,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,     0,   453,   454,   455,     0,   430,     0,
       0,   944,   431,     0,     0,   456,   427,  1093,     0,     0,
       0,     0,     0,   428,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,   430,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   432,     0,   453,   454,   455,     0,     0,     0,
       0,     0,     0,     0,   430,   456,     0,     0,   431,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   427,     0,   453,   454,   455,     0,   432,   428,
       0,  1046,     0,     0,     0,   456,     0,     0,     0,     0,
       0,   429,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   427,     0,
     453,   454,   455,     0,     0,   428,     0,     0,     0,     0,
       0,   456,     0,     0,     0,     0,     0,   429,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,   431,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   427,     0,     0,     0,     0,     0,
       0,   428,     0,     0,     0,     0,     0,     0,     0,     0,
     214,     0,     0,   429,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
     431,   433,   434,     0,   435,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,     0,     0,   453,   454,   455,     0,
     432,     0,     0,     0,     0,     0,     0,   456,     0,     0,
       0,     0,   430,     0,     0,     0,   431,   433,   434,     0,
     435,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     427,  1107,   453,   454,   455,  1118,   432,   428,     0,     0,
       0,     0,     0,   456,     0,     0,     0,     0,     0,   429,
       0,     0,     0,   433,   434,     0,   435,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   427,     0,   453,   454,
     455,     0,     0,   428,     0,     0,     0,     0,     0,   456,
       0,     0,     0,     0,     0,   429,  1149,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,   431,     0,     0,     0,   427,  1168,     0,     0,
       0,     0,     0,   428,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,   431,   433,
     434,     0,   435,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,     0,     0,   453,   454,   455,     0,   432,     0,
       0,     0,     0,  1146,   430,   456,   427,     0,   431,     0,
       0,     0,     0,   428,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,   429,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   432,     0,
     453,   454,   455,     0,     0,     0,   427,     0,     0,     0,
       0,   456,     0,   428,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,   429,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,     0,     0,
     453,   454,   455,     0,   430,     0,   427,     0,   431,     0,
       0,   456,     0,   428,     0,  1204,     0,  1212,     0,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   432,     0,
       0,     0,     0,     0,   430,     0,   427,     0,   431,     0,
       0,     0,     0,   428,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,   429,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   432,     0,
     453,   454,   455,     0,   430,     0,   427,     0,   431,     0,
       0,   456,     0,   428,     0,   433,   434,  1252,   435,   436,
     437,     0,   438,   439,   440,   429,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   432,     0,
     453,   454,   455,     0,   430,     0,   427,     0,   431,     0,
       0,   456,     0,   428,     0,   433,   434,  1253,   435,   436,
     437,     0,   438,   439,   440,   429,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   432,     0,
     453,   454,   455,     0,   430,     0,   427,     0,   431,     0,
       0,   456,     0,   428,     0,   433,   434,  1254,   435,   436,
     437,     0,   438,   439,   440,   429,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   432,     0,
     453,   454,   455,     0,   430,     0,   427,     0,   431,     0,
       0,   456,     0,   428,     0,   433,   434,  1255,   435,   436,
     437,     0,   438,   439,   440,   429,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   432,     0,
     453,   454,   455,     0,   430,     0,     0,     0,   431,     0,
       0,   456,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,   429,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   432,     0,
     453,   454,   455,     0,   919,     0,     0,     0,   431,     0,
       0,   456,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,   429,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   432,     0,
     453,   454,   455,     0,   430,     0,     0,     0,   431,     0,
       0,   456,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   432,     0,
     453,   454,   455,     0,   430,     0,     0,     0,   431,     0,
       0,   456,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
       0,   446,   447,   448,   449,   450,   451,   452,   432,     0,
     453,   454,   455,     0,     0,     0,     0,     0,     0,     0,
       0,   456,     0,     0,     0,   433,   434,     0,   435,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,   444,
       0,   446,   447,   448,   449,   450,     0,   452,     0,     0,
     453,   454,   455,   608,     0,   230,     0,     7,     0,     0,
       0,   456,     0,     0,     0,     0,     0,     0,     0,     0,
     231,   232,     0,   233,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,   235,     0,     0,     0,
       0,     0,   236,     0,     0,     0,     0,     0,   237,     0,
       0,     0,   238,     0,     0,   239,     0,   240,     0,     0,
       0,   230,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,   242,   243,     0,   231,   232,     0,   233,
       0,   244,     0,     0,   234,     0,    24,     0,     0,     0,
       0,   245,   235,     0,     0,     0,     0,     0,   236,     0,
     246,   247,     0,   248,   237,   249,     0,   250,   238,     0,
     251,   239,     0,   240,   252,     0,   253,     0,     0,   254,
       0,     0,   241,     0,     0,     0,     0,     0,     0,   242,
     243,     0,     0,     0,     0,     0,     0,   244,    57,     0,
       0,     0,     0,     0,     0,     0,     0,   245,     0,     0,
      66,     0,     0,   230,     0,     7,   246,   247,     0,   248,
       0,   249,     0,   250,     0,     0,   251,     0,   231,   232,
     252,   233,   253,     0,    86,   254,   234,     0,     0,     0,
       0,     0,     0,     0,   235,     0,     0,     0,     0,     0,
     236,     0,     0,     0,     0,     0,   237,     0,     0,     0,
     238,     0,     0,   239,     0,   240,     0,     0,     0,  -313,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,     0,  -313,  -313,     0,  -313,     0,   244,
       0,     0,  -313,     0,     0,     0,     0,     0,     0,   245,
    -313,     0,     0,     0,     0,     0,  -313,     0,   246,   247,
       0,   248,  -313,   249,     0,   250,  -313,     0,   251,  -313,
       0,  -313,   252,     0,   253,   230,     0,   254,     0,     0,
    -313,     0,     0,     0,     0,     0,     0,  -313,  -313,     0,
     231,   232,     0,   233,     0,  -313,     0,     0,   234,     0,
       0,     0,     0,     0,     0,  -313,   235,     0,     0,     0,
       0,     0,   236,     0,  -313,  -313,     0,  -313,   237,  -313,
       0,  -313,   238,     0,  -313,   239,     0,   240,  -313,     0,
    -313,  -314,     0,  -313,     0,     0,   241,     0,     0,     0,
       0,     0,     0,   242,   243,     0,  -314,  -314,     0,  -314,
       0,   244,     0,     0,  -314,     0,     0,     0,     0,     0,
       0,   245,  -314,     0,     0,     0,     0,     0,  -314,     0,
     246,   247,     0,   248,  -314,   249,     0,   250,  -314,     0,
     251,  -314,     0,  -314,   252,     0,   253,     0,     0,   254,
       0,     0,  -314,     0,     0,     0,     0,     0,     0,  -314,
    -314,     0,     0,     0,     0,     0,     0,  -314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -314,     0,     0,
       0,     0,     0,     0,     0,     0,  -314,  -314,     0,  -314,
       0,  -314,     0,  -314,     0,     0,  -314,     0,     0,     0,
    -314,     0,  -314,     0,     0,  -314
};

static const yytype_int16 yycheck[] =
{
       2,     2,   401,   165,    63,   112,   266,    21,   484,    27,
     584,   418,     2,   546,   609,    63,   573,    63,   508,   393,
     181,   667,   223,   669,    26,   229,    68,   620,   815,   133,
     617,   291,   905,   810,   781,     1,    26,   863,   541,     1,
     203,   383,   775,   403,    33,    33,    88,   946,     1,    67,
       5,    48,    25,     1,   517,    48,   430,   431,    23,   833,
      33,   103,   266,    41,    42,    43,     3,    33,    41,    42,
      43,   331,    57,   871,    76,     0,    48,    76,    80,    81,
      33,    25,   104,    19,   121,   166,    76,   291,     3,    33,
      80,    81,     3,     3,    36,     1,    62,    41,    42,    43,
     102,   262,   476,   477,   106,   124,    25,   119,   121,    62,
      33,    33,   102,    33,    33,    33,   106,    33,    55,   118,
      92,   121,    41,    42,    43,    90,    57,   331,    76,   599,
      57,    57,     2,   603,   604,   111,   148,  1010,   142,   176,
      55,   107,    48,    79,    55,    55,   150,   120,   113,   171,
     948,   148,   171,   119,   107,   148,    26,   620,    57,   148,
     148,    48,    98,   176,   126,     2,   119,    91,   105,   111,
     118,    95,   121,   172,   761,  1001,   120,   537,   114,   176,
    1079,   147,   171,   171,    94,   147,   148,   744,   173,    26,
     105,    48,   939,   148,   147,   105,   570,   175,   121,   946,
     166,   120,   175,   121,   367,   171,    76,  1033,  1034,    29,
      80,    81,   174,   215,   474,   475,   949,   172,   171,   996,
     126,    41,    42,    43,   172,   215,  1000,   176,    47,    28,
      28,   175,   102,    92,   121,   439,   106,   727,    62,    76,
     814,   147,   148,    80,    81,   587,   166,   173,   171,   171,
      48,   171,    57,   171,    52,   171,   175,   418,    28,   512,
     166,   148,    48,    57,   121,   102,   148,    86,   270,   106,
     474,   475,   173,   275,   148,  1101,   779,    32,   382,    77,
     270,    80,    80,   148,    82,   275,   539,    28,   148,   176,
     550,   148,   174,    92,    92,   119,   115,   148,    47,   148,
    1173,  1127,   176,    28,   508,   166,    92,    77,  1095,   174,
      80,   172,  1059,   111,   174,    57,     3,   116,    67,   176,
      12,   172,    92,    48,   525,   174,   488,    52,   529,   799,
     150,   918,  1079,   148,    21,   148,    91,    86,   148,    80,
      95,   111,    47,   402,   164,   215,   550,    34,   173,   351,
     351,    92,    77,   166,   947,    80,    91,    82,    57,   174,
      95,   351,    67,   365,   174,   148,   115,    92,    55,   573,
     111,   861,   374,   105,   392,   365,   174,    64,   215,   199,
     398,    86,    54,   148,   374,   387,   111,   142,    57,   590,
    1135,   174,   147,   126,   149,   150,   173,   387,   175,   154,
     270,  1047,  1048,    75,   148,   275,   161,    57,     2,   174,
     115,   968,   902,    85,   169,   989,   418,   148,   105,   152,
     148,   463,   464,   173,   111,   175,   148,   469,   172,   905,
     472,   473,    26,   270,   148,   148,    80,    81,   275,   259,
     260,   148,  1037,   174,    32,   148,   174,    57,   148,   174,
     148,   271,   174,  1198,  1199,  1200,  1201,  1202,  1203,   148,
     174,   174,   836,   481,   863,   148,   148,   174,   166,   166,
     148,   174,     9,   847,   174,   882,   173,   166,   148,   538,
     148,   351,    76,   857,   166,   173,    80,    81,   166,   172,
     538,   311,   538,    80,    81,   365,   166,   499,   166,   501,
     166,   148,   609,    91,   374,    57,   172,    95,   102,   499,
      84,   501,   106,    87,   351,   148,   102,   387,    28,   166,
    1094,   563,   564,   727,   898,   292,   293,   901,   365,   148,
     148,   126,    57,   166,  1010,   148,  1131,   374,    48,   148,
     744,   361,    52,  1013,  1014,   919,   920,   166,   166,     2,
     387,   569,   173,   166,   142,  1025,  1026,   166,   153,   147,
     155,   156,   150,   158,   159,    91,   154,    77,    57,    95,
      80,    57,    82,    26,    57,   577,   577,   397,   592,    57,
     142,   401,    92,    28,   173,   147,   175,   577,   150,     5,
     166,   965,   154,   967,   563,   564,    91,   173,   418,   175,
      95,   111,  1001,    48,   150,   809,   810,    52,  1141,    91,
      91,   773,   171,    95,    95,    32,    57,  1087,  1088,  1089,
    1090,   215,   173,    76,   175,     3,   147,    80,    81,   499,
     171,   501,    77,   173,  1033,    80,   171,    82,   173,  1038,
      18,    46,   173,    21,   175,    57,   166,    92,    26,   102,
     173,   165,   175,   106,   166,    32,    51,   861,    36,    54,
     173,  1194,   499,   483,   501,    70,   111,   173,    46,   175,
     166,   171,   166,   835,    91,    53,   270,    55,    95,   166,
      75,   275,   166,    78,    86,   173,    64,   166,   173,   509,
      85,   166,   512,   166,    72,   166,   173,  1173,   902,   148,
     742,   147,  1101,   166,   174,   525,   173,   577,   166,   148,
     174,    89,     9,     3,    91,   535,   111,   173,    95,   539,
      67,   882,   764,   101,    80,   142,   143,   105,   145,   146,
     147,   171,   149,   150,   112,   737,   126,   154,    92,   117,
     577,    92,   166,   160,   161,    70,   748,   164,   173,   751,
     167,   168,   169,   150,   126,   174,   148,   351,   748,   126,
     126,   751,   215,   583,   968,   142,   143,    35,   145,   146,
     147,   365,   149,   150,   594,    35,    67,   154,   174,   147,
     374,   126,    22,   174,   161,   174,   171,   120,   148,   609,
     167,   168,   169,   387,   173,   126,   172,   107,   173,   173,
     150,   150,   150,   166,   863,   142,   150,   150,   150,   150,
     869,   150,  1072,  1073,   150,   150,   150,   270,   150,   878,
     150,   150,   275,   150,   842,    35,   166,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   166,   845,   166,    32,   166,   849,   166,    35,
     176,   174,   147,   673,   856,   845,   172,   174,   166,   849,
     148,   173,   171,    48,    35,   176,   856,   172,  1072,  1073,
     176,   170,   174,   278,   173,   280,   175,   166,   748,   166,
     176,   751,   147,   166,   148,    33,    35,   173,   173,   166,
     174,   148,   165,   174,   176,   166,   176,   176,   351,   174,
     174,   572,   165,   111,    91,   499,   968,   501,    95,   374,
     265,   748,   365,   807,   751,    25,   367,   919,   920,   391,
     935,   374,   327,   961,   926,   340,   928,  1034,   385,   931,
     523,   538,   882,   753,   387,   878,   926,    32,   928,   957,
     760,   931,  1001,   305,  1194,  1040,  1204,   673,   869,  1197,
     770,   834,   131,   520,   956,   142,   143,   760,  1119,   146,
     147,    -1,   149,   150,   966,    -1,   956,   154,    -1,   789,
      -1,    -1,    -1,    -1,   161,   845,   966,    -1,    -1,   849,
     167,   168,   169,   577,    -1,    -1,   856,    -1,    -1,    -1,
    1004,  1005,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      95,     2,    -1,    -1,    -1,    -1,    -1,    -1,   845,    -1,
    1012,    -1,   849,    -1,   834,    -1,  1018,    -1,    -1,   856,
    1127,    -1,  1012,    -1,  1131,    26,    -1,    -1,  1018,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1101,    -1,    -1,    -1,   499,   142,   501,    -1,
      -1,    -1,   147,    -1,   149,   150,   926,    -1,   928,   154,
      -1,   931,   882,  1224,  1066,  1226,   161,    -1,  1229,  1230,
      -1,    -1,   167,   168,   169,    76,  1066,    -1,    -1,    80,
      81,    -1,    -1,    -1,    -1,    -1,   956,    -1,    91,   926,
      -1,   928,    95,    -1,   931,    -1,   966,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,  1113,    -1,  1115,    -1,   935,    -1,    -1,  1120,   956,
      -1,  1123,    -1,  1113,   577,  1115,    -1,    -1,    -1,   966,
    1120,    -1,    -1,  1123,    -1,    -1,    -1,    -1,    -1,   142,
     143,   961,  1012,    -1,   147,    -1,   149,   150,  1018,    -1,
    1152,   154,    -1,    -1,   748,    -1,  1158,   751,   161,    -1,
    1162,  1163,  1152,    -1,   167,   168,   169,    -1,  1158,    -1,
      -1,    -1,  1162,  1163,    -1,  1012,  1178,    -1,    -1,    -1,
    1182,  1018,  1184,    -1,    -1,    -1,    -1,    -1,  1178,    -1,
     595,   596,  1182,    -1,  1184,    -1,  1066,    -1,   603,   604,
      -1,    -1,    -1,  1205,  1205,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   215,  1205,    -1,  1037,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1066,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1113,     2,  1115,  1248,  1249,  1250,  1251,
    1120,   845,    -1,  1123,    -1,   849,    -1,    -1,  1248,  1249,
    1250,  1251,   856,    -1,    -1,    -1,    -1,    -1,    26,   270,
      -1,    -1,    -1,    -1,   275,    -1,  1113,    -1,  1115,    -1,
      -1,    -1,  1152,  1120,    -1,    -1,  1123,    -1,  1158,    -1,
      -1,    -1,  1162,  1163,    -1,   748,    -1,    -1,   751,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1178,    -1,
      -1,  1131,  1182,    -1,  1184,  1152,     1,    -1,    76,    -1,
       5,  1158,    80,    81,    -1,  1162,  1163,    -1,    -1,    -1,
      -1,    -1,   926,    -1,   928,  1205,    -1,   931,    23,    -1,
      -1,  1178,    27,    28,   102,  1182,    -1,  1184,   106,    -1,
     351,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,
      -1,    -1,   956,    -1,   365,    -1,    51,    -1,  1205,    54,
      -1,    -1,   966,   374,    -1,    -1,    -1,    -1,  1248,  1249,
    1250,  1251,    -1,    -1,    -1,     2,   387,    -1,    73,    -1,
      75,    -1,   845,    78,   799,    80,   849,    82,    -1,    84,
      85,    -1,    87,   856,    -1,    90,    -1,    92,    -1,    26,
      -1,  1248,  1249,  1250,  1251,    -1,    -1,    -1,  1012,    -1,
      -1,    -1,     5,    -1,  1018,    -1,   111,    -1,   113,    -1,
      -1,   116,   837,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
     855,    -1,    -1,    36,   859,    38,    39,   215,    -1,    76,
      -1,    44,    -1,    80,    81,    -1,    -1,    -1,    51,    -1,
      -1,    -1,  1066,   926,    -1,   928,    -1,    -1,   931,    -1,
      -1,   166,    -1,    -1,    -1,   102,    -1,   172,    -1,   106,
      73,    -1,    -1,    -1,    -1,    78,    -1,    80,   499,    82,
     501,    84,    -1,   956,    87,    -1,    -1,    90,    -1,    92,
      -1,    -1,   270,   966,    -1,    -1,    -1,   275,    -1,  1113,
      -1,  1115,    -1,    -1,    -1,    -1,  1120,    -1,   111,  1123,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1152,  1012,
      -1,    -1,    -1,     5,  1158,  1018,    -1,    26,  1162,  1163,
      -1,    -1,    -1,    -1,    -1,    -1,   577,    -1,    -1,    68,
      -1,    23,    -1,   166,  1178,    27,    28,    -1,  1182,   172,
    1184,    -1,    -1,   351,    36,    -1,    38,    39,   215,    88,
      -1,    -1,    44,    -1,    -1,    -1,    -1,   365,    -1,    51,
      -1,  1205,    -1,  1066,   103,    -1,   374,    76,    -1,    -1,
    1025,  1026,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,
      -1,    73,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    84,   102,    -1,    87,    -1,   106,    90,    -1,
      92,    -1,    -1,   270,  1248,  1249,  1250,  1251,   275,    -1,
    1113,    -1,  1115,    -1,    -1,    -1,    -1,  1120,    -1,   111,
    1123,   113,    -1,    -1,   116,    -1,    -1,    -1,  1083,    -1,
      -1,    -1,    -1,    -1,  1089,  1090,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1152,
    1105,  1106,    -1,  1108,    -1,  1158,    -1,    -1,    -1,  1162,
    1163,    -1,    -1,    -1,    -1,    -1,    -1,  1122,    -1,    -1,
    1125,    -1,    -1,    -1,   166,  1178,    -1,    -1,    -1,  1182,
     172,  1184,    -1,    -1,   351,    -1,    -1,    -1,    -1,    -1,
     229,   499,    -1,   501,    -1,    -1,    -1,   748,   365,    -1,
     751,    -1,  1205,    -1,    -1,    -1,   215,   374,    -1,    -1,
      -1,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,
     387,    -1,    -1,    -1,    -1,    -1,    -1,   266,    18,    19,
      -1,    21,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,  1248,  1249,  1250,  1251,    -1,
      40,    -1,   291,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,   270,    -1,    53,    -1,    55,   275,    -1,    -1,   577,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    23,   331,    -1,   845,    27,    28,    -1,   849,    89,
      -1,    -1,    -1,    -1,    36,   856,    38,    39,    98,    99,
      -1,   101,    44,   103,    -1,   105,    -1,    -1,   108,    51,
      -1,    -1,   112,    -1,   114,    -1,    -1,   117,    -1,    -1,
      -1,    -1,   499,    -1,   501,    -1,    -1,    -1,    -1,    -1,
      -1,    73,   351,    -1,    -1,    -1,    78,    -1,    80,    -1,
      -1,     3,    -1,    -1,    -1,    -1,   365,    -1,    90,    -1,
      92,    -1,    -1,    -1,    -1,   374,    18,    19,    -1,    21,
      -1,    -1,    -1,    -1,    26,   926,    -1,   928,   387,   111,
     931,   113,    34,   173,   116,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,   956,    -1,    -1,    -1,    -1,
     577,    -1,    64,    -1,    -1,   966,    -1,    -1,    -1,    71,
      72,    -1,    -1,    -1,   463,   464,    -1,    79,    -1,    -1,
     469,    -1,    -1,   472,   473,   474,   475,    89,    -1,    -1,
     748,     5,    -1,   751,    -1,    -1,    98,    99,    -1,   101,
      -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    23,
     112,  1012,   114,    27,    28,   117,    -1,  1018,    -1,   508,
      -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
     499,    -1,   501,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,
      -1,   550,    -1,    -1,    78,  1066,    80,    -1,    82,   171,
      84,   173,    -1,    87,   563,   564,    90,    -1,    92,    -1,
      -1,    -1,    -1,    -1,   573,    -1,    -1,   845,    -1,    -1,
      -1,   849,    -1,    -1,    -1,    -1,    -1,   111,   856,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,     5,    -1,    -1,
      -1,    -1,  1113,    -1,  1115,    -1,    -1,    -1,   577,  1120,
      -1,    -1,  1123,    -1,    -1,    23,    -1,    -1,    -1,    27,
      28,   748,    -1,    -1,   751,    -1,    -1,    -1,    36,    -1,
      38,    39,    -1,    -1,    -1,    -1,    44,    -1,    32,    -1,
      -1,  1152,   166,    51,    -1,    -1,    -1,  1158,   172,    -1,
      -1,  1162,  1163,    -1,    -1,    -1,    -1,    -1,   926,    -1,
     928,    -1,    -1,   931,    -1,    73,    -1,  1178,    -1,    -1,
      78,  1182,    80,  1184,    82,    -1,    84,    -1,    -1,    87,
      -1,    -1,    90,    -1,    92,    -1,    -1,    -1,   956,    -1,
      -1,    -1,    -1,    -1,  1205,    -1,    -1,    91,   966,    -1,
      -1,    95,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   845,    -1,
      -1,    -1,   849,    -1,    -1,    -1,    -1,    -1,   727,   856,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1248,  1249,  1250,
    1251,    -1,    -1,   742,  1012,   744,    -1,    -1,   142,   143,
    1018,   145,   146,   147,    -1,   149,   150,   151,   166,    -1,
     154,    -1,    -1,    -1,    -1,   764,   160,   161,    -1,    -1,
     164,    -1,     3,   167,   168,   169,    -1,    -1,    -1,   748,
      -1,    -1,   751,    -1,   178,    -1,    -1,    18,    19,    -1,
      21,    -1,    -1,    -1,    -1,    26,    -1,    -1,  1066,   926,
      -1,   928,    -1,    34,   931,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,   956,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   966,
      71,    72,    -1,    -1,    -1,  1113,    -1,  1115,    79,    -1,
      -1,    26,  1120,    -1,    -1,  1123,    -1,    -1,    89,    -1,
      -1,    -1,   861,    -1,    -1,    -1,    -1,    98,    99,    -1,
     101,    -1,   103,    -1,   105,    -1,   845,   108,    -1,    -1,
     849,   112,    -1,   114,  1152,  1012,   117,   856,    -1,    -1,
    1158,  1018,    -1,    -1,  1162,  1163,    -1,    -1,    -1,    -1,
      -1,    76,    -1,   902,    -1,    -1,    -1,    -1,    -1,    -1,
    1178,    -1,    -1,    -1,  1182,    -1,  1184,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,     5,    -1,    -1,    -1,  1205,    -1,  1066,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    27,    28,   926,    -1,   928,
      -1,    -1,   931,    -1,    36,    -1,    38,    39,    -1,   968,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,
    1248,  1249,  1250,  1251,    -1,    -1,  1113,   956,  1115,    -1,
      -1,    -1,    -1,  1120,    -1,    -1,  1123,   966,    -1,    -1,
      -1,    73,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      -1,    -1,    84,    -1,    -1,    87,    -1,    -1,    90,    -1,
      92,    -1,    -1,    -1,    -1,  1152,    -1,    -1,    -1,    -1,
      -1,  1158,    -1,    -1,    -1,  1162,  1163,    -1,    -1,   111,
     215,   113,    -1,  1012,   116,    -1,    -1,    -1,    -1,  1018,
      -1,  1178,    -1,    -1,    -1,  1182,    -1,  1184,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1072,  1073,    -1,    -1,    -1,  1205,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    -1,   166,   270,    -1,  1066,    27,    -1,
     275,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    48,
      49,  1248,  1249,  1250,  1251,    54,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    67,    68,
      69,    70,    71,    72,  1113,    -1,  1115,    76,    -1,    78,
      -1,  1120,    -1,    -1,  1123,    -1,    85,    -1,    -1,    88,
      -1,    90,    -1,    92,    -1,    -1,    95,    96,    -1,    98,
      -1,    -1,    -1,   102,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1152,    -1,    -1,    -1,    -1,    -1,  1158,
      -1,    -1,    -1,  1162,  1163,    -1,    -1,    -1,    -1,    -1,
      -1,     3,   131,    -1,   133,    -1,    -1,    -1,    -1,  1178,
      -1,    -1,   387,  1182,    -1,  1184,    18,    19,    -1,    21,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,  1205,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,   174,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,   197,    71,
      72,    -1,   201,   202,    -1,    -1,    -1,    79,    -1,  1248,
    1249,  1250,  1251,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,   101,
     229,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
     112,    -1,   114,    -1,    -1,   117,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    -1,   499,    -1,   501,    -1,    -1,    -1,
      -1,    -1,    18,    19,    -1,    21,    -1,   266,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,   291,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,   173,    -1,   302,   303,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    -1,   317,   318,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   331,    89,    -1,    -1,   335,    -1,    -1,    -1,
      -1,    -1,    98,    99,    -1,   101,    -1,   103,    -1,   105,
      -1,    -1,   108,    -1,   353,    -1,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,   363,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,   377,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   386,    -1,    -1,
      -1,    -1,   391,   392,   393,    -1,    -1,    -1,    -1,   398,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   427,   428,
     429,   430,   431,   432,    46,   434,   435,   436,   437,   438,
      -1,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,    70,    -1,
      -1,    -1,    -1,    -1,   463,   464,    -1,    -1,    80,    81,
     469,    -1,    -1,   472,   473,   474,   475,   476,   477,    -1,
      -1,    -1,   481,   482,    -1,   484,    -1,    -1,    -1,    -1,
      -1,   490,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    -1,   501,   748,    -1,   504,   751,    -1,    -1,   508,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   517,    -1,
     519,    -1,    -1,    -1,   523,    -1,    -1,   526,   527,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   538,
      -1,    -1,   541,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   550,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   560,    -1,    -1,   563,   564,    -1,    -1,    -1,    -1,
     569,   570,    -1,    -1,   573,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   584,   585,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   598,
     845,    -1,    -1,    -1,   849,    -1,    -1,    -1,    -1,   221,
      -1,   856,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   620,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    27,    28,   261,
     262,    -1,    -1,    -1,    -1,    -1,    36,   269,    38,    39,
      -1,    -1,    -1,    -1,    44,   664,   278,    -1,   280,    -1,
      -1,    51,   671,    -1,    54,   287,    -1,    -1,    -1,    -1,
      -1,   926,    -1,   928,    -1,    -1,   931,    67,    -1,    -1,
      -1,    -1,    -1,    73,    -1,    75,    -1,    -1,    78,    -1,
      80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    -1,
      90,   956,    92,    -1,    -1,   327,    -1,    -1,    -1,    -1,
      -1,   966,    -1,    -1,    -1,    -1,    -1,   726,   727,    -1,
      -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,   742,    -1,   744,   745,   746,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   764,    -1,  1012,    -1,    -1,
      -1,    -1,    -1,  1018,    -1,    -1,    -1,    -1,    -1,    -1,
     779,    -1,    -1,    -1,   783,    -1,   166,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   807,    -1,
      -1,    -1,   811,    -1,   813,   814,   815,    -1,   817,    -1,
     819,  1066,    -1,   822,   823,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   836,    -1,   838,
      -1,    -1,    -1,   842,    -1,    -1,    -1,    -1,   847,   848,
     849,    -1,   851,   852,    -1,    -1,    -1,   856,   857,    -1,
      -1,    -1,   861,    -1,    -1,    -1,    -1,   866,  1113,    -1,
    1115,    -1,    -1,    -1,    -1,  1120,    -1,    -1,  1123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   898,
     899,   900,   901,   902,    -1,    -1,   905,  1152,    -1,    -1,
      -1,    -1,    -1,  1158,    -1,    -1,    13,  1162,  1163,    -1,
     919,   920,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
     929,    -1,    -1,  1178,    -1,    32,    -1,  1182,    -1,  1184,
      37,   553,    -1,    -1,    -1,    -1,    -1,    -1,   947,    -1,
      -1,    -1,   951,    -1,    -1,    -1,    -1,    -1,   957,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   965,    -1,   967,   968,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    -1,    -1,   595,   596,    -1,    -1,    -1,    -1,    -1,
     989,   603,   604,    -1,    91,    -1,    -1,   996,    95,   998,
      -1,    -1,    -1,  1248,  1249,  1250,  1251,    -1,    -1,  1008,
      -1,  1010,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1023,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1042,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,  1054,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,   166,
     167,   168,   169,  1072,  1073,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,  1085,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1093,  1094,  1095,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1113,    -1,  1115,    -1,    -1,  1118,
      -1,  1120,    -1,    -1,  1123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     752,    -1,    -1,    -1,    -1,    -1,  1145,  1146,    -1,  1148,
    1149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1168,
      -1,    -1,    -1,    -1,  1173,    -1,  1175,   789,    -1,    -1,
    1179,    -1,    -1,    -1,    -1,  1184,  1185,   799,    -1,  1188,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1196,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1210,  1211,  1212,  1213,  1214,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   837,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   855,    -1,    -1,    -1,   859,    -1,  1248,
    1249,  1250,  1251,  1252,  1253,  1254,  1255,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     0,     1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    17,    18,    19,    -1,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,   936,    49,    50,    51,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    69,    -1,    71,    72,    73,
      -1,    75,   964,    -1,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,   105,   106,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,    -1,   120,    -1,   122,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,  1025,  1026,    -1,    20,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,   166,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,   175,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1083,    -1,    -1,    -1,    -1,    -1,  1089,  1090,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    95,    -1,  1105,  1106,    -1,  1108,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,  1119,    -1,    -1,
    1122,    -1,    -1,  1125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,    -1,    -1,
     134,   135,   136,   137,    -1,   139,   140,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,  1160,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1224,    -1,  1226,    -1,    -1,  1229,  1230,     1,
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    11,
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
      -1,   171,    -1,   173,     1,   175,     3,     4,   178,     6,
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
      -1,    32,    77,    -1,    79,    80,    81,    82,    83,    -1,
      -1,    -1,    -1,    88,    -1,    90,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    98,    -1,   100,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,   174,
     175,    -1,     4,   178,     6,     7,     8,     9,    10,    11,
      -1,   142,   143,    15,   145,   146,   147,    19,   149,   150,
     151,    23,    -1,   154,   155,   156,    28,   158,   159,   160,
     161,    -1,    -1,   164,    36,    -1,   167,   168,   169,    41,
      42,    43,    -1,    45,    -1,    -1,    48,   178,    -1,    -1,
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
      -1,    13,    -1,    -1,    -1,    -1,    18,    19,    20,    21,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,   142,    -1,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    56,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    64,    -1,    -1,   173,    -1,   175,    -1,    -1,
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
      13,    14,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    32,
      -1,   125,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    74,    -1,   167,   168,   169,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    91,    13,
      -1,    -1,    95,    -1,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    91,    -1,    -1,
      13,    95,    -1,    -1,    -1,   178,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,   171,    91,    13,
      -1,    -1,    95,    -1,   178,    -1,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    91,   171,    -1,
      13,    95,    -1,    -1,    -1,   178,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,   171,    91,    13,
      -1,    -1,    95,    -1,   178,    -1,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,   125,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    91,    -1,    13,
      -1,    95,    -1,    -1,    -1,   178,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,    91,    -1,    -1,
      -1,    95,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
     104,    13,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,   125,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    74,   167,   168,   169,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    91,
      13,    -1,    -1,    95,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    91,    -1,
      13,    -1,    95,    -1,    -1,    -1,   178,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    14,    -1,    -1,    -1,    -1,    -1,    20,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    32,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,    91,    -1,
      13,    -1,    95,   176,    -1,   178,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      13,    -1,    95,    -1,    -1,    -1,    -1,    20,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    32,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   125,    -1,   167,   168,   169,    -1,    91,    -1,
      -1,    -1,    95,   176,    -1,   178,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   125,    -1,   167,   168,   169,    -1,    91,    -1,
      13,    -1,    95,    -1,    -1,   178,    -1,    20,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    32,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   125,    -1,   167,   168,   169,    -1,   171,    -1,
      13,    -1,    -1,    -1,    -1,   178,    -1,    20,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    32,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,   166,   167,   168,   169,    -1,    91,    -1,
      13,    14,    95,    -1,    -1,   178,    -1,    20,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   125,    -1,   167,   168,   169,    -1,    91,    -1,
      13,    -1,    95,    -1,    -1,   178,    -1,    20,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    32,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   125,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,   174,    -1,    -1,    -1,   178,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,    91,    13,
      -1,    -1,    95,    -1,    -1,   178,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    32,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    91,    -1,    -1,
      13,    95,    -1,    -1,    -1,   178,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   125,   167,   168,   169,    -1,    -1,    91,    -1,
      13,    14,    95,    -1,   178,    -1,    -1,    20,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    32,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   125,    -1,   167,   168,   169,    -1,    -1,    -1,
      13,   174,    -1,    -1,    -1,   178,    -1,    20,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    32,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,    91,    -1,
      -1,   174,    95,    -1,    -1,   178,    13,    14,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   125,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,   178,    -1,    -1,    95,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    13,    -1,   167,   168,   169,    -1,   125,    20,
      -1,   174,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    13,    -1,
     167,   168,   169,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    32,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      95,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    95,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      13,   166,   167,   168,   169,   124,   125,    20,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    13,    -1,   167,   168,
     169,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    13,    14,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,   125,    -1,
      -1,    -1,    -1,   176,    91,   178,    13,    -1,    95,    -1,
      -1,    -1,    -1,    20,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    32,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   125,    -1,
     167,   168,   169,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      -1,   178,    -1,    20,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    32,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,    91,    -1,    13,    -1,    95,    -1,
      -1,   178,    -1,    20,    -1,    62,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    13,    -1,    95,    -1,
      -1,    -1,    -1,    20,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    32,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   125,    -1,
     167,   168,   169,    -1,    91,    -1,    13,    -1,    95,    -1,
      -1,   178,    -1,    20,    -1,   142,   143,   104,   145,   146,
     147,    -1,   149,   150,   151,    32,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   125,    -1,
     167,   168,   169,    -1,    91,    -1,    13,    -1,    95,    -1,
      -1,   178,    -1,    20,    -1,   142,   143,   104,   145,   146,
     147,    -1,   149,   150,   151,    32,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   125,    -1,
     167,   168,   169,    -1,    91,    -1,    13,    -1,    95,    -1,
      -1,   178,    -1,    20,    -1,   142,   143,   104,   145,   146,
     147,    -1,   149,   150,   151,    32,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   125,    -1,
     167,   168,   169,    -1,    91,    -1,    13,    -1,    95,    -1,
      -1,   178,    -1,    20,    -1,   142,   143,   104,   145,   146,
     147,    -1,   149,   150,   151,    32,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   125,    -1,
     167,   168,   169,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,   178,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    32,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   125,    -1,
     167,   168,   169,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,   178,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    32,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   125,    -1,
     167,   168,   169,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,   178,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   125,    -1,
     167,   168,   169,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,   178,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,   125,    -1,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,   162,    -1,   164,    -1,    -1,
     167,   168,   169,     1,    -1,     3,    -1,     5,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    19,    -1,    21,    -1,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    -1,    18,    19,    -1,    21,
      -1,    79,    -1,    -1,    26,    -1,    28,    -1,    -1,    -1,
      -1,    89,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      98,    99,    -1,   101,    46,   103,    -1,   105,    50,    -1,
     108,    53,    -1,    55,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      92,    -1,    -1,     3,    -1,     5,    98,    99,    -1,   101,
      -1,   103,    -1,   105,    -1,    -1,   108,    -1,    18,    19,
     112,    21,   114,    -1,   116,   117,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    -1,    18,    19,    -1,    21,    -1,    79,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    98,    99,
      -1,   101,    46,   103,    -1,   105,    50,    -1,   108,    53,
      -1,    55,   112,    -1,   114,     3,    -1,   117,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,
      18,    19,    -1,    21,    -1,    79,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    98,    99,    -1,   101,    46,   103,
      -1,   105,    50,    -1,   108,    53,    -1,    55,   112,    -1,
     114,     3,    -1,   117,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    -1,    18,    19,    -1,    21,
      -1,    79,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      98,    99,    -1,   101,    46,   103,    -1,   105,    50,    -1,
     108,    53,    -1,    55,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,   101,
      -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
     112,    -1,   114,    -1,    -1,   117
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
     351,   353,   354,   355,   363,   364,   365,   366,   367,   368,
     370,   373,   374,   375,   376,   377,   378,   379,   380,   382,
     383,   384,   385,   386,   166,    23,    36,    45,    55,    57,
      90,   103,   171,   240,   250,   277,   347,   354,   364,   365,
     370,   373,   375,   376,   121,   356,   357,     3,    55,   219,
     370,   356,   111,   326,    92,   219,   187,   341,   370,   173,
       3,    18,    19,    21,    26,    34,    40,    46,    50,    53,
      55,    64,    71,    72,    79,    89,    98,    99,   101,   103,
     105,   108,   112,   114,   117,   220,   221,   223,    12,    28,
     116,   246,   370,    84,    87,   203,   173,   105,   221,   221,
     221,   173,   221,   328,   329,    33,   207,   226,   370,   262,
     263,   370,   370,    19,    79,    98,   114,   370,   370,   370,
       9,   173,   230,   229,    28,    34,    48,    50,    52,    77,
      80,    92,    99,   103,   111,   186,   225,   281,   282,   283,
     305,   306,   307,   333,   338,   370,   341,   109,   110,   166,
     285,   286,   369,   370,   372,   370,   226,   370,   370,   370,
     102,   173,   187,   370,   370,    73,   189,   193,   207,   189,
     207,   370,   372,   370,   370,   370,   370,   370,     1,   172,
     185,   208,   341,   152,   342,   343,   372,   370,    82,   187,
      23,    36,    39,    73,    90,   171,   190,   191,   192,   203,
     207,   195,   150,   197,   171,    47,    86,   115,   204,    27,
     325,   370,     9,   266,   370,   371,    25,    33,    41,    42,
      43,   120,   175,   242,   243,   350,   352,    57,   147,   268,
     221,   171,   173,   301,    54,    75,    85,   309,    28,    77,
      80,    92,   111,   310,    28,    80,    92,   111,   308,   221,
     321,   322,     1,   328,   165,   166,   370,    13,    20,    32,
      91,    95,   125,   142,   143,   145,   146,   147,   149,   150,
     151,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   167,   168,   169,   178,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   170,   297,   173,   175,    91,    95,   370,   221,
      41,   175,   242,    57,   173,   187,   166,   207,   324,   166,
     148,   166,   222,   344,   345,   346,   372,   221,   221,   104,
     207,   104,   126,   207,   297,   204,   344,   166,   173,   173,
     207,   187,   173,   221,   330,   331,     1,   147,   337,    48,
     148,   187,   226,   148,   226,    14,   173,   173,   226,   327,
     344,   231,   231,    48,    92,   306,   307,   174,   148,   173,
     221,   147,   166,   370,   370,   124,   287,   166,   171,   226,
     173,   344,   166,   370,   255,   255,   166,   172,   172,   185,
     148,   172,   370,   148,   148,   174,   174,     9,   370,   175,
     242,   243,     3,   173,   198,     1,   172,   208,   215,   216,
     370,   210,   370,    67,    37,    74,   166,   266,   268,   111,
     237,   290,   370,   187,    80,   246,   370,   123,   176,   244,
     341,   370,   381,   244,   370,   221,   341,   171,     1,   194,
     221,   272,   275,   174,   302,   304,   305,   310,    92,     1,
     147,   335,   336,    92,     1,     3,    13,    18,    20,    21,
      26,    46,    53,    55,    56,    64,    72,    89,   101,   105,
     112,   117,   142,   143,   144,   145,   146,   147,   149,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   164,
     167,   168,   169,   170,   173,   221,   294,   295,   296,   297,
     347,   126,   323,   148,   166,   166,   166,   370,   370,   370,
     244,   370,   244,   370,   370,   370,   370,   370,   370,   370,
       3,    21,    34,    64,   105,   111,   222,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,    70,   372,   372,   372,   372,   372,
     344,   344,   244,   370,   244,   370,   104,   173,   341,   370,
     221,    28,    48,    92,   116,   358,   361,   362,   370,   386,
     328,   370,   126,   174,   148,   126,   126,   187,    35,   187,
     370,    35,   370,    67,   174,   345,   221,   248,   249,   331,
     148,   174,   225,   370,   126,   332,   370,   329,   262,   221,
     324,   370,   370,   324,   174,   221,   310,   283,   331,   147,
     339,   340,   338,   288,   206,     1,   260,   344,   174,    22,
     256,   172,   174,   174,   342,   372,   174,   342,   341,   370,
     197,   174,   199,   200,   346,   172,   172,   148,   166,    14,
     150,   148,   166,    14,    37,    74,   221,   142,   143,   144,
     145,   146,   160,   164,   169,   209,   296,   297,   298,   370,
     209,   211,   268,   171,   238,   324,    48,   356,   120,   221,
     226,   226,   173,   370,   121,   176,   359,    48,   148,   176,
     359,   124,   148,   176,   226,   359,    33,    48,   226,   359,
     166,   173,     1,   269,   172,   275,   126,   148,   172,   148,
     174,   337,     1,   225,   333,   334,   107,   311,   173,   300,
     370,   142,   150,   300,   300,   370,   322,   173,   175,   166,
     166,   166,   166,   166,   166,   174,   176,   345,    48,   176,
      33,    48,   173,    48,    92,   148,   174,    18,    21,    26,
      46,    53,    64,    72,    89,   101,   112,   117,   347,    91,
      91,   166,   372,   346,   370,   370,    35,   187,    35,    35,
     187,   104,   207,   221,   174,   148,   174,   174,   330,   337,
      70,   372,   221,   174,   174,   328,   340,   147,   284,   174,
     338,   152,   299,   332,   370,   172,    76,   118,   172,   261,
     174,   173,   207,   221,   257,    48,   176,    48,   148,   174,
     215,   222,    18,    19,    21,    26,    46,    50,    53,    72,
      79,    89,    98,    99,   101,   103,   112,   114,   117,   171,
     214,   298,   370,   370,   209,   211,   148,   166,    14,   166,
     171,   269,   321,   328,   244,   370,   226,   370,    48,   341,
     173,   187,   176,   244,   370,   176,   187,   370,   176,   370,
     176,   370,   226,    45,   370,   244,   370,   226,   345,   172,
      84,    87,   172,   186,   194,   228,   370,   273,   274,   304,
     311,    62,   119,   315,   302,   303,   174,   295,   297,   174,
     176,   174,   244,   370,    45,   244,   370,   345,   361,   337,
     347,   347,   187,   187,   370,   187,    35,   166,   166,   249,
     207,   332,   173,   173,   166,   299,   332,   333,   311,   340,
     370,   187,   240,   341,   257,   147,   207,   244,   370,   244,
     370,   200,   209,    14,    37,    74,   166,   166,   298,   370,
     370,   269,   172,   166,   166,   356,   356,   166,   370,   174,
     360,   361,   187,   176,   359,   176,   359,   187,   124,   370,
      33,   226,   359,    33,   226,   359,   174,   194,   228,   228,
     275,   194,   315,     3,    55,    94,   105,   316,   317,   318,
     370,   291,   174,   300,   300,   176,   176,   370,    33,    33,
     174,   332,    35,   187,   344,   344,   299,   332,    33,   226,
     174,   370,   176,   176,   172,   370,   209,   211,    14,   172,
     226,   226,   226,   148,   174,    45,   187,   370,   176,    45,
     187,   370,   176,   370,   104,    45,   370,   226,    45,   370,
     226,   166,   228,   275,   278,   318,   119,   148,   126,   153,
     155,   156,   158,   159,    62,    33,   166,   206,   312,   313,
      45,    45,   104,    45,    45,   187,   174,   174,   187,   207,
     166,   166,   370,   361,   370,   187,   370,   187,   370,   370,
     370,   313,   370,   317,   318,   318,   318,   318,   318,   318,
     316,   185,   370,   370,   370,   370,   165,   165,   104,   104,
     104,   104,   104,   104,   104,   104,   370,   370,   370,   370
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
     355,   355,   355,   355,   356,   356,   357,   358,   358,   359,
     360,   360,   361,   361,   361,   362,   362,   362,   362,   362,
     362,   363,   363,   363,   363,   363,   364,   364,   364,   364,
     364,   365,   366,   366,   366,   366,   366,   366,   367,   368,
     369,   369,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   371,
     371,   372,   372,   372,   373,   373,   373,   373,   374,   374,
     374,   374,   374,   375,   375,   375,   376,   376,   376,   376,
     376,   376,   377,   377,   377,   377,   378,   378,   379,   379,
     380,   380,   380,   380,   380,   380,   380,   380,   380,   380,
     380,   380,   380,   381,   381,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   383,   383,
     383,   383,   383,   383,   383,   384,   384,   384,   384,   385,
     385,   385,   385,   386,   386,   386,   386,   386,   386,   386
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
       1,     1,     1,     1,     0,     1,     4,     1,     3,     4,
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
#line 636 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 5904 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 640 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 5910 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 641 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 5916 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 648 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 5924 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 656 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 5932 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 660 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 5941 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 670 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 5949 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 675 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 5957 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5963 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5969 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5975 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5981 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5987 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5993 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5999 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 689 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6005 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 690 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6011 "bison-chpl-lib.cpp"
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
#line 6033 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 709 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6039 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 710 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6045 "bison-chpl-lib.cpp"
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
#line 6057 "bison-chpl-lib.cpp"
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
#line 6070 "bison-chpl-lib.cpp"
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
#line 6083 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TLABEL ident_def stmt  */
#line 738 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6091 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 742 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6102 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt_base: error TSEMI  */
#line 749 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6111 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: assignment_stmt  */
#line 756 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6117 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: if_stmt  */
#line 757 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6123 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: loop_stmt  */
#line 758 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6129 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: select_stmt  */
#line 759 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6135 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: manage_stmt  */
#line 760 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6141 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 761 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6147 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: throw_stmt  */
#line 762 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6153 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 764 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6159 "bison-chpl-lib.cpp"
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
#line 6183 "bison-chpl-lib.cpp"
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
#line 6195 "bison-chpl-lib.cpp"
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
#line 6216 "bison-chpl-lib.cpp"
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
#line 6236 "bison-chpl-lib.cpp"
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
#line 6257 "bison-chpl-lib.cpp"
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
#line 6278 "bison-chpl-lib.cpp"
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
#line 6298 "bison-chpl-lib.cpp"
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
#line 6320 "bison-chpl-lib.cpp"
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
#line 6332 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 927 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6340 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 934 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6349 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 941 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6357 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 945 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6368 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 955 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6374 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 957 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6382 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 961 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6390 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 968 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6399 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 973 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6409 "bison-chpl-lib.cpp"
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
#line 6431 "bison-chpl-lib.cpp"
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
#line 6452 "bison-chpl-lib.cpp"
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
#line 6473 "bison-chpl-lib.cpp"
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
#line 6492 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1058 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6498 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1059 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6505 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1061 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6512 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1066 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6518 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1067 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6525 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1073 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6535 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1091 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6544 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1096 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6553 "bison-chpl-lib.cpp"
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
#line 6566 "bison-chpl-lib.cpp"
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
#line 6585 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1131 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6591 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1132 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6597 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1137 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6605 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1141 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6613 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1145 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6622 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1150 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6630 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1154 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6638 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1158 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6647 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1166 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6656 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1171 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6666 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1177 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6675 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1182 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6686 "bison-chpl-lib.cpp"
    break;

  case 90: /* opt_only_ls: %empty  */
#line 1190 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6692 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: renames_ls  */
#line 1191 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6698 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1196 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6707 "bison-chpl-lib.cpp"
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
#line 6719 "bison-chpl-lib.cpp"
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
#line 6732 "bison-chpl-lib.cpp"
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
#line 6744 "bison-chpl-lib.cpp"
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
#line 6757 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1237 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6766 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr  */
#line 1273 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6774 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT all_op_name  */
#line 1277 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6783 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1282 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6792 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TAS ident_use  */
#line 1287 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6802 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1293 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6812 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_expr  */
#line 1301 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6818 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_ls: import_ls TCOMMA import_expr  */
#line 1302 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6824 "bison-chpl-lib.cpp"
    break;

  case 122: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1307 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6834 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1316 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6842 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1320 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6850 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1324 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6858 "bison-chpl-lib.cpp"
    break;

  case 126: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1328 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6866 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1332 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6874 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1336 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6882 "bison-chpl-lib.cpp"
    break;

  case 129: /* opt_label_ident: %empty  */
#line 1344 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6888 "bison-chpl-lib.cpp"
    break;

  case 130: /* opt_label_ident: TIDENT  */
#line 1345 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6894 "bison-chpl-lib.cpp"
    break;

  case 131: /* opt_label_ident: TINIT  */
#line 1346 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6900 "bison-chpl-lib.cpp"
    break;

  case 140: /* ident_use: TIDENT  */
#line 1372 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6906 "bison-chpl-lib.cpp"
    break;

  case 141: /* ident_use: TINIT  */
#line 1373 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6912 "bison-chpl-lib.cpp"
    break;

  case 142: /* ident_use: TTHIS  */
#line 1374 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6918 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TBOOL  */
#line 1401 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6924 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TENUM  */
#line 1402 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6930 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TINT  */
#line 1403 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6936 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TUINT  */
#line 1404 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6942 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TREAL  */
#line 1405 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6948 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TIMAG  */
#line 1406 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6954 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TCOMPLEX  */
#line 1407 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6960 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TBYTES  */
#line 1408 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6966 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TSTRING  */
#line 1409 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6972 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TLOCALE  */
#line 1410 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6978 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TNOTHING  */
#line 1411 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6984 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TVOID  */
#line 1412 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6990 "bison-chpl-lib.cpp"
    break;

  case 178: /* do_stmt: TDO stmt  */
#line 1426 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 6996 "bison-chpl-lib.cpp"
    break;

  case 179: /* do_stmt: block_stmt  */
#line 1427 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7002 "bison-chpl-lib.cpp"
    break;

  case 180: /* return_stmt: TRETURN TSEMI  */
#line 1432 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7013 "bison-chpl-lib.cpp"
    break;

  case 181: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1439 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7024 "bison-chpl-lib.cpp"
    break;

  case 182: /* class_level_stmt: TSEMI  */
#line 1449 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7032 "bison-chpl-lib.cpp"
    break;

  case 183: /* class_level_stmt: inner_class_level_stmt  */
#line 1453 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7042 "bison-chpl-lib.cpp"
    break;

  case 184: /* $@1: %empty  */
#line 1458 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7049 "bison-chpl-lib.cpp"
    break;

  case 185: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1460 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7058 "bison-chpl-lib.cpp"
    break;

  case 186: /* $@2: %empty  */
#line 1464 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7065 "bison-chpl-lib.cpp"
    break;

  case 187: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1466 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7074 "bison-chpl-lib.cpp"
    break;

  case 195: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1484 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7083 "bison-chpl-lib.cpp"
    break;

  case 196: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1489 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7092 "bison-chpl-lib.cpp"
    break;

  case 197: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1494 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7101 "bison-chpl-lib.cpp"
    break;

  case 198: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1499 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7109 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_start: TFORWARDING  */
#line 1506 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7118 "bison-chpl-lib.cpp"
    break;

  case 200: /* extern_or_export: TEXTERN  */
#line 1513 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7124 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_or_export: TEXPORT  */
#line 1514 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7130 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1519 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7140 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1529 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7150 "bison-chpl-lib.cpp"
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
#line 7162 "bison-chpl-lib.cpp"
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
#line 7184 "bison-chpl-lib.cpp"
    break;

  case 206: /* $@3: %empty  */
#line 1562 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7192 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1566 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7201 "bison-chpl-lib.cpp"
    break;

  case 208: /* $@4: %empty  */
#line 1571 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7210 "bison-chpl-lib.cpp"
    break;

  case 209: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1576 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7219 "bison-chpl-lib.cpp"
    break;

  case 210: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1584 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7227 "bison-chpl-lib.cpp"
    break;

  case 211: /* no_loop_attributes: %empty  */
#line 1589 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7233 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1592 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7239 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1593 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7245 "bison-chpl-lib.cpp"
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
#line 7272 "bison-chpl-lib.cpp"
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
#line 7295 "bison-chpl-lib.cpp"
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
#line 7318 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1670 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7327 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1675 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7336 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1680 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7345 "bison-chpl-lib.cpp"
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
#line 7374 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1710 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7383 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw expr TIN expr forall_intent_clause do_stmt  */
#line 1715 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7392 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1720 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7401 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1725 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7410 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1730 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7419 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: for_loop_kw expr forall_intent_clause do_stmt  */
#line 1735 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7428 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1740 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7437 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: for_loop_kw zippered_iterator forall_intent_clause do_stmt  */
#line 1745 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7446 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1750 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7455 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1755 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7464 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1760 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7473 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1765 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7482 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1770 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7491 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt_base: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1775 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7500 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1780 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7510 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt_base: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1786 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7520 "bison-chpl-lib.cpp"
    break;

  case 240: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1795 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7530 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF expr TTHEN stmt  */
#line 1804 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7538 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF expr block_stmt  */
#line 1808 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7546 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1812 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7554 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1816 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7562 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1820 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7570 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF ifvar block_stmt  */
#line 1824 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7578 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1828 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7586 "bison-chpl-lib.cpp"
    break;

  case 248: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1832 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7594 "bison-chpl-lib.cpp"
    break;

  case 249: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1836 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7603 "bison-chpl-lib.cpp"
    break;

  case 250: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1841 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7612 "bison-chpl-lib.cpp"
    break;

  case 251: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1846 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7621 "bison-chpl-lib.cpp"
    break;

  case 252: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1851 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7630 "bison-chpl-lib.cpp"
    break;

  case 253: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1859 "chpl.ypp"
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
#line 7650 "bison-chpl-lib.cpp"
    break;

  case 254: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1875 "chpl.ypp"
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
#line 7670 "bison-chpl-lib.cpp"
    break;

  case 255: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1894 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7678 "bison-chpl-lib.cpp"
    break;

  case 256: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1898 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7686 "bison-chpl-lib.cpp"
    break;

  case 257: /* ifc_formal_ls: ifc_formal  */
#line 1904 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7692 "bison-chpl-lib.cpp"
    break;

  case 258: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1905 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7698 "bison-chpl-lib.cpp"
    break;

  case 259: /* ifc_formal: ident_def  */
#line 1910 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7704 "bison-chpl-lib.cpp"
    break;

  case 279: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1954 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7712 "bison-chpl-lib.cpp"
    break;

  case 280: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1958 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7720 "bison-chpl-lib.cpp"
    break;

  case 281: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1962 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7728 "bison-chpl-lib.cpp"
    break;

  case 282: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1969 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7736 "bison-chpl-lib.cpp"
    break;

  case 283: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1973 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7744 "bison-chpl-lib.cpp"
    break;

  case 284: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1977 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7752 "bison-chpl-lib.cpp"
    break;

  case 285: /* try_stmt: TTRY tryable_stmt  */
#line 1984 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7760 "bison-chpl-lib.cpp"
    break;

  case 286: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1988 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7768 "bison-chpl-lib.cpp"
    break;

  case 287: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1992 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7776 "bison-chpl-lib.cpp"
    break;

  case 288: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1996 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7784 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr_ls: %empty  */
#line 2002 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7790 "bison-chpl-lib.cpp"
    break;

  case 290: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2003 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7796 "bison-chpl-lib.cpp"
    break;

  case 291: /* catch_expr: TCATCH block_stmt  */
#line 2008 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7804 "bison-chpl-lib.cpp"
    break;

  case 292: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2012 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7812 "bison-chpl-lib.cpp"
    break;

  case 293: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2016 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7820 "bison-chpl-lib.cpp"
    break;

  case 294: /* catch_expr_inner: ident_def  */
#line 2023 "chpl.ypp"
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
#line 7838 "bison-chpl-lib.cpp"
    break;

  case 295: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2037 "chpl.ypp"
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
#line 7856 "bison-chpl-lib.cpp"
    break;

  case 296: /* throw_stmt: TTHROW expr TSEMI  */
#line 2054 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7866 "bison-chpl-lib.cpp"
    break;

  case 297: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2062 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7875 "bison-chpl-lib.cpp"
    break;

  case 298: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2067 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7886 "bison-chpl-lib.cpp"
    break;

  case 299: /* when_stmt_ls: %empty  */
#line 2076 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7892 "bison-chpl-lib.cpp"
    break;

  case 300: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2077 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7898 "bison-chpl-lib.cpp"
    break;

  case 301: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2082 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7906 "bison-chpl-lib.cpp"
    break;

  case 302: /* when_stmt: TOTHERWISE stmt  */
#line 2086 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7915 "bison-chpl-lib.cpp"
    break;

  case 303: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2091 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 7925 "bison-chpl-lib.cpp"
    break;

  case 304: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2100 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 7934 "bison-chpl-lib.cpp"
    break;

  case 305: /* manager_expr: expr TAS ident_def  */
#line 2105 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 7942 "bison-chpl-lib.cpp"
    break;

  case 306: /* manager_expr: expr  */
#line 2109 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 7950 "bison-chpl-lib.cpp"
    break;

  case 307: /* manager_expr_ls: manager_expr  */
#line 2115 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7956 "bison-chpl-lib.cpp"
    break;

  case 308: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2116 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7962 "bison-chpl-lib.cpp"
    break;

  case 309: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2121 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 7971 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2131 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 7981 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2137 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 7993 "bison-chpl-lib.cpp"
    break;

  case 312: /* class_start: class_tag ident_def  */
#line 2149 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8001 "bison-chpl-lib.cpp"
    break;

  case 313: /* class_tag: TCLASS  */
#line 2155 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8007 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_tag: TRECORD  */
#line 2156 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8013 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_tag: TUNION  */
#line 2157 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8019 "bison-chpl-lib.cpp"
    break;

  case 316: /* opt_inherit: %empty  */
#line 2161 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8025 "bison-chpl-lib.cpp"
    break;

  case 317: /* opt_inherit: TCOLON expr_ls  */
#line 2162 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8031 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_level_stmt_ls: %empty  */
#line 2166 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8040 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2171 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8048 "bison-chpl-lib.cpp"
    break;

  case 320: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2175 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8056 "bison-chpl-lib.cpp"
    break;

  case 321: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2179 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8064 "bison-chpl-lib.cpp"
    break;

  case 322: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2183 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8072 "bison-chpl-lib.cpp"
    break;

  case 323: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
#line 2191 "chpl.ypp"
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
#line 8095 "bison-chpl-lib.cpp"
    break;

  case 324: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
#line 2210 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8109 "bison-chpl-lib.cpp"
    break;

  case 325: /* enum_header: TENUM ident_def  */
#line 2223 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8117 "bison-chpl-lib.cpp"
    break;

  case 326: /* enum_ls: enum_item  */
#line 2230 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8126 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_ls: enum_ls TCOMMA  */
#line 2235 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8136 "bison-chpl-lib.cpp"
    break;

  case 328: /* $@5: %empty  */
#line 2241 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8145 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2246 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8154 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2251 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8163 "bison-chpl-lib.cpp"
    break;

  case 331: /* $@6: %empty  */
#line 2256 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8172 "bison-chpl-lib.cpp"
    break;

  case 332: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2261 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8181 "bison-chpl-lib.cpp"
    break;

  case 333: /* enum_item: ident_def  */
#line 2271 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8192 "bison-chpl-lib.cpp"
    break;

  case 334: /* enum_item: ident_def TASSIGN expr  */
#line 2278 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8205 "bison-chpl-lib.cpp"
    break;

  case 335: /* lambda_decl_start: TLAMBDA  */
#line 2290 "chpl.ypp"
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
#line 8221 "bison-chpl-lib.cpp"
    break;

  case 336: /* $@7: %empty  */
#line 2307 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8231 "bison-chpl-lib.cpp"
    break;

  case 337: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2313 "chpl.ypp"
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
#line 8253 "bison-chpl-lib.cpp"
    break;

  case 339: /* linkage_spec: linkage_spec_empty  */
#line 2336 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8259 "bison-chpl-lib.cpp"
    break;

  case 340: /* linkage_spec: TINLINE  */
#line 2337 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8266 "bison-chpl-lib.cpp"
    break;

  case 341: /* linkage_spec: TOVERRIDE  */
#line 2339 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8273 "bison-chpl-lib.cpp"
    break;

  case 342: /* opt_fn_type_formal_ls: %empty  */
#line 2344 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8279 "bison-chpl-lib.cpp"
    break;

  case 343: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2345 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8285 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_type_formal_ls: fn_type_formal  */
#line 2349 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8291 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2350 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8297 "bison-chpl-lib.cpp"
    break;

  case 346: /* fn_type_formal: named_formal  */
#line 2355 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8303 "bison-chpl-lib.cpp"
    break;

  case 347: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2358 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8309 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_type_formal: formal_type  */
#line 2360 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8315 "bison-chpl-lib.cpp"
    break;

  case 349: /* opt_fn_type_ret_type: %empty  */
#line 2364 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8321 "bison-chpl-lib.cpp"
    break;

  case 350: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2365 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8327 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2378 "chpl.ypp"
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
#line 8346 "bison-chpl-lib.cpp"
    break;

  case 352: /* $@8: %empty  */
#line 2396 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8356 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2402 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8369 "bison-chpl-lib.cpp"
    break;

  case 354: /* $@9: %empty  */
#line 2411 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8379 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2417 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8392 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2429 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8400 "bison-chpl-lib.cpp"
    break;

  case 357: /* $@10: %empty  */
#line 2438 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8408 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2442 "chpl.ypp"
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
#line 8430 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2463 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8442 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2471 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8454 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2479 "chpl.ypp"
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
#line 8469 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2490 "chpl.ypp"
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
#line 8484 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2501 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8494 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2510 "chpl.ypp"
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
#line 8510 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2525 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8516 "bison-chpl-lib.cpp"
    break;

  case 369: /* fn_ident: ident_def TBANG  */
#line 2532 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8526 "bison-chpl-lib.cpp"
    break;

  case 409: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2589 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8532 "bison-chpl-lib.cpp"
    break;

  case 410: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2590 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8538 "bison-chpl-lib.cpp"
    break;

  case 411: /* opt_formal_ls: %empty  */
#line 2594 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8544 "bison-chpl-lib.cpp"
    break;

  case 412: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2595 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8550 "bison-chpl-lib.cpp"
    break;

  case 413: /* req_formal_ls: TLP TRP  */
#line 2599 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8556 "bison-chpl-lib.cpp"
    break;

  case 414: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2600 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8562 "bison-chpl-lib.cpp"
    break;

  case 415: /* formal_ls_inner: formal  */
#line 2604 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8568 "bison-chpl-lib.cpp"
    break;

  case 416: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2605 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8574 "bison-chpl-lib.cpp"
    break;

  case 417: /* formal_ls: %empty  */
#line 2609 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8580 "bison-chpl-lib.cpp"
    break;

  case 418: /* formal_ls: formal_ls_inner  */
#line 2610 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8586 "bison-chpl-lib.cpp"
    break;

  case 420: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2619 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8594 "bison-chpl-lib.cpp"
    break;

  case 421: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2624 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8602 "bison-chpl-lib.cpp"
    break;

  case 422: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2629 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8610 "bison-chpl-lib.cpp"
    break;

  case 423: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2634 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8618 "bison-chpl-lib.cpp"
    break;

  case 424: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2639 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8626 "bison-chpl-lib.cpp"
    break;

  case 425: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2644 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8634 "bison-chpl-lib.cpp"
    break;

  case 426: /* opt_formal_intent_tag: %empty  */
#line 2650 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8643 "bison-chpl-lib.cpp"
    break;

  case 427: /* opt_formal_intent_tag: required_intent_tag  */
#line 2655 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8652 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TIN  */
#line 2662 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8658 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TINOUT  */
#line 2663 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8664 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TOUT  */
#line 2664 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8670 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TCONST TIN  */
#line 2665 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8676 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TCONST TREF  */
#line 2666 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 8682 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TCONST  */
#line 2667 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 8688 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TPARAM  */
#line 2668 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 8694 "bison-chpl-lib.cpp"
    break;

  case 435: /* required_intent_tag: TREF  */
#line 2669 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 8700 "bison-chpl-lib.cpp"
    break;

  case 436: /* required_intent_tag: TTYPE  */
#line 2670 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 8706 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: %empty  */
#line 2674 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8712 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TPARAM  */
#line 2675 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 8718 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TREF  */
#line 2676 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 8724 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TCONST TREF  */
#line 2677 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 8730 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TCONST  */
#line 2678 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 8736 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TTYPE  */
#line 2679 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 8742 "bison-chpl-lib.cpp"
    break;

  case 443: /* proc_iter_or_op: TPROC  */
#line 2683 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8748 "bison-chpl-lib.cpp"
    break;

  case 444: /* proc_iter_or_op: TITER  */
#line 2684 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8754 "bison-chpl-lib.cpp"
    break;

  case 445: /* proc_iter_or_op: TOPERATOR  */
#line 2685 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8760 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_ret_tag: %empty  */
#line 2689 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 8766 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_ret_tag: TOUT  */
#line 2690 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 8772 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_ret_tag: TCONST  */
#line 2691 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 8778 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: TCONST TREF  */
#line 2692 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 8784 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TREF  */
#line 2693 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 8790 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TPARAM  */
#line 2694 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 8796 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TTYPE  */
#line 2695 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 8802 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_throws_error: %empty  */
#line 2699 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8808 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_throws_error: TTHROWS  */
#line 2700 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8814 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_function_body_stmt: TSEMI  */
#line 2703 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8820 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_function_body_stmt: function_body_stmt  */
#line 2704 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8826 "bison-chpl-lib.cpp"
    break;

  case 457: /* function_body_stmt: block_stmt_body  */
#line 2708 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8832 "bison-chpl-lib.cpp"
    break;

  case 458: /* function_body_stmt: TDO toplevel_stmt  */
#line 2709 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 8838 "bison-chpl-lib.cpp"
    break;

  case 459: /* query_expr: TQUERIEDIDENT  */
#line 2713 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8844 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_lifetime_where: %empty  */
#line 2718 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8850 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_lifetime_where: TWHERE expr  */
#line 2720 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8856 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2722 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8862 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2724 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 8868 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2726 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 8874 "bison-chpl-lib.cpp"
    break;

  case 465: /* lifetime_components_expr: lifetime_expr  */
#line 2731 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8880 "bison-chpl-lib.cpp"
    break;

  case 466: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2733 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8886 "bison-chpl-lib.cpp"
    break;

  case 467: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2738 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8892 "bison-chpl-lib.cpp"
    break;

  case 468: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2740 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8898 "bison-chpl-lib.cpp"
    break;

  case 469: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2742 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8904 "bison-chpl-lib.cpp"
    break;

  case 470: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2744 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8910 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2746 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8916 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2748 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8922 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2750 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 8928 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_ident: TIDENT  */
#line 2754 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8934 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_ident: TINIT  */
#line 2755 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8940 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_ident: TTHIS  */
#line 2756 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8946 "bison-chpl-lib.cpp"
    break;

  case 477: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2761 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 8955 "bison-chpl-lib.cpp"
    break;

  case 478: /* type_alias_decl_stmt_start: TTYPE  */
#line 2770 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 8963 "bison-chpl-lib.cpp"
    break;

  case 479: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2774 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 8972 "bison-chpl-lib.cpp"
    break;

  case 480: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2782 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 8980 "bison-chpl-lib.cpp"
    break;

  case 481: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2786 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8988 "bison-chpl-lib.cpp"
    break;

  case 482: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2793 "chpl.ypp"
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
#line 9012 "bison-chpl-lib.cpp"
    break;

  case 483: /* opt_init_type: %empty  */
#line 2815 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9018 "bison-chpl-lib.cpp"
    break;

  case 484: /* opt_init_type: TASSIGN expr  */
#line 2817 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9024 "bison-chpl-lib.cpp"
    break;

  case 485: /* var_decl_type: TPARAM  */
#line 2821 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 486: /* var_decl_type: TCONST TREF  */
#line 2822 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9036 "bison-chpl-lib.cpp"
    break;

  case 487: /* var_decl_type: TREF  */
#line 2823 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9042 "bison-chpl-lib.cpp"
    break;

  case 488: /* var_decl_type: TCONST  */
#line 2824 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9048 "bison-chpl-lib.cpp"
    break;

  case 489: /* var_decl_type: TVAR  */
#line 2825 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9054 "bison-chpl-lib.cpp"
    break;

  case 490: /* $@11: %empty  */
#line 2830 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9063 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2834 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9072 "bison-chpl-lib.cpp"
    break;

  case 492: /* $@12: %empty  */
#line 2839 "chpl.ypp"
  {
    // Use a mid-rule action to preserve the 'on' expr
    context->noteVarDestinationExpr((yyvsp[0].expr));
  }
#line 9081 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_stmt: TON expr $@12 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2844 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9090 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2849 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9099 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_stmt: var_decl_type error TSEMI  */
#line 2854 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    context->resetDeclStateOnError();
  }
#line 9108 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2862 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9116 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2866 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9124 "bison-chpl-lib.cpp"
    break;

  case 498: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2873 "chpl.ypp"
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
#line 9148 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2893 "chpl.ypp"
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
#line 9168 "bison-chpl-lib.cpp"
    break;

  case 500: /* tuple_var_decl_component: ident_def  */
#line 2912 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9176 "bison-chpl-lib.cpp"
    break;

  case 501: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2916 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9184 "bison-chpl-lib.cpp"
    break;

  case 502: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2923 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9190 "bison-chpl-lib.cpp"
    break;

  case 503: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2925 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9196 "bison-chpl-lib.cpp"
    break;

  case 504: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2927 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9202 "bison-chpl-lib.cpp"
    break;

  case 505: /* opt_init_expr: %empty  */
#line 2933 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9208 "bison-chpl-lib.cpp"
    break;

  case 506: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2934 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9214 "bison-chpl-lib.cpp"
    break;

  case 507: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2935 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9220 "bison-chpl-lib.cpp"
    break;

  case 508: /* formal_or_ret_type_expr: expr  */
#line 2939 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9226 "bison-chpl-lib.cpp"
    break;

  case 509: /* ret_type: formal_or_ret_type_expr  */
#line 2943 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9232 "bison-chpl-lib.cpp"
    break;

  case 510: /* ret_type: reserved_type_ident_use  */
#line 2944 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9238 "bison-chpl-lib.cpp"
    break;

  case 511: /* ret_type: error  */
#line 2945 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9244 "bison-chpl-lib.cpp"
    break;

  case 512: /* colon_ret_type: TCOLON ret_type  */
#line 2949 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9250 "bison-chpl-lib.cpp"
    break;

  case 513: /* colon_ret_type: error  */
#line 2950 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9256 "bison-chpl-lib.cpp"
    break;

  case 514: /* opt_ret_type: %empty  */
#line 2954 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9262 "bison-chpl-lib.cpp"
    break;

  case 516: /* opt_type: %empty  */
#line 2959 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9268 "bison-chpl-lib.cpp"
    break;

  case 517: /* opt_type: TCOLON expr  */
#line 2960 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9274 "bison-chpl-lib.cpp"
    break;

  case 518: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2961 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9280 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_type: error  */
#line 2962 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9286 "bison-chpl-lib.cpp"
    break;

  case 520: /* formal_type: formal_or_ret_type_expr  */
#line 2966 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9292 "bison-chpl-lib.cpp"
    break;

  case 521: /* formal_type: reserved_type_ident_use  */
#line 2967 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9298 "bison-chpl-lib.cpp"
    break;

  case 522: /* colon_formal_type: TCOLON formal_type  */
#line 2971 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9304 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_colon_formal_type: %empty  */
#line 2975 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9310 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_colon_formal_type: colon_formal_type  */
#line 2976 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9316 "bison-chpl-lib.cpp"
    break;

  case 525: /* expr_ls: expr  */
#line 2982 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9322 "bison-chpl-lib.cpp"
    break;

  case 526: /* expr_ls: expr_ls TCOMMA expr  */
#line 2983 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9328 "bison-chpl-lib.cpp"
    break;

  case 527: /* tuple_component: opt_try_expr  */
#line 2987 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9334 "bison-chpl-lib.cpp"
    break;

  case 528: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2992 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9342 "bison-chpl-lib.cpp"
    break;

  case 529: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2996 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9350 "bison-chpl-lib.cpp"
    break;

  case 530: /* opt_actual_ls: %empty  */
#line 3002 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9356 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_actual_ls: actual_ls  */
#line 3003 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9362 "bison-chpl-lib.cpp"
    break;

  case 532: /* actual_ls: actual_expr  */
#line 3008 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9371 "bison-chpl-lib.cpp"
    break;

  case 533: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3013 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9381 "bison-chpl-lib.cpp"
    break;

  case 534: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3021 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9387 "bison-chpl-lib.cpp"
    break;

  case 535: /* actual_expr: opt_try_expr  */
#line 3022 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9393 "bison-chpl-lib.cpp"
    break;

  case 536: /* ident_expr: ident_use  */
#line 3026 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9399 "bison-chpl-lib.cpp"
    break;

  case 537: /* ident_expr: scalar_type  */
#line 3027 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9405 "bison-chpl-lib.cpp"
    break;

  case 543: /* sub_type_level_expr: TSINGLE expr  */
#line 3040 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9411 "bison-chpl-lib.cpp"
    break;

  case 544: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3042 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9417 "bison-chpl-lib.cpp"
    break;

  case 545: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3044 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9423 "bison-chpl-lib.cpp"
    break;

  case 546: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3046 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9429 "bison-chpl-lib.cpp"
    break;

  case 547: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3048 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9439 "bison-chpl-lib.cpp"
    break;

  case 548: /* sub_type_level_expr: TATOMIC expr  */
#line 3054 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9445 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TSYNC expr  */
#line 3056 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9451 "bison-chpl-lib.cpp"
    break;

  case 550: /* sub_type_level_expr: TOWNED  */
#line 3059 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9457 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TOWNED expr  */
#line 3061 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9463 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TUNMANAGED  */
#line 3063 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9469 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3065 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9475 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TSHARED  */
#line 3067 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9481 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TSHARED expr  */
#line 3069 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9487 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TBORROWED  */
#line 3071 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9493 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TBORROWED expr  */
#line 3073 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9499 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TCLASS  */
#line 3075 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9505 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TRECORD  */
#line 3077 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9511 "bison-chpl-lib.cpp"
    break;

  case 560: /* for_expr: no_loop_attributes for_expr_base  */
#line 3081 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9517 "bison-chpl-lib.cpp"
    break;

  case 561: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3086 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9525 "bison-chpl-lib.cpp"
    break;

  case 562: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3090 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9533 "bison-chpl-lib.cpp"
    break;

  case 563: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3094 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9541 "bison-chpl-lib.cpp"
    break;

  case 564: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3098 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9554 "bison-chpl-lib.cpp"
    break;

  case 565: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3107 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9567 "bison-chpl-lib.cpp"
    break;

  case 566: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3116 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9580 "bison-chpl-lib.cpp"
    break;

  case 567: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3127 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9586 "bison-chpl-lib.cpp"
    break;

  case 568: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3132 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9594 "bison-chpl-lib.cpp"
    break;

  case 569: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3136 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9602 "bison-chpl-lib.cpp"
    break;

  case 570: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3140 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9610 "bison-chpl-lib.cpp"
    break;

  case 571: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3144 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9618 "bison-chpl-lib.cpp"
    break;

  case 572: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3148 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9626 "bison-chpl-lib.cpp"
    break;

  case 573: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3152 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9634 "bison-chpl-lib.cpp"
    break;

  case 574: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3156 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9642 "bison-chpl-lib.cpp"
    break;

  case 575: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3163 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9656 "bison-chpl-lib.cpp"
    break;

  case 576: /* nil_expr: TNIL  */
#line 3180 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9662 "bison-chpl-lib.cpp"
    break;

  case 584: /* opt_task_intent_ls: %empty  */
#line 3198 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9668 "bison-chpl-lib.cpp"
    break;

  case 585: /* opt_task_intent_ls: task_intent_clause  */
#line 3199 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9674 "bison-chpl-lib.cpp"
    break;

  case 586: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3204 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9682 "bison-chpl-lib.cpp"
    break;

  case 587: /* task_intent_ls: intent_expr  */
#line 3210 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9688 "bison-chpl-lib.cpp"
    break;

  case 588: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3211 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9694 "bison-chpl-lib.cpp"
    break;

  case 589: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3216 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 9704 "bison-chpl-lib.cpp"
    break;

  case 590: /* forall_intent_ls: intent_expr  */
#line 3224 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9710 "bison-chpl-lib.cpp"
    break;

  case 591: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3225 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9716 "bison-chpl-lib.cpp"
    break;

  case 592: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3230 "chpl.ypp"
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
#line 9735 "bison-chpl-lib.cpp"
    break;

  case 593: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3245 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9743 "bison-chpl-lib.cpp"
    break;

  case 594: /* intent_expr: expr TREDUCE ident_expr  */
#line 3249 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9751 "bison-chpl-lib.cpp"
    break;

  case 595: /* task_var_prefix: TCONST  */
#line 3255 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 9757 "bison-chpl-lib.cpp"
    break;

  case 596: /* task_var_prefix: TIN  */
#line 3256 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 9763 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_var_prefix: TCONST TIN  */
#line 3257 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 9769 "bison-chpl-lib.cpp"
    break;

  case 598: /* task_var_prefix: TREF  */
#line 3258 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 9775 "bison-chpl-lib.cpp"
    break;

  case 599: /* task_var_prefix: TCONST TREF  */
#line 3259 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 9781 "bison-chpl-lib.cpp"
    break;

  case 600: /* task_var_prefix: TVAR  */
#line 3260 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 9787 "bison-chpl-lib.cpp"
    break;

  case 601: /* new_maybe_decorated: TNEW  */
#line 3265 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9793 "bison-chpl-lib.cpp"
    break;

  case 602: /* new_maybe_decorated: TNEW TOWNED  */
#line 3267 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9799 "bison-chpl-lib.cpp"
    break;

  case 603: /* new_maybe_decorated: TNEW TSHARED  */
#line 3269 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9805 "bison-chpl-lib.cpp"
    break;

  case 604: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3271 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9811 "bison-chpl-lib.cpp"
    break;

  case 605: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3273 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9817 "bison-chpl-lib.cpp"
    break;

  case 606: /* new_expr: new_maybe_decorated expr  */
#line 3279 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 9825 "bison-chpl-lib.cpp"
    break;

  case 607: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3286 "chpl.ypp"
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
#line 9841 "bison-chpl-lib.cpp"
    break;

  case 608: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3298 "chpl.ypp"
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
#line 9857 "bison-chpl-lib.cpp"
    break;

  case 609: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3310 "chpl.ypp"
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
#line 9874 "bison-chpl-lib.cpp"
    break;

  case 610: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3323 "chpl.ypp"
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
#line 9892 "bison-chpl-lib.cpp"
    break;

  case 611: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3340 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9900 "bison-chpl-lib.cpp"
    break;

  case 612: /* range_literal_expr: expr TDOTDOT expr  */
#line 3347 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 9909 "bison-chpl-lib.cpp"
    break;

  case 613: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3352 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 9918 "bison-chpl-lib.cpp"
    break;

  case 614: /* range_literal_expr: expr TDOTDOT  */
#line 3357 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 9927 "bison-chpl-lib.cpp"
    break;

  case 615: /* range_literal_expr: TDOTDOT expr  */
#line 3362 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 9937 "bison-chpl-lib.cpp"
    break;

  case 616: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3368 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 9947 "bison-chpl-lib.cpp"
    break;

  case 617: /* range_literal_expr: TDOTDOT  */
#line 3374 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 9957 "bison-chpl-lib.cpp"
    break;

  case 618: /* cast_expr: expr TCOLON expr  */
#line 3404 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 9965 "bison-chpl-lib.cpp"
    break;

  case 619: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3411 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 9973 "bison-chpl-lib.cpp"
    break;

  case 620: /* super_expr: fn_expr  */
#line 3417 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 9979 "bison-chpl-lib.cpp"
    break;

  case 624: /* expr: sub_type_level_expr TQUESTION  */
#line 3426 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 9985 "bison-chpl-lib.cpp"
    break;

  case 625: /* expr: TQUESTION  */
#line 3428 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 9991 "bison-chpl-lib.cpp"
    break;

  case 629: /* expr: fn_type  */
#line 3433 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 9997 "bison-chpl-lib.cpp"
    break;

  case 639: /* opt_expr: %empty  */
#line 3447 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10003 "bison-chpl-lib.cpp"
    break;

  case 640: /* opt_expr: expr  */
#line 3448 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10009 "bison-chpl-lib.cpp"
    break;

  case 641: /* opt_try_expr: TTRY expr  */
#line 3452 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10015 "bison-chpl-lib.cpp"
    break;

  case 642: /* opt_try_expr: TTRYBANG expr  */
#line 3453 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10021 "bison-chpl-lib.cpp"
    break;

  case 643: /* opt_try_expr: super_expr  */
#line 3454 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10027 "bison-chpl-lib.cpp"
    break;

  case 649: /* call_base_expr: expr TBANG  */
#line 3471 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10035 "bison-chpl-lib.cpp"
    break;

  case 650: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3474 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10041 "bison-chpl-lib.cpp"
    break;

  case 653: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3481 "chpl.ypp"
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
#line 10057 "bison-chpl-lib.cpp"
    break;

  case 654: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3493 "chpl.ypp"
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
#line 10073 "bison-chpl-lib.cpp"
    break;

  case 655: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3505 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10081 "bison-chpl-lib.cpp"
    break;

  case 656: /* dot_expr: expr TDOT ident_use  */
#line 3512 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10087 "bison-chpl-lib.cpp"
    break;

  case 657: /* dot_expr: expr TDOT TTYPE  */
#line 3514 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10093 "bison-chpl-lib.cpp"
    break;

  case 658: /* dot_expr: expr TDOT TDOMAIN  */
#line 3516 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10099 "bison-chpl-lib.cpp"
    break;

  case 659: /* dot_expr: expr TDOT TLOCALE  */
#line 3518 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10105 "bison-chpl-lib.cpp"
    break;

  case 660: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3520 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10111 "bison-chpl-lib.cpp"
    break;

  case 661: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3522 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10117 "bison-chpl-lib.cpp"
    break;

  case 662: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3531 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenthLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10127 "bison-chpl-lib.cpp"
    break;

  case 663: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3537 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10135 "bison-chpl-lib.cpp"
    break;

  case 664: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3541 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10143 "bison-chpl-lib.cpp"
    break;

  case 665: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3545 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10151 "bison-chpl-lib.cpp"
    break;

  case 666: /* bool_literal: TFALSE  */
#line 3551 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10157 "bison-chpl-lib.cpp"
    break;

  case 667: /* bool_literal: TTRUE  */
#line 3552 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10163 "bison-chpl-lib.cpp"
    break;

  case 668: /* str_bytes_literal: STRINGLITERAL  */
#line 3556 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10169 "bison-chpl-lib.cpp"
    break;

  case 669: /* str_bytes_literal: BYTESLITERAL  */
#line 3557 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10175 "bison-chpl-lib.cpp"
    break;

  case 672: /* literal_expr: INTLITERAL  */
#line 3563 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10181 "bison-chpl-lib.cpp"
    break;

  case 673: /* literal_expr: REALLITERAL  */
#line 3564 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10187 "bison-chpl-lib.cpp"
    break;

  case 674: /* literal_expr: IMAGLITERAL  */
#line 3565 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10193 "bison-chpl-lib.cpp"
    break;

  case 675: /* literal_expr: CSTRINGLITERAL  */
#line 3566 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10199 "bison-chpl-lib.cpp"
    break;

  case 676: /* literal_expr: TNONE  */
#line 3567 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10205 "bison-chpl-lib.cpp"
    break;

  case 677: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3569 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10214 "bison-chpl-lib.cpp"
    break;

  case 678: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3574 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10223 "bison-chpl-lib.cpp"
    break;

  case 679: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3579 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10232 "bison-chpl-lib.cpp"
    break;

  case 680: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3584 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10241 "bison-chpl-lib.cpp"
    break;

  case 681: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3589 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10250 "bison-chpl-lib.cpp"
    break;

  case 682: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3594 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10259 "bison-chpl-lib.cpp"
    break;

  case 683: /* assoc_expr_ls: expr TALIAS expr  */
#line 3603 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10268 "bison-chpl-lib.cpp"
    break;

  case 684: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3608 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10278 "bison-chpl-lib.cpp"
    break;

  case 685: /* binary_op_expr: expr TPLUS expr  */
#line 3616 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10284 "bison-chpl-lib.cpp"
    break;

  case 686: /* binary_op_expr: expr TMINUS expr  */
#line 3617 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10290 "bison-chpl-lib.cpp"
    break;

  case 687: /* binary_op_expr: expr TSTAR expr  */
#line 3618 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10296 "bison-chpl-lib.cpp"
    break;

  case 688: /* binary_op_expr: expr TDIVIDE expr  */
#line 3619 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10302 "bison-chpl-lib.cpp"
    break;

  case 689: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3620 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10308 "bison-chpl-lib.cpp"
    break;

  case 690: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3621 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10314 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TMOD expr  */
#line 3622 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10320 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TEQUAL expr  */
#line 3623 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10326 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3624 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10332 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3625 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10338 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3626 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10344 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TLESS expr  */
#line 3627 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10350 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TGREATER expr  */
#line 3628 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10356 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TBAND expr  */
#line 3629 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10362 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TBOR expr  */
#line 3630 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10368 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TBXOR expr  */
#line 3631 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10374 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TAND expr  */
#line 3632 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10380 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TOR expr  */
#line 3633 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10386 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TEXP expr  */
#line 3634 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10392 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TBY expr  */
#line 3635 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10398 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TALIGN expr  */
#line 3636 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10404 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr THASH expr  */
#line 3637 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10410 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TDMAPPED expr  */
#line 3638 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10416 "bison-chpl-lib.cpp"
    break;

  case 708: /* unary_op_expr: TPLUS expr  */
#line 3642 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10422 "bison-chpl-lib.cpp"
    break;

  case 709: /* unary_op_expr: TMINUS expr  */
#line 3643 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10428 "bison-chpl-lib.cpp"
    break;

  case 710: /* unary_op_expr: TMINUSMINUS expr  */
#line 3644 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10434 "bison-chpl-lib.cpp"
    break;

  case 711: /* unary_op_expr: TPLUSPLUS expr  */
#line 3645 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10440 "bison-chpl-lib.cpp"
    break;

  case 712: /* unary_op_expr: TBANG expr  */
#line 3646 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10446 "bison-chpl-lib.cpp"
    break;

  case 713: /* unary_op_expr: expr TBANG  */
#line 3647 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10454 "bison-chpl-lib.cpp"
    break;

  case 714: /* unary_op_expr: TBNOT expr  */
#line 3650 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10460 "bison-chpl-lib.cpp"
    break;

  case 715: /* reduce_expr: expr TREDUCE expr  */
#line 3655 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10468 "bison-chpl-lib.cpp"
    break;

  case 716: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3659 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10476 "bison-chpl-lib.cpp"
    break;

  case 717: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3663 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10484 "bison-chpl-lib.cpp"
    break;

  case 718: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3667 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10492 "bison-chpl-lib.cpp"
    break;

  case 719: /* scan_expr: expr TSCAN expr  */
#line 3674 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10500 "bison-chpl-lib.cpp"
    break;

  case 720: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3678 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10508 "bison-chpl-lib.cpp"
    break;

  case 721: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3682 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10516 "bison-chpl-lib.cpp"
    break;

  case 722: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3686 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10524 "bison-chpl-lib.cpp"
    break;


#line 10528 "bison-chpl-lib.cpp"

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
