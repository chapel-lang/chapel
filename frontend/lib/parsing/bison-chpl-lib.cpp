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
  YYSYMBOL_for_loop_kw = 241,              /* for_loop_kw  */
  YYSYMBOL_loop_stmt_base = 242,           /* loop_stmt_base  */
  YYSYMBOL_zippered_iterator = 243,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 244,                  /* if_stmt  */
  YYSYMBOL_ifvar = 245,                    /* ifvar  */
  YYSYMBOL_interface_start = 246,          /* interface_start  */
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
#define YYLAST   17990

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  204
/* YYNRULES -- Number of rules.  */
#define YYNRULES  739
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1262

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
       0,   665,   665,   669,   670,   675,   676,   684,   688,   698,
     703,   710,   711,   712,   713,   714,   715,   716,   717,   718,
     719,   737,   738,   739,   747,   756,   765,   769,   776,   784,
     785,   786,   787,   788,   789,   790,   791,   793,   813,   821,
     838,   854,   871,   888,   904,   925,   926,   927,   931,   941,
     942,   946,   950,   951,   955,   962,   970,   973,   983,   985,
     990,   996,  1001,  1010,  1014,  1035,  1052,  1069,  1087,  1088,
    1090,  1095,  1096,  1101,  1119,  1124,  1129,  1141,  1160,  1161,
    1165,  1169,  1173,  1178,  1182,  1186,  1194,  1199,  1205,  1210,
    1219,  1220,  1224,  1229,  1237,  1246,  1254,  1265,  1273,  1274,
    1275,  1276,  1277,  1278,  1279,  1280,  1281,  1282,  1283,  1284,
    1285,  1286,  1287,  1288,  1300,  1304,  1309,  1314,  1320,  1329,
    1330,  1334,  1343,  1347,  1351,  1355,  1359,  1363,  1372,  1373,
    1374,  1378,  1379,  1380,  1381,  1382,  1386,  1387,  1388,  1400,
    1401,  1402,  1407,  1408,  1409,  1410,  1411,  1412,  1413,  1414,
    1415,  1416,  1417,  1418,  1419,  1420,  1421,  1422,  1423,  1424,
    1428,  1429,  1430,  1431,  1432,  1433,  1434,  1435,  1436,  1437,
    1438,  1439,  1446,  1447,  1448,  1452,  1453,  1457,  1464,  1474,
    1478,  1484,  1484,  1490,  1490,  1499,  1500,  1501,  1502,  1503,
    1504,  1505,  1509,  1514,  1519,  1524,  1531,  1539,  1540,  1544,
    1553,  1560,  1569,  1588,  1587,  1597,  1596,  1609,  1615,  1618,
    1619,  1628,  1629,  1630,  1631,  1635,  1658,  1677,  1696,  1721,
    1726,  1731,  1736,  1741,  1746,  1751,  1756,  1761,  1766,  1771,
    1776,  1781,  1786,  1791,  1797,  1806,  1815,  1819,  1823,  1827,
    1831,  1835,  1839,  1843,  1847,  1852,  1857,  1862,  1870,  1885,
    1903,  1910,  1916,  1925,  1926,  1931,  1936,  1937,  1938,  1939,
    1940,  1941,  1942,  1943,  1944,  1945,  1946,  1947,  1952,  1957,
    1958,  1959,  1960,  1968,  1969,  1973,  1977,  1981,  1988,  1992,
    1996,  2003,  2007,  2011,  2015,  2022,  2023,  2027,  2031,  2035,
    2042,  2055,  2071,  2079,  2084,  2094,  2095,  2099,  2103,  2108,
    2117,  2122,  2126,  2133,  2134,  2138,  2148,  2154,  2166,  2173,
    2174,  2175,  2179,  2180,  2184,  2188,  2192,  2196,  2200,  2208,
    2227,  2240,  2247,  2252,  2259,  2258,  2268,  2274,  2273,  2288,
    2296,  2309,  2327,  2324,  2353,  2357,  2358,  2360,  2365,  2366,
    2370,  2371,  2375,  2378,  2380,  2385,  2386,  2397,  2418,  2417,
    2433,  2432,  2450,  2460,  2457,  2485,  2494,  2503,  2513,  2523,
    2532,  2547,  2548,  2556,  2557,  2558,  2567,  2568,  2569,  2570,
    2571,  2572,  2573,  2574,  2575,  2576,  2577,  2578,  2579,  2580,
    2581,  2582,  2583,  2584,  2585,  2586,  2587,  2588,  2589,  2590,
    2591,  2595,  2596,  2597,  2598,  2599,  2600,  2601,  2602,  2603,
    2604,  2605,  2606,  2611,  2612,  2616,  2617,  2621,  2622,  2626,
    2627,  2631,  2632,  2636,  2637,  2641,  2645,  2649,  2654,  2659,
    2664,  2669,  2677,  2681,  2689,  2690,  2691,  2692,  2693,  2694,
    2695,  2696,  2697,  2698,  2702,  2703,  2704,  2705,  2706,  2707,
    2708,  2709,  2710,  2711,  2712,  2716,  2717,  2718,  2722,  2723,
    2724,  2725,  2726,  2727,  2728,  2729,  2730,  2731,  2732,  2736,
    2737,  2740,  2741,  2745,  2746,  2750,  2755,  2756,  2758,  2760,
    2762,  2767,  2769,  2774,  2776,  2778,  2780,  2782,  2784,  2786,
    2791,  2792,  2793,  2797,  2806,  2810,  2818,  2822,  2829,  2851,
    2852,  2857,  2858,  2859,  2860,  2861,  2866,  2865,  2875,  2874,
    2884,  2889,  2897,  2901,  2908,  2927,  2946,  2950,  2957,  2959,
    2961,  2968,  2969,  2970,  2974,  2978,  2979,  2980,  2984,  2985,
    2989,  2990,  2994,  2995,  2996,  2997,  3001,  3002,  3006,  3010,
    3011,  3017,  3018,  3022,  3026,  3030,  3037,  3038,  3042,  3047,
    3056,  3057,  3061,  3062,  3069,  3070,  3071,  3072,  3073,  3074,
    3076,  3078,  3080,  3086,  3088,  3091,  3093,  3095,  3097,  3099,
    3101,  3103,  3105,  3107,  3109,  3114,  3118,  3122,  3126,  3130,
    3134,  3143,  3152,  3164,  3168,  3172,  3176,  3180,  3184,  3188,
    3192,  3199,  3217,  3225,  3226,  3227,  3228,  3229,  3230,  3231,
    3235,  3236,  3240,  3244,  3251,  3258,  3268,  3269,  3273,  3277,
    3281,  3288,  3289,  3290,  3291,  3292,  3293,  3294,  3295,  3296,
    3297,  3301,  3303,  3305,  3307,  3309,  3315,  3322,  3335,  3348,
    3362,  3380,  3387,  3392,  3397,  3402,  3408,  3414,  3444,  3451,
    3458,  3459,  3463,  3465,  3466,  3468,  3470,  3471,  3472,  3473,
    3476,  3477,  3478,  3479,  3480,  3481,  3482,  3483,  3484,  3488,
    3489,  3493,  3494,  3495,  3499,  3500,  3501,  3502,  3511,  3512,
    3515,  3516,  3517,  3521,  3533,  3545,  3552,  3554,  3556,  3558,
    3560,  3562,  3571,  3577,  3581,  3585,  3592,  3593,  3597,  3598,
    3602,  3603,  3604,  3605,  3606,  3607,  3608,  3609,  3614,  3619,
    3624,  3629,  3634,  3643,  3648,  3657,  3658,  3659,  3660,  3661,
    3662,  3663,  3664,  3665,  3666,  3667,  3668,  3669,  3670,  3671,
    3672,  3673,  3674,  3675,  3676,  3677,  3678,  3679,  3683,  3684,
    3685,  3686,  3687,  3688,  3691,  3695,  3699,  3703,  3707,  3714,
    3718,  3722,  3726,  3734,  3735,  3736,  3737,  3738,  3739,  3740
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
  "TSCAN", "TSELECT", "TSERIAL", "TSHARED", "TSPARSE", "TSTRING",
  "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS", "TTHROW", "TTHROWS", "TTRUE",
  "TTRY", "TTRYBANG", "TTYPE", "TUINT", "TUNION", "TUNMANAGED", "TUSE",
  "TVAR", "TVOID", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP",
  "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR",
  "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR",
  "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY", "TASSIGNPLUS",
  "TASSIGNREDUCE", "TASSIGNSL", "TASSIGNSR", "TATMARK", "TBANG", "TBAND",
  "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT",
  "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL",
  "THASH", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOTEQUAL", "TOR",
  "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR",
  "TSWAP", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR", "TRSBR", "TNOELSE",
  "TDOTDOTOPENHIGH", "TUPLUS", "TUMINUS", "TLNOT", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt", "stmt_base",
  "tryable_stmt", "decl_base", "collect_attributes", "attribute_receiver",
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
  "interface_start", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "try_stmt", "catch_expr_ls",
  "catch_expr", "catch_expr_inner", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "manager_expr", "manager_expr_ls",
  "manage_stmt", "class_decl_stmt", "class_start", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt", "enum_header",
  "enum_ls", "$@5", "$@6", "enum_item", "lambda_decl_start",
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

#define YYPACT_NINF (-920)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-740)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -920,    71,  4152,  -920,   -80,    50,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920, 11949,     3,    81,    96, 13799,   106,
   17756,     3,   113,   167,   241,    81,  5552, 11949,   199, 17815,
    -920,   367,   346,  -920, 10045,   351,   174,   275,  -920,   362,
     248, 17815, 17815, 17815,  -920,  2814, 10908,   397, 11949, 11949,
     237,  -920,   420,   446, 11949,  -920, 13799,  -920, 11949,   503,
     347,    20,   206, 13153,   477, 17874,  -920, 11949,  8150, 11949,
   10908, 13799,   436,   491,   385,  5552,   511, 11949,   528,  7118,
    7118,  -920,   533,  -920, 13799,  -920,   542, 10217,  -920, 11949,
    -920, 11949,  -920,  -920, 13627, 11949,  -920, 10389,  -920,  -920,
    -920,  4502,  8322, 11949,  -920,  5202,  -920,  -920,  -920,  -920,
   17443,   555,  -920,   449,   433,  -920,    33,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    7462,  -920,  7634,  -920,  -920,   108,  -920,  -920,   -68,  -920,
     554,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,   469,
   17815,  -920,   447,   450,   344,  -920,   310,  -920,  -920,  -920,
     207,   817,  -920,  -920, 17815,  1920,  -920,   453,   457,  -920,
    -920,  -920,   454,   466, 11949,   468,   470,  -920,  -920,  -920,
   17222,  1221,   224,   472,   473,  -920,  -920,   309,  -920,  -920,
    -920,  -920,  -920,   358,  -920,  -920,  -920, 11949,  -920, 17815,
    -920, 11949, 11949,    23,   577,   328,  -920,  -920,  -920,  -920,
   17222,   384,  -920,  -920,    40,  5552,  -920,  -920,  -920,   475,
     161,   471,  -920,    72,  -920,   479,  -920,   177, 17222,  8494,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920, 17815, 17815,
      26, 14288,  -920,  -920,   553,  8494,   482,   478,  -920,  5552,
    3648,    78,     8,  -920,  5552,  -920,  -920, 15271,  -920,    18,
   15680,   875,  -920,   480,   485,  -920, 15271,   161,   875,  -920,
    8494,  1445,  1445,    62,     0,  -920,     4,  -920,  -920,  -920,
    -920,  7806,  -920,  -920,   663,  -920,   476,   495,  -920,  -920,
    3932,   505,  -920,  -920, 17222,   247, 11949, 11949,  -920,   -49,
    -920,  -920, 17222,   493, 15799,  -920, 15271,   161,   487,  8494,
    -920, 17222, 15839, 11949,  -920,  -920,  -920,  -920,  -920,   161,
     497,   228,   228,  1282,   875,   875,    28,  -920,  -920,  4677,
      19, 11949,   513,   -42,   492,  1282,   655,  -920,  -920, 17815,
    -920, 11949,  -920,  4502,  -920,   265,  -920,   553,  -920,  -920,
     664,   494,  4852, 11949,  -920, 11949,   601,  -920,  -920, 14942,
      27,   469, 17222,   280,  -920,  5552,   589,  -920,  -920, 10045,
   10561, 11080,  -920,  -920,  -920, 17815,  -920, 17815, 11949,   500,
    -920, 17531,   340,   207,  -920,  -920,  -920,  -920,    77,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,    80,   196,  -920,  -920,
    -920,  -920,  -920,  -920,  -920, 13460,   546,   266,  -920,   508,
     293,   402,  -920,   526, 11949, 11949, 11949, 11080, 11080, 11949,
     438, 11949, 11949, 11949, 11949, 11949,   520, 13627, 11949, 11949,
   11949, 11949, 11949, 11949, 11949, 11949, 11949, 11949, 11949, 11949,
   11949, 11949, 11949, 11949,   608,  -920,  -920,  -920,  -920,  -920,
   10217, 10217,  -920,  -920,  -920,  -920, 10217,  -920,  -920, 10217,
   10217,  8494,  8666, 11080, 11080, 15879,   507,  -920, 11252, 11080,
   17815,  -920,  6599,  -920,  -920,  -920,  2814,  -920, 11949,  -920,
     556,   512,   548,  -920,  -920,   563,   568,  5552,   667,  5552,
    -920,   668, 11949,   631,   537,  -920, 10217,  -920,  3648,  -920,
    -920,   -15,  -920, 12121,   574, 11949,  2814,  -920,  -920, 11949,
    -920, 17606, 11949, 11949,  -920,    72,   539,  -920,  -920,  -920,
    -920, 17815,  -920,   207, 13325,  3648,   567, 12121,  -920, 17222,
   17222,  -920,   471,  -920,    39,  -920,  8494,   543,  -920, 15271,
     692,   692,  -920,  -920,  -920,  -920, 11427,  -920, 15958,  8841,
    9013,  -920,  -920,  -920, 17222, 11080, 11080,  -920,   449,  9185,
    -920,   172,  -920,  5027,  -920,   294, 15998,   295, 15032, 17815,
    6943,  6771,  -920,   469,   547,  -920,    72,  -920,   605, 17815,
      13, 15271,   557, 13941,   -45,   119, 16039,   -64,    37, 14777,
    -920,   105,  -920,    98,   573,  2114,   559, 17697,   602,   178,
    -920,  -920,   153,  -920,  -920,   111,  -920,  -920,  -920,  3317,
    -920,   622,  -920,  -920,   560,   582,  -920,   586,  -920,   587,
     592,   593,   595,   597,  -920,   600,   604,   607,   613,   615,
     616,   618,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920, 11949,   609,   619,   560,  -920,   560,
    -920, 11949,  -920, 17815,  -920,  -920,  -920,  3914,  3914,   376,
    -920,   376,  -920,   376,  2312,   541,   757,   460,   161,   228,
    -920,   442,  -920,  -920,  -920,  -920,  -920,  1282,   642,   376,
    1078,  1078,  3914,  1078,  1078,  1360,   228,   642,  3970,  1360,
     875,   875,   228,  1282,   606,   610,   614,   628,   633,   639,
     565,   579,  -920,   376,  -920,   376, 11949, 10217,    87,   724,
   15351,   598,   596,   203,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,   171,  -920,  1753, 17301,   444,   304, 17222, 10217,
    -920, 10217, 11949, 11949,   737,  5552,   739, 16190,  5552, 15430,
   17815,  -920,   180,   209,  3648,    78,  -920, 17222,  9357,  -920,
   17222,  -920,  -920,  -920, 17815, 16230, 16270,  2814,  -920,   567,
     635,  -920,   226, 12121,  -920,   158,  -920, 11949,  -920,   636,
     -31,   638,  -920,  2194,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,   154,    58, 14851,  -920,  -920,   231,  -920,  -920,
    -920,  -920, 11949,  -920,    73, 14046, 11949,  -920, 11949,  6943,
    6771,  -920,  9529,   452,  9701,   455,   458,  9873,  7978,   474,
     305,  -920,  -920,  -920, 16349,   659,   647,   645,  -920, 17815,
    2814, 11949,   769,  -920,  -920, 11949, 17222,  5552,   643, 11080,
   11599,  4327,   650, 11949, 11774,  -920,  5552,  -920,    13,  5727,
   11080,  -920,    13, 17815,   471,  -920, 10217,   649,  1962,  -920,
    -920, 11949,    48,  -920,   994,  -920,   622,  -920,  -920,  -920,
    -920,  -920,     6,   386,  -920, 16389,  -920, 14205,  -920,  -920,
   17222,  -920,   653,   654,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,   254, 11080, 13941, 11949, 12293, 11080, 10217,  -920,
    -920,  -920,  7290,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,    35, 11080, 11080,  -920,  -920,
    -920, 17222, 17222,  5552,  -920,  5552, 11949,  -920,  5552,   787,
     665,   669,  -920,  -920,   574,  -920,  -920,  -920,   661,   666,
     315,   158, 11949,   622,   567,  -920, 10736,  -920,  -920, 17222,
    -920,  5552, 11949,  -920,  -920,  -920, 17815,  -920,   693,   471,
   11080,  5552, 11080, 10217,  -920,  -920,  -920,  -920,  -920,   442,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  6943,  -920,  -920, 16437, 15521,   317,   675,
    6943,  -920, 11949,  -920,  -920,  2233,   329,   332, 16540, 11949,
     264,  -920,  5552,   -32, 15595,  -920,  -920, 17222,  5552, 17222,
    -920, 16588, 17222,  -920, 10045, 17222,    60, 15111,  -920,  -920,
    -920,   271,  -920,  -920,  -920,  -920,  3829,  1575,  -920, 17222,
   17815,   555,  -920,     6,   181, 11949,  -920,   694,   670,  -920,
     560,   560,  -920,  -920,  -920,   671, 16628, 11949,   809, 16779,
     283,  -920,   574,   288,   298,  -920,  -920, 17222,   812,  5552,
    -920,  -920,  -920,  8494,  8494,  -920,  -920,  -920,  -920,  -920,
     158, 17222,  -920,   272,    21,   676, 11949,  -920,   -30, 15640,
      58, 15191,  -920,   229, 11949,  6943,  6771,  -920,  -920,  -920,
   16819, 17222,  2478,  -920,  -920,  -920,  -920, 15271,  -920,  -920,
    5902,   678,  6077,   679,  -920, 11949, 14368,  6252,  -920,    13,
    6427,  -920,    13,   686,  1575,  -920,  -920,  -920, 17697,  -920,
    -920,  -920,   145,  -920,   -20,  -920,   205, 16860,    68,  -920,
    -920,  -920, 12465, 12637, 16975, 12809, 12981,  -920,  -920,  5552,
    -920,   683,   684,  -920,  -920,  5552,  -920,   471, 17222,  5552,
    5552,  -920, 17222,   339,   695, 11949,  -920,  -920, 10045,  -920,
   17222,  5552, 10045,  -920, 17222,  5552, 17222,  5552, 10045, 17222,
    -920, 10045, 17222,  -920,  -920,  -920,  -920,    61,  -920, 11949,
     181,   145,   145,   145,   145,   145,   145,   181,  5377,  -920,
    -920,  -920,  -920, 11949, 11949, 11949, 11949, 11949,  -920,   698,
     699,   605,  -920,  -920,  -920, 17222, 14448,  -920, 14528,  -920,
   16190, 14608, 14688,  -920, 17222,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,   711,  -920, 17023, 17063, 17142, 17182,  -920,  -920,
    5552,  5552,  5552,  5552, 11949, 11949, 11949, 11949, 16190, 16190,
   16190, 16190
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   139,   465,    55,   682,   683,
     684,   678,   679,   685,   208,   590,   128,   160,   561,   167,
     563,   590,   166,   496,   494,   128,     0,   208,   273,   161,
     198,   197,   676,   196,   208,   165,    68,   274,   336,   162,
     140,     0,     0,     0,   331,     0,   208,   169,   208,   208,
     611,   582,   686,   170,   208,   337,   555,   491,   208,     0,
       0,   183,   181,   422,   164,   564,   493,   208,   208,   208,
     208,   559,     0,   168,     0,     0,   141,   208,   677,   208,
     208,   484,   163,   311,   557,   495,   171,   208,   735,   208,
     737,   208,   738,   739,   627,   208,   736,   208,   635,   179,
     734,     0,   208,   208,     4,     0,     5,    10,    11,    12,
      48,    51,    52,    56,     0,    45,    71,    13,    77,    14,
      15,    16,    17,    29,   542,   543,    22,    46,   180,   190,
     208,   199,   649,   191,    18,     0,    31,    30,     0,    47,
       0,   268,    19,   645,    21,    35,    32,    33,   189,   312,
       0,   187,     0,     0,   642,   335,     0,   639,   185,   352,
     448,   434,   637,   188,     0,     0,   186,   654,   633,   632,
     636,   546,   544,     0,   208,   643,   644,   648,   647,   646,
       0,   545,     0,   655,   656,   657,   680,   681,   638,   548,
     547,   640,   641,     0,    28,   563,   161,   208,   140,     0,
     564,   208,   208,     0,     0,   642,   654,   544,   643,   644,
     553,   545,   655,   656,     0,     0,   591,   129,   130,     0,
     562,     0,   485,     0,   492,     0,    20,     0,   531,   208,
     136,   142,   153,   148,   147,   155,   133,   146,   156,   143,
     137,   157,   131,   158,   151,   145,   152,   149,   150,   132,
     134,   144,   154,   159,   138,   321,   135,   207,     0,     0,
       0,     0,    70,    69,    71,   208,     0,     0,   250,     0,
       0,     0,     0,   502,     0,   176,    40,     0,   303,     0,
     302,   720,   615,   612,   613,   614,   498,   556,   721,     7,
     208,   334,   334,   429,   173,   424,   174,   425,   426,   430,
     431,   172,   432,   433,   422,   527,     0,   339,   340,   342,
       0,   423,   526,   344,   514,     0,   208,   208,   177,   639,
     630,   653,   631,     0,     0,    43,     0,   560,     0,   208,
      44,   554,     0,   208,   281,    48,   285,   282,   285,   558,
       0,   722,   724,   625,   719,   718,     0,    74,    78,     0,
       0,   208,     0,     0,   533,   626,     0,     6,   309,     0,
     197,   208,   310,     0,    49,     0,     9,    71,    50,    53,
       0,    60,     0,   208,    72,   208,     0,   496,   195,     0,
     678,   312,   650,   203,   214,     0,   211,   212,   213,   208,
     208,   208,   210,   565,   573,     0,   252,     0,   208,     0,
     308,     0,   422,   448,   446,   447,   445,   360,   450,   455,
     458,   449,   453,   452,   454,   457,     0,   438,   440,   444,
     443,   435,   436,   439,   442,     0,   489,     0,   486,     0,
       0,   634,    34,   616,   208,   208,   208,   208,   208,   208,
     723,   208,   208,   208,   208,   208,     0,   624,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   391,   398,   399,   400,   395,   397,
     208,   208,   393,   396,   394,   392,   208,   402,   401,   208,
     208,   208,   208,   208,   208,     0,     0,   211,   208,   208,
       0,   595,     0,    36,    24,    37,     0,    25,   208,    38,
     542,     0,   537,   538,   541,     0,     0,     0,   241,     0,
     391,   237,   208,     0,     0,    23,   208,    26,     0,   506,
     508,     0,   525,   208,   511,   208,     0,   175,    39,   208,
     305,     0,   208,   208,    41,     0,     0,   184,   182,   427,
     428,     0,   423,   448,   422,     0,   529,   208,   121,   651,
     652,   350,     0,   178,     0,    42,   208,     0,   292,     0,
     283,   284,    27,    76,    75,    79,   208,   687,     0,   208,
     208,   674,   672,     8,   498,   208,   208,   209,    56,   208,
      54,     0,    65,     0,   119,     0,   114,     0,    86,     0,
     208,   208,   192,   312,     0,   205,     0,   202,     0,     0,
       0,     0,     0,   574,     0,     0,   531,     0,     0,     0,
     255,     0,   253,   279,   313,     0,     0,     0,   329,     0,
     322,   409,     0,   411,   415,     0,   456,   451,   519,     0,
     521,   459,   441,   437,   407,   136,   387,   142,   385,   148,
     147,   146,   143,   137,   389,   157,   158,   145,   149,   132,
     144,   159,   384,   366,   369,   367,   368,   390,   379,   370,
     383,   375,   373,   386,   374,   372,   377,   382,   371,   376,
     380,   381,   378,   388,   208,   363,     0,   407,   364,   407,
     361,   208,   488,     0,   483,   501,   500,   715,   714,   717,
     726,   725,   730,   729,   711,   708,   709,   710,   628,   698,
     139,     0,   668,   669,   141,   667,   666,   622,   702,   713,
     707,   705,   716,   706,   704,   696,   701,   703,   712,   695,
     699,   700,   697,   623,     0,     0,     0,     0,     0,     0,
       0,     0,   728,   727,   732,   731,   208,   208,     0,     0,
       0,   279,     0,   601,   602,   608,   607,   609,   604,   610,
     606,   593,     0,   596,     0,     0,     0,     0,   532,   208,
     550,   208,   208,   208,   240,     0,   236,     0,     0,     0,
       0,   549,     0,     0,   509,     0,   524,   523,   208,   504,
     621,   503,   304,   301,     0,     0,     0,     0,   665,   529,
     345,   341,     0,   208,   530,   511,   343,   208,   349,     0,
       0,     0,   551,     0,   286,   688,   629,   673,   534,   533,
     675,   535,     0,     0,     0,    57,    58,     0,    61,    63,
      67,    66,   208,    97,     0,     0,   208,    92,   208,   208,
     208,    64,   208,   366,   208,   367,   368,   208,   208,   378,
       0,   403,   404,    81,    80,    91,     0,     0,   314,     0,
       0,   208,     0,   217,   216,   208,   575,     0,     0,   208,
     208,     0,     0,   208,   208,   691,     0,   225,     0,     0,
     208,   223,     0,     0,     0,   276,   208,     0,   334,   320,
     326,   208,   324,   319,   422,   410,   459,   517,   516,   515,
     518,   460,   466,   422,   359,     0,   365,     0,   355,   356,
     490,   487,     0,     0,   127,   125,   126,   124,   123,   122,
     663,   664,     0,   208,   689,   208,   208,   208,   208,   594,
     603,   605,   208,   592,   160,   167,   166,   165,   162,   169,
     170,   164,   168,   163,   171,     0,   208,   208,   497,   540,
     539,   249,   248,     0,   243,     0,   208,   239,     0,   245,
       0,   278,   507,   510,   511,   512,   513,   300,     0,     0,
       0,   511,   208,   459,   529,   528,   208,   418,   416,   351,
     294,     0,   208,   293,   296,   552,     0,   287,   290,     0,
     208,     0,   208,   208,    59,   120,   117,    98,   109,   104,
     103,   102,   111,    99,   112,   107,   101,   108,   105,   106,
     100,   110,   113,   208,   116,   115,    88,    87,     0,     0,
     208,   193,   208,   194,   314,   334,     0,     0,     0,   208,
       0,   233,     0,     0,     0,   690,   231,   576,     0,   693,
     692,     0,   569,   226,   208,   568,     0,     0,   224,   254,
     251,     0,   307,   183,   181,   306,   334,   334,   315,   330,
       0,     0,   412,   466,     0,   208,   353,   414,     0,   362,
     407,   407,   670,   671,   278,     0,     0,   208,     0,     0,
       0,   597,   511,   654,   654,   242,   238,   581,   244,     0,
      73,   275,   505,   208,   208,   499,   419,   417,   346,   347,
     511,   406,   298,     0,     0,     0,   208,   288,     0,     0,
       0,     0,    62,     0,   208,   208,   208,    93,    95,    84,
      83,    82,   334,   200,   206,   204,   215,     0,   235,   234,
       0,     0,     0,     0,   232,   208,     0,     0,   221,     0,
       0,   219,     0,   280,   334,   317,   316,   325,     0,   332,
     480,   481,     0,   482,   468,   471,     0,   467,     0,   408,
     357,   358,   208,   208,     0,   208,   208,   280,   598,     0,
     247,     0,     0,   421,   420,     0,   297,     0,   291,     0,
       0,   118,    89,     0,     0,   208,   201,   218,   208,   229,
     578,     0,   208,   227,   577,     0,   694,     0,   208,   567,
     222,   208,   566,   220,   277,   318,   328,     0,   479,   208,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   461,
     463,   354,   462,   208,   208,   208,   208,   208,   246,   617,
     618,   299,   289,    94,    96,    85,     0,   230,     0,   228,
     572,     0,     0,   333,   470,   472,   473,   476,   477,   478,
     474,   475,   469,   464,     0,     0,     0,     0,   619,   620,
       0,     0,     0,     0,   208,   208,   208,   208,   580,   579,
     571,   570
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -920,  -920,  -920,    -1,   -57,  1840,  -920,   261,   760,  -920,
    -920,   410,  -390,   -97,  -920,   297,  -920,  -920,  -112,  -920,
    -920,    36,   612,  -920,  -544,  2918,   501,  -575,  -920,  -802,
    -920,  -920,  -920,    55,  -920,  -920,  -920,   847,  -920,  3134,
    -170,  -920,  -920,  -468,   -37,  -920,  -841,  -920,  -920,   236,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,    17,  -920,
    -145,   514,  -375,  -920,   498,  -920,  -920,  -920,     7,  1465,
    -920,  -920,  -920,  -920,   544,  -920,   -95,  -920,  -920,  -920,
    -920,   355,  -920,  -920,  -920,  -101,  -920,  -346,  -796,  -920,
    -920,  -920,  -920,  -920,  -607,  -920,   136,  -920,  -920,  -920,
    -920,  -920,   345,  -920,   107,  -920,  -920,  -920,  -920,   509,
    -920,  -920,  -920,  -920,    -9,  -422,  -179,  -787,  -919,  -654,
    -920,    -3,  -920,     9,   -51,   591,   -46,  -920,  -920,  -354,
    -826,  -920,  -306,  -920,  -157,  -307,  -292,  -609,  -920,  -920,
      63,   233,  -920,  -203,   781,  -920,  -920,  -158,   392,   147,
    -429,  -769,  -608,  -920,  -920,  -920,  -596,  -513,  -920,  -750,
     -23,    10,  -920,  -260,  -489,  -557,    -2,  -920,  -920,  -920,
    -920,  -920,  -920,   431,  -920,   898,  -340,  -920,    14,  -920,
    -920,   465,   646,  -920,  -920,  -920,  -920,  2673,  -920,   -55,
     931,  -920,  1260,  1631,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -474
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   348,   105,   106,   107,   108,   109,   365,
     366,   110,   111,   112,   113,   371,   580,   817,   818,   114,
     115,   116,   376,   117,   118,   119,   349,   845,   587,   846,
     120,   121,  1004,   584,   585,   122,   123,   219,   254,   271,
     124,   256,   125,   305,   534,   126,   127,   292,   291,   128,
     129,   130,   131,   132,   133,   596,   849,   134,   203,   136,
     391,   392,   604,   137,   260,   138,   139,   611,   612,   204,
     141,   142,   143,   144,   560,   804,   979,   145,   146,   800,
     974,   278,   279,   147,   148,   149,   150,   399,   878,   151,
     152,   619,  1050,  1051,   620,   153,   205,  1197,   155,   156,
     306,   307,   308,   963,   157,   320,   552,   797,   158,   159,
    1148,   160,   161,   676,   677,   841,   842,   843,   967,   894,
     403,   622,  1058,   623,   624,   310,   542,   425,   407,   416,
     892,  1211,  1212,   162,  1056,  1144,  1145,  1146,   163,   164,
     427,   428,   682,   165,   166,   223,   535,   272,   273,   520,
     521,   779,   312,   890,   630,   631,   524,   313,   794,   795,
     350,   352,   353,   501,   502,   503,   206,   168,   169,   393,
     170,   394,   171,   207,   173,   215,   216,   752,   753,   754,
     174,   208,   209,   177,   178,   179,   321,   180,   383,   504,
     211,   182,   212,   213,   185,   186,   187,   188,   607,   189,
     190,   191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   104,   480,   678,   227,   514,   304,   430,   798,   276,
     880,   617,   309,   323,   369,   840,   608,   311,   756,   135,
     496,   889,   819,   898,   167,   899,   968,   772,  1009,   886,
     536,   381,   340,   325,   796,   594,   522,  1048,  1005,   961,
     799,   491,  1086,   135,   315,   971,   274,   354,   384,   625,
     358,   274,  1015,  -327,   274,   776,   525,  -273,   489,   500,
    1053,  -274,   690,   692,   487,   387,   388,   -70,  1054,   557,
     866,     3,   264,   167,   551,   214,   700,   167,   167,   522,
     373,   628,   512,   864,   217,   194,   972,   -70,   214,   773,
     214,   274,   135,  1127,  1208,   500,   135,   135,  1199,   167,
      24,  1208,   363,   167,   395,   570,   -70,  -256,   732,   734,
     539,   865,   522,  -520,   739,  -295,   792,   362,   135,   374,
     500,  -348,   135,   214,  1055,   626,  -522,  1200,   198,   507,
     857,   571,   774,   384,   -70,   913,   218,  1089,   154,    83,
     973,   385,  -520,  1120,  -522,  1169,   367,   375,  1140,   386,
     387,   388,    57,  -257,   540,   526,  -295,   214,   775,   500,
    -522,   888,   154,  -263,    66,   529,   566,   859,   498,   627,
    -262,  1163,   229,  -522,  1048,   319,   265,   704,   214,   954,
     214,   523,  -522,   363,  1140,  1082,  -520,    85,   363,   790,
     567,   363,  1087,   194,   319,  -323,   363,   488,  -520,   563,
    1141,   813,   980,  -522,   940,  1135,  1136,   363,  -522,   319,
    -295,   154,   492,   167,  1090,   154,   154,  -522,  1112,  -323,
     576,   730,   731,  1109,   523,  -522,   629,   389,   363,  -522,
     363,   363,   135,  1209,   860,   408,  1141,   154,   363,   214,
     528,   154,   530,  -522,   632,  -520,   679,   847,   912,  1143,
    -520,   920,   873,   -69,  1008,   409,   282,   523,   262,   410,
     436,   263,   914,   875,   858,   862,   860,   167,   868,   872,
     876,  1048,   167,   -69,   214,  1142,   706,   222,   874,   593,
     965,  -522,   390,   778,   411,  1143,   135,   412,   633,   555,
     384,   135,   -69,  1195,   861,   921,   801,   384,   385,   413,
     884,   498,   440,  1158,  1174,  1165,   386,   387,   388,   966,
     446,   500,   500,   386,   387,   388,   283,   414,   922,   437,
     -69,  1164,   415,   438,   498,   882,   885,   761,   784,   981,
    1201,    38,   787,   224,  -334,   284,   319,   194,   757,  1072,
     334,   337,   499,   820,   923,   304,   500,   167,   565,   883,
     285,   154,   266,   951,  1088,  -334,   774,  1202,    55,  1203,
    1204,   167,  1205,  1206,   404,  -334,   135,   605,   293,   440,
     167,   229,   319,   774,   444,   614,  1010,   446,   983,   257,
     135,   449,   952,   167,   389,   405,   500,  1041,   295,   135,
     595,   389,   297,   850,   498,   406,   481,   319,   482,   964,
    1171,   761,   135,  -271,   984,   154,  1150,  1151,  -261,   500,
     154,   498,   548,   683,   293,   725,   726,   298,   761,  -258,
     299,   727,    59,   680,   728,   729,   819,  1064,  1103,  1070,
     761,   684,   300,   172,   295,  -600,   319,  1118,   297,   575,
     526,   822,   826,  1137,  1133,  -599,   390,   265,   756,   483,
     302,   526,  1010,   484,  -265,   303,  1157,   172,   686,   823,
     827,  -600,   526,   298,  1010,   738,   299,   175,    59,   938,
    1011,  -599,   862,   868,   872,   678,   683,  -269,   300,   526,
    1085,  -662,  1107,  -662,  1023,   154,  1010,   304,  1047,   335,
     335,   175,   436,   309,  1114,  1036,   302,  1115,   311,   154,
    -661,   303,  -661,  -266,  1223,   167,   172,   167,   154,  -587,
     172,   172,   289,   621,   809,   809,  -661,   440,  -661,   290,
     369,   154,   444,   700,   135,   446,   135,   537,   538,   449,
    1173,  1196,   172,  1198,  -260,   937,   172,   328,  1065,   484,
     175,   701,  1068,  -737,   175,   175,  -738,  -737,  -264,  -739,
    -738,   437,   812,  -739,   702,   438,  -658,   329,  -658,  -413,
       7,   690,   732,   853,   854,  -734,   175,   500,  -270,  -734,
     175,   867,   871,   436,  -660,   198,  -660,   319,   319,   808,
     811,   167,   565,   319,   703,  -272,   319,   319,   319,   319,
    -259,   500,  1236,  1237,  1238,  1239,  1240,  1241,   370,  -267,
     135,   440,   441,   372,  1210,  1098,   444,  1100,   445,   446,
    -659,   397,  -659,   449,   902,   398,   903,   401,  -584,  -583,
     456,   431,   402,   319,   704,  1047,   460,   461,   462,   960,
     705,   432,   437,  -588,   490,  -589,   438,  -586,  -585,   374,
     494,   363,   544,   154,   497,   154,   172,   515,   176,   543,
     516,   547,   532,  1210,   986,   706,  1134,   533,   553,   556,
     569,  1138,   562,   319,   573,   572,   579,   578,   589,   599,
     615,   681,   176,   685,   436,   446,   319,   319,   724,   737,
     175,   759,   440,  1121,  1123,   760,   319,   444,   762,   445,
     446,   293,  1017,   763,   449,   761,  1129,  1132,   770,   778,
     172,   456,   765,   768,   939,   172,   500,   460,   461,   462,
     771,   295,   788,   793,   803,   297,   802,   848,  1061,   154,
     498,   176,  1047,   956,   851,   176,   176,   881,   891,   855,
     879,  -139,   893,   437,   175,  -160,  -167,   438,   910,   175,
     298,  -166,  -165,   299,  -162,   356,  -140,   176,   500,  -169,
     896,   176,   935,  -170,   911,   300,  -164,   915,  1121,  1123,
    1129,  1132,  -168,   167,  -141,  -163,   167,  -171,   897,   919,
     918,   904,   943,   302,   945,   905,   867,   871,   303,   906,
     172,   962,   135,   440,   441,   135,   442,   443,   444,   436,
     445,   446,   447,   907,   172,   449,   450,   451,   908,   453,
     454,   455,   456,   172,   909,   459,  1010,   970,   460,   461,
     462,   975,  1013,   500,   175,  1014,   172,  1019,  1022,   463,
    1042,  1046,  1079,  1161,  1162,  1028,  1062,   304,   175,  1063,
    1080,  1033,  1020,  1083,  1081,  1038,   304,   175,  1084,  1096,
    1108,   884,  1155,  1149,   319,   417,  1152,  1159,   437,  1167,
     175,  1194,   438,  1181,  1185,   167,  1219,  1220,  1200,   167,
    1224,   176,  1248,  1249,   167,   418,   319,   167,   319,   419,
     364,  1102,   225,   583,   135,   815,   513,   985,   135,   577,
    1039,  1095,   561,   135,   782,   319,   135,   600,  1060,   791,
    1057,  1233,   597,  1052,   420,   541,  1139,   421,   440,   441,
    1242,   154,   443,   444,   154,   445,   446,   436,  1235,   422,
     449,   378,  1016,   500,   500,   176,   901,   456,   781,   221,
     176,   953,     0,   460,   461,   462,     0,   423,     0,     0,
       0,     0,   424,   181,  1073,  1074,  1071,     0,   172,     0,
     172,   167,     0,   167,     0,     0,   167,   512,     0,  1094,
     369,     0,     0,     0,     0,     0,     0,   181,  1046,     0,
     135,     0,   135,     0,     0,   135,   437,     0,     0,   167,
     438,     0,   175,     0,   175,     0,     0,     0,     0,   167,
       0,     0,     0,   319,     0,     0,     0,     0,  1093,     0,
       0,     0,     0,   154,     0,   176,     0,   154,   135,  1128,
    1131,     0,   154,     0,     0,   154,   181,     0,     0,   176,
     181,   181,     0,     0,   172,     0,   440,     0,   176,     0,
     167,   444,   293,   445,   446,   319,   167,     0,   449,     0,
       0,   176,   181,     0,     0,   456,   181,   369,     0,   135,
       0,   369,   295,   462,     0,   135,   297,   512,   175,   512,
       0,     0,   512,   512,     0,  1046,     0,  1166,     0,     0,
       0,     0,     0,  1128,  1131,     0,     0,     0,     0,     0,
       0,   298,     0,     0,   299,     0,    59,   167,     0,   154,
    1177,   154,     0,     0,   154,     0,   300,     0,     0,     0,
     319,     0,  1190,     0,     0,  1193,   135,     0,     0,     0,
       0,     0,     0,     0,   302,     0,     0,   154,     0,   303,
     436,     0,     0,     0,     0,     0,     0,   154,   167,     0,
     167,     0,     0,     0,     0,   167,     0,     0,   167,     0,
       0,     0,     0,     0,     0,     0,     0,   135,     0,   135,
       0,     0,     0,     0,   135,     0,   181,   135,     0,     0,
       0,     0,     0,   176,     0,   176,     0,   167,   154,     0,
       0,     0,     0,   167,   154,     0,     0,   167,   167,   437,
       0,     0,     0,   438,     0,     0,   135,     0,     0,   167,
       0,     0,   135,   167,     0,   167,   135,   135,     0,     0,
     319,   319,     0,     0,     0,     0,   172,     0,   135,   172,
     181,     0,   135,     0,   135,   181,   167,  1243,     0,     0,
       0,     0,     0,     0,     0,   154,     0,     0,     0,   440,
     441,     0,   442,   443,   444,   135,   445,   446,   447,   176,
     175,   449,     0,   175,     0,     0,     0,   455,   456,     0,
       0,   459,     0,     0,   460,   461,   462,     0,   167,   167,
     167,   167,     0,     0,     0,   463,   154,     0,   154,     0,
       0,     0,   183,   154,     0,     0,   154,   135,   135,   135,
     135,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     181,     0,     0,     0,     0,     0,   183,     0,   172,     0,
       0,     0,   172,     0,   181,   154,     0,   172,     0,     0,
     172,   154,     0,   181,     0,   154,   154,     0,     0,     0,
       0,     0,     0,     0,   436,     0,   181,   154,     0,     0,
       0,   154,   175,   154,     0,     0,   175,     0,     0,     0,
       0,   175,     0,     0,   175,   183,     0,     0,     0,   183,
     183,     0,     0,     0,   154,     0,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   183,     0,     0,     0,   183,     0,     0,     0,     0,
       0,     0,     0,   437,   172,     0,   172,   438,     0,   172,
       0,     0,     0,     0,     0,     0,   154,   154,   154,   154,
     479,     0,   436,  -658,     0,  -658,     0,     0,     0,     0,
       0,     0,   172,     0,     0,     0,     0,     0,   175,     0,
     175,   176,   172,   175,   176,     0,     0,     0,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,     0,     0,     0,   449,   175,     0,   181,     0,
     181,   455,   456,     0,     0,   459,   175,     0,   460,   461,
     462,   437,     0,   172,     0,   438,     0,     0,     0,   172,
       0,     0,     0,     0,     0,     0,     0,   140,   358,     0,
       0,     0,    23,    24,     0,   183,     0,     0,     0,     0,
       0,   359,     0,    30,   360,     0,     0,   175,     0,    33,
       0,   140,     0,   175,     0,     0,    38,     0,     0,     0,
       0,   440,   441,   176,   442,   443,   444,   176,   445,   446,
     172,     0,   176,   449,   181,   176,     0,     0,   361,     0,
     456,     0,     0,    55,     0,    57,   460,   461,   462,   183,
       0,     0,     0,     0,   183,   362,     0,    66,     0,     0,
     140,     0,     0,     0,   175,     0,     0,     0,     0,     0,
       0,   172,     0,   172,     0,    81,     0,    83,   172,     0,
      85,   172,     0,     0,     0,     0,   140,     0,     0,     0,
     140,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     0,     0,     0,     0,   175,     0,   175,     0,   176,
     172,   176,   175,     0,   176,   175,   172,     0,   358,     0,
     172,   172,    23,    24,     0,     0,     0,     0,     0,   183,
       0,   359,   172,    30,   360,     0,   172,   176,   172,    33,
       0,     0,     0,   183,   175,     0,    38,   176,     0,     0,
     175,     0,   183,   184,   175,   175,     0,     0,     0,   172,
       0,     0,     0,     0,     0,   183,   175,     0,   361,     0,
     175,     0,   175,    55,     0,    57,     0,   184,     0,  1043,
       0,     0,  1044,     0,     0,   362,     0,    66,   176,     0,
       0,     0,     0,   175,   176,     0,     0,     0,     0,     0,
     140,   172,   172,   172,   172,    81,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,   181,     0,     0,   181,
       0,     0,     0,     0,     0,     0,   184,     0,     0,     0,
     184,   184,     0,     0,     0,   175,   175,   175,   175,     0,
       0,     0,     0,     0,     0,   176,     0,     0,     0,     0,
       0,     0,   184,     0,   140,     0,   184,     0,     0,   140,
      99,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   700,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   176,   183,   176,   183,
       0,   924,     0,   176,   925,     0,   176,     0,     0,   926,
       0,     0,     0,     0,     0,     0,     0,     0,   181,   196,
       0,     0,   181,     0,     0,     0,     0,   181,     0,   927,
     181,     0,     0,     0,     0,   176,   928,     0,   198,     0,
       0,   176,     0,     0,   140,   176,   176,   929,     0,     0,
       0,     0,     0,     0,     0,   930,     0,   176,   140,     0,
       0,   176,     0,   176,     0,     0,     0,   140,     0,     0,
       0,     0,   931,   183,     0,     0,   184,     0,     0,     0,
     140,     0,     0,   932,   176,     0,     0,   704,     0,     0,
       0,     0,     0,     0,   933,     0,   226,     0,     0,   934,
       0,     0,     0,     0,   181,     0,   181,     0,     0,   181,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   176,   176,   176,   176,
     184,     0,   181,     0,     0,   184,     0,     0,     0,     0,
       0,     0,   181,     0,     0,   330,     0,     0,     0,     0,
       0,   429,     0,   230,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   231,   232,
       0,   233,     0,     0,     0,   357,   234,     0,     0,     0,
       0,     0,     0,   181,   235,     0,     0,     0,     0,   181,
     236,     0,     0,     0,     0,     0,   237,     7,     0,     0,
     238,     0,   140,   239,   140,   240,     0,     0,     0,     0,
     184,     0,     0,     0,   241,   358,     0,     0,     0,    23,
      24,   242,   243,     0,   184,     0,     0,     0,   359,   244,
      30,   360,     0,   184,     0,     0,    33,     0,     0,   245,
     181,     0,     0,    38,     0,     0,   184,     0,   246,     0,
     247,     0,   248,     0,   249,   183,     0,   250,   183,     0,
       0,   251,     0,   252,     0,   361,   253,     0,     0,     0,
      55,     0,    57,     0,    59,     0,  1043,     0,   140,  1044,
       0,   181,   362,   181,    66,   493,     0,     0,   181,     0,
       0,   181,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    81,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     181,     0,   270,     0,     0,     0,   181,     0,     0,     0,
     181,   181,     0,     0,     0,     0,     0,     0,     0,   517,
       0,     0,   181,     0,   527,   877,   181,   183,   181,  -314,
       0,   183,     0,     0,     0,     0,   183,    99,     0,   183,
       0,     0,     0,  1045,     0,     0,     0,  -314,   184,   181,
     184,  -314,  -314,     0,     0,     0,     0,     0,     0,     0,
    -314,     0,  -314,  -314,     0,     0,     0,     0,  -314,     0,
       0,     0,     0,     0,     0,  -314,     0,     0,  -314,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   181,   181,   181,   181,     0,     0,  -314,     0,  -314,
       0,     0,  -314,     0,  -314,     0,  -314,   230,  -314,  -314,
       0,  -314,     0,   183,  -314,   183,  -314,     0,   183,     0,
       0,     0,   231,   232,   184,   233,     0,     0,     0,     0,
     234,     0,     0,     0,  -314,   598,  -314,     0,   235,  -314,
     140,   183,     0,   140,   236,     0,     0,     0,     7,     0,
     237,   183,     0,     0,   238,     0,     0,   239,     0,   240,
       0,     0,     0,     0,     0,     0,   358,     0,   241,     0,
      23,    24,     0,     0,     0,   242,   243,     0,     0,   359,
       0,    30,   360,   244,     0,     0,     0,    33,     0,  -314,
       0,     0,   183,   245,    38,  -314,     0,     0,   183,     0,
       0,     0,   246,     0,   247,     0,   248,     0,   249,     0,
       0,   250,     0,     0,     0,   251,   361,   252,     0,     0,
     253,    55,     0,    57,     0,    59,     0,  1043,     0,     0,
    1044,     0,   140,   362,     0,    66,   140,     0,     0,     0,
       0,   140,     0,     0,   140,     0,     0,     0,     0,   183,
       0,     0,     0,    81,   436,    83,     0,   764,    85,   766,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   363,     0,   976,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     183,     0,   183,     0,     0,     0,     0,   183,     0,     0,
     183,     0,     0,     0,     0,     0,   184,     0,    99,   184,
       0,     0,     0,   437,  1113,     0,     0,   438,   140,     0,
     140,     0,     0,   140,     0,     0,     0,     0,     0,   183,
       0,     0,     0,     0,     0,   183,     0,     0,     0,   183,
     183,     0,     0,     0,     0,     0,   140,     0,     0,     0,
       0,   183,     0,     0,     0,   183,   140,   183,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   183,   453,
     454,   455,   456,   457,     0,   459,     0,     0,   460,   461,
     462,     0,     0,     7,     0,     0,     0,   140,   184,   463,
       0,     0,   184,   140,     0,     0,     0,   184,     0,     0,
     184,   358,     0,     0,     0,    23,    24,     0,     0,     0,
     183,   183,   183,   183,   359,     0,    30,   360,     0,     0,
       0,     0,    33,     0,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   140,     0,     0,     0,     0,     0,
       0,   361,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,  1043,     0,     0,  1044,     0,     0,   362,     0,
      66,     0,     0,     0,   184,     0,   184,     0,     0,   184,
       0,     0,     0,     0,     0,   140,     0,   140,    81,     0,
      83,     0,   140,    85,     0,   140,     0,     0,     0,     0,
       0,     0,   184,     0,     0,   944,     0,     0,   947,     0,
       0,     0,   184,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   140,     0,     0,     0,     0,     0,
     140,     0,     0,     0,   140,   140,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   140,     0,     0,  1176,
     140,     0,   140,   184,     0,     0,     0,     0,     0,   184,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   140,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   210,     0,     0,
       0,   220,     0,     0,     0,     0,     0,  1021,     0,     0,
     228,  1026,     0,     0,     0,     0,   527,   261,     0,   527,
     184,     0,     0,     0,     0,   140,   140,   140,   140,   277,
       0,   280,   281,     0,     0,     0,     0,   286,     0,   287,
       0,   288,     0,     0,     0,     0,   314,     0,     0,     0,
     228,   322,   324,   326,   327,     0,     0,     0,   331,     0,
     332,   184,     0,   184,     0,     0,     0,   339,   184,     0,
     322,   184,   341,     0,   342,     0,     0,   343,   344,     0,
     345,     0,     0,     0,   228,   322,   355,     0,     0,     0,
       0,     0,     0,  1075,     0,  1076,     0,     0,  1078,     0,
     184,     0,     0,     0,     0,     0,   184,     0,     0,     0,
     184,   184,     0,   379,     0,   382,     0,     0,     0,     0,
       0,  1092,   184,     0,     0,     0,   184,   230,   184,     0,
       0,  1026,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   231,   232,     0,   233,     0,     0,     0,   184,
     234,     0,     0,     0,     0,     0,     0,   433,   235,     0,
       0,     0,     0,     0,   236,     0,     0,     0,     0,     0,
     237,     0,  1119,     0,   238,     0,     0,   239,  1124,   240,
     485,     0,     0,     0,   331,   228,     0,     0,   241,     0,
       0,   184,   184,   184,   184,   242,   243,     0,     0,     0,
       0,     0,     0,   244,     0,     0,     0,     0,     0,     0,
       0,     0,   322,   245,     0,     0,     0,     0,     0,     0,
       0,     0,   246,     0,   247,     0,   248,     0,   249,  1160,
       0,   250,     0,     0,     0,   251,     0,   252,     0,     0,
     253,     0,     0,     0,     0,     0,     0,     0,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1179,     0,  1183,   322,   275,     0,     0,   527,     0,     0,
     527,     0,     0,     0,   331,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   270,     0,   275,   549,
     550,     0,     0,     0,     0,     0,     0,   336,   338,  1218,
       0,     0,   322,     0,     0,  1221,   559,     0,     0,  1179,
    1183,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1227,     0,     0,   568,  1229,     0,   766,   368,     0,
       0,     0,     0,     0,   574,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   586,     0,   588,     0,
       0,     0,     0,     0,     0,     0,   396,     0,     0,     0,
       0,     0,   601,   606,   609,     0,     0,     0,     0,     0,
       0,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     766,   766,   766,   766,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   687,   688,   689,
     691,   693,   694,     0,   695,   696,   697,   698,   699,     0,
     707,   708,   709,   710,   711,   712,   713,   714,   715,   716,
     717,   718,   719,   720,   721,   722,   723,     0,     0,   495,
       0,     0,     0,   322,   322,     0,     0,     0,     0,   322,
       0,     0,   322,   322,   322,   322,   733,   735,     0,     0,
       0,   606,   740,   255,     0,   755,     0,     0,     0,     0,
       0,   758,     0,     0,     0,   267,   268,   269,   508,   511,
       0,     0,   767,     0,     0,   769,     0,     0,     0,   322,
       0,     0,     0,     0,     0,   275,   777,   275,   780,     0,
       0,     0,   280,     0,   275,   785,   786,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   314,     0,     0,
     314,     0,     0,     0,     0,     0,     0,     0,     0,   322,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   758,
       0,     0,   322,   322,   275,     0,     0,     0,   228,   814,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   844,   844,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   856,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   426,     0,
       0,     0,   314,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   887,     0,
       5,     6,     0,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,   486,     0,    17,    18,     0,    19,     0,
     195,     0,  -208,    22,     0,     0,     0,   895,     0,     0,
       0,   294,     0,   196,   900,     0,     0,    32,  -208,  -208,
    -208,     0,   197,    35,     0,     0,     0,   296,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,   505,   506,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,   519,    63,    64,   200,     0,   767,
     322,     0,     0,     0,     0,    71,    72,    73,    74,   301,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
      84,     0,   322,    86,   322,   941,   942,     0,     0,     0,
       0,    88,     0,     0,   546,     0,     0,     0,     0,     0,
       0,   322,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,   314,    94,     0,     0,
     969,     0,     0,     0,     0,     0,    95,   275,     0,    96,
      97,    98,     0,     0,     0,   100,     0,   202,     0,   102,
       0,  -208,     0,   255,   103,   586,     0,     0,     0,  1006,
       0,  1007,   844,   844,     0,   341,     0,   342,     0,     0,
     344,   345,     0,     0,     0,     0,     0,     0,   275,   275,
       0,     0,     0,     0,  1018,     0,   275,   275,   228,   610,
       0,   613,  1024,   758,  1027,   618,  1029,  1031,     0,  1032,
       0,     0,  1035,  1037,     0,     0,     0,     0,     0,   322,
       0,     0,     0,     0,  1049,     0,     0,     0,     0,   675,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1066,  1027,  1032,  1035,
    1069,   322,     0,     0,     0,   755,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   691,
     733,     0,     0,     0,     0,     0,     0,     0,     0,  1077,
       0,     0,     0,     0,   741,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   314,     0,     0,     0,  1091,
       0,     0,     0,     0,     0,   228,     0,     0,     0,     0,
       0,   230,   519,  1099,     0,  1101,   322,     0,     0,     0,
       0,     0,     0,     0,     0,   783,   231,   232,     0,   233,
       0,     0,     0,     0,   234,   789,   844,     0,     0,   519,
       0,     0,   235,  1110,     0,  1111,     0,   949,   236,     0,
       0,     0,  1117,     0,   237,     0,     0,     0,   238,     0,
       0,   239,     0,   240,     0,     0,     0,  1126,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,   242,
     243,   977,     0,   831,     0,     0,     0,   244,  1147,     0,
       0,   275,   275,   852,     0,     0,     0,   245,     0,     0,
    1154,     0,     0,     0,     0,     0,   246,     0,   247,     0,
     248,   618,   249,     0,     0,   250,   322,   322,     0,   251,
       0,   252,     0,     0,   253,     0,     0,     0,     0,  1168,
       0,     0,     0,     0,     0,     0,     0,  1172,   844,   844,
       0,     0,     0,     0,     0,     0,   275,     0,     0,     0,
     275,     0,  1040,  1180,     0,  1184,     0,     0,  1186,     0,
    1189,     0,     0,  1192,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   426,     0,     0,
     518,     0,     0,     0,     0,  1180,  1184,     0,  1189,  1192,
       0,     0,     0,     0,     7,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1225,     0,
       0,  1226,   358,     0,     0,  1228,    23,    24,     0,     0,
    1230,  1231,     0,     0,  1232,   359,     0,    30,   360,     0,
       0,     0,  1234,    33,     0,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,  1244,  1245,  1230,  1246,
    1247,     0,     0,     0,     0,     0,     0,  1097,     0,     0,
       0,     0,   361,     0,   950,     0,     0,    55,   519,    57,
       0,   356,     0,  1043,     0,     0,  1044,     0,   957,   362,
       0,    66,     0,  1258,  1259,  1260,  1261,  1258,  1259,  1260,
    1261,     0,     0,     0,     0,   230,     0,   978,     0,    81,
       0,    83,     0,     0,    85,     0,   436,     0,     0,     0,
     231,   232,     0,   233,   275,   275,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,   235,     0,     0,     0,
       0,     0,   236,     0,     0,     0,     0,     0,   237,     0,
       0,     0,   238,   426,     0,   239,     0,   240,     0,     0,
       0,     0,     0,     0,    99,     0,   241,     0,     0,     0,
       0,     0,   436,   242,   243,   437,     0,   610,     0,   438,
       0,   244,   275,     0,     0,     0,     0,     0,   275,   275,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
     246,   675,   247,     0,   248,   275,   249,     0,   439,   250,
       0,     0,     0,   251,   511,   252,     0,   275,   253,     0,
     275,     0,     0,     0,     0,   440,   441,     0,   442,   443,
     444,   437,   445,   446,   447,   438,   448,   449,   450,   451,
       0,   453,   454,   455,   456,   457,   458,   459,     0,     0,
     460,   461,   462,     0,     0,  1222,     0,     0,     0,     0,
       0,   463,     0,     0,   439,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   545,     0,     0,     0,     0,     0,
     978,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,     0,   453,   454,   455,
     456,   457,     0,   459,     0,     0,   460,   461,   462,     0,
       0,     0,     0,     0,   511,     0,   511,   463,     0,   511,
     511,     0,    -2,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,    16,
      17,    18,     0,    19,     0,    20,    21,  -208,    22,    23,
      24,    25,    26,    27,   618,  -208,    28,     0,    29,     0,
      30,    31,    32,  -208,  -208,  -208,    33,    34,    35,   -68,
       0,    36,    37,    38,     0,    39,  -334,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -334,     0,     0,
      55,    56,    57,    58,    59,    60,    61,  -334,   -68,    62,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,    83,    84,   -68,    85,    86,     0,
       0,  -208,   618,    87,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,    99,     0,     0,
     100,     0,   101,     0,   102,     0,  -208,     0,     4,   103,
       5,     6,     7,     8,     9,    10,    11,    12,    13,     0,
    -689,     0,    14,    15,    16,    17,    18,  -689,    19,     0,
      20,    21,  -208,    22,    23,    24,    25,    26,    27,  -689,
    -208,    28,  -689,    29,     0,    30,    31,    32,  -208,  -208,
    -208,    33,    34,    35,   -68,     0,    36,    37,    38,     0,
      39,  -334,    40,     0,    41,    42,    43,    44,    45,     0,
      46,    47,    48,    49,   -68,    50,    51,     0,    52,    53,
      54,     0,  -334,     0,     0,    55,    56,    57,    58,     0,
      60,    61,  -334,   -68,    62,    63,    64,    65,  -689,    66,
      67,    68,  -689,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,    83,
      84,   -68,    85,    86,     0,     0,  -208,     0,    87,     0,
       0,  -689,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,  -689,
      91,  -689,  -689,  -689,  -689,  -689,  -689,  -689,     0,  -689,
    -689,  -689,  -689,  -689,  -689,  -689,  -689,  -689,  -689,  -689,
    -689,    98,    99,  -689,  -689,  -689,     0,   101,  -689,   102,
       0,  -208,     0,   346,  -689,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,    16,
      17,    18,     0,    19,     0,    20,    21,  -208,    22,    23,
      24,    25,    26,    27,     0,  -208,    28,     0,    29,     0,
      30,    31,    32,  -208,  -208,  -208,    33,    34,    35,   -68,
       0,    36,    37,    38,     0,    39,  -334,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -334,     0,     0,
      55,    56,    57,    58,    59,    60,    61,  -334,   -68,    62,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,    83,    84,   -68,    85,    86,     0,
       0,  -208,     0,    87,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,    99,     0,     0,
     100,     0,   101,   347,   102,     0,  -208,     0,     4,   103,
       5,     6,     7,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,    15,    16,    17,    18,     0,    19,     0,
      20,    21,  -208,    22,    23,    24,    25,    26,    27,     0,
    -208,    28,     0,    29,     0,    30,    31,    32,  -208,  -208,
    -208,    33,    34,    35,   -68,     0,    36,    37,    38,     0,
      39,  -334,    40,     0,    41,    42,    43,    44,    45,     0,
      46,    47,    48,    49,   -68,    50,    51,     0,    52,    53,
      54,     0,  -334,     0,     0,    55,    56,    57,    58,    59,
      60,    61,  -334,   -68,    62,    63,    64,    65,     0,    66,
      67,    68,     0,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,    83,
      84,   -68,    85,    86,     0,     0,  -208,     0,    87,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,    99,     0,     0,   100,     0,   101,   564,   102,
       0,  -208,     0,   581,   103,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,    16,
      17,    18,     0,    19,     0,    20,    21,  -208,    22,    23,
      24,    25,    26,    27,     0,  -208,    28,     0,    29,     0,
      30,    31,    32,  -208,  -208,  -208,    33,    34,    35,   -68,
       0,    36,    37,    38,     0,    39,  -334,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -334,     0,     0,
      55,    56,    57,    58,    59,    60,    61,  -334,   -68,    62,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,    83,    84,   -68,    85,    86,     0,
       0,  -208,     0,    87,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,    99,     0,     0,
     100,     0,   101,   582,   102,     0,  -208,     0,     4,   103,
       5,     6,     7,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,    15,    16,    17,    18,     0,    19,     0,
      20,    21,  -208,    22,    23,    24,    25,    26,    27,     0,
    -208,    28,     0,    29,     0,    30,    31,    32,  -208,  -208,
    -208,    33,    34,    35,   -68,     0,    36,    37,    38,     0,
      39,  -334,    40,     0,    41,    42,    43,    44,    45,     0,
      46,    47,    48,    49,   -68,    50,    51,     0,    52,    53,
      54,     0,  -334,     0,     0,    55,    56,    57,    58,    59,
      60,    61,  -334,   -68,    62,    63,    64,    65,     0,    66,
      67,    68,     0,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,    83,
      84,   -68,    85,    86,     0,     0,  -208,     0,    87,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,    99,     0,     0,   100,     0,   101,   821,   102,
       0,  -208,     0,     4,   103,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,    16,
      17,    18,     0,    19,     0,    20,    21,  -208,    22,    23,
      24,    25,    26,    27,     0,  -208,    28,     0,    29,     0,
      30,    31,    32,  -208,  -208,  -208,    33,    34,    35,   -68,
       0,    36,    37,    38,     0,    39,  -334,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -334,     0,     0,
      55,    56,    57,    58,   356,    60,    61,  -334,   -68,    62,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,    83,    84,   -68,    85,    86,     0,
       0,  -208,     0,    87,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,    99,     0,     0,
     100,     0,   101,     0,   102,     0,  -208,     0,     4,   103,
       5,     6,     7,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,    15,    16,    17,    18,     0,    19,     0,
      20,    21,  -208,    22,    23,    24,    25,    26,    27,     0,
    -208,    28,     0,    29,     0,    30,    31,    32,  -208,  -208,
    -208,    33,    34,    35,   -68,     0,    36,    37,    38,     0,
      39,  -334,    40,     0,    41,    42,    43,    44,    45,     0,
      46,    47,    48,    49,   -68,    50,    51,     0,    52,    53,
      54,     0,  -334,     0,     0,    55,    56,    57,    58,    59,
      60,    61,  -334,   -68,    62,    63,    64,    65,     0,    66,
      67,    68,     0,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,    83,
      84,   -68,    85,    86,     0,     0,  -208,     0,    87,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,    99,     0,     0,   100,     0,   101,     0,   102,
       0,  -208,     0,     4,   103,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,    16,
      17,    18,     0,    19,     0,    20,    21,  -208,    22,    23,
      24,    25,    26,    27,     0,  -208,    28,     0,    29,     0,
      30,    31,    32,  -208,  -208,  -208,    33,    34,    35,   -68,
       0,    36,    37,    38,     0,    39,  -334,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -334,     0,     0,
      55,    56,    57,    58,     0,    60,    61,  -334,   -68,    62,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,    83,    84,   -68,    85,    86,     0,
       0,  -208,     0,    87,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,    99,     0,     0,
     100,     0,   101,     0,   102,     0,  -208,     0,     4,   103,
       5,     6,     7,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,    15,    16,    17,    18,     0,    19,     0,
      20,    21,  -208,    22,    23,    24,    25,    26,    27,     0,
    -208,    28,     0,    29,     0,    30,    31,    32,  -208,  -208,
    -208,    33,  1034,    35,   -68,     0,    36,    37,    38,     0,
      39,  -334,    40,     0,    41,    42,    43,    44,    45,     0,
      46,    47,    48,    49,   -68,    50,    51,     0,    52,    53,
      54,     0,  -334,     0,     0,    55,    56,    57,    58,     0,
      60,    61,  -334,   -68,    62,    63,    64,    65,     0,    66,
      67,    68,     0,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,    83,
      84,   -68,    85,    86,     0,     0,  -208,     0,    87,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,    99,     0,     0,   100,     0,   101,     0,   102,
       0,  -208,     0,     4,   103,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,    16,
      17,    18,     0,    19,     0,    20,    21,  -208,    22,    23,
      24,    25,    26,    27,     0,  -208,    28,     0,    29,     0,
      30,    31,    32,  -208,  -208,  -208,    33,  1178,    35,   -68,
       0,    36,    37,    38,     0,    39,  -334,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -334,     0,     0,
      55,    56,    57,    58,     0,    60,    61,  -334,   -68,    62,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,    83,    84,   -68,    85,    86,     0,
       0,  -208,     0,    87,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,    99,     0,     0,
     100,     0,   101,     0,   102,     0,  -208,     0,     4,   103,
       5,     6,     7,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,    15,    16,    17,    18,     0,    19,     0,
      20,    21,  -208,    22,    23,    24,    25,    26,    27,     0,
    -208,    28,     0,    29,     0,    30,    31,    32,  -208,  -208,
    -208,    33,  1182,    35,   -68,     0,    36,    37,    38,     0,
      39,  -334,    40,     0,    41,    42,    43,    44,    45,     0,
      46,    47,    48,    49,   -68,    50,    51,     0,    52,    53,
      54,     0,  -334,     0,     0,    55,    56,    57,    58,     0,
      60,    61,  -334,   -68,    62,    63,    64,    65,     0,    66,
      67,    68,     0,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,    83,
      84,   -68,    85,    86,     0,     0,  -208,     0,    87,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,    99,     0,     0,   100,     0,   101,     0,   102,
       0,  -208,     0,     4,   103,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,    16,
      17,    18,     0,    19,     0,    20,    21,  -208,    22,    23,
      24,    25,    26,    27,     0,  -208,    28,     0,    29,     0,
      30,    31,    32,  -208,  -208,  -208,    33,  1188,    35,   -68,
       0,    36,    37,    38,     0,    39,  -334,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -334,     0,     0,
      55,    56,    57,    58,     0,    60,    61,  -334,   -68,    62,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,    83,    84,   -68,    85,    86,     0,
       0,  -208,     0,    87,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,    99,     0,     0,
     100,     0,   101,     0,   102,     0,  -208,     0,     4,   103,
       5,     6,     7,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,    15,    16,    17,    18,     0,    19,     0,
      20,    21,  -208,    22,    23,    24,    25,    26,    27,     0,
    -208,    28,     0,    29,     0,    30,    31,    32,  -208,  -208,
    -208,    33,  1191,    35,   -68,     0,    36,    37,    38,     0,
      39,  -334,    40,     0,    41,    42,    43,    44,    45,     0,
      46,    47,    48,    49,   -68,    50,    51,     0,    52,    53,
      54,     0,  -334,     0,     0,    55,    56,    57,    58,     0,
      60,    61,  -334,   -68,    62,    63,    64,    65,     0,    66,
      67,    68,     0,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,    79,    80,    81,    82,    83,
      84,   -68,    85,    86,     0,     0,  -208,     0,    87,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,    99,     0,     0,   100,     0,   101,     0,   102,
     742,  -208,     5,     6,   103,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,  -208,    22,     0,   743,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
    -208,  -208,  -208,     0,   197,    35,     0,   744,     0,    37,
       0,   745,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,   746,     0,    56,   747,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,   748,     0,     0,     0,     0,     0,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,     0,     0,   749,
      82,     0,    84,     0,   750,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,   751,  -208,     5,     6,   103,     8,     9,    10,
      11,    12,    13,     0,   636,     0,    14,     0,     0,    17,
      18,   638,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,   644,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,   201,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,   510,   465,   466,   467,
     468,   469,     0,     0,   472,   473,   474,   475,     0,   477,
     478,     0,   832,   833,   834,   835,   836,   657,     0,   658,
       0,    94,     0,   659,   660,   661,   662,   663,   664,   665,
     837,   667,   668,    96,   838,    98,   -90,   670,   671,   839,
     673,   202,     0,   102,     0,     0,     5,     6,   103,     8,
       9,    10,    11,    12,    13,     0,   636,     0,    14,     0,
       0,    17,    18,   638,    19,     0,   195,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   196,
       0,     0,     0,    32,     0,     0,     0,     0,   197,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   198,   644,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   200,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,   201,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,   510,   465,
     466,   467,   468,   469,     0,     0,   472,   473,   474,   475,
       0,   477,   478,     0,   832,   833,   834,   835,   836,   657,
       0,   658,     0,    94,     0,   659,   660,   661,   662,   663,
     664,   665,   837,   667,   668,    96,   838,    98,     0,   670,
     671,   839,   673,   202,     0,   102,     0,     0,     0,     0,
     103,     5,     6,     7,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,    15,     0,    17,    18,     0,    19,
       0,   195,    21,     0,    22,     0,     0,     0,     0,    27,
       0,     0,    28,     0,   196,     0,     0,     0,    32,     0,
       0,     0,     0,    34,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   198,     0,   199,     0,     0,    44,    45,
       0,    46,    47,    48,    49,     0,    50,    51,     0,    52,
      53,   333,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   200,     0,
       0,     0,     0,     0,    69,    70,    71,    72,    73,    74,
      75,     0,    76,    77,     0,    78,     0,     0,     0,    82,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,     0,     0,     0,     0,    94,     0,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
      96,    97,    98,     0,     0,     0,   100,     0,   101,     0,
     102,     0,     0,     5,     6,   103,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   195,     0,     0,    22,     0,   743,     0,
       0,     0,     0,     0,    28,     0,   196,     0,     0,     0,
      32,     0,     0,     0,     0,   197,    35,     0,   744,     0,
      37,     0,   745,    39,     0,   198,     0,   199,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,   746,     0,    56,
     747,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     200,     0,   748,     0,     0,     0,     0,     0,    71,    72,
      73,    74,   201,     0,    76,     0,     0,    78,     0,     0,
     749,    82,     0,    84,     0,   750,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     202,     0,   102,     0,     0,     5,     6,   103,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   195,     0,     0,    22,   377,
      24,     0,     0,     0,     0,     0,    28,     0,   196,     0,
       0,     0,    32,     0,     0,     0,     0,   197,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   198,     0,   199,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,   361,     0,     0,     0,     0,
       0,    56,    57,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   200,     0,    66,     0,     0,     0,     0,     0,
      71,    72,    73,    74,   201,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,    84,     0,    85,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,     0,     0,     0,
     100,     0,   202,     0,   102,     0,     0,     5,     6,   103,
       8,     9,    10,   380,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,    20,     0,  -208,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     196,     0,     0,     0,    32,  -208,  -208,  -208,     0,   197,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   198,
       0,   199,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,    65,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,   201,     0,    76,     0,
       0,    78,     0,     0,     0,    82,    83,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,  -208,     5,
       6,   103,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   195,
       0,  -208,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   196,     0,     0,     0,    32,  -208,  -208,  -208,
       0,   197,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   198,     0,   199,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   200,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,   201,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   202,     0,   102,     0,
    -208,     5,     6,   103,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   195,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   196,     0,     0,     0,    32,     0,
       0,     0,     0,   197,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   198,     0,   199,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   200,  -733,
       0,     0,     0,  -733,     0,     0,    71,    72,    73,    74,
     201,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,     0,  -376,     0,     0,    94,     0,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
      96,    97,    98,  -376,     0,     0,   100,     0,   202,  -376,
     102,     0,     0,     5,     6,   103,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   195,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   196,     0,     0,     0,
      32,     0,     0,     0,     0,   197,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   198,     0,   199,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     200,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,   201,     0,    76,     0,     0,    78,   316,   317,
       0,    82,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,   318,     0,     0,   100,     0,
     202,     0,   102,     0,     0,     5,     6,   103,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   195,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   196,     0,
       0,     0,    32,     0,     0,     0,     0,   197,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   198,     0,   199,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   200,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,   201,     0,    76,     0,     0,    78,
     316,   317,     0,    82,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,   351,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,     0,     0,     0,
     100,     0,   202,     0,   102,     0,     0,     5,     6,   103,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   195,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     196,     0,     0,     0,    32,     0,     0,     0,     0,   197,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   198,
       0,   199,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   200,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,   201,     0,    76,     0,
       0,    78,   316,   317,     0,    82,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,  -536,     0,     5,
       6,   103,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   195,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   196,     0,     0,     0,    32,     0,     0,     0,
       0,   197,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   198,     0,   199,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   200,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,   201,     0,
      76,     0,     0,    78,   316,   317,     0,    82,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   202,     0,   102,     0,
       0,  -536,     0,   103,     5,     6,     0,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,   201,     0,    76,     0,     0,    78,   316,
     317,     0,    82,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   202,     0,   102,   807,     0,     5,     6,   103,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   195,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   196,
       0,     0,     0,    32,     0,     0,     0,     0,   197,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   198,     0,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   200,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,   201,     0,    76,     0,     0,
      78,   316,   317,     0,    82,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    91,    92,    93,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,    96,    97,    98,     0,     0,
       0,   100,     0,   202,     0,   102,   810,     0,     5,     6,
     103,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   195,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   196,     0,     0,     0,    32,     0,     0,     0,     0,
     197,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     198,     0,   199,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   200,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,   201,     0,    76,
       0,     0,    78,   316,   317,     0,    82,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   202,     0,   102,   816,     0,
       5,     6,   103,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,   197,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,   955,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,   201,
       0,    76,     0,     0,    78,   316,   317,     0,    82,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,     0,     0,     0,   100,     0,   202,     0,   102,
       0,     0,     5,     6,   103,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,  -384,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,  -384,     0,     0,   100,     0,   202,
    -384,   102,     0,     0,     5,     6,   103,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,   201,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,  -369,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,  -369,     0,     0,   100,
       0,   202,  -369,   102,     0,     0,     5,     6,   103,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   195,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   196,
       0,     0,     0,    32,     0,     0,     0,     0,   197,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   198,     0,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   200,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,   201,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    91,    92,    93,     0,
    -377,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,    96,    97,    98,  -377,     0,
       0,   100,     0,   202,  -377,   102,     0,     0,     5,     6,
     103,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   195,     0,
       0,    22,     0,   258,     0,     0,     0,     0,     0,    28,
       0,   196,     0,     0,     0,    32,     0,     0,     0,     0,
     197,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     198,     0,   199,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   200,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,   201,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,    84,     0,
     259,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   202,     0,   102,     0,     0,
       5,     6,   103,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,   197,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,   201,
       0,    76,     0,     0,    78,   316,   317,     0,    82,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,     0,     0,     0,   100,     0,   202,     0,   102,
       0,     0,     5,     6,   103,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
    -733,     0,     0,     0,  -733,     0,     0,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,     0,     0,     5,     6,   103,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,   201,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,   602,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   202,     0,   102,     0,     0,   603,     0,   103,     5,
       6,     0,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   195,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   196,     0,     0,     0,    32,     0,     0,     0,
       0,   197,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   198,     0,   199,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   200,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,   201,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,  -405,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   202,     0,   102,  -405,
       0,     5,     6,   103,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   195,     0,     0,    22,     0,     0,     0,     0,     0,
       0,   274,    28,     0,   196,     0,     0,     0,    32,     0,
       0,     0,     0,   197,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   198,     0,   199,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   200,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
     201,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,     0,     0,     0,     0,    94,     0,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
      96,    97,    98,     0,     0,     0,   100,     0,   101,     0,
     102,     0,     0,     5,     6,   103,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   195,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   196,     0,     0,     0,
      32,     0,     0,     0,     0,   197,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   198,     0,   199,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     200,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,   201,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,   602,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     202,     0,   102,     0,     0,     5,     6,   103,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   195,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   196,     0,
       0,     0,    32,     0,     0,     0,     0,   197,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   198,     0,   199,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   200,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,   201,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,     0,     0,     0,
     100,     0,   202,     0,   102,     0,     0,   603,     0,   103,
       5,     6,     0,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,   197,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,   201,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,     0,     0,     0,   100,     0,   202,   805,   102,
       0,     0,     5,     6,   103,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,     0,     0,  1025,     0,   103,     5,     6,     0,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   195,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     196,     0,     0,     0,    32,     0,     0,     0,     0,   197,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   198,
       0,   199,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   200,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,   201,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,  1030,
       0,   103,     5,     6,     0,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,     0,     0,     5,     6,   103,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,   294,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,   197,    35,     0,     0,
       0,   296,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,   301,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   202,     0,   102,     0,     0,     5,     6,   103,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   195,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   196,
       0,     0,     0,    32,     0,     0,     0,     0,  1067,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   198,     0,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   200,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,   201,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    91,    92,    93,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,    96,    97,    98,     0,     0,
       0,   100,     0,   202,     0,   102,     0,     0,     5,     6,
     103,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   195,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   196,     0,     0,     0,    32,     0,     0,     0,     0,
    1213,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     198,     0,   199,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   200,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,   201,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   202,     0,   102,     0,     0,
       5,     6,   103,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,  1214,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,   201,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,     0,     0,     0,   100,     0,   202,     0,   102,
       0,     0,     5,     6,   103,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,  1216,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,     0,     0,     5,     6,   103,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,  1217,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,   201,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   202,     0,   102,     0,     0,     0,     6,   103,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,     0,    18,     0,     0,     0,   195,     0,  -208,     0,
       0,   293,     0,     0,     0,     0,     0,     0,     0,   196,
       0,     0,     0,     0,  -208,  -208,  -208,     0,   197,     0,
       0,   295,     0,     0,     0,   297,     0,     0,     0,     0,
     199,     0,     0,    44,    45,     0,     0,     0,     0,    49,
       0,    50,    51,     0,     0,     0,     0,     0,     0,     0,
     298,     0,    56,   299,    58,    59,    60,     0,     0,     0,
       0,    63,     0,   200,     0,   300,     0,     0,     0,     0,
       0,    71,    72,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,   302,     0,     0,    84,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    91,    92,    93,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,    96,    97,    98,     0,     0,
       0,   100,     0,   202,     0,   102,  -338,  -208,     0,     6,
     103,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,     0,    18,     0,     0,     0,   195,     0,
    -208,     0,     0,   293,     0,     0,     0,     0,     0,     0,
       0,   196,     0,     0,     0,     0,  -208,  -208,  -208,     0,
     197,     0,     0,   295,     0,     0,     0,   297,     0,     0,
       0,     0,   199,     0,     0,    44,    45,     0,     0,     0,
       0,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,     0,   298,     0,    56,   299,    58,    59,    60,     0,
       0,     0,     0,    63,     0,   200,     0,   300,     0,     0,
       0,     0,     0,    71,    72,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,     0,    84,     0,
     303,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   634,     0,   635,     0,     0,    89,    90,    91,    92,
      93,     0,     0,   636,     0,    94,     0,     0,   637,   232,
     638,   639,     0,     0,    95,     0,   640,    96,    97,    98,
       0,     0,     0,   100,   235,   202,   196,   102,     0,  -208,
     236,     0,   103,     0,     0,     0,   641,     0,     0,     0,
     238,     0,     0,   642,     0,   643,   644,     0,     0,     0,
       0,     0,     0,     0,   645,     0,     0,     0,     0,     0,
       0,   242,   646,     0,     0,     0,     0,     0,     0,   244,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   647,
       0,     0,     0,     0,     0,     0,     0,     0,   246,     0,
     648,     0,   248,     0,   649,     0,     0,   250,     0,     0,
       0,   650,     0,   252,     0,     0,   651,     0,     0,     0,
       0,     0,     0,     0,     0,   510,   465,   466,   467,   468,
     469,     0,     0,   472,   473,   474,   475,     0,   477,   478,
       0,   652,   653,   654,   655,   656,   657,     0,   658,     0,
       0,     0,   659,   660,   661,   662,   663,   664,   665,   666,
     667,   668,     0,   669,     0,     0,   670,   671,   672,   673,
       5,     6,   674,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,  -208,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,  -208,  -208,
    -208,     0,   197,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,   201,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,     0,
      97,    98,     0,     0,     0,   100,     0,   202,     0,   102,
       0,  -208,     5,     6,     0,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,  -208,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
    -208,  -208,  -208,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,   201,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,     0,    91,     0,     5,     6,     0,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,    98,   195,     0,  -208,    22,     0,   202,
       0,   102,     0,  -208,     0,    28,     0,   196,     0,     0,
       0,    32,  -208,  -208,  -208,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,   201,     0,    76,     0,     0,    78,   700,
       0,     0,    82,     0,    84,     0,     0,    86,     0,   636,
       0,     0,     0,     0,   987,   988,   638,   989,     0,     0,
       0,     0,   990,     0,     0,     0,     0,     0,     0,     0,
     702,     0,    89,     0,    91,     0,     0,     0,     0,     0,
       0,     0,   991,     0,     0,     0,   992,     0,     0,   993,
       0,   198,   644,     0,     0,    98,     0,     0,     0,     0,
     703,     0,     0,   102,     0,  -208,     0,     0,   994,     0,
       0,     0,     0,     0,     0,   995,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   996,     0,     0,     0,     0,
       0,     0,     0,     0,   997,     0,   998,     0,   999,     0,
     704,     0,     0,     0,     0,     0,   705,  1000,     0,  1001,
       0,     0,  1002,     0,     0,     0,     0,     0,     0,     0,
       0,   510,   465,   466,   467,   468,   469,     0,     0,   472,
     473,   474,   475,     0,   477,   478,     0,   652,   653,   654,
     655,   656,   657,     0,   658,     0,     0,     0,   659,   660,
     661,   662,   663,   664,   665,   666,   667,   668,   230,   669,
       0,     0,   670,   671,   672,   673,  1003,     0,   636,     0,
       0,     0,     0,   231,   232,   638,   233,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,   235,
       0,     0,     0,     0,     0,   236,     0,     0,     0,     0,
       0,   237,     0,     0,     0,   238,     0,     0,   239,     0,
     240,   644,     0,     0,     0,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,     0,     0,
       0,     0,     0,     0,   244,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   245,     0,     0,     0,     0,     0,
       0,   434,     0,   246,     0,   247,     0,   248,   435,   249,
       0,     0,   250,     0,     0,     0,   251,     0,   252,     0,
     436,   253,     0,     0,     0,     0,     0,     0,     0,     0,
     510,   465,   466,   467,   468,   469,     0,     0,   472,   473,
     474,   475,     0,   477,   478,     0,   652,   653,   654,   655,
     656,   657,     0,   658,     0,     0,     0,   659,   660,   661,
     662,   663,   664,   665,   666,   667,   668,     0,   669,     0,
       0,   670,   671,   672,   673,     0,     0,     0,     0,   437,
       0,   434,     0,   438,     0,     0,     0,     0,   435,     0,
       0,   509,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   439,   510,   465,   466,   467,   468,   469,     0,
       0,   472,   473,   474,   475,     0,   477,   478,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,     0,     0,   460,   461,   462,     0,   363,   437,
       0,   434,     0,   438,     0,   463,     0,     0,   435,     0,
       0,  1187,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   439,   510,   465,   466,   467,   468,   469,     0,
       0,   472,   473,   474,   475,     0,   477,   478,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,     0,     0,   460,   461,   462,     0,   363,   437,
       0,   434,     0,   438,     0,   463,     0,     0,   435,     0,
       0,  1250,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   439,   510,   465,   466,   467,   468,   469,     0,
       0,   472,   473,   474,   475,     0,   477,   478,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,     0,     0,   460,   461,   462,     0,   363,   437,
       0,   434,     0,   438,     0,   463,     0,     0,   435,     0,
       0,  1251,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   439,   510,   465,   466,   467,   468,   469,     0,
       0,   472,   473,   474,   475,     0,   477,   478,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,     0,     0,   460,   461,   462,     0,   363,   437,
       0,   434,     0,   438,     0,   463,     0,     0,   435,     0,
       0,  1252,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   439,   510,   465,   466,   467,   468,   469,     0,
       0,   472,   473,   474,   475,     0,   477,   478,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,     0,     0,   460,   461,   462,     0,   363,   437,
       0,     0,     0,   438,     0,   463,     0,     0,     0,     0,
     434,  1253,     0,     0,     0,     0,     0,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
     869,     0,   439,   510,   465,   466,   467,   468,   469,     0,
       0,   472,   473,   474,   475,   870,   477,   478,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,     0,     0,   460,   461,   462,     0,   363,     0,
       0,     0,     0,     0,   434,   463,     0,     0,   437,     0,
       0,   435,   438,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   274,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   214,     0,   982,
       0,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,     0,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,     0,   437,   460,   461,   462,   438,   363,     0,     0,
       0,     0,     0,     0,   463,   434,     0,     0,     0,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,   214,     0,     0,   436,   439,     0,     0,     0,   590,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,     0,   591,   460,   461,   462,
       0,   363,     0,     0,     0,     0,     0,     0,   463,     0,
       0,     0,     0,   437,     0,     0,     0,   438,     0,     0,
       0,     0,     0,     0,     0,   434,   828,     0,     0,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,   439,     0,     0,   829,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   830,   592,   460,   461,
     462,     0,     0,     0,     0,     0,     0,     0,     0,   463,
       0,     0,     0,   437,   434,     0,     0,   438,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,  1130,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,     0,   460,   461,
     462,     0,   437,     0,   434,     0,   438,     0,     0,   463,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   274,     0,     0,     0,     0,     0,
       0,   214,     0,     0,     0,   439,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,     0,     0,   460,   461,   462,
       0,   363,   437,     0,   434,     0,   438,     0,   463,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   274,     0,     0,     0,     0,     0,
       0,   214,     0,     0,     0,   439,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,     0,     0,   460,   461,   462,
       0,   363,   437,     0,   434,     0,   438,     0,   463,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   916,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   439,     0,     0,     0,   917,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,     0,     0,   460,   461,   462,
       0,   363,   437,   434,     0,     0,   438,     0,   463,     0,
     435,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   439,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,     0,     0,   460,   461,   462,
       0,   437,     0,     0,     0,   438,     0,     0,   463,     0,
       0,     0,     0,   948,   434,     0,     0,     0,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   439,     0,     0,     0,  1105,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,  1106,   460,   461,   462,     0,
     363,     0,     0,     0,     0,     0,     0,   463,   434,     0,
       0,     0,   437,     0,     0,   435,   438,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   439,     0,     0,     0,     0,
       0,     0,     0,   434,     0,     0,     0,     0,     0,     0,
     435,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   436,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,     0,   437,   460,   461,   462,
     438,     0,     0,   434,   531,     0,     0,     0,   463,     0,
     435,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,   214,     0,     0,     0,   439,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,     0,     0,     0,   438,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,     0,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
     214,   460,   461,   462,   439,     0,     0,     0,     0,     0,
    1122,   437,   463,     0,     0,   438,     0,     0,     0,     0,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   439,     0,   460,   461,   462,     0,
       0,     0,   434,     0,     0,  1170,     0,   463,     0,   435,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   436,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,     0,   460,   461,   462,     0,
       0,     0,   434,     0,     0,     0,     0,   463,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,     0,   434,     0,   438,     0,     0,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   439,     0,     0,     0,     0,     0,     0,
     437,     0,     0,     0,   438,     0,     0,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   439,     0,   460,   461,   462,     0,   554,
     437,   434,     0,     0,   438,     0,   463,     0,   435,     0,
     440,   441,   736,   442,   443,   444,     0,   445,   446,   447,
     436,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   439,   558,   460,   461,   462,     0,     0,
       0,   434,   824,     0,     0,     0,   463,     0,   435,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     436,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,     0,     0,   460,   461,   462,     0,   437,
       0,     0,   434,   438,     0,     0,   463,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   439,     0,     0,     0,     0,     0,     0,   437,
       0,     0,     0,   438,     0,     0,     0,     0,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   439,     0,   460,   461,   462,     0,     0,     0,
     437,   806,     0,     0,   438,   463,     0,     0,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   825,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   863,   439,   460,   461,   462,     0,     0,     0,
       0,     0,     0,     0,     0,   463,     0,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   434,     0,   460,   461,   462,     0,     0,
     435,     0,     0,     0,     0,     0,   463,     0,     0,     0,
       0,     0,   436,     0,     0,   946,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   434,     0,     0,     0,     0,     0,     0,
     435,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,     0,   434,     0,   438,     0,     0,     0,     0,
     435,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,     0,     0,     0,     0,     0,
       0,   437,     0,     0,     0,   438,     0,     0,     0,     0,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   439,     0,   460,   461,   462,     0,
       0,   437,   434,  1012,     0,   438,     0,   463,     0,   435,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   436,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   439,     0,   460,   461,   462,     0,
       0,     0,   434,   958,     0,     0,     0,   463,     0,   435,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   436,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,     0,   460,   461,   462,     0,
     437,     0,     0,   959,   438,     0,     0,   463,     0,     0,
     434,  1104,     0,     0,     0,     0,     0,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,   439,     0,     0,     0,     0,     0,     0,
     437,     0,     0,     0,   438,     0,     0,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   439,     0,   460,   461,   462,     0,     0,
       0,     0,     0,     0,     0,     0,   463,     0,   437,     0,
     440,   441,   438,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   434,     0,   460,   461,   462,     0,     0,
     435,   439,  1059,     0,     0,     0,   463,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,     0,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   434,     0,   460,   461,   462,     0,     0,   435,     0,
       0,     0,     0,     0,   463,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,     0,     0,     0,   438,     0,     0,     0,     0,
       0,   434,     0,     0,     0,     0,     0,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,   439,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
       0,   440,   441,   438,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,  1116,   460,   461,   462,     0,
       0,  1125,   439,     0,     0,     0,     0,   463,     0,   437,
       0,     0,     0,   438,     0,     0,     0,     0,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   439,     0,   460,   461,   462,     0,     0,     0,
       0,     0,     0,     0,     0,   463,     0,     0,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   434,     0,   460,   461,   462,     0,     0,   435,
       0,     0,     0,  1153,     0,   463,     0,     0,     0,     0,
       0,   436,  1156,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,  1175,     0,     0,     0,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,     0,     0,   434,   438,     0,     0,     0,     0,     0,
     435,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   439,     0,     0,     0,     0,     0,     0,
     437,     0,     0,     0,   438,     0,     0,     0,     0,     0,
     440,   441,  1207,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   439,     0,   460,   461,   462,     0,     0,
       0,   437,     0,     0,     0,   438,   463,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,     0,   439,   460,   461,   462,   434,     0,
       0,     0,     0,     0,     0,   435,   463,     0,     0,     0,
       0,   440,   441,     0,   442,   443,   444,   436,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,     0,   460,   461,   462,     0,
       0,     0,     0,     0,     0,     0,   434,   463,     0,     0,
       0,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   437,     0,     0,     0,
     438,     0,     0,     0,     0,     0,   434,     0,  1215,     0,
       0,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,   439,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,     0,   440,   441,   438,   442,
     443,   444,     0,   445,   446,   447,  1254,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
       0,   460,   461,   462,     0,     0,     0,   439,     0,     0,
       0,     0,   463,     0,   437,   434,     0,     0,   438,     0,
       0,     0,   435,     0,   440,   441,  1255,   442,   443,   444,
       0,   445,   446,   447,   436,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   439,     0,   460,
     461,   462,     0,     0,     0,   434,     0,     0,     0,     0,
     463,     0,   435,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   436,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   437,     0,   434,     0,   438,     0,     0,
     463,     0,   435,     0,     0,  1256,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,     0,     0,     0,
       0,     0,     0,   437,     0,     0,     0,   438,     0,     0,
       0,     0,     0,   440,   441,  1257,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   439,     0,   460,   461,
     462,     0,     0,   437,   434,     0,     0,   438,     0,   463,
       0,   435,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   436,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   439,     0,   460,   461,
     462,     0,     0,     0,     0,     0,     0,     0,     0,   463,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,     0,   460,   461,
     462,     0,   936,     0,     0,     0,   438,     0,     0,   463,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   439,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,     0,   358,   460,   461,   462,
      23,    24,     0,     0,     0,     0,     0,     0,   463,   359,
       0,    30,   360,     0,     0,     0,     0,    33,     0,     0,
       0,     0,     0,     0,    38,     0,     0,  -334,     0,     0,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
     -68,     0,     0,     0,     0,     0,   361,     0,  -334,     0,
       0,    55,     0,    57,     0,     0,     0,    61,  -334,   -68,
      62,     0,   616,   362,   230,    66,     7,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   231,
     232,     0,   233,    81,     0,    83,     0,   234,    85,     0,
       0,     0,     0,     0,     0,   235,     0,     0,     0,     0,
       0,   236,     0,     0,     0,     0,     0,   237,     0,     0,
       0,   238,     0,     0,   239,     0,   240,     0,     0,     0,
       0,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,   242,   243,     0,     0,     0,     0,    99,   230,
     244,     0,     0,   363,     0,     0,     0,     0,     0,     0,
     245,     0,     0,     0,   231,   232,     0,   233,     0,   246,
       0,   247,   234,   248,    24,   249,     0,     0,   250,     0,
     235,     0,   251,     0,   252,     0,   236,   253,     0,     0,
       0,     0,   237,     0,     0,     0,   238,     0,     0,   239,
       0,   240,     0,     0,     0,     0,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,   242,   243,     0,
       0,     0,     0,     0,     0,   244,    57,     0,     0,     0,
       0,     0,     0,     0,     0,   245,     0,     0,    66,     0,
     230,     0,     7,     0,   246,     0,   247,     0,   248,     0,
     249,     0,     0,   250,     0,   231,   232,   251,   233,   252,
       0,    85,   253,   234,     0,     0,     0,     0,     0,     0,
       0,   235,     0,     0,     0,     0,     0,   236,     0,     0,
       0,     0,     0,   237,     0,     0,     0,   238,     0,     0,
     239,     0,   240,     0,     0,     0,     0,     0,     0,  -309,
       0,   241,     0,     0,     0,     0,     0,     0,   242,   243,
       0,     0,     0,     0,  -309,  -309,   244,  -309,     0,     0,
       0,     0,  -309,     0,     0,     0,   245,     0,     0,     0,
    -309,     0,     0,     0,     0,   246,  -309,   247,     0,   248,
       0,   249,  -309,     0,   250,     0,  -309,     0,   251,  -309,
     252,  -309,     0,   253,     0,     0,     0,     0,   230,     0,
    -309,     0,     0,     0,     0,     0,     0,  -309,  -309,     0,
       0,     0,     0,   231,   232,  -309,   233,     0,     0,     0,
       0,   234,     0,     0,     0,  -309,     0,     0,     0,   235,
       0,     0,     0,     0,  -309,   236,  -309,     0,  -309,     0,
    -309,   237,     0,  -309,     0,   238,     0,  -309,   239,  -309,
     240,     0,  -309,     0,     0,     0,     0,  -310,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,     0,     0,
       0,     0,  -310,  -310,   244,  -310,     0,     0,     0,     0,
    -310,     0,     0,     0,   245,     0,     0,     0,  -310,     0,
       0,     0,     0,   246,  -310,   247,     0,   248,     0,   249,
    -310,     0,   250,     0,  -310,     0,   251,  -310,   252,  -310,
       0,   253,     0,     0,     0,     0,     0,     0,  -310,     0,
       0,     0,     0,     0,     0,  -310,  -310,     0,     0,     0,
       0,     0,     0,  -310,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -310,     0,     0,     0,     0,     0,     0,
       0,     0,  -310,     0,  -310,     0,  -310,     0,  -310,     0,
       0,  -310,     0,     0,     0,  -310,     0,  -310,     0,     0,
    -310
};

static const yytype_int16 yycheck[] =
{
       2,     2,   181,   425,    27,   265,    63,   165,   552,    46,
     617,   401,    63,    68,   111,   590,   391,    63,   492,     2,
     223,   629,   579,   677,    26,   679,   795,   516,   830,   625,
     290,   132,    87,    70,   547,   381,     1,   878,   825,   789,
       1,     1,   961,    26,    67,    76,    33,   102,    25,   403,
      23,    33,   848,     5,    33,   523,    48,    57,   203,   229,
     886,    57,   437,   438,    41,    42,    43,    47,    62,   329,
      33,     0,    36,    75,   123,   120,     3,    79,    80,     1,
      47,     1,   261,   147,     3,   165,   117,    67,   120,   518,
     120,    33,    75,    33,    33,   265,    79,    80,   118,   101,
      28,    33,   170,   105,   172,   147,    86,    57,   483,   484,
      48,   175,     1,    33,   489,    76,   545,    90,   101,    86,
     290,   170,   105,   120,   118,    48,    48,   147,    55,   103,
     175,   173,   147,    25,   114,    48,    55,   963,     2,   112,
     171,    33,    62,   175,    33,   175,   110,   114,     3,    41,
      42,    43,    80,    57,    92,   147,   117,   120,   173,   329,
     125,   629,    26,    57,    92,   147,   147,    48,   147,    92,
      57,  1090,   172,    62,  1015,    68,   172,   104,   120,   775,
     120,   146,   147,   170,     3,   954,   106,   115,   170,   543,
     171,   170,   961,   165,    87,   147,   170,   174,   118,   171,
      55,   576,    48,   125,   761,  1046,  1047,   170,   173,   102,
     171,    75,   172,   215,   964,    79,    80,   106,  1014,   171,
     365,   481,   482,  1010,   146,   147,   146,   119,   170,   118,
     170,   170,   215,   165,   147,    28,    55,   101,   170,   120,
     277,   105,   279,   165,    48,   165,   425,   593,   737,   104,
     170,    48,   147,    47,   829,    48,    19,   146,    84,    52,
      32,    87,   175,   165,   604,   605,   147,   269,   608,   609,
     172,  1112,   274,    67,   120,    94,   446,   110,   173,   380,
     793,   170,   174,   125,    77,   104,   269,    80,    92,   326,
      25,   274,    86,  1134,   175,    92,   556,    25,    33,    92,
     147,   147,   141,  1072,  1106,    33,    41,    42,    43,   151,
     149,   481,   482,    41,    42,    43,    79,   110,   147,    91,
     114,  1090,   115,    95,   147,   147,   173,   147,   531,   175,
     125,    51,   535,    92,    54,    98,   229,   165,   496,   935,
      79,    80,   165,   171,   173,   402,   516,   349,   349,   171,
     113,   215,   104,   173,   962,    75,   147,   152,    78,   154,
     155,   363,   157,   158,    54,    85,   349,   390,    28,   141,
     372,   172,   265,   147,   146,   398,   147,   149,   147,    12,
     363,   153,   173,   385,   119,    75,   556,   876,    48,   372,
     110,   119,    52,   596,   147,    85,   172,   290,   174,   173,
     171,   147,   385,    57,   173,   269,  1060,  1061,    57,   579,
     274,   147,   165,   147,    28,   470,   471,    77,   147,    57,
      80,   476,    82,   425,   479,   480,   983,   173,  1003,   918,
     147,   165,    92,     2,    48,   147,   329,   173,    52,   174,
     147,   147,   147,  1050,   173,   147,   174,   172,   922,    91,
     110,   147,   147,    95,    57,   115,   173,    26,   165,   165,
     165,   173,   147,    77,   147,   488,    80,     2,    82,   165,
     165,   173,   812,   813,   814,   897,   147,    57,    92,   147,
     165,   172,   165,   174,   859,   349,   147,   544,   878,    79,
      80,    26,    32,   544,   165,   870,   110,   165,   544,   363,
     172,   115,   174,    57,   165,   507,    75,   509,   372,   165,
      79,    80,     9,   173,   569,   570,   172,   141,   174,   172,
     617,   385,   146,     3,   507,   149,   509,   291,   292,   153,
    1105,  1138,   101,  1142,    57,    91,   105,   101,   913,    95,
      75,    21,   917,    91,    79,    80,    91,    95,    57,    91,
      95,    91,   575,    95,    34,    95,   172,   172,   174,   173,
       5,   936,   937,   600,   601,    91,   101,   737,    57,    95,
     105,   608,   609,    32,   172,    55,   174,   470,   471,   569,
     570,   583,   583,   476,    64,    57,   479,   480,   481,   482,
      57,   761,  1201,  1202,  1203,  1204,  1205,  1206,   149,    57,
     583,   141,   142,   170,  1148,   980,   146,   982,   148,   149,
     172,    57,   174,   153,   172,   146,   174,   170,   165,   165,
     160,   164,   172,   516,   104,  1015,   166,   167,   168,   787,
     110,   165,    91,   165,    57,   165,    95,   165,   165,    86,
     165,   170,   147,   507,   165,   509,   215,   165,     2,   173,
     172,   146,   172,  1197,   824,   825,  1046,   172,   165,   172,
     147,  1051,   165,   556,     9,   173,   172,     3,    67,    80,
     170,   125,    26,   165,    32,   149,   569,   570,    70,   172,
     215,   125,   141,  1023,  1024,   173,   579,   146,   125,   148,
     149,    28,   850,   125,   153,   147,  1036,  1037,    67,   125,
     269,   160,    35,    35,   759,   274,   876,   166,   167,   168,
     173,    48,   173,   146,    22,    52,   173,   170,   897,   583,
     147,    75,  1112,   778,   119,    79,    80,   125,   106,   172,
     171,   149,   172,    91,   269,   149,   149,    95,   173,   274,
      77,   149,   149,    80,   149,    82,   149,   101,   918,   149,
     141,   105,   754,   149,   175,    92,   149,    33,  1098,  1099,
    1100,  1101,   149,   765,   149,   149,   768,   149,   149,   173,
     172,   165,    35,   110,    35,   165,   813,   814,   115,   165,
     349,   146,   765,   141,   142,   768,   144,   145,   146,    32,
     148,   149,   150,   165,   363,   153,   154,   155,   165,   157,
     158,   159,   160,   372,   165,   163,   147,   171,   166,   167,
     168,   173,   165,   983,   349,   170,   385,    48,   175,   177,
     171,   878,    35,  1083,  1084,   175,   173,   884,   363,   175,
     165,   868,   855,   172,   165,   872,   893,   372,   172,   146,
     165,   147,    33,   173,   737,    28,   175,    35,    91,   173,
     385,   165,    95,   175,   175,   857,   173,   173,   147,   861,
     165,   215,   164,   164,   866,    48,   759,   869,   761,    52,
     110,   983,    25,   372,   857,   578,   264,   822,   861,   365,
     873,   976,   338,   866,   529,   778,   869,   389,   897,   544,
     893,  1197,   383,   884,    77,   304,  1053,    80,   141,   142,
    1207,   765,   145,   146,   768,   148,   149,    32,  1200,    92,
     153,   130,   849,  1083,  1084,   269,   683,   160,   526,    21,
     274,   774,    -1,   166,   167,   168,    -1,   110,    -1,    -1,
      -1,    -1,   115,     2,   936,   937,   922,    -1,   507,    -1,
     509,   943,    -1,   945,    -1,    -1,   948,  1126,    -1,   972,
    1047,    -1,    -1,    -1,    -1,    -1,    -1,    26,  1015,    -1,
     943,    -1,   945,    -1,    -1,   948,    91,    -1,    -1,   971,
      95,    -1,   507,    -1,   509,    -1,    -1,    -1,    -1,   981,
      -1,    -1,    -1,   876,    -1,    -1,    -1,    -1,   971,    -1,
      -1,    -1,    -1,   857,    -1,   349,    -1,   861,   981,  1036,
    1037,    -1,   866,    -1,    -1,   869,    75,    -1,    -1,   363,
      79,    80,    -1,    -1,   583,    -1,   141,    -1,   372,    -1,
    1022,   146,    28,   148,   149,   918,  1028,    -1,   153,    -1,
      -1,   385,   101,    -1,    -1,   160,   105,  1134,    -1,  1022,
      -1,  1138,    48,   168,    -1,  1028,    52,  1226,   583,  1228,
      -1,    -1,  1231,  1232,    -1,  1112,    -1,  1094,    -1,    -1,
      -1,    -1,    -1,  1100,  1101,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    80,    -1,    82,  1079,    -1,   943,
    1117,   945,    -1,    -1,   948,    -1,    92,    -1,    -1,    -1,
     983,    -1,  1129,    -1,    -1,  1132,  1079,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,   971,    -1,   115,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   981,  1120,    -1,
    1122,    -1,    -1,    -1,    -1,  1127,    -1,    -1,  1130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1120,    -1,  1122,
      -1,    -1,    -1,    -1,  1127,    -1,   215,  1130,    -1,    -1,
      -1,    -1,    -1,   507,    -1,   509,    -1,  1159,  1022,    -1,
      -1,    -1,    -1,  1165,  1028,    -1,    -1,  1169,  1170,    91,
      -1,    -1,    -1,    95,    -1,    -1,  1159,    -1,    -1,  1181,
      -1,    -1,  1165,  1185,    -1,  1187,  1169,  1170,    -1,    -1,
    1083,  1084,    -1,    -1,    -1,    -1,   765,    -1,  1181,   768,
     269,    -1,  1185,    -1,  1187,   274,  1208,  1208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1079,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,  1208,   148,   149,   150,   583,
     765,   153,    -1,   768,    -1,    -1,    -1,   159,   160,    -1,
      -1,   163,    -1,    -1,   166,   167,   168,    -1,  1250,  1251,
    1252,  1253,    -1,    -1,    -1,   177,  1120,    -1,  1122,    -1,
      -1,    -1,     2,  1127,    -1,    -1,  1130,  1250,  1251,  1252,
    1253,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     349,    -1,    -1,    -1,    -1,    -1,    26,    -1,   857,    -1,
      -1,    -1,   861,    -1,   363,  1159,    -1,   866,    -1,    -1,
     869,  1165,    -1,   372,    -1,  1169,  1170,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,   385,  1181,    -1,    -1,
      -1,  1185,   857,  1187,    -1,    -1,   861,    -1,    -1,    -1,
      -1,   866,    -1,    -1,   869,    75,    -1,    -1,    -1,    79,
      80,    -1,    -1,    -1,  1208,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   101,    -1,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,   943,    -1,   945,    95,    -1,   948,
      -1,    -1,    -1,    -1,    -1,    -1,  1250,  1251,  1252,  1253,
     169,    -1,    32,   172,    -1,   174,    -1,    -1,    -1,    -1,
      -1,    -1,   971,    -1,    -1,    -1,    -1,    -1,   943,    -1,
     945,   765,   981,   948,   768,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,    -1,    -1,    -1,   153,   971,    -1,   507,    -1,
     509,   159,   160,    -1,    -1,   163,   981,    -1,   166,   167,
     168,    91,    -1,  1022,    -1,    95,    -1,    -1,    -1,  1028,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,    23,    -1,
      -1,    -1,    27,    28,    -1,   215,    -1,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    -1,  1022,    -1,    44,
      -1,    26,    -1,  1028,    -1,    -1,    51,    -1,    -1,    -1,
      -1,   141,   142,   857,   144,   145,   146,   861,   148,   149,
    1079,    -1,   866,   153,   583,   869,    -1,    -1,    73,    -1,
     160,    -1,    -1,    78,    -1,    80,   166,   167,   168,   269,
      -1,    -1,    -1,    -1,   274,    90,    -1,    92,    -1,    -1,
      75,    -1,    -1,    -1,  1079,    -1,    -1,    -1,    -1,    -1,
      -1,  1120,    -1,  1122,    -1,   110,    -1,   112,  1127,    -1,
     115,  1130,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       5,    -1,    -1,    -1,    -1,  1120,    -1,  1122,    -1,   943,
    1159,   945,  1127,    -1,   948,  1130,  1165,    -1,    23,    -1,
    1169,  1170,    27,    28,    -1,    -1,    -1,    -1,    -1,   349,
      -1,    36,  1181,    38,    39,    -1,  1185,   971,  1187,    44,
      -1,    -1,    -1,   363,  1159,    -1,    51,   981,    -1,    -1,
    1165,    -1,   372,     2,  1169,  1170,    -1,    -1,    -1,  1208,
      -1,    -1,    -1,    -1,    -1,   385,  1181,    -1,    73,    -1,
    1185,    -1,  1187,    78,    -1,    80,    -1,    26,    -1,    84,
      -1,    -1,    87,    -1,    -1,    90,    -1,    92,  1022,    -1,
      -1,    -1,    -1,  1208,  1028,    -1,    -1,    -1,    -1,    -1,
     215,  1250,  1251,  1252,  1253,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,   765,    -1,    -1,   768,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      79,    80,    -1,    -1,    -1,  1250,  1251,  1252,  1253,    -1,
      -1,    -1,    -1,    -1,    -1,  1079,    -1,    -1,    -1,    -1,
      -1,    -1,   101,    -1,   269,    -1,   105,    -1,    -1,   274,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1120,   507,  1122,   509,
      -1,    18,    -1,  1127,    21,    -1,  1130,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   857,    36,
      -1,    -1,   861,    -1,    -1,    -1,    -1,   866,    -1,    46,
     869,    -1,    -1,    -1,    -1,  1159,    53,    -1,    55,    -1,
      -1,  1165,    -1,    -1,   349,  1169,  1170,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    -1,  1181,   363,    -1,
      -1,  1185,    -1,  1187,    -1,    -1,    -1,   372,    -1,    -1,
      -1,    -1,    89,   583,    -1,    -1,   215,    -1,    -1,    -1,
     385,    -1,    -1,   100,  1208,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,    26,    -1,    -1,   116,
      -1,    -1,    -1,    -1,   943,    -1,   945,    -1,    -1,   948,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1250,  1251,  1252,  1253,
     269,    -1,   971,    -1,    -1,   274,    -1,    -1,    -1,    -1,
      -1,    -1,   981,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      -1,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,
      -1,    21,    -1,    -1,    -1,   105,    26,    -1,    -1,    -1,
      -1,    -1,    -1,  1022,    34,    -1,    -1,    -1,    -1,  1028,
      40,    -1,    -1,    -1,    -1,    -1,    46,     5,    -1,    -1,
      50,    -1,   507,    53,   509,    55,    -1,    -1,    -1,    -1,
     349,    -1,    -1,    -1,    64,    23,    -1,    -1,    -1,    27,
      28,    71,    72,    -1,   363,    -1,    -1,    -1,    36,    79,
      38,    39,    -1,   372,    -1,    -1,    44,    -1,    -1,    89,
    1079,    -1,    -1,    51,    -1,    -1,   385,    -1,    98,    -1,
     100,    -1,   102,    -1,   104,   765,    -1,   107,   768,    -1,
      -1,   111,    -1,   113,    -1,    73,   116,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    84,    -1,   583,    87,
      -1,  1120,    90,  1122,    92,   215,    -1,    -1,  1127,    -1,
      -1,  1130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1159,    -1,   172,    -1,    -1,    -1,  1165,    -1,    -1,    -1,
    1169,  1170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   269,
      -1,    -1,  1181,    -1,   274,     1,  1185,   857,  1187,     5,
      -1,   861,    -1,    -1,    -1,    -1,   866,   165,    -1,   869,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    23,   507,  1208,
     509,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1250,  1251,  1252,  1253,    -1,    -1,    73,    -1,    75,
      -1,    -1,    78,    -1,    80,    -1,    82,     3,    84,    85,
      -1,    87,    -1,   943,    90,   945,    92,    -1,   948,    -1,
      -1,    -1,    18,    19,   583,    21,    -1,    -1,    -1,    -1,
      26,    -1,    -1,    -1,   110,   385,   112,    -1,    34,   115,
     765,   971,    -1,   768,    40,    -1,    -1,    -1,     5,    -1,
      46,   981,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    64,    -1,
      27,    28,    -1,    -1,    -1,    71,    72,    -1,    -1,    36,
      -1,    38,    39,    79,    -1,    -1,    -1,    44,    -1,   165,
      -1,    -1,  1022,    89,    51,   171,    -1,    -1,  1028,    -1,
      -1,    -1,    98,    -1,   100,    -1,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    73,   113,    -1,    -1,
     116,    78,    -1,    80,    -1,    82,    -1,    84,    -1,    -1,
      87,    -1,   857,    90,    -1,    92,   861,    -1,    -1,    -1,
      -1,   866,    -1,    -1,   869,    -1,    -1,    -1,    -1,  1079,
      -1,    -1,    -1,   110,    32,   112,    -1,   507,   115,   509,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   170,    -1,   172,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1120,    -1,  1122,    -1,    -1,    -1,    -1,  1127,    -1,    -1,
    1130,    -1,    -1,    -1,    -1,    -1,   765,    -1,   165,   768,
      -1,    -1,    -1,    91,   171,    -1,    -1,    95,   943,    -1,
     945,    -1,    -1,   948,    -1,    -1,    -1,    -1,    -1,  1159,
      -1,    -1,    -1,    -1,    -1,  1165,    -1,    -1,    -1,  1169,
    1170,    -1,    -1,    -1,    -1,    -1,   971,    -1,    -1,    -1,
      -1,  1181,    -1,    -1,    -1,  1185,   981,  1187,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,  1208,   157,
     158,   159,   160,   161,    -1,   163,    -1,    -1,   166,   167,
     168,    -1,    -1,     5,    -1,    -1,    -1,  1022,   857,   177,
      -1,    -1,   861,  1028,    -1,    -1,    -1,   866,    -1,    -1,
     869,    23,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,
    1250,  1251,  1252,  1253,    36,    -1,    38,    39,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1079,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    84,    -1,    -1,    87,    -1,    -1,    90,    -1,
      92,    -1,    -1,    -1,   943,    -1,   945,    -1,    -1,   948,
      -1,    -1,    -1,    -1,    -1,  1120,    -1,  1122,   110,    -1,
     112,    -1,  1127,   115,    -1,  1130,    -1,    -1,    -1,    -1,
      -1,    -1,   971,    -1,    -1,   765,    -1,    -1,   768,    -1,
      -1,    -1,   981,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1159,    -1,    -1,    -1,    -1,    -1,
    1165,    -1,    -1,    -1,  1169,  1170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,  1181,    -1,    -1,   171,
    1185,    -1,  1187,  1022,    -1,    -1,    -1,    -1,    -1,  1028,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    -1,   857,    -1,    -1,
      27,   861,    -1,    -1,    -1,    -1,   866,    34,    -1,   869,
    1079,    -1,    -1,    -1,    -1,  1250,  1251,  1252,  1253,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    54,    -1,    56,
      -1,    58,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    -1,    -1,    -1,    75,    -1,
      77,  1120,    -1,  1122,    -1,    -1,    -1,    84,  1127,    -1,
      87,  1130,    89,    -1,    91,    -1,    -1,    94,    95,    -1,
      97,    -1,    -1,    -1,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,   943,    -1,   945,    -1,    -1,   948,    -1,
    1159,    -1,    -1,    -1,    -1,    -1,  1165,    -1,    -1,    -1,
    1169,  1170,    -1,   130,    -1,   132,    -1,    -1,    -1,    -1,
      -1,   971,  1181,    -1,    -1,    -1,  1185,     3,  1187,    -1,
      -1,   981,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    19,    -1,    21,    -1,    -1,    -1,  1208,
      26,    -1,    -1,    -1,    -1,    -1,    -1,   174,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,  1022,    -1,    50,    -1,    -1,    53,  1028,    55,
     197,    -1,    -1,    -1,   201,   202,    -1,    -1,    64,    -1,
      -1,  1250,  1251,  1252,  1253,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   229,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    -1,   100,    -1,   102,    -1,   104,  1079,
      -1,   107,    -1,    -1,    -1,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   265,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1120,    -1,  1122,   290,    46,    -1,    -1,  1127,    -1,    -1,
    1130,    -1,    -1,    -1,   301,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,    70,   316,
     317,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,  1159,
      -1,    -1,   329,    -1,    -1,  1165,   333,    -1,    -1,  1169,
    1170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1181,    -1,    -1,   351,  1185,    -1,  1187,   110,    -1,
      -1,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   373,    -1,   375,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,   389,   390,   391,    -1,    -1,    -1,    -1,    -1,
      -1,   398,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1250,  1251,  1252,  1253,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   434,   435,   436,
     437,   438,   439,    -1,   441,   442,   443,   444,   445,    -1,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,    -1,    -1,   221,
      -1,    -1,    -1,   470,   471,    -1,    -1,    -1,    -1,   476,
      -1,    -1,   479,   480,   481,   482,   483,   484,    -1,    -1,
      -1,   488,   489,    29,    -1,   492,    -1,    -1,    -1,    -1,
      -1,   498,    -1,    -1,    -1,    41,    42,    43,   260,   261,
      -1,    -1,   509,    -1,    -1,   512,    -1,    -1,    -1,   516,
      -1,    -1,    -1,    -1,    -1,   277,   523,   279,   525,    -1,
      -1,    -1,   529,    -1,   286,   532,   533,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   544,    -1,    -1,
     547,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   556,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   566,
      -1,    -1,   569,   570,   326,    -1,    -1,    -1,   575,   576,
      -1,    -1,   579,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   590,   591,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   603,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,
      -1,    -1,   629,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,    -1,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,   199,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    25,    26,    -1,    -1,    -1,   674,    -1,    -1,
      -1,    34,    -1,    36,   681,    -1,    -1,    40,    41,    42,
      43,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,   258,   259,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,   270,    88,    89,    90,    -1,   736,
     737,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
     113,    -1,   759,   116,   761,   762,   763,    -1,    -1,    -1,
      -1,   124,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,   778,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,   793,   150,    -1,    -1,
     797,    -1,    -1,    -1,    -1,    -1,   159,   559,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,   359,   177,   822,    -1,    -1,    -1,   826,
      -1,   828,   829,   830,    -1,   832,    -1,   834,    -1,    -1,
     837,   838,    -1,    -1,    -1,    -1,    -1,    -1,   600,   601,
      -1,    -1,    -1,    -1,   851,    -1,   608,   609,   855,   395,
      -1,   397,   859,   860,   861,   401,   863,   864,    -1,   866,
      -1,    -1,   869,   870,    -1,    -1,    -1,    -1,    -1,   876,
      -1,    -1,    -1,    -1,   881,    -1,    -1,    -1,    -1,   425,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   913,   914,   915,   916,
     917,   918,    -1,    -1,    -1,   922,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   936,
     937,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   946,
      -1,    -1,    -1,    -1,   490,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   962,    -1,    -1,    -1,   966,
      -1,    -1,    -1,    -1,    -1,   972,    -1,    -1,    -1,    -1,
      -1,     3,   518,   980,    -1,   982,   983,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   531,    18,    19,    -1,    21,
      -1,    -1,    -1,    -1,    26,   541,  1003,    -1,    -1,   545,
      -1,    -1,    34,  1010,    -1,  1012,    -1,   769,    40,    -1,
      -1,    -1,  1019,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,  1034,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,   803,    -1,   589,    -1,    -1,    -1,    79,  1055,    -1,
      -1,   813,   814,   599,    -1,    -1,    -1,    89,    -1,    -1,
    1067,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,    -1,
     102,   617,   104,    -1,    -1,   107,  1083,  1084,    -1,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,  1096,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1104,  1105,  1106,
      -1,    -1,    -1,    -1,    -1,    -1,   868,    -1,    -1,    -1,
     872,    -1,   874,  1120,    -1,  1122,    -1,    -1,  1125,    -1,
    1127,    -1,    -1,  1130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   683,    -1,    -1,
     172,    -1,    -1,    -1,    -1,  1152,  1153,    -1,  1155,  1156,
      -1,    -1,    -1,    -1,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1175,    -1,
      -1,  1178,    23,    -1,    -1,  1182,    27,    28,    -1,    -1,
    1187,  1188,    -1,    -1,  1191,    36,    -1,    38,    39,    -1,
      -1,    -1,  1199,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,  1213,  1214,  1215,  1216,
    1217,    -1,    -1,    -1,    -1,    -1,    -1,   979,    -1,    -1,
      -1,    -1,    73,    -1,   770,    -1,    -1,    78,   774,    80,
      -1,    82,    -1,    84,    -1,    -1,    87,    -1,   784,    90,
      -1,    92,    -1,  1250,  1251,  1252,  1253,  1254,  1255,  1256,
    1257,    -1,    -1,    -1,    -1,     3,    -1,   803,    -1,   110,
      -1,   112,    -1,    -1,   115,    -1,    32,    -1,    -1,    -1,
      18,    19,    -1,    21,  1036,  1037,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,   849,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    32,    71,    72,    91,    -1,   873,    -1,    95,
      -1,    79,  1094,    -1,    -1,    -1,    -1,    -1,  1100,  1101,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,   897,   100,    -1,   102,  1117,   104,    -1,   124,   107,
      -1,    -1,    -1,   111,  1126,   113,    -1,  1129,   116,    -1,
    1132,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    91,   148,   149,   150,    95,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,    -1,    -1,  1167,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,
     976,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,    -1,   163,    -1,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,  1226,    -1,  1228,   177,    -1,  1231,
    1232,    -1,     0,     1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,  1050,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,    -1,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    -1,
      -1,   119,  1138,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      13,    -1,    15,    16,    17,    18,    19,    20,    21,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    66,    67,    68,    69,    -1,    71,    72,
      73,    -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,   105,    -1,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,    -1,   170,   171,   172,
      -1,   174,    -1,     1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,    -1,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,   171,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    16,    17,    18,    19,    -1,    21,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    66,    67,    68,    69,    -1,    71,    72,
      73,    -1,    75,    -1,    -1,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,   105,    -1,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,   171,   172,
      -1,   174,    -1,     1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,    -1,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,   171,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    16,    17,    18,    19,    -1,    21,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    66,    67,    68,    69,    -1,    71,    72,
      73,    -1,    75,    -1,    -1,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,   105,    -1,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,   171,   172,
      -1,   174,    -1,     1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,    -1,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    16,    17,    18,    19,    -1,    21,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    66,    67,    68,    69,    -1,    71,    72,
      73,    -1,    75,    -1,    -1,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,   105,    -1,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    -1,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,    -1,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    16,    17,    18,    19,    -1,    21,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    66,    67,    68,    69,    -1,    71,    72,
      73,    -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,   105,    -1,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    -1,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,    -1,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    16,    17,    18,    19,    -1,    21,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    66,    67,    68,    69,    -1,    71,    72,
      73,    -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,   105,    -1,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    -1,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,    -1,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    16,    17,    18,    19,    -1,    21,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    55,    -1,    57,    58,    59,    60,    61,    -1,
      63,    64,    65,    66,    67,    68,    69,    -1,    71,    72,
      73,    -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    84,    85,    86,    87,    88,    89,    90,    -1,    92,
      93,    94,    -1,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,   105,    -1,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
       1,   174,     3,     4,   177,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    25,    26,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    43,    -1,    45,    46,    -1,    48,    -1,    50,
      -1,    52,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    77,    -1,    79,    80,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,
     111,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,   173,   174,     3,     4,   177,     6,     7,     8,
       9,    10,    11,    -1,    13,    -1,    15,    -1,    -1,    18,
      19,    20,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    56,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,    -1,   141,   142,   143,   144,   145,   146,    -1,   148,
      -1,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,    -1,   172,    -1,    -1,     3,     4,   177,     6,
       7,     8,     9,    10,    11,    -1,    13,    -1,    15,    -1,
      -1,    18,    19,    20,    21,    -1,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    56,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,    -1,   141,   142,   143,   144,   145,   146,
      -1,   148,    -1,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,   166,
     167,   168,   169,   170,    -1,   172,    -1,    -1,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    16,    -1,    18,    19,    -1,    21,
      -1,    23,    24,    -1,    26,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,
      -1,    63,    64,    65,    66,    -1,    68,    69,    -1,    71,
      72,    73,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,
      -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
     102,    -1,   104,   105,    -1,   107,    -1,    -1,    -1,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,    -1,     3,     4,   177,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,
      -1,    21,    -1,    23,    -1,    -1,    26,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    45,    46,    -1,    48,    -1,
      50,    -1,    52,    53,    -1,    55,    -1,    57,    -1,    -1,
      60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,
      -1,    71,    72,    -1,    -1,    -1,    -1,    77,    -1,    79,
      80,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,
      90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
     110,   111,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,     3,     4,   177,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,    27,
      28,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,
      -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,
      68,    69,    -1,    71,    72,    73,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    -1,    83,    -1,    -1,    -1,    -1,
      88,    89,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,   113,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,    -1,     3,     4,   177,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    43,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,
      66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,   112,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,     3,
       4,   177,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,
      -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    43,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,
      64,    -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,     3,     4,   177,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,
      -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,
      -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,    91,
      -1,    -1,    -1,    95,    -1,    -1,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,   147,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,   171,
     172,    -1,    -1,     3,     4,   177,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,
      -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,
      60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,   108,   109,
      -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,     3,     4,   177,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,
      -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,
      68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
     108,   109,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,    -1,     3,     4,   177,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,
      66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,   108,   109,    -1,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,   173,    -1,     3,
       4,   177,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,
      64,    -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,   108,   109,    -1,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
      -1,   175,    -1,   177,     3,     4,    -1,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,   108,
     109,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,   173,    -1,     3,     4,   177,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,   108,   109,    -1,   111,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,   173,    -1,     3,     4,
     177,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,
      -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,
      -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,   108,   109,    -1,   111,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,   173,    -1,
       3,     4,   177,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,   108,   109,    -1,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,    -1,     3,     4,   177,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,   147,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
     171,   172,    -1,    -1,     3,     4,   177,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,   147,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,    -1,     3,     4,   177,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
     147,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,   171,   172,    -1,    -1,     3,     4,
     177,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,
      -1,    26,    -1,    28,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,
      -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,
      -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,
       3,     4,   177,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,   108,   109,    -1,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,    -1,     3,     4,   177,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      91,    -1,    -1,    -1,    95,    -1,    -1,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,     3,     4,   177,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,   122,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,    -1,   175,    -1,   177,     3,
       4,    -1,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,
      64,    -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,   147,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,   173,
      -1,     3,     4,   177,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,
      -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,
      -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,    -1,     3,     4,   177,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,
      -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,
      60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,     3,     4,   177,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,
      -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,
      68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,    -1,   175,    -1,   177,
       3,     4,    -1,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,   171,   172,
      -1,    -1,     3,     4,   177,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,   175,    -1,   177,     3,     4,    -1,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,
      66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,   175,
      -1,   177,     3,     4,    -1,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,     3,     4,   177,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,    -1,     3,     4,   177,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,    -1,     3,     4,
     177,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,
      -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,
      -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,
       3,     4,   177,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,    -1,     3,     4,   177,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,     3,     4,   177,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,    -1,    -1,     4,   177,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    23,    -1,    25,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    41,    42,    43,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    66,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    80,    81,    82,    83,    -1,    -1,    -1,
      -1,    88,    -1,    90,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,   173,   174,    -1,     4,
     177,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    19,    -1,    -1,    -1,    23,    -1,
      25,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    41,    42,    43,    -1,
      45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,
      -1,    66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    80,    81,    82,    83,    -1,
      -1,    -1,    -1,    88,    -1,    90,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    98,    99,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,     3,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    13,    -1,   150,    -1,    -1,    18,    19,
      20,    21,    -1,    -1,   159,    -1,    26,   162,   163,   164,
      -1,    -1,    -1,   168,    34,   170,    36,   172,    -1,   174,
      40,    -1,   177,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
     100,    -1,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,    -1,   138,   139,
      -1,   141,   142,   143,   144,   145,   146,    -1,   148,    -1,
      -1,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,   163,    -1,    -1,   166,   167,   168,   169,
       3,     4,   172,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      43,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,     3,     4,    -1,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    43,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,   143,    -1,     3,     4,    -1,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,   164,    23,    -1,    25,    26,    -1,   170,
      -1,   172,    -1,   174,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    43,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,     3,
      -1,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,    13,
      -1,    -1,    -1,    -1,    18,    19,    20,    21,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,   141,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    56,    -1,    -1,   164,    -1,    -1,    -1,    -1,
      64,    -1,    -1,   172,    -1,   174,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,   100,    -1,   102,    -1,
     104,    -1,    -1,    -1,    -1,    -1,   110,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,    -1,   138,   139,    -1,   141,   142,   143,
     144,   145,   146,    -1,   148,    -1,    -1,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,     3,   163,
      -1,    -1,   166,   167,   168,   169,   170,    -1,    13,    -1,
      -1,    -1,    -1,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    -1,    98,    -1,   100,    -1,   102,    20,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,    -1,
      32,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,   143,   144,
     145,   146,    -1,   148,    -1,    -1,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,   163,    -1,
      -1,   166,   167,   168,   169,    -1,    -1,    -1,    -1,    91,
      -1,    13,    -1,    95,    -1,    -1,    -1,    -1,    20,    -1,
      -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,   170,    91,
      -1,    13,    -1,    95,    -1,   177,    -1,    -1,    20,    -1,
      -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,   170,    91,
      -1,    13,    -1,    95,    -1,   177,    -1,    -1,    20,    -1,
      -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,   170,    91,
      -1,    13,    -1,    95,    -1,   177,    -1,    -1,    20,    -1,
      -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,   170,    91,
      -1,    13,    -1,    95,    -1,   177,    -1,    -1,    20,    -1,
      -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,   170,    91,
      -1,    -1,    -1,    95,    -1,   177,    -1,    -1,    -1,    -1,
      13,   103,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    48,   138,   139,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    13,   177,    -1,    -1,    91,    -1,
      -1,    20,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    48,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    91,   166,   167,   168,    95,   170,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    32,   124,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    74,   166,   167,   168,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    14,    -1,    -1,    -1,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,   124,    -1,    -1,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    74,   165,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    91,    13,    -1,    -1,    95,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,    91,    -1,    13,    -1,    95,    -1,    -1,   177,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,   170,    91,    -1,    13,    -1,    95,    -1,   177,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,   170,    91,    -1,    13,    -1,    95,    -1,   177,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,   170,    91,    13,    -1,    -1,    95,    -1,   177,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,   177,    -1,
      -1,    -1,    -1,   103,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,   124,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    74,   166,   167,   168,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    13,    -1,
      -1,    -1,    91,    -1,    -1,    20,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    32,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    91,   166,   167,   168,
      95,    -1,    -1,    13,    14,    -1,    -1,    -1,   177,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,   120,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    95,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
     120,   166,   167,   168,   124,    -1,    -1,    -1,    -1,    -1,
     175,    91,   177,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    13,    -1,    -1,   175,    -1,   177,    -1,    20,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    32,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
      -1,    -1,    13,    -1,    -1,    -1,    -1,   177,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    13,    -1,    95,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,   170,
      91,    13,    -1,    -1,    95,    -1,   177,    -1,    20,    -1,
     141,   142,   103,   144,   145,   146,    -1,   148,   149,   150,
      32,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   124,   165,   166,   167,   168,    -1,    -1,
      -1,    13,    14,    -1,    -1,    -1,   177,    -1,    20,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      32,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,    91,
      -1,    -1,    13,    95,    -1,    -1,   177,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      91,   173,    -1,    -1,    95,   177,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   123,   124,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    13,    -1,   166,   167,   168,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    13,    -1,    95,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    91,    13,    14,    -1,    95,    -1,   177,    -1,    20,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    32,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    13,   173,    -1,    -1,    -1,   177,    -1,    20,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    32,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
      91,    -1,    -1,   173,    95,    -1,    -1,   177,    -1,    -1,
      13,    14,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    91,    -1,
     141,   142,    95,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    13,    -1,   166,   167,   168,    -1,    -1,
      20,   124,   173,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    13,    -1,   166,   167,   168,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,   141,   142,    95,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,   165,   166,   167,   168,    -1,
      -1,   123,   124,    -1,    -1,    -1,    -1,   177,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    13,    -1,   166,   167,   168,    -1,    -1,    20,
      -1,    -1,    -1,   175,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    13,    14,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    13,    95,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
     141,   142,    62,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    95,   177,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,   124,   166,   167,   168,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    20,   177,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    32,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,   177,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    13,    -1,   103,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,   141,   142,    95,   144,
     145,   146,    -1,   148,   149,   150,   103,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,   177,    -1,    91,    13,    -1,    -1,    95,    -1,
      -1,    -1,    20,    -1,   141,   142,   103,   144,   145,   146,
      -1,   148,   149,   150,    32,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   124,    -1,   166,
     167,   168,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,
     177,    -1,    20,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    32,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    -1,    91,    -1,    13,    -1,    95,    -1,    -1,
     177,    -1,    20,    -1,    -1,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,   141,   142,   103,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,    -1,    91,    13,    -1,    -1,    95,    -1,   177,
      -1,    20,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    32,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    23,   166,   167,   168,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,   177,    36,
      -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    73,    -1,    75,    -1,
      -1,    78,    -1,    80,    -1,    -1,    -1,    84,    85,    86,
      87,    -1,     1,    90,     3,    92,     5,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      19,    -1,    21,   110,    -1,   112,    -1,    26,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    -1,    -1,    -1,    -1,   165,     3,
      79,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    18,    19,    -1,    21,    -1,    98,
      -1,   100,    26,   102,    28,   104,    -1,    -1,   107,    -1,
      34,    -1,   111,    -1,   113,    -1,    40,   116,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    -1,
       3,    -1,     5,    -1,    98,    -1,   100,    -1,   102,    -1,
     104,    -1,    -1,   107,    -1,    18,    19,   111,    21,   113,
      -1,   115,   116,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      -1,    -1,    -1,    -1,    18,    19,    79,    21,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    98,    40,   100,    -1,   102,
      -1,   104,    46,    -1,   107,    -1,    50,    -1,   111,    53,
     113,    55,    -1,   116,    -1,    -1,    -1,    -1,     3,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,
      -1,    -1,    -1,    18,    19,    79,    21,    -1,    -1,    -1,
      -1,    26,    -1,    -1,    -1,    89,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    98,    40,   100,    -1,   102,    -1,
     104,    46,    -1,   107,    -1,    50,    -1,   111,    53,   113,
      55,    -1,   116,    -1,    -1,    -1,    -1,     3,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    -1,
      -1,    -1,    18,    19,    79,    21,    -1,    -1,    -1,    -1,
      26,    -1,    -1,    -1,    89,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    98,    40,   100,    -1,   102,    -1,   104,
      46,    -1,   107,    -1,    50,    -1,   111,    53,   113,    55,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    -1,   100,    -1,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,   113,    -1,    -1,
     116
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   182,   183,     0,     1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    15,    16,    17,    18,    19,    21,
      23,    24,    26,    27,    28,    29,    30,    31,    34,    36,
      38,    39,    40,    44,    45,    46,    49,    50,    51,    53,
      55,    57,    58,    59,    60,    61,    63,    64,    65,    66,
      68,    69,    71,    72,    73,    78,    79,    80,    81,    82,
      83,    84,    87,    88,    89,    90,    92,    93,    94,    96,
      97,    98,    99,   100,   101,   102,   104,   105,   107,   108,
     109,   110,   111,   112,   113,   115,   116,   121,   124,   141,
     142,   143,   144,   145,   150,   159,   162,   163,   164,   165,
     168,   170,   172,   177,   184,   185,   186,   187,   188,   189,
     192,   193,   194,   195,   200,   201,   202,   204,   205,   206,
     211,   212,   216,   217,   221,   223,   226,   227,   230,   231,
     232,   233,   234,   235,   238,   239,   240,   244,   246,   247,
     250,   251,   252,   253,   254,   258,   259,   264,   265,   266,
     267,   270,   271,   276,   277,   279,   280,   285,   289,   290,
     292,   293,   314,   319,   320,   324,   325,   347,   348,   349,
     351,   353,   354,   355,   361,   362,   363,   364,   365,   366,
     368,   371,   372,   373,   374,   375,   376,   377,   378,   380,
     381,   382,   383,   384,   165,    23,    36,    45,    55,    57,
      90,   102,   170,   239,   250,   277,   347,   354,   362,   363,
     368,   371,   373,   374,   120,   356,   357,     3,    55,   218,
     368,   356,   110,   326,    92,   218,   186,   341,   368,   172,
       3,    18,    19,    21,    26,    34,    40,    46,    50,    53,
      55,    64,    71,    72,    79,    89,    98,   100,   102,   104,
     107,   111,   113,   116,   219,   220,   222,    12,    28,   115,
     245,   368,    84,    87,   202,   172,   104,   220,   220,   220,
     172,   220,   328,   329,    33,   206,   225,   368,   262,   263,
     368,   368,    19,    79,    98,   113,   368,   368,   368,     9,
     172,   229,   228,    28,    34,    48,    50,    52,    77,    80,
      92,   102,   110,   115,   185,   224,   281,   282,   283,   305,
     306,   307,   333,   338,   368,   341,   108,   109,   165,   285,
     286,   367,   368,   370,   368,   225,   368,   368,   101,   172,
     186,   368,   368,    73,   188,   192,   206,   188,   206,   368,
     370,   368,   368,   368,   368,   368,     1,   171,   184,   207,
     341,   151,   342,   343,   370,   368,    82,   186,    23,    36,
      39,    73,    90,   170,   189,   190,   191,   202,   206,   194,
     149,   196,   170,    47,    86,   114,   203,    27,   325,   368,
       9,   266,   368,   369,    25,    33,    41,    42,    43,   119,
     174,   241,   242,   350,   352,   172,   206,    57,   146,   268,
     220,   170,   172,   301,    54,    75,    85,   309,    28,    48,
      52,    77,    80,    92,   110,   115,   310,    28,    48,    52,
      77,    80,    92,   110,   115,   308,   220,   321,   322,     1,
     328,   164,   165,   368,    13,    20,    32,    91,    95,   124,
     141,   142,   144,   145,   146,   148,   149,   150,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     166,   167,   168,   177,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   169,
     297,   172,   174,    91,    95,   368,   220,    41,   174,   241,
      57,     1,   172,   186,   165,   206,   324,   165,   147,   165,
     221,   344,   345,   346,   370,   220,   220,   103,   206,   103,
     125,   206,   297,   203,   344,   165,   172,   186,   172,   220,
     330,   331,     1,   146,   337,    48,   147,   186,   225,   147,
     225,    14,   172,   172,   225,   327,   344,   230,   230,    48,
      92,   306,   307,   173,   147,   172,   220,   146,   165,   368,
     368,   123,   287,   165,   170,   225,   172,   344,   165,   368,
     255,   255,   165,   171,   171,   184,   147,   171,   368,   147,
     147,   173,   173,     9,   368,   174,   241,   242,     3,   172,
     197,     1,   171,   207,   214,   215,   368,   209,   368,    67,
      37,    74,   165,   266,   268,   110,   236,   290,   186,    80,
     245,   368,   122,   175,   243,   341,   368,   379,   243,   368,
     220,   248,   249,   220,   341,   170,     1,   193,   220,   272,
     275,   173,   302,   304,   305,   310,    48,    92,     1,   146,
     335,   336,    48,    92,     1,     3,    13,    18,    20,    21,
      26,    46,    53,    55,    56,    64,    72,    89,   100,   104,
     111,   116,   141,   142,   143,   144,   145,   146,   148,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   163,
     166,   167,   168,   169,   172,   220,   294,   295,   296,   297,
     347,   125,   323,   147,   165,   165,   165,   368,   368,   368,
     243,   368,   243,   368,   368,   368,   368,   368,   368,   368,
       3,    21,    34,    64,   104,   110,   221,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,    70,   370,   370,   370,   370,   370,
     344,   344,   243,   368,   243,   368,   103,   172,   341,   243,
     368,   220,     1,    28,    48,    52,    77,    80,    92,   110,
     115,   173,   358,   359,   360,   368,   384,   328,   368,   125,
     173,   147,   125,   125,   186,    35,   186,   368,    35,   368,
      67,   173,   345,   331,   147,   173,   224,   368,   125,   332,
     368,   329,   262,   220,   324,   368,   368,   324,   173,   220,
     310,   283,   331,   146,   339,   340,   338,   288,   205,     1,
     260,   344,   173,    22,   256,   171,   173,   173,   342,   370,
     173,   342,   341,   243,   368,   196,   173,   198,   199,   346,
     171,   171,   147,   165,    14,   149,   147,   165,    14,    37,
      74,   220,   141,   142,   143,   144,   145,   159,   163,   168,
     208,   296,   297,   298,   368,   208,   210,   268,   170,   237,
     324,   119,   220,   225,   225,   172,   368,   175,   357,    48,
     147,   175,   357,   123,   147,   175,    33,   225,   357,    33,
      48,   225,   357,   147,   173,   165,   172,     1,   269,   171,
     275,   125,   147,   171,   147,   173,   337,     1,   224,   333,
     334,   106,   311,   172,   300,   368,   141,   149,   300,   300,
     368,   322,   172,   174,   165,   165,   165,   165,   165,   165,
     173,   175,   345,    48,   175,    33,    33,    48,   172,   173,
      48,    92,   147,   173,    18,    21,    26,    46,    53,    64,
      72,    89,   100,   111,   116,   347,    91,    91,   165,   370,
     346,   368,   368,    35,   186,    35,    35,   186,   103,   206,
     220,   173,   173,   330,   337,    70,   370,   220,   173,   173,
     328,   340,   146,   284,   173,   338,   151,   299,   332,   368,
     171,    76,   117,   171,   261,   173,   172,   206,   220,   257,
      48,   175,    48,   147,   173,   214,   221,    18,    19,    21,
      26,    46,    50,    53,    72,    79,    89,    98,   100,   102,
     111,   113,   116,   170,   213,   298,   368,   368,   208,   210,
     147,   165,    14,   165,   170,   269,   321,   328,   368,    48,
     341,   186,   175,   243,   368,   175,   186,   368,   175,   368,
     175,   368,   368,   225,    45,   368,   243,   368,   225,   249,
     206,   345,   171,    84,    87,   171,   185,   193,   227,   368,
     273,   274,   304,   311,    62,   118,   315,   302,   303,   173,
     295,   297,   173,   175,   173,   243,   368,    45,   243,   368,
     345,   359,   337,   347,   347,   186,   186,   368,   186,    35,
     165,   165,   332,   172,   172,   165,   299,   332,   333,   311,
     340,   368,   186,   239,   341,   257,   146,   206,   243,   368,
     243,   368,   199,   208,    14,    37,    74,   165,   165,   298,
     368,   368,   269,   171,   165,   165,   165,   368,   173,   186,
     175,   357,   175,   357,   186,   123,   368,    33,   225,   357,
      33,   225,   357,   173,   193,   227,   227,   275,   193,   315,
       3,    55,    94,   104,   316,   317,   318,   368,   291,   173,
     300,   300,   175,   175,   368,    33,    33,   173,   332,    35,
     186,   344,   344,   299,   332,    33,   225,   173,   368,   175,
     175,   171,   368,   208,   210,    14,   171,   225,    45,   186,
     368,   175,    45,   186,   368,   175,   368,   103,    45,   368,
     225,    45,   368,   225,   165,   227,   275,   278,   318,   118,
     147,   125,   152,   154,   155,   157,   158,    62,    33,   165,
     205,   312,   313,    45,    45,   103,    45,    45,   186,   173,
     173,   186,   206,   165,   165,   368,   368,   186,   368,   186,
     368,   368,   368,   313,   368,   317,   318,   318,   318,   318,
     318,   318,   316,   184,   368,   368,   368,   368,   164,   164,
     103,   103,   103,   103,   103,   103,   103,   103,   368,   368,
     368,   368
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   181,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   189,   189,   189,   190,   191,
     191,   192,   193,   193,   194,   195,   196,   196,   197,   197,
     197,   198,   198,   199,   200,   201,   201,   201,   202,   202,
     202,   203,   203,   204,   205,   205,   205,   206,   207,   207,
     208,   208,   208,   208,   208,   208,   209,   209,   209,   209,
     210,   210,   211,   211,   211,   211,   211,   212,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   214,   214,   214,   214,   214,   215,
     215,   216,   217,   217,   217,   217,   217,   217,   218,   218,
     218,   219,   219,   219,   219,   219,   220,   220,   220,   221,
     221,   221,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   224,   224,   224,   225,   225,   226,   226,   227,
     227,   228,   227,   229,   227,   230,   230,   230,   230,   230,
     230,   230,   231,   231,   231,   231,   232,   233,   233,   234,
     235,   235,   235,   236,   235,   237,   235,   238,   239,   240,
     240,   241,   241,   241,   241,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   243,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   245,   245,
     246,   247,   247,   248,   248,   249,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   251,
     251,   251,   251,   251,   251,   252,   252,   252,   253,   253,
     253,   254,   254,   254,   254,   255,   255,   256,   256,   256,
     257,   257,   258,   259,   259,   260,   260,   261,   261,   261,
     262,   262,   262,   263,   263,   264,   265,   265,   266,   267,
     267,   267,   268,   268,   269,   269,   269,   269,   269,   270,
     270,   271,   272,   272,   273,   272,   272,   274,   272,   275,
     275,   276,   278,   277,   279,   280,   280,   280,   281,   281,
     282,   282,   283,   283,   283,   284,   284,   285,   287,   286,
     288,   286,   289,   291,   290,   292,   292,   292,   292,   292,
     293,   294,   294,   295,   295,   295,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   297,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   298,   298,   299,   299,   300,   300,   301,
     301,   302,   302,   303,   303,   304,   305,   305,   305,   305,
     305,   305,   306,   306,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   309,   309,   309,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   311,
     311,   312,   312,   313,   313,   314,   315,   315,   315,   315,
     315,   316,   316,   317,   317,   317,   317,   317,   317,   317,
     318,   318,   318,   319,   320,   320,   321,   321,   322,   323,
     323,   324,   324,   324,   324,   324,   326,   325,   327,   325,
     325,   325,   328,   328,   329,   329,   330,   330,   331,   331,
     331,   332,   332,   332,   333,   334,   334,   334,   335,   335,
     336,   336,   337,   337,   337,   337,   338,   338,   339,   340,
     340,   341,   341,   342,   343,   343,   344,   344,   345,   345,
     346,   346,   347,   347,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   349,   350,   350,   350,   350,
     350,   350,   350,   351,   352,   352,   352,   352,   352,   352,
     352,   353,   354,   355,   355,   355,   355,   355,   355,   355,
     356,   356,   357,   357,   357,   357,   358,   358,   359,   359,
     359,   360,   360,   360,   360,   360,   360,   360,   360,   360,
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
       2,     1,     1,     3,     3,     3,     3,     3,     2,     1,
       1,     1,     1,     1,     2,     1,     3,     3,     3,     3,
       2,     3,     3,     2,     2,     1,     1,     1,     0,     1,
       1,     1,     1,     2,     3,     1,     0,     3,     2,     3,
       0,     1,     3,     1,     4,     3,     4,     4,     0,     1,
       1,     0,     1,     6,     2,     3,     3,     1,     1,     2,
       1,     1,     3,     3,     3,     5,     1,     3,     3,     5,
       0,     1,     4,     6,     8,     6,     8,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     5,     1,
       3,     3,     4,     4,     4,     4,     4,     4,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     2,     3,     1,
       1,     0,     3,     0,     3,     1,     1,     1,     1,     1,
       1,     1,     3,     5,     5,     2,     1,     1,     1,     1,
       6,     7,     3,     0,     6,     0,     6,     2,     0,     3,
       2,     1,     1,     1,     1,     5,     3,     3,     6,     5,
       6,     5,     6,     3,     4,     3,     4,     6,     7,     6,
       7,     4,     5,     4,     5,     4,     4,     3,     6,     5,
       4,     3,     6,     5,     6,     5,     8,     7,     4,     4,
       2,     5,     2,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     6,     4,     7,     5,     3,
       6,     2,     2,     3,     3,     0,     2,     2,     3,     5,
       1,     3,     3,     5,     5,     0,     2,     3,     2,     4,
       4,     3,     1,     1,     3,     3,     5,     5,     2,     1,
       1,     1,     0,     2,     0,     2,     3,     3,     4,     4,
       4,     2,     1,     2,     0,     4,     2,     0,     5,     1,
       3,     1,     0,     8,     0,     1,     1,     1,     0,     1,
       1,     3,     1,     3,     1,     0,     2,     6,     0,     3,
       0,     4,     1,     0,     7,     4,     4,     6,     6,     4,
       2,     1,     3,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     0,     3,     2,
       3,     1,     3,     0,     1,     1,     4,     5,     4,     5,
       6,     6,     0,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     0,     1,     1,     2,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     0,     1,
       1,     2,     1,     1,     1,     1,     2,     1,     1,     0,
       1,     1,     1,     1,     2,     1,     0,     2,     2,     4,
       4,     1,     3,     3,     3,     3,     3,     3,     3,     2,
       1,     1,     1,     3,     1,     2,     1,     3,     2,     0,
       2,     1,     2,     1,     1,     1,     0,     5,     0,     6,
       3,     3,     1,     3,     3,     5,     1,     3,     1,     2,
       3,     0,     2,     2,     1,     1,     1,     1,     2,     1,
       0,     1,     0,     2,     2,     1,     1,     1,     2,     0,
       1,     1,     3,     1,     3,     3,     0,     1,     1,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     4,     5,     2,     2,     1,     2,     1,     2,     1,
       2,     1,     2,     1,     1,     2,     6,     6,     4,     4,
       9,     9,     7,     2,     2,     3,     4,     6,     6,     9,
       9,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     4,     3,     4,     2,     1,     3,     4,     3,
       3,     1,     1,     2,     1,     2,     1,     1,     1,     1,
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
#line 665 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 5985 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 669 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 5991 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 670 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 5997 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 677 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6005 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 685 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6013 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 689 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6022 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 699 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6030 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 704 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6038 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 711 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6044 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 712 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6050 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 713 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6056 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 714 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6062 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 715 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6068 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 716 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6074 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 717 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6080 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 718 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6086 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: TDEFER stmt  */
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
#line 6108 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: try_stmt  */
#line 737 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6114 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: return_stmt  */
#line 738 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6120 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: TINIT TTHIS TSEMI  */
#line 740 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr));
    auto node = Init::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6132 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 748 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6145 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 757 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6158 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 766 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6166 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 770 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6177 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 777 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6186 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 784 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6192 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 785 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6198 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 786 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6204 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 787 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6210 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 788 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6216 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 789 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6222 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 790 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6228 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 792 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6234 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
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
#line 6258 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 814 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6270 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL expr do_stmt  */
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
#line 6291 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL do_stmt  */
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
#line 6311 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TON expr do_stmt  */
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
#line 6332 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSERIAL expr do_stmt  */
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
#line 6353 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL do_stmt  */
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
#line 6373 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSYNC stmt  */
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
#line 6395 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 931 "chpl.ypp"
  {
    auto attributeGroup = context->buildAttributeGroup((yyloc));
    if (attributeGroup != nullptr) {
      context->resetAttributeGroupPartsState();
    }
    context->loopAttributes.push_back(std::move(attributeGroup));
  }
#line 6407 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 956 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6415 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 963 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6424 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 970 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6432 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 974 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6443 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 984 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6449 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 986 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6457 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 990 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6465 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 997 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6474 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 1002 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6484 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 1015 "chpl.ypp"
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
#line 6506 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 1036 "chpl.ypp"
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
#line 6527 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 1053 "chpl.ypp"
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
#line 6548 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1070 "chpl.ypp"
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
#line 6567 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1087 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6573 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1088 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6580 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1090 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6587 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1095 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6593 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1096 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6600 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1102 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6610 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1120 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6619 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1125 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6628 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1130 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6641 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1142 "chpl.ypp"
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
#line 6660 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1160 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6666 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1161 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6672 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1166 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6680 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1170 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6688 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1174 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6697 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1179 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6705 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1183 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6713 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1187 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6722 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1195 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6731 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1200 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6741 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1206 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6750 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1211 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6761 "bison-chpl-lib.cpp"
    break;

  case 90: /* opt_only_ls: %empty  */
#line 1219 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6767 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: renames_ls  */
#line 1220 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6773 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1225 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6782 "bison-chpl-lib.cpp"
    break;

  case 93: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1230 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6794 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1238 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6807 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1247 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6819 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1255 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6832 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1266 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6841 "bison-chpl-lib.cpp"
    break;

  case 114: /* import_expr: expr  */
#line 1301 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6849 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr TDOT all_op_name  */
#line 1305 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6858 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1310 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6867 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TAS ident_use  */
#line 1315 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6877 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1321 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6887 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_ls: import_expr  */
#line 1329 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6893 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_ls TCOMMA import_expr  */
#line 1330 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6899 "bison-chpl-lib.cpp"
    break;

  case 121: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1335 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6909 "bison-chpl-lib.cpp"
    break;

  case 122: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1344 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6917 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1348 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6925 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1352 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6933 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1356 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6941 "bison-chpl-lib.cpp"
    break;

  case 126: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1360 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6949 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1364 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6957 "bison-chpl-lib.cpp"
    break;

  case 128: /* opt_label_ident: %empty  */
#line 1372 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6963 "bison-chpl-lib.cpp"
    break;

  case 129: /* opt_label_ident: TIDENT  */
#line 1373 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6969 "bison-chpl-lib.cpp"
    break;

  case 130: /* opt_label_ident: TINIT  */
#line 1374 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6975 "bison-chpl-lib.cpp"
    break;

  case 139: /* ident_use: TIDENT  */
#line 1400 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6981 "bison-chpl-lib.cpp"
    break;

  case 140: /* ident_use: TINIT  */
#line 1401 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6987 "bison-chpl-lib.cpp"
    break;

  case 141: /* ident_use: TTHIS  */
#line 1402 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6993 "bison-chpl-lib.cpp"
    break;

  case 160: /* scalar_type: TBOOL  */
#line 1428 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6999 "bison-chpl-lib.cpp"
    break;

  case 161: /* scalar_type: TENUM  */
#line 1429 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7005 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TINT  */
#line 1430 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7011 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TUINT  */
#line 1431 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7017 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TREAL  */
#line 1432 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7023 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TIMAG  */
#line 1433 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7029 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TCOMPLEX  */
#line 1434 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7035 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TBYTES  */
#line 1435 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7041 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TSTRING  */
#line 1436 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7047 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TLOCALE  */
#line 1437 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7053 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TNOTHING  */
#line 1438 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7059 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TVOID  */
#line 1439 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7065 "bison-chpl-lib.cpp"
    break;

  case 175: /* do_stmt: TDO stmt  */
#line 1452 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7071 "bison-chpl-lib.cpp"
    break;

  case 176: /* do_stmt: block_stmt  */
#line 1453 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7077 "bison-chpl-lib.cpp"
    break;

  case 177: /* return_stmt: TRETURN TSEMI  */
#line 1458 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7088 "bison-chpl-lib.cpp"
    break;

  case 178: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1465 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7099 "bison-chpl-lib.cpp"
    break;

  case 179: /* class_level_stmt: TSEMI  */
#line 1475 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7107 "bison-chpl-lib.cpp"
    break;

  case 180: /* class_level_stmt: inner_class_level_stmt  */
#line 1479 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7117 "bison-chpl-lib.cpp"
    break;

  case 181: /* $@1: %empty  */
#line 1484 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7124 "bison-chpl-lib.cpp"
    break;

  case 182: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1486 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7133 "bison-chpl-lib.cpp"
    break;

  case 183: /* $@2: %empty  */
#line 1490 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7140 "bison-chpl-lib.cpp"
    break;

  case 184: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1492 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7149 "bison-chpl-lib.cpp"
    break;

  case 192: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1510 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7158 "bison-chpl-lib.cpp"
    break;

  case 193: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1515 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7167 "bison-chpl-lib.cpp"
    break;

  case 194: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1520 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7176 "bison-chpl-lib.cpp"
    break;

  case 195: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1525 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7184 "bison-chpl-lib.cpp"
    break;

  case 196: /* forwarding_decl_start: TFORWARDING  */
#line 1532 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7193 "bison-chpl-lib.cpp"
    break;

  case 197: /* extern_or_export: TEXTERN  */
#line 1539 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7199 "bison-chpl-lib.cpp"
    break;

  case 198: /* extern_or_export: TEXPORT  */
#line 1540 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7205 "bison-chpl-lib.cpp"
    break;

  case 199: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1545 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7215 "bison-chpl-lib.cpp"
    break;

  case 200: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1555 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7225 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1562 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7237 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1570 "chpl.ypp"
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
#line 7259 "bison-chpl-lib.cpp"
    break;

  case 203: /* $@3: %empty  */
#line 1588 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7267 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1592 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-4])), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7276 "bison-chpl-lib.cpp"
    break;

  case 205: /* $@4: %empty  */
#line 1597 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7285 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1602 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yylsp[-5]), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7294 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1610 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7302 "bison-chpl-lib.cpp"
    break;

  case 208: /* no_loop_attributes: %empty  */
#line 1615 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7308 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1618 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7314 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1619 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7320 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1636 "chpl.ypp"
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
#line 7347 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1659 "chpl.ypp"
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
#line 7370 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1678 "chpl.ypp"
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
#line 7393 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1697 "chpl.ypp"
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
#line 7422 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1722 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7431 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: for_loop_kw expr TIN expr task_intent_clause do_stmt  */
#line 1727 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7440 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1732 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7449 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator task_intent_clause do_stmt  */
#line 1737 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7458 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1742 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7467 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw expr task_intent_clause do_stmt  */
#line 1747 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7476 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1752 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7485 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: for_loop_kw zippered_iterator task_intent_clause do_stmt  */
#line 1757 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7494 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1762 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7503 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TLSBR expr_ls TIN expr task_intent_clause TRSBR stmt  */
#line 1767 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7512 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1772 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7521 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator task_intent_clause TRSBR stmt  */
#line 1777 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7530 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1782 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7539 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR expr_ls task_intent_clause TRSBR stmt  */
#line 1787 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7548 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1792 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7558 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TLSBR zippered_iterator task_intent_clause TRSBR stmt  */
#line 1798 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7568 "bison-chpl-lib.cpp"
    break;

  case 235: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1807 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7578 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF expr TTHEN stmt  */
#line 1816 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7586 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr block_stmt  */
#line 1820 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7594 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1824 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7602 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1828 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7610 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1832 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7618 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar block_stmt  */
#line 1836 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7626 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1840 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7634 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1844 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7642 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1848 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7651 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1853 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7660 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1858 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7669 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1863 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7678 "bison-chpl-lib.cpp"
    break;

  case 248: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1871 "chpl.ypp"
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
#line 7697 "bison-chpl-lib.cpp"
    break;

  case 249: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1886 "chpl.ypp"
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
#line 7716 "bison-chpl-lib.cpp"
    break;

  case 250: /* interface_start: TINTERFACE ident_def  */
#line 1904 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Interface);
  }
#line 7724 "bison-chpl-lib.cpp"
    break;

  case 251: /* interface_stmt: interface_start TLP ifc_formal_ls TRP block_stmt  */
#line 1911 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-4].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), parts, (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7734 "bison-chpl-lib.cpp"
    break;

  case 252: /* interface_stmt: interface_start block_stmt  */
#line 1917 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-1].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), parts, nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7744 "bison-chpl-lib.cpp"
    break;

  case 253: /* ifc_formal_ls: ifc_formal  */
#line 1925 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7750 "bison-chpl-lib.cpp"
    break;

  case 254: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1926 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7756 "bison-chpl-lib.cpp"
    break;

  case 255: /* ifc_formal: ident_def  */
#line 1931 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7762 "bison-chpl-lib.cpp"
    break;

  case 275: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1974 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7770 "bison-chpl-lib.cpp"
    break;

  case 276: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1978 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7778 "bison-chpl-lib.cpp"
    break;

  case 277: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1982 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7786 "bison-chpl-lib.cpp"
    break;

  case 278: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1989 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7794 "bison-chpl-lib.cpp"
    break;

  case 279: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1993 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7802 "bison-chpl-lib.cpp"
    break;

  case 280: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1997 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7810 "bison-chpl-lib.cpp"
    break;

  case 281: /* try_stmt: TTRY tryable_stmt  */
#line 2004 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7818 "bison-chpl-lib.cpp"
    break;

  case 282: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2008 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7826 "bison-chpl-lib.cpp"
    break;

  case 283: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2012 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7834 "bison-chpl-lib.cpp"
    break;

  case 284: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2016 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7842 "bison-chpl-lib.cpp"
    break;

  case 285: /* catch_expr_ls: %empty  */
#line 2022 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7848 "bison-chpl-lib.cpp"
    break;

  case 286: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2023 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7854 "bison-chpl-lib.cpp"
    break;

  case 287: /* catch_expr: TCATCH block_stmt  */
#line 2028 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7862 "bison-chpl-lib.cpp"
    break;

  case 288: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2032 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7870 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2036 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7878 "bison-chpl-lib.cpp"
    break;

  case 290: /* catch_expr_inner: ident_def  */
#line 2043 "chpl.ypp"
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
#line 7895 "bison-chpl-lib.cpp"
    break;

  case 291: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2056 "chpl.ypp"
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
#line 7912 "bison-chpl-lib.cpp"
    break;

  case 292: /* throw_stmt: TTHROW expr TSEMI  */
#line 2072 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7922 "bison-chpl-lib.cpp"
    break;

  case 293: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2080 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7931 "bison-chpl-lib.cpp"
    break;

  case 294: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2085 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7942 "bison-chpl-lib.cpp"
    break;

  case 295: /* when_stmt_ls: %empty  */
#line 2094 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7948 "bison-chpl-lib.cpp"
    break;

  case 296: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2095 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7954 "bison-chpl-lib.cpp"
    break;

  case 297: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2100 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7962 "bison-chpl-lib.cpp"
    break;

  case 298: /* when_stmt: TOTHERWISE stmt  */
#line 2104 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7971 "bison-chpl-lib.cpp"
    break;

  case 299: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2109 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 7981 "bison-chpl-lib.cpp"
    break;

  case 300: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2118 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 7990 "bison-chpl-lib.cpp"
    break;

  case 301: /* manager_expr: expr TAS ident_def  */
#line 2123 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 7998 "bison-chpl-lib.cpp"
    break;

  case 302: /* manager_expr: expr  */
#line 2127 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8006 "bison-chpl-lib.cpp"
    break;

  case 303: /* manager_expr_ls: manager_expr  */
#line 2133 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8012 "bison-chpl-lib.cpp"
    break;

  case 304: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2134 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8018 "bison-chpl-lib.cpp"
    break;

  case 305: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2139 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8027 "bison-chpl-lib.cpp"
    break;

  case 306: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2149 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8037 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2155 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8049 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_start: class_tag ident_def  */
#line 2167 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8057 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_tag: TCLASS  */
#line 2173 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8063 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_tag: TRECORD  */
#line 2174 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8069 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_tag: TUNION  */
#line 2175 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8075 "bison-chpl-lib.cpp"
    break;

  case 312: /* opt_inherit: %empty  */
#line 2179 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8081 "bison-chpl-lib.cpp"
    break;

  case 313: /* opt_inherit: TCOLON expr_ls  */
#line 2180 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8087 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_level_stmt_ls: %empty  */
#line 2184 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8096 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2189 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8104 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2193 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8112 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2197 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8120 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2201 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8128 "bison-chpl-lib.cpp"
    break;

  case 319: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
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
#line 8151 "bison-chpl-lib.cpp"
    break;

  case 320: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
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
#line 8165 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_header: TENUM ident_def  */
#line 2241 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8173 "bison-chpl-lib.cpp"
    break;

  case 322: /* enum_ls: enum_item  */
#line 2248 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8182 "bison-chpl-lib.cpp"
    break;

  case 323: /* enum_ls: enum_ls TCOMMA  */
#line 2253 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8192 "bison-chpl-lib.cpp"
    break;

  case 324: /* $@5: %empty  */
#line 2259 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8201 "bison-chpl-lib.cpp"
    break;

  case 325: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2264 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8210 "bison-chpl-lib.cpp"
    break;

  case 326: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2269 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8219 "bison-chpl-lib.cpp"
    break;

  case 327: /* $@6: %empty  */
#line 2274 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8228 "bison-chpl-lib.cpp"
    break;

  case 328: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2279 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8237 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_item: ident_def  */
#line 2289 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[0])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8249 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_item: ident_def TASSIGN expr  */
#line 2297 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[-2])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8263 "bison-chpl-lib.cpp"
    break;

  case 331: /* lambda_decl_start: TLAMBDA  */
#line 2310 "chpl.ypp"
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
#line 8279 "bison-chpl-lib.cpp"
    break;

  case 332: /* $@7: %empty  */
#line 2327 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8289 "bison-chpl-lib.cpp"
    break;

  case 333: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2333 "chpl.ypp"
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
#line 8312 "bison-chpl-lib.cpp"
    break;

  case 335: /* linkage_spec: linkage_spec_empty  */
#line 2357 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8318 "bison-chpl-lib.cpp"
    break;

  case 336: /* linkage_spec: TINLINE  */
#line 2358 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8325 "bison-chpl-lib.cpp"
    break;

  case 337: /* linkage_spec: TOVERRIDE  */
#line 2360 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8332 "bison-chpl-lib.cpp"
    break;

  case 338: /* opt_fn_type_formal_ls: %empty  */
#line 2365 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8338 "bison-chpl-lib.cpp"
    break;

  case 339: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2366 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8344 "bison-chpl-lib.cpp"
    break;

  case 340: /* fn_type_formal_ls: fn_type_formal  */
#line 2370 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8350 "bison-chpl-lib.cpp"
    break;

  case 341: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2371 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8356 "bison-chpl-lib.cpp"
    break;

  case 342: /* fn_type_formal: named_formal  */
#line 2376 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8362 "bison-chpl-lib.cpp"
    break;

  case 343: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2379 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8368 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_type_formal: formal_type  */
#line 2381 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8374 "bison-chpl-lib.cpp"
    break;

  case 345: /* opt_fn_type_ret_type: %empty  */
#line 2385 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8380 "bison-chpl-lib.cpp"
    break;

  case 346: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2386 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8386 "bison-chpl-lib.cpp"
    break;

  case 347: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2399 "chpl.ypp"
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
#line 8406 "bison-chpl-lib.cpp"
    break;

  case 348: /* $@8: %empty  */
#line 2418 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8416 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2424 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8429 "bison-chpl-lib.cpp"
    break;

  case 350: /* $@9: %empty  */
#line 2433 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8439 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2439 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8452 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2451 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8460 "bison-chpl-lib.cpp"
    break;

  case 353: /* $@10: %empty  */
#line 2460 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8468 "bison-chpl-lib.cpp"
    break;

  case 354: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2464 "chpl.ypp"
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
#line 8491 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2486 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.thisIntentLoc = (yylsp[-2]);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8504 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2495 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.thisIntentLoc = (yylsp[-2]);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8517 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
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
#line 8531 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2514 "chpl.ypp"
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

  case 359: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2524 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8555 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2533 "chpl.ypp"
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
#line 8571 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2549 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenthLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 8580 "bison-chpl-lib.cpp"
    break;

  case 365: /* fn_ident: ident_def TBANG  */
#line 2559 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8590 "bison-chpl-lib.cpp"
    break;

  case 405: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2616 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8596 "bison-chpl-lib.cpp"
    break;

  case 406: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2617 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8602 "bison-chpl-lib.cpp"
    break;

  case 407: /* opt_formal_ls: %empty  */
#line 2621 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8608 "bison-chpl-lib.cpp"
    break;

  case 408: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2622 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8614 "bison-chpl-lib.cpp"
    break;

  case 409: /* req_formal_ls: TLP TRP  */
#line 2626 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8620 "bison-chpl-lib.cpp"
    break;

  case 410: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2627 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8626 "bison-chpl-lib.cpp"
    break;

  case 411: /* formal_ls_inner: formal  */
#line 2631 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8632 "bison-chpl-lib.cpp"
    break;

  case 412: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2632 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8638 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal_ls: %empty  */
#line 2636 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8644 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_ls: formal_ls_inner  */
#line 2637 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8650 "bison-chpl-lib.cpp"
    break;

  case 416: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2646 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8658 "bison-chpl-lib.cpp"
    break;

  case 417: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2651 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8666 "bison-chpl-lib.cpp"
    break;

  case 418: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2656 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8674 "bison-chpl-lib.cpp"
    break;

  case 419: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2661 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8682 "bison-chpl-lib.cpp"
    break;

  case 420: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2666 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yylsp[-5]), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8690 "bison-chpl-lib.cpp"
    break;

  case 421: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2671 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8698 "bison-chpl-lib.cpp"
    break;

  case 422: /* opt_formal_intent_tag: %empty  */
#line 2677 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);
  }
#line 8707 "bison-chpl-lib.cpp"
    break;

  case 423: /* opt_formal_intent_tag: required_intent_tag  */
#line 2682 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8716 "bison-chpl-lib.cpp"
    break;

  case 424: /* required_intent_tag: TIN  */
#line 2689 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::IN);            }
#line 8722 "bison-chpl-lib.cpp"
    break;

  case 425: /* required_intent_tag: TINOUT  */
#line 2690 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::INOUT);         }
#line 8728 "bison-chpl-lib.cpp"
    break;

  case 426: /* required_intent_tag: TOUT  */
#line 2691 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::OUT);           }
#line 8734 "bison-chpl-lib.cpp"
    break;

  case 427: /* required_intent_tag: TCONST TIN  */
#line 2692 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_IN);      }
#line 8740 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TCONST TREF  */
#line 2693 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);     }
#line 8746 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TCONST  */
#line 2694 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);         }
#line 8752 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TPARAM  */
#line 2695 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);         }
#line 8758 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TREF  */
#line 2696 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);           }
#line 8764 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TTYPE  */
#line 2697 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);          }
#line 8770 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TVAR  */
#line 2698 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR); }
#line 8776 "bison-chpl-lib.cpp"
    break;

  case 434: /* opt_this_intent_tag: %empty  */
#line 2702 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);     }
#line 8782 "bison-chpl-lib.cpp"
    break;

  case 435: /* opt_this_intent_tag: TPARAM  */
#line 2703 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);              }
#line 8788 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_this_intent_tag: TREF  */
#line 2704 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);                }
#line 8794 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: TCONST TREF  */
#line 2705 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);          }
#line 8800 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TCONST  */
#line 2706 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);              }
#line 8806 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TTYPE  */
#line 2707 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);               }
#line 8812 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TIN  */
#line 2708 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::IN);       }
#line 8818 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TCONST TIN  */
#line 2709 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::CONST_IN); }
#line 8824 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TVAR  */
#line 2710 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR);      }
#line 8830 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_this_intent_tag: TOUT  */
#line 2711 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::OUT);      }
#line 8836 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_this_intent_tag: TINOUT  */
#line 2712 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::INOUT);    }
#line 8842 "bison-chpl-lib.cpp"
    break;

  case 445: /* proc_iter_or_op: TPROC  */
#line 2716 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8848 "bison-chpl-lib.cpp"
    break;

  case 446: /* proc_iter_or_op: TITER  */
#line 2717 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8854 "bison-chpl-lib.cpp"
    break;

  case 447: /* proc_iter_or_op: TOPERATOR  */
#line 2718 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8860 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_ret_tag: %empty  */
#line 2722 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::DEFAULT_RETURN_INTENT); }
#line 8866 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: TOUT  */
#line 2723 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::OUT);                   }
#line 8872 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TCONST  */
#line 2724 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST);                 }
#line 8878 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TCONST TREF  */
#line 2725 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST_REF);             }
#line 8884 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TREF  */
#line 2726 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::REF);                   }
#line 8890 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_ret_tag: TPARAM  */
#line 2727 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::PARAM);                 }
#line 8896 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: TTYPE  */
#line 2728 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::TYPE);                  }
#line 8902 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: TIN  */
#line 2729 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::IN);            }
#line 8908 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: TCONST TIN  */
#line 2730 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::CONST_IN);      }
#line 8914 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_ret_tag: TVAR  */
#line 2731 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::VAR);           }
#line 8920 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_ret_tag: TINOUT  */
#line 2732 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::INOUT);         }
#line 8926 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_throws_error: %empty  */
#line 2736 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8932 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_throws_error: TTHROWS  */
#line 2737 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8938 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_function_body_stmt: TSEMI  */
#line 2740 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8944 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_function_body_stmt: function_body_stmt  */
#line 2741 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8950 "bison-chpl-lib.cpp"
    break;

  case 463: /* function_body_stmt: block_stmt_body  */
#line 2745 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8956 "bison-chpl-lib.cpp"
    break;

  case 464: /* function_body_stmt: TDO toplevel_stmt  */
#line 2746 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 8962 "bison-chpl-lib.cpp"
    break;

  case 465: /* query_expr: TQUERIEDIDENT  */
#line 2750 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8968 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_lifetime_where: %empty  */
#line 2755 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8974 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_lifetime_where: TWHERE expr  */
#line 2757 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8980 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2759 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8986 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2761 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 8992 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2763 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 8998 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_components_expr: lifetime_expr  */
#line 2768 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9004 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2770 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9010 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2775 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9016 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2777 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9022 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2779 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9028 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2781 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9034 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2783 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9040 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2785 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9046 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2787 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9052 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_ident: TIDENT  */
#line 2791 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9058 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_ident: TINIT  */
#line 2792 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9064 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_ident: TTHIS  */
#line 2793 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9070 "bison-chpl-lib.cpp"
    break;

  case 483: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2798 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9079 "bison-chpl-lib.cpp"
    break;

  case 484: /* type_alias_decl_stmt_start: TTYPE  */
#line 2807 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9087 "bison-chpl-lib.cpp"
    break;

  case 485: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2811 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9096 "bison-chpl-lib.cpp"
    break;

  case 486: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2819 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9104 "bison-chpl-lib.cpp"
    break;

  case 487: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2823 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9112 "bison-chpl-lib.cpp"
    break;

  case 488: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2830 "chpl.ypp"
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
#line 9135 "bison-chpl-lib.cpp"
    break;

  case 489: /* opt_init_type: %empty  */
#line 2851 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9141 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_init_type: TASSIGN expr  */
#line 2853 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9147 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_type: TPARAM  */
#line 2857 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9153 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_type: TCONST TREF  */
#line 2858 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9159 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_type: TREF  */
#line 2859 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9165 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_type: TCONST  */
#line 2860 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9171 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_type: TVAR  */
#line 2861 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9177 "bison-chpl-lib.cpp"
    break;

  case 496: /* $@11: %empty  */
#line 2866 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9186 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2870 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-4]), (yylsp[-3])), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9195 "bison-chpl-lib.cpp"
    break;

  case 498: /* $@12: %empty  */
#line 2875 "chpl.ypp"
  {
    // Use a mid-rule action to preserve the 'on' expr
    context->noteVarDestinationExpr((yyvsp[0].expr));
  }
#line 9204 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt: TON expr $@12 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2880 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-4])), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9213 "bison-chpl-lib.cpp"
    break;

  case 500: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2885 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9222 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt: var_decl_type error TSEMI  */
#line 2890 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    context->resetDeclStateOnError();
  }
#line 9231 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2898 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9239 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2902 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9247 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2909 "chpl.ypp"
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
#line 9270 "bison-chpl-lib.cpp"
    break;

  case 505: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2928 "chpl.ypp"
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
#line 9290 "bison-chpl-lib.cpp"
    break;

  case 506: /* tuple_var_decl_component: ident_def  */
#line 2947 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9298 "bison-chpl-lib.cpp"
    break;

  case 507: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2951 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9306 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2958 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9312 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2960 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9318 "bison-chpl-lib.cpp"
    break;

  case 510: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2962 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9324 "bison-chpl-lib.cpp"
    break;

  case 511: /* opt_init_expr: %empty  */
#line 2968 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9330 "bison-chpl-lib.cpp"
    break;

  case 512: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2969 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9336 "bison-chpl-lib.cpp"
    break;

  case 513: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2970 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9342 "bison-chpl-lib.cpp"
    break;

  case 514: /* formal_or_ret_type_expr: expr  */
#line 2974 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9348 "bison-chpl-lib.cpp"
    break;

  case 515: /* ret_type: formal_or_ret_type_expr  */
#line 2978 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9354 "bison-chpl-lib.cpp"
    break;

  case 516: /* ret_type: reserved_type_ident_use  */
#line 2979 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9360 "bison-chpl-lib.cpp"
    break;

  case 517: /* ret_type: error  */
#line 2980 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9366 "bison-chpl-lib.cpp"
    break;

  case 518: /* colon_ret_type: TCOLON ret_type  */
#line 2984 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9372 "bison-chpl-lib.cpp"
    break;

  case 519: /* colon_ret_type: error  */
#line 2985 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9378 "bison-chpl-lib.cpp"
    break;

  case 520: /* opt_ret_type: %empty  */
#line 2989 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9384 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_type: %empty  */
#line 2994 "chpl.ypp"
                                 { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9390 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_type: TCOLON expr  */
#line 2995 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9396 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2996 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9402 "bison-chpl-lib.cpp"
    break;

  case 525: /* opt_type: error  */
#line 2997 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9408 "bison-chpl-lib.cpp"
    break;

  case 526: /* formal_type: formal_or_ret_type_expr  */
#line 3001 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9414 "bison-chpl-lib.cpp"
    break;

  case 527: /* formal_type: reserved_type_ident_use  */
#line 3002 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9420 "bison-chpl-lib.cpp"
    break;

  case 528: /* colon_formal_type: TCOLON formal_type  */
#line 3006 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9426 "bison-chpl-lib.cpp"
    break;

  case 529: /* opt_colon_formal_type: %empty  */
#line 3010 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9432 "bison-chpl-lib.cpp"
    break;

  case 530: /* opt_colon_formal_type: colon_formal_type  */
#line 3011 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9438 "bison-chpl-lib.cpp"
    break;

  case 531: /* expr_ls: expr  */
#line 3017 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9444 "bison-chpl-lib.cpp"
    break;

  case 532: /* expr_ls: expr_ls TCOMMA expr  */
#line 3018 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9450 "bison-chpl-lib.cpp"
    break;

  case 533: /* tuple_component: opt_try_expr  */
#line 3022 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9456 "bison-chpl-lib.cpp"
    break;

  case 534: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3027 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9464 "bison-chpl-lib.cpp"
    break;

  case 535: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3031 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9472 "bison-chpl-lib.cpp"
    break;

  case 536: /* opt_actual_ls: %empty  */
#line 3037 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9478 "bison-chpl-lib.cpp"
    break;

  case 537: /* opt_actual_ls: actual_ls  */
#line 3038 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9484 "bison-chpl-lib.cpp"
    break;

  case 538: /* actual_ls: actual_expr  */
#line 3043 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9493 "bison-chpl-lib.cpp"
    break;

  case 539: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3048 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9503 "bison-chpl-lib.cpp"
    break;

  case 540: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3056 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9509 "bison-chpl-lib.cpp"
    break;

  case 541: /* actual_expr: opt_try_expr  */
#line 3057 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9515 "bison-chpl-lib.cpp"
    break;

  case 542: /* ident_expr: ident_use  */
#line 3061 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9521 "bison-chpl-lib.cpp"
    break;

  case 543: /* ident_expr: scalar_type  */
#line 3062 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9527 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3075 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9533 "bison-chpl-lib.cpp"
    break;

  case 550: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3077 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9539 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3079 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9545 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3081 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9555 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TATOMIC expr  */
#line 3087 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9561 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TSYNC expr  */
#line 3089 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9567 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TOWNED  */
#line 3092 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9573 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TOWNED expr  */
#line 3094 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9579 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TUNMANAGED  */
#line 3096 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9585 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3098 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9591 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TSHARED  */
#line 3100 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9597 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TSHARED expr  */
#line 3102 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9603 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TBORROWED  */
#line 3104 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9609 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TBORROWED expr  */
#line 3106 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9615 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TCLASS  */
#line 3108 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9621 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TRECORD  */
#line 3110 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9627 "bison-chpl-lib.cpp"
    break;

  case 565: /* for_expr: no_loop_attributes for_expr_base  */
#line 3114 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9633 "bison-chpl-lib.cpp"
    break;

  case 566: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3119 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9641 "bison-chpl-lib.cpp"
    break;

  case 567: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3123 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9649 "bison-chpl-lib.cpp"
    break;

  case 568: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3127 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9657 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr_base: for_loop_kw zippered_iterator TDO expr  */
#line 3131 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "expression-level loops with 'zip' must have an index.");
  }
#line 9665 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3135 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9678 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3144 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9691 "bison-chpl-lib.cpp"
    break;

  case 572: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3153 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9704 "bison-chpl-lib.cpp"
    break;

  case 573: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3164 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9710 "bison-chpl-lib.cpp"
    break;

  case 574: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3169 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9718 "bison-chpl-lib.cpp"
    break;

  case 575: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3173 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9726 "bison-chpl-lib.cpp"
    break;

  case 576: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3177 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9734 "bison-chpl-lib.cpp"
    break;

  case 577: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3181 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9742 "bison-chpl-lib.cpp"
    break;

  case 578: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3185 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9750 "bison-chpl-lib.cpp"
    break;

  case 579: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3189 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9758 "bison-chpl-lib.cpp"
    break;

  case 580: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3193 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9766 "bison-chpl-lib.cpp"
    break;

  case 581: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3200 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9780 "bison-chpl-lib.cpp"
    break;

  case 582: /* nil_expr: TNIL  */
#line 3217 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9786 "bison-chpl-lib.cpp"
    break;

  case 590: /* opt_task_intent_ls: %empty  */
#line 3235 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9792 "bison-chpl-lib.cpp"
    break;

  case 591: /* opt_task_intent_ls: task_intent_clause  */
#line 3236 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9798 "bison-chpl-lib.cpp"
    break;

  case 592: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3241 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9806 "bison-chpl-lib.cpp"
    break;

  case 593: /* task_intent_clause: TWITH TLP TRP  */
#line 3245 "chpl.ypp"
  {
    auto err = context->syntax((yyloc), "'with' clause cannot be empty.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-2]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9817 "bison-chpl-lib.cpp"
    break;

  case 594: /* task_intent_clause: TWITH TLP error TRP  */
#line 3252 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[-2]), "invalid intent expression in 'with' clause.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9828 "bison-chpl-lib.cpp"
    break;

  case 595: /* task_intent_clause: TWITH error  */
#line 3259 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[0]), "missing parentheses around 'with' clause intents.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-1]), dummyLoc, dummyLoc, dummyLoc, exprs);
  }
#line 9839 "bison-chpl-lib.cpp"
    break;

  case 596: /* task_intent_ls: intent_expr  */
#line 3268 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9845 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3269 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9851 "bison-chpl-lib.cpp"
    break;

  case 598: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3274 "chpl.ypp"
  {
    (yyval.expr) = context->buildTaskIntent((yyloc), (yylsp[-2]), nullptr, (yyvsp[-2].expr), (yyvsp[-3].taskIntent), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9859 "bison-chpl-lib.cpp"
    break;

  case 599: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3278 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9867 "bison-chpl-lib.cpp"
    break;

  case 600: /* intent_expr: expr TREDUCE ident_expr  */
#line 3282 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9875 "bison-chpl-lib.cpp"
    break;

  case 601: /* task_var_prefix: TCONST  */
#line 3288 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST);          }
#line 9881 "bison-chpl-lib.cpp"
    break;

  case 602: /* task_var_prefix: TIN  */
#line 3289 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::IN);             }
#line 9887 "bison-chpl-lib.cpp"
    break;

  case 603: /* task_var_prefix: TCONST TIN  */
#line 3290 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_IN);       }
#line 9893 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_var_prefix: TREF  */
#line 3291 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::REF);            }
#line 9899 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_var_prefix: TCONST TREF  */
#line 3292 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_REF);      }
#line 9905 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_var_prefix: TVAR  */
#line 3293 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::VAR);            }
#line 9911 "bison-chpl-lib.cpp"
    break;

  case 607: /* task_var_prefix: TOUT  */
#line 3294 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::OUT);   }
#line 9917 "bison-chpl-lib.cpp"
    break;

  case 608: /* task_var_prefix: TINOUT  */
#line 3295 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::INOUT); }
#line 9923 "bison-chpl-lib.cpp"
    break;

  case 609: /* task_var_prefix: TPARAM  */
#line 3296 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::PARAM); }
#line 9929 "bison-chpl-lib.cpp"
    break;

  case 610: /* task_var_prefix: TTYPE  */
#line 3297 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::TYPE);  }
#line 9935 "bison-chpl-lib.cpp"
    break;

  case 611: /* new_maybe_decorated: TNEW  */
#line 3302 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9941 "bison-chpl-lib.cpp"
    break;

  case 612: /* new_maybe_decorated: TNEW TOWNED  */
#line 3304 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9947 "bison-chpl-lib.cpp"
    break;

  case 613: /* new_maybe_decorated: TNEW TSHARED  */
#line 3306 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9953 "bison-chpl-lib.cpp"
    break;

  case 614: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3308 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9959 "bison-chpl-lib.cpp"
    break;

  case 615: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3310 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9965 "bison-chpl-lib.cpp"
    break;

  case 616: /* new_expr: new_maybe_decorated expr  */
#line 3316 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 9973 "bison-chpl-lib.cpp"
    break;

  case 617: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3323 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenthLocation((yyvsp[-4].expr), LOC2((yylsp[-5]), (yylsp[-3])));
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
#line 9990 "bison-chpl-lib.cpp"
    break;

  case 618: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3336 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenthLocation((yyvsp[-4].expr), LOC2((yylsp[-5]), (yylsp[-3])));
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
#line 10007 "bison-chpl-lib.cpp"
    break;

  case 619: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3349 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenthLocation((yyvsp[-5].expr), LOC2((yylsp[-6]), (yylsp[-4])));
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
#line 10025 "bison-chpl-lib.cpp"
    break;

  case 620: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3363 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenthLocation((yyvsp[-5].expr), LOC2((yylsp[-6]), (yylsp[-4])));
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
#line 10044 "bison-chpl-lib.cpp"
    break;

  case 621: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3381 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10052 "bison-chpl-lib.cpp"
    break;

  case 622: /* range_literal_expr: expr TDOTDOT expr  */
#line 3388 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10061 "bison-chpl-lib.cpp"
    break;

  case 623: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3393 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10070 "bison-chpl-lib.cpp"
    break;

  case 624: /* range_literal_expr: expr TDOTDOT  */
#line 3398 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10079 "bison-chpl-lib.cpp"
    break;

  case 625: /* range_literal_expr: TDOTDOT expr  */
#line 3403 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10089 "bison-chpl-lib.cpp"
    break;

  case 626: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3409 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10099 "bison-chpl-lib.cpp"
    break;

  case 627: /* range_literal_expr: TDOTDOT  */
#line 3415 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10109 "bison-chpl-lib.cpp"
    break;

  case 628: /* cast_expr: expr TCOLON expr  */
#line 3445 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10117 "bison-chpl-lib.cpp"
    break;

  case 629: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3452 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10125 "bison-chpl-lib.cpp"
    break;

  case 630: /* super_expr: fn_expr  */
#line 3458 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10131 "bison-chpl-lib.cpp"
    break;

  case 634: /* expr: sub_type_level_expr TQUESTION  */
#line 3467 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10137 "bison-chpl-lib.cpp"
    break;

  case 635: /* expr: TQUESTION  */
#line 3469 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10143 "bison-chpl-lib.cpp"
    break;

  case 639: /* expr: fn_type  */
#line 3474 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10149 "bison-chpl-lib.cpp"
    break;

  case 649: /* opt_expr: %empty  */
#line 3488 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10155 "bison-chpl-lib.cpp"
    break;

  case 650: /* opt_expr: expr  */
#line 3489 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10161 "bison-chpl-lib.cpp"
    break;

  case 651: /* opt_try_expr: TTRY expr  */
#line 3493 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10167 "bison-chpl-lib.cpp"
    break;

  case 652: /* opt_try_expr: TTRYBANG expr  */
#line 3494 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10173 "bison-chpl-lib.cpp"
    break;

  case 653: /* opt_try_expr: super_expr  */
#line 3495 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10179 "bison-chpl-lib.cpp"
    break;

  case 659: /* call_base_expr: expr TBANG  */
#line 3512 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10187 "bison-chpl-lib.cpp"
    break;

  case 660: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3515 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10193 "bison-chpl-lib.cpp"
    break;

  case 663: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3522 "chpl.ypp"
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
#line 10209 "bison-chpl-lib.cpp"
    break;

  case 664: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3534 "chpl.ypp"
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
#line 10225 "bison-chpl-lib.cpp"
    break;

  case 665: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3546 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10233 "bison-chpl-lib.cpp"
    break;

  case 666: /* dot_expr: expr TDOT ident_use  */
#line 3553 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10239 "bison-chpl-lib.cpp"
    break;

  case 667: /* dot_expr: expr TDOT TTYPE  */
#line 3555 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10245 "bison-chpl-lib.cpp"
    break;

  case 668: /* dot_expr: expr TDOT TDOMAIN  */
#line 3557 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10251 "bison-chpl-lib.cpp"
    break;

  case 669: /* dot_expr: expr TDOT TLOCALE  */
#line 3559 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10257 "bison-chpl-lib.cpp"
    break;

  case 670: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3561 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10263 "bison-chpl-lib.cpp"
    break;

  case 671: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3563 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10269 "bison-chpl-lib.cpp"
    break;

  case 672: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3572 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenthLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10279 "bison-chpl-lib.cpp"
    break;

  case 673: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3578 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10287 "bison-chpl-lib.cpp"
    break;

  case 674: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3582 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10295 "bison-chpl-lib.cpp"
    break;

  case 675: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3586 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10303 "bison-chpl-lib.cpp"
    break;

  case 676: /* bool_literal: TFALSE  */
#line 3592 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10309 "bison-chpl-lib.cpp"
    break;

  case 677: /* bool_literal: TTRUE  */
#line 3593 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10315 "bison-chpl-lib.cpp"
    break;

  case 678: /* str_bytes_literal: STRINGLITERAL  */
#line 3597 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10321 "bison-chpl-lib.cpp"
    break;

  case 679: /* str_bytes_literal: BYTESLITERAL  */
#line 3598 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10327 "bison-chpl-lib.cpp"
    break;

  case 682: /* literal_expr: INTLITERAL  */
#line 3604 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10333 "bison-chpl-lib.cpp"
    break;

  case 683: /* literal_expr: REALLITERAL  */
#line 3605 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10339 "bison-chpl-lib.cpp"
    break;

  case 684: /* literal_expr: IMAGLITERAL  */
#line 3606 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10345 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: CSTRINGLITERAL  */
#line 3607 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10351 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: TNONE  */
#line 3608 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10357 "bison-chpl-lib.cpp"
    break;

  case 687: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3610 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10366 "bison-chpl-lib.cpp"
    break;

  case 688: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3615 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10375 "bison-chpl-lib.cpp"
    break;

  case 689: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3620 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10384 "bison-chpl-lib.cpp"
    break;

  case 690: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3625 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10393 "bison-chpl-lib.cpp"
    break;

  case 691: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3630 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10402 "bison-chpl-lib.cpp"
    break;

  case 692: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3635 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10411 "bison-chpl-lib.cpp"
    break;

  case 693: /* assoc_expr_ls: expr TALIAS expr  */
#line 3644 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10420 "bison-chpl-lib.cpp"
    break;

  case 694: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3649 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10430 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TPLUS expr  */
#line 3657 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10436 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TMINUS expr  */
#line 3658 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10442 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TSTAR expr  */
#line 3659 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10448 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TDIVIDE expr  */
#line 3660 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10454 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3661 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10460 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3662 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10466 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TMOD expr  */
#line 3663 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10472 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TEQUAL expr  */
#line 3664 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10478 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3665 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10484 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3666 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10490 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3667 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10496 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TLESS expr  */
#line 3668 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10502 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TGREATER expr  */
#line 3669 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10508 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TBAND expr  */
#line 3670 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10514 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TBOR expr  */
#line 3671 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10520 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TBXOR expr  */
#line 3672 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10526 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TAND expr  */
#line 3673 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10532 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TOR expr  */
#line 3674 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10538 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TEXP expr  */
#line 3675 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10544 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TBY expr  */
#line 3676 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10550 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TALIGN expr  */
#line 3677 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10556 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr THASH expr  */
#line 3678 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10562 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TDMAPPED expr  */
#line 3679 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10568 "bison-chpl-lib.cpp"
    break;

  case 718: /* unary_op_expr: TPLUS expr  */
#line 3683 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10574 "bison-chpl-lib.cpp"
    break;

  case 719: /* unary_op_expr: TMINUS expr  */
#line 3684 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10580 "bison-chpl-lib.cpp"
    break;

  case 720: /* unary_op_expr: TMINUSMINUS expr  */
#line 3685 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10586 "bison-chpl-lib.cpp"
    break;

  case 721: /* unary_op_expr: TPLUSPLUS expr  */
#line 3686 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10592 "bison-chpl-lib.cpp"
    break;

  case 722: /* unary_op_expr: TBANG expr  */
#line 3687 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10598 "bison-chpl-lib.cpp"
    break;

  case 723: /* unary_op_expr: expr TBANG  */
#line 3688 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10606 "bison-chpl-lib.cpp"
    break;

  case 724: /* unary_op_expr: TBNOT expr  */
#line 3691 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10612 "bison-chpl-lib.cpp"
    break;

  case 725: /* reduce_expr: expr TREDUCE expr  */
#line 3696 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10620 "bison-chpl-lib.cpp"
    break;

  case 726: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3700 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10628 "bison-chpl-lib.cpp"
    break;

  case 727: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3704 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10636 "bison-chpl-lib.cpp"
    break;

  case 728: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3708 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10644 "bison-chpl-lib.cpp"
    break;

  case 729: /* scan_expr: expr TSCAN expr  */
#line 3715 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10652 "bison-chpl-lib.cpp"
    break;

  case 730: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3719 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10660 "bison-chpl-lib.cpp"
    break;

  case 731: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3723 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10668 "bison-chpl-lib.cpp"
    break;

  case 732: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3727 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10676 "bison-chpl-lib.cpp"
    break;


#line 10680 "bison-chpl-lib.cpp"

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
