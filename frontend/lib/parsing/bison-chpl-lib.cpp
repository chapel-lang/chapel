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
#define YYLAST   17984

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  180
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  209
/* YYNRULES -- Number of rules.  */
#define YYNRULES  751
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1276

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
    2642,  2643,  2644,  2645,  2650,  2651,  2655,  2656,  2660,  2661,
    2665,  2666,  2670,  2671,  2675,  2676,  2680,  2684,  2688,  2693,
    2698,  2703,  2708,  2716,  2720,  2728,  2729,  2730,  2731,  2732,
    2733,  2734,  2735,  2736,  2737,  2741,  2742,  2743,  2744,  2745,
    2746,  2747,  2748,  2749,  2750,  2751,  2755,  2756,  2757,  2761,
    2762,  2763,  2764,  2765,  2766,  2767,  2768,  2769,  2770,  2771,
    2775,  2776,  2779,  2780,  2784,  2785,  2789,  2794,  2795,  2797,
    2799,  2801,  2806,  2808,  2813,  2815,  2817,  2819,  2821,  2823,
    2825,  2830,  2831,  2832,  2836,  2845,  2851,  2861,  2865,  2872,
    2894,  2895,  2900,  2901,  2902,  2903,  2904,  2908,  2917,  2916,
    2926,  2925,  2935,  2940,  2949,  2953,  2960,  2979,  2998,  3002,
    3009,  3011,  3013,  3020,  3021,  3022,  3026,  3030,  3031,  3032,
    3036,  3037,  3041,  3042,  3046,  3047,  3048,  3049,  3053,  3054,
    3058,  3062,  3063,  3069,  3070,  3074,  3079,  3084,  3092,  3095,
    3101,  3102,  3108,  3112,  3116,  3123,  3124,  3128,  3133,  3142,
    3143,  3147,  3148,  3155,  3156,  3157,  3158,  3159,  3160,  3162,
    3164,  3166,  3172,  3174,  3177,  3179,  3181,  3183,  3185,  3187,
    3189,  3191,  3193,  3195,  3200,  3204,  3208,  3212,  3216,  3220,
    3229,  3238,  3250,  3254,  3258,  3262,  3266,  3270,  3274,  3278,
    3285,  3303,  3311,  3312,  3313,  3314,  3315,  3316,  3317,  3321,
    3322,  3326,  3330,  3337,  3344,  3354,  3355,  3359,  3363,  3367,
    3374,  3375,  3376,  3377,  3378,  3379,  3380,  3381,  3382,  3383,
    3387,  3389,  3391,  3393,  3395,  3401,  3408,  3421,  3434,  3448,
    3466,  3473,  3478,  3483,  3488,  3494,  3500,  3530,  3537,  3544,
    3545,  3549,  3551,  3552,  3554,  3556,  3557,  3558,  3559,  3562,
    3563,  3564,  3565,  3566,  3567,  3568,  3569,  3570,  3574,  3575,
    3579,  3580,  3581,  3585,  3586,  3587,  3588,  3597,  3598,  3601,
    3602,  3603,  3607,  3619,  3631,  3638,  3640,  3642,  3644,  3646,
    3648,  3657,  3663,  3667,  3671,  3678,  3679,  3683,  3684,  3688,
    3689,  3690,  3691,  3692,  3693,  3694,  3699,  3704,  3709,  3714,
    3719,  3727,  3737,  3747,  3752,  3761,  3766,  3775,  3776,  3777,
    3778,  3779,  3780,  3781,  3782,  3783,  3784,  3785,  3786,  3787,
    3788,  3789,  3790,  3791,  3792,  3793,  3794,  3795,  3796,  3797,
    3801,  3802,  3803,  3804,  3805,  3806,  3809,  3813,  3817,  3821,
    3825,  3832,  3836,  3840,  3844,  3852,  3853,  3854,  3855,  3856,
    3857,  3858
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

#define YYPACT_NINF (-928)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-752)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -928,    99,  3810,  -928,   -84,    88,  -928,  -928,  -928,  -928,
    -928,  -928,  -928, 11910,    78,    60,   118, 13725,   222, 17751,
      78,   271,   154,    66,    60,  5202, 11910,   161, 17810,  -928,
     333,   325,  -928,  9843,   330,   281,   267,  -928,   403,   376,
   17810, 17810, 17810,  -928,  1900, 11049,   438, 11910, 11910,   243,
    -928,   447,   452, 11910,  -928, 13725,  -928, 11910,   512,   372,
      15,   347, 13082,   473, 17869,  -928, 11910,  7959, 11910, 11049,
   13725,   448,   498,   389,  5202,   509, 11910,   531,  6933,  6933,
    -928,   544,  -928, 13725,  -928,   547, 10014,  -928, 11910,  -928,
   11910,  -928,  -928, 13554, 11910,  -928, 10185,  -928,  -928,  -928,
    4158,  8130, 11910,  -928,  4854,  -928,  -928,  -928,  -928, 17472,
     563,  -928,   460,   445,  -928,    45,  -928,  -928,  -928,  -928,
    -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,  7275,
    -928,  7446,  -928,  -928,    28,  -928,  -928,   475,  -928,   566,
    -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,   474, 17810,
    -928,   468,   483,   359,  -928,   305,  -928,  -928,  -928,   771,
    1096,  -928,  -928, 17810,  -928,  1743,  -928,   481,   495,  -928,
    -928,  -928,   499,   502, 11910,   503,   510,  -928,  -928,  -928,
   17251,  2178,   479,   521,   525,  -928,  -928,   480,  -928,  -928,
    -928,  -928,  -928,   156,  -928,  -928,  -928, 11910,  -928, 17810,
    -928, 11910, 11910,    95,   606,   484,  -928,  -928,  -928,  -928,
   17251,   488,  -928,  -928,    46,  5202,  -928,  -928,  -928,   529,
      11,   522,  -928,   214,  -928,   530,  -928,   111, 17251,  8301,
    -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,
    -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,
    -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928, 17810, 17810,
     -13, 14317,  -928,  -928,   588,  8301,   532,   533,  -928,  5202,
    3613,   146,    38,  -928,  5202,  -928,  -928, 15300,  -928,    25,
   15709,   172,  -928,   536,   537,  -928, 15300,    11,   172,  -928,
    8301,  1798,  1798,    48,    36,  -928,    69,  -928,  -928,  -928,
    -928,  7617,  -928,  -928,   504,  -928,   528,   535,  -928,  -928,
   13829,   552,  -928,  -928, 17251,   205, 11910, 11910,  -928,   -21,
    -928,  -928, 17251,   534, 15828,  -928, 15300,    11,   538,  8301,
    -928, 17251, 15868, 11910,  -928,  -928,  -928,  -928,  -928,    11,
     539,   421,   421,   948,   172,   172,   239,  -928,  -928,  4332,
      89, 11910,   556,    97,   540,   948,   704,  -928,  -928, 17810,
    -928, 11910,  -928,  4158,  -928,    80,  -928,   588,  -928,  -928,
     712,   546,  4506, 11910,  -928, 11910,   652,  -928,  -928, 14971,
      42,   474, 17251,   500,  -928,  5202,   640,  -928,  -928,  9843,
   10356, 11220,  -928,  -928,  -928, 17810,  -928, 17810, 11910,   553,
    -928, 16829,   262,   771,  -928,  -928,  -928,  -928,   189,  -928,
    -928,  -928,  -928,  -928,  -928,  -928,    55,   192,  -928,  -928,
    -928,  -928,  -928,  -928,  -928, 13388,   597,   241,  -928,   560,
     304,   507,  -928,   578, 11910, 11910, 11910, 11220, 11220, 11910,
     513, 11910, 11910, 11910, 11910, 11910,   303, 13554, 11910, 11910,
   11910, 11910, 11910, 11910, 11910, 11910, 11910, 11910, 11910, 11910,
   11910, 11910, 11910, 11910,   659,  -928,  -928,  -928,  -928,  -928,
   10014, 10014,  -928,  -928,  -928,  -928, 10014,  -928,  -928, 10014,
   10014,  8301,  8472, 11220, 11220, 15908,   561,  -928, 11391, 11220,
   17810,  -928,  6243,  -928,  -928,  -928,  1900,  -928, 11910,  -928,
     607,   567,   587,  -928,  -928,   611,   616,  5202,   709,  5202,
    -928,   710, 11910,   681,   579,  -928, 10014,  -928,  3613,  -928,
    -928,   153,  -928, 12081,   632, 11910,  1900,  -928,  -928, 11910,
    -928, 17601, 11910, 11910,  -928,   214,   590,  -928,  -928,  -928,
    -928, 17810,  -928,   771, 13253,  3613,   614, 12081,  -928, 17251,
   17251,  -928,   522,  -928,    52,  -928,  8301,   593,  -928, 15300,
     739,   739,  -928,  -928,  -928,  -928, 11565,  -928, 15987,  8646,
    8817,  -928,  -928,  -928, 17251, 11220, 11220,  -928,   460,  8988,
    -928,   346,  -928,  4680,  -928,   307, 16027,   308, 15061, 17810,
    6759,  6588,  -928,   474,   599,  -928,  -928,   214,  -928,   661,
   17810,    54, 15300,   609,  3169,    -9,   130, 11910,   335, 16068,
     -31,    26, 14806,  -928,   184,  -928,   287,   631,  1939,   612,
   17692,   660,   101,  -928,  -928,   188,  -928,  -928,    65,  -928,
    -928,  -928,  6417,  -928,   680,  -928,  -928,   615,   639,  -928,
     643,  -928,   644,   646,   647,   648,   651,  -928,   653,   654,
     656,   657,   658,   663,   665,  -928,  -928,  -928,  -928,  -928,
    -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,
    -928,  -928,  -928,  -928,  -928,  -928,  -928, 11910,   667,   666,
     615,  -928,   615,  -928, 11910,  -928, 17810,  -928,  -928,  -928,
   17449, 17449,   459,  -928,   459,  -928,   459, 17290,   865,  2070,
     743,    11,   421,  -928,   517,  -928,  -928,  -928,  -928,  -928,
     948, 14910,   459,   605,   605, 17449,   605,   605,  1337,   421,
   14910,  1547,  1337,   172,   172,   421,   948,   645,   655,   664,
     670,   671,   672,   636,   641,  -928,   459,  -928,   459, 11910,
   10014,    91,   785, 15380,   649,   676,   217,  -928,  -928,  -928,
    -928,  -928,  -928,  -928,  -928,   203,  -928,   758, 17330,   384,
     316, 17251, 10014,  -928, 10014, 11910, 11910,   789,  5202,   791,
   16219,  5202, 15459, 17810,  -928,   216,   227,  3613,   146,  -928,
   17251,  9159,  -928, 17251,  -928,  -928,  -928, 17810, 16259, 16299,
    1900,  -928,   614,   682,  -928,   228, 12081,  -928,   260,  -928,
   11910,  -928,   668,   -20,   677,  -928,  2773,  -928,  -928,  -928,
    -928,  -928,  -928,  -928,  -928,   275,    33, 14880,  -928,  -928,
     252,  -928,  -928,  -928,  -928, 11910,  -928,    59, 13928, 11910,
    -928, 11910,  6759,  6588,  -928,  9330,   496,  9501,   516,   526,
    9672,  7788,   527,   329,  -928,  -928,  -928, 16378,   685,   687,
     675,  -928, 17810,  1900, 11910,   808,  -928,  -928, 11910, 17251,
    5202,   686, 11220, 10530,  -928,  3984,   373,   693,   713,  -928,
     385, 11910, 11736,  -928,  5202,  -928,    54,  5376, 11220,  -928,
      54, 17810,   522,  -928, 10014,   699,  2009,  -928,  -928, 11910,
      68,  -928,   596,  -928,   680,  -928,  -928,  -928,  -928,  -928,
      32,   551,  -928, 16418,  -928, 14086,  -928,  -928, 17251,  -928,
     684,   697,  -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,
     258, 11220,  3169, 11910, 12252, 11220, 10014,  -928,  -928,  -928,
    7104,  -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,
    -928,  -928,  -928,    43, 11220, 11220,  -928,  -928,  -928, 17251,
   17251,  5202,  -928,  5202, 11910,  -928,  5202,   840,   714,   717,
    -928,  -928,   632,  -928,  -928,  -928,   705,   706,   336,   260,
   11910,   680,   614,  -928, 10704,  -928,  -928, 17251,  -928,  5202,
   11910,  -928,  -928,  -928, 17810,  -928,   741,   522, 11220,  5202,
   11220, 10014,  -928,  -928,  -928,  -928,  -928,   517,  -928,  -928,
    -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,
    -928,  6759,  -928,  -928, 16466, 15550,   337,   728,  6759,  -928,
   11910,  -928,  -928, 14242,   356,   358, 16569, 11910,   265,  -928,
    5202,    86, 15624,  -928,   388,  -928, 17251,  -928,  -928,  5202,
   10875,  -928, 17251,  -928, 16617, 17251,  -928,  9843, 17251,    35,
   15140,  -928,  -928,  -928,   293,  -928,  -928,  -928,  -928,  2655,
    3402,  -928, 17251, 17810,   563,  -928,    32,   292, 11910,  -928,
     747,   725,  -928,   615,   615,  -928,  -928,  -928,   724, 16657,
   11910,   867, 16808,   294,  -928,   632,   295,   301,  -928,  -928,
   17251,   866,  5202,  -928,  -928,  -928,  8301,  8301,  -928,  -928,
    -928,  -928,  -928,   260, 17251,  -928,   100,    27,   729, 11910,
    -928,   123, 15669,    33, 15220,  -928,   180, 11910,  6759,  6588,
    -928,  -928,  -928, 16848, 17251, 14289,  -928,  -928,  -928,  -928,
   15300,  -928,  -928,  5550,   730,  5724,   731,  -928,  -928, 11910,
   14397,  5898,  -928,    54,  6072,  -928,    54,   748,  3402,  -928,
    -928,  -928, 17692,  -928,  -928,  -928,   242,  -928,   -23,  -928,
     440, 16889,    17,  -928,  -928,  -928, 12423, 12594, 17004, 12765,
   12936,  -928,  -928,  5202,  -928,   735,   742,  -928,  -928,  5202,
    -928,   522, 17251,  5202,  5202,  -928, 17251,   360,   752, 11910,
    -928,  -928,  9843,  -928, 17251,  5202,  9843,  -928, 17251,  5202,
   17251,  5202,  9843, 17251,  -928,  9843, 17251,  -928,  -928,  -928,
    -928,    56,  -928, 11910,   292,   242,   242,   242,   242,   242,
     242,   292,  5028,  -928,  -928,  -928,  -928, 11910, 11910, 11910,
   11910, 11910,  -928,   755,   756,   661,  -928,  -928,  -928, 17251,
   14477,  -928, 14557,  -928, 16219, 14637, 14717,  -928, 17251,  -928,
    -928,  -928,  -928,  -928,  -928,  -928,   774,  -928, 17052, 17092,
   17171, 17211,  -928,  -928,  5202,  5202,  5202,  5202, 11910, 11910,
   11910, 11910, 16219, 16219, 16219, 16219
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   139,   466,    55,   691,   692,
     693,   687,   688,   209,   599,   128,   160,   570,   167,   572,
     599,   166,   498,   495,   128,     0,   209,   274,   161,   198,
     197,   685,   196,   209,   165,    68,   275,   337,   162,   140,
       0,     0,     0,   332,     0,   209,   169,   209,   209,   620,
     591,   694,   170,   209,   338,   564,   492,   209,     0,     0,
     183,   181,   423,   164,   573,   494,   209,   209,   209,   209,
     568,     0,   168,     0,     0,   141,   209,   686,   209,   209,
     485,   163,   312,   566,   496,   171,   209,   747,   209,   749,
     209,   750,   751,   636,   209,   748,   209,   644,   179,   746,
       0,   209,   209,     4,     0,     5,    10,    11,    12,    48,
      51,    52,    56,     0,    45,    71,    13,    77,    14,    15,
      16,    17,    29,   551,   552,    22,    46,   180,   190,   209,
     199,   658,   191,    18,     0,    31,    30,     0,    47,     0,
     269,    19,   654,    21,    35,    32,    33,   189,   313,     0,
     187,     0,     0,   651,   336,     0,   648,   185,   353,   449,
     435,   646,   188,     0,   497,     0,   186,   663,   642,   641,
     645,   555,   553,     0,   209,   652,   653,   657,   656,   655,
       0,   554,     0,   664,   665,   666,   689,   690,   647,   557,
     556,   649,   650,     0,    28,   572,   161,   209,   140,     0,
     573,   209,   209,     0,     0,   651,   663,   553,   652,   653,
     562,   554,   664,   665,     0,     0,   600,   129,   130,     0,
     571,     0,   486,     0,   493,     0,    20,     0,   533,   209,
     136,   142,   153,   148,   147,   155,   133,   146,   156,   143,
     137,   157,   131,   158,   151,   145,   152,   149,   150,   132,
     134,   144,   154,   159,   138,   322,   135,   208,     0,     0,
       0,     0,    70,    69,    71,   209,     0,     0,   251,     0,
       0,     0,     0,   504,     0,   176,    40,     0,   304,     0,
     303,   732,   624,   621,   622,   623,   500,   565,   733,     7,
     209,   335,   335,   430,   173,   425,   174,   426,   427,   431,
     432,   172,   433,   434,   423,   529,     0,   340,   341,   343,
       0,   424,   528,   345,   516,     0,   209,   209,   177,   648,
     639,   662,   640,     0,     0,    43,     0,   569,     0,   209,
      44,   563,     0,   209,   282,    48,   286,   283,   286,   567,
       0,   734,   736,   634,   731,   730,     0,    74,    78,     0,
       0,   209,     0,     0,   542,   635,     0,     6,   310,     0,
     197,   209,   311,     0,    49,     0,     9,    71,    50,    53,
       0,    60,     0,   209,    72,   209,     0,   498,   195,     0,
     687,   313,   659,   204,   215,     0,   212,   213,   214,   209,
     209,   209,   211,   574,   582,     0,   253,     0,   209,     0,
     309,     0,   423,   449,   447,   448,   446,   361,   451,   456,
     459,   450,   454,   453,   455,   458,     0,   439,   441,   445,
     444,   436,   437,   440,   443,     0,   490,     0,   487,     0,
       0,   643,    34,   625,   209,   209,   209,   209,   209,   209,
     735,   209,   209,   209,   209,   209,     0,   633,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   392,   399,   400,   401,   396,   398,
     209,   209,   394,   397,   395,   393,   209,   403,   402,   209,
     209,   209,   209,   209,   209,     0,     0,   212,   209,   209,
       0,   604,     0,    36,    24,    37,     0,    25,   209,    38,
     551,     0,   546,   547,   550,     0,     0,     0,   242,     0,
     392,   238,   209,     0,     0,    23,   209,    26,     0,   508,
     510,     0,   527,   209,   513,   209,     0,   175,    39,   209,
     306,     0,   209,   209,    41,     0,     0,   184,   182,   428,
     429,     0,   424,   449,   423,     0,   531,   209,   121,   660,
     661,   351,     0,   178,     0,    42,   209,     0,   293,     0,
     284,   285,    27,    76,    75,    79,   209,   695,     0,   209,
     209,   683,   681,     8,   500,   209,   209,   210,    56,   209,
      54,     0,    65,     0,   119,     0,   114,     0,    86,     0,
     209,   209,   192,   313,     0,   200,   206,     0,   203,     0,
       0,     0,     0,     0,   583,     0,     0,   209,     0,   533,
       0,     0,     0,   256,     0,   254,   280,   314,     0,     0,
       0,   330,     0,   323,   410,     0,   412,   416,     0,   457,
     452,   521,     0,   523,   460,   442,   438,   408,   136,   388,
     142,   386,   148,   147,   146,   143,   137,   390,   157,   158,
     145,   149,   132,   144,   159,   385,   367,   370,   368,   369,
     391,   380,   371,   384,   376,   374,   387,   375,   373,   378,
     383,   372,   377,   381,   382,   379,   389,   209,   364,     0,
     408,   365,   408,   362,   209,   489,     0,   484,   503,   502,
     727,   726,   729,   738,   737,   742,   741,   723,   720,   721,
     722,   637,   710,   139,     0,   677,   678,   141,   676,   675,
     631,   714,   725,   719,   717,   728,   718,   716,   708,   713,
     715,   724,   707,   711,   712,   709,   632,     0,     0,     0,
       0,     0,     0,     0,     0,   740,   739,   744,   743,   209,
     209,     0,     0,     0,   280,     0,   610,   611,   617,   616,
     618,   613,   619,   615,   602,     0,   605,     0,     0,     0,
       0,   534,   209,   559,   209,   209,   209,   241,     0,   237,
       0,     0,     0,     0,   558,     0,     0,   511,     0,   526,
     525,   209,   506,   630,   505,   305,   302,     0,     0,     0,
       0,   674,   531,   346,   342,     0,   209,   532,   513,   344,
     209,   350,     0,     0,     0,   560,     0,   287,   696,   638,
     682,   543,   542,   684,   544,     0,     0,     0,    57,    58,
       0,    61,    63,    67,    66,   209,    97,     0,     0,   209,
      92,   209,   209,   209,    64,   209,   367,   209,   368,   369,
     209,   209,   379,     0,   404,   405,    81,    80,    91,     0,
       0,   315,     0,     0,   209,     0,   218,   217,   209,   584,
       0,     0,   209,   209,   540,     0,   535,     0,   538,   699,
     537,   209,   209,   703,     0,   226,     0,     0,   209,   224,
       0,     0,     0,   277,   209,     0,   335,   321,   327,   209,
     325,   320,   423,   411,   460,   519,   518,   517,   520,   461,
     467,   423,   360,     0,   366,     0,   356,   357,   491,   488,
       0,     0,   127,   125,   126,   124,   123,   122,   672,   673,
       0,   209,   697,   209,   209,   209,   209,   603,   612,   614,
     209,   601,   160,   167,   166,   165,   162,   169,   170,   164,
     168,   163,   171,     0,   209,   209,   499,   549,   548,   250,
     249,     0,   244,     0,   209,   240,     0,   246,     0,   279,
     509,   512,   513,   514,   515,   301,     0,     0,     0,   513,
     209,   460,   531,   530,   209,   419,   417,   352,   295,     0,
     209,   294,   297,   561,     0,   288,   291,     0,   209,     0,
     209,   209,    59,   120,   117,    98,   109,   104,   103,   102,
     111,    99,   112,   107,   101,   108,   105,   106,   100,   110,
     113,   209,   116,   115,    88,    87,     0,     0,   209,   193,
     209,   194,   315,   335,     0,     0,     0,   209,     0,   234,
       0,     0,     0,   698,   536,   232,   585,   541,   701,     0,
     209,   700,   705,   704,     0,   578,   227,   209,   577,     0,
       0,   225,   255,   252,     0,   308,   183,   181,   307,   335,
     335,   316,   331,     0,     0,   413,   467,     0,   209,   354,
     415,     0,   363,   408,   408,   679,   680,   279,     0,     0,
     209,     0,     0,     0,   606,   513,   663,   663,   243,   239,
     590,   245,     0,    73,   276,   507,   209,   209,   501,   420,
     418,   347,   348,   513,   407,   299,     0,     0,     0,   209,
     289,     0,     0,     0,     0,    62,     0,   209,   209,   209,
      93,    95,    84,    83,    82,   335,   201,   207,   205,   216,
       0,   236,   235,     0,     0,     0,     0,   702,   233,   209,
       0,     0,   222,     0,     0,   220,     0,   281,   335,   318,
     317,   326,     0,   333,   481,   482,     0,   483,   469,   472,
       0,   468,     0,   409,   358,   359,   209,   209,     0,   209,
     209,   281,   607,     0,   248,     0,     0,   422,   421,     0,
     298,     0,   292,     0,     0,   118,    89,     0,     0,   209,
     202,   219,   209,   230,   587,     0,   209,   228,   586,     0,
     706,     0,   209,   576,   223,   209,   575,   221,   278,   319,
     329,     0,   480,   209,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   462,   464,   355,   463,   209,   209,   209,
     209,   209,   247,   626,   627,   300,   290,    94,    96,    85,
       0,   231,     0,   229,   581,     0,     0,   334,   471,   473,
     474,   477,   478,   479,   475,   476,   470,   465,     0,     0,
       0,     0,   628,   629,     0,     0,     0,     0,   209,   209,
     209,   209,   589,   588,   580,   579
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -928,  -928,  -928,    -1,   -60,  1301,  -928,   339,   812,  -928,
    -928,   497,  -388,  -107,  -928,   344,  -928,  -928,   -68,  -928,
    -928,    76,   662,  -928,  -547,  2447,   555,  -584,  -928,  -801,
    -928,  -928,  -928,   103,  -928,  -928,  -928,   900,  -928,   138,
     -39,  -928,  -928,  -449,    37,  -928,  -848,  -928,  -928,   342,
    -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,     5,
    -928,  -157,   564,  -320,  -928,   541,  -928,  -928,  -928,    50,
    1530,  -928,  -928,  -928,  -928,   594,  -928,   -50,  -928,  -928,
    -928,  -928,   404,  -928,  -928,  -928,   -98,  -928,  -341,  -806,
    -928,  -928,  -928,  -928,  -928,  -608,  -928,    29,  -928,  -928,
    -928,  -928,  -928,   391,  -928,  1651,  -928,  -928,  -928,  -928,
     554,  -928,  -928,  -928,  -928,    31,  -410,  -171,  -785,  -927,
    -658,  -928,    40,  -928,    47,   -53,   634,   -48,  -928,  -928,
    -352,  -816,  -928,  -266,  -928,  -120,  -274,  -264,  -730,  -928,
    -928,    96,   274,  -928,   426,  -206,   835,  -928,  -928,  -161,
     439,   190,  -384,  -770,  -605,  -928,  -928,  -928,  -591,  -508,
    -928,  -751,   -18,  -928,  -928,  -579,    71,  -928,  -254,  -482,
    -558,    -2,  -928,  -928,  -928,  -928,  -928,  -928,   207,  -928,
     946,  -586,  -928,    41,  -928,  -928,   357,   467,  -928,  -928,
    -928,  -928,  2538,  -928,   -51,  1004,  -928,  1201,  1379,  -928,
    -928,  -928,  -928,  -928,  -928,  -928,  -928,  -928,  -474
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   348,   104,   105,   106,   107,   108,   365,
     366,   109,   110,   111,   112,   371,   580,   820,   821,   113,
     114,   115,   376,   116,   117,   118,   349,   848,   587,   849,
     119,   120,  1012,   584,   585,   121,   122,   219,   254,   271,
     123,   256,   124,   305,   534,   125,   126,   292,   291,   127,
     128,   129,   130,   131,   596,   132,   597,   852,   133,   203,
     135,   391,   392,   605,   136,   260,   137,   138,   614,   615,
     204,   140,   141,   142,   143,   560,   807,   987,   144,   145,
     803,   982,   278,   279,   146,   147,   148,   149,   399,   886,
     150,   151,   622,  1063,  1064,   623,   152,   205,  1211,   154,
     155,   306,   307,   308,   971,   156,   320,   552,   800,   157,
     158,  1162,   159,   160,   679,   680,   844,   845,   846,   975,
     902,   403,   625,  1071,   626,   627,   310,   542,   425,   407,
     416,   900,  1225,  1226,   161,  1069,  1158,  1159,  1160,   162,
     163,   427,   428,   685,   164,   165,   166,   223,   535,   272,
     273,   520,   521,   782,   312,   898,   633,   634,   524,   313,
     797,   798,   350,   607,   608,   866,   352,   353,   501,   502,
     503,   206,   168,   169,   393,   170,   394,   171,   207,   173,
     215,   216,   755,   756,   757,   174,   208,   209,   177,   178,
     179,   321,   180,   383,   504,   211,   182,   212,   213,   185,
     186,   187,   188,   610,   189,   190,   191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   103,   304,   369,   430,   801,   843,   134,   227,   309,
     480,   514,   888,   620,   311,   681,   323,   496,   759,   861,
     867,   822,   906,   167,   907,   876,   880,   897,   976,   870,
     134,   153,  1017,   381,   775,   340,   536,   894,  1061,   799,
     594,   969,  1099,  1013,   522,  1023,   489,   491,   315,  1222,
     354,   628,   384,   802,   153,   979,   631,   274,   874,   274,
     385,   -70,   703,   217,   358,   274,   522,  1141,   386,   387,
     388,   611,   167,  -328,   779,   557,   167,   167,  1066,   134,
     194,   -70,   276,   134,   134,   525,   274,  -522,  1222,   507,
     512,   373,  -274,  1067,  1213,   539,   980,  -524,   167,     3,
     -70,   551,   167,   153,   384,   134,   325,   153,   153,   134,
     214,   264,   385,   198,   218,   872,  -522,   693,   695,   384,
     386,   387,   388,  1214,   384,  -275,  -524,  -296,   -70,   153,
     374,   362,  1179,   153,   776,   487,   387,   388,   921,   540,
     386,   387,   388,   873,  -257,   214,   389,   522,  -349,  1068,
     981,   440,   214,    82,   214,  1102,   363,   224,   375,   446,
    -522,   795,   707,   735,   737,   860,   255,  -524,  -296,   742,
    -524,   529,  -522,   498,  -258,  1061,  1177,   862,   267,   268,
     269,  1223,  -524,   896,   526,   367,   363,   962,   523,  -524,
     500,   793,  1095,  -524,   363,   363,   363,   214,   389,  1100,
     632,   390,   363,   436,   363,   214,   948,   229,   576,   172,
     523,  1149,  1150,   167,  -324,  -524,  1125,   492,   389,  -522,
     134,  1103,  -296,   363,  -522,   363,   500,   733,   734,   867,
     876,   880,   172,  1122,  -524,   566,   629,   863,  -324,   635,
     265,    23,   214,   570,   153,  1154,   483,   890,  1016,   214,
     484,   500,   850,   575,   682,   864,   816,   498,   920,   567,
    1133,   282,   437,   222,   928,   922,   438,   167,   488,   571,
    -524,   891,   167,   390,   134,   499,   863,  1061,  -264,   134,
     630,   172,   593,   636,  1034,   172,   172,   400,   973,   293,
     500,   523,  -524,    56,   864,  1154,  1155,  1183,   153,   777,
    1209,   426,   804,   153,   865,    65,   703,   172,   929,   295,
    -524,   172,   440,   297,   528,  1172,   530,   444,  1188,   445,
     446,   283,   988,   704,   449,   778,  1018,  -263,    84,   790,
     881,   456,   229,  1178,   892,   760,   705,   486,   298,   462,
     284,   299,   304,    58,   257,  1157,  1155,   167,   565,   930,
    1185,   498,  1085,   300,   134,   285,   882,   198,   404,   175,
     893,   167,   764,   555,   262,  1101,   706,   263,   134,   548,
     167,   302,   606,   777,   777,   931,   303,   134,   153,   405,
     617,  -272,   175,   167,   781,  1156,  -262,   686,   959,   406,
     134,   853,   153,   -69,   214,  1157,   505,   506,   991,   960,
     972,   153,  1054,   194,   764,   687,   707,   709,   519,   563,
     974,   498,   708,   -69,   153,  1164,  1165,   334,   337,   728,
     729,   498,   172,   683,   992,   730,  1212,  1116,   731,   732,
    1077,   175,   -69,   822,   624,   175,   175,  1131,   265,   764,
     764,  -609,   500,   500,  1083,  1134,  1136,  -608,   546,   989,
     526,   883,   436,   825,   829,  1151,   759,   175,   884,  -259,
     -69,   175,   526,  1143,  1146,  1147,  1171,  -609,   689,   176,
     741,   826,   830,  -608,   945,  1018,   172,   500,   484,   266,
     946,   172,   526,  1018,   304,  1250,  1251,  1252,  1253,  1254,
    1255,   309,   176,  1019,  -266,   681,   311,   255,  1060,   864,
    1098,  1120,   686,  -270,   526,   167,  1018,   167,  -267,   869,
     194,   437,   134,   369,   134,   438,   823,   500,   812,   812,
    1127,   289,  1128,  -596,  1237,  1134,  1136,  1143,  1146,  -261,
    -670,   293,  -670,   613,  1187,   616,   153,  1037,   153,   621,
     500,   176,  1031,   290,  1210,   176,   176,  1038,   328,  1037,
      37,   295,  1037,  -335,  -265,   297,   172,   815,  1049,  1041,
     329,   440,  1137,   678,  1215,  -271,   444,   176,     7,   446,
     172,   176,   175,   449,  -335,   335,   335,    54,   293,   172,
     298,   167,   565,   299,  -335,   356,  -749,  -273,   134,   868,
    -749,  1216,   172,  1217,  1218,   300,  1219,  1220,   295,   440,
    -260,  1078,   297,  -268,   444,  1081,  -750,   446,   370,   595,
    -750,   449,   153,   302,   372,  1224,  -751,  -746,   303,   398,
    -751,  -746,   397,   293,   693,   735,   175,   298,   744,   968,
     299,   175,    58,   537,   538,  1060,   436,   401,   856,   857,
     811,   814,   300,   295,   363,  -593,   395,   297,   875,   879,
     481,  -671,   482,  -671,   402,  -670,   519,  -670,   431,  -667,
     302,  -667,   490,  -592,  1224,   303,   432,  -597,  1111,   786,
    1113,  1148,   298,   374,  -598,   299,  1152,    58,  -669,   792,
    -669,   544,   176,   519,  -668,  -595,  -668,   300,   910,  -594,
     911,   363,  1025,   494,   497,   437,   515,   547,   553,   438,
     543,   500,   569,   562,   516,   302,   175,   532,   533,   556,
     303,   947,   572,   573,   172,   578,   172,   579,   589,   600,
     175,   684,   618,  -414,   688,   500,   446,   834,   727,   175,
     964,   762,   740,   764,  1074,   765,   176,  1060,   855,   763,
     766,   176,   175,   768,   771,   440,   441,   773,   442,   443,
     444,   774,   445,   446,   447,   943,   781,   449,   621,   796,
     806,   703,   791,   455,   456,   805,   167,   459,   851,   167,
     460,   461,   462,   134,   436,   932,   134,   498,   933,   854,
     858,   463,   887,   934,   889,   899,   901,  -139,   994,   709,
     172,  -160,  -167,   196,  -166,  -165,  -162,   153,   408,  -140,
     153,  -169,  -170,   935,  -164,  -168,  -141,   904,   918,   912,
     936,  -163,   198,  -171,   905,   919,   176,   923,   409,   913,
     926,   937,   410,   951,   426,   953,  1059,   970,   914,   938,
     176,  1018,   304,   437,   915,   916,   917,   438,   978,   176,
    1028,   304,  1175,  1176,  1022,   500,   939,   411,   927,   983,
     412,  1021,   176,   875,   879,  1027,  1075,   940,   167,  1040,
    1030,   707,   413,   167,   175,   134,   175,  1039,   941,  1055,
     134,  1076,   167,   942,  1092,   167,  1096,  1097,  1093,   134,
     414,  1094,   134,   440,   441,   415,  1109,   500,   444,   153,
     445,   446,  1121,   892,   153,   449,   436,  1163,  1166,  1169,
    1173,  1181,   456,   153,  1195,  1199,   153,  1233,   460,   461,
     462,   958,  1208,  1046,  1234,   519,  1238,  1051,  1262,  1263,
    1214,   364,   818,  1115,   225,   965,   513,   583,   993,   577,
     601,  1052,   561,   785,  1108,   794,  1073,   598,   541,  1065,
     175,  1070,  1086,  1087,   986,  1247,  1153,  1256,  1024,   167,
    1249,   167,   500,   369,   167,   437,   134,   787,   134,   438,
     909,   134,  1107,  1059,   378,   784,   221,   961,     0,   512,
       0,  1084,     0,     0,   176,   172,   176,   167,   172,   436,
     153,     0,   153,     0,  1106,   153,     0,   167,     0,     0,
     426,     0,     0,     0,   134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   440,   181,     0,   153,     0,
     444,     0,   445,   446,     0,     0,     0,   449,   153,   613,
       0,     0,     0,     0,   456,     0,     0,     0,   167,   181,
     460,   461,   462,     0,     0,   134,     0,   167,   437,     0,
       0,   369,   438,   678,   134,   369,     0,     0,     0,     0,
     176,     0,     0,     0,     0,     0,     0,   500,   500,   153,
       0,     0,     0,     0,     0,  1059,     0,   172,   153,   512,
       0,   512,   172,     0,   512,   512,     0,     0,   181,     0,
       0,   172,   181,   181,   172,     0,  1142,  1145,   440,   441,
     167,   442,   443,   444,     0,   445,   446,   134,     0,     0,
     449,     0,     0,     0,   181,     0,   455,   456,   181,     0,
     459,     0,     0,   460,   461,   462,     0,     0,     0,     0,
       0,   153,   986,   417,     0,   175,     0,     0,   175,     0,
       0,   167,     0,   167,     0,     0,     0,     0,   134,   167,
     134,     0,   167,   418,  1180,     0,   134,   419,     0,   134,
    1142,  1145,     0,     0,     0,     0,     0,     0,   172,     0,
     172,     0,   153,   172,   153,     0,     0,  1191,     0,     0,
     153,   167,   420,   153,     0,   421,     0,   167,   134,     0,
    1204,   167,   167,  1207,   134,     0,   172,   422,   134,   134,
       0,     0,     0,   167,     0,     0,   172,   167,     0,   167,
     134,   621,   153,   183,   134,   423,   134,     0,   153,     0,
     424,     0,   153,   153,     0,     0,     0,   175,     0,   181,
     167,  1257,   175,     0,   153,     0,   183,   134,   153,     0,
     153,   175,     0,     0,   175,   176,     0,   172,   176,     0,
       0,     0,     0,     0,     0,     0,   172,     0,     0,     0,
       0,   153,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   167,   167,   167,   167,     0,     0,     0,   134,
     134,   134,   134,   181,     0,   183,     0,     0,   181,   183,
     183,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     621,     0,     0,   153,   153,   153,   153,     0,     0,   172,
       0,   183,     0,     0,     0,   183,     0,     0,   175,     0,
     175,     0,     0,   175,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   226,   176,     0,     0,
       0,     0,   176,     0,     0,     0,   175,     0,     0,     0,
     172,   176,   172,     0,   176,     0,   175,     0,   172,     0,
       0,   172,     0,   181,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   181,   436,     0,
       0,     0,     0,     0,     0,   330,   181,     0,     0,     0,
     172,   184,     0,     0,     0,     0,   172,   175,     0,   181,
     172,   172,     0,     0,     0,     0,   175,     0,     0,     0,
       0,     0,   172,     0,   184,   357,   172,     0,   172,     0,
       0,     0,     0,     0,     0,     0,   183,     0,   176,     0,
     176,     0,     0,   176,     0,     0,     0,   437,     0,   172,
       0,   438,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   176,     0,     0,   175,
       0,     0,     0,   184,     0,     0,   176,   184,   184,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     183,   172,   172,   172,   172,   183,     0,   440,   441,   184,
     442,   443,   444,   184,   445,   446,     0,     0,     0,   449,
     175,     0,   175,     0,     0,     0,   456,   176,   175,     0,
       0,   175,   460,   461,   462,     0,   176,     0,     0,     0,
       0,   181,     0,   181,     0,     0,   493,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     175,     0,   139,     0,     0,     0,   175,     0,     0,     0,
     175,   175,     0,     0,     0,     0,     0,     0,     0,     0,
     183,     0,   175,     0,     0,   139,   175,     0,   175,   176,
       0,     0,     0,     0,   183,     0,     0,     0,     0,     0,
     517,     0,     0,   183,     0,   527,     0,     0,   436,   175,
       0,     0,     0,     0,     0,     0,   183,   181,     0,     0,
       0,     0,     0,     0,   184,     0,     0,     0,     0,     0,
     176,     0,   176,     0,   139,     0,     0,     0,   176,     0,
       0,   176,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   175,   175,   175,   175,     0,     0,     0,     0,     0,
     139,     0,     0,     0,   139,     0,     0,   437,     0,     0,
     176,   438,     0,     0,     0,     0,   176,     0,   184,     0,
     176,   176,     0,   184,     0,     0,     0,     0,     0,     0,
       0,     0,   176,     0,     0,     0,   176,     0,   176,     0,
     439,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   599,   440,   441,   176,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,     0,   453,   454,   455,   456,   457,   183,   459,
     183,     0,   460,   461,   462,     0,     0,     0,   319,     0,
       0,     0,     0,   463,     0,     0,     0,     0,   184,     0,
       0,   176,   176,   176,   176,     0,     0,   319,     0,     0,
       0,     0,   184,     0,   429,   139,   230,     0,     0,     0,
       0,   184,   319,     0,     0,     0,     0,     0,     0,     0,
     231,   232,     0,   233,   184,     0,     0,     0,   234,     0,
       0,     0,   181,     0,     0,   181,   235,     0,     0,     0,
       0,     0,   236,     0,   183,     0,     0,     0,   237,     0,
       0,     0,   238,     0,     0,   239,     0,   240,     0,   139,
       0,     0,     0,     0,   139,     0,   241,     0,   767,     0,
     769,     0,     0,   242,   243,     0,     0,     0,     0,     0,
     358,   244,     0,     0,    22,    23,     0,     0,     0,     0,
       0,   245,     0,   359,     0,    29,   360,     0,     0,     0,
     246,    32,   247,     0,   248,     0,   249,     0,    37,   250,
       0,     0,     0,   251,     0,   252,     0,     0,   253,     0,
       0,     0,     0,     0,   181,     0,     0,     0,     0,   181,
     361,     0,     0,     0,     0,    54,     0,    56,   181,   139,
     319,   181,     0,     0,     0,     0,   184,   362,   184,    65,
       0,     0,     0,   139,     0,     0,     0,     0,     0,     0,
       0,     0,   139,   230,     0,     0,     0,    80,     0,    82,
       0,     0,    84,     0,   270,   139,   319,   231,   232,     0,
     233,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,   235,     0,     0,     0,     0,     0,   236,
     885,   319,     0,     0,  -315,   237,     0,     0,     0,   238,
       0,     0,   239,     0,   240,   181,     0,   181,     0,     0,
     181,  -315,   184,   241,     0,  -315,  -315,     0,     0,   183,
     242,   243,   183,     0,  -315,     0,  -315,  -315,   244,     0,
     319,     0,  -315,   181,     0,     0,     0,     0,   245,  -315,
       0,     0,  -315,   181,     0,     0,     0,   246,     0,   247,
       0,   248,     0,   249,     0,     0,   250,     0,     0,     0,
     251,  -315,   252,  -315,     7,   253,  -315,     0,  -315,     0,
    -315,     0,  -315,  -315,     0,  -315,     0,     0,  -315,     0,
    -315,   358,     0,     0,   181,    22,    23,   139,     0,   139,
       0,     0,     0,   181,   359,     0,    29,   360,  -315,     0,
    -315,     0,    32,  -315,     0,     0,     0,     0,     0,    37,
       0,   183,     0,     0,     0,     0,   183,     0,     0,   952,
       0,   270,   955,     0,     0,   183,     0,     0,   183,     0,
       0,   361,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,  1056,     0,     0,  1057,   181,     0,   362,     0,
      65,   436,     0,  -315,     0,     0,     0,     0,     0,  -315,
       0,     0,     0,   139,     0,     0,     0,     0,    80,     0,
      82,   319,   319,    84,     0,     0,     0,   319,     0,     0,
     319,   319,   319,   319,     0,     0,     0,   181,     0,   181,
       0,     0,     0,     0,     0,   181,     0,   184,   181,     0,
     184,     0,   183,     0,   183,     0,     0,   183,     0,     0,
     437,  1029,     0,     0,   438,     0,  1035,   319,     0,     0,
       0,     0,     0,    98,     0,   527,     0,   181,   527,  1058,
     183,     0,     0,   181,     0,     0,     0,   181,   181,     0,
     183,     0,     0,     0,     0,     0,     0,     0,     0,   181,
       0,     0,     0,   181,     0,   181,     0,   319,     0,     0,
     440,   441,     0,     0,   443,   444,     0,   445,   446,     0,
     319,   319,   449,     0,     0,     0,   181,     0,     0,   456,
     319,   183,     0,     0,     0,   460,   461,   462,     0,   184,
     183,     0,     0,     0,   184,     0,     0,     0,     0,     0,
       0,     0,  1088,   184,  1089,     0,   184,  1091,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   181,   181,
     181,   181,     0,     0,     0,     0,     0,     0,     0,     0,
    1105,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1035,     0,     0,   183,     0,     0,     0,     0,   139,     0,
       0,   139,   464,   465,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,   476,   477,   478,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     184,  1132,   184,     0,   183,   184,   183,     0,     0,     0,
    1138,     0,   183,     0,     0,   183,   479,     0,     0,  -667,
       0,  -667,     0,     0,     0,     0,     0,     0,   184,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   184,     0,
       0,     0,     0,     0,   183,     0,     0,     0,     0,     0,
     183,     0,     0,     0,   183,   183,     0,     0,     0,     0,
     139,   319,     0,  1174,     0,   139,   183,     0,     0,     0,
     183,     0,   183,     0,   139,     0,     0,   139,     0,   184,
       0,     0,     0,   319,     0,   319,     0,     0,   184,     0,
       0,     0,     0,   183,     0,     0,     0,     0,     0,     0,
       0,     0,   319,     0,  1193,     0,  1197,     0,     0,     0,
       0,     0,   527,     0,     0,   527,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   183,   183,   183,   183,     0,
       0,   184,     0,     0,  1232,     0,     0,     0,     0,     0,
    1235,   139,     0,   139,  1193,  1197,   139,     0,     0,     0,
       0,     0,   275,     0,     0,     0,  1241,     0,     0,     0,
    1243,     0,   769,     0,     0,     0,     0,     0,     0,   139,
       0,     0,   184,     0,   184,     0,   275,     0,     0,   139,
     184,     0,     0,   184,     0,   336,   338,     0,     0,     0,
       0,     0,     0,     0,     0,   319,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   210,   184,     0,     0,   220,   368,     0,   184,     0,
     139,     0,   184,   184,   228,   769,   769,   769,   769,   139,
       0,   261,     0,     0,   184,     0,     0,   319,   184,     0,
     184,     0,     0,   277,   396,   280,   281,     0,     0,     0,
       0,   286,     0,   287,     0,   288,     0,     0,     0,     0,
     314,   184,     0,     0,   228,   322,   324,   326,   327,     0,
       0,     0,   331,     0,   332,     0,     0,     0,     0,     0,
       0,   339,   139,     0,   322,     0,   341,     0,   342,     0,
       0,   343,   344,     0,   345,     0,     0,     0,   228,   322,
     355,     0,   319,   184,   184,   184,   184,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       7,     0,     0,   139,     0,   139,     0,   379,   495,   382,
       0,   139,     0,     0,   139,     0,     0,   358,     0,     0,
       0,    22,    23,     0,     0,     0,     0,     0,     0,     0,
     359,     0,    29,   360,     0,     0,     0,     0,    32,     0,
       0,     0,     0,   139,     0,    37,     0,   508,   511,   139,
       0,     0,   433,   139,   139,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   275,   139,   275,   361,     0,   139,
       0,   139,    54,   275,    56,   485,   356,     0,  1056,   331,
     228,  1057,     0,     0,   362,     0,    65,   319,   319,     0,
       0,     0,   139,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    82,   322,     0,    84,
       0,     0,     0,   275,     0,     0,   230,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     231,   232,     0,   233,   139,   139,   139,   139,   234,     0,
       0,     0,     0,   322,     0,     0,   235,     0,     0,     0,
       0,     0,   236,     0,     0,     0,     0,     0,   237,    98,
       0,     0,   238,     0,     0,   239,     0,   240,   322,     0,
       0,     0,     0,     0,     0,     0,   241,     0,     0,   331,
       0,     0,     0,   242,   243,     0,     0,     0,     0,     0,
       0,   244,     0,     0,   549,   550,     0,     0,     0,     0,
       0,   245,     0,     0,     0,     0,     0,   322,     0,     0,
     246,   559,   247,     0,   248,     0,   249,     0,     0,   250,
       0,     0,     0,   251,     0,   252,     0,     0,   253,   568,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   574,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   586,     0,   588,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   602,   609,   612,
       0,     0,     0,     0,     0,     0,   228,     0,     0,     0,
       0,     0,   363,     0,   984,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   690,   691,   692,   694,   696,   697,     0,   698,
     699,   700,   701,   702,     0,   710,   711,   712,   713,   714,
     715,   716,   717,   718,   719,   720,   721,   722,   723,   724,
     725,   726,     0,     0,     0,     0,   275,     0,   322,   322,
       0,     0,     0,     0,   322,     0,     0,   322,   322,   322,
     322,   736,   738,     0,     0,     0,   609,   743,     0,     0,
     758,     0,     0,     0,     0,     0,   761,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   770,   275,   275,
     772,     0,     0,     0,   322,     0,     0,     0,   275,   275,
       0,   780,     0,   783,     0,     0,     0,   280,     0,     0,
     788,   789,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   314,     0,     0,   314,     0,     0,     0,     0,
       0,     0,     0,     0,   322,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   761,     0,     0,   322,   322,     0,
       0,     0,     0,   228,   817,     0,     0,   322,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   847,   847,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   859,     0,     0,   228,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     314,     0,     5,     6,     0,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,  -209,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,  -209,
    -209,  -209,     0,   197,    34,   903,     0,     0,    36,   957,
       0,    38,   908,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,   985,     0,     0,    62,    63,   200,     0,
       0,     0,     0,   275,   275,     0,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,   770,   322,    81,
       0,    83,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     322,     0,   322,   949,   950,     0,     0,     0,     0,    88,
       0,    90,     0,     0,     0,     0,     0,     0,     0,   322,
       0,     0,     0,   275,     0,     0,     0,   275,     0,  1053,
       0,     0,    97,     0,   314,     0,     0,     0,   977,     0,
     101,     0,  -209,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   586,     0,     0,     0,  1014,     0,  1015,
     847,   847,     0,   341,     0,   342,     0,     0,   344,   345,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1026,     0,     0,     0,   228,     0,     0,     0,
    1032,   761,     0,  1036,     0,     0,     0,     7,     0,  1042,
    1044,     0,  1045,     0,     0,  1048,  1050,     0,     0,     0,
       0,     0,   322,     0,   358,     0,     0,  1062,    22,    23,
       0,     0,     0,     0,  1110,     0,     0,   359,     0,    29,
     360,     0,     0,     0,     0,    32,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,     0,     0,     0,  1079,
    1036,  1045,  1048,  1082,   322,     0,     0,     0,   758,     0,
       0,     0,     0,     0,   361,     0,     0,     0,     0,    54,
       0,    56,   694,   736,     0,  1056,     0,     0,  1057,     0,
       0,   362,  1090,    65,     0,     0,   275,   275,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   314,     0,
       0,    80,  1104,    82,     0,     0,    84,     0,   228,     0,
       0,     0,     0,     0,     0,     0,  1112,     0,  1114,   322,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   847,
       0,     0,     0,     0,   275,     0,  1123,     0,  1124,     0,
     275,   275,     0,     0,     0,  1130,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   275,   761,     0,
       0,     0,     0,     0,     0,  1140,     0,   511,     0,     0,
     275,     0,     0,   275,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1161,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   230,     0,  1168,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1236,     0,
     231,   232,     0,   233,   322,   322,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,   235,  1182,     0,     0,
       0,     0,   236,     0,     0,  1186,   847,   847,   237,     0,
       0,     0,   238,     0,     0,   239,     0,   240,     0,     0,
       0,  1194,     0,  1198,     0,     0,   241,  1200,     0,  1203,
       0,     0,  1206,   242,   243,     0,     0,   511,     0,   511,
       0,   244,   511,   511,     0,     0,     0,     0,     0,     0,
       0,   245,     0,     0,  1194,  1198,     0,  1203,  1206,     0,
     246,     0,   247,     0,   248,     0,   249,     0,     0,   250,
       0,     0,     0,   251,     0,   252,     0,  1239,   253,     0,
    1240,     0,     0,     0,  1242,     0,     0,     0,     0,  1244,
    1245,     0,     0,  1246,     0,     0,     0,     0,     0,     0,
       0,  1248,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1258,  1259,  1244,  1260,  1261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   518,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1272,  1273,  1274,  1275,  1272,  1273,  1274,  1275,
      -2,     4,     0,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -209,    21,    22,    23,    24,    25,
      26,     0,  -209,    27,     0,    28,     0,    29,    30,    31,
    -209,  -209,  -209,    32,    33,    34,   -68,     0,    35,    36,
      37,     0,    38,  -335,    39,     0,    40,    41,    42,    43,
      44,     0,    45,    46,    47,    48,   -68,    49,    50,     0,
      51,    52,    53,     0,  -335,     0,     0,    54,    55,    56,
      57,    58,    59,    60,  -335,   -68,    61,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,     0,    75,    76,     0,    77,    78,    79,    80,
      81,    82,    83,   -68,    84,    85,     0,     0,  -209,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,    98,     0,     0,    99,     0,   100,
       0,   101,     0,  -209,     0,     4,   102,     5,     6,     7,
       8,     9,    10,    11,    12,     0,  -697,     0,    13,    14,
      15,    16,    17,  -697,    18,     0,    19,    20,  -209,    21,
      22,    23,    24,    25,    26,  -697,  -209,    27,  -697,    28,
       0,    29,    30,    31,  -209,  -209,  -209,    32,    33,    34,
     -68,     0,    35,    36,    37,     0,    38,  -335,    39,     0,
      40,    41,    42,    43,    44,     0,    45,    46,    47,    48,
     -68,    49,    50,     0,    51,    52,    53,     0,  -335,     0,
       0,    54,    55,    56,    57,     0,    59,    60,  -335,   -68,
      61,    62,    63,    64,  -697,    65,    66,    67,  -697,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,    82,    83,   -68,    84,    85,
       0,     0,  -209,     0,    86,     0,     0,  -697,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,  -697,    90,  -697,  -697,  -697,
    -697,  -697,  -697,  -697,     0,  -697,  -697,  -697,  -697,  -697,
    -697,  -697,  -697,  -697,  -697,  -697,  -697,    97,    98,  -697,
    -697,  -697,     0,   100,  -697,   101,     0,  -209,     0,   346,
    -697,     5,     6,     7,     8,     9,    10,    11,    12,     0,
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
      96,    97,    98,     0,     0,    99,     0,   100,   347,   101,
       0,  -209,     0,     4,   102,     5,     6,     7,     8,     9,
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
       0,   100,   564,   101,     0,  -209,     0,   581,   102,     5,
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
      98,     0,     0,    99,     0,   100,   582,   101,     0,  -209,
       0,     4,   102,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -209,    21,    22,    23,    24,    25,
      26,     0,  -209,    27,     0,    28,     0,    29,    30,    31,
    -209,  -209,  -209,    32,    33,    34,   -68,     0,    35,    36,
      37,     0,    38,  -335,    39,     0,    40,    41,    42,    43,
      44,     0,    45,    46,    47,    48,   -68,    49,    50,     0,
      51,    52,    53,     0,  -335,     0,     0,    54,    55,    56,
      57,    58,    59,    60,  -335,   -68,    61,    62,    63,    64,
       0,    65,    66,    67,     0,    68,    69,    70,    71,    72,
      73,    74,     0,    75,    76,     0,    77,    78,    79,    80,
      81,    82,    83,   -68,    84,    85,     0,     0,  -209,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,    98,     0,     0,    99,     0,   100,
     824,   101,     0,  -209,     0,     4,   102,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -209,    21,
      22,    23,    24,    25,    26,     0,  -209,    27,     0,    28,
       0,    29,    30,    31,  -209,  -209,  -209,    32,    33,    34,
     -68,     0,    35,    36,    37,     0,    38,  -335,    39,     0,
      40,    41,    42,    43,    44,     0,    45,    46,    47,    48,
     -68,    49,    50,     0,    51,    52,    53,     0,  -335,     0,
       0,    54,    55,    56,    57,   356,    59,    60,  -335,   -68,
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
      53,     0,  -335,     0,     0,    54,    55,    56,    57,    58,
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
      30,    31,  -209,  -209,  -209,    32,    33,    34,   -68,     0,
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
    1047,    34,   -68,     0,    35,    36,    37,     0,    38,  -335,
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
    -209,  -209,  -209,    32,  1192,    34,   -68,     0,    35,    36,
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
       0,    29,    30,    31,  -209,  -209,  -209,    32,  1196,    34,
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
    -209,    32,  1202,    34,   -68,     0,    35,    36,    37,     0,
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
      30,    31,  -209,  -209,  -209,    32,  1205,    34,   -68,     0,
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
       0,   100,     0,   101,   745,  -209,     5,     6,   102,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,  -209,    21,     0,
     746,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,  -209,  -209,  -209,     0,   197,    34,     0,
     747,     0,    36,     0,   748,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,   749,
       0,    55,   750,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,     0,   751,     0,     0,     0,     0,     0,
      70,    71,    72,    73,   201,     0,    75,     0,     0,    77,
       0,     0,   752,    81,     0,    83,     0,   753,    85,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   202,     0,   101,   754,  -209,     0,   895,   102,
       5,     6,     0,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,  -209,    21,     0,     0,     0,     0,     0,     0,     0,
     294,     0,   196,     0,     0,     0,    31,  -209,  -209,  -209,
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
    -209,     5,     6,   102,     8,     9,    10,    11,    12,     0,
     639,     0,    13,     0,     0,    16,    17,   641,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,   647,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,   201,
       0,    75,     0,     0,    77,     0,     0,     0,    81,     0,
      83,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,    87,   510,   465,   466,   467,   468,   469,     0,     0,
     472,   473,   474,   475,     0,   477,   478,     0,   835,   836,
     837,   838,   839,   660,     0,   661,     0,    93,     0,   662,
     663,   664,   665,   666,   667,   668,   840,   670,   671,    95,
     841,    97,   -90,   673,   674,   842,   676,   202,     0,   101,
       0,     0,     5,     6,   102,     8,     9,    10,    11,    12,
       0,   639,     0,    13,     0,     0,    16,    17,   641,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,   197,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,   647,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,    83,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,    87,   510,   465,   466,   467,   468,   469,     0,
       0,   472,   473,   474,   475,     0,   477,   478,     0,   835,
     836,   837,   838,   839,   660,     0,   661,     0,    93,     0,
     662,   663,   664,   665,   666,   667,   668,   840,   670,   671,
      95,   841,    97,     0,   673,   674,   842,   676,   202,     0,
     101,     0,     0,     0,     0,   102,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,     0,
      16,    17,     0,    18,     0,   195,    20,     0,    21,     0,
       0,     0,     0,    26,     0,     0,    27,     0,   196,     0,
       0,     0,    31,     0,     0,     0,     0,    33,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,    45,    46,    47,    48,     0,
      49,    50,     0,    51,    52,   333,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,    73,    74,     0,    75,    76,     0,    77,
       0,     0,     0,    81,     0,    83,     0,     0,    85,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   100,     0,   101,     0,     0,     5,     6,   102,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,   746,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,   747,     0,    36,     0,   748,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
     749,     0,    55,   750,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,   751,     0,     0,     0,     0,
       0,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,   752,    81,     0,    83,     0,   753,    85,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   202,     0,   101,     0,     0,     5,     6,
     102,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,   377,    23,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   198,
       0,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,   361,     0,     0,
       0,     0,     0,    55,    56,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,     0,    65,     0,     0,     0,
       0,     0,    70,    71,    72,    73,   201,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,    83,     0,    84,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   202,     0,   101,     0,     0,     5,
       6,   102,     8,     9,    10,   380,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,    19,     0,
    -209,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,  -209,  -209,  -209,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,    64,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,   201,     0,    75,
       0,     0,    77,     0,     0,     0,    81,    82,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   202,     0,   101,     0,  -209,
       5,     6,   102,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,  -209,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   196,     0,     0,     0,    31,  -209,  -209,  -209,
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
    -209,     5,     6,   102,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,  -745,     0,
       0,     0,  -745,     0,     0,    70,    71,    72,    73,   201,
       0,    75,     0,     0,    77,     0,     0,     0,    81,     0,
      83,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,  -377,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,  -377,     0,     0,    99,     0,   202,  -377,   101,
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
     201,     0,    75,     0,     0,    77,   316,   317,     0,    81,
       0,    83,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,   318,     0,     0,    99,     0,   202,     0,
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
     351,     0,     0,     0,     0,     0,     0,     0,    94,     0,
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
      72,    73,   201,     0,    75,     0,     0,    77,   316,   317,
       0,    81,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     202,     0,   101,  -545,     0,     5,     6,   102,     8,     9,
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
       0,   202,     0,   101,     0,     0,  -545,     0,   102,     5,
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
       0,     0,    77,   316,   317,     0,    81,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   202,     0,   101,   810,     0,
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
      97,     0,     0,     0,    99,     0,   202,     0,   101,   813,
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
     819,     0,     5,     6,   102,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,     0,
       0,     0,     0,   197,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,   963,    51,
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
      73,   201,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,  -385,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,  -385,     0,     0,    99,     0,   202,
    -385,   101,     0,     0,     5,     6,   102,     8,     9,    10,
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
       0,    88,    89,    90,    91,    92,     0,  -370,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,  -370,     0,     0,    99,     0,
     202,  -370,   101,     0,     0,     5,     6,   102,     8,     9,
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
       0,     0,    88,    89,    90,    91,    92,     0,  -378,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,  -378,     0,     0,    99,
       0,   202,  -378,   101,     0,     0,     5,     6,   102,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   195,     0,     0,    21,     0,
     258,     0,     0,     0,     0,     0,    27,     0,   196,     0,
       0,     0,    31,     0,     0,     0,     0,   197,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   198,     0,   199,
       0,     0,    43,    44,     0,     0,    46,     0,    48,     0,
      49,    50,     0,    51,    52,     0,     0,     0,     0,     0,
       0,    55,     0,    57,     0,    59,     0,     0,     0,     0,
      62,    63,   200,     0,     0,     0,     0,     0,     0,     0,
      70,    71,    72,    73,   201,     0,    75,     0,     0,    77,
       0,     0,     0,    81,     0,    83,     0,   259,    85,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
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
      77,   316,   317,     0,    81,     0,    83,     0,     0,    85,
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
       0,     0,    62,    63,   200,  -745,     0,     0,     0,  -745,
       0,     0,    70,    71,    72,    73,   201,     0,    75,     0,
       0,    77,     0,     0,     0,    81,     0,    83,     0,     0,
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
       0,    85,     0,     0,     0,     0,     0,   603,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   202,     0,   101,     0,     0,
     604,     0,   102,     5,     6,     0,     8,     9,    10,    11,
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
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,   864,     0,     0,    99,     0,   202,
       0,   101,     0,     0,  1033,     0,   102,     5,     6,     0,
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
    -406,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   202,     0,   101,  -406,     0,     5,     6,
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
       0,    77,     0,     0,     0,    81,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,  -539,
       0,     0,    99,     0,   202,     0,   101,     0,     0,  -539,
       0,   102,     5,     6,     0,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,   274,    27,     0,   196,     0,     0,     0,    31,     0,
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
      95,    96,    97,     0,     0,     0,    99,     0,   100,     0,
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
      73,   201,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,   603,     0,    87,     0,     0,     0,     0,     0,     0,
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
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     202,     0,   101,     0,     0,   604,     0,   102,     5,     6,
       0,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
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
       0,     0,    99,     0,   202,   808,   101,     0,     0,     5,
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
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   202,     0,   101,     0,     0,
    1043,     0,   102,     5,     6,     0,     8,     9,    10,    11,
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
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   202,
       0,   101,     0,     0,     5,     6,   102,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   294,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
     296,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,   301,     0,    75,     0,     0,    77,     0,     0,
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
       0,    31,     0,     0,     0,     0,  1080,    34,     0,     0,
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
       0,     0,    31,     0,     0,     0,     0,  1227,    34,     0,
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
      99,     0,   202,     0,   101,     0,     0,     5,     6,   102,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,  1228,    34,
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
       0,    99,     0,   202,     0,   101,     0,     0,     5,     6,
     102,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,     0,     0,     0,     0,  1230,
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
       0,     0,    99,     0,   202,     0,   101,     0,     0,     5,
       6,   102,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
    1231,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,   201,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     6,     0,     8,     9,
      10,    11,    12,     0,    94,     0,    13,    95,    96,    97,
      17,     0,     0,    99,   195,   202,  -209,   101,     0,   293,
       0,     0,   102,     0,     0,     0,     0,   196,     0,     0,
       0,     0,  -209,  -209,  -209,     0,   197,     0,     0,   295,
       0,     0,     0,   297,     0,     0,     0,     0,   199,     0,
       0,    43,    44,     0,     0,     0,     0,    48,     0,    49,
      50,     0,     0,     0,     0,     0,     0,     0,   298,     0,
      55,   299,    57,    58,    59,     0,     0,     0,     0,    62,
       0,   200,     0,   300,     0,     0,     0,     0,     0,    70,
      71,     0,    73,     0,     0,     0,     0,     0,     0,     0,
       0,   302,     0,     0,    83,     0,   303,     0,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   202,     0,   101,  -339,  -209,     0,     6,   102,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
       0,    17,     0,     0,     0,   195,     0,  -209,     0,     0,
     293,     0,     0,     0,     0,     0,     0,     0,   196,     0,
       0,     0,     0,  -209,  -209,  -209,     0,   197,     0,     0,
     295,     0,     0,     0,   297,     0,     0,     0,     0,   199,
       0,     0,    43,    44,     0,     0,     0,     0,    48,     0,
      49,    50,     0,     0,     0,     0,     0,     0,     0,   298,
       0,    55,   299,    57,    58,    59,     0,     0,     0,     0,
      62,     0,   200,     0,   300,     0,     0,     0,     0,     0,
      70,    71,     0,    73,     0,     0,     0,     0,     0,     0,
       0,     0,   302,     0,     0,    83,     0,   303,     0,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   637,
       0,   638,     0,    88,    89,    90,    91,    92,     0,     0,
     639,     0,    93,     0,     0,   640,   232,   641,   642,     0,
       0,    94,     0,   643,    95,    96,    97,     0,     0,     0,
      99,   235,   202,   196,   101,     0,  -209,   236,     0,   102,
       0,     0,     0,   644,     0,     0,     0,   238,     0,     0,
     645,     0,   646,   647,     0,     0,     0,     0,     0,     0,
       0,   648,     0,     0,     0,     0,     0,     0,   242,   649,
       0,     0,     0,     0,     0,     0,   244,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   650,     0,     0,     0,
       0,     0,     0,     0,     0,   246,     0,   651,     0,   248,
       0,   652,     0,     0,   250,     0,     0,     0,   653,     0,
     252,     0,     0,   654,     0,     0,     0,     0,     0,     0,
       0,     0,   510,   465,   466,   467,   468,   469,     0,     0,
     472,   473,   474,   475,     0,   477,   478,     0,   655,   656,
     657,   658,   659,   660,     0,   661,     0,     0,     0,   662,
     663,   664,   665,   666,   667,   668,   669,   670,   671,     0,
     672,     0,     0,   673,   674,   675,   676,     5,     6,   677,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,  -209,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   196,
       0,     0,     0,    31,  -209,  -209,  -209,     0,   197,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,    83,     0,     0,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,     0,    96,    97,     0,     0,
       0,    99,     0,   202,     0,   101,     0,  -209,     5,     6,
       0,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   195,     0,  -209,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     196,     0,     0,     0,    31,  -209,  -209,  -209,     0,   197,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   198,
       0,   199,     0,     0,    43,    44,     0,     0,    46,     0,
      48,     0,    49,    50,     0,    51,    52,     0,     0,     0,
       0,     0,     0,    55,     0,    57,     0,    59,     0,     0,
       0,     0,    62,    63,   200,     0,     0,     0,     0,     0,
       0,     0,    70,    71,    72,    73,   201,     0,    75,     0,
       0,    77,   230,     0,     0,    81,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,   231,   232,     0,   233,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,     0,     0,    88,     0,    90,   236,     0,
       0,     0,     0,     0,   237,     0,     0,     0,   238,     0,
       0,   239,     0,   240,     0,     0,     0,     0,    97,     0,
       0,     0,   241,     0,   202,     0,   101,     0,  -209,   242,
     243,     0,     0,     0,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,   246,     0,   247,     0,
     248,   703,   249,     0,     0,   250,     0,     0,     0,   251,
     639,   252,     0,     0,   253,   995,   996,   641,   997,     0,
       0,     0,     0,   998,     0,     0,     0,     0,     0,     0,
       0,   705,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   999,     0,     0,     0,  1000,     0,     0,
    1001,     0,   198,   647,     0,     0,     0,     0,     0,     0,
       0,   706,     0,     0,     0,     0,     0,     0,     0,  1002,
     545,     0,     0,     0,     0,     0,  1003,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1004,     0,     0,     0,
       0,     0,     0,     0,     0,  1005,     0,  1006,     0,  1007,
       0,   707,     0,     0,     0,     0,     0,   708,  1008,     0,
    1009,     0,     0,  1010,     0,     0,     0,     0,     0,     0,
       0,     0,   510,   465,   466,   467,   468,   469,     0,     0,
     472,   473,   474,   475,     0,   477,   478,     0,   655,   656,
     657,   658,   659,   660,     0,   661,     0,     0,     0,   662,
     663,   664,   665,   666,   667,   668,   669,   670,   671,   230,
     672,     0,     0,   673,   674,   675,   676,  1011,   639,     0,
       0,     0,     0,   231,   232,   641,   233,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,   235,
       0,     0,     0,     0,     0,   236,     0,     0,     0,     0,
       0,   237,     0,     0,     0,   238,     0,     0,   239,     0,
     240,   647,     0,     0,     0,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,     0,     0,
       0,     0,     0,     0,   244,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   245,     0,     0,     0,     0,     0,
       0,     0,     0,   246,     0,   247,     0,   248,     0,   249,
       0,     0,   250,     0,     0,     0,   251,     0,   252,     0,
       0,   253,     0,     0,     0,     0,     0,     0,     0,     0,
     510,   465,   466,   467,   468,   469,     0,     0,   472,   473,
     474,   475,     0,   477,   478,     0,   655,   656,   657,   658,
     659,   660,     0,   661,     0,     0,     0,   662,   663,   664,
     665,   666,   667,   668,   669,   670,   671,     7,   672,     0,
       0,   673,   674,   675,   676,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   358,     0,     0,     0,    22,    23,
       0,     0,     0,     0,     0,     0,     0,   359,     0,    29,
     360,     0,     0,     0,     0,    32,     0,     0,     0,     0,
       0,     0,    37,     0,     7,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   358,     0,     0,   361,    22,    23,     0,     0,    54,
       0,    56,     0,    58,   359,  1056,    29,   360,  1057,   434,
       0,   362,    32,    65,     0,     0,   435,     0,     0,    37,
       0,     0,     0,     0,     0,     0,     0,     0,   436,     0,
       0,    80,     0,    82,     0,     0,    84,     0,     0,     0,
       0,   361,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,  1056,     0,     0,  1057,     0,     0,   362,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      82,     0,     0,    84,     0,     0,    98,   437,     0,   434,
       0,   438,  1126,     0,     0,     0,   435,     0,     0,   509,
       0,     0,     0,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,   510,   465,   466,   467,   468,   469,     0,     0,   472,
     473,   474,   475,    98,   477,   478,     0,   440,   441,  1190,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
       0,     0,   460,   461,   462,     0,   363,   437,     0,   434,
       0,   438,     0,   463,     0,     0,   435,     0,     0,  1201,
       0,     0,     0,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,   510,   465,   466,   467,   468,   469,     0,     0,   472,
     473,   474,   475,     0,   477,   478,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
       0,     0,   460,   461,   462,     0,   363,   437,     0,   434,
       0,   438,     0,   463,     0,     0,   435,     0,     0,  1264,
       0,     0,     0,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,   510,   465,   466,   467,   468,   469,     0,     0,   472,
     473,   474,   475,     0,   477,   478,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
       0,     0,   460,   461,   462,     0,   363,   437,     0,   434,
       0,   438,     0,   463,     0,     0,   435,     0,     0,  1265,
       0,     0,     0,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,   510,   465,   466,   467,   468,   469,     0,     0,   472,
     473,   474,   475,     0,   477,   478,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
       0,     0,   460,   461,   462,     0,   363,   437,     0,   434,
       0,   438,     0,   463,     0,     0,   435,     0,     0,  1266,
       0,     0,     0,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,   510,   465,   466,   467,   468,   469,     0,     0,   472,
     473,   474,   475,     0,   477,   478,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
       0,     0,   460,   461,   462,     0,   363,   437,     0,     0,
       0,   438,     0,   463,     0,     0,     0,     0,   434,  1267,
       0,     0,     0,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,   877,     0,
     439,   510,   465,   466,   467,   468,   469,     0,     0,   472,
     473,   474,   475,   878,   477,   478,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
       0,     0,   460,   461,   462,     0,   363,     0,     0,     0,
       0,     0,   434,   463,     0,     0,   437,     0,     0,   435,
     438,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   274,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   214,     0,   990,     0,   439,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,     0,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
     437,   460,   461,   462,   438,   363,     0,     0,     0,     0,
       0,     0,   463,   434,     0,     0,     0,     0,     0,     0,
     435,     0,     0,     0,     0,     0,     0,     0,     0,   214,
     437,     0,   436,   439,   438,     0,     0,   590,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,     0,   591,   460,   461,   462,     0,   363,
     440,   441,     0,   442,   443,   444,   463,   445,   446,   447,
       0,   437,   449,   450,   451,   438,   453,   454,   455,   456,
       0,     0,   459,   434,   831,   460,   461,   462,     0,     0,
     435,     0,     0,     0,     0,     0,   463,     0,     0,     0,
       0,     0,   436,     0,   439,     0,     0,   832,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   833,   592,   460,   461,   462,     0,
       0,     0,     0,     0,     0,     0,     0,   463,     0,     0,
       0,   437,   434,     0,     0,   438,     0,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,  1144,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,     0,   460,   461,   462,     0,
     437,     0,   434,     0,   438,     0,     0,   463,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   274,     0,     0,     0,     0,     0,     0,   214,
       0,     0,     0,   439,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,     0,     0,   460,   461,   462,     0,   363,
     437,     0,   434,     0,   438,     0,   463,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   274,     0,     0,     0,     0,     0,     0,   214,
       0,     0,     0,   439,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,     0,     0,   460,   461,   462,     0,   363,
     437,     0,   434,     0,   438,     0,   463,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   924,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   439,     0,     0,     0,   925,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,     0,     0,   460,   461,   462,     0,   363,
     437,   434,     0,     0,   438,     0,   463,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   439,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,     0,     0,   460,   461,   462,     0,   437,
       0,     0,     0,   438,     0,     0,   463,     0,     0,     0,
       0,   956,   434,     0,     0,     0,     0,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   439,     0,     0,     0,  1118,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,     0,  1119,   460,   461,   462,     0,   363,     0,
       0,     0,     0,     0,     0,   463,   434,     0,     0,     0,
     437,     0,     0,   435,   438,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   439,     0,     0,     0,     0,     0,     0,
       0,   434,     0,     0,     0,     0,     0,     0,   435,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     436,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,     0,   437,   460,   461,   462,   438,     0,
       0,   434,   531,     0,     0,     0,   463,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,   214,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
       0,     0,     0,   438,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,   214,   460,
     461,   462,   439,     0,     0,     0,     0,     0,  1135,   437,
     463,     0,     0,   438,     0,     0,     0,     0,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   439,     0,   460,   461,   462,     0,     0,     0,
     434,     0,     0,  1184,     0,   463,     0,   435,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,   436,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,     0,     0,   460,   461,   462,     0,     0,     0,
     434,     0,     0,     0,     0,   463,     0,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,     0,
     434,     0,   438,     0,     0,     0,     0,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   439,     0,     0,     0,     0,     0,     0,   437,     0,
       0,     0,   438,     0,     0,     0,     0,     0,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,     0,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   439,     0,   460,   461,   462,     0,   554,   437,   434,
       0,     0,   438,     0,   463,     0,   435,     0,   440,   441,
     739,   442,   443,   444,     0,   445,   446,   447,   436,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   439,   558,   460,   461,   462,     0,     0,     0,   434,
     827,     0,     0,     0,   463,     0,   435,     0,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,   436,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,     0,     0,   460,   461,   462,     0,   437,     0,     0,
     434,   438,     0,     0,   463,     0,     0,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,     0,     0,     0,     0,     0,     0,   437,     0,     0,
       0,   438,     0,     0,     0,     0,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     439,     0,   460,   461,   462,     0,     0,     0,   437,   809,
       0,     0,   438,   463,     0,     0,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   828,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     871,   439,   460,   461,   462,     0,     0,     0,     0,     0,
       0,     0,     0,   463,     0,     0,     0,     0,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,     0,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   434,     0,   460,   461,   462,     0,     0,   435,     0,
       0,     0,     0,     0,   463,     0,     0,     0,     0,     0,
     436,     0,     0,   954,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   434,     0,     0,     0,     0,     0,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
       0,   434,     0,   438,     0,     0,     0,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   439,     0,     0,     0,     0,     0,     0,   437,
       0,     0,     0,   438,     0,     0,     0,     0,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   439,     0,   460,   461,   462,     0,     0,   437,
     434,  1020,     0,   438,     0,   463,     0,   435,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,   436,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   439,     0,   460,   461,   462,     0,     0,     0,
     434,   966,     0,     0,     0,   463,     0,   435,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,   436,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,     0,     0,   460,   461,   462,     0,   437,     0,
       0,   967,   438,     0,     0,   463,     0,     0,   434,  1117,
       0,     0,     0,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,   439,     0,     0,     0,     0,     0,     0,   437,     0,
       0,     0,   438,     0,     0,     0,     0,     0,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,     0,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   439,     0,   460,   461,   462,     0,     0,     0,     0,
       0,     0,     0,     0,   463,     0,   437,     0,   440,   441,
     438,   442,   443,   444,     0,   445,   446,   447,     0,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   434,     0,   460,   461,   462,     0,     0,   435,   439,
    1072,     0,     0,     0,   463,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,     0,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   434,
       0,   460,   461,   462,     0,     0,   435,     0,     0,     0,
       0,     0,   463,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
       0,     0,     0,   438,     0,     0,     0,     0,     0,   434,
       0,     0,     0,     0,     0,     0,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   436,     0,
       0,     0,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,     0,   440,
     441,   438,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,     0,  1129,   460,   461,   462,     0,     0,  1139,
     439,     0,     0,     0,     0,   463,     0,   437,     0,     0,
       0,   438,     0,     0,     0,     0,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     439,     0,   460,   461,   462,     0,     0,     0,     0,     0,
       0,     0,     0,   463,     0,     0,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     434,     0,   460,   461,   462,     0,     0,   435,     0,     0,
     619,  1167,   230,   463,     7,     0,     0,     0,     0,   436,
    1170,     0,     0,     0,     0,     0,   231,   232,     0,   233,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
     434,  1189,   235,     0,     0,     0,     0,   435,   236,     0,
       0,     0,     0,     0,   237,     0,     0,     0,   238,   436,
       0,   239,     0,   240,     0,     0,     0,     0,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,   437,   242,
     243,   434,   438,     0,     0,     0,     0,   244,   435,     0,
       0,     0,     0,     0,     0,     0,     0,   245,     0,     0,
     436,     0,     0,     0,     0,     0,   246,     0,   247,     0,
     248,   439,   249,     0,     0,   250,     0,     0,   437,   251,
       0,   252,   438,     0,   253,     0,     0,     0,   440,   441,
    1221,   442,   443,   444,     0,   445,   446,   447,     0,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   439,     0,   460,   461,   462,     0,     0,     0,   437,
       0,     0,     0,   438,   463,     0,     0,     0,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,     0,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,     0,   439,   460,   461,   462,   434,     0,     0,     0,
       0,     0,     0,   435,   463,     0,     0,     0,     0,   440,
     441,     0,   442,   443,   444,   436,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,     0,     0,   460,   461,   462,     0,     0,     0,
       0,     0,     0,     0,   434,   463,     0,     0,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,     0,     0,     0,   438,     0,
       0,     0,     0,     0,   434,     0,  1229,     0,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,     0,   440,   441,   438,   442,   443,   444,
       0,   445,   446,   447,  1268,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,     0,     0,   439,     0,     0,     0,     0,
     463,     0,   437,   434,     0,     0,   438,     0,     0,     0,
     435,     0,   440,   441,  1269,   442,   443,   444,     0,   445,
     446,   447,   436,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   439,     0,   460,   461,   462,
       0,     0,     0,   434,     0,     0,     0,     0,   463,     0,
     435,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   436,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,     0,     0,   460,   461,   462,
       0,   437,     0,   434,     0,   438,     0,     0,   463,     0,
     435,     0,     0,  1270,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,     0,     0,     0,     0,     0,
       0,   437,     0,     0,     0,   438,     0,     0,     0,     0,
       0,   440,   441,  1271,   442,   443,   444,     0,   445,   446,
     447,   436,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   439,     0,   460,   461,   462,     0,
       0,   437,   434,     0,     0,   438,     0,   463,     0,   435,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   436,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   439,     0,   460,   461,   462,     0,
     437,     0,     0,     0,   438,     0,     0,   463,     0,     0,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,     0,   460,   461,   462,     0,
     944,     0,     0,     0,   438,     0,     0,   463,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,     0,   453,   454,   455,   456,
     457,     0,   459,   439,     0,   460,   461,   462,     0,     0,
       0,     0,     0,     0,     0,     0,   463,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     436,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,     0,   358,   460,   461,   462,    22,    23,
       0,     0,     0,     0,     0,     0,   463,   359,     0,    29,
     360,     0,     0,     0,     0,    32,     0,     0,     0,     0,
       0,     0,    37,     0,     0,  -335,     0,     0,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,   -68,   437,
       0,     0,     0,   438,   361,     0,  -335,     0,     0,    54,
       0,    56,     0,     0,     0,    60,  -335,   -68,    61,     0,
       0,   362,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,   439,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,    82,     0,     0,    84,     0,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   230,   453,   454,   455,   456,   457,
     458,   459,     0,     0,   460,   461,   462,     0,   231,   232,
       0,   233,     0,     0,     0,   463,   234,     0,    23,     0,
       0,     0,     0,     0,   235,     0,    98,     0,     0,     0,
     236,   363,     0,     0,     0,     0,   237,     0,     0,     0,
     238,     0,     0,   239,     0,   240,     0,     0,     0,     0,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,     0,     0,     0,     0,     0,     0,   244,
      56,     0,     0,     0,     0,     0,     0,     0,     0,   245,
       0,     0,    65,     0,     0,   230,     0,     7,   246,     0,
     247,     0,   248,     0,   249,     0,     0,   250,     0,   231,
     232,   251,   233,   252,     0,    84,   253,   234,     0,     0,
       0,     0,     0,     0,     0,   235,     0,     0,     0,     0,
       0,   236,     0,     0,     0,     0,     0,   237,     0,     0,
       0,   238,     0,     0,   239,     0,   240,     0,     0,     0,
       0,     0,     0,     0,  -310,   241,     0,     0,     0,     0,
       0,     0,   242,   243,     0,     0,     0,     0,  -310,  -310,
     244,  -310,     0,     0,     0,     0,  -310,     0,     0,     0,
     245,     0,     0,     0,  -310,     0,     0,     0,     0,   246,
    -310,   247,     0,   248,     0,   249,  -310,     0,   250,     0,
    -310,     0,   251,  -310,   252,  -310,     0,   253,     0,     0,
       0,     0,     0,   230,  -310,     0,     0,     0,     0,     0,
       0,  -310,  -310,     0,     0,     0,     0,   231,   232,  -310,
     233,     0,     0,     0,     0,   234,     0,     0,     0,  -310,
       0,     0,     0,   235,     0,     0,     0,     0,  -310,   236,
    -310,     0,  -310,     0,  -310,   237,     0,  -310,     0,   238,
       0,  -310,   239,  -310,   240,     0,  -310,     0,     0,     0,
       0,     0,  -311,   241,     0,     0,     0,     0,     0,     0,
     242,   243,     0,     0,     0,     0,  -311,  -311,   244,  -311,
       0,     0,     0,     0,  -311,     0,     0,     0,   245,     0,
       0,     0,  -311,     0,     0,     0,     0,   246,  -311,   247,
       0,   248,     0,   249,  -311,     0,   250,     0,  -311,     0,
     251,  -311,   252,  -311,     0,   253,     0,     0,     0,     0,
       0,     0,  -311,     0,     0,     0,     0,     0,     0,  -311,
    -311,     0,     0,     0,     0,     0,     0,  -311,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -311,     0,     0,
       0,     0,     0,     0,     0,     0,  -311,     0,  -311,     0,
    -311,     0,  -311,     0,     0,  -311,     0,     0,     0,  -311,
       0,  -311,     0,     0,  -311
};

static const yytype_int16 yycheck[] =
{
       2,     2,    62,   110,   165,   552,   590,     2,    26,    62,
     181,   265,   620,   401,    62,   425,    67,   223,   492,   605,
     606,   579,   680,    25,   682,   611,   612,   632,   798,   608,
      25,     2,   833,   131,   516,    86,   290,   628,   886,   547,
     381,   792,   969,   828,     1,   851,   203,     1,    66,    32,
     101,   403,    24,     1,    25,    75,     1,    32,    32,    32,
      32,    46,     3,     3,    22,    32,     1,    32,    40,    41,
      42,   391,    74,     5,   523,   329,    78,    79,   894,    74,
     164,    66,    45,    78,    79,    47,    32,    32,    32,   102,
     261,    46,    56,    61,   117,    47,   116,    32,   100,     0,
      85,   122,   104,    74,    24,   100,    69,    78,    79,   104,
     119,    35,    32,    54,    54,   146,    61,   437,   438,    24,
      40,    41,    42,   146,    24,    56,    61,    75,   113,   100,
      85,    89,    32,   104,   518,    40,    41,    42,    47,    91,
      40,    41,    42,   174,    56,   119,   118,     1,   169,   117,
     170,   140,   119,   111,   119,   971,   169,    91,   113,   148,
     105,   545,   103,   483,   484,   174,    28,   124,   116,   489,
     105,   146,   117,   146,    56,  1023,  1103,    47,    40,    41,
      42,   164,   117,   632,   146,   109,   169,   778,   145,   146,
     229,   543,   962,    47,   169,   169,   169,   119,   118,   969,
     145,   173,   169,    31,   169,   119,   764,   171,   365,     2,
     145,  1059,  1060,   215,   146,   172,  1022,   171,   118,   164,
     215,   972,   170,   169,   169,   169,   265,   481,   482,   815,
     816,   817,    25,  1018,   169,   146,    47,   146,   170,    47,
     171,    27,   119,   146,   215,     3,    90,   146,   832,   119,
      94,   290,   593,   173,   425,   164,   576,   146,   740,   170,
     174,    18,    90,   109,    47,   174,    94,   269,   173,   172,
     124,   170,   274,   173,   269,   164,   146,  1125,    56,   274,
      91,    74,   380,    91,   863,    78,    79,   149,   796,    27,
     329,   145,   146,    79,   164,     3,    54,   174,   269,   146,
    1148,   163,   556,   274,   174,    91,     3,   100,    91,    47,
     164,   104,   140,    51,   277,  1085,   279,   145,  1119,   147,
     148,    78,    47,    20,   152,   172,   146,    56,   114,   535,
     146,   159,   171,  1103,   146,   496,    33,   199,    76,   167,
      97,    79,   402,    81,    11,   103,    54,   349,   349,   146,
     170,   146,   943,    91,   349,   112,   172,    54,    53,     2,
     172,   363,   146,   326,    83,   970,    63,    86,   363,   164,
     372,   109,   390,   146,   146,   172,   114,   372,   349,    74,
     398,    56,    25,   385,   124,    93,    56,   146,   172,    84,
     385,   597,   363,    46,   119,   103,   258,   259,   146,   172,
     172,   372,   884,   164,   146,   164,   103,   446,   270,   170,
     150,   146,   109,    66,   385,  1073,  1074,    78,    79,   470,
     471,   146,   215,   425,   172,   476,  1156,  1011,   479,   480,
     172,    74,    85,   991,   172,    78,    79,   172,   171,   146,
     146,   146,   481,   482,   926,  1031,  1032,   146,   310,   174,
     146,   164,    31,   146,   146,  1063,   930,   100,   171,    56,
     113,   104,   146,  1049,  1050,   172,   172,   172,   164,     2,
     488,   164,   164,   172,    90,   146,   269,   516,    94,   103,
     164,   274,   146,   146,   544,  1215,  1216,  1217,  1218,  1219,
    1220,   544,    25,   164,    56,   905,   544,   359,   886,   164,
     164,   164,   146,    56,   146,   507,   146,   509,    56,   174,
     164,    90,   507,   620,   509,    94,   170,   556,   569,   570,
     164,     9,   164,   164,   164,  1111,  1112,  1113,  1114,    56,
     171,    27,   173,   395,  1118,   397,   507,   164,   509,   401,
     579,    74,   862,   171,  1152,    78,    79,   174,   100,   164,
      50,    47,   164,    53,    56,    51,   349,   575,   878,   174,
     171,   140,   174,   425,   124,    56,   145,   100,     5,   148,
     363,   104,   215,   152,    74,    78,    79,    77,    27,   372,
      76,   583,   583,    79,    84,    81,    90,    56,   583,   607,
      94,   151,   385,   153,   154,    91,   156,   157,    47,   140,
      56,   921,    51,    56,   145,   925,    90,   148,   148,   109,
      94,   152,   583,   109,   169,  1162,    90,    90,   114,   145,
      94,    94,    56,    27,   944,   945,   269,    76,   490,   790,
      79,   274,    81,   291,   292,  1023,    31,   169,   601,   602,
     569,   570,    91,    47,   169,   164,   171,    51,   611,   612,
     171,   171,   173,   173,   171,   171,   518,   173,   163,   171,
     109,   173,    56,   164,  1211,   114,   164,   164,   988,   531,
     990,  1059,    76,    85,   164,    79,  1064,    81,   171,   541,
     173,   146,   215,   545,   171,   164,   173,    91,   171,   164,
     173,   169,   853,   164,   164,    90,   164,   145,   164,    94,
     172,   740,   146,   164,   171,   109,   349,   171,   171,   171,
     114,   762,   172,     9,   507,     3,   509,   171,    66,    79,
     363,   124,   169,   172,   164,   764,   148,   589,    69,   372,
     781,   124,   171,   146,   905,   124,   269,  1125,   600,   172,
     124,   274,   385,    34,    34,   140,   141,    66,   143,   144,
     145,   172,   147,   148,   149,   757,   124,   152,   620,   145,
      21,     3,   172,   158,   159,   172,   768,   162,   169,   771,
     165,   166,   167,   768,    31,    17,   771,   146,    20,   118,
     171,   176,   170,    25,   124,   105,   171,   148,   827,   828,
     583,   148,   148,    35,   148,   148,   148,   768,    27,   148,
     771,   148,   148,    45,   148,   148,   148,   140,   172,   164,
      52,   148,    54,   148,   148,   174,   349,    32,    47,   164,
     171,    63,    51,    34,   686,    34,   886,   145,   164,    71,
     363,   146,   892,    90,   164,   164,   164,    94,   170,   372,
     858,   901,  1096,  1097,   169,   884,    88,    76,   172,   172,
      79,   164,   385,   816,   817,    47,   172,    99,   860,   146,
     174,   103,    91,   865,   507,   860,   509,   174,   110,   170,
     865,   174,   874,   115,    34,   877,   171,   171,   164,   874,
     109,   164,   877,   140,   141,   114,   145,   926,   145,   860,
     147,   148,   164,   146,   865,   152,    31,   172,   174,    32,
      34,   172,   159,   874,   174,   174,   877,   172,   165,   166,
     167,   773,   164,   876,   172,   777,   164,   880,   163,   163,
     146,   109,   578,   991,    24,   787,   264,   372,   825,   365,
     389,   881,   338,   529,   984,   544,   905,   383,   304,   892,
     583,   901,   944,   945,   806,  1211,  1066,  1221,   852,   951,
    1214,   953,   991,  1060,   956,    90,   951,   531,   953,    94,
     686,   956,   980,  1023,   129,   526,    20,   777,    -1,  1140,
      -1,   930,    -1,    -1,   507,   768,   509,   979,   771,    31,
     951,    -1,   953,    -1,   979,   956,    -1,   989,    -1,    -1,
     852,    -1,    -1,    -1,   989,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,     2,    -1,   979,    -1,
     145,    -1,   147,   148,    -1,    -1,    -1,   152,   989,   881,
      -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,  1030,    25,
     165,   166,   167,    -1,    -1,  1030,    -1,  1039,    90,    -1,
      -1,  1148,    94,   905,  1039,  1152,    -1,    -1,    -1,    -1,
     583,    -1,    -1,    -1,    -1,    -1,    -1,  1096,  1097,  1030,
      -1,    -1,    -1,    -1,    -1,  1125,    -1,   860,  1039,  1240,
      -1,  1242,   865,    -1,  1245,  1246,    -1,    -1,    74,    -1,
      -1,   874,    78,    79,   877,    -1,  1049,  1050,   140,   141,
    1092,   143,   144,   145,    -1,   147,   148,  1092,    -1,    -1,
     152,    -1,    -1,    -1,   100,    -1,   158,   159,   104,    -1,
     162,    -1,    -1,   165,   166,   167,    -1,    -1,    -1,    -1,
      -1,  1092,   984,    27,    -1,   768,    -1,    -1,   771,    -1,
      -1,  1133,    -1,  1135,    -1,    -1,    -1,    -1,  1133,  1141,
    1135,    -1,  1144,    47,  1107,    -1,  1141,    51,    -1,  1144,
    1113,  1114,    -1,    -1,    -1,    -1,    -1,    -1,   951,    -1,
     953,    -1,  1133,   956,  1135,    -1,    -1,  1130,    -1,    -1,
    1141,  1173,    76,  1144,    -1,    79,    -1,  1179,  1173,    -1,
    1143,  1183,  1184,  1146,  1179,    -1,   979,    91,  1183,  1184,
      -1,    -1,    -1,  1195,    -1,    -1,   989,  1199,    -1,  1201,
    1195,  1063,  1173,     2,  1199,   109,  1201,    -1,  1179,    -1,
     114,    -1,  1183,  1184,    -1,    -1,    -1,   860,    -1,   215,
    1222,  1222,   865,    -1,  1195,    -1,    25,  1222,  1199,    -1,
    1201,   874,    -1,    -1,   877,   768,    -1,  1030,   771,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1039,    -1,    -1,    -1,
      -1,  1222,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1264,  1265,  1266,  1267,    -1,    -1,    -1,  1264,
    1265,  1266,  1267,   269,    -1,    74,    -1,    -1,   274,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1152,    -1,    -1,  1264,  1265,  1266,  1267,    -1,    -1,  1092,
      -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   951,    -1,
     953,    -1,    -1,   956,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,   860,    -1,    -1,
      -1,    -1,   865,    -1,    -1,    -1,   979,    -1,    -1,    -1,
    1133,   874,  1135,    -1,   877,    -1,   989,    -1,  1141,    -1,
      -1,  1144,    -1,   349,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   363,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    74,   372,    -1,    -1,    -1,
    1173,     2,    -1,    -1,    -1,    -1,  1179,  1030,    -1,   385,
    1183,  1184,    -1,    -1,    -1,    -1,  1039,    -1,    -1,    -1,
      -1,    -1,  1195,    -1,    25,   104,  1199,    -1,  1201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   215,    -1,   951,    -1,
     953,    -1,    -1,   956,    -1,    -1,    -1,    90,    -1,  1222,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   979,    -1,    -1,  1092,
      -1,    -1,    -1,    74,    -1,    -1,   989,    78,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     269,  1264,  1265,  1266,  1267,   274,    -1,   140,   141,   100,
     143,   144,   145,   104,   147,   148,    -1,    -1,    -1,   152,
    1133,    -1,  1135,    -1,    -1,    -1,   159,  1030,  1141,    -1,
      -1,  1144,   165,   166,   167,    -1,  1039,    -1,    -1,    -1,
      -1,   507,    -1,   509,    -1,    -1,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1173,    -1,     2,    -1,    -1,    -1,  1179,    -1,    -1,    -1,
    1183,  1184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     349,    -1,  1195,    -1,    -1,    25,  1199,    -1,  1201,  1092,
      -1,    -1,    -1,    -1,   363,    -1,    -1,    -1,    -1,    -1,
     269,    -1,    -1,   372,    -1,   274,    -1,    -1,    31,  1222,
      -1,    -1,    -1,    -1,    -1,    -1,   385,   583,    -1,    -1,
      -1,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,
    1133,    -1,  1135,    -1,    74,    -1,    -1,    -1,  1141,    -1,
      -1,  1144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1264,  1265,  1266,  1267,    -1,    -1,    -1,    -1,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,    90,    -1,    -1,
    1173,    94,    -1,    -1,    -1,    -1,  1179,    -1,   269,    -1,
    1183,  1184,    -1,   274,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1195,    -1,    -1,    -1,  1199,    -1,  1201,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   385,   140,   141,  1222,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   507,   162,
     509,    -1,   165,   166,   167,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,   349,    -1,
      -1,  1264,  1265,  1266,  1267,    -1,    -1,    86,    -1,    -1,
      -1,    -1,   363,    -1,     1,   215,     3,    -1,    -1,    -1,
      -1,   372,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,   385,    -1,    -1,    -1,    25,    -1,
      -1,    -1,   768,    -1,    -1,   771,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,   583,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,   269,
      -1,    -1,    -1,    -1,   274,    -1,    63,    -1,   507,    -1,
     509,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      22,    78,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,
      97,    43,    99,    -1,   101,    -1,   103,    -1,    50,   106,
      -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,   860,    -1,    -1,    -1,    -1,   865,
      72,    -1,    -1,    -1,    -1,    77,    -1,    79,   874,   349,
     229,   877,    -1,    -1,    -1,    -1,   507,    89,   509,    91,
      -1,    -1,    -1,   363,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   372,     3,    -1,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,   171,   385,   265,    17,    18,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
       1,   290,    -1,    -1,     5,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,   951,    -1,   953,    -1,    -1,
     956,    22,   583,    63,    -1,    26,    27,    -1,    -1,   768,
      70,    71,   771,    -1,    35,    -1,    37,    38,    78,    -1,
     329,    -1,    43,   979,    -1,    -1,    -1,    -1,    88,    50,
      -1,    -1,    53,   989,    -1,    -1,    -1,    97,    -1,    99,
      -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    72,   112,    74,     5,   115,    77,    -1,    79,    -1,
      81,    -1,    83,    84,    -1,    86,    -1,    -1,    89,    -1,
      91,    22,    -1,    -1,  1030,    26,    27,   507,    -1,   509,
      -1,    -1,    -1,  1039,    35,    -1,    37,    38,   109,    -1,
     111,    -1,    43,   114,    -1,    -1,    -1,    -1,    -1,    50,
      -1,   860,    -1,    -1,    -1,    -1,   865,    -1,    -1,   768,
      -1,   171,   771,    -1,    -1,   874,    -1,    -1,   877,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    86,  1092,    -1,    89,    -1,
      91,    31,    -1,   164,    -1,    -1,    -1,    -1,    -1,   170,
      -1,    -1,    -1,   583,    -1,    -1,    -1,    -1,   109,    -1,
     111,   470,   471,   114,    -1,    -1,    -1,   476,    -1,    -1,
     479,   480,   481,   482,    -1,    -1,    -1,  1133,    -1,  1135,
      -1,    -1,    -1,    -1,    -1,  1141,    -1,   768,  1144,    -1,
     771,    -1,   951,    -1,   953,    -1,    -1,   956,    -1,    -1,
      90,   860,    -1,    -1,    94,    -1,   865,   516,    -1,    -1,
      -1,    -1,    -1,   164,    -1,   874,    -1,  1173,   877,   170,
     979,    -1,    -1,  1179,    -1,    -1,    -1,  1183,  1184,    -1,
     989,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1195,
      -1,    -1,    -1,  1199,    -1,  1201,    -1,   556,    -1,    -1,
     140,   141,    -1,    -1,   144,   145,    -1,   147,   148,    -1,
     569,   570,   152,    -1,    -1,    -1,  1222,    -1,    -1,   159,
     579,  1030,    -1,    -1,    -1,   165,   166,   167,    -1,   860,
    1039,    -1,    -1,    -1,   865,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   951,   874,   953,    -1,   877,   956,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1264,  1265,
    1266,  1267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     979,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     989,    -1,    -1,  1092,    -1,    -1,    -1,    -1,   768,    -1,
      -1,   771,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     951,  1030,   953,    -1,  1133,   956,  1135,    -1,    -1,    -1,
    1039,    -1,  1141,    -1,    -1,  1144,   168,    -1,    -1,   171,
      -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,   979,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   989,    -1,
      -1,    -1,    -1,    -1,  1173,    -1,    -1,    -1,    -1,    -1,
    1179,    -1,    -1,    -1,  1183,  1184,    -1,    -1,    -1,    -1,
     860,   740,    -1,  1092,    -1,   865,  1195,    -1,    -1,    -1,
    1199,    -1,  1201,    -1,   874,    -1,    -1,   877,    -1,  1030,
      -1,    -1,    -1,   762,    -1,   764,    -1,    -1,  1039,    -1,
      -1,    -1,    -1,  1222,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   781,    -1,  1133,    -1,  1135,    -1,    -1,    -1,
      -1,    -1,  1141,    -1,    -1,  1144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1264,  1265,  1266,  1267,    -1,
      -1,  1092,    -1,    -1,  1173,    -1,    -1,    -1,    -1,    -1,
    1179,   951,    -1,   953,  1183,  1184,   956,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,  1195,    -1,    -1,    -1,
    1199,    -1,  1201,    -1,    -1,    -1,    -1,    -1,    -1,   979,
      -1,    -1,  1133,    -1,  1135,    -1,    69,    -1,    -1,   989,
    1141,    -1,    -1,  1144,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   884,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,  1173,    -1,    -1,    17,   109,    -1,  1179,    -1,
    1030,    -1,  1183,  1184,    26,  1264,  1265,  1266,  1267,  1039,
      -1,    33,    -1,    -1,  1195,    -1,    -1,   926,  1199,    -1,
    1201,    -1,    -1,    45,   137,    47,    48,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      62,  1222,    -1,    -1,    66,    67,    68,    69,    70,    -1,
      -1,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    83,  1092,    -1,    86,    -1,    88,    -1,    90,    -1,
      -1,    93,    94,    -1,    96,    -1,    -1,    -1,   100,   101,
     102,    -1,   991,  1264,  1265,  1266,  1267,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       5,    -1,    -1,  1133,    -1,  1135,    -1,   129,   221,   131,
      -1,  1141,    -1,    -1,  1144,    -1,    -1,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    -1,    -1,  1173,    -1,    50,    -1,   260,   261,  1179,
      -1,    -1,   174,  1183,  1184,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   277,  1195,   279,    72,    -1,  1199,
      -1,  1201,    77,   286,    79,   197,    81,    -1,    83,   201,
     202,    86,    -1,    -1,    89,    -1,    91,  1096,  1097,    -1,
      -1,    -1,  1222,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,   111,   229,    -1,   114,
      -1,    -1,    -1,   326,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,  1264,  1265,  1266,  1267,    25,    -1,
      -1,    -1,    -1,   265,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,   164,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,   290,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,   301,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,   316,   317,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,   329,    -1,    -1,
      97,   333,    99,    -1,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,   351,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   361,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   373,    -1,   375,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   389,   390,   391,
      -1,    -1,    -1,    -1,    -1,    -1,   398,    -1,    -1,    -1,
      -1,    -1,   169,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   434,   435,   436,   437,   438,   439,    -1,   441,
     442,   443,   444,   445,    -1,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,    -1,    -1,    -1,    -1,   559,    -1,   470,   471,
      -1,    -1,    -1,    -1,   476,    -1,    -1,   479,   480,   481,
     482,   483,   484,    -1,    -1,    -1,   488,   489,    -1,    -1,
     492,    -1,    -1,    -1,    -1,    -1,   498,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   509,   601,   602,
     512,    -1,    -1,    -1,   516,    -1,    -1,    -1,   611,   612,
      -1,   523,    -1,   525,    -1,    -1,    -1,   529,    -1,    -1,
     532,   533,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   544,    -1,    -1,   547,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   556,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   566,    -1,    -1,   569,   570,    -1,
      -1,    -1,    -1,   575,   576,    -1,    -1,   579,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   590,   591,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   604,    -1,    -1,   607,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     632,    -1,     3,     4,    -1,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    24,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    42,    -1,    44,    45,   677,    -1,    -1,    49,   772,
      -1,    52,   684,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,   806,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,   816,   817,    -1,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,   739,   740,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     762,    -1,   764,   765,   766,    -1,    -1,    -1,    -1,   140,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   781,
      -1,    -1,    -1,   876,    -1,    -1,    -1,   880,    -1,   882,
      -1,    -1,   163,    -1,   796,    -1,    -1,    -1,   800,    -1,
     171,    -1,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   825,    -1,    -1,    -1,   829,    -1,   831,
     832,   833,    -1,   835,    -1,   837,    -1,    -1,   840,   841,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   854,    -1,    -1,    -1,   858,    -1,    -1,    -1,
     862,   863,    -1,   865,    -1,    -1,    -1,     5,    -1,   871,
     872,    -1,   874,    -1,    -1,   877,   878,    -1,    -1,    -1,
      -1,    -1,   884,    -1,    22,    -1,    -1,   889,    26,    27,
      -1,    -1,    -1,    -1,   987,    -1,    -1,    35,    -1,    37,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,   921,
     922,   923,   924,   925,   926,    -1,    -1,    -1,   930,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    77,
      -1,    79,   944,   945,    -1,    83,    -1,    -1,    86,    -1,
      -1,    89,   954,    91,    -1,    -1,  1049,  1050,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   970,    -1,
      -1,   109,   974,   111,    -1,    -1,   114,    -1,   980,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   988,    -1,   990,   991,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1011,
      -1,    -1,    -1,    -1,  1107,    -1,  1018,    -1,  1020,    -1,
    1113,  1114,    -1,    -1,    -1,  1027,   164,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1130,  1040,    -1,
      -1,    -1,    -1,    -1,    -1,  1047,    -1,  1140,    -1,    -1,
    1143,    -1,    -1,  1146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1068,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,  1080,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1181,    -1,
      17,    18,    -1,    20,  1096,  1097,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,  1109,    -1,    -1,
      -1,    -1,    39,    -1,    -1,  1117,  1118,  1119,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,
      -1,  1133,    -1,  1135,    -1,    -1,    63,  1139,    -1,  1141,
      -1,    -1,  1144,    70,    71,    -1,    -1,  1240,    -1,  1242,
      -1,    78,  1245,  1246,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,  1166,  1167,    -1,  1169,  1170,    -1,
      97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,   112,    -1,  1189,   115,    -1,
    1192,    -1,    -1,    -1,  1196,    -1,    -1,    -1,    -1,  1201,
    1202,    -1,    -1,  1205,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1227,  1228,  1229,  1230,  1231,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1264,  1265,  1266,  1267,  1268,  1269,  1270,  1271,
       0,     1,    -1,     3,     4,     5,     6,     7,     8,     9,
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
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    18,    19,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    -1,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,   169,   170,   171,    -1,   173,    -1,     1,
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
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,     1,
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
      -1,   169,    -1,   171,     1,   173,     3,     4,   176,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    24,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    42,    -1,    44,    45,    -1,
      47,    -1,    49,    -1,    51,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    78,    79,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,   109,   110,    -1,   112,    -1,   114,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,   172,   173,    -1,     1,   176,
       3,     4,    -1,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    42,
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
     173,     3,     4,   176,     6,     7,     8,     9,    10,    -1,
      12,    -1,    14,    -1,    -1,    17,    18,    19,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    55,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,   124,   125,   126,   127,   128,   129,    -1,    -1,
     132,   133,   134,   135,    -1,   137,   138,    -1,   140,   141,
     142,   143,   144,   145,    -1,   147,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,    -1,   171,
      -1,    -1,     3,     4,   176,     6,     7,     8,     9,    10,
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
     161,   162,   163,    -1,   165,   166,   167,   168,   169,    -1,
     171,    -1,    -1,    -1,    -1,   176,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    -1,
      17,    18,    -1,    20,    -1,    22,    23,    -1,    25,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    60,    -1,    62,    63,    64,    65,    -1,
      67,    68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
      -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,    -1,     3,     4,   176,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    47,    -1,    49,    -1,    51,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    78,    79,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,   109,   110,    -1,   112,    -1,   114,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,    -1,     3,     4,
     176,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    72,    -1,    -1,
      -1,    -1,    -1,    78,    79,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,   112,    -1,   114,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,     3,
       4,   176,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    42,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,   111,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
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
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,     3,     4,   176,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    -1,    94,    -1,    -1,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,   146,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,   164,    -1,    -1,   167,    -1,   169,   170,   171,
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
     101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,
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
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
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
      99,   100,   101,    -1,   103,    -1,    -1,   106,   107,   108,
      -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,   172,    -1,     3,     4,   176,     6,     7,
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
      -1,    -1,   106,   107,   108,    -1,   110,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,   172,    -1,
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
     172,    -1,     3,     4,   176,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    69,    70,
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
     140,   141,   142,   143,   144,    -1,   146,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,   169,
     170,   171,    -1,    -1,     3,     4,   176,     6,     7,     8,
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
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,   112,    -1,   114,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
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
     106,   107,   108,    -1,   110,    -1,   112,    -1,    -1,   115,
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
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,    94,
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
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,    -1,   174,    -1,   176,     3,     4,    -1,
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
     146,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,   172,    -1,     3,     4,
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
      -1,   106,    -1,    -1,    -1,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,   174,
      -1,   176,     3,     4,    -1,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
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
      -1,   121,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
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
     169,    -1,   171,    -1,    -1,   174,    -1,   176,     3,     4,
      -1,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
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
      -1,    -1,   167,    -1,   169,   170,   171,    -1,    -1,     3,
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
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,
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
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,     4,    -1,     6,     7,
       8,     9,    10,    -1,   158,    -1,    14,   161,   162,   163,
      18,    -1,    -1,   167,    22,   169,    24,   171,    -1,    27,
      -1,    -1,   176,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    42,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    -1,    -1,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      78,    79,    80,    81,    82,    -1,    -1,    -1,    -1,    87,
      -1,    89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    97,
      98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,   172,   173,    -1,     4,   176,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    22,    -1,    24,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    42,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    -1,    -1,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    79,    80,    81,    82,    -1,    -1,    -1,    -1,
      87,    -1,    89,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      97,    98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      12,    -1,   149,    -1,    -1,    17,    18,    19,    20,    -1,
      -1,   158,    -1,    25,   161,   162,   163,    -1,    -1,    -1,
     167,    33,   169,    35,   171,    -1,   173,    39,    -1,   176,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    -1,    99,    -1,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,    -1,    -1,
     132,   133,   134,   135,    -1,   137,   138,    -1,   140,   141,
     142,   143,   144,   145,    -1,   147,    -1,    -1,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
     162,    -1,    -1,   165,   166,   167,   168,     3,     4,   171,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    24,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    42,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,    -1,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,     3,     4,
      -1,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    24,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    42,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,     3,    -1,    -1,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,   140,    -1,   142,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,   163,    -1,
      -1,    -1,    63,    -1,   169,    -1,   171,    -1,   173,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    99,    -1,
     101,     3,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      12,   112,    -1,    -1,   115,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
     171,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    -1,    99,    -1,   101,
      -1,   103,    -1,    -1,    -1,    -1,    -1,   109,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,    -1,    -1,
     132,   133,   134,   135,    -1,   137,   138,    -1,   140,   141,
     142,   143,   144,   145,    -1,   147,    -1,    -1,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,     3,
     162,    -1,    -1,   165,   166,   167,   168,   169,    12,    -1,
      -1,    -1,    -1,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    99,    -1,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,    -1,    -1,   132,   133,
     134,   135,    -1,   137,   138,    -1,   140,   141,   142,   143,
     144,   145,    -1,   147,    -1,    -1,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,     5,   162,    -1,
      -1,   165,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    -1,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    72,    26,    27,    -1,    -1,    77,
      -1,    79,    -1,    81,    35,    83,    37,    38,    86,    12,
      -1,    89,    43,    91,    -1,    -1,    19,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    86,    -1,    -1,    89,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
     111,    -1,    -1,   114,    -1,    -1,   164,    90,    -1,    12,
      -1,    94,   170,    -1,    -1,    -1,    19,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,   129,    -1,    -1,   132,
     133,   134,   135,   164,   137,   138,    -1,   140,   141,   170,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    90,    -1,    12,
      -1,    94,    -1,   176,    -1,    -1,    19,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,   129,    -1,    -1,   132,
     133,   134,   135,    -1,   137,   138,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    90,    -1,    12,
      -1,    94,    -1,   176,    -1,    -1,    19,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,   129,    -1,    -1,   132,
     133,   134,   135,    -1,   137,   138,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    90,    -1,    12,
      -1,    94,    -1,   176,    -1,    -1,    19,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,   129,    -1,    -1,   132,
     133,   134,   135,    -1,   137,   138,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    90,    -1,    12,
      -1,    94,    -1,   176,    -1,    -1,    19,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,   129,    -1,    -1,   132,
     133,   134,   135,    -1,   137,   138,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    90,    -1,    -1,
      -1,    94,    -1,   176,    -1,    -1,    -1,    -1,    12,   102,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
     123,   124,   125,   126,   127,   128,   129,    -1,    -1,   132,
     133,   134,   135,    47,   137,   138,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    -1,    -1,    -1,
      -1,    -1,    12,   176,    -1,    -1,    90,    -1,    -1,    19,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,    47,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
      90,   165,   166,   167,    94,   169,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      90,    -1,    31,   123,    94,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,    73,   165,   166,   167,    -1,   169,
     140,   141,    -1,   143,   144,   145,   176,   147,   148,   149,
      -1,    90,   152,   153,   154,    94,   156,   157,   158,   159,
      -1,    -1,   162,    12,    13,   165,   166,   167,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,    31,    -1,   123,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    73,   164,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,    90,    12,    -1,    -1,    94,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      90,    -1,    12,    -1,    94,    -1,    -1,   176,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,   169,
      90,    -1,    12,    -1,    94,    -1,   176,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,   169,
      90,    -1,    12,    -1,    94,    -1,   176,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,   169,
      90,    12,    -1,    -1,    94,    -1,   176,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,   176,    -1,    -1,    -1,
      -1,   102,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,   123,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,    -1,    73,   165,   166,   167,    -1,   169,    -1,
      -1,    -1,    -1,    -1,    -1,   176,    12,    -1,    -1,    -1,
      90,    -1,    -1,    19,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      31,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,    90,   165,   166,   167,    94,    -1,
      -1,    12,    13,    -1,    -1,    -1,   176,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,   119,   165,
     166,   167,   123,    -1,    -1,    -1,    -1,    -1,   174,    90,
     176,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   123,    -1,   165,   166,   167,    -1,    -1,    -1,
      12,    -1,    -1,   174,    -1,   176,    -1,    19,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    31,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,   176,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      12,    -1,    94,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   123,    -1,   165,   166,   167,    -1,   169,    90,    12,
      -1,    -1,    94,    -1,   176,    -1,    19,    -1,   140,   141,
     102,   143,   144,   145,    -1,   147,   148,   149,    31,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   123,   164,   165,   166,   167,    -1,    -1,    -1,    12,
      13,    -1,    -1,    -1,   176,    -1,    19,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    31,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    90,    -1,    -1,
      12,    94,    -1,    -1,   176,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     123,    -1,   165,   166,   167,    -1,    -1,    -1,    90,   172,
      -1,    -1,    94,   176,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     122,   123,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    12,    -1,   165,   166,   167,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    12,    -1,    94,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   123,    -1,   165,   166,   167,    -1,    -1,    90,
      12,    13,    -1,    94,    -1,   176,    -1,    19,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    31,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   123,    -1,   165,   166,   167,    -1,    -1,    -1,
      12,   172,    -1,    -1,    -1,   176,    -1,    19,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    31,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    90,    -1,
      -1,   172,    94,    -1,    -1,   176,    -1,    -1,    12,    13,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   123,    -1,   165,   166,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,    -1,    90,    -1,   140,   141,
      94,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    12,    -1,   165,   166,   167,    -1,    -1,    19,   123,
     172,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    12,
      -1,   165,   166,   167,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,   140,
     141,    94,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,    -1,   164,   165,   166,   167,    -1,    -1,   122,
     123,    -1,    -1,    -1,    -1,   176,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     123,    -1,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      12,    -1,   165,   166,   167,    -1,    -1,    19,    -1,    -1,
       1,   174,     3,   176,     5,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      12,    13,    33,    -1,    -1,    -1,    -1,    19,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    31,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    90,    70,
      71,    12,    94,    -1,    -1,    -1,    -1,    78,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    97,    -1,    99,    -1,
     101,   123,   103,    -1,    -1,   106,    -1,    -1,    90,   110,
      -1,   112,    94,    -1,   115,    -1,    -1,    -1,   140,   141,
      61,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   123,    -1,   165,   166,   167,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,   176,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    -1,   123,   165,   166,   167,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,   176,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    31,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,   176,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    12,    -1,   102,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,   140,   141,    94,   143,   144,   145,
      -1,   147,   148,   149,   102,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
     176,    -1,    90,    12,    -1,    -1,    94,    -1,    -1,    -1,
      19,    -1,   140,   141,   102,   143,   144,   145,    -1,   147,
     148,   149,    31,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   123,    -1,   165,   166,   167,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,   176,    -1,
      19,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    31,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    90,    -1,    12,    -1,    94,    -1,    -1,   176,    -1,
      19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,   140,   141,   102,   143,   144,   145,    -1,   147,   148,
     149,    31,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   123,    -1,   165,   166,   167,    -1,
      -1,    90,    12,    -1,    -1,    94,    -1,   176,    -1,    19,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    31,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   123,    -1,   165,   166,   167,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,   176,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,   176,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,    -1,   162,   123,    -1,   165,   166,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      31,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,    22,   165,   166,   167,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,   176,    35,    -1,    37,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    90,
      -1,    -1,    -1,    94,    72,    -1,    74,    -1,    -1,    77,
      -1,    79,    -1,    -1,    -1,    83,    84,    85,    86,    -1,
      -1,    89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,     3,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,   176,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,   164,    -1,    -1,    -1,
      39,   169,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    91,    -1,    -1,     3,    -1,     5,    97,    -1,
      99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,    17,
      18,   110,    20,   112,    -1,   114,   115,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    17,    18,
      78,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    97,
      39,    99,    -1,   101,    -1,   103,    45,    -1,   106,    -1,
      49,    -1,   110,    52,   112,    54,    -1,   115,    -1,    -1,
      -1,    -1,    -1,     3,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    17,    18,    78,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    97,    39,
      99,    -1,   101,    -1,   103,    45,    -1,   106,    -1,    49,
      -1,   110,    52,   112,    54,    -1,   115,    -1,    -1,    -1,
      -1,    -1,     3,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    17,    18,    78,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    97,    39,    99,
      -1,   101,    -1,   103,    45,    -1,   106,    -1,    49,    -1,
     110,    52,   112,    54,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    99,    -1,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,   112,    -1,    -1,   115
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
     124,   205,   297,   202,   348,   164,   171,   185,   171,   219,
     331,   332,     1,   145,   338,    47,   146,   185,   224,   146,
     224,    13,   171,   171,   224,   328,   348,   229,   229,    47,
      91,   306,   307,   172,   146,   171,   219,   145,   164,   372,
     372,   122,   287,   164,   169,   224,   171,   348,   164,   372,
     255,   255,   164,   170,   170,   183,   146,   170,   372,   146,
     146,   172,   172,     9,   372,   173,   241,   242,     3,   171,
     196,     1,   170,   206,   213,   214,   372,   208,   372,    66,
      36,    73,   164,   266,   268,   109,   234,   236,   290,   185,
      79,   245,   372,   121,   174,   243,   342,   343,   344,   372,
     383,   243,   372,   219,   248,   249,   219,   342,   169,     1,
     192,   219,   272,   275,   172,   302,   304,   305,   310,    47,
      91,     1,   145,   336,   337,    47,    91,     1,     3,    12,
      17,    19,    20,    25,    45,    52,    54,    55,    63,    71,
      88,    99,   103,   110,   115,   140,   141,   142,   143,   144,
     145,   147,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   162,   165,   166,   167,   168,   171,   219,   294,
     295,   296,   297,   351,   124,   323,   146,   164,   164,   164,
     372,   372,   372,   243,   372,   243,   372,   372,   372,   372,
     372,   372,   372,     3,    20,    33,    63,   103,   109,   220,
     372,   372,   372,   372,   372,   372,   372,   372,   372,   372,
     372,   372,   372,   372,   372,   372,   372,    69,   374,   374,
     374,   374,   374,   348,   348,   243,   372,   243,   372,   102,
     171,   342,   243,   372,   219,     1,    27,    47,    51,    76,
      79,    91,   109,   114,   172,   362,   363,   364,   372,   388,
     329,   372,   124,   172,   146,   124,   124,   185,    34,   185,
     372,    34,   372,    66,   172,   349,   332,   146,   172,   223,
     372,   124,   333,   372,   330,   262,   219,   324,   372,   372,
     325,   172,   219,   310,   283,   332,   145,   340,   341,   339,
     288,   204,     1,   260,   348,   172,    21,   256,   170,   172,
     172,   346,   374,   172,   346,   342,   243,   372,   195,   172,
     197,   198,   350,   170,   170,   146,   164,    13,   148,   146,
     164,    13,    36,    73,   219,   140,   141,   142,   143,   144,
     158,   162,   167,   207,   296,   297,   298,   372,   207,   209,
     268,   169,   237,   325,   118,   219,   224,   224,   171,   372,
     174,   361,    47,   146,   164,   174,   345,   361,   342,   174,
     345,   122,   146,   174,    32,   224,   361,    32,    47,   224,
     361,   146,   172,   164,   171,     1,   269,   170,   275,   124,
     146,   170,   146,   172,   338,     1,   223,   334,   335,   105,
     311,   171,   300,   372,   140,   148,   300,   300,   372,   322,
     171,   173,   164,   164,   164,   164,   164,   164,   172,   174,
     349,    47,   174,    32,    32,    47,   171,   172,    47,    91,
     146,   172,    17,    20,    25,    45,    52,    63,    71,    88,
      99,   110,   115,   351,    90,    90,   164,   374,   350,   372,
     372,    34,   185,    34,    34,   185,   102,   205,   219,   172,
     172,   331,   338,    69,   374,   219,   172,   172,   329,   341,
     145,   284,   172,   339,   150,   299,   333,   372,   170,    75,
     116,   170,   261,   172,   171,   205,   219,   257,    47,   174,
      47,   146,   172,   213,   220,    17,    18,    20,    25,    45,
      49,    52,    71,    78,    88,    97,    99,   101,   110,   112,
     115,   169,   212,   298,   372,   372,   207,   209,   146,   164,
      13,   164,   169,   269,   321,   329,   372,    47,   342,   185,
     174,   243,   372,   174,   345,   185,   372,   164,   174,   174,
     146,   174,   372,   174,   372,   372,   224,    44,   372,   243,
     372,   224,   249,   205,   349,   170,    83,    86,   170,   184,
     192,   226,   372,   273,   274,   304,   311,    61,   117,   315,
     302,   303,   172,   295,   297,   172,   174,   172,   243,   372,
      44,   243,   372,   349,   363,   338,   351,   351,   185,   185,
     372,   185,    34,   164,   164,   333,   171,   171,   164,   299,
     333,   334,   311,   341,   372,   185,   239,   342,   257,   145,
     205,   243,   372,   243,   372,   198,   207,    13,    36,    73,
     164,   164,   298,   372,   372,   269,   170,   164,   164,   164,
     372,   172,   185,   174,   361,   174,   361,   174,   185,   122,
     372,    32,   224,   361,    32,   224,   361,   172,   192,   226,
     226,   275,   192,   315,     3,    54,    93,   103,   316,   317,
     318,   372,   291,   172,   300,   300,   174,   174,   372,    32,
      32,   172,   333,    34,   185,   348,   348,   299,   333,    32,
     224,   172,   372,   174,   174,   170,   372,   207,   209,    13,
     170,   224,    44,   185,   372,   174,    44,   185,   372,   174,
     372,   102,    44,   372,   224,    44,   372,   224,   164,   226,
     275,   278,   318,   117,   146,   124,   151,   153,   154,   156,
     157,    61,    32,   164,   204,   312,   313,    44,    44,   102,
      44,    44,   185,   172,   172,   185,   205,   164,   164,   372,
     372,   185,   372,   185,   372,   372,   372,   313,   372,   317,
     318,   318,   318,   318,   318,   318,   316,   183,   372,   372,
     372,   372,   163,   163,   102,   102,   102,   102,   102,   102,
     102,   102,   372,   372,   372,   372
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
     340,   341,   341,   342,   342,   343,   343,   343,   344,   344,
     345,   345,   346,   347,   347,   348,   348,   349,   349,   350,
     350,   351,   351,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   353,   354,   354,   354,   354,   354,
     354,   354,   355,   356,   356,   356,   356,   356,   356,   356,
     357,   358,   359,   359,   359,   359,   359,   359,   359,   360,
     360,   361,   361,   361,   361,   362,   362,   363,   363,   363,
     364,   364,   364,   364,   364,   364,   364,   364,   364,   364,
     365,   365,   365,   365,   365,   366,   366,   366,   366,   366,
     367,   368,   368,   368,   368,   368,   368,   369,   370,   371,
     371,   372,   372,   372,   372,   372,   372,   372,   372,   372,
     372,   372,   372,   372,   372,   372,   372,   372,   373,   373,
     374,   374,   374,   375,   375,   375,   375,   376,   376,   376,
     376,   376,   377,   377,   377,   378,   378,   378,   378,   378,
     378,   379,   379,   379,   379,   380,   380,   381,   381,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   383,   383,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   384,   384,
     385,   385,   385,   385,   385,   385,   385,   386,   386,   386,
     386,   387,   387,   387,   387,   388,   388,   388,   388,   388,
     388,   388
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
       2,     0,     1,     1,     3,     2,     3,     2,     2,     3,
       1,     2,     1,     3,     3,     0,     1,     1,     3,     3,
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
       1,     1,     1,     1,     1,     3,     4,     4,     5,     4,
       5,     5,     6,     4,     5,     3,     5,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     1
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
#line 5997 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 690 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6003 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 691 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6009 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 698 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6017 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 706 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6025 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 710 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6034 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 720 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6042 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 725 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6050 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 732 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6056 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 733 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6062 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 734 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6068 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 735 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6074 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 736 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6080 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 737 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6086 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 738 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6092 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 739 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6098 "bison-chpl-lib.cpp"
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
#line 6120 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: try_stmt  */
#line 758 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6126 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: return_stmt  */
#line 759 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6132 "bison-chpl-lib.cpp"
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
#line 6144 "bison-chpl-lib.cpp"
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
#line 6157 "bison-chpl-lib.cpp"
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
#line 6170 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 787 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6178 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 791 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6189 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 798 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6198 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 805 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6204 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 806 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6210 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 807 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6216 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 808 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6222 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 809 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6228 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 810 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6234 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 811 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6240 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 813 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6246 "bison-chpl-lib.cpp"
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
#line 6270 "bison-chpl-lib.cpp"
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
#line 6282 "bison-chpl-lib.cpp"
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
#line 6303 "bison-chpl-lib.cpp"
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
#line 6323 "bison-chpl-lib.cpp"
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
#line 6344 "bison-chpl-lib.cpp"
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
#line 6365 "bison-chpl-lib.cpp"
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
#line 6385 "bison-chpl-lib.cpp"
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
#line 6407 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 952 "chpl.ypp"
  {
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 6415 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 973 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6423 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 980 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6432 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 987 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6440 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 991 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6451 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 1001 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6457 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 1003 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6465 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 1007 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6473 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 1014 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6482 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 1019 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6492 "bison-chpl-lib.cpp"
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
#line 6514 "bison-chpl-lib.cpp"
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
#line 6535 "bison-chpl-lib.cpp"
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
#line 6556 "bison-chpl-lib.cpp"
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
#line 6575 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1104 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6581 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1105 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6588 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1107 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6595 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1112 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6601 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1113 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6608 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1119 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6618 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1137 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6627 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1142 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6636 "bison-chpl-lib.cpp"
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
#line 6649 "bison-chpl-lib.cpp"
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
#line 6668 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1177 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6674 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1178 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6680 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1183 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6688 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1187 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6696 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1191 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6705 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1196 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6713 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1200 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6721 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1204 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6730 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1212 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6739 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1217 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6749 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1223 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6758 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1228 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6769 "bison-chpl-lib.cpp"
    break;

  case 90: /* opt_only_ls: %empty  */
#line 1236 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6775 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: renames_ls  */
#line 1237 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6781 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1242 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6790 "bison-chpl-lib.cpp"
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
#line 6802 "bison-chpl-lib.cpp"
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
#line 6815 "bison-chpl-lib.cpp"
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
#line 6827 "bison-chpl-lib.cpp"
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
#line 6840 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1283 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6849 "bison-chpl-lib.cpp"
    break;

  case 114: /* import_expr: expr  */
#line 1318 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6857 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr TDOT all_op_name  */
#line 1322 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6866 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1327 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6875 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TAS ident_use  */
#line 1332 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6885 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1338 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6895 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_ls: import_expr  */
#line 1346 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6901 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_ls TCOMMA import_expr  */
#line 1347 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6907 "bison-chpl-lib.cpp"
    break;

  case 121: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1352 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6917 "bison-chpl-lib.cpp"
    break;

  case 122: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1361 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6925 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1365 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6933 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1369 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6941 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1373 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6949 "bison-chpl-lib.cpp"
    break;

  case 126: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1377 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6957 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1381 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6965 "bison-chpl-lib.cpp"
    break;

  case 128: /* opt_label_ident: %empty  */
#line 1389 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6971 "bison-chpl-lib.cpp"
    break;

  case 129: /* opt_label_ident: TIDENT  */
#line 1390 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6977 "bison-chpl-lib.cpp"
    break;

  case 130: /* opt_label_ident: TINIT  */
#line 1391 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6983 "bison-chpl-lib.cpp"
    break;

  case 139: /* ident_use: TIDENT  */
#line 1417 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6989 "bison-chpl-lib.cpp"
    break;

  case 140: /* ident_use: TINIT  */
#line 1418 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6995 "bison-chpl-lib.cpp"
    break;

  case 141: /* ident_use: TTHIS  */
#line 1419 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7001 "bison-chpl-lib.cpp"
    break;

  case 160: /* scalar_type: TBOOL  */
#line 1445 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7007 "bison-chpl-lib.cpp"
    break;

  case 161: /* scalar_type: TENUM  */
#line 1446 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7013 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TINT  */
#line 1447 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7019 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TUINT  */
#line 1448 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7025 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TREAL  */
#line 1449 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7031 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TIMAG  */
#line 1450 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7037 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TCOMPLEX  */
#line 1451 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7043 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TBYTES  */
#line 1452 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7049 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TSTRING  */
#line 1453 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7055 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TLOCALE  */
#line 1454 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7061 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TNOTHING  */
#line 1455 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7067 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TVOID  */
#line 1456 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7073 "bison-chpl-lib.cpp"
    break;

  case 175: /* do_stmt: TDO stmt  */
#line 1469 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7079 "bison-chpl-lib.cpp"
    break;

  case 176: /* do_stmt: block_stmt  */
#line 1470 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7085 "bison-chpl-lib.cpp"
    break;

  case 177: /* return_stmt: TRETURN TSEMI  */
#line 1475 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7096 "bison-chpl-lib.cpp"
    break;

  case 178: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1482 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7107 "bison-chpl-lib.cpp"
    break;

  case 179: /* class_level_stmt: TSEMI  */
#line 1492 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7115 "bison-chpl-lib.cpp"
    break;

  case 180: /* class_level_stmt: inner_class_level_stmt  */
#line 1496 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7125 "bison-chpl-lib.cpp"
    break;

  case 181: /* $@1: %empty  */
#line 1501 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7132 "bison-chpl-lib.cpp"
    break;

  case 182: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1503 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7141 "bison-chpl-lib.cpp"
    break;

  case 183: /* $@2: %empty  */
#line 1507 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7148 "bison-chpl-lib.cpp"
    break;

  case 184: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1509 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7157 "bison-chpl-lib.cpp"
    break;

  case 192: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1527 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7166 "bison-chpl-lib.cpp"
    break;

  case 193: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1532 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7175 "bison-chpl-lib.cpp"
    break;

  case 194: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1537 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7184 "bison-chpl-lib.cpp"
    break;

  case 195: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1542 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7192 "bison-chpl-lib.cpp"
    break;

  case 196: /* forwarding_decl_start: TFORWARDING  */
#line 1549 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7201 "bison-chpl-lib.cpp"
    break;

  case 197: /* extern_or_export: TEXTERN  */
#line 1556 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7207 "bison-chpl-lib.cpp"
    break;

  case 198: /* extern_or_export: TEXPORT  */
#line 1557 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7213 "bison-chpl-lib.cpp"
    break;

  case 199: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1562 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7223 "bison-chpl-lib.cpp"
    break;

  case 200: /* extern_export_kind_inner: TTYPE  */
#line 1575 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7232 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1584 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7242 "bison-chpl-lib.cpp"
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
#line 7254 "bison-chpl-lib.cpp"
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
#line 7276 "bison-chpl-lib.cpp"
    break;

  case 204: /* $@3: %empty  */
#line 1617 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7284 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 1621 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-4])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7293 "bison-chpl-lib.cpp"
    break;

  case 206: /* $@4: %empty  */
#line 1626 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7302 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr extern_export_kind_inner $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1631 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yylsp[-5]), (yyvsp[-3].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7311 "bison-chpl-lib.cpp"
    break;

  case 208: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1639 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7319 "bison-chpl-lib.cpp"
    break;

  case 209: /* no_loop_attributes: %empty  */
#line 1645 "chpl.ypp"
  {
    // Even though there are no attributes, there could be loop pragmas.
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 7328 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1651 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7334 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1652 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7340 "bison-chpl-lib.cpp"
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
#line 7367 "bison-chpl-lib.cpp"
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
#line 7390 "bison-chpl-lib.cpp"
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
#line 7413 "bison-chpl-lib.cpp"
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
#line 7442 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1755 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7451 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: for_loop_kw expr TIN expr task_intent_clause do_stmt  */
#line 1760 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7460 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1765 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7469 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator task_intent_clause do_stmt  */
#line 1770 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7478 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1775 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7487 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw expr task_intent_clause do_stmt  */
#line 1780 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7496 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1785 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7505 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: for_loop_kw zippered_iterator task_intent_clause do_stmt  */
#line 1790 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7514 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1795 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7523 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TLSBR expr_ls TIN expr task_intent_clause TRSBR stmt  */
#line 1800 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7532 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1805 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7541 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator task_intent_clause TRSBR stmt  */
#line 1810 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7550 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1815 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7559 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR expr_ls task_intent_clause TRSBR stmt  */
#line 1820 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7568 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1825 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7578 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TLSBR zippered_iterator task_intent_clause TRSBR stmt  */
#line 1831 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7588 "bison-chpl-lib.cpp"
    break;

  case 236: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1840 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7598 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr TTHEN stmt  */
#line 1849 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7606 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr block_stmt  */
#line 1853 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7614 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1857 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7622 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1861 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7630 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1865 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7638 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF ifvar block_stmt  */
#line 1869 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7646 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1873 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7654 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1877 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7662 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1881 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7671 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1886 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7680 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1891 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7689 "bison-chpl-lib.cpp"
    break;

  case 248: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1896 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7698 "bison-chpl-lib.cpp"
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
#line 7718 "bison-chpl-lib.cpp"
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
#line 7738 "bison-chpl-lib.cpp"
    break;

  case 251: /* interface_start: TINTERFACE ident_def  */
#line 1939 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Interface);
  }
#line 7746 "bison-chpl-lib.cpp"
    break;

  case 252: /* interface_stmt: interface_start TLP ifc_formal_ls TRP block_stmt  */
#line 1946 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-4].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), parts, (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7756 "bison-chpl-lib.cpp"
    break;

  case 253: /* interface_stmt: interface_start block_stmt  */
#line 1952 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-1].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), parts, nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7766 "bison-chpl-lib.cpp"
    break;

  case 254: /* ifc_formal_ls: ifc_formal  */
#line 1960 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7772 "bison-chpl-lib.cpp"
    break;

  case 255: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1961 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7778 "bison-chpl-lib.cpp"
    break;

  case 256: /* ifc_formal: ident_def  */
#line 1966 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7784 "bison-chpl-lib.cpp"
    break;

  case 276: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 2009 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7792 "bison-chpl-lib.cpp"
    break;

  case 277: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 2013 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7800 "bison-chpl-lib.cpp"
    break;

  case 278: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 2017 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7808 "bison-chpl-lib.cpp"
    break;

  case 279: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2024 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7816 "bison-chpl-lib.cpp"
    break;

  case 280: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 2028 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7824 "bison-chpl-lib.cpp"
    break;

  case 281: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2032 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7832 "bison-chpl-lib.cpp"
    break;

  case 282: /* try_stmt: TTRY tryable_stmt  */
#line 2039 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7840 "bison-chpl-lib.cpp"
    break;

  case 283: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2043 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7848 "bison-chpl-lib.cpp"
    break;

  case 284: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2047 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7856 "bison-chpl-lib.cpp"
    break;

  case 285: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2051 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7864 "bison-chpl-lib.cpp"
    break;

  case 286: /* catch_expr_ls: %empty  */
#line 2057 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7870 "bison-chpl-lib.cpp"
    break;

  case 287: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2058 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7876 "bison-chpl-lib.cpp"
    break;

  case 288: /* catch_expr: TCATCH block_stmt  */
#line 2063 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7884 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2067 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7892 "bison-chpl-lib.cpp"
    break;

  case 290: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2071 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7900 "bison-chpl-lib.cpp"
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
#line 7919 "bison-chpl-lib.cpp"
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
#line 7938 "bison-chpl-lib.cpp"
    break;

  case 293: /* throw_stmt: TTHROW expr TSEMI  */
#line 2111 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7948 "bison-chpl-lib.cpp"
    break;

  case 294: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2119 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7957 "bison-chpl-lib.cpp"
    break;

  case 295: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2124 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7968 "bison-chpl-lib.cpp"
    break;

  case 296: /* when_stmt_ls: %empty  */
#line 2133 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7974 "bison-chpl-lib.cpp"
    break;

  case 297: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2134 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7980 "bison-chpl-lib.cpp"
    break;

  case 298: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2139 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7988 "bison-chpl-lib.cpp"
    break;

  case 299: /* when_stmt: TOTHERWISE stmt  */
#line 2143 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7997 "bison-chpl-lib.cpp"
    break;

  case 300: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2148 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 8007 "bison-chpl-lib.cpp"
    break;

  case 301: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2157 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8016 "bison-chpl-lib.cpp"
    break;

  case 302: /* manager_expr: expr TAS ident_def  */
#line 2162 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8024 "bison-chpl-lib.cpp"
    break;

  case 303: /* manager_expr: expr  */
#line 2166 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8032 "bison-chpl-lib.cpp"
    break;

  case 304: /* manager_expr_ls: manager_expr  */
#line 2172 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8038 "bison-chpl-lib.cpp"
    break;

  case 305: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2173 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8044 "bison-chpl-lib.cpp"
    break;

  case 306: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2178 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8053 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2188 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8063 "bison-chpl-lib.cpp"
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
#line 8075 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_start: class_tag ident_def  */
#line 2206 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8083 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_tag: TCLASS  */
#line 2212 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8089 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_tag: TRECORD  */
#line 2213 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8095 "bison-chpl-lib.cpp"
    break;

  case 312: /* class_tag: TUNION  */
#line 2214 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8101 "bison-chpl-lib.cpp"
    break;

  case 313: /* opt_inherit: %empty  */
#line 2218 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8107 "bison-chpl-lib.cpp"
    break;

  case 314: /* opt_inherit: TCOLON expr_ls  */
#line 2219 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8113 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_level_stmt_ls: %empty  */
#line 2223 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8122 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2228 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8130 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2232 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8138 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2236 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8146 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2240 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8154 "bison-chpl-lib.cpp"
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
#line 8177 "bison-chpl-lib.cpp"
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
#line 8191 "bison-chpl-lib.cpp"
    break;

  case 322: /* enum_header: TENUM ident_def  */
#line 2280 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8199 "bison-chpl-lib.cpp"
    break;

  case 323: /* enum_ls: enum_item  */
#line 2287 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8208 "bison-chpl-lib.cpp"
    break;

  case 324: /* enum_ls: enum_ls TCOMMA  */
#line 2292 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8218 "bison-chpl-lib.cpp"
    break;

  case 325: /* $@5: %empty  */
#line 2298 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8227 "bison-chpl-lib.cpp"
    break;

  case 326: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2303 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8236 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2308 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8245 "bison-chpl-lib.cpp"
    break;

  case 328: /* $@6: %empty  */
#line 2313 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8254 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2318 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8263 "bison-chpl-lib.cpp"
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
#line 8275 "bison-chpl-lib.cpp"
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
#line 8289 "bison-chpl-lib.cpp"
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
#line 8305 "bison-chpl-lib.cpp"
    break;

  case 333: /* $@7: %empty  */
#line 2366 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8315 "bison-chpl-lib.cpp"
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
#line 8338 "bison-chpl-lib.cpp"
    break;

  case 336: /* linkage_spec: linkage_spec_empty  */
#line 2396 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8344 "bison-chpl-lib.cpp"
    break;

  case 337: /* linkage_spec: TINLINE  */
#line 2397 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8351 "bison-chpl-lib.cpp"
    break;

  case 338: /* linkage_spec: TOVERRIDE  */
#line 2399 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8358 "bison-chpl-lib.cpp"
    break;

  case 339: /* opt_fn_type_formal_ls: %empty  */
#line 2404 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8364 "bison-chpl-lib.cpp"
    break;

  case 340: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2405 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8370 "bison-chpl-lib.cpp"
    break;

  case 341: /* fn_type_formal_ls: fn_type_formal  */
#line 2409 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8376 "bison-chpl-lib.cpp"
    break;

  case 342: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2410 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8382 "bison-chpl-lib.cpp"
    break;

  case 343: /* fn_type_formal: named_formal  */
#line 2415 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8388 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2418 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8394 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_type_formal: formal_type  */
#line 2420 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8400 "bison-chpl-lib.cpp"
    break;

  case 346: /* opt_fn_type_ret_type: %empty  */
#line 2424 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8406 "bison-chpl-lib.cpp"
    break;

  case 347: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2425 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8412 "bison-chpl-lib.cpp"
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
#line 8432 "bison-chpl-lib.cpp"
    break;

  case 349: /* $@8: %empty  */
#line 2457 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8442 "bison-chpl-lib.cpp"
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
#line 8455 "bison-chpl-lib.cpp"
    break;

  case 351: /* $@9: %empty  */
#line 2472 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8465 "bison-chpl-lib.cpp"
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
#line 8478 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2490 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8486 "bison-chpl-lib.cpp"
    break;

  case 354: /* $@10: %empty  */
#line 2499 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8494 "bison-chpl-lib.cpp"
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
#line 8517 "bison-chpl-lib.cpp"
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
#line 8530 "bison-chpl-lib.cpp"
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
#line 8543 "bison-chpl-lib.cpp"
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
#line 8557 "bison-chpl-lib.cpp"
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
#line 8571 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2563 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8581 "bison-chpl-lib.cpp"
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
#line 8597 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2588 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 8606 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_ident: ident_def TBANG  */
#line 2598 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8616 "bison-chpl-lib.cpp"
    break;

  case 406: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2655 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8622 "bison-chpl-lib.cpp"
    break;

  case 407: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2656 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8628 "bison-chpl-lib.cpp"
    break;

  case 408: /* opt_formal_ls: %empty  */
#line 2660 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8634 "bison-chpl-lib.cpp"
    break;

  case 409: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2661 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8640 "bison-chpl-lib.cpp"
    break;

  case 410: /* req_formal_ls: TLP TRP  */
#line 2665 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8646 "bison-chpl-lib.cpp"
    break;

  case 411: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2666 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8652 "bison-chpl-lib.cpp"
    break;

  case 412: /* formal_ls_inner: formal  */
#line 2670 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8658 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2671 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8664 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_ls: %empty  */
#line 2675 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8670 "bison-chpl-lib.cpp"
    break;

  case 415: /* formal_ls: formal_ls_inner  */
#line 2676 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8676 "bison-chpl-lib.cpp"
    break;

  case 417: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2685 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8684 "bison-chpl-lib.cpp"
    break;

  case 418: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2690 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8692 "bison-chpl-lib.cpp"
    break;

  case 419: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2695 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8700 "bison-chpl-lib.cpp"
    break;

  case 420: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2700 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8708 "bison-chpl-lib.cpp"
    break;

  case 421: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2705 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yylsp[-5]), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8716 "bison-chpl-lib.cpp"
    break;

  case 422: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2710 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8724 "bison-chpl-lib.cpp"
    break;

  case 423: /* opt_formal_intent_tag: %empty  */
#line 2716 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);
  }
#line 8733 "bison-chpl-lib.cpp"
    break;

  case 424: /* opt_formal_intent_tag: required_intent_tag  */
#line 2721 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8742 "bison-chpl-lib.cpp"
    break;

  case 425: /* required_intent_tag: TIN  */
#line 2728 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::IN);            }
#line 8748 "bison-chpl-lib.cpp"
    break;

  case 426: /* required_intent_tag: TINOUT  */
#line 2729 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::INOUT);         }
#line 8754 "bison-chpl-lib.cpp"
    break;

  case 427: /* required_intent_tag: TOUT  */
#line 2730 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::OUT);           }
#line 8760 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TCONST TIN  */
#line 2731 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_IN);      }
#line 8766 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TCONST TREF  */
#line 2732 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);     }
#line 8772 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TCONST  */
#line 2733 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);         }
#line 8778 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TPARAM  */
#line 2734 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);         }
#line 8784 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TREF  */
#line 2735 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);           }
#line 8790 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TTYPE  */
#line 2736 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);          }
#line 8796 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TVAR  */
#line 2737 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR); }
#line 8802 "bison-chpl-lib.cpp"
    break;

  case 435: /* opt_this_intent_tag: %empty  */
#line 2741 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);     }
#line 8808 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_this_intent_tag: TPARAM  */
#line 2742 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);              }
#line 8814 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: TREF  */
#line 2743 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);                }
#line 8820 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TCONST TREF  */
#line 2744 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);          }
#line 8826 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TCONST  */
#line 2745 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);              }
#line 8832 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TTYPE  */
#line 2746 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);               }
#line 8838 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TIN  */
#line 2747 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::IN);       }
#line 8844 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TCONST TIN  */
#line 2748 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::CONST_IN); }
#line 8850 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_this_intent_tag: TVAR  */
#line 2749 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR);      }
#line 8856 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_this_intent_tag: TOUT  */
#line 2750 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::OUT);      }
#line 8862 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_this_intent_tag: TINOUT  */
#line 2751 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::INOUT);    }
#line 8868 "bison-chpl-lib.cpp"
    break;

  case 446: /* proc_iter_or_op: TPROC  */
#line 2755 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8874 "bison-chpl-lib.cpp"
    break;

  case 447: /* proc_iter_or_op: TITER  */
#line 2756 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8880 "bison-chpl-lib.cpp"
    break;

  case 448: /* proc_iter_or_op: TOPERATOR  */
#line 2757 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8886 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: %empty  */
#line 2761 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::DEFAULT_RETURN_INTENT); }
#line 8892 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TOUT  */
#line 2762 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::OUT);                   }
#line 8898 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TCONST  */
#line 2763 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST);                 }
#line 8904 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TCONST TREF  */
#line 2764 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST_REF);             }
#line 8910 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_ret_tag: TREF  */
#line 2765 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::REF);                   }
#line 8916 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: TPARAM  */
#line 2766 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::PARAM);                 }
#line 8922 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: TTYPE  */
#line 2767 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::TYPE);                  }
#line 8928 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: TIN  */
#line 2768 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::IN);            }
#line 8934 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_ret_tag: TCONST TIN  */
#line 2769 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::CONST_IN);      }
#line 8940 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_ret_tag: TVAR  */
#line 2770 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::VAR);           }
#line 8946 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_ret_tag: TINOUT  */
#line 2771 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::INOUT);         }
#line 8952 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_throws_error: %empty  */
#line 2775 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8958 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_throws_error: TTHROWS  */
#line 2776 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8964 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_function_body_stmt: TSEMI  */
#line 2779 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8970 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_function_body_stmt: function_body_stmt  */
#line 2780 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8976 "bison-chpl-lib.cpp"
    break;

  case 464: /* function_body_stmt: block_stmt_body  */
#line 2784 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8982 "bison-chpl-lib.cpp"
    break;

  case 465: /* function_body_stmt: TDO toplevel_stmt  */
#line 2785 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 8988 "bison-chpl-lib.cpp"
    break;

  case 466: /* query_expr: TQUERIEDIDENT  */
#line 2789 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8994 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_lifetime_where: %empty  */
#line 2794 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9000 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_lifetime_where: TWHERE expr  */
#line 2796 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9006 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2798 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9012 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2800 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9018 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2802 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9024 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_components_expr: lifetime_expr  */
#line 2807 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2809 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9036 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2814 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9042 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2816 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9048 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2818 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9054 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2820 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9060 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2822 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9066 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2824 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9072 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2826 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9078 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_ident: TIDENT  */
#line 2830 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9084 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_ident: TINIT  */
#line 2831 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9090 "bison-chpl-lib.cpp"
    break;

  case 483: /* lifetime_ident: TTHIS  */
#line 2832 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9096 "bison-chpl-lib.cpp"
    break;

  case 484: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2837 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9105 "bison-chpl-lib.cpp"
    break;

  case 485: /* type_alias_decl_stmt_start: TTYPE  */
#line 2846 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9115 "bison-chpl-lib.cpp"
    break;

  case 486: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2852 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9126 "bison-chpl-lib.cpp"
    break;

  case 487: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2862 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9134 "bison-chpl-lib.cpp"
    break;

  case 488: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2866 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9142 "bison-chpl-lib.cpp"
    break;

  case 489: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2873 "chpl.ypp"
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
#line 9165 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_init_type: %empty  */
#line 2894 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9171 "bison-chpl-lib.cpp"
    break;

  case 491: /* opt_init_type: TASSIGN expr  */
#line 2896 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9177 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_type: TPARAM  */
#line 2900 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9183 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_type: TCONST TREF  */
#line 2901 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9189 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_type: TREF  */
#line 2902 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9195 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_type: TCONST  */
#line 2903 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9201 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_type: TVAR  */
#line 2904 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9207 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_start: var_decl_type  */
#line 2909 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9216 "bison-chpl-lib.cpp"
    break;

  case 498: /* $@11: %empty  */
#line 2917 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9225 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt: TCONFIG $@11 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2921 "chpl.ypp"
                                              {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-4]), (yylsp[-3])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9234 "bison-chpl-lib.cpp"
    break;

  case 500: /* $@12: %empty  */
#line 2926 "chpl.ypp"
  {
    // Use a mid-rule action to preserve the 'on' expr
    context->noteVarDestinationExpr((yyvsp[0].expr));
  }
#line 9243 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt: TON expr $@12 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2931 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-4])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9252 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt: var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2936 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9261 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt: var_decl_start error TSEMI  */
#line 2941 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    delete (yyvsp[-2].attributeGroup);
    context->resetDeclStateOnError();
  }
#line 9271 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2950 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9279 "bison-chpl-lib.cpp"
    break;

  case 505: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2954 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9287 "bison-chpl-lib.cpp"
    break;

  case 506: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2961 "chpl.ypp"
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
#line 9310 "bison-chpl-lib.cpp"
    break;

  case 507: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2980 "chpl.ypp"
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
#line 9330 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_var_decl_component: ident_def  */
#line 2999 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9338 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 3003 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9346 "bison-chpl-lib.cpp"
    break;

  case 510: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 3010 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9352 "bison-chpl-lib.cpp"
    break;

  case 511: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 3012 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9358 "bison-chpl-lib.cpp"
    break;

  case 512: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 3014 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9364 "bison-chpl-lib.cpp"
    break;

  case 513: /* opt_init_expr: %empty  */
#line 3020 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9370 "bison-chpl-lib.cpp"
    break;

  case 514: /* opt_init_expr: TASSIGN TNOINIT  */
#line 3021 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9376 "bison-chpl-lib.cpp"
    break;

  case 515: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 3022 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9382 "bison-chpl-lib.cpp"
    break;

  case 516: /* formal_or_ret_type_expr: expr  */
#line 3026 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9388 "bison-chpl-lib.cpp"
    break;

  case 517: /* ret_type: formal_or_ret_type_expr  */
#line 3030 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9394 "bison-chpl-lib.cpp"
    break;

  case 518: /* ret_type: reserved_type_ident_use  */
#line 3031 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9400 "bison-chpl-lib.cpp"
    break;

  case 519: /* ret_type: error  */
#line 3032 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9406 "bison-chpl-lib.cpp"
    break;

  case 520: /* colon_ret_type: TCOLON ret_type  */
#line 3036 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9412 "bison-chpl-lib.cpp"
    break;

  case 521: /* colon_ret_type: error  */
#line 3037 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9418 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_ret_type: %empty  */
#line 3041 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9424 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_type: %empty  */
#line 3046 "chpl.ypp"
                                 { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9430 "bison-chpl-lib.cpp"
    break;

  case 525: /* opt_type: TCOLON expr  */
#line 3047 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9436 "bison-chpl-lib.cpp"
    break;

  case 526: /* opt_type: TCOLON reserved_type_ident_use  */
#line 3048 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9442 "bison-chpl-lib.cpp"
    break;

  case 527: /* opt_type: error  */
#line 3049 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9448 "bison-chpl-lib.cpp"
    break;

  case 528: /* formal_type: formal_or_ret_type_expr  */
#line 3053 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9454 "bison-chpl-lib.cpp"
    break;

  case 529: /* formal_type: reserved_type_ident_use  */
#line 3054 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9460 "bison-chpl-lib.cpp"
    break;

  case 530: /* colon_formal_type: TCOLON formal_type  */
#line 3058 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9466 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_colon_formal_type: %empty  */
#line 3062 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9472 "bison-chpl-lib.cpp"
    break;

  case 532: /* opt_colon_formal_type: colon_formal_type  */
#line 3063 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9478 "bison-chpl-lib.cpp"
    break;

  case 533: /* expr_ls: expr  */
#line 3069 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9484 "bison-chpl-lib.cpp"
    break;

  case 534: /* expr_ls: expr_ls TCOMMA expr  */
#line 3070 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9490 "bison-chpl-lib.cpp"
    break;

  case 535: /* expr_ls_end_semi: expr_ls semicolon_list  */
#line 3074 "chpl.ypp"
                         {
    (yyval.ndArrayList) = context->makeNDArrayList();
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElement((yyvsp[-1].exprList), (yylsp[-1])));
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9500 "bison-chpl-lib.cpp"
    break;

  case 536: /* expr_ls_end_semi: expr_ls TCOMMA semicolon_list  */
#line 3079 "chpl.ypp"
                                {
    (yyval.ndArrayList) = context->makeNDArrayList();
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElement((yyvsp[-2].exprList), (yylsp[-2])));
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9510 "bison-chpl-lib.cpp"
    break;

  case 537: /* expr_ls_end_semi: expr_ls_semi semicolon_list  */
#line 3085 "chpl.ypp"
  {
    (yyval.ndArrayList) = (yyvsp[-1].ndArrayList);
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9519 "bison-chpl-lib.cpp"
    break;

  case 538: /* expr_ls_semi: expr_ls_end_semi expr_ls  */
#line 3092 "chpl.ypp"
                           {
    (yyval.ndArrayList) = context->appendNDArrayList((yyvsp[-1].ndArrayList), makeNDArrayElement((yyvsp[0].exprList), (yylsp[0])));
  }
#line 9527 "bison-chpl-lib.cpp"
    break;

  case 539: /* expr_ls_semi: expr_ls_end_semi expr_ls TCOMMA  */
#line 3095 "chpl.ypp"
                                  {
    (yyval.ndArrayList) = context->appendNDArrayList((yyvsp[-2].ndArrayList), makeNDArrayElement((yyvsp[-1].exprList), (yylsp[-1])));
  }
#line 9535 "bison-chpl-lib.cpp"
    break;

  case 540: /* semicolon_list: TSEMI  */
#line 3101 "chpl.ypp"
                       { (yyval.counter) = 1; }
#line 9541 "bison-chpl-lib.cpp"
    break;

  case 541: /* semicolon_list: semicolon_list TSEMI  */
#line 3102 "chpl.ypp"
                       { (yyval.counter) = (yyvsp[-1].counter) + 1; }
#line 9547 "bison-chpl-lib.cpp"
    break;

  case 542: /* tuple_component: opt_try_expr  */
#line 3108 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9553 "bison-chpl-lib.cpp"
    break;

  case 543: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3113 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9561 "bison-chpl-lib.cpp"
    break;

  case 544: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3117 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9569 "bison-chpl-lib.cpp"
    break;

  case 545: /* opt_actual_ls: %empty  */
#line 3123 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9575 "bison-chpl-lib.cpp"
    break;

  case 546: /* opt_actual_ls: actual_ls  */
#line 3124 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9581 "bison-chpl-lib.cpp"
    break;

  case 547: /* actual_ls: actual_expr  */
#line 3129 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9590 "bison-chpl-lib.cpp"
    break;

  case 548: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3134 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9600 "bison-chpl-lib.cpp"
    break;

  case 549: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3142 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9606 "bison-chpl-lib.cpp"
    break;

  case 550: /* actual_expr: opt_try_expr  */
#line 3143 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9612 "bison-chpl-lib.cpp"
    break;

  case 551: /* ident_expr: ident_use  */
#line 3147 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9618 "bison-chpl-lib.cpp"
    break;

  case 552: /* ident_expr: scalar_type  */
#line 3148 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9624 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3161 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9630 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3163 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9636 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3165 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9642 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3167 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9652 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TATOMIC expr  */
#line 3173 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9658 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TSYNC expr  */
#line 3175 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9664 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TOWNED  */
#line 3178 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9670 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TOWNED expr  */
#line 3180 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9676 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TUNMANAGED  */
#line 3182 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9682 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3184 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9688 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TSHARED  */
#line 3186 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9694 "bison-chpl-lib.cpp"
    break;

  case 569: /* sub_type_level_expr: TSHARED expr  */
#line 3188 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9700 "bison-chpl-lib.cpp"
    break;

  case 570: /* sub_type_level_expr: TBORROWED  */
#line 3190 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9706 "bison-chpl-lib.cpp"
    break;

  case 571: /* sub_type_level_expr: TBORROWED expr  */
#line 3192 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9712 "bison-chpl-lib.cpp"
    break;

  case 572: /* sub_type_level_expr: TCLASS  */
#line 3194 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9718 "bison-chpl-lib.cpp"
    break;

  case 573: /* sub_type_level_expr: TRECORD  */
#line 3196 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9724 "bison-chpl-lib.cpp"
    break;

  case 574: /* for_expr: no_loop_attributes for_expr_base  */
#line 3200 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9730 "bison-chpl-lib.cpp"
    break;

  case 575: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3205 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9738 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3209 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9746 "bison-chpl-lib.cpp"
    break;

  case 577: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3213 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9754 "bison-chpl-lib.cpp"
    break;

  case 578: /* for_expr_base: for_loop_kw zippered_iterator TDO expr  */
#line 3217 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "expression-level loops with 'zip' must have an index.");
  }
#line 9762 "bison-chpl-lib.cpp"
    break;

  case 579: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3221 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9775 "bison-chpl-lib.cpp"
    break;

  case 580: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3230 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9788 "bison-chpl-lib.cpp"
    break;

  case 581: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3239 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9801 "bison-chpl-lib.cpp"
    break;

  case 582: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3250 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9807 "bison-chpl-lib.cpp"
    break;

  case 583: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3255 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9815 "bison-chpl-lib.cpp"
    break;

  case 584: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3259 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9823 "bison-chpl-lib.cpp"
    break;

  case 585: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3263 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9831 "bison-chpl-lib.cpp"
    break;

  case 586: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3267 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9839 "bison-chpl-lib.cpp"
    break;

  case 587: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3271 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9847 "bison-chpl-lib.cpp"
    break;

  case 588: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3275 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9855 "bison-chpl-lib.cpp"
    break;

  case 589: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3279 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9863 "bison-chpl-lib.cpp"
    break;

  case 590: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3286 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9877 "bison-chpl-lib.cpp"
    break;

  case 591: /* nil_expr: TNIL  */
#line 3303 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9883 "bison-chpl-lib.cpp"
    break;

  case 599: /* opt_task_intent_ls: %empty  */
#line 3321 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9889 "bison-chpl-lib.cpp"
    break;

  case 600: /* opt_task_intent_ls: task_intent_clause  */
#line 3322 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9895 "bison-chpl-lib.cpp"
    break;

  case 601: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3327 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9903 "bison-chpl-lib.cpp"
    break;

  case 602: /* task_intent_clause: TWITH TLP TRP  */
#line 3331 "chpl.ypp"
  {
    auto err = context->syntax((yyloc), "'with' clause cannot be empty.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-2]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9914 "bison-chpl-lib.cpp"
    break;

  case 603: /* task_intent_clause: TWITH TLP error TRP  */
#line 3338 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[-2]), "invalid intent expression in 'with' clause.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9925 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_intent_clause: TWITH error  */
#line 3345 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[0]), "missing parentheses around 'with' clause intents.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-1]), dummyLoc, dummyLoc, dummyLoc, exprs);
  }
#line 9936 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_intent_ls: intent_expr  */
#line 3354 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9942 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3355 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9948 "bison-chpl-lib.cpp"
    break;

  case 607: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3360 "chpl.ypp"
  {
    (yyval.expr) = context->buildTaskIntent((yyloc), (yylsp[-2]), nullptr, (yyvsp[-2].expr), (yyvsp[-3].taskIntent), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9956 "bison-chpl-lib.cpp"
    break;

  case 608: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3364 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9964 "bison-chpl-lib.cpp"
    break;

  case 609: /* intent_expr: expr TREDUCE ident_expr  */
#line 3368 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9972 "bison-chpl-lib.cpp"
    break;

  case 610: /* task_var_prefix: TCONST  */
#line 3374 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST);          }
#line 9978 "bison-chpl-lib.cpp"
    break;

  case 611: /* task_var_prefix: TIN  */
#line 3375 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::IN);             }
#line 9984 "bison-chpl-lib.cpp"
    break;

  case 612: /* task_var_prefix: TCONST TIN  */
#line 3376 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_IN);       }
#line 9990 "bison-chpl-lib.cpp"
    break;

  case 613: /* task_var_prefix: TREF  */
#line 3377 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::REF);            }
#line 9996 "bison-chpl-lib.cpp"
    break;

  case 614: /* task_var_prefix: TCONST TREF  */
#line 3378 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_REF);      }
#line 10002 "bison-chpl-lib.cpp"
    break;

  case 615: /* task_var_prefix: TVAR  */
#line 3379 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::VAR);            }
#line 10008 "bison-chpl-lib.cpp"
    break;

  case 616: /* task_var_prefix: TOUT  */
#line 3380 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::OUT);   }
#line 10014 "bison-chpl-lib.cpp"
    break;

  case 617: /* task_var_prefix: TINOUT  */
#line 3381 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::INOUT); }
#line 10020 "bison-chpl-lib.cpp"
    break;

  case 618: /* task_var_prefix: TPARAM  */
#line 3382 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::PARAM); }
#line 10026 "bison-chpl-lib.cpp"
    break;

  case 619: /* task_var_prefix: TTYPE  */
#line 3383 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::TYPE);  }
#line 10032 "bison-chpl-lib.cpp"
    break;

  case 620: /* new_maybe_decorated: TNEW  */
#line 3388 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10038 "bison-chpl-lib.cpp"
    break;

  case 621: /* new_maybe_decorated: TNEW TOWNED  */
#line 3390 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10044 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_maybe_decorated: TNEW TSHARED  */
#line 3392 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10050 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3394 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10056 "bison-chpl-lib.cpp"
    break;

  case 624: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3396 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10062 "bison-chpl-lib.cpp"
    break;

  case 625: /* new_expr: new_maybe_decorated expr  */
#line 3402 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10070 "bison-chpl-lib.cpp"
    break;

  case 626: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3409 "chpl.ypp"
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
#line 10087 "bison-chpl-lib.cpp"
    break;

  case 627: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3422 "chpl.ypp"
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
#line 10104 "bison-chpl-lib.cpp"
    break;

  case 628: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3435 "chpl.ypp"
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
#line 10122 "bison-chpl-lib.cpp"
    break;

  case 629: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3449 "chpl.ypp"
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
#line 10141 "bison-chpl-lib.cpp"
    break;

  case 630: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3467 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10149 "bison-chpl-lib.cpp"
    break;

  case 631: /* range_literal_expr: expr TDOTDOT expr  */
#line 3474 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10158 "bison-chpl-lib.cpp"
    break;

  case 632: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3479 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10167 "bison-chpl-lib.cpp"
    break;

  case 633: /* range_literal_expr: expr TDOTDOT  */
#line 3484 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10176 "bison-chpl-lib.cpp"
    break;

  case 634: /* range_literal_expr: TDOTDOT expr  */
#line 3489 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10186 "bison-chpl-lib.cpp"
    break;

  case 635: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3495 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10196 "bison-chpl-lib.cpp"
    break;

  case 636: /* range_literal_expr: TDOTDOT  */
#line 3501 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10206 "bison-chpl-lib.cpp"
    break;

  case 637: /* cast_expr: expr TCOLON expr  */
#line 3531 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10214 "bison-chpl-lib.cpp"
    break;

  case 638: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3538 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10222 "bison-chpl-lib.cpp"
    break;

  case 639: /* super_expr: fn_expr  */
#line 3544 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10228 "bison-chpl-lib.cpp"
    break;

  case 643: /* expr: sub_type_level_expr TQUESTION  */
#line 3553 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10234 "bison-chpl-lib.cpp"
    break;

  case 644: /* expr: TQUESTION  */
#line 3555 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10240 "bison-chpl-lib.cpp"
    break;

  case 648: /* expr: fn_type  */
#line 3560 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10246 "bison-chpl-lib.cpp"
    break;

  case 658: /* opt_expr: %empty  */
#line 3574 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10252 "bison-chpl-lib.cpp"
    break;

  case 659: /* opt_expr: expr  */
#line 3575 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10258 "bison-chpl-lib.cpp"
    break;

  case 660: /* opt_try_expr: TTRY expr  */
#line 3579 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10264 "bison-chpl-lib.cpp"
    break;

  case 661: /* opt_try_expr: TTRYBANG expr  */
#line 3580 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10270 "bison-chpl-lib.cpp"
    break;

  case 662: /* opt_try_expr: super_expr  */
#line 3581 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10276 "bison-chpl-lib.cpp"
    break;

  case 668: /* call_base_expr: expr TBANG  */
#line 3598 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10284 "bison-chpl-lib.cpp"
    break;

  case 669: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3601 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10290 "bison-chpl-lib.cpp"
    break;

  case 672: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3608 "chpl.ypp"
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
#line 10306 "bison-chpl-lib.cpp"
    break;

  case 673: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3620 "chpl.ypp"
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
#line 10322 "bison-chpl-lib.cpp"
    break;

  case 674: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3632 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10330 "bison-chpl-lib.cpp"
    break;

  case 675: /* dot_expr: expr TDOT ident_use  */
#line 3639 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10336 "bison-chpl-lib.cpp"
    break;

  case 676: /* dot_expr: expr TDOT TTYPE  */
#line 3641 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10342 "bison-chpl-lib.cpp"
    break;

  case 677: /* dot_expr: expr TDOT TDOMAIN  */
#line 3643 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10348 "bison-chpl-lib.cpp"
    break;

  case 678: /* dot_expr: expr TDOT TLOCALE  */
#line 3645 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10354 "bison-chpl-lib.cpp"
    break;

  case 679: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3647 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10360 "bison-chpl-lib.cpp"
    break;

  case 680: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3649 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10366 "bison-chpl-lib.cpp"
    break;

  case 681: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3658 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10376 "bison-chpl-lib.cpp"
    break;

  case 682: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3664 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10384 "bison-chpl-lib.cpp"
    break;

  case 683: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3668 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10392 "bison-chpl-lib.cpp"
    break;

  case 684: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3672 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10400 "bison-chpl-lib.cpp"
    break;

  case 685: /* bool_literal: TFALSE  */
#line 3678 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10406 "bison-chpl-lib.cpp"
    break;

  case 686: /* bool_literal: TTRUE  */
#line 3679 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10412 "bison-chpl-lib.cpp"
    break;

  case 687: /* str_bytes_literal: STRINGLITERAL  */
#line 3683 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10418 "bison-chpl-lib.cpp"
    break;

  case 688: /* str_bytes_literal: BYTESLITERAL  */
#line 3684 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10424 "bison-chpl-lib.cpp"
    break;

  case 691: /* literal_expr: INTLITERAL  */
#line 3690 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10430 "bison-chpl-lib.cpp"
    break;

  case 692: /* literal_expr: REALLITERAL  */
#line 3691 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10436 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: IMAGLITERAL  */
#line 3692 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10442 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: TNONE  */
#line 3693 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10448 "bison-chpl-lib.cpp"
    break;

  case 695: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3695 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10457 "bison-chpl-lib.cpp"
    break;

  case 696: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3700 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10466 "bison-chpl-lib.cpp"
    break;

  case 697: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3705 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10475 "bison-chpl-lib.cpp"
    break;

  case 698: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3710 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10484 "bison-chpl-lib.cpp"
    break;

  case 699: /* literal_expr: no_loop_attributes TLSBR expr_ls_semi TRSBR  */
#line 3715 "chpl.ypp"
  {
    (yyval.expr) = context->buildNDArray((yyloc), (yyvsp[-1].ndArrayList));
    context->popLoopAttributeGroup();
  }
#line 10493 "bison-chpl-lib.cpp"
    break;

  case 700: /* literal_expr: no_loop_attributes TLSBR expr_ls_semi semicolon_list TRSBR  */
#line 3720 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    (yyval.expr) = context->buildNDArray((yyloc), (yyvsp[-2].ndArrayList));
    context->popLoopAttributeGroup();
  }
#line 10505 "bison-chpl-lib.cpp"
    break;

  case 701: /* literal_expr: no_loop_attributes TLSBR expr_ls semicolon_list TRSBR  */
#line 3728 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    AstList lst;
    lst.push_back(ArrayRow::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))));
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), std::move(lst)).release();
    context->popLoopAttributeGroup();
  }
#line 10519 "bison-chpl-lib.cpp"
    break;

  case 702: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA semicolon_list TRSBR  */
#line 3738 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    AstList lst;
    lst.push_back(ArrayRow::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-3].exprList))));
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), std::move(lst)).release();
    context->popLoopAttributeGroup();
  }
#line 10533 "bison-chpl-lib.cpp"
    break;

  case 703: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3748 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10542 "bison-chpl-lib.cpp"
    break;

  case 704: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3753 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10551 "bison-chpl-lib.cpp"
    break;

  case 705: /* assoc_expr_ls: expr TALIAS expr  */
#line 3762 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10560 "bison-chpl-lib.cpp"
    break;

  case 706: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3767 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10570 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TPLUS expr  */
#line 3775 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10576 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TMINUS expr  */
#line 3776 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10582 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TSTAR expr  */
#line 3777 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10588 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TDIVIDE expr  */
#line 3778 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10594 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3779 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10600 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3780 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10606 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TMOD expr  */
#line 3781 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10612 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TEQUAL expr  */
#line 3782 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10618 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3783 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10624 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3784 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10630 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3785 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10636 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TLESS expr  */
#line 3786 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10642 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TGREATER expr  */
#line 3787 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10648 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TBAND expr  */
#line 3788 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10654 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr TBOR expr  */
#line 3789 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10660 "bison-chpl-lib.cpp"
    break;

  case 722: /* binary_op_expr: expr TBXOR expr  */
#line 3790 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10666 "bison-chpl-lib.cpp"
    break;

  case 723: /* binary_op_expr: expr TAND expr  */
#line 3791 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10672 "bison-chpl-lib.cpp"
    break;

  case 724: /* binary_op_expr: expr TOR expr  */
#line 3792 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10678 "bison-chpl-lib.cpp"
    break;

  case 725: /* binary_op_expr: expr TEXP expr  */
#line 3793 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10684 "bison-chpl-lib.cpp"
    break;

  case 726: /* binary_op_expr: expr TBY expr  */
#line 3794 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10690 "bison-chpl-lib.cpp"
    break;

  case 727: /* binary_op_expr: expr TALIGN expr  */
#line 3795 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10696 "bison-chpl-lib.cpp"
    break;

  case 728: /* binary_op_expr: expr THASH expr  */
#line 3796 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10702 "bison-chpl-lib.cpp"
    break;

  case 729: /* binary_op_expr: expr TDMAPPED expr  */
#line 3797 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10708 "bison-chpl-lib.cpp"
    break;

  case 730: /* unary_op_expr: TPLUS expr  */
#line 3801 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10714 "bison-chpl-lib.cpp"
    break;

  case 731: /* unary_op_expr: TMINUS expr  */
#line 3802 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10720 "bison-chpl-lib.cpp"
    break;

  case 732: /* unary_op_expr: TMINUSMINUS expr  */
#line 3803 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10726 "bison-chpl-lib.cpp"
    break;

  case 733: /* unary_op_expr: TPLUSPLUS expr  */
#line 3804 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10732 "bison-chpl-lib.cpp"
    break;

  case 734: /* unary_op_expr: TBANG expr  */
#line 3805 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10738 "bison-chpl-lib.cpp"
    break;

  case 735: /* unary_op_expr: expr TBANG  */
#line 3806 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10746 "bison-chpl-lib.cpp"
    break;

  case 736: /* unary_op_expr: TBNOT expr  */
#line 3809 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10752 "bison-chpl-lib.cpp"
    break;

  case 737: /* reduce_expr: expr TREDUCE expr  */
#line 3814 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10760 "bison-chpl-lib.cpp"
    break;

  case 738: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3818 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10768 "bison-chpl-lib.cpp"
    break;

  case 739: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3822 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10776 "bison-chpl-lib.cpp"
    break;

  case 740: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3826 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10784 "bison-chpl-lib.cpp"
    break;

  case 741: /* scan_expr: expr TSCAN expr  */
#line 3833 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10792 "bison-chpl-lib.cpp"
    break;

  case 742: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3837 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10800 "bison-chpl-lib.cpp"
    break;

  case 743: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3841 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10808 "bison-chpl-lib.cpp"
    break;

  case 744: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3845 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10816 "bison-chpl-lib.cpp"
    break;


#line 10820 "bison-chpl-lib.cpp"

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
