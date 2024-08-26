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
  YYSYMBOL_enum_header = 270,              /* enum_header  */
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
  YYSYMBOL_326_12 = 326,                   /* $@12  */
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
  YYSYMBOL_intent_expr = 358,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 359,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 360,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 361,                 /* new_expr  */
  YYSYMBOL_let_expr = 362,                 /* let_expr  */
  YYSYMBOL_range_literal_expr = 363,       /* range_literal_expr  */
  YYSYMBOL_cast_expr = 364,                /* cast_expr  */
  YYSYMBOL_tuple_expand_expr = 365,        /* tuple_expand_expr  */
  YYSYMBOL_super_expr = 366,               /* super_expr  */
  YYSYMBOL_expr = 367,                     /* expr  */
  YYSYMBOL_opt_expr = 368,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 369,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 370,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 371,           /* call_base_expr  */
  YYSYMBOL_call_expr = 372,                /* call_expr  */
  YYSYMBOL_dot_expr = 373,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 374,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 375,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 376,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 377,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 378,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 379,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 380,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 381,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 382,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 383       /* reduce_scan_op_expr  */
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

#line 507 "bison-chpl-lib.cpp"

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
#define YYLAST   17931

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  203
/* YYNRULES -- Number of rules.  */
#define YYNRULES  738
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1261

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
    1284,  1285,  1286,  1287,  1299,  1303,  1308,  1313,  1319,  1328,
    1329,  1333,  1342,  1346,  1350,  1354,  1358,  1362,  1371,  1372,
    1373,  1377,  1378,  1379,  1380,  1381,  1385,  1386,  1387,  1399,
    1400,  1401,  1406,  1407,  1408,  1409,  1410,  1411,  1412,  1413,
    1414,  1415,  1416,  1417,  1418,  1419,  1420,  1421,  1422,  1423,
    1427,  1428,  1429,  1430,  1431,  1432,  1433,  1434,  1435,  1436,
    1437,  1438,  1445,  1446,  1447,  1451,  1452,  1456,  1463,  1473,
    1477,  1483,  1483,  1489,  1489,  1498,  1499,  1500,  1501,  1502,
    1503,  1504,  1508,  1513,  1518,  1523,  1530,  1538,  1539,  1543,
    1552,  1559,  1568,  1587,  1586,  1596,  1595,  1608,  1614,  1617,
    1618,  1627,  1628,  1629,  1630,  1634,  1657,  1676,  1695,  1720,
    1725,  1730,  1735,  1740,  1745,  1750,  1755,  1760,  1765,  1770,
    1775,  1780,  1785,  1790,  1796,  1805,  1814,  1818,  1822,  1826,
    1830,  1834,  1838,  1842,  1846,  1851,  1856,  1861,  1869,  1884,
    1902,  1906,  1913,  1914,  1919,  1924,  1925,  1926,  1927,  1928,
    1929,  1930,  1931,  1932,  1933,  1934,  1935,  1940,  1945,  1946,
    1947,  1948,  1956,  1957,  1961,  1965,  1969,  1976,  1980,  1984,
    1991,  1995,  1999,  2003,  2010,  2011,  2015,  2019,  2023,  2030,
    2043,  2059,  2067,  2072,  2082,  2083,  2087,  2091,  2096,  2105,
    2110,  2114,  2121,  2122,  2126,  2136,  2142,  2154,  2161,  2162,
    2163,  2167,  2168,  2172,  2176,  2180,  2184,  2188,  2196,  2215,
    2228,  2235,  2240,  2247,  2246,  2256,  2262,  2261,  2276,  2283,
    2295,  2313,  2310,  2339,  2343,  2344,  2346,  2351,  2352,  2356,
    2357,  2361,  2364,  2366,  2371,  2372,  2383,  2404,  2403,  2419,
    2418,  2436,  2446,  2443,  2471,  2480,  2489,  2499,  2509,  2518,
    2533,  2534,  2538,  2539,  2540,  2549,  2550,  2551,  2552,  2553,
    2554,  2555,  2556,  2557,  2558,  2559,  2560,  2561,  2562,  2563,
    2564,  2565,  2566,  2567,  2568,  2569,  2570,  2571,  2572,  2573,
    2577,  2578,  2579,  2580,  2581,  2582,  2583,  2584,  2585,  2586,
    2587,  2588,  2593,  2594,  2598,  2599,  2603,  2604,  2608,  2609,
    2613,  2614,  2618,  2619,  2623,  2627,  2631,  2636,  2641,  2646,
    2651,  2659,  2663,  2671,  2672,  2673,  2674,  2675,  2676,  2677,
    2678,  2679,  2680,  2684,  2685,  2686,  2687,  2688,  2689,  2690,
    2691,  2692,  2693,  2694,  2698,  2699,  2700,  2704,  2705,  2706,
    2707,  2708,  2709,  2710,  2711,  2712,  2713,  2714,  2718,  2719,
    2722,  2723,  2727,  2728,  2732,  2737,  2738,  2740,  2742,  2744,
    2749,  2751,  2756,  2758,  2760,  2762,  2764,  2766,  2768,  2773,
    2774,  2775,  2779,  2788,  2792,  2800,  2804,  2811,  2833,  2834,
    2839,  2840,  2841,  2842,  2843,  2848,  2847,  2857,  2856,  2866,
    2871,  2879,  2883,  2890,  2909,  2928,  2932,  2939,  2941,  2943,
    2950,  2951,  2952,  2956,  2960,  2961,  2962,  2966,  2967,  2971,
    2972,  2976,  2977,  2978,  2979,  2983,  2984,  2988,  2992,  2993,
    2999,  3000,  3004,  3008,  3012,  3019,  3020,  3024,  3029,  3038,
    3039,  3043,  3044,  3051,  3052,  3053,  3054,  3055,  3056,  3058,
    3060,  3062,  3068,  3070,  3073,  3075,  3077,  3079,  3081,  3083,
    3085,  3087,  3089,  3091,  3096,  3100,  3104,  3108,  3112,  3116,
    3125,  3134,  3146,  3150,  3154,  3158,  3162,  3166,  3170,  3174,
    3181,  3199,  3207,  3208,  3209,  3210,  3211,  3212,  3213,  3217,
    3218,  3222,  3226,  3233,  3240,  3250,  3251,  3255,  3259,  3263,
    3270,  3271,  3272,  3273,  3274,  3275,  3276,  3277,  3278,  3279,
    3283,  3285,  3287,  3289,  3291,  3297,  3304,  3316,  3328,  3341,
    3358,  3365,  3370,  3375,  3380,  3386,  3392,  3422,  3429,  3436,
    3437,  3441,  3443,  3444,  3446,  3448,  3449,  3450,  3451,  3454,
    3455,  3456,  3457,  3458,  3459,  3460,  3461,  3462,  3466,  3467,
    3471,  3472,  3473,  3477,  3478,  3479,  3480,  3489,  3490,  3493,
    3494,  3495,  3499,  3511,  3523,  3530,  3532,  3534,  3536,  3538,
    3540,  3549,  3555,  3559,  3563,  3570,  3571,  3575,  3576,  3580,
    3581,  3582,  3583,  3584,  3585,  3586,  3587,  3592,  3597,  3602,
    3607,  3612,  3621,  3626,  3635,  3636,  3637,  3638,  3639,  3640,
    3641,  3642,  3643,  3644,  3645,  3646,  3647,  3648,  3649,  3650,
    3651,  3652,  3653,  3654,  3655,  3656,  3657,  3661,  3662,  3663,
    3664,  3665,  3666,  3669,  3673,  3677,  3681,  3685,  3692,  3696,
    3700,  3704,  3712,  3713,  3714,  3715,  3716,  3717,  3718
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

#define YYPACT_NINF (-920)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-739)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -920,   147,  3842,  -920,   -15,   142,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920, 11968,   102,    97,   176, 13818,   251,
   17697,   102,   268,   183,   247,    97,  5396, 11968,   172, 17756,
    -920,   368,   332,  -920, 10064,   409,   316,   258,  -920,   425,
     421, 17756, 17756, 17756,  -920,  2154, 10927,   442, 11968, 11968,
      64,  -920,   474,   496, 11968,  -920, 13818,  -920, 11968,   567,
     426,   244,   329, 13172,   547, 17815,  -920, 11968,  8169, 11968,
   10927, 13818,   509,   554,   452,  5396,   556, 11968,   572,  7137,
    7137,  -920,   573,  -920, 13818,  -920,   578, 10236,  -920, 11968,
    -920, 11968,  -920,  -920, 13646, 11968,  -920, 10408,  -920,  -920,
    -920,  4346,  8341, 11968,  -920,  5046,  -920,  -920,  -920,  -920,
    2650,   633,  -920,   497,   485,  -920,   227,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    7481,  -920,  7653,  -920,  -920,    73,  -920,  -920,  -920,   601,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,   519, 17756,
    -920,   499,   501,   221,  -920,   324,  -920,  -920,  -920,  1146,
    1226,  -920,  -920, 17756,  1499,  -920,   517,   520,  -920,  -920,
    -920,   518,   522, 11968,   524,   532,  -920,  -920,  -920, 17161,
    1718,   253,   533,   534,  -920,  -920,   394,  -920,  -920,  -920,
    -920,  -920,   -20,  -920,  -920,  -920, 11968,  -920, 17756,  -920,
   11968, 11968,    21,   628,   403,  -920,  -920,  -920,  -920, 17161,
     417,  -920,  -920,    37,  5396,  -920,  -920,  -920,   537,   -59,
     531,  -920,   275,  -920,   538,  -920,   191, 17161,  8513,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920, 17756, 17756,   -43,
    3959,  -920,  -920,   604,  8513,   542,   539,   472,  5396,  2257,
      38,    13,  -920,  5396,  -920,  -920, 15210,  -920,    18, 15619,
     466,  -920,   540,   541,  -920, 15210,   -59,   466,  -920,  8513,
    1696,  1696,   196,    32,  -920,    85,  -920,  -920,  -920,  -920,
    7825,  -920,  -920,   812,  -920,   535,   563,  -920,  -920,  2418,
     568,  -920,  -920, 17161,   226, 11968, 11968,  -920,   -44,  -920,
    -920, 17161,   551, 15738,  -920, 15210,   -59,   546,  8513,  -920,
   17161, 15778, 11968,  -920,  -920,  -920,  -920,  -920,   -59,   557,
     406,   406,   311,   466,   466,   304,  -920,  -920,  4521,   121,
   11968,   577,   -80,   555,   311,   710,  -920,  -920, 17756,  -920,
   11968,  -920,  4346,  -920,   207,  -920,   604,  -920,  -920,   729,
     562,  4696, 11968,  -920, 11968,   668,  -920,  -920, 14881,   170,
     519, 17161,   439,  -920,  5396,   656,  -920,  -920, 10064, 10580,
   11099,  -920,  -920,  -920, 17756, 11968,   569,  -920, 17488,    56,
    1146,  -920,  -920,  -920,  -920,   205,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,    69,   222,  -920,  -920,  -920,  -920,  -920,
    -920,  -920, 13479,   612,   372,  -920,   576,   373,   471,  -920,
     589, 11968, 11968, 11968, 11099, 11099, 11968,   482, 11968, 11968,
   11968, 11968, 11968,   319, 13646, 11968, 11968, 11968, 11968, 11968,
   11968, 11968, 11968, 11968, 11968, 11968, 11968, 11968, 11968, 11968,
   11968,   672,  -920,  -920,  -920,  -920,  -920, 10236, 10236,  -920,
    -920,  -920,  -920, 10236,  -920,  -920, 10236, 10236,  8513,  8685,
   11099, 11099, 15818,   571,  -920, 11271, 11099, 17756,  -920,  6443,
    -920,  -920,  -920,  2154,  -920, 11968,  -920,   620,   582,   613,
    -920,  -920,   636,   638,  5396,   731,  5396,  -920,   732, 11968,
     697,   595,  -920, 10236, 17756,  -920,  -920,  2257,  -920,  -920,
     184,  -920, 12140,   644, 11968,  2154,  -920,  -920, 11968,  -920,
   17547, 11968, 11968,  -920,   275,   597,  -920,  -920,  -920,  -920,
   17756,  -920,  1146, 13344,  2257,   629, 12140,  -920, 17161, 17161,
    -920,   531,  -920,    47,  -920,  8513,   603,  -920, 15210,   752,
     752,  -920,  -920,  -920,  -920, 11446,  -920, 15897,  8860,  9032,
    -920,  -920,  -920, 17161, 11099, 11099,  -920,   497,  9204,  -920,
     340,  -920,  4871,  -920,   379, 15937,   395, 14971, 17756,  6962,
    6790,  -920,   519,   607,  -920,   275,  -920,   660, 17756,    16,
   15210,   611, 13960,   -65,    63, 15978,   112,    24, 14716,   259,
     639,  1837,   614, 17638,   662,   131,  -920,  -920,   202,  -920,
    -920,    55,  -920,  -920,  -920,  6618,  -920,   682,  -920,  -920,
     617,   643,  -920,   647,  -920,   661,   664,   669,   670,   671,
    -920,   673,   676,   679,   680,   683,   684,   685,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
   11968,   659,   690,   617,  -920,   617,  -920, 11968,  -920, 17756,
    -920,  -920,  -920, 17280, 17280,   487,  -920,   487,  -920,   487,
    3629,   689,   605,   954,   -59,   406,  -920,   506,  -920,  -920,
    -920,  -920,  -920,   311,   649,   487,  1516,  1516, 17280,  1516,
    1516,   526,   406,   649, 17320,   526,   466,   466,   406,   311,
     640,   678,   681,   687,   688,   694,   674,   666,  -920,   487,
    -920,   487, 11968, 10236,   108,   817, 15290,   695,   696,   236,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,   224,  -920,
    2097, 17240,   415,   398, 17161, 10236,  -920, 10236, 11968, 11968,
     827,  5396,   835, 16129,  5396, 15369, 17756,  -920,   237,  -920,
     238,  -920,   245,  2257,    38,  -920, 17161,  9376,  -920, 17161,
    -920,  -920,  -920, 17756, 16169, 16209,  2154,  -920,   629,   725,
    -920,   285, 12140,  -920,   310,  -920, 11968,  -920,   703,   141,
     702,  -920,  1970,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,   134,    54, 14790,  -920,  -920,   290,  -920,  -920,  -920,
    -920, 11968,  -920,   264, 14065, 11968,  -920, 11968,  6962,  6790,
    -920,  9548,   448,  9720,   514,   525,  9892,  7997,   536,   399,
    -920,  -920,  -920, 16288,   734,   711,   708,  -920, 17756,  2154,
   11968,   831,  -920,  -920, 11968, 17161,  5396,   709, 11099, 11618,
    4171,   713, 11968, 11793,  -920,  5396,  -920,    16,  5571, 11099,
    -920,    16,  -920, 10236,   712,  2688,  -920,  -920, 11968,    42,
    -920,   820,  -920,   682,  -920,  -920,  -920,  -920,  -920,     6,
      57,  -920, 16328,  -920, 14224,  -920,  -920, 17161,  -920,   717,
     716,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,   294,
   11099, 13960, 11968, 12312, 11099, 10236,  -920,  -920,  -920,  7309,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,    34, 11099, 11099,  -920,  -920,  -920, 17161, 17161,
    5396,  -920,  5396, 11968,  -920,  5396,   851,   728,   730, 17756,
     531,  -920,  -920,   644,  -920,  -920,  -920,   724,   726,   422,
     310, 11968,   682,   629,  -920, 10755,  -920,  -920, 17161,  -920,
    5396, 11968,  -920,  -920,  -920, 17756,  -920,   753,   531, 11099,
    5396, 11099, 10236,  -920,  -920,  -920,  -920,  -920,   506,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  6962,  -920,  -920, 16376, 15460,   431,   736,  6962,
    -920, 11968,  -920,  -920,  2796,   432,   436, 16479, 11968,   307,
    -920,  5396,   -40, 15534,  -920,  -920, 17161,  5396, 17161,  -920,
   16527, 17161,  -920, 10064, 17161,   136, 15050,  -920,   318,  -920,
    -920,  -920,  -920,  3397,  3499,  -920, 17161, 17756,   633,  -920,
       6,   260, 11968,  -920,   756,   735,  -920,   617,   617,  -920,
    -920,  -920,   738, 16567, 11968,   872, 16718,   334,  -920,   644,
     339,   349,  -920,  -920, 17161,   874,  5396,  -920,  -920,  -920,
    -920,  -920,  8513,  8513,  -920,  -920,  -920,  -920,  -920,   310,
   17161,  -920,   309,    95,   741, 11968,  -920,    76, 15579,    54,
   15130,  -920,   139, 11968,  6962,  6790,  -920,  -920,  -920, 16758,
   17161,  3159,  -920,  -920,  -920,  -920, 15210,  -920,  -920,  5746,
     742,  5921,   745,  -920, 11968, 14307,  6096,  -920,    16,  6271,
    -920,    16,   746,  3499,  -920,  -920,  -920, 17638,  -920,  -920,
    -920,   266,  -920,   -46,  -920,   378, 16799,    58,  -920,  -920,
    -920, 12484, 12656, 16914, 12828, 13000,  -920,  -920,  5396,  -920,
     743,   748,  -920,  -920,  5396,  -920,   531, 17161,  5396,  5396,
    -920, 17161,   441,   758, 11968,  -920,  -920, 10064,  -920, 17161,
    5396, 10064,  -920, 17161,  5396, 17161,  5396, 10064, 17161,  -920,
   10064, 17161,  -920,  -920,  -920,  -920,    20,  -920, 11968,   260,
     266,   266,   266,   266,   266,   266,   260,  5221,  -920,  -920,
    -920,  -920, 11968, 11968, 11968, 11968, 11968,  -920,   761,   764,
     660,  -920,  -920,  -920, 17161, 14387,  -920, 14467,  -920, 16129,
   14547, 14627,  -920, 17161,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,   760,  -920, 16962, 17002, 17081, 17121,  -920,  -920,  5396,
    5396,  5396,  5396, 11968, 11968, 11968, 11968, 16129, 16129, 16129,
   16129
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   139,   464,    55,   681,   682,
     683,   677,   678,   684,   208,   589,   128,   160,   560,   167,
     562,   589,   166,   495,   493,   128,     0,   208,   272,   161,
     198,   197,   675,   196,   208,   165,    68,   273,   335,   162,
     140,     0,     0,     0,   330,     0,   208,   169,   208,   208,
     610,   581,   685,   170,   208,   336,   554,   490,   208,     0,
       0,   183,   181,   421,   164,   563,   492,   208,   208,   208,
     208,   558,     0,   168,     0,     0,   141,   208,   676,   208,
     208,   483,   163,   310,   556,   494,   171,   208,   734,   208,
     736,   208,   737,   738,   626,   208,   735,   208,   634,   179,
     733,     0,   208,   208,     4,     0,     5,    10,    11,    12,
      48,    51,    52,    56,     0,    46,    71,    13,    77,    14,
      15,    16,    17,    30,   541,   542,    23,    47,   180,   190,
     208,   199,   648,   191,    18,     0,    32,    31,    20,     0,
     267,    19,   644,    22,    36,    33,    34,   189,   311,     0,
     187,     0,     0,   641,   334,     0,   638,   185,   351,   447,
     433,   636,   188,     0,     0,   186,   653,   632,   631,   635,
     545,   543,     0,   208,   642,   643,   647,   646,   645,     0,
     544,     0,   654,   655,   656,   679,   680,   637,   547,   546,
     639,   640,     0,    29,   562,   161,   208,   140,     0,   563,
     208,   208,     0,     0,   641,   653,   543,   642,   643,   552,
     544,   654,   655,     0,     0,   590,   129,   130,     0,   561,
       0,   484,     0,   491,     0,    21,     0,   530,   208,   136,
     142,   153,   148,   147,   155,   133,   146,   156,   143,   137,
     157,   131,   158,   151,   145,   152,   149,   150,   132,   134,
     144,   154,   159,   138,   320,   135,   207,     0,     0,     0,
       0,    70,    69,    71,   208,     0,     0,     0,     0,     0,
       0,     0,   501,     0,   176,    41,     0,   302,     0,   301,
     719,   614,   611,   612,   613,   497,   555,   720,     7,   208,
     333,   333,   428,   173,   423,   174,   424,   425,   429,   430,
     172,   431,   432,   421,   526,     0,   338,   339,   341,     0,
     422,   525,   343,   513,     0,   208,   208,   177,   638,   629,
     652,   630,     0,     0,    44,     0,   559,     0,   208,    45,
     553,     0,   208,   280,    48,   284,   281,   284,   557,     0,
     721,   723,   624,   718,   717,     0,    74,    78,     0,     0,
     208,     0,     0,   532,   625,     0,     6,   308,     0,   197,
     208,   309,     0,    49,     0,     9,    71,    50,    53,     0,
      60,     0,   208,    72,   208,     0,   495,   195,     0,   677,
     311,   649,   203,   214,     0,   211,   212,   213,   208,   208,
     208,   210,   564,   572,     0,   208,     0,   307,     0,   421,
     447,   445,   446,   444,   359,   449,   454,   457,   448,   452,
     451,   453,   456,     0,   437,   439,   443,   442,   434,   435,
     438,   441,     0,   488,     0,   485,     0,     0,   633,    35,
     615,   208,   208,   208,   208,   208,   208,   722,   208,   208,
     208,   208,   208,     0,   623,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   390,   397,   398,   399,   394,   396,   208,   208,   392,
     395,   393,   391,   208,   401,   400,   208,   208,   208,   208,
     208,   208,     0,     0,   211,   208,   208,     0,   594,     0,
      37,    25,    38,     0,    26,   208,    39,   541,     0,   536,
     537,   540,     0,     0,     0,   241,     0,   390,   237,   208,
       0,     0,    24,   208,     0,   251,    27,     0,   505,   507,
       0,   524,   208,   510,   208,     0,   175,    40,   208,   304,
       0,   208,   208,    42,     0,     0,   184,   182,   426,   427,
       0,   422,   447,   421,     0,   528,   208,   121,   650,   651,
     349,     0,   178,     0,    43,   208,     0,   291,     0,   282,
     283,    28,    76,    75,    79,   208,   686,     0,   208,   208,
     673,   671,     8,   497,   208,   208,   209,    56,   208,    54,
       0,    65,     0,   119,     0,   114,     0,    86,     0,   208,
     208,   192,   311,     0,   205,     0,   202,     0,     0,     0,
       0,     0,   573,     0,     0,   530,     0,     0,     0,   278,
     312,     0,     0,     0,   328,     0,   321,   408,     0,   410,
     414,     0,   455,   450,   518,     0,   520,   458,   440,   436,
     406,   136,   386,   142,   384,   148,   147,   146,   143,   137,
     388,   157,   158,   145,   149,   132,   144,   159,   383,   365,
     368,   366,   367,   389,   378,   369,   382,   374,   372,   385,
     373,   371,   376,   381,   370,   375,   379,   380,   377,   387,
     208,   362,     0,   406,   363,   406,   360,   208,   487,     0,
     482,   500,   499,   714,   713,   716,   725,   724,   729,   728,
     710,   707,   708,   709,   627,   697,   139,     0,   667,   668,
     141,   666,   665,   621,   701,   712,   706,   704,   715,   705,
     703,   695,   700,   702,   711,   694,   698,   699,   696,   622,
       0,     0,     0,     0,     0,     0,     0,     0,   727,   726,
     731,   730,   208,   208,     0,     0,     0,   278,     0,   600,
     601,   607,   606,   608,   603,   609,   605,   592,     0,   595,
       0,     0,     0,     0,   531,   208,   549,   208,   208,   208,
     240,     0,   236,     0,     0,     0,     0,   548,     0,   254,
       0,   252,     0,   508,     0,   523,   522,   208,   503,   620,
     502,   303,   300,     0,     0,     0,     0,   664,   528,   344,
     340,     0,   208,   529,   510,   342,   208,   348,     0,     0,
       0,   550,     0,   285,   687,   628,   672,   533,   532,   674,
     534,     0,     0,     0,    57,    58,     0,    61,    63,    67,
      66,   208,    97,     0,     0,   208,    92,   208,   208,   208,
      64,   208,   365,   208,   366,   367,   208,   208,   377,     0,
     402,   403,    81,    80,    91,     0,     0,   313,     0,     0,
     208,     0,   217,   216,   208,   574,     0,     0,   208,   208,
       0,     0,   208,   208,   690,     0,   225,     0,     0,   208,
     223,     0,   275,   208,     0,   333,   319,   325,   208,   323,
     318,   421,   409,   458,   516,   515,   514,   517,   459,   465,
     421,   358,     0,   364,     0,   354,   355,   489,   486,     0,
       0,   127,   125,   126,   124,   123,   122,   662,   663,     0,
     208,   688,   208,   208,   208,   208,   593,   602,   604,   208,
     591,   160,   167,   166,   165,   162,   169,   170,   164,   168,
     163,   171,     0,   208,   208,   496,   539,   538,   249,   248,
       0,   243,     0,   208,   239,     0,   245,     0,   277,     0,
       0,   506,   509,   510,   511,   512,   299,     0,     0,     0,
     510,   208,   458,   528,   527,   208,   417,   415,   350,   293,
       0,   208,   292,   295,   551,     0,   286,   289,     0,   208,
       0,   208,   208,    59,   120,   117,    98,   109,   104,   103,
     102,   111,    99,   112,   107,   101,   108,   105,   106,   100,
     110,   113,   208,   116,   115,    88,    87,     0,     0,   208,
     193,   208,   194,   313,   333,     0,     0,     0,   208,     0,
     233,     0,     0,     0,   689,   231,   575,     0,   692,   691,
       0,   568,   226,   208,   567,     0,     0,   224,     0,   306,
     183,   181,   305,   333,   333,   314,   329,     0,     0,   411,
     465,     0,   208,   352,   413,     0,   361,   406,   406,   669,
     670,   277,     0,     0,   208,     0,     0,     0,   596,   510,
     653,   653,   242,   238,   580,   244,     0,    73,   274,   253,
     250,   504,   208,   208,   498,   418,   416,   345,   346,   510,
     405,   297,     0,     0,     0,   208,   287,     0,     0,     0,
       0,    62,     0,   208,   208,   208,    93,    95,    84,    83,
      82,   333,   200,   206,   204,   215,     0,   235,   234,     0,
       0,     0,     0,   232,   208,     0,     0,   221,     0,     0,
     219,     0,   279,   333,   316,   315,   324,     0,   331,   479,
     480,     0,   481,   467,   470,     0,   466,     0,   407,   356,
     357,   208,   208,     0,   208,   208,   279,   597,     0,   247,
       0,     0,   420,   419,     0,   296,     0,   290,     0,     0,
     118,    89,     0,     0,   208,   201,   218,   208,   229,   577,
       0,   208,   227,   576,     0,   693,     0,   208,   566,   222,
     208,   565,   220,   276,   317,   327,     0,   478,   208,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   460,   462,
     353,   461,   208,   208,   208,   208,   208,   246,   616,   617,
     298,   288,    94,    96,    85,     0,   230,     0,   228,   571,
       0,     0,   332,   469,   471,   472,   475,   476,   477,   473,
     474,   468,   463,     0,     0,     0,     0,   618,   619,     0,
       0,     0,     0,   208,   208,   208,   208,   579,   578,   570,
     569
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -920,  -920,  -920,     0,   -54,  2109,  -920,    33,   823,  -920,
    -920,   367,  -391,  -100,  -920,   357,  -920,  -920,   -45,  -920,
    -920,    22,   686,  -920,  -545,  2940,   565,  -576,  -920,  -801,
    -920,  -920,  -920,   124,  -920,  -920,  -920,   917,  -920,    78,
     884,  -920,  -920,  -457,   663,  -920,  -838,  -920,  -920,    26,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,    17,  -920,
    -148,   583,  -340,  -920,   564,  -920,  -920,     1,  1534,  -920,
    -920,  -920,  -920,   618,  -920,   -24,  -920,  -920,  -920,  -920,
     428,  -920,  -920,  -920,  -107,  -920,  -346,  -802,  -920,  -920,
    -920,  -920,  -920,  -603,  -920,    50,  -920,  -920,  -920,  -920,
    -920,   420,  -920,   -28,  -920,  -920,  -920,  -920,   584,  -920,
    -920,  -920,  -920,    71,  -406,  -179,  -795,  -919,  -653,  -920,
      68,  -920,    86,   -49,   667,   -48,  -920,  -920,  -364,  -817,
    -920,  -227,  -920,   -79,  -233,  -225,  -541,  -920,  -920,   127,
     297,  -920,  -201,   847,  -920,  -920,  -161,   454,   208,  -255,
    -762,  -602,  -920,  -920,  -920,  -595,  -515,  -920,  -755,   -23,
     -53,  -920,  -259,  -496,  -560,    -2,  -920,  -920,  -920,  -920,
    -920,  -920,   200,  -920,   961,  -450,  -920,    65,  -920,  -920,
     219,   651,  -920,  -920,  -920,  -920,  2549,  -920,   -60,   927,
    -920,  1211,  1380,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -477
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   347,   105,   106,   107,   108,   109,   364,
     365,   110,   111,   112,   113,   370,   579,   816,   817,   114,
     115,   116,   375,   117,   118,   119,   348,   844,   586,   845,
     120,   121,  1003,   583,   584,   122,   123,   218,   253,   270,
     124,   255,   125,   304,   533,   126,   127,   291,   290,   128,
     129,   130,   131,   132,   133,   595,   848,   134,   202,   136,
     390,   391,   603,   137,   259,   138,   770,   771,   203,   140,
     141,   142,   143,   559,   803,   978,   144,   145,   799,   973,
     277,   278,   146,   147,   148,   149,   396,   875,   150,   151,
     615,  1047,  1048,   616,   152,   204,  1196,   154,   155,   305,
     306,   307,   962,   156,   319,   551,   796,   157,   158,  1147,
     159,   160,   672,   673,   840,   841,   842,   966,   891,   400,
     618,  1055,   619,   620,   309,   541,   422,   404,   413,   889,
    1210,  1211,   161,  1053,  1143,  1144,  1145,   162,   163,   424,
     425,   678,   164,   165,   222,   534,   271,   272,   519,   520,
     778,   311,   887,   626,   627,   523,   312,   793,   794,   349,
     351,   352,   498,   499,   500,   205,   167,   168,   392,   169,
     393,   170,   206,   172,   214,   215,   748,   749,   750,   173,
     207,   208,   176,   177,   178,   320,   179,   382,   501,   210,
     181,   211,   212,   184,   185,   186,   187,   606,   188,   189,
     190,   191,   192
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     166,   477,   104,   427,   226,   511,   797,   613,   322,   303,
     877,   368,   752,   839,   308,   310,   674,   768,   818,   135,
     895,   493,   896,   886,   166,   380,   883,   339,  1008,  1004,
     535,   795,   967,   960,   593,   521,   621,  1045,   488,   521,
     318,  1085,   353,   135,   314,  1014,   383,  -326,   798,   273,
     607,   273,   153,  1207,   486,   213,   521,   865,   263,   318,
     504,   524,   484,   386,   387,   775,  1050,   569,  1051,   556,
     624,   480,  1198,   166,   318,   481,   153,   166,   166,   550,
     213,   509,   437,   281,   292,   292,  -521,   273,  -521,  -272,
     443,  1207,   135,   570,   686,   688,   135,   135,   383,   166,
     216,  1199,  -519,   166,   294,   294,   384,   254,   296,   296,
     856,   858,   333,   336,   385,   386,   387,  -521,   135,   266,
     267,   268,   135,  -294,  1052,   153,  -347,   362,   273,   153,
     153,  -519,   366,   297,   297,  1119,   298,   298,    59,    59,
     728,   730,  -273,   282,   213,  1088,   735,     3,   299,   299,
     193,   153,   217,   857,   861,   153,   910,   867,   871,  -521,
     525,  -521,   283,  -521,  -294,   528,   301,   301,   885,  1126,
    1162,   302,   302,  -521,   213,  -519,  1045,   284,   789,   953,
     522,  -521,   979,   213,   522,  -521,   362,  -519,   362,  -322,
     362,  1081,   388,   357,   362,   485,   213,   937,  1086,  -255,
     318,   522,   171,  -521,   228,  1134,  1135,  -521,  1089,   489,
     859,  1111,   166,  -322,  1108,   625,   575,   970,  -294,   726,
     727,   174,   213,  1208,   362,  -521,   171,   397,   362,   617,
    -412,   135,   383,  -256,  -519,   812,   318,   909,   860,  -519,
     384,   423,   495,   675,   538,   174,   846,   389,   385,   386,
     387,  1168,  1007,   622,   213,   859,   213,   264,   971,   863,
     361,   318,   772,  1139,   153,   362,   166,   696,   565,  1139,
     628,   166,   592,  1045,   372,   171,   483,   964,   879,   171,
     171,   495,    83,   911,   917,   135,  1009,   864,   539,   791,
     135,   -70,   566,   221,   174,  1194,   800,   623,   174,   174,
     318,   171,   880,    24,  1173,   171,   362,  1157,  -262,   980,
    1170,   -70,   972,   373,   629,  1140,   536,   537,   153,   197,
     174,  1140,   696,   153,   174,  -261,   388,  1163,   918,   783,
     -70,   773,   753,   786,   383,   502,   503,  1069,   495,   223,
     697,   374,  1164,   433,   228,   303,   166,   518,   564,   881,
     385,   386,   387,   698,  1141,    57,   496,   774,   -70,  1087,
     166,   861,   867,   871,  1142,   135,   604,    66,   700,   166,
    1142,   919,   610,   495,   197,   882,   -69,  1038,   401,   135,
     256,   574,   166,   699,   757,   949,  -586,   545,   135,  -270,
      85,   547,   773,  -660,   849,  -660,   -69,   920,   153,   402,
     261,   135,   434,   262,  1149,  1150,   435,   721,   722,   403,
     948,   950,   153,   723,   171,   -69,   724,   725,   951,  1067,
     676,   153,   818,   700,   872,   478,  1102,   479,   388,   701,
     264,   873,   773,   174,   153,   777,   254,   982,   433,   318,
     318,   757,   752,   -69,  1136,   318,   334,   334,   318,   318,
     318,   318,   437,   438,   495,   439,   440,   441,   963,   442,
     443,   965,   734,   983,   446,   757,  -260,  1061,   171,   193,
     452,   453,   609,   171,   456,   562,   614,   457,   458,   459,
    1117,   757,  -257,   389,  1044,   318,  -599,   174,   674,   303,
      38,  1132,   174,  -333,   308,   310,  -598,   434,   433,  -264,
     671,   435,   166,  1200,   166,   193,   934,  1156,   808,   808,
     481,   819,  -599,   368,  -333,   807,   810,    55,  1022,   679,
     525,   135,  -598,   135,  -333,   265,   821,   318,  1172,  1035,
    1201,  -268,  1202,  1203,  1195,  1204,  1205,   680,   682,  -736,
     318,   318,   825,  -736,   822,   525,  1009,   437,   171,   594,
     318,   811,   441,  -265,   153,   443,   153,   434,   433,   446,
     826,   435,   171,   935,  1010,   737,  -661,   174,  -661,   525,
    1062,   171,  1120,  1122,  1065,  -660,   288,  -660,  1009,   679,
     166,   174,   564,   525,   171,  1128,  1131,  1084,  1009,  -657,
     174,  -657,   769,   686,   728,   518,  1106,  1113,   289,   135,
    1197,  1114,  1209,   174,  -259,  -737,  1222,   437,   782,  -737,
     327,  -263,   441,  -269,   442,   443,  -738,   434,   788,   446,
    -738,   435,   518,  1044,   328,   959,   453,  -733,   437,  -271,
    -258,  -733,   153,   441,   459,  -266,   443,   433,     7,  1097,
     446,  1099,   362,  -659,   514,  -659,   369,  1120,  1122,  1128,
    1131,  1209,  1133,   175,  -658,   371,  -658,  1137,   394,  1235,
    1236,  1237,  1238,  1239,  1240,   395,   830,   437,   438,   398,
     439,   440,   441,   399,   442,   443,   851,   175,   899,   446,
     900,   433,  -583,  -582,   428,   487,   453,   429,  1016,  -587,
     373,   614,   457,   458,   459,   936,   434,  -588,  -585,  -584,
     435,   362,   491,   494,   171,   318,   171,   512,   542,   275,
     543,   513,   531,   532,   546,  1058,   552,   955,   555,   572,
    1044,   433,   561,   174,   568,   174,   175,   318,   571,   318,
     175,   175,   577,   324,   578,   588,   598,   677,   443,   611,
     434,   681,   720,   733,   435,   755,   437,   438,   932,   318,
     440,   441,   175,   442,   443,   756,   175,   423,   446,   166,
     757,   758,   166,   759,   766,   453,   761,   764,   767,   777,
     787,   457,   458,   459,   802,   792,   801,   847,   135,   850,
     434,   135,   171,   854,   435,   876,   495,   878,   888,   890,
     437,   438,  -139,   439,   440,   441,  -160,   442,   443,   444,
     893,   174,   446,   447,   448,   901,   450,   451,   452,   453,
    -167,   153,   456,  -166,   153,   457,   458,   459,  -165,  -162,
    -140,  1043,  -169,  1160,  1161,  -170,   460,   303,  -164,  -168,
     437,  1019,  -141,  -163,  -171,   441,   303,   442,   443,   894,
     292,   908,   446,   902,   947,   318,   903,   907,   292,   453,
     912,   518,   904,   905,   166,   457,   458,   459,   166,   906,
     294,   956,   940,   166,   296,   175,   166,   915,   294,   916,
     942,   961,   296,   135,   969,   974,  1012,   135,  1013,  1018,
     977,  1009,   135,  1039,  1021,   135,  1076,   318,  1027,   297,
    1059,  1060,   298,  1077,   355,  1078,  1082,   297,  1083,  1095,
     298,  1107,    59,   881,   299,  1154,   153,  1199,  1148,  1158,
     153,  1193,   299,  1151,  1166,   153,  1218,  1180,   153,   175,
    1184,  1219,   301,  1223,   175,  1247,   423,   302,  1248,   180,
     301,  1070,  1071,   363,   814,   302,   582,  1101,   166,   527,
     166,   529,   224,   166,   368,   984,   509,   576,  1093,   510,
    1079,  1094,   599,   180,   318,   560,   781,   135,  1054,   135,
    1043,   171,   135,   790,   171,  1057,   596,  1049,   166,  1232,
     540,  1138,   671,  1241,  1234,  1015,   898,   377,   166,   780,
     174,   952,   220,   174,  1068,     0,   433,  1092,   554,     0,
     153,     0,   153,     0,     0,   153,     0,   135,     0,   175,
       0,     0,   180,     0,     0,     0,   180,   180,     0,     0,
       0,     0,     0,   175,     0,     0,     0,     0,     0,   166,
     153,     0,   175,     0,     0,   166,     0,   769,   180,     0,
     153,     0,   180,   368,     0,   175,     0,   368,   135,     0,
       0,     0,     0,     0,   135,   434,   509,     0,   509,   435,
       0,   509,   509,   977,   318,   318,   171,  1043,     0,     0,
     171,     0,     0,     0,     0,   171,     0,     0,   171,     0,
       0,   153,     0,     0,   166,   174,     0,   153,     0,   174,
       0,     0,     0,     0,   174,     0,     0,   174,     0,     0,
       0,     0,     0,   135,     0,   437,   438,     0,     0,     0,
     441,     0,   442,   443,     0,     0,     0,   446,     0,     0,
       0,     0,   497,     0,   453,     0,     0,   166,     0,   166,
     457,   458,   459,     0,   166,   614,   153,   166,     0,     0,
       0,     0,     0,     0,     0,     0,   135,     0,   135,     0,
     171,   180,   171,   135,     0,   171,   135,     0,   497,     0,
       0,     0,     0,     0,     0,   175,   166,   175,     0,   174,
       0,   174,   166,     0,   174,     0,   166,   166,     0,   153,
     171,   153,     0,   497,   405,   135,   153,     0,   166,   153,
     171,   135,   166,     0,   166,   135,   135,     0,     0,   174,
       0,     0,     0,     0,   406,   180,     0,   135,   407,   174,
     180,   135,     0,   135,     0,   166,     0,  1242,   153,     0,
       0,     0,   497,   182,   153,   614,     0,     0,   153,   153,
       0,   171,     0,   408,   135,     0,   409,   171,     0,     0,
     153,     0,     0,   175,   153,     0,   153,   182,   410,     0,
     174,     0,     0,     0,     0,     0,   174,   166,   166,   166,
     166,     0,     0,     0,   414,     0,   411,   153,     0,     0,
       0,   412,   852,   853,     0,     0,   135,   135,   135,   135,
     866,   870,     0,     0,   415,   180,   171,     0,   416,     0,
       0,     0,     0,     0,     0,     0,   182,     0,     0,   180,
     182,   182,     0,     0,     0,   174,     0,     0,   180,   153,
     153,   153,   153,   417,     0,     0,   418,     0,     0,     0,
       0,   180,   182,     0,     0,     0,   182,     0,   419,   171,
       0,   171,     0,     0,     0,     0,   171,   702,     0,   171,
       0,     0,     0,     0,     0,     0,   420,     0,   174,     0,
     174,   421,     0,     0,     0,   174,     0,     0,   174,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   171,     0,
       0,     0,   497,   497,   171,     0,     0,     0,   171,   171,
       0,     0,     0,     0,     0,     0,     0,   174,     0,     0,
     171,     0,   183,   174,   171,     0,   171,   174,   174,     0,
       0,     0,     0,     0,     0,     0,     0,   497,     0,   174,
       0,     0,     0,   174,     0,   174,   183,   171,     0,     0,
       0,     0,   175,     0,     0,   175,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   182,   174,     0,     0,     0,
       0,   180,     0,   180,     0,     0,     0,     0,     0,   497,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
     171,   171,   171,     0,     0,   183,     0,     0,     0,   183,
     183,     0,   497,     0,     0,     0,     0,     0,   174,   174,
     174,   174,     0,     0,     0,   866,   870,     0,     0,   182,
       0,   183,     0,     0,   182,   183,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     426,     0,   229,     0,     0,     0,     0,   175,     0,   180,
       0,   175,     0,     0,     0,     0,   175,   230,   231,   175,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
    1032,     0,     0,   234,  1037,     0,   139,     0,     0,   235,
       0,     0,     0,     0,     0,   236,     0,     0,   433,   237,
       0,     0,   238,     0,   239,     0,     0,     0,     0,   182,
     139,     0,     0,   240,     0,     0,     0,     0,     0,     0,
     241,   242,     0,   182,     0,     0,     0,     0,   243,     0,
       0,     0,   182,     0,     0,     0,     0,     0,   244,     0,
       0,   175,     0,   175,   183,   182,   175,   245,     0,   246,
       0,   247,     0,   248,     0,     0,   249,   434,     0,   139,
     250,   435,   251,     0,     0,   252,     0,   497,     0,     0,
       0,   175,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   175,     0,     0,     0,   139,     0,     0,     0,   139,
       0,   497,     0,     0,     0,     0,     0,     0,   183,     0,
       0,     0,     0,   183,     0,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,     0,   446,
       0,   269,   175,     0,     0,   452,   453,     0,   175,   456,
       0,     0,   457,   458,   459,     0,     0,     0,   180,     0,
       0,   180,     0,   460,     0,     0,     0,     0,  1127,  1130,
       0,     0,     0,     0,     0,     0,     0,   985,   702,     0,
       0,     0,     0,     0,     0,   182,     0,   182,     0,   357,
       0,     0,     0,    23,    24,     0,     0,   175,   183,     0,
       0,     0,   358,     0,    30,   359,     0,     0,     0,     0,
      33,     0,   183,     0,     0,     0,     0,    38,   139,     0,
       0,   183,     0,     0,     0,     0,  1165,   497,     0,     0,
       0,     0,  1127,  1130,   183,     0,     0,     0,     0,   360,
     175,     0,   175,     0,    55,     0,    57,   175,     0,  1176,
     175,     0,     0,   180,     0,     0,   361,   180,    66,     0,
       0,  1189,   180,   182,  1192,   180,     0,     0,     0,   497,
       0,     0,   139,     0,     0,     0,    81,   139,    83,   175,
       0,    85,     0,     0,     0,   175,     0,     0,     0,   175,
     175,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   175,     0,     0,     0,   175,     0,   175,   874,     0,
       0,     0,  -313,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   175,     0,
    -313,     0,     0,     0,  -313,  -313,   497,   180,     0,   180,
       0,     0,   180,  -313,     0,  -313,  -313,     0,     0,     0,
       0,  -313,   139,     0,   183,     0,   183,   476,  -313,     0,
    -657,  -313,  -657,     0,     0,     0,   139,   180,     0,     0,
     175,   175,   175,   175,     0,   139,     0,   180,     0,     0,
    -313,     0,  -313,     0,     0,  -313,     0,  -313,   139,  -313,
       0,  -313,  -313,     0,  -313,     0,     0,  -313,     0,  -313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -313,   180,  -313,
       0,     0,  -313,     0,   180,     0,     0,     0,     0,     0,
       0,     0,   183,     0,     0,     0,   497,   497,     0,     0,
       0,     0,   182,   229,     0,   182,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   230,   231,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,  -313,   180,   234,     0,     0,     0,  -313,     0,
     235,     0,     0,     0,     0,     0,   236,     0,     0,     0,
     237,     0,     0,   238,     0,   239,     0,     0,     0,     0,
       0,     0,     0,     0,   240,     0,     0,     0,   139,     0,
     139,   241,   242,     0,     0,     0,   180,     0,   180,   243,
       0,     0,     0,   180,     0,     0,   180,     0,     0,   244,
       0,     0,     0,     0,     0,     0,     0,   182,   245,     0,
     246,   182,   247,     0,   248,     0,   182,   249,     0,   182,
       0,   250,     0,   251,     0,   180,   252,     0,     0,     0,
       0,   180,     0,     0,     0,   180,   180,     0,     0,     0,
     696,     0,     0,     0,     0,     0,     0,   180,     0,     0,
       0,   180,     0,   180,     0,   921,   139,     0,   922,     0,
       0,     0,     0,   923,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   195,   180,   225,     0,     0,     0,     0,
     362,   183,   975,   924,   183,     0,     0,     0,     0,     0,
     925,   182,   197,   182,     0,     0,   182,   229,     0,     0,
       0,   926,     0,     0,     0,     0,     0,     0,     0,   927,
       0,     0,   230,   231,     0,   232,   180,   180,   180,   180,
     233,   182,     0,     0,   329,     0,   928,     0,   234,     0,
       0,   182,     0,     0,   235,     0,     0,   929,     0,     0,
     236,   700,     0,     0,   237,     0,     0,   238,   930,   239,
       0,     0,     0,   931,   356,     0,     0,     0,   240,     0,
       0,     0,     0,     0,     0,   241,   242,     0,     0,     0,
       0,     0,   182,   243,     0,     0,   183,     0,   182,     0,
     183,     0,     0,   244,     0,   183,     0,     0,   183,     0,
       0,     0,   245,     0,   246,     0,   247,     0,   248,     0,
     229,   249,     0,     0,     0,   250,     0,   251,     0,     0,
     252,     0,     0,     0,     0,   230,   231,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,   182,     0,     0,
       0,   234,     0,     0,     0,   139,     0,   235,   139,     0,
       0,     0,     0,   236,     0,     0,     0,   237,     0,     0,
     238,     0,   239,     0,     0,     0,     0,     0,     0,     0,
     183,   240,   183,   490,     0,   183,   269,     0,   241,   242,
     182,     0,   182,     0,     0,     0,   243,   182,     0,     0,
     182,     0,     0,     0,     0,     0,   244,     0,     0,     0,
     183,     0,     0,     0,     0,   245,     0,   246,     0,   247,
     183,   248,     0,     0,   249,     0,     0,     0,   250,   182,
     251,     0,     0,   252,     0,   182,     0,   516,     0,   182,
     182,     0,   526,     0,     0,     0,     0,     0,     0,     0,
     139,   182,     0,     0,   139,   182,     0,   182,     0,   139,
       0,   183,   139,     0,     0,     0,     0,   183,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   182,     0,
       0,   229,     0,     0,     0,     0,     0,     0,     0,   517,
       0,     0,     0,     0,     0,     0,   230,   231,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,     0,   183,     0,   235,     0,
     182,   182,   182,   182,   236,     0,     0,     0,   237,     0,
       0,   238,     0,   239,   139,     0,   139,     0,     0,   139,
       0,     0,   240,     0,     0,     0,     0,     0,     0,   241,
     242,     0,     0,   597,     0,     0,     0,   243,     0,   183,
       0,   183,     0,     0,   139,     0,   183,   244,     0,   183,
       0,     0,     0,     0,   139,     0,   245,     0,   246,     0,
     247,     0,   248,     0,     0,   249,     0,     0,     0,   250,
       0,   251,     0,     0,   252,     0,     0,     0,   183,     0,
       0,     0,     0,     0,   183,     0,     0,     0,   183,   183,
       0,     0,     0,     0,     0,   139,     0,     0,     0,     0,
     183,   139,     0,   209,   183,     0,   183,   219,     0,     0,
       0,     0,     0,     0,     0,     0,   227,     0,     0,     0,
       0,     0,     0,   260,     0,     0,     0,   183,     0,     0,
     544,     0,     0,     0,     0,   276,     0,   279,   280,     0,
       0,     0,     0,   285,     0,   286,     0,   287,     0,     0,
     139,     0,   313,   760,     0,   762,   227,   321,   323,   325,
     326,     0,     0,     0,   330,     0,   331,     0,     0,   183,
     183,   183,   183,   338,     0,     0,   321,     0,   340,     0,
     341,     0,     0,   342,   343,     0,   344,     0,     0,     0,
     227,   321,   354,   139,     0,   139,     0,     0,     0,     0,
     139,     0,     0,   139,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   357,     0,     0,     0,    23,    24,   378,
       0,   381,     0,     0,     0,     0,   358,     0,    30,   359,
       0,     0,   139,     7,    33,     0,     0,     0,   139,     0,
       0,    38,   139,   139,  -333,     0,     0,     0,     0,     0,
       0,   357,     0,     0,   139,    23,    24,   -68,   139,     0,
     139,     0,   430,   360,   358,  -333,    30,   359,    55,     0,
      57,     0,    33,     0,    61,  -333,   -68,    62,     0,    38,
     361,   139,    66,     0,     0,   482,     0,     0,     0,   330,
     227,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      81,   360,    83,     0,     0,    85,    55,     0,    57,     0,
      59,     0,  1040,     0,     0,  1041,     0,   321,   361,     0,
      66,     0,     0,   139,   139,   139,   139,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    81,     0,
      83,     7,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   321,     0,    99,     0,     0,     0,   357,
     362,     0,     0,    23,    24,     0,     0,     0,     0,     0,
       0,     0,   358,     0,    30,   359,     0,     0,   321,     0,
      33,     0,     0,     0,     0,     0,     0,    38,     0,   330,
       0,     0,     0,    99,     0,     0,     0,     0,     0,  1042,
       0,     0,     0,     0,   548,   549,     0,     0,     0,   360,
     941,     0,     0,   944,    55,     0,    57,   321,    59,     0,
    1040,   558,     0,  1041,     0,     0,   361,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   567,
       0,     0,     0,     0,     0,     0,    81,     0,    83,   573,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   585,     0,   587,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   600,   605,   608,
       0,     0,     0,     0,   227,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,     0,  1020,     0,  1112,     0,  1025,
       0,     0,     0,     0,   526,     0,     0,   526,     0,     0,
     683,   684,   685,   687,   689,   690,   274,   691,   692,   693,
     694,   695,     0,   703,   704,   705,   706,   707,   708,   709,
     710,   711,   712,   713,   714,   715,   716,   717,   718,   719,
     274,     0,     0,     0,     0,     0,   321,   321,     0,   335,
     337,     0,   321,     0,     0,   321,   321,   321,   321,   729,
     731,     0,     0,     0,   605,   736,     0,     0,   751,     0,
       0,     0,     0,     0,   754,     0,     0,     0,     0,  1072,
     367,  1073,     0,     0,  1075,   763,     0,     0,   765,     0,
       0,     0,   321,     0,     0,     0,     0,     0,     0,     0,
       0,   776,     0,   779,     0,     0,     0,   279,     0,  1091,
     784,   785,     0,     0,     0,     0,     0,     0,     0,  1025,
       0,     0,   313,     0,     0,   313,     0,     0,     0,     0,
       0,     0,     0,     0,   321,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   754,     0,     0,   321,   321,     0,
       0,     0,     0,   227,   813,     0,     0,   321,     0,     0,
    1118,     0,     0,     0,     0,     0,  1123,     0,   843,   843,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   855,     0,     0,     0,     0,     0,     0,     0,     0,
     492,     0,     0,     0,     7,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   313,     0,     0,     0,     0,     0,
       0,     0,   357,     0,     0,  1159,    23,    24,     0,     0,
       0,     0,     0,     0,     0,   358,     0,    30,   359,   505,
     508,     0,     0,    33,     0,     0,     0,   515,     0,     0,
      38,     0,     0,     0,     0,     0,   274,     0,   274,   892,
       0,     0,     0,     0,     0,   274,   897,     0,  1178,     0,
    1182,     0,   360,     0,     0,   526,     0,    55,   526,    57,
       0,    59,     0,  1040,     0,     0,  1041,     0,     0,   361,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   274,     0,  1217,     0,    81,
       0,    83,     0,  1220,    85,     0,     0,  1178,  1182,     0,
       0,   763,   321,     0,     0,     0,     0,     0,     0,  1226,
       0,     0,     0,  1228,     0,   762,     0,     0,     0,     0,
       0,     0,     0,     0,   321,     0,   321,   938,   939,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,   321,     0,     0,     0,
    1175,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,     0,     0,     0,   968,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   762,   762,
     762,   762,     0,     0,     0,     0,     0,     0,     0,     0,
     585,     0,     0,     0,  1005,     0,  1006,   843,   843,     0,
     340,     0,   341,     0,     0,   343,   344,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1017,
       0,     0,     7,   227,     0,     0,     0,  1023,   754,  1026,
       0,  1028,  1030,     0,  1031,     0,     0,  1034,  1036,     0,
     357,     0,   321,     0,    23,    24,     0,  1046,     0,     0,
       0,     0,     0,   358,     0,    30,   359,     0,     0,     0,
       0,    33,     0,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1063,
    1026,  1031,  1034,  1066,   321,     0,     0,     0,   751,     0,
     360,     0,     0,     0,     0,    55,     0,    57,     0,   355,
       0,  1040,   687,   729,  1041,     0,     0,   361,     0,    66,
       0,     0,  1074,     0,     0,     0,     0,     0,   274,     0,
       0,     0,     0,     0,     7,     0,     0,    81,     0,    83,
     313,     0,    85,     0,  1090,     0,     0,     0,     0,     0,
     227,     0,   357,     0,     0,     0,    23,    24,  1098,     0,
    1100,   321,     0,     0,     0,   358,     0,    30,   359,   274,
     274,     0,     0,    33,     0,     0,     0,   274,   274,     0,
      38,   843,     0,     0,     0,     0,     0,     0,  1109,     0,
    1110,     0,    99,     0,     0,     0,     0,  1116,     0,     0,
       0,     0,   360,     0,     0,     0,     0,    55,     0,    57,
       0,     0,  1125,  1040,     0,     0,  1041,     0,     0,   361,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1146,     0,     0,     0,     0,     0,     0,     0,    81,
       0,    83,     0,  1153,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   321,   321,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1167,     0,     0,     0,     0,     0,
       0,     0,  1171,   843,   843,     0,     0,     0,     0,     0,
       0,   433,     0,     0,    99,     0,     0,     0,  1179,     0,
    1183,     0,     0,  1185,     0,  1188,     0,     0,  1191,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1179,  1183,     0,  1188,  1191,   946,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,     0,     0,  1224,   435,     0,  1225,     0,     0,     0,
    1227,     0,     0,     0,     0,  1229,  1230,     0,     0,  1231,
       0,     0,   976,     0,     0,     0,     0,  1233,     0,     0,
       0,     0,   274,   274,     0,     0,     0,     0,     0,     0,
       0,  1243,  1244,  1229,  1245,  1246,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,     0,   450,   451,   452,   453,
     454,     0,   456,     0,     0,   457,   458,   459,  1257,  1258,
    1259,  1260,  1257,  1258,  1259,  1260,   460,   274,     0,     0,
       0,   274,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    -2,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,    16,
      17,    18,     0,    19,     0,    20,    21,  -208,    22,    23,
      24,    25,    26,    27,     0,  -208,    28,     0,    29,     0,
      30,    31,    32,  -208,  -208,  -208,    33,    34,    35,   -68,
    1080,    36,    37,    38,     0,    39,  -333,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -333,  1096,     0,
      55,    56,    57,    58,    59,    60,    61,  -333,   -68,    62,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,     0,    76,    77,     0,    78,
      79,    80,    81,    82,    83,    84,   -68,    85,    86,     0,
       0,  -208,     0,    87,     0,     0,    88,     0,     0,     0,
       0,     0,   431,     0,     0,   274,   274,     0,     0,   432,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,   433,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,    99,     0,     0,
     100,     0,   101,     0,   102,     0,  -208,     0,     0,   103,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   274,     0,     0,     0,     0,     0,   274,
     274,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,     0,     0,     0,   435,     0,   274,     0,     0,     0,
       0,     0,   506,     0,     0,   508,     0,     0,   274,     0,
       0,   274,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   507,   462,   463,   464,   465,   466,
       0,     0,   469,   470,   471,   472,     0,   474,   475,     0,
     437,   438,     0,   439,   440,   441,  1221,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,     0,   457,   458,   459,     0,   362,
       0,     0,     0,     0,     0,     0,   460,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   508,     0,   508,     0,     0,
     508,   508,     4,     0,     5,     6,     7,     8,     9,    10,
      11,    12,    13,     0,  -688,     0,    14,    15,    16,    17,
      18,  -688,    19,     0,    20,    21,  -208,    22,    23,    24,
      25,    26,    27,  -688,  -208,    28,  -688,    29,     0,    30,
      31,    32,  -208,  -208,  -208,    33,    34,    35,   -68,     0,
      36,    37,    38,     0,    39,  -333,    40,     0,    41,    42,
      43,    44,    45,     0,    46,    47,    48,    49,   -68,    50,
      51,     0,    52,    53,    54,     0,  -333,     0,     0,    55,
      56,    57,    58,     0,    60,    61,  -333,   -68,    62,    63,
      64,    65,  -688,    66,    67,    68,  -688,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,     0,    78,    79,
      80,    81,    82,    83,    84,   -68,    85,    86,     0,     0,
    -208,     0,    87,     0,     0,  -688,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,  -688,    91,  -688,  -688,  -688,  -688,  -688,
    -688,  -688,     0,  -688,  -688,  -688,  -688,  -688,  -688,  -688,
    -688,  -688,  -688,  -688,  -688,    98,    99,  -688,  -688,  -688,
       0,   101,  -688,   102,     0,  -208,     0,   345,  -688,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,    15,    16,    17,    18,     0,    19,     0,    20,
      21,  -208,    22,    23,    24,    25,    26,    27,     0,  -208,
      28,     0,    29,     0,    30,    31,    32,  -208,  -208,  -208,
      33,    34,    35,   -68,     0,    36,    37,    38,     0,    39,
    -333,    40,     0,    41,    42,    43,    44,    45,     0,    46,
      47,    48,    49,   -68,    50,    51,     0,    52,    53,    54,
       0,  -333,     0,     0,    55,    56,    57,    58,    59,    60,
      61,  -333,   -68,    62,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,    83,    84,
     -68,    85,    86,     0,     0,  -208,     0,    87,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,    99,     0,     0,   100,     0,   101,   346,   102,     0,
    -208,     0,     4,   103,     5,     6,     7,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,    15,    16,    17,
      18,     0,    19,     0,    20,    21,  -208,    22,    23,    24,
      25,    26,    27,     0,  -208,    28,     0,    29,     0,    30,
      31,    32,  -208,  -208,  -208,    33,    34,    35,   -68,     0,
      36,    37,    38,     0,    39,  -333,    40,     0,    41,    42,
      43,    44,    45,     0,    46,    47,    48,    49,   -68,    50,
      51,     0,    52,    53,    54,     0,  -333,     0,     0,    55,
      56,    57,    58,    59,    60,    61,  -333,   -68,    62,    63,
      64,    65,     0,    66,    67,    68,     0,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,     0,    78,    79,
      80,    81,    82,    83,    84,   -68,    85,    86,     0,     0,
    -208,     0,    87,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,    99,     0,     0,   100,
       0,   101,   563,   102,     0,  -208,     0,   580,   103,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,    15,    16,    17,    18,     0,    19,     0,    20,
      21,  -208,    22,    23,    24,    25,    26,    27,     0,  -208,
      28,     0,    29,     0,    30,    31,    32,  -208,  -208,  -208,
      33,    34,    35,   -68,     0,    36,    37,    38,     0,    39,
    -333,    40,     0,    41,    42,    43,    44,    45,     0,    46,
      47,    48,    49,   -68,    50,    51,     0,    52,    53,    54,
       0,  -333,     0,     0,    55,    56,    57,    58,    59,    60,
      61,  -333,   -68,    62,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,    83,    84,
     -68,    85,    86,     0,     0,  -208,     0,    87,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,    99,     0,     0,   100,     0,   101,   581,   102,     0,
    -208,     0,     4,   103,     5,     6,     7,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,    15,    16,    17,
      18,     0,    19,     0,    20,    21,  -208,    22,    23,    24,
      25,    26,    27,     0,  -208,    28,     0,    29,     0,    30,
      31,    32,  -208,  -208,  -208,    33,    34,    35,   -68,     0,
      36,    37,    38,     0,    39,  -333,    40,     0,    41,    42,
      43,    44,    45,     0,    46,    47,    48,    49,   -68,    50,
      51,     0,    52,    53,    54,     0,  -333,     0,     0,    55,
      56,    57,    58,    59,    60,    61,  -333,   -68,    62,    63,
      64,    65,     0,    66,    67,    68,     0,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,     0,    78,    79,
      80,    81,    82,    83,    84,   -68,    85,    86,     0,     0,
    -208,     0,    87,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,    99,     0,     0,   100,
       0,   101,   820,   102,     0,  -208,     0,     4,   103,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,    15,    16,    17,    18,     0,    19,     0,    20,
      21,  -208,    22,    23,    24,    25,    26,    27,     0,  -208,
      28,     0,    29,     0,    30,    31,    32,  -208,  -208,  -208,
      33,    34,    35,   -68,     0,    36,    37,    38,     0,    39,
    -333,    40,     0,    41,    42,    43,    44,    45,     0,    46,
      47,    48,    49,   -68,    50,    51,     0,    52,    53,    54,
       0,  -333,     0,     0,    55,    56,    57,    58,   355,    60,
      61,  -333,   -68,    62,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,    83,    84,
     -68,    85,    86,     0,     0,  -208,     0,    87,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,    99,     0,     0,   100,     0,   101,     0,   102,     0,
    -208,     0,     4,   103,     5,     6,     7,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,    15,    16,    17,
      18,     0,    19,     0,    20,    21,  -208,    22,    23,    24,
      25,    26,    27,     0,  -208,    28,     0,    29,     0,    30,
      31,    32,  -208,  -208,  -208,    33,    34,    35,   -68,     0,
      36,    37,    38,     0,    39,  -333,    40,     0,    41,    42,
      43,    44,    45,     0,    46,    47,    48,    49,   -68,    50,
      51,     0,    52,    53,    54,     0,  -333,     0,     0,    55,
      56,    57,    58,    59,    60,    61,  -333,   -68,    62,    63,
      64,    65,     0,    66,    67,    68,     0,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,     0,    78,    79,
      80,    81,    82,    83,    84,   -68,    85,    86,     0,     0,
    -208,     0,    87,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,    99,     0,     0,   100,
       0,   101,     0,   102,     0,  -208,     0,     4,   103,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,    15,    16,    17,    18,     0,    19,     0,    20,
      21,  -208,    22,    23,    24,    25,    26,    27,     0,  -208,
      28,     0,    29,     0,    30,    31,    32,  -208,  -208,  -208,
      33,    34,    35,   -68,     0,    36,    37,    38,     0,    39,
    -333,    40,     0,    41,    42,    43,    44,    45,     0,    46,
      47,    48,    49,   -68,    50,    51,     0,    52,    53,    54,
       0,  -333,     0,     0,    55,    56,    57,    58,     0,    60,
      61,  -333,   -68,    62,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,    83,    84,
     -68,    85,    86,     0,     0,  -208,     0,    87,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,    99,     0,     0,   100,     0,   101,     0,   102,     0,
    -208,     0,     4,   103,     5,     6,     7,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,    15,    16,    17,
      18,     0,    19,     0,    20,    21,  -208,    22,    23,    24,
      25,    26,    27,     0,  -208,    28,     0,    29,     0,    30,
      31,    32,  -208,  -208,  -208,    33,  1033,    35,   -68,     0,
      36,    37,    38,     0,    39,  -333,    40,     0,    41,    42,
      43,    44,    45,     0,    46,    47,    48,    49,   -68,    50,
      51,     0,    52,    53,    54,     0,  -333,     0,     0,    55,
      56,    57,    58,     0,    60,    61,  -333,   -68,    62,    63,
      64,    65,     0,    66,    67,    68,     0,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,     0,    78,    79,
      80,    81,    82,    83,    84,   -68,    85,    86,     0,     0,
    -208,     0,    87,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,    99,     0,     0,   100,
       0,   101,     0,   102,     0,  -208,     0,     4,   103,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,    15,    16,    17,    18,     0,    19,     0,    20,
      21,  -208,    22,    23,    24,    25,    26,    27,     0,  -208,
      28,     0,    29,     0,    30,    31,    32,  -208,  -208,  -208,
      33,  1177,    35,   -68,     0,    36,    37,    38,     0,    39,
    -333,    40,     0,    41,    42,    43,    44,    45,     0,    46,
      47,    48,    49,   -68,    50,    51,     0,    52,    53,    54,
       0,  -333,     0,     0,    55,    56,    57,    58,     0,    60,
      61,  -333,   -68,    62,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,    83,    84,
     -68,    85,    86,     0,     0,  -208,     0,    87,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,    99,     0,     0,   100,     0,   101,     0,   102,     0,
    -208,     0,     4,   103,     5,     6,     7,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,    15,    16,    17,
      18,     0,    19,     0,    20,    21,  -208,    22,    23,    24,
      25,    26,    27,     0,  -208,    28,     0,    29,     0,    30,
      31,    32,  -208,  -208,  -208,    33,  1181,    35,   -68,     0,
      36,    37,    38,     0,    39,  -333,    40,     0,    41,    42,
      43,    44,    45,     0,    46,    47,    48,    49,   -68,    50,
      51,     0,    52,    53,    54,     0,  -333,     0,     0,    55,
      56,    57,    58,     0,    60,    61,  -333,   -68,    62,    63,
      64,    65,     0,    66,    67,    68,     0,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,     0,    78,    79,
      80,    81,    82,    83,    84,   -68,    85,    86,     0,     0,
    -208,     0,    87,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,    99,     0,     0,   100,
       0,   101,     0,   102,     0,  -208,     0,     4,   103,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,    15,    16,    17,    18,     0,    19,     0,    20,
      21,  -208,    22,    23,    24,    25,    26,    27,     0,  -208,
      28,     0,    29,     0,    30,    31,    32,  -208,  -208,  -208,
      33,  1187,    35,   -68,     0,    36,    37,    38,     0,    39,
    -333,    40,     0,    41,    42,    43,    44,    45,     0,    46,
      47,    48,    49,   -68,    50,    51,     0,    52,    53,    54,
       0,  -333,     0,     0,    55,    56,    57,    58,     0,    60,
      61,  -333,   -68,    62,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,    79,    80,    81,    82,    83,    84,
     -68,    85,    86,     0,     0,  -208,     0,    87,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,    99,     0,     0,   100,     0,   101,     0,   102,     0,
    -208,     0,     4,   103,     5,     6,     7,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,    15,    16,    17,
      18,     0,    19,     0,    20,    21,  -208,    22,    23,    24,
      25,    26,    27,     0,  -208,    28,     0,    29,     0,    30,
      31,    32,  -208,  -208,  -208,    33,  1190,    35,   -68,     0,
      36,    37,    38,     0,    39,  -333,    40,     0,    41,    42,
      43,    44,    45,     0,    46,    47,    48,    49,   -68,    50,
      51,     0,    52,    53,    54,     0,  -333,     0,     0,    55,
      56,    57,    58,     0,    60,    61,  -333,   -68,    62,    63,
      64,    65,     0,    66,    67,    68,     0,    69,    70,    71,
      72,    73,    74,    75,     0,    76,    77,     0,    78,    79,
      80,    81,    82,    83,    84,   -68,    85,    86,     0,     0,
    -208,     0,    87,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,    99,     0,     0,   100,
       0,   101,     0,   102,   738,  -208,     5,     6,   103,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   194,     0,  -208,    22,
       0,   739,     0,     0,     0,     0,     0,    28,     0,   195,
       0,     0,     0,    32,  -208,  -208,  -208,     0,   196,    35,
       0,   740,     0,    37,     0,   741,    39,     0,   197,     0,
     198,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
     742,     0,    56,   743,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   199,     0,   744,     0,     0,     0,     0,
       0,    71,    72,    73,    74,   200,     0,    76,     0,     0,
      78,     0,     0,   745,    82,     0,    84,     0,   746,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    91,    92,    93,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,    96,    97,    98,     0,     0,
       0,   100,     0,   201,     0,   102,   747,  -208,     0,   884,
     103,     5,     6,     0,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   194,     0,  -208,    22,     0,     0,     0,     0,     0,
       0,     0,   293,     0,   195,     0,     0,     0,    32,  -208,
    -208,  -208,     0,   196,    35,     0,     0,     0,   295,     0,
       0,    39,     0,   197,     0,   198,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   199,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
     300,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,     0,     0,     0,     0,    94,     0,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
      96,    97,    98,     0,     0,     0,   100,     0,   201,     0,
     102,     0,  -208,     5,     6,   103,     8,     9,    10,    11,
      12,    13,     0,   632,     0,    14,     0,     0,    17,    18,
     634,    19,     0,   194,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   195,     0,     0,     0,
      32,     0,     0,     0,     0,   196,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   197,   640,   198,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     199,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,   200,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,   507,   462,   463,   464,   465,
     466,     0,     0,   469,   470,   471,   472,     0,   474,   475,
       0,   831,   832,   833,   834,   835,   653,     0,   654,     0,
      94,     0,   655,   656,   657,   658,   659,   660,   661,   836,
     663,   664,    96,   837,    98,   -90,   666,   667,   838,   669,
     201,     0,   102,     0,     0,     5,     6,   103,     8,     9,
      10,    11,    12,    13,     0,   632,     0,    14,     0,     0,
      17,    18,   634,    19,     0,   194,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   195,     0,
       0,     0,    32,     0,     0,     0,     0,   196,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   197,   640,   198,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   199,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,   200,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,   507,   462,   463,
     464,   465,   466,     0,     0,   469,   470,   471,   472,     0,
     474,   475,     0,   831,   832,   833,   834,   835,   653,     0,
     654,     0,    94,     0,   655,   656,   657,   658,   659,   660,
     661,   836,   663,   664,    96,   837,    98,     0,   666,   667,
     838,   669,   201,     0,   102,     0,     0,     0,     0,   103,
       5,     6,     7,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,    15,     0,    17,    18,     0,    19,     0,
     194,    21,     0,    22,     0,     0,     0,     0,    27,     0,
       0,    28,     0,   195,     0,     0,     0,    32,     0,     0,
       0,     0,    34,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   197,     0,   198,     0,     0,    44,    45,     0,
      46,    47,    48,    49,     0,    50,    51,     0,    52,    53,
     332,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   199,     0,     0,
       0,     0,     0,    69,    70,    71,    72,    73,    74,    75,
       0,    76,    77,     0,    78,     0,     0,     0,    82,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,     0,     0,     0,   100,     0,   101,     0,   102,
       0,     0,     5,     6,   103,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   194,     0,     0,    22,     0,   739,     0,     0,
       0,     0,     0,    28,     0,   195,     0,     0,     0,    32,
       0,     0,     0,     0,   196,    35,     0,   740,     0,    37,
       0,   741,    39,     0,   197,     0,   198,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,   742,     0,    56,   743,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   199,
       0,   744,     0,     0,     0,     0,     0,    71,    72,    73,
      74,   200,     0,    76,     0,     0,    78,     0,     0,   745,
      82,     0,    84,     0,   746,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   201,
       0,   102,     0,     0,     5,     6,   103,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   194,     0,     0,    22,   376,    24,
       0,     0,     0,     0,     0,    28,     0,   195,     0,     0,
       0,    32,     0,     0,     0,     0,   196,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   197,     0,   198,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,   360,     0,     0,     0,     0,     0,
      56,    57,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   199,     0,    66,     0,     0,     0,     0,     0,    71,
      72,    73,    74,   200,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,    84,     0,    85,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,     5,     6,   103,     8,
       9,    10,   379,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,    20,     0,  -208,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   195,
       0,     0,     0,    32,  -208,  -208,  -208,     0,   196,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   197,     0,
     198,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,    65,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,   200,     0,    76,     0,     0,
      78,     0,     0,     0,    82,    83,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    91,    92,    93,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,    96,    97,    98,     0,     0,
       0,   100,     0,   201,     0,   102,     0,  -208,     5,     6,
     103,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   194,     0,
    -208,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   195,     0,     0,     0,    32,  -208,  -208,  -208,     0,
     196,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     197,     0,   198,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   199,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,   200,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,     0,  -208,
       5,     6,   103,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     194,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   195,     0,     0,     0,    32,     0,     0,
       0,     0,   196,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   197,     0,   198,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   199,  -732,     0,
       0,     0,  -732,     0,     0,    71,    72,    73,    74,   200,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,  -375,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,  -375,     0,     0,   100,     0,   201,  -375,   102,
       0,     0,     5,     6,   103,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   194,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   195,     0,     0,     0,    32,
       0,     0,     0,     0,   196,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   197,     0,   198,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   199,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,   200,     0,    76,     0,     0,    78,   315,   316,     0,
      82,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,   317,     0,     0,   100,     0,   201,
       0,   102,     0,     0,     5,     6,   103,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   194,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   195,     0,     0,
       0,    32,     0,     0,     0,     0,   196,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   197,     0,   198,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   199,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,   200,     0,    76,     0,     0,    78,   315,
     316,     0,    82,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,   350,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,     5,     6,   103,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   194,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   195,
       0,     0,     0,    32,     0,     0,     0,     0,   196,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   197,     0,
     198,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   199,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,   200,     0,    76,     0,     0,
      78,   315,   316,     0,    82,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    91,    92,    93,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,    96,    97,    98,     0,     0,
       0,   100,     0,   201,     0,   102,  -535,     0,     5,     6,
     103,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   194,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   195,     0,     0,     0,    32,     0,     0,     0,     0,
     196,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     197,     0,   198,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   199,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,   200,     0,    76,
       0,     0,    78,   315,   316,     0,    82,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,     0,     0,
    -535,     0,   103,     5,     6,     0,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   194,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   195,     0,     0,     0,
      32,     0,     0,     0,     0,   196,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   197,     0,   198,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     199,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,   200,     0,    76,     0,     0,    78,   315,   316,
       0,    82,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,   806,     0,     5,     6,   103,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   194,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   195,     0,
       0,     0,    32,     0,     0,     0,     0,   196,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   197,     0,   198,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   199,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,   200,     0,    76,     0,     0,    78,
     315,   316,     0,    82,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,     0,     0,     0,
     100,     0,   201,     0,   102,   809,     0,     5,     6,   103,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   194,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     195,     0,     0,     0,    32,     0,     0,     0,     0,   196,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   197,
       0,   198,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   199,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,   200,     0,    76,     0,
       0,    78,   315,   316,     0,    82,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   201,     0,   102,   815,     0,     5,
       6,   103,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   194,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   195,     0,     0,     0,    32,     0,     0,     0,
       0,   196,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   197,     0,   198,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,   954,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   199,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,   200,     0,
      76,     0,     0,    78,   315,   316,     0,    82,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,     0,
       0,     5,     6,   103,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   194,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   195,     0,     0,     0,    32,     0,
       0,     0,     0,   196,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   197,     0,   198,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   199,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
     200,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,     0,  -383,     0,     0,    94,     0,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
      96,    97,    98,  -383,     0,     0,   100,     0,   201,  -383,
     102,     0,     0,     5,     6,   103,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   194,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   195,     0,     0,     0,
      32,     0,     0,     0,     0,   196,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   197,     0,   198,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     199,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,   200,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,  -368,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,  -368,     0,     0,   100,     0,
     201,  -368,   102,     0,     0,     5,     6,   103,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   194,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   195,     0,
       0,     0,    32,     0,     0,     0,     0,   196,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   197,     0,   198,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   199,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,   200,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,  -376,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,  -376,     0,     0,
     100,     0,   201,  -376,   102,     0,     0,     5,     6,   103,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   194,     0,     0,
      22,     0,   257,     0,     0,     0,     0,     0,    28,     0,
     195,     0,     0,     0,    32,     0,     0,     0,     0,   196,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   197,
       0,   198,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   199,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,   200,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,    84,     0,   258,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   201,     0,   102,     0,     0,     5,
       6,   103,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   194,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   195,     0,     0,     0,    32,     0,     0,     0,
       0,   196,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   197,     0,   198,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   199,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,   200,     0,
      76,     0,     0,    78,   315,   316,     0,    82,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,     0,
       0,     5,     6,   103,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   194,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   195,     0,     0,     0,    32,     0,
       0,     0,     0,   196,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   197,     0,   198,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   199,  -732,
       0,     0,     0,  -732,     0,     0,    71,    72,    73,    74,
     200,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,     0,     0,     0,     0,    94,     0,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
      96,    97,    98,     0,     0,     0,   100,     0,   201,     0,
     102,     0,     0,     5,     6,   103,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   194,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   195,     0,     0,     0,
      32,     0,     0,     0,     0,   196,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   197,     0,   198,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     199,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,   200,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,   601,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,   602,     0,   103,     5,     6,
       0,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   194,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   195,     0,     0,     0,    32,     0,     0,     0,     0,
     196,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     197,     0,   198,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   199,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,   200,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,  -404,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   201,     0,   102,  -404,     0,
       5,     6,   103,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     194,     0,     0,    22,     0,     0,     0,     0,     0,     0,
     273,    28,     0,   195,     0,     0,     0,    32,     0,     0,
       0,     0,   196,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   197,     0,   198,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   199,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,   200,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,     0,     0,     0,   100,     0,   101,     0,   102,
       0,     0,     5,     6,   103,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   194,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   195,     0,     0,     0,    32,
       0,     0,     0,     0,   196,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   197,     0,   198,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   199,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,   200,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,   601,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   201,
       0,   102,     0,     0,     5,     6,   103,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   194,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   195,     0,     0,
       0,    32,     0,     0,     0,     0,   196,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   197,     0,   198,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   199,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,   200,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   201,     0,   102,     0,     0,   602,     0,   103,     5,
       6,     0,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   194,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   195,     0,     0,     0,    32,     0,     0,     0,
       0,   196,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   197,     0,   198,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   199,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,   200,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,   804,   102,     0,
       0,     5,     6,   103,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   194,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   195,     0,     0,     0,    32,     0,
       0,     0,     0,   196,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   197,     0,   198,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   199,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
     200,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,     0,     0,     0,     0,    94,     0,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
      96,    97,    98,     0,     0,     0,   100,     0,   201,     0,
     102,     0,     0,  1024,     0,   103,     5,     6,     0,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   194,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   195,
       0,     0,     0,    32,     0,     0,     0,     0,   196,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   197,     0,
     198,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   199,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,   200,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    91,    92,    93,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,    96,    97,    98,     0,     0,
       0,   100,     0,   201,     0,   102,     0,     0,  1029,     0,
     103,     5,     6,     0,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   194,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   195,     0,     0,     0,    32,     0,
       0,     0,     0,   196,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   197,     0,   198,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   199,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
     200,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,     0,     0,     0,     0,    94,     0,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
      96,    97,    98,     0,     0,     0,   100,     0,   201,     0,
     102,     0,     0,     5,     6,   103,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   194,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,   293,     0,   195,     0,     0,     0,
      32,     0,     0,     0,     0,   196,    35,     0,     0,     0,
     295,     0,     0,    39,     0,   197,     0,   198,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     199,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,   300,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,     5,     6,   103,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   194,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   195,     0,
       0,     0,    32,     0,     0,     0,     0,  1064,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   197,     0,   198,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   199,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,   200,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,     0,     0,     0,
     100,     0,   201,     0,   102,     0,     0,     5,     6,   103,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   194,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     195,     0,     0,     0,    32,     0,     0,     0,     0,  1212,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   197,
       0,   198,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   199,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,   200,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   201,     0,   102,     0,     0,     5,
       6,   103,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   194,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   195,     0,     0,     0,    32,     0,     0,     0,
       0,  1213,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   197,     0,   198,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   199,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,   200,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,     0,
       0,     5,     6,   103,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   194,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   195,     0,     0,     0,    32,     0,
       0,     0,     0,  1215,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   197,     0,   198,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   199,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
     200,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,     0,     0,     0,     0,    94,     0,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
      96,    97,    98,     0,     0,     0,   100,     0,   201,     0,
     102,     0,     0,     5,     6,   103,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   194,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   195,     0,     0,     0,
      32,     0,     0,     0,     0,  1216,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   197,     0,   198,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     199,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,   200,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     201,     0,   102,     0,     0,     0,     6,   103,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
       0,    18,     0,     0,     0,   194,     0,  -208,     0,     0,
     292,     0,     0,     0,     0,     0,     0,     0,   195,     0,
       0,     0,     0,  -208,  -208,  -208,     0,   196,     0,     0,
     294,     0,     0,     0,   296,     0,     0,     0,     0,   198,
       0,     0,    44,    45,     0,     0,     0,     0,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,     0,   297,
       0,    56,   298,    58,    59,    60,     0,     0,     0,     0,
      63,     0,   199,     0,   299,     0,     0,     0,     0,     0,
      71,    72,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,   301,     0,     0,    84,     0,   302,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,     0,     0,     0,
     100,     0,   201,     0,   102,  -337,  -208,     0,     6,   103,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,     0,    18,     0,     0,     0,   194,     0,  -208,
       0,     0,   292,     0,     0,     0,     0,     0,     0,     0,
     195,     0,     0,     0,     0,  -208,  -208,  -208,     0,   196,
       0,     0,   294,     0,     0,     0,   296,     0,     0,     0,
       0,   198,     0,     0,    44,    45,     0,     0,     0,     0,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
       0,   297,     0,    56,   298,    58,    59,    60,     0,     0,
       0,     0,    63,     0,   199,     0,   299,     0,     0,     0,
       0,     0,    71,    72,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,   301,     0,     0,    84,     0,   302,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     630,     0,   631,     0,     0,    89,    90,    91,    92,    93,
       0,     0,   632,     0,    94,     0,     0,   633,   231,   634,
     635,     0,     0,    95,     0,   636,    96,    97,    98,     0,
       0,     0,   100,   234,   201,   195,   102,     0,  -208,   235,
       0,   103,     0,     0,     0,   637,     0,     0,     0,   237,
       0,     0,   638,     0,   639,   640,     0,     0,     0,     0,
       0,     0,     0,   641,     0,     0,     0,     0,     0,     0,
     241,   642,     0,     0,     0,     0,     0,     0,   243,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   643,     0,
       0,     0,     0,     0,     0,     0,     0,   245,     0,   644,
       0,   247,     0,   645,     0,     0,   249,     0,     0,     0,
     646,     0,   251,     0,     0,   647,     0,     0,     0,     0,
       0,     0,     0,     0,   507,   462,   463,   464,   465,   466,
       0,     0,   469,   470,   471,   472,     0,   474,   475,     0,
     648,   649,   650,   651,   652,   653,     0,   654,     0,     0,
       0,   655,   656,   657,   658,   659,   660,   661,   662,   663,
     664,     0,   665,     0,     0,   666,   667,   668,   669,     5,
       6,   670,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   194,
       0,  -208,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   195,     0,     0,     0,    32,  -208,  -208,  -208,
       0,   196,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   197,     0,   198,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   199,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,   200,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,     0,    97,
      98,     0,     0,     0,   100,     0,   201,     0,   102,     0,
    -208,     5,     6,     0,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   194,     0,  -208,    22,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   195,     0,     0,     0,    32,  -208,
    -208,  -208,     0,   196,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   197,     0,   198,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   199,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
     200,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
       0,    91,     0,     5,     6,     0,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,    98,   194,     0,  -208,    22,     0,   201,     0,
     102,     0,  -208,     0,    28,     0,   195,     0,     0,     0,
      32,  -208,  -208,  -208,     0,   196,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   197,     0,   198,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     199,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,   200,     0,    76,     0,     0,    78,   696,     0,
       0,    82,     0,    84,     0,     0,    86,     0,   632,     0,
       0,     0,     0,   986,   987,   634,   988,     0,     0,     0,
       0,   989,     0,     0,     0,     0,     0,     0,     0,   698,
       0,    89,     0,    91,     0,     0,     0,     0,     0,     0,
       0,   990,     0,     0,     0,   991,     0,     0,   992,     0,
     197,   640,     0,     0,    98,     0,     0,     0,     0,   699,
       0,     0,   102,     0,  -208,     0,     0,   993,     0,     0,
       0,     0,     0,     0,   994,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   995,     0,     0,     0,     0,     0,
       0,     0,     0,   996,     0,   997,     0,   998,     0,   700,
       0,     0,     0,     0,     0,   701,   999,     0,  1000,     0,
       0,  1001,     0,     0,     0,     0,     0,     0,     0,     0,
     507,   462,   463,   464,   465,   466,     0,     0,   469,   470,
     471,   472,     0,   474,   475,     0,   648,   649,   650,   651,
     652,   653,     0,   654,     0,     0,     0,   655,   656,   657,
     658,   659,   660,   661,   662,   663,   664,   229,   665,     0,
       0,   666,   667,   668,   669,  1002,     0,   632,     0,     0,
       0,     0,   230,   231,   634,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,   235,     0,     0,     0,     0,     0,
     236,     0,     0,     0,   237,     0,     0,   238,     0,   239,
     640,     0,     0,     0,     0,     0,     0,     0,   240,     0,
       0,     0,     0,     0,     0,   241,   242,     0,     0,     0,
       0,     0,     0,   243,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   244,     0,     0,     0,     0,     0,     0,
     431,     0,   245,     0,   246,     0,   247,   432,   248,     0,
       0,   249,     0,     0,     0,   250,     0,   251,     0,   433,
     252,     0,     0,     0,     0,     0,     0,     0,     0,   507,
     462,   463,   464,   465,   466,     0,     0,   469,   470,   471,
     472,     0,   474,   475,     0,   648,   649,   650,   651,   652,
     653,     0,   654,     0,     0,     0,   655,   656,   657,   658,
     659,   660,   661,   662,   663,   664,     0,   665,     0,     0,
     666,   667,   668,   669,     0,     0,     0,     0,   434,     0,
     431,     0,   435,     0,     0,     0,     0,   432,     0,     0,
    1186,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   507,   462,   463,   464,   465,   466,     0,     0,
     469,   470,   471,   472,     0,   474,   475,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,     0,   362,   434,     0,
     431,     0,   435,     0,   460,     0,     0,   432,     0,     0,
    1249,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   507,   462,   463,   464,   465,   466,     0,     0,
     469,   470,   471,   472,     0,   474,   475,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,     0,   362,   434,     0,
     431,     0,   435,     0,   460,     0,     0,   432,     0,     0,
    1250,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   507,   462,   463,   464,   465,   466,     0,     0,
     469,   470,   471,   472,     0,   474,   475,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,     0,   362,   434,     0,
     431,     0,   435,     0,   460,     0,     0,   432,     0,     0,
    1251,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   507,   462,   463,   464,   465,   466,     0,     0,
     469,   470,   471,   472,     0,   474,   475,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,     0,   362,   434,     0,
       0,     0,   435,     0,   460,     0,     0,     0,     0,   431,
    1252,     0,     0,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,   868,
       0,   436,   507,   462,   463,   464,   465,   466,     0,     0,
     469,   470,   471,   472,   869,   474,   475,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,     0,   362,     0,     0,
       0,     0,     0,   431,   460,     0,     0,   434,     0,     0,
     432,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,   273,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   213,     0,   981,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,   434,   457,   458,   459,   435,   362,     0,     0,     0,
       0,     0,     0,   460,   431,     0,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,     0,
     213,     0,     0,   433,   436,     0,     0,     0,   589,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,   590,   457,   458,   459,     0,
     362,     0,     0,     0,     0,     0,     0,   460,     0,     0,
       0,     0,   434,     0,     0,     0,   435,     0,     0,     0,
       0,     0,     0,     0,   431,   827,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,   436,     0,     0,   828,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   829,   591,   457,   458,   459,
       0,     0,     0,     0,     0,     0,     0,     0,   460,     0,
       0,     0,   434,   431,     0,     0,   435,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,  1129,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,     0,     0,   457,   458,   459,
       0,   434,     0,   431,     0,   435,     0,     0,   460,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,   273,     0,     0,     0,     0,     0,     0,
     213,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,     0,   457,   458,   459,     0,
     362,   434,     0,   431,     0,   435,     0,   460,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,   273,     0,     0,     0,     0,     0,     0,
     213,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,     0,   457,   458,   459,     0,
     362,   434,     0,   431,     0,   435,     0,   460,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,   913,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,   914,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,     0,   457,   458,   459,     0,
     362,   434,   431,     0,     0,   435,     0,   460,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,     0,   457,   458,   459,     0,
     434,     0,     0,     0,   435,     0,     0,   460,     0,     0,
       0,     0,   945,   431,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,   436,     0,     0,     0,  1104,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,  1105,   457,   458,   459,     0,   362,
       0,     0,     0,     0,     0,     0,   460,   431,     0,     0,
       0,   434,     0,     0,   432,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,   432,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   433,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,   434,   457,   458,   459,   435,
       0,     0,   431,   530,     0,     0,     0,   460,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,   213,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,     0,     0,     0,   435,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,     0,   213,
     457,   458,   459,   436,     0,     0,     0,     0,     0,  1121,
     434,   460,     0,     0,   435,     0,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   436,     0,   457,   458,   459,     0,     0,
       0,   431,     0,     0,  1169,     0,   460,     0,   432,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     433,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,     0,   457,   458,   459,     0,     0,
       0,   431,     0,     0,     0,     0,   460,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
       0,   431,     0,   435,     0,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,   434,
       0,     0,     0,   435,     0,     0,     0,     0,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   436,     0,   457,   458,   459,     0,   553,   434,
     431,     0,     0,   435,     0,   460,     0,   432,     0,   437,
     438,   732,   439,   440,   441,     0,   442,   443,   444,   433,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   436,   557,   457,   458,   459,     0,     0,     0,
     431,   823,     0,     0,     0,   460,     0,   432,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,   433,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,     0,   434,     0,
       0,   431,   435,     0,     0,   460,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,   434,     0,
       0,     0,   435,     0,     0,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   436,     0,   457,   458,   459,     0,     0,     0,   434,
     805,     0,     0,   435,   460,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   824,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   862,   436,   457,   458,   459,     0,     0,     0,     0,
       0,     0,     0,     0,   460,     0,     0,     0,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   431,     0,   457,   458,   459,     0,     0,   432,
       0,     0,     0,     0,     0,   460,     0,     0,     0,     0,
       0,   433,     0,     0,   943,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,     0,   431,     0,   435,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
     434,     0,     0,     0,   435,     0,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   436,     0,   457,   458,   459,     0,     0,
     434,   431,  1011,     0,   435,     0,   460,     0,   432,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     433,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   436,     0,   457,   458,   459,     0,     0,
       0,   431,   957,     0,     0,     0,   460,     0,   432,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     433,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,     0,   457,   458,   459,     0,   434,
       0,     0,   958,   435,     0,     0,   460,     0,     0,   431,
    1103,     0,     0,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,   434,
       0,     0,     0,   435,     0,     0,     0,     0,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   436,     0,   457,   458,   459,     0,     0,     0,
       0,     0,     0,     0,     0,   460,     0,   434,     0,   437,
     438,   435,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   431,     0,   457,   458,   459,     0,     0,   432,
     436,  1056,     0,     0,     0,   460,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     431,     0,   457,   458,   459,     0,     0,   432,     0,     0,
       0,     0,     0,   460,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,     0,     0,     0,   435,     0,     0,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,     0,
     437,   438,   435,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,  1115,   457,   458,   459,     0,     0,
    1124,   436,     0,     0,     0,     0,   460,     0,   434,     0,
       0,     0,   435,     0,     0,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   436,     0,   457,   458,   459,     0,     0,     0,     0,
       0,     0,     0,     0,   460,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   431,     0,   457,   458,   459,     0,     0,   432,     0,
       0,     0,  1152,     0,   460,     0,     0,     0,     0,     0,
     433,  1155,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,  1174,     0,     0,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
       0,     0,   431,   435,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,   434,
       0,     0,     0,   435,     0,     0,     0,     0,     0,   437,
     438,  1206,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   436,     0,   457,   458,   459,     0,     0,     0,
     434,     0,     0,     0,   435,   460,     0,     0,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,   436,   457,   458,   459,   431,     0,     0,
       0,     0,     0,     0,   432,   460,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,   433,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,     0,   457,   458,   459,     0,     0,
       0,     0,     0,     0,     0,   431,   460,     0,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,     0,   435,
       0,     0,     0,     0,     0,   431,     0,  1214,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   434,     0,   437,   438,   435,   439,   440,
     441,     0,   442,   443,   444,  1253,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,     0,     0,
     457,   458,   459,     0,     0,     0,   436,     0,     0,     0,
       0,   460,     0,   434,   431,     0,     0,   435,     0,     0,
       0,   432,     0,   437,   438,  1254,   439,   440,   441,     0,
     442,   443,   444,   433,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   436,     0,   457,   458,
     459,     0,     0,     0,   431,     0,     0,     0,     0,   460,
       0,   432,     0,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   433,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,     0,     0,   457,   458,
     459,     0,   434,     0,   431,     0,   435,     0,     0,   460,
       0,   432,     0,     0,  1255,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,   434,     0,     0,     0,   435,     0,     0,     0,
       0,     0,   437,   438,  1256,   439,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   436,     0,   457,   458,   459,
       0,     0,   434,   431,     0,     0,   435,     0,   460,     0,
     432,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   433,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   436,     0,   457,   458,   459,
       0,     0,     0,     0,     0,     0,     0,     0,   460,     0,
       0,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   433,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,     0,     0,   457,   458,   459,
       0,   933,     0,     0,     0,   435,     0,     0,   460,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,   434,     0,     0,     0,   435,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   436,     0,   457,   458,   459,     0,
       0,   434,     0,     0,     0,   435,     0,   460,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,     0,   450,   451,   452,
     453,   454,   455,   456,   436,     0,   457,   458,   459,     0,
       0,     0,     0,     0,     0,     0,     0,   460,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,     0,   450,   451,   452,
     453,   454,     0,   456,     0,     0,   457,   458,   459,   612,
       0,   229,     0,     7,     0,     0,     0,   460,     0,     0,
       0,     0,     0,     0,     0,     0,   230,   231,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,   235,     0,
       0,     0,     0,     0,   236,     0,     0,     0,   237,     0,
       0,   238,     0,   239,     0,     0,     0,     0,     0,     0,
     229,     0,   240,     0,     0,     0,     0,     0,     0,   241,
     242,     0,     0,     0,     0,   230,   231,   243,   232,     0,
       0,     0,     0,   233,     0,    24,     0,   244,     0,     0,
       0,   234,     0,     0,     0,     0,   245,   235,   246,     0,
     247,     0,   248,   236,     0,   249,     0,   237,     0,   250,
     238,   251,   239,     0,   252,     0,     0,     0,     0,     0,
       0,   240,     0,     0,     0,     0,     0,     0,   241,   242,
       0,     0,     0,     0,     0,     0,   243,    57,     0,     0,
       0,     0,     0,     0,     0,     0,   244,     0,     0,    66,
       0,   229,     0,     7,     0,   245,     0,   246,     0,   247,
       0,   248,     0,     0,   249,     0,   230,   231,   250,   232,
     251,     0,    85,   252,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,   235,     0,
       0,     0,     0,     0,   236,     0,     0,     0,   237,     0,
       0,   238,     0,   239,     0,     0,     0,     0,     0,     0,
    -308,     0,   240,     0,     0,     0,     0,     0,     0,   241,
     242,     0,     0,     0,     0,  -308,  -308,   243,  -308,     0,
       0,     0,     0,  -308,     0,     0,     0,   244,     0,     0,
       0,  -308,     0,     0,     0,     0,   245,  -308,   246,     0,
     247,     0,   248,  -308,     0,   249,     0,  -308,     0,   250,
    -308,   251,  -308,     0,   252,     0,     0,     0,     0,   229,
       0,  -308,     0,     0,     0,     0,     0,     0,  -308,  -308,
       0,     0,     0,     0,   230,   231,  -308,   232,     0,     0,
       0,     0,   233,     0,     0,     0,  -308,     0,     0,     0,
     234,     0,     0,     0,     0,  -308,   235,  -308,     0,  -308,
       0,  -308,   236,     0,  -308,     0,   237,     0,  -308,   238,
    -308,   239,     0,  -308,     0,     0,     0,     0,  -309,     0,
     240,     0,     0,     0,     0,     0,     0,   241,   242,     0,
       0,     0,     0,  -309,  -309,   243,  -309,     0,     0,     0,
       0,  -309,     0,     0,     0,   244,     0,     0,     0,  -309,
       0,     0,     0,     0,   245,  -309,   246,     0,   247,     0,
     248,  -309,     0,   249,     0,  -309,     0,   250,  -309,   251,
    -309,     0,   252,     0,     0,     0,     0,     0,     0,  -309,
       0,     0,     0,     0,     0,     0,  -309,  -309,     0,     0,
       0,     0,     0,     0,  -309,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -309,     0,     0,     0,     0,     0,
       0,     0,     0,  -309,     0,  -309,     0,  -309,     0,  -309,
       0,     0,  -309,     0,     0,     0,  -309,     0,  -309,     0,
       0,  -309
};

static const yytype_int16 yycheck[] =
{
       2,   180,     2,   164,    27,   264,   551,   398,    68,    63,
     613,   111,   489,   589,    63,    63,   422,   513,   578,     2,
     673,   222,   675,   625,    26,   132,   621,    87,   829,   824,
     289,   546,   794,   788,   380,     1,   400,   875,     1,     1,
      68,   960,   102,    26,    67,   847,    25,     5,     1,    33,
     390,    33,     2,    33,   202,   120,     1,    33,    36,    87,
     103,    48,    41,    42,    43,   522,   883,   147,    62,   328,
       1,    91,   118,    75,   102,    95,    26,    79,    80,   123,
     120,   260,   141,    19,    28,    28,    48,    33,    33,    57,
     149,    33,    75,   173,   434,   435,    79,    80,    25,   101,
       3,   147,    33,   105,    48,    48,    33,    29,    52,    52,
     175,    48,    79,    80,    41,    42,    43,    62,   101,    41,
      42,    43,   105,    76,   118,    75,   170,   170,    33,    79,
      80,    62,   110,    77,    77,   175,    80,    80,    82,    82,
     480,   481,    57,    79,   120,   962,   486,     0,    92,    92,
     165,   101,    55,   603,   604,   105,    48,   607,   608,   125,
     147,   106,    98,   125,   117,   147,   110,   110,   625,    33,
    1089,   115,   115,   118,   120,   106,  1014,   113,   542,   774,
     146,   147,    48,   120,   146,   147,   170,   118,   170,   147,
     170,   953,   119,    23,   170,   174,   120,   757,   960,    57,
     228,   146,     2,   165,   172,  1043,  1044,   173,   963,   172,
     147,  1013,   214,   171,  1009,   146,   364,    76,   171,   478,
     479,     2,   120,   165,   170,   170,    26,   149,   170,   173,
     173,   214,    25,    57,   165,   575,   264,   733,   175,   170,
      33,   163,   147,   422,    48,    26,   592,   174,    41,    42,
      43,   175,   828,    48,   120,   147,   120,   172,   117,   147,
      90,   289,   517,     3,   214,   170,   268,     3,   147,     3,
      48,   273,   379,  1111,    47,    75,   198,   792,   147,    79,
      80,   147,   112,   175,    48,   268,   147,   175,    92,   544,
     273,    47,   171,   110,    75,  1133,   555,    92,    79,    80,
     328,   101,   171,    28,  1105,   105,   170,  1069,    57,   175,
     171,    67,   171,    86,    92,    55,   290,   291,   268,    55,
     101,    55,     3,   273,   105,    57,   119,  1089,    92,   530,
      86,   147,   493,   534,    25,   257,   258,   932,   147,    92,
      21,   114,    33,    32,   172,   399,   348,   269,   348,   147,
      41,    42,    43,    34,    94,    80,   165,   173,   114,   961,
     362,   811,   812,   813,   104,   348,   389,    92,   104,   371,
     104,   147,   395,   147,    55,   173,    47,   873,    54,   362,
      12,   174,   384,    64,   147,   147,   165,   309,   371,    57,
     115,   165,   147,   172,   595,   174,    67,   173,   348,    75,
      84,   384,    91,    87,  1057,  1058,    95,   467,   468,    85,
     173,   173,   362,   473,   214,    86,   476,   477,   173,   915,
     422,   371,   982,   104,   165,   172,  1002,   174,   119,   110,
     172,   172,   147,   214,   384,   125,   358,   147,    32,   467,
     468,   147,   919,   114,  1047,   473,    79,    80,   476,   477,
     478,   479,   141,   142,   147,   144,   145,   146,   173,   148,
     149,   151,   485,   173,   153,   147,    57,   173,   268,   165,
     159,   160,   394,   273,   163,   171,   398,   166,   167,   168,
     173,   147,    57,   174,   875,   513,   147,   268,   894,   543,
      51,   173,   273,    54,   543,   543,   147,    91,    32,    57,
     422,    95,   504,   125,   506,   165,    91,   173,   568,   569,
      95,   171,   173,   613,    75,   568,   569,    78,   858,   147,
     147,   504,   173,   506,    85,   104,   147,   555,  1104,   869,
     152,    57,   154,   155,  1137,   157,   158,   165,   165,    91,
     568,   569,   147,    95,   165,   147,   147,   141,   348,   110,
     578,   574,   146,    57,   504,   149,   506,    91,    32,   153,
     165,    95,   362,   165,   165,   487,   172,   348,   174,   147,
     910,   371,  1022,  1023,   914,   172,     9,   174,   147,   147,
     582,   362,   582,   147,   384,  1035,  1036,   165,   147,   172,
     371,   174,   514,   933,   934,   517,   165,   165,   172,   582,
    1141,   165,  1147,   384,    57,    91,   165,   141,   530,    95,
     101,    57,   146,    57,   148,   149,    91,    91,   540,   153,
      95,    95,   544,  1014,   172,   786,   160,    91,   141,    57,
      57,    95,   582,   146,   168,    57,   149,    32,     5,   979,
     153,   981,   170,   172,   172,   174,   149,  1097,  1098,  1099,
    1100,  1196,  1043,     2,   172,   170,   174,  1048,    57,  1200,
    1201,  1202,  1203,  1204,  1205,   146,   588,   141,   142,   170,
     144,   145,   146,   172,   148,   149,   598,    26,   172,   153,
     174,    32,   165,   165,   164,    57,   160,   165,   849,   165,
      86,   613,   166,   167,   168,   755,    91,   165,   165,   165,
      95,   170,   165,   165,   504,   733,   506,   165,   173,    46,
     147,   172,   172,   172,   146,   894,   165,   777,   172,     9,
    1111,    32,   165,   504,   147,   506,    75,   755,   173,   757,
      79,    80,     3,    70,   172,    67,    80,   125,   149,   170,
      91,   165,    70,   172,    95,   125,   141,   142,   750,   777,
     145,   146,   101,   148,   149,   173,   105,   679,   153,   761,
     147,   125,   764,   125,    67,   160,    35,    35,   173,   125,
     173,   166,   167,   168,    22,   146,   173,   170,   761,   119,
      91,   764,   582,   172,    95,   171,   147,   125,   106,   172,
     141,   142,   149,   144,   145,   146,   149,   148,   149,   150,
     141,   582,   153,   154,   155,   165,   157,   158,   159,   160,
     149,   761,   163,   149,   764,   166,   167,   168,   149,   149,
     149,   875,   149,  1082,  1083,   149,   177,   881,   149,   149,
     141,   854,   149,   149,   149,   146,   890,   148,   149,   149,
      28,   175,   153,   165,   766,   873,   165,   173,    28,   160,
      33,   773,   165,   165,   856,   166,   167,   168,   860,   165,
      48,   783,    35,   865,    52,   214,   868,   172,    48,   173,
      35,   146,    52,   856,   171,   173,   165,   860,   170,    48,
     802,   147,   865,   171,   175,   868,    35,   915,   175,    77,
     173,   175,    80,   165,    82,   165,   172,    77,   172,   146,
      80,   165,    82,   147,    92,    33,   856,   147,   173,    35,
     860,   165,    92,   175,   173,   865,   173,   175,   868,   268,
     175,   173,   110,   165,   273,   164,   848,   115,   164,     2,
     110,   933,   934,   110,   577,   115,   371,   982,   940,   276,
     942,   278,    25,   945,  1044,   821,  1125,   364,   971,   263,
     949,   975,   388,    26,   982,   337,   528,   940,   890,   942,
    1014,   761,   945,   543,   764,   894,   382,   881,   970,  1196,
     303,  1050,   894,  1206,  1199,   848,   679,   130,   980,   525,
     761,   773,    21,   764,   919,    -1,    32,   970,   325,    -1,
     940,    -1,   942,    -1,    -1,   945,    -1,   980,    -1,   348,
      -1,    -1,    75,    -1,    -1,    -1,    79,    80,    -1,    -1,
      -1,    -1,    -1,   362,    -1,    -1,    -1,    -1,    -1,  1021,
     970,    -1,   371,    -1,    -1,  1027,    -1,   949,   101,    -1,
     980,    -1,   105,  1133,    -1,   384,    -1,  1137,  1021,    -1,
      -1,    -1,    -1,    -1,  1027,    91,  1225,    -1,  1227,    95,
      -1,  1230,  1231,   975,  1082,  1083,   856,  1111,    -1,    -1,
     860,    -1,    -1,    -1,    -1,   865,    -1,    -1,   868,    -1,
      -1,  1021,    -1,    -1,  1076,   856,    -1,  1027,    -1,   860,
      -1,    -1,    -1,    -1,   865,    -1,    -1,   868,    -1,    -1,
      -1,    -1,    -1,  1076,    -1,   141,   142,    -1,    -1,    -1,
     146,    -1,   148,   149,    -1,    -1,    -1,   153,    -1,    -1,
      -1,    -1,   228,    -1,   160,    -1,    -1,  1119,    -1,  1121,
     166,   167,   168,    -1,  1126,  1047,  1076,  1129,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1119,    -1,  1121,    -1,
     940,   214,   942,  1126,    -1,   945,  1129,    -1,   264,    -1,
      -1,    -1,    -1,    -1,    -1,   504,  1158,   506,    -1,   940,
      -1,   942,  1164,    -1,   945,    -1,  1168,  1169,    -1,  1119,
     970,  1121,    -1,   289,    28,  1158,  1126,    -1,  1180,  1129,
     980,  1164,  1184,    -1,  1186,  1168,  1169,    -1,    -1,   970,
      -1,    -1,    -1,    -1,    48,   268,    -1,  1180,    52,   980,
     273,  1184,    -1,  1186,    -1,  1207,    -1,  1207,  1158,    -1,
      -1,    -1,   328,     2,  1164,  1137,    -1,    -1,  1168,  1169,
      -1,  1021,    -1,    77,  1207,    -1,    80,  1027,    -1,    -1,
    1180,    -1,    -1,   582,  1184,    -1,  1186,    26,    92,    -1,
    1021,    -1,    -1,    -1,    -1,    -1,  1027,  1249,  1250,  1251,
    1252,    -1,    -1,    -1,    28,    -1,   110,  1207,    -1,    -1,
      -1,   115,   599,   600,    -1,    -1,  1249,  1250,  1251,  1252,
     607,   608,    -1,    -1,    48,   348,  1076,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,   362,
      79,    80,    -1,    -1,    -1,  1076,    -1,    -1,   371,  1249,
    1250,  1251,  1252,    77,    -1,    -1,    80,    -1,    -1,    -1,
      -1,   384,   101,    -1,    -1,    -1,   105,    -1,    92,  1119,
      -1,  1121,    -1,    -1,    -1,    -1,  1126,   443,    -1,  1129,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,  1119,    -1,
    1121,   115,    -1,    -1,    -1,  1126,    -1,    -1,  1129,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1158,    -1,
      -1,    -1,   478,   479,  1164,    -1,    -1,    -1,  1168,  1169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1158,    -1,    -1,
    1180,    -1,     2,  1164,  1184,    -1,  1186,  1168,  1169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   513,    -1,  1180,
      -1,    -1,    -1,  1184,    -1,  1186,    26,  1207,    -1,    -1,
      -1,    -1,   761,    -1,    -1,   764,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,  1207,    -1,    -1,    -1,
      -1,   504,    -1,   506,    -1,    -1,    -1,    -1,    -1,   555,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1249,
    1250,  1251,  1252,    -1,    -1,    75,    -1,    -1,    -1,    79,
      80,    -1,   578,    -1,    -1,    -1,    -1,    -1,  1249,  1250,
    1251,  1252,    -1,    -1,    -1,   812,   813,    -1,    -1,   268,
      -1,   101,    -1,    -1,   273,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,     3,    -1,    -1,    -1,    -1,   856,    -1,   582,
      -1,   860,    -1,    -1,    -1,    -1,   865,    18,    19,   868,
      21,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
     867,    -1,    -1,    34,   871,    -1,     2,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    32,    50,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,   348,
      26,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    -1,   362,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,   371,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,   940,    -1,   942,   214,   384,   945,    98,    -1,   100,
      -1,   102,    -1,   104,    -1,    -1,   107,    91,    -1,    75,
     111,    95,   113,    -1,    -1,   116,    -1,   733,    -1,    -1,
      -1,   970,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   980,    -1,    -1,    -1,   101,    -1,    -1,    -1,   105,
      -1,   757,    -1,    -1,    -1,    -1,    -1,    -1,   268,    -1,
      -1,    -1,    -1,   273,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,    -1,   153,
      -1,   172,  1021,    -1,    -1,   159,   160,    -1,  1027,   163,
      -1,    -1,   166,   167,   168,    -1,    -1,    -1,   761,    -1,
      -1,   764,    -1,   177,    -1,    -1,    -1,    -1,  1035,  1036,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   823,   824,    -1,
      -1,    -1,    -1,    -1,    -1,   504,    -1,   506,    -1,    23,
      -1,    -1,    -1,    27,    28,    -1,    -1,  1076,   348,    -1,
      -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,
      44,    -1,   362,    -1,    -1,    -1,    -1,    51,   214,    -1,
      -1,   371,    -1,    -1,    -1,    -1,  1093,   873,    -1,    -1,
      -1,    -1,  1099,  1100,   384,    -1,    -1,    -1,    -1,    73,
    1119,    -1,  1121,    -1,    78,    -1,    80,  1126,    -1,  1116,
    1129,    -1,    -1,   856,    -1,    -1,    90,   860,    92,    -1,
      -1,  1128,   865,   582,  1131,   868,    -1,    -1,    -1,   915,
      -1,    -1,   268,    -1,    -1,    -1,   110,   273,   112,  1158,
      -1,   115,    -1,    -1,    -1,  1164,    -1,    -1,    -1,  1168,
    1169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1180,    -1,    -1,    -1,  1184,    -1,  1186,     1,    -1,
      -1,    -1,     5,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,  1207,    -1,
      23,    -1,    -1,    -1,    27,    28,   982,   940,    -1,   942,
      -1,    -1,   945,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    44,   348,    -1,   504,    -1,   506,   169,    51,    -1,
     172,    54,   174,    -1,    -1,    -1,   362,   970,    -1,    -1,
    1249,  1250,  1251,  1252,    -1,   371,    -1,   980,    -1,    -1,
      73,    -1,    75,    -1,    -1,    78,    -1,    80,   384,    82,
      -1,    84,    85,    -1,    87,    -1,    -1,    90,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,  1021,   112,
      -1,    -1,   115,    -1,  1027,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   582,    -1,    -1,    -1,  1082,  1083,    -1,    -1,
      -1,    -1,   761,     3,    -1,   764,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,
      -1,    21,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,   165,  1076,    34,    -1,    -1,    -1,   171,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,   504,    -1,
     506,    71,    72,    -1,    -1,    -1,  1119,    -1,  1121,    79,
      -1,    -1,    -1,  1126,    -1,    -1,  1129,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   856,    98,    -1,
     100,   860,   102,    -1,   104,    -1,   865,   107,    -1,   868,
      -1,   111,    -1,   113,    -1,  1158,   116,    -1,    -1,    -1,
      -1,  1164,    -1,    -1,    -1,  1168,  1169,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    -1,  1180,    -1,    -1,
      -1,  1184,    -1,  1186,    -1,    18,   582,    -1,    21,    -1,
      -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,  1207,    26,    -1,    -1,    -1,    -1,
     170,   761,   172,    46,   764,    -1,    -1,    -1,    -1,    -1,
      53,   940,    55,   942,    -1,    -1,   945,     3,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    18,    19,    -1,    21,  1249,  1250,  1251,  1252,
      26,   970,    -1,    -1,    75,    -1,    89,    -1,    34,    -1,
      -1,   980,    -1,    -1,    40,    -1,    -1,   100,    -1,    -1,
      46,   104,    -1,    -1,    50,    -1,    -1,    53,   111,    55,
      -1,    -1,    -1,   116,   105,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,
      -1,    -1,  1021,    79,    -1,    -1,   856,    -1,  1027,    -1,
     860,    -1,    -1,    89,    -1,   865,    -1,    -1,   868,    -1,
      -1,    -1,    98,    -1,   100,    -1,   102,    -1,   104,    -1,
       3,   107,    -1,    -1,    -1,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    18,    19,    -1,    21,    -1,
      -1,    -1,    -1,    26,    -1,    -1,    -1,  1076,    -1,    -1,
      -1,    34,    -1,    -1,    -1,   761,    -1,    40,   764,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     940,    64,   942,   214,    -1,   945,   172,    -1,    71,    72,
    1119,    -1,  1121,    -1,    -1,    -1,    79,  1126,    -1,    -1,
    1129,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
     970,    -1,    -1,    -1,    -1,    98,    -1,   100,    -1,   102,
     980,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,  1158,
     113,    -1,    -1,   116,    -1,  1164,    -1,   268,    -1,  1168,
    1169,    -1,   273,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     856,  1180,    -1,    -1,   860,  1184,    -1,  1186,    -1,   865,
      -1,  1021,   868,    -1,    -1,    -1,    -1,  1027,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1207,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    -1,    21,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,  1076,    -1,    40,    -1,
    1249,  1250,  1251,  1252,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,   940,    -1,   942,    -1,    -1,   945,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,   384,    -1,    -1,    -1,    79,    -1,  1119,
      -1,  1121,    -1,    -1,   970,    -1,  1126,    89,    -1,  1129,
      -1,    -1,    -1,    -1,   980,    -1,    98,    -1,   100,    -1,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,  1158,    -1,
      -1,    -1,    -1,    -1,  1164,    -1,    -1,    -1,  1168,  1169,
      -1,    -1,    -1,    -1,    -1,  1021,    -1,    -1,    -1,    -1,
    1180,  1027,    -1,    14,  1184,    -1,  1186,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,  1207,    -1,    -1,
     172,    -1,    -1,    -1,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    54,    -1,    56,    -1,    58,    -1,    -1,
    1076,    -1,    63,   504,    -1,   506,    67,    68,    69,    70,
      71,    -1,    -1,    -1,    75,    -1,    77,    -1,    -1,  1249,
    1250,  1251,  1252,    84,    -1,    -1,    87,    -1,    89,    -1,
      91,    -1,    -1,    94,    95,    -1,    97,    -1,    -1,    -1,
     101,   102,   103,  1119,    -1,  1121,    -1,    -1,    -1,    -1,
    1126,    -1,    -1,  1129,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    27,    28,   130,
      -1,   132,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,
      -1,    -1,  1158,     5,    44,    -1,    -1,    -1,  1164,    -1,
      -1,    51,  1168,  1169,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,  1180,    27,    28,    67,  1184,    -1,
    1186,    -1,   173,    73,    36,    75,    38,    39,    78,    -1,
      80,    -1,    44,    -1,    84,    85,    86,    87,    -1,    51,
      90,  1207,    92,    -1,    -1,   196,    -1,    -1,    -1,   200,
     201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    73,   112,    -1,    -1,   115,    78,    -1,    80,    -1,
      82,    -1,    84,    -1,    -1,    87,    -1,   228,    90,    -1,
      92,    -1,    -1,  1249,  1250,  1251,  1252,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
     112,     5,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   264,    -1,   165,    -1,    -1,    -1,    23,
     170,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    38,    39,    -1,    -1,   289,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,   300,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,   315,   316,    -1,    -1,    -1,    73,
     761,    -1,    -1,   764,    78,    -1,    80,   328,    82,    -1,
      84,   332,    -1,    87,    -1,    -1,    90,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   350,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,   360,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   372,    -1,   374,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   388,   389,   390,
      -1,    -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,   856,    -1,   171,    -1,   860,
      -1,    -1,    -1,    -1,   865,    -1,    -1,   868,    -1,    -1,
     431,   432,   433,   434,   435,   436,    46,   438,   439,   440,
     441,   442,    -1,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
      70,    -1,    -1,    -1,    -1,    -1,   467,   468,    -1,    79,
      80,    -1,   473,    -1,    -1,   476,   477,   478,   479,   480,
     481,    -1,    -1,    -1,   485,   486,    -1,    -1,   489,    -1,
      -1,    -1,    -1,    -1,   495,    -1,    -1,    -1,    -1,   940,
     110,   942,    -1,    -1,   945,   506,    -1,    -1,   509,    -1,
      -1,    -1,   513,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   522,    -1,   524,    -1,    -1,    -1,   528,    -1,   970,
     531,   532,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   980,
      -1,    -1,   543,    -1,    -1,   546,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   555,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   565,    -1,    -1,   568,   569,    -1,
      -1,    -1,    -1,   574,   575,    -1,    -1,   578,    -1,    -1,
    1021,    -1,    -1,    -1,    -1,    -1,  1027,    -1,   589,   590,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   602,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     220,    -1,    -1,    -1,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   625,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,  1076,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,   259,
     260,    -1,    -1,    44,    -1,    -1,    -1,   267,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,   276,    -1,   278,   670,
      -1,    -1,    -1,    -1,    -1,   285,   677,    -1,  1119,    -1,
    1121,    -1,    73,    -1,    -1,  1126,    -1,    78,  1129,    80,
      -1,    82,    -1,    84,    -1,    -1,    87,    -1,    -1,    90,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   325,    -1,  1158,    -1,   110,
      -1,   112,    -1,  1164,   115,    -1,    -1,  1168,  1169,    -1,
      -1,   732,   733,    -1,    -1,    -1,    -1,    -1,    -1,  1180,
      -1,    -1,    -1,  1184,    -1,  1186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   755,    -1,   757,   758,   759,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,   777,    -1,    -1,    -1,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   792,    -1,    -1,    -1,   796,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1249,  1250,
    1251,  1252,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     821,    -1,    -1,    -1,   825,    -1,   827,   828,   829,    -1,
     831,    -1,   833,    -1,    -1,   836,   837,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   850,
      -1,    -1,     5,   854,    -1,    -1,    -1,   858,   859,   860,
      -1,   862,   863,    -1,   865,    -1,    -1,   868,   869,    -1,
      23,    -1,   873,    -1,    27,    28,    -1,   878,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   910,
     911,   912,   913,   914,   915,    -1,    -1,    -1,   919,    -1,
      73,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    84,   933,   934,    87,    -1,    -1,    90,    -1,    92,
      -1,    -1,   943,    -1,    -1,    -1,    -1,    -1,   558,    -1,
      -1,    -1,    -1,    -1,     5,    -1,    -1,   110,    -1,   112,
     961,    -1,   115,    -1,   965,    -1,    -1,    -1,    -1,    -1,
     971,    -1,    23,    -1,    -1,    -1,    27,    28,   979,    -1,
     981,   982,    -1,    -1,    -1,    36,    -1,    38,    39,   599,
     600,    -1,    -1,    44,    -1,    -1,    -1,   607,   608,    -1,
      51,  1002,    -1,    -1,    -1,    -1,    -1,    -1,  1009,    -1,
    1011,    -1,   165,    -1,    -1,    -1,    -1,  1018,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    -1,  1033,    84,    -1,    -1,    87,    -1,    -1,    90,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1052,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,   112,    -1,  1064,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1082,  1083,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1095,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1103,  1104,  1105,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,   165,    -1,    -1,    -1,  1119,    -1,
    1121,    -1,    -1,  1124,    -1,  1126,    -1,    -1,  1129,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1151,  1152,    -1,  1154,  1155,   765,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,  1174,    95,    -1,  1177,    -1,    -1,    -1,
    1181,    -1,    -1,    -1,    -1,  1186,  1187,    -1,    -1,  1190,
      -1,    -1,   802,    -1,    -1,    -1,    -1,  1198,    -1,    -1,
      -1,    -1,   812,   813,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1212,  1213,  1214,  1215,  1216,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,    -1,   163,    -1,    -1,   166,   167,   168,  1249,  1250,
    1251,  1252,  1253,  1254,  1255,  1256,   177,   867,    -1,    -1,
      -1,   871,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     0,     1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
     950,    49,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    72,    73,    -1,    75,   978,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,    -1,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    13,    -1,    -1,  1035,  1036,    -1,    -1,    20,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    32,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1093,    -1,    -1,    -1,    -1,    -1,  1099,
    1100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,  1116,    -1,    -1,    -1,
      -1,    -1,   103,    -1,    -1,  1125,    -1,    -1,  1128,    -1,
      -1,  1131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,    -1,
     141,   142,    -1,   144,   145,   146,  1166,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1225,    -1,  1227,    -1,    -1,
    1230,  1231,     1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    13,    -1,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,
      79,    80,    81,    -1,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
      -1,   170,   171,   172,    -1,   174,    -1,     1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    17,    18,    19,    -1,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    69,    -1,    71,    72,    73,
      -1,    75,    -1,    -1,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,    -1,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    16,    17,    18,
      19,    -1,    21,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,   174,    -1,     1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    17,    18,    19,    -1,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    69,    -1,    71,    72,    73,
      -1,    75,    -1,    -1,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,    -1,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    16,    17,    18,
      19,    -1,    21,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,   174,    -1,     1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    17,    18,    19,    -1,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    69,    -1,    71,    72,    73,
      -1,    75,    -1,    -1,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,    -1,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    16,    17,    18,
      19,    -1,    21,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    17,    18,    19,    -1,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    69,    -1,    71,    72,    73,
      -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,    -1,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    16,    17,    18,
      19,    -1,    21,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,
      79,    80,    81,    -1,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    17,    18,    19,    -1,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    69,    -1,    71,    72,    73,
      -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,    -1,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    16,    17,    18,
      19,    -1,    21,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,
      79,    80,    81,    -1,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    17,    18,    19,    -1,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    69,    -1,    71,    72,    73,
      -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,    -1,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    16,    17,    18,
      19,    -1,    21,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,
      79,    80,    81,    -1,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,     1,   174,     3,     4,   177,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    -1,    21,    -1,    23,    -1,    25,    26,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    43,    -1,    45,    46,
      -1,    48,    -1,    50,    -1,    52,    53,    -1,    55,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      77,    -1,    79,    80,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,   110,   111,    -1,   113,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,   173,   174,    -1,     1,
     177,     3,     4,    -1,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    43,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
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
     172,    -1,   174,     3,     4,   177,     6,     7,     8,     9,
      10,    11,    -1,    13,    -1,    15,    -1,    -1,    18,    19,
      20,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    56,    57,    -1,    -1,
      60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,    -1,   138,   139,
      -1,   141,   142,   143,   144,   145,   146,    -1,   148,    -1,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,    -1,   172,    -1,    -1,     3,     4,   177,     6,     7,
       8,     9,    10,    11,    -1,    13,    -1,    15,    -1,    -1,
      18,    19,    20,    21,    -1,    23,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    56,    57,
      -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,
      68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,   143,   144,   145,   146,    -1,
     148,    -1,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,   166,   167,
     168,   169,   170,    -1,   172,    -1,    -1,    -1,    -1,   177,
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
      -1,   104,   105,    -1,   107,    -1,    -1,    -1,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,    -1,     3,     4,   177,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    48,    -1,    50,
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
      -1,   172,    -1,    -1,     3,     4,   177,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    27,    28,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,   113,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,    -1,     3,     4,   177,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    -1,    21,    -1,    23,    -1,    25,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    43,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,   112,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,     3,     4,
     177,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    43,    -1,
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
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
       3,     4,   177,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    91,    -1,
      -1,    -1,    95,    -1,    -1,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,   147,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,   171,   172,
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
     101,   102,    -1,   104,    -1,    -1,   107,   108,   109,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
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
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,   108,
     109,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,
     175,    -1,   177,     3,     4,    -1,     6,     7,     8,     9,
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
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,   173,    -1,     3,     4,   177,     6,     7,
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
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,   173,    -1,     3,     4,   177,
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
      64,    -1,    66,    -1,    68,    69,    70,    71,    72,    -1,
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
      -1,     3,     4,   177,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
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
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,   147,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,   171,   172,    -1,    -1,     3,     4,   177,     6,     7,
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
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,   147,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,   171,   172,    -1,    -1,     3,     4,   177,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,
      26,    -1,    28,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,
      66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,   113,    -1,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,     3,
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
      -1,     3,     4,   177,     6,     7,     8,     9,    10,    11,
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
     170,    -1,   172,    -1,    -1,   175,    -1,   177,     3,     4,
      -1,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
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
     145,    -1,   147,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,   173,    -1,
       3,     4,   177,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
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
      -1,   122,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
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
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,   171,   172,    -1,
      -1,     3,     4,   177,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
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
     172,    -1,    -1,   175,    -1,   177,     3,     4,    -1,     6,
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
      -1,   168,    -1,   170,    -1,   172,    -1,    -1,   175,    -1,
     177,     3,     4,    -1,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
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
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
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
      -1,   107,    -1,    -1,    -1,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,     3,
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
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
      -1,     3,     4,   177,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
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
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,    -1,     4,   177,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    23,    -1,    25,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    41,    42,    43,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    66,    -1,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,
      88,    -1,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,   173,   174,    -1,     4,   177,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    23,    -1,    25,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    -1,    41,    42,    43,    -1,    45,
      -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,
      66,    -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    80,    81,    82,    83,    -1,    -1,
      -1,    -1,    88,    -1,    90,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    98,    99,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,     3,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    13,    -1,   150,    -1,    -1,    18,    19,    20,
      21,    -1,    -1,   159,    -1,    26,   162,   163,   164,    -1,
      -1,    -1,   168,    34,   170,    36,   172,    -1,   174,    40,
      -1,   177,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
      -1,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,    -1,
     141,   142,   143,   144,   145,   146,    -1,   148,    -1,    -1,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,   163,    -1,    -1,   166,   167,   168,   169,     3,
       4,   172,     6,     7,     8,     9,    10,    11,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,     3,     4,    -1,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    43,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,
      -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,
      -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,   143,    -1,     3,     4,    -1,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,
      -1,    21,   164,    23,    -1,    25,    26,    -1,   170,    -1,
     172,    -1,   174,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    43,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,
      60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,     3,    -1,
      -1,   111,    -1,   113,    -1,    -1,   116,    -1,    13,    -1,
      -1,    -1,    -1,    18,    19,    20,    21,    -1,    -1,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,   141,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    56,    -1,    -1,   164,    -1,    -1,    -1,    -1,    64,
      -1,    -1,   172,    -1,   174,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,   100,    -1,   102,    -1,   104,
      -1,    -1,    -1,    -1,    -1,   110,   111,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,   143,   144,
     145,   146,    -1,   148,    -1,    -1,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,     3,   163,    -1,
      -1,   166,   167,   168,   169,   170,    -1,    13,    -1,    -1,
      -1,    -1,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    98,    -1,   100,    -1,   102,    20,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,   113,    -1,    32,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,    -1,   138,   139,    -1,   141,   142,   143,   144,   145,
     146,    -1,   148,    -1,    -1,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,   163,    -1,    -1,
     166,   167,   168,   169,    -1,    -1,    -1,    -1,    91,    -1,
      13,    -1,    95,    -1,    -1,    -1,    -1,    20,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,    -1,   138,   139,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,    -1,   170,    91,    -1,
      13,    -1,    95,    -1,   177,    -1,    -1,    20,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,    -1,   138,   139,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,    -1,   170,    91,    -1,
      13,    -1,    95,    -1,   177,    -1,    -1,    20,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,    -1,   138,   139,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,    -1,   170,    91,    -1,
      13,    -1,    95,    -1,   177,    -1,    -1,    20,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,    -1,   138,   139,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,    -1,   170,    91,    -1,
      -1,    -1,    95,    -1,   177,    -1,    -1,    -1,    -1,    13,
     103,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,    48,   138,   139,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    13,   177,    -1,    -1,    91,    -1,    -1,
      20,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    48,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    91,   166,   167,   168,    95,   170,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    32,   124,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    74,   166,   167,   168,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    14,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,   124,    -1,    -1,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    74,   165,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    91,    13,    -1,    -1,    95,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,    91,    -1,    13,    -1,    95,    -1,    -1,   177,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
     170,    91,    -1,    13,    -1,    95,    -1,   177,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
     170,    91,    -1,    13,    -1,    95,    -1,   177,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
     170,    91,    13,    -1,    -1,    95,    -1,   177,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,   177,    -1,    -1,
      -1,    -1,   103,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,   124,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    74,   166,   167,   168,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    13,    -1,    -1,
      -1,    91,    -1,    -1,    20,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    32,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    91,   166,   167,   168,    95,
      -1,    -1,    13,    14,    -1,    -1,    -1,   177,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,   120,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    95,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,   120,
     166,   167,   168,   124,    -1,    -1,    -1,    -1,    -1,   175,
      91,   177,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    13,    -1,    -1,   175,    -1,   177,    -1,    20,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      32,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,   177,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    13,    -1,    95,    -1,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,   170,    91,
      13,    -1,    -1,    95,    -1,   177,    -1,    20,    -1,   141,
     142,   103,   144,   145,   146,    -1,   148,   149,   150,    32,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   124,   165,   166,   167,   168,    -1,    -1,    -1,
      13,    14,    -1,    -1,    -1,   177,    -1,    20,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    32,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,    91,    -1,
      -1,    13,    95,    -1,    -1,   177,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,    91,
     173,    -1,    -1,    95,   177,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   123,   124,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    13,    -1,   166,   167,   168,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    20,
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
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      91,    13,    14,    -1,    95,    -1,   177,    -1,    20,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      32,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    13,   173,    -1,    -1,    -1,   177,    -1,    20,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      32,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,    91,
      -1,    -1,   173,    95,    -1,    -1,   177,    -1,    -1,    13,
      14,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    91,    -1,   141,
     142,    95,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    13,    -1,   166,   167,   168,    -1,    -1,    20,
     124,   173,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      13,    -1,   166,   167,   168,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
     141,   142,    95,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,   165,   166,   167,   168,    -1,    -1,
     123,   124,    -1,    -1,    -1,    -1,   177,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    13,    -1,   166,   167,   168,    -1,    -1,    20,    -1,
      -1,    -1,   175,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    14,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    13,    95,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,   141,
     142,    62,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    95,   177,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,   124,   166,   167,   168,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    20,   177,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    32,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,   177,    -1,    -1,    -1,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    13,    -1,   103,    -1,    -1,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,   141,   142,    95,   144,   145,
     146,    -1,   148,   149,   150,   103,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,   177,    -1,    91,    13,    -1,    -1,    95,    -1,    -1,
      -1,    20,    -1,   141,   142,   103,   144,   145,   146,    -1,
     148,   149,   150,    32,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,   177,
      -1,    20,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    32,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,    91,    -1,    13,    -1,    95,    -1,    -1,   177,
      -1,    20,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,   141,   142,   103,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   124,    -1,   166,   167,   168,
      -1,    -1,    91,    13,    -1,    -1,    95,    -1,   177,    -1,
      20,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    32,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   124,    -1,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    32,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,   177,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,    -1,   163,    -1,    -1,   166,   167,   168,     1,
      -1,     3,    -1,     5,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    -1,    21,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
       3,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,    -1,    -1,    18,    19,    79,    21,    -1,
      -1,    -1,    -1,    26,    -1,    28,    -1,    89,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    98,    40,   100,    -1,
     102,    -1,   104,    46,    -1,   107,    -1,    50,    -1,   111,
      53,   113,    55,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    92,
      -1,     3,    -1,     5,    -1,    98,    -1,   100,    -1,   102,
      -1,   104,    -1,    -1,   107,    -1,    18,    19,   111,    21,
     113,    -1,   115,   116,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
       3,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,    -1,    -1,    18,    19,    79,    21,    -1,
      -1,    -1,    -1,    26,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    98,    40,   100,    -1,
     102,    -1,   104,    46,    -1,   107,    -1,    50,    -1,   111,
      53,   113,    55,    -1,   116,    -1,    -1,    -1,    -1,     3,
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
      55,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,   100,    -1,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,    -1,
      -1,   116
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
     232,   233,   234,   235,   238,   239,   240,   244,   246,   249,
     250,   251,   252,   253,   257,   258,   263,   264,   265,   266,
     269,   270,   275,   276,   278,   279,   284,   288,   289,   291,
     292,   313,   318,   319,   323,   324,   346,   347,   348,   350,
     352,   353,   354,   360,   361,   362,   363,   364,   365,   367,
     370,   371,   372,   373,   374,   375,   376,   377,   379,   380,
     381,   382,   383,   165,    23,    36,    45,    55,    57,    90,
     102,   170,   239,   249,   276,   346,   353,   361,   362,   367,
     370,   372,   373,   120,   355,   356,     3,    55,   218,   367,
     355,   110,   325,    92,   218,   186,   340,   367,   172,     3,
      18,    19,    21,    26,    34,    40,    46,    50,    53,    55,
      64,    71,    72,    79,    89,    98,   100,   102,   104,   107,
     111,   113,   116,   219,   220,   222,    12,    28,   115,   245,
     367,    84,    87,   202,   172,   104,   220,   220,   220,   172,
     220,   327,   328,    33,   206,   225,   367,   261,   262,   367,
     367,    19,    79,    98,   113,   367,   367,   367,     9,   172,
     229,   228,    28,    34,    48,    50,    52,    77,    80,    92,
     102,   110,   115,   185,   224,   280,   281,   282,   304,   305,
     306,   332,   337,   367,   340,   108,   109,   165,   284,   285,
     366,   367,   369,   367,   225,   367,   367,   101,   172,   186,
     367,   367,    73,   188,   192,   206,   188,   206,   367,   369,
     367,   367,   367,   367,   367,     1,   171,   184,   207,   340,
     151,   341,   342,   369,   367,    82,   186,    23,    36,    39,
      73,    90,   170,   189,   190,   191,   202,   206,   194,   149,
     196,   170,    47,    86,   114,   203,    27,   324,   367,     9,
     265,   367,   368,    25,    33,    41,    42,    43,   119,   174,
     241,   242,   349,   351,    57,   146,   267,   220,   170,   172,
     300,    54,    75,    85,   308,    28,    48,    52,    77,    80,
      92,   110,   115,   309,    28,    48,    52,    77,    80,    92,
     110,   115,   307,   220,   320,   321,     1,   327,   164,   165,
     367,    13,    20,    32,    91,    95,   124,   141,   142,   144,
     145,   146,   148,   149,   150,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   166,   167,   168,
     177,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   169,   296,   172,   174,
      91,    95,   367,   220,    41,   174,   241,    57,     1,   172,
     186,   165,   206,   323,   165,   147,   165,   221,   343,   344,
     345,   369,   220,   220,   103,   206,   103,   125,   206,   296,
     203,   343,   165,   172,   172,   206,   186,   172,   220,   329,
     330,     1,   146,   336,    48,   147,   186,   225,   147,   225,
      14,   172,   172,   225,   326,   343,   230,   230,    48,    92,
     305,   306,   173,   147,   172,   220,   146,   165,   367,   367,
     123,   286,   165,   170,   225,   172,   343,   165,   367,   254,
     254,   165,   171,   171,   184,   147,   171,   367,   147,   147,
     173,   173,     9,   367,   174,   241,   242,     3,   172,   197,
       1,   171,   207,   214,   215,   367,   209,   367,    67,    37,
      74,   165,   265,   267,   110,   236,   289,   186,    80,   245,
     367,   122,   175,   243,   340,   367,   378,   243,   367,   220,
     340,   170,     1,   193,   220,   271,   274,   173,   301,   303,
     304,   309,    48,    92,     1,   146,   334,   335,    48,    92,
       1,     3,    13,    18,    20,    21,    26,    46,    53,    55,
      56,    64,    72,    89,   100,   104,   111,   116,   141,   142,
     143,   144,   145,   146,   148,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   163,   166,   167,   168,   169,
     172,   220,   293,   294,   295,   296,   346,   125,   322,   147,
     165,   165,   165,   367,   367,   367,   243,   367,   243,   367,
     367,   367,   367,   367,   367,   367,     3,    21,    34,    64,
     104,   110,   221,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
      70,   369,   369,   369,   369,   369,   343,   343,   243,   367,
     243,   367,   103,   172,   340,   243,   367,   220,     1,    28,
      48,    52,    77,    80,    92,   110,   115,   173,   357,   358,
     359,   367,   383,   327,   367,   125,   173,   147,   125,   125,
     186,    35,   186,   367,    35,   367,    67,   173,   344,   220,
     247,   248,   330,   147,   173,   224,   367,   125,   331,   367,
     328,   261,   220,   323,   367,   367,   323,   173,   220,   309,
     282,   330,   146,   338,   339,   337,   287,   205,     1,   259,
     343,   173,    22,   255,   171,   173,   173,   341,   369,   173,
     341,   340,   243,   367,   196,   173,   198,   199,   345,   171,
     171,   147,   165,    14,   149,   147,   165,    14,    37,    74,
     220,   141,   142,   143,   144,   145,   159,   163,   168,   208,
     295,   296,   297,   367,   208,   210,   267,   170,   237,   323,
     119,   220,   225,   225,   172,   367,   175,   356,    48,   147,
     175,   356,   123,   147,   175,    33,   225,   356,    33,    48,
     225,   356,   165,   172,     1,   268,   171,   274,   125,   147,
     171,   147,   173,   336,     1,   224,   332,   333,   106,   310,
     172,   299,   367,   141,   149,   299,   299,   367,   321,   172,
     174,   165,   165,   165,   165,   165,   165,   173,   175,   344,
      48,   175,    33,    33,    48,   172,   173,    48,    92,   147,
     173,    18,    21,    26,    46,    53,    64,    72,    89,   100,
     111,   116,   346,    91,    91,   165,   369,   345,   367,   367,
      35,   186,    35,    35,   186,   103,   206,   220,   173,   147,
     173,   173,   329,   336,    70,   369,   220,   173,   173,   327,
     339,   146,   283,   173,   337,   151,   298,   331,   367,   171,
      76,   117,   171,   260,   173,   172,   206,   220,   256,    48,
     175,    48,   147,   173,   214,   221,    18,    19,    21,    26,
      46,    50,    53,    72,    79,    89,    98,   100,   102,   111,
     113,   116,   170,   213,   297,   367,   367,   208,   210,   147,
     165,    14,   165,   170,   268,   320,   327,   367,    48,   340,
     186,   175,   243,   367,   175,   186,   367,   175,   367,   175,
     367,   367,   225,    45,   367,   243,   367,   225,   344,   171,
      84,    87,   171,   185,   193,   227,   367,   272,   273,   303,
     310,    62,   118,   314,   301,   302,   173,   294,   296,   173,
     175,   173,   243,   367,    45,   243,   367,   344,   358,   336,
     346,   346,   186,   186,   367,   186,    35,   165,   165,   248,
     206,   331,   172,   172,   165,   298,   331,   332,   310,   339,
     367,   186,   239,   340,   256,   146,   206,   243,   367,   243,
     367,   199,   208,    14,    37,    74,   165,   165,   297,   367,
     367,   268,   171,   165,   165,   165,   367,   173,   186,   175,
     356,   175,   356,   186,   123,   367,    33,   225,   356,    33,
     225,   356,   173,   193,   227,   227,   274,   193,   314,     3,
      55,    94,   104,   315,   316,   317,   367,   290,   173,   299,
     299,   175,   175,   367,    33,    33,   173,   331,    35,   186,
     343,   343,   298,   331,    33,   225,   173,   367,   175,   175,
     171,   367,   208,   210,    14,   171,   225,    45,   186,   367,
     175,    45,   186,   367,   175,   367,   103,    45,   367,   225,
      45,   367,   225,   165,   227,   274,   277,   317,   118,   147,
     125,   152,   154,   155,   157,   158,    62,    33,   165,   205,
     311,   312,    45,    45,   103,    45,    45,   186,   173,   173,
     186,   206,   165,   165,   367,   367,   186,   367,   186,   367,
     367,   367,   312,   367,   316,   317,   317,   317,   317,   317,
     317,   315,   184,   367,   367,   367,   367,   164,   164,   103,
     103,   103,   103,   103,   103,   103,   103,   367,   367,   367,
     367
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   181,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   189,   189,   190,   191,
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
     246,   246,   247,   247,   248,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   250,   250,
     250,   250,   250,   250,   251,   251,   251,   252,   252,   252,
     253,   253,   253,   253,   254,   254,   255,   255,   255,   256,
     256,   257,   258,   258,   259,   259,   260,   260,   260,   261,
     261,   261,   262,   262,   263,   264,   264,   265,   266,   266,
     266,   267,   267,   268,   268,   268,   268,   268,   269,   269,
     270,   271,   271,   272,   271,   271,   273,   271,   274,   274,
     275,   277,   276,   278,   279,   279,   279,   280,   280,   281,
     281,   282,   282,   282,   283,   283,   284,   286,   285,   287,
     285,   288,   290,   289,   291,   291,   291,   291,   291,   292,
     293,   293,   294,   294,   294,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   297,   297,   298,   298,   299,   299,   300,   300,
     301,   301,   302,   302,   303,   304,   304,   304,   304,   304,
     304,   305,   305,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   308,   308,   308,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   310,   310,
     311,   311,   312,   312,   313,   314,   314,   314,   314,   314,
     315,   315,   316,   316,   316,   316,   316,   316,   316,   317,
     317,   317,   318,   319,   319,   320,   320,   321,   322,   322,
     323,   323,   323,   323,   323,   325,   324,   326,   324,   324,
     324,   327,   327,   328,   328,   329,   329,   330,   330,   330,
     331,   331,   331,   332,   333,   333,   333,   334,   334,   335,
     335,   336,   336,   336,   336,   337,   337,   338,   339,   339,
     340,   340,   341,   342,   342,   343,   343,   344,   344,   345,
     345,   346,   346,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   348,   349,   349,   349,   349,   349,
     349,   349,   350,   351,   351,   351,   351,   351,   351,   351,
     352,   353,   354,   354,   354,   354,   354,   354,   354,   355,
     355,   356,   356,   356,   356,   357,   357,   358,   358,   358,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     360,   360,   360,   360,   360,   361,   361,   361,   361,   361,
     362,   363,   363,   363,   363,   363,   363,   364,   365,   366,
     366,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   368,   368,
     369,   369,   369,   370,   370,   370,   370,   371,   371,   371,
     371,   371,   372,   372,   372,   373,   373,   373,   373,   373,
     373,   374,   374,   374,   374,   375,   375,   376,   376,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   378,   378,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   379,   379,   379,   379,   380,   380,   380,
     380,   380,   380,   380,   381,   381,   381,   381,   382,   382,
     382,   382,   383,   383,   383,   383,   383,   383,   383
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
       6,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     6,     4,     7,     5,     3,     6,
       2,     2,     3,     3,     0,     2,     2,     3,     5,     1,
       3,     3,     5,     5,     0,     2,     3,     2,     4,     4,
       3,     1,     1,     3,     3,     5,     5,     2,     1,     1,
       1,     0,     2,     0,     2,     3,     3,     4,     4,     4,
       2,     1,     2,     0,     4,     2,     0,     5,     1,     3,
       1,     0,     8,     0,     1,     1,     1,     0,     1,     1,
       3,     1,     3,     1,     0,     2,     6,     0,     3,     0,
       4,     1,     0,     7,     4,     4,     6,     6,     4,     2,
       1,     3,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     0,     3,     2,     3,
       1,     3,     0,     1,     1,     4,     5,     4,     5,     6,
       6,     0,     1,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     0,     1,     1,     2,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     0,     1,     1,
       2,     1,     1,     1,     1,     2,     1,     1,     0,     1,
       1,     1,     1,     2,     1,     0,     2,     2,     4,     4,
       1,     3,     3,     3,     3,     3,     3,     3,     2,     1,
       1,     1,     3,     1,     2,     1,     3,     2,     0,     2,
       1,     2,     1,     1,     1,     0,     5,     0,     6,     3,
       3,     1,     3,     3,     5,     1,     3,     1,     2,     3,
       0,     2,     2,     1,     1,     1,     1,     2,     1,     0,
       1,     0,     2,     2,     1,     1,     1,     2,     0,     1,
       1,     3,     1,     3,     3,     0,     1,     1,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       4,     5,     2,     2,     1,     2,     1,     2,     1,     2,
       1,     2,     1,     1,     2,     6,     6,     4,     4,     9,
       9,     7,     2,     2,     3,     4,     6,     6,     9,     9,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     4,     3,     4,     2,     1,     3,     4,     3,     3,
       1,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     8,     8,     9,     9,
       4,     3,     3,     2,     2,     2,     1,     3,     4,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     4,     4,     4,     3,     3,     3,     3,     5,
       5,     3,     4,     3,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     4,     4,     5,
       4,     5,     3,     5,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1
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
#line 5971 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 668 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 5977 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 669 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 5983 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 676 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 5991 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 684 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 5999 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 688 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6008 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 698 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6016 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 703 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6024 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 710 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6030 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 711 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6036 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 712 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6042 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 713 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6048 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 714 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6054 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 715 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6060 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 716 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6066 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 717 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6072 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 718 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6078 "bison-chpl-lib.cpp"
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
#line 6100 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 737 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6106 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 738 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6112 "bison-chpl-lib.cpp"
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
#line 6124 "bison-chpl-lib.cpp"
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
#line 6137 "bison-chpl-lib.cpp"
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
#line 6150 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TLABEL ident_def stmt  */
#line 766 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6158 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 770 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6169 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt_base: error TSEMI  */
#line 777 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6178 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: assignment_stmt  */
#line 784 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6184 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: if_stmt  */
#line 785 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6190 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: loop_stmt  */
#line 786 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6196 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: select_stmt  */
#line 787 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6202 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: manage_stmt  */
#line 788 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6208 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 789 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6214 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: throw_stmt  */
#line 790 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6220 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 792 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6226 "bison-chpl-lib.cpp"
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
#line 6250 "bison-chpl-lib.cpp"
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
#line 6262 "bison-chpl-lib.cpp"
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
#line 6283 "bison-chpl-lib.cpp"
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
#line 6303 "bison-chpl-lib.cpp"
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
#line 6324 "bison-chpl-lib.cpp"
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
#line 6345 "bison-chpl-lib.cpp"
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
#line 6365 "bison-chpl-lib.cpp"
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
#line 6387 "bison-chpl-lib.cpp"
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
#line 6399 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 955 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6407 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 962 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6416 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 969 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6424 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 973 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6435 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 983 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6441 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 985 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6449 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 989 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6457 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 996 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6466 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 1001 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6476 "bison-chpl-lib.cpp"
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
#line 6498 "bison-chpl-lib.cpp"
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
#line 6519 "bison-chpl-lib.cpp"
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
#line 6540 "bison-chpl-lib.cpp"
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
#line 6559 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1086 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6565 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1087 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6572 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1089 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6579 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1094 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6585 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1095 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6592 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1101 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6602 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1119 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6611 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1124 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6620 "bison-chpl-lib.cpp"
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
#line 6633 "bison-chpl-lib.cpp"
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
#line 6652 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1159 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6658 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1160 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6664 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1165 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6672 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1169 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6680 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1173 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6689 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1178 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6697 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1182 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6705 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1186 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6714 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1194 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6723 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1199 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6733 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1205 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6742 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1210 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6753 "bison-chpl-lib.cpp"
    break;

  case 90: /* opt_only_ls: %empty  */
#line 1218 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6759 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: renames_ls  */
#line 1219 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6765 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1224 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6774 "bison-chpl-lib.cpp"
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
#line 6786 "bison-chpl-lib.cpp"
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
#line 6799 "bison-chpl-lib.cpp"
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
#line 6811 "bison-chpl-lib.cpp"
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
#line 6824 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1265 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6833 "bison-chpl-lib.cpp"
    break;

  case 114: /* import_expr: expr  */
#line 1300 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6841 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr TDOT all_op_name  */
#line 1304 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6850 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1309 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6859 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TAS ident_use  */
#line 1314 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6869 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1320 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6879 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_ls: import_expr  */
#line 1328 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6885 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_ls TCOMMA import_expr  */
#line 1329 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6891 "bison-chpl-lib.cpp"
    break;

  case 121: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1334 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6901 "bison-chpl-lib.cpp"
    break;

  case 122: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1343 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6909 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1347 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6917 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1351 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6925 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1355 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6933 "bison-chpl-lib.cpp"
    break;

  case 126: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1359 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6941 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1363 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6949 "bison-chpl-lib.cpp"
    break;

  case 128: /* opt_label_ident: %empty  */
#line 1371 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6955 "bison-chpl-lib.cpp"
    break;

  case 129: /* opt_label_ident: TIDENT  */
#line 1372 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6961 "bison-chpl-lib.cpp"
    break;

  case 130: /* opt_label_ident: TINIT  */
#line 1373 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6967 "bison-chpl-lib.cpp"
    break;

  case 139: /* ident_use: TIDENT  */
#line 1399 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6973 "bison-chpl-lib.cpp"
    break;

  case 140: /* ident_use: TINIT  */
#line 1400 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6979 "bison-chpl-lib.cpp"
    break;

  case 141: /* ident_use: TTHIS  */
#line 1401 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6985 "bison-chpl-lib.cpp"
    break;

  case 160: /* scalar_type: TBOOL  */
#line 1427 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6991 "bison-chpl-lib.cpp"
    break;

  case 161: /* scalar_type: TENUM  */
#line 1428 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6997 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TINT  */
#line 1429 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7003 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TUINT  */
#line 1430 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7009 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TREAL  */
#line 1431 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7015 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TIMAG  */
#line 1432 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7021 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TCOMPLEX  */
#line 1433 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7027 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TBYTES  */
#line 1434 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7033 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TSTRING  */
#line 1435 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7039 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TLOCALE  */
#line 1436 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7045 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TNOTHING  */
#line 1437 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7051 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TVOID  */
#line 1438 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7057 "bison-chpl-lib.cpp"
    break;

  case 175: /* do_stmt: TDO stmt  */
#line 1451 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7063 "bison-chpl-lib.cpp"
    break;

  case 176: /* do_stmt: block_stmt  */
#line 1452 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7069 "bison-chpl-lib.cpp"
    break;

  case 177: /* return_stmt: TRETURN TSEMI  */
#line 1457 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7080 "bison-chpl-lib.cpp"
    break;

  case 178: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1464 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7091 "bison-chpl-lib.cpp"
    break;

  case 179: /* class_level_stmt: TSEMI  */
#line 1474 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7099 "bison-chpl-lib.cpp"
    break;

  case 180: /* class_level_stmt: inner_class_level_stmt  */
#line 1478 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7109 "bison-chpl-lib.cpp"
    break;

  case 181: /* $@1: %empty  */
#line 1483 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7116 "bison-chpl-lib.cpp"
    break;

  case 182: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1485 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7125 "bison-chpl-lib.cpp"
    break;

  case 183: /* $@2: %empty  */
#line 1489 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7132 "bison-chpl-lib.cpp"
    break;

  case 184: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1491 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7141 "bison-chpl-lib.cpp"
    break;

  case 192: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1509 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7150 "bison-chpl-lib.cpp"
    break;

  case 193: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1514 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7159 "bison-chpl-lib.cpp"
    break;

  case 194: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1519 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7168 "bison-chpl-lib.cpp"
    break;

  case 195: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1524 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7176 "bison-chpl-lib.cpp"
    break;

  case 196: /* forwarding_decl_start: TFORWARDING  */
#line 1531 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7185 "bison-chpl-lib.cpp"
    break;

  case 197: /* extern_or_export: TEXTERN  */
#line 1538 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7191 "bison-chpl-lib.cpp"
    break;

  case 198: /* extern_or_export: TEXPORT  */
#line 1539 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7197 "bison-chpl-lib.cpp"
    break;

  case 199: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1544 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7207 "bison-chpl-lib.cpp"
    break;

  case 200: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1554 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7217 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1561 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7229 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1569 "chpl.ypp"
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
#line 7251 "bison-chpl-lib.cpp"
    break;

  case 203: /* $@3: %empty  */
#line 1587 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7259 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1591 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7268 "bison-chpl-lib.cpp"
    break;

  case 205: /* $@4: %empty  */
#line 1596 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7277 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1601 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7286 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1609 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7294 "bison-chpl-lib.cpp"
    break;

  case 208: /* no_loop_attributes: %empty  */
#line 1614 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7300 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1617 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7306 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1618 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7312 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1635 "chpl.ypp"
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
#line 7339 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1658 "chpl.ypp"
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
#line 7362 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1677 "chpl.ypp"
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
#line 7385 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1696 "chpl.ypp"
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
#line 7414 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1721 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7423 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: for_loop_kw expr TIN expr task_intent_clause do_stmt  */
#line 1726 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7432 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1731 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7441 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator task_intent_clause do_stmt  */
#line 1736 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7450 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1741 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7459 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw expr task_intent_clause do_stmt  */
#line 1746 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7468 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1751 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7477 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: for_loop_kw zippered_iterator task_intent_clause do_stmt  */
#line 1756 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7486 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1761 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7495 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TLSBR expr_ls TIN expr task_intent_clause TRSBR stmt  */
#line 1766 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7504 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1771 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7513 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator task_intent_clause TRSBR stmt  */
#line 1776 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7522 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1781 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7531 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR expr_ls task_intent_clause TRSBR stmt  */
#line 1786 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7540 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1791 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7550 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TLSBR zippered_iterator task_intent_clause TRSBR stmt  */
#line 1797 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7560 "bison-chpl-lib.cpp"
    break;

  case 235: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1806 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7570 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF expr TTHEN stmt  */
#line 1815 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7578 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr block_stmt  */
#line 1819 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7586 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1823 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7594 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1827 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7602 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1831 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7610 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar block_stmt  */
#line 1835 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7618 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1839 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7626 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1843 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7634 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1847 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7643 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1852 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7652 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1857 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7661 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1862 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7670 "bison-chpl-lib.cpp"
    break;

  case 248: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1870 "chpl.ypp"
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
#line 7689 "bison-chpl-lib.cpp"
    break;

  case 249: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1885 "chpl.ypp"
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
#line 7708 "bison-chpl-lib.cpp"
    break;

  case 250: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1903 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7716 "bison-chpl-lib.cpp"
    break;

  case 251: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1907 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7724 "bison-chpl-lib.cpp"
    break;

  case 252: /* ifc_formal_ls: ifc_formal  */
#line 1913 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7730 "bison-chpl-lib.cpp"
    break;

  case 253: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1914 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7736 "bison-chpl-lib.cpp"
    break;

  case 254: /* ifc_formal: ident_def  */
#line 1919 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7742 "bison-chpl-lib.cpp"
    break;

  case 274: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1962 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7750 "bison-chpl-lib.cpp"
    break;

  case 275: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1966 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7758 "bison-chpl-lib.cpp"
    break;

  case 276: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1970 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7766 "bison-chpl-lib.cpp"
    break;

  case 277: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1977 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7774 "bison-chpl-lib.cpp"
    break;

  case 278: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1981 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7782 "bison-chpl-lib.cpp"
    break;

  case 279: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1985 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7790 "bison-chpl-lib.cpp"
    break;

  case 280: /* try_stmt: TTRY tryable_stmt  */
#line 1992 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7798 "bison-chpl-lib.cpp"
    break;

  case 281: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1996 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7806 "bison-chpl-lib.cpp"
    break;

  case 282: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2000 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7814 "bison-chpl-lib.cpp"
    break;

  case 283: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2004 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7822 "bison-chpl-lib.cpp"
    break;

  case 284: /* catch_expr_ls: %empty  */
#line 2010 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7828 "bison-chpl-lib.cpp"
    break;

  case 285: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2011 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7834 "bison-chpl-lib.cpp"
    break;

  case 286: /* catch_expr: TCATCH block_stmt  */
#line 2016 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7842 "bison-chpl-lib.cpp"
    break;

  case 287: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2020 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7850 "bison-chpl-lib.cpp"
    break;

  case 288: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2024 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7858 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr_inner: ident_def  */
#line 2031 "chpl.ypp"
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
#line 7875 "bison-chpl-lib.cpp"
    break;

  case 290: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2044 "chpl.ypp"
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
#line 7892 "bison-chpl-lib.cpp"
    break;

  case 291: /* throw_stmt: TTHROW expr TSEMI  */
#line 2060 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7902 "bison-chpl-lib.cpp"
    break;

  case 292: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2068 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7911 "bison-chpl-lib.cpp"
    break;

  case 293: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2073 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7922 "bison-chpl-lib.cpp"
    break;

  case 294: /* when_stmt_ls: %empty  */
#line 2082 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7928 "bison-chpl-lib.cpp"
    break;

  case 295: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2083 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7934 "bison-chpl-lib.cpp"
    break;

  case 296: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2088 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7942 "bison-chpl-lib.cpp"
    break;

  case 297: /* when_stmt: TOTHERWISE stmt  */
#line 2092 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7951 "bison-chpl-lib.cpp"
    break;

  case 298: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2097 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 7961 "bison-chpl-lib.cpp"
    break;

  case 299: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2106 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 7970 "bison-chpl-lib.cpp"
    break;

  case 300: /* manager_expr: expr TAS ident_def  */
#line 2111 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 7978 "bison-chpl-lib.cpp"
    break;

  case 301: /* manager_expr: expr  */
#line 2115 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 7986 "bison-chpl-lib.cpp"
    break;

  case 302: /* manager_expr_ls: manager_expr  */
#line 2121 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7992 "bison-chpl-lib.cpp"
    break;

  case 303: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2122 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7998 "bison-chpl-lib.cpp"
    break;

  case 304: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2127 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8007 "bison-chpl-lib.cpp"
    break;

  case 305: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2137 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8017 "bison-chpl-lib.cpp"
    break;

  case 306: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2143 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8029 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_start: class_tag ident_def  */
#line 2155 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8037 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_tag: TCLASS  */
#line 2161 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8043 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_tag: TRECORD  */
#line 2162 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8049 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_tag: TUNION  */
#line 2163 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8055 "bison-chpl-lib.cpp"
    break;

  case 311: /* opt_inherit: %empty  */
#line 2167 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8061 "bison-chpl-lib.cpp"
    break;

  case 312: /* opt_inherit: TCOLON expr_ls  */
#line 2168 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8067 "bison-chpl-lib.cpp"
    break;

  case 313: /* class_level_stmt_ls: %empty  */
#line 2172 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8076 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2177 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8084 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2181 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8092 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2185 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8100 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2189 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8108 "bison-chpl-lib.cpp"
    break;

  case 318: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
#line 2197 "chpl.ypp"
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
#line 8131 "bison-chpl-lib.cpp"
    break;

  case 319: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
#line 2216 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8145 "bison-chpl-lib.cpp"
    break;

  case 320: /* enum_header: TENUM ident_def  */
#line 2229 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8153 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_ls: enum_item  */
#line 2236 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8162 "bison-chpl-lib.cpp"
    break;

  case 322: /* enum_ls: enum_ls TCOMMA  */
#line 2241 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8172 "bison-chpl-lib.cpp"
    break;

  case 323: /* $@5: %empty  */
#line 2247 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8181 "bison-chpl-lib.cpp"
    break;

  case 324: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2252 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8190 "bison-chpl-lib.cpp"
    break;

  case 325: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2257 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8199 "bison-chpl-lib.cpp"
    break;

  case 326: /* $@6: %empty  */
#line 2262 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8208 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2267 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8217 "bison-chpl-lib.cpp"
    break;

  case 328: /* enum_item: ident_def  */
#line 2277 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8228 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_item: ident_def TASSIGN expr  */
#line 2284 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8241 "bison-chpl-lib.cpp"
    break;

  case 330: /* lambda_decl_start: TLAMBDA  */
#line 2296 "chpl.ypp"
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
#line 8257 "bison-chpl-lib.cpp"
    break;

  case 331: /* $@7: %empty  */
#line 2313 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8267 "bison-chpl-lib.cpp"
    break;

  case 332: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2319 "chpl.ypp"
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
#line 8290 "bison-chpl-lib.cpp"
    break;

  case 334: /* linkage_spec: linkage_spec_empty  */
#line 2343 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8296 "bison-chpl-lib.cpp"
    break;

  case 335: /* linkage_spec: TINLINE  */
#line 2344 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8303 "bison-chpl-lib.cpp"
    break;

  case 336: /* linkage_spec: TOVERRIDE  */
#line 2346 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8310 "bison-chpl-lib.cpp"
    break;

  case 337: /* opt_fn_type_formal_ls: %empty  */
#line 2351 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8316 "bison-chpl-lib.cpp"
    break;

  case 338: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2352 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8322 "bison-chpl-lib.cpp"
    break;

  case 339: /* fn_type_formal_ls: fn_type_formal  */
#line 2356 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8328 "bison-chpl-lib.cpp"
    break;

  case 340: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2357 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8334 "bison-chpl-lib.cpp"
    break;

  case 341: /* fn_type_formal: named_formal  */
#line 2362 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8340 "bison-chpl-lib.cpp"
    break;

  case 342: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2365 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8346 "bison-chpl-lib.cpp"
    break;

  case 343: /* fn_type_formal: formal_type  */
#line 2367 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8352 "bison-chpl-lib.cpp"
    break;

  case 344: /* opt_fn_type_ret_type: %empty  */
#line 2371 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8358 "bison-chpl-lib.cpp"
    break;

  case 345: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2372 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8364 "bison-chpl-lib.cpp"
    break;

  case 346: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2385 "chpl.ypp"
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
#line 8384 "bison-chpl-lib.cpp"
    break;

  case 347: /* $@8: %empty  */
#line 2404 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8394 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2410 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8407 "bison-chpl-lib.cpp"
    break;

  case 349: /* $@9: %empty  */
#line 2419 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8417 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2425 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8430 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2437 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8438 "bison-chpl-lib.cpp"
    break;

  case 352: /* $@10: %empty  */
#line 2446 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8446 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2450 "chpl.ypp"
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
#line 8469 "bison-chpl-lib.cpp"
    break;

  case 354: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2472 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.thisIntentLoc = (yylsp[-2]);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8482 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2481 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.thisIntentLoc = (yylsp[-2]);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8495 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2490 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.thisIntentLoc = (yylsp[-4]);
      fp.receiver = context->buildThisFormal((yylsp[-3]), (yylsp[-4]), (yyvsp[-4].intentTag), (yyvsp[-3].expr), /*initExpr*/ nullptr);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8509 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2500 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.thisIntentLoc = (yylsp[-4]);
      fp.receiver = context->buildThisFormal((yylsp[-3]), (yylsp[-4]), (yyvsp[-4].intentTag), (yyvsp[-3].expr), /*initExpr*/ nullptr);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8523 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2510 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8533 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2519 "chpl.ypp"
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
#line 8549 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2534 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8555 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_ident: ident_def TBANG  */
#line 2541 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8565 "bison-chpl-lib.cpp"
    break;

  case 404: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2598 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8571 "bison-chpl-lib.cpp"
    break;

  case 405: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2599 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8577 "bison-chpl-lib.cpp"
    break;

  case 406: /* opt_formal_ls: %empty  */
#line 2603 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8583 "bison-chpl-lib.cpp"
    break;

  case 407: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2604 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8589 "bison-chpl-lib.cpp"
    break;

  case 408: /* req_formal_ls: TLP TRP  */
#line 2608 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8595 "bison-chpl-lib.cpp"
    break;

  case 409: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2609 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8601 "bison-chpl-lib.cpp"
    break;

  case 410: /* formal_ls_inner: formal  */
#line 2613 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8607 "bison-chpl-lib.cpp"
    break;

  case 411: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2614 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8613 "bison-chpl-lib.cpp"
    break;

  case 412: /* formal_ls: %empty  */
#line 2618 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8619 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal_ls: formal_ls_inner  */
#line 2619 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8625 "bison-chpl-lib.cpp"
    break;

  case 415: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2628 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8633 "bison-chpl-lib.cpp"
    break;

  case 416: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2633 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8641 "bison-chpl-lib.cpp"
    break;

  case 417: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2638 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8649 "bison-chpl-lib.cpp"
    break;

  case 418: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2643 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8657 "bison-chpl-lib.cpp"
    break;

  case 419: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2648 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yylsp[-5]), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8665 "bison-chpl-lib.cpp"
    break;

  case 420: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2653 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8673 "bison-chpl-lib.cpp"
    break;

  case 421: /* opt_formal_intent_tag: %empty  */
#line 2659 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);
  }
#line 8682 "bison-chpl-lib.cpp"
    break;

  case 422: /* opt_formal_intent_tag: required_intent_tag  */
#line 2664 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8691 "bison-chpl-lib.cpp"
    break;

  case 423: /* required_intent_tag: TIN  */
#line 2671 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::IN);            }
#line 8697 "bison-chpl-lib.cpp"
    break;

  case 424: /* required_intent_tag: TINOUT  */
#line 2672 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::INOUT);         }
#line 8703 "bison-chpl-lib.cpp"
    break;

  case 425: /* required_intent_tag: TOUT  */
#line 2673 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::OUT);           }
#line 8709 "bison-chpl-lib.cpp"
    break;

  case 426: /* required_intent_tag: TCONST TIN  */
#line 2674 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_IN);      }
#line 8715 "bison-chpl-lib.cpp"
    break;

  case 427: /* required_intent_tag: TCONST TREF  */
#line 2675 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);     }
#line 8721 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TCONST  */
#line 2676 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);         }
#line 8727 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TPARAM  */
#line 2677 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);         }
#line 8733 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TREF  */
#line 2678 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);           }
#line 8739 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TTYPE  */
#line 2679 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);          }
#line 8745 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TVAR  */
#line 2680 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR); }
#line 8751 "bison-chpl-lib.cpp"
    break;

  case 433: /* opt_this_intent_tag: %empty  */
#line 2684 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);     }
#line 8757 "bison-chpl-lib.cpp"
    break;

  case 434: /* opt_this_intent_tag: TPARAM  */
#line 2685 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);              }
#line 8763 "bison-chpl-lib.cpp"
    break;

  case 435: /* opt_this_intent_tag: TREF  */
#line 2686 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);                }
#line 8769 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_this_intent_tag: TCONST TREF  */
#line 2687 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);          }
#line 8775 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: TCONST  */
#line 2688 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);              }
#line 8781 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TTYPE  */
#line 2689 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);               }
#line 8787 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TIN  */
#line 2690 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::IN);       }
#line 8793 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TCONST TIN  */
#line 2691 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::CONST_IN); }
#line 8799 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TVAR  */
#line 2692 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR);      }
#line 8805 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TOUT  */
#line 2693 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::OUT);      }
#line 8811 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_this_intent_tag: TINOUT  */
#line 2694 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::INOUT);    }
#line 8817 "bison-chpl-lib.cpp"
    break;

  case 444: /* proc_iter_or_op: TPROC  */
#line 2698 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8823 "bison-chpl-lib.cpp"
    break;

  case 445: /* proc_iter_or_op: TITER  */
#line 2699 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8829 "bison-chpl-lib.cpp"
    break;

  case 446: /* proc_iter_or_op: TOPERATOR  */
#line 2700 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8835 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_ret_tag: %empty  */
#line 2704 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::DEFAULT_RETURN_INTENT); }
#line 8841 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_ret_tag: TOUT  */
#line 2705 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::OUT);                   }
#line 8847 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: TCONST  */
#line 2706 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST);                 }
#line 8853 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TCONST TREF  */
#line 2707 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST_REF);             }
#line 8859 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TREF  */
#line 2708 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::REF);                   }
#line 8865 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TPARAM  */
#line 2709 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::PARAM);                 }
#line 8871 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_ret_tag: TTYPE  */
#line 2710 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::TYPE);                  }
#line 8877 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: TIN  */
#line 2711 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::IN);            }
#line 8883 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: TCONST TIN  */
#line 2712 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::CONST_IN);      }
#line 8889 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: TVAR  */
#line 2713 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::VAR);           }
#line 8895 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_ret_tag: TINOUT  */
#line 2714 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::INOUT);         }
#line 8901 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_throws_error: %empty  */
#line 2718 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8907 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_throws_error: TTHROWS  */
#line 2719 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8913 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_function_body_stmt: TSEMI  */
#line 2722 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8919 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_function_body_stmt: function_body_stmt  */
#line 2723 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8925 "bison-chpl-lib.cpp"
    break;

  case 462: /* function_body_stmt: block_stmt_body  */
#line 2727 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8931 "bison-chpl-lib.cpp"
    break;

  case 463: /* function_body_stmt: TDO toplevel_stmt  */
#line 2728 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 8937 "bison-chpl-lib.cpp"
    break;

  case 464: /* query_expr: TQUERIEDIDENT  */
#line 2732 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8943 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_lifetime_where: %empty  */
#line 2737 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8949 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_lifetime_where: TWHERE expr  */
#line 2739 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8955 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2741 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8961 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2743 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 8967 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2745 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 8973 "bison-chpl-lib.cpp"
    break;

  case 470: /* lifetime_components_expr: lifetime_expr  */
#line 2750 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8979 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2752 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8985 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2757 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8991 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2759 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8997 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2761 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9003 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2763 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9009 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2765 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9015 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2767 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9021 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2769 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9027 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_ident: TIDENT  */
#line 2773 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9033 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_ident: TINIT  */
#line 2774 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9039 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_ident: TTHIS  */
#line 2775 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9045 "bison-chpl-lib.cpp"
    break;

  case 482: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2780 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9054 "bison-chpl-lib.cpp"
    break;

  case 483: /* type_alias_decl_stmt_start: TTYPE  */
#line 2789 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9062 "bison-chpl-lib.cpp"
    break;

  case 484: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2793 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9071 "bison-chpl-lib.cpp"
    break;

  case 485: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2801 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9079 "bison-chpl-lib.cpp"
    break;

  case 486: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2805 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9087 "bison-chpl-lib.cpp"
    break;

  case 487: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2812 "chpl.ypp"
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
#line 9110 "bison-chpl-lib.cpp"
    break;

  case 488: /* opt_init_type: %empty  */
#line 2833 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9116 "bison-chpl-lib.cpp"
    break;

  case 489: /* opt_init_type: TASSIGN expr  */
#line 2835 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9122 "bison-chpl-lib.cpp"
    break;

  case 490: /* var_decl_type: TPARAM  */
#line 2839 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9128 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_type: TCONST TREF  */
#line 2840 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9134 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_type: TREF  */
#line 2841 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9140 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_type: TCONST  */
#line 2842 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9146 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_type: TVAR  */
#line 2843 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9152 "bison-chpl-lib.cpp"
    break;

  case 495: /* $@11: %empty  */
#line 2848 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9161 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2852 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9170 "bison-chpl-lib.cpp"
    break;

  case 497: /* $@12: %empty  */
#line 2857 "chpl.ypp"
  {
    // Use a mid-rule action to preserve the 'on' expr
    context->noteVarDestinationExpr((yyvsp[0].expr));
  }
#line 9179 "bison-chpl-lib.cpp"
    break;

  case 498: /* var_decl_stmt: TON expr $@12 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2862 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9188 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2867 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9197 "bison-chpl-lib.cpp"
    break;

  case 500: /* var_decl_stmt: var_decl_type error TSEMI  */
#line 2872 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    context->resetDeclStateOnError();
  }
#line 9206 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2880 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9214 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2884 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9222 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2891 "chpl.ypp"
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
#line 9245 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2910 "chpl.ypp"
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
#line 9265 "bison-chpl-lib.cpp"
    break;

  case 505: /* tuple_var_decl_component: ident_def  */
#line 2929 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9273 "bison-chpl-lib.cpp"
    break;

  case 506: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2933 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9281 "bison-chpl-lib.cpp"
    break;

  case 507: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2940 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9287 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2942 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9293 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2944 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9299 "bison-chpl-lib.cpp"
    break;

  case 510: /* opt_init_expr: %empty  */
#line 2950 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9305 "bison-chpl-lib.cpp"
    break;

  case 511: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2951 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9311 "bison-chpl-lib.cpp"
    break;

  case 512: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2952 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9317 "bison-chpl-lib.cpp"
    break;

  case 513: /* formal_or_ret_type_expr: expr  */
#line 2956 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9323 "bison-chpl-lib.cpp"
    break;

  case 514: /* ret_type: formal_or_ret_type_expr  */
#line 2960 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9329 "bison-chpl-lib.cpp"
    break;

  case 515: /* ret_type: reserved_type_ident_use  */
#line 2961 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9335 "bison-chpl-lib.cpp"
    break;

  case 516: /* ret_type: error  */
#line 2962 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9341 "bison-chpl-lib.cpp"
    break;

  case 517: /* colon_ret_type: TCOLON ret_type  */
#line 2966 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9347 "bison-chpl-lib.cpp"
    break;

  case 518: /* colon_ret_type: error  */
#line 2967 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9353 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_ret_type: %empty  */
#line 2971 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9359 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_type: %empty  */
#line 2976 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9365 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_type: TCOLON expr  */
#line 2977 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9371 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2978 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9377 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_type: error  */
#line 2979 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9383 "bison-chpl-lib.cpp"
    break;

  case 525: /* formal_type: formal_or_ret_type_expr  */
#line 2983 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9389 "bison-chpl-lib.cpp"
    break;

  case 526: /* formal_type: reserved_type_ident_use  */
#line 2984 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9395 "bison-chpl-lib.cpp"
    break;

  case 527: /* colon_formal_type: TCOLON formal_type  */
#line 2988 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9401 "bison-chpl-lib.cpp"
    break;

  case 528: /* opt_colon_formal_type: %empty  */
#line 2992 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9407 "bison-chpl-lib.cpp"
    break;

  case 529: /* opt_colon_formal_type: colon_formal_type  */
#line 2993 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9413 "bison-chpl-lib.cpp"
    break;

  case 530: /* expr_ls: expr  */
#line 2999 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9419 "bison-chpl-lib.cpp"
    break;

  case 531: /* expr_ls: expr_ls TCOMMA expr  */
#line 3000 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9425 "bison-chpl-lib.cpp"
    break;

  case 532: /* tuple_component: opt_try_expr  */
#line 3004 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9431 "bison-chpl-lib.cpp"
    break;

  case 533: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3009 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9439 "bison-chpl-lib.cpp"
    break;

  case 534: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3013 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9447 "bison-chpl-lib.cpp"
    break;

  case 535: /* opt_actual_ls: %empty  */
#line 3019 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9453 "bison-chpl-lib.cpp"
    break;

  case 536: /* opt_actual_ls: actual_ls  */
#line 3020 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9459 "bison-chpl-lib.cpp"
    break;

  case 537: /* actual_ls: actual_expr  */
#line 3025 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9468 "bison-chpl-lib.cpp"
    break;

  case 538: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3030 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9478 "bison-chpl-lib.cpp"
    break;

  case 539: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3038 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9484 "bison-chpl-lib.cpp"
    break;

  case 540: /* actual_expr: opt_try_expr  */
#line 3039 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9490 "bison-chpl-lib.cpp"
    break;

  case 541: /* ident_expr: ident_use  */
#line 3043 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9496 "bison-chpl-lib.cpp"
    break;

  case 542: /* ident_expr: scalar_type  */
#line 3044 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9502 "bison-chpl-lib.cpp"
    break;

  case 548: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3057 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9508 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3059 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9514 "bison-chpl-lib.cpp"
    break;

  case 550: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3061 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9520 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3063 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9530 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TATOMIC expr  */
#line 3069 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9536 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TSYNC expr  */
#line 3071 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9542 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TOWNED  */
#line 3074 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9548 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TOWNED expr  */
#line 3076 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9554 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TUNMANAGED  */
#line 3078 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9560 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3080 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9566 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TSHARED  */
#line 3082 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9572 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TSHARED expr  */
#line 3084 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9578 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TBORROWED  */
#line 3086 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9584 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TBORROWED expr  */
#line 3088 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9590 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TCLASS  */
#line 3090 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9596 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TRECORD  */
#line 3092 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9602 "bison-chpl-lib.cpp"
    break;

  case 564: /* for_expr: no_loop_attributes for_expr_base  */
#line 3096 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9608 "bison-chpl-lib.cpp"
    break;

  case 565: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3101 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9616 "bison-chpl-lib.cpp"
    break;

  case 566: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3105 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9624 "bison-chpl-lib.cpp"
    break;

  case 567: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3109 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9632 "bison-chpl-lib.cpp"
    break;

  case 568: /* for_expr_base: for_loop_kw zippered_iterator TDO expr  */
#line 3113 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "expression-level loops with 'zip' must have an index.");
  }
#line 9640 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3117 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9653 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3126 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9666 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3135 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9679 "bison-chpl-lib.cpp"
    break;

  case 572: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3146 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9685 "bison-chpl-lib.cpp"
    break;

  case 573: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3151 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9693 "bison-chpl-lib.cpp"
    break;

  case 574: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3155 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9701 "bison-chpl-lib.cpp"
    break;

  case 575: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3159 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9709 "bison-chpl-lib.cpp"
    break;

  case 576: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3163 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9717 "bison-chpl-lib.cpp"
    break;

  case 577: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3167 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9725 "bison-chpl-lib.cpp"
    break;

  case 578: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3171 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9733 "bison-chpl-lib.cpp"
    break;

  case 579: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3175 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9741 "bison-chpl-lib.cpp"
    break;

  case 580: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3182 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9755 "bison-chpl-lib.cpp"
    break;

  case 581: /* nil_expr: TNIL  */
#line 3199 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9761 "bison-chpl-lib.cpp"
    break;

  case 589: /* opt_task_intent_ls: %empty  */
#line 3217 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9767 "bison-chpl-lib.cpp"
    break;

  case 590: /* opt_task_intent_ls: task_intent_clause  */
#line 3218 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9773 "bison-chpl-lib.cpp"
    break;

  case 591: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3223 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9781 "bison-chpl-lib.cpp"
    break;

  case 592: /* task_intent_clause: TWITH TLP TRP  */
#line 3227 "chpl.ypp"
  {
    auto err = context->syntax((yyloc), "'with' clause cannot be empty.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-2]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9792 "bison-chpl-lib.cpp"
    break;

  case 593: /* task_intent_clause: TWITH TLP error TRP  */
#line 3234 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[-2]), "invalid intent expression in 'with' clause.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9803 "bison-chpl-lib.cpp"
    break;

  case 594: /* task_intent_clause: TWITH error  */
#line 3241 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[0]), "missing parentheses around 'with' clause intents.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-1]), dummyLoc, dummyLoc, dummyLoc, exprs);
  }
#line 9814 "bison-chpl-lib.cpp"
    break;

  case 595: /* task_intent_ls: intent_expr  */
#line 3250 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9820 "bison-chpl-lib.cpp"
    break;

  case 596: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3251 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9826 "bison-chpl-lib.cpp"
    break;

  case 597: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3256 "chpl.ypp"
  {
    (yyval.expr) = context->buildTaskIntent((yyloc), (yylsp[-2]), nullptr, (yyvsp[-2].expr), (yyvsp[-3].taskIntent), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9834 "bison-chpl-lib.cpp"
    break;

  case 598: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3260 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9842 "bison-chpl-lib.cpp"
    break;

  case 599: /* intent_expr: expr TREDUCE ident_expr  */
#line 3264 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9850 "bison-chpl-lib.cpp"
    break;

  case 600: /* task_var_prefix: TCONST  */
#line 3270 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST);          }
#line 9856 "bison-chpl-lib.cpp"
    break;

  case 601: /* task_var_prefix: TIN  */
#line 3271 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::IN);             }
#line 9862 "bison-chpl-lib.cpp"
    break;

  case 602: /* task_var_prefix: TCONST TIN  */
#line 3272 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_IN);       }
#line 9868 "bison-chpl-lib.cpp"
    break;

  case 603: /* task_var_prefix: TREF  */
#line 3273 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::REF);            }
#line 9874 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_var_prefix: TCONST TREF  */
#line 3274 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_REF);      }
#line 9880 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_var_prefix: TVAR  */
#line 3275 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::VAR);            }
#line 9886 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_var_prefix: TOUT  */
#line 3276 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::OUT);   }
#line 9892 "bison-chpl-lib.cpp"
    break;

  case 607: /* task_var_prefix: TINOUT  */
#line 3277 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::INOUT); }
#line 9898 "bison-chpl-lib.cpp"
    break;

  case 608: /* task_var_prefix: TPARAM  */
#line 3278 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::PARAM); }
#line 9904 "bison-chpl-lib.cpp"
    break;

  case 609: /* task_var_prefix: TTYPE  */
#line 3279 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::TYPE);  }
#line 9910 "bison-chpl-lib.cpp"
    break;

  case 610: /* new_maybe_decorated: TNEW  */
#line 3284 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9916 "bison-chpl-lib.cpp"
    break;

  case 611: /* new_maybe_decorated: TNEW TOWNED  */
#line 3286 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9922 "bison-chpl-lib.cpp"
    break;

  case 612: /* new_maybe_decorated: TNEW TSHARED  */
#line 3288 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9928 "bison-chpl-lib.cpp"
    break;

  case 613: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3290 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9934 "bison-chpl-lib.cpp"
    break;

  case 614: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3292 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9940 "bison-chpl-lib.cpp"
    break;

  case 615: /* new_expr: new_maybe_decorated expr  */
#line 3298 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 9948 "bison-chpl-lib.cpp"
    break;

  case 616: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3305 "chpl.ypp"
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
#line 9964 "bison-chpl-lib.cpp"
    break;

  case 617: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3317 "chpl.ypp"
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
#line 9980 "bison-chpl-lib.cpp"
    break;

  case 618: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
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
    (yyval.expr) = context->buildNewExpr((yyloc), New::OWNED, call.release());
  }
#line 9997 "bison-chpl-lib.cpp"
    break;

  case 619: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3342 "chpl.ypp"
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
#line 10015 "bison-chpl-lib.cpp"
    break;

  case 620: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3359 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10023 "bison-chpl-lib.cpp"
    break;

  case 621: /* range_literal_expr: expr TDOTDOT expr  */
#line 3366 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10032 "bison-chpl-lib.cpp"
    break;

  case 622: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3371 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10041 "bison-chpl-lib.cpp"
    break;

  case 623: /* range_literal_expr: expr TDOTDOT  */
#line 3376 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10050 "bison-chpl-lib.cpp"
    break;

  case 624: /* range_literal_expr: TDOTDOT expr  */
#line 3381 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10060 "bison-chpl-lib.cpp"
    break;

  case 625: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3387 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10070 "bison-chpl-lib.cpp"
    break;

  case 626: /* range_literal_expr: TDOTDOT  */
#line 3393 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10080 "bison-chpl-lib.cpp"
    break;

  case 627: /* cast_expr: expr TCOLON expr  */
#line 3423 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10088 "bison-chpl-lib.cpp"
    break;

  case 628: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3430 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10096 "bison-chpl-lib.cpp"
    break;

  case 629: /* super_expr: fn_expr  */
#line 3436 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10102 "bison-chpl-lib.cpp"
    break;

  case 633: /* expr: sub_type_level_expr TQUESTION  */
#line 3445 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10108 "bison-chpl-lib.cpp"
    break;

  case 634: /* expr: TQUESTION  */
#line 3447 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10114 "bison-chpl-lib.cpp"
    break;

  case 638: /* expr: fn_type  */
#line 3452 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10120 "bison-chpl-lib.cpp"
    break;

  case 648: /* opt_expr: %empty  */
#line 3466 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10126 "bison-chpl-lib.cpp"
    break;

  case 649: /* opt_expr: expr  */
#line 3467 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10132 "bison-chpl-lib.cpp"
    break;

  case 650: /* opt_try_expr: TTRY expr  */
#line 3471 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10138 "bison-chpl-lib.cpp"
    break;

  case 651: /* opt_try_expr: TTRYBANG expr  */
#line 3472 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10144 "bison-chpl-lib.cpp"
    break;

  case 652: /* opt_try_expr: super_expr  */
#line 3473 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10150 "bison-chpl-lib.cpp"
    break;

  case 658: /* call_base_expr: expr TBANG  */
#line 3490 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10158 "bison-chpl-lib.cpp"
    break;

  case 659: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3493 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10164 "bison-chpl-lib.cpp"
    break;

  case 662: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3500 "chpl.ypp"
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
#line 10180 "bison-chpl-lib.cpp"
    break;

  case 663: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3512 "chpl.ypp"
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
#line 10196 "bison-chpl-lib.cpp"
    break;

  case 664: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3524 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10204 "bison-chpl-lib.cpp"
    break;

  case 665: /* dot_expr: expr TDOT ident_use  */
#line 3531 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10210 "bison-chpl-lib.cpp"
    break;

  case 666: /* dot_expr: expr TDOT TTYPE  */
#line 3533 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10216 "bison-chpl-lib.cpp"
    break;

  case 667: /* dot_expr: expr TDOT TDOMAIN  */
#line 3535 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10222 "bison-chpl-lib.cpp"
    break;

  case 668: /* dot_expr: expr TDOT TLOCALE  */
#line 3537 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10228 "bison-chpl-lib.cpp"
    break;

  case 669: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3539 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10234 "bison-chpl-lib.cpp"
    break;

  case 670: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3541 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10240 "bison-chpl-lib.cpp"
    break;

  case 671: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3550 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenthLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10250 "bison-chpl-lib.cpp"
    break;

  case 672: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3556 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10258 "bison-chpl-lib.cpp"
    break;

  case 673: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3560 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10266 "bison-chpl-lib.cpp"
    break;

  case 674: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3564 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10274 "bison-chpl-lib.cpp"
    break;

  case 675: /* bool_literal: TFALSE  */
#line 3570 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10280 "bison-chpl-lib.cpp"
    break;

  case 676: /* bool_literal: TTRUE  */
#line 3571 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10286 "bison-chpl-lib.cpp"
    break;

  case 677: /* str_bytes_literal: STRINGLITERAL  */
#line 3575 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10292 "bison-chpl-lib.cpp"
    break;

  case 678: /* str_bytes_literal: BYTESLITERAL  */
#line 3576 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10298 "bison-chpl-lib.cpp"
    break;

  case 681: /* literal_expr: INTLITERAL  */
#line 3582 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10304 "bison-chpl-lib.cpp"
    break;

  case 682: /* literal_expr: REALLITERAL  */
#line 3583 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10310 "bison-chpl-lib.cpp"
    break;

  case 683: /* literal_expr: IMAGLITERAL  */
#line 3584 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10316 "bison-chpl-lib.cpp"
    break;

  case 684: /* literal_expr: CSTRINGLITERAL  */
#line 3585 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10322 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: TNONE  */
#line 3586 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10328 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3588 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10337 "bison-chpl-lib.cpp"
    break;

  case 687: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3593 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10346 "bison-chpl-lib.cpp"
    break;

  case 688: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3598 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10355 "bison-chpl-lib.cpp"
    break;

  case 689: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3603 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10364 "bison-chpl-lib.cpp"
    break;

  case 690: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3608 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10373 "bison-chpl-lib.cpp"
    break;

  case 691: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3613 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10382 "bison-chpl-lib.cpp"
    break;

  case 692: /* assoc_expr_ls: expr TALIAS expr  */
#line 3622 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10391 "bison-chpl-lib.cpp"
    break;

  case 693: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3627 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10401 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TPLUS expr  */
#line 3635 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10407 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TMINUS expr  */
#line 3636 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10413 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TSTAR expr  */
#line 3637 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10419 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TDIVIDE expr  */
#line 3638 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10425 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3639 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10431 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3640 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10437 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TMOD expr  */
#line 3641 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10443 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TEQUAL expr  */
#line 3642 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10449 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3643 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10455 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3644 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10461 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3645 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10467 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TLESS expr  */
#line 3646 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10473 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TGREATER expr  */
#line 3647 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10479 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TBAND expr  */
#line 3648 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10485 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TBOR expr  */
#line 3649 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10491 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TBXOR expr  */
#line 3650 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10497 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TAND expr  */
#line 3651 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10503 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TOR expr  */
#line 3652 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10509 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TEXP expr  */
#line 3653 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10515 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TBY expr  */
#line 3654 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10521 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TALIGN expr  */
#line 3655 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10527 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr THASH expr  */
#line 3656 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10533 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TDMAPPED expr  */
#line 3657 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10539 "bison-chpl-lib.cpp"
    break;

  case 717: /* unary_op_expr: TPLUS expr  */
#line 3661 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10545 "bison-chpl-lib.cpp"
    break;

  case 718: /* unary_op_expr: TMINUS expr  */
#line 3662 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10551 "bison-chpl-lib.cpp"
    break;

  case 719: /* unary_op_expr: TMINUSMINUS expr  */
#line 3663 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10557 "bison-chpl-lib.cpp"
    break;

  case 720: /* unary_op_expr: TPLUSPLUS expr  */
#line 3664 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10563 "bison-chpl-lib.cpp"
    break;

  case 721: /* unary_op_expr: TBANG expr  */
#line 3665 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10569 "bison-chpl-lib.cpp"
    break;

  case 722: /* unary_op_expr: expr TBANG  */
#line 3666 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10577 "bison-chpl-lib.cpp"
    break;

  case 723: /* unary_op_expr: TBNOT expr  */
#line 3669 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10583 "bison-chpl-lib.cpp"
    break;

  case 724: /* reduce_expr: expr TREDUCE expr  */
#line 3674 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10591 "bison-chpl-lib.cpp"
    break;

  case 725: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3678 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10599 "bison-chpl-lib.cpp"
    break;

  case 726: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3682 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10607 "bison-chpl-lib.cpp"
    break;

  case 727: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3686 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10615 "bison-chpl-lib.cpp"
    break;

  case 728: /* scan_expr: expr TSCAN expr  */
#line 3693 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10623 "bison-chpl-lib.cpp"
    break;

  case 729: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3697 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10631 "bison-chpl-lib.cpp"
    break;

  case 730: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3701 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10639 "bison-chpl-lib.cpp"
    break;

  case 731: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3705 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10647 "bison-chpl-lib.cpp"
    break;


#line 10651 "bison-chpl-lib.cpp"

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
