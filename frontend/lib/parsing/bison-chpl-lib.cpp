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
  YYSYMBOL_TSPARSE = 98,                   /* TSPARSE  */
  YYSYMBOL_TSTRING = 99,                   /* TSTRING  */
  YYSYMBOL_TSUBDOMAIN = 100,               /* TSUBDOMAIN  */
  YYSYMBOL_TSYNC = 101,                    /* TSYNC  */
  YYSYMBOL_TTHEN = 102,                    /* TTHEN  */
  YYSYMBOL_TTHIS = 103,                    /* TTHIS  */
  YYSYMBOL_TTHROW = 104,                   /* TTHROW  */
  YYSYMBOL_TTHROWS = 105,                  /* TTHROWS  */
  YYSYMBOL_TTRUE = 106,                    /* TTRUE  */
  YYSYMBOL_TTRY = 107,                     /* TTRY  */
  YYSYMBOL_TTRYBANG = 108,                 /* TTRYBANG  */
  YYSYMBOL_TTYPE = 109,                    /* TTYPE  */
  YYSYMBOL_TUINT = 110,                    /* TUINT  */
  YYSYMBOL_TUNION = 111,                   /* TUNION  */
  YYSYMBOL_TUNMANAGED = 112,               /* TUNMANAGED  */
  YYSYMBOL_TUSE = 113,                     /* TUSE  */
  YYSYMBOL_TVAR = 114,                     /* TVAR  */
  YYSYMBOL_TVOID = 115,                    /* TVOID  */
  YYSYMBOL_TWHEN = 116,                    /* TWHEN  */
  YYSYMBOL_TWHERE = 117,                   /* TWHERE  */
  YYSYMBOL_TWHILE = 118,                   /* TWHILE  */
  YYSYMBOL_TWITH = 119,                    /* TWITH  */
  YYSYMBOL_TYIELD = 120,                   /* TYIELD  */
  YYSYMBOL_TZIP = 121,                     /* TZIP  */
  YYSYMBOL_TALIAS = 122,                   /* TALIAS  */
  YYSYMBOL_TAND = 123,                     /* TAND  */
  YYSYMBOL_TASSIGN = 124,                  /* TASSIGN  */
  YYSYMBOL_TASSIGNBAND = 125,              /* TASSIGNBAND  */
  YYSYMBOL_TASSIGNBOR = 126,               /* TASSIGNBOR  */
  YYSYMBOL_TASSIGNBXOR = 127,              /* TASSIGNBXOR  */
  YYSYMBOL_TASSIGNDIVIDE = 128,            /* TASSIGNDIVIDE  */
  YYSYMBOL_TASSIGNEXP = 129,               /* TASSIGNEXP  */
  YYSYMBOL_TASSIGNLAND = 130,              /* TASSIGNLAND  */
  YYSYMBOL_TASSIGNLOR = 131,               /* TASSIGNLOR  */
  YYSYMBOL_TASSIGNMINUS = 132,             /* TASSIGNMINUS  */
  YYSYMBOL_TASSIGNMOD = 133,               /* TASSIGNMOD  */
  YYSYMBOL_TASSIGNMULTIPLY = 134,          /* TASSIGNMULTIPLY  */
  YYSYMBOL_TASSIGNPLUS = 135,              /* TASSIGNPLUS  */
  YYSYMBOL_TASSIGNREDUCE = 136,            /* TASSIGNREDUCE  */
  YYSYMBOL_TASSIGNSL = 137,                /* TASSIGNSL  */
  YYSYMBOL_TASSIGNSR = 138,                /* TASSIGNSR  */
  YYSYMBOL_TATMARK = 139,                  /* TATMARK  */
  YYSYMBOL_TBANG = 140,                    /* TBANG  */
  YYSYMBOL_TBAND = 141,                    /* TBAND  */
  YYSYMBOL_TBNOT = 142,                    /* TBNOT  */
  YYSYMBOL_TBOR = 143,                     /* TBOR  */
  YYSYMBOL_TBXOR = 144,                    /* TBXOR  */
  YYSYMBOL_TCOLON = 145,                   /* TCOLON  */
  YYSYMBOL_TCOMMA = 146,                   /* TCOMMA  */
  YYSYMBOL_TDIVIDE = 147,                  /* TDIVIDE  */
  YYSYMBOL_TDOT = 148,                     /* TDOT  */
  YYSYMBOL_TDOTDOT = 149,                  /* TDOTDOT  */
  YYSYMBOL_TDOTDOTDOT = 150,               /* TDOTDOTDOT  */
  YYSYMBOL_TEQUAL = 151,                   /* TEQUAL  */
  YYSYMBOL_TEXP = 152,                     /* TEXP  */
  YYSYMBOL_TGREATER = 153,                 /* TGREATER  */
  YYSYMBOL_TGREATEREQUAL = 154,            /* TGREATEREQUAL  */
  YYSYMBOL_THASH = 155,                    /* THASH  */
  YYSYMBOL_TLESS = 156,                    /* TLESS  */
  YYSYMBOL_TLESSEQUAL = 157,               /* TLESSEQUAL  */
  YYSYMBOL_TMINUS = 158,                   /* TMINUS  */
  YYSYMBOL_TMOD = 159,                     /* TMOD  */
  YYSYMBOL_TNOTEQUAL = 160,                /* TNOTEQUAL  */
  YYSYMBOL_TOR = 161,                      /* TOR  */
  YYSYMBOL_TPLUS = 162,                    /* TPLUS  */
  YYSYMBOL_TQUESTION = 163,                /* TQUESTION  */
  YYSYMBOL_TSEMI = 164,                    /* TSEMI  */
  YYSYMBOL_TSHIFTLEFT = 165,               /* TSHIFTLEFT  */
  YYSYMBOL_TSHIFTRIGHT = 166,              /* TSHIFTRIGHT  */
  YYSYMBOL_TSTAR = 167,                    /* TSTAR  */
  YYSYMBOL_TSWAP = 168,                    /* TSWAP  */
  YYSYMBOL_TLCBR = 169,                    /* TLCBR  */
  YYSYMBOL_TRCBR = 170,                    /* TRCBR  */
  YYSYMBOL_TLP = 171,                      /* TLP  */
  YYSYMBOL_TRP = 172,                      /* TRP  */
  YYSYMBOL_TLSBR = 173,                    /* TLSBR  */
  YYSYMBOL_TRSBR = 174,                    /* TRSBR  */
  YYSYMBOL_TNOELSE = 175,                  /* TNOELSE  */
  YYSYMBOL_TDOTDOTOPENHIGH = 176,          /* TDOTDOTOPENHIGH  */
  YYSYMBOL_TUPLUS = 177,                   /* TUPLUS  */
  YYSYMBOL_TUMINUS = 178,                  /* TUMINUS  */
  YYSYMBOL_TLNOT = 179,                    /* TLNOT  */
  YYSYMBOL_YYACCEPT = 180,                 /* $accept  */
  YYSYMBOL_program = 181,                  /* program  */
  YYSYMBOL_toplevel_stmt_ls = 182,         /* toplevel_stmt_ls  */
  YYSYMBOL_toplevel_stmt = 183,            /* toplevel_stmt  */
  YYSYMBOL_pragma_ls = 184,                /* pragma_ls  */
  YYSYMBOL_stmt = 185,                     /* stmt  */
  YYSYMBOL_stmt_base = 186,                /* stmt_base  */
  YYSYMBOL_tryable_stmt = 187,             /* tryable_stmt  */
  YYSYMBOL_decl_base = 188,                /* decl_base  */
  YYSYMBOL_collect_attributes = 189,       /* collect_attributes  */
  YYSYMBOL_attribute_receiver = 190,       /* attribute_receiver  */
  YYSYMBOL_attribute_group = 191,          /* attribute_group  */
  YYSYMBOL_attribute_decl_stmt_ls = 192,   /* attribute_decl_stmt_ls  */
  YYSYMBOL_attribute_decl_stmt = 193,      /* attribute_decl_stmt  */
  YYSYMBOL_attribute_decl_begin = 194,     /* attribute_decl_begin  */
  YYSYMBOL_opt_attribute_decl_toolspace = 195, /* opt_attribute_decl_toolspace  */
  YYSYMBOL_opt_attribute_actuals = 196,    /* opt_attribute_actuals  */
  YYSYMBOL_attribute_actuals_ls = 197,     /* attribute_actuals_ls  */
  YYSYMBOL_attribute_actual = 198,         /* attribute_actual  */
  YYSYMBOL_module_decl_start = 199,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 200,         /* module_decl_stmt  */
  YYSYMBOL_opt_access_control = 201,       /* opt_access_control  */
  YYSYMBOL_opt_prototype = 202,            /* opt_prototype  */
  YYSYMBOL_include_module_stmt = 203,      /* include_module_stmt  */
  YYSYMBOL_block_stmt_body = 204,          /* block_stmt_body  */
  YYSYMBOL_block_stmt = 205,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 206,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 207,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 208,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 209,              /* opt_only_ls  */
  YYSYMBOL_use_stmt = 210,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 211,              /* import_stmt  */
  YYSYMBOL_import_internal_type_ident = 212, /* import_internal_type_ident  */
  YYSYMBOL_import_expr = 213,              /* import_expr  */
  YYSYMBOL_import_ls = 214,                /* import_ls  */
  YYSYMBOL_require_stmt = 215,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 216,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 217,          /* opt_label_ident  */
  YYSYMBOL_reserved_word_ident = 218,      /* reserved_word_ident  */
  YYSYMBOL_ident_def = 219,                /* ident_def  */
  YYSYMBOL_ident_use = 220,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 221,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 222,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 223,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 224,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 225,              /* return_stmt  */
  YYSYMBOL_class_level_stmt = 226,         /* class_level_stmt  */
  YYSYMBOL_227_1 = 227,                    /* $@1  */
  YYSYMBOL_228_2 = 228,                    /* $@2  */
  YYSYMBOL_inner_class_level_stmt = 229,   /* inner_class_level_stmt  */
  YYSYMBOL_forwarding_decl_stmt = 230,     /* forwarding_decl_stmt  */
  YYSYMBOL_forwarding_decl_start = 231,    /* forwarding_decl_start  */
  YYSYMBOL_extern_or_export = 232,         /* extern_or_export  */
  YYSYMBOL_extern_export_decl_stmt_start = 233, /* extern_export_decl_stmt_start  */
  YYSYMBOL_extern_export_kind_inner = 234, /* extern_export_kind_inner  */
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
  YYSYMBOL_var_decl_start = 325,           /* var_decl_start  */
  YYSYMBOL_var_decl_stmt = 326,            /* var_decl_stmt  */
  YYSYMBOL_327_11 = 327,                   /* $@11  */
  YYSYMBOL_328_12 = 328,                   /* $@12  */
  YYSYMBOL_var_decl_stmt_inner_ls = 329,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 330,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 331, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 332, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 333,            /* opt_init_expr  */
  YYSYMBOL_formal_or_ret_type_expr = 334,  /* formal_or_ret_type_expr  */
  YYSYMBOL_ret_type = 335,                 /* ret_type  */
  YYSYMBOL_colon_ret_type = 336,           /* colon_ret_type  */
  YYSYMBOL_opt_ret_type = 337,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 338,                 /* opt_type  */
  YYSYMBOL_formal_type = 339,              /* formal_type  */
  YYSYMBOL_colon_formal_type = 340,        /* colon_formal_type  */
  YYSYMBOL_opt_colon_formal_type = 341,    /* opt_colon_formal_type  */
  YYSYMBOL_expr_ls = 342,                  /* expr_ls  */
  YYSYMBOL_expr_ls_end_semi = 343,         /* expr_ls_end_semi  */
  YYSYMBOL_expr_ls_semi = 344,             /* expr_ls_semi  */
  YYSYMBOL_semicolon_list = 345,           /* semicolon_list  */
  YYSYMBOL_tuple_component = 346,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 347,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 348,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 349,                /* actual_ls  */
  YYSYMBOL_actual_expr = 350,              /* actual_expr  */
  YYSYMBOL_ident_expr = 351,               /* ident_expr  */
  YYSYMBOL_sub_type_level_expr = 352,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 353,                 /* for_expr  */
  YYSYMBOL_for_expr_base = 354,            /* for_expr_base  */
  YYSYMBOL_bracket_loop_expr = 355,        /* bracket_loop_expr  */
  YYSYMBOL_bracket_loop_expr_base = 356,   /* bracket_loop_expr_base  */
  YYSYMBOL_cond_expr = 357,                /* cond_expr  */
  YYSYMBOL_nil_expr = 358,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 359,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 360,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 361,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 362,           /* task_intent_ls  */
  YYSYMBOL_intent_expr = 363,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 364,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 365,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 366,                 /* new_expr  */
  YYSYMBOL_let_expr = 367,                 /* let_expr  */
  YYSYMBOL_range_literal_expr = 368,       /* range_literal_expr  */
  YYSYMBOL_cast_expr = 369,                /* cast_expr  */
  YYSYMBOL_tuple_expand_expr = 370,        /* tuple_expand_expr  */
  YYSYMBOL_super_expr = 371,               /* super_expr  */
  YYSYMBOL_expr = 372,                     /* expr  */
  YYSYMBOL_opt_expr = 373,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 374,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 375,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 376,           /* call_base_expr  */
  YYSYMBOL_call_expr = 377,                /* call_expr  */
  YYSYMBOL_dot_expr = 378,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 379,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 380,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 381,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 382,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 383,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 384,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 385,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 386,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 387,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 388       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 375 "chpl.ypp"

  // include the definition of methods for ParserContext
  #include "ParserContextImpl.h"
  // include helper macros for use in writing productions
  #include "parser-help.h"
  // include implementation of error logging hook
  #include "parser-yyerror.h"

#line 512 "bison-chpl-lib.cpp"

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
#define YYLAST   17880

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  180
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  209
/* YYNRULES -- Number of rules.  */
#define YYNRULES  753
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1278

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   434


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
     175,   176,   177,   178,   179
};

#if YYCHPL_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   686,   686,   690,   691,   696,   697,   705,   709,   719,
     724,   731,   732,   733,   734,   735,   736,   737,   738,   739,
     740,   758,   759,   760,   768,   777,   786,   790,   797,   805,
     806,   807,   808,   809,   810,   811,   812,   814,   834,   842,
     859,   875,   892,   909,   925,   946,   947,   948,   952,   958,
     959,   963,   967,   968,   972,   979,   987,   990,  1000,  1002,
    1007,  1013,  1018,  1027,  1031,  1052,  1069,  1086,  1104,  1105,
    1107,  1112,  1113,  1118,  1136,  1141,  1146,  1158,  1177,  1178,
    1182,  1186,  1190,  1195,  1199,  1203,  1211,  1216,  1222,  1227,
    1236,  1237,  1241,  1246,  1254,  1263,  1271,  1282,  1290,  1291,
    1292,  1293,  1294,  1295,  1296,  1297,  1298,  1299,  1300,  1301,
    1302,  1303,  1304,  1305,  1317,  1321,  1326,  1331,  1337,  1346,
    1347,  1351,  1360,  1364,  1368,  1372,  1376,  1380,  1389,  1390,
    1391,  1395,  1396,  1397,  1398,  1399,  1403,  1404,  1405,  1417,
    1418,  1419,  1424,  1425,  1426,  1427,  1428,  1429,  1430,  1431,
    1432,  1433,  1434,  1435,  1436,  1437,  1438,  1439,  1440,  1441,
    1445,  1446,  1447,  1448,  1449,  1450,  1451,  1452,  1453,  1454,
    1455,  1456,  1463,  1464,  1465,  1469,  1470,  1474,  1481,  1491,
    1495,  1501,  1501,  1507,  1507,  1516,  1517,  1518,  1519,  1520,
    1521,  1522,  1526,  1531,  1536,  1541,  1548,  1556,  1557,  1561,
    1574,  1582,  1589,  1598,  1617,  1616,  1626,  1625,  1638,  1645,
    1651,  1652,  1661,  1662,  1663,  1664,  1668,  1691,  1710,  1729,
    1754,  1759,  1764,  1769,  1774,  1779,  1784,  1789,  1794,  1799,
    1804,  1809,  1814,  1819,  1824,  1830,  1839,  1848,  1852,  1856,
    1860,  1864,  1868,  1872,  1876,  1880,  1885,  1890,  1895,  1903,
    1919,  1938,  1945,  1951,  1960,  1961,  1966,  1971,  1972,  1973,
    1974,  1975,  1976,  1977,  1978,  1979,  1980,  1981,  1982,  1987,
    1992,  1993,  1994,  1995,  2003,  2004,  2008,  2012,  2016,  2023,
    2027,  2031,  2038,  2042,  2046,  2050,  2057,  2058,  2062,  2066,
    2070,  2077,  2092,  2110,  2118,  2123,  2133,  2134,  2138,  2142,
    2147,  2156,  2161,  2165,  2172,  2173,  2177,  2187,  2193,  2205,
    2212,  2213,  2214,  2218,  2219,  2223,  2227,  2231,  2235,  2239,
    2247,  2266,  2279,  2286,  2291,  2298,  2297,  2307,  2313,  2312,
    2327,  2335,  2348,  2366,  2363,  2392,  2396,  2397,  2399,  2404,
    2405,  2409,  2410,  2414,  2417,  2419,  2424,  2425,  2436,  2457,
    2456,  2472,  2471,  2489,  2499,  2496,  2524,  2533,  2542,  2552,
    2562,  2571,  2586,  2587,  2595,  2596,  2597,  2606,  2607,  2608,
    2609,  2610,  2611,  2612,  2613,  2614,  2615,  2616,  2617,  2618,
    2619,  2620,  2621,  2622,  2623,  2624,  2625,  2626,  2627,  2628,
    2629,  2630,  2634,  2635,  2636,  2637,  2638,  2639,  2640,  2641,
    2642,  2643,  2644,  2645,  2646,  2647,  2652,  2653,  2657,  2658,
    2662,  2663,  2667,  2668,  2672,  2673,  2677,  2678,  2682,  2686,
    2690,  2695,  2700,  2705,  2710,  2718,  2722,  2730,  2731,  2732,
    2733,  2734,  2735,  2736,  2737,  2738,  2739,  2743,  2744,  2745,
    2746,  2747,  2748,  2749,  2750,  2751,  2752,  2753,  2757,  2758,
    2759,  2763,  2764,  2765,  2766,  2767,  2768,  2769,  2770,  2771,
    2772,  2773,  2777,  2778,  2781,  2782,  2786,  2787,  2791,  2796,
    2797,  2799,  2801,  2803,  2808,  2810,  2815,  2817,  2819,  2821,
    2823,  2825,  2827,  2832,  2833,  2834,  2838,  2847,  2853,  2863,
    2867,  2874,  2896,  2897,  2902,  2903,  2904,  2905,  2906,  2910,
    2919,  2918,  2928,  2927,  2937,  2942,  2951,  2955,  2962,  2981,
    3000,  3004,  3011,  3013,  3015,  3022,  3023,  3024,  3028,  3032,
    3033,  3034,  3038,  3039,  3043,  3044,  3048,  3049,  3050,  3051,
    3055,  3056,  3060,  3064,  3065,  3071,  3072,  3076,  3081,  3086,
    3094,  3097,  3103,  3104,  3110,  3114,  3118,  3125,  3126,  3130,
    3135,  3144,  3145,  3149,  3150,  3157,  3158,  3159,  3160,  3161,
    3162,  3164,  3166,  3168,  3174,  3176,  3179,  3181,  3183,  3185,
    3187,  3189,  3191,  3193,  3195,  3197,  3202,  3206,  3210,  3214,
    3218,  3222,  3231,  3240,  3252,  3256,  3260,  3264,  3268,  3272,
    3276,  3280,  3287,  3305,  3313,  3314,  3315,  3316,  3317,  3318,
    3319,  3323,  3324,  3328,  3332,  3339,  3346,  3356,  3357,  3361,
    3365,  3369,  3376,  3377,  3378,  3379,  3380,  3381,  3382,  3383,
    3384,  3385,  3389,  3391,  3393,  3395,  3397,  3403,  3410,  3423,
    3436,  3450,  3468,  3475,  3480,  3485,  3490,  3496,  3502,  3532,
    3539,  3546,  3547,  3551,  3553,  3554,  3556,  3558,  3559,  3560,
    3561,  3564,  3565,  3566,  3567,  3568,  3569,  3570,  3571,  3572,
    3576,  3577,  3581,  3582,  3583,  3587,  3588,  3589,  3590,  3599,
    3600,  3603,  3604,  3605,  3609,  3621,  3633,  3640,  3642,  3644,
    3646,  3648,  3650,  3659,  3665,  3669,  3673,  3680,  3681,  3685,
    3686,  3690,  3691,  3692,  3693,  3694,  3695,  3696,  3701,  3706,
    3711,  3716,  3721,  3729,  3739,  3749,  3754,  3763,  3768,  3777,
    3778,  3779,  3780,  3781,  3782,  3783,  3784,  3785,  3786,  3787,
    3788,  3789,  3790,  3791,  3792,  3793,  3794,  3795,  3796,  3797,
    3798,  3799,  3803,  3804,  3805,  3806,  3807,  3808,  3811,  3815,
    3819,  3823,  3827,  3834,  3838,  3842,  3846,  3854,  3855,  3856,
    3857,  3858,  3859,  3860
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
  "IMAGLITERAL", "STRINGLITERAL", "BYTESLITERAL", "EXTERNCODE", "TALIGN",
  "TAS", "TATOMIC", "TBEGIN", "TBREAK", "TBOOL", "TBORROWED", "TBY",
  "TBYTES", "TCATCH", "TCLASS", "TCOBEGIN", "TCOFORALL", "TCOMPLEX",
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
  "TSPARSE", "TSTRING", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS", "TTHROW",
  "TTHROWS", "TTRUE", "TTRY", "TTRYBANG", "TTYPE", "TUINT", "TUNION",
  "TUNMANAGED", "TUSE", "TVAR", "TVOID", "TWHEN", "TWHERE", "TWHILE",
  "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND",
  "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP",
  "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
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
  "extern_export_decl_stmt_start", "extern_export_kind_inner",
  "extern_export_decl_stmt", "$@3", "$@4", "extern_block_stmt",
  "no_loop_attributes", "loop_stmt", "for_loop_kw", "loop_stmt_base",
  "zippered_iterator", "if_stmt", "ifvar", "interface_start",
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
  "var_decl_start", "var_decl_stmt", "$@11", "$@12",
  "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
  "tuple_var_decl_component", "tuple_var_decl_stmt_inner_ls",
  "opt_init_expr", "formal_or_ret_type_expr", "ret_type", "colon_ret_type",
  "opt_ret_type", "opt_type", "formal_type", "colon_formal_type",
  "opt_colon_formal_type", "expr_ls", "expr_ls_end_semi", "expr_ls_semi",
  "semicolon_list", "tuple_component", "tuple_expr_ls", "opt_actual_ls",
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

#define YYPACT_NINF (-909)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-754)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -909,    94,  3816,  -909,   -23,   127,  -909,  -909,  -909,  -909,
    -909,  -909,  -909, 11916,    31,    62,   152, 13731,   186, 17647,
      31,   191,   271,   238,    62,  5208, 11916,   217, 17706,  -909,
     417,   394,  -909,  9849,   432,   178,   322,  -909,   445,   401,
   17706, 17706, 17706,  -909,  3146, 11055,   468, 11916, 11916,   253,
    -909,   525,   541, 11916,  -909, 13731,  -909, 11916,   520,   403,
     259,   328, 13088,   543, 17765,  -909, 11916,  7965, 11916, 11055,
   13731,   507,   566,   462,  5208,   578, 11916,   582,  6939,  6939,
    -909,   586,  -909, 13731,  -909,   600, 10020,  -909, 11916,  -909,
   11916,  -909,  -909, 13560, 11916,  -909, 10191,  -909,  -909,  -909,
    4164,  8136, 11916,  -909,  4860,  -909,  -909,  -909,  -909, 17368,
     652,  -909,   512,   504,  -909,   282,  -909,  -909,  -909,  -909,
    -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,  7281,
    -909,  7452,  -909,  -909,    96,  -909,  -909,   124,  -909,   619,
    -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,   529, 17706,
    -909,   509,   508,   418,  -909,   343,  -909,  -909,  -909,   968,
    1098,  -909,  -909, 17706,  -909,  1777,  -909,   517,   523,  -909,
    -909,  -909,   524,   526, 11916,   528,   532,  -909,  -909,  -909,
   17147,  2318,   380,   535,   536,  -909,  -909,   415,  -909,  -909,
    -909,  -909,  -909,   426,  -909,  -909,  -909, 11916,  -909, 17706,
    -909, 11916, 11916,    45,   641,   466,  -909,  -909,  -909,  -909,
   17147,   474,  -909,  -909,    50,  5208,  -909,  -909,  -909,   537,
       6,   538,  -909,   333,  -909,   540,  -909,    23, 17147,  8307,
    -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,
    -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,
    -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909, 17706, 17706,
     -38, 14213,  -909,  -909,   617,  8307,   546,   534,  -909,  5208,
    3505,    66,    12,  -909,  5208,  -909,  -909, 15196,  -909,    25,
   15605,  1166,  -909,   544,   547,  -909, 15196,     6,  1166,  -909,
    8307,  2031,  2031,    85,    33,  -909,    58,  -909,  -909,  -909,
    -909,  7623,  -909,  -909,   890,  -909,   539,   567,  -909,  -909,
    3619,   572,  -909,  -909, 17147,   137, 11916, 11916,  -909,   -44,
    -909,  -909, 17147,   555, 15724,  -909, 15196,     6,   550,  8307,
    -909, 17147, 15764, 11916,  -909,  -909,  -909,  -909,  -909,     6,
     559,   604,   604,   518,  1166,  1166,   160,  -909,  -909,  4338,
     194, 11916,   579,   132,   552,   518,   717,  -909,  -909, 17706,
    -909, 11916,  -909,  4164,  -909,   121,  -909,   617,  -909,  -909,
     724,   557,  4512, 11916,  -909, 11916,   665,  -909,  -909, 14867,
      53,   529, 17147,   410,  -909,  5208,   653,  -909,  -909,  9849,
   10362, 11226,  -909,  -909,  -909, 17706,  -909, 17706, 11916,   564,
    -909, 16725,    68,   968,  -909,  -909,  -909,  -909,   229,  -909,
    -909,  -909,  -909,  -909,  -909,  -909,    90,   235,  -909,  -909,
    -909,  -909,  -909,  -909,  -909, 13394,   610,   257,  -909,   571,
     284,   475,  -909,   589, 11916, 11916, 11916, 11226, 11226, 11916,
     481, 11916, 11916, 11916, 11916, 11916,   353, 13560, 11916, 11916,
   11916, 11916, 11916, 11916, 11916, 11916, 11916, 11916, 11916, 11916,
   11916, 11916, 11916, 11916,   670,  -909,  -909,  -909,  -909,  -909,
   10020, 10020,  -909,  -909,  -909,  -909, 10020,  -909,  -909, 10020,
   10020,  8307,  8478, 11226, 11226, 15804,   569,  -909, 11397, 11226,
   17706,  -909,  6249,  -909,  -909,  -909,  3146,  -909, 11916,  -909,
     621,   574,   597,  -909,  -909,   623,   624,  5208,   719,  5208,
    -909,  -909,  -909,   721, 11916,   685,   588,  -909, 10020,  -909,
    3505,  -909,  -909,   177,  -909, 12087,   640, 11916,  3146,  -909,
    -909, 11916,  -909, 17497, 11916, 11916,  -909,   333,   601,  -909,
    -909,  -909,  -909, 17706,  -909,   968, 13259,  3505,   627, 12087,
    -909, 17147, 17147,  -909,   538,  -909,    52,  -909,  8307,   607,
    -909, 15196,   748,   748,  -909,  -909,  -909,  -909, 11571,  -909,
   15883,  8652,  8823,  -909,  -909,  -909, 17147, 11226, 11226,  -909,
     512,  8994,  -909,   388,  -909,  4686,  -909,   287, 15923,   303,
   14957, 17706,  6765,  6594,  -909,   529,   612,  -909,  -909,   333,
    -909,   669, 17706,    72, 15196,   620, 13873,   -53,   138, 11916,
     -74, 15964,   -50,    24, 14702,  -909,   233,  -909,   220,   642,
    1634,   628, 17588,   666,   211,  -909,  -909,   239,  -909,  -909,
      79,  -909,  -909,  -909,  6423,  -909,   684,  -909,  -909,   622,
     649,  -909,   658,  -909,   661,   662,   663,   672,   673,  -909,
     674,   675,   676,   681,   683,   687,   688,  -909,  -909,  -909,
    -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,
    -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909, 11916,
     677,   689,   622,  -909,   622,  -909, 11916,  -909, 17706,  -909,
    -909,  -909,  3297,  3297,   473,  -909,   473,  -909,   473, 17186,
     764,   972,   428,     6,   604,  -909,   482,  -909,  -909,  -909,
    -909,  -909,   518, 14806,   473,   618,   618,  3297,   618,   618,
     660,   604, 14806, 17345,   660,  1166,  1166,   604,   518,   650,
     654,   668,   678,   680,   682,   690,   639,  -909,   473,  -909,
     473, 11916, 10020,   144,   801, 15276,   667,   691,   241,  -909,
    -909,  -909,  -909,  -909,  -909,  -909,  -909,   254,  -909,  2101,
   17226,   469,   309, 17147, 10020,  -909, 10020, 11916, 11916,   806,
    5208,   807, 16115,  5208, 15355, 17706,  -909,   274,   292,  3505,
      66,  -909, 17147,  9165,  -909, 17147,  -909,  -909,  -909, 17706,
   16155, 16195,  3146,  -909,   627,   700,  -909,   293, 12087,  -909,
     316,  -909, 11916,  -909,   679,   205,   697,  -909,  2785,  -909,
    -909,  -909,  -909,  -909,  -909,  -909,  -909,   181,    36, 14776,
    -909,  -909,   296,  -909,  -909,  -909,  -909, 11916,  -909,   216,
   13972, 11916,  -909, 11916,  6765,  6594,  -909,  9336,   472,  9507,
     506,   533,  9678,  7794,   542,   339,  -909,  -909,  -909, 16274,
     701,   693,   702,  -909, 17706,  3146, 11916,   812,  -909,  -909,
   11916, 17147,  5208,   696, 11226, 10536,  -909,  3990,   225,   699,
     729,  -909,   315, 11916, 11742,  -909,  5208,  -909,    72,  5382,
   11226,  -909,    72, 17706,   538,  -909, 10020,   706,  1407,  -909,
    -909, 11916,    55,  -909,  1208,  -909,   684,  -909,  -909,  -909,
    -909,  -909,     0,   262,  -909, 16314,  -909, 14130,  -909,  -909,
   17147,  -909,   710,   704,  -909,  -909,  -909,  -909,  -909,  -909,
    -909,  -909,   334, 11226, 13873, 11916, 12258, 11226, 10020,  -909,
    -909,  -909,  7110,  -909,  -909,  -909,  -909,  -909,  -909,  -909,
    -909,  -909,  -909,  -909,  -909,    43, 11226, 11226,  -909,  -909,
    -909, 17147, 17147,  5208,  -909,  5208, 11916,  -909,  5208,   814,
     722,   723,  -909,  -909,   640,  -909,  -909,  -909,   712,   714,
     367,   316, 11916,   684,   627,  -909, 10710,  -909,  -909, 17147,
    -909,  5208, 11916,  -909,  -909,  -909, 17706,  -909,   743,   538,
   11226,  5208, 11226, 10020,  -909,  -909,  -909,  -909,  -909,   482,
    -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,
    -909,  -909,  -909,  6765,  -909,  -909, 16362, 15446,   384,   725,
    6765,  -909, 11916,  -909,  -909,  1907,   391,   393, 16465, 11916,
     336,  -909,  5208,   -26, 15520,  -909,   351,  -909, 17147,  -909,
    -909,  5208, 10881,  -909, 17147,  -909, 16513, 17147,  -909,  9849,
   17147,    41, 15036,  -909,  -909,  -909,   337,  -909,  -909,  -909,
    -909,  2740,  3250,  -909, 17147, 17706,   652,  -909,     0,   243,
   11916,  -909,   746,   726,  -909,   622,   622,  -909,  -909,  -909,
     720, 16553, 11916,   861, 16704,   349,  -909,   640,   354,   356,
    -909,  -909, 17147,   863,  5208,  -909,  -909,  -909,  8307,  8307,
    -909,  -909,  -909,  -909,  -909,   316, 17147,  -909,   133,    80,
     727, 11916,  -909,   -22, 15565,    36, 15116,  -909,   223, 11916,
    6765,  6594,  -909,  -909,  -909, 16744, 17147,  2215,  -909,  -909,
    -909,  -909, 15196,  -909,  -909,  5556,   732,  5730,   733,  -909,
    -909, 11916, 14293,  5904,  -909,    72,  6078,  -909,    72,   731,
    3250,  -909,  -909,  -909, 17588,  -909,  -909,  -909,   242,  -909,
     -16,  -909,   321, 16785,    86,  -909,  -909,  -909, 12429, 12600,
   16900, 12771, 12942,  -909,  -909,  5208,  -909,   728,   738,  -909,
    -909,  5208,  -909,   538, 17147,  5208,  5208,  -909, 17147,   414,
     737, 11916,  -909,  -909,  9849,  -909, 17147,  5208,  9849,  -909,
   17147,  5208, 17147,  5208,  9849, 17147,  -909,  9849, 17147,  -909,
    -909,  -909,  -909,    91,  -909, 11916,   243,   242,   242,   242,
     242,   242,   242,   243,  5034,  -909,  -909,  -909,  -909, 11916,
   11916, 11916, 11916, 11916,  -909,   740,   750,   669,  -909,  -909,
    -909, 17147, 14373,  -909, 14453,  -909, 16115, 14533, 14613,  -909,
   17147,  -909,  -909,  -909,  -909,  -909,  -909,  -909,   772,  -909,
   16948, 16988, 17067, 17107,  -909,  -909,  5208,  5208,  5208,  5208,
   11916, 11916, 11916, 11916, 16115, 16115, 16115, 16115
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   139,   468,    55,   693,   694,
     695,   689,   690,   209,   601,   128,   160,   572,   167,   574,
     601,   166,   500,   497,   128,     0,   209,   274,   161,   198,
     197,   687,   196,   209,   165,    68,   275,   337,   162,   140,
       0,     0,     0,   332,     0,   209,   169,   209,   209,   622,
     593,   696,   170,   209,   338,   566,   494,   209,     0,     0,
     183,   181,   425,   164,   575,   496,   209,   209,   209,   209,
     570,     0,   168,     0,     0,   141,   209,   688,   209,   209,
     487,   163,   312,   568,   498,   171,   209,   749,   209,   751,
     209,   752,   753,   638,   209,   750,   209,   646,   179,   748,
       0,   209,   209,     4,     0,     5,    10,    11,    12,    48,
      51,    52,    56,     0,    45,    71,    13,    77,    14,    15,
      16,    17,    29,   553,   554,    22,    46,   180,   190,   209,
     199,   660,   191,    18,     0,    31,    30,     0,    47,     0,
     269,    19,   656,    21,    35,    32,    33,   189,   313,     0,
     187,     0,     0,   653,   336,     0,   650,   185,   353,   451,
     437,   648,   188,     0,   499,     0,   186,   665,   644,   643,
     647,   557,   555,     0,   209,   654,   655,   659,   658,   657,
       0,   556,     0,   666,   667,   668,   691,   692,   649,   559,
     558,   651,   652,     0,    28,   574,   161,   209,   140,     0,
     575,   209,   209,     0,     0,   653,   665,   555,   654,   655,
     564,   556,   666,   667,     0,     0,   602,   129,   130,     0,
     573,     0,   488,     0,   495,     0,    20,     0,   535,   209,
     136,   142,   153,   148,   147,   155,   133,   146,   156,   143,
     137,   157,   131,   158,   151,   145,   152,   149,   150,   132,
     134,   144,   154,   159,   138,   322,   135,   208,     0,     0,
       0,     0,    70,    69,    71,   209,     0,     0,   251,     0,
       0,     0,     0,   506,     0,   176,    40,     0,   304,     0,
     303,   734,   626,   623,   624,   625,   502,   567,   735,     7,
     209,   335,   335,   432,   173,   427,   174,   428,   429,   433,
     434,   172,   435,   436,   425,   531,     0,   340,   341,   343,
       0,   426,   530,   345,   518,     0,   209,   209,   177,   650,
     641,   664,   642,     0,     0,    43,     0,   571,     0,   209,
      44,   565,     0,   209,   282,    48,   286,   283,   286,   569,
       0,   736,   738,   636,   733,   732,     0,    74,    78,     0,
       0,   209,     0,     0,   544,   637,     0,     6,   310,     0,
     197,   209,   311,     0,    49,     0,     9,    71,    50,    53,
       0,    60,     0,   209,    72,   209,     0,   500,   195,     0,
     689,   313,   661,   204,   215,     0,   212,   213,   214,   209,
     209,   209,   211,   576,   584,     0,   253,     0,   209,     0,
     309,     0,   425,   451,   449,   450,   448,   361,   453,   458,
     461,   452,   456,   455,   457,   460,     0,   441,   443,   447,
     446,   438,   439,   442,   445,     0,   492,     0,   489,     0,
       0,   645,    34,   627,   209,   209,   209,   209,   209,   209,
     737,   209,   209,   209,   209,   209,     0,   635,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   392,   399,   400,   401,   396,   398,
     209,   209,   394,   397,   395,   393,   209,   405,   404,   209,
     209,   209,   209,   209,   209,     0,     0,   212,   209,   209,
       0,   606,     0,    36,    24,    37,     0,    25,   209,    38,
     553,     0,   548,   549,   552,     0,     0,     0,   242,     0,
     392,   402,   403,   238,   209,     0,     0,    23,   209,    26,
       0,   510,   512,     0,   529,   209,   515,   209,     0,   175,
      39,   209,   306,     0,   209,   209,    41,     0,     0,   184,
     182,   430,   431,     0,   426,   451,   425,     0,   533,   209,
     121,   662,   663,   351,     0,   178,     0,    42,   209,     0,
     293,     0,   284,   285,    27,    76,    75,    79,   209,   697,
       0,   209,   209,   685,   683,     8,   502,   209,   209,   210,
      56,   209,    54,     0,    65,     0,   119,     0,   114,     0,
      86,     0,   209,   209,   192,   313,     0,   200,   206,     0,
     203,     0,     0,     0,     0,     0,   585,     0,     0,   209,
       0,   535,     0,     0,     0,   256,     0,   254,   280,   314,
       0,     0,     0,   330,     0,   323,   412,     0,   414,   418,
       0,   459,   454,   523,     0,   525,   462,   444,   440,   410,
     136,   388,   142,   386,   148,   147,   146,   143,   137,   390,
     157,   158,   145,   149,   132,   144,   159,   385,   367,   370,
     368,   369,   391,   380,   371,   384,   376,   374,   387,   375,
     373,   378,   383,   372,   377,   381,   382,   379,   389,   209,
     364,     0,   410,   365,   410,   362,   209,   491,     0,   486,
     505,   504,   729,   728,   731,   740,   739,   744,   743,   725,
     722,   723,   724,   639,   712,   139,     0,   679,   680,   141,
     678,   677,   633,   716,   727,   721,   719,   730,   720,   718,
     710,   715,   717,   726,   709,   713,   714,   711,   634,     0,
       0,     0,     0,     0,     0,     0,     0,   742,   741,   746,
     745,   209,   209,     0,     0,     0,   280,     0,   612,   613,
     619,   618,   620,   615,   621,   617,   604,     0,   607,     0,
       0,     0,     0,   536,   209,   561,   209,   209,   209,   241,
       0,   237,     0,     0,     0,     0,   560,     0,     0,   513,
       0,   528,   527,   209,   508,   632,   507,   305,   302,     0,
       0,     0,     0,   676,   533,   346,   342,     0,   209,   534,
     515,   344,   209,   350,     0,     0,     0,   562,     0,   287,
     698,   640,   684,   545,   544,   686,   546,     0,     0,     0,
      57,    58,     0,    61,    63,    67,    66,   209,    97,     0,
       0,   209,    92,   209,   209,   209,    64,   209,   367,   209,
     368,   369,   209,   209,   379,     0,   406,   407,    81,    80,
      91,     0,     0,   315,     0,     0,   209,     0,   218,   217,
     209,   586,     0,     0,   209,   209,   542,     0,   537,     0,
     540,   701,   539,   209,   209,   705,     0,   226,     0,     0,
     209,   224,     0,     0,     0,   277,   209,     0,   335,   321,
     327,   209,   325,   320,   425,   413,   462,   521,   520,   519,
     522,   463,   469,   425,   360,     0,   366,     0,   356,   357,
     493,   490,     0,     0,   127,   125,   126,   124,   123,   122,
     674,   675,     0,   209,   699,   209,   209,   209,   209,   605,
     614,   616,   209,   603,   160,   167,   166,   165,   162,   169,
     170,   164,   168,   163,   171,     0,   209,   209,   501,   551,
     550,   250,   249,     0,   244,     0,   209,   240,     0,   246,
       0,   279,   511,   514,   515,   516,   517,   301,     0,     0,
       0,   515,   209,   462,   533,   532,   209,   421,   419,   352,
     295,     0,   209,   294,   297,   563,     0,   288,   291,     0,
     209,     0,   209,   209,    59,   120,   117,    98,   109,   104,
     103,   102,   111,    99,   112,   107,   101,   108,   105,   106,
     100,   110,   113,   209,   116,   115,    88,    87,     0,     0,
     209,   193,   209,   194,   315,   335,     0,     0,     0,   209,
       0,   234,     0,     0,     0,   700,   538,   232,   587,   543,
     703,     0,   209,   702,   707,   706,     0,   580,   227,   209,
     579,     0,     0,   225,   255,   252,     0,   308,   183,   181,
     307,   335,   335,   316,   331,     0,     0,   415,   469,     0,
     209,   354,   417,     0,   363,   410,   410,   681,   682,   279,
       0,     0,   209,     0,     0,     0,   608,   515,   665,   665,
     243,   239,   592,   245,     0,    73,   276,   509,   209,   209,
     503,   422,   420,   347,   348,   515,   409,   299,     0,     0,
       0,   209,   289,     0,     0,     0,     0,    62,     0,   209,
     209,   209,    93,    95,    84,    83,    82,   335,   201,   207,
     205,   216,     0,   236,   235,     0,     0,     0,     0,   704,
     233,   209,     0,     0,   222,     0,     0,   220,     0,   281,
     335,   318,   317,   326,     0,   333,   483,   484,     0,   485,
     471,   474,     0,   470,     0,   411,   358,   359,   209,   209,
       0,   209,   209,   281,   609,     0,   248,     0,     0,   424,
     423,     0,   298,     0,   292,     0,     0,   118,    89,     0,
       0,   209,   202,   219,   209,   230,   589,     0,   209,   228,
     588,     0,   708,     0,   209,   578,   223,   209,   577,   221,
     278,   319,   329,     0,   482,   209,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   464,   466,   355,   465,   209,
     209,   209,   209,   209,   247,   628,   629,   300,   290,    94,
      96,    85,     0,   231,     0,   229,   583,     0,     0,   334,
     473,   475,   476,   479,   480,   481,   477,   478,   472,   467,
       0,     0,     0,     0,   630,   631,     0,     0,     0,     0,
     209,   209,   209,   209,   591,   590,   582,   581
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -909,  -909,  -909,    -1,   -60,   828,  -909,   273,   811,  -909,
    -909,   486,  -397,  -105,  -909,   341,  -909,  -909,   -71,  -909,
    -909,   143,   664,  -909,  -545,  2090,   553,  -576,  -909,  -799,
    -909,  -909,  -909,    97,  -909,  -909,  -909,   902,  -909,     7,
    -219,  -909,  -909,  -455,    37,  -909,  -859,  -909,  -909,   324,
    -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,     5,
    -909,  -148,   562,  -303,  -909,   545,  -909,  -909,  -909,    56,
    1532,  -909,  -909,  -909,  -909,   595,  -909,   -51,  -909,  -909,
    -909,  -909,   405,  -909,  -909,  -909,  -107,  -909,  -342,  -808,
    -909,  -909,  -909,  -909,  -909,  -611,  -909,    29,  -909,  -909,
    -909,  -909,  -909,   392,  -909,  1959,  -909,  -909,  -909,  -909,
     560,  -909,  -909,  -909,  -909,    39,  -417,  -169,  -789,  -908,
    -632,  -909,    44,  -909,    46,   -48,   644,   -47,  -909,  -909,
    -345,  -834,  -909,  -264,  -909,  -118,  -271,  -262,  -616,  -909,
    -909,   101,   276,  -909,   429,  -202,   830,  -909,  -909,  -162,
     433,   188,  -421,  -772,  -614,  -909,  -909,  -909,  -608,  -511,
    -909,  -751,     8,  -909,  -909,  -573,   100,  -909,  -248,  -499,
    -568,    -2,  -909,  -909,  -909,  -909,  -909,  -909,   207,  -909,
     948,  -581,  -909,    38,  -909,  -909,   357,   467,  -909,  -909,
    -909,  -909,  2542,  -909,   -61,  1006,  -909,  1203,  1381,  -909,
    -909,  -909,  -909,  -909,  -909,  -909,  -909,  -909,  -474
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   348,   104,   105,   106,   107,   108,   365,
     366,   109,   110,   111,   112,   371,   582,   822,   823,   113,
     114,   115,   376,   116,   117,   118,   349,   850,   589,   851,
     119,   120,  1014,   586,   587,   121,   122,   219,   254,   271,
     123,   256,   124,   305,   536,   125,   126,   292,   291,   127,
     128,   129,   130,   131,   598,   132,   599,   854,   133,   203,
     135,   391,   392,   607,   136,   260,   137,   138,   616,   617,
     204,   140,   141,   142,   143,   562,   809,   989,   144,   145,
     805,   984,   278,   279,   146,   147,   148,   149,   399,   888,
     150,   151,   624,  1065,  1066,   625,   152,   205,  1213,   154,
     155,   306,   307,   308,   973,   156,   320,   554,   802,   157,
     158,  1164,   159,   160,   681,   682,   846,   847,   848,   977,
     904,   403,   627,  1073,   628,   629,   310,   544,   425,   407,
     416,   902,  1227,  1228,   161,  1071,  1160,  1161,  1162,   162,
     163,   427,   428,   687,   164,   165,   166,   223,   537,   272,
     273,   522,   523,   784,   312,   900,   635,   636,   526,   313,
     799,   800,   350,   609,   610,   868,   352,   353,   501,   502,
     503,   206,   168,   169,   393,   170,   394,   171,   207,   173,
     215,   216,   757,   758,   759,   174,   208,   209,   177,   178,
     179,   321,   180,   383,   504,   211,   182,   212,   213,   185,
     186,   187,   188,   612,   189,   190,   191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   103,   304,   430,   622,   369,   323,   134,   683,   803,
     500,   890,   480,   824,   309,   311,   845,   516,   761,   777,
     899,   496,   896,   167,   381,   340,   863,   869,   978,  1063,
     134,   153,   878,   882,   227,   255,  1019,   872,   801,   596,
     354,  1015,   538,   971,   524,  1025,   500,   267,   268,   269,
     908,   491,   909,   804,   153,   489,   876,   274,   630,   527,
    -328,  1069,  1068,  1101,   507,   217,   214,   524,   274,   384,
     781,   500,   167,  1143,   315,   358,   167,   167,   553,   134,
     524,   559,   276,   134,   134,   487,   387,   388,   613,  -274,
     866,   633,   514,   214,     3,   293,   874,   214,   167,   778,
     871,  1215,   167,   153,   274,   134,   325,   153,   153,   134,
     500,  -526,   274,  -526,  -275,   295,   218,  1070,  1224,   297,
     384,   862,  -524,  1224,   875,  -349,   797,  -296,   385,   153,
    1216,   363,   541,   153,   695,   697,   386,   387,   388,  1104,
    -526,   194,   362,   214,   298,   384,   440,   299,  1135,    58,
     214,  -524,  1185,   385,   446,   214,   400,   384,   528,   300,
     214,   386,   387,   388,    82,  1181,  1063,  -526,  -296,   498,
     426,   531,   964,   386,   387,   388,   542,   302,   264,   898,
     737,   739,   303,  -257,  -526,   864,   744,   499,   525,  -526,
    -526,   923,  1097,   363,   363,  -524,  -526,  1179,   950,  1102,
     795,  -324,  1151,  1152,   229,   363,   486,  -524,  -258,   172,
     363,   525,  -526,   167,   389,  -526,  1127,   578,   488,   705,
     134,   492,  -296,  1105,   525,  -324,   498,   711,   990,   265,
    -526,  1124,   172,   735,   736,   634,   869,   878,   882,   389,
     626,   363,  -264,   922,   153,  1156,  1156,  -263,  -526,   363,
    1225,   389,   367,   852,  -524,   363,   684,   214,  1018,  -524,
     363,   262,   500,   500,   263,   505,   506,   167,  1063,   390,
     198,   282,   167,   595,   134,   818,   631,   521,   572,   134,
     981,   172,   637,   498,   865,   172,   172,   975,   930,   293,
     865,  1211,  1036,   363,   577,   395,  1157,  1157,   153,   500,
     214,   550,   866,   153,   573,   -70,   390,   172,   866,   295,
     806,   172,   867,   297,   530,  1174,   532,   548,   924,   709,
     632,   982,  1190,   779,   194,   -70,   638,   498,   373,   224,
     565,   283,   931,  1180,   762,   792,  1158,  1087,   298,   500,
     568,   299,   304,    58,   -70,  1159,  1159,   167,   567,   780,
     284,   334,   337,   300,   134,   991,   705,   892,  1103,   175,
      23,   167,   500,   557,   569,   285,   255,   374,   134,  1020,
     167,   302,   -70,   706,   -69,   983,   303,   134,   153,   883,
     222,   893,   175,   167,   885,   894,   707,  1056,   229,  1039,
     134,   886,   153,  1187,   -69,   375,   404,   855,   608,  1040,
     932,   153,   615,   688,   618,   884,   619,   198,   623,   730,
     731,   895,    56,   -69,   153,   732,   708,   405,   733,   734,
     766,   689,   172,   685,    65,   824,   933,   406,   257,  1085,
     528,   175,   680,   827,  -416,   175,   175,  1118,   779,   779,
     783,   -69,   993,  1166,  1167,  1217,   961,    84,   691,   831,
    -272,   828,  1136,  1138,  1153,   528,   709,   175,   761,   436,
      37,   175,   710,  -335,   962,   974,   976,   832,   994,   176,
    1145,  1148,  1218,   948,  1219,  1220,   172,  1221,  1222,  1039,
     766,   172,   498,   766,  -335,  1020,   304,    54,  -262,  1043,
     683,  1062,   176,   265,  -335,   766,   743,   746,   309,   311,
    -611,  -259,  -610,  1021,   266,   167,  1079,   167,  1133,  1149,
     814,   814,   134,   528,   134,  1039,   483,   369,   437,   597,
     484,  1173,   438,   500,  -266,  1139,  -611,   521,  -610,   289,
    1020,  1100,  1136,  1138,  1145,  1148,   153,   688,   153,   528,
     788,   176,  1214,  1212,  1189,   176,   176,   500,  1122,   436,
     794,   481,   194,   482,   521,  1129,   172,  1130,   825,   947,
    1020,  1033,  -751,   484,   335,   335,  -751,   176,   440,   441,
     172,   176,   175,   444,   290,   445,   446,  1051,  1239,   172,
     449,  -270,  -598,   167,   567,   817,  -673,   456,  -673,  -672,
     134,  -672,   172,   460,   461,   462,  -752,  -267,   836,  -261,
    -752,  1252,  1253,  1254,  1255,  1256,  1257,   328,   437,   857,
     996,   711,   438,   440,   153,   539,   540,   870,   444,  1226,
    1080,   446,  -265,  -753,  1083,   449,   175,  -753,  1062,   623,
     970,   175,  -748,   329,  -271,   436,  -748,  -672,  -273,  -672,
     858,   859,  -260,   695,   737,  -669,  -671,  -669,  -671,   436,
     877,   881,  -670,   912,  -670,   913,  -268,     7,   440,   441,
     370,   442,   443,   444,  1150,   445,   446,   500,  1226,  1154,
     449,   813,   816,   372,   398,   397,   455,   456,   401,   402,
     459,  -595,   176,   460,   461,   462,   431,  1113,  -594,  1115,
     432,   436,  -599,  1027,   437,   426,  -600,   490,   438,  -597,
    -596,   494,   374,   949,   497,   518,   175,   363,   437,   500,
     517,   545,   438,   546,   172,   534,   172,   549,   535,   555,
     175,   558,   966,   564,   574,   571,   575,   580,   581,   175,
    1062,   591,   602,   620,   686,   690,   176,   446,  1076,   729,
     742,   176,   175,   766,   440,   764,   765,   767,   768,   444,
     437,   775,   446,   770,   438,   773,   449,   945,   440,   441,
     776,   442,   443,   444,   783,   445,   446,   447,   167,   808,
     449,   167,   798,   793,   500,   134,   455,   456,   134,   807,
     459,   853,   960,   460,   461,   462,   521,   856,   498,   901,
     891,   860,   172,   903,   463,   436,   967,  -139,   889,   153,
     440,   441,   153,   442,   443,   444,  -160,   445,   446,  -167,
    -166,  -165,   449,   921,   914,   988,   176,   906,   915,   456,
    -162,  -140,  -169,  -170,  -164,   460,   461,   462,  1061,  -168,
     176,  -141,   916,   925,   304,  -163,  -171,   907,   928,   176,
     953,   955,   917,   304,   918,   972,   919,  1020,  1094,   980,
    1177,  1178,   176,   226,   437,   877,   881,  1023,   438,  1029,
     167,   426,   920,   929,   175,   167,   175,   134,  1030,   985,
    1032,  1024,   134,  1041,   167,  1042,  1057,   167,  1078,   500,
     500,   134,  1077,  1098,   134,  1099,  1095,  1096,  1111,  1123,
     615,   153,   894,  1171,  1168,  1210,   153,  1175,  1165,  1183,
    1235,  1240,   330,  1264,   440,   153,  1197,  1201,   153,   444,
    1236,   445,   446,  1265,   680,  1048,   449,   293,  1216,  1053,
     364,   820,  1117,   456,   995,   585,   225,   579,   515,   460,
     461,   462,   357,   563,   603,  1110,   787,   295,   796,  1054,
    1067,   297,   175,   600,  1088,  1089,  1075,  1072,   543,  1249,
    1155,   167,  1258,   167,  1251,  1026,   167,   369,   134,   378,
     134,   786,   789,   134,   911,  1061,   298,   963,   221,   299,
    1086,   356,     0,   514,   176,     0,   176,   172,     0,   167,
     172,   300,   153,     0,   153,     0,  1108,   153,     0,   167,
    1109,     0,     0,   988,     0,   408,   134,     0,     0,   302,
       0,     0,     0,   436,   303,     0,     0,     0,   181,     0,
     153,     0,     0,     0,     0,   409,     0,     0,     0,   410,
     153,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     167,   181,     0,     0,     0,     0,     0,   134,     0,   167,
       0,     0,     0,   493,   411,   369,   134,   412,     0,   369,
       0,     0,   176,     0,     0,     0,     0,     0,     0,   413,
       0,   153,   437,     0,     0,     0,   438,  1061,     0,   172,
     153,     0,   623,   514,   172,   514,     0,   414,   514,   514,
     181,     0,   415,   172,   181,   181,   172,     0,  1144,  1147,
       0,     0,   167,     0,     0,     0,     0,   519,     0,   134,
       0,     0,   529,     0,     0,     0,   181,     0,     0,     0,
     181,     0,   440,   441,     0,     0,   443,   444,     0,   445,
     446,     0,     0,   153,   449,   417,     0,   175,     0,     0,
     175,   456,     0,   167,     0,   167,     0,   460,   461,   462,
     134,   167,   134,     0,   167,   418,  1182,     0,   134,   419,
       0,   134,  1144,  1147,     0,     0,     0,     0,     0,     0,
     172,   623,   172,     0,   153,   172,   153,     0,     0,  1193,
       0,     0,   153,   167,   420,   153,     0,   421,     0,   167,
     134,     0,  1206,   167,   167,  1209,   134,     0,   172,   422,
     134,   134,     0,     0,     0,   167,     0,   436,   172,   167,
       0,   167,   134,     0,   153,   183,   134,   423,   134,     0,
     153,     0,   424,   601,   153,   153,     0,     0,     0,   175,
       0,   181,   167,  1259,   175,     0,   153,     0,   183,   134,
     153,     0,   153,   175,     0,   293,   175,   176,     0,   172,
     176,     0,     0,     0,     0,     0,     0,     0,   172,     0,
       0,     0,     0,   153,     0,   295,   437,     0,     0,   297,
     438,     0,     0,     0,   167,   167,   167,   167,     0,     0,
       0,   134,   134,   134,   134,   181,     0,   183,     0,     0,
     181,   183,   183,     0,   298,     0,     0,   299,     0,    58,
       0,     0,     0,     0,     0,   153,   153,   153,   153,   300,
       0,   172,     0,   183,     0,     0,   440,   183,     0,     0,
     175,   444,   175,   445,   446,   175,     0,   302,   449,     0,
       0,     0,   303,     0,     0,   456,     0,     0,     0,   176,
       0,     0,     0,   462,   176,   769,     0,   771,   175,     0,
       0,     0,   172,   176,   172,     0,   176,     0,   175,     0,
     172,     0,     0,   172,     0,   181,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   181,
       0,     0,     0,     0,     0,     0,     0,     0,   181,     0,
       0,     0,   172,   184,     0,     0,     0,     0,   172,   175,
       0,   181,   172,   172,     0,     0,     0,     0,   175,     0,
       0,     0,     0,     0,   172,     0,   184,     0,   172,     0,
     172,     0,     7,     0,     0,     0,     0,     0,   183,     0,
     176,     0,   176,     0,     0,   176,     0,     0,     0,   358,
       0,   172,     0,    22,    23,     0,     0,     0,     0,     0,
       0,     0,   359,     0,    29,   360,     0,     0,   176,     0,
      32,   175,     0,     0,     0,   184,     0,    37,   176,   184,
     184,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   183,   172,   172,   172,   172,   183,     0,   361,
       0,   184,     0,     0,    54,   184,    56,     0,    58,     0,
    1058,     0,   175,  1059,   175,     0,   362,     0,    65,   176,
     175,     0,     0,   175,     0,     0,     0,     0,   176,     0,
       0,     0,     0,   181,     0,   181,    80,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   175,     0,   139,     0,     0,     0,   175,     0,
       0,     0,   175,   175,     0,     0,     0,     0,     0,     0,
       0,     0,   183,     0,   175,     0,     0,   139,   175,     0,
     175,   176,     0,     0,     0,     0,   183,     0,     0,     0,
       0,    98,     0,     0,     0,   183,     0,  1060,     0,     0,
       0,   175,     0,     0,     0,     0,     0,     0,   183,     0,
       0,   181,     0,     0,     0,     0,   184,     0,   954,     0,
       0,   957,   176,     0,   176,     0,   139,     0,     0,     0,
     176,     0,     0,   176,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   175,   175,   175,   175,     0,     0,     0,
       0,     0,   139,     0,     0,   887,   139,     0,     0,  -315,
       0,     0,   176,     0,     0,     0,     0,     0,   176,     0,
     184,     0,   176,   176,     0,   184,  -315,     0,     0,     0,
    -315,  -315,     0,     0,   176,     0,     0,     0,   176,  -315,
     176,  -315,  -315,     0,     0,     0,     0,  -315,     0,     0,
       0,     0,     0,     0,  -315,     0,     0,  -315,     0,     0,
    1031,   176,     0,     0,     0,  1037,     0,     0,     0,     0,
       0,     0,     0,     0,   529,     0,  -315,   529,  -315,     0,
     183,  -315,   183,  -315,     0,  -315,     0,  -315,  -315,     0,
    -315,     0,     0,  -315,     0,  -315,     0,     0,     0,     0,
     184,     0,     0,   176,   176,   176,   176,     0,     0,     0,
       0,     0,     0,  -315,   184,  -315,     0,   139,  -315,     0,
       0,     0,     0,   184,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   184,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   181,     0,   429,   181,
     230,  1090,     0,  1091,     0,     0,  1093,     0,   183,     0,
       0,     0,     0,     0,   231,   232,     0,   233,  -315,     0,
       0,   139,   234,     0,  -315,     0,   139,     0,     0,  1107,
     235,     0,     0,     0,     0,     0,   236,     0,     0,  1037,
       0,     0,   237,     0,     0,     0,   238,     0,     0,   239,
       0,   240,     0,     0,     0,     0,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,   242,   243,     0,
       0,     0,     0,     0,     0,   244,     0,     0,     0,     0,
    1134,     0,     0,     0,     0,   245,     0,     0,   181,  1140,
       0,     0,     0,   181,   246,     0,   247,     0,   248,     0,
     249,   139,   181,   250,     0,   181,     0,   251,   184,   252,
     184,     0,   253,     0,     0,   139,     0,     0,     0,     0,
       0,     0,     0,     0,   139,     0,     0,     0,     0,     0,
       0,     0,     7,     0,     0,     0,     0,   139,     0,     0,
       0,     0,  1176,     0,     0,     0,     0,     0,     0,   358,
       0,     0,     0,    22,    23,     0,     0,     0,     0,     0,
       0,     0,   359,     0,    29,   360,     0,     0,   270,     0,
      32,     0,     0,     0,     0,     0,     0,    37,     0,   181,
       0,   181,     0,  1195,   181,  1199,   184,     0,     0,     0,
       0,   529,     0,   183,   529,     0,   183,     0,     0,   361,
       0,     0,     0,     0,    54,     0,    56,   181,    58,     0,
    1058,     0,     0,  1059,     0,     0,   362,   181,    65,     0,
       0,     0,     0,  1234,     0,     0,     0,     0,     0,  1237,
       0,     0,     0,  1195,  1199,     0,    80,     0,    82,     0,
       0,    84,     0,     0,     0,  1243,   319,     0,     0,  1245,
       0,   771,     0,     0,     0,     0,     0,     0,   181,   139,
       0,   139,     0,     0,     0,   319,     0,   181,     0,     0,
       0,     0,     0,   358,     0,     0,     0,    22,    23,     0,
     319,     0,     0,     0,     0,   183,   359,     0,    29,   360,
     183,    98,     0,     0,    32,     0,     0,  1128,     0,   183,
       0,    37,   183,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   771,   771,   771,   771,     0,     0,
     181,     0,     0,   361,   705,     0,     0,     0,    54,     0,
      56,     0,     0,     0,     0,     0,     0,   139,   934,     0,
     362,   935,    65,     0,     0,     0,   936,     0,     0,     0,
       0,     0,     0,     0,     0,   275,   196,     0,     0,     0,
      80,   181,    82,   181,     0,    84,   937,     0,     0,   181,
       0,   184,   181,   938,   184,   198,   183,     0,   183,   275,
       0,   183,     0,     0,   939,     0,     0,     0,   336,   338,
       0,     0,   940,     0,     0,     0,     0,     0,     0,     0,
       0,   181,     0,     0,   183,     0,     0,   181,   319,   941,
       0,   181,   181,     0,   183,     0,     0,     0,     0,   368,
     942,     0,     0,   181,   709,     0,     0,   181,     0,   181,
       0,   943,     0,     0,     0,     0,   944,     0,     0,     0,
       7,     0,     0,     0,   319,     0,     0,   396,     0,     0,
     181,     0,     0,     0,     0,   183,     0,   358,     0,     0,
       0,    22,    23,   184,   183,     0,     0,     0,   184,   319,
     359,     0,    29,   360,     0,     0,     0,   184,    32,     0,
     184,     0,     0,     0,     0,    37,     0,     0,     0,     0,
       0,     0,   181,   181,   181,   181,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   361,   319,     0,
       0,     0,    54,     0,    56,     0,    58,   183,  1058,     0,
       0,  1059,   139,     0,   362,   139,    65,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    82,     0,     0,    84,
       0,     0,     0,     0,   184,     0,   184,     0,   183,   184,
     183,     0,     0,     0,     0,     0,   183,     0,     0,   183,
     508,   513,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   184,     0,     0,     0,     0,   275,     0,   275,
       0,     0,   184,     0,     0,     0,   275,     0,   183,    98,
       0,     0,     0,     0,   183,  1192,     0,     0,   183,   183,
       0,     0,     0,     0,   139,     0,     0,     0,     0,   139,
     183,     0,     0,     0,   183,     0,   183,     0,   139,     0,
       0,   139,     0,   184,     0,     0,   275,     0,     0,     0,
       0,     0,   184,     0,     0,     0,     0,   183,     0,   319,
     319,     0,     0,     0,     0,   319,     0,     0,   319,   319,
     319,   319,   464,   465,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,   476,   477,   478,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   183,
     183,   183,   183,     0,     0,   184,     0,   319,     0,     0,
       0,     0,     0,     0,     0,   139,   479,   139,     0,  -669,
     139,  -669,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   139,     0,     0,   184,   319,   184,     0,
       0,     0,     0,   139,   184,     0,     0,   184,     0,     0,
     319,   319,     0,     0,     0,     0,     0,     0,     0,     0,
     319,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   210,   184,     0,     0,   220,
       0,     0,   184,     0,   139,     0,   184,   184,   228,     0,
       0,     0,     0,   139,     0,   261,     0,     0,   184,     0,
       0,     0,   184,     0,   184,     0,     0,   277,     0,   280,
     281,     0,     0,     0,     0,   286,     0,   287,     0,   288,
       0,     0,     0,     0,   314,   184,     0,     0,   228,   322,
     324,   326,   327,     0,     0,     0,   331,     0,   332,     0,
       0,     0,     0,     0,     0,   339,   139,     0,   322,     0,
     341,     0,   342,     0,     0,   343,   344,     0,   345,     0,
       0,     0,   228,   322,   355,     0,     0,   184,   184,   184,
     184,   275,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   139,     0,   139,
       0,   379,     0,   382,     0,   139,     0,     0,   139,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   275,   275,     0,     0,     0,     0,     0,
       0,   319,     0,   275,   275,     0,     0,   139,     0,     0,
       0,     0,     0,   139,     0,     0,   433,   139,   139,     0,
       0,     0,     0,   319,     0,   319,     0,     0,     0,   139,
       0,     0,     0,   139,     0,   139,     0,     0,     0,   485,
       0,     0,   319,   331,   228,     7,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   139,     0,     0,     0,
       0,     0,   358,     0,     0,     0,    22,    23,     0,     0,
       0,   322,     0,     0,     0,   359,     0,    29,   360,     0,
       0,     0,     0,    32,     0,     0,     0,     0,   230,     0,
      37,     0,     0,     0,     0,     0,     0,     0,   139,   139,
     139,   139,   231,   232,     0,   233,     0,   322,     0,     0,
     234,     0,   361,     0,     0,     0,     0,    54,   235,    56,
       0,   356,     0,  1058,   236,     0,  1059,     0,     0,   362,
     237,    65,   322,     0,   238,     0,     0,   239,     0,   240,
       0,     0,     0,   331,     0,   319,     0,     0,   241,    80,
       0,    82,     0,     0,    84,   242,   243,     0,   551,   552,
       0,     0,     0,   244,   959,     0,     0,     0,     0,     0,
       0,   322,     0,   245,     0,   561,     0,     0,     0,     0,
       0,     0,   246,     0,   247,     0,   248,   319,   249,     0,
       0,   250,     0,   570,     0,   251,     0,   252,   987,     0,
     253,     0,     0,   576,    98,     0,     0,     0,   275,   275,
       0,     0,     0,     0,     0,   588,     0,   590,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   604,   611,   614,     0,     0,     0,     0,     0,     0,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   319,     0,   363,     0,   986,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   275,     0,
       0,     0,   275,     0,  1055,     0,   692,   693,   694,   696,
     698,   699,     0,   700,   701,   702,   703,   704,     0,   712,
     713,   714,   715,   716,   717,   718,   719,   720,   721,   722,
     723,   724,   725,   726,   727,   728,     0,     0,     0,     0,
       0,     0,   322,   322,     0,     0,     0,     0,   322,     0,
       0,   322,   322,   322,   322,   738,   740,     0,     0,     0,
     611,   745,     0,     0,   760,     0,     0,     0,     0,     0,
     763,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   772,     0,     0,     0,     0,   774,   319,   319,     0,
     322,     0,     0,     0,     0,     0,     0,   782,     0,   785,
       0,     0,     0,   280,     0,     0,   790,   791,     0,  1112,
       0,     0,     0,     0,     0,     0,     0,     0,   314,     0,
       0,   314,     0,     0,     0,     0,     0,     0,     0,     0,
     322,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     763,     0,     0,   322,   322,     0,     0,     0,     0,   228,
     819,     0,     0,   322,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   849,   849,     0,     0,     0,     0,
       0,   275,   275,     0,     0,     0,     0,     0,   861,   230,
       0,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   231,   232,     0,   233,     0,     0,     0,
       0,   234,     0,     0,     0,     0,   314,     0,     0,   235,
       0,     0,     0,     0,     0,   236,     0,     0,     0,     0,
       0,   237,     0,     0,     0,   238,     0,     0,   239,   275,
     240,     0,     0,     0,     0,   275,   275,     0,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,     0,     0,
       0,   905,   275,     0,   244,     0,     0,     0,   910,     0,
       0,     0,   513,     0,   245,   275,     0,     0,   275,     0,
       0,     0,     0,   246,     0,   247,     0,   248,     0,   249,
       0,     0,   250,     0,     0,     7,   251,     0,   252,     0,
       0,   253,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   358,  1238,     0,     0,    22,    23,     0,     0,
       0,     0,     0,   772,   322,   359,     0,    29,   360,     0,
       0,     0,     0,    32,     0,     0,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,   322,     0,   322,   951,
     952,     0,     0,     0,     0,     0,     0,   270,     0,     0,
       0,     0,   361,     0,     0,   322,     0,    54,   436,    56,
       0,     0,   513,  1058,   513,     0,  1059,   513,   513,   362,
     314,    65,     0,     0,   979,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    80,
       0,    82,     0,     0,    84,     0,     0,     0,     0,   588,
       0,     0,     0,  1016,     0,  1017,   849,   849,     0,   341,
       0,   342,     0,     0,   344,   345,     0,   437,     0,     0,
       0,   438,     0,     0,     0,     0,     0,     0,  1028,     0,
       0,     0,   228,     0,     0,     0,  1034,   763,     0,  1038,
       0,     0,     0,     0,    98,  1044,  1046,     0,  1047,     0,
     439,  1050,  1052,     0,     0,     0,     0,     0,   322,     0,
       0,     0,     0,  1064,     0,     0,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,     0,   453,   454,   455,   456,   457,   458,   459,
       0,     0,   460,   461,   462,  1081,  1038,  1047,  1050,  1084,
     322,     0,     0,   463,   760,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   696,   738,
       0,     0,     0,     0,     0,     0,     0,     0,  1092,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   230,     0,
       0,     0,     0,     0,   314,     0,     0,     0,  1106,     0,
       0,     0,   231,   232,   228,   233,     0,     0,     0,     0,
     234,     0,  1114,     0,  1116,   322,     0,     0,   235,     0,
       0,     0,     0,     0,   236,     0,     0,     0,     0,     0,
     237,     0,     0,     0,   238,   849,     0,   239,     0,   240,
       0,     0,  1125,     0,  1126,     0,     0,     0,   241,     0,
       0,  1132,     0,     0,     0,   242,   243,     0,     0,     0,
       0,     0,     0,   244,   763,     0,     0,     0,     0,     0,
       0,  1142,     0,   245,     0,     0,     0,     0,     0,     0,
       0,     0,   246,     0,   247,     0,   248,     0,   249,     0,
       0,   250,  1163,     0,     0,   251,     0,   252,     0,     0,
     253,     0,   230,     0,  1170,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   231,   232,     0,   233,
     322,   322,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,  1184,     0,     0,     0,     0,   236,     0,
       0,  1188,   849,   849,   237,     0,     0,     0,   238,     0,
       0,   239,     0,   240,     0,     0,   520,  1196,     0,  1200,
       0,     0,   241,  1202,     0,  1205,     0,     0,  1208,   242,
     243,     0,     0,     0,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   245,     0,     0,
    1196,  1200,     0,  1205,  1208,     0,   246,     0,   247,     0,
     248,     0,   249,     0,     0,   250,     0,     0,     0,   251,
       0,   252,     0,  1241,   253,     0,  1242,     0,     0,     0,
    1244,     0,     0,     0,     0,  1246,  1247,     0,     0,  1248,
       0,     0,     0,     0,     0,     0,     0,  1250,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1260,  1261,  1246,  1262,  1263,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     547,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1274,  1275,
    1276,  1277,  1274,  1275,  1276,  1277,    -2,     4,     0,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -209,    21,    22,    23,    24,    25,    26,     0,  -209,    27,
       0,    28,     0,    29,    30,    31,  -209,  -209,  -209,    32,
      33,    34,   -68,     0,    35,    36,    37,     0,    38,  -335,
      39,     0,    40,    41,    42,    43,    44,     0,    45,    46,
      47,    48,   -68,    49,    50,     0,    51,    52,    53,     0,
    -335,     0,     0,    54,    55,    56,    57,    58,    59,    60,
    -335,   -68,    61,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,     0,    75,
      76,     0,    77,    78,    79,    80,    81,    82,    83,   -68,
      84,    85,     0,     0,  -209,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
      98,     0,     0,    99,     0,   100,     0,   101,     0,  -209,
       0,     4,   102,     5,     6,     7,     8,     9,    10,    11,
      12,     0,  -699,     0,    13,    14,    15,    16,    17,  -699,
      18,     0,    19,    20,  -209,    21,    22,    23,    24,    25,
      26,  -699,  -209,    27,  -699,    28,     0,    29,    30,    31,
    -209,  -209,  -209,    32,    33,    34,   -68,     0,    35,    36,
      37,     0,    38,  -335,    39,     0,    40,    41,    42,    43,
      44,     0,    45,    46,    47,    48,   -68,    49,    50,     0,
      51,    52,    53,     0,  -335,     0,     0,    54,    55,    56,
      57,     0,    59,    60,  -335,   -68,    61,    62,    63,    64,
    -699,    65,    66,    67,  -699,    68,    69,    70,    71,    72,
      73,    74,     0,    75,    76,     0,    77,    78,    79,    80,
      81,    82,    83,   -68,    84,    85,     0,     0,  -209,     0,
      86,     0,     0,  -699,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,  -699,    90,  -699,  -699,  -699,  -699,  -699,  -699,  -699,
       0,  -699,  -699,  -699,  -699,  -699,  -699,  -699,  -699,  -699,
    -699,  -699,  -699,    97,    98,  -699,  -699,  -699,     0,   100,
    -699,   101,     0,  -209,     0,   346,  -699,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -209,    21,
      22,    23,    24,    25,    26,     0,  -209,    27,     0,    28,
       0,    29,    30,    31,  -209,  -209,  -209,    32,    33,    34,
     -68,     0,    35,    36,    37,     0,    38,  -335,    39,     0,
      40,    41,    42,    43,    44,     0,    45,    46,    47,    48,
     -68,    49,    50,     0,    51,    52,    53,     0,  -335,     0,
       0,    54,    55,    56,    57,    58,    59,    60,  -335,   -68,
      61,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,    82,    83,   -68,    84,    85,
       0,     0,  -209,     0,    86,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,    98,     0,
       0,    99,     0,   100,   347,   101,     0,  -209,     0,     4,
     102,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,  -209,    21,    22,    23,    24,    25,    26,     0,
    -209,    27,     0,    28,     0,    29,    30,    31,  -209,  -209,
    -209,    32,    33,    34,   -68,     0,    35,    36,    37,     0,
      38,  -335,    39,     0,    40,    41,    42,    43,    44,     0,
      45,    46,    47,    48,   -68,    49,    50,     0,    51,    52,
      53,     0,  -335,     0,     0,    54,    55,    56,    57,    58,
      59,    60,  -335,   -68,    61,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
       0,    75,    76,     0,    77,    78,    79,    80,    81,    82,
      83,   -68,    84,    85,     0,     0,  -209,     0,    86,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,    98,     0,     0,    99,     0,   100,   566,   101,
       0,  -209,     0,   583,   102,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,  -209,    21,    22,    23,
      24,    25,    26,     0,  -209,    27,     0,    28,     0,    29,
      30,    31,  -209,  -209,  -209,    32,    33,    34,   -68,     0,
      35,    36,    37,     0,    38,  -335,    39,     0,    40,    41,
      42,    43,    44,     0,    45,    46,    47,    48,   -68,    49,
      50,     0,    51,    52,    53,     0,  -335,     0,     0,    54,
      55,    56,    57,    58,    59,    60,  -335,   -68,    61,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,     0,    75,    76,     0,    77,    78,
      79,    80,    81,    82,    83,   -68,    84,    85,     0,     0,
    -209,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,    98,     0,     0,    99,
       0,   100,   584,   101,     0,  -209,     0,     4,   102,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -209,    21,    22,    23,    24,    25,    26,     0,  -209,    27,
       0,    28,     0,    29,    30,    31,  -209,  -209,  -209,    32,
      33,    34,   -68,     0,    35,    36,    37,     0,    38,  -335,
      39,     0,    40,    41,    42,    43,    44,     0,    45,    46,
      47,    48,   -68,    49,    50,     0,    51,    52,    53,     0,
    -335,     0,     0,    54,    55,    56,    57,    58,    59,    60,
    -335,   -68,    61,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,     0,    75,
      76,     0,    77,    78,    79,    80,    81,    82,    83,   -68,
      84,    85,     0,     0,  -209,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
      98,     0,     0,    99,     0,   100,   826,   101,     0,  -209,
       0,     4,   102,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -209,    21,    22,    23,    24,    25,
      26,     0,  -209,    27,     0,    28,     0,    29,    30,    31,
    -209,  -209,  -209,    32,    33,    34,   -68,     0,    35,    36,
      37,     0,    38,  -335,    39,     0,    40,    41,    42,    43,
      44,     0,    45,    46,    47,    48,   -68,    49,    50,     0,
      51,    52,    53,     0,  -335,     0,     0,    54,    55,    56,
      57,   356,    59,    60,  -335,   -68,    61,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,     0,    75,    76,     0,    77,    78,    79,    80,
      81,    82,    83,   -68,    84,    85,     0,     0,  -209,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,    98,     0,     0,    99,     0,   100,
       0,   101,     0,  -209,     0,     4,   102,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -209,    21,
      22,    23,    24,    25,    26,     0,  -209,    27,     0,    28,
       0,    29,    30,    31,  -209,  -209,  -209,    32,    33,    34,
     -68,     0,    35,    36,    37,     0,    38,  -335,    39,     0,
      40,    41,    42,    43,    44,     0,    45,    46,    47,    48,
     -68,    49,    50,     0,    51,    52,    53,     0,  -335,     0,
       0,    54,    55,    56,    57,    58,    59,    60,  -335,   -68,
      61,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,    82,    83,   -68,    84,    85,
       0,     0,  -209,     0,    86,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,    98,     0,
       0,    99,     0,   100,     0,   101,     0,  -209,     0,     4,
     102,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,  -209,    21,    22,    23,    24,    25,    26,     0,
    -209,    27,     0,    28,     0,    29,    30,    31,  -209,  -209,
    -209,    32,    33,    34,   -68,     0,    35,    36,    37,     0,
      38,  -335,    39,     0,    40,    41,    42,    43,    44,     0,
      45,    46,    47,    48,   -68,    49,    50,     0,    51,    52,
      53,     0,  -335,     0,     0,    54,    55,    56,    57,     0,
      59,    60,  -335,   -68,    61,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
       0,    75,    76,     0,    77,    78,    79,    80,    81,    82,
      83,   -68,    84,    85,     0,     0,  -209,     0,    86,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,    98,     0,     0,    99,     0,   100,     0,   101,
       0,  -209,     0,     4,   102,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,  -209,    21,    22,    23,
      24,    25,    26,     0,  -209,    27,     0,    28,     0,    29,
      30,    31,  -209,  -209,  -209,    32,  1049,    34,   -68,     0,
      35,    36,    37,     0,    38,  -335,    39,     0,    40,    41,
      42,    43,    44,     0,    45,    46,    47,    48,   -68,    49,
      50,     0,    51,    52,    53,     0,  -335,     0,     0,    54,
      55,    56,    57,     0,    59,    60,  -335,   -68,    61,    62,
      63,    64,     0,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,     0,    75,    76,     0,    77,    78,
      79,    80,    81,    82,    83,   -68,    84,    85,     0,     0,
    -209,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,    98,     0,     0,    99,
       0,   100,     0,   101,     0,  -209,     0,     4,   102,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -209,    21,    22,    23,    24,    25,    26,     0,  -209,    27,
       0,    28,     0,    29,    30,    31,  -209,  -209,  -209,    32,
    1194,    34,   -68,     0,    35,    36,    37,     0,    38,  -335,
      39,     0,    40,    41,    42,    43,    44,     0,    45,    46,
      47,    48,   -68,    49,    50,     0,    51,    52,    53,     0,
    -335,     0,     0,    54,    55,    56,    57,     0,    59,    60,
    -335,   -68,    61,    62,    63,    64,     0,    65,    66,    67,
       0,    68,    69,    70,    71,    72,    73,    74,     0,    75,
      76,     0,    77,    78,    79,    80,    81,    82,    83,   -68,
      84,    85,     0,     0,  -209,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
      98,     0,     0,    99,     0,   100,     0,   101,     0,  -209,
       0,     4,   102,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -209,    21,    22,    23,    24,    25,
      26,     0,  -209,    27,     0,    28,     0,    29,    30,    31,
    -209,  -209,  -209,    32,  1198,    34,   -68,     0,    35,    36,
      37,     0,    38,  -335,    39,     0,    40,    41,    42,    43,
      44,     0,    45,    46,    47,    48,   -68,    49,    50,     0,
      51,    52,    53,     0,  -335,     0,     0,    54,    55,    56,
      57,     0,    59,    60,  -335,   -68,    61,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,     0,    75,    76,     0,    77,    78,    79,    80,
      81,    82,    83,   -68,    84,    85,     0,     0,  -209,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,    98,     0,     0,    99,     0,   100,
       0,   101,     0,  -209,     0,     4,   102,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -209,    21,
      22,    23,    24,    25,    26,     0,  -209,    27,     0,    28,
       0,    29,    30,    31,  -209,  -209,  -209,    32,  1204,    34,
     -68,     0,    35,    36,    37,     0,    38,  -335,    39,     0,
      40,    41,    42,    43,    44,     0,    45,    46,    47,    48,
     -68,    49,    50,     0,    51,    52,    53,     0,  -335,     0,
       0,    54,    55,    56,    57,     0,    59,    60,  -335,   -68,
      61,    62,    63,    64,     0,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,    82,    83,   -68,    84,    85,
       0,     0,  -209,     0,    86,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,    98,     0,
       0,    99,     0,   100,     0,   101,     0,  -209,     0,     4,
     102,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,  -209,    21,    22,    23,    24,    25,    26,     0,
    -209,    27,     0,    28,     0,    29,    30,    31,  -209,  -209,
    -209,    32,  1207,    34,   -68,     0,    35,    36,    37,     0,
      38,  -335,    39,     0,    40,    41,    42,    43,    44,     0,
      45,    46,    47,    48,   -68,    49,    50,     0,    51,    52,
      53,     0,  -335,     0,     0,    54,    55,    56,    57,     0,
      59,    60,  -335,   -68,    61,    62,    63,    64,     0,    65,
      66,    67,     0,    68,    69,    70,    71,    72,    73,    74,
       0,    75,    76,     0,    77,    78,    79,    80,    81,    82,
      83,   -68,    84,    85,     0,     0,  -209,     0,    86,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,    98,     0,     0,    99,     0,   100,     0,   101,
     747,  -209,     5,     6,   102,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,  -209,    21,     0,   748,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,  -209,
    -209,  -209,     0,   197,    34,     0,   749,     0,    36,     0,
     750,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,   751,     0,    55,   752,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
     753,     0,     0,     0,     0,     0,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,   754,    81,
       0,    83,     0,   755,    85,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   202,     0,
     101,   756,  -209,     0,   897,   102,     5,     6,     0,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,  -209,    21,     0,
       0,     0,     0,     0,     0,     0,   294,     0,   196,     0,
       0,     0,    31,  -209,  -209,  -209,     0,   197,    34,     0,
       0,     0,   296,     0,     0,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,   301,     0,    75,     0,     0,    77,
       0,     0,     0,    81,     0,    83,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   202,     0,   101,     0,  -209,     5,     6,   102,
       8,     9,    10,    11,    12,     0,   641,     0,    13,     0,
       0,    16,    17,   643,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   198,   649,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,    83,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,    87,   510,   465,
     466,   467,   468,   469,   511,   512,   472,   473,   474,   475,
       0,   477,   478,     0,   837,   838,   839,   840,   841,   662,
       0,   663,     0,    93,     0,   664,   665,   666,   667,   668,
     669,   670,   842,   672,   673,    95,   843,    97,   -90,   675,
     676,   844,   678,   202,     0,   101,     0,     0,     5,     6,
     102,     8,     9,    10,    11,    12,     0,   641,     0,    13,
       0,     0,    16,    17,   643,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   198,
     649,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,     0,     0,     0,
       0,     0,     0,    55,     0,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,   201,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,   510,
     465,   466,   467,   468,   469,   511,   512,   472,   473,   474,
     475,     0,   477,   478,     0,   837,   838,   839,   840,   841,
     662,     0,   663,     0,    93,     0,   664,   665,   666,   667,
     668,   669,   670,   842,   672,   673,    95,   843,    97,     0,
     675,   676,   844,   678,   202,     0,   101,     0,     0,     0,
       0,   102,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,     0,    16,    17,     0,    18,
       0,   195,    20,     0,    21,     0,     0,     0,     0,    26,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,    33,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,    50,     0,    51,
      52,   333,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,     0,     0,     0,    81,
       0,    83,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   100,     0,
     101,     0,     0,     5,     6,   102,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,   748,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,   749,     0,    36,
       0,   750,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,   751,     0,    55,   752,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   200,
       0,   753,     0,     0,     0,     0,     0,    70,    71,    72,
      73,   201,     0,    75,     0,     0,    77,     0,     0,   754,
      81,     0,    83,     0,   755,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   202,
       0,   101,     0,     0,     5,     6,   102,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,   377,    23,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,   361,     0,     0,     0,     0,     0,    55,
      56,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,    65,     0,     0,     0,     0,     0,    70,    71,
      72,    73,   201,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,    83,     0,    84,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     202,     0,   101,     0,     0,     5,     6,   102,     8,     9,
      10,   380,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,    19,     0,  -209,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,  -209,  -209,  -209,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,    64,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,   201,     0,    75,     0,     0,    77,     0,
       0,     0,    81,    82,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   202,     0,   101,     0,  -209,     5,     6,   102,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,  -209,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,  -209,  -209,  -209,     0,   197,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,   201,     0,    75,     0,     0,    77,
       0,     0,     0,    81,     0,    83,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   202,     0,   101,     0,  -209,     5,     6,   102,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,  -747,     0,     0,     0,  -747,     0,
       0,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,    83,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
    -377,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,  -377,     0,
       0,    99,     0,   202,  -377,   101,     0,     0,     5,     6,
     102,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   198,
       0,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,     0,     0,     0,
       0,     0,     0,    55,     0,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,   201,     0,    75,     0,
       0,    77,   316,   317,     0,    81,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,   318,
       0,     0,    99,     0,   202,     0,   101,     0,     0,     5,
       6,   102,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,   201,     0,    75,
       0,     0,    77,   316,   317,     0,    81,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,   351,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   202,     0,   101,     0,     0,
       5,     6,   102,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,   197,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,   201,     0,
      75,     0,     0,    77,   316,   317,     0,    81,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   202,     0,   101,  -547,
       0,     5,     6,   102,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,   201,
       0,    75,     0,     0,    77,   316,   317,     0,    81,     0,
      83,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   202,     0,   101,
       0,     0,  -547,     0,   102,     5,     6,     0,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,   201,     0,    75,     0,     0,    77,   316,
     317,     0,    81,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   202,     0,   101,   812,     0,     5,     6,   102,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,     0,     0,     0,     0,   197,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,   201,     0,    75,     0,     0,    77,
     316,   317,     0,    81,     0,    83,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   202,     0,   101,   815,     0,     5,     6,   102,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,   316,   317,     0,    81,     0,    83,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   202,     0,   101,   821,     0,     5,     6,
     102,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   198,
       0,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,   965,    51,    52,     0,     0,     0,
       0,     0,     0,    55,     0,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,   201,     0,    75,     0,
       0,    77,   316,   317,     0,    81,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   202,     0,   101,     0,     0,     5,
       6,   102,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,   201,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,  -385,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
    -385,     0,     0,    99,     0,   202,  -385,   101,     0,     0,
       5,     6,   102,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,   197,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,   201,     0,
      75,     0,     0,    77,     0,     0,     0,    81,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,  -370,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,  -370,     0,     0,    99,     0,   202,  -370,   101,     0,
       0,     5,     6,   102,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,   201,
       0,    75,     0,     0,    77,     0,     0,     0,    81,     0,
      83,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,  -378,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,  -378,     0,     0,    99,     0,   202,  -378,   101,
       0,     0,     5,     6,   102,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,     0,    21,     0,   258,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,   197,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,    83,     0,   259,    85,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   202,     0,
     101,     0,     0,     5,     6,   102,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   200,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,   201,     0,    75,     0,     0,    77,   316,   317,     0,
      81,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   202,
       0,   101,     0,     0,     5,     6,   102,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,  -747,     0,     0,     0,  -747,     0,     0,    70,    71,
      72,    73,   201,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     202,     0,   101,     0,     0,     5,     6,   102,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,   201,     0,    75,     0,     0,    77,     0,
       0,     0,    81,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,   605,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   202,     0,   101,     0,     0,   606,     0,   102,     5,
       6,     0,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,   201,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
     866,     0,     0,    99,     0,   202,     0,   101,     0,     0,
    1035,     0,   102,     5,     6,     0,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   200,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,   201,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,  -408,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   202,
       0,   101,  -408,     0,     5,     6,   102,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,   201,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,  -541,     0,     0,    99,     0,
     202,     0,   101,     0,     0,  -541,     0,   102,     5,     6,
       0,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,   274,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   198,
       0,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,     0,     0,     0,
       0,     0,     0,    55,     0,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,   201,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   100,     0,   101,     0,     0,     5,
       6,   102,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,   201,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,   605,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   202,     0,   101,     0,     0,
       5,     6,   102,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,   197,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,   201,     0,
      75,     0,     0,    77,     0,     0,     0,    81,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   202,     0,   101,     0,
       0,   606,     0,   102,     5,     6,     0,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,   201,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     202,   810,   101,     0,     0,     5,     6,   102,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,   201,     0,    75,     0,     0,    77,     0,
       0,     0,    81,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   202,     0,   101,     0,     0,  1045,     0,   102,     5,
       6,     0,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,   201,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   202,     0,   101,     0,     0,
       5,     6,   102,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
     294,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,   197,    34,     0,     0,     0,   296,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,   301,     0,
      75,     0,     0,    77,     0,     0,     0,    81,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   202,     0,   101,     0,
       0,     5,     6,   102,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,  1082,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,   201,
       0,    75,     0,     0,    77,     0,     0,     0,    81,     0,
      83,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   202,     0,   101,
       0,     0,     5,     6,   102,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,  1229,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,    83,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   202,     0,
     101,     0,     0,     5,     6,   102,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,  1230,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   200,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,   201,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   202,
       0,   101,     0,     0,     5,     6,   102,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,  1232,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,   201,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     202,     0,   101,     0,     0,     5,     6,   102,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,  1233,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,   201,     0,    75,     0,     0,    77,     0,
       0,     0,    81,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     6,     0,     8,     9,    10,    11,    12,     0,
      94,     0,    13,    95,    96,    97,    17,     0,     0,    99,
     195,   202,  -209,   101,     0,   293,     0,     0,   102,     0,
       0,     0,     0,   196,     0,     0,     0,     0,  -209,  -209,
    -209,     0,   197,     0,     0,   295,     0,     0,     0,   297,
       0,     0,     0,     0,   199,     0,     0,    43,    44,     0,
       0,     0,     0,    48,     0,    49,    50,     0,     0,     0,
       0,     0,     0,     0,   298,     0,    55,   299,    57,    58,
      59,     0,     0,     0,     0,    62,     0,   200,     0,   300,
       0,     0,     0,     0,     0,    70,    71,     0,    73,     0,
       0,     0,     0,     0,     0,     0,     0,   302,     0,     0,
      83,     0,   303,     0,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   202,     0,   101,
    -339,  -209,     0,     6,   102,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,     0,    17,     0,     0,
       0,   195,     0,  -209,     0,     0,   293,     0,     0,     0,
       0,     0,     0,     0,   196,     0,     0,     0,     0,  -209,
    -209,  -209,     0,   197,     0,     0,   295,     0,     0,     0,
     297,     0,     0,     0,     0,   199,     0,     0,    43,    44,
       0,     0,     0,     0,    48,     0,    49,    50,     0,     0,
       0,     0,     0,     0,     0,   298,     0,    55,   299,    57,
      58,    59,     0,     0,     0,     0,    62,     0,   200,     0,
     300,     0,     0,     0,     0,     0,    70,    71,     0,    73,
       0,     0,     0,     0,     0,     0,     0,     0,   302,     0,
       0,    83,     0,   303,     0,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   639,     0,   640,     0,    88,
      89,    90,    91,    92,     0,     0,   641,     0,    93,     0,
       0,   642,   232,   643,   644,     0,     0,    94,     0,   645,
      95,    96,    97,     0,     0,     0,    99,   235,   202,   196,
     101,     0,  -209,   236,     0,   102,     0,     0,     0,   646,
       0,     0,     0,   238,     0,     0,   647,     0,   648,   649,
       0,     0,     0,     0,     0,     0,     0,   650,     0,     0,
       0,     0,     0,     0,   242,   651,     0,     0,     0,     0,
       0,     0,   244,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   652,     0,     0,     0,     0,     0,     0,     0,
       0,   246,     0,   653,     0,   248,     0,   654,     0,     0,
     250,     0,     0,     0,   655,     0,   252,     0,     0,   656,
       0,     0,     0,     0,     0,     0,     0,     0,   510,   465,
     466,   467,   468,   469,   511,   512,   472,   473,   474,   475,
       0,   477,   478,     0,   657,   658,   659,   660,   661,   662,
       0,   663,     0,     0,     0,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,     0,   674,     0,     0,   675,
     676,   677,   678,     5,     6,   679,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,  -209,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
    -209,  -209,  -209,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   200,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,   201,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,     0,    96,    97,     0,     0,     0,    99,     0,   202,
       0,   101,     0,  -209,     5,     6,     0,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,  -209,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,  -209,  -209,  -209,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,   201,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    90,     0,     0,     5,     6,     0,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,    97,   195,     0,  -209,    21,     0,
     202,     0,   101,     0,  -209,     0,    27,     0,   196,     0,
       0,     0,    31,  -209,  -209,  -209,     0,   197,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,   201,   705,    75,     0,     0,    77,
       0,     0,     0,    81,   641,    83,     0,     0,    85,   997,
     998,   643,   999,     0,     0,     0,     0,  1000,     0,     0,
       0,     0,     0,     0,     0,   707,     0,     0,     0,     0,
       0,     0,     0,    88,     0,    90,     0,  1001,     0,     0,
       0,  1002,     0,     0,  1003,     0,   198,   649,     0,     0,
       0,     0,     0,     0,     0,   708,    97,     0,     0,     0,
       0,     0,     0,  1004,   101,     0,  -209,     0,     0,     0,
    1005,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1006,     0,     0,     0,     0,     0,     0,     0,     0,  1007,
       0,  1008,     0,  1009,     0,   709,     0,     0,     0,     0,
       0,   710,  1010,     0,  1011,     0,     0,  1012,     0,     0,
       0,     0,     0,     0,     0,     0,   510,   465,   466,   467,
     468,   469,   511,   512,   472,   473,   474,   475,     0,   477,
     478,     0,   657,   658,   659,   660,   661,   662,     0,   663,
       0,     0,     0,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   230,   674,     0,     0,   675,   676,   677,
     678,  1013,   641,     0,     0,     0,     0,   231,   232,   643,
     233,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,   235,     0,     0,     0,     0,     0,   236,
       0,     0,     0,     0,     0,   237,     0,     0,     0,   238,
       0,     0,   239,     0,   240,   649,     0,     0,     0,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
     242,   243,     0,     0,     0,     0,     0,     0,   244,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   245,     0,
       0,     0,     0,     0,     0,   434,     0,   246,     0,   247,
       0,   248,   435,   249,     0,     0,   250,     0,     0,     0,
     251,     0,   252,     0,   436,   253,     0,     0,     0,     0,
       0,     0,     0,     0,   510,   465,   466,   467,   468,   469,
     511,   512,   472,   473,   474,   475,     0,   477,   478,     0,
     657,   658,   659,   660,   661,   662,     0,   663,     0,     0,
       0,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,     0,   674,     0,     0,   675,   676,   677,   678,     0,
       0,     0,     0,   437,     0,   434,     0,   438,     0,     0,
       0,     0,   435,     0,     0,   509,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,   510,   465,   466,
     467,   468,   469,   511,   512,   472,   473,   474,   475,     0,
     477,   478,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,     0,   460,   461,
     462,     0,   363,   437,     0,   434,     0,   438,     0,   463,
       0,     0,   435,     0,     0,  1203,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,   510,   465,   466,
     467,   468,   469,   511,   512,   472,   473,   474,   475,     0,
     477,   478,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,     0,   460,   461,
     462,     0,   363,   437,     0,   434,     0,   438,     0,   463,
       0,     0,   435,     0,     0,  1266,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,   510,   465,   466,
     467,   468,   469,   511,   512,   472,   473,   474,   475,     0,
     477,   478,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,     0,   460,   461,
     462,     0,   363,   437,     0,   434,     0,   438,     0,   463,
       0,     0,   435,     0,     0,  1267,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,   510,   465,   466,
     467,   468,   469,   511,   512,   472,   473,   474,   475,     0,
     477,   478,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,     0,   460,   461,
     462,     0,   363,   437,     0,   434,     0,   438,     0,   463,
       0,     0,   435,     0,     0,  1268,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,   510,   465,   466,
     467,   468,   469,   511,   512,   472,   473,   474,   475,     0,
     477,   478,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,     0,   460,   461,
     462,     0,   363,   437,     0,     0,     0,   438,     0,   463,
       0,     0,     0,     0,   434,  1269,     0,     0,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   879,     0,   439,   510,   465,   466,
     467,   468,   469,   511,   512,   472,   473,   474,   475,   880,
     477,   478,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,     0,   460,   461,
     462,     0,   363,     0,     0,     0,     0,     0,   434,   463,
       0,     0,   437,     0,     0,   435,   438,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,   274,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   214,     0,   992,     0,   439,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,     0,   437,   460,   461,   462,
     438,   363,     0,     0,     0,     0,     0,     0,   463,   434,
       0,     0,     0,     0,     0,     0,   435,     0,     0,     0,
       0,     0,     0,     0,     0,   214,   437,     0,   436,   439,
     438,     0,     0,   592,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,     0,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
     593,   460,   461,   462,     0,   363,   440,   441,     0,   442,
     443,   444,   463,   445,   446,   447,     0,   437,   449,   450,
     451,   438,   453,   454,   455,   456,     0,     0,   459,   434,
     833,   460,   461,   462,     0,     0,   435,     0,     0,     0,
       0,     0,   463,     0,     0,     0,     0,     0,   436,     0,
     439,     0,     0,   834,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     835,   594,   460,   461,   462,     0,     0,     0,     0,     0,
       0,     0,     0,   463,     0,     0,     0,   437,   434,     0,
       0,   438,     0,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,  1146,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
       0,     0,   460,   461,   462,     0,   437,     0,   434,     0,
     438,     0,     0,   463,     0,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,   274,     0,
       0,     0,     0,     0,     0,   214,     0,     0,     0,   439,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,     0,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
       0,   460,   461,   462,     0,   363,   437,     0,   434,     0,
     438,     0,   463,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,   274,     0,
       0,     0,     0,     0,     0,   214,     0,     0,     0,   439,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,     0,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
       0,   460,   461,   462,     0,   363,   437,     0,   434,     0,
     438,     0,   463,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,   926,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   439,
       0,     0,     0,   927,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,     0,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
       0,   460,   461,   462,     0,   363,   437,   434,     0,     0,
     438,     0,   463,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   439,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,     0,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
       0,   460,   461,   462,     0,   437,     0,     0,     0,   438,
       0,     0,   463,     0,     0,     0,     0,   958,   434,     0,
       0,     0,     0,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,   439,     0,
       0,     0,  1120,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,     0,  1121,
     460,   461,   462,     0,   363,     0,     0,     0,     0,     0,
       0,   463,   434,     0,     0,     0,   437,     0,     0,   435,
     438,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   439,
       0,     0,     0,     0,     0,     0,     0,   434,     0,     0,
       0,     0,     0,     0,   435,     0,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   436,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
     437,   460,   461,   462,   438,     0,     0,   434,   533,     0,
       0,     0,   463,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,     0,     0,   214,
       0,     0,     0,   439,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,     0,     0,     0,   438,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,     0,   214,   460,   461,   462,   439,     0,
       0,     0,     0,     0,  1137,   437,   463,     0,     0,   438,
       0,     0,     0,     0,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   439,     0,
     460,   461,   462,     0,     0,     0,   434,     0,     0,  1186,
       0,   463,     0,   435,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   436,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,     0,     0,
     460,   461,   462,     0,     0,     0,   434,     0,     0,     0,
       0,   463,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,     0,   434,     0,   438,     0,
       0,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,   437,     0,     0,     0,   438,     0,
       0,     0,     0,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   439,     0,   460,
     461,   462,     0,   556,   437,   434,     0,     0,   438,     0,
     463,     0,   435,     0,   440,   441,   741,   442,   443,   444,
       0,   445,   446,   447,   436,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   439,   560,   460,
     461,   462,     0,     0,     0,   434,   829,     0,     0,     0,
     463,     0,   435,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   436,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   437,     0,     0,   434,   438,     0,     0,
     463,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,     0,     0,     0,
       0,     0,     0,   437,     0,     0,     0,   438,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   439,     0,   460,   461,
     462,     0,     0,     0,   437,   811,     0,     0,   438,   463,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   830,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   873,   439,   460,   461,
     462,     0,     0,     0,     0,     0,     0,     0,     0,   463,
       0,     0,     0,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   434,     0,   460,
     461,   462,     0,     0,   435,     0,     0,     0,     0,     0,
     463,     0,     0,     0,     0,     0,   436,     0,     0,   956,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   434,     0,     0,
       0,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,     0,   434,     0,   438,
       0,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   439,     0,
       0,     0,     0,     0,     0,   437,     0,     0,     0,   438,
       0,     0,     0,     0,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   439,     0,
     460,   461,   462,     0,     0,   437,   434,  1022,     0,   438,
       0,   463,     0,   435,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   436,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   439,     0,
     460,   461,   462,     0,     0,     0,   434,   968,     0,     0,
       0,   463,     0,   435,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   436,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,     0,     0,
     460,   461,   462,     0,   437,     0,     0,   969,   438,     0,
       0,   463,     0,     0,   434,  1119,     0,     0,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,   437,     0,     0,     0,   438,     0,
       0,     0,     0,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   439,     0,   460,
     461,   462,     0,     0,     0,     0,     0,     0,     0,     0,
     463,     0,   437,     0,   440,   441,   438,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   434,     0,   460,
     461,   462,     0,     0,   435,   439,  1074,     0,     0,     0,
     463,     0,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   434,     0,   460,   461,   462,
       0,     0,   435,     0,     0,     0,     0,     0,   463,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,     0,     0,     0,   438,
       0,     0,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,   439,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,     0,   440,   441,   438,   442,   443,
     444,     0,   445,   446,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,     0,  1131,
     460,   461,   462,     0,     0,  1141,   439,     0,     0,     0,
       0,   463,     0,   437,     0,     0,     0,   438,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   439,     0,   460,   461,
     462,     0,     0,     0,     0,     0,     0,     0,     0,   463,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   434,     0,   460,   461,
     462,     0,     0,   435,     0,     0,   621,  1169,   230,   463,
       7,     0,     0,     0,     0,   436,  1172,     0,     0,     0,
       0,     0,   231,   232,     0,   233,     0,     0,     0,     0,
     234,     0,     0,     0,     0,     0,   434,  1191,   235,     0,
       0,     0,     0,   435,   236,     0,     0,     0,     0,     0,
     237,     0,     0,     0,   238,   436,     0,   239,     0,   240,
       0,     0,     0,     0,     0,     0,     0,     0,   241,     0,
       0,     0,     0,     0,   437,   242,   243,   434,   438,     0,
       0,     0,     0,   244,   435,     0,     0,     0,     0,     0,
       0,     0,     0,   245,     0,     0,   436,     0,     0,     0,
       0,     0,   246,     0,   247,     0,   248,   439,   249,     0,
       0,   250,     0,     0,   437,   251,     0,   252,   438,     0,
     253,     0,     0,     0,   440,   441,  1223,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   439,     0,   460,
     461,   462,     0,     0,     0,   437,     0,     0,     0,   438,
     463,     0,     0,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,   439,   460,
     461,   462,   434,     0,     0,     0,     0,     0,     0,   435,
     463,     0,     0,     0,     0,   440,   441,     0,   442,   443,
     444,   436,   445,   446,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,     0,     0,
     460,   461,   462,     0,     0,     0,     0,     0,     0,     0,
     434,   463,     0,     0,     0,     0,     0,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,     0,     0,     0,   438,     0,     0,     0,     0,     0,
     434,     0,  1231,     0,     0,     0,     0,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,   439,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,     0,
     440,   441,   438,   442,   443,   444,     0,   445,   446,   447,
    1270,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,     0,     0,   460,   461,   462,     0,     0,
       0,   439,     0,     0,     0,     0,   463,     0,   437,   434,
       0,     0,   438,     0,     0,     0,   435,     0,   440,   441,
    1271,   442,   443,   444,     0,   445,   446,   447,   436,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   439,     0,   460,   461,   462,     0,     0,     0,   434,
       0,     0,     0,     0,   463,     0,   435,     0,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,   436,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,     0,     0,   460,   461,   462,     0,   437,     0,   434,
       0,   438,     0,     0,   463,     0,   435,     0,     0,  1272,
       0,     0,     0,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,     0,     0,     0,     0,     0,     0,   437,     0,     0,
       0,   438,     0,     0,     0,     0,     0,   440,   441,  1273,
     442,   443,   444,     0,   445,   446,   447,   436,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     439,     0,   460,   461,   462,     0,     0,   437,   434,     0,
       0,   438,     0,   463,     0,   435,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,   436,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     439,     0,   460,   461,   462,     0,   437,     0,     0,     0,
     438,     0,     0,   463,     0,     0,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
       0,     0,   460,   461,   462,     0,   946,     0,     0,     0,
     438,     0,     0,   463,     0,     0,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,     0,   448,   449,   450,
     451,     0,   453,   454,   455,   456,   457,     0,   459,   439,
       0,   460,   461,   462,     0,     0,     0,     0,     0,     0,
       0,     0,   463,     0,     0,     0,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   436,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
     358,   460,   461,   462,    22,    23,     0,     0,     0,     0,
       0,     0,   463,   359,     0,    29,   360,     0,     0,     0,
       0,    32,     0,     0,     0,     0,     0,     0,    37,     0,
       0,  -335,     0,     0,     0,    41,     0,     0,     0,     0,
       0,     0,     0,     0,   -68,   437,     0,     0,     0,   438,
     361,     0,  -335,     0,     0,    54,     0,    56,     0,     0,
       0,    60,  -335,   -68,    61,     0,     0,   362,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,   439,     0,
       0,     0,     0,     0,     0,     0,     0,    80,     0,    82,
       0,     0,    84,     0,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,     0,   448,   449,   450,   451,
     230,   453,   454,   455,   456,   457,     0,   459,     0,     0,
     460,   461,   462,     0,   231,   232,     0,   233,     0,     0,
       0,   463,   234,     0,    23,     0,     0,     0,     0,     0,
     235,     0,    98,     0,     0,     0,   236,   363,     0,     0,
       0,     0,   237,     0,     0,     0,   238,     0,     0,   239,
       0,   240,     0,     0,     0,     0,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,   242,   243,     0,
       0,     0,     0,     0,     0,   244,    56,     0,     0,     0,
       0,     0,     0,     0,     0,   245,     0,     0,    65,     0,
       0,   230,     0,     7,   246,     0,   247,     0,   248,     0,
     249,     0,     0,   250,     0,   231,   232,   251,   233,   252,
       0,    84,   253,   234,     0,     0,     0,     0,     0,     0,
       0,   235,     0,     0,     0,     0,     0,   236,     0,     0,
       0,     0,     0,   237,     0,     0,     0,   238,     0,     0,
     239,     0,   240,     0,     0,     0,     0,     0,     0,     0,
    -310,   241,     0,     0,     0,     0,     0,     0,   242,   243,
       0,     0,     0,     0,  -310,  -310,   244,  -310,     0,     0,
       0,     0,  -310,     0,     0,     0,   245,     0,     0,     0,
    -310,     0,     0,     0,     0,   246,  -310,   247,     0,   248,
       0,   249,  -310,     0,   250,     0,  -310,     0,   251,  -310,
     252,  -310,     0,   253,     0,     0,     0,     0,     0,   230,
    -310,     0,     0,     0,     0,     0,     0,  -310,  -310,     0,
       0,     0,     0,   231,   232,  -310,   233,     0,     0,     0,
       0,   234,     0,     0,     0,  -310,     0,     0,     0,   235,
       0,     0,     0,     0,  -310,   236,  -310,     0,  -310,     0,
    -310,   237,     0,  -310,     0,   238,     0,  -310,   239,  -310,
     240,     0,  -310,     0,     0,     0,     0,     0,  -311,   241,
       0,     0,     0,     0,     0,     0,   242,   243,     0,     0,
       0,     0,  -311,  -311,   244,  -311,     0,     0,     0,     0,
    -311,     0,     0,     0,   245,     0,     0,     0,  -311,     0,
       0,     0,     0,   246,  -311,   247,     0,   248,     0,   249,
    -311,     0,   250,     0,  -311,     0,   251,  -311,   252,  -311,
       0,   253,     0,     0,     0,     0,     0,     0,  -311,     0,
       0,     0,     0,     0,     0,  -311,  -311,     0,     0,     0,
       0,     0,     0,  -311,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -311,     0,     0,     0,     0,     0,     0,
       0,     0,  -311,     0,  -311,     0,  -311,     0,  -311,     0,
       0,  -311,     0,     0,     0,  -311,     0,  -311,     0,     0,
    -311
};

static const yytype_int16 yycheck[] =
{
       2,     2,    62,   165,   401,   110,    67,     2,   425,   554,
     229,   622,   181,   581,    62,    62,   592,   265,   492,   518,
     634,   223,   630,    25,   131,    86,   607,   608,   800,   888,
      25,     2,   613,   614,    26,    28,   835,   610,   549,   381,
     101,   830,   290,   794,     1,   853,   265,    40,    41,    42,
     682,     1,   684,     1,    25,   203,    32,    32,   403,    47,
       5,    61,   896,   971,   102,     3,   119,     1,    32,    24,
     525,   290,    74,    32,    66,    22,    78,    79,   122,    74,
       1,   329,    45,    78,    79,    40,    41,    42,   391,    56,
     164,     1,   261,   119,     0,    27,   146,   119,   100,   520,
     174,   117,   104,    74,    32,   100,    69,    78,    79,   104,
     329,    32,    32,    47,    56,    47,    54,   117,    32,    51,
      24,   174,    32,    32,   174,   169,   547,    75,    32,   100,
     146,   169,    47,   104,   437,   438,    40,    41,    42,   973,
      61,   164,    89,   119,    76,    24,   140,    79,   174,    81,
     119,    61,   174,    32,   148,   119,   149,    24,   146,    91,
     119,    40,    41,    42,   111,    32,  1025,   124,   116,   146,
     163,   146,   780,    40,    41,    42,    91,   109,    35,   634,
     483,   484,   114,    56,   105,    47,   489,   164,   145,   146,
     124,    47,   964,   169,   169,   105,   117,  1105,   766,   971,
     545,   146,  1061,  1062,   171,   169,   199,   117,    56,     2,
     169,   145,   146,   215,   118,   172,  1024,   365,   173,     3,
     215,   171,   170,   974,   145,   170,   146,   446,    47,   171,
     164,  1020,    25,   481,   482,   145,   817,   818,   819,   118,
     172,   169,    56,   742,   215,     3,     3,    56,   169,   169,
     164,   118,   109,   595,   164,   169,   425,   119,   834,   169,
     169,    83,   481,   482,    86,   258,   259,   269,  1127,   173,
      54,    18,   274,   380,   269,   578,    47,   270,   146,   274,
      75,    74,    47,   146,   146,    78,    79,   798,    47,    27,
     146,  1150,   865,   169,   173,   171,    54,    54,   269,   518,
     119,   164,   164,   274,   172,    46,   173,   100,   164,    47,
     558,   104,   174,    51,   277,  1087,   279,   310,   174,   103,
      91,   116,  1121,   146,   164,    66,    91,   146,    46,    91,
     170,    78,    91,  1105,   496,   537,    93,   945,    76,   558,
     146,    79,   402,    81,    85,   103,   103,   349,   349,   172,
      97,    78,    79,    91,   349,   174,     3,   146,   972,     2,
      27,   363,   581,   326,   170,   112,   359,    85,   363,   146,
     372,   109,   113,    20,    46,   170,   114,   372,   349,   146,
     109,   170,    25,   385,   164,   146,    33,   886,   171,   164,
     385,   171,   363,   170,    66,   113,    53,   599,   390,   174,
     146,   372,   395,   146,   397,   172,   398,    54,   401,   470,
     471,   172,    79,    85,   385,   476,    63,    74,   479,   480,
     146,   164,   215,   425,    91,   993,   172,    84,    11,   928,
     146,    74,   425,   146,   172,    78,    79,  1013,   146,   146,
     124,   113,   146,  1075,  1076,   124,   172,   114,   164,   146,
      56,   164,  1033,  1034,  1065,   146,   103,   100,   932,    31,
      50,   104,   109,    53,   172,   172,   150,   164,   172,     2,
    1051,  1052,   151,   164,   153,   154,   269,   156,   157,   164,
     146,   274,   146,   146,    74,   146,   546,    77,    56,   174,
     907,   888,    25,   171,    84,   146,   488,   490,   546,   546,
     146,    56,   146,   164,   103,   507,   172,   509,   172,   172,
     571,   572,   507,   146,   509,   164,    90,   622,    90,   109,
      94,   172,    94,   742,    56,   174,   172,   520,   172,     9,
     146,   164,  1113,  1114,  1115,  1116,   507,   146,   509,   146,
     533,    74,  1158,  1154,  1120,    78,    79,   766,   164,    31,
     543,   171,   164,   173,   547,   164,   349,   164,   170,    90,
     146,   864,    90,    94,    78,    79,    94,   100,   140,   141,
     363,   104,   215,   145,   171,   147,   148,   880,   164,   372,
     152,    56,   164,   585,   585,   577,   171,   159,   173,   171,
     585,   173,   385,   165,   166,   167,    90,    56,   591,    56,
      94,  1217,  1218,  1219,  1220,  1221,  1222,   100,    90,   602,
     829,   830,    94,   140,   585,   291,   292,   609,   145,  1164,
     923,   148,    56,    90,   927,   152,   269,    94,  1025,   622,
     792,   274,    90,   171,    56,    31,    94,   171,    56,   173,
     603,   604,    56,   946,   947,   171,   171,   173,   173,    31,
     613,   614,   171,   171,   173,   173,    56,     5,   140,   141,
     148,   143,   144,   145,  1061,   147,   148,   886,  1213,  1066,
     152,   571,   572,   169,   145,    56,   158,   159,   169,   171,
     162,   164,   215,   165,   166,   167,   163,   990,   164,   992,
     164,    31,   164,   855,    90,   688,   164,    56,    94,   164,
     164,   164,    85,   764,   164,   171,   349,   169,    90,   928,
     164,   172,    94,   146,   507,   171,   509,   145,   171,   164,
     363,   171,   783,   164,   172,   146,     9,     3,   171,   372,
    1127,    66,    79,   169,   124,   164,   269,   148,   907,    69,
     171,   274,   385,   146,   140,   124,   172,   124,   124,   145,
      90,    66,   148,    34,    94,    34,   152,   759,   140,   141,
     172,   143,   144,   145,   124,   147,   148,   149,   770,    21,
     152,   773,   145,   172,   993,   770,   158,   159,   773,   172,
     162,   169,   775,   165,   166,   167,   779,   118,   146,   105,
     124,   171,   585,   171,   176,    31,   789,   148,   170,   770,
     140,   141,   773,   143,   144,   145,   148,   147,   148,   148,
     148,   148,   152,   174,   164,   808,   349,   140,   164,   159,
     148,   148,   148,   148,   148,   165,   166,   167,   888,   148,
     363,   148,   164,    32,   894,   148,   148,   148,   171,   372,
      34,    34,   164,   903,   164,   145,   164,   146,    34,   170,
    1098,  1099,   385,    25,    90,   818,   819,   164,    94,    47,
     862,   854,   172,   172,   507,   867,   509,   862,   860,   172,
     174,   169,   867,   174,   876,   146,   170,   879,   174,  1098,
    1099,   876,   172,   171,   879,   171,   164,   164,   145,   164,
     883,   862,   146,    32,   174,   164,   867,    34,   172,   172,
     172,   164,    74,   163,   140,   876,   174,   174,   879,   145,
     172,   147,   148,   163,   907,   878,   152,    27,   146,   882,
     109,   580,   993,   159,   827,   372,    24,   365,   264,   165,
     166,   167,   104,   338,   389,   986,   531,    47,   546,   883,
     894,    51,   585,   383,   946,   947,   907,   903,   304,  1213,
    1068,   953,  1223,   955,  1216,   854,   958,  1062,   953,   129,
     955,   528,   533,   958,   688,  1025,    76,   779,    20,    79,
     932,    81,    -1,  1142,   507,    -1,   509,   770,    -1,   981,
     773,    91,   953,    -1,   955,    -1,   981,   958,    -1,   991,
     982,    -1,    -1,   986,    -1,    27,   991,    -1,    -1,   109,
      -1,    -1,    -1,    31,   114,    -1,    -1,    -1,     2,    -1,
     981,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,    51,
     991,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1032,    25,    -1,    -1,    -1,    -1,    -1,  1032,    -1,  1041,
      -1,    -1,    -1,   215,    76,  1150,  1041,    79,    -1,  1154,
      -1,    -1,   585,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,  1032,    90,    -1,    -1,    -1,    94,  1127,    -1,   862,
    1041,    -1,  1065,  1242,   867,  1244,    -1,   109,  1247,  1248,
      74,    -1,   114,   876,    78,    79,   879,    -1,  1051,  1052,
      -1,    -1,  1094,    -1,    -1,    -1,    -1,   269,    -1,  1094,
      -1,    -1,   274,    -1,    -1,    -1,   100,    -1,    -1,    -1,
     104,    -1,   140,   141,    -1,    -1,   144,   145,    -1,   147,
     148,    -1,    -1,  1094,   152,    27,    -1,   770,    -1,    -1,
     773,   159,    -1,  1135,    -1,  1137,    -1,   165,   166,   167,
    1135,  1143,  1137,    -1,  1146,    47,  1109,    -1,  1143,    51,
      -1,  1146,  1115,  1116,    -1,    -1,    -1,    -1,    -1,    -1,
     953,  1154,   955,    -1,  1135,   958,  1137,    -1,    -1,  1132,
      -1,    -1,  1143,  1175,    76,  1146,    -1,    79,    -1,  1181,
    1175,    -1,  1145,  1185,  1186,  1148,  1181,    -1,   981,    91,
    1185,  1186,    -1,    -1,    -1,  1197,    -1,    31,   991,  1201,
      -1,  1203,  1197,    -1,  1175,     2,  1201,   109,  1203,    -1,
    1181,    -1,   114,   385,  1185,  1186,    -1,    -1,    -1,   862,
      -1,   215,  1224,  1224,   867,    -1,  1197,    -1,    25,  1224,
    1201,    -1,  1203,   876,    -1,    27,   879,   770,    -1,  1032,
     773,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1041,    -1,
      -1,    -1,    -1,  1224,    -1,    47,    90,    -1,    -1,    51,
      94,    -1,    -1,    -1,  1266,  1267,  1268,  1269,    -1,    -1,
      -1,  1266,  1267,  1268,  1269,   269,    -1,    74,    -1,    -1,
     274,    78,    79,    -1,    76,    -1,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    -1,  1266,  1267,  1268,  1269,    91,
      -1,  1094,    -1,   100,    -1,    -1,   140,   104,    -1,    -1,
     953,   145,   955,   147,   148,   958,    -1,   109,   152,    -1,
      -1,    -1,   114,    -1,    -1,   159,    -1,    -1,    -1,   862,
      -1,    -1,    -1,   167,   867,   507,    -1,   509,   981,    -1,
      -1,    -1,  1135,   876,  1137,    -1,   879,    -1,   991,    -1,
    1143,    -1,    -1,  1146,    -1,   349,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   363,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   372,    -1,
      -1,    -1,  1175,     2,    -1,    -1,    -1,    -1,  1181,  1032,
      -1,   385,  1185,  1186,    -1,    -1,    -1,    -1,  1041,    -1,
      -1,    -1,    -1,    -1,  1197,    -1,    25,    -1,  1201,    -1,
    1203,    -1,     5,    -1,    -1,    -1,    -1,    -1,   215,    -1,
     953,    -1,   955,    -1,    -1,   958,    -1,    -1,    -1,    22,
      -1,  1224,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    37,    38,    -1,    -1,   981,    -1,
      43,  1094,    -1,    -1,    -1,    74,    -1,    50,   991,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   269,  1266,  1267,  1268,  1269,   274,    -1,    72,
      -1,   100,    -1,    -1,    77,   104,    79,    -1,    81,    -1,
      83,    -1,  1135,    86,  1137,    -1,    89,    -1,    91,  1032,
    1143,    -1,    -1,  1146,    -1,    -1,    -1,    -1,  1041,    -1,
      -1,    -1,    -1,   507,    -1,   509,   109,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1175,    -1,     2,    -1,    -1,    -1,  1181,    -1,
      -1,    -1,  1185,  1186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   349,    -1,  1197,    -1,    -1,    25,  1201,    -1,
    1203,  1094,    -1,    -1,    -1,    -1,   363,    -1,    -1,    -1,
      -1,   164,    -1,    -1,    -1,   372,    -1,   170,    -1,    -1,
      -1,  1224,    -1,    -1,    -1,    -1,    -1,    -1,   385,    -1,
      -1,   585,    -1,    -1,    -1,    -1,   215,    -1,   770,    -1,
      -1,   773,  1135,    -1,  1137,    -1,    74,    -1,    -1,    -1,
    1143,    -1,    -1,  1146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1266,  1267,  1268,  1269,    -1,    -1,    -1,
      -1,    -1,   100,    -1,    -1,     1,   104,    -1,    -1,     5,
      -1,    -1,  1175,    -1,    -1,    -1,    -1,    -1,  1181,    -1,
     269,    -1,  1185,  1186,    -1,   274,    22,    -1,    -1,    -1,
      26,    27,    -1,    -1,  1197,    -1,    -1,    -1,  1201,    35,
    1203,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
     862,  1224,    -1,    -1,    -1,   867,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   876,    -1,    72,   879,    74,    -1,
     507,    77,   509,    79,    -1,    81,    -1,    83,    84,    -1,
      86,    -1,    -1,    89,    -1,    91,    -1,    -1,    -1,    -1,
     349,    -1,    -1,  1266,  1267,  1268,  1269,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   363,   111,    -1,   215,   114,    -1,
      -1,    -1,    -1,   372,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   385,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   770,    -1,     1,   773,
       3,   953,    -1,   955,    -1,    -1,   958,    -1,   585,    -1,
      -1,    -1,    -1,    -1,    17,    18,    -1,    20,   164,    -1,
      -1,   269,    25,    -1,   170,    -1,   274,    -1,    -1,   981,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,   991,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
    1032,    -1,    -1,    -1,    -1,    88,    -1,    -1,   862,  1041,
      -1,    -1,    -1,   867,    97,    -1,    99,    -1,   101,    -1,
     103,   349,   876,   106,    -1,   879,    -1,   110,   507,   112,
     509,    -1,   115,    -1,    -1,   363,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   372,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     5,    -1,    -1,    -1,    -1,   385,    -1,    -1,
      -1,    -1,  1094,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    37,    38,    -1,    -1,   171,    -1,
      43,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,   953,
      -1,   955,    -1,  1135,   958,  1137,   585,    -1,    -1,    -1,
      -1,  1143,    -1,   770,  1146,    -1,   773,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    77,    -1,    79,   981,    81,    -1,
      83,    -1,    -1,    86,    -1,    -1,    89,   991,    91,    -1,
      -1,    -1,    -1,  1175,    -1,    -1,    -1,    -1,    -1,  1181,
      -1,    -1,    -1,  1185,  1186,    -1,   109,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,  1197,    67,    -1,    -1,  1201,
      -1,  1203,    -1,    -1,    -1,    -1,    -1,    -1,  1032,   507,
      -1,   509,    -1,    -1,    -1,    86,    -1,  1041,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,
     101,    -1,    -1,    -1,    -1,   862,    35,    -1,    37,    38,
     867,   164,    -1,    -1,    43,    -1,    -1,   170,    -1,   876,
      -1,    50,   879,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1266,  1267,  1268,  1269,    -1,    -1,
    1094,    -1,    -1,    72,     3,    -1,    -1,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,   585,    17,    -1,
      89,    20,    91,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    35,    -1,    -1,    -1,
     109,  1135,   111,  1137,    -1,   114,    45,    -1,    -1,  1143,
      -1,   770,  1146,    52,   773,    54,   953,    -1,   955,    69,
      -1,   958,    -1,    -1,    63,    -1,    -1,    -1,    78,    79,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1175,    -1,    -1,   981,    -1,    -1,  1181,   229,    88,
      -1,  1185,  1186,    -1,   991,    -1,    -1,    -1,    -1,   109,
      99,    -1,    -1,  1197,   103,    -1,    -1,  1201,    -1,  1203,
      -1,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
       5,    -1,    -1,    -1,   265,    -1,    -1,   137,    -1,    -1,
    1224,    -1,    -1,    -1,    -1,  1032,    -1,    22,    -1,    -1,
      -1,    26,    27,   862,  1041,    -1,    -1,    -1,   867,   290,
      35,    -1,    37,    38,    -1,    -1,    -1,   876,    43,    -1,
     879,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,  1266,  1267,  1268,  1269,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,   329,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,  1094,    83,    -1,
      -1,    86,   770,    -1,    89,   773,    91,    -1,    -1,    -1,
      -1,   221,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,   953,    -1,   955,    -1,  1135,   958,
    1137,    -1,    -1,    -1,    -1,    -1,  1143,    -1,    -1,  1146,
     260,   261,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   981,    -1,    -1,    -1,    -1,   277,    -1,   279,
      -1,    -1,   991,    -1,    -1,    -1,   286,    -1,  1175,   164,
      -1,    -1,    -1,    -1,  1181,   170,    -1,    -1,  1185,  1186,
      -1,    -1,    -1,    -1,   862,    -1,    -1,    -1,    -1,   867,
    1197,    -1,    -1,    -1,  1201,    -1,  1203,    -1,   876,    -1,
      -1,   879,    -1,  1032,    -1,    -1,   326,    -1,    -1,    -1,
      -1,    -1,  1041,    -1,    -1,    -1,    -1,  1224,    -1,   470,
     471,    -1,    -1,    -1,    -1,   476,    -1,    -1,   479,   480,
     481,   482,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1266,
    1267,  1268,  1269,    -1,    -1,  1094,    -1,   518,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   953,   168,   955,    -1,   171,
     958,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   981,    -1,    -1,  1135,   558,  1137,    -1,
      -1,    -1,    -1,   991,  1143,    -1,    -1,  1146,    -1,    -1,
     571,   572,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     581,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,  1175,    -1,    -1,    17,
      -1,    -1,  1181,    -1,  1032,    -1,  1185,  1186,    26,    -1,
      -1,    -1,    -1,  1041,    -1,    33,    -1,    -1,  1197,    -1,
      -1,    -1,  1201,    -1,  1203,    -1,    -1,    45,    -1,    47,
      48,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    57,
      -1,    -1,    -1,    -1,    62,  1224,    -1,    -1,    66,    67,
      68,    69,    70,    -1,    -1,    -1,    74,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    83,  1094,    -1,    86,    -1,
      88,    -1,    90,    -1,    -1,    93,    94,    -1,    96,    -1,
      -1,    -1,   100,   101,   102,    -1,    -1,  1266,  1267,  1268,
    1269,   561,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1135,    -1,  1137,
      -1,   129,    -1,   131,    -1,  1143,    -1,    -1,  1146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   603,   604,    -1,    -1,    -1,    -1,    -1,
      -1,   742,    -1,   613,   614,    -1,    -1,  1175,    -1,    -1,
      -1,    -1,    -1,  1181,    -1,    -1,   174,  1185,  1186,    -1,
      -1,    -1,    -1,   764,    -1,   766,    -1,    -1,    -1,  1197,
      -1,    -1,    -1,  1201,    -1,  1203,    -1,    -1,    -1,   197,
      -1,    -1,   783,   201,   202,     5,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1224,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,
      -1,   229,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,     3,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1266,  1267,
    1268,  1269,    17,    18,    -1,    20,    -1,   265,    -1,    -1,
      25,    -1,    72,    -1,    -1,    -1,    -1,    77,    33,    79,
      -1,    81,    -1,    83,    39,    -1,    86,    -1,    -1,    89,
      45,    91,   290,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    -1,    -1,   301,    -1,   886,    -1,    -1,    63,   109,
      -1,   111,    -1,    -1,   114,    70,    71,    -1,   316,   317,
      -1,    -1,    -1,    78,   774,    -1,    -1,    -1,    -1,    -1,
      -1,   329,    -1,    88,    -1,   333,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    99,    -1,   101,   928,   103,    -1,
      -1,   106,    -1,   351,    -1,   110,    -1,   112,   808,    -1,
     115,    -1,    -1,   361,   164,    -1,    -1,    -1,   818,   819,
      -1,    -1,    -1,    -1,    -1,   373,    -1,   375,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   389,   390,   391,    -1,    -1,    -1,    -1,    -1,    -1,
     398,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   993,    -1,   169,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   878,    -1,
      -1,    -1,   882,    -1,   884,    -1,   434,   435,   436,   437,
     438,   439,    -1,   441,   442,   443,   444,   445,    -1,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,    -1,    -1,    -1,    -1,
      -1,    -1,   470,   471,    -1,    -1,    -1,    -1,   476,    -1,
      -1,   479,   480,   481,   482,   483,   484,    -1,    -1,    -1,
     488,   489,    -1,    -1,   492,    -1,    -1,    -1,    -1,    -1,
     498,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   509,    -1,    -1,    -1,    -1,   514,  1098,  1099,    -1,
     518,    -1,    -1,    -1,    -1,    -1,    -1,   525,    -1,   527,
      -1,    -1,    -1,   531,    -1,    -1,   534,   535,    -1,   989,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   546,    -1,
      -1,   549,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     558,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     568,    -1,    -1,   571,   572,    -1,    -1,    -1,    -1,   577,
     578,    -1,    -1,   581,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   592,   593,    -1,    -1,    -1,    -1,
      -1,  1051,  1052,    -1,    -1,    -1,    -1,    -1,   606,     3,
      -1,   609,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,   634,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,  1109,
      54,    -1,    -1,    -1,    -1,  1115,  1116,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,   679,  1132,    -1,    78,    -1,    -1,    -1,   686,    -1,
      -1,    -1,  1142,    -1,    88,  1145,    -1,    -1,  1148,    -1,
      -1,    -1,    -1,    97,    -1,    99,    -1,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,     5,   110,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,  1183,    -1,    -1,    26,    27,    -1,    -1,
      -1,    -1,    -1,   741,   742,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,   764,    -1,   766,   767,
     768,    -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    72,    -1,    -1,   783,    -1,    77,    31,    79,
      -1,    -1,  1242,    83,  1244,    -1,    86,  1247,  1248,    89,
     798,    91,    -1,    -1,   802,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,   827,
      -1,    -1,    -1,   831,    -1,   833,   834,   835,    -1,   837,
      -1,   839,    -1,    -1,   842,   843,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,   856,    -1,
      -1,    -1,   860,    -1,    -1,    -1,   864,   865,    -1,   867,
      -1,    -1,    -1,    -1,   164,   873,   874,    -1,   876,    -1,
     123,   879,   880,    -1,    -1,    -1,    -1,    -1,   886,    -1,
      -1,    -1,    -1,   891,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,   923,   924,   925,   926,   927,
     928,    -1,    -1,   176,   932,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   946,   947,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   956,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,   972,    -1,    -1,    -1,   976,    -1,
      -1,    -1,    17,    18,   982,    20,    -1,    -1,    -1,    -1,
      25,    -1,   990,    -1,   992,   993,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    -1,    49,  1013,    -1,    52,    -1,    54,
      -1,    -1,  1020,    -1,  1022,    -1,    -1,    -1,    63,    -1,
      -1,  1029,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,  1042,    -1,    -1,    -1,    -1,    -1,
      -1,  1049,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    99,    -1,   101,    -1,   103,    -1,
      -1,   106,  1070,    -1,    -1,   110,    -1,   112,    -1,    -1,
     115,    -1,     3,    -1,  1082,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,
    1098,  1099,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,  1111,    -1,    -1,    -1,    -1,    39,    -1,
      -1,  1119,  1120,  1121,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    -1,   171,  1135,    -1,  1137,
      -1,    -1,    63,  1141,    -1,  1143,    -1,    -1,  1146,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
    1168,  1169,    -1,  1171,  1172,    -1,    97,    -1,    99,    -1,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,   112,    -1,  1191,   115,    -1,  1194,    -1,    -1,    -1,
    1198,    -1,    -1,    -1,    -1,  1203,  1204,    -1,    -1,  1207,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1229,  1230,  1231,  1232,  1233,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1266,  1267,
    1268,  1269,  1270,  1271,  1272,  1273,     0,     1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
     164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     1,   176,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    19,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    -1,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    -1,   169,
     170,   171,    -1,   173,    -1,     1,   176,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,
      -1,   167,    -1,   169,   170,   171,    -1,   173,    -1,     1,
     176,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,   164,    -1,    -1,   167,    -1,   169,   170,   171,
      -1,   173,    -1,     1,   176,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
     118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,
      -1,   169,   170,   171,    -1,   173,    -1,     1,   176,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
     164,    -1,    -1,   167,    -1,   169,   170,   171,    -1,   173,
      -1,     1,   176,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    93,    -1,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     1,   176,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,     1,
     176,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     1,   176,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    -1,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
     118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     1,   176,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
     164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     1,   176,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    -1,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    93,    -1,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     1,   176,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    -1,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,     1,
     176,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,
       1,   173,     3,     4,   176,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    24,    25,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    42,    -1,    44,    45,    -1,    47,    -1,    49,    -1,
      51,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    78,    79,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,   110,
      -1,   112,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,   172,   173,    -1,     1,   176,     3,     4,    -1,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    24,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    42,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,     3,     4,   176,
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    -1,
      -1,    17,    18,    19,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    55,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
      -1,   137,   138,    -1,   140,   141,   142,   143,   144,   145,
      -1,   147,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,    -1,   171,    -1,    -1,     3,     4,
     176,     6,     7,     8,     9,    10,    -1,    12,    -1,    14,
      -1,    -1,    17,    18,    19,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      55,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,   137,   138,    -1,   140,   141,   142,   143,   144,
     145,    -1,   147,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
     165,   166,   167,   168,   169,    -1,   171,    -1,    -1,    -1,
      -1,   176,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    -1,    17,    18,    -1,    20,
      -1,    22,    23,    -1,    25,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    62,    63,    64,    65,    -1,    67,    68,    -1,    70,
      71,    72,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,    -1,    -1,    -1,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,    -1,     3,     4,   176,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,    49,
      -1,    51,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    78,    79,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,
     110,    -1,   112,    -1,   114,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,    -1,     3,     4,   176,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    26,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,    78,
      79,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,   112,    -1,   114,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,    -1,     3,     4,   176,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    42,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,   111,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,     3,     4,   176,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    24,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    42,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,     3,     4,   176,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    -1,    94,    -1,
      -1,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
     146,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,
      -1,   167,    -1,   169,   170,   171,    -1,    -1,     3,     4,
     176,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,   107,   108,    -1,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,     3,
       4,   176,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,   107,   108,    -1,   110,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,
       3,     4,   176,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,   107,   108,    -1,   110,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,   172,
      -1,     3,     4,   176,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,    -1,   174,    -1,   176,     3,     4,    -1,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,   107,
     108,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,   172,    -1,     3,     4,   176,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
     107,   108,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,   172,    -1,     3,     4,   176,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,   107,   108,    -1,   110,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,   172,    -1,     3,     4,
     176,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,   107,   108,    -1,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,     3,
       4,   176,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,   146,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
     164,    -1,    -1,   167,    -1,   169,   170,   171,    -1,    -1,
       3,     4,   176,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,   146,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,   170,   171,    -1,
      -1,     3,     4,   176,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,   146,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,   164,    -1,    -1,   167,    -1,   169,   170,   171,
      -1,    -1,     3,     4,   176,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,   112,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,    -1,     3,     4,   176,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,
     110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,    -1,     3,     4,   176,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    -1,    94,    -1,    -1,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,    -1,     3,     4,   176,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,    -1,   174,    -1,   176,     3,
       4,    -1,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
     164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,
     174,    -1,   176,     3,     4,    -1,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,   146,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,   172,    -1,     3,     4,   176,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,    -1,   174,    -1,   176,     3,     4,
      -1,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,     3,
       4,   176,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,
       3,     4,   176,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
      -1,   174,    -1,   176,     3,     4,    -1,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,   170,   171,    -1,    -1,     3,     4,   176,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,    -1,   174,    -1,   176,     3,
       4,    -1,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,
       3,     4,   176,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
      -1,     3,     4,   176,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,    -1,     3,     4,   176,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,    -1,     3,     4,   176,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,    -1,     3,     4,   176,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,    -1,     3,     4,   176,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,     4,    -1,     6,     7,     8,     9,    10,    -1,
     158,    -1,    14,   161,   162,   163,    18,    -1,    -1,   167,
      22,   169,    24,   171,    -1,    27,    -1,    -1,   176,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    -1,    -1,    65,    -1,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    79,    80,    81,
      82,    -1,    -1,    -1,    -1,    87,    -1,    89,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    97,    98,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
     172,   173,    -1,     4,   176,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    -1,    18,    -1,    -1,
      -1,    22,    -1,    24,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    -1,    -1,    65,    -1,    67,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    79,    80,
      81,    82,    -1,    -1,    -1,    -1,    87,    -1,    89,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    97,    98,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    12,    -1,   149,    -1,
      -1,    17,    18,    19,    20,    -1,    -1,   158,    -1,    25,
     161,   162,   163,    -1,    -1,    -1,   167,    33,   169,    35,
     171,    -1,   173,    39,    -1,   176,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
      -1,   137,   138,    -1,   140,   141,   142,   143,   144,   145,
      -1,   147,    -1,    -1,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    -1,   162,    -1,    -1,   165,
     166,   167,   168,     3,     4,   171,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    24,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    42,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,    -1,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,     3,     4,    -1,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    24,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    42,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,   142,    -1,    -1,     3,     4,    -1,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,   163,    22,    -1,    24,    25,    -1,
     169,    -1,   171,    -1,   173,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    42,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,     3,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    12,   112,    -1,    -1,   115,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,   142,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    71,   171,    -1,   173,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    99,    -1,   101,    -1,   103,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,   137,
     138,    -1,   140,   141,   142,   143,   144,   145,    -1,   147,
      -1,    -1,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,     3,   162,    -1,    -1,   165,   166,   167,
     168,   169,    12,    -1,    -1,    -1,    -1,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    97,    -1,    99,
      -1,   101,    19,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,   112,    -1,    31,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,    -1,   137,   138,    -1,
     140,   141,   142,   143,   144,   145,    -1,   147,    -1,    -1,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,    -1,   162,    -1,    -1,   165,   166,   167,   168,    -1,
      -1,    -1,    -1,    90,    -1,    12,    -1,    94,    -1,    -1,
      -1,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    -1,
     137,   138,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,   169,    90,    -1,    12,    -1,    94,    -1,   176,
      -1,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    -1,
     137,   138,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,   169,    90,    -1,    12,    -1,    94,    -1,   176,
      -1,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    -1,
     137,   138,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,   169,    90,    -1,    12,    -1,    94,    -1,   176,
      -1,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    -1,
     137,   138,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,   169,    90,    -1,    12,    -1,    94,    -1,   176,
      -1,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    -1,
     137,   138,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,   169,    90,    -1,    -1,    -1,    94,    -1,   176,
      -1,    -1,    -1,    -1,    12,   102,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    47,
     137,   138,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,   169,    -1,    -1,    -1,    -1,    -1,    12,   176,
      -1,    -1,    90,    -1,    -1,    19,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,    47,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    90,   165,   166,   167,
      94,   169,    -1,    -1,    -1,    -1,    -1,    -1,   176,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    90,    -1,    31,   123,
      94,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
      73,   165,   166,   167,    -1,   169,   140,   141,    -1,   143,
     144,   145,   176,   147,   148,   149,    -1,    90,   152,   153,
     154,    94,   156,   157,   158,   159,    -1,    -1,   162,    12,
      13,   165,   166,   167,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,    31,    -1,
     123,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      73,   164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,    -1,    -1,    -1,    90,    12,    -1,
      -1,    94,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    90,    -1,    12,    -1,
      94,    -1,    -1,   176,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    90,    -1,    12,    -1,
      94,    -1,   176,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    90,    -1,    12,    -1,
      94,    -1,   176,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    90,    12,    -1,    -1,
      94,    -1,   176,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,   176,    -1,    -1,    -1,    -1,   102,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,   123,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    -1,    73,
     165,   166,   167,    -1,   169,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    12,    -1,    -1,    -1,    90,    -1,    -1,    19,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    31,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
      90,   165,   166,   167,    94,    -1,    -1,    12,    13,    -1,
      -1,    -1,   176,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,   119,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,   119,   165,   166,   167,   123,    -1,
      -1,    -1,    -1,    -1,   174,    90,   176,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   123,    -1,
     165,   166,   167,    -1,    -1,    -1,    12,    -1,    -1,   174,
      -1,   176,    -1,    19,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    31,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,   176,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    12,    -1,    94,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   123,    -1,   165,
     166,   167,    -1,   169,    90,    12,    -1,    -1,    94,    -1,
     176,    -1,    19,    -1,   140,   141,   102,   143,   144,   145,
      -1,   147,   148,   149,    31,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   123,   164,   165,
     166,   167,    -1,    -1,    -1,    12,    13,    -1,    -1,    -1,
     176,    -1,    19,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    31,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    90,    -1,    -1,    12,    94,    -1,    -1,
     176,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   123,    -1,   165,   166,
     167,    -1,    -1,    -1,    90,   172,    -1,    -1,    94,   176,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   122,   123,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    12,    -1,   165,
     166,   167,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     176,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    12,    -1,    94,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   123,    -1,
     165,   166,   167,    -1,    -1,    90,    12,    13,    -1,    94,
      -1,   176,    -1,    19,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    31,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   123,    -1,
     165,   166,   167,    -1,    -1,    -1,    12,   172,    -1,    -1,
      -1,   176,    -1,    19,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    31,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    90,    -1,    -1,   172,    94,    -1,
      -1,   176,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   123,    -1,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,    -1,    90,    -1,   140,   141,    94,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    12,    -1,   165,
     166,   167,    -1,    -1,    19,   123,   172,    -1,    -1,    -1,
     176,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    12,    -1,   165,   166,   167,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   176,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,   140,   141,    94,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    -1,   164,
     165,   166,   167,    -1,    -1,   122,   123,    -1,    -1,    -1,
      -1,   176,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   123,    -1,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    12,    -1,   165,   166,
     167,    -1,    -1,    19,    -1,    -1,     1,   174,     3,   176,
       5,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    12,    13,    33,    -1,
      -1,    -1,    -1,    19,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    -1,    49,    31,    -1,    52,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    90,    70,    71,    12,    94,    -1,
      -1,    -1,    -1,    78,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    99,    -1,   101,   123,   103,    -1,
      -1,   106,    -1,    -1,    90,   110,    -1,   112,    94,    -1,
     115,    -1,    -1,    -1,   140,   141,    61,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   123,    -1,   165,
     166,   167,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
     176,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,   123,   165,
     166,   167,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
     176,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    31,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,   176,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      12,    -1,   102,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
     140,   141,    94,   143,   144,   145,    -1,   147,   148,   149,
     102,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,   176,    -1,    90,    12,
      -1,    -1,    94,    -1,    -1,    -1,    19,    -1,   140,   141,
     102,   143,   144,   145,    -1,   147,   148,   149,    31,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   123,    -1,   165,   166,   167,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,   176,    -1,    19,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    31,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    90,    -1,    12,
      -1,    94,    -1,    -1,   176,    -1,    19,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,   140,   141,   102,
     143,   144,   145,    -1,   147,   148,   149,    31,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     123,    -1,   165,   166,   167,    -1,    -1,    90,    12,    -1,
      -1,    94,    -1,   176,    -1,    19,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    31,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     123,    -1,   165,   166,   167,    -1,    90,    -1,    -1,    -1,
      94,    -1,    -1,   176,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    90,    -1,    -1,    -1,
      94,    -1,    -1,   176,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,    -1,   162,   123,
      -1,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    31,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
      22,   165,   166,   167,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    35,    -1,    37,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    90,    -1,    -1,    -1,    94,
      72,    -1,    74,    -1,    -1,    77,    -1,    79,    -1,    -1,
      -1,    83,    84,    85,    86,    -1,    -1,    89,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
       3,   156,   157,   158,   159,   160,    -1,   162,    -1,    -1,
     165,   166,   167,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,   176,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,   164,    -1,    -1,    -1,    39,   169,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    91,    -1,
      -1,     3,    -1,     5,    97,    -1,    99,    -1,   101,    -1,
     103,    -1,    -1,   106,    -1,    17,    18,   110,    20,   112,
      -1,   114,   115,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    17,    18,    78,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    97,    39,    99,    -1,   101,
      -1,   103,    45,    -1,   106,    -1,    49,    -1,   110,    52,
     112,    54,    -1,   115,    -1,    -1,    -1,    -1,    -1,     3,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    17,    18,    78,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    88,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    97,    39,    99,    -1,   101,    -1,
     103,    45,    -1,   106,    -1,    49,    -1,   110,    52,   112,
      54,    -1,   115,    -1,    -1,    -1,    -1,    -1,     3,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    17,    18,    78,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    88,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    97,    39,    99,    -1,   101,    -1,   103,
      45,    -1,   106,    -1,    49,    -1,   110,    52,   112,    54,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    99,    -1,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,   112,    -1,    -1,
     115
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   181,   182,     0,     1,     3,     4,     5,     6,     7,
       8,     9,    10,    14,    15,    16,    17,    18,    20,    22,
      23,    25,    26,    27,    28,    29,    30,    33,    35,    37,
      38,    39,    43,    44,    45,    48,    49,    50,    52,    54,
      56,    57,    58,    59,    60,    62,    63,    64,    65,    67,
      68,    70,    71,    72,    77,    78,    79,    80,    81,    82,
      83,    86,    87,    88,    89,    91,    92,    93,    95,    96,
      97,    98,    99,   100,   101,   103,   104,   106,   107,   108,
     109,   110,   111,   112,   114,   115,   120,   123,   140,   141,
     142,   143,   144,   149,   158,   161,   162,   163,   164,   167,
     169,   171,   176,   183,   184,   185,   186,   187,   188,   191,
     192,   193,   194,   199,   200,   201,   203,   204,   205,   210,
     211,   215,   216,   220,   222,   225,   226,   229,   230,   231,
     232,   233,   235,   238,   239,   240,   244,   246,   247,   250,
     251,   252,   253,   254,   258,   259,   264,   265,   266,   267,
     270,   271,   276,   277,   279,   280,   285,   289,   290,   292,
     293,   314,   319,   320,   324,   325,   326,   351,   352,   353,
     355,   357,   358,   359,   365,   366,   367,   368,   369,   370,
     372,   375,   376,   377,   378,   379,   380,   381,   382,   384,
     385,   386,   387,   388,   164,    22,    35,    44,    54,    56,
      89,   101,   169,   239,   250,   277,   351,   358,   366,   367,
     372,   375,   377,   378,   119,   360,   361,     3,    54,   217,
     372,   360,   109,   327,    91,   217,   185,   342,   372,   171,
       3,    17,    18,    20,    25,    33,    39,    45,    49,    52,
      54,    63,    70,    71,    78,    88,    97,    99,   101,   103,
     106,   110,   112,   115,   218,   219,   221,    11,    27,   114,
     245,   372,    83,    86,   201,   171,   103,   219,   219,   219,
     171,   219,   329,   330,    32,   205,   224,   372,   262,   263,
     372,   372,    18,    78,    97,   112,   372,   372,   372,     9,
     171,   228,   227,    27,    33,    47,    49,    51,    76,    79,
      91,   101,   109,   114,   184,   223,   281,   282,   283,   305,
     306,   307,   334,   339,   372,   342,   107,   108,   164,   285,
     286,   371,   372,   374,   372,   224,   372,   372,   100,   171,
     185,   372,   372,    72,   187,   191,   205,   187,   205,   372,
     374,   372,   372,   372,   372,   372,     1,   170,   183,   206,
     342,   150,   346,   347,   374,   372,    81,   185,    22,    35,
      38,    72,    89,   169,   188,   189,   190,   201,   205,   193,
     148,   195,   169,    46,    85,   113,   202,    26,   326,   372,
       9,   266,   372,   373,    24,    32,    40,    41,    42,   118,
     173,   241,   242,   354,   356,   171,   205,    56,   145,   268,
     219,   169,   171,   301,    53,    74,    84,   309,    27,    47,
      51,    76,    79,    91,   109,   114,   310,    27,    47,    51,
      76,    79,    91,   109,   114,   308,   219,   321,   322,     1,
     329,   163,   164,   372,    12,    19,    31,    90,    94,   123,
     140,   141,   143,   144,   145,   147,   148,   149,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     165,   166,   167,   176,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   168,
     297,   171,   173,    90,    94,   372,   219,    40,   173,   241,
      56,     1,   171,   185,   164,   205,   325,   164,   146,   164,
     220,   348,   349,   350,   374,   219,   219,   102,   205,   102,
     124,   130,   131,   205,   297,   202,   348,   164,   171,   185,
     171,   219,   331,   332,     1,   145,   338,    47,   146,   185,
     224,   146,   224,    13,   171,   171,   224,   328,   348,   229,
     229,    47,    91,   306,   307,   172,   146,   171,   219,   145,
     164,   372,   372,   122,   287,   164,   169,   224,   171,   348,
     164,   372,   255,   255,   164,   170,   170,   183,   146,   170,
     372,   146,   146,   172,   172,     9,   372,   173,   241,   242,
       3,   171,   196,     1,   170,   206,   213,   214,   372,   208,
     372,    66,    36,    73,   164,   266,   268,   109,   234,   236,
     290,   185,    79,   245,   372,   121,   174,   243,   342,   343,
     344,   372,   383,   243,   372,   219,   248,   249,   219,   342,
     169,     1,   192,   219,   272,   275,   172,   302,   304,   305,
     310,    47,    91,     1,   145,   336,   337,    47,    91,     1,
       3,    12,    17,    19,    20,    25,    45,    52,    54,    55,
      63,    71,    88,    99,   103,   110,   115,   140,   141,   142,
     143,   144,   145,   147,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   162,   165,   166,   167,   168,   171,
     219,   294,   295,   296,   297,   351,   124,   323,   146,   164,
     164,   164,   372,   372,   372,   243,   372,   243,   372,   372,
     372,   372,   372,   372,   372,     3,    20,    33,    63,   103,
     109,   220,   372,   372,   372,   372,   372,   372,   372,   372,
     372,   372,   372,   372,   372,   372,   372,   372,   372,    69,
     374,   374,   374,   374,   374,   348,   348,   243,   372,   243,
     372,   102,   171,   342,   243,   372,   219,     1,    27,    47,
      51,    76,    79,    91,   109,   114,   172,   362,   363,   364,
     372,   388,   329,   372,   124,   172,   146,   124,   124,   185,
      34,   185,   372,    34,   372,    66,   172,   349,   332,   146,
     172,   223,   372,   124,   333,   372,   330,   262,   219,   324,
     372,   372,   325,   172,   219,   310,   283,   332,   145,   340,
     341,   339,   288,   204,     1,   260,   348,   172,    21,   256,
     170,   172,   172,   346,   374,   172,   346,   342,   243,   372,
     195,   172,   197,   198,   350,   170,   170,   146,   164,    13,
     148,   146,   164,    13,    36,    73,   219,   140,   141,   142,
     143,   144,   158,   162,   167,   207,   296,   297,   298,   372,
     207,   209,   268,   169,   237,   325,   118,   219,   224,   224,
     171,   372,   174,   361,    47,   146,   164,   174,   345,   361,
     342,   174,   345,   122,   146,   174,    32,   224,   361,    32,
      47,   224,   361,   146,   172,   164,   171,     1,   269,   170,
     275,   124,   146,   170,   146,   172,   338,     1,   223,   334,
     335,   105,   311,   171,   300,   372,   140,   148,   300,   300,
     372,   322,   171,   173,   164,   164,   164,   164,   164,   164,
     172,   174,   349,    47,   174,    32,    32,    47,   171,   172,
      47,    91,   146,   172,    17,    20,    25,    45,    52,    63,
      71,    88,    99,   110,   115,   351,    90,    90,   164,   374,
     350,   372,   372,    34,   185,    34,    34,   185,   102,   205,
     219,   172,   172,   331,   338,    69,   374,   219,   172,   172,
     329,   341,   145,   284,   172,   339,   150,   299,   333,   372,
     170,    75,   116,   170,   261,   172,   171,   205,   219,   257,
      47,   174,    47,   146,   172,   213,   220,    17,    18,    20,
      25,    45,    49,    52,    71,    78,    88,    97,    99,   101,
     110,   112,   115,   169,   212,   298,   372,   372,   207,   209,
     146,   164,    13,   164,   169,   269,   321,   329,   372,    47,
     342,   185,   174,   243,   372,   174,   345,   185,   372,   164,
     174,   174,   146,   174,   372,   174,   372,   372,   224,    44,
     372,   243,   372,   224,   249,   205,   349,   170,    83,    86,
     170,   184,   192,   226,   372,   273,   274,   304,   311,    61,
     117,   315,   302,   303,   172,   295,   297,   172,   174,   172,
     243,   372,    44,   243,   372,   349,   363,   338,   351,   351,
     185,   185,   372,   185,    34,   164,   164,   333,   171,   171,
     164,   299,   333,   334,   311,   341,   372,   185,   239,   342,
     257,   145,   205,   243,   372,   243,   372,   198,   207,    13,
      36,    73,   164,   164,   298,   372,   372,   269,   170,   164,
     164,   164,   372,   172,   185,   174,   361,   174,   361,   174,
     185,   122,   372,    32,   224,   361,    32,   224,   361,   172,
     192,   226,   226,   275,   192,   315,     3,    54,    93,   103,
     316,   317,   318,   372,   291,   172,   300,   300,   174,   174,
     372,    32,    32,   172,   333,    34,   185,   348,   348,   299,
     333,    32,   224,   172,   372,   174,   174,   170,   372,   207,
     209,    13,   170,   224,    44,   185,   372,   174,    44,   185,
     372,   174,   372,   102,    44,   372,   224,    44,   372,   224,
     164,   226,   275,   278,   318,   117,   146,   124,   151,   153,
     154,   156,   157,    61,    32,   164,   204,   312,   313,    44,
      44,   102,    44,    44,   185,   172,   172,   185,   205,   164,
     164,   372,   372,   185,   372,   185,   372,   372,   372,   313,
     372,   317,   318,   318,   318,   318,   318,   318,   316,   183,
     372,   372,   372,   372,   163,   163,   102,   102,   102,   102,
     102,   102,   102,   102,   372,   372,   372,   372
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   180,   181,   182,   182,   183,   183,   184,   184,   185,
     185,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   188,   188,   188,   189,   190,
     190,   191,   192,   192,   193,   194,   195,   195,   196,   196,
     196,   197,   197,   198,   199,   200,   200,   200,   201,   201,
     201,   202,   202,   203,   204,   204,   204,   205,   206,   206,
     207,   207,   207,   207,   207,   207,   208,   208,   208,   208,
     209,   209,   210,   210,   210,   210,   210,   211,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   213,   213,   213,   213,   213,   214,
     214,   215,   216,   216,   216,   216,   216,   216,   217,   217,
     217,   218,   218,   218,   218,   218,   219,   219,   219,   220,
     220,   220,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   223,   223,   223,   224,   224,   225,   225,   226,
     226,   227,   226,   228,   226,   229,   229,   229,   229,   229,
     229,   229,   230,   230,   230,   230,   231,   232,   232,   233,
     234,   235,   235,   235,   236,   235,   237,   235,   238,   239,
     240,   240,   241,   241,   241,   241,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   243,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   245,
     245,   246,   247,   247,   248,   248,   249,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     251,   251,   251,   251,   251,   251,   252,   252,   252,   253,
     253,   253,   254,   254,   254,   254,   255,   255,   256,   256,
     256,   257,   257,   258,   259,   259,   260,   260,   261,   261,
     261,   262,   262,   262,   263,   263,   264,   265,   265,   266,
     267,   267,   267,   268,   268,   269,   269,   269,   269,   269,
     270,   270,   271,   272,   272,   273,   272,   272,   274,   272,
     275,   275,   276,   278,   277,   279,   280,   280,   280,   281,
     281,   282,   282,   283,   283,   283,   284,   284,   285,   287,
     286,   288,   286,   289,   291,   290,   292,   292,   292,   292,
     292,   293,   294,   294,   295,   295,   295,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   298,   298,   299,   299,
     300,   300,   301,   301,   302,   302,   303,   303,   304,   305,
     305,   305,   305,   305,   305,   306,   306,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   309,   309,
     309,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   311,   311,   312,   312,   313,   313,   314,   315,
     315,   315,   315,   315,   316,   316,   317,   317,   317,   317,
     317,   317,   317,   318,   318,   318,   319,   320,   320,   321,
     321,   322,   323,   323,   324,   324,   324,   324,   324,   325,
     327,   326,   328,   326,   326,   326,   329,   329,   330,   330,
     331,   331,   332,   332,   332,   333,   333,   333,   334,   335,
     335,   335,   336,   336,   337,   337,   338,   338,   338,   338,
     339,   339,   340,   341,   341,   342,   342,   343,   343,   343,
     344,   344,   345,   345,   346,   347,   347,   348,   348,   349,
     349,   350,   350,   351,   351,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   353,   354,   354,   354,
     354,   354,   354,   354,   355,   356,   356,   356,   356,   356,
     356,   356,   357,   358,   359,   359,   359,   359,   359,   359,
     359,   360,   360,   361,   361,   361,   361,   362,   362,   363,
     363,   363,   364,   364,   364,   364,   364,   364,   364,   364,
     364,   364,   365,   365,   365,   365,   365,   366,   366,   366,
     366,   366,   367,   368,   368,   368,   368,   368,   368,   369,
     370,   371,   371,   372,   372,   372,   372,   372,   372,   372,
     372,   372,   372,   372,   372,   372,   372,   372,   372,   372,
     373,   373,   374,   374,   374,   375,   375,   375,   375,   376,
     376,   376,   376,   376,   377,   377,   377,   378,   378,   378,
     378,   378,   378,   379,   379,   379,   379,   380,   380,   381,
     381,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   383,   383,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   384,   384,
     384,   384,   385,   385,   385,   385,   385,   385,   385,   386,
     386,   386,   386,   387,   387,   387,   387,   388,   388,   388,
     388,   388,   388,   388
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
       1,     6,     7,     3,     0,     6,     0,     6,     2,     0,
       3,     2,     1,     1,     1,     1,     5,     3,     3,     6,
       5,     6,     5,     6,     3,     4,     3,     4,     6,     7,
       6,     7,     4,     5,     4,     5,     4,     4,     3,     6,
       5,     4,     3,     6,     5,     6,     5,     8,     7,     4,
       4,     2,     5,     2,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     6,     4,     7,     5,
       3,     6,     2,     2,     3,     3,     0,     2,     2,     3,
       5,     1,     3,     3,     5,     5,     0,     2,     3,     2,
       4,     4,     3,     1,     1,     3,     3,     5,     5,     2,
       1,     1,     1,     0,     2,     0,     2,     3,     3,     4,
       4,     4,     2,     1,     2,     0,     4,     2,     0,     5,
       1,     3,     1,     0,     8,     0,     1,     1,     1,     0,
       1,     1,     3,     1,     3,     1,     0,     2,     6,     0,
       3,     0,     4,     1,     0,     7,     4,     4,     6,     6,
       4,     2,     1,     3,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       0,     3,     2,     3,     1,     3,     0,     1,     1,     4,
       5,     4,     5,     6,     6,     0,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     0,     1,     1,
       2,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     0,     1,     1,     2,     1,     1,     1,     1,     2,
       1,     1,     0,     1,     1,     1,     1,     2,     1,     0,
       2,     2,     4,     4,     1,     3,     3,     3,     3,     3,
       3,     3,     2,     1,     1,     1,     3,     1,     2,     1,
       3,     2,     0,     2,     1,     2,     1,     1,     1,     1,
       0,     5,     0,     6,     3,     3,     1,     3,     3,     5,
       1,     3,     1,     2,     3,     0,     2,     2,     1,     1,
       1,     1,     2,     1,     0,     1,     0,     2,     2,     1,
       1,     1,     2,     0,     1,     1,     3,     2,     3,     2,
       2,     3,     1,     2,     1,     3,     3,     0,     1,     1,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     4,     5,     2,     2,     1,     2,     1,     2,
       1,     2,     1,     2,     1,     1,     2,     6,     6,     4,
       4,     9,     9,     7,     2,     2,     3,     4,     6,     6,
       9,     9,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     4,     3,     4,     2,     1,     3,     4,
       3,     3,     1,     1,     2,     1,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     2,     8,     8,
       9,     9,     4,     3,     3,     2,     2,     2,     1,     3,
       4,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     4,     4,     4,     3,     3,     3,
       3,     5,     5,     3,     4,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     4,     4,
       5,     4,     5,     5,     6,     4,     5,     3,     5,     3,
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
#line 686 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 5977 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 690 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 5983 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 691 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 5989 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 698 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 5997 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 706 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6005 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 710 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6014 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 720 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6022 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 725 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6030 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 732 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6036 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 733 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6042 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 734 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6048 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 735 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6054 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 736 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6060 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 737 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6066 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 738 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6072 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 739 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6078 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: TDEFER stmt  */
#line 741 "chpl.ypp"
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

  case 21: /* stmt_base: try_stmt  */
#line 758 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6106 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: return_stmt  */
#line 759 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6112 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: TINIT TTHIS TSEMI  */
#line 761 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr));
    auto node = Init::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6124 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 769 "chpl.ypp"
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

  case 25: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 778 "chpl.ypp"
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

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 787 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6158 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 791 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6169 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 798 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6178 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 805 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6184 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 806 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6190 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 807 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6196 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 808 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6202 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 809 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6208 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 810 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6214 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 811 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6220 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 813 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6226 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 815 "chpl.ypp"
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

  case 38: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 835 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6262 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 843 "chpl.ypp"
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

  case 40: /* tryable_stmt: TLOCAL do_stmt  */
#line 860 "chpl.ypp"
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

  case 41: /* tryable_stmt: TON expr do_stmt  */
#line 876 "chpl.ypp"
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

  case 42: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 893 "chpl.ypp"
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

  case 43: /* tryable_stmt: TSERIAL do_stmt  */
#line 910 "chpl.ypp"
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

  case 44: /* tryable_stmt: TSYNC stmt  */
#line 926 "chpl.ypp"
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
#line 952 "chpl.ypp"
  {
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 6395 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 973 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6403 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 980 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6412 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 987 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6420 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 991 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6431 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 1001 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6437 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 1003 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6445 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 1007 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6453 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 1014 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6462 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 1019 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6472 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 1032 "chpl.ypp"
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
#line 6494 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 1053 "chpl.ypp"
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
#line 6515 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 1070 "chpl.ypp"
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
#line 6536 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1087 "chpl.ypp"
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
#line 6555 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1104 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6561 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1105 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6568 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1107 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6575 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1112 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6581 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1113 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6588 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1119 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6598 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1137 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6607 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1142 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6616 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1147 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6629 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1159 "chpl.ypp"
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
#line 6648 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1177 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6654 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1178 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6660 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1183 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6668 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1187 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6676 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1191 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6685 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1196 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6693 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1200 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6701 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1204 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6710 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1212 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6719 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1217 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6729 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1223 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6738 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1228 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6749 "bison-chpl-lib.cpp"
    break;

  case 90: /* opt_only_ls: %empty  */
#line 1236 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6755 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: renames_ls  */
#line 1237 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6761 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1242 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6770 "bison-chpl-lib.cpp"
    break;

  case 93: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1247 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6782 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1255 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6795 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1264 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6807 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1272 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6820 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1283 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6829 "bison-chpl-lib.cpp"
    break;

  case 114: /* import_expr: expr  */
#line 1318 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6837 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr TDOT all_op_name  */
#line 1322 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6846 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1327 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6855 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TAS ident_use  */
#line 1332 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6865 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1338 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6875 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_ls: import_expr  */
#line 1346 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6881 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_ls TCOMMA import_expr  */
#line 1347 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6887 "bison-chpl-lib.cpp"
    break;

  case 121: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1352 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6897 "bison-chpl-lib.cpp"
    break;

  case 122: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1361 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6905 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1365 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6913 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1369 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6921 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1373 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6929 "bison-chpl-lib.cpp"
    break;

  case 126: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1377 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6937 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1381 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6945 "bison-chpl-lib.cpp"
    break;

  case 128: /* opt_label_ident: %empty  */
#line 1389 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6951 "bison-chpl-lib.cpp"
    break;

  case 129: /* opt_label_ident: TIDENT  */
#line 1390 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6957 "bison-chpl-lib.cpp"
    break;

  case 130: /* opt_label_ident: TINIT  */
#line 1391 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6963 "bison-chpl-lib.cpp"
    break;

  case 139: /* ident_use: TIDENT  */
#line 1417 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6969 "bison-chpl-lib.cpp"
    break;

  case 140: /* ident_use: TINIT  */
#line 1418 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6975 "bison-chpl-lib.cpp"
    break;

  case 141: /* ident_use: TTHIS  */
#line 1419 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6981 "bison-chpl-lib.cpp"
    break;

  case 160: /* scalar_type: TBOOL  */
#line 1445 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6987 "bison-chpl-lib.cpp"
    break;

  case 161: /* scalar_type: TENUM  */
#line 1446 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6993 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TINT  */
#line 1447 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6999 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TUINT  */
#line 1448 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7005 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TREAL  */
#line 1449 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7011 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TIMAG  */
#line 1450 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7017 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TCOMPLEX  */
#line 1451 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7023 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TBYTES  */
#line 1452 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7029 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TSTRING  */
#line 1453 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7035 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TLOCALE  */
#line 1454 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7041 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TNOTHING  */
#line 1455 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7047 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TVOID  */
#line 1456 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7053 "bison-chpl-lib.cpp"
    break;

  case 175: /* do_stmt: TDO stmt  */
#line 1469 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7059 "bison-chpl-lib.cpp"
    break;

  case 176: /* do_stmt: block_stmt  */
#line 1470 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7065 "bison-chpl-lib.cpp"
    break;

  case 177: /* return_stmt: TRETURN TSEMI  */
#line 1475 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7076 "bison-chpl-lib.cpp"
    break;

  case 178: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1482 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7087 "bison-chpl-lib.cpp"
    break;

  case 179: /* class_level_stmt: TSEMI  */
#line 1492 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7095 "bison-chpl-lib.cpp"
    break;

  case 180: /* class_level_stmt: inner_class_level_stmt  */
#line 1496 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7105 "bison-chpl-lib.cpp"
    break;

  case 181: /* $@1: %empty  */
#line 1501 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7112 "bison-chpl-lib.cpp"
    break;

  case 182: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1503 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7121 "bison-chpl-lib.cpp"
    break;

  case 183: /* $@2: %empty  */
#line 1507 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7128 "bison-chpl-lib.cpp"
    break;

  case 184: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1509 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7137 "bison-chpl-lib.cpp"
    break;

  case 192: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1527 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7146 "bison-chpl-lib.cpp"
    break;

  case 193: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1532 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7155 "bison-chpl-lib.cpp"
    break;

  case 194: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1537 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7164 "bison-chpl-lib.cpp"
    break;

  case 195: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1542 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7172 "bison-chpl-lib.cpp"
    break;

  case 196: /* forwarding_decl_start: TFORWARDING  */
#line 1549 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7181 "bison-chpl-lib.cpp"
    break;

  case 197: /* extern_or_export: TEXTERN  */
#line 1556 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7187 "bison-chpl-lib.cpp"
    break;

  case 198: /* extern_or_export: TEXPORT  */
#line 1557 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7193 "bison-chpl-lib.cpp"
    break;

  case 199: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1562 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7203 "bison-chpl-lib.cpp"
    break;

  case 200: /* extern_export_kind_inner: TTYPE  */
#line 1575 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7212 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1584 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7222 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1591 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7234 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1599 "chpl.ypp"
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
#line 7256 "bison-chpl-lib.cpp"
    break;

  case 204: /* $@3: %empty  */
#line 1617 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7264 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 1621 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-4])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7273 "bison-chpl-lib.cpp"
    break;

  case 206: /* $@4: %empty  */
#line 1626 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7282 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr extern_export_kind_inner $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1631 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yylsp[-5]), (yyvsp[-3].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7291 "bison-chpl-lib.cpp"
    break;

  case 208: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1639 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7299 "bison-chpl-lib.cpp"
    break;

  case 209: /* no_loop_attributes: %empty  */
#line 1645 "chpl.ypp"
  {
    // Even though there are no attributes, there could be loop pragmas.
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 7308 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1651 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7314 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1652 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7320 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1669 "chpl.ypp"
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

  case 217: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1692 "chpl.ypp"
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

  case 218: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1711 "chpl.ypp"
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

  case 219: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1730 "chpl.ypp"
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

  case 220: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1755 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7431 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: for_loop_kw expr TIN expr task_intent_clause do_stmt  */
#line 1760 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7440 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1765 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7449 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator task_intent_clause do_stmt  */
#line 1770 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7458 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1775 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7467 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw expr task_intent_clause do_stmt  */
#line 1780 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7476 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1785 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7485 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: for_loop_kw zippered_iterator task_intent_clause do_stmt  */
#line 1790 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7494 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1795 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7503 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TLSBR expr_ls TIN expr task_intent_clause TRSBR stmt  */
#line 1800 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7512 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1805 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7521 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator task_intent_clause TRSBR stmt  */
#line 1810 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7530 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1815 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7539 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR expr_ls task_intent_clause TRSBR stmt  */
#line 1820 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7548 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1825 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7558 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TLSBR zippered_iterator task_intent_clause TRSBR stmt  */
#line 1831 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7568 "bison-chpl-lib.cpp"
    break;

  case 236: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1840 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7578 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr TTHEN stmt  */
#line 1849 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7586 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr block_stmt  */
#line 1853 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7594 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1857 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7602 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1861 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7610 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1865 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7618 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF ifvar block_stmt  */
#line 1869 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7626 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1873 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7634 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1877 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7642 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1881 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7651 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1886 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7660 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1891 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7669 "bison-chpl-lib.cpp"
    break;

  case 248: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1896 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7678 "bison-chpl-lib.cpp"
    break;

  case 249: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1904 "chpl.ypp"
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
    BUILDER->noteDeclNameLocation(varDecl.get(), LOC((yylsp[-2])));
    (yyval.expr) = varDecl.release();
  }
#line 7698 "bison-chpl-lib.cpp"
    break;

  case 250: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1920 "chpl.ypp"
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
    BUILDER->noteDeclNameLocation(varDecl.get(), LOC((yylsp[-2])));
    (yyval.expr) = varDecl.release();
  }
#line 7718 "bison-chpl-lib.cpp"
    break;

  case 251: /* interface_start: TINTERFACE ident_def  */
#line 1939 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Interface);
  }
#line 7726 "bison-chpl-lib.cpp"
    break;

  case 252: /* interface_stmt: interface_start TLP ifc_formal_ls TRP block_stmt  */
#line 1946 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-4].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), parts, (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7736 "bison-chpl-lib.cpp"
    break;

  case 253: /* interface_stmt: interface_start block_stmt  */
#line 1952 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-1].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), parts, nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7746 "bison-chpl-lib.cpp"
    break;

  case 254: /* ifc_formal_ls: ifc_formal  */
#line 1960 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7752 "bison-chpl-lib.cpp"
    break;

  case 255: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1961 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7758 "bison-chpl-lib.cpp"
    break;

  case 256: /* ifc_formal: ident_def  */
#line 1966 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7764 "bison-chpl-lib.cpp"
    break;

  case 276: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 2009 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7772 "bison-chpl-lib.cpp"
    break;

  case 277: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 2013 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7780 "bison-chpl-lib.cpp"
    break;

  case 278: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 2017 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7788 "bison-chpl-lib.cpp"
    break;

  case 279: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2024 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7796 "bison-chpl-lib.cpp"
    break;

  case 280: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 2028 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7804 "bison-chpl-lib.cpp"
    break;

  case 281: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2032 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7812 "bison-chpl-lib.cpp"
    break;

  case 282: /* try_stmt: TTRY tryable_stmt  */
#line 2039 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7820 "bison-chpl-lib.cpp"
    break;

  case 283: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2043 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7828 "bison-chpl-lib.cpp"
    break;

  case 284: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2047 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7836 "bison-chpl-lib.cpp"
    break;

  case 285: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2051 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7844 "bison-chpl-lib.cpp"
    break;

  case 286: /* catch_expr_ls: %empty  */
#line 2057 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7850 "bison-chpl-lib.cpp"
    break;

  case 287: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2058 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7856 "bison-chpl-lib.cpp"
    break;

  case 288: /* catch_expr: TCATCH block_stmt  */
#line 2063 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7864 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2067 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7872 "bison-chpl-lib.cpp"
    break;

  case 290: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2071 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7880 "bison-chpl-lib.cpp"
    break;

  case 291: /* catch_expr_inner: ident_def  */
#line 2078 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)), /*attributeGroup*/ nullptr,
                         Decl::DEFAULT_VISIBILITY,
                         Decl::DEFAULT_LINKAGE,
                         /*linkageName*/ nullptr,
                         /*name*/ (yyvsp[0].uniqueStr),
                         Variable::VAR,
                         /*isConfig*/ false,
                         /*isField*/ false,
                         /*typeExpression*/ nullptr,
                         /*initExpression*/ nullptr);
    BUILDER->noteDeclNameLocation(varDecl.get(), LOC((yylsp[0])));
    (yyval.expr) = varDecl.release();
  }
#line 7899 "bison-chpl-lib.cpp"
    break;

  case 292: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2093 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)), /*attributeGroup*/ nullptr,
                         Decl::DEFAULT_VISIBILITY,
                         Decl::DEFAULT_LINKAGE,
                         /*linkageName*/ nullptr,
                         /*name*/ (yyvsp[-2].uniqueStr),
                         Variable::VAR,
                         /*isConfig*/ false,
                         /*isField*/ false,
                         /*typeExpression*/ toOwned((yyvsp[0].expr)),
                         /*initExpression*/ nullptr);
    BUILDER->noteDeclNameLocation(varDecl.get(), LOC((yylsp[-2])));
    (yyval.expr) = varDecl.release();
  }
#line 7918 "bison-chpl-lib.cpp"
    break;

  case 293: /* throw_stmt: TTHROW expr TSEMI  */
#line 2111 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7928 "bison-chpl-lib.cpp"
    break;

  case 294: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2119 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7937 "bison-chpl-lib.cpp"
    break;

  case 295: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2124 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7948 "bison-chpl-lib.cpp"
    break;

  case 296: /* when_stmt_ls: %empty  */
#line 2133 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7954 "bison-chpl-lib.cpp"
    break;

  case 297: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2134 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7960 "bison-chpl-lib.cpp"
    break;

  case 298: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2139 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7968 "bison-chpl-lib.cpp"
    break;

  case 299: /* when_stmt: TOTHERWISE stmt  */
#line 2143 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7977 "bison-chpl-lib.cpp"
    break;

  case 300: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2148 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 7987 "bison-chpl-lib.cpp"
    break;

  case 301: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2157 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 7996 "bison-chpl-lib.cpp"
    break;

  case 302: /* manager_expr: expr TAS ident_def  */
#line 2162 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8004 "bison-chpl-lib.cpp"
    break;

  case 303: /* manager_expr: expr  */
#line 2166 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8012 "bison-chpl-lib.cpp"
    break;

  case 304: /* manager_expr_ls: manager_expr  */
#line 2172 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8018 "bison-chpl-lib.cpp"
    break;

  case 305: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2173 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8024 "bison-chpl-lib.cpp"
    break;

  case 306: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2178 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8033 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2188 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8043 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2194 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8055 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_start: class_tag ident_def  */
#line 2206 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8063 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_tag: TCLASS  */
#line 2212 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8069 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_tag: TRECORD  */
#line 2213 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8075 "bison-chpl-lib.cpp"
    break;

  case 312: /* class_tag: TUNION  */
#line 2214 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8081 "bison-chpl-lib.cpp"
    break;

  case 313: /* opt_inherit: %empty  */
#line 2218 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8087 "bison-chpl-lib.cpp"
    break;

  case 314: /* opt_inherit: TCOLON expr_ls  */
#line 2219 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8093 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_level_stmt_ls: %empty  */
#line 2223 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8102 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2228 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8110 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2232 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8118 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2236 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8126 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2240 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8134 "bison-chpl-lib.cpp"
    break;

  case 320: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
#line 2248 "chpl.ypp"
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
#line 8157 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
#line 2267 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8171 "bison-chpl-lib.cpp"
    break;

  case 322: /* enum_header: TENUM ident_def  */
#line 2280 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8179 "bison-chpl-lib.cpp"
    break;

  case 323: /* enum_ls: enum_item  */
#line 2287 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8188 "bison-chpl-lib.cpp"
    break;

  case 324: /* enum_ls: enum_ls TCOMMA  */
#line 2292 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8198 "bison-chpl-lib.cpp"
    break;

  case 325: /* $@5: %empty  */
#line 2298 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8207 "bison-chpl-lib.cpp"
    break;

  case 326: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2303 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8216 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2308 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8225 "bison-chpl-lib.cpp"
    break;

  case 328: /* $@6: %empty  */
#line 2313 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8234 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2318 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8243 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_item: ident_def  */
#line 2328 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[0])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8255 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_item: ident_def TASSIGN expr  */
#line 2336 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[-2])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8269 "bison-chpl-lib.cpp"
    break;

  case 332: /* lambda_decl_start: TLAMBDA  */
#line 2349 "chpl.ypp"
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
#line 8285 "bison-chpl-lib.cpp"
    break;

  case 333: /* $@7: %empty  */
#line 2366 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8295 "bison-chpl-lib.cpp"
    break;

  case 334: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2372 "chpl.ypp"
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
#line 8318 "bison-chpl-lib.cpp"
    break;

  case 336: /* linkage_spec: linkage_spec_empty  */
#line 2396 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8324 "bison-chpl-lib.cpp"
    break;

  case 337: /* linkage_spec: TINLINE  */
#line 2397 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8331 "bison-chpl-lib.cpp"
    break;

  case 338: /* linkage_spec: TOVERRIDE  */
#line 2399 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8338 "bison-chpl-lib.cpp"
    break;

  case 339: /* opt_fn_type_formal_ls: %empty  */
#line 2404 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8344 "bison-chpl-lib.cpp"
    break;

  case 340: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2405 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8350 "bison-chpl-lib.cpp"
    break;

  case 341: /* fn_type_formal_ls: fn_type_formal  */
#line 2409 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8356 "bison-chpl-lib.cpp"
    break;

  case 342: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2410 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8362 "bison-chpl-lib.cpp"
    break;

  case 343: /* fn_type_formal: named_formal  */
#line 2415 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8368 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2418 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8374 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_type_formal: formal_type  */
#line 2420 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8380 "bison-chpl-lib.cpp"
    break;

  case 346: /* opt_fn_type_ret_type: %empty  */
#line 2424 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8386 "bison-chpl-lib.cpp"
    break;

  case 347: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2425 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8392 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2438 "chpl.ypp"
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
#line 8412 "bison-chpl-lib.cpp"
    break;

  case 349: /* $@8: %empty  */
#line 2457 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8422 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2463 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8435 "bison-chpl-lib.cpp"
    break;

  case 351: /* $@9: %empty  */
#line 2472 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8445 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2478 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8458 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2490 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8466 "bison-chpl-lib.cpp"
    break;

  case 354: /* $@10: %empty  */
#line 2499 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8474 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2503 "chpl.ypp"
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
#line 8497 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2525 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.thisIntentLoc = (yylsp[-2]);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8510 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2534 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.thisIntentLoc = (yylsp[-2]);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8523 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2543 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.thisIntentLoc = (yylsp[-4]);
      fp.receiver = context->buildThisFormal((yylsp[-3]), (yylsp[-4]), (yyvsp[-4].intentTag), (yyvsp[-3].expr), /*initExpr*/ nullptr);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8537 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2553 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.thisIntentLoc = (yylsp[-4]);
      fp.receiver = context->buildThisFormal((yylsp[-3]), (yylsp[-4]), (yyvsp[-4].intentTag), (yyvsp[-3].expr), /*initExpr*/ nullptr);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8551 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2563 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8561 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2572 "chpl.ypp"
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
#line 8577 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2588 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 8586 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_ident: ident_def TBANG  */
#line 2598 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8596 "bison-chpl-lib.cpp"
    break;

  case 408: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2657 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8602 "bison-chpl-lib.cpp"
    break;

  case 409: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2658 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8608 "bison-chpl-lib.cpp"
    break;

  case 410: /* opt_formal_ls: %empty  */
#line 2662 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8614 "bison-chpl-lib.cpp"
    break;

  case 411: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2663 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8620 "bison-chpl-lib.cpp"
    break;

  case 412: /* req_formal_ls: TLP TRP  */
#line 2667 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8626 "bison-chpl-lib.cpp"
    break;

  case 413: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2668 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8632 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_ls_inner: formal  */
#line 2672 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8638 "bison-chpl-lib.cpp"
    break;

  case 415: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2673 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8644 "bison-chpl-lib.cpp"
    break;

  case 416: /* formal_ls: %empty  */
#line 2677 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8650 "bison-chpl-lib.cpp"
    break;

  case 417: /* formal_ls: formal_ls_inner  */
#line 2678 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8656 "bison-chpl-lib.cpp"
    break;

  case 419: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2687 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8664 "bison-chpl-lib.cpp"
    break;

  case 420: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2692 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8672 "bison-chpl-lib.cpp"
    break;

  case 421: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2697 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8680 "bison-chpl-lib.cpp"
    break;

  case 422: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2702 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8688 "bison-chpl-lib.cpp"
    break;

  case 423: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2707 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yylsp[-5]), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8696 "bison-chpl-lib.cpp"
    break;

  case 424: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2712 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8704 "bison-chpl-lib.cpp"
    break;

  case 425: /* opt_formal_intent_tag: %empty  */
#line 2718 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);
  }
#line 8713 "bison-chpl-lib.cpp"
    break;

  case 426: /* opt_formal_intent_tag: required_intent_tag  */
#line 2723 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8722 "bison-chpl-lib.cpp"
    break;

  case 427: /* required_intent_tag: TIN  */
#line 2730 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::IN);            }
#line 8728 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TINOUT  */
#line 2731 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::INOUT);         }
#line 8734 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TOUT  */
#line 2732 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::OUT);           }
#line 8740 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TCONST TIN  */
#line 2733 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_IN);      }
#line 8746 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TCONST TREF  */
#line 2734 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);     }
#line 8752 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TCONST  */
#line 2735 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);         }
#line 8758 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TPARAM  */
#line 2736 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);         }
#line 8764 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TREF  */
#line 2737 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);           }
#line 8770 "bison-chpl-lib.cpp"
    break;

  case 435: /* required_intent_tag: TTYPE  */
#line 2738 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);          }
#line 8776 "bison-chpl-lib.cpp"
    break;

  case 436: /* required_intent_tag: TVAR  */
#line 2739 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR); }
#line 8782 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: %empty  */
#line 2743 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);     }
#line 8788 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TPARAM  */
#line 2744 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);              }
#line 8794 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TREF  */
#line 2745 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);                }
#line 8800 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TCONST TREF  */
#line 2746 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);          }
#line 8806 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TCONST  */
#line 2747 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);              }
#line 8812 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TTYPE  */
#line 2748 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);               }
#line 8818 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_this_intent_tag: TIN  */
#line 2749 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::IN);       }
#line 8824 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_this_intent_tag: TCONST TIN  */
#line 2750 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::CONST_IN); }
#line 8830 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_this_intent_tag: TVAR  */
#line 2751 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR);      }
#line 8836 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_this_intent_tag: TOUT  */
#line 2752 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::OUT);      }
#line 8842 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_this_intent_tag: TINOUT  */
#line 2753 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::INOUT);    }
#line 8848 "bison-chpl-lib.cpp"
    break;

  case 448: /* proc_iter_or_op: TPROC  */
#line 2757 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8854 "bison-chpl-lib.cpp"
    break;

  case 449: /* proc_iter_or_op: TITER  */
#line 2758 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8860 "bison-chpl-lib.cpp"
    break;

  case 450: /* proc_iter_or_op: TOPERATOR  */
#line 2759 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8866 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: %empty  */
#line 2763 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::DEFAULT_RETURN_INTENT); }
#line 8872 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TOUT  */
#line 2764 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::OUT);                   }
#line 8878 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_ret_tag: TCONST  */
#line 2765 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST);                 }
#line 8884 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: TCONST TREF  */
#line 2766 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST_REF);             }
#line 8890 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: TREF  */
#line 2767 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::REF);                   }
#line 8896 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: TPARAM  */
#line 2768 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::PARAM);                 }
#line 8902 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_ret_tag: TTYPE  */
#line 2769 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::TYPE);                  }
#line 8908 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_ret_tag: TIN  */
#line 2770 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::IN);            }
#line 8914 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_ret_tag: TCONST TIN  */
#line 2771 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::CONST_IN);      }
#line 8920 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_ret_tag: TVAR  */
#line 2772 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::VAR);           }
#line 8926 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_ret_tag: TINOUT  */
#line 2773 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::INOUT);         }
#line 8932 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_throws_error: %empty  */
#line 2777 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8938 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_throws_error: TTHROWS  */
#line 2778 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8944 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_function_body_stmt: TSEMI  */
#line 2781 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8950 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_function_body_stmt: function_body_stmt  */
#line 2782 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8956 "bison-chpl-lib.cpp"
    break;

  case 466: /* function_body_stmt: block_stmt_body  */
#line 2786 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8962 "bison-chpl-lib.cpp"
    break;

  case 467: /* function_body_stmt: TDO toplevel_stmt  */
#line 2787 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 8968 "bison-chpl-lib.cpp"
    break;

  case 468: /* query_expr: TQUERIEDIDENT  */
#line 2791 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8974 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_lifetime_where: %empty  */
#line 2796 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8980 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_lifetime_where: TWHERE expr  */
#line 2798 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8986 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2800 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8992 "bison-chpl-lib.cpp"
    break;

  case 472: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2802 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 8998 "bison-chpl-lib.cpp"
    break;

  case 473: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2804 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9004 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_components_expr: lifetime_expr  */
#line 2809 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9010 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2811 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9016 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2816 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9022 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2818 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9028 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2820 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9034 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2822 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9040 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2824 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9046 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2826 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9052 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2828 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9058 "bison-chpl-lib.cpp"
    break;

  case 483: /* lifetime_ident: TIDENT  */
#line 2832 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9064 "bison-chpl-lib.cpp"
    break;

  case 484: /* lifetime_ident: TINIT  */
#line 2833 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9070 "bison-chpl-lib.cpp"
    break;

  case 485: /* lifetime_ident: TTHIS  */
#line 2834 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9076 "bison-chpl-lib.cpp"
    break;

  case 486: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2839 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9085 "bison-chpl-lib.cpp"
    break;

  case 487: /* type_alias_decl_stmt_start: TTYPE  */
#line 2848 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9095 "bison-chpl-lib.cpp"
    break;

  case 488: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2854 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9106 "bison-chpl-lib.cpp"
    break;

  case 489: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2864 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9114 "bison-chpl-lib.cpp"
    break;

  case 490: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2868 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9122 "bison-chpl-lib.cpp"
    break;

  case 491: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2875 "chpl.ypp"
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
#line 9145 "bison-chpl-lib.cpp"
    break;

  case 492: /* opt_init_type: %empty  */
#line 2896 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9151 "bison-chpl-lib.cpp"
    break;

  case 493: /* opt_init_type: TASSIGN expr  */
#line 2898 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9157 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_type: TPARAM  */
#line 2902 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9163 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_type: TCONST TREF  */
#line 2903 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9169 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_type: TREF  */
#line 2904 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9175 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_type: TCONST  */
#line 2905 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9181 "bison-chpl-lib.cpp"
    break;

  case 498: /* var_decl_type: TVAR  */
#line 2906 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9187 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_start: var_decl_type  */
#line 2911 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9196 "bison-chpl-lib.cpp"
    break;

  case 500: /* $@11: %empty  */
#line 2919 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9205 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt: TCONFIG $@11 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2923 "chpl.ypp"
                                              {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-4]), (yylsp[-3])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9214 "bison-chpl-lib.cpp"
    break;

  case 502: /* $@12: %empty  */
#line 2928 "chpl.ypp"
  {
    // Use a mid-rule action to preserve the 'on' expr
    context->noteVarDestinationExpr((yyvsp[0].expr));
  }
#line 9223 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt: TON expr $@12 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2933 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-4])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9232 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_stmt: var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2938 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9241 "bison-chpl-lib.cpp"
    break;

  case 505: /* var_decl_stmt: var_decl_start error TSEMI  */
#line 2943 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    delete (yyvsp[-2].attributeGroup);
    context->resetDeclStateOnError();
  }
#line 9251 "bison-chpl-lib.cpp"
    break;

  case 506: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2952 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9259 "bison-chpl-lib.cpp"
    break;

  case 507: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2956 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9267 "bison-chpl-lib.cpp"
    break;

  case 508: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2963 "chpl.ypp"
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
#line 9290 "bison-chpl-lib.cpp"
    break;

  case 509: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2982 "chpl.ypp"
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
#line 9310 "bison-chpl-lib.cpp"
    break;

  case 510: /* tuple_var_decl_component: ident_def  */
#line 3001 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9318 "bison-chpl-lib.cpp"
    break;

  case 511: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 3005 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9326 "bison-chpl-lib.cpp"
    break;

  case 512: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 3012 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9332 "bison-chpl-lib.cpp"
    break;

  case 513: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 3014 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9338 "bison-chpl-lib.cpp"
    break;

  case 514: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 3016 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9344 "bison-chpl-lib.cpp"
    break;

  case 515: /* opt_init_expr: %empty  */
#line 3022 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9350 "bison-chpl-lib.cpp"
    break;

  case 516: /* opt_init_expr: TASSIGN TNOINIT  */
#line 3023 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9356 "bison-chpl-lib.cpp"
    break;

  case 517: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 3024 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9362 "bison-chpl-lib.cpp"
    break;

  case 518: /* formal_or_ret_type_expr: expr  */
#line 3028 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9368 "bison-chpl-lib.cpp"
    break;

  case 519: /* ret_type: formal_or_ret_type_expr  */
#line 3032 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9374 "bison-chpl-lib.cpp"
    break;

  case 520: /* ret_type: reserved_type_ident_use  */
#line 3033 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9380 "bison-chpl-lib.cpp"
    break;

  case 521: /* ret_type: error  */
#line 3034 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9386 "bison-chpl-lib.cpp"
    break;

  case 522: /* colon_ret_type: TCOLON ret_type  */
#line 3038 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9392 "bison-chpl-lib.cpp"
    break;

  case 523: /* colon_ret_type: error  */
#line 3039 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9398 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_ret_type: %empty  */
#line 3043 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9404 "bison-chpl-lib.cpp"
    break;

  case 526: /* opt_type: %empty  */
#line 3048 "chpl.ypp"
                                 { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9410 "bison-chpl-lib.cpp"
    break;

  case 527: /* opt_type: TCOLON expr  */
#line 3049 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9416 "bison-chpl-lib.cpp"
    break;

  case 528: /* opt_type: TCOLON reserved_type_ident_use  */
#line 3050 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9422 "bison-chpl-lib.cpp"
    break;

  case 529: /* opt_type: error  */
#line 3051 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9428 "bison-chpl-lib.cpp"
    break;

  case 530: /* formal_type: formal_or_ret_type_expr  */
#line 3055 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9434 "bison-chpl-lib.cpp"
    break;

  case 531: /* formal_type: reserved_type_ident_use  */
#line 3056 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9440 "bison-chpl-lib.cpp"
    break;

  case 532: /* colon_formal_type: TCOLON formal_type  */
#line 3060 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9446 "bison-chpl-lib.cpp"
    break;

  case 533: /* opt_colon_formal_type: %empty  */
#line 3064 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9452 "bison-chpl-lib.cpp"
    break;

  case 534: /* opt_colon_formal_type: colon_formal_type  */
#line 3065 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9458 "bison-chpl-lib.cpp"
    break;

  case 535: /* expr_ls: expr  */
#line 3071 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9464 "bison-chpl-lib.cpp"
    break;

  case 536: /* expr_ls: expr_ls TCOMMA expr  */
#line 3072 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9470 "bison-chpl-lib.cpp"
    break;

  case 537: /* expr_ls_end_semi: expr_ls semicolon_list  */
#line 3076 "chpl.ypp"
                         {
    (yyval.ndArrayList) = context->makeNDArrayList();
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElement((yyvsp[-1].exprList), (yylsp[-1])));
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9480 "bison-chpl-lib.cpp"
    break;

  case 538: /* expr_ls_end_semi: expr_ls TCOMMA semicolon_list  */
#line 3081 "chpl.ypp"
                                {
    (yyval.ndArrayList) = context->makeNDArrayList();
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElement((yyvsp[-2].exprList), (yylsp[-2])));
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9490 "bison-chpl-lib.cpp"
    break;

  case 539: /* expr_ls_end_semi: expr_ls_semi semicolon_list  */
#line 3087 "chpl.ypp"
  {
    (yyval.ndArrayList) = (yyvsp[-1].ndArrayList);
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9499 "bison-chpl-lib.cpp"
    break;

  case 540: /* expr_ls_semi: expr_ls_end_semi expr_ls  */
#line 3094 "chpl.ypp"
                           {
    (yyval.ndArrayList) = context->appendNDArrayList((yyvsp[-1].ndArrayList), makeNDArrayElement((yyvsp[0].exprList), (yylsp[0])));
  }
#line 9507 "bison-chpl-lib.cpp"
    break;

  case 541: /* expr_ls_semi: expr_ls_end_semi expr_ls TCOMMA  */
#line 3097 "chpl.ypp"
                                  {
    (yyval.ndArrayList) = context->appendNDArrayList((yyvsp[-2].ndArrayList), makeNDArrayElement((yyvsp[-1].exprList), (yylsp[-1])));
  }
#line 9515 "bison-chpl-lib.cpp"
    break;

  case 542: /* semicolon_list: TSEMI  */
#line 3103 "chpl.ypp"
                       { (yyval.counter) = 1; }
#line 9521 "bison-chpl-lib.cpp"
    break;

  case 543: /* semicolon_list: semicolon_list TSEMI  */
#line 3104 "chpl.ypp"
                       { (yyval.counter) = (yyvsp[-1].counter) + 1; }
#line 9527 "bison-chpl-lib.cpp"
    break;

  case 544: /* tuple_component: opt_try_expr  */
#line 3110 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9533 "bison-chpl-lib.cpp"
    break;

  case 545: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3115 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9541 "bison-chpl-lib.cpp"
    break;

  case 546: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3119 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9549 "bison-chpl-lib.cpp"
    break;

  case 547: /* opt_actual_ls: %empty  */
#line 3125 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9555 "bison-chpl-lib.cpp"
    break;

  case 548: /* opt_actual_ls: actual_ls  */
#line 3126 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9561 "bison-chpl-lib.cpp"
    break;

  case 549: /* actual_ls: actual_expr  */
#line 3131 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9570 "bison-chpl-lib.cpp"
    break;

  case 550: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3136 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9580 "bison-chpl-lib.cpp"
    break;

  case 551: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3144 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9586 "bison-chpl-lib.cpp"
    break;

  case 552: /* actual_expr: opt_try_expr  */
#line 3145 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9592 "bison-chpl-lib.cpp"
    break;

  case 553: /* ident_expr: ident_use  */
#line 3149 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9598 "bison-chpl-lib.cpp"
    break;

  case 554: /* ident_expr: scalar_type  */
#line 3150 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9604 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3163 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9610 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3165 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9616 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3167 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9622 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3169 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9632 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TATOMIC expr  */
#line 3175 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9638 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TSYNC expr  */
#line 3177 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9644 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TOWNED  */
#line 3180 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9650 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TOWNED expr  */
#line 3182 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9656 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TUNMANAGED  */
#line 3184 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9662 "bison-chpl-lib.cpp"
    break;

  case 569: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3186 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9668 "bison-chpl-lib.cpp"
    break;

  case 570: /* sub_type_level_expr: TSHARED  */
#line 3188 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9674 "bison-chpl-lib.cpp"
    break;

  case 571: /* sub_type_level_expr: TSHARED expr  */
#line 3190 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9680 "bison-chpl-lib.cpp"
    break;

  case 572: /* sub_type_level_expr: TBORROWED  */
#line 3192 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9686 "bison-chpl-lib.cpp"
    break;

  case 573: /* sub_type_level_expr: TBORROWED expr  */
#line 3194 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9692 "bison-chpl-lib.cpp"
    break;

  case 574: /* sub_type_level_expr: TCLASS  */
#line 3196 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9698 "bison-chpl-lib.cpp"
    break;

  case 575: /* sub_type_level_expr: TRECORD  */
#line 3198 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9704 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr: no_loop_attributes for_expr_base  */
#line 3202 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9710 "bison-chpl-lib.cpp"
    break;

  case 577: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3207 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9718 "bison-chpl-lib.cpp"
    break;

  case 578: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3211 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9726 "bison-chpl-lib.cpp"
    break;

  case 579: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3215 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9734 "bison-chpl-lib.cpp"
    break;

  case 580: /* for_expr_base: for_loop_kw zippered_iterator TDO expr  */
#line 3219 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "expression-level loops with 'zip' must have an index.");
  }
#line 9742 "bison-chpl-lib.cpp"
    break;

  case 581: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3223 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9755 "bison-chpl-lib.cpp"
    break;

  case 582: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3232 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9768 "bison-chpl-lib.cpp"
    break;

  case 583: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3241 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9781 "bison-chpl-lib.cpp"
    break;

  case 584: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3252 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9787 "bison-chpl-lib.cpp"
    break;

  case 585: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3257 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9795 "bison-chpl-lib.cpp"
    break;

  case 586: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3261 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9803 "bison-chpl-lib.cpp"
    break;

  case 587: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3265 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9811 "bison-chpl-lib.cpp"
    break;

  case 588: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3269 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9819 "bison-chpl-lib.cpp"
    break;

  case 589: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3273 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9827 "bison-chpl-lib.cpp"
    break;

  case 590: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3277 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9835 "bison-chpl-lib.cpp"
    break;

  case 591: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3281 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9843 "bison-chpl-lib.cpp"
    break;

  case 592: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3288 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9857 "bison-chpl-lib.cpp"
    break;

  case 593: /* nil_expr: TNIL  */
#line 3305 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9863 "bison-chpl-lib.cpp"
    break;

  case 601: /* opt_task_intent_ls: %empty  */
#line 3323 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9869 "bison-chpl-lib.cpp"
    break;

  case 602: /* opt_task_intent_ls: task_intent_clause  */
#line 3324 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9875 "bison-chpl-lib.cpp"
    break;

  case 603: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3329 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9883 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_intent_clause: TWITH TLP TRP  */
#line 3333 "chpl.ypp"
  {
    auto err = context->syntax((yyloc), "'with' clause cannot be empty.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-2]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9894 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_intent_clause: TWITH TLP error TRP  */
#line 3340 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[-2]), "invalid intent expression in 'with' clause.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9905 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_intent_clause: TWITH error  */
#line 3347 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[0]), "missing parentheses around 'with' clause intents.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-1]), dummyLoc, dummyLoc, dummyLoc, exprs);
  }
#line 9916 "bison-chpl-lib.cpp"
    break;

  case 607: /* task_intent_ls: intent_expr  */
#line 3356 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9922 "bison-chpl-lib.cpp"
    break;

  case 608: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3357 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9928 "bison-chpl-lib.cpp"
    break;

  case 609: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3362 "chpl.ypp"
  {
    (yyval.expr) = context->buildTaskIntent((yyloc), (yylsp[-2]), nullptr, (yyvsp[-2].expr), (yyvsp[-3].taskIntent), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9936 "bison-chpl-lib.cpp"
    break;

  case 610: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3366 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9944 "bison-chpl-lib.cpp"
    break;

  case 611: /* intent_expr: expr TREDUCE ident_expr  */
#line 3370 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9952 "bison-chpl-lib.cpp"
    break;

  case 612: /* task_var_prefix: TCONST  */
#line 3376 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST);          }
#line 9958 "bison-chpl-lib.cpp"
    break;

  case 613: /* task_var_prefix: TIN  */
#line 3377 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::IN);             }
#line 9964 "bison-chpl-lib.cpp"
    break;

  case 614: /* task_var_prefix: TCONST TIN  */
#line 3378 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_IN);       }
#line 9970 "bison-chpl-lib.cpp"
    break;

  case 615: /* task_var_prefix: TREF  */
#line 3379 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::REF);            }
#line 9976 "bison-chpl-lib.cpp"
    break;

  case 616: /* task_var_prefix: TCONST TREF  */
#line 3380 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_REF);      }
#line 9982 "bison-chpl-lib.cpp"
    break;

  case 617: /* task_var_prefix: TVAR  */
#line 3381 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::VAR);            }
#line 9988 "bison-chpl-lib.cpp"
    break;

  case 618: /* task_var_prefix: TOUT  */
#line 3382 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::OUT);   }
#line 9994 "bison-chpl-lib.cpp"
    break;

  case 619: /* task_var_prefix: TINOUT  */
#line 3383 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::INOUT); }
#line 10000 "bison-chpl-lib.cpp"
    break;

  case 620: /* task_var_prefix: TPARAM  */
#line 3384 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::PARAM); }
#line 10006 "bison-chpl-lib.cpp"
    break;

  case 621: /* task_var_prefix: TTYPE  */
#line 3385 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::TYPE);  }
#line 10012 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_maybe_decorated: TNEW  */
#line 3390 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10018 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_maybe_decorated: TNEW TOWNED  */
#line 3392 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10024 "bison-chpl-lib.cpp"
    break;

  case 624: /* new_maybe_decorated: TNEW TSHARED  */
#line 3394 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10030 "bison-chpl-lib.cpp"
    break;

  case 625: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3396 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10036 "bison-chpl-lib.cpp"
    break;

  case 626: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3398 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10042 "bison-chpl-lib.cpp"
    break;

  case 627: /* new_expr: new_maybe_decorated expr  */
#line 3404 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10050 "bison-chpl-lib.cpp"
    break;

  case 628: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3411 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenLocation((yyvsp[-4].expr), LOC2((yylsp[-5]), (yylsp[-3])));
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
#line 10067 "bison-chpl-lib.cpp"
    break;

  case 629: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3424 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenLocation((yyvsp[-4].expr), LOC2((yylsp[-5]), (yylsp[-3])));
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
#line 10084 "bison-chpl-lib.cpp"
    break;

  case 630: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3437 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenLocation((yyvsp[-5].expr), LOC2((yylsp[-6]), (yylsp[-4])));
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
#line 10102 "bison-chpl-lib.cpp"
    break;

  case 631: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3451 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenLocation((yyvsp[-5].expr), LOC2((yylsp[-6]), (yylsp[-4])));
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
#line 10121 "bison-chpl-lib.cpp"
    break;

  case 632: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3469 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10129 "bison-chpl-lib.cpp"
    break;

  case 633: /* range_literal_expr: expr TDOTDOT expr  */
#line 3476 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10138 "bison-chpl-lib.cpp"
    break;

  case 634: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3481 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10147 "bison-chpl-lib.cpp"
    break;

  case 635: /* range_literal_expr: expr TDOTDOT  */
#line 3486 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10156 "bison-chpl-lib.cpp"
    break;

  case 636: /* range_literal_expr: TDOTDOT expr  */
#line 3491 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10166 "bison-chpl-lib.cpp"
    break;

  case 637: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3497 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10176 "bison-chpl-lib.cpp"
    break;

  case 638: /* range_literal_expr: TDOTDOT  */
#line 3503 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10186 "bison-chpl-lib.cpp"
    break;

  case 639: /* cast_expr: expr TCOLON expr  */
#line 3533 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10194 "bison-chpl-lib.cpp"
    break;

  case 640: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3540 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10202 "bison-chpl-lib.cpp"
    break;

  case 641: /* super_expr: fn_expr  */
#line 3546 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10208 "bison-chpl-lib.cpp"
    break;

  case 645: /* expr: sub_type_level_expr TQUESTION  */
#line 3555 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10214 "bison-chpl-lib.cpp"
    break;

  case 646: /* expr: TQUESTION  */
#line 3557 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10220 "bison-chpl-lib.cpp"
    break;

  case 650: /* expr: fn_type  */
#line 3562 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10226 "bison-chpl-lib.cpp"
    break;

  case 660: /* opt_expr: %empty  */
#line 3576 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10232 "bison-chpl-lib.cpp"
    break;

  case 661: /* opt_expr: expr  */
#line 3577 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10238 "bison-chpl-lib.cpp"
    break;

  case 662: /* opt_try_expr: TTRY expr  */
#line 3581 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10244 "bison-chpl-lib.cpp"
    break;

  case 663: /* opt_try_expr: TTRYBANG expr  */
#line 3582 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10250 "bison-chpl-lib.cpp"
    break;

  case 664: /* opt_try_expr: super_expr  */
#line 3583 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10256 "bison-chpl-lib.cpp"
    break;

  case 670: /* call_base_expr: expr TBANG  */
#line 3600 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10264 "bison-chpl-lib.cpp"
    break;

  case 671: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3603 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10270 "bison-chpl-lib.cpp"
    break;

  case 674: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3610 "chpl.ypp"
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
#line 10286 "bison-chpl-lib.cpp"
    break;

  case 675: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3622 "chpl.ypp"
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
#line 10302 "bison-chpl-lib.cpp"
    break;

  case 676: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3634 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10310 "bison-chpl-lib.cpp"
    break;

  case 677: /* dot_expr: expr TDOT ident_use  */
#line 3641 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10316 "bison-chpl-lib.cpp"
    break;

  case 678: /* dot_expr: expr TDOT TTYPE  */
#line 3643 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10322 "bison-chpl-lib.cpp"
    break;

  case 679: /* dot_expr: expr TDOT TDOMAIN  */
#line 3645 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10328 "bison-chpl-lib.cpp"
    break;

  case 680: /* dot_expr: expr TDOT TLOCALE  */
#line 3647 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10334 "bison-chpl-lib.cpp"
    break;

  case 681: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3649 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10340 "bison-chpl-lib.cpp"
    break;

  case 682: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3651 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10346 "bison-chpl-lib.cpp"
    break;

  case 683: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3660 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10356 "bison-chpl-lib.cpp"
    break;

  case 684: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3666 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10364 "bison-chpl-lib.cpp"
    break;

  case 685: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3670 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10372 "bison-chpl-lib.cpp"
    break;

  case 686: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3674 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10380 "bison-chpl-lib.cpp"
    break;

  case 687: /* bool_literal: TFALSE  */
#line 3680 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10386 "bison-chpl-lib.cpp"
    break;

  case 688: /* bool_literal: TTRUE  */
#line 3681 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10392 "bison-chpl-lib.cpp"
    break;

  case 689: /* str_bytes_literal: STRINGLITERAL  */
#line 3685 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10398 "bison-chpl-lib.cpp"
    break;

  case 690: /* str_bytes_literal: BYTESLITERAL  */
#line 3686 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10404 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: INTLITERAL  */
#line 3692 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10410 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: REALLITERAL  */
#line 3693 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10416 "bison-chpl-lib.cpp"
    break;

  case 695: /* literal_expr: IMAGLITERAL  */
#line 3694 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10422 "bison-chpl-lib.cpp"
    break;

  case 696: /* literal_expr: TNONE  */
#line 3695 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10428 "bison-chpl-lib.cpp"
    break;

  case 697: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3697 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10437 "bison-chpl-lib.cpp"
    break;

  case 698: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3702 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10446 "bison-chpl-lib.cpp"
    break;

  case 699: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3707 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10455 "bison-chpl-lib.cpp"
    break;

  case 700: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3712 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10464 "bison-chpl-lib.cpp"
    break;

  case 701: /* literal_expr: no_loop_attributes TLSBR expr_ls_semi TRSBR  */
#line 3717 "chpl.ypp"
  {
    (yyval.expr) = context->buildNDArray((yyloc), (yyvsp[-1].ndArrayList));
    context->popLoopAttributeGroup();
  }
#line 10473 "bison-chpl-lib.cpp"
    break;

  case 702: /* literal_expr: no_loop_attributes TLSBR expr_ls_semi semicolon_list TRSBR  */
#line 3722 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    (yyval.expr) = context->buildNDArray((yyloc), (yyvsp[-2].ndArrayList));
    context->popLoopAttributeGroup();
  }
#line 10485 "bison-chpl-lib.cpp"
    break;

  case 703: /* literal_expr: no_loop_attributes TLSBR expr_ls semicolon_list TRSBR  */
#line 3730 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    AstList lst;
    lst.push_back(ArrayRow::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))));
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), std::move(lst)).release();
    context->popLoopAttributeGroup();
  }
#line 10499 "bison-chpl-lib.cpp"
    break;

  case 704: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA semicolon_list TRSBR  */
#line 3740 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    AstList lst;
    lst.push_back(ArrayRow::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-3].exprList))));
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), std::move(lst)).release();
    context->popLoopAttributeGroup();
  }
#line 10513 "bison-chpl-lib.cpp"
    break;

  case 705: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3750 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10522 "bison-chpl-lib.cpp"
    break;

  case 706: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3755 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10531 "bison-chpl-lib.cpp"
    break;

  case 707: /* assoc_expr_ls: expr TALIAS expr  */
#line 3764 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10540 "bison-chpl-lib.cpp"
    break;

  case 708: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3769 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10550 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TPLUS expr  */
#line 3777 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10556 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TMINUS expr  */
#line 3778 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10562 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TSTAR expr  */
#line 3779 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10568 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TDIVIDE expr  */
#line 3780 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10574 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3781 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10580 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3782 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10586 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TMOD expr  */
#line 3783 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10592 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TEQUAL expr  */
#line 3784 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10598 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3785 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10604 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3786 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10610 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3787 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10616 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TLESS expr  */
#line 3788 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10622 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr TGREATER expr  */
#line 3789 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10628 "bison-chpl-lib.cpp"
    break;

  case 722: /* binary_op_expr: expr TBAND expr  */
#line 3790 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10634 "bison-chpl-lib.cpp"
    break;

  case 723: /* binary_op_expr: expr TBOR expr  */
#line 3791 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10640 "bison-chpl-lib.cpp"
    break;

  case 724: /* binary_op_expr: expr TBXOR expr  */
#line 3792 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10646 "bison-chpl-lib.cpp"
    break;

  case 725: /* binary_op_expr: expr TAND expr  */
#line 3793 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10652 "bison-chpl-lib.cpp"
    break;

  case 726: /* binary_op_expr: expr TOR expr  */
#line 3794 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10658 "bison-chpl-lib.cpp"
    break;

  case 727: /* binary_op_expr: expr TEXP expr  */
#line 3795 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10664 "bison-chpl-lib.cpp"
    break;

  case 728: /* binary_op_expr: expr TBY expr  */
#line 3796 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10670 "bison-chpl-lib.cpp"
    break;

  case 729: /* binary_op_expr: expr TALIGN expr  */
#line 3797 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10676 "bison-chpl-lib.cpp"
    break;

  case 730: /* binary_op_expr: expr THASH expr  */
#line 3798 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10682 "bison-chpl-lib.cpp"
    break;

  case 731: /* binary_op_expr: expr TDMAPPED expr  */
#line 3799 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10688 "bison-chpl-lib.cpp"
    break;

  case 732: /* unary_op_expr: TPLUS expr  */
#line 3803 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10694 "bison-chpl-lib.cpp"
    break;

  case 733: /* unary_op_expr: TMINUS expr  */
#line 3804 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10700 "bison-chpl-lib.cpp"
    break;

  case 734: /* unary_op_expr: TMINUSMINUS expr  */
#line 3805 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10706 "bison-chpl-lib.cpp"
    break;

  case 735: /* unary_op_expr: TPLUSPLUS expr  */
#line 3806 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10712 "bison-chpl-lib.cpp"
    break;

  case 736: /* unary_op_expr: TBANG expr  */
#line 3807 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10718 "bison-chpl-lib.cpp"
    break;

  case 737: /* unary_op_expr: expr TBANG  */
#line 3808 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10726 "bison-chpl-lib.cpp"
    break;

  case 738: /* unary_op_expr: TBNOT expr  */
#line 3811 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10732 "bison-chpl-lib.cpp"
    break;

  case 739: /* reduce_expr: expr TREDUCE expr  */
#line 3816 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10740 "bison-chpl-lib.cpp"
    break;

  case 740: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3820 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10748 "bison-chpl-lib.cpp"
    break;

  case 741: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3824 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10756 "bison-chpl-lib.cpp"
    break;

  case 742: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3828 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10764 "bison-chpl-lib.cpp"
    break;

  case 743: /* scan_expr: expr TSCAN expr  */
#line 3835 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10772 "bison-chpl-lib.cpp"
    break;

  case 744: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3839 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10780 "bison-chpl-lib.cpp"
    break;

  case 745: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3843 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10788 "bison-chpl-lib.cpp"
    break;

  case 746: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3847 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10796 "bison-chpl-lib.cpp"
    break;


#line 10800 "bison-chpl-lib.cpp"

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
