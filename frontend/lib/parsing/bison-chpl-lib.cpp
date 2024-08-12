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
#line 355 "chpl.ypp"

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
#define YYLAST   18096

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  182
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  203
/* YYNRULES -- Number of rules.  */
#define YYNRULES  742
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1266

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
       0,   664,   664,   668,   669,   674,   675,   683,   687,   697,
     702,   709,   710,   711,   712,   713,   714,   715,   716,   717,
     718,   719,   737,   738,   739,   747,   756,   765,   769,   776,
     784,   785,   786,   787,   788,   789,   790,   791,   793,   813,
     821,   838,   854,   871,   888,   904,   925,   926,   930,   940,
     941,   945,   949,   950,   954,   961,   969,   972,   982,   984,
     989,   995,  1000,  1009,  1013,  1034,  1051,  1068,  1086,  1087,
    1089,  1094,  1095,  1100,  1118,  1123,  1128,  1140,  1159,  1160,
    1164,  1168,  1172,  1177,  1181,  1185,  1193,  1198,  1204,  1209,
    1218,  1219,  1223,  1228,  1236,  1245,  1253,  1264,  1272,  1273,
    1274,  1275,  1276,  1277,  1278,  1279,  1280,  1281,  1282,  1283,
    1284,  1285,  1286,  1287,  1288,  1300,  1304,  1309,  1314,  1320,
    1329,  1330,  1334,  1343,  1347,  1351,  1355,  1359,  1363,  1372,
    1373,  1374,  1378,  1379,  1380,  1381,  1382,  1386,  1387,  1388,
    1400,  1401,  1402,  1407,  1408,  1409,  1410,  1411,  1412,  1413,
    1414,  1415,  1416,  1417,  1418,  1419,  1420,  1421,  1422,  1423,
    1424,  1425,  1429,  1430,  1431,  1432,  1433,  1434,  1435,  1436,
    1437,  1438,  1439,  1440,  1447,  1448,  1449,  1450,  1454,  1455,
    1459,  1466,  1476,  1480,  1486,  1486,  1492,  1492,  1501,  1502,
    1503,  1504,  1505,  1506,  1507,  1511,  1516,  1521,  1526,  1533,
    1541,  1542,  1546,  1555,  1562,  1571,  1590,  1589,  1599,  1598,
    1611,  1617,  1620,  1621,  1630,  1631,  1632,  1633,  1637,  1660,
    1679,  1698,  1723,  1728,  1733,  1738,  1743,  1748,  1753,  1758,
    1763,  1768,  1773,  1778,  1783,  1788,  1793,  1799,  1808,  1817,
    1821,  1825,  1829,  1833,  1837,  1841,  1845,  1849,  1854,  1859,
    1864,  1872,  1887,  1905,  1909,  1916,  1917,  1922,  1927,  1928,
    1929,  1930,  1931,  1932,  1933,  1934,  1935,  1936,  1937,  1938,
    1943,  1948,  1949,  1950,  1951,  1960,  1961,  1965,  1969,  1973,
    1980,  1984,  1988,  1995,  1999,  2003,  2007,  2014,  2015,  2019,
    2023,  2027,  2034,  2047,  2063,  2071,  2076,  2086,  2087,  2091,
    2095,  2100,  2109,  2114,  2118,  2125,  2126,  2130,  2140,  2146,
    2158,  2165,  2166,  2167,  2171,  2172,  2176,  2180,  2184,  2188,
    2192,  2200,  2219,  2232,  2239,  2244,  2251,  2250,  2260,  2266,
    2265,  2280,  2287,  2299,  2317,  2314,  2343,  2347,  2348,  2350,
    2355,  2356,  2360,  2361,  2365,  2368,  2370,  2375,  2376,  2387,
    2408,  2407,  2423,  2422,  2440,  2450,  2447,  2475,  2484,  2493,
    2503,  2513,  2522,  2537,  2538,  2542,  2543,  2544,  2553,  2554,
    2555,  2556,  2557,  2558,  2559,  2560,  2561,  2562,  2563,  2564,
    2565,  2566,  2567,  2568,  2569,  2570,  2571,  2572,  2573,  2574,
    2575,  2576,  2577,  2581,  2582,  2583,  2584,  2585,  2586,  2587,
    2588,  2589,  2590,  2591,  2592,  2597,  2598,  2602,  2603,  2607,
    2608,  2612,  2613,  2617,  2618,  2622,  2623,  2627,  2631,  2635,
    2640,  2645,  2650,  2655,  2663,  2667,  2675,  2676,  2677,  2678,
    2679,  2680,  2681,  2682,  2683,  2684,  2688,  2689,  2690,  2691,
    2692,  2693,  2694,  2695,  2696,  2697,  2698,  2702,  2703,  2704,
    2708,  2709,  2710,  2711,  2712,  2713,  2714,  2715,  2716,  2717,
    2718,  2722,  2723,  2726,  2727,  2731,  2732,  2736,  2741,  2742,
    2744,  2746,  2748,  2753,  2755,  2760,  2762,  2764,  2766,  2768,
    2770,  2772,  2777,  2778,  2779,  2783,  2792,  2796,  2804,  2808,
    2815,  2837,  2838,  2843,  2844,  2845,  2846,  2847,  2852,  2851,
    2861,  2860,  2870,  2875,  2883,  2887,  2894,  2913,  2932,  2936,
    2943,  2945,  2947,  2954,  2955,  2956,  2960,  2964,  2965,  2966,
    2970,  2971,  2975,  2976,  2980,  2981,  2982,  2983,  2987,  2988,
    2992,  2996,  2997,  3003,  3004,  3008,  3012,  3016,  3023,  3024,
    3028,  3033,  3042,  3043,  3047,  3048,  3055,  3056,  3057,  3058,
    3059,  3060,  3062,  3064,  3066,  3068,  3074,  3076,  3079,  3081,
    3083,  3085,  3087,  3089,  3091,  3093,  3095,  3097,  3102,  3106,
    3110,  3114,  3118,  3122,  3131,  3140,  3152,  3156,  3160,  3164,
    3168,  3172,  3176,  3180,  3187,  3205,  3213,  3214,  3215,  3216,
    3217,  3218,  3219,  3223,  3224,  3228,  3232,  3239,  3246,  3256,
    3257,  3261,  3265,  3269,  3276,  3277,  3278,  3279,  3280,  3281,
    3282,  3283,  3284,  3285,  3289,  3291,  3293,  3295,  3297,  3303,
    3310,  3322,  3334,  3347,  3364,  3371,  3376,  3381,  3386,  3392,
    3398,  3428,  3435,  3442,  3443,  3447,  3449,  3450,  3452,  3454,
    3455,  3456,  3457,  3460,  3461,  3462,  3463,  3464,  3465,  3466,
    3467,  3468,  3472,  3473,  3477,  3478,  3479,  3483,  3484,  3485,
    3486,  3495,  3496,  3499,  3500,  3501,  3505,  3517,  3529,  3536,
    3538,  3540,  3542,  3544,  3546,  3555,  3561,  3565,  3569,  3576,
    3577,  3581,  3582,  3586,  3587,  3588,  3589,  3590,  3591,  3592,
    3593,  3598,  3603,  3608,  3613,  3618,  3627,  3632,  3641,  3642,
    3643,  3644,  3645,  3646,  3647,  3648,  3649,  3650,  3651,  3652,
    3653,  3654,  3655,  3656,  3657,  3658,  3659,  3660,  3661,  3662,
    3663,  3667,  3668,  3669,  3670,  3671,  3672,  3675,  3679,  3683,
    3687,  3691,  3698,  3702,  3706,  3710,  3718,  3719,  3720,  3721,
    3722,  3723,  3724
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

#define YYPACT_NINF (-915)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-743)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -915,   112,  4479,  -915,   -90,    68,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,  -915, 12326,     8,    82,    85,  2901,   217,
   17867,     8,   231,   196,   235,    82,  5892, 12326,   165, 17923,
    -915,   331,   302,  -915, 10411,   345,   188,   215,  -915,   406,
     349, 17923, 17923, 17923,  -915,  2108, 11279,   409, 12326, 12326,
     276,  -915,   421,   436, 12326,  -915,  2901,  -915, 12326,   492,
     343,   237,   239, 13537,   453, 17979,  -915, 12326,  8505, 12326,
   11279,  2901, 12326,   424,   494,   386,  5892,   526, 12326,   529,
    7467,  7467,  -915,   530,  -915,  2901,  -915,   531, 10584,  -915,
   12326,  -915, 12326,  -915,  -915, 14014, 12326,  -915, 10757,  -915,
    -915,  -915,  4836,  8678, 12326,  -915,  5540,  -915,  -915,  -915,
    -915, 17616,   577,  -915,   439,   419,  -915,   190,  -915,  -915,
    -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,
    -915,  7813,  -915,  7986,  -915,  -915,    21,  -915,  -915,  -915,
     534,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,   446,
   17923,  -915,   423,   426,   348,  -915,   236,  -915,  -915,  -915,
     524,   525,  -915,  -915, 17923,  1666,  -915,   430,   435,  -915,
    -915,  -915,   448,   449, 12326,   452,   454,  -915,  -915,  -915,
   17433,  1863,   205,   456,   458,  -915,  -915,   329,  -915,  -915,
    -915,  -915,  -915,    80,  -915,  -915,  -915, 12326,  -915, 17923,
    -915, 12326, 12326,    64,   554,   372,  -915,  -915,  -915,  -915,
   17433,   382,  -915,  -915,    43,  5892,  -915,  -915,  -915,   459,
     168,   441,  -915,   232,  -915,   461,  -915,   199, 17433,  8851,
    -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915, 17923,
   17923,   -44, 14537,  -915,  -915,   542,  8851,   467,   464,   387,
    5892,  3312,    76,     9,  -915,  5892,  -915,  -915, 15521,  -915,
      19, 15972,   839,  -915,   466,   470,  -915, 15521,   168,   839,
    -915,  8851,  1585,  1585,    36,    58,  -915,    88,  -915,  -915,
    -915,  -915,  8159,  8159,  -915,  -915,   515,  -915,   471,   499,
    -915,  -915,  3988,   501,  -915,  -915, 17433,   233, 12326, 12326,
    -915,   -33,  -915,  -915, 17433,   480, 16012,  -915, 15521,   168,
   17433,   476,  8851,  -915, 17433, 16052, 12326,  -915,  -915,  -915,
    -915,  -915,   168,   487,   225,   225,  2344,   839,   839,   229,
    -915,  -915,  5012,   108, 12326,   506,   -73,   482,  2344,   648,
    -915,  -915, 17923,  -915, 12326,  -915,  4836,  -915,   107,  -915,
     542,  -915,  -915,   662,   493,  5188, 12326,  -915, 12326,   600,
    -915,  -915, 15191,    73,   446, 17433,   363,  -915,  5892,   589,
    -915,  -915, 10411, 10930, 11452,  -915,  -915,  -915, 17923, 12326,
     500,  -915,  3422,   680,   524,  -915,  -915,  -915,  -915,   170,
    -915,  -915,  -915,  -915,  -915,  -915,  -915,    37,   187,  -915,
    -915,  -915,  -915,  -915,  -915,  -915, 13846,   544,   249,  -915,
     507,   264,   389,  -915,   522, 12326, 12326, 12326, 11452, 11452,
   12326,   393, 12326, 12326, 12326, 12326, 12326,   280, 14014, 12326,
   12326, 12326, 12326, 12326, 12326, 12326, 12326, 12326, 12326, 12326,
   12326, 12326, 12326, 12326, 12326,   604,  -915,  -915,  -915,  -915,
    -915, 10584, 10584,  -915,  -915,  -915,  -915, 10584,  -915,  -915,
   10584, 10584,  8851,  9024, 11452, 11452, 16132,   503,  -915, 11625,
   11452, 17923,  -915,  4286,  -915,  -915,  -915,  2108,  -915, 12326,
    -915,   553,   510,   532,  -915,  -915,   555,   561,  5892,   656,
    5892,  -915,   674, 12326,   646,   540,  -915, 10584, 17923,  -915,
    -915,  3312,  -915,  -915,   -58,  -915, 12499,   590, 12326,  2108,
    -915,  -915, 12326,  -915, 17755, 12326, 12326,  -915,   232,   541,
    -915,  -915,  -915,  -915, 17923,  -915,   524, 13710,  3312,   570,
   12499,  -915, 17433, 17433,  -915,   441,  -915,    50,  -915,  8851,
     548,  -915, 15521,   701,   701,  -915,  -915,  -915,  -915, 11801,
    -915, 16172,  9200,  9373,  -915,  -915,  -915, 17433, 11452, 11452,
    -915,   439,  9546,  -915,   277,  -915,  5364,  -915,   268, 16212,
     274, 15282, 17923,  7291,  7118,  -915,   446,   556,  -915,   232,
    -915,   605, 17923,    34, 15521,   558, 14187,   -65,   117, 16292,
     -68,    33, 15021,   164,   578,  1758,   563, 17811,   614,   194,
    -915,  -915,   -53,  -915,  -915,    40,  -915,  -915,  -915,  6945,
    -915,   635,  -915,  -915,   572,   597,  -915,   599,  -915,   601,
     603,   608,   609,   611,  -915,   615,   620,   625,   629,   630,
     631,   633,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,  -915, 12326,   622,   634,   572,  -915,   572,
    -915, 12326,  -915, 17923,  -915,  -915,  -915,   543,   543,   370,
    -915,   370,  -915,   370, 17193,   879,   587,   967,   168,   225,
    -915,   396,  -915,  -915,  -915,  -915,  -915,  2344, 17593,   370,
    1411,  1411,   543,  1411,  1411,   330,   225, 17593,  1862,   330,
     839,   839,   225,  2344,   584,   602,   621,   624,   626,   628,
     612,   619,  -915,   370,  -915,   370, 12326, 10584,   118,   755,
   15602,   627,   623,   221,  -915,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,   143,  -915,  2586, 17473,   397,   284, 17433, 10584,
    -915, 10584, 12326, 12326,   764,  5892,   766, 16371,  5892, 15681,
   17923,  -915,   160,  -915,   167,  -915,   171,  3312,    76,  -915,
   17433,  9719,  -915, 17433,  -915,  -915,  -915, 17923, 16412, 16452,
    2108,  -915,   570,   658,  -915,   183, 12499,  -915,   206,  -915,
   12326,  -915,   636,    16,   632,  -915,  1861,  -915,  -915,  -915,
    -915,  -915,  -915,  -915,  -915,   133,    77, 15100,  -915,  -915,
     213,  -915,  -915,  -915,  -915, 12326,  -915,    84, 14293, 12326,
    -915, 12326,  7291,  7118,  -915,  9892,   420, 10065,   444,   445,
   10238,  8332,   455,   285,  -915,  -915,  -915, 16532,   659,   644,
     640,  -915, 17923,  2108, 12326,   768,  -915,  -915, 12326, 17433,
    5892,   641, 11452, 11974,  4660,   643, 12326, 12150,  -915,  5892,
    -915,    34,  6068, 11452,  -915,    34,  -915, 10584,   654,  2246,
    -915,  -915, 12326,    53,  -915,   750,  -915,   635,  -915,  -915,
    -915,  -915,  -915,     3,   741,  -915, 16572,  -915, 14453,  -915,
    -915, 17433,  -915,   655,   663,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,   222, 11452, 14187, 12326, 12672, 11452, 10584,
    -915,  -915,  -915,  7640,  -915,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,  -915,  -915,  -915,    47, 11452, 11452,  -915,
    -915,  -915, 17433, 17433,  5892,  -915,  5892, 12326,  -915,  5892,
     796,   669,   672, 17923,   441,  -915,  -915,   590,  -915,  -915,
    -915,   667,   668,   305,   206, 12326,   635,   570,  -915, 11106,
    -915,  -915, 17433,  -915,  5892, 12326,  -915,  -915,  -915, 17923,
    -915,   697,   441, 11452,  5892, 11452, 10584,  -915,  -915,  -915,
    -915,  -915,   396,  -915,  -915,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,  -915,  -915,  -915,  -915,  7291,  -915,  -915,
   16618, 15773,   316,   679,  7291,  -915, 12326,  -915,  -915,  2504,
     317,   337, 16724, 12326,   234,  -915,  5892,   -12, 15852,  -915,
    -915, 17433,  5892, 17433,  -915, 16770, 17433,  -915, 10411, 17433,
     129, 15361,  -915,   245,  -915,  -915,  -915,  -915,  1430,  3656,
    -915, 17433, 17923,   577,  -915,     3,    78, 12326,  -915,   698,
     673,  -915,   572,   572,  -915,  -915,  -915,   675, 16810, 12326,
     815, 16962,   255,  -915,   590,   265,   278,  -915,  -915, 17433,
     818,  5892,  -915,  -915,  -915,  -915,  -915,  8851,  8851,  -915,
    -915,  -915,  -915,  -915,   206, 17433,  -915,   110,    26,   676,
   12326,  -915,    -7, 15932,    77, 15442,  -915,   212, 12326,  7291,
    7118,  -915,  -915,  -915, 17002, 17433,  4112,  -915,  -915,  -915,
    -915, 15521,  -915,  -915,  6244,   683,  6420,   684,  -915, 12326,
   14616,  6596,  -915,    34,  6772,  -915,    34,   689,  3656,  -915,
    -915,  -915, 17811,  -915,  -915,  -915,   248,  -915,   -31,  -915,
     618, 17048,    87,  -915,  -915,  -915, 12845, 13018, 17154, 13191,
   13364,  -915,  -915,  5892,  -915,   699,   700,  -915,  -915,  5892,
    -915,   441, 17433,  5892,  5892,  -915, 17433,   341,   706, 12326,
    -915,  -915, 10411,  -915, 17433,  5892, 10411,  -915, 17433,  5892,
   17433,  5892, 10411, 17433,  -915, 10411, 17433,  -915,  -915,  -915,
    -915,    70,  -915, 12326,    78,   248,   248,   248,   248,   248,
     248,    78,  5716,  -915,  -915,  -915,  -915, 12326, 12326, 12326,
   12326, 12326,  -915,   710,   711,   605,  -915,  -915,  -915, 17433,
   14697,  -915, 14776,  -915, 16371, 14857, 14936,  -915, 17433,  -915,
    -915,  -915,  -915,  -915,  -915,  -915,   715,  -915, 17233, 17313,
   17353, 17393,  -915,  -915,  5892,  5892,  5892,  5892, 12326, 12326,
   12326, 12326, 16371, 16371, 16371, 16371
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   140,   467,    55,   685,   686,
     687,   681,   682,   688,   211,   593,   129,   162,   564,   169,
     566,   593,   168,   498,   496,   129,     0,   211,   275,   163,
     201,   200,   679,   199,   211,   167,    68,   276,   338,   164,
     141,     0,     0,     0,   333,     0,   211,   171,   211,   211,
     614,   585,   689,   172,   211,   339,   558,   493,   211,     0,
       0,   186,   184,   424,   166,   567,   495,   211,   211,   211,
     211,   562,   211,     0,   170,     0,     0,   142,   211,   680,
     211,   211,   486,   165,   313,   560,   497,   173,   211,   738,
     211,   740,   211,   741,   742,   630,   211,   739,   211,   638,
     182,   737,     0,   211,   211,     4,     0,     5,    10,    11,
      12,    48,    51,    52,    56,     0,    46,    71,    13,    77,
      14,    15,    16,    17,    30,   544,   545,    23,    47,   183,
     193,   211,   202,   652,   194,    18,     0,    32,    31,    20,
       0,   270,    19,   648,    22,    36,    33,    34,   192,   314,
       0,   190,     0,     0,   645,   337,     0,   642,   188,   354,
     450,   436,   640,   191,     0,     0,   189,   657,   636,   635,
     639,   548,   546,     0,   211,   646,   647,   651,   650,   649,
       0,   547,     0,   658,   659,   660,   683,   684,   641,   550,
     549,   643,   644,     0,    29,   566,   163,   211,   141,     0,
     567,   211,   211,     0,     0,   645,   657,   546,   646,   647,
     556,   547,   658,   659,     0,     0,   594,   130,   131,     0,
     565,     0,   487,     0,   494,     0,    21,     0,   533,   211,
     137,   143,   155,   149,   148,   157,   134,   147,   158,   144,
     138,   159,   132,   160,   153,   146,   154,   152,   150,   151,
     133,   135,   145,   156,   161,   139,   323,   136,   210,     0,
       0,     0,     0,    70,    69,    71,   211,     0,     0,     0,
       0,     0,     0,     0,   504,     0,   179,    41,     0,   305,
       0,   304,   723,   618,   615,   616,   617,   500,   559,   724,
       7,   211,   336,   336,   431,   176,   426,   177,   427,   428,
     432,   433,   175,   174,   434,   435,   424,   529,     0,   341,
     342,   344,     0,   425,   528,   346,   516,     0,   211,   211,
     180,   642,   633,   656,   634,     0,     0,    44,     0,   563,
     551,     0,   211,    45,   557,     0,   211,   283,    48,   287,
     284,   287,   561,     0,   725,   727,   628,   722,   721,     0,
      74,    78,     0,     0,   211,     0,     0,   535,   629,     0,
       6,   311,     0,   200,   211,   312,     0,    49,     0,     9,
      71,    50,    53,     0,    60,     0,   211,    72,   211,     0,
     498,   198,     0,   681,   314,   653,   206,   217,     0,   214,
     215,   216,   211,   211,   211,   213,   568,   576,     0,   211,
       0,   310,     0,   424,   450,   448,   449,   447,   362,   452,
     457,   460,   451,   455,   454,   456,   459,     0,   440,   442,
     446,   445,   437,   438,   441,   444,     0,   491,     0,   488,
       0,     0,   637,    35,   619,   211,   211,   211,   211,   211,
     211,   726,   211,   211,   211,   211,   211,     0,   627,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   393,   400,   401,   402,   397,
     399,   211,   211,   395,   398,   396,   394,   211,   404,   403,
     211,   211,   211,   211,   211,   211,     0,     0,   214,   211,
     211,     0,   598,     0,    37,    25,    38,     0,    26,   211,
      39,   544,     0,   539,   540,   543,     0,     0,     0,   244,
       0,   393,   240,   211,     0,     0,    24,   211,     0,   254,
      27,     0,   508,   510,     0,   527,   211,   513,   211,     0,
     178,    40,   211,   307,     0,   211,   211,    42,     0,     0,
     187,   185,   429,   430,     0,   425,   450,   424,     0,   531,
     211,   122,   654,   655,   352,     0,   181,     0,    43,   211,
       0,   294,     0,   285,   286,    28,    76,    75,    79,   211,
     690,     0,   211,   211,   677,   675,     8,   500,   211,   211,
     212,    56,   211,    54,     0,    65,     0,   120,     0,   115,
       0,    86,     0,   211,   211,   195,   314,     0,   208,     0,
     205,     0,     0,     0,     0,     0,   577,     0,     0,   533,
       0,     0,     0,   281,   315,     0,     0,     0,   331,     0,
     324,   411,     0,   413,   417,     0,   458,   453,   521,     0,
     523,   461,   443,   439,   409,   137,   389,   143,   387,   149,
     148,   147,   144,   138,   391,   159,   160,   146,   150,   133,
     145,   161,   386,   368,   371,   369,   370,   392,   381,   372,
     385,   377,   375,   388,   376,   374,   379,   384,   373,   378,
     382,   383,   380,   390,   211,   365,     0,   409,   366,   409,
     363,   211,   490,     0,   485,   503,   502,   718,   717,   720,
     729,   728,   733,   732,   714,   711,   712,   713,   631,   701,
     140,     0,   671,   672,   142,   670,   669,   625,   705,   716,
     710,   708,   719,   709,   707,   699,   704,   706,   715,   698,
     702,   703,   700,   626,     0,     0,     0,     0,     0,     0,
       0,     0,   731,   730,   735,   734,   211,   211,     0,     0,
       0,   281,     0,   604,   605,   611,   610,   612,   607,   613,
     609,   596,     0,   599,     0,     0,     0,     0,   534,   211,
     553,   211,   211,   211,   243,     0,   239,     0,     0,     0,
       0,   552,     0,   257,     0,   255,     0,   511,     0,   526,
     525,   211,   506,   624,   505,   306,   303,     0,     0,     0,
       0,   668,   531,   347,   343,     0,   211,   532,   513,   345,
     211,   351,     0,     0,     0,   554,     0,   288,   691,   632,
     676,   536,   535,   678,   537,     0,     0,     0,    57,    58,
       0,    61,    63,    67,    66,   211,    97,     0,     0,   211,
      92,   211,   211,   211,    64,   211,   368,   211,   369,   370,
     211,   211,   380,     0,   405,   406,    81,    80,    91,     0,
       0,   316,     0,     0,   211,     0,   220,   219,   211,   578,
       0,     0,   211,   211,     0,     0,   211,   211,   694,     0,
     228,     0,     0,   211,   226,     0,   278,   211,     0,   336,
     322,   328,   211,   326,   321,   424,   412,   461,   519,   518,
     517,   520,   462,   468,   424,   361,     0,   367,     0,   357,
     358,   492,   489,     0,     0,   128,   126,   127,   125,   124,
     123,   666,   667,     0,   211,   692,   211,   211,   211,   211,
     597,   606,   608,   211,   595,   162,   169,   168,   167,   164,
     171,   172,   166,   170,   165,   173,     0,   211,   211,   499,
     542,   541,   252,   251,     0,   246,     0,   211,   242,     0,
     248,     0,   280,     0,     0,   509,   512,   513,   514,   515,
     302,     0,     0,     0,   513,   211,   461,   531,   530,   211,
     420,   418,   353,   296,     0,   211,   295,   298,   555,     0,
     289,   292,     0,   211,     0,   211,   211,    59,   121,   118,
      98,   110,   104,   103,   102,   112,    99,   113,   108,   101,
     109,   107,   105,   106,   100,   111,   114,   211,   117,   116,
      88,    87,     0,     0,   211,   196,   211,   197,   316,   336,
       0,     0,     0,   211,     0,   236,     0,     0,     0,   693,
     234,   579,     0,   696,   695,     0,   572,   229,   211,   571,
       0,     0,   227,     0,   309,   186,   184,   308,   336,   336,
     317,   332,     0,     0,   414,   468,     0,   211,   355,   416,
       0,   364,   409,   409,   673,   674,   280,     0,     0,   211,
       0,     0,     0,   600,   513,   657,   657,   245,   241,   584,
     247,     0,    73,   277,   256,   253,   507,   211,   211,   501,
     421,   419,   348,   349,   513,   408,   300,     0,     0,     0,
     211,   290,     0,     0,     0,     0,    62,     0,   211,   211,
     211,    93,    95,    84,    83,    82,   336,   203,   209,   207,
     218,     0,   238,   237,     0,     0,     0,     0,   235,   211,
       0,     0,   224,     0,     0,   222,     0,   282,   336,   319,
     318,   327,     0,   334,   482,   483,     0,   484,   470,   473,
       0,   469,     0,   410,   359,   360,   211,   211,     0,   211,
     211,   282,   601,     0,   250,     0,     0,   423,   422,     0,
     299,     0,   293,     0,     0,   119,    89,     0,     0,   211,
     204,   221,   211,   232,   581,     0,   211,   230,   580,     0,
     697,     0,   211,   570,   225,   211,   569,   223,   279,   320,
     330,     0,   481,   211,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   463,   465,   356,   464,   211,   211,   211,
     211,   211,   249,   620,   621,   301,   291,    94,    96,    85,
       0,   233,     0,   231,   575,     0,     0,   335,   472,   474,
     475,   478,   479,   480,   476,   477,   471,   466,     0,     0,
       0,     0,   622,   623,     0,     0,     0,     0,   211,   211,
     211,   211,   583,   582,   574,   573
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -915,  -915,  -915,    -1,   -57,  2500,  -915,   126,   767,  -915,
    -915,   498,  -398,  -104,  -915,   298,  -915,  -915,  -103,  -915,
    -915,    35,   617,  -915,  -539,  3008,   505,  -580,  -915,  -811,
    -915,  -915,  -915,    59,  -915,  -915,  -915,   860,  -915,  3574,
    -219,  -915,  -915,  -471,    48,  -915,  -849,  -915,  -915,   288,
    -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,    17,  -915,
    -142,   519,  -308,  -915,   496,  -915,  -915,   -63,  1533,  -915,
    -915,  -915,  -915,   551,  -915,   -86,  -915,  -915,  -915,  -915,
     362,  -915,  -915,  -915,  -106,  -915,  -350,  -809,  -915,  -915,
    -915,  -915,  -915,  -608,  -915,   134,  -915,  -915,  -915,  -915,
    -915,   350,  -915,   332,  -915,  -915,  -915,  -915,   509,  -915,
    -915,  -915,  -915,     0,  -411,  -179,  -797,  -914,  -656,  -915,
       2,  -915,    15,   -52,   595,   -51,  -915,  -915,  -355,  -805,
    -915,  -298,  -915,  -149,  -304,  -296,  -748,  -915,  -915,    57,
     227,  -915,  -205,   781,  -915,  -915,  -158,   384,   139,  -251,
    -772,  -609,  -915,  -915,  -915,  -600,  -518,  -915,  -755,   -22,
    -162,  -915,  -263,  -488,  -568,    -2,  -915,  -915,  -915,  -915,
    -915,  -915,   153,  -915,   896,  -572,  -915,    -3,  -915,  -915,
     468,   929,  -915,  -915,  -915,  -915,  2728,  -915,   -35,  1218,
    -915,  1280,  1471,  -915,  -915,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -476
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   351,   106,   107,   108,   109,   110,   368,
     369,   111,   112,   113,   114,   374,   583,   820,   821,   115,
     116,   117,   379,   118,   119,   120,   352,   848,   590,   849,
     121,   122,  1008,   587,   588,   123,   124,   219,   255,   272,
     125,   257,   126,   307,   537,   127,   128,   293,   292,   129,
     130,   131,   132,   133,   134,   599,   852,   135,   203,   137,
     394,   395,   607,   138,   261,   139,   774,   775,   204,   141,
     142,   143,   144,   563,   807,   982,   145,   146,   803,   977,
     279,   280,   147,   148,   149,   150,   400,   879,   151,   152,
     619,  1052,  1053,   620,   153,   205,  1201,   155,   156,   308,
     309,   310,   966,   157,   322,   555,   800,   158,   159,  1152,
     160,   161,   676,   677,   844,   845,   846,   970,   895,   404,
     622,  1060,   623,   624,   312,   545,   426,   408,   417,   893,
    1215,  1216,   162,  1058,  1148,  1149,  1150,   163,   164,   428,
     429,   682,   165,   166,   223,   538,   273,   274,   523,   524,
     782,   314,   891,   630,   631,   527,   315,   797,   798,   353,
     355,   356,   502,   503,   504,   206,   168,   169,   396,   170,
     397,   171,   207,   173,   215,   216,   752,   753,   754,   174,
     208,   209,   177,   178,   179,   323,   180,   386,   505,   211,
     182,   212,   213,   185,   186,   187,   188,   610,   189,   190,
     191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   105,   481,   515,   617,   227,   306,   431,   372,   881,
     501,   311,   313,   843,   822,   678,   801,   756,   497,   136,
     890,   899,  1013,   900,   167,   887,   971,   384,   539,   772,
    1050,  1009,   799,   325,   597,   861,   865,   964,   628,   871,
     875,   525,  1019,   136,   492,   317,   387,   501,   525,   625,
    1090,   802,   275,   343,   388,   779,   214,   528,  -329,   275,
     508,   490,   389,   390,   391,  1056,   869,   275,   357,   560,
    -522,   265,   501,  -524,   167,   573,   194,   525,   167,   167,
     867,  1144,  1055,   513,   542,   217,   611,   700,  1203,   387,
     777,   554,   974,   136,   277,   885,   361,   136,   136,  -522,
     167,   574,  -524,  1212,   167,   488,   390,   391,   868,   214,
     275,   860,     3,   501,   214,  -275,   778,  1204,   327,   136,
    1212,   886,  1057,   136,  -524,  -258,  -297,   366,   543,   214,
     690,   692,   387,  1145,   975,   387,   154,   218,  -350,   198,
     388,   392,  -259,  1169,  -522,  -276,   370,  -524,   389,   390,
     391,   389,   390,   391,   214,   172,  -522,   529,   889,  -524,
     154,  1093,  1131,   365,  1124,   862,   914,   532,  -297,  1173,
    1050,   484,  1146,  -524,   499,   485,   732,   734,   957,   172,
    1167,   983,   739,  1147,   629,  1086,    84,   526,   976,   704,
     366,   793,  1091,   941,   526,  -524,   393,   366,   214,  1139,
    1140,  -325,  -524,  -522,   366,   366,   337,   340,  -522,  1116,
     154,  -524,  1094,   167,   154,   154,   493,  1113,   626,   730,
     731,  -524,  -297,   526,  -524,  -325,   579,   392,   706,   172,
     392,   229,   136,   172,   172,   632,   154,   376,   214,   489,
     154,   366,  -524,   865,   871,   875,   850,   679,   366,   913,
     214,  1144,  1012,  1213,   214,   172,   569,   437,   366,   172,
      24,   266,   627,   501,   501,   863,   863,  1050,   167,   921,
     776,   816,   263,   167,  -265,   264,   377,   596,   968,   633,
     570,   499,   578,   700,   -70,   393,   -69,   136,  -264,  1199,
     405,   923,   136,   864,   915,   283,   804,   795,   501,  1178,
     366,   701,  1162,  1145,   -70,   378,   -69,   222,   761,   984,
     441,   406,    57,   922,   702,   953,   438,   924,   447,   777,
     439,   407,  1168,   -70,    66,   -69,   531,   224,   533,   787,
     876,   777,   781,   790,   952,   198,  1074,   877,   229,   757,
     501,   954,   883,   258,   703,   955,   306,   499,    86,   154,
     167,   568,   -70,  1147,   -69,   284,  1092,   967,   969,  -273,
    1014,   986,   437,   501,   167,   500,   884,   441,   172,   136,
     761,   608,   445,   167,   285,   447,   558,   614,   482,   450,
     483,   499,   499,   136,  1175,   704,   167,   987,   266,  1043,
     286,   705,   136,   761,   853,   194,  1066,   683,  1202,   551,
     321,   566,  -263,   761,   154,   136,  1154,  1155,  1122,   154,
     811,   814,   529,  -603,    38,   684,   825,  -336,   822,  1137,
     321,   438,   829,   172,   680,   439,  -602,  1107,   172,  1161,
     686,  1072,   529,  1014,   826,   321,   725,   726,  -336,  -603,
     830,    55,   727,   194,  1141,   728,   729,   756,  -336,   823,
     939,  1015,  -602,   529,   267,  1125,  1127,  1240,  1241,  1242,
    1243,  1244,  1245,  -260,  1014,   683,  -267,   738,  1133,  1136,
     175,  1089,   441,   442,   598,   443,   444,   445,  -271,   446,
     447,  1049,  1111,  1118,   450,   529,   154,   678,   938,  1014,
     306,   457,   485,  -268,   175,   311,   313,   461,   462,   463,
     154,   290,  -665,  1119,  -665,   172,   167,  1227,   167,   154,
    -262,  -740,   441,   372,  -590,  -740,   291,   445,   501,   172,
     447,  -664,   154,  -664,   450,   136,   331,   136,   172,  1177,
    1125,  1127,  1133,  1136,  1200,  -741,  -742,   812,   812,  -741,
    -742,   172,   501,   294,   175,  -664,  -737,  -664,   175,   175,
    -737,  -266,   409,   418,  1027,  -661,   815,  -661,   366,   332,
     518,   321,  -663,   296,  -663,  1040,  -662,   298,  -662,   903,
     175,   904,   410,   419,   175,   437,   411,   420,   338,   338,
     540,   541,     7,  -272,   167,   568,  -274,  -261,  -269,   373,
     375,   398,   299,   399,   402,   300,  -587,   359,   321,   403,
     432,   412,   421,   136,   413,   422,  1067,   301,   989,   706,
    1070,   491,   366,  1214,  -586,   433,   414,   423,  -591,   437,
    -592,  1049,  -589,   321,  -588,   495,   304,   498,   377,   690,
     732,   305,   963,   516,   438,   415,   424,   517,   439,   535,
     416,   425,   154,   536,   154,   546,   556,   547,   550,   559,
    1138,   856,   857,   565,   572,  1142,   575,   576,   501,   870,
     874,   172,  1214,   172,   321,   581,   582,   592,   440,   602,
     681,   615,   447,   685,   724,  1102,   737,  1104,   438,   759,
     761,   762,   439,   175,   760,   441,   442,   763,   443,   444,
     445,   765,   446,   447,   448,  1021,   449,   450,   451,   452,
     501,   454,   455,   456,   457,   458,   459,   460,   294,   768,
     461,   462,   463,   770,   771,   791,   781,   796,  1049,  1063,
     154,   464,   805,   806,   940,   854,   499,   851,   296,   441,
     442,   858,   298,   444,   445,   880,   446,   447,   175,   172,
     882,   450,   892,   175,  1205,   894,   959,  -140,   457,  -162,
     905,  -169,   936,  -168,   461,   462,   463,   299,  -167,  -164,
     300,  -141,    59,   167,   897,  -171,   167,   501,   906,   294,
    -172,  1206,   301,  1207,  1208,  -166,  1209,  1210,   294,  -170,
    -142,  -165,   136,  -173,   898,   136,   911,   907,   916,   296,
     908,   304,   909,   298,   910,   912,   305,   920,   296,   944,
     919,   946,   298,   321,   321,   965,   978,  1014,   973,   321,
    1017,  1018,   321,   321,   321,   321,  1023,  1026,   299,  1032,
     175,   300,  1048,    59,  1165,  1166,  1044,   299,   306,  1064,
     300,  1081,    59,   301,   175,  1082,  1024,   306,  1083,  1065,
    1087,  1088,   301,   175,  1100,  1112,   885,  1153,  1159,   321,
    1171,  1156,   304,  1163,   621,  1198,   175,   305,   167,  1185,
    1189,   304,   167,  1204,   870,   874,   305,   167,   501,   501,
     167,   437,  1228,  1223,  1224,  1252,  1253,   136,   367,   818,
     586,   136,   514,  1106,   988,   225,   136,   580,   603,   136,
    1084,   321,   564,  1099,   785,   600,  1059,   794,  1062,   154,
    1054,   544,   154,  1237,   321,   321,  1143,  1246,  1239,  1020,
     902,   437,   381,   784,   321,  -415,   956,   221,   172,  1037,
    1073,   172,     0,  1042,     0,     0,     0,     0,     0,     0,
     438,   176,     0,     0,   439,  1075,  1076,     0,     0,     0,
       0,     0,   167,     0,   167,   372,     0,   167,     0,     0,
       0,   513,     0,  1098,     0,   176,     0,     0,     0,     0,
       0,   136,  1048,   136,     0,     0,   136,     0,     0,     0,
     438,     0,   167,     0,   439,     0,   175,     0,   175,     0,
       0,   441,   167,     0,     0,     0,   445,     0,   446,   447,
       0,  1097,     0,   450,   154,     0,     0,     0,   154,   437,
     457,   136,     0,   154,     0,   176,   154,     0,   463,   176,
     176,     0,     0,   172,     0,     0,     0,   172,     0,     0,
       0,   441,   172,     0,   167,   172,   445,     0,   446,   447,
     167,   176,     0,   450,   372,   176,     0,     0,   372,     0,
     457,     0,     0,   136,     0,     0,   461,   462,   463,   136,
       0,   513,     0,   513,   175,     0,   513,   513,   438,  1048,
       0,     0,   439,     0,     0,     0,     0,     0,     0,   321,
       0,     0,     0,     0,     0,     0,     0,     0,   154,   167,
     154,     0,     0,   154,     0,     0,     0,     0,  1132,  1135,
       0,   321,     0,   321,     0,     0,     0,   172,   136,   172,
       0,     0,   172,     0,     0,     0,     0,     0,   154,   441,
     442,     0,     0,   321,   445,     0,   446,   447,   154,     0,
       0,   450,   167,     0,   167,     0,     0,   172,   457,   167,
       0,     0,   167,     0,   461,   462,   463,   172,     0,     0,
       0,   136,     0,   136,   176,     0,  1170,     0,   136,     0,
       0,   136,  1132,  1135,     0,     0,     0,     0,     0,     0,
     154,   167,     0,     0,     0,     0,   154,   167,     0,  1181,
       0,   167,   167,     0,     0,     0,     0,     0,     0,   172,
     136,  1194,     0,   167,  1197,   172,   136,   167,     0,   167,
     136,   136,     0,     0,     0,     0,     0,     0,     0,   176,
       0,     0,   136,     0,   176,     0,   136,     0,   136,   321,
     167,  1247,     0,     0,     0,   154,     0,     0,     0,     0,
     181,     0,     0,     0,     0,     0,     0,     0,     0,   136,
       0,     0,     0,   175,   172,     0,   175,     0,     0,     0,
       0,     0,     0,     0,   181,     0,     0,     0,     0,     0,
       0,   321,   167,   167,   167,   167,     0,     0,   154,     0,
     154,     0,     0,     0,     0,   154,     0,     0,   154,     0,
       0,   136,   136,   136,   136,     0,     0,   172,     0,   172,
       0,   176,   183,     0,   172,     0,     0,   172,     0,     0,
       0,     0,     0,     0,   181,   176,     0,   154,   181,   181,
       0,     0,     0,   154,   176,     0,   183,   154,   154,     0,
       0,     0,     0,     0,     0,     0,   172,   176,   321,   154,
     181,     0,   172,   154,   181,   154,   172,   172,   175,     0,
       0,     0,   175,     0,     0,     0,     0,   175,   172,     0,
     175,     0,   172,     0,   172,     0,   154,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   183,     0,     0,     0,
     183,   183,     0,     0,     0,   172,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   183,     0,     0,     0,   183,     0,   154,   154,
     154,   154,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   172,   172,   172,
     172,     0,   175,     0,   175,     0,     0,   175,     0,   321,
     321,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   181,     0,     7,     0,   176,     0,   176,
       0,     0,   175,   437,     0,     0,     0,     0,     0,     0,
       0,     0,   175,   361,     0,     0,     0,    23,    24,     0,
       0,     0,     0,     0,     0,     0,   362,     0,    30,   363,
       0,     0,     0,   184,    33,     0,     0,     0,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,   181,     0,
       0,     0,     0,   181,   175,   183,     0,   184,     0,     0,
     175,     0,   438,   364,     0,     0,   439,     0,    55,     0,
      57,     0,   359,     0,  1045,   176,     0,  1046,     0,     0,
     365,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   140,     0,     0,     0,     0,
       0,    82,     0,    84,     0,     0,    86,   184,     0,   175,
     183,   184,   184,   441,   442,   183,   443,   444,   445,   140,
     446,   447,   448,     0,     0,   450,     0,     0,     0,     0,
     181,   456,   457,   184,     0,   460,     0,   184,   461,   462,
     463,     0,     0,     0,   181,     0,     0,     0,     0,   464,
       0,     0,   175,   181,   175,     0,   100,     0,     0,   175,
       0,     0,   175,     0,     0,     0,   181,     0,   361,   140,
       0,     0,    23,    24,     0,     0,     0,     0,     0,     0,
       0,   362,     0,    30,   363,     0,     0,     0,     0,    33,
       0,   175,   183,     0,     0,   140,    38,   175,     0,   140,
       0,   175,   175,     0,     0,     0,   183,     0,     0,     0,
       0,     0,     0,   175,     0,   183,     0,   175,   364,   175,
       0,     0,     0,    55,     0,    57,     0,   430,   183,   230,
       0,     0,     0,     0,     0,   365,     0,    66,     0,     0,
     175,     0,     0,     0,   231,   232,   184,   233,     0,     0,
       0,     0,   234,     0,   176,     0,    82,   176,    84,     0,
     235,    86,     0,     0,     0,     0,   236,     0,     0,     0,
       0,     0,   237,     0,     0,     0,   238,     0,     0,   239,
       0,   240,   175,   175,   175,   175,   181,     0,   181,     0,
     241,     0,     0,     0,     0,     0,     0,   242,   243,     0,
       0,   184,     0,     0,     0,   244,   184,     0,   140,     0,
       0,     0,     0,     0,     0,   245,     0,     0,     0,   878,
       0,     0,     0,  -316,   246,   247,     0,   248,     0,   249,
       0,   250,     0,     0,   251,     0,     0,     0,   252,     0,
     253,  -316,     0,   254,     0,  -316,  -316,     0,   183,   176,
     183,     0,     0,   176,  -316,     0,  -316,  -316,   176,     0,
       0,   176,  -316,   140,   181,     0,     0,     0,   140,  -316,
       0,     0,  -316,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   184,     0,     0,     0,     0,     0,     0,
       0,  -316,     0,  -316,     0,     0,  -316,   184,  -316,   271,
    -316,     0,  -316,  -316,     0,  -316,   184,     0,  -316,     0,
    -316,     0,     0,     0,     0,     0,     0,     0,     0,   184,
       0,     0,     0,     0,   230,     0,   183,     0,     0,  -316,
       0,  -316,     0,   176,  -316,   176,     0,     0,   176,   231,
     232,     0,   233,     0,     0,   140,     0,   234,     0,     0,
       0,     0,     0,     0,   437,   235,     0,     0,     0,   140,
       0,   236,     0,   176,     0,     0,     0,   237,   140,     0,
       0,   238,     0,   176,   239,     0,   240,     0,     0,     0,
       0,   140,     0,     0,  -316,   241,     0,     0,     0,     0,
    -316,     0,   242,   243,     0,     0,     0,     0,     0,     0,
     244,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,     0,     0,   438,     0,   176,     0,   439,     0,   246,
     247,   176,   248,     0,   249,     0,   250,     0,     0,   251,
       0,     0,     0,   252,     0,   253,     0,     0,   254,   184,
       0,   184,     0,   181,     0,     0,   181,   440,     0,   465,
     466,   467,   468,   469,   470,   471,   472,   473,   474,   475,
     476,   477,   478,   479,   441,   442,     0,   443,   444,   445,
     176,   446,   447,   448,     0,   449,   450,   451,   452,     0,
     454,   455,   456,   457,   458,     0,   460,     0,     0,   461,
     462,   463,   366,   480,   979,     0,  -661,     0,  -661,     0,
     464,   140,     0,   140,     0,   183,     0,     0,   183,     0,
       0,     0,     0,   176,     0,   176,     0,   184,     0,     0,
     176,     0,     0,   176,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   181,     0,
       0,     0,   181,     0,     0,     0,     0,   181,     0,     0,
     181,     0,   176,     0,     0,     0,     0,     0,   176,     0,
       0,     0,   176,   176,     0,     0,     0,     0,     0,     0,
       0,   230,     0,     0,   176,     0,     0,     0,   176,   140,
     176,     0,     0,     0,     0,     0,   231,   232,     0,   233,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
     183,   176,   235,     0,   183,     0,     0,     0,   236,   183,
       0,     0,   183,     0,   237,     0,     0,     0,   238,     0,
       0,   239,   181,   240,   181,     0,     0,   181,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,   242,
     243,     0,     0,   176,   176,   176,   176,   244,     0,     0,
       0,     0,   181,     0,     0,     0,     0,   245,     0,     0,
       0,     0,   181,     0,     0,     0,   246,   247,     0,   248,
       0,   249,     0,   250,     0,     0,   251,     0,     0,     0,
     252,     0,   253,     0,   183,   254,   183,     0,     0,   183,
       0,     0,     0,     0,     0,     0,   184,     0,     0,   184,
       0,     0,     0,     0,   181,     0,     0,     0,     0,     0,
     181,     7,     0,     0,   183,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   183,     0,     0,     0,     0,   361,
       0,     0,     0,    23,    24,     0,     0,     0,     0,     0,
       0,   271,   362,     0,    30,   363,     0,     0,     0,     0,
      33,     0,     0,     0,     0,     0,     0,    38,   140,   181,
       0,   140,     0,     0,     0,     0,   183,     0,     0,     0,
       0,     0,   183,     0,     0,     0,     0,     0,     0,   364,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
    1045,   184,     0,  1046,     0,   184,   365,     0,    66,     0,
     184,     0,   181,   184,   181,     0,     0,     0,     0,   181,
       0,     0,   181,     0,     0,     0,     0,    82,     0,    84,
       0,   183,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   437,     0,     0,     0,
       0,   181,     0,     0,     0,     0,     0,   181,     0,     0,
       0,   181,   181,   140,     0,     0,     0,   140,     0,     0,
       0,     0,   140,   181,   183,   140,   183,   181,     0,   181,
       0,   183,   100,     0,   183,   184,     0,   184,  1047,     0,
     184,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     181,     0,     0,     0,     0,   438,     0,     0,     0,   439,
       0,     0,     0,   183,     0,   184,     0,     0,     0,   183,
       0,     0,     0,   183,   183,   184,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   183,     0,     0,     0,   183,
       0,   183,   181,   181,   181,   181,     0,   140,     0,   140,
       0,     0,   140,     0,     0,     0,   441,   442,     0,   443,
     444,   445,   183,   446,   447,     0,     0,   184,   450,     0,
       0,     0,     0,   184,   456,   457,     0,   140,   460,     7,
       0,   461,   462,   463,     0,     0,     0,   140,     0,     0,
       0,     0,     0,     0,     0,     0,   226,   361,     0,     0,
       0,    23,    24,     0,   183,   183,   183,   183,     0,     0,
     362,     0,    30,   363,     0,     0,     0,     0,    33,     0,
       0,     0,   184,     0,     0,    38,     0,     0,     0,   140,
       0,     0,     0,     0,     0,   140,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   333,   364,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,  1045,   700,
       0,  1046,     0,     0,   365,   184,    66,   184,     0,     0,
       0,     0,   184,     0,   925,   184,   360,   926,     0,     0,
       0,     0,   927,     0,   140,    82,     0,    84,     0,     0,
      86,     0,   196,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   928,     0,   184,     0,     0,     0,     0,   929,
     184,   198,     0,     0,   184,   184,     0,     0,     0,     0,
     930,     0,     0,     0,     0,     0,   184,   140,   931,   140,
     184,     0,   184,     0,   140,     0,     0,   140,     0,     0,
     100,     0,     0,     0,     0,   932,  1117,     0,     0,     0,
       0,     0,     0,   184,     0,     0,     0,   933,     0,     0,
       0,   704,     0,     0,     0,     0,   140,     0,   934,     0,
       0,     0,   140,   935,     0,     0,   140,   140,     0,     0,
       0,     0,     0,     0,     0,   494,     0,     0,   140,     0,
       0,     0,   140,     0,   140,   184,   184,   184,   184,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   210,     0,     0,   140,   220,     0,     0,     0,
       0,     0,     0,     0,     0,   228,     0,     0,     0,     0,
       0,     0,   262,     0,     0,     0,     0,     0,     0,     0,
     520,     0,     0,     0,   278,   530,   281,   282,     0,     0,
       0,     0,   287,     0,   288,     0,   289,   140,   140,   140,
     140,   316,     0,     0,     0,   228,   324,   326,   328,   329,
     330,     0,     0,     0,   334,     0,   335,     0,     0,     0,
       0,     0,     0,   342,     0,     0,   324,     0,   344,     0,
     345,     0,     0,   346,   347,     0,   348,     0,     0,     0,
     228,   324,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   382,
       0,   385,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   601,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,     0,     5,     6,     0,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,   486,  -211,    22,     0,   334,
     228,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,  -211,  -211,  -211,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,   324,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,   324,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   201,     0,    77,     0,   764,    79,
     766,     0,     0,    83,     0,    85,     0,     0,    87,   324,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     330,   334,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,     0,    92,   552,   553,     0,     0,
       0,     0,     0,     0,   276,     0,     0,     0,     0,     0,
     324,     0,     0,     0,   562,     0,    99,     0,     0,     0,
       0,     0,   202,     0,   103,     0,  -211,     0,   276,     0,
       0,     0,   571,     0,     0,     0,     0,     0,   339,   341,
       0,     0,   577,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   589,     0,   591,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   371,
     604,   609,   612,     0,     0,     0,     0,   228,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   687,   688,   689,   691,   693,   694,     0,
     695,   696,   697,   698,   699,     0,   707,   708,   709,   710,
     711,   712,   713,   714,   715,   716,   717,   718,   719,   720,
     721,   722,   723,     0,     0,     0,     0,     0,     0,   324,
     324,     0,     0,     0,     0,   324,     0,     0,   324,   324,
     324,   324,   733,   735,     0,     0,     0,   609,   740,     0,
       0,   755,     0,     0,     0,     0,     0,   758,     0,   496,
       0,     0,     0,     0,     0,     0,     0,     0,   767,     0,
       0,   769,     0,     0,     0,   324,     0,     0,     0,     0,
       0,     0,     0,     0,   780,     0,   783,     0,     0,     0,
     281,     0,     0,   788,   789,   945,     0,     0,   948,   509,
     512,     0,     0,     0,     0,   316,     0,   519,   316,     0,
       0,     0,     0,     0,     0,     0,   276,   324,   276,     0,
       0,     0,     0,     0,     0,   276,     0,   758,     0,     0,
     324,   324,     0,     0,     0,     0,   228,   817,     0,     0,
     324,     0,     0,     0,     0,   230,     0,     0,     0,     0,
       0,   847,   847,     0,     0,     0,     0,     0,     0,     0,
     231,   232,     0,   233,   859,     0,   276,     0,   234,     0,
       0,     0,     0,     0,     0,     0,   235,     0,     0,     0,
       0,     0,   236,     0,     0,     0,     0,   316,   237,     0,
    1025,     0,   238,     0,  1030,   239,     0,   240,     0,   530,
       0,     0,   530,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,   242,   243,     0,     0,     0,     0,     0,
       0,   244,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   245,   896,     0,     0,     0,     0,     0,     0,   901,
     246,   247,     0,   248,     0,   249,     0,   250,     0,     0,
     251,     0,     0,   616,   252,   230,   253,     7,     0,   254,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     231,   232,     0,   233,  1077,     0,  1078,     0,   234,  1080,
       0,     0,     0,     0,     0,     0,   235,     0,     0,     0,
       0,     0,   236,     0,   767,   324,     0,     0,   237,     0,
       0,     0,   238,     0,  1096,   239,     0,   240,     0,     0,
       0,     0,     0,     0,  1030,   521,   241,   324,     0,   324,
     942,   943,     0,   242,   243,     0,     0,     0,     0,     0,
       0,   244,     0,     0,     0,     0,     0,     0,     0,   324,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
     246,   247,     0,   248,   316,   249,  1123,   250,   972,     0,
     251,     0,  1128,     0,   252,     0,   253,     0,     0,   254,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   589,     0,     0,     0,  1010,     0,  1011,
     847,   847,     0,   344,     0,   345,     0,     0,   347,   348,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1164,  1022,     0,     0,     0,   228,     0,     0,     0,
    1028,   758,  1031,     0,  1033,  1035,     0,  1036,     0,     0,
    1039,  1041,     0,   256,     0,   324,     0,     0,     0,     0,
    1051,   276,   276,     0,     0,   268,   269,   270,     0,   276,
     276,     0,     0,     0,  1183,     0,  1187,     0,     0,     0,
       0,   530,     0,     0,   530,     0,     0,     0,     0,     0,
       0,     0,  1068,  1031,  1036,  1039,  1071,   324,     0,     0,
       0,   755,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     0,  1222,     0,   691,   733,     0,     0,  1225,
       0,     0,     0,  1183,  1187,  1079,     0,     0,     0,   361,
       0,     0,     0,    23,    24,  1231,     0,     0,     0,  1233,
       0,   766,   362,   316,    30,   363,     0,  1095,     0,     0,
      33,     0,     0,   228,     0,     0,     0,    38,     0,     0,
       0,  1103,     0,  1105,   324,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   401,     0,     0,     0,     0,   364,
       0,     0,     0,     0,    55,   847,    57,     0,   427,     0,
    1045,     0,  1114,  1046,  1115,     0,   365,     0,    66,     0,
       0,  1121,     0,     0,   766,   766,   766,   766,     0,     0,
       0,     0,     0,     0,     0,     0,  1130,    82,     0,    84,
       0,     0,    86,   487,     0,     0,     0,   950,     0,     0,
       0,     0,     0,     0,     0,  1151,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1158,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   980,   324,   324,     0,     0,     0,
       0,     0,   100,     0,   276,   276,     0,     0,  1172,     0,
       0,     0,     0,   506,   507,     0,  1176,   847,   847,     0,
       0,     0,     0,     0,     0,   522,     0,     0,     0,     0,
       0,     0,  1184,     0,  1188,     0,     0,  1190,     0,  1193,
       0,     0,  1196,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   276,
       0,     0,     0,   276,  1184,  1188,   549,  1193,  1196,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1229,     0,     0,
    1230,     0,     0,     0,  1232,     0,     0,     0,     0,  1234,
    1235,     0,     0,  1236,     0,     0,     0,     0,     0,     0,
       0,  1238,     0,     0,     0,     0,   256,     0,     0,     0,
       0,     0,     0,     0,     0,  1248,  1249,  1234,  1250,  1251,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1085,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   613,     0,     0,     0,   618,     0,     0,     0,
       0,     0,  1262,  1263,  1264,  1265,  1262,  1263,  1264,  1265,
    1101,   230,     0,     0,     0,     0,     0,     0,     0,     0,
     675,     0,     0,     0,     0,     0,   231,   232,     0,   233,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,     0,     0,     0,     0,     0,   236,     0,
       0,     0,     0,     0,   237,     0,     0,     0,   238,     0,
       0,   239,     0,   240,     0,     0,     0,     0,   276,   276,
       0,     0,   241,     0,     0,     0,     0,     0,     0,   242,
     243,     0,     0,     0,     0,   741,     0,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,   246,   247,     0,   248,
       0,   249,   773,   250,     0,   522,   251,     0,     0,     0,
     252,     0,   253,     0,     0,   254,   276,     0,   786,     0,
       0,     0,   276,   276,     0,     0,     0,     7,   792,     0,
       0,     0,   522,     0,     0,     0,     0,     0,     0,   276,
       0,     0,     0,     0,     0,   361,     0,     0,   512,    23,
      24,   276,     0,     0,   276,     0,     0,     0,   362,     0,
      30,   363,     0,     0,     0,     0,    33,     0,     0,     0,
       0,   548,     0,    38,     0,     0,   834,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   855,     0,     0,  1226,
       0,     0,     0,     0,     0,   364,     0,     0,     0,     0,
      55,   618,    57,     0,    59,     0,  1045,     0,     0,  1046,
       0,     0,   365,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    82,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   512,     0,
     512,     0,     0,   512,   512,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   427,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,  1180,     0,     0,   742,     0,     5,
       6,     0,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   195,
       0,  -211,    22,     0,   743,     0,     0,     0,     0,     0,
      28,     0,   196,     0,     0,     0,    32,  -211,  -211,  -211,
       0,   197,    35,     0,   744,     0,    37,     0,   745,    39,
       0,   198,     0,   199,   951,     0,    44,    45,     0,     0,
      47,   522,    49,     0,    50,    51,     0,    52,    53,     0,
       0,   960,     0,   746,     0,    56,   747,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   200,     0,   748,     0,
     981,     0,     0,     0,    71,    72,    73,    74,    75,   201,
       0,    77,     0,     0,    79,     0,     0,   749,    83,     0,
      85,     0,   750,    87,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   427,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
     751,  -211,     0,     0,   104,     0,     0,     0,     0,     0,
       0,     0,   675,     0,     0,     0,     0,     0,     0,    -2,
       4,     0,     5,     6,     7,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,    15,    16,    17,    18,     0,
      19,     0,    20,    21,  -211,    22,    23,    24,    25,    26,
      27,     0,  -211,    28,     0,    29,     0,    30,    31,    32,
    -211,  -211,  -211,    33,    34,    35,   -68,   773,    36,    37,
      38,     0,    39,  -336,    40,     0,    41,    42,    43,    44,
      45,     0,    46,    47,    48,    49,   -68,    50,    51,     0,
      52,    53,    54,   981,  -336,     0,     0,    55,    56,    57,
      58,    59,    60,    61,  -336,   -68,    62,    63,    64,    65,
       0,    66,    67,    68,     0,    69,    70,    71,    72,    73,
      74,    75,    76,     0,    77,    78,     0,    79,    80,    81,
      82,    83,    84,    85,   -68,    86,    87,     0,     0,  -211,
       0,    88,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,   618,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   100,     0,     0,   101,     0,
     102,     0,   103,     0,  -211,     0,     0,   104,     0,     0,
       0,     4,     0,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     0,  -692,     0,    14,    15,    16,    17,    18,
    -692,    19,     0,    20,    21,  -211,    22,    23,    24,    25,
      26,    27,  -692,  -211,    28,  -692,    29,     0,    30,    31,
      32,  -211,  -211,  -211,    33,    34,    35,   -68,     0,    36,
      37,    38,     0,    39,  -336,    40,   618,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,   -68,    50,    51,
       0,    52,    53,    54,     0,  -336,     0,     0,    55,    56,
      57,    58,     0,    60,    61,  -336,   -68,    62,    63,    64,
      65,  -692,    66,    67,    68,  -692,    69,    70,    71,    72,
      73,    74,    75,    76,     0,    77,    78,     0,    79,    80,
      81,    82,    83,    84,    85,   -68,    86,    87,     0,     0,
    -211,     0,    88,     0,     0,  -692,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,  -692,    92,  -692,  -692,  -692,  -692,  -692,
    -692,  -692,     0,  -692,  -692,  -692,  -692,  -692,  -692,  -692,
    -692,  -692,  -692,  -692,  -692,    99,   100,  -692,  -692,  -692,
       0,   102,  -692,   103,     0,  -211,     0,   349,  -692,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,    15,    16,    17,    18,     0,    19,     0,    20,
      21,  -211,    22,    23,    24,    25,    26,    27,     0,  -211,
      28,     0,    29,     0,    30,    31,    32,  -211,  -211,  -211,
      33,    34,    35,   -68,     0,    36,    37,    38,     0,    39,
    -336,    40,     0,    41,    42,    43,    44,    45,     0,    46,
      47,    48,    49,   -68,    50,    51,     0,    52,    53,    54,
       0,  -336,     0,     0,    55,    56,    57,    58,    59,    60,
      61,  -336,   -68,    62,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,    77,    78,     0,    79,    80,    81,    82,    83,    84,
      85,   -68,    86,    87,     0,     0,  -211,     0,    88,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,   350,   103,
       0,  -211,     0,     4,   104,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,    16,
      17,    18,     0,    19,     0,    20,    21,  -211,    22,    23,
      24,    25,    26,    27,     0,  -211,    28,     0,    29,     0,
      30,    31,    32,  -211,  -211,  -211,    33,    34,    35,   -68,
       0,    36,    37,    38,     0,    39,  -336,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -336,     0,     0,
      55,    56,    57,    58,    59,    60,    61,  -336,   -68,    62,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,    76,     0,    77,    78,     0,
      79,    80,    81,    82,    83,    84,    85,   -68,    86,    87,
       0,     0,  -211,     0,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,   567,   103,     0,  -211,     0,   584,
     104,     5,     6,     7,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,    15,    16,    17,    18,     0,    19,
       0,    20,    21,  -211,    22,    23,    24,    25,    26,    27,
       0,  -211,    28,     0,    29,     0,    30,    31,    32,  -211,
    -211,  -211,    33,    34,    35,   -68,     0,    36,    37,    38,
       0,    39,  -336,    40,     0,    41,    42,    43,    44,    45,
       0,    46,    47,    48,    49,   -68,    50,    51,     0,    52,
      53,    54,     0,  -336,     0,     0,    55,    56,    57,    58,
      59,    60,    61,  -336,   -68,    62,    63,    64,    65,     0,
      66,    67,    68,     0,    69,    70,    71,    72,    73,    74,
      75,    76,     0,    77,    78,     0,    79,    80,    81,    82,
      83,    84,    85,   -68,    86,    87,     0,     0,  -211,     0,
      88,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
     585,   103,     0,  -211,     0,     4,   104,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
      15,    16,    17,    18,     0,    19,     0,    20,    21,  -211,
      22,    23,    24,    25,    26,    27,     0,  -211,    28,     0,
      29,     0,    30,    31,    32,  -211,  -211,  -211,    33,    34,
      35,   -68,     0,    36,    37,    38,     0,    39,  -336,    40,
       0,    41,    42,    43,    44,    45,     0,    46,    47,    48,
      49,   -68,    50,    51,     0,    52,    53,    54,     0,  -336,
       0,     0,    55,    56,    57,    58,    59,    60,    61,  -336,
     -68,    62,    63,    64,    65,     0,    66,    67,    68,     0,
      69,    70,    71,    72,    73,    74,    75,    76,     0,    77,
      78,     0,    79,    80,    81,    82,    83,    84,    85,   -68,
      86,    87,     0,     0,  -211,     0,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,   824,   103,     0,  -211,
       0,     4,   104,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,    15,    16,    17,    18,
       0,    19,     0,    20,    21,  -211,    22,    23,    24,    25,
      26,    27,     0,  -211,    28,     0,    29,     0,    30,    31,
      32,  -211,  -211,  -211,    33,    34,    35,   -68,     0,    36,
      37,    38,     0,    39,  -336,    40,     0,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,   -68,    50,    51,
       0,    52,    53,    54,     0,  -336,     0,     0,    55,    56,
      57,    58,   359,    60,    61,  -336,   -68,    62,    63,    64,
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
    -336,    40,     0,    41,    42,    43,    44,    45,     0,    46,
      47,    48,    49,   -68,    50,    51,     0,    52,    53,    54,
       0,  -336,     0,     0,    55,    56,    57,    58,    59,    60,
      61,  -336,   -68,    62,    63,    64,    65,     0,    66,    67,
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
      30,    31,    32,  -211,  -211,  -211,    33,    34,    35,   -68,
       0,    36,    37,    38,     0,    39,  -336,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -336,     0,     0,
      55,    56,    57,    58,     0,    60,    61,  -336,   -68,    62,
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
    -211,  -211,    33,  1038,    35,   -68,     0,    36,    37,    38,
       0,    39,  -336,    40,     0,    41,    42,    43,    44,    45,
       0,    46,    47,    48,    49,   -68,    50,    51,     0,    52,
      53,    54,     0,  -336,     0,     0,    55,    56,    57,    58,
       0,    60,    61,  -336,   -68,    62,    63,    64,    65,     0,
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
      29,     0,    30,    31,    32,  -211,  -211,  -211,    33,  1182,
      35,   -68,     0,    36,    37,    38,     0,    39,  -336,    40,
       0,    41,    42,    43,    44,    45,     0,    46,    47,    48,
      49,   -68,    50,    51,     0,    52,    53,    54,     0,  -336,
       0,     0,    55,    56,    57,    58,     0,    60,    61,  -336,
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
      32,  -211,  -211,  -211,    33,  1186,    35,   -68,     0,    36,
      37,    38,     0,    39,  -336,    40,     0,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,   -68,    50,    51,
       0,    52,    53,    54,     0,  -336,     0,     0,    55,    56,
      57,    58,     0,    60,    61,  -336,   -68,    62,    63,    64,
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
      33,  1192,    35,   -68,     0,    36,    37,    38,     0,    39,
    -336,    40,     0,    41,    42,    43,    44,    45,     0,    46,
      47,    48,    49,   -68,    50,    51,     0,    52,    53,    54,
       0,  -336,     0,     0,    55,    56,    57,    58,     0,    60,
      61,  -336,   -68,    62,    63,    64,    65,     0,    66,    67,
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
      30,    31,    32,  -211,  -211,  -211,    33,  1195,    35,   -68,
       0,    36,    37,    38,     0,    39,  -336,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -336,     0,     0,
      55,    56,    57,    58,     0,    60,    61,  -336,   -68,    62,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,    76,     0,    77,    78,     0,
      79,    80,    81,    82,    83,    84,    85,   -68,    86,    87,
       0,     0,  -211,     0,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,     0,   103,   888,  -211,     5,     6,
     104,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   195,     0,
    -211,    22,     0,     0,     0,     0,     0,     0,     0,   295,
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
       0,   636,     0,    14,     0,     0,    17,    18,   638,    19,
       0,   195,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   196,     0,     0,     0,    32,     0,
       0,     0,     0,   197,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   198,   644,   199,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   200,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   201,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    89,   511,   466,   467,   468,   469,   470,
       0,     0,   473,   474,   475,   476,     0,   478,   479,     0,
     835,   836,   837,   838,   839,   657,     0,   658,     0,    95,
       0,   659,   660,   661,   662,   663,   664,   665,   840,   667,
     668,    97,   841,    99,   -90,   670,   671,   842,   673,   202,
       0,   103,     0,     0,     5,     6,   104,     8,     9,    10,
      11,    12,    13,     0,   636,     0,    14,     0,     0,    17,
      18,   638,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,   644,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   201,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    89,   511,   466,   467,
     468,   469,   470,     0,     0,   473,   474,   475,   476,     0,
     478,   479,     0,   835,   836,   837,   838,   839,   657,     0,
     658,     0,    95,     0,   659,   660,   661,   662,   663,   664,
     665,   840,   667,   668,    97,   841,    99,     0,   670,   671,
     842,   673,   202,     0,   103,     0,     0,     0,     0,   104,
       5,     6,     7,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,    15,     0,    17,    18,     0,    19,     0,
     195,    21,     0,    22,     0,     0,     0,     0,    27,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,    34,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
      46,    47,    48,    49,     0,    50,    51,     0,    52,    53,
     336,     0,     0,     0,     0,     0,    56,     0,    58,     0,
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
       0,    19,     0,   195,     0,     0,    22,     0,   743,     0,
       0,     0,     0,     0,    28,     0,   196,     0,     0,     0,
      32,     0,     0,     0,     0,   197,    35,     0,   744,     0,
      37,     0,   745,    39,     0,   198,     0,   199,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,   746,     0,    56,
     747,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     200,     0,   748,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   201,     0,    77,     0,     0,    79,     0,
       0,   749,    83,     0,    85,     0,   750,    87,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,     0,     5,     6,   104,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   195,     0,     0,    22,
     380,    24,     0,     0,     0,     0,     0,    28,     0,   196,
       0,     0,     0,    32,     0,     0,     0,     0,   197,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   198,     0,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,   364,     0,     0,     0,
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
       6,   104,     8,     9,    10,   383,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,    20,
       0,  -211,    22,     0,     0,     0,     0,     0,     0,     0,
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
      19,     0,   195,     0,  -211,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
    -211,  -211,  -211,     0,   197,    35,     0,     0,     0,    37,
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
     202,     0,   103,     0,  -211,     5,     6,   104,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   195,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   196,     0,
       0,     0,    32,     0,     0,     0,     0,   197,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   198,     0,   199,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   200,  -736,     0,     0,     0,  -736,     0,     0,
      71,    72,    73,    74,    75,   201,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
    -378,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,  -378,     0,
       0,   101,     0,   202,  -378,   103,     0,     0,     5,     6,
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
      77,     0,     0,    79,   318,   319,     0,    83,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,   320,     0,     0,   101,     0,   202,     0,   103,     0,
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
      75,   201,     0,    77,     0,     0,    79,   318,   319,     0,
      83,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
     354,     0,     0,     0,     0,     0,     0,     0,    96,     0,
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
     318,   319,     0,    83,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   202,     0,   103,  -538,     0,     5,     6,   104,
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
       0,     0,    79,   318,   319,     0,    83,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,     0,     0,
    -538,     0,   104,     5,     6,     0,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   195,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   196,     0,     0,     0,
      32,     0,     0,     0,     0,   197,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   198,     0,   199,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     200,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   201,     0,    77,     0,     0,    79,   318,
     319,     0,    83,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,   810,     0,     5,     6,   104,     8,
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
       0,    79,   318,   319,     0,    83,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,   813,     0,     5,
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
       0,    77,     0,     0,    79,   318,   319,     0,    83,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
     819,     0,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,   958,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   201,     0,    77,     0,     0,    79,   318,   319,
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
    -386,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,  -386,     0,
       0,   101,     0,   202,  -386,   103,     0,     0,     5,     6,
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
      93,    94,     0,  -371,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,  -371,     0,     0,   101,     0,   202,  -371,   103,     0,
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
      90,    91,    92,    93,    94,     0,  -379,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,  -379,     0,     0,   101,     0,   202,
    -379,   103,     0,     0,     5,     6,   104,     8,     9,    10,
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
       0,     0,    79,   318,   319,     0,    83,     0,    85,     0,
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
      60,     0,     0,     0,     0,    63,    64,   200,  -736,     0,
       0,     0,  -736,     0,     0,    71,    72,    73,    74,    75,
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
       0,     0,     0,   605,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,     0,   606,     0,   104,     5,
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
      92,    93,    94,     0,  -407,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
    -407,     0,     5,     6,   104,     8,     9,    10,    11,    12,
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
       0,     0,     0,     0,     0,   605,     0,    89,     0,     0,
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
       0,   606,     0,   104,     5,     6,     0,     8,     9,    10,
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
     101,     0,   202,   808,   103,     0,     0,     5,     6,   104,
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
    1029,     0,   104,     5,     6,     0,     8,     9,    10,    11,
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
       0,   202,     0,   103,     0,     0,  1034,     0,   104,     5,
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
       0,     0,    32,     0,     0,     0,     0,  1069,    35,     0,
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
    1217,    35,     0,     0,     0,    37,     0,     0,    39,     0,
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
       0,     0,     0,  1218,    35,     0,     0,     0,    37,     0,
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
       0,    32,     0,     0,     0,     0,  1220,    35,     0,     0,
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
     196,     0,     0,     0,    32,     0,     0,     0,     0,  1221,
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
     195,     0,  -211,     0,     0,   294,     0,     0,     0,     0,
       0,     0,     0,   196,     0,     0,     0,     0,  -211,  -211,
    -211,     0,   197,     0,     0,   296,     0,     0,     0,   298,
       0,     0,     0,     0,   199,     0,     0,    44,    45,     0,
       0,     0,     0,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,     0,   299,     0,    56,   300,    58,    59,
      60,     0,     0,     0,     0,    63,     0,   200,     0,   301,
       0,     0,     0,     0,     0,    71,     0,    73,     0,    75,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,    85,     0,   305,     0,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,  -340,  -211,     0,     6,   104,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,     0,    18,
       0,     0,     0,   195,     0,  -211,     0,     0,   294,     0,
       0,     0,     0,     0,     0,     0,   196,     0,     0,     0,
       0,  -211,  -211,  -211,     0,   197,     0,     0,   296,     0,
       0,     0,   298,     0,     0,     0,     0,   199,     0,     0,
      44,    45,     0,     0,     0,     0,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,     0,   299,     0,    56,
     300,    58,    59,    60,     0,     0,     0,     0,    63,     0,
     200,     0,   301,     0,     0,     0,     0,     0,    71,     0,
      73,     0,    75,     0,     0,     0,     0,     0,     0,     0,
       0,   304,     0,     0,    85,     0,   305,     0,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   634,     0,   635,
       0,     0,    90,    91,    92,    93,    94,     0,     0,   636,
       0,    95,     0,     0,   637,   232,   638,   639,     0,     0,
      96,     0,   640,    97,    98,    99,     0,     0,     0,   101,
     235,   202,   196,   103,     0,  -211,   236,     0,   104,     0,
       0,     0,   641,     0,     0,     0,   238,     0,     0,   642,
       0,   643,   644,     0,     0,     0,     0,     0,     0,     0,
     645,     0,     0,     0,     0,     0,     0,   242,   646,     0,
       0,     0,     0,     0,     0,   244,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   647,     0,     0,     0,     0,
       0,     0,     0,     0,   246,   247,     0,   648,     0,   249,
       0,   649,     0,     0,   251,     0,     0,     0,   650,     0,
     253,     0,     0,   651,     0,     0,     0,     0,     0,     0,
       0,     0,   511,   466,   467,   468,   469,   470,     0,     0,
     473,   474,   475,   476,     0,   478,   479,     0,   652,   653,
     654,   655,   656,   657,     0,   658,     0,     0,     0,   659,
     660,   661,   662,   663,   664,   665,   666,   667,   668,     0,
     669,     0,     0,   670,   671,   672,   673,     5,     6,   674,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   195,     0,  -211,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     196,     0,     0,     0,    32,  -211,  -211,  -211,     0,   197,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   198,
       0,   199,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   200,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   201,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,     0,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,     0,  -211,
       5,     6,     0,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,  -211,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,  -211,  -211,
    -211,     0,   197,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     201,     0,    77,     0,     0,    79,   700,     0,     0,    83,
       0,    85,     0,     0,    87,     0,   636,     0,     0,     0,
       0,   990,   991,   638,   992,     0,     0,     0,     0,   993,
       0,     0,     0,     0,     0,     0,     0,   702,     0,    90,
       0,    92,     0,     0,     0,     0,     0,     0,     0,   994,
       0,     0,     0,   995,     0,     0,   996,     0,   198,   644,
       0,     0,    99,     0,     0,     0,     0,   703,     0,     0,
     103,     0,  -211,     0,     0,   997,     0,     0,     0,     0,
       0,     0,   998,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   999,     0,     0,     0,     0,     0,     0,     0,
       0,  1000,  1001,     0,  1002,     0,  1003,     0,   704,     0,
       0,     0,     0,     0,   705,  1004,     0,  1005,     0,     0,
    1006,     0,     0,     0,     0,     0,     0,     0,     0,   511,
     466,   467,   468,   469,   470,     0,     0,   473,   474,   475,
     476,     0,   478,   479,     0,   652,   653,   654,   655,   656,
     657,     0,   658,     0,     0,     0,   659,   660,   661,   662,
     663,   664,   665,   666,   667,   668,   230,   669,     0,     0,
     670,   671,   672,   673,  1007,     0,   636,     0,     0,     0,
       0,   231,   232,   638,   233,     0,     0,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,   235,     0,     0,
       0,     0,     0,   236,     0,     0,     0,     0,     0,   237,
       0,     0,     0,   238,     0,     0,   239,     0,   240,   644,
       0,     0,     0,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,   242,   243,     0,     0,     0,     0,
       0,     0,   244,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   245,     0,     0,     0,     0,     0,     0,     0,
     435,   246,   247,     0,   248,     0,   249,   436,   250,     0,
       0,   251,     0,     0,     0,   252,     0,   253,     0,   437,
     254,     0,     0,     0,     0,     0,     0,     0,     0,   511,
     466,   467,   468,   469,   470,     0,     0,   473,   474,   475,
     476,     0,   478,   479,     0,   652,   653,   654,   655,   656,
     657,     0,   658,     0,     0,     0,   659,   660,   661,   662,
     663,   664,   665,   666,   667,   668,     0,   669,     0,     0,
     670,   671,   672,   673,     0,     0,     0,     0,   438,   435,
       0,     0,   439,     0,     0,     0,   436,     0,     0,     0,
       0,   510,     0,     0,     0,     0,     0,     0,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   440,   511,   466,   467,   468,   469,   470,     0,
       0,   473,   474,   475,   476,     0,   478,   479,     0,   441,
     442,     0,   443,   444,   445,     0,   446,   447,   448,     0,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,     0,     0,   461,   462,   463,   438,   366,     0,
     435,   439,     0,     0,     0,   464,     0,   436,     0,     0,
    1191,     0,     0,     0,     0,     0,     0,     0,     0,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   440,   511,   466,   467,   468,   469,   470,     0,     0,
     473,   474,   475,   476,     0,   478,   479,     0,   441,   442,
       0,   443,   444,   445,     0,   446,   447,   448,     0,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,     0,     0,   461,   462,   463,     0,   366,   438,   435,
       0,     0,   439,     0,   464,     0,   436,     0,     0,     0,
       0,  1254,     0,     0,     0,     0,     0,     0,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   440,   511,   466,   467,   468,   469,   470,     0,
       0,   473,   474,   475,   476,     0,   478,   479,     0,   441,
     442,     0,   443,   444,   445,     0,   446,   447,   448,     0,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,     0,     0,   461,   462,   463,   438,   366,     0,
     435,   439,     0,     0,     0,   464,     0,   436,     0,     0,
    1255,     0,     0,     0,     0,     0,     0,     0,     0,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   440,   511,   466,   467,   468,   469,   470,     0,     0,
     473,   474,   475,   476,     0,   478,   479,     0,   441,   442,
       0,   443,   444,   445,     0,   446,   447,   448,     0,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,     0,     0,   461,   462,   463,     0,   366,   438,   435,
       0,     0,   439,     0,   464,     0,   436,     0,     0,     0,
       0,  1256,     0,     0,     0,     0,     0,     0,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   440,   511,   466,   467,   468,   469,   470,     0,
       0,   473,   474,   475,   476,     0,   478,   479,     0,   441,
     442,     0,   443,   444,   445,     0,   446,   447,   448,     0,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,     0,     0,   461,   462,   463,   438,   366,     0,
       0,   439,     0,     0,   435,   464,     0,     0,     0,     0,
    1257,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,   872,     0,     0,     0,     0,     0,
       0,   440,   511,   466,   467,   468,   469,   470,     0,   873,
     473,   474,   475,   476,     0,   478,   479,     0,   441,   442,
       0,   443,   444,   445,     0,   446,   447,   448,     0,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,     0,     0,   461,   462,   463,     0,   366,     0,     0,
       0,     0,   438,   435,   464,     0,   439,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,   275,     0,     0,     0,     0,     0,     0,
       0,     0,   214,     0,     0,     0,   440,     0,   985,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   442,     0,   443,   444,   445,     0,
     446,   447,   448,     0,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,     0,     0,   461,   462,
     463,   438,   366,     0,     0,   439,     0,     0,     0,   464,
       0,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   214,     0,   437,     0,   440,     0,     0,   593,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   441,   442,     0,   443,   444,   445,     0,   446,
     447,   448,     0,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   460,   594,     0,   461,   462,   463,
       0,   366,     0,     0,     0,     0,     0,     0,   464,     0,
       0,     0,   438,     0,     0,     0,   439,     0,     0,     0,
       0,     0,     0,     0,     0,   435,   831,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,     0,   440,     0,     0,   832,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   442,     0,   443,   444,   445,     0,
     446,   447,   448,     0,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   833,   595,   461,   462,
     463,     0,     0,     0,     0,     0,     0,     0,     0,   464,
       0,     0,     0,   438,   435,     0,     0,   439,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,  1134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   440,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,     0,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,     0,     0,   461,
     462,   463,   438,     0,     0,   435,   439,     0,     0,     0,
     464,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,   275,     0,     0,     0,     0,
       0,     0,   214,     0,     0,     0,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   442,     0,   443,   444,   445,     0,
     446,   447,   448,     0,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,     0,     0,   461,   462,
     463,     0,   366,   438,   435,     0,     0,   439,     0,   464,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,   275,     0,     0,     0,     0,     0,
       0,     0,     0,   214,     0,     0,     0,   440,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,     0,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,     0,     0,   461,
     462,   463,   438,   366,     0,   435,   439,     0,     0,     0,
     464,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,   917,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   440,     0,     0,     0,
     918,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   442,     0,   443,   444,   445,     0,
     446,   447,   448,     0,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,     0,     0,   461,   462,
     463,     0,   366,   438,   435,     0,     0,   439,     0,   464,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   440,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,     0,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,     0,     0,   461,
     462,   463,   438,     0,     0,     0,   439,     0,     0,     0,
     464,     0,     0,     0,     0,   949,   435,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,   440,     0,     0,     0,
    1109,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   441,   442,     0,   443,   444,   445,     0,
     446,   447,   448,     0,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,     0,  1110,   461,   462,
     463,     0,   366,     0,     0,     0,     0,     0,     0,   464,
       0,     0,     0,     0,   438,   435,     0,     0,   439,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   441,   442,     0,   443,   444,
     445,     0,   446,   447,   448,     0,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,     0,     0,
     461,   462,   463,   438,     0,   435,     0,   439,     0,     0,
       0,   464,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,     0,     0,     0,     0,     0,
       0,     0,     0,   214,     0,     0,     0,   440,     0,     0,
       0,     0,     0,     0,     0,   435,   534,     0,     0,     0,
       0,     0,   436,     0,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,   437,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,     0,     0,   461,
     462,   463,     0,   438,     0,   435,     0,   439,  1126,     0,
     464,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,     0,     0,     0,     0,     0,
       0,     0,     0,   214,     0,     0,     0,   440,     0,     0,
       0,     0,     0,   438,     0,   435,     0,   439,     0,     0,
       0,     0,   436,     0,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,   437,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   440,     0,   461,
     462,   463,     0,   438,     0,     0,     0,   439,  1174,     0,
     464,     0,     0,     0,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,     0,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   440,     0,   461,
     462,   463,     0,   438,     0,   435,     0,   439,     0,     0,
     464,     0,   436,     0,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,   437,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   440,     0,   461,
     462,   463,     0,   557,     0,   435,     0,     0,     0,     0,
     464,     0,   436,     0,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,   437,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,     0,   561,   461,
     462,   463,     0,   438,     0,   435,   827,   439,     0,     0,
     464,     0,   436,     0,     0,     0,   736,     0,     0,     0,
       0,     0,     0,     0,   437,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   440,     0,     0,
       0,     0,     0,   438,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,     0,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   440,     0,   461,
     462,   463,     0,   438,     0,   435,     0,   439,     0,     0,
     464,     0,   436,     0,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,   437,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   440,     0,   461,
     462,   463,     0,     0,     0,     0,   809,     0,     0,     0,
     464,     0,     0,     0,   441,   442,     0,   443,   444,   445,
       0,   446,   828,   448,     0,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,     0,     0,   461,
     462,   463,     0,   438,   435,     0,     0,   439,     0,     0,
     464,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,     0,     0,   947,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   866,   440,     0,     0,
       0,     0,     0,     0,     0,   435,     0,     0,     0,     0,
       0,     0,   436,     0,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,   437,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,     0,     0,   461,
     462,   463,   438,     0,     0,   435,   439,     0,     0,     0,
     464,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   440,     0,     0,     0,
       0,     0,     0,   438,     0,     0,     0,   439,     0,     0,
       0,     0,     0,   441,   442,     0,   443,   444,   445,     0,
     446,   447,   448,     0,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,     0,   440,   461,   462,
     463,     0,     0,   438,     0,   435,  1016,   439,     0,   464,
       0,     0,   436,     0,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,   437,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   440,     0,   461,
     462,   463,     0,     0,     0,   435,   961,     0,     0,     0,
     464,     0,   436,     0,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,   437,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,     0,     0,   461,
     462,   463,     0,   438,     0,     0,   962,   439,     0,     0,
     464,   435,  1108,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,     0,     0,     0,     0,     0,     0,   440,     0,     0,
       0,     0,     0,   438,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,     0,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   440,     0,   461,
     462,   463,     0,     0,     0,     0,     0,     0,     0,   438,
     464,     0,     0,   439,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,     0,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   435,     0,   461,
     462,   463,     0,   440,   436,     0,  1061,     0,     0,     0,
     464,     0,     0,     0,     0,     0,   437,     0,     0,     0,
     441,   442,     0,   443,   444,   445,     0,   446,   447,   448,
       0,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   435,     0,   461,   462,   463,     0,     0,
     436,     0,     0,     0,     0,     0,   464,     0,     0,     0,
       0,     0,   437,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   438,     0,     0,     0,   439,
       0,     0,     0,   435,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,     0,     0,     0,     0,     0,     0,   440,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,     0,     0,     0,   439,   441,   442,     0,   443,
     444,   445,     0,   446,   447,   448,     0,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,     0,
    1120,   461,   462,   463,  1129,   440,     0,     0,     0,     0,
       0,   438,   464,     0,     0,   439,     0,     0,     0,     0,
       0,     0,   441,   442,     0,   443,   444,   445,     0,   446,
     447,   448,     0,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   460,   440,     0,   461,   462,   463,
       0,     0,     0,     0,     0,     0,     0,     0,   464,     0,
       0,     0,   441,   442,     0,   443,   444,   445,     0,   446,
     447,   448,     0,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   460,   435,     0,   461,   462,   463,
       0,     0,   436,     0,     0,     0,  1157,     0,   464,     0,
       0,     0,     0,     0,   437,  1160,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   435,  1179,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   438,     0,     0,     0,   439,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,     0,     0,     0,     0,     0,     0,   440,     0,     0,
       0,     0,     0,   438,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,   441,   442,     0,   443,   444,   445,
    1211,   446,   447,   448,     0,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   440,     0,   461,
     462,   463,     0,     0,     0,     0,     0,     0,     0,   438,
     464,     0,     0,   439,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,     0,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   435,     0,   461,
     462,   463,     0,   440,   436,     0,     0,     0,     0,     0,
     464,     0,     0,     0,     0,     0,   437,     0,     0,     0,
     441,   442,     0,   443,   444,   445,     0,   446,   447,   448,
       0,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,     0,     0,   461,   462,   463,     0,     0,
       0,     0,     0,     0,     0,   437,   464,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   438,   435,     0,     0,   439,
       0,     0,     0,   436,     0,     0,     0,     0,  1219,     0,
       0,     0,     0,     0,     0,   437,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   440,
       0,     0,     0,     0,   438,     0,     0,     0,   439,     0,
       0,     0,     0,     0,     0,     0,   441,   442,     0,   443,
     444,   445,     0,   446,   447,   448,     0,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,     0,
       0,   461,   462,   463,   438,     0,   435,     0,   439,     0,
       0,     0,   464,   436,     0,   441,   442,  1258,   443,   444,
     445,     0,   446,   447,   448,   437,   449,   450,   451,   452,
       0,   454,   455,   456,   457,   458,     0,   460,   440,     0,
     461,   462,   463,     0,     0,     0,   435,     0,     0,     0,
       0,   464,     0,   436,     0,   441,   442,     0,   443,   444,
     445,     0,   446,   447,   448,   437,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,     0,     0,
     461,   462,   463,     0,   438,     0,   435,     0,   439,     0,
       0,   464,     0,   436,     0,     0,     0,  1259,     0,     0,
       0,     0,     0,     0,     0,   437,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   440,     0,
       0,     0,     0,     0,   438,     0,   435,     0,   439,     0,
       0,     0,     0,   436,     0,   441,   442,  1260,   443,   444,
     445,     0,   446,   447,   448,   437,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   440,     0,
     461,   462,   463,     0,   438,     0,   435,     0,   439,     0,
       0,   464,     0,   436,     0,   441,   442,  1261,   443,   444,
     445,     0,   446,   447,   448,   437,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   440,     0,
     461,   462,   463,     0,   438,     0,     0,     0,   439,     0,
       0,   464,     0,     0,     0,   441,   442,     0,   443,   444,
     445,     0,   446,   447,   448,     0,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   440,     0,
     461,   462,   463,     0,   937,     0,     0,     0,   439,     0,
       0,   464,     0,     0,     0,   441,   442,     0,   443,   444,
     445,     0,   446,   447,   448,     0,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   440,     0,
     461,   462,   463,     0,     0,     0,     0,     0,     0,     0,
       0,   464,     0,     0,     0,   441,   442,     0,   443,   444,
     445,     0,   446,   447,   448,   437,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,     0,   361,
     461,   462,   463,    23,    24,     0,     0,     0,     0,     0,
       0,   464,   362,     0,    30,   363,     0,     0,     0,     0,
      33,     0,     0,     0,     0,     0,     0,    38,     0,     0,
    -336,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   -68,   438,     0,     0,     0,   439,   364,
       0,  -336,     0,     0,    55,     0,    57,     0,     0,     0,
      61,  -336,   -68,    62,     0,     0,   365,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,    84,
       0,     0,    86,     0,     0,   441,   442,     0,   443,   444,
     445,     0,   446,   447,   448,     0,     0,   450,   451,   452,
       0,   454,   455,   456,   457,     0,     0,   460,   230,     0,
     461,   462,   463,     0,     0,     0,     0,     0,     0,     0,
       0,   464,     0,   231,   232,     0,   233,     0,     0,     0,
       0,   234,   100,    24,     0,     0,     0,   366,     0,   235,
       0,     0,     0,     0,     0,   236,     0,     0,     0,     0,
       0,   237,     0,     0,     0,   238,     0,     0,   239,     0,
     240,     0,     0,     0,   230,     0,     7,     0,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,     0,   231,
     232,     0,   233,     0,   244,    57,     0,   234,     0,     0,
       0,     0,     0,     0,   245,   235,     0,    66,     0,     0,
       0,   236,     0,   246,   247,     0,   248,   237,   249,     0,
     250,   238,     0,   251,   239,     0,   240,   252,     0,   253,
    -311,    86,   254,     0,     0,   241,     0,     0,     0,     0,
       0,     0,   242,   243,     0,  -311,  -311,     0,  -311,     0,
     244,     0,     0,  -311,     0,     0,     0,     0,     0,     0,
     245,  -311,     0,     0,     0,     0,     0,  -311,     0,   246,
     247,     0,   248,  -311,   249,     0,   250,  -311,     0,   251,
    -311,     0,  -311,   252,     0,   253,   230,     0,   254,     0,
       0,  -311,     0,     0,     0,     0,     0,     0,  -311,  -311,
       0,   231,   232,     0,   233,     0,  -311,     0,     0,   234,
       0,     0,     0,     0,     0,     0,  -311,   235,     0,     0,
       0,     0,     0,   236,     0,  -311,  -311,     0,  -311,   237,
    -311,     0,  -311,   238,     0,  -311,   239,     0,   240,  -311,
       0,  -311,  -312,     0,  -311,     0,     0,   241,     0,     0,
       0,     0,     0,     0,   242,   243,     0,  -312,  -312,     0,
    -312,     0,   244,     0,     0,  -312,     0,     0,     0,     0,
       0,     0,   245,  -312,     0,     0,     0,     0,     0,  -312,
       0,   246,   247,     0,   248,  -312,   249,     0,   250,  -312,
       0,   251,  -312,     0,  -312,   252,     0,   253,     0,     0,
     254,     0,     0,  -312,     0,     0,     0,     0,     0,     0,
    -312,  -312,     0,     0,     0,     0,     0,     0,  -312,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -312,     0,
       0,     0,     0,     0,     0,     0,     0,  -312,  -312,     0,
    -312,     0,  -312,     0,  -312,     0,     0,  -312,     0,     0,
       0,  -312,     0,  -312,     0,     0,  -312
};

static const yytype_int16 yycheck[] =
{
       2,     2,   181,   266,   402,    27,    63,   165,   112,   617,
     229,    63,    63,   593,   582,   426,   555,   493,   223,     2,
     629,   677,   833,   679,    26,   625,   798,   133,   291,   517,
     879,   828,   550,    68,   384,   607,   608,   792,     1,   611,
     612,     1,   851,    26,     1,    67,    25,   266,     1,   404,
     964,     1,    33,    88,    33,   526,   121,    48,     5,    33,
     104,   203,    41,    42,    43,    62,    33,    33,   103,   332,
      33,    36,   291,    33,    76,   148,   166,     1,    80,    81,
     148,     3,   887,   262,    48,     3,   394,     3,   119,    25,
     148,   124,    76,    76,    46,   148,    23,    80,    81,    62,
     102,   174,    62,    33,   106,    41,    42,    43,   176,   121,
      33,   176,     0,   332,   121,    57,   174,   148,    70,   102,
      33,   174,   119,   106,    48,    57,    76,   171,    92,   121,
     438,   439,    25,    55,   118,    25,     2,    55,   171,    55,
      33,   120,    57,    33,   107,    57,   111,   107,    41,    42,
      43,    41,    42,    43,   121,     2,   119,   148,   629,   119,
      26,   966,    33,    90,   176,    48,    48,   148,   118,   176,
    1019,    91,    94,   126,   148,    95,   484,   485,   778,    26,
    1094,    48,   490,   105,   147,   957,   113,   147,   172,   105,
     171,   546,   964,   761,   147,   148,   175,   171,   121,  1048,
    1049,   148,   126,   166,   171,   171,    80,    81,   171,  1018,
      76,   171,   967,   215,    80,    81,   173,  1014,    48,   482,
     483,   174,   172,   147,   148,   172,   368,   120,   447,    76,
     120,   173,   215,    80,    81,    48,   102,    47,   121,   175,
     106,   171,   166,   815,   816,   817,   596,   426,   171,   737,
     121,     3,   832,   166,   121,   102,   148,    32,   171,   106,
      28,   173,    92,   482,   483,   148,   148,  1116,   270,    48,
     521,   579,    84,   275,    57,    87,    86,   383,   796,    92,
     172,   148,   175,     3,    47,   175,    47,   270,    57,  1138,
      54,   148,   275,   176,   176,    19,   559,   548,   517,  1110,
     171,    21,  1074,    55,    67,   115,    67,   111,   148,   176,
     142,    75,    80,    92,    34,   148,    91,   174,   150,   148,
      95,    85,  1094,    86,    92,    86,   278,    92,   280,   534,
     166,   148,   126,   538,   174,    55,   936,   173,   173,   497,
     559,   174,   148,    12,    64,   174,   403,   148,   116,   215,
     352,   352,   115,   105,   115,    79,   965,   174,   152,    57,
     148,   148,    32,   582,   366,   166,   172,   142,   215,   352,
     148,   393,   147,   375,    98,   150,   328,   399,   173,   154,
     175,   148,   148,   366,   172,   105,   388,   174,   173,   877,
     114,   111,   375,   148,   599,   166,   174,   148,  1146,   166,
      68,   172,    57,   148,   270,   388,  1062,  1063,   174,   275,
     572,   573,   148,   148,    51,   166,   148,    54,   986,   174,
      88,    91,   148,   270,   426,    95,   148,  1007,   275,   174,
     166,   919,   148,   148,   166,   103,   471,   472,    75,   174,
     166,    78,   477,   166,  1052,   480,   481,   923,    85,   172,
     166,   166,   174,   148,   105,  1027,  1028,  1205,  1206,  1207,
    1208,  1209,  1210,    57,   148,   148,    57,   489,  1040,  1041,
       2,   166,   142,   143,   111,   145,   146,   147,    57,   149,
     150,   879,   166,   166,   154,   148,   352,   898,    91,   148,
     547,   161,    95,    57,    26,   547,   547,   167,   168,   169,
     366,     9,   173,   166,   175,   352,   508,   166,   510,   375,
      57,    91,   142,   617,   166,    95,   173,   147,   737,   366,
     150,   173,   388,   175,   154,   508,   102,   510,   375,  1109,
    1102,  1103,  1104,  1105,  1142,    91,    91,   572,   573,    95,
      95,   388,   761,    28,    76,   173,    91,   175,    80,    81,
      95,    57,    28,    28,   862,   173,   578,   175,   171,   173,
     173,   229,   173,    48,   175,   873,   173,    52,   175,   173,
     102,   175,    48,    48,   106,    32,    52,    52,    80,    81,
     292,   293,     5,    57,   586,   586,    57,    57,    57,   150,
     171,    57,    77,   147,   171,    80,   166,    82,   266,   173,
     165,    77,    77,   586,    80,    80,   914,    92,   827,   828,
     918,    57,   171,  1152,   166,   166,    92,    92,   166,    32,
     166,  1019,   166,   291,   166,   166,   111,   166,    86,   937,
     938,   116,   790,   166,    91,   111,   111,   173,    95,   173,
     116,   116,   508,   173,   510,   174,   166,   148,   147,   173,
    1048,   603,   604,   166,   148,  1053,   174,     9,   877,   611,
     612,   508,  1201,   510,   332,     3,   173,    67,   125,    80,
     126,   171,   150,   166,    70,   983,   173,   985,    91,   126,
     148,   126,    95,   215,   174,   142,   143,   126,   145,   146,
     147,    35,   149,   150,   151,   853,   153,   154,   155,   156,
     919,   158,   159,   160,   161,   162,   163,   164,    28,    35,
     167,   168,   169,    67,   174,   174,   126,   147,  1116,   898,
     586,   178,   174,    22,   759,   120,   148,   171,    48,   142,
     143,   173,    52,   146,   147,   172,   149,   150,   270,   586,
     126,   154,   107,   275,   126,   173,   781,   150,   161,   150,
     166,   150,   754,   150,   167,   168,   169,    77,   150,   150,
      80,   150,    82,   765,   142,   150,   768,   986,   166,    28,
     150,   153,    92,   155,   156,   150,   158,   159,    28,   150,
     150,   150,   765,   150,   150,   768,   174,   166,    33,    48,
     166,   111,   166,    52,   166,   176,   116,   174,    48,    35,
     173,    35,    52,   471,   472,   147,   174,   148,   172,   477,
     166,   171,   480,   481,   482,   483,    48,   176,    77,   176,
     352,    80,   879,    82,  1087,  1088,   172,    77,   885,   174,
      80,    35,    82,    92,   366,   166,   858,   894,   166,   176,
     173,   173,    92,   375,   147,   166,   148,   174,    33,   517,
     174,   176,   111,    35,   174,   166,   388,   116,   860,   176,
     176,   111,   864,   148,   816,   817,   116,   869,  1087,  1088,
     872,    32,   166,   174,   174,   165,   165,   860,   111,   581,
     375,   864,   265,   986,   825,    25,   869,   368,   392,   872,
     953,   559,   341,   979,   532,   386,   894,   547,   898,   765,
     885,   306,   768,  1201,   572,   573,  1055,  1211,  1204,   852,
     683,    32,   131,   529,   582,   174,   777,    21,   765,   871,
     923,   768,    -1,   875,    -1,    -1,    -1,    -1,    -1,    -1,
      91,     2,    -1,    -1,    95,   937,   938,    -1,    -1,    -1,
      -1,    -1,   944,    -1,   946,  1049,    -1,   949,    -1,    -1,
      -1,  1130,    -1,   975,    -1,    26,    -1,    -1,    -1,    -1,
      -1,   944,  1019,   946,    -1,    -1,   949,    -1,    -1,    -1,
      91,    -1,   974,    -1,    95,    -1,   508,    -1,   510,    -1,
      -1,   142,   984,    -1,    -1,    -1,   147,    -1,   149,   150,
      -1,   974,    -1,   154,   860,    -1,    -1,    -1,   864,    32,
     161,   984,    -1,   869,    -1,    76,   872,    -1,   169,    80,
      81,    -1,    -1,   860,    -1,    -1,    -1,   864,    -1,    -1,
      -1,   142,   869,    -1,  1026,   872,   147,    -1,   149,   150,
    1032,   102,    -1,   154,  1138,   106,    -1,    -1,  1142,    -1,
     161,    -1,    -1,  1026,    -1,    -1,   167,   168,   169,  1032,
      -1,  1230,    -1,  1232,   586,    -1,  1235,  1236,    91,  1116,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,   737,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   944,  1081,
     946,    -1,    -1,   949,    -1,    -1,    -1,    -1,  1040,  1041,
      -1,   759,    -1,   761,    -1,    -1,    -1,   944,  1081,   946,
      -1,    -1,   949,    -1,    -1,    -1,    -1,    -1,   974,   142,
     143,    -1,    -1,   781,   147,    -1,   149,   150,   984,    -1,
      -1,   154,  1124,    -1,  1126,    -1,    -1,   974,   161,  1131,
      -1,    -1,  1134,    -1,   167,   168,   169,   984,    -1,    -1,
      -1,  1124,    -1,  1126,   215,    -1,  1098,    -1,  1131,    -1,
      -1,  1134,  1104,  1105,    -1,    -1,    -1,    -1,    -1,    -1,
    1026,  1163,    -1,    -1,    -1,    -1,  1032,  1169,    -1,  1121,
      -1,  1173,  1174,    -1,    -1,    -1,    -1,    -1,    -1,  1026,
    1163,  1133,    -1,  1185,  1136,  1032,  1169,  1189,    -1,  1191,
    1173,  1174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   270,
      -1,    -1,  1185,    -1,   275,    -1,  1189,    -1,  1191,   877,
    1212,  1212,    -1,    -1,    -1,  1081,    -1,    -1,    -1,    -1,
       2,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1212,
      -1,    -1,    -1,   765,  1081,    -1,   768,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,   919,  1254,  1255,  1256,  1257,    -1,    -1,  1124,    -1,
    1126,    -1,    -1,    -1,    -1,  1131,    -1,    -1,  1134,    -1,
      -1,  1254,  1255,  1256,  1257,    -1,    -1,  1124,    -1,  1126,
      -1,   352,     2,    -1,  1131,    -1,    -1,  1134,    -1,    -1,
      -1,    -1,    -1,    -1,    76,   366,    -1,  1163,    80,    81,
      -1,    -1,    -1,  1169,   375,    -1,    26,  1173,  1174,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1163,   388,   986,  1185,
     102,    -1,  1169,  1189,   106,  1191,  1173,  1174,   860,    -1,
      -1,    -1,   864,    -1,    -1,    -1,    -1,   869,  1185,    -1,
     872,    -1,  1189,    -1,  1191,    -1,  1212,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,  1212,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,    -1,    -1,   106,    -1,  1254,  1255,
    1256,  1257,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1254,  1255,  1256,
    1257,    -1,   944,    -1,   946,    -1,    -1,   949,    -1,  1087,
    1088,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   215,    -1,     5,    -1,   508,    -1,   510,
      -1,    -1,   974,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   984,    23,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,
      -1,    -1,    -1,     2,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,   270,    -1,
      -1,    -1,    -1,   275,  1026,   215,    -1,    26,    -1,    -1,
    1032,    -1,    91,    73,    -1,    -1,    95,    -1,    78,    -1,
      80,    -1,    82,    -1,    84,   586,    -1,    87,    -1,    -1,
      90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,    -1,
      -1,   111,    -1,   113,    -1,    -1,   116,    76,    -1,  1081,
     270,    80,    81,   142,   143,   275,   145,   146,   147,    26,
     149,   150,   151,    -1,    -1,   154,    -1,    -1,    -1,    -1,
     352,   160,   161,   102,    -1,   164,    -1,   106,   167,   168,
     169,    -1,    -1,    -1,   366,    -1,    -1,    -1,    -1,   178,
      -1,    -1,  1124,   375,  1126,    -1,   166,    -1,    -1,  1131,
      -1,    -1,  1134,    -1,    -1,    -1,   388,    -1,    23,    76,
      -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,
      -1,  1163,   352,    -1,    -1,   102,    51,  1169,    -1,   106,
      -1,  1173,  1174,    -1,    -1,    -1,   366,    -1,    -1,    -1,
      -1,    -1,    -1,  1185,    -1,   375,    -1,  1189,    73,  1191,
      -1,    -1,    -1,    78,    -1,    80,    -1,     1,   388,     3,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    92,    -1,    -1,
    1212,    -1,    -1,    -1,    18,    19,   215,    21,    -1,    -1,
      -1,    -1,    26,    -1,   765,    -1,   111,   768,   113,    -1,
      34,   116,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,  1254,  1255,  1256,  1257,   508,    -1,   510,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,
      -1,   270,    -1,    -1,    -1,    79,   275,    -1,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,     1,
      -1,    -1,    -1,     5,    98,    99,    -1,   101,    -1,   103,
      -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,
     114,    23,    -1,   117,    -1,    27,    28,    -1,   508,   860,
     510,    -1,    -1,   864,    36,    -1,    38,    39,   869,    -1,
      -1,   872,    44,   270,   586,    -1,    -1,    -1,   275,    51,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   352,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    -1,    75,    -1,    -1,    78,   366,    80,   173,
      82,    -1,    84,    85,    -1,    87,   375,    -1,    90,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   388,
      -1,    -1,    -1,    -1,     3,    -1,   586,    -1,    -1,   111,
      -1,   113,    -1,   944,   116,   946,    -1,    -1,   949,    18,
      19,    -1,    21,    -1,    -1,   352,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    34,    -1,    -1,    -1,   366,
      -1,    40,    -1,   974,    -1,    -1,    -1,    46,   375,    -1,
      -1,    50,    -1,   984,    53,    -1,    55,    -1,    -1,    -1,
      -1,   388,    -1,    -1,   166,    64,    -1,    -1,    -1,    -1,
     172,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    91,    -1,  1026,    -1,    95,    -1,    98,
      99,  1032,   101,    -1,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,   508,
      -1,   510,    -1,   765,    -1,    -1,   768,   125,    -1,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   142,   143,    -1,   145,   146,   147,
    1081,   149,   150,   151,    -1,   153,   154,   155,   156,    -1,
     158,   159,   160,   161,   162,    -1,   164,    -1,    -1,   167,
     168,   169,   171,   170,   173,    -1,   173,    -1,   175,    -1,
     178,   508,    -1,   510,    -1,   765,    -1,    -1,   768,    -1,
      -1,    -1,    -1,  1124,    -1,  1126,    -1,   586,    -1,    -1,
    1131,    -1,    -1,  1134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   860,    -1,
      -1,    -1,   864,    -1,    -1,    -1,    -1,   869,    -1,    -1,
     872,    -1,  1163,    -1,    -1,    -1,    -1,    -1,  1169,    -1,
      -1,    -1,  1173,  1174,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    -1,  1185,    -1,    -1,    -1,  1189,   586,
    1191,    -1,    -1,    -1,    -1,    -1,    18,    19,    -1,    21,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
     860,  1212,    34,    -1,   864,    -1,    -1,    -1,    40,   869,
      -1,    -1,   872,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,   944,    55,   946,    -1,    -1,   949,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,  1254,  1255,  1256,  1257,    79,    -1,    -1,
      -1,    -1,   974,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,   984,    -1,    -1,    -1,    98,    99,    -1,   101,
      -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
     112,    -1,   114,    -1,   944,   117,   946,    -1,    -1,   949,
      -1,    -1,    -1,    -1,    -1,    -1,   765,    -1,    -1,   768,
      -1,    -1,    -1,    -1,  1026,    -1,    -1,    -1,    -1,    -1,
    1032,     5,    -1,    -1,   974,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   984,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,   173,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    51,   765,  1081,
      -1,   768,    -1,    -1,    -1,    -1,  1026,    -1,    -1,    -1,
      -1,    -1,  1032,    -1,    -1,    -1,    -1,    -1,    -1,    73,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      84,   860,    -1,    87,    -1,   864,    90,    -1,    92,    -1,
     869,    -1,  1124,   872,  1126,    -1,    -1,    -1,    -1,  1131,
      -1,    -1,  1134,    -1,    -1,    -1,    -1,   111,    -1,   113,
      -1,  1081,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,  1163,    -1,    -1,    -1,    -1,    -1,  1169,    -1,    -1,
      -1,  1173,  1174,   860,    -1,    -1,    -1,   864,    -1,    -1,
      -1,    -1,   869,  1185,  1124,   872,  1126,  1189,    -1,  1191,
      -1,  1131,   166,    -1,  1134,   944,    -1,   946,   172,    -1,
     949,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1212,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    -1,  1163,    -1,   974,    -1,    -1,    -1,  1169,
      -1,    -1,    -1,  1173,  1174,   984,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1185,    -1,    -1,    -1,  1189,
      -1,  1191,  1254,  1255,  1256,  1257,    -1,   944,    -1,   946,
      -1,    -1,   949,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,  1212,   149,   150,    -1,    -1,  1026,   154,    -1,
      -1,    -1,    -1,  1032,   160,   161,    -1,   974,   164,     5,
      -1,   167,   168,   169,    -1,    -1,    -1,   984,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    23,    -1,    -1,
      -1,    27,    28,    -1,  1254,  1255,  1256,  1257,    -1,    -1,
      36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,  1081,    -1,    -1,    51,    -1,    -1,    -1,  1026,
      -1,    -1,    -1,    -1,    -1,  1032,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    73,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    84,     3,
      -1,    87,    -1,    -1,    90,  1124,    92,  1126,    -1,    -1,
      -1,    -1,  1131,    -1,    18,  1134,   106,    21,    -1,    -1,
      -1,    -1,    26,    -1,  1081,   111,    -1,   113,    -1,    -1,
     116,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    -1,  1163,    -1,    -1,    -1,    -1,    53,
    1169,    55,    -1,    -1,  1173,  1174,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,  1185,  1124,    72,  1126,
    1189,    -1,  1191,    -1,  1131,    -1,    -1,  1134,    -1,    -1,
     166,    -1,    -1,    -1,    -1,    89,   172,    -1,    -1,    -1,
      -1,    -1,    -1,  1212,    -1,    -1,    -1,   101,    -1,    -1,
      -1,   105,    -1,    -1,    -1,    -1,  1163,    -1,   112,    -1,
      -1,    -1,  1169,   117,    -1,    -1,  1173,  1174,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   215,    -1,    -1,  1185,    -1,
      -1,    -1,  1189,    -1,  1191,  1254,  1255,  1256,  1257,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    -1,    -1,  1212,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     270,    -1,    -1,    -1,    46,   275,    48,    49,    -1,    -1,
      -1,    -1,    54,    -1,    56,    -1,    58,  1254,  1255,  1256,
    1257,    63,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    -1,    -1,    -1,    76,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    88,    -1,    90,    -1,
      92,    -1,    -1,    95,    96,    -1,    98,    -1,    -1,    -1,
     102,   103,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   388,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   174,    -1,     3,     4,    -1,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,   197,    25,    26,    -1,   201,
     202,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    43,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,   229,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,   266,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,    -1,   105,    -1,   508,   108,
     510,    -1,    -1,   112,    -1,   114,    -1,    -1,   117,   291,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     302,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,   144,   318,   319,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
     332,    -1,    -1,    -1,   336,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,   173,    -1,   175,    -1,    70,    -1,
      -1,    -1,   354,    -1,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,   364,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   376,    -1,   378,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     392,   393,   394,    -1,    -1,    -1,    -1,   399,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   435,   436,   437,   438,   439,   440,    -1,
     442,   443,   444,   445,   446,    -1,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,    -1,    -1,    -1,    -1,    -1,    -1,   471,
     472,    -1,    -1,    -1,    -1,   477,    -1,    -1,   480,   481,
     482,   483,   484,   485,    -1,    -1,    -1,   489,   490,    -1,
      -1,   493,    -1,    -1,    -1,    -1,    -1,   499,    -1,   221,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   510,    -1,
      -1,   513,    -1,    -1,    -1,   517,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   526,    -1,   528,    -1,    -1,    -1,
     532,    -1,    -1,   535,   536,   765,    -1,    -1,   768,   261,
     262,    -1,    -1,    -1,    -1,   547,    -1,   269,   550,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   278,   559,   280,    -1,
      -1,    -1,    -1,    -1,    -1,   287,    -1,   569,    -1,    -1,
     572,   573,    -1,    -1,    -1,    -1,   578,   579,    -1,    -1,
     582,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,   593,   594,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    19,    -1,    21,   606,    -1,   328,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,   629,    46,    -1,
     860,    -1,    50,    -1,   864,    53,    -1,    55,    -1,   869,
      -1,    -1,   872,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,   674,    -1,    -1,    -1,    -1,    -1,    -1,   681,
      98,    99,    -1,   101,    -1,   103,    -1,   105,    -1,    -1,
     108,    -1,    -1,     1,   112,     3,   114,     5,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    19,    -1,    21,   944,    -1,   946,    -1,    26,   949,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,   736,   737,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,   974,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,   984,   173,    64,   759,    -1,   761,
     762,   763,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   781,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,   101,   796,   103,  1026,   105,   800,    -1,
     108,    -1,  1032,    -1,   112,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   825,    -1,    -1,    -1,   829,    -1,   831,
     832,   833,    -1,   835,    -1,   837,    -1,    -1,   840,   841,
     562,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1081,   854,    -1,    -1,    -1,   858,    -1,    -1,    -1,
     862,   863,   864,    -1,   866,   867,    -1,   869,    -1,    -1,
     872,   873,    -1,    29,    -1,   877,    -1,    -1,    -1,    -1,
     882,   603,   604,    -1,    -1,    41,    42,    43,    -1,   611,
     612,    -1,    -1,    -1,  1124,    -1,  1126,    -1,    -1,    -1,
      -1,  1131,    -1,    -1,  1134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   914,   915,   916,   917,   918,   919,    -1,    -1,
      -1,   923,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     5,    -1,  1163,    -1,   937,   938,    -1,    -1,  1169,
      -1,    -1,    -1,  1173,  1174,   947,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    27,    28,  1185,    -1,    -1,    -1,  1189,
      -1,  1191,    36,   965,    38,    39,    -1,   969,    -1,    -1,
      44,    -1,    -1,   975,    -1,    -1,    -1,    51,    -1,    -1,
      -1,   983,    -1,   985,   986,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    73,
      -1,    -1,    -1,    -1,    78,  1007,    80,    -1,   164,    -1,
      84,    -1,  1014,    87,  1016,    -1,    90,    -1,    92,    -1,
      -1,  1023,    -1,    -1,  1254,  1255,  1256,  1257,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1038,   111,    -1,   113,
      -1,    -1,   116,   199,    -1,    -1,    -1,   769,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1057,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1069,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   806,  1087,  1088,    -1,    -1,    -1,
      -1,    -1,   166,    -1,   816,   817,    -1,    -1,  1100,    -1,
      -1,    -1,    -1,   259,   260,    -1,  1108,  1109,  1110,    -1,
      -1,    -1,    -1,    -1,    -1,   271,    -1,    -1,    -1,    -1,
      -1,    -1,  1124,    -1,  1126,    -1,    -1,  1129,    -1,  1131,
      -1,    -1,  1134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   871,
      -1,    -1,    -1,   875,  1156,  1157,   312,  1159,  1160,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1179,    -1,    -1,
    1182,    -1,    -1,    -1,  1186,    -1,    -1,    -1,    -1,  1191,
    1192,    -1,    -1,  1195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1203,    -1,    -1,    -1,    -1,   362,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1217,  1218,  1219,  1220,  1221,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   954,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   398,    -1,    -1,    -1,   402,    -1,    -1,    -1,
      -1,    -1,  1254,  1255,  1256,  1257,  1258,  1259,  1260,  1261,
     982,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     426,    -1,    -1,    -1,    -1,    -1,    18,    19,    -1,    21,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,  1040,  1041,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,    -1,    -1,   491,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,   101,
      -1,   103,   518,   105,    -1,   521,   108,    -1,    -1,    -1,
     112,    -1,   114,    -1,    -1,   117,  1098,    -1,   534,    -1,
      -1,    -1,  1104,  1105,    -1,    -1,    -1,     5,   544,    -1,
      -1,    -1,   548,    -1,    -1,    -1,    -1,    -1,    -1,  1121,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,  1130,    27,
      28,  1133,    -1,    -1,  1136,    -1,    -1,    -1,    36,    -1,
      38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      -1,   173,    -1,    51,    -1,    -1,   592,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   602,    -1,    -1,  1171,
      -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,
      78,   617,    80,    -1,    82,    -1,    84,    -1,    -1,    87,
      -1,    -1,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1230,    -1,
    1232,    -1,    -1,  1235,  1236,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   683,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,   172,    -1,    -1,     1,    -1,     3,
       4,    -1,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,
      -1,    25,    26,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    43,
      -1,    45,    46,    -1,    48,    -1,    50,    -1,    52,    53,
      -1,    55,    -1,    57,   770,    -1,    60,    61,    -1,    -1,
      64,   777,    66,    -1,    68,    69,    -1,    71,    72,    -1,
      -1,   787,    -1,    77,    -1,    79,    80,    81,    -1,    83,
      -1,    -1,    -1,    -1,    88,    89,    90,    -1,    92,    -1,
     806,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
      -1,   105,    -1,    -1,   108,    -1,    -1,   111,   112,    -1,
     114,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   852,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
     174,   175,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   898,    -1,    -1,    -1,    -1,    -1,    -1,     0,
       1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    16,    17,    18,    19,    -1,
      21,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,   953,    49,    50,
      51,    -1,    53,    54,    55,    -1,    57,    58,    59,    60,
      61,    -1,    63,    64,    65,    66,    67,    68,    69,    -1,
      71,    72,    73,   979,    75,    -1,    -1,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    94,    -1,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,   105,   106,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,    -1,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,  1052,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,    -1,   178,    -1,    -1,
      -1,     1,    -1,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    13,    -1,    15,    16,    17,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,  1142,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,    69,
      -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,   105,   106,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
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
      -1,   171,    -1,   173,    -1,   175,    -1,     1,   178,     3,
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
      -1,   169,    -1,   171,    -1,   173,     1,   175,     3,     4,
     178,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
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
      -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      21,    -1,    23,    -1,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    43,    -1,    45,    46,    -1,    -1,    -1,    50,
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
     171,    -1,   173,    -1,   175,     3,     4,   178,     6,     7,
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
      23,    -1,    25,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    -1,    -1,    66,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    80,    81,    82,
      83,    -1,    -1,    -1,    -1,    88,    -1,    90,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    98,    -1,   100,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,   174,   175,    -1,     4,   178,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    23,    -1,    25,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    41,    42,    43,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    57,    -1,    -1,
      60,    61,    -1,    -1,    -1,    -1,    66,    -1,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      80,    81,    82,    83,    -1,    -1,    -1,    -1,    88,    -1,
      90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    98,    -1,
     100,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    13,
      -1,   151,    -1,    -1,    18,    19,    20,    21,    -1,    -1,
     160,    -1,    26,   163,   164,   165,    -1,    -1,    -1,   169,
      34,   171,    36,   173,    -1,   175,    40,    -1,   178,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,    -1,   101,    -1,   103,
      -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,
     114,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,   129,   130,   131,    -1,    -1,
     134,   135,   136,   137,    -1,   139,   140,    -1,   142,   143,
     144,   145,   146,   147,    -1,   149,    -1,    -1,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
     164,    -1,    -1,   167,   168,   169,   170,     3,     4,   173,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    43,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,
      66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,   112,    -1,   114,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,    -1,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
       3,     4,    -1,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      43,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,    -1,   105,    -1,    -1,   108,     3,    -1,    -1,   112,
      -1,   114,    -1,    -1,   117,    -1,    13,    -1,    -1,    -1,
      -1,    18,    19,    20,    21,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,   142,
      -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    56,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    64,    -1,    -1,
     173,    -1,   175,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,   101,    -1,   103,    -1,   105,    -1,
      -1,    -1,    -1,    -1,   111,   112,    -1,   114,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,    -1,    -1,   134,   135,   136,
     137,    -1,   139,   140,    -1,   142,   143,   144,   145,   146,
     147,    -1,   149,    -1,    -1,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,     3,   164,    -1,    -1,
     167,   168,   169,   170,   171,    -1,    13,    -1,    -1,    -1,
      -1,    18,    19,    20,    21,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    98,    99,    -1,   101,    -1,   103,    20,   105,    -1,
      -1,   108,    -1,    -1,    -1,   112,    -1,   114,    -1,    32,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,    -1,    -1,   134,   135,   136,
     137,    -1,   139,   140,    -1,   142,   143,   144,   145,   146,
     147,    -1,   149,    -1,    -1,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,   164,    -1,    -1,
     167,   168,   169,   170,    -1,    -1,    -1,    -1,    91,    13,
      -1,    -1,    95,    -1,    -1,    -1,    20,    -1,    -1,    -1,
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
      -1,    95,    -1,    -1,    13,   178,    -1,    -1,    -1,    -1,
     104,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,    -1,    48,
     134,   135,   136,   137,    -1,   139,   140,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,   171,    -1,    -1,
      -1,    -1,    91,    13,   178,    -1,    95,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,   125,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    91,   171,    -1,    -1,    95,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    32,    -1,   125,    -1,    -1,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    74,    -1,   167,   168,   169,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    14,    -1,    -1,    -1,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,   125,    -1,    -1,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    74,   166,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    91,    13,    -1,    -1,    95,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    91,    -1,    -1,    13,    95,    -1,    -1,    -1,
     178,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    -1,   171,    91,    13,    -1,    -1,    95,    -1,   178,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    91,   171,    -1,    13,    95,    -1,    -1,    -1,
     178,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    -1,   171,    91,    13,    -1,    -1,    95,    -1,   178,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,   104,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,   125,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    74,   167,   168,
     169,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    91,    13,    -1,    -1,    95,    -1,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    91,    -1,    13,    -1,    95,    -1,    -1,
      -1,   178,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    14,    -1,    -1,    -1,
      -1,    -1,    20,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    32,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,    91,    -1,    13,    -1,    95,   176,    -1,
     178,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    13,    -1,    95,    -1,    -1,
      -1,    -1,    20,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    32,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   125,    -1,   167,
     168,   169,    -1,    91,    -1,    -1,    -1,    95,   176,    -1,
     178,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   125,    -1,   167,
     168,   169,    -1,    91,    -1,    13,    -1,    95,    -1,    -1,
     178,    -1,    20,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    32,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   125,    -1,   167,
     168,   169,    -1,   171,    -1,    13,    -1,    -1,    -1,    -1,
     178,    -1,    20,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    32,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,   166,   167,
     168,   169,    -1,    91,    -1,    13,    14,    95,    -1,    -1,
     178,    -1,    20,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   125,    -1,   167,
     168,   169,    -1,    91,    -1,    13,    -1,    95,    -1,    -1,
     178,    -1,    20,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    32,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   125,    -1,   167,
     168,   169,    -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,
     178,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,    91,    13,    -1,    -1,    95,    -1,    -1,
     178,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    32,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    91,    -1,    -1,    13,    95,    -1,    -1,    -1,
     178,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,   125,   167,   168,
     169,    -1,    -1,    91,    -1,    13,    14,    95,    -1,   178,
      -1,    -1,    20,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    32,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   125,    -1,   167,
     168,   169,    -1,    -1,    -1,    13,   174,    -1,    -1,    -1,
     178,    -1,    20,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    32,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,    91,    -1,    -1,   174,    95,    -1,    -1,
     178,    13,    14,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   125,    -1,   167,
     168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
     178,    -1,    -1,    95,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    13,    -1,   167,
     168,   169,    -1,   125,    20,    -1,   174,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    13,    -1,   167,   168,   169,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    95,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
     166,   167,   168,   169,   124,   125,    -1,    -1,    -1,    -1,
      -1,    91,   178,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   125,    -1,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    13,    -1,   167,   168,   169,
      -1,    -1,    20,    -1,    -1,    -1,   176,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    14,    -1,    -1,    -1,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      62,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   125,    -1,   167,
     168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
     178,    -1,    -1,    95,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    13,    -1,   167,
     168,   169,    -1,   125,    20,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    13,    -1,    -1,    95,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    91,    -1,    13,    -1,    95,    -1,
      -1,    -1,   178,    20,    -1,   142,   143,   104,   145,   146,
     147,    -1,   149,   150,   151,    32,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,   162,    -1,   164,   125,    -1,
     167,   168,   169,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      -1,   178,    -1,    20,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    32,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,    91,    -1,    13,    -1,    95,    -1,
      -1,   178,    -1,    20,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    13,    -1,    95,    -1,
      -1,    -1,    -1,    20,    -1,   142,   143,   104,   145,   146,
     147,    -1,   149,   150,   151,    32,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   125,    -1,
     167,   168,   169,    -1,    91,    -1,    13,    -1,    95,    -1,
      -1,   178,    -1,    20,    -1,   142,   143,   104,   145,   146,
     147,    -1,   149,   150,   151,    32,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   125,    -1,
     167,   168,   169,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,   178,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   125,    -1,
     167,   168,   169,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,   178,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   125,    -1,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    32,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    23,
     167,   168,   169,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    91,    -1,    -1,    -1,    95,    73,
      -1,    75,    -1,    -1,    78,    -1,    80,    -1,    -1,    -1,
      84,    85,    86,    87,    -1,    -1,    90,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,    -1,   154,   155,   156,
      -1,   158,   159,   160,   161,    -1,    -1,   164,     3,    -1,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    18,    19,    -1,    21,    -1,    -1,    -1,
      -1,    26,   166,    28,    -1,    -1,    -1,   171,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,     3,    -1,     5,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    18,
      19,    -1,    21,    -1,    79,    80,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    34,    -1,    92,    -1,    -1,
      -1,    40,    -1,    98,    99,    -1,   101,    46,   103,    -1,
     105,    50,    -1,   108,    53,    -1,    55,   112,    -1,   114,
       3,   116,   117,    -1,    -1,    64,    -1,    -1,    -1,    -1,
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
      80,    92,    99,   103,   111,   116,   186,   225,   281,   282,
     283,   305,   306,   307,   333,   338,   368,   341,   109,   110,
     166,   285,   286,   367,   368,   370,   368,   226,   368,   368,
     368,   102,   173,   187,   368,   368,    73,   189,   193,   207,
     189,   207,   368,   370,   368,   368,   368,   368,   368,     1,
     172,   185,   208,   341,   152,   342,   343,   370,   368,    82,
     187,    23,    36,    39,    73,    90,   171,   190,   191,   192,
     203,   207,   195,   150,   197,   171,    47,    86,   115,   204,
      27,   325,   368,     9,   266,   368,   369,    25,    33,    41,
      42,    43,   120,   175,   242,   243,   350,   352,    57,   147,
     268,   221,   171,   173,   301,    54,    75,    85,   309,    28,
      48,    52,    77,    80,    92,   111,   116,   310,    28,    48,
      52,    77,    80,    92,   111,   116,   308,   221,   321,   322,
       1,   328,   165,   166,   368,    13,    20,    32,    91,    95,
     125,   142,   143,   145,   146,   147,   149,   150,   151,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   167,   168,   169,   178,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     170,   297,   173,   175,    91,    95,   368,   221,    41,   175,
     242,    57,     1,   173,   187,   166,   207,   324,   166,   148,
     166,   222,   344,   345,   346,   370,   221,   221,   104,   207,
     104,   126,   207,   297,   204,   344,   166,   173,   173,   207,
     187,   173,   221,   330,   331,     1,   147,   337,    48,   148,
     187,   226,   148,   226,    14,   173,   173,   226,   327,   344,
     231,   231,    48,    92,   306,   307,   174,   148,   173,   221,
     147,   166,   368,   368,   124,   287,   166,   171,   226,   173,
     344,   166,   368,   255,   255,   166,   172,   172,   185,   148,
     172,   368,   148,   148,   174,   174,     9,   368,   175,   242,
     243,     3,   173,   198,     1,   172,   208,   215,   216,   368,
     210,   368,    67,    37,    74,   166,   266,   268,   111,   237,
     290,   187,    80,   246,   368,   123,   176,   244,   341,   368,
     379,   244,   368,   221,   341,   171,     1,   194,   221,   272,
     275,   174,   302,   304,   305,   310,    48,    92,     1,   147,
     335,   336,    48,    92,     1,     3,    13,    18,    20,    21,
      26,    46,    53,    55,    56,    64,    72,    89,   101,   105,
     112,   117,   142,   143,   144,   145,   146,   147,   149,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   164,
     167,   168,   169,   170,   173,   221,   294,   295,   296,   297,
     347,   126,   323,   148,   166,   166,   166,   368,   368,   368,
     244,   368,   244,   368,   368,   368,   368,   368,   368,   368,
       3,    21,    34,    64,   105,   111,   222,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,    70,   370,   370,   370,   370,   370,
     344,   344,   244,   368,   244,   368,   104,   173,   341,   244,
     368,   221,     1,    28,    48,    52,    77,    80,    92,   111,
     116,   174,   358,   359,   360,   368,   384,   328,   368,   126,
     174,   148,   126,   126,   187,    35,   187,   368,    35,   368,
      67,   174,   345,   221,   248,   249,   331,   148,   174,   225,
     368,   126,   332,   368,   329,   262,   221,   324,   368,   368,
     324,   174,   221,   310,   283,   331,   147,   339,   340,   338,
     288,   206,     1,   260,   344,   174,    22,   256,   172,   174,
     174,   342,   370,   174,   342,   341,   244,   368,   197,   174,
     199,   200,   346,   172,   172,   148,   166,    14,   150,   148,
     166,    14,    37,    74,   221,   142,   143,   144,   145,   146,
     160,   164,   169,   209,   296,   297,   298,   368,   209,   211,
     268,   171,   238,   324,   120,   221,   226,   226,   173,   368,
     176,   357,    48,   148,   176,   357,   124,   148,   176,    33,
     226,   357,    33,    48,   226,   357,   166,   173,     1,   269,
     172,   275,   126,   148,   172,   148,   174,   337,     1,   225,
     333,   334,   107,   311,   173,   300,   368,   142,   150,   300,
     300,   368,   322,   173,   175,   166,   166,   166,   166,   166,
     166,   174,   176,   345,    48,   176,    33,    33,    48,   173,
     174,    48,    92,   148,   174,    18,    21,    26,    46,    53,
      64,    72,    89,   101,   112,   117,   347,    91,    91,   166,
     370,   346,   368,   368,    35,   187,    35,    35,   187,   104,
     207,   221,   174,   148,   174,   174,   330,   337,    70,   370,
     221,   174,   174,   328,   340,   147,   284,   174,   338,   152,
     299,   332,   368,   172,    76,   118,   172,   261,   174,   173,
     207,   221,   257,    48,   176,    48,   148,   174,   215,   222,
      18,    19,    21,    26,    46,    50,    53,    72,    79,    89,
      98,    99,   101,   103,   112,   114,   117,   171,   214,   298,
     368,   368,   209,   211,   148,   166,    14,   166,   171,   269,
     321,   328,   368,    48,   341,   187,   176,   244,   368,   176,
     187,   368,   176,   368,   176,   368,   368,   226,    45,   368,
     244,   368,   226,   345,   172,    84,    87,   172,   186,   194,
     228,   368,   273,   274,   304,   311,    62,   119,   315,   302,
     303,   174,   295,   297,   174,   176,   174,   244,   368,    45,
     244,   368,   345,   359,   337,   347,   347,   187,   187,   368,
     187,    35,   166,   166,   249,   207,   332,   173,   173,   166,
     299,   332,   333,   311,   340,   368,   187,   240,   341,   257,
     147,   207,   244,   368,   244,   368,   200,   209,    14,    37,
      74,   166,   166,   298,   368,   368,   269,   172,   166,   166,
     166,   368,   174,   187,   176,   357,   176,   357,   187,   124,
     368,    33,   226,   357,    33,   226,   357,   174,   194,   228,
     228,   275,   194,   315,     3,    55,    94,   105,   316,   317,
     318,   368,   291,   174,   300,   300,   176,   176,   368,    33,
      33,   174,   332,    35,   187,   344,   344,   299,   332,    33,
     226,   174,   368,   176,   176,   172,   368,   209,   211,    14,
     172,   226,    45,   187,   368,   176,    45,   187,   368,   176,
     368,   104,    45,   368,   226,    45,   368,   226,   166,   228,
     275,   278,   318,   119,   148,   126,   153,   155,   156,   158,
     159,    62,    33,   166,   206,   312,   313,    45,    45,   104,
      45,    45,   187,   174,   174,   187,   207,   166,   166,   368,
     368,   187,   368,   187,   368,   368,   368,   313,   368,   317,
     318,   318,   318,   318,   318,   318,   316,   185,   368,   368,
     368,   368,   165,   165,   104,   104,   104,   104,   104,   104,
     104,   104,   368,   368,   368,   368
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
     239,   240,   241,   241,   242,   242,   242,   242,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   244,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   246,   246,   247,   247,   248,   248,   249,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   251,   251,   251,   251,   251,   251,   252,   252,   252,
     253,   253,   253,   254,   254,   254,   254,   255,   255,   256,
     256,   256,   257,   257,   258,   259,   259,   260,   260,   261,
     261,   261,   262,   262,   262,   263,   263,   264,   265,   265,
     266,   267,   267,   267,   268,   268,   269,   269,   269,   269,
     269,   270,   270,   271,   272,   272,   273,   272,   272,   274,
     272,   275,   275,   276,   278,   277,   279,   280,   280,   280,
     281,   281,   282,   282,   283,   283,   283,   284,   284,   285,
     287,   286,   288,   286,   289,   291,   290,   292,   292,   292,
     292,   292,   293,   294,   294,   295,   295,   295,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   298,   298,   299,   299,   300,
     300,   301,   301,   302,   302,   303,   303,   304,   305,   305,
     305,   305,   305,   305,   306,   306,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   309,   309,   309,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   311,   311,   312,   312,   313,   313,   314,   315,   315,
     315,   315,   315,   316,   316,   317,   317,   317,   317,   317,
     317,   317,   318,   318,   318,   319,   320,   320,   321,   321,
     322,   323,   323,   324,   324,   324,   324,   324,   326,   325,
     327,   325,   325,   325,   328,   328,   329,   329,   330,   330,
     331,   331,   331,   332,   332,   332,   333,   334,   334,   334,
     335,   335,   336,   336,   337,   337,   337,   337,   338,   338,
     339,   340,   340,   341,   341,   342,   343,   343,   344,   344,
     345,   345,   346,   346,   347,   347,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   349,   350,
     350,   350,   350,   350,   350,   350,   351,   352,   352,   352,
     352,   352,   352,   352,   353,   354,   355,   355,   355,   355,
     355,   355,   355,   356,   356,   357,   357,   357,   357,   358,
     358,   359,   359,   359,   360,   360,   360,   360,   360,   360,
     360,   360,   360,   360,   361,   361,   361,   361,   361,   362,
     362,   362,   362,   362,   363,   364,   364,   364,   364,   364,
     364,   365,   366,   367,   367,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   369,   369,   370,   370,   370,   371,   371,   371,
     371,   372,   372,   372,   372,   372,   373,   373,   373,   374,
     374,   374,   374,   374,   374,   375,   375,   375,   375,   376,
     376,   377,   377,   378,   378,   378,   378,   378,   378,   378,
     378,   378,   378,   378,   378,   378,   379,   379,   380,   380,
     380,   380,   380,   380,   380,   380,   380,   380,   380,   380,
     380,   380,   380,   380,   380,   380,   380,   380,   380,   380,
     380,   381,   381,   381,   381,   381,   381,   381,   382,   382,
     382,   382,   383,   383,   383,   383,   384,   384,   384,   384,
     384,   384,   384
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
       2,     0,     3,     2,     1,     1,     1,     1,     5,     3,
       3,     6,     5,     6,     5,     6,     3,     4,     3,     4,
       6,     7,     6,     7,     4,     5,     4,     5,     4,     4,
       3,     6,     5,     4,     3,     6,     5,     6,     5,     8,
       7,     4,     4,     6,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     6,     4,     7,
       5,     3,     6,     2,     2,     3,     3,     0,     2,     2,
       3,     5,     1,     3,     3,     5,     5,     0,     2,     3,
       2,     4,     4,     3,     1,     1,     3,     3,     5,     5,
       2,     1,     1,     1,     0,     2,     0,     2,     3,     3,
       4,     4,     4,     2,     1,     2,     0,     4,     2,     0,
       5,     1,     3,     1,     0,     8,     0,     1,     1,     1,
       0,     1,     1,     3,     1,     3,     1,     0,     2,     6,
       0,     3,     0,     4,     1,     0,     7,     4,     4,     6,
       6,     4,     2,     1,     3,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     0,
       3,     2,     3,     1,     3,     0,     1,     1,     4,     5,
       4,     5,     6,     6,     0,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     0,     1,     1,     2,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       0,     1,     1,     2,     1,     1,     1,     1,     2,     1,
       1,     0,     1,     1,     1,     1,     2,     1,     0,     2,
       2,     4,     4,     1,     3,     3,     3,     3,     3,     3,
       3,     2,     1,     1,     1,     3,     1,     2,     1,     3,
       2,     0,     2,     1,     2,     1,     1,     1,     0,     5,
       0,     6,     3,     3,     1,     3,     3,     5,     1,     3,
       1,     2,     3,     0,     2,     2,     1,     1,     1,     1,
       2,     1,     0,     1,     0,     2,     2,     1,     1,     1,
       2,     0,     1,     1,     3,     1,     3,     3,     0,     1,
       1,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     4,     4,     4,     5,     2,     2,     1,     2,
       1,     2,     1,     2,     1,     2,     1,     1,     2,     6,
       6,     4,     4,     9,     9,     7,     2,     2,     3,     4,
       6,     6,     9,     9,     6,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     4,     3,     4,     2,     1,
       3,     4,     3,     3,     1,     1,     2,     1,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     2,
       8,     8,     9,     9,     4,     3,     3,     2,     2,     2,
       1,     3,     4,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     2,     2,     1,     1,     4,     4,     4,     3,
       3,     3,     3,     5,     5,     3,     4,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     4,     4,     5,     4,     5,     3,     5,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     1,     1
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
#line 664 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6007 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 668 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6013 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 669 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6019 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 676 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6027 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 684 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6035 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 688 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6044 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 698 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6052 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 703 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6060 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 710 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6066 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 711 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6072 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 712 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6078 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 713 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6084 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 714 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6090 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 715 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6096 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 716 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6102 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 717 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6108 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 718 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6114 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: TDEFER stmt  */
#line 720 "chpl.ypp"
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
#line 6136 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 737 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6142 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 738 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6148 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TINIT TTHIS TSEMI  */
#line 740 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr));
    auto node = Init::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6160 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 748 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6173 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 757 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6186 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TLABEL ident_def stmt  */
#line 766 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6194 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 770 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6205 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt_base: error TSEMI  */
#line 777 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6214 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: assignment_stmt  */
#line 784 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6220 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: if_stmt  */
#line 785 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6226 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: loop_stmt  */
#line 786 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6232 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: select_stmt  */
#line 787 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6238 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: manage_stmt  */
#line 788 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6244 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 789 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6250 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: throw_stmt  */
#line 790 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6256 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 792 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6262 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 794 "chpl.ypp"
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
#line 6286 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 814 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6298 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 822 "chpl.ypp"
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
#line 6319 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TLOCAL do_stmt  */
#line 839 "chpl.ypp"
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
#line 6339 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TON expr do_stmt  */
#line 855 "chpl.ypp"
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
#line 6360 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 872 "chpl.ypp"
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
#line 6381 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSERIAL do_stmt  */
#line 889 "chpl.ypp"
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
#line 6401 "bison-chpl-lib.cpp"
    break;

  case 45: /* tryable_stmt: TSYNC stmt  */
#line 905 "chpl.ypp"
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
#line 6423 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 930 "chpl.ypp"
  {
    auto attributeGroup = context->buildAttributeGroup((yyloc));
    if (attributeGroup != nullptr) {
      context->resetAttributeGroupPartsState();
    }
    context->loopAttributes.push_back(std::move(attributeGroup));
  }
#line 6435 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 955 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6443 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 962 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6452 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 969 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6460 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 973 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6471 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 983 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6477 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 985 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6485 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 989 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6493 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 996 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6502 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 1001 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6512 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 1014 "chpl.ypp"
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
#line 6534 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 1035 "chpl.ypp"
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
#line 6555 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 1052 "chpl.ypp"
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
#line 6576 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1069 "chpl.ypp"
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
#line 6595 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1086 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6601 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1087 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6608 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1089 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6615 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1094 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6621 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1095 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6628 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1101 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6638 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1119 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6647 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1124 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6656 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1129 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6669 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1141 "chpl.ypp"
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
#line 6688 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1159 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6694 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1160 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6700 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1165 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6708 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1169 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6716 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1173 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6725 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1178 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6733 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1182 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6741 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1186 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6750 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1194 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6759 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1199 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6769 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1205 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6778 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1210 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6789 "bison-chpl-lib.cpp"
    break;

  case 90: /* opt_only_ls: %empty  */
#line 1218 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6795 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: renames_ls  */
#line 1219 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6801 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1224 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6810 "bison-chpl-lib.cpp"
    break;

  case 93: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1229 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6822 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1237 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6835 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1246 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6847 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1254 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6860 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1265 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6869 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr  */
#line 1301 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6877 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT all_op_name  */
#line 1305 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6886 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1310 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6895 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TAS ident_use  */
#line 1315 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6905 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1321 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6915 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_expr  */
#line 1329 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6921 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_ls: import_ls TCOMMA import_expr  */
#line 1330 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6927 "bison-chpl-lib.cpp"
    break;

  case 122: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1335 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6937 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1344 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6945 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1348 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6953 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1352 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6961 "bison-chpl-lib.cpp"
    break;

  case 126: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1356 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6969 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1360 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6977 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1364 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6985 "bison-chpl-lib.cpp"
    break;

  case 129: /* opt_label_ident: %empty  */
#line 1372 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6991 "bison-chpl-lib.cpp"
    break;

  case 130: /* opt_label_ident: TIDENT  */
#line 1373 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6997 "bison-chpl-lib.cpp"
    break;

  case 131: /* opt_label_ident: TINIT  */
#line 1374 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7003 "bison-chpl-lib.cpp"
    break;

  case 140: /* ident_use: TIDENT  */
#line 1400 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7009 "bison-chpl-lib.cpp"
    break;

  case 141: /* ident_use: TINIT  */
#line 1401 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7015 "bison-chpl-lib.cpp"
    break;

  case 142: /* ident_use: TTHIS  */
#line 1402 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7021 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TBOOL  */
#line 1429 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7027 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TENUM  */
#line 1430 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7033 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TINT  */
#line 1431 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7039 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TUINT  */
#line 1432 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7045 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TREAL  */
#line 1433 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7051 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TIMAG  */
#line 1434 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7057 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TCOMPLEX  */
#line 1435 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7063 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TBYTES  */
#line 1436 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7069 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TSTRING  */
#line 1437 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7075 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TLOCALE  */
#line 1438 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7081 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TNOTHING  */
#line 1439 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7087 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TVOID  */
#line 1440 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7093 "bison-chpl-lib.cpp"
    break;

  case 178: /* do_stmt: TDO stmt  */
#line 1454 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7099 "bison-chpl-lib.cpp"
    break;

  case 179: /* do_stmt: block_stmt  */
#line 1455 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7105 "bison-chpl-lib.cpp"
    break;

  case 180: /* return_stmt: TRETURN TSEMI  */
#line 1460 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7116 "bison-chpl-lib.cpp"
    break;

  case 181: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1467 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7127 "bison-chpl-lib.cpp"
    break;

  case 182: /* class_level_stmt: TSEMI  */
#line 1477 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7135 "bison-chpl-lib.cpp"
    break;

  case 183: /* class_level_stmt: inner_class_level_stmt  */
#line 1481 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7145 "bison-chpl-lib.cpp"
    break;

  case 184: /* $@1: %empty  */
#line 1486 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7152 "bison-chpl-lib.cpp"
    break;

  case 185: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1488 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7161 "bison-chpl-lib.cpp"
    break;

  case 186: /* $@2: %empty  */
#line 1492 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7168 "bison-chpl-lib.cpp"
    break;

  case 187: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1494 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7177 "bison-chpl-lib.cpp"
    break;

  case 195: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1512 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7186 "bison-chpl-lib.cpp"
    break;

  case 196: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1517 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7195 "bison-chpl-lib.cpp"
    break;

  case 197: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1522 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7204 "bison-chpl-lib.cpp"
    break;

  case 198: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1527 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7212 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_start: TFORWARDING  */
#line 1534 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7221 "bison-chpl-lib.cpp"
    break;

  case 200: /* extern_or_export: TEXTERN  */
#line 1541 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7227 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_or_export: TEXPORT  */
#line 1542 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7233 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1547 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7243 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1557 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7253 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1564 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7265 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1572 "chpl.ypp"
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
#line 7287 "bison-chpl-lib.cpp"
    break;

  case 206: /* $@3: %empty  */
#line 1590 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7295 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1594 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7304 "bison-chpl-lib.cpp"
    break;

  case 208: /* $@4: %empty  */
#line 1599 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7313 "bison-chpl-lib.cpp"
    break;

  case 209: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1604 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7322 "bison-chpl-lib.cpp"
    break;

  case 210: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1612 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7330 "bison-chpl-lib.cpp"
    break;

  case 211: /* no_loop_attributes: %empty  */
#line 1617 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7336 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1620 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7342 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1621 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7348 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1638 "chpl.ypp"
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
#line 7375 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1661 "chpl.ypp"
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
#line 7398 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1680 "chpl.ypp"
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
#line 7421 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1699 "chpl.ypp"
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
#line 7450 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1724 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7459 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: for_loop_kw expr TIN expr task_intent_clause do_stmt  */
#line 1729 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7468 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1734 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7477 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator task_intent_clause do_stmt  */
#line 1739 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7486 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1744 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7495 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: for_loop_kw expr task_intent_clause do_stmt  */
#line 1749 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7504 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1754 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7513 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: for_loop_kw zippered_iterator task_intent_clause do_stmt  */
#line 1759 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7522 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1764 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7531 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TLSBR expr_ls TIN expr task_intent_clause TRSBR stmt  */
#line 1769 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7540 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1774 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7549 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator task_intent_clause TRSBR stmt  */
#line 1779 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7558 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1784 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7567 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TLSBR expr_ls task_intent_clause TRSBR stmt  */
#line 1789 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7576 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1794 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7586 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt_base: TLSBR zippered_iterator task_intent_clause TRSBR stmt  */
#line 1800 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7596 "bison-chpl-lib.cpp"
    break;

  case 238: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1809 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7606 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr TTHEN stmt  */
#line 1818 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7614 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF expr block_stmt  */
#line 1822 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7622 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1826 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7630 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1830 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7638 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1834 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7646 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF ifvar block_stmt  */
#line 1838 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7654 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1842 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7662 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1846 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7670 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1850 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7679 "bison-chpl-lib.cpp"
    break;

  case 248: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1855 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7688 "bison-chpl-lib.cpp"
    break;

  case 249: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1860 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7697 "bison-chpl-lib.cpp"
    break;

  case 250: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1865 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7706 "bison-chpl-lib.cpp"
    break;

  case 251: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1873 "chpl.ypp"
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
#line 7725 "bison-chpl-lib.cpp"
    break;

  case 252: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1888 "chpl.ypp"
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
#line 7744 "bison-chpl-lib.cpp"
    break;

  case 253: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1906 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7752 "bison-chpl-lib.cpp"
    break;

  case 254: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1910 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7760 "bison-chpl-lib.cpp"
    break;

  case 255: /* ifc_formal_ls: ifc_formal  */
#line 1916 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7766 "bison-chpl-lib.cpp"
    break;

  case 256: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1917 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7772 "bison-chpl-lib.cpp"
    break;

  case 257: /* ifc_formal: ident_def  */
#line 1922 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7778 "bison-chpl-lib.cpp"
    break;

  case 277: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1966 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7786 "bison-chpl-lib.cpp"
    break;

  case 278: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1970 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7794 "bison-chpl-lib.cpp"
    break;

  case 279: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1974 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7802 "bison-chpl-lib.cpp"
    break;

  case 280: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1981 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7810 "bison-chpl-lib.cpp"
    break;

  case 281: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1985 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7818 "bison-chpl-lib.cpp"
    break;

  case 282: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1989 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7826 "bison-chpl-lib.cpp"
    break;

  case 283: /* try_stmt: TTRY tryable_stmt  */
#line 1996 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7834 "bison-chpl-lib.cpp"
    break;

  case 284: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2000 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7842 "bison-chpl-lib.cpp"
    break;

  case 285: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2004 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7850 "bison-chpl-lib.cpp"
    break;

  case 286: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2008 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7858 "bison-chpl-lib.cpp"
    break;

  case 287: /* catch_expr_ls: %empty  */
#line 2014 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7864 "bison-chpl-lib.cpp"
    break;

  case 288: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2015 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7870 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr: TCATCH block_stmt  */
#line 2020 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7878 "bison-chpl-lib.cpp"
    break;

  case 290: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2024 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7886 "bison-chpl-lib.cpp"
    break;

  case 291: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2028 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7894 "bison-chpl-lib.cpp"
    break;

  case 292: /* catch_expr_inner: ident_def  */
#line 2035 "chpl.ypp"
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
#line 7911 "bison-chpl-lib.cpp"
    break;

  case 293: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2048 "chpl.ypp"
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
#line 7928 "bison-chpl-lib.cpp"
    break;

  case 294: /* throw_stmt: TTHROW expr TSEMI  */
#line 2064 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7938 "bison-chpl-lib.cpp"
    break;

  case 295: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2072 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7947 "bison-chpl-lib.cpp"
    break;

  case 296: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2077 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7958 "bison-chpl-lib.cpp"
    break;

  case 297: /* when_stmt_ls: %empty  */
#line 2086 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7964 "bison-chpl-lib.cpp"
    break;

  case 298: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2087 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7970 "bison-chpl-lib.cpp"
    break;

  case 299: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2092 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7978 "bison-chpl-lib.cpp"
    break;

  case 300: /* when_stmt: TOTHERWISE stmt  */
#line 2096 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7987 "bison-chpl-lib.cpp"
    break;

  case 301: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2101 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 7997 "bison-chpl-lib.cpp"
    break;

  case 302: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2110 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8006 "bison-chpl-lib.cpp"
    break;

  case 303: /* manager_expr: expr TAS ident_def  */
#line 2115 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8014 "bison-chpl-lib.cpp"
    break;

  case 304: /* manager_expr: expr  */
#line 2119 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8022 "bison-chpl-lib.cpp"
    break;

  case 305: /* manager_expr_ls: manager_expr  */
#line 2125 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8028 "bison-chpl-lib.cpp"
    break;

  case 306: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2126 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8034 "bison-chpl-lib.cpp"
    break;

  case 307: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2131 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8043 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2141 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8053 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2147 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8065 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_start: class_tag ident_def  */
#line 2159 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8073 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_tag: TCLASS  */
#line 2165 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8079 "bison-chpl-lib.cpp"
    break;

  case 312: /* class_tag: TRECORD  */
#line 2166 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8085 "bison-chpl-lib.cpp"
    break;

  case 313: /* class_tag: TUNION  */
#line 2167 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8091 "bison-chpl-lib.cpp"
    break;

  case 314: /* opt_inherit: %empty  */
#line 2171 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8097 "bison-chpl-lib.cpp"
    break;

  case 315: /* opt_inherit: TCOLON expr_ls  */
#line 2172 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8103 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_level_stmt_ls: %empty  */
#line 2176 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8112 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2181 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8120 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2185 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8128 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2189 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8136 "bison-chpl-lib.cpp"
    break;

  case 320: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2193 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8144 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
#line 2201 "chpl.ypp"
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
#line 8167 "bison-chpl-lib.cpp"
    break;

  case 322: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
#line 2220 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8181 "bison-chpl-lib.cpp"
    break;

  case 323: /* enum_header: TENUM ident_def  */
#line 2233 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8189 "bison-chpl-lib.cpp"
    break;

  case 324: /* enum_ls: enum_item  */
#line 2240 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8198 "bison-chpl-lib.cpp"
    break;

  case 325: /* enum_ls: enum_ls TCOMMA  */
#line 2245 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8208 "bison-chpl-lib.cpp"
    break;

  case 326: /* $@5: %empty  */
#line 2251 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8217 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2256 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8226 "bison-chpl-lib.cpp"
    break;

  case 328: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2261 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8235 "bison-chpl-lib.cpp"
    break;

  case 329: /* $@6: %empty  */
#line 2266 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8244 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2271 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8253 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_item: ident_def  */
#line 2281 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8264 "bison-chpl-lib.cpp"
    break;

  case 332: /* enum_item: ident_def TASSIGN expr  */
#line 2288 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8277 "bison-chpl-lib.cpp"
    break;

  case 333: /* lambda_decl_start: TLAMBDA  */
#line 2300 "chpl.ypp"
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
#line 8293 "bison-chpl-lib.cpp"
    break;

  case 334: /* $@7: %empty  */
#line 2317 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8303 "bison-chpl-lib.cpp"
    break;

  case 335: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2323 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-7].functionParts);
      fp.name = context->buildIdent((yylsp[-7]), STR("lambda"));
      fp.formals = (yyvsp[-6].exprList);
      fp.returnIntent = (yyvsp[-5].returnTag);
      fp.returnIntentLoc = (yylsp[-5]);
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
#line 8326 "bison-chpl-lib.cpp"
    break;

  case 337: /* linkage_spec: linkage_spec_empty  */
#line 2347 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8332 "bison-chpl-lib.cpp"
    break;

  case 338: /* linkage_spec: TINLINE  */
#line 2348 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8339 "bison-chpl-lib.cpp"
    break;

  case 339: /* linkage_spec: TOVERRIDE  */
#line 2350 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8346 "bison-chpl-lib.cpp"
    break;

  case 340: /* opt_fn_type_formal_ls: %empty  */
#line 2355 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8352 "bison-chpl-lib.cpp"
    break;

  case 341: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2356 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8358 "bison-chpl-lib.cpp"
    break;

  case 342: /* fn_type_formal_ls: fn_type_formal  */
#line 2360 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8364 "bison-chpl-lib.cpp"
    break;

  case 343: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2361 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8370 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_type_formal: named_formal  */
#line 2366 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8376 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2369 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8382 "bison-chpl-lib.cpp"
    break;

  case 346: /* fn_type_formal: formal_type  */
#line 2371 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8388 "bison-chpl-lib.cpp"
    break;

  case 347: /* opt_fn_type_ret_type: %empty  */
#line 2375 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8394 "bison-chpl-lib.cpp"
    break;

  case 348: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2376 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8400 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2389 "chpl.ypp"
  {
    FunctionParts fp = context->makeFunctionParts(false, false);
    fp.kind = Function::PROC;
    fp.name = context->buildIdent((yylsp[-5]), STR("proc"));
    fp.formals = (yyvsp[-4].exprList);
    fp.returnIntent = (yyvsp[-2].returnTag);
    fp.returnIntentLoc = (yylsp[-2]);
    fp.returnType = (yyvsp[-1].expr);
    fp.throws = ((yyvsp[0].throwsTag) == ThrowsTag_THROWS);
    fp.body = nullptr;
    fp.comments = nullptr;
    fp.attributeGroup = nullptr;
    fp.visibility = context->visibility;
    (yyval.functionParts) = fp;
  }
#line 8420 "bison-chpl-lib.cpp"
    break;

  case 350: /* $@8: %empty  */
#line 2408 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8430 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2414 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8443 "bison-chpl-lib.cpp"
    break;

  case 352: /* $@9: %empty  */
#line 2423 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8453 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2429 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8466 "bison-chpl-lib.cpp"
    break;

  case 354: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2441 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8474 "bison-chpl-lib.cpp"
    break;

  case 355: /* $@10: %empty  */
#line 2450 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8482 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2454 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-6].functionParts);
    fp.returnIntent = (yyvsp[-5].returnTag);
    fp.returnIntentLoc = (yylsp[-5]);
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
          ((yyvsp[-5].returnTag).isValid && (Function::ReturnIntent)(yyvsp[-5].returnTag).intent != Function::DEFAULT_RETURN_INTENT ? (yylsp[-5]) : (yylsp[-6]))));
    context->exitScopeForFunctionDecl(fp);
    (yyval.functionParts) = fp;
  }
#line 8505 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2476 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.thisIntentLoc = (yylsp[-2]);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8518 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2485 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.thisIntentLoc = (yylsp[-2]);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8531 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2494 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.thisIntentLoc = (yylsp[-4]);
      fp.receiver = context->buildThisFormal((yylsp[-3]), (yylsp[-4]), (yyvsp[-4].intentTag), (yyvsp[-3].expr), /*initExpr*/ nullptr);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8545 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2504 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.thisIntentLoc = (yylsp[-4]);
      fp.receiver = context->buildThisFormal((yylsp[-3]), (yylsp[-4]), (yyvsp[-4].intentTag), (yyvsp[-3].expr), /*initExpr*/ nullptr);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8559 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2514 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8569 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2523 "chpl.ypp"
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
#line 8585 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2538 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8591 "bison-chpl-lib.cpp"
    break;

  case 367: /* fn_ident: ident_def TBANG  */
#line 2545 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8601 "bison-chpl-lib.cpp"
    break;

  case 407: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2602 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8607 "bison-chpl-lib.cpp"
    break;

  case 408: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2603 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8613 "bison-chpl-lib.cpp"
    break;

  case 409: /* opt_formal_ls: %empty  */
#line 2607 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8619 "bison-chpl-lib.cpp"
    break;

  case 410: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2608 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8625 "bison-chpl-lib.cpp"
    break;

  case 411: /* req_formal_ls: TLP TRP  */
#line 2612 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8631 "bison-chpl-lib.cpp"
    break;

  case 412: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2613 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8637 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal_ls_inner: formal  */
#line 2617 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8643 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2618 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8649 "bison-chpl-lib.cpp"
    break;

  case 415: /* formal_ls: %empty  */
#line 2622 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8655 "bison-chpl-lib.cpp"
    break;

  case 416: /* formal_ls: formal_ls_inner  */
#line 2623 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8661 "bison-chpl-lib.cpp"
    break;

  case 418: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2632 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8669 "bison-chpl-lib.cpp"
    break;

  case 419: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2637 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8677 "bison-chpl-lib.cpp"
    break;

  case 420: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2642 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8685 "bison-chpl-lib.cpp"
    break;

  case 421: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2647 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8693 "bison-chpl-lib.cpp"
    break;

  case 422: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2652 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yylsp[-5]), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8701 "bison-chpl-lib.cpp"
    break;

  case 423: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2657 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8709 "bison-chpl-lib.cpp"
    break;

  case 424: /* opt_formal_intent_tag: %empty  */
#line 2663 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);
  }
#line 8718 "bison-chpl-lib.cpp"
    break;

  case 425: /* opt_formal_intent_tag: required_intent_tag  */
#line 2668 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8727 "bison-chpl-lib.cpp"
    break;

  case 426: /* required_intent_tag: TIN  */
#line 2675 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::IN);            }
#line 8733 "bison-chpl-lib.cpp"
    break;

  case 427: /* required_intent_tag: TINOUT  */
#line 2676 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::INOUT);         }
#line 8739 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TOUT  */
#line 2677 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::OUT);           }
#line 8745 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TCONST TIN  */
#line 2678 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_IN);      }
#line 8751 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TCONST TREF  */
#line 2679 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);     }
#line 8757 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TCONST  */
#line 2680 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);         }
#line 8763 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TPARAM  */
#line 2681 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);         }
#line 8769 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TREF  */
#line 2682 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);           }
#line 8775 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TTYPE  */
#line 2683 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);          }
#line 8781 "bison-chpl-lib.cpp"
    break;

  case 435: /* required_intent_tag: TVAR  */
#line 2684 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR); }
#line 8787 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_this_intent_tag: %empty  */
#line 2688 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);     }
#line 8793 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: TPARAM  */
#line 2689 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);              }
#line 8799 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TREF  */
#line 2690 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);                }
#line 8805 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TCONST TREF  */
#line 2691 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);          }
#line 8811 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TCONST  */
#line 2692 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);              }
#line 8817 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TTYPE  */
#line 2693 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);               }
#line 8823 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TIN  */
#line 2694 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::IN);       }
#line 8829 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_this_intent_tag: TCONST TIN  */
#line 2695 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::CONST_IN); }
#line 8835 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_this_intent_tag: TVAR  */
#line 2696 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR);      }
#line 8841 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_this_intent_tag: TOUT  */
#line 2697 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::OUT);      }
#line 8847 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_this_intent_tag: TINOUT  */
#line 2698 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::INOUT);    }
#line 8853 "bison-chpl-lib.cpp"
    break;

  case 447: /* proc_iter_or_op: TPROC  */
#line 2702 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8859 "bison-chpl-lib.cpp"
    break;

  case 448: /* proc_iter_or_op: TITER  */
#line 2703 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8865 "bison-chpl-lib.cpp"
    break;

  case 449: /* proc_iter_or_op: TOPERATOR  */
#line 2704 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8871 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: %empty  */
#line 2708 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::DEFAULT_RETURN_INTENT); }
#line 8877 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TOUT  */
#line 2709 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::OUT);                   }
#line 8883 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TCONST  */
#line 2710 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST);                 }
#line 8889 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_ret_tag: TCONST TREF  */
#line 2711 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST_REF);             }
#line 8895 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: TREF  */
#line 2712 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::REF);                   }
#line 8901 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: TPARAM  */
#line 2713 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::PARAM);                 }
#line 8907 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: TTYPE  */
#line 2714 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::TYPE);                  }
#line 8913 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_ret_tag: TIN  */
#line 2715 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::IN);            }
#line 8919 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_ret_tag: TCONST TIN  */
#line 2716 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::CONST_IN);      }
#line 8925 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_ret_tag: TVAR  */
#line 2717 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::VAR);           }
#line 8931 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_ret_tag: TINOUT  */
#line 2718 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::INOUT);         }
#line 8937 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_throws_error: %empty  */
#line 2722 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8943 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_throws_error: TTHROWS  */
#line 2723 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8949 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_function_body_stmt: TSEMI  */
#line 2726 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8955 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_function_body_stmt: function_body_stmt  */
#line 2727 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8961 "bison-chpl-lib.cpp"
    break;

  case 465: /* function_body_stmt: block_stmt_body  */
#line 2731 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8967 "bison-chpl-lib.cpp"
    break;

  case 466: /* function_body_stmt: TDO toplevel_stmt  */
#line 2732 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 8973 "bison-chpl-lib.cpp"
    break;

  case 467: /* query_expr: TQUERIEDIDENT  */
#line 2736 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8979 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_lifetime_where: %empty  */
#line 2741 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8985 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_lifetime_where: TWHERE expr  */
#line 2743 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8991 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2745 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8997 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2747 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9003 "bison-chpl-lib.cpp"
    break;

  case 472: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2749 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9009 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_components_expr: lifetime_expr  */
#line 2754 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9015 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2756 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9021 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2761 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9027 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2763 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9033 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2765 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9039 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2767 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9045 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2769 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9051 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2771 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9057 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2773 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9063 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_ident: TIDENT  */
#line 2777 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9069 "bison-chpl-lib.cpp"
    break;

  case 483: /* lifetime_ident: TINIT  */
#line 2778 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9075 "bison-chpl-lib.cpp"
    break;

  case 484: /* lifetime_ident: TTHIS  */
#line 2779 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9081 "bison-chpl-lib.cpp"
    break;

  case 485: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2784 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9090 "bison-chpl-lib.cpp"
    break;

  case 486: /* type_alias_decl_stmt_start: TTYPE  */
#line 2793 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9098 "bison-chpl-lib.cpp"
    break;

  case 487: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2797 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9107 "bison-chpl-lib.cpp"
    break;

  case 488: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2805 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9115 "bison-chpl-lib.cpp"
    break;

  case 489: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2809 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9123 "bison-chpl-lib.cpp"
    break;

  case 490: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2816 "chpl.ypp"
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
#line 9146 "bison-chpl-lib.cpp"
    break;

  case 491: /* opt_init_type: %empty  */
#line 2837 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9152 "bison-chpl-lib.cpp"
    break;

  case 492: /* opt_init_type: TASSIGN expr  */
#line 2839 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9158 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_type: TPARAM  */
#line 2843 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9164 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_type: TCONST TREF  */
#line 2844 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9170 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_type: TREF  */
#line 2845 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9176 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_type: TCONST  */
#line 2846 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9182 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_type: TVAR  */
#line 2847 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9188 "bison-chpl-lib.cpp"
    break;

  case 498: /* $@11: %empty  */
#line 2852 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9197 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2856 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9206 "bison-chpl-lib.cpp"
    break;

  case 500: /* $@12: %empty  */
#line 2861 "chpl.ypp"
  {
    // Use a mid-rule action to preserve the 'on' expr
    context->noteVarDestinationExpr((yyvsp[0].expr));
  }
#line 9215 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt: TON expr $@12 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2866 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9224 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2871 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9233 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt: var_decl_type error TSEMI  */
#line 2876 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    context->resetDeclStateOnError();
  }
#line 9242 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2884 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9250 "bison-chpl-lib.cpp"
    break;

  case 505: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2888 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9258 "bison-chpl-lib.cpp"
    break;

  case 506: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2895 "chpl.ypp"
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
#line 9281 "bison-chpl-lib.cpp"
    break;

  case 507: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2914 "chpl.ypp"
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
#line 9301 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_var_decl_component: ident_def  */
#line 2933 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9309 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2937 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9317 "bison-chpl-lib.cpp"
    break;

  case 510: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2944 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9323 "bison-chpl-lib.cpp"
    break;

  case 511: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2946 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9329 "bison-chpl-lib.cpp"
    break;

  case 512: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2948 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9335 "bison-chpl-lib.cpp"
    break;

  case 513: /* opt_init_expr: %empty  */
#line 2954 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9341 "bison-chpl-lib.cpp"
    break;

  case 514: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2955 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9347 "bison-chpl-lib.cpp"
    break;

  case 515: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2956 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9353 "bison-chpl-lib.cpp"
    break;

  case 516: /* formal_or_ret_type_expr: expr  */
#line 2960 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9359 "bison-chpl-lib.cpp"
    break;

  case 517: /* ret_type: formal_or_ret_type_expr  */
#line 2964 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9365 "bison-chpl-lib.cpp"
    break;

  case 518: /* ret_type: reserved_type_ident_use  */
#line 2965 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9371 "bison-chpl-lib.cpp"
    break;

  case 519: /* ret_type: error  */
#line 2966 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9377 "bison-chpl-lib.cpp"
    break;

  case 520: /* colon_ret_type: TCOLON ret_type  */
#line 2970 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9383 "bison-chpl-lib.cpp"
    break;

  case 521: /* colon_ret_type: error  */
#line 2971 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9389 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_ret_type: %empty  */
#line 2975 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9395 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_type: %empty  */
#line 2980 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9401 "bison-chpl-lib.cpp"
    break;

  case 525: /* opt_type: TCOLON expr  */
#line 2981 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9407 "bison-chpl-lib.cpp"
    break;

  case 526: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2982 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9413 "bison-chpl-lib.cpp"
    break;

  case 527: /* opt_type: error  */
#line 2983 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9419 "bison-chpl-lib.cpp"
    break;

  case 528: /* formal_type: formal_or_ret_type_expr  */
#line 2987 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9425 "bison-chpl-lib.cpp"
    break;

  case 529: /* formal_type: reserved_type_ident_use  */
#line 2988 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9431 "bison-chpl-lib.cpp"
    break;

  case 530: /* colon_formal_type: TCOLON formal_type  */
#line 2992 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9437 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_colon_formal_type: %empty  */
#line 2996 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9443 "bison-chpl-lib.cpp"
    break;

  case 532: /* opt_colon_formal_type: colon_formal_type  */
#line 2997 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9449 "bison-chpl-lib.cpp"
    break;

  case 533: /* expr_ls: expr  */
#line 3003 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9455 "bison-chpl-lib.cpp"
    break;

  case 534: /* expr_ls: expr_ls TCOMMA expr  */
#line 3004 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9461 "bison-chpl-lib.cpp"
    break;

  case 535: /* tuple_component: opt_try_expr  */
#line 3008 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9467 "bison-chpl-lib.cpp"
    break;

  case 536: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3013 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9475 "bison-chpl-lib.cpp"
    break;

  case 537: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3017 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9483 "bison-chpl-lib.cpp"
    break;

  case 538: /* opt_actual_ls: %empty  */
#line 3023 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9489 "bison-chpl-lib.cpp"
    break;

  case 539: /* opt_actual_ls: actual_ls  */
#line 3024 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9495 "bison-chpl-lib.cpp"
    break;

  case 540: /* actual_ls: actual_expr  */
#line 3029 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9504 "bison-chpl-lib.cpp"
    break;

  case 541: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3034 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9514 "bison-chpl-lib.cpp"
    break;

  case 542: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3042 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9520 "bison-chpl-lib.cpp"
    break;

  case 543: /* actual_expr: opt_try_expr  */
#line 3043 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9526 "bison-chpl-lib.cpp"
    break;

  case 544: /* ident_expr: ident_use  */
#line 3047 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9532 "bison-chpl-lib.cpp"
    break;

  case 545: /* ident_expr: scalar_type  */
#line 3048 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9538 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TSINGLE expr  */
#line 3061 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9544 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3063 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9550 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3065 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9556 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3067 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9562 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3069 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9572 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TATOMIC expr  */
#line 3075 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9578 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TSYNC expr  */
#line 3077 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9584 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TOWNED  */
#line 3080 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9590 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TOWNED expr  */
#line 3082 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9596 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TUNMANAGED  */
#line 3084 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9602 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3086 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9608 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TSHARED  */
#line 3088 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9614 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TSHARED expr  */
#line 3090 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9620 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TBORROWED  */
#line 3092 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9626 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TBORROWED expr  */
#line 3094 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9632 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TCLASS  */
#line 3096 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9638 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TRECORD  */
#line 3098 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9644 "bison-chpl-lib.cpp"
    break;

  case 568: /* for_expr: no_loop_attributes for_expr_base  */
#line 3102 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9650 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3107 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9658 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3111 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9666 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3115 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9674 "bison-chpl-lib.cpp"
    break;

  case 572: /* for_expr_base: for_loop_kw zippered_iterator TDO expr  */
#line 3119 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "expression-level loops with 'zip' must have an index.");
  }
#line 9682 "bison-chpl-lib.cpp"
    break;

  case 573: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3123 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9695 "bison-chpl-lib.cpp"
    break;

  case 574: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3132 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9708 "bison-chpl-lib.cpp"
    break;

  case 575: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3141 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9721 "bison-chpl-lib.cpp"
    break;

  case 576: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3152 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9727 "bison-chpl-lib.cpp"
    break;

  case 577: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3157 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9735 "bison-chpl-lib.cpp"
    break;

  case 578: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3161 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9743 "bison-chpl-lib.cpp"
    break;

  case 579: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3165 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9751 "bison-chpl-lib.cpp"
    break;

  case 580: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3169 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9759 "bison-chpl-lib.cpp"
    break;

  case 581: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3173 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9767 "bison-chpl-lib.cpp"
    break;

  case 582: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3177 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9775 "bison-chpl-lib.cpp"
    break;

  case 583: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3181 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9783 "bison-chpl-lib.cpp"
    break;

  case 584: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3188 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9797 "bison-chpl-lib.cpp"
    break;

  case 585: /* nil_expr: TNIL  */
#line 3205 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9803 "bison-chpl-lib.cpp"
    break;

  case 593: /* opt_task_intent_ls: %empty  */
#line 3223 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9809 "bison-chpl-lib.cpp"
    break;

  case 594: /* opt_task_intent_ls: task_intent_clause  */
#line 3224 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9815 "bison-chpl-lib.cpp"
    break;

  case 595: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3229 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9823 "bison-chpl-lib.cpp"
    break;

  case 596: /* task_intent_clause: TWITH TLP TRP  */
#line 3233 "chpl.ypp"
  {
    auto err = context->syntax((yyloc), "'with' clause cannot be empty.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-2]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9834 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_intent_clause: TWITH TLP error TRP  */
#line 3240 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[-2]), "invalid intent expression in 'with' clause.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9845 "bison-chpl-lib.cpp"
    break;

  case 598: /* task_intent_clause: TWITH error  */
#line 3247 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[0]), "missing parentheses around 'with' clause intents.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-1]), dummyLoc, dummyLoc, dummyLoc, exprs);
  }
#line 9856 "bison-chpl-lib.cpp"
    break;

  case 599: /* task_intent_ls: intent_expr  */
#line 3256 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9862 "bison-chpl-lib.cpp"
    break;

  case 600: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3257 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9868 "bison-chpl-lib.cpp"
    break;

  case 601: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3262 "chpl.ypp"
  {
    (yyval.expr) = context->buildTaskIntent((yyloc), (yylsp[-2]), nullptr, (yyvsp[-2].expr), (yyvsp[-3].taskIntent), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9876 "bison-chpl-lib.cpp"
    break;

  case 602: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3266 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9884 "bison-chpl-lib.cpp"
    break;

  case 603: /* intent_expr: expr TREDUCE ident_expr  */
#line 3270 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9892 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_var_prefix: TCONST  */
#line 3276 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST);          }
#line 9898 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_var_prefix: TIN  */
#line 3277 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::IN);             }
#line 9904 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_var_prefix: TCONST TIN  */
#line 3278 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_IN);       }
#line 9910 "bison-chpl-lib.cpp"
    break;

  case 607: /* task_var_prefix: TREF  */
#line 3279 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::REF);            }
#line 9916 "bison-chpl-lib.cpp"
    break;

  case 608: /* task_var_prefix: TCONST TREF  */
#line 3280 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_REF);      }
#line 9922 "bison-chpl-lib.cpp"
    break;

  case 609: /* task_var_prefix: TVAR  */
#line 3281 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::VAR);            }
#line 9928 "bison-chpl-lib.cpp"
    break;

  case 610: /* task_var_prefix: TOUT  */
#line 3282 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::OUT);   }
#line 9934 "bison-chpl-lib.cpp"
    break;

  case 611: /* task_var_prefix: TINOUT  */
#line 3283 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::INOUT); }
#line 9940 "bison-chpl-lib.cpp"
    break;

  case 612: /* task_var_prefix: TPARAM  */
#line 3284 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::PARAM); }
#line 9946 "bison-chpl-lib.cpp"
    break;

  case 613: /* task_var_prefix: TTYPE  */
#line 3285 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::TYPE);  }
#line 9952 "bison-chpl-lib.cpp"
    break;

  case 614: /* new_maybe_decorated: TNEW  */
#line 3290 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9958 "bison-chpl-lib.cpp"
    break;

  case 615: /* new_maybe_decorated: TNEW TOWNED  */
#line 3292 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9964 "bison-chpl-lib.cpp"
    break;

  case 616: /* new_maybe_decorated: TNEW TSHARED  */
#line 3294 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9970 "bison-chpl-lib.cpp"
    break;

  case 617: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3296 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9976 "bison-chpl-lib.cpp"
    break;

  case 618: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3298 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9982 "bison-chpl-lib.cpp"
    break;

  case 619: /* new_expr: new_maybe_decorated expr  */
#line 3304 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 9990 "bison-chpl-lib.cpp"
    break;

  case 620: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3311 "chpl.ypp"
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
#line 10006 "bison-chpl-lib.cpp"
    break;

  case 621: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3323 "chpl.ypp"
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
#line 10022 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3335 "chpl.ypp"
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
#line 10039 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3348 "chpl.ypp"
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
#line 10057 "bison-chpl-lib.cpp"
    break;

  case 624: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3365 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10065 "bison-chpl-lib.cpp"
    break;

  case 625: /* range_literal_expr: expr TDOTDOT expr  */
#line 3372 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10074 "bison-chpl-lib.cpp"
    break;

  case 626: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3377 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10083 "bison-chpl-lib.cpp"
    break;

  case 627: /* range_literal_expr: expr TDOTDOT  */
#line 3382 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10092 "bison-chpl-lib.cpp"
    break;

  case 628: /* range_literal_expr: TDOTDOT expr  */
#line 3387 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10102 "bison-chpl-lib.cpp"
    break;

  case 629: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3393 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10112 "bison-chpl-lib.cpp"
    break;

  case 630: /* range_literal_expr: TDOTDOT  */
#line 3399 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10122 "bison-chpl-lib.cpp"
    break;

  case 631: /* cast_expr: expr TCOLON expr  */
#line 3429 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10130 "bison-chpl-lib.cpp"
    break;

  case 632: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3436 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10138 "bison-chpl-lib.cpp"
    break;

  case 633: /* super_expr: fn_expr  */
#line 3442 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10144 "bison-chpl-lib.cpp"
    break;

  case 637: /* expr: sub_type_level_expr TQUESTION  */
#line 3451 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10150 "bison-chpl-lib.cpp"
    break;

  case 638: /* expr: TQUESTION  */
#line 3453 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10156 "bison-chpl-lib.cpp"
    break;

  case 642: /* expr: fn_type  */
#line 3458 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10162 "bison-chpl-lib.cpp"
    break;

  case 652: /* opt_expr: %empty  */
#line 3472 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10168 "bison-chpl-lib.cpp"
    break;

  case 653: /* opt_expr: expr  */
#line 3473 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10174 "bison-chpl-lib.cpp"
    break;

  case 654: /* opt_try_expr: TTRY expr  */
#line 3477 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10180 "bison-chpl-lib.cpp"
    break;

  case 655: /* opt_try_expr: TTRYBANG expr  */
#line 3478 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10186 "bison-chpl-lib.cpp"
    break;

  case 656: /* opt_try_expr: super_expr  */
#line 3479 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10192 "bison-chpl-lib.cpp"
    break;

  case 662: /* call_base_expr: expr TBANG  */
#line 3496 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10200 "bison-chpl-lib.cpp"
    break;

  case 663: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3499 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10206 "bison-chpl-lib.cpp"
    break;

  case 666: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3506 "chpl.ypp"
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
#line 10222 "bison-chpl-lib.cpp"
    break;

  case 667: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3518 "chpl.ypp"
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
#line 10238 "bison-chpl-lib.cpp"
    break;

  case 668: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3530 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10246 "bison-chpl-lib.cpp"
    break;

  case 669: /* dot_expr: expr TDOT ident_use  */
#line 3537 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10252 "bison-chpl-lib.cpp"
    break;

  case 670: /* dot_expr: expr TDOT TTYPE  */
#line 3539 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10258 "bison-chpl-lib.cpp"
    break;

  case 671: /* dot_expr: expr TDOT TDOMAIN  */
#line 3541 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10264 "bison-chpl-lib.cpp"
    break;

  case 672: /* dot_expr: expr TDOT TLOCALE  */
#line 3543 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10270 "bison-chpl-lib.cpp"
    break;

  case 673: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3545 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10276 "bison-chpl-lib.cpp"
    break;

  case 674: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3547 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10282 "bison-chpl-lib.cpp"
    break;

  case 675: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3556 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenthLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10292 "bison-chpl-lib.cpp"
    break;

  case 676: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3562 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10300 "bison-chpl-lib.cpp"
    break;

  case 677: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3566 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10308 "bison-chpl-lib.cpp"
    break;

  case 678: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3570 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10316 "bison-chpl-lib.cpp"
    break;

  case 679: /* bool_literal: TFALSE  */
#line 3576 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10322 "bison-chpl-lib.cpp"
    break;

  case 680: /* bool_literal: TTRUE  */
#line 3577 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10328 "bison-chpl-lib.cpp"
    break;

  case 681: /* str_bytes_literal: STRINGLITERAL  */
#line 3581 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10334 "bison-chpl-lib.cpp"
    break;

  case 682: /* str_bytes_literal: BYTESLITERAL  */
#line 3582 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10340 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: INTLITERAL  */
#line 3588 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10346 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: REALLITERAL  */
#line 3589 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10352 "bison-chpl-lib.cpp"
    break;

  case 687: /* literal_expr: IMAGLITERAL  */
#line 3590 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10358 "bison-chpl-lib.cpp"
    break;

  case 688: /* literal_expr: CSTRINGLITERAL  */
#line 3591 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10364 "bison-chpl-lib.cpp"
    break;

  case 689: /* literal_expr: TNONE  */
#line 3592 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10370 "bison-chpl-lib.cpp"
    break;

  case 690: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3594 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10379 "bison-chpl-lib.cpp"
    break;

  case 691: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3599 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10388 "bison-chpl-lib.cpp"
    break;

  case 692: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3604 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10397 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3609 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10406 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3614 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10415 "bison-chpl-lib.cpp"
    break;

  case 695: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3619 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10424 "bison-chpl-lib.cpp"
    break;

  case 696: /* assoc_expr_ls: expr TALIAS expr  */
#line 3628 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10433 "bison-chpl-lib.cpp"
    break;

  case 697: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3633 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10443 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TPLUS expr  */
#line 3641 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10449 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TMINUS expr  */
#line 3642 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10455 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TSTAR expr  */
#line 3643 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10461 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TDIVIDE expr  */
#line 3644 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10467 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3645 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10473 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3646 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10479 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TMOD expr  */
#line 3647 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10485 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TEQUAL expr  */
#line 3648 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10491 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3649 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10497 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3650 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10503 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3651 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10509 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TLESS expr  */
#line 3652 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10515 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TGREATER expr  */
#line 3653 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10521 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TBAND expr  */
#line 3654 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10527 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TBOR expr  */
#line 3655 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10533 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TBXOR expr  */
#line 3656 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10539 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TAND expr  */
#line 3657 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10545 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TOR expr  */
#line 3658 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10551 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TEXP expr  */
#line 3659 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10557 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TBY expr  */
#line 3660 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10563 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TALIGN expr  */
#line 3661 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10569 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr THASH expr  */
#line 3662 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10575 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TDMAPPED expr  */
#line 3663 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10581 "bison-chpl-lib.cpp"
    break;

  case 721: /* unary_op_expr: TPLUS expr  */
#line 3667 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10587 "bison-chpl-lib.cpp"
    break;

  case 722: /* unary_op_expr: TMINUS expr  */
#line 3668 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10593 "bison-chpl-lib.cpp"
    break;

  case 723: /* unary_op_expr: TMINUSMINUS expr  */
#line 3669 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10599 "bison-chpl-lib.cpp"
    break;

  case 724: /* unary_op_expr: TPLUSPLUS expr  */
#line 3670 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10605 "bison-chpl-lib.cpp"
    break;

  case 725: /* unary_op_expr: TBANG expr  */
#line 3671 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10611 "bison-chpl-lib.cpp"
    break;

  case 726: /* unary_op_expr: expr TBANG  */
#line 3672 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10619 "bison-chpl-lib.cpp"
    break;

  case 727: /* unary_op_expr: TBNOT expr  */
#line 3675 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10625 "bison-chpl-lib.cpp"
    break;

  case 728: /* reduce_expr: expr TREDUCE expr  */
#line 3680 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10633 "bison-chpl-lib.cpp"
    break;

  case 729: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3684 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10641 "bison-chpl-lib.cpp"
    break;

  case 730: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3688 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10649 "bison-chpl-lib.cpp"
    break;

  case 731: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3692 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10657 "bison-chpl-lib.cpp"
    break;

  case 732: /* scan_expr: expr TSCAN expr  */
#line 3699 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10665 "bison-chpl-lib.cpp"
    break;

  case 733: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3703 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10673 "bison-chpl-lib.cpp"
    break;

  case 734: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3707 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10681 "bison-chpl-lib.cpp"
    break;

  case 735: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3711 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10689 "bison-chpl-lib.cpp"
    break;


#line 10693 "bison-chpl-lib.cpp"

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
