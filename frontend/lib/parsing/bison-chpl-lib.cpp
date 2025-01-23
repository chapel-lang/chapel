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
  YYSYMBOL_tuple_component = 343,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 344,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 345,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 346,                /* actual_ls  */
  YYSYMBOL_actual_expr = 347,              /* actual_expr  */
  YYSYMBOL_ident_expr = 348,               /* ident_expr  */
  YYSYMBOL_sub_type_level_expr = 349,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 350,                 /* for_expr  */
  YYSYMBOL_for_expr_base = 351,            /* for_expr_base  */
  YYSYMBOL_bracket_loop_expr = 352,        /* bracket_loop_expr  */
  YYSYMBOL_bracket_loop_expr_base = 353,   /* bracket_loop_expr_base  */
  YYSYMBOL_cond_expr = 354,                /* cond_expr  */
  YYSYMBOL_nil_expr = 355,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 356,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 357,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 358,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 359,           /* task_intent_ls  */
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
#line 355 "chpl.ypp"

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
#define YYLAST   17816

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  180
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  206
/* YYNRULES -- Number of rules.  */
#define YYNRULES  740
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1263

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
       0,   664,   664,   668,   669,   674,   675,   683,   687,   697,
     702,   709,   710,   711,   712,   713,   714,   715,   716,   717,
     718,   736,   737,   738,   746,   755,   764,   768,   775,   783,
     784,   785,   786,   787,   788,   789,   790,   792,   812,   820,
     837,   853,   870,   887,   903,   924,   925,   926,   930,   936,
     937,   941,   945,   946,   950,   957,   965,   968,   978,   980,
     985,   991,   996,  1005,  1009,  1030,  1047,  1064,  1082,  1083,
    1085,  1090,  1091,  1096,  1114,  1119,  1124,  1136,  1155,  1156,
    1160,  1164,  1168,  1173,  1177,  1181,  1189,  1194,  1200,  1205,
    1214,  1215,  1219,  1224,  1232,  1241,  1249,  1260,  1268,  1269,
    1270,  1271,  1272,  1273,  1274,  1275,  1276,  1277,  1278,  1279,
    1280,  1281,  1282,  1283,  1295,  1299,  1304,  1309,  1315,  1324,
    1325,  1329,  1338,  1342,  1346,  1350,  1354,  1358,  1367,  1368,
    1369,  1373,  1374,  1375,  1376,  1377,  1381,  1382,  1383,  1395,
    1396,  1397,  1402,  1403,  1404,  1405,  1406,  1407,  1408,  1409,
    1410,  1411,  1412,  1413,  1414,  1415,  1416,  1417,  1418,  1419,
    1423,  1424,  1425,  1426,  1427,  1428,  1429,  1430,  1431,  1432,
    1433,  1434,  1441,  1442,  1443,  1447,  1448,  1452,  1459,  1469,
    1473,  1479,  1479,  1485,  1485,  1494,  1495,  1496,  1497,  1498,
    1499,  1500,  1504,  1509,  1514,  1519,  1526,  1534,  1535,  1539,
    1552,  1560,  1567,  1576,  1595,  1594,  1604,  1603,  1616,  1623,
    1629,  1630,  1639,  1640,  1641,  1642,  1646,  1669,  1688,  1707,
    1732,  1737,  1742,  1747,  1752,  1757,  1762,  1767,  1772,  1777,
    1782,  1787,  1792,  1797,  1802,  1808,  1817,  1826,  1830,  1834,
    1838,  1842,  1846,  1850,  1854,  1858,  1863,  1868,  1873,  1881,
    1897,  1916,  1923,  1929,  1938,  1939,  1944,  1949,  1950,  1951,
    1952,  1953,  1954,  1955,  1956,  1957,  1958,  1959,  1960,  1965,
    1970,  1971,  1972,  1973,  1981,  1982,  1986,  1990,  1994,  2001,
    2005,  2009,  2016,  2020,  2024,  2028,  2035,  2036,  2040,  2044,
    2048,  2055,  2070,  2088,  2096,  2101,  2111,  2112,  2116,  2120,
    2125,  2134,  2139,  2143,  2150,  2151,  2155,  2165,  2171,  2183,
    2190,  2191,  2192,  2196,  2197,  2201,  2205,  2209,  2213,  2217,
    2225,  2244,  2257,  2264,  2269,  2276,  2275,  2285,  2291,  2290,
    2305,  2313,  2326,  2344,  2341,  2370,  2374,  2375,  2377,  2382,
    2383,  2387,  2388,  2392,  2395,  2397,  2402,  2403,  2414,  2435,
    2434,  2450,  2449,  2467,  2477,  2474,  2502,  2511,  2520,  2530,
    2540,  2549,  2564,  2565,  2573,  2574,  2575,  2584,  2585,  2586,
    2587,  2588,  2589,  2590,  2591,  2592,  2593,  2594,  2595,  2596,
    2597,  2598,  2599,  2600,  2601,  2602,  2603,  2604,  2605,  2606,
    2607,  2608,  2612,  2613,  2614,  2615,  2616,  2617,  2618,  2619,
    2620,  2621,  2622,  2623,  2628,  2629,  2633,  2634,  2638,  2639,
    2643,  2644,  2648,  2649,  2653,  2654,  2658,  2662,  2666,  2671,
    2676,  2681,  2686,  2694,  2698,  2706,  2707,  2708,  2709,  2710,
    2711,  2712,  2713,  2714,  2715,  2719,  2720,  2721,  2722,  2723,
    2724,  2725,  2726,  2727,  2728,  2729,  2733,  2734,  2735,  2739,
    2740,  2741,  2742,  2743,  2744,  2745,  2746,  2747,  2748,  2749,
    2753,  2754,  2757,  2758,  2762,  2763,  2767,  2772,  2773,  2775,
    2777,  2779,  2784,  2786,  2791,  2793,  2795,  2797,  2799,  2801,
    2803,  2808,  2809,  2810,  2814,  2823,  2829,  2839,  2843,  2850,
    2872,  2873,  2878,  2879,  2880,  2881,  2882,  2886,  2895,  2894,
    2904,  2903,  2913,  2918,  2927,  2931,  2938,  2957,  2976,  2980,
    2987,  2989,  2991,  2998,  2999,  3000,  3004,  3008,  3009,  3010,
    3014,  3015,  3019,  3020,  3024,  3025,  3026,  3027,  3031,  3032,
    3036,  3040,  3041,  3047,  3048,  3052,  3056,  3060,  3067,  3068,
    3072,  3077,  3086,  3087,  3091,  3092,  3099,  3100,  3101,  3102,
    3103,  3104,  3106,  3108,  3110,  3116,  3118,  3121,  3123,  3125,
    3127,  3129,  3131,  3133,  3135,  3137,  3139,  3144,  3148,  3152,
    3156,  3160,  3164,  3173,  3182,  3194,  3198,  3202,  3206,  3210,
    3214,  3218,  3222,  3229,  3247,  3255,  3256,  3257,  3258,  3259,
    3260,  3261,  3265,  3266,  3270,  3274,  3281,  3288,  3298,  3299,
    3303,  3307,  3311,  3318,  3319,  3320,  3321,  3322,  3323,  3324,
    3325,  3326,  3327,  3331,  3333,  3335,  3337,  3339,  3345,  3352,
    3365,  3378,  3392,  3410,  3417,  3422,  3427,  3432,  3438,  3444,
    3474,  3481,  3488,  3489,  3493,  3495,  3496,  3498,  3500,  3501,
    3502,  3503,  3506,  3507,  3508,  3509,  3510,  3511,  3512,  3513,
    3514,  3518,  3519,  3523,  3524,  3525,  3529,  3530,  3531,  3532,
    3541,  3542,  3545,  3546,  3547,  3551,  3563,  3575,  3582,  3584,
    3586,  3588,  3590,  3592,  3601,  3607,  3611,  3615,  3622,  3623,
    3627,  3628,  3632,  3633,  3634,  3635,  3636,  3637,  3638,  3643,
    3648,  3653,  3658,  3663,  3672,  3677,  3686,  3687,  3688,  3689,
    3690,  3691,  3692,  3693,  3694,  3695,  3696,  3697,  3698,  3699,
    3700,  3701,  3702,  3703,  3704,  3705,  3706,  3707,  3708,  3712,
    3713,  3714,  3715,  3716,  3717,  3720,  3724,  3728,  3732,  3736,
    3743,  3747,  3751,  3755,  3763,  3764,  3765,  3766,  3767,  3768,
    3769
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
  "opt_colon_formal_type", "expr_ls", "tuple_component", "tuple_expr_ls",
  "opt_actual_ls", "actual_ls", "actual_expr", "ident_expr",
  "sub_type_level_expr", "for_expr", "for_expr_base", "bracket_loop_expr",
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

#define YYTABLE_NINF (-741)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -915,   101,  4215,  -915,   -59,    99,  -915,  -915,  -915,  -915,
    -915,  -915,  -915, 11967,     1,    70,   103, 13782,   141, 17583,
       1,   240,    63,   235,    70,  5607, 11967,   145, 17642,  -915,
     335,   273,  -915, 10074,   300,   218,   198,  -915,   323,   312,
   17642, 17642, 17642,  -915,  2741, 10932,   344, 11967, 11967,   205,
    -915,   381,   398, 11967,  -915, 13782,  -915, 11967,   379,   278,
     242,   247, 13139,   403, 17701,  -915, 11967,  8190, 11967, 10932,
   13782,   364,   466,   369,  5607,   498, 11967,   502,  7164,  7164,
    -915,   521,  -915, 13782,  -915,   535, 10245,  -915, 11967,  -915,
   11967,  -915,  -915, 13611, 11967,  -915, 10416,  -915,  -915,  -915,
    4563,  8361, 11967,  -915,  5259,  -915,  -915,  -915,  -915, 17356,
     601,  -915,   463,   451,  -915,    34,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  7506,
    -915,  7677,  -915,  -915,   102,  -915,  -915,   376,  -915,   565,
    -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,   478, 17642,
    -915,   455,   456,   330,  -915,   225,  -915,  -915,  -915,   675,
     709,  -915,  -915, 17642,  -915,  1851,  -915,   462,   467,  -915,
    -915,  -915,   472,   473, 11967,   476,   480,  -915,  -915,  -915,
   17025,  2100,   375,   483,   484,  -915,  -915,   401,  -915,  -915,
    -915,  -915,  -915,   169,  -915,  -915,  -915, 11967,  -915, 17642,
    -915, 11967, 11967,    23,   573,   402,  -915,  -915,  -915,  -915,
   17025,   407,  -915,  -915,    38,  5607,  -915,  -915,  -915,   485,
     -37,   489,  -915,   224,  -915,   488,  -915,   -76, 17025,  8532,
    -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915, 17642, 17642,
       2,  3789,  -915,  -915,   546,  8532,   490,   482,  -915,  5607,
    3029,   109,     8,  -915,  5607,  -915,  -915, 15070,  -915,    52,
   15483,   610,  -915,   492,   494,  -915, 15070,   -37,   610,  -915,
    8532,  1730,  1730,   180,   -11,  -915,   106,  -915,  -915,  -915,
    -915,  7848,  -915,  -915,   537,  -915,   487,   522,  -915,  -915,
    3133,   516,  -915,  -915, 17025,   148, 11967, 11967,  -915,    11,
    -915,  -915, 17025,   503, 15602,  -915, 15070,   -37,   499,  8532,
    -915, 17025, 15642, 11967,  -915,  -915,  -915,  -915,  -915,   -37,
     505,   318,   318,  2028,   610,   610,   292,  -915,  -915,  4737,
     -63, 11967,   525,   159,   501,  2028,   665,  -915,  -915, 17642,
    -915, 11967,  -915,  4563,  -915,   108,  -915,   546,  -915,  -915,
     672,   506,  4911, 11967,  -915, 11967,   612,  -915,  -915,  4033,
      57,   478, 17025,   290,  -915,  5607,   597,  -915,  -915, 10074,
   10587, 11103,  -915,  -915,  -915, 17642,  -915, 17642, 11967,   510,
    -915, 16603,   263,   675,  -915,  -915,  -915,  -915,   194,  -915,
    -915,  -915,  -915,  -915,  -915,  -915,    84,   209,  -915,  -915,
    -915,  -915,  -915,  -915,  -915, 13445,   558,   222,  -915,   520,
     268,   428,  -915,   539, 11967, 11967, 11967, 11103, 11103, 11967,
     436, 11967, 11967, 11967, 11967, 11967,   308, 13611, 11967, 11967,
   11967, 11967, 11967, 11967, 11967, 11967, 11967, 11967, 11967, 11967,
   11967, 11967, 11967, 11967,   620,  -915,  -915,  -915,  -915,  -915,
   10245, 10245,  -915,  -915,  -915,  -915, 10245,  -915,  -915, 10245,
   10245,  8532,  8703, 11103, 11103, 15682,   519,  -915, 11274, 11103,
   17642,  -915,  6648,  -915,  -915,  -915,  2741,  -915, 11967,  -915,
     568,   527,   547,  -915,  -915,   577,   581,  5607,   674,  5607,
    -915,   676, 11967,   646,   541,  -915, 10245,  -915,  3029,  -915,
    -915,   191,  -915, 12138,   590, 11967,  2741,  -915,  -915, 11967,
    -915, 17433, 11967, 11967,  -915,   224,   543,  -915,  -915,  -915,
    -915, 17642,  -915,   675, 13310,  3029,   572, 12138,  -915, 17025,
   17025,  -915,   489,  -915,    48,  -915,  8532,   549,  -915, 15070,
     697,   697,  -915,  -915,  -915,  -915, 11448,  -915, 15761,  8877,
    9048,  -915,  -915,  -915, 17025, 11103, 11103,  -915,   463,  9219,
    -915,   336,  -915,  5085,  -915,   304, 15801,   319, 14838, 17642,
    6990,  6819,  -915,   478,   556,  -915,  -915,   224,  -915,   609,
   17642,    62, 15070,   559, 13924,   -45,    12, 15842,   -65,    50,
   14673,  -915,   206,  -915,   -58,   585,  1623,   571, 17524,   614,
     112,  -915,  -915,   213,  -915,  -915,    61,  -915,  -915,  -915,
    3990,  -915,   637,  -915,  -915,   574,   595,  -915,   598,  -915,
     600,   604,   611,   613,   623,  -915,   626,   630,   631,   632,
     633,   634,   644,  -915,  -915,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,  -915,  -915, 11967,   628,   648,   574,  -915,
     574,  -915, 11967,  -915, 17642,  -915,  -915,  -915,  2783,  2783,
     431,  -915,   431,  -915,   431,   650,   985,   390,  1411,   -37,
     318,  -915,   439,  -915,  -915,  -915,  -915,  -915,  2028, 15562,
     431,   745,   745,  2783,   745,   745,  1948,   318, 15562,  1308,
    1948,   610,   610,   318,  2028,   641,   647,   649,   655,   657,
     658,   652,   575,  -915,   431,  -915,   431, 11967, 10245,    19,
     788, 15154,   654,   660,   229,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,   219,  -915,   984, 17104,   231,   332, 17025,
   10245,  -915, 10245, 11967, 11967,   799,  5607,   804, 15993,  5607,
   15233, 17642,  -915,   241,   244,  3029,   109,  -915, 17025,  9390,
    -915, 17025,  -915,  -915,  -915, 17642, 16033, 16073,  2741,  -915,
     572,   695,  -915,   261, 12138,  -915,   251,  -915, 11967,  -915,
     671,   -17,   677,  -915,  2310,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,   123,    80, 14747,  -915,  -915,   272,  -915,
    -915,  -915,  -915, 11967,  -915,    97, 14023, 11967,  -915, 11967,
    6990,  6819,  -915,  9561,   367,  9732,   420,   435,  9903,  8019,
     442,   340,  -915,  -915,  -915, 16152,   698,   679,   682,  -915,
   17642,  2741, 11967,   805,  -915,  -915, 11967, 17025,  5607,   684,
   11103, 11619,  4389,   685, 11967, 11793,  -915,  5607,  -915,    62,
    5781, 11103,  -915,    62, 17642,   489,  -915, 10245,   683,  1994,
    -915,  -915, 11967,    33,  -915,   656,  -915,   637,  -915,  -915,
    -915,  -915,  -915,    10,   615,  -915, 16192,  -915, 14181,  -915,
    -915, 17025,  -915,   689,   688,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,   288, 11103, 13924, 11967, 12309, 11103, 10245,
    -915,  -915,  -915,  7335,  -915,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,  -915,  -915,  -915,    39, 11103, 11103,  -915,
    -915,  -915, 17025, 17025,  5607,  -915,  5607, 11967,  -915,  5607,
     812,   702,   703,  -915,  -915,   590,  -915,  -915,  -915,   699,
     700,   347,   251, 11967,   637,   572,  -915, 10761,  -915,  -915,
   17025,  -915,  5607, 11967,  -915,  -915,  -915, 17642,  -915,   724,
     489, 11103,  5607, 11103, 10245,  -915,  -915,  -915,  -915,  -915,
     439,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,  -915,  6990,  -915,  -915, 16240, 15324,   351,
     708,  6990,  -915, 11967,  -915,  -915,  3250,   352,   356, 16343,
   11967,   297,  -915,  5607,   -44, 15398,  -915,  -915, 17025,  5607,
   17025,  -915, 16391, 17025,  -915, 10074, 17025,   105, 14912,  -915,
    -915,  -915,   301,  -915,  -915,  -915,  -915, 17262, 17309,  -915,
   17025, 17642,   601,  -915,    10,    64, 11967,  -915,   727,   705,
    -915,   574,   574,  -915,  -915,  -915,   706, 16431, 11967,   842,
   16582,   305,  -915,   590,   307,   309,  -915,  -915, 17025,   847,
    5607,  -915,  -915,  -915,  8532,  8532,  -915,  -915,  -915,  -915,
    -915,   251, 17025,  -915,   150,    93,   710, 11967,  -915,    20,
   15443,    80, 14996,  -915,   152, 11967,  6990,  6819,  -915,  -915,
    -915, 16622, 17025,  3438,  -915,  -915,  -915,  -915, 15070,  -915,
    -915,  5955,   717,  6129,   722,  -915, 11967, 14264,  6303,  -915,
      62,  6477,  -915,    62,   719, 17309,  -915,  -915,  -915, 17524,
    -915,  -915,  -915,   184,  -915,   172,  -915,   285, 16663,    83,
    -915,  -915,  -915, 12480, 12651, 16778, 12822, 12993,  -915,  -915,
    5607,  -915,   726,   728,  -915,  -915,  5607,  -915,   489, 17025,
    5607,  5607,  -915, 17025,   363,   735, 11967,  -915,  -915, 10074,
    -915, 17025,  5607, 10074,  -915, 17025,  5607, 17025,  5607, 10074,
   17025,  -915, 10074, 17025,  -915,  -915,  -915,  -915,    65,  -915,
   11967,    64,   184,   184,   184,   184,   184,   184,    64,  5433,
    -915,  -915,  -915,  -915, 11967, 11967, 11967, 11967, 11967,  -915,
     738,   743,   609,  -915,  -915,  -915, 17025, 14344,  -915, 14424,
    -915, 15993, 14504, 14584,  -915, 17025,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,   762,  -915, 16826, 16866, 16945, 16985,  -915,
    -915,  5607,  5607,  5607,  5607, 11967, 11967, 11967, 11967, 15993,
   15993, 15993, 15993
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   139,   466,    55,   684,   685,
     686,   680,   681,   209,   592,   128,   160,   563,   167,   565,
     592,   166,   498,   495,   128,     0,   209,   274,   161,   198,
     197,   678,   196,   209,   165,    68,   275,   337,   162,   140,
       0,     0,     0,   332,     0,   209,   169,   209,   209,   613,
     584,   687,   170,   209,   338,   557,   492,   209,     0,     0,
     183,   181,   423,   164,   566,   494,   209,   209,   209,   209,
     561,     0,   168,     0,     0,   141,   209,   679,   209,   209,
     485,   163,   312,   559,   496,   171,   209,   736,   209,   738,
     209,   739,   740,   629,   209,   737,   209,   637,   179,   735,
       0,   209,   209,     4,     0,     5,    10,    11,    12,    48,
      51,    52,    56,     0,    45,    71,    13,    77,    14,    15,
      16,    17,    29,   544,   545,    22,    46,   180,   190,   209,
     199,   651,   191,    18,     0,    31,    30,     0,    47,     0,
     269,    19,   647,    21,    35,    32,    33,   189,   313,     0,
     187,     0,     0,   644,   336,     0,   641,   185,   353,   449,
     435,   639,   188,     0,   497,     0,   186,   656,   635,   634,
     638,   548,   546,     0,   209,   645,   646,   650,   649,   648,
       0,   547,     0,   657,   658,   659,   682,   683,   640,   550,
     549,   642,   643,     0,    28,   565,   161,   209,   140,     0,
     566,   209,   209,     0,     0,   644,   656,   546,   645,   646,
     555,   547,   657,   658,     0,     0,   593,   129,   130,     0,
     564,     0,   486,     0,   493,     0,    20,     0,   533,   209,
     136,   142,   153,   148,   147,   155,   133,   146,   156,   143,
     137,   157,   131,   158,   151,   145,   152,   149,   150,   132,
     134,   144,   154,   159,   138,   322,   135,   208,     0,     0,
       0,     0,    70,    69,    71,   209,     0,     0,   251,     0,
       0,     0,     0,   504,     0,   176,    40,     0,   304,     0,
     303,   721,   617,   614,   615,   616,   500,   558,   722,     7,
     209,   335,   335,   430,   173,   425,   174,   426,   427,   431,
     432,   172,   433,   434,   423,   529,     0,   340,   341,   343,
       0,   424,   528,   345,   516,     0,   209,   209,   177,   641,
     632,   655,   633,     0,     0,    43,     0,   562,     0,   209,
      44,   556,     0,   209,   282,    48,   286,   283,   286,   560,
       0,   723,   725,   627,   720,   719,     0,    74,    78,     0,
       0,   209,     0,     0,   535,   628,     0,     6,   310,     0,
     197,   209,   311,     0,    49,     0,     9,    71,    50,    53,
       0,    60,     0,   209,    72,   209,     0,   498,   195,     0,
     680,   313,   652,   204,   215,     0,   212,   213,   214,   209,
     209,   209,   211,   567,   575,     0,   253,     0,   209,     0,
     309,     0,   423,   449,   447,   448,   446,   361,   451,   456,
     459,   450,   454,   453,   455,   458,     0,   439,   441,   445,
     444,   436,   437,   440,   443,     0,   490,     0,   487,     0,
       0,   636,    34,   618,   209,   209,   209,   209,   209,   209,
     724,   209,   209,   209,   209,   209,     0,   626,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   392,   399,   400,   401,   396,   398,
     209,   209,   394,   397,   395,   393,   209,   403,   402,   209,
     209,   209,   209,   209,   209,     0,     0,   212,   209,   209,
       0,   597,     0,    36,    24,    37,     0,    25,   209,    38,
     544,     0,   539,   540,   543,     0,     0,     0,   242,     0,
     392,   238,   209,     0,     0,    23,   209,    26,     0,   508,
     510,     0,   527,   209,   513,   209,     0,   175,    39,   209,
     306,     0,   209,   209,    41,     0,     0,   184,   182,   428,
     429,     0,   424,   449,   423,     0,   531,   209,   121,   653,
     654,   351,     0,   178,     0,    42,   209,     0,   293,     0,
     284,   285,    27,    76,    75,    79,   209,   688,     0,   209,
     209,   676,   674,     8,   500,   209,   209,   210,    56,   209,
      54,     0,    65,     0,   119,     0,   114,     0,    86,     0,
     209,   209,   192,   313,     0,   200,   206,     0,   203,     0,
       0,     0,     0,     0,   576,     0,     0,   533,     0,     0,
       0,   256,     0,   254,   280,   314,     0,     0,     0,   330,
       0,   323,   410,     0,   412,   416,     0,   457,   452,   521,
       0,   523,   460,   442,   438,   408,   136,   388,   142,   386,
     148,   147,   146,   143,   137,   390,   157,   158,   145,   149,
     132,   144,   159,   385,   367,   370,   368,   369,   391,   380,
     371,   384,   376,   374,   387,   375,   373,   378,   383,   372,
     377,   381,   382,   379,   389,   209,   364,     0,   408,   365,
     408,   362,   209,   489,     0,   484,   503,   502,   716,   715,
     718,   727,   726,   731,   730,   712,   709,   710,   711,   630,
     699,   139,     0,   670,   671,   141,   669,   668,   624,   703,
     714,   708,   706,   717,   707,   705,   697,   702,   704,   713,
     696,   700,   701,   698,   625,     0,     0,     0,     0,     0,
       0,     0,     0,   729,   728,   733,   732,   209,   209,     0,
       0,     0,   280,     0,   603,   604,   610,   609,   611,   606,
     612,   608,   595,     0,   598,     0,     0,     0,     0,   534,
     209,   552,   209,   209,   209,   241,     0,   237,     0,     0,
       0,     0,   551,     0,     0,   511,     0,   526,   525,   209,
     506,   623,   505,   305,   302,     0,     0,     0,     0,   667,
     531,   346,   342,     0,   209,   532,   513,   344,   209,   350,
       0,     0,     0,   553,     0,   287,   689,   631,   675,   536,
     535,   677,   537,     0,     0,     0,    57,    58,     0,    61,
      63,    67,    66,   209,    97,     0,     0,   209,    92,   209,
     209,   209,    64,   209,   367,   209,   368,   369,   209,   209,
     379,     0,   404,   405,    81,    80,    91,     0,     0,   315,
       0,     0,   209,     0,   218,   217,   209,   577,     0,     0,
     209,   209,     0,     0,   209,   209,   692,     0,   226,     0,
       0,   209,   224,     0,     0,     0,   277,   209,     0,   335,
     321,   327,   209,   325,   320,   423,   411,   460,   519,   518,
     517,   520,   461,   467,   423,   360,     0,   366,     0,   356,
     357,   491,   488,     0,     0,   127,   125,   126,   124,   123,
     122,   665,   666,     0,   209,   690,   209,   209,   209,   209,
     596,   605,   607,   209,   594,   160,   167,   166,   165,   162,
     169,   170,   164,   168,   163,   171,     0,   209,   209,   499,
     542,   541,   250,   249,     0,   244,     0,   209,   240,     0,
     246,     0,   279,   509,   512,   513,   514,   515,   301,     0,
       0,     0,   513,   209,   460,   531,   530,   209,   419,   417,
     352,   295,     0,   209,   294,   297,   554,     0,   288,   291,
       0,   209,     0,   209,   209,    59,   120,   117,    98,   109,
     104,   103,   102,   111,    99,   112,   107,   101,   108,   105,
     106,   100,   110,   113,   209,   116,   115,    88,    87,     0,
       0,   209,   193,   209,   194,   315,   335,     0,     0,     0,
     209,     0,   234,     0,     0,     0,   691,   232,   578,     0,
     694,   693,     0,   571,   227,   209,   570,     0,     0,   225,
     255,   252,     0,   308,   183,   181,   307,   335,   335,   316,
     331,     0,     0,   413,   467,     0,   209,   354,   415,     0,
     363,   408,   408,   672,   673,   279,     0,     0,   209,     0,
       0,     0,   599,   513,   656,   656,   243,   239,   583,   245,
       0,    73,   276,   507,   209,   209,   501,   420,   418,   347,
     348,   513,   407,   299,     0,     0,     0,   209,   289,     0,
       0,     0,     0,    62,     0,   209,   209,   209,    93,    95,
      84,    83,    82,   335,   201,   207,   205,   216,     0,   236,
     235,     0,     0,     0,     0,   233,   209,     0,     0,   222,
       0,     0,   220,     0,   281,   335,   318,   317,   326,     0,
     333,   481,   482,     0,   483,   469,   472,     0,   468,     0,
     409,   358,   359,   209,   209,     0,   209,   209,   281,   600,
       0,   248,     0,     0,   422,   421,     0,   298,     0,   292,
       0,     0,   118,    89,     0,     0,   209,   202,   219,   209,
     230,   580,     0,   209,   228,   579,     0,   695,     0,   209,
     569,   223,   209,   568,   221,   278,   319,   329,     0,   480,
     209,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     462,   464,   355,   463,   209,   209,   209,   209,   209,   247,
     619,   620,   300,   290,    94,    96,    85,     0,   231,     0,
     229,   574,     0,     0,   334,   471,   473,   474,   477,   478,
     479,   475,   476,   470,   465,     0,     0,     0,     0,   621,
     622,     0,     0,     0,     0,   209,   209,   209,   209,   582,
     581,   573,   572
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -915,  -915,  -915,    -1,   -49,  2718,  -915,   393,   806,  -915,
    -915,   515,  -397,  -105,  -915,   338,  -915,  -915,   -71,  -915,
    -915,    43,   653,  -915,  -541,  2981,   542,  -573,  -915,  -800,
    -915,  -915,  -915,    95,  -915,  -915,  -915,   895,  -915,  1592,
    -221,  -915,  -915,  -455,   -42,  -915,  -843,  -915,  -915,   311,
    -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,  -915,    17,
    -915,  -149,   555,  -348,  -915,   533,  -915,  -915,  -915,    51,
    1548,  -915,  -915,  -915,  -915,   586,  -915,   -50,  -915,  -915,
    -915,  -915,   397,  -915,  -915,  -915,  -101,  -915,  -347,  -803,
    -915,  -915,  -915,  -915,  -915,  -611,  -915,   136,  -915,  -915,
    -915,  -915,  -915,   384,  -915,   116,  -915,  -915,  -915,  -915,
     550,  -915,  -915,  -915,  -915,    31,  -411,  -175,  -789,  -914,
    -658,  -915,    36,  -915,    49,   -53,   635,   -52,  -915,  -915,
    -362,  -836,  -915,  -261,  -915,  -116,  -268,  -260,  -809,  -915,
    -915,    96,   264,  -915,   418,  -205,   821,  -915,  -915,  -163,
     425,   178,  -431,  -767,  -606,  -915,  -915,  -915,  -600,  -514,
    -915,  -758,   -14,    45,  -915,  -237,  -495,  -554,    -2,  -915,
    -915,  -915,  -915,  -915,  -915,   348,  -915,   934,  -549,  -915,
      32,  -915,  -915,   465,   560,  -915,  -915,  -915,  -915,  2519,
    -915,   -51,  1031,  -915,  1274,  1475,  -915,  -915,  -915,  -915,
    -915,  -915,  -915,  -915,  -915,  -477
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   348,   104,   105,   106,   107,   108,   365,
     366,   109,   110,   111,   112,   371,   580,   818,   819,   113,
     114,   115,   376,   116,   117,   118,   349,   846,   587,   847,
     119,   120,  1005,   584,   585,   121,   122,   219,   254,   271,
     123,   256,   124,   305,   534,   125,   126,   292,   291,   127,
     128,   129,   130,   131,   596,   132,   597,   850,   133,   203,
     135,   391,   392,   605,   136,   260,   137,   138,   612,   613,
     204,   140,   141,   142,   143,   560,   805,   980,   144,   145,
     801,   975,   278,   279,   146,   147,   148,   149,   399,   879,
     150,   151,   620,  1051,  1052,   621,   152,   205,  1198,   154,
     155,   306,   307,   308,   964,   156,   320,   552,   798,   157,
     158,  1149,   159,   160,   677,   678,   842,   843,   844,   968,
     895,   403,   623,  1059,   624,   625,   310,   542,   425,   407,
     416,   893,  1212,  1213,   161,  1057,  1145,  1146,  1147,   162,
     163,   427,   428,   683,   164,   165,   166,   223,   535,   272,
     273,   520,   521,   780,   312,   891,   631,   632,   524,   313,
     795,   796,   350,   352,   353,   501,   502,   503,   206,   168,
     169,   393,   170,   394,   171,   207,   173,   215,   216,   753,
     754,   755,   174,   208,   209,   177,   178,   179,   321,   180,
     383,   504,   211,   182,   212,   213,   185,   186,   187,   188,
     608,   189,   190,   191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   103,   430,   276,   618,   369,   480,   881,   500,   309,
     311,   799,   227,   304,   679,   757,   323,   841,   496,   134,
     899,   773,   900,   167,   890,   820,   887,   325,   514,   969,
     381,  1010,   962,   797,   594,   340,  1049,  1006,  -328,   491,
     522,   626,   134,   609,   500,  -274,  1016,   384,  1087,   800,
     354,  1054,   315,   536,   489,   525,   859,   863,   972,   860,
     869,   873,   522,   487,   387,   388,   914,  1141,   777,   500,
     498,  1055,   167,   217,   214,   214,   167,   167,   264,   358,
     373,   865,   867,   566,   274,   629,   512,   774,   499,   691,
     693,   134,   557,  -524,   274,   134,   134,  1209,   167,   973,
     701,     3,   167,   440,   507,   194,   876,   567,   500,   866,
     522,   446,   274,   877,   793,  1209,  -522,   134,  1142,   374,
     214,   134,  -524,  -296,   218,   274,   384,  1056,  1090,   858,
    1121,   214,   384,   551,   385,   733,   735,  1128,   153,   214,
     385,   740,   386,   387,   388,  -522,   362,   375,   386,   387,
     388,   198,   367,   974,   526,  -257,  -524,  1143,   861,  -258,
     229,   153,  -275,  -524,  -296,   861,  -524,  1144,    82,   214,
     981,   363,   222,  1049,   384,   889,   955,  1164,  -524,  -324,
    -349,   791,  1166,   319,   523,  -524,   862,  1141,  1083,  -522,
     386,   387,   388,   915,  1170,  1088,   488,  -264,   529,   214,
     705,  -522,   319,  -324,  1136,  1137,   523,  1091,   941,   492,
     153,  -524,  1113,   167,   153,   153,   576,   319,  -296,   363,
     389,   363,  1110,   282,   214,   707,   389,   539,   814,   630,
    -524,   363,   134,  -524,   363,   528,   153,   530,  1142,   498,
     153,   627,   214,   913,   731,   732,   848,  1210,  -522,   363,
     680,    23,   363,  -522,   523,  -524,   633,  1009,   883,   483,
     500,   500,   363,   484,   863,   869,   873,   167,   389,   498,
    1049,   540,   167,  -524,   363,   390,   921,   265,   404,   593,
     966,   575,   884,   283,   555,   628,   134,  1144,   -70,  1200,
     293,   134,  1196,   -69,   498,   500,  -263,   982,  1011,   405,
     634,   262,   284,    56,   263,   570,  1159,  1175,   -70,   406,
     295,   701,   548,   -69,   297,    65,   229,   285,  1201,   802,
     922,   938,  1172,   390,  1165,   484,   224,   -70,   702,  -272,
     788,   571,   -69,   758,  1199,   500,  1073,   775,    84,   298,
      37,   703,   299,  -335,    58,   319,   257,   167,   565,   436,
     172,   153,   874,   304,   300,   -70,  -262,  1089,   500,   885,
     -69,   167,   198,   776,  -335,   923,   134,    54,   684,   265,
     167,   704,   302,   172,  -335,   779,   606,   303,   875,  -259,
     134,   319,  1042,   167,   615,   886,   685,   762,   289,   134,
     775,   924,   851,  1237,  1238,  1239,  1240,  1241,  1242,   595,
    -266,   967,   134,  1151,  1152,   153,   319,   775,   437,  1202,
     153,   705,   438,   952,   526,   266,   953,   706,   984,   726,
     727,   436,   172,   681,  1071,   728,   172,   172,   729,   730,
     820,  1104,   687,   965,   762,   622,  1203,  -270,  1204,  1205,
    1138,  1206,  1207,   498,   985,   319,   757,   762,   172,   290,
     823,   762,   172,  -602,  -267,  -601,   194,  -738,   440,  -261,
    1065,  -738,   563,   444,   328,   827,   446,   175,   824,  1119,
     449,   334,   337,  1134,   739,  1122,  1124,  1158,   526,  -602,
     437,  -601,  1048,   828,   438,   153,  1011,   679,  1130,  1133,
     175,   309,   311,   526,  -589,   304,   939,  1011,   684,   153,
     194,  -663,   526,  -663,  1012,   167,   821,   167,   153,  1011,
    -739,  1086,  1024,   369,  -739,  1108,  1115,   500,   810,   810,
    1116,   153,  -265,  1037,   134,  -740,   134,  1224,  1197,  -740,
     440,   441,  -735,  1174,   443,   444,  -735,   445,   446,   175,
     329,   500,   449,   175,   175,   363,   481,   395,   482,   456,
    1122,  1124,  1130,  1133,  -271,   460,   461,   462,  -273,   854,
     855,   813,   176,   172,   293,   175,  1066,   868,   872,   175,
    1069,   440,  -664,  -663,  -664,  -663,   444,  -260,  -660,   446,
    -660,   167,   565,   449,   295,   176,   319,   319,   297,   691,
     733,  -268,   319,   335,   335,   319,   319,   319,   319,  -662,
     134,  -662,   537,   538,   987,   707,     7,  -661,  1211,  -661,
     903,   370,   904,   298,   809,   812,   299,   172,   356,  1048,
     372,   397,   172,   398,   401,   961,  -586,   402,   300,   490,
     431,   374,   319,  1099,   176,  1101,  -585,   432,   176,   176,
    -590,   436,   293,   153,  -591,   153,   302,  -588,  -587,   494,
    1135,   303,   497,   516,   515,  1139,   500,  1211,   363,   543,
     176,   547,   295,   532,   176,   533,   297,   553,   544,   562,
     556,   569,   319,   572,   573,   578,   600,   579,   589,   616,
     175,   436,   682,   293,   686,   319,   319,   446,  1018,   725,
     738,   298,   760,   762,   299,   319,    58,   172,   500,   761,
     437,   763,   408,   295,   438,   764,   300,   297,   766,   940,
     769,   172,   771,   772,   779,   789,  1048,   794,   804,   153,
     172,   803,   409,  1062,   302,   849,   410,   852,   957,   303,
     856,   498,   298,   172,   175,   299,   417,    58,   882,   175,
     437,   880,   892,  -139,   438,   894,  -160,   300,  -167,   912,
     440,   411,  -166,   936,   412,   444,   418,   445,   446,  -165,
     419,  -162,   449,   500,   167,   302,   413,   167,   897,   456,
     303,  -140,   868,   872,  -169,   176,   436,   462,  -170,  -164,
    -168,  -141,  -163,   134,   414,   420,   134,  -414,   421,   415,
     440,   441,  -171,   442,   443,   444,   898,   445,   446,   447,
     422,   448,   449,   450,   451,   905,   453,   454,   455,   456,
     457,   906,   459,   907,   175,   460,   461,   462,   423,   908,
     916,   909,   910,   424,   911,   919,   463,  1034,   175,   176,
    1047,  1039,   920,   944,   176,   437,   304,   175,   946,   438,
     963,   971,  1021,  1014,  1011,   304,  1080,  1162,  1163,   976,
     175,  1015,  1020,  1043,   319,   172,   167,   172,  1023,  1029,
     167,  1063,  1064,   500,   500,   167,  1081,  1082,   167,  1097,
    1084,  1085,  1109,   885,  1156,   134,   319,  1150,   319,   134,
    1153,  1160,  1168,  1195,   134,   440,   441,   134,   442,   443,
     444,  1182,   445,   446,   447,   319,  1186,   449,  1220,  1225,
    1221,  1249,   153,   455,   456,   153,  1250,   459,  1201,   176,
     460,   461,   462,  1103,   583,   364,   816,   513,   986,   225,
     577,   463,   601,   176,   561,  1040,   783,  1096,   792,  1061,
    1058,   172,   176,   598,  1053,  1074,  1075,  1234,  1140,   541,
    1243,  1236,   167,   369,   167,   176,  1017,   167,   902,   785,
     378,   782,   512,   954,   221,  1072,     0,     0,     0,  1095,
       0,   134,     0,   134,     0,     0,   134,  1047,     0,     0,
     167,     0,   175,     0,   175,     0,     0,     0,     0,     0,
     167,     0,     0,     0,     0,     0,     0,   701,     0,  1094,
       0,     0,     0,   319,   153,  1129,  1132,     0,   153,   134,
       0,   925,     0,   153,   926,     0,   153,     0,     0,   927,
       0,     0,     0,     0,     0,     0,   436,     0,     0,   196,
       0,   167,     0,     0,     0,     0,     0,   167,     0,   928,
     369,     0,     0,   181,   369,   319,   929,     0,   198,     0,
     134,     0,     0,     0,     0,     0,   134,   930,   175,     0,
       0,     0,   512,  1167,   512,   931,   181,   512,   512,  1129,
    1132,     0,     0,     0,  1047,     0,     0,   176,     0,   176,
       0,     0,   932,     0,     0,   437,  1178,     0,   167,   438,
     153,     0,   153,   933,     0,   153,     0,   705,  1191,     0,
       0,  1194,     0,     0,   934,     0,     0,   134,     0,   935,
     319,     0,     0,     0,     0,   181,     0,     0,   153,   181,
     181,     0,     0,     0,   172,     0,     0,   172,   153,   167,
       0,   167,     0,     0,     0,   440,   167,     0,     0,   167,
     444,   181,   445,   446,     0,   181,     0,   449,   134,     0,
     134,     0,     0,   176,   456,   134,     0,     0,   134,     0,
     460,   461,   462,     0,     0,     0,     0,     0,   167,   153,
       0,     0,     0,     0,   167,   153,     0,     0,   167,   167,
       0,     0,     0,     0,     0,     0,     0,   134,     0,     0,
     167,     0,     0,   134,   167,     0,   167,   134,   134,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   134,
     319,   319,     0,   134,     0,   134,   172,   167,  1244,     0,
     172,     0,     0,     0,     0,   172,   153,     0,   172,     0,
       0,     0,     0,     0,     0,     0,   134,     0,     0,     0,
       0,   175,     0,     0,   175,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   181,     0,     0,   167,
     167,   167,   167,     0,     0,     0,     0,   153,     0,   153,
       0,     0,     0,     0,   153,     0,     0,   153,   134,   134,
     134,   134,     0,     0,     0,     0,   183,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   172,     0,   172,     0,   153,   172,     0,   183,
     181,     0,   153,     0,     0,   181,   153,   153,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   153,     0,
     172,     0,   153,   175,   153,     0,   176,   175,     0,   176,
     172,     0,   175,     0,     0,   175,     0,     0,     0,   436,
       0,     0,     0,     0,     0,   153,     0,     0,   183,     0,
       0,     0,   183,   183,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   172,     0,     0,   183,     0,     0,   172,   183,     0,
     181,     0,     0,     0,     0,     0,     0,   153,   153,   153,
     153,     0,     0,     0,   181,     0,     0,     0,   437,     0,
       0,     0,   438,   181,     0,     0,     0,     0,     0,   175,
       0,   175,     0,     0,   175,     0,   181,     0,   176,     0,
       0,     0,   176,     0,     0,     0,     0,   176,   172,     0,
     176,   439,     0,     0,     0,     0,     0,   175,     0,     0,
       0,     0,   436,     0,     0,     0,     0,   175,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,     0,   448,
     449,   450,   451,     0,   453,   454,   455,   456,   457,   172,
     459,   172,     0,   460,   461,   462,   172,   184,     0,   172,
       0,     0,     0,     0,   463,     0,     0,     0,   175,   183,
       0,     0,     0,     0,   175,     0,     0,     0,     0,     0,
     184,   437,     0,     0,   176,   438,   176,     0,   172,   176,
       0,     0,     0,     0,   172,     0,     0,     0,   172,   172,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     172,     0,   176,     0,   172,     0,   172,     0,   181,     0,
     181,     0,   176,   183,     0,   175,     0,     0,   183,   184,
     139,   440,   441,   184,   184,     0,   444,   172,   445,   446,
       0,     0,     0,   449,     0,     0,     0,     0,     0,     0,
     456,     0,     0,   139,     0,   184,   460,   461,   462,   184,
       0,     0,     0,   176,     0,     0,   175,     0,   175,   176,
       0,     0,     0,   175,     0,     0,   175,     0,     0,   172,
     172,   172,   172,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   181,     0,     0,     0,     0,     0,
     255,     0,   139,   183,   878,   175,     0,     0,  -315,     0,
       0,   175,   267,   268,   269,   175,   175,   183,     0,     0,
     176,     0,     0,     0,     0,  -315,   183,   175,   139,  -315,
    -315,   175,   139,   175,     0,     0,     0,     0,  -315,   183,
    -315,  -315,     0,     0,     0,     0,  -315,     0,     0,     0,
       0,     0,     0,  -315,   175,     0,  -315,     0,     0,     0,
       0,   176,     0,   176,     0,     0,     0,     0,   176,     0,
     184,   176,     0,     0,     0,  -315,     0,  -315,     0,     0,
    -315,     0,  -315,     0,  -315,     0,  -315,  -315,     0,  -315,
       0,     0,  -315,     0,  -315,     0,   175,   175,   175,   175,
     176,     0,     0,     0,     0,     0,   176,     0,     0,     0,
     176,   176,  -315,     0,  -315,     0,     0,  -315,     0,     0,
       0,   400,   176,     0,   184,     0,   176,     0,   176,   184,
       0,     0,   358,     0,     0,   426,    22,    23,     0,     0,
       0,     0,     0,   139,     0,   359,     0,    29,   360,   176,
       0,     0,     0,    32,     0,     0,     0,     0,     0,     0,
      37,   183,     0,   183,     0,     0,     0,  -315,     0,     0,
       0,   486,     0,  -315,     0,     0,     0,   181,     0,     0,
     181,     0,   361,     0,     0,     0,     0,    54,     0,    56,
       0,   176,   176,   176,   176,     0,     0,   139,     0,   362,
       0,    65,   139,     0,   184,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   184,    80,
       0,    82,     0,     0,    84,     0,     0,   184,     0,     0,
     505,   506,   429,     0,   230,     0,     0,   183,     0,     0,
     184,     0,   519,     0,     0,     0,     0,     0,   231,   232,
       0,   233,     0,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,   235,     0,     0,     0,     0,   181,
     236,     0,     0,   181,     0,     0,   237,   139,   181,     0,
     238,   181,   546,   239,     0,   240,     0,     0,     0,     0,
       0,   139,     0,     0,   241,     0,     0,     0,     0,     0,
     139,   242,   243,     0,     0,     0,     0,     0,     0,   244,
       0,     0,     0,   139,     0,     0,     0,     0,     0,   245,
       0,     0,     0,     0,     0,     0,     0,     0,   246,     0,
     247,   255,   248,     0,   249,     0,     0,   250,     0,     0,
       0,   251,     0,   252,     0,     0,   253,     0,     0,     0,
       0,     0,     0,     0,     0,   181,     0,   181,     0,   436,
     181,     0,   184,     0,   184,     0,     0,   611,     0,   614,
       0,     0,     0,   619,     0,     0,     0,     0,     0,     7,
       0,     0,     0,   181,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   181,     0,     0,   358,   676,     0,     0,
      22,    23,   270,     0,     0,     0,     0,     0,     0,   359,
       0,    29,   360,     0,     0,     0,     0,    32,   437,     0,
     183,     0,   438,   183,    37,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   181,   139,     0,   139,   184,   436,
     181,     0,     0,     0,     0,     0,   361,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,  1044,     0,     0,
    1045,     0,   742,   362,     0,    65,     0,     0,   440,   441,
       0,   442,   443,   444,     0,   445,   446,     0,     0,     0,
     449,     0,     0,    80,     0,    82,     0,   456,    84,     0,
     519,   181,     0,   460,   461,   462,     0,     0,   437,     0,
       0,     0,   438,   784,     0,     0,     0,     0,     0,     0,
       0,   139,   183,   790,     0,     0,   183,   519,     0,     0,
       0,   183,     0,     0,   183,     0,     0,     0,     0,     0,
       0,     0,   181,     0,   181,     0,     0,     0,    98,   181,
       0,     0,   181,     0,  1046,     0,     0,     0,   440,   441,
       0,   442,   443,   444,     0,   445,   446,     0,     0,     0,
     449,   832,     0,     0,     0,     0,   455,   456,     0,     0,
     459,   181,   853,   460,   461,   462,     0,   181,     0,     0,
       0,   181,   181,     0,     0,     0,     0,     0,     0,     0,
     619,     0,     0,   181,     0,     0,     0,   181,   183,   181,
     183,     0,     0,   183,   464,   465,   466,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   478,     0,
     181,   184,     0,     0,   184,     0,   183,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   183,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   479,     0,
       0,  -660,     0,  -660,     0,     0,   426,     0,     0,     0,
       0,     0,   181,   181,   181,   181,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   183,     0,     0,
       0,     0,     0,   183,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   230,   139,     0,     0,   139,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   231,   232,     0,
     233,     0,     0,   184,     0,   234,     0,   184,     0,     0,
       0,     0,   184,   235,     0,   184,     0,     0,     0,   236,
       0,     0,     0,     0,   183,   237,     0,     0,     0,   238,
       0,     0,   239,   951,   240,     0,     0,   519,     0,     0,
       0,     0,     0,   241,     0,     0,     0,   958,     0,     0,
     242,   243,     0,     0,     0,     0,     0,     0,   244,     0,
       0,     0,     0,     0,     0,   183,   979,   183,   245,     0,
       0,     0,   183,     0,     0,   183,   139,   246,     0,   247,
     139,   248,     0,   249,     0,   139,   250,     0,   139,   184,
     251,   184,   252,     0,   184,   253,     0,     0,     0,     0,
       0,     0,     0,     0,   183,     0,     0,     0,     0,     0,
     183,     0,   426,     0,   183,   183,     0,   184,     0,     0,
       0,     0,     0,     0,     0,     0,   183,   184,     0,     0,
     183,     0,   183,     0,     0,     0,   611,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
       0,   977,     0,   183,     0,     0,     0,     0,     0,     0,
     676,     0,   139,     0,   139,     0,     0,   139,   184,     0,
       0,     0,     0,     0,   184,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     139,     0,     0,     0,     0,   183,   183,   183,   183,     0,
     139,     0,   210,     0,     0,     0,   220,     0,     0,     0,
       0,     0,     0,     0,     0,   228,     0,     0,     0,     0,
       0,     0,   261,     0,     0,   184,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,   280,   281,     0,   979,
       0,   139,   286,     0,   287,     0,   288,   139,     0,     0,
       0,   314,     0,     0,     0,   228,   322,   324,   326,   327,
       0,     0,     0,   331,     0,   332,   184,     0,   184,     0,
       0,     0,   339,   184,     0,   322,   184,   341,     0,   342,
       0,     0,   343,   344,     0,   345,     0,     0,     0,   228,
     322,   355,     0,     0,     0,     0,     0,     0,   139,     0,
       0,     0,     0,     0,     0,   184,     0,     0,     0,     0,
       0,   184,     0,   619,     0,   184,   184,     0,   379,     0,
     382,     0,     0,     0,     0,     0,     0,   184,     0,     0,
       0,   184,     0,   184,     0,     0,     0,     0,     0,   139,
       0,   139,     0,     0,     0,     0,   139,     0,     0,   139,
       0,     0,     0,     0,   184,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   139,     0,
       0,     0,     0,     0,   139,     0,   485,     0,   139,   139,
     331,   228,     0,     0,     0,     0,   184,   184,   184,   184,
     139,   619,     0,     0,   139,     0,   139,     0,     0,     0,
       0,     0,     0,   226,   230,     0,     0,     0,   322,     0,
       0,     0,     0,     0,     0,     0,     0,   139,   231,   232,
       0,   233,     0,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,   235,     0,     0,     0,     0,     0,
     236,     0,     0,     0,   322,     0,   237,     0,     0,     0,
     238,     0,   330,   239,     0,   240,     0,     0,     0,   139,
     139,   139,   139,     0,   241,     0,     0,     0,     0,   322,
       0,   242,   243,     0,   436,     0,     0,     0,     0,   244,
     331,     0,   357,     0,     0,     0,     0,     0,     0,   245,
       0,     0,     0,     0,     0,   549,   550,     0,   246,     0,
     247,     0,   248,     0,   249,     0,     0,   250,   322,     0,
       0,   251,   559,   252,     0,     0,   253,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     568,     0,     0,   437,     0,     0,     0,   438,     0,     0,
     574,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   586,     0,   588,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,     0,   602,   607,
     610,     0,   270,     0,     0,     0,     0,   228,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   493,   448,   449,   450,   451,     0,   453,
     454,   455,   456,   457,   458,   459,     0,     0,   460,   461,
     462,     0,     0,   688,   689,   690,   692,   694,   695,   463,
     696,   697,   698,   699,   700,     0,   708,   709,   710,   711,
     712,   713,   714,   715,   716,   717,   718,   719,   720,   721,
     722,   723,   724,     0,     0,     0,     0,   517,     0,   322,
     322,     0,   527,     0,     0,   322,     0,     0,   322,   322,
     322,   322,   734,   736,     0,     0,     0,   607,   741,     0,
       0,   756,     0,     0,     0,     0,     0,   759,     0,     0,
       0,     0,     0,     0,     0,     0,   275,     0,   768,     0,
       0,   770,   230,     0,     0,   322,     0,     0,     0,     0,
       0,     0,   778,     0,   781,     0,   231,   232,   280,   233,
     275,   786,   787,     0,   234,     0,     0,     0,     0,   336,
     338,     0,   235,   314,     0,     0,   314,     0,   236,     0,
       0,     0,     0,     0,   237,   322,     0,     0,   238,     0,
       0,   239,     0,   240,     0,   759,     0,     0,   322,   322,
     368,     0,   241,     0,   228,   815,     0,     0,   322,   242,
     243,     0,     0,   599,     0,     0,     0,   244,     0,   845,
     845,     0,     0,     0,     0,     0,     0,   245,   396,     0,
       0,     0,     0,   857,     0,     0,   246,     0,   247,     0,
     248,     0,   249,     0,     0,   250,   230,     0,     0,   251,
       0,   252,     0,     0,   253,     0,     0,     0,     0,   314,
     231,   232,     0,   233,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,   235,     0,     0,     0,
       0,     0,   236,     0,     0,     0,     0,     0,   237,     0,
       0,     0,   238,     0,     0,   239,     0,   240,     0,     0,
       0,     0,     0,     0,   896,     0,   241,     0,     0,     0,
     518,   901,   495,   242,   243,     0,     0,     0,     0,     0,
       0,   244,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   245,     0,     0,     0,   765,     0,   767,     0,     0,
     246,     0,   247,     0,   248,     0,   249,     0,     0,   250,
       0,   508,   511,   251,     0,   252,     0,     0,   253,     0,
       0,     0,     0,     0,     0,     7,   768,   322,   275,     0,
     275,     0,     0,     0,     0,     0,     0,   275,     0,     0,
       0,     0,   358,     0,     0,     0,    22,    23,     0,   322,
       0,   322,   942,   943,     0,   359,     0,    29,   360,     0,
       0,     0,     0,    32,     0,     0,     0,     0,   322,     0,
      37,     0,     0,     0,   545,     0,     0,   275,     0,     0,
       0,     0,     0,   314,     0,     0,     0,   970,     0,     0,
       0,     0,   361,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,  1044,     0,     0,  1045,     0,     0,   362,
       0,    65,   586,     0,     0,     0,  1007,     0,  1008,   845,
     845,     0,   341,     0,   342,     0,     0,   344,   345,    80,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,  1019,     0,     0,     0,   228,     0,     0,     0,  1025,
     759,  1028,     0,  1030,  1032,     0,  1033,     0,     0,  1036,
    1038,     0,     0,     0,     0,     0,   322,     0,     0,     0,
       0,  1050,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
    1114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1067,  1028,  1033,  1036,  1070,   322,     0,
       0,     0,   756,     7,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   692,   734,     0,     0,
     358,     0,     0,     0,    22,    23,  1078,     0,     0,     0,
       0,     0,     0,   359,     0,    29,   360,     0,     0,     0,
       0,    32,   314,     0,   945,     0,  1092,   948,    37,     0,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
    1100,     0,  1102,   322,     0,     0,     0,     0,     0,     0,
     361,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,  1044,     0,   845,  1045,     0,     0,   362,     0,    65,
    1111,     0,  1112,     0,     0,     0,     0,     0,     0,  1118,
     275,     0,     0,     0,     0,     0,     0,    80,     0,    82,
       0,     0,    84,     0,  1127,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1148,  1022,     0,     0,     0,
    1027,     0,   275,   275,     0,   527,     0,  1155,   527,     0,
     275,   275,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,   322,   322,     0,     0,     0,  1177,     0,
       0,     0,     0,     0,     0,     0,  1169,     0,     0,     0,
       0,     0,     0,     0,  1173,   845,   845,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1181,     0,  1185,     0,     0,  1187,     0,  1190,     0,     0,
    1193,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1076,     0,  1077,     0,     0,  1079,     0,     0,
       0,     0,  1181,  1185,     0,  1190,  1193,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1093,     0,     0,     0,     0,  1226,     0,     0,  1227,     0,
    1027,     0,  1229,     0,     0,     0,     0,  1231,  1232,     0,
       0,  1233,     0,     0,     0,     0,     0,     0,     0,  1235,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1245,  1246,  1231,  1247,  1248,     0,     0,
       0,  1120,     0,     0,     0,     0,     0,  1125,     0,     0,
       0,   950,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1259,  1260,  1261,  1262,  1259,  1260,  1261,  1262,     0,     0,
       0,     0,     0,     0,     0,   978,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   275,   275,     0,  1161,     0,
       0,   434,     0,     0,     0,     0,     0,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1180,
       0,  1184,     0,     0,     0,     0,   527,     0,     0,   527,
     275,     0,     0,     0,   275,     0,  1041,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1219,   437,
       0,     0,     0,   438,  1222,     0,     0,     0,  1180,  1184,
       0,   509,     0,     0,     0,     0,     0,     0,     0,     0,
    1228,     0,     0,     0,  1230,     0,   767,     0,     0,     0,
       0,     0,   439,   510,   465,   466,   467,   468,   469,     0,
       0,   472,   473,   474,   475,     0,   477,   478,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,     0,     0,   460,   461,   462,     0,   363,     0,
       0,  1098,     0,     0,     0,   463,     0,     0,     0,   767,
     767,   767,   767,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   888,     0,     5,     6,     0,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,  -209,    21,     0,     0,   275,   275,
       0,     0,     0,   294,     0,   196,     0,     0,     0,    31,
    -209,  -209,  -209,     0,   197,    34,     0,     0,     0,   296,
       0,     0,    38,     0,   198,   434,   199,     0,     0,    43,
      44,     0,   435,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,   436,     0,     0,     0,    55,   590,
      57,     0,    59,     0,     0,     0,   275,    62,    63,   200,
       0,     0,   275,   275,     0,     0,     0,    70,    71,    72,
      73,   301,     0,    75,     0,     0,    77,     0,     0,   275,
      81,     0,    83,     0,     0,    85,   591,     0,   511,     0,
       0,   275,     0,    87,   275,     0,     0,     0,     0,     0,
       0,     0,     0,   437,     0,     0,     0,   438,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,    94,  1223,
       0,    95,    96,    97,     0,     0,   439,    99,     0,   202,
       0,   101,     0,  -209,     0,     0,   102,     0,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,   592,   460,   461,
     462,     0,     0,     0,     0,     0,     0,     0,   511,   463,
     511,     0,     0,   511,   511,    -2,     4,     0,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -209,
      21,    22,    23,    24,    25,    26,     0,  -209,    27,     0,
      28,     0,    29,    30,    31,  -209,  -209,  -209,    32,    33,
      34,   -68,     0,    35,    36,    37,     0,    38,  -335,    39,
       0,    40,    41,    42,    43,    44,     0,    45,    46,    47,
      48,   -68,    49,    50,     0,    51,    52,    53,     0,  -335,
       0,     0,    54,    55,    56,    57,    58,    59,    60,  -335,
     -68,    61,    62,    63,    64,     0,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,     0,    75,    76,
       0,    77,    78,    79,    80,    81,    82,    83,   -68,    84,
      85,     0,     0,  -209,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,    98,
       0,     0,    99,     0,   100,     0,   101,     0,  -209,     0,
       4,   102,     5,     6,     7,     8,     9,    10,    11,    12,
       0,  -690,     0,    13,    14,    15,    16,    17,  -690,    18,
       0,    19,    20,  -209,    21,    22,    23,    24,    25,    26,
    -690,  -209,    27,  -690,    28,     0,    29,    30,    31,  -209,
    -209,  -209,    32,    33,    34,   -68,     0,    35,    36,    37,
       0,    38,  -335,    39,     0,    40,    41,    42,    43,    44,
       0,    45,    46,    47,    48,   -68,    49,    50,     0,    51,
      52,    53,     0,  -335,     0,     0,    54,    55,    56,    57,
       0,    59,    60,  -335,   -68,    61,    62,    63,    64,  -690,
      65,    66,    67,  -690,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
      82,    83,   -68,    84,    85,     0,     0,  -209,     0,    86,
       0,     0,  -690,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
    -690,    90,  -690,  -690,  -690,  -690,  -690,  -690,  -690,     0,
    -690,  -690,  -690,  -690,  -690,  -690,  -690,  -690,  -690,  -690,
    -690,  -690,    97,    98,  -690,  -690,  -690,     0,   100,  -690,
     101,     0,  -209,     0,   346,  -690,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,  -209,    21,    22,
      23,    24,    25,    26,     0,  -209,    27,     0,    28,     0,
      29,    30,    31,  -209,  -209,  -209,    32,    33,    34,   -68,
       0,    35,    36,    37,     0,    38,  -335,    39,     0,    40,
      41,    42,    43,    44,     0,    45,    46,    47,    48,   -68,
      49,    50,     0,    51,    52,    53,     0,  -335,     0,     0,
      54,    55,    56,    57,    58,    59,    60,  -335,   -68,    61,
      62,    63,    64,     0,    65,    66,    67,     0,    68,    69,
      70,    71,    72,    73,    74,     0,    75,    76,     0,    77,
      78,    79,    80,    81,    82,    83,   -68,    84,    85,     0,
       0,  -209,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,    98,     0,     0,
      99,     0,   100,   347,   101,     0,  -209,     0,     4,   102,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,  -209,    21,    22,    23,    24,    25,    26,     0,  -209,
      27,     0,    28,     0,    29,    30,    31,  -209,  -209,  -209,
      32,    33,    34,   -68,     0,    35,    36,    37,     0,    38,
    -335,    39,     0,    40,    41,    42,    43,    44,     0,    45,
      46,    47,    48,   -68,    49,    50,     0,    51,    52,    53,
       0,  -335,     0,     0,    54,    55,    56,    57,    58,    59,
      60,  -335,   -68,    61,    62,    63,    64,     0,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,     0,
      75,    76,     0,    77,    78,    79,    80,    81,    82,    83,
     -68,    84,    85,     0,     0,  -209,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,     0,   100,   564,   101,     0,
    -209,     0,   581,   102,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -209,    21,    22,    23,    24,
      25,    26,     0,  -209,    27,     0,    28,     0,    29,    30,
      31,  -209,  -209,  -209,    32,    33,    34,   -68,     0,    35,
      36,    37,     0,    38,  -335,    39,     0,    40,    41,    42,
      43,    44,     0,    45,    46,    47,    48,   -68,    49,    50,
       0,    51,    52,    53,     0,  -335,     0,     0,    54,    55,
      56,    57,    58,    59,    60,  -335,   -68,    61,    62,    63,
      64,     0,    65,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,     0,    75,    76,     0,    77,    78,    79,
      80,    81,    82,    83,   -68,    84,    85,     0,     0,  -209,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,    98,     0,     0,    99,     0,
     100,   582,   101,     0,  -209,     0,     4,   102,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -209,
      21,    22,    23,    24,    25,    26,     0,  -209,    27,     0,
      28,     0,    29,    30,    31,  -209,  -209,  -209,    32,    33,
      34,   -68,     0,    35,    36,    37,     0,    38,  -335,    39,
       0,    40,    41,    42,    43,    44,     0,    45,    46,    47,
      48,   -68,    49,    50,     0,    51,    52,    53,     0,  -335,
       0,     0,    54,    55,    56,    57,    58,    59,    60,  -335,
     -68,    61,    62,    63,    64,     0,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,     0,    75,    76,
       0,    77,    78,    79,    80,    81,    82,    83,   -68,    84,
      85,     0,     0,  -209,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,    98,
       0,     0,    99,     0,   100,   822,   101,     0,  -209,     0,
       4,   102,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -209,    21,    22,    23,    24,    25,    26,
       0,  -209,    27,     0,    28,     0,    29,    30,    31,  -209,
    -209,  -209,    32,    33,    34,   -68,     0,    35,    36,    37,
       0,    38,  -335,    39,     0,    40,    41,    42,    43,    44,
       0,    45,    46,    47,    48,   -68,    49,    50,     0,    51,
      52,    53,     0,  -335,     0,     0,    54,    55,    56,    57,
     356,    59,    60,  -335,   -68,    61,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
      82,    83,   -68,    84,    85,     0,     0,  -209,     0,    86,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,    98,     0,     0,    99,     0,   100,     0,
     101,     0,  -209,     0,     4,   102,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,  -209,    21,    22,
      23,    24,    25,    26,     0,  -209,    27,     0,    28,     0,
      29,    30,    31,  -209,  -209,  -209,    32,    33,    34,   -68,
       0,    35,    36,    37,     0,    38,  -335,    39,     0,    40,
      41,    42,    43,    44,     0,    45,    46,    47,    48,   -68,
      49,    50,     0,    51,    52,    53,     0,  -335,     0,     0,
      54,    55,    56,    57,    58,    59,    60,  -335,   -68,    61,
      62,    63,    64,     0,    65,    66,    67,     0,    68,    69,
      70,    71,    72,    73,    74,     0,    75,    76,     0,    77,
      78,    79,    80,    81,    82,    83,   -68,    84,    85,     0,
       0,  -209,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,    98,     0,     0,
      99,     0,   100,     0,   101,     0,  -209,     0,     4,   102,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,  -209,    21,    22,    23,    24,    25,    26,     0,  -209,
      27,     0,    28,     0,    29,    30,    31,  -209,  -209,  -209,
      32,    33,    34,   -68,     0,    35,    36,    37,     0,    38,
    -335,    39,     0,    40,    41,    42,    43,    44,     0,    45,
      46,    47,    48,   -68,    49,    50,     0,    51,    52,    53,
       0,  -335,     0,     0,    54,    55,    56,    57,     0,    59,
      60,  -335,   -68,    61,    62,    63,    64,     0,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,     0,
      75,    76,     0,    77,    78,    79,    80,    81,    82,    83,
     -68,    84,    85,     0,     0,  -209,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,     0,   100,     0,   101,     0,
    -209,     0,     4,   102,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -209,    21,    22,    23,    24,
      25,    26,     0,  -209,    27,     0,    28,     0,    29,    30,
      31,  -209,  -209,  -209,    32,  1035,    34,   -68,     0,    35,
      36,    37,     0,    38,  -335,    39,     0,    40,    41,    42,
      43,    44,     0,    45,    46,    47,    48,   -68,    49,    50,
       0,    51,    52,    53,     0,  -335,     0,     0,    54,    55,
      56,    57,     0,    59,    60,  -335,   -68,    61,    62,    63,
      64,     0,    65,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,     0,    75,    76,     0,    77,    78,    79,
      80,    81,    82,    83,   -68,    84,    85,     0,     0,  -209,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,    98,     0,     0,    99,     0,
     100,     0,   101,     0,  -209,     0,     4,   102,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -209,
      21,    22,    23,    24,    25,    26,     0,  -209,    27,     0,
      28,     0,    29,    30,    31,  -209,  -209,  -209,    32,  1179,
      34,   -68,     0,    35,    36,    37,     0,    38,  -335,    39,
       0,    40,    41,    42,    43,    44,     0,    45,    46,    47,
      48,   -68,    49,    50,     0,    51,    52,    53,     0,  -335,
       0,     0,    54,    55,    56,    57,     0,    59,    60,  -335,
     -68,    61,    62,    63,    64,     0,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,     0,    75,    76,
       0,    77,    78,    79,    80,    81,    82,    83,   -68,    84,
      85,     0,     0,  -209,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,    98,
       0,     0,    99,     0,   100,     0,   101,     0,  -209,     0,
       4,   102,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -209,    21,    22,    23,    24,    25,    26,
       0,  -209,    27,     0,    28,     0,    29,    30,    31,  -209,
    -209,  -209,    32,  1183,    34,   -68,     0,    35,    36,    37,
       0,    38,  -335,    39,     0,    40,    41,    42,    43,    44,
       0,    45,    46,    47,    48,   -68,    49,    50,     0,    51,
      52,    53,     0,  -335,     0,     0,    54,    55,    56,    57,
       0,    59,    60,  -335,   -68,    61,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
      82,    83,   -68,    84,    85,     0,     0,  -209,     0,    86,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,    98,     0,     0,    99,     0,   100,     0,
     101,     0,  -209,     0,     4,   102,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,  -209,    21,    22,
      23,    24,    25,    26,     0,  -209,    27,     0,    28,     0,
      29,    30,    31,  -209,  -209,  -209,    32,  1189,    34,   -68,
       0,    35,    36,    37,     0,    38,  -335,    39,     0,    40,
      41,    42,    43,    44,     0,    45,    46,    47,    48,   -68,
      49,    50,     0,    51,    52,    53,     0,  -335,     0,     0,
      54,    55,    56,    57,     0,    59,    60,  -335,   -68,    61,
      62,    63,    64,     0,    65,    66,    67,     0,    68,    69,
      70,    71,    72,    73,    74,     0,    75,    76,     0,    77,
      78,    79,    80,    81,    82,    83,   -68,    84,    85,     0,
       0,  -209,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,    98,     0,     0,
      99,     0,   100,     0,   101,     0,  -209,     0,     4,   102,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,  -209,    21,    22,    23,    24,    25,    26,     0,  -209,
      27,     0,    28,     0,    29,    30,    31,  -209,  -209,  -209,
      32,  1192,    34,   -68,     0,    35,    36,    37,     0,    38,
    -335,    39,     0,    40,    41,    42,    43,    44,     0,    45,
      46,    47,    48,   -68,    49,    50,     0,    51,    52,    53,
       0,  -335,     0,     0,    54,    55,    56,    57,     0,    59,
      60,  -335,   -68,    61,    62,    63,    64,     0,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,     0,
      75,    76,     0,    77,    78,    79,    80,    81,    82,    83,
     -68,    84,    85,     0,     0,  -209,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,     0,   100,     0,   101,   743,
    -209,     5,     6,   102,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,  -209,    21,     0,   744,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,  -209,  -209,
    -209,     0,   197,    34,     0,   745,     0,    36,     0,   746,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,   747,     0,    55,   748,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,   749,
       0,     0,     0,     0,     0,    70,    71,    72,    73,   201,
       0,    75,     0,     0,    77,     0,     0,   750,    81,     0,
      83,     0,   751,    85,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   202,     0,   101,
     752,  -209,     5,     6,   102,     8,     9,    10,    11,    12,
       0,   637,     0,    13,     0,     0,    16,    17,   639,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,   197,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,   645,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,    83,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,    87,   510,   465,   466,   467,   468,   469,     0,
       0,   472,   473,   474,   475,     0,   477,   478,     0,   833,
     834,   835,   836,   837,   658,     0,   659,     0,    93,     0,
     660,   661,   662,   663,   664,   665,   666,   838,   668,   669,
      95,   839,    97,   -90,   671,   672,   840,   674,   202,     0,
     101,     0,     0,     5,     6,   102,     8,     9,    10,    11,
      12,     0,   637,     0,    13,     0,     0,    16,    17,   639,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   198,   645,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   200,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,   201,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,   510,   465,   466,   467,   468,   469,
       0,     0,   472,   473,   474,   475,     0,   477,   478,     0,
     833,   834,   835,   836,   837,   658,     0,   659,     0,    93,
       0,   660,   661,   662,   663,   664,   665,   666,   838,   668,
     669,    95,   839,    97,     0,   671,   672,   840,   674,   202,
       0,   101,     0,     0,     0,     0,   102,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
       0,    16,    17,     0,    18,     0,   195,    20,     0,    21,
       0,     0,     0,     0,    26,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,    33,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,    50,     0,    51,    52,   333,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,     0,     0,     0,    81,     0,    83,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   100,     0,   101,     0,     0,     5,     6,
     102,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,   744,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,   745,     0,    36,     0,   746,    38,     0,   198,
       0,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,     0,     0,     0,
       0,   747,     0,    55,   748,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,     0,   749,     0,     0,     0,
       0,     0,    70,    71,    72,    73,   201,     0,    75,     0,
       0,    77,     0,     0,   750,    81,     0,    83,     0,   751,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   202,     0,   101,     0,     0,     5,
       6,   102,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,   377,    23,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,   361,     0,
       0,     0,     0,     0,    55,    56,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,    65,     0,     0,
       0,     0,     0,    70,    71,    72,    73,   201,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,    83,     0,
      84,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   202,     0,   101,     0,     0,
       5,     6,   102,     8,     9,    10,   380,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,    19,
       0,  -209,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   196,     0,     0,     0,    31,  -209,  -209,  -209,
       0,   197,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,    64,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,   201,     0,
      75,     0,     0,    77,     0,     0,     0,    81,    82,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   202,     0,   101,     0,
    -209,     5,     6,   102,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,  -209,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,  -209,  -209,
    -209,     0,   197,    34,     0,     0,     0,    36,     0,     0,
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
       0,  -209,     5,     6,   102,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,   197,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,  -734,
       0,     0,     0,  -734,     0,     0,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,    83,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,  -377,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,  -377,     0,     0,    99,     0,   202,  -377,
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
       0,    95,    96,    97,   318,     0,     0,    99,     0,   202,
       0,   101,     0,     0,     5,     6,   102,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,   201,     0,    75,     0,     0,    77,   316,   317,
       0,    81,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,   351,     0,     0,     0,     0,     0,     0,     0,    94,
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
      71,    72,    73,   201,     0,    75,     0,     0,    77,   316,
     317,     0,    81,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   202,     0,   101,  -538,     0,     5,     6,   102,     8,
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
      99,     0,   202,     0,   101,     0,     0,  -538,     0,   102,
       5,     6,     0,     8,     9,    10,    11,    12,     0,     0,
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
      97,     0,     0,     0,    99,     0,   202,     0,   101,   808,
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
     811,     0,     5,     6,   102,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,   197,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,   316,   317,     0,    81,
       0,    83,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   202,     0,
     101,   817,     0,     5,     6,   102,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
       0,     0,     0,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,   956,
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
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,   201,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,  -385,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,  -385,     0,     0,    99,     0,
     202,  -385,   101,     0,     0,     5,     6,   102,     8,     9,
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
       0,     0,    88,    89,    90,    91,    92,     0,  -370,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,  -370,     0,     0,    99,
       0,   202,  -370,   101,     0,     0,     5,     6,   102,     8,
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
       0,     0,     0,    81,     0,    83,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,  -378,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,  -378,     0,     0,
      99,     0,   202,  -378,   101,     0,     0,     5,     6,   102,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,   258,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,    83,     0,   259,    85,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   202,     0,   101,     0,     0,     5,     6,
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
       0,     0,     0,    62,    63,   200,  -734,     0,     0,     0,
    -734,     0,     0,    70,    71,    72,    73,   201,     0,    75,
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
      27,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,   197,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,   201,     0,
      75,     0,     0,    77,     0,     0,     0,    81,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,   603,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   202,     0,   101,     0,
       0,   604,     0,   102,     5,     6,     0,     8,     9,    10,
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
       0,    88,    89,    90,    91,    92,     0,  -406,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     202,     0,   101,  -406,     0,     5,     6,   102,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,   274,    27,     0,   196,     0,     0,
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
       0,   100,     0,   101,     0,     0,     5,     6,   102,     8,
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
       0,     0,     0,    81,     0,    83,     0,     0,    85,     0,
       0,     0,     0,     0,   603,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   202,     0,   101,     0,     0,     5,     6,   102,
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
      77,     0,     0,     0,    81,     0,    83,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   202,     0,   101,     0,     0,   604,     0,
     102,     5,     6,     0,     8,     9,    10,    11,    12,     0,
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
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   202,   806,   101,
       0,     0,     5,     6,   102,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,   197,    34,     0,     0,     0,    36,     0,
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
     101,     0,     0,  1026,     0,   102,     5,     6,     0,     8,
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
       0,     0,     0,    81,     0,    83,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   202,     0,   101,     0,     0,  1031,     0,   102,
       5,     6,     0,     8,     9,    10,    11,    12,     0,     0,
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
       0,     5,     6,   102,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,   294,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,   296,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,   301,
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
       0,     0,     0,  1068,    34,     0,     0,     0,    36,     0,
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
       0,     0,     0,     0,  1214,    34,     0,     0,     0,    36,
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
      31,     0,     0,     0,     0,  1215,    34,     0,     0,     0,
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
       0,    31,     0,     0,     0,     0,  1217,    34,     0,     0,
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
       0,   202,     0,   101,     0,     0,     5,     6,   102,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,     0,     0,     0,     0,  1218,    34,     0,
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
       0,     0,    93,     6,     0,     8,     9,    10,    11,    12,
       0,    94,     0,    13,    95,    96,    97,    17,     0,     0,
      99,   195,   202,  -209,   101,     0,   293,     0,     0,   102,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   202,     0,
     101,  -339,  -209,     0,     6,   102,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,     0,    17,     0,
       0,     0,   195,     0,  -209,     0,     0,   293,     0,     0,
       0,     0,     0,     0,     0,   196,     0,     0,     0,     0,
    -209,  -209,  -209,     0,   197,     0,     0,   295,     0,     0,
       0,   297,     0,     0,     0,     0,   199,     0,     0,    43,
      44,     0,     0,     0,     0,    48,     0,    49,    50,     0,
       0,     0,     0,     0,     0,     0,   298,     0,    55,   299,
      57,    58,    59,     0,     0,     0,     0,    62,     0,   200,
       0,   300,     0,     0,     0,     0,     0,    70,    71,     0,
      73,     0,     0,     0,     0,     0,     0,     0,     0,   302,
       0,     0,    83,     0,   303,     0,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   635,     0,   636,     0,
      88,    89,    90,    91,    92,     0,     0,   637,     0,    93,
       0,     0,   638,   232,   639,   640,     0,     0,    94,     0,
     641,    95,    96,    97,     0,     0,     0,    99,   235,   202,
     196,   101,     0,  -209,   236,     0,   102,     0,     0,     0,
     642,     0,     0,     0,   238,     0,     0,   643,     0,   644,
     645,     0,     0,     0,     0,     0,     0,     0,   646,     0,
       0,     0,     0,     0,     0,   242,   647,     0,     0,     0,
       0,     0,     0,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   648,     0,     0,     0,     0,     0,     0,
       0,     0,   246,     0,   649,     0,   248,     0,   650,     0,
       0,   250,     0,     0,     0,   651,     0,   252,     0,     0,
     652,     0,     0,     0,     0,     0,     0,     0,     0,   510,
     465,   466,   467,   468,   469,     0,     0,   472,   473,   474,
     475,     0,   477,   478,     0,   653,   654,   655,   656,   657,
     658,     0,   659,     0,     0,     0,   660,   661,   662,   663,
     664,   665,   666,   667,   668,   669,     0,   670,     0,     0,
     671,   672,   673,   674,     5,     6,   675,     8,     9,    10,
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
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,     0,    96,    97,     0,     0,     0,    99,     0,
     202,     0,   101,     0,  -209,     5,     6,     0,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,  -209,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,  -209,  -209,  -209,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,   201,     0,    75,     0,     0,    77,     0,
       0,     0,    81,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    90,     0,     0,     5,     6,     0,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,    97,   195,     0,  -209,    21,
       0,   202,     0,   101,     0,  -209,     0,    27,     0,   196,
       0,     0,     0,    31,  -209,  -209,  -209,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,   201,   701,    75,     0,     0,
      77,     0,     0,     0,    81,   637,    83,     0,     0,    85,
     988,   989,   639,   990,     0,     0,     0,     0,   991,     0,
       0,     0,     0,     0,     0,     0,   703,     0,     0,     0,
       0,     0,     0,     0,    88,     0,    90,     0,   992,     0,
       0,     0,   993,     0,     0,   994,     0,   198,   645,     0,
       0,     0,     0,     0,     0,     0,   704,    97,     0,     0,
       0,     0,     0,     0,   995,   101,     0,  -209,     0,     0,
       0,   996,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   997,     0,     0,     0,     0,     0,     0,     0,     0,
     998,     0,   999,     0,  1000,     0,   705,     0,     0,     0,
       0,     0,   706,  1001,     0,  1002,     0,     0,  1003,     0,
       0,     0,     0,     0,     0,     0,     0,   510,   465,   466,
     467,   468,   469,     0,     0,   472,   473,   474,   475,     0,
     477,   478,     0,   653,   654,   655,   656,   657,   658,     0,
     659,     0,     0,     0,   660,   661,   662,   663,   664,   665,
     666,   667,   668,   669,   230,   670,     0,     0,   671,   672,
     673,   674,  1004,   637,     0,     0,     0,     0,   231,   232,
     639,   233,     0,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,   235,     0,     0,     0,     0,     0,
     236,     0,     0,     0,     0,     0,   237,     0,     0,     0,
     238,     0,     0,   239,     0,   240,   645,     0,     0,     0,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,     0,     0,     0,     0,     0,     0,   244,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   245,
       0,     0,     0,     0,     0,     0,   434,     0,   246,     0,
     247,     0,   248,   435,   249,     0,     0,   250,     0,     0,
       0,   251,     0,   252,     0,   436,   253,     0,     0,     0,
       0,     0,     0,     0,     0,   510,   465,   466,   467,   468,
     469,     0,     0,   472,   473,   474,   475,     0,   477,   478,
       0,   653,   654,   655,   656,   657,   658,     0,   659,     0,
       0,     0,   660,   661,   662,   663,   664,   665,   666,   667,
     668,   669,     0,   670,     0,     0,   671,   672,   673,   674,
       0,     0,     0,     0,   437,     0,   434,     0,   438,     0,
       0,     0,     0,   435,     0,     0,  1188,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,   510,   465,
     466,   467,   468,   469,     0,     0,   472,   473,   474,   475,
       0,   477,   478,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   363,   437,     0,   434,     0,   438,     0,
     463,     0,     0,   435,     0,     0,  1251,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,   510,   465,
     466,   467,   468,   469,     0,     0,   472,   473,   474,   475,
       0,   477,   478,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   363,   437,     0,   434,     0,   438,     0,
     463,     0,     0,   435,     0,     0,  1252,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,   510,   465,
     466,   467,   468,   469,     0,     0,   472,   473,   474,   475,
       0,   477,   478,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   363,   437,     0,   434,     0,   438,     0,
     463,     0,     0,   435,     0,     0,  1253,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,   510,   465,
     466,   467,   468,   469,     0,     0,   472,   473,   474,   475,
       0,   477,   478,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   363,   437,     0,     0,     0,   438,     0,
     463,     0,     0,     0,     0,   434,  1254,     0,     0,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,   870,     0,   439,   510,   465,
     466,   467,   468,   469,     0,     0,   472,   473,   474,   475,
     871,   477,   478,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   363,     0,     0,     0,     0,     0,   434,
     463,     0,     0,   437,     0,     0,   435,   438,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   436,   274,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   214,     0,   983,     0,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,   437,   460,   461,
     462,   438,   363,     0,     0,     0,     0,     0,     0,   463,
     434,   829,     0,     0,     0,     0,     0,   435,     0,     0,
       0,     0,     0,     0,     0,     0,   214,     0,     0,   436,
     439,     0,     0,     0,   830,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
       0,   831,   460,   461,   462,     0,   363,     0,     0,     0,
       0,     0,     0,   463,   434,     0,     0,     0,   437,     0,
       0,   435,   438,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,  1131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,     0,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,     0,   437,   460,   461,   462,   438,     0,   434,     0,
       0,     0,     0,     0,   463,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,   274,     0,
       0,   214,     0,     0,     0,   439,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,     0,     0,   460,   461,   462,
       0,   363,   434,     0,     0,     0,   437,     0,   463,   435,
     438,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   274,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   214,     0,     0,     0,   439,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,     0,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
     437,   460,   461,   462,   438,   363,   434,     0,     0,     0,
       0,     0,   463,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,   917,     0,     0,     0,
       0,     0,     0,   439,     0,     0,     0,     0,     0,     0,
       0,   918,     0,     0,     0,     0,     0,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,     0,     0,   460,   461,   462,     0,   363,
       0,     0,     0,     0,   437,   434,   463,     0,   438,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   437,     0,     0,     0,   438,     0,     0,
     463,     0,     0,     0,     0,   949,   434,     0,     0,     0,
       0,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,   439,     0,     0,     0,
    1106,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,  1107,   460,   461,
     462,     0,   363,     0,     0,     0,     0,     0,     0,   463,
     434,     0,     0,     0,   437,     0,     0,   435,   438,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,   435,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   436,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,   437,   460,
     461,   462,   438,     0,     0,   434,   531,     0,     0,     0,
     463,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,   214,     0,     0,
       0,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,     0,     0,     0,   438,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,     0,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,     0,   214,   460,   461,   462,   439,     0,     0,     0,
       0,     0,  1123,   437,   463,     0,     0,   438,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   436,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   439,     0,   460,   461,
     462,     0,     0,     0,   434,     0,     0,  1171,     0,   463,
       0,   435,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   436,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,     0,   460,   461,
     462,     0,   437,     0,   434,     0,   438,     0,     0,   463,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,     0,   434,     0,   438,     0,     0,     0,
       0,   435,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   436,   449,   450,   451,     0,   453,   454,
     455,   456,     0,     0,   459,   439,     0,   460,   461,   462,
       0,     0,   437,     0,     0,     0,   438,     0,   463,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   439,     0,   460,   461,   462,
       0,   554,   437,   434,     0,     0,   438,     0,   463,     0,
     435,     0,   440,   441,   737,   442,   443,   444,     0,   445,
     446,   447,   436,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   439,   558,   460,   461,   462,
       0,     0,     0,   434,   825,     0,     0,     0,   463,     0,
     435,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   436,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,     0,     0,   460,   461,   462,
       0,   437,     0,     0,   434,   438,     0,     0,   463,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,     0,     0,     0,     0,     0,
       0,   437,     0,     0,     0,   438,     0,     0,     0,     0,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   439,     0,   460,   461,   462,     0,
       0,     0,   437,   807,     0,     0,   438,   463,     0,     0,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   826,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   864,   439,   460,   461,   462,     0,
       0,     0,     0,     0,     0,     0,     0,   463,     0,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   434,     0,   460,   461,   462,
       0,     0,   435,     0,     0,     0,     0,     0,   463,     0,
       0,     0,     0,     0,   436,     0,     0,   947,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,     0,   434,     0,   438,     0,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,     0,     0,     0,
       0,     0,     0,   437,     0,     0,     0,   438,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   439,     0,   460,   461,
     462,     0,     0,   437,   434,  1013,     0,   438,     0,   463,
       0,   435,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   436,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   439,     0,   460,   461,
     462,     0,     0,     0,   434,   959,     0,     0,     0,   463,
       0,   435,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   436,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,     0,   460,   461,
     462,     0,   437,     0,     0,   960,   438,     0,     0,   463,
       0,     0,   434,  1105,     0,     0,     0,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,   439,     0,     0,     0,     0,
       0,     0,   437,     0,     0,     0,   438,     0,     0,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   439,     0,   460,   461,   462,
       0,     0,     0,     0,     0,     0,     0,     0,   463,     0,
     437,     0,   440,   441,   438,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   434,     0,   460,   461,   462,
       0,     0,   435,   439,  1060,     0,     0,     0,   463,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   434,     0,   460,   461,   462,     0,     0,
     435,     0,     0,     0,     0,     0,   463,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,     0,     0,     0,   438,     0,     0,
       0,     0,     0,   434,     0,     0,     0,     0,     0,     0,
     435,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,     0,   440,   441,   438,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,  1117,   460,   461,
     462,     0,     0,  1126,   439,     0,     0,     0,     0,   463,
       0,   437,     0,     0,     0,   438,     0,     0,     0,     0,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   439,     0,   460,   461,   462,     0,
       0,     0,     0,     0,     0,     0,     0,   463,     0,     0,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   434,     0,   460,   461,   462,     0,
       0,   435,     0,     0,   617,  1154,   230,   463,     7,     0,
       0,     0,     0,   436,  1157,     0,     0,     0,     0,     0,
     231,   232,     0,   233,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,   434,  1176,   235,     0,     0,     0,
       0,   435,   236,     0,     0,     0,     0,     0,   237,     0,
       0,     0,   238,   436,     0,   239,     0,   240,     0,     0,
       0,     0,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,   437,   242,   243,   434,   438,     0,     0,     0,
       0,   244,   435,     0,     0,     0,     0,     0,     0,     0,
       0,   245,     0,     0,   436,     0,     0,     0,     0,     0,
     246,     0,   247,     0,   248,   439,   249,     0,     0,   250,
       0,     0,   437,   251,     0,   252,   438,     0,   253,     0,
       0,     0,   440,   441,  1208,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   439,     0,   460,   461,   462,
       0,     0,     0,   437,     0,     0,     0,   438,   463,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,     0,   439,   460,   461,   462,
     434,     0,     0,     0,     0,     0,     0,   435,   463,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,   436,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,     0,   460,   461,
     462,     0,     0,     0,     0,     0,     0,     0,   434,   463,
       0,     0,     0,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,     0,
       0,     0,   438,     0,     0,     0,     0,     0,   434,     0,
    1216,     0,     0,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   437,     0,   440,   441,
     438,   442,   443,   444,     0,   445,   446,   447,  1255,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,     0,     0,   460,   461,   462,     0,     0,     0,   439,
       0,     0,     0,     0,   463,     0,   437,   434,     0,     0,
     438,     0,     0,     0,   435,     0,   440,   441,  1256,   442,
     443,   444,     0,   445,   446,   447,   436,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   439,
       0,   460,   461,   462,     0,     0,     0,   434,     0,     0,
       0,     0,   463,     0,   435,     0,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   436,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
       0,   460,   461,   462,     0,   437,     0,   434,     0,   438,
       0,     0,   463,     0,   435,     0,     0,  1257,     0,     0,
       0,     0,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   439,     0,
       0,     0,     0,     0,     0,   437,     0,     0,     0,   438,
       0,     0,     0,     0,     0,   440,   441,  1258,   442,   443,
     444,     0,   445,   446,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   439,     0,
     460,   461,   462,     0,     0,   437,   434,     0,     0,   438,
       0,   463,     0,   435,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   436,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   439,     0,
     460,   461,   462,     0,     0,     0,     0,     0,     0,     0,
       0,   463,     0,     0,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,     0,     0,
     460,   461,   462,     0,   937,     0,     0,     0,   438,     0,
       0,   463,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     7,     0,   460,
     461,   462,     0,     0,     0,     0,     0,     0,     0,     0,
     463,     0,     0,     0,   358,     0,     0,     0,    22,    23,
       0,     0,     0,     0,     0,     0,     0,   359,     0,    29,
     360,     0,     0,     0,     0,    32,     0,     0,     0,     0,
       0,     0,    37,     0,     7,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   358,     0,     0,   361,    22,    23,     0,     0,    54,
       0,    56,     0,   356,   359,  1044,    29,   360,  1045,     0,
       0,   362,    32,    65,     0,     0,     0,     0,     0,    37,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,    82,     0,     0,    84,     0,   358,     0,
       0,   361,    22,    23,     0,     0,    54,     0,    56,     0,
       0,   359,  1044,    29,   360,  1045,     0,     0,   362,    32,
      65,     0,     0,     0,     0,     0,    37,     0,     0,  -335,
       0,     0,     0,    41,     0,     0,     0,     0,    80,     0,
      82,     0,   -68,    84,     0,     0,    98,     0,   361,     0,
    -335,     0,     0,    54,     0,    56,   230,     0,     0,    60,
    -335,   -68,    61,     0,     0,   362,     0,    65,     0,     0,
     231,   232,     0,   233,     0,     0,     0,     0,   234,     0,
      23,     0,     0,     0,     0,    80,   235,    82,     0,     0,
      84,     0,   236,    98,     0,     0,     0,     0,   237,     0,
       0,     0,   238,     0,     0,   239,     0,   240,     0,     0,
       0,     0,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,   242,   243,     0,     0,     0,     0,     0,
       0,   244,    56,     0,     0,     0,     0,     0,     0,     0,
      98,   245,     0,     0,    65,   363,     0,   230,     0,     7,
     246,     0,   247,     0,   248,     0,   249,     0,     0,   250,
       0,   231,   232,   251,   233,   252,     0,    84,   253,   234,
       0,     0,     0,     0,     0,     0,     0,   235,     0,     0,
       0,     0,     0,   236,     0,     0,     0,     0,     0,   237,
       0,     0,     0,   238,     0,     0,   239,     0,   240,     0,
       0,     0,     0,     0,     0,     0,  -310,   241,     0,     0,
       0,     0,     0,     0,   242,   243,     0,     0,     0,     0,
    -310,  -310,   244,  -310,     0,     0,     0,     0,  -310,     0,
       0,     0,   245,     0,     0,     0,  -310,     0,     0,     0,
       0,   246,  -310,   247,     0,   248,     0,   249,  -310,     0,
     250,     0,  -310,     0,   251,  -310,   252,  -310,     0,   253,
       0,     0,     0,     0,     0,   230,  -310,     0,     0,     0,
       0,     0,     0,  -310,  -310,     0,     0,     0,     0,   231,
     232,  -310,   233,     0,     0,     0,     0,   234,     0,     0,
       0,  -310,     0,     0,     0,   235,     0,     0,     0,     0,
    -310,   236,  -310,     0,  -310,     0,  -310,   237,     0,  -310,
       0,   238,     0,  -310,   239,  -310,   240,     0,  -310,     0,
       0,     0,     0,     0,  -311,   241,     0,     0,     0,     0,
       0,     0,   242,   243,     0,     0,     0,     0,  -311,  -311,
     244,  -311,     0,     0,     0,     0,  -311,     0,     0,     0,
     245,     0,     0,     0,  -311,     0,     0,     0,     0,   246,
    -311,   247,     0,   248,     0,   249,  -311,     0,   250,     0,
    -311,     0,   251,  -311,   252,  -311,     0,   253,     0,     0,
       0,     0,     0,     0,  -311,     0,     0,     0,     0,     0,
       0,  -311,  -311,     0,     0,     0,     0,     0,     0,  -311,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -311,
       0,     0,     0,     0,     0,     0,     0,     0,  -311,     0,
    -311,     0,  -311,     0,  -311,     0,     0,  -311,     0,     0,
       0,  -311,     0,  -311,     0,     0,  -311
};

static const yytype_int16 yycheck[] =
{
       2,     2,   165,    45,   401,   110,   181,   618,   229,    62,
      62,   552,    26,    62,   425,   492,    67,   590,   223,     2,
     678,   516,   680,    25,   630,   579,   626,    69,   265,   796,
     131,   831,   790,   547,   381,    86,   879,   826,     5,     1,
       1,   403,    25,   391,   265,    56,   849,    24,   962,     1,
     101,   887,    66,   290,   203,    47,   605,   606,    75,    47,
     609,   610,     1,    40,    41,    42,    47,     3,   523,   290,
     146,    61,    74,     3,   119,   119,    78,    79,    35,    22,
      46,   146,    32,   146,    32,     1,   261,   518,   164,   437,
     438,    74,   329,    32,    32,    78,    79,    32,   100,   116,
       3,     0,   104,   140,   102,   164,   164,   170,   329,   174,
       1,   148,    32,   171,   545,    32,    32,   100,    54,    85,
     119,   104,    61,    75,    54,    32,    24,   117,   964,   174,
     174,   119,    24,   122,    32,   483,   484,    32,     2,   119,
      32,   489,    40,    41,    42,    61,    89,   113,    40,    41,
      42,    54,   109,   170,   146,    56,    47,    93,   146,    56,
     171,    25,    56,   124,   116,   146,   105,   103,   111,   119,
      47,   169,   109,  1016,    24,   630,   776,  1091,   117,   146,
     169,   543,    32,    67,   145,   146,   174,     3,   955,   105,
      40,    41,    42,   174,   174,   962,   173,    56,   146,   119,
     103,   117,    86,   170,  1047,  1048,   145,   965,   762,   171,
      74,   172,  1015,   215,    78,    79,   365,   101,   170,   169,
     118,   169,  1011,    18,   119,   446,   118,    47,   576,   145,
     169,   169,   215,   124,   169,   277,   100,   279,    54,   146,
     104,    47,   119,   738,   481,   482,   593,   164,   164,   169,
     425,    27,   169,   169,   145,   146,    47,   830,   146,    90,
     481,   482,   169,    94,   813,   814,   815,   269,   118,   146,
    1113,    91,   274,   164,   169,   173,    47,   171,    53,   380,
     794,   173,   170,    78,   326,    91,   269,   103,    46,   117,
      27,   274,  1135,    46,   146,   516,    56,   174,   146,    74,
      91,    83,    97,    79,    86,   146,  1073,  1107,    66,    84,
      47,     3,   164,    66,    51,    91,   171,   112,   146,   556,
      91,    90,   170,   173,  1091,    94,    91,    85,    20,    56,
     535,   172,    85,   496,  1143,   556,   936,   146,   114,    76,
      50,    33,    79,    53,    81,   229,    11,   349,   349,    31,
       2,   215,   146,   402,    91,   113,    56,   963,   579,   146,
     113,   363,    54,   172,    74,   146,   349,    77,   146,   171,
     372,    63,   109,    25,    84,   124,   390,   114,   172,    56,
     363,   265,   877,   385,   398,   172,   164,   146,     9,   372,
     146,   172,   597,  1202,  1203,  1204,  1205,  1206,  1207,   109,
      56,   150,   385,  1061,  1062,   269,   290,   146,    90,   124,
     274,   103,    94,   172,   146,   103,   172,   109,   146,   470,
     471,    31,    74,   425,   919,   476,    78,    79,   479,   480,
     984,  1004,   164,   172,   146,   172,   151,    56,   153,   154,
    1051,   156,   157,   146,   172,   329,   923,   146,   100,   171,
     146,   146,   104,   146,    56,   146,   164,    90,   140,    56,
     172,    94,   170,   145,   100,   146,   148,     2,   164,   172,
     152,    78,    79,   172,   488,  1024,  1025,   172,   146,   172,
      90,   172,   879,   164,    94,   349,   146,   898,  1037,  1038,
      25,   544,   544,   146,   164,   544,   164,   146,   146,   363,
     164,   171,   146,   173,   164,   507,   170,   509,   372,   146,
      90,   164,   860,   618,    94,   164,   164,   738,   569,   570,
     164,   385,    56,   871,   507,    90,   509,   164,  1139,    94,
     140,   141,    90,  1106,   144,   145,    94,   147,   148,    74,
     171,   762,   152,    78,    79,   169,   171,   171,   173,   159,
    1099,  1100,  1101,  1102,    56,   165,   166,   167,    56,   601,
     602,   575,     2,   215,    27,   100,   914,   609,   610,   104,
     918,   140,   171,   171,   173,   173,   145,    56,   171,   148,
     173,   583,   583,   152,    47,    25,   470,   471,    51,   937,
     938,    56,   476,    78,    79,   479,   480,   481,   482,   171,
     583,   173,   291,   292,   825,   826,     5,   171,  1149,   173,
     171,   148,   173,    76,   569,   570,    79,   269,    81,  1016,
     169,    56,   274,   145,   169,   788,   164,   171,    91,    56,
     163,    85,   516,   981,    74,   983,   164,   164,    78,    79,
     164,    31,    27,   507,   164,   509,   109,   164,   164,   164,
    1047,   114,   164,   171,   164,  1052,   877,  1198,   169,   172,
     100,   145,    47,   171,   104,   171,    51,   164,   146,   164,
     171,   146,   556,   172,     9,     3,    79,   171,    66,   169,
     215,    31,   124,    27,   164,   569,   570,   148,   851,    69,
     171,    76,   124,   146,    79,   579,    81,   349,   919,   172,
      90,   124,    27,    47,    94,   124,    91,    51,    34,   760,
      34,   363,    66,   172,   124,   172,  1113,   145,    21,   583,
     372,   172,    47,   898,   109,   169,    51,   118,   779,   114,
     171,   146,    76,   385,   269,    79,    27,    81,   124,   274,
      90,   170,   105,   148,    94,   171,   148,    91,   148,   174,
     140,    76,   148,   755,    79,   145,    47,   147,   148,   148,
      51,   148,   152,   984,   766,   109,    91,   769,   140,   159,
     114,   148,   814,   815,   148,   215,    31,   167,   148,   148,
     148,   148,   148,   766,   109,    76,   769,   172,    79,   114,
     140,   141,   148,   143,   144,   145,   148,   147,   148,   149,
      91,   151,   152,   153,   154,   164,   156,   157,   158,   159,
     160,   164,   162,   164,   349,   165,   166,   167,   109,   164,
      32,   164,   164,   114,   172,   171,   176,   869,   363,   269,
     879,   873,   172,    34,   274,    90,   885,   372,    34,    94,
     145,   170,   856,   164,   146,   894,    34,  1084,  1085,   172,
     385,   169,    47,   170,   738,   507,   858,   509,   174,   174,
     862,   172,   174,  1084,  1085,   867,   164,   164,   870,   145,
     171,   171,   164,   146,    32,   858,   760,   172,   762,   862,
     174,    34,   172,   164,   867,   140,   141,   870,   143,   144,
     145,   174,   147,   148,   149,   779,   174,   152,   172,   164,
     172,   163,   766,   158,   159,   769,   163,   162,   146,   349,
     165,   166,   167,   984,   372,   109,   578,   264,   823,    24,
     365,   176,   389,   363,   338,   874,   529,   977,   544,   898,
     894,   583,   372,   383,   885,   937,   938,  1198,  1054,   304,
    1208,  1201,   944,  1048,   946,   385,   850,   949,   684,   531,
     129,   526,  1127,   775,    20,   923,    -1,    -1,    -1,   973,
      -1,   944,    -1,   946,    -1,    -1,   949,  1016,    -1,    -1,
     972,    -1,   507,    -1,   509,    -1,    -1,    -1,    -1,    -1,
     982,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,   972,
      -1,    -1,    -1,   877,   858,  1037,  1038,    -1,   862,   982,
      -1,    17,    -1,   867,    20,    -1,   870,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    35,
      -1,  1023,    -1,    -1,    -1,    -1,    -1,  1029,    -1,    45,
    1135,    -1,    -1,     2,  1139,   919,    52,    -1,    54,    -1,
    1023,    -1,    -1,    -1,    -1,    -1,  1029,    63,   583,    -1,
      -1,    -1,  1227,  1095,  1229,    71,    25,  1232,  1233,  1101,
    1102,    -1,    -1,    -1,  1113,    -1,    -1,   507,    -1,   509,
      -1,    -1,    88,    -1,    -1,    90,  1118,    -1,  1080,    94,
     944,    -1,   946,    99,    -1,   949,    -1,   103,  1130,    -1,
      -1,  1133,    -1,    -1,   110,    -1,    -1,  1080,    -1,   115,
     984,    -1,    -1,    -1,    -1,    74,    -1,    -1,   972,    78,
      79,    -1,    -1,    -1,   766,    -1,    -1,   769,   982,  1121,
      -1,  1123,    -1,    -1,    -1,   140,  1128,    -1,    -1,  1131,
     145,   100,   147,   148,    -1,   104,    -1,   152,  1121,    -1,
    1123,    -1,    -1,   583,   159,  1128,    -1,    -1,  1131,    -1,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,  1160,  1023,
      -1,    -1,    -1,    -1,  1166,  1029,    -1,    -1,  1170,  1171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1160,    -1,    -1,
    1182,    -1,    -1,  1166,  1186,    -1,  1188,  1170,  1171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1182,
    1084,  1085,    -1,  1186,    -1,  1188,   858,  1209,  1209,    -1,
     862,    -1,    -1,    -1,    -1,   867,  1080,    -1,   870,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1209,    -1,    -1,    -1,
      -1,   766,    -1,    -1,   769,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   215,    -1,    -1,  1251,
    1252,  1253,  1254,    -1,    -1,    -1,    -1,  1121,    -1,  1123,
      -1,    -1,    -1,    -1,  1128,    -1,    -1,  1131,  1251,  1252,
    1253,  1254,    -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   944,    -1,   946,    -1,  1160,   949,    -1,    25,
     269,    -1,  1166,    -1,    -1,   274,  1170,  1171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1182,    -1,
     972,    -1,  1186,   858,  1188,    -1,   766,   862,    -1,   769,
     982,    -1,   867,    -1,    -1,   870,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,  1209,    -1,    -1,    74,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1023,    -1,    -1,   100,    -1,    -1,  1029,   104,    -1,
     349,    -1,    -1,    -1,    -1,    -1,    -1,  1251,  1252,  1253,
    1254,    -1,    -1,    -1,   363,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,   372,    -1,    -1,    -1,    -1,    -1,   944,
      -1,   946,    -1,    -1,   949,    -1,   385,    -1,   858,    -1,
      -1,    -1,   862,    -1,    -1,    -1,    -1,   867,  1080,    -1,
     870,   123,    -1,    -1,    -1,    -1,    -1,   972,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,   982,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,  1121,
     162,  1123,    -1,   165,   166,   167,  1128,     2,    -1,  1131,
      -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,  1023,   215,
      -1,    -1,    -1,    -1,  1029,    -1,    -1,    -1,    -1,    -1,
      25,    90,    -1,    -1,   944,    94,   946,    -1,  1160,   949,
      -1,    -1,    -1,    -1,  1166,    -1,    -1,    -1,  1170,  1171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1182,    -1,   972,    -1,  1186,    -1,  1188,    -1,   507,    -1,
     509,    -1,   982,   269,    -1,  1080,    -1,    -1,   274,    74,
       2,   140,   141,    78,    79,    -1,   145,  1209,   147,   148,
      -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,    25,    -1,   100,   165,   166,   167,   104,
      -1,    -1,    -1,  1023,    -1,    -1,  1121,    -1,  1123,  1029,
      -1,    -1,    -1,  1128,    -1,    -1,  1131,    -1,    -1,  1251,
    1252,  1253,  1254,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   583,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    74,   349,     1,  1160,    -1,    -1,     5,    -1,
      -1,  1166,    40,    41,    42,  1170,  1171,   363,    -1,    -1,
    1080,    -1,    -1,    -1,    -1,    22,   372,  1182,   100,    26,
      27,  1186,   104,  1188,    -1,    -1,    -1,    -1,    35,   385,
      37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    -1,    -1,    50,  1209,    -1,    53,    -1,    -1,    -1,
      -1,  1121,    -1,  1123,    -1,    -1,    -1,    -1,  1128,    -1,
     215,  1131,    -1,    -1,    -1,    72,    -1,    74,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    83,    84,    -1,    86,
      -1,    -1,    89,    -1,    91,    -1,  1251,  1252,  1253,  1254,
    1160,    -1,    -1,    -1,    -1,    -1,  1166,    -1,    -1,    -1,
    1170,  1171,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,   149,  1182,    -1,   269,    -1,  1186,    -1,  1188,   274,
      -1,    -1,    22,    -1,    -1,   163,    26,    27,    -1,    -1,
      -1,    -1,    -1,   215,    -1,    35,    -1,    37,    38,  1209,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      50,   507,    -1,   509,    -1,    -1,    -1,   164,    -1,    -1,
      -1,   199,    -1,   170,    -1,    -1,    -1,   766,    -1,    -1,
     769,    -1,    72,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,  1251,  1252,  1253,  1254,    -1,    -1,   269,    -1,    89,
      -1,    91,   274,    -1,   349,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   363,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,   372,    -1,    -1,
     258,   259,     1,    -1,     3,    -1,    -1,   583,    -1,    -1,
     385,    -1,   270,    -1,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,   858,
      39,    -1,    -1,   862,    -1,    -1,    45,   349,   867,    -1,
      49,   870,   310,    52,    -1,    54,    -1,    -1,    -1,    -1,
      -1,   363,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
     372,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,   385,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      99,   359,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   944,    -1,   946,    -1,    31,
     949,    -1,   507,    -1,   509,    -1,    -1,   395,    -1,   397,
      -1,    -1,    -1,   401,    -1,    -1,    -1,    -1,    -1,     5,
      -1,    -1,    -1,   972,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   982,    -1,    -1,    22,   425,    -1,    -1,
      26,    27,   171,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    37,    38,    -1,    -1,    -1,    -1,    43,    90,    -1,
     766,    -1,    94,   769,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1023,   507,    -1,   509,   583,    31,
    1029,    -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      86,    -1,   490,    89,    -1,    91,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,    -1,    -1,    -1,
     152,    -1,    -1,   109,    -1,   111,    -1,   159,   114,    -1,
     518,  1080,    -1,   165,   166,   167,    -1,    -1,    90,    -1,
      -1,    -1,    94,   531,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   583,   858,   541,    -1,    -1,   862,   545,    -1,    -1,
      -1,   867,    -1,    -1,   870,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1121,    -1,  1123,    -1,    -1,    -1,   164,  1128,
      -1,    -1,  1131,    -1,   170,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,    -1,    -1,    -1,
     152,   589,    -1,    -1,    -1,    -1,   158,   159,    -1,    -1,
     162,  1160,   600,   165,   166,   167,    -1,  1166,    -1,    -1,
      -1,  1170,  1171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     618,    -1,    -1,  1182,    -1,    -1,    -1,  1186,   944,  1188,
     946,    -1,    -1,   949,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    -1,
    1209,   766,    -1,    -1,   769,    -1,   972,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   982,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,
      -1,   171,    -1,   173,    -1,    -1,   684,    -1,    -1,    -1,
      -1,    -1,  1251,  1252,  1253,  1254,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1023,    -1,    -1,
      -1,    -1,    -1,  1029,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,   766,    -1,    -1,   769,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,
      20,    -1,    -1,   858,    -1,    25,    -1,   862,    -1,    -1,
      -1,    -1,   867,    33,    -1,   870,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,  1080,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,   771,    54,    -1,    -1,   775,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,   785,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,  1121,   804,  1123,    88,    -1,
      -1,    -1,  1128,    -1,    -1,  1131,   858,    97,    -1,    99,
     862,   101,    -1,   103,    -1,   867,   106,    -1,   870,   944,
     110,   946,   112,    -1,   949,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1160,    -1,    -1,    -1,    -1,    -1,
    1166,    -1,   850,    -1,  1170,  1171,    -1,   972,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1182,   982,    -1,    -1,
    1186,    -1,  1188,    -1,    -1,    -1,   874,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
      -1,   171,    -1,  1209,    -1,    -1,    -1,    -1,    -1,    -1,
     898,    -1,   944,    -1,   946,    -1,    -1,   949,  1023,    -1,
      -1,    -1,    -1,    -1,  1029,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     972,    -1,    -1,    -1,    -1,  1251,  1252,  1253,  1254,    -1,
     982,    -1,    13,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,  1080,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    47,    48,    -1,   977,
      -1,  1023,    53,    -1,    55,    -1,    57,  1029,    -1,    -1,
      -1,    62,    -1,    -1,    -1,    66,    67,    68,    69,    70,
      -1,    -1,    -1,    74,    -1,    76,  1121,    -1,  1123,    -1,
      -1,    -1,    83,  1128,    -1,    86,  1131,    88,    -1,    90,
      -1,    -1,    93,    94,    -1,    96,    -1,    -1,    -1,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,  1080,    -1,
      -1,    -1,    -1,    -1,    -1,  1160,    -1,    -1,    -1,    -1,
      -1,  1166,    -1,  1051,    -1,  1170,  1171,    -1,   129,    -1,
     131,    -1,    -1,    -1,    -1,    -1,    -1,  1182,    -1,    -1,
      -1,  1186,    -1,  1188,    -1,    -1,    -1,    -1,    -1,  1121,
      -1,  1123,    -1,    -1,    -1,    -1,  1128,    -1,    -1,  1131,
      -1,    -1,    -1,    -1,  1209,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1160,    -1,
      -1,    -1,    -1,    -1,  1166,    -1,   197,    -1,  1170,  1171,
     201,   202,    -1,    -1,    -1,    -1,  1251,  1252,  1253,  1254,
    1182,  1139,    -1,    -1,  1186,    -1,  1188,    -1,    -1,    -1,
      -1,    -1,    -1,    25,     3,    -1,    -1,    -1,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1209,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,   265,    -1,    45,    -1,    -1,    -1,
      49,    -1,    74,    52,    -1,    54,    -1,    -1,    -1,  1251,
    1252,  1253,  1254,    -1,    63,    -1,    -1,    -1,    -1,   290,
      -1,    70,    71,    -1,    31,    -1,    -1,    -1,    -1,    78,
     301,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,   316,   317,    -1,    97,    -1,
      99,    -1,   101,    -1,   103,    -1,    -1,   106,   329,    -1,
      -1,   110,   333,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     351,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
     361,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   373,    -1,   375,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,   389,   390,
     391,    -1,   171,    -1,    -1,    -1,    -1,   398,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,   215,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    -1,   434,   435,   436,   437,   438,   439,   176,
     441,   442,   443,   444,   445,    -1,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,    -1,    -1,    -1,    -1,   269,    -1,   470,
     471,    -1,   274,    -1,    -1,   476,    -1,    -1,   479,   480,
     481,   482,   483,   484,    -1,    -1,    -1,   488,   489,    -1,
      -1,   492,    -1,    -1,    -1,    -1,    -1,   498,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,   509,    -1,
      -1,   512,     3,    -1,    -1,   516,    -1,    -1,    -1,    -1,
      -1,    -1,   523,    -1,   525,    -1,    17,    18,   529,    20,
      69,   532,   533,    -1,    25,    -1,    -1,    -1,    -1,    78,
      79,    -1,    33,   544,    -1,    -1,   547,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,   556,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,   566,    -1,    -1,   569,   570,
     109,    -1,    63,    -1,   575,   576,    -1,    -1,   579,    70,
      71,    -1,    -1,   385,    -1,    -1,    -1,    78,    -1,   590,
     591,    -1,    -1,    -1,    -1,    -1,    -1,    88,   137,    -1,
      -1,    -1,    -1,   604,    -1,    -1,    97,    -1,    99,    -1,
     101,    -1,   103,    -1,    -1,   106,     3,    -1,    -1,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,   630,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,   675,    -1,    63,    -1,    -1,    -1,
     171,   682,   221,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,   507,    -1,   509,    -1,    -1,
      97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,
      -1,   260,   261,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,     5,   737,   738,   277,    -1,
     279,    -1,    -1,    -1,    -1,    -1,    -1,   286,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,   760,
      -1,   762,   763,   764,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,   779,    -1,
      50,    -1,    -1,    -1,   171,    -1,    -1,   326,    -1,    -1,
      -1,    -1,    -1,   794,    -1,    -1,    -1,   798,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    86,    -1,    -1,    89,
      -1,    91,   823,    -1,    -1,    -1,   827,    -1,   829,   830,
     831,    -1,   833,    -1,   835,    -1,    -1,   838,   839,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,   852,    -1,    -1,    -1,   856,    -1,    -1,    -1,   860,
     861,   862,    -1,   864,   865,    -1,   867,    -1,    -1,   870,
     871,    -1,    -1,    -1,    -1,    -1,   877,    -1,    -1,    -1,
      -1,   882,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   914,   915,   916,   917,   918,   919,    -1,
      -1,    -1,   923,     5,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   937,   938,    -1,    -1,
      22,    -1,    -1,    -1,    26,    27,   947,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,
      -1,    43,   963,    -1,   766,    -1,   967,   769,    50,    -1,
      -1,    -1,   973,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     981,    -1,   983,   984,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    83,    -1,  1004,    86,    -1,    -1,    89,    -1,    91,
    1011,    -1,  1013,    -1,    -1,    -1,    -1,    -1,    -1,  1020,
     559,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,  1035,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1056,   858,    -1,    -1,    -1,
     862,    -1,   601,   602,    -1,   867,    -1,  1068,   870,    -1,
     609,   610,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   164,  1084,  1085,    -1,    -1,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1097,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1105,  1106,  1107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1121,    -1,  1123,    -1,    -1,  1126,    -1,  1128,    -1,    -1,
    1131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   944,    -1,   946,    -1,    -1,   949,    -1,    -1,
      -1,    -1,  1153,  1154,    -1,  1156,  1157,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     972,    -1,    -1,    -1,    -1,  1176,    -1,    -1,  1179,    -1,
     982,    -1,  1183,    -1,    -1,    -1,    -1,  1188,  1189,    -1,
      -1,  1192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1200,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1214,  1215,  1216,  1217,  1218,    -1,    -1,
      -1,  1023,    -1,    -1,    -1,    -1,    -1,  1029,    -1,    -1,
      -1,   770,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1251,  1252,  1253,  1254,  1255,  1256,  1257,  1258,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   804,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   814,   815,    -1,  1080,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1121,
      -1,  1123,    -1,    -1,    -1,    -1,  1128,    -1,    -1,  1131,
     869,    -1,    -1,    -1,   873,    -1,   875,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1160,    90,
      -1,    -1,    -1,    94,  1166,    -1,    -1,    -1,  1170,  1171,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1182,    -1,    -1,    -1,  1186,    -1,  1188,    -1,    -1,    -1,
      -1,    -1,   123,   124,   125,   126,   127,   128,   129,    -1,
      -1,   132,   133,   134,   135,    -1,   137,   138,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,   169,    -1,
      -1,   980,    -1,    -1,    -1,   176,    -1,    -1,    -1,  1251,
    1252,  1253,  1254,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,     3,     4,    -1,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    24,    25,    -1,    -1,  1037,  1038,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    42,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    12,    56,    -1,    -1,    59,
      60,    -1,    19,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    31,    -1,    -1,    -1,    78,    36,
      80,    -1,    82,    -1,    -1,    -1,  1095,    87,    88,    89,
      -1,    -1,  1101,  1102,    -1,    -1,    -1,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,  1118,
     110,    -1,   112,    -1,    -1,   115,    73,    -1,  1127,    -1,
      -1,  1130,    -1,   123,  1133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,  1168,
      -1,   161,   162,   163,    -1,    -1,   123,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1227,   176,
    1229,    -1,    -1,  1232,  1233,     0,     1,    -1,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,    71,    72,    -1,    74,
      -1,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       1,   176,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      -1,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,   169,   170,
     171,    -1,   173,    -1,     1,   176,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,
     167,    -1,   169,   170,   171,    -1,   173,    -1,     1,   176,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,   170,   171,    -1,
     173,    -1,     1,   176,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    -1,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,
     169,   170,   171,    -1,   173,    -1,     1,   176,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,    71,    72,    -1,    74,
      -1,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,   170,   171,    -1,   173,    -1,
       1,   176,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     1,   176,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     1,   176,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     1,   176,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      89,    -1,    91,    92,    93,    -1,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     1,   176,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,    71,    72,    -1,    74,
      -1,    -1,    77,    78,    79,    80,    -1,    82,    83,    84,
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       1,   176,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      -1,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     1,   176,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     1,   176,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,     1,
     173,     3,     4,   176,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    24,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      42,    -1,    44,    45,    -1,    47,    -1,    49,    -1,    51,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    78,    79,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,   110,    -1,
     112,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
     172,   173,     3,     4,   176,     6,     7,     8,     9,    10,
      -1,    12,    -1,    14,    -1,    -1,    17,    18,    19,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    55,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,   125,   126,   127,   128,   129,    -1,
      -1,   132,   133,   134,   135,    -1,   137,   138,    -1,   140,
     141,   142,   143,   144,   145,    -1,   147,    -1,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
     171,    -1,    -1,     3,     4,   176,     6,     7,     8,     9,
      10,    -1,    12,    -1,    14,    -1,    -1,    17,    18,    19,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    55,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,   132,   133,   134,   135,    -1,   137,   138,    -1,
     140,   141,   142,   143,   144,   145,    -1,   147,    -1,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,   165,   166,   167,   168,   169,
      -1,   171,    -1,    -1,    -1,    -1,   176,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      -1,    17,    18,    -1,    20,    -1,    22,    23,    -1,    25,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    64,    65,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,    -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,    -1,     3,     4,
     176,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    47,    -1,    49,    -1,    51,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,   109,   110,    -1,   112,    -1,   114,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,     3,
       4,   176,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    72,    -1,
      -1,    -1,    -1,    -1,    78,    79,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,    -1,
     114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,
       3,     4,   176,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    42,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,   111,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,     3,     4,   176,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      42,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
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
      -1,   173,     3,     4,   176,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    90,
      -1,    -1,    -1,    94,    -1,    -1,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,   146,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,   164,    -1,    -1,   167,    -1,   169,   170,
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
      -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,   169,
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
      99,   100,   101,    -1,   103,    -1,    -1,   106,   107,   108,
      -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
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
     167,    -1,   169,    -1,   171,    -1,    -1,   174,    -1,   176,
       3,     4,    -1,     6,     7,     8,     9,    10,    -1,    -1,
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
     172,    -1,     3,     4,   176,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,   172,    -1,     3,     4,   176,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    69,
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
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,   146,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,
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
      -1,    -1,   140,   141,   142,   143,   144,    -1,   146,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,
      -1,   169,   170,   171,    -1,    -1,     3,     4,   176,     6,
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
      -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,   146,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,
     167,    -1,   169,   170,   171,    -1,    -1,     3,     4,   176,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,   112,    -1,   114,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,    -1,     3,     4,
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
      -1,    -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,
      94,    -1,    -1,    97,    98,    99,   100,   101,    -1,   103,
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
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,
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
      -1,   140,   141,   142,   143,   144,    -1,   146,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,   172,    -1,     3,     4,   176,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,    -1,
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
      -1,   169,    -1,   171,    -1,    -1,     3,     4,   176,     6,
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
      -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,   121,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,    -1,     3,     4,   176,
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
     106,    -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,    -1,   174,    -1,
     176,     3,     4,    -1,     6,     7,     8,     9,    10,    -1,
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
     162,   163,    -1,    -1,    -1,   167,    -1,   169,   170,   171,
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
     171,    -1,    -1,   174,    -1,   176,     3,     4,    -1,     6,
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
      -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,    -1,   174,    -1,   176,
       3,     4,    -1,     6,     7,     8,     9,    10,    -1,    -1,
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
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,    -1,     3,     4,   176,     6,
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
      -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,     4,    -1,     6,     7,     8,     9,    10,
      -1,   158,    -1,    14,   161,   162,   163,    18,    -1,    -1,
     167,    22,   169,    24,   171,    -1,    27,    -1,    -1,   176,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,   172,   173,    -1,     4,   176,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    22,    -1,    24,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    79,
      80,    81,    82,    -1,    -1,    -1,    -1,    87,    -1,    89,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    97,    98,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    12,    -1,   149,
      -1,    -1,    17,    18,    19,    20,    -1,    -1,   158,    -1,
      25,   161,   162,   163,    -1,    -1,    -1,   167,    33,   169,
      35,   171,    -1,   173,    39,    -1,   176,    -1,    -1,    -1,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    99,    -1,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,    -1,    -1,   132,   133,   134,
     135,    -1,   137,   138,    -1,   140,   141,   142,   143,   144,
     145,    -1,   147,    -1,    -1,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,   162,    -1,    -1,
     165,   166,   167,   168,     3,     4,   171,     6,     7,     8,
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
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,    -1,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,     3,     4,    -1,     6,     7,
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
      -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,   142,    -1,    -1,     3,     4,    -1,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,   163,    22,    -1,    24,    25,
      -1,   169,    -1,   171,    -1,   173,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    42,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,     3,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    12,   112,    -1,    -1,   115,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,   142,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,   163,    -1,    -1,
      -1,    -1,    -1,    -1,    71,   171,    -1,   173,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,    -1,
      -1,    -1,   109,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,    -1,    -1,   132,   133,   134,   135,    -1,
     137,   138,    -1,   140,   141,   142,   143,   144,   145,    -1,
     147,    -1,    -1,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,     3,   162,    -1,    -1,   165,   166,
     167,   168,   169,    12,    -1,    -1,    -1,    -1,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    97,    -1,
      99,    -1,   101,    19,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,   112,    -1,    31,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,    -1,    -1,   132,   133,   134,   135,    -1,   137,   138,
      -1,   140,   141,   142,   143,   144,   145,    -1,   147,    -1,
      -1,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,    -1,   162,    -1,    -1,   165,   166,   167,   168,
      -1,    -1,    -1,    -1,    90,    -1,    12,    -1,    94,    -1,
      -1,    -1,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,   129,    -1,    -1,   132,   133,   134,   135,
      -1,   137,   138,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    90,    -1,    12,    -1,    94,    -1,
     176,    -1,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,   129,    -1,    -1,   132,   133,   134,   135,
      -1,   137,   138,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    90,    -1,    12,    -1,    94,    -1,
     176,    -1,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,   129,    -1,    -1,   132,   133,   134,   135,
      -1,   137,   138,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    90,    -1,    12,    -1,    94,    -1,
     176,    -1,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,   129,    -1,    -1,   132,   133,   134,   135,
      -1,   137,   138,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    90,    -1,    -1,    -1,    94,    -1,
     176,    -1,    -1,    -1,    -1,    12,   102,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,   123,   124,   125,
     126,   127,   128,   129,    -1,    -1,   132,   133,   134,   135,
      47,   137,   138,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    -1,    -1,    -1,    -1,    -1,    12,
     176,    -1,    -1,    90,    -1,    -1,    19,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,    -1,    47,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,    90,   165,   166,
     167,    94,   169,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,    31,
     123,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,    73,   165,   166,   167,    -1,   169,    -1,    -1,    -1,
      -1,    -1,    -1,   176,    12,    -1,    -1,    -1,    90,    -1,
      -1,    19,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    -1,    90,   165,   166,   167,    94,    -1,    12,    -1,
      -1,    -1,    -1,    -1,   176,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,   119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    12,    -1,    -1,    -1,    90,    -1,   176,    19,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
      90,   165,   166,   167,    94,   169,    12,    -1,    -1,    -1,
      -1,    -1,   176,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    -1,    -1,    -1,    90,    12,   176,    -1,    94,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
     176,    -1,    -1,    -1,    -1,   102,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,   123,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,    73,   165,   166,
     167,    -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      12,    -1,    -1,    -1,    90,    -1,    -1,    19,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    31,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    90,   165,
     166,   167,    94,    -1,    -1,    12,    13,    -1,    -1,    -1,
     176,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,   119,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    -1,   119,   165,   166,   167,   123,    -1,    -1,    -1,
      -1,    -1,   174,    90,   176,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    31,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   123,    -1,   165,   166,
     167,    -1,    -1,    -1,    12,    -1,    -1,   174,    -1,   176,
      -1,    19,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    31,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    90,    -1,    12,    -1,    94,    -1,    -1,   176,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    12,    -1,    94,    -1,    -1,    -1,
      -1,    19,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,    31,   152,   153,   154,    -1,   156,   157,
     158,   159,    -1,    -1,   162,   123,    -1,   165,   166,   167,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,   176,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   123,    -1,   165,   166,   167,
      -1,   169,    90,    12,    -1,    -1,    94,    -1,   176,    -1,
      19,    -1,   140,   141,   102,   143,   144,   145,    -1,   147,
     148,   149,    31,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   123,   164,   165,   166,   167,
      -1,    -1,    -1,    12,    13,    -1,    -1,    -1,   176,    -1,
      19,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    31,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    90,    -1,    -1,    12,    94,    -1,    -1,   176,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   123,    -1,   165,   166,   167,    -1,
      -1,    -1,    90,   172,    -1,    -1,    94,   176,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   122,   123,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    12,    -1,   165,   166,   167,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   176,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    12,    -1,    94,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   123,    -1,   165,   166,
     167,    -1,    -1,    90,    12,    13,    -1,    94,    -1,   176,
      -1,    19,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    31,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   123,    -1,   165,   166,
     167,    -1,    -1,    -1,    12,   172,    -1,    -1,    -1,   176,
      -1,    19,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    31,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    90,    -1,    -1,   172,    94,    -1,    -1,   176,
      -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   123,    -1,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,
      90,    -1,   140,   141,    94,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    12,    -1,   165,   166,   167,
      -1,    -1,    19,   123,   172,    -1,    -1,    -1,   176,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    12,    -1,   165,   166,   167,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,   140,   141,    94,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,   164,   165,   166,
     167,    -1,    -1,   122,   123,    -1,    -1,    -1,    -1,   176,
      -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   123,    -1,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    12,    -1,   165,   166,   167,    -1,
      -1,    19,    -1,    -1,     1,   174,     3,   176,     5,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    12,    13,    33,    -1,    -1,    -1,
      -1,    19,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    31,    -1,    52,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    90,    70,    71,    12,    94,    -1,    -1,    -1,
      -1,    78,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    99,    -1,   101,   123,   103,    -1,    -1,   106,
      -1,    -1,    90,   110,    -1,   112,    94,    -1,   115,    -1,
      -1,    -1,   140,   141,    61,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   123,    -1,   165,   166,   167,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,   176,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,   123,   165,   166,   167,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,   176,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    31,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,   176,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    12,    -1,
     102,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,   140,   141,
      94,   143,   144,   145,    -1,   147,   148,   149,   102,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,   176,    -1,    90,    12,    -1,    -1,
      94,    -1,    -1,    -1,    19,    -1,   140,   141,   102,   143,
     144,   145,    -1,   147,   148,   149,    31,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   123,
      -1,   165,   166,   167,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,   176,    -1,    19,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    31,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,    90,    -1,    12,    -1,    94,
      -1,    -1,   176,    -1,    19,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,   140,   141,   102,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   123,    -1,
     165,   166,   167,    -1,    -1,    90,    12,    -1,    -1,    94,
      -1,   176,    -1,    19,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    31,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   123,    -1,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,     5,    -1,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    -1,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    72,    26,    27,    -1,    -1,    77,
      -1,    79,    -1,    81,    35,    83,    37,    38,    86,    -1,
      -1,    89,    43,    91,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,   111,    -1,    -1,   114,    -1,    22,    -1,
      -1,    72,    26,    27,    -1,    -1,    77,    -1,    79,    -1,
      -1,    35,    83,    37,    38,    86,    -1,    -1,    89,    43,
      91,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,   109,    -1,
     111,    -1,    66,   114,    -1,    -1,   164,    -1,    72,    -1,
      74,    -1,    -1,    77,    -1,    79,     3,    -1,    -1,    83,
      84,    85,    86,    -1,    -1,    89,    -1,    91,    -1,    -1,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,   109,    33,   111,    -1,    -1,
     114,    -1,    39,   164,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     164,    88,    -1,    -1,    91,   169,    -1,     3,    -1,     5,
      97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,
      -1,    17,    18,   110,    20,   112,    -1,   114,   115,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      17,    18,    78,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    97,    39,    99,    -1,   101,    -1,   103,    45,    -1,
     106,    -1,    49,    -1,   110,    52,   112,    54,    -1,   115,
      -1,    -1,    -1,    -1,    -1,     3,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    17,
      18,    78,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      97,    39,    99,    -1,   101,    -1,   103,    45,    -1,   106,
      -1,    49,    -1,   110,    52,   112,    54,    -1,   115,    -1,
      -1,    -1,    -1,    -1,     3,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    17,    18,
      78,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    97,
      39,    99,    -1,   101,    -1,   103,    45,    -1,   106,    -1,
      49,    -1,   110,    52,   112,    54,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,   112,    -1,    -1,   115
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
     293,   314,   319,   320,   324,   325,   326,   348,   349,   350,
     352,   354,   355,   356,   362,   363,   364,   365,   366,   367,
     369,   372,   373,   374,   375,   376,   377,   378,   379,   381,
     382,   383,   384,   385,   164,    22,    35,    44,    54,    56,
      89,   101,   169,   239,   250,   277,   348,   355,   363,   364,
     369,   372,   374,   375,   119,   357,   358,     3,    54,   217,
     369,   357,   109,   327,    91,   217,   185,   342,   369,   171,
       3,    17,    18,    20,    25,    33,    39,    45,    49,    52,
      54,    63,    70,    71,    78,    88,    97,    99,   101,   103,
     106,   110,   112,   115,   218,   219,   221,    11,    27,   114,
     245,   369,    83,    86,   201,   171,   103,   219,   219,   219,
     171,   219,   329,   330,    32,   205,   224,   369,   262,   263,
     369,   369,    18,    78,    97,   112,   369,   369,   369,     9,
     171,   228,   227,    27,    33,    47,    49,    51,    76,    79,
      91,   101,   109,   114,   184,   223,   281,   282,   283,   305,
     306,   307,   334,   339,   369,   342,   107,   108,   164,   285,
     286,   368,   369,   371,   369,   224,   369,   369,   100,   171,
     185,   369,   369,    72,   187,   191,   205,   187,   205,   369,
     371,   369,   369,   369,   369,   369,     1,   170,   183,   206,
     342,   150,   343,   344,   371,   369,    81,   185,    22,    35,
      38,    72,    89,   169,   188,   189,   190,   201,   205,   193,
     148,   195,   169,    46,    85,   113,   202,    26,   326,   369,
       9,   266,   369,   370,    24,    32,    40,    41,    42,   118,
     173,   241,   242,   351,   353,   171,   205,    56,   145,   268,
     219,   169,   171,   301,    53,    74,    84,   309,    27,    47,
      51,    76,    79,    91,   109,   114,   310,    27,    47,    51,
      76,    79,    91,   109,   114,   308,   219,   321,   322,     1,
     329,   163,   164,   369,    12,    19,    31,    90,    94,   123,
     140,   141,   143,   144,   145,   147,   148,   149,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     165,   166,   167,   176,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   168,
     297,   171,   173,    90,    94,   369,   219,    40,   173,   241,
      56,     1,   171,   185,   164,   205,   325,   164,   146,   164,
     220,   345,   346,   347,   371,   219,   219,   102,   205,   102,
     124,   205,   297,   202,   345,   164,   171,   185,   171,   219,
     331,   332,     1,   145,   338,    47,   146,   185,   224,   146,
     224,    13,   171,   171,   224,   328,   345,   229,   229,    47,
      91,   306,   307,   172,   146,   171,   219,   145,   164,   369,
     369,   122,   287,   164,   169,   224,   171,   345,   164,   369,
     255,   255,   164,   170,   170,   183,   146,   170,   369,   146,
     146,   172,   172,     9,   369,   173,   241,   242,     3,   171,
     196,     1,   170,   206,   213,   214,   369,   208,   369,    66,
      36,    73,   164,   266,   268,   109,   234,   236,   290,   185,
      79,   245,   369,   121,   174,   243,   342,   369,   380,   243,
     369,   219,   248,   249,   219,   342,   169,     1,   192,   219,
     272,   275,   172,   302,   304,   305,   310,    47,    91,     1,
     145,   336,   337,    47,    91,     1,     3,    12,    17,    19,
      20,    25,    45,    52,    54,    55,    63,    71,    88,    99,
     103,   110,   115,   140,   141,   142,   143,   144,   145,   147,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     162,   165,   166,   167,   168,   171,   219,   294,   295,   296,
     297,   348,   124,   323,   146,   164,   164,   164,   369,   369,
     369,   243,   369,   243,   369,   369,   369,   369,   369,   369,
     369,     3,    20,    33,    63,   103,   109,   220,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,    69,   371,   371,   371,   371,
     371,   345,   345,   243,   369,   243,   369,   102,   171,   342,
     243,   369,   219,     1,    27,    47,    51,    76,    79,    91,
     109,   114,   172,   359,   360,   361,   369,   385,   329,   369,
     124,   172,   146,   124,   124,   185,    34,   185,   369,    34,
     369,    66,   172,   346,   332,   146,   172,   223,   369,   124,
     333,   369,   330,   262,   219,   324,   369,   369,   325,   172,
     219,   310,   283,   332,   145,   340,   341,   339,   288,   204,
       1,   260,   345,   172,    21,   256,   170,   172,   172,   343,
     371,   172,   343,   342,   243,   369,   195,   172,   197,   198,
     347,   170,   170,   146,   164,    13,   148,   146,   164,    13,
      36,    73,   219,   140,   141,   142,   143,   144,   158,   162,
     167,   207,   296,   297,   298,   369,   207,   209,   268,   169,
     237,   325,   118,   219,   224,   224,   171,   369,   174,   358,
      47,   146,   174,   358,   122,   146,   174,    32,   224,   358,
      32,    47,   224,   358,   146,   172,   164,   171,     1,   269,
     170,   275,   124,   146,   170,   146,   172,   338,     1,   223,
     334,   335,   105,   311,   171,   300,   369,   140,   148,   300,
     300,   369,   322,   171,   173,   164,   164,   164,   164,   164,
     164,   172,   174,   346,    47,   174,    32,    32,    47,   171,
     172,    47,    91,   146,   172,    17,    20,    25,    45,    52,
      63,    71,    88,    99,   110,   115,   348,    90,    90,   164,
     371,   347,   369,   369,    34,   185,    34,    34,   185,   102,
     205,   219,   172,   172,   331,   338,    69,   371,   219,   172,
     172,   329,   341,   145,   284,   172,   339,   150,   299,   333,
     369,   170,    75,   116,   170,   261,   172,   171,   205,   219,
     257,    47,   174,    47,   146,   172,   213,   220,    17,    18,
      20,    25,    45,    49,    52,    71,    78,    88,    97,    99,
     101,   110,   112,   115,   169,   212,   298,   369,   369,   207,
     209,   146,   164,    13,   164,   169,   269,   321,   329,   369,
      47,   342,   185,   174,   243,   369,   174,   185,   369,   174,
     369,   174,   369,   369,   224,    44,   369,   243,   369,   224,
     249,   205,   346,   170,    83,    86,   170,   184,   192,   226,
     369,   273,   274,   304,   311,    61,   117,   315,   302,   303,
     172,   295,   297,   172,   174,   172,   243,   369,    44,   243,
     369,   346,   360,   338,   348,   348,   185,   185,   369,   185,
      34,   164,   164,   333,   171,   171,   164,   299,   333,   334,
     311,   341,   369,   185,   239,   342,   257,   145,   205,   243,
     369,   243,   369,   198,   207,    13,    36,    73,   164,   164,
     298,   369,   369,   269,   170,   164,   164,   164,   369,   172,
     185,   174,   358,   174,   358,   185,   122,   369,    32,   224,
     358,    32,   224,   358,   172,   192,   226,   226,   275,   192,
     315,     3,    54,    93,   103,   316,   317,   318,   369,   291,
     172,   300,   300,   174,   174,   369,    32,    32,   172,   333,
      34,   185,   345,   345,   299,   333,    32,   224,   172,   369,
     174,   174,   170,   369,   207,   209,    13,   170,   224,    44,
     185,   369,   174,    44,   185,   369,   174,   369,   102,    44,
     369,   224,    44,   369,   224,   164,   226,   275,   278,   318,
     117,   146,   124,   151,   153,   154,   156,   157,    61,    32,
     164,   204,   312,   313,    44,    44,   102,    44,    44,   185,
     172,   172,   185,   205,   164,   164,   369,   369,   185,   369,
     185,   369,   369,   369,   313,   369,   317,   318,   318,   318,
     318,   318,   318,   316,   183,   369,   369,   369,   369,   163,
     163,   102,   102,   102,   102,   102,   102,   102,   102,   369,
     369,   369,   369
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
     297,   297,   297,   297,   298,   298,   299,   299,   300,   300,
     301,   301,   302,   302,   303,   303,   304,   305,   305,   305,
     305,   305,   305,   306,   306,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   309,   309,   309,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     311,   311,   312,   312,   313,   313,   314,   315,   315,   315,
     315,   315,   316,   316,   317,   317,   317,   317,   317,   317,
     317,   318,   318,   318,   319,   320,   320,   321,   321,   322,
     323,   323,   324,   324,   324,   324,   324,   325,   327,   326,
     328,   326,   326,   326,   329,   329,   330,   330,   331,   331,
     332,   332,   332,   333,   333,   333,   334,   335,   335,   335,
     336,   336,   337,   337,   338,   338,   338,   338,   339,   339,
     340,   341,   341,   342,   342,   343,   344,   344,   345,   345,
     346,   346,   347,   347,   348,   348,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   350,   351,   351,
     351,   351,   351,   351,   351,   352,   353,   353,   353,   353,
     353,   353,   353,   354,   355,   356,   356,   356,   356,   356,
     356,   356,   357,   357,   358,   358,   358,   358,   359,   359,
     360,   360,   360,   361,   361,   361,   361,   361,   361,   361,
     361,   361,   361,   362,   362,   362,   362,   362,   363,   363,
     363,   363,   363,   364,   365,   365,   365,   365,   365,   365,
     366,   367,   368,   368,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   370,   370,   371,   371,   371,   372,   372,   372,   372,
     373,   373,   373,   373,   373,   374,   374,   374,   375,   375,
     375,   375,   375,   375,   376,   376,   376,   376,   377,   377,
     378,   378,   379,   379,   379,   379,   379,   379,   379,   379,
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
       1,     1,     1,     1,     1,     1,     1,     2,     0,     3,
       2,     3,     1,     3,     0,     1,     1,     4,     5,     4,
       5,     6,     6,     0,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     1,     1,     0,     1,     1,     2,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     0,
       1,     1,     2,     1,     1,     1,     1,     2,     1,     1,
       0,     1,     1,     1,     1,     2,     1,     0,     2,     2,
       4,     4,     1,     3,     3,     3,     3,     3,     3,     3,
       2,     1,     1,     1,     3,     1,     2,     1,     3,     2,
       0,     2,     1,     2,     1,     1,     1,     1,     0,     5,
       0,     6,     3,     3,     1,     3,     3,     5,     1,     3,
       1,     2,     3,     0,     2,     2,     1,     1,     1,     1,
       2,     1,     0,     1,     0,     2,     2,     1,     1,     1,
       2,     0,     1,     1,     3,     1,     3,     3,     0,     1,
       1,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     4,     5,     2,     2,     1,     2,     1,
       2,     1,     2,     1,     2,     1,     1,     2,     6,     6,
       4,     4,     9,     9,     7,     2,     2,     3,     4,     6,
       6,     9,     9,     6,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     4,     3,     4,     2,     1,     3,
       4,     3,     3,     1,     1,     2,     1,     2,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     2,     8,
       8,     9,     9,     4,     3,     3,     2,     2,     2,     1,
       3,     4,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     4,     4,     4,     3,     3,
       3,     3,     5,     5,     3,     4,     3,     4,     1,     1,
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
#line 664 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 5953 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 668 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 5959 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 669 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 5965 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 676 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 5973 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 684 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 5981 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 688 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 5990 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 698 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 5998 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 703 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6006 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 710 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6012 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 711 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6018 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 712 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6024 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 713 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6030 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 714 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6036 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 715 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6042 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 716 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6048 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 717 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6054 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: TDEFER stmt  */
#line 719 "chpl.ypp"
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
#line 6076 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: try_stmt  */
#line 736 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6082 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: return_stmt  */
#line 737 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6088 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: TINIT TTHIS TSEMI  */
#line 739 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr));
    auto node = Init::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6100 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 747 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6113 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 756 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6126 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 765 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6134 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 769 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6145 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 776 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6154 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 783 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6160 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 784 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6166 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 785 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6172 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 786 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6178 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 787 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6184 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 788 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6190 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 789 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6196 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 791 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6202 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 793 "chpl.ypp"
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
#line 6226 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 813 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6238 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 821 "chpl.ypp"
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
#line 6259 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL do_stmt  */
#line 838 "chpl.ypp"
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
#line 6279 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TON expr do_stmt  */
#line 854 "chpl.ypp"
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
#line 6300 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 871 "chpl.ypp"
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
#line 6321 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL do_stmt  */
#line 888 "chpl.ypp"
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
#line 6341 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSYNC stmt  */
#line 904 "chpl.ypp"
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
#line 6363 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 930 "chpl.ypp"
  {
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 6371 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 951 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6379 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 958 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6388 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 965 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6396 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 969 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6407 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 979 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6413 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 981 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6421 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 985 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6429 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 992 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6438 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 997 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6448 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 1010 "chpl.ypp"
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
#line 6470 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 1031 "chpl.ypp"
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
#line 6491 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 1048 "chpl.ypp"
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
#line 6512 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1065 "chpl.ypp"
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
#line 6531 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1082 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6537 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1083 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6544 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1085 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6551 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1090 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6557 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1091 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6564 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1097 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6574 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1115 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6583 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1120 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6592 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1125 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6605 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1137 "chpl.ypp"
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
#line 6624 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1155 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6630 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1156 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6636 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1161 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6644 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1165 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6652 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1169 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6661 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1174 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6669 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1178 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6677 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1182 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6686 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1190 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6695 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1195 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6705 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1201 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6714 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1206 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6725 "bison-chpl-lib.cpp"
    break;

  case 90: /* opt_only_ls: %empty  */
#line 1214 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6731 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: renames_ls  */
#line 1215 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6737 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1220 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6746 "bison-chpl-lib.cpp"
    break;

  case 93: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1225 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6758 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1233 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6771 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1242 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6783 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1250 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6796 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1261 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6805 "bison-chpl-lib.cpp"
    break;

  case 114: /* import_expr: expr  */
#line 1296 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6813 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr TDOT all_op_name  */
#line 1300 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6822 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1305 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6831 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TAS ident_use  */
#line 1310 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6841 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1316 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6851 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_ls: import_expr  */
#line 1324 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6857 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_ls TCOMMA import_expr  */
#line 1325 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6863 "bison-chpl-lib.cpp"
    break;

  case 121: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1330 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6873 "bison-chpl-lib.cpp"
    break;

  case 122: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1339 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6881 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1343 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6889 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1347 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6897 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1351 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6905 "bison-chpl-lib.cpp"
    break;

  case 126: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1355 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6913 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1359 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6921 "bison-chpl-lib.cpp"
    break;

  case 128: /* opt_label_ident: %empty  */
#line 1367 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6927 "bison-chpl-lib.cpp"
    break;

  case 129: /* opt_label_ident: TIDENT  */
#line 1368 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6933 "bison-chpl-lib.cpp"
    break;

  case 130: /* opt_label_ident: TINIT  */
#line 1369 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6939 "bison-chpl-lib.cpp"
    break;

  case 139: /* ident_use: TIDENT  */
#line 1395 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6945 "bison-chpl-lib.cpp"
    break;

  case 140: /* ident_use: TINIT  */
#line 1396 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6951 "bison-chpl-lib.cpp"
    break;

  case 141: /* ident_use: TTHIS  */
#line 1397 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6957 "bison-chpl-lib.cpp"
    break;

  case 160: /* scalar_type: TBOOL  */
#line 1423 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6963 "bison-chpl-lib.cpp"
    break;

  case 161: /* scalar_type: TENUM  */
#line 1424 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6969 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TINT  */
#line 1425 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6975 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TUINT  */
#line 1426 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6981 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TREAL  */
#line 1427 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6987 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TIMAG  */
#line 1428 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6993 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TCOMPLEX  */
#line 1429 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6999 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TBYTES  */
#line 1430 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7005 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TSTRING  */
#line 1431 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7011 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TLOCALE  */
#line 1432 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7017 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TNOTHING  */
#line 1433 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7023 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TVOID  */
#line 1434 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7029 "bison-chpl-lib.cpp"
    break;

  case 175: /* do_stmt: TDO stmt  */
#line 1447 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7035 "bison-chpl-lib.cpp"
    break;

  case 176: /* do_stmt: block_stmt  */
#line 1448 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7041 "bison-chpl-lib.cpp"
    break;

  case 177: /* return_stmt: TRETURN TSEMI  */
#line 1453 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7052 "bison-chpl-lib.cpp"
    break;

  case 178: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1460 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7063 "bison-chpl-lib.cpp"
    break;

  case 179: /* class_level_stmt: TSEMI  */
#line 1470 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7071 "bison-chpl-lib.cpp"
    break;

  case 180: /* class_level_stmt: inner_class_level_stmt  */
#line 1474 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7081 "bison-chpl-lib.cpp"
    break;

  case 181: /* $@1: %empty  */
#line 1479 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7088 "bison-chpl-lib.cpp"
    break;

  case 182: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1481 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7097 "bison-chpl-lib.cpp"
    break;

  case 183: /* $@2: %empty  */
#line 1485 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7104 "bison-chpl-lib.cpp"
    break;

  case 184: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1487 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7113 "bison-chpl-lib.cpp"
    break;

  case 192: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1505 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7122 "bison-chpl-lib.cpp"
    break;

  case 193: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1510 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7131 "bison-chpl-lib.cpp"
    break;

  case 194: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1515 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7140 "bison-chpl-lib.cpp"
    break;

  case 195: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1520 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7148 "bison-chpl-lib.cpp"
    break;

  case 196: /* forwarding_decl_start: TFORWARDING  */
#line 1527 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7157 "bison-chpl-lib.cpp"
    break;

  case 197: /* extern_or_export: TEXTERN  */
#line 1534 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7163 "bison-chpl-lib.cpp"
    break;

  case 198: /* extern_or_export: TEXPORT  */
#line 1535 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7169 "bison-chpl-lib.cpp"
    break;

  case 199: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1540 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7179 "bison-chpl-lib.cpp"
    break;

  case 200: /* extern_export_kind_inner: TTYPE  */
#line 1553 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7188 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1562 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7198 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1569 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7210 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1577 "chpl.ypp"
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
#line 7232 "bison-chpl-lib.cpp"
    break;

  case 204: /* $@3: %empty  */
#line 1595 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7240 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 1599 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-4])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7249 "bison-chpl-lib.cpp"
    break;

  case 206: /* $@4: %empty  */
#line 1604 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7258 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr extern_export_kind_inner $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1609 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yylsp[-5]), (yyvsp[-3].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7267 "bison-chpl-lib.cpp"
    break;

  case 208: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1617 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7275 "bison-chpl-lib.cpp"
    break;

  case 209: /* no_loop_attributes: %empty  */
#line 1623 "chpl.ypp"
  {
    // Even though there are no attributes, there could be loop pragmas.
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 7284 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1629 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7290 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1630 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7296 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1647 "chpl.ypp"
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
#line 7323 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1670 "chpl.ypp"
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
#line 7346 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1689 "chpl.ypp"
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
#line 7369 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1708 "chpl.ypp"
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
#line 7398 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1733 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7407 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: for_loop_kw expr TIN expr task_intent_clause do_stmt  */
#line 1738 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7416 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1743 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7425 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator task_intent_clause do_stmt  */
#line 1748 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7434 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1753 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7443 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw expr task_intent_clause do_stmt  */
#line 1758 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7452 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1763 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7461 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: for_loop_kw zippered_iterator task_intent_clause do_stmt  */
#line 1768 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7470 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1773 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7479 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TLSBR expr_ls TIN expr task_intent_clause TRSBR stmt  */
#line 1778 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7488 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1783 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7497 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator task_intent_clause TRSBR stmt  */
#line 1788 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7506 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1793 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7515 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR expr_ls task_intent_clause TRSBR stmt  */
#line 1798 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7524 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1803 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7534 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TLSBR zippered_iterator task_intent_clause TRSBR stmt  */
#line 1809 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7544 "bison-chpl-lib.cpp"
    break;

  case 236: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1818 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7554 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr TTHEN stmt  */
#line 1827 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7562 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr block_stmt  */
#line 1831 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7570 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1835 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7578 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1839 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7586 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1843 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7594 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF ifvar block_stmt  */
#line 1847 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7602 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1851 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7610 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1855 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7618 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1859 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7627 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1864 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7636 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1869 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7645 "bison-chpl-lib.cpp"
    break;

  case 248: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1874 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7654 "bison-chpl-lib.cpp"
    break;

  case 249: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1882 "chpl.ypp"
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
#line 7674 "bison-chpl-lib.cpp"
    break;

  case 250: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1898 "chpl.ypp"
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
#line 7694 "bison-chpl-lib.cpp"
    break;

  case 251: /* interface_start: TINTERFACE ident_def  */
#line 1917 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Interface);
  }
#line 7702 "bison-chpl-lib.cpp"
    break;

  case 252: /* interface_stmt: interface_start TLP ifc_formal_ls TRP block_stmt  */
#line 1924 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-4].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), parts, (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7712 "bison-chpl-lib.cpp"
    break;

  case 253: /* interface_stmt: interface_start block_stmt  */
#line 1930 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-1].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), parts, nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7722 "bison-chpl-lib.cpp"
    break;

  case 254: /* ifc_formal_ls: ifc_formal  */
#line 1938 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7728 "bison-chpl-lib.cpp"
    break;

  case 255: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1939 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7734 "bison-chpl-lib.cpp"
    break;

  case 256: /* ifc_formal: ident_def  */
#line 1944 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7740 "bison-chpl-lib.cpp"
    break;

  case 276: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1987 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7748 "bison-chpl-lib.cpp"
    break;

  case 277: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1991 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7756 "bison-chpl-lib.cpp"
    break;

  case 278: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1995 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7764 "bison-chpl-lib.cpp"
    break;

  case 279: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2002 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7772 "bison-chpl-lib.cpp"
    break;

  case 280: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 2006 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7780 "bison-chpl-lib.cpp"
    break;

  case 281: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2010 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7788 "bison-chpl-lib.cpp"
    break;

  case 282: /* try_stmt: TTRY tryable_stmt  */
#line 2017 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7796 "bison-chpl-lib.cpp"
    break;

  case 283: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2021 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7804 "bison-chpl-lib.cpp"
    break;

  case 284: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2025 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7812 "bison-chpl-lib.cpp"
    break;

  case 285: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2029 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7820 "bison-chpl-lib.cpp"
    break;

  case 286: /* catch_expr_ls: %empty  */
#line 2035 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7826 "bison-chpl-lib.cpp"
    break;

  case 287: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2036 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7832 "bison-chpl-lib.cpp"
    break;

  case 288: /* catch_expr: TCATCH block_stmt  */
#line 2041 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7840 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2045 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7848 "bison-chpl-lib.cpp"
    break;

  case 290: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2049 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7856 "bison-chpl-lib.cpp"
    break;

  case 291: /* catch_expr_inner: ident_def  */
#line 2056 "chpl.ypp"
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
#line 7875 "bison-chpl-lib.cpp"
    break;

  case 292: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2071 "chpl.ypp"
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
#line 7894 "bison-chpl-lib.cpp"
    break;

  case 293: /* throw_stmt: TTHROW expr TSEMI  */
#line 2089 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7904 "bison-chpl-lib.cpp"
    break;

  case 294: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2097 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7913 "bison-chpl-lib.cpp"
    break;

  case 295: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2102 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7924 "bison-chpl-lib.cpp"
    break;

  case 296: /* when_stmt_ls: %empty  */
#line 2111 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7930 "bison-chpl-lib.cpp"
    break;

  case 297: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2112 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7936 "bison-chpl-lib.cpp"
    break;

  case 298: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2117 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7944 "bison-chpl-lib.cpp"
    break;

  case 299: /* when_stmt: TOTHERWISE stmt  */
#line 2121 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7953 "bison-chpl-lib.cpp"
    break;

  case 300: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2126 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 7963 "bison-chpl-lib.cpp"
    break;

  case 301: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2135 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 7972 "bison-chpl-lib.cpp"
    break;

  case 302: /* manager_expr: expr TAS ident_def  */
#line 2140 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 7980 "bison-chpl-lib.cpp"
    break;

  case 303: /* manager_expr: expr  */
#line 2144 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 7988 "bison-chpl-lib.cpp"
    break;

  case 304: /* manager_expr_ls: manager_expr  */
#line 2150 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7994 "bison-chpl-lib.cpp"
    break;

  case 305: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2151 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8000 "bison-chpl-lib.cpp"
    break;

  case 306: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2156 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8009 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2166 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8019 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2172 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8031 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_start: class_tag ident_def  */
#line 2184 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8039 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_tag: TCLASS  */
#line 2190 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8045 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_tag: TRECORD  */
#line 2191 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8051 "bison-chpl-lib.cpp"
    break;

  case 312: /* class_tag: TUNION  */
#line 2192 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8057 "bison-chpl-lib.cpp"
    break;

  case 313: /* opt_inherit: %empty  */
#line 2196 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8063 "bison-chpl-lib.cpp"
    break;

  case 314: /* opt_inherit: TCOLON expr_ls  */
#line 2197 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8069 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_level_stmt_ls: %empty  */
#line 2201 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8078 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2206 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8086 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2210 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8094 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2214 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8102 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2218 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8110 "bison-chpl-lib.cpp"
    break;

  case 320: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
#line 2226 "chpl.ypp"
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
#line 8133 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
#line 2245 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8147 "bison-chpl-lib.cpp"
    break;

  case 322: /* enum_header: TENUM ident_def  */
#line 2258 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8155 "bison-chpl-lib.cpp"
    break;

  case 323: /* enum_ls: enum_item  */
#line 2265 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8164 "bison-chpl-lib.cpp"
    break;

  case 324: /* enum_ls: enum_ls TCOMMA  */
#line 2270 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8174 "bison-chpl-lib.cpp"
    break;

  case 325: /* $@5: %empty  */
#line 2276 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8183 "bison-chpl-lib.cpp"
    break;

  case 326: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2281 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8192 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2286 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8201 "bison-chpl-lib.cpp"
    break;

  case 328: /* $@6: %empty  */
#line 2291 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8210 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2296 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8219 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_item: ident_def  */
#line 2306 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[0])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8231 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_item: ident_def TASSIGN expr  */
#line 2314 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[-2])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8245 "bison-chpl-lib.cpp"
    break;

  case 332: /* lambda_decl_start: TLAMBDA  */
#line 2327 "chpl.ypp"
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
#line 8261 "bison-chpl-lib.cpp"
    break;

  case 333: /* $@7: %empty  */
#line 2344 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8271 "bison-chpl-lib.cpp"
    break;

  case 334: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2350 "chpl.ypp"
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
#line 8294 "bison-chpl-lib.cpp"
    break;

  case 336: /* linkage_spec: linkage_spec_empty  */
#line 2374 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8300 "bison-chpl-lib.cpp"
    break;

  case 337: /* linkage_spec: TINLINE  */
#line 2375 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8307 "bison-chpl-lib.cpp"
    break;

  case 338: /* linkage_spec: TOVERRIDE  */
#line 2377 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8314 "bison-chpl-lib.cpp"
    break;

  case 339: /* opt_fn_type_formal_ls: %empty  */
#line 2382 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8320 "bison-chpl-lib.cpp"
    break;

  case 340: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2383 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8326 "bison-chpl-lib.cpp"
    break;

  case 341: /* fn_type_formal_ls: fn_type_formal  */
#line 2387 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8332 "bison-chpl-lib.cpp"
    break;

  case 342: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2388 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8338 "bison-chpl-lib.cpp"
    break;

  case 343: /* fn_type_formal: named_formal  */
#line 2393 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8344 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2396 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8350 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_type_formal: formal_type  */
#line 2398 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8356 "bison-chpl-lib.cpp"
    break;

  case 346: /* opt_fn_type_ret_type: %empty  */
#line 2402 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8362 "bison-chpl-lib.cpp"
    break;

  case 347: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2403 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8368 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2416 "chpl.ypp"
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
#line 8388 "bison-chpl-lib.cpp"
    break;

  case 349: /* $@8: %empty  */
#line 2435 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8398 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2441 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8411 "bison-chpl-lib.cpp"
    break;

  case 351: /* $@9: %empty  */
#line 2450 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8421 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2456 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8434 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2468 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8442 "bison-chpl-lib.cpp"
    break;

  case 354: /* $@10: %empty  */
#line 2477 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8450 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2481 "chpl.ypp"
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
#line 8473 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2503 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.thisIntentLoc = (yylsp[-2]);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8486 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2512 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.thisIntentLoc = (yylsp[-2]);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8499 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2521 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.thisIntentLoc = (yylsp[-4]);
      fp.receiver = context->buildThisFormal((yylsp[-3]), (yylsp[-4]), (yyvsp[-4].intentTag), (yyvsp[-3].expr), /*initExpr*/ nullptr);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8513 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2531 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.thisIntentLoc = (yylsp[-4]);
      fp.receiver = context->buildThisFormal((yylsp[-3]), (yylsp[-4]), (yyvsp[-4].intentTag), (yyvsp[-3].expr), /*initExpr*/ nullptr);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8527 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2541 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8537 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2550 "chpl.ypp"
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
#line 8553 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2566 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 8562 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_ident: ident_def TBANG  */
#line 2576 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8572 "bison-chpl-lib.cpp"
    break;

  case 406: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2633 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8578 "bison-chpl-lib.cpp"
    break;

  case 407: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2634 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8584 "bison-chpl-lib.cpp"
    break;

  case 408: /* opt_formal_ls: %empty  */
#line 2638 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8590 "bison-chpl-lib.cpp"
    break;

  case 409: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2639 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8596 "bison-chpl-lib.cpp"
    break;

  case 410: /* req_formal_ls: TLP TRP  */
#line 2643 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8602 "bison-chpl-lib.cpp"
    break;

  case 411: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2644 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8608 "bison-chpl-lib.cpp"
    break;

  case 412: /* formal_ls_inner: formal  */
#line 2648 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8614 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2649 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8620 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_ls: %empty  */
#line 2653 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8626 "bison-chpl-lib.cpp"
    break;

  case 415: /* formal_ls: formal_ls_inner  */
#line 2654 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8632 "bison-chpl-lib.cpp"
    break;

  case 417: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2663 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8640 "bison-chpl-lib.cpp"
    break;

  case 418: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2668 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8648 "bison-chpl-lib.cpp"
    break;

  case 419: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2673 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8656 "bison-chpl-lib.cpp"
    break;

  case 420: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2678 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8664 "bison-chpl-lib.cpp"
    break;

  case 421: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2683 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yylsp[-5]), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8672 "bison-chpl-lib.cpp"
    break;

  case 422: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2688 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8680 "bison-chpl-lib.cpp"
    break;

  case 423: /* opt_formal_intent_tag: %empty  */
#line 2694 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);
  }
#line 8689 "bison-chpl-lib.cpp"
    break;

  case 424: /* opt_formal_intent_tag: required_intent_tag  */
#line 2699 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8698 "bison-chpl-lib.cpp"
    break;

  case 425: /* required_intent_tag: TIN  */
#line 2706 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::IN);            }
#line 8704 "bison-chpl-lib.cpp"
    break;

  case 426: /* required_intent_tag: TINOUT  */
#line 2707 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::INOUT);         }
#line 8710 "bison-chpl-lib.cpp"
    break;

  case 427: /* required_intent_tag: TOUT  */
#line 2708 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::OUT);           }
#line 8716 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TCONST TIN  */
#line 2709 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_IN);      }
#line 8722 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TCONST TREF  */
#line 2710 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);     }
#line 8728 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TCONST  */
#line 2711 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);         }
#line 8734 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TPARAM  */
#line 2712 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);         }
#line 8740 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TREF  */
#line 2713 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);           }
#line 8746 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TTYPE  */
#line 2714 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);          }
#line 8752 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TVAR  */
#line 2715 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR); }
#line 8758 "bison-chpl-lib.cpp"
    break;

  case 435: /* opt_this_intent_tag: %empty  */
#line 2719 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);     }
#line 8764 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_this_intent_tag: TPARAM  */
#line 2720 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);              }
#line 8770 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: TREF  */
#line 2721 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);                }
#line 8776 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TCONST TREF  */
#line 2722 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);          }
#line 8782 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TCONST  */
#line 2723 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);              }
#line 8788 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TTYPE  */
#line 2724 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);               }
#line 8794 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TIN  */
#line 2725 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::IN);       }
#line 8800 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TCONST TIN  */
#line 2726 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::CONST_IN); }
#line 8806 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_this_intent_tag: TVAR  */
#line 2727 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR);      }
#line 8812 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_this_intent_tag: TOUT  */
#line 2728 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::OUT);      }
#line 8818 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_this_intent_tag: TINOUT  */
#line 2729 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::INOUT);    }
#line 8824 "bison-chpl-lib.cpp"
    break;

  case 446: /* proc_iter_or_op: TPROC  */
#line 2733 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8830 "bison-chpl-lib.cpp"
    break;

  case 447: /* proc_iter_or_op: TITER  */
#line 2734 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8836 "bison-chpl-lib.cpp"
    break;

  case 448: /* proc_iter_or_op: TOPERATOR  */
#line 2735 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8842 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: %empty  */
#line 2739 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::DEFAULT_RETURN_INTENT); }
#line 8848 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TOUT  */
#line 2740 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::OUT);                   }
#line 8854 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TCONST  */
#line 2741 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST);                 }
#line 8860 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TCONST TREF  */
#line 2742 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST_REF);             }
#line 8866 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_ret_tag: TREF  */
#line 2743 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::REF);                   }
#line 8872 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: TPARAM  */
#line 2744 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::PARAM);                 }
#line 8878 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: TTYPE  */
#line 2745 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::TYPE);                  }
#line 8884 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: TIN  */
#line 2746 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::IN);            }
#line 8890 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_ret_tag: TCONST TIN  */
#line 2747 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::CONST_IN);      }
#line 8896 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_ret_tag: TVAR  */
#line 2748 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::VAR);           }
#line 8902 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_ret_tag: TINOUT  */
#line 2749 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::INOUT);         }
#line 8908 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_throws_error: %empty  */
#line 2753 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8914 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_throws_error: TTHROWS  */
#line 2754 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8920 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_function_body_stmt: TSEMI  */
#line 2757 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8926 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_function_body_stmt: function_body_stmt  */
#line 2758 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8932 "bison-chpl-lib.cpp"
    break;

  case 464: /* function_body_stmt: block_stmt_body  */
#line 2762 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8938 "bison-chpl-lib.cpp"
    break;

  case 465: /* function_body_stmt: TDO toplevel_stmt  */
#line 2763 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 8944 "bison-chpl-lib.cpp"
    break;

  case 466: /* query_expr: TQUERIEDIDENT  */
#line 2767 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8950 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_lifetime_where: %empty  */
#line 2772 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8956 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_lifetime_where: TWHERE expr  */
#line 2774 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8962 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2776 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8968 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2778 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 8974 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2780 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 8980 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_components_expr: lifetime_expr  */
#line 2785 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8986 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2787 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8992 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2792 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8998 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2794 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9004 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2796 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9010 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2798 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9016 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2800 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9022 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2802 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9028 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2804 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9034 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_ident: TIDENT  */
#line 2808 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9040 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_ident: TINIT  */
#line 2809 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9046 "bison-chpl-lib.cpp"
    break;

  case 483: /* lifetime_ident: TTHIS  */
#line 2810 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9052 "bison-chpl-lib.cpp"
    break;

  case 484: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2815 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9061 "bison-chpl-lib.cpp"
    break;

  case 485: /* type_alias_decl_stmt_start: TTYPE  */
#line 2824 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9071 "bison-chpl-lib.cpp"
    break;

  case 486: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2830 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9082 "bison-chpl-lib.cpp"
    break;

  case 487: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2840 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9090 "bison-chpl-lib.cpp"
    break;

  case 488: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2844 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9098 "bison-chpl-lib.cpp"
    break;

  case 489: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2851 "chpl.ypp"
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
#line 9121 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_init_type: %empty  */
#line 2872 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9127 "bison-chpl-lib.cpp"
    break;

  case 491: /* opt_init_type: TASSIGN expr  */
#line 2874 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9133 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_type: TPARAM  */
#line 2878 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9139 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_type: TCONST TREF  */
#line 2879 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9145 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_type: TREF  */
#line 2880 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9151 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_type: TCONST  */
#line 2881 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9157 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_type: TVAR  */
#line 2882 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9163 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_start: var_decl_type  */
#line 2887 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9172 "bison-chpl-lib.cpp"
    break;

  case 498: /* $@11: %empty  */
#line 2895 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9181 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt: TCONFIG $@11 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2899 "chpl.ypp"
                                              {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-4]), (yylsp[-3])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9190 "bison-chpl-lib.cpp"
    break;

  case 500: /* $@12: %empty  */
#line 2904 "chpl.ypp"
  {
    // Use a mid-rule action to preserve the 'on' expr
    context->noteVarDestinationExpr((yyvsp[0].expr));
  }
#line 9199 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt: TON expr $@12 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2909 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-4])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9208 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt: var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2914 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9217 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt: var_decl_start error TSEMI  */
#line 2919 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    delete (yyvsp[-2].attributeGroup);
    context->resetDeclStateOnError();
  }
#line 9227 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2928 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9235 "bison-chpl-lib.cpp"
    break;

  case 505: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2932 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9243 "bison-chpl-lib.cpp"
    break;

  case 506: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2939 "chpl.ypp"
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
#line 9266 "bison-chpl-lib.cpp"
    break;

  case 507: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2958 "chpl.ypp"
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
#line 9286 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_var_decl_component: ident_def  */
#line 2977 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9294 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2981 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9302 "bison-chpl-lib.cpp"
    break;

  case 510: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2988 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9308 "bison-chpl-lib.cpp"
    break;

  case 511: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2990 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9314 "bison-chpl-lib.cpp"
    break;

  case 512: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2992 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9320 "bison-chpl-lib.cpp"
    break;

  case 513: /* opt_init_expr: %empty  */
#line 2998 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9326 "bison-chpl-lib.cpp"
    break;

  case 514: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2999 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9332 "bison-chpl-lib.cpp"
    break;

  case 515: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 3000 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9338 "bison-chpl-lib.cpp"
    break;

  case 516: /* formal_or_ret_type_expr: expr  */
#line 3004 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9344 "bison-chpl-lib.cpp"
    break;

  case 517: /* ret_type: formal_or_ret_type_expr  */
#line 3008 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9350 "bison-chpl-lib.cpp"
    break;

  case 518: /* ret_type: reserved_type_ident_use  */
#line 3009 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9356 "bison-chpl-lib.cpp"
    break;

  case 519: /* ret_type: error  */
#line 3010 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9362 "bison-chpl-lib.cpp"
    break;

  case 520: /* colon_ret_type: TCOLON ret_type  */
#line 3014 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9368 "bison-chpl-lib.cpp"
    break;

  case 521: /* colon_ret_type: error  */
#line 3015 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9374 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_ret_type: %empty  */
#line 3019 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9380 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_type: %empty  */
#line 3024 "chpl.ypp"
                                 { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9386 "bison-chpl-lib.cpp"
    break;

  case 525: /* opt_type: TCOLON expr  */
#line 3025 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9392 "bison-chpl-lib.cpp"
    break;

  case 526: /* opt_type: TCOLON reserved_type_ident_use  */
#line 3026 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9398 "bison-chpl-lib.cpp"
    break;

  case 527: /* opt_type: error  */
#line 3027 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9404 "bison-chpl-lib.cpp"
    break;

  case 528: /* formal_type: formal_or_ret_type_expr  */
#line 3031 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9410 "bison-chpl-lib.cpp"
    break;

  case 529: /* formal_type: reserved_type_ident_use  */
#line 3032 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9416 "bison-chpl-lib.cpp"
    break;

  case 530: /* colon_formal_type: TCOLON formal_type  */
#line 3036 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9422 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_colon_formal_type: %empty  */
#line 3040 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9428 "bison-chpl-lib.cpp"
    break;

  case 532: /* opt_colon_formal_type: colon_formal_type  */
#line 3041 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9434 "bison-chpl-lib.cpp"
    break;

  case 533: /* expr_ls: expr  */
#line 3047 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9440 "bison-chpl-lib.cpp"
    break;

  case 534: /* expr_ls: expr_ls TCOMMA expr  */
#line 3048 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9446 "bison-chpl-lib.cpp"
    break;

  case 535: /* tuple_component: opt_try_expr  */
#line 3052 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9452 "bison-chpl-lib.cpp"
    break;

  case 536: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3057 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9460 "bison-chpl-lib.cpp"
    break;

  case 537: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3061 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9468 "bison-chpl-lib.cpp"
    break;

  case 538: /* opt_actual_ls: %empty  */
#line 3067 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9474 "bison-chpl-lib.cpp"
    break;

  case 539: /* opt_actual_ls: actual_ls  */
#line 3068 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9480 "bison-chpl-lib.cpp"
    break;

  case 540: /* actual_ls: actual_expr  */
#line 3073 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9489 "bison-chpl-lib.cpp"
    break;

  case 541: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3078 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9499 "bison-chpl-lib.cpp"
    break;

  case 542: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3086 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9505 "bison-chpl-lib.cpp"
    break;

  case 543: /* actual_expr: opt_try_expr  */
#line 3087 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9511 "bison-chpl-lib.cpp"
    break;

  case 544: /* ident_expr: ident_use  */
#line 3091 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9517 "bison-chpl-lib.cpp"
    break;

  case 545: /* ident_expr: scalar_type  */
#line 3092 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9523 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3105 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9529 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3107 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9535 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3109 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9541 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3111 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9551 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TATOMIC expr  */
#line 3117 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9557 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TSYNC expr  */
#line 3119 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9563 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TOWNED  */
#line 3122 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9569 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TOWNED expr  */
#line 3124 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9575 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TUNMANAGED  */
#line 3126 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9581 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3128 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9587 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TSHARED  */
#line 3130 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9593 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TSHARED expr  */
#line 3132 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9599 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TBORROWED  */
#line 3134 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9605 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TBORROWED expr  */
#line 3136 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9611 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TCLASS  */
#line 3138 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9617 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TRECORD  */
#line 3140 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9623 "bison-chpl-lib.cpp"
    break;

  case 567: /* for_expr: no_loop_attributes for_expr_base  */
#line 3144 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9629 "bison-chpl-lib.cpp"
    break;

  case 568: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3149 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9637 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3153 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9645 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3157 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9653 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr_base: for_loop_kw zippered_iterator TDO expr  */
#line 3161 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "expression-level loops with 'zip' must have an index.");
  }
#line 9661 "bison-chpl-lib.cpp"
    break;

  case 572: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3165 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9674 "bison-chpl-lib.cpp"
    break;

  case 573: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3174 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9687 "bison-chpl-lib.cpp"
    break;

  case 574: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3183 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9700 "bison-chpl-lib.cpp"
    break;

  case 575: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3194 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9706 "bison-chpl-lib.cpp"
    break;

  case 576: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3199 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9714 "bison-chpl-lib.cpp"
    break;

  case 577: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3203 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9722 "bison-chpl-lib.cpp"
    break;

  case 578: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3207 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9730 "bison-chpl-lib.cpp"
    break;

  case 579: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3211 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9738 "bison-chpl-lib.cpp"
    break;

  case 580: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3215 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9746 "bison-chpl-lib.cpp"
    break;

  case 581: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3219 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9754 "bison-chpl-lib.cpp"
    break;

  case 582: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3223 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9762 "bison-chpl-lib.cpp"
    break;

  case 583: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3230 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9776 "bison-chpl-lib.cpp"
    break;

  case 584: /* nil_expr: TNIL  */
#line 3247 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9782 "bison-chpl-lib.cpp"
    break;

  case 592: /* opt_task_intent_ls: %empty  */
#line 3265 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9788 "bison-chpl-lib.cpp"
    break;

  case 593: /* opt_task_intent_ls: task_intent_clause  */
#line 3266 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9794 "bison-chpl-lib.cpp"
    break;

  case 594: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3271 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9802 "bison-chpl-lib.cpp"
    break;

  case 595: /* task_intent_clause: TWITH TLP TRP  */
#line 3275 "chpl.ypp"
  {
    auto err = context->syntax((yyloc), "'with' clause cannot be empty.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-2]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9813 "bison-chpl-lib.cpp"
    break;

  case 596: /* task_intent_clause: TWITH TLP error TRP  */
#line 3282 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[-2]), "invalid intent expression in 'with' clause.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9824 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_intent_clause: TWITH error  */
#line 3289 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[0]), "missing parentheses around 'with' clause intents.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-1]), dummyLoc, dummyLoc, dummyLoc, exprs);
  }
#line 9835 "bison-chpl-lib.cpp"
    break;

  case 598: /* task_intent_ls: intent_expr  */
#line 3298 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9841 "bison-chpl-lib.cpp"
    break;

  case 599: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3299 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9847 "bison-chpl-lib.cpp"
    break;

  case 600: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3304 "chpl.ypp"
  {
    (yyval.expr) = context->buildTaskIntent((yyloc), (yylsp[-2]), nullptr, (yyvsp[-2].expr), (yyvsp[-3].taskIntent), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9855 "bison-chpl-lib.cpp"
    break;

  case 601: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3308 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9863 "bison-chpl-lib.cpp"
    break;

  case 602: /* intent_expr: expr TREDUCE ident_expr  */
#line 3312 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9871 "bison-chpl-lib.cpp"
    break;

  case 603: /* task_var_prefix: TCONST  */
#line 3318 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST);          }
#line 9877 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_var_prefix: TIN  */
#line 3319 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::IN);             }
#line 9883 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_var_prefix: TCONST TIN  */
#line 3320 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_IN);       }
#line 9889 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_var_prefix: TREF  */
#line 3321 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::REF);            }
#line 9895 "bison-chpl-lib.cpp"
    break;

  case 607: /* task_var_prefix: TCONST TREF  */
#line 3322 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_REF);      }
#line 9901 "bison-chpl-lib.cpp"
    break;

  case 608: /* task_var_prefix: TVAR  */
#line 3323 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::VAR);            }
#line 9907 "bison-chpl-lib.cpp"
    break;

  case 609: /* task_var_prefix: TOUT  */
#line 3324 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::OUT);   }
#line 9913 "bison-chpl-lib.cpp"
    break;

  case 610: /* task_var_prefix: TINOUT  */
#line 3325 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::INOUT); }
#line 9919 "bison-chpl-lib.cpp"
    break;

  case 611: /* task_var_prefix: TPARAM  */
#line 3326 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::PARAM); }
#line 9925 "bison-chpl-lib.cpp"
    break;

  case 612: /* task_var_prefix: TTYPE  */
#line 3327 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::TYPE);  }
#line 9931 "bison-chpl-lib.cpp"
    break;

  case 613: /* new_maybe_decorated: TNEW  */
#line 3332 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9937 "bison-chpl-lib.cpp"
    break;

  case 614: /* new_maybe_decorated: TNEW TOWNED  */
#line 3334 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9943 "bison-chpl-lib.cpp"
    break;

  case 615: /* new_maybe_decorated: TNEW TSHARED  */
#line 3336 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9949 "bison-chpl-lib.cpp"
    break;

  case 616: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3338 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9955 "bison-chpl-lib.cpp"
    break;

  case 617: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3340 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9961 "bison-chpl-lib.cpp"
    break;

  case 618: /* new_expr: new_maybe_decorated expr  */
#line 3346 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 9969 "bison-chpl-lib.cpp"
    break;

  case 619: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3353 "chpl.ypp"
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
#line 9986 "bison-chpl-lib.cpp"
    break;

  case 620: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3366 "chpl.ypp"
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
#line 10003 "bison-chpl-lib.cpp"
    break;

  case 621: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3379 "chpl.ypp"
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
#line 10021 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3393 "chpl.ypp"
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
#line 10040 "bison-chpl-lib.cpp"
    break;

  case 623: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3411 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10048 "bison-chpl-lib.cpp"
    break;

  case 624: /* range_literal_expr: expr TDOTDOT expr  */
#line 3418 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10057 "bison-chpl-lib.cpp"
    break;

  case 625: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3423 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10066 "bison-chpl-lib.cpp"
    break;

  case 626: /* range_literal_expr: expr TDOTDOT  */
#line 3428 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10075 "bison-chpl-lib.cpp"
    break;

  case 627: /* range_literal_expr: TDOTDOT expr  */
#line 3433 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10085 "bison-chpl-lib.cpp"
    break;

  case 628: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3439 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10095 "bison-chpl-lib.cpp"
    break;

  case 629: /* range_literal_expr: TDOTDOT  */
#line 3445 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10105 "bison-chpl-lib.cpp"
    break;

  case 630: /* cast_expr: expr TCOLON expr  */
#line 3475 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10113 "bison-chpl-lib.cpp"
    break;

  case 631: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3482 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10121 "bison-chpl-lib.cpp"
    break;

  case 632: /* super_expr: fn_expr  */
#line 3488 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10127 "bison-chpl-lib.cpp"
    break;

  case 636: /* expr: sub_type_level_expr TQUESTION  */
#line 3497 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10133 "bison-chpl-lib.cpp"
    break;

  case 637: /* expr: TQUESTION  */
#line 3499 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10139 "bison-chpl-lib.cpp"
    break;

  case 641: /* expr: fn_type  */
#line 3504 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10145 "bison-chpl-lib.cpp"
    break;

  case 651: /* opt_expr: %empty  */
#line 3518 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10151 "bison-chpl-lib.cpp"
    break;

  case 652: /* opt_expr: expr  */
#line 3519 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10157 "bison-chpl-lib.cpp"
    break;

  case 653: /* opt_try_expr: TTRY expr  */
#line 3523 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10163 "bison-chpl-lib.cpp"
    break;

  case 654: /* opt_try_expr: TTRYBANG expr  */
#line 3524 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10169 "bison-chpl-lib.cpp"
    break;

  case 655: /* opt_try_expr: super_expr  */
#line 3525 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10175 "bison-chpl-lib.cpp"
    break;

  case 661: /* call_base_expr: expr TBANG  */
#line 3542 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10183 "bison-chpl-lib.cpp"
    break;

  case 662: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3545 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10189 "bison-chpl-lib.cpp"
    break;

  case 665: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3552 "chpl.ypp"
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
#line 10205 "bison-chpl-lib.cpp"
    break;

  case 666: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3564 "chpl.ypp"
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
#line 10221 "bison-chpl-lib.cpp"
    break;

  case 667: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3576 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10229 "bison-chpl-lib.cpp"
    break;

  case 668: /* dot_expr: expr TDOT ident_use  */
#line 3583 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10235 "bison-chpl-lib.cpp"
    break;

  case 669: /* dot_expr: expr TDOT TTYPE  */
#line 3585 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10241 "bison-chpl-lib.cpp"
    break;

  case 670: /* dot_expr: expr TDOT TDOMAIN  */
#line 3587 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10247 "bison-chpl-lib.cpp"
    break;

  case 671: /* dot_expr: expr TDOT TLOCALE  */
#line 3589 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10253 "bison-chpl-lib.cpp"
    break;

  case 672: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3591 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10259 "bison-chpl-lib.cpp"
    break;

  case 673: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3593 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10265 "bison-chpl-lib.cpp"
    break;

  case 674: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3602 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10275 "bison-chpl-lib.cpp"
    break;

  case 675: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3608 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10283 "bison-chpl-lib.cpp"
    break;

  case 676: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3612 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10291 "bison-chpl-lib.cpp"
    break;

  case 677: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3616 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10299 "bison-chpl-lib.cpp"
    break;

  case 678: /* bool_literal: TFALSE  */
#line 3622 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10305 "bison-chpl-lib.cpp"
    break;

  case 679: /* bool_literal: TTRUE  */
#line 3623 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10311 "bison-chpl-lib.cpp"
    break;

  case 680: /* str_bytes_literal: STRINGLITERAL  */
#line 3627 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10317 "bison-chpl-lib.cpp"
    break;

  case 681: /* str_bytes_literal: BYTESLITERAL  */
#line 3628 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10323 "bison-chpl-lib.cpp"
    break;

  case 684: /* literal_expr: INTLITERAL  */
#line 3634 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10329 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: REALLITERAL  */
#line 3635 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10335 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: IMAGLITERAL  */
#line 3636 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10341 "bison-chpl-lib.cpp"
    break;

  case 687: /* literal_expr: TNONE  */
#line 3637 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10347 "bison-chpl-lib.cpp"
    break;

  case 688: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3639 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10356 "bison-chpl-lib.cpp"
    break;

  case 689: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3644 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10365 "bison-chpl-lib.cpp"
    break;

  case 690: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3649 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10374 "bison-chpl-lib.cpp"
    break;

  case 691: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3654 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10383 "bison-chpl-lib.cpp"
    break;

  case 692: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3659 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10392 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3664 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10401 "bison-chpl-lib.cpp"
    break;

  case 694: /* assoc_expr_ls: expr TALIAS expr  */
#line 3673 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10410 "bison-chpl-lib.cpp"
    break;

  case 695: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3678 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10420 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TPLUS expr  */
#line 3686 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10426 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TMINUS expr  */
#line 3687 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10432 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TSTAR expr  */
#line 3688 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10438 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TDIVIDE expr  */
#line 3689 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10444 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3690 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10450 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3691 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10456 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TMOD expr  */
#line 3692 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10462 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TEQUAL expr  */
#line 3693 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10468 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3694 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10474 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3695 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10480 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3696 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10486 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TLESS expr  */
#line 3697 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10492 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TGREATER expr  */
#line 3698 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10498 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TBAND expr  */
#line 3699 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10504 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TBOR expr  */
#line 3700 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10510 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TBXOR expr  */
#line 3701 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10516 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TAND expr  */
#line 3702 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10522 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TOR expr  */
#line 3703 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10528 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TEXP expr  */
#line 3704 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10534 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TBY expr  */
#line 3705 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10540 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TALIGN expr  */
#line 3706 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10546 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr THASH expr  */
#line 3707 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10552 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TDMAPPED expr  */
#line 3708 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10558 "bison-chpl-lib.cpp"
    break;

  case 719: /* unary_op_expr: TPLUS expr  */
#line 3712 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10564 "bison-chpl-lib.cpp"
    break;

  case 720: /* unary_op_expr: TMINUS expr  */
#line 3713 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10570 "bison-chpl-lib.cpp"
    break;

  case 721: /* unary_op_expr: TMINUSMINUS expr  */
#line 3714 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10576 "bison-chpl-lib.cpp"
    break;

  case 722: /* unary_op_expr: TPLUSPLUS expr  */
#line 3715 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10582 "bison-chpl-lib.cpp"
    break;

  case 723: /* unary_op_expr: TBANG expr  */
#line 3716 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10588 "bison-chpl-lib.cpp"
    break;

  case 724: /* unary_op_expr: expr TBANG  */
#line 3717 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10596 "bison-chpl-lib.cpp"
    break;

  case 725: /* unary_op_expr: TBNOT expr  */
#line 3720 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10602 "bison-chpl-lib.cpp"
    break;

  case 726: /* reduce_expr: expr TREDUCE expr  */
#line 3725 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10610 "bison-chpl-lib.cpp"
    break;

  case 727: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3729 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10618 "bison-chpl-lib.cpp"
    break;

  case 728: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3733 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10626 "bison-chpl-lib.cpp"
    break;

  case 729: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3737 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10634 "bison-chpl-lib.cpp"
    break;

  case 730: /* scan_expr: expr TSCAN expr  */
#line 3744 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10642 "bison-chpl-lib.cpp"
    break;

  case 731: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3748 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10650 "bison-chpl-lib.cpp"
    break;

  case 732: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3752 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10658 "bison-chpl-lib.cpp"
    break;

  case 733: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3756 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10666 "bison-chpl-lib.cpp"
    break;


#line 10670 "bison-chpl-lib.cpp"

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
