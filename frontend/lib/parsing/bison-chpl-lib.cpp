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
#line 374 "chpl.ypp"

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
#define YYLAST   17938

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  180
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  209
/* YYNRULES -- Number of rules.  */
#define YYNRULES  749
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1275

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
       0,   685,   685,   689,   690,   695,   696,   704,   708,   718,
     723,   730,   731,   732,   733,   734,   735,   736,   737,   738,
     739,   757,   758,   759,   767,   776,   785,   789,   796,   804,
     805,   806,   807,   808,   809,   810,   811,   813,   833,   841,
     858,   874,   891,   908,   924,   945,   946,   947,   951,   957,
     958,   962,   966,   967,   971,   978,   986,   989,   999,  1001,
    1006,  1012,  1017,  1026,  1030,  1051,  1068,  1085,  1103,  1104,
    1106,  1111,  1112,  1117,  1135,  1140,  1145,  1157,  1176,  1177,
    1181,  1185,  1189,  1194,  1198,  1202,  1210,  1215,  1221,  1226,
    1235,  1236,  1240,  1245,  1253,  1262,  1270,  1281,  1289,  1290,
    1291,  1292,  1293,  1294,  1295,  1296,  1297,  1298,  1299,  1300,
    1301,  1302,  1303,  1304,  1316,  1320,  1325,  1330,  1336,  1345,
    1346,  1350,  1359,  1363,  1367,  1371,  1375,  1379,  1388,  1389,
    1390,  1394,  1395,  1396,  1397,  1398,  1402,  1403,  1404,  1416,
    1417,  1418,  1423,  1424,  1425,  1426,  1427,  1428,  1429,  1430,
    1431,  1432,  1433,  1434,  1435,  1436,  1437,  1438,  1439,  1440,
    1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,  1452,  1453,
    1454,  1455,  1462,  1463,  1464,  1468,  1469,  1473,  1480,  1490,
    1494,  1500,  1500,  1506,  1506,  1515,  1516,  1517,  1518,  1519,
    1520,  1521,  1525,  1530,  1535,  1540,  1547,  1555,  1556,  1560,
    1573,  1581,  1588,  1597,  1616,  1615,  1625,  1624,  1637,  1644,
    1650,  1651,  1660,  1661,  1662,  1663,  1667,  1690,  1709,  1728,
    1753,  1758,  1763,  1768,  1773,  1778,  1783,  1788,  1793,  1798,
    1803,  1808,  1813,  1818,  1823,  1829,  1838,  1847,  1851,  1855,
    1859,  1863,  1867,  1871,  1875,  1879,  1884,  1889,  1894,  1902,
    1918,  1937,  1944,  1950,  1959,  1960,  1965,  1970,  1971,  1972,
    1973,  1974,  1975,  1976,  1977,  1978,  1979,  1980,  1981,  1986,
    1991,  1992,  1993,  1994,  2002,  2003,  2007,  2011,  2015,  2022,
    2026,  2030,  2037,  2041,  2045,  2049,  2056,  2057,  2061,  2065,
    2069,  2076,  2091,  2109,  2117,  2122,  2132,  2133,  2137,  2141,
    2146,  2155,  2160,  2164,  2171,  2172,  2176,  2186,  2192,  2204,
    2211,  2212,  2213,  2217,  2218,  2222,  2226,  2230,  2234,  2238,
    2246,  2265,  2278,  2285,  2290,  2297,  2296,  2306,  2312,  2311,
    2326,  2334,  2347,  2365,  2362,  2391,  2395,  2396,  2398,  2403,
    2404,  2408,  2409,  2413,  2416,  2418,  2423,  2424,  2435,  2456,
    2455,  2471,  2470,  2488,  2498,  2495,  2523,  2532,  2541,  2551,
    2561,  2570,  2585,  2586,  2594,  2595,  2596,  2605,  2606,  2607,
    2608,  2609,  2610,  2611,  2612,  2613,  2614,  2615,  2616,  2617,
    2618,  2619,  2620,  2621,  2622,  2623,  2624,  2625,  2626,  2627,
    2628,  2629,  2633,  2634,  2635,  2636,  2637,  2638,  2639,  2640,
    2641,  2642,  2643,  2644,  2649,  2650,  2654,  2655,  2659,  2660,
    2664,  2665,  2669,  2670,  2674,  2675,  2679,  2683,  2687,  2692,
    2697,  2702,  2707,  2715,  2719,  2727,  2728,  2729,  2730,  2731,
    2732,  2733,  2734,  2735,  2736,  2740,  2741,  2742,  2743,  2744,
    2745,  2746,  2747,  2748,  2749,  2750,  2754,  2755,  2756,  2760,
    2761,  2762,  2763,  2764,  2765,  2766,  2767,  2768,  2769,  2770,
    2774,  2775,  2778,  2779,  2783,  2784,  2788,  2793,  2794,  2796,
    2798,  2800,  2805,  2807,  2812,  2814,  2816,  2818,  2820,  2822,
    2824,  2829,  2830,  2831,  2835,  2844,  2850,  2860,  2864,  2871,
    2893,  2894,  2899,  2900,  2901,  2902,  2903,  2907,  2916,  2915,
    2925,  2924,  2934,  2939,  2948,  2952,  2959,  2978,  2997,  3001,
    3008,  3010,  3012,  3019,  3020,  3021,  3025,  3029,  3030,  3031,
    3035,  3036,  3040,  3041,  3045,  3046,  3047,  3048,  3052,  3053,
    3057,  3061,  3062,  3068,  3069,  3073,  3078,  3083,  3091,  3094,
    3100,  3101,  3107,  3111,  3115,  3122,  3123,  3127,  3132,  3141,
    3142,  3146,  3147,  3154,  3155,  3156,  3157,  3158,  3159,  3161,
    3163,  3165,  3171,  3173,  3176,  3178,  3180,  3182,  3184,  3186,
    3188,  3190,  3192,  3194,  3199,  3203,  3207,  3211,  3215,  3219,
    3228,  3237,  3249,  3253,  3257,  3261,  3265,  3269,  3273,  3277,
    3284,  3302,  3310,  3311,  3312,  3313,  3314,  3315,  3316,  3320,
    3321,  3325,  3329,  3336,  3343,  3353,  3354,  3358,  3362,  3366,
    3373,  3374,  3375,  3376,  3377,  3378,  3379,  3380,  3381,  3382,
    3386,  3388,  3390,  3392,  3394,  3400,  3407,  3420,  3433,  3447,
    3465,  3472,  3477,  3482,  3487,  3493,  3499,  3529,  3536,  3543,
    3544,  3548,  3550,  3551,  3553,  3555,  3556,  3557,  3558,  3561,
    3562,  3563,  3564,  3565,  3566,  3567,  3568,  3569,  3573,  3574,
    3578,  3579,  3580,  3584,  3585,  3586,  3587,  3596,  3597,  3600,
    3601,  3602,  3606,  3618,  3630,  3637,  3639,  3641,  3643,  3645,
    3647,  3656,  3662,  3666,  3670,  3677,  3678,  3682,  3683,  3687,
    3688,  3689,  3690,  3691,  3692,  3693,  3698,  3703,  3708,  3713,
    3718,  3723,  3728,  3737,  3742,  3751,  3752,  3753,  3754,  3755,
    3756,  3757,  3758,  3759,  3760,  3761,  3762,  3763,  3764,  3765,
    3766,  3767,  3768,  3769,  3770,  3771,  3772,  3773,  3777,  3778,
    3779,  3780,  3781,  3782,  3785,  3789,  3793,  3797,  3801,  3808,
    3812,  3816,  3820,  3828,  3829,  3830,  3831,  3832,  3833,  3834
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

#define YYPACT_NINF (-920)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-750)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -920,   100,  3950,  -920,   -48,    84,  -920,  -920,  -920,  -920,
    -920,  -920,  -920, 12050,    30,    91,   104, 13694,   126, 17705,
      30,   168,   124,   169,    91,  5342, 12050,   127, 17764,  -920,
     251,   275,  -920,  9983,   284,   386,   194,  -920,   295,   279,
   17764, 17764, 17764,  -920,  2069, 11189,   372, 12050, 12050,   294,
    -920,   414,   422, 12050,  -920, 13694,  -920, 12050,   447,   320,
     331,   400,  3213,   469, 17823,  -920, 12050,  8099, 12050, 11189,
   13694,   460,   518,   426,  5342,   531, 12050,   545,  7073,  7073,
    -920,   553,  -920, 13694,  -920,   574, 10154,  -920, 12050,  -920,
   12050,  -920,  -920, 13523, 12050,  -920, 10325,  -920,  -920,  -920,
    4298,  8270, 12050,  -920,  4994,  -920,  -920,  -920,  -920, 17354,
     627,  -920,   487,   474,  -920,    35,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  7415,
    -920,  7586,  -920,  -920,    29,  -920,  -920,    70,  -920,   582,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,   502, 17764,
    -920,   481,   483,   144,  -920,   248,  -920,  -920,  -920,   672,
     799,  -920,  -920, 17764,  -920,  1489,  -920,   488,   492,  -920,
    -920,  -920,   496,   497, 12050,   499,   501,  -920,  -920,  -920,
   17023,  1659,   234,   506,   512,  -920,  -920,   349,  -920,  -920,
    -920,  -920,  -920,   300,  -920,  -920,  -920, 12050,  -920, 17764,
    -920, 12050, 12050,    85,   602,   382,  -920,  -920,  -920,  -920,
   17023,   434,  -920,  -920,    46,  5342,  -920,  -920,  -920,   516,
     -29,   513,  -920,    55,  -920,   517,  -920,   -42, 17023,  8441,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920, 17764, 17764,
     -27,  3754,  -920,  -920,   598,  8441,   520,   519,  -920,  5342,
    2362,   132,     8,  -920,  5342,  -920,  -920,  3256,  -920,    32,
   15481,   451,  -920,   522,   529,  -920,  3256,   -29,   451,  -920,
    8441,  2417,  2417,    41,    -6,  -920,    10,  -920,  -920,  -920,
    -920,  7757,  -920,  -920,   555,  -920,   515,   542,  -920,  -920,
    3547,   544,  -920,  -920, 17023,    89, 12050, 12050,  -920,   -54,
    -920,  -920, 17023,   527, 15600,  -920,  3256,   -29,   533,  8441,
    -920, 17023, 15640, 12050,  -920,  -920,  -920,  -920,  -920,   -29,
     538,   190,   190,  2729,   451,   451,   401,  -920,  -920,  4472,
     153, 12050,   560,    13,   536,  2729,   700,  -920,  -920, 17764,
    -920, 12050,  -920,  4298,  -920,   131,  -920,   598,  -920,  -920,
     707,   557,  4646, 12050,  -920, 12050,   660,  -920,  -920, 14823,
     208,   502, 17023,   383,  -920,  5342,   651,  -920,  -920,  9983,
   10496, 11360,  -920,  -920,  -920, 17764,  -920, 17764, 12050,   562,
    -920, 16601,   278,   672,  -920,  -920,  -920,  -920,    45,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,    78,   218,  -920,  -920,
    -920,  -920,  -920,  -920,  -920, 13357,   609,   191,  -920,   571,
     214,   449,  -920,   590, 12050, 12050, 12050, 11360, 11360, 12050,
     452, 12050, 12050, 12050, 12050, 12050,   308, 13523, 12050, 12050,
   12050, 12050, 12050, 12050, 12050, 12050, 12050, 12050, 12050, 12050,
   12050, 12050, 12050, 12050,   670,  -920,  -920,  -920,  -920,  -920,
   10154, 10154,  -920,  -920,  -920,  -920, 10154,  -920,  -920, 10154,
   10154,  8441,  8612, 11360, 11360, 15680,   575,  -920, 11531, 11360,
   17764,  -920,  6383,  -920,  -920,  -920,  2069,  -920, 12050,  -920,
     623,   578,   608,  -920,  -920,   632,   640,  5342,   731,  5342,
    -920,   733, 12050,   704,   599,  -920, 10154,  -920,  2362,  -920,
    -920,    68,  -920, 12221,   648, 12050,  2069,  -920,  -920, 12050,
    -920, 17555, 12050, 12050,  -920,    55,   601,  -920,  -920,  -920,
    -920, 17764,  -920,   672, 13222,  2362,   629, 12221,  -920, 17023,
   17023,  -920,   513,  -920,    48,  -920,  8441,   607,  -920,  3256,
     759,   759,  -920,  -920,  -920,  -920, 11705,  -920, 15759,  8786,
    8957,  -920,  -920,  -920, 17023, 11360, 11360,  -920,   487,  9128,
    -920,   405,  -920,  4820,  -920,   239, 15799,   337, 14913, 17764,
    6899,  6728,  -920,   502,   615,  -920,  -920,    55,  -920,   671,
   17764,    65,  3256,   619, 13836,   -36,   160, 12050,   260, 15840,
     -62,    24,  2866,  -920,   148,  -920,   329,   645,  1845,   624,
   17646,   669,   242,  -920,  -920,   149,  -920,  -920,    92,  -920,
    -920,  -920,  6557,  -920,   692,  -920,  -920,   634,   661,  -920,
     667,  -920,   677,   679,   680,   681,   684,  -920,   685,   686,
     689,   690,   691,   694,   696,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920, 12050,   668,   701,
     634,  -920,   634,  -920, 12050,  -920, 17764,  -920,  -920,  -920,
   17329, 17329,   411,  -920,   411,  -920,   411, 17062,   722,  2573,
     577,   -29,   190,  -920,   453,  -920,  -920,  -920,  -920,  -920,
    2729,  2130,   411,   655,   655, 17329,   655,   655,  1195,   190,
    2130, 17403,  1195,   451,   451,   190,  2729,   654,   687,   693,
     695,   697,   708,   647,   674,  -920,   411,  -920,   411, 12050,
   10154,   109,   821, 15152,   683,   688,   219,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,   167,  -920,   541, 17102,   416,
     340, 17023, 10154,  -920, 10154, 12050, 12050,   822,  5342,   830,
   15991,  5342, 15231, 17764,  -920,   180,   204,  2362,   132,  -920,
   17023,  9299,  -920, 17023,  -920,  -920,  -920, 17764, 16031, 16071,
    2069,  -920,   629,   710,  -920,   212, 12221,  -920,   298,  -920,
   12050,  -920,   698,    -8,   699,  -920,  1965,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,   227,    31, 14733,  -920,  -920,
     286,  -920,  -920,  -920,  -920, 12050,  -920,    77, 13935, 12050,
    -920, 12050,  6899,  6728,  -920,  9470,   433,  9641,   456,   495,
    9812,  7928,   498,   357,  -920,  -920,  -920, 16150,   719,   715,
     716,  -920, 17764,  2069, 12050,   837,  -920,  -920, 12050, 17023,
    5342,   717, 11360, 10670,  -920,  4124,   729,   720,   752,   725,
    -920,   729, 12050, 11876,  -920,  5342,  -920,    65,  5516, 11360,
    -920,    65, 17764,   513,  -920, 10154,   732,  2719,  -920,  -920,
   12050,    52,  -920,   885,  -920,   692,  -920,  -920,  -920,  -920,
    -920,     1,   317,  -920, 16190,  -920, 14093,  -920,  -920, 17023,
    -920,   735,   727,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,   296, 11360, 13836, 12050, 12392, 11360, 10154,  -920,  -920,
    -920,  7244,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,    43, 11360, 11360,  -920,  -920,  -920,
   17023, 17023,  5342,  -920,  5342, 12050,  -920,  5342,   869,   740,
     741,  -920,  -920,   648,  -920,  -920,  -920,   739,   743,   368,
     298, 12050,   692,   629,  -920, 10844,  -920,  -920, 17023,  -920,
    5342, 12050,  -920,  -920,  -920, 17764,  -920,   770,   513, 11360,
    5342, 11360, 10154,  -920,  -920,  -920,  -920,  -920,   453,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  6899,  -920,  -920, 16238, 15322,   369,   753,  6899,
    -920, 12050,  -920,  -920, 14249,   373,   385, 16341, 12050,   299,
    -920,  5342,   -33, 15396,  -920,   729,  -920, 17023,  -920,  5342,
   11015,  -920, 17023,  -920, 16389, 17023,  -920,  9983, 17023,    42,
   14992,  -920,  -920,  -920,   305,  -920,  -920,  -920,  -920, 17260,
   17307,  -920, 17023, 17764,   627,  -920,     1,   350, 12050,  -920,
     772,   744,  -920,   634,   634,  -920,  -920,  -920,   745, 16429,
   12050,   888, 16580,   316,  -920,   648,   318,   330,  -920,  -920,
   17023,   887,  5342,  -920,  -920,  -920,  8441,  8441,  -920,  -920,
    -920,  -920,  -920,   298, 17023,  -920,   134,    81,   751, 12050,
    -920,   -30, 15441,    31, 15072,  -920,   303, 12050,  6899,  6728,
    -920,  -920,  -920, 16620, 17023, 14296,  -920,  -920,  -920,  -920,
    3256,  -920,  -920,  5690,   750,  5864,   754,  -920, 12050, 14324,
    6038,  -920,    65,  6212,  -920,    65,   761, 17307,  -920,  -920,
    -920, 17646,  -920,  -920,  -920,   371,  -920,   -39,  -920,   521,
   16661,    67,  -920,  -920,  -920, 12563, 12734, 16776, 12905, 13076,
    -920,  -920,  5342,  -920,   755,   757,  -920,  -920,  5342,  -920,
     513, 17023,  5342,  5342,  -920, 17023,   390,   762, 12050,  -920,
    -920,  9983,  -920, 17023,  5342,  9983,  -920, 17023,  5342, 17023,
    5342,  9983, 17023,  -920,  9983, 17023,  -920,  -920,  -920,  -920,
      69,  -920, 12050,   350,   371,   371,   371,   371,   371,   371,
     350,  5168,  -920,  -920,  -920,  -920, 12050, 12050, 12050, 12050,
   12050,  -920,   767,   771,   671,  -920,  -920,  -920, 17023, 14404,
    -920, 14484,  -920, 15991, 14564, 14644,  -920, 17023,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,   787,  -920, 16824, 16864, 16943,
   16983,  -920,  -920,  5342,  5342,  5342,  5342, 12050, 12050, 12050,
   12050, 15991, 15991, 15991, 15991
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
     485,   163,   312,   566,   496,   171,   209,   745,   209,   747,
     209,   748,   749,   636,   209,   746,   209,   644,   179,   744,
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
     303,   730,   624,   621,   622,   623,   500,   565,   731,     7,
     209,   335,   335,   430,   173,   425,   174,   426,   427,   431,
     432,   172,   433,   434,   423,   529,     0,   340,   341,   343,
       0,   424,   528,   345,   516,     0,   209,   209,   177,   648,
     639,   662,   640,     0,     0,    43,     0,   569,     0,   209,
      44,   563,     0,   209,   282,    48,   286,   283,   286,   567,
       0,   732,   734,   634,   729,   728,     0,    74,    78,     0,
       0,   209,     0,     0,   542,   635,     0,     6,   310,     0,
     197,   209,   311,     0,    49,     0,     9,    71,    50,    53,
       0,    60,     0,   209,    72,   209,     0,   498,   195,     0,
     687,   313,   659,   204,   215,     0,   212,   213,   214,   209,
     209,   209,   211,   574,   582,     0,   253,     0,   209,     0,
     309,     0,   423,   449,   447,   448,   446,   361,   451,   456,
     459,   450,   454,   453,   455,   458,     0,   439,   441,   445,
     444,   436,   437,   440,   443,     0,   490,     0,   487,     0,
       0,   643,    34,   625,   209,   209,   209,   209,   209,   209,
     733,   209,   209,   209,   209,   209,     0,   633,   209,   209,
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
     725,   724,   727,   736,   735,   740,   739,   721,   718,   719,
     720,   637,   708,   139,     0,   677,   678,   141,   676,   675,
     631,   712,   723,   717,   715,   726,   716,   714,   706,   711,
     713,   722,   705,   709,   710,   707,   632,     0,     0,     0,
       0,     0,     0,     0,     0,   738,   737,   742,   741,   209,
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
       0,     0,   209,   209,   540,     0,   535,     0,   538,   540,
     699,   537,   209,   209,   701,     0,   226,     0,     0,   209,
     224,     0,     0,     0,   277,   209,     0,   335,   321,   327,
     209,   325,   320,   423,   411,   460,   519,   518,   517,   520,
     461,   467,   423,   360,     0,   366,     0,   356,   357,   491,
     488,     0,     0,   127,   125,   126,   124,   123,   122,   672,
     673,     0,   209,   697,   209,   209,   209,   209,   603,   612,
     614,   209,   601,   160,   167,   166,   165,   162,   169,   170,
     164,   168,   163,   171,     0,   209,   209,   499,   549,   548,
     250,   249,     0,   244,     0,   209,   240,     0,   246,     0,
     279,   509,   512,   513,   514,   515,   301,     0,     0,     0,
     513,   209,   460,   531,   530,   209,   419,   417,   352,   295,
       0,   209,   294,   297,   561,     0,   288,   291,     0,   209,
       0,   209,   209,    59,   120,   117,    98,   109,   104,   103,
     102,   111,    99,   112,   107,   101,   108,   105,   106,   100,
     110,   113,   209,   116,   115,    88,    87,     0,     0,   209,
     193,   209,   194,   315,   335,     0,     0,     0,   209,     0,
     234,     0,     0,     0,   698,   536,   232,   585,   541,     0,
     209,   700,   703,   702,     0,   578,   227,   209,   577,     0,
       0,   225,   255,   252,     0,   308,   183,   181,   307,   335,
     335,   316,   331,     0,     0,   413,   467,     0,   209,   354,
     415,     0,   363,   408,   408,   679,   680,   279,     0,     0,
     209,     0,     0,     0,   606,   513,   663,   663,   243,   239,
     590,   245,     0,    73,   276,   507,   209,   209,   501,   420,
     418,   347,   348,   513,   407,   299,     0,     0,     0,   209,
     289,     0,     0,     0,     0,    62,     0,   209,   209,   209,
      93,    95,    84,    83,    82,   335,   201,   207,   205,   216,
       0,   236,   235,     0,     0,     0,     0,   233,   209,     0,
       0,   222,     0,     0,   220,     0,   281,   335,   318,   317,
     326,     0,   333,   481,   482,     0,   483,   469,   472,     0,
     468,     0,   409,   358,   359,   209,   209,     0,   209,   209,
     281,   607,     0,   248,     0,     0,   422,   421,     0,   298,
       0,   292,     0,     0,   118,    89,     0,     0,   209,   202,
     219,   209,   230,   587,     0,   209,   228,   586,     0,   704,
       0,   209,   576,   223,   209,   575,   221,   278,   319,   329,
       0,   480,   209,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   462,   464,   355,   463,   209,   209,   209,   209,
     209,   247,   626,   627,   300,   290,    94,    96,    85,     0,
     231,     0,   229,   581,     0,     0,   334,   471,   473,   474,
     477,   478,   479,   475,   476,   470,   465,     0,     0,     0,
       0,   628,   629,     0,     0,     0,     0,   209,   209,   209,
     209,   589,   588,   580,   579
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -920,  -920,  -920,     0,   -57,  1357,  -920,   224,   826,  -920,
    -920,   494,  -391,  -109,  -920,   359,  -920,  -920,   -53,  -920,
    -920,    19,   676,  -920,  -544,  2704,   566,  -575,  -920,  -805,
    -920,  -920,  -920,   116,  -920,  -920,  -920,   918,  -920,  1800,
    -200,  -920,  -920,  -438,   -34,  -920,  -854,  -920,  -920,   288,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,    17,
    -920,  -157,   581,  -332,  -920,   559,  -920,  -920,  -920,    72,
    1644,  -920,  -920,  -920,  -920,   611,  -920,   -28,  -920,  -920,
    -920,  -920,   430,  -920,  -920,  -920,   -92,  -920,  -345,  -803,
    -920,  -920,  -920,  -920,  -920,  -616,  -920,   112,  -920,  -920,
    -920,  -920,  -920,   418,  -920,   -61,  -920,  -920,  -920,  -920,
     580,  -920,  -920,  -920,  -920,    54,  -407,  -174,  -791,  -919,
    -660,  -920,    66,  -920,    80,   -50,   666,   -46,  -920,  -920,
    -351,  -837,  -920,  -235,  -920,   -87,  -240,  -232,  -457,  -920,
    -920,   130,   297,  -920,   454,  -202,   855,  -920,  -920,  -148,
     463,   215,  -415,  -767,  -608,  -920,  -920,  -920,  -601,  -506,
    -920,  -754,   -23,  -920,  -920,  -576,    79,  -920,  -256,  -486,
    -565,    -2,  -920,  -920,  -920,  -920,  -920,  -920,   185,  -920,
     971,  -336,  -920,    62,  -920,  -920,   438,   537,  -920,  -920,
    -920,  -920,  2602,  -920,   -41,   637,  -920,  1172,  1378,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -479
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   348,   104,   105,   106,   107,   108,   365,
     366,   109,   110,   111,   112,   371,   580,   820,   821,   113,
     114,   115,   376,   116,   117,   118,   349,   848,   587,   849,
     119,   120,  1013,   584,   585,   121,   122,   219,   254,   271,
     123,   256,   124,   305,   534,   125,   126,   292,   291,   127,
     128,   129,   130,   131,   596,   132,   597,   852,   133,   203,
     135,   391,   392,   605,   136,   260,   137,   138,   614,   615,
     204,   140,   141,   142,   143,   560,   807,   988,   144,   145,
     803,   983,   278,   279,   146,   147,   148,   149,   399,   887,
     150,   151,   622,  1063,  1064,   623,   152,   205,  1210,   154,
     155,   306,   307,   308,   972,   156,   320,   552,   800,   157,
     158,  1161,   159,   160,   679,   680,   844,   845,   846,   976,
     903,   403,   625,  1071,   626,   627,   310,   542,   425,   407,
     416,   901,  1224,  1225,   161,  1069,  1157,  1158,  1159,   162,
     163,   427,   428,   685,   164,   165,   166,   223,   535,   272,
     273,   520,   521,   782,   312,   899,   633,   634,   524,   313,
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
     167,   369,   103,   227,   889,   304,   319,   480,   801,   514,
     620,   276,   309,   759,   822,   843,   311,   430,   681,   134,
     907,   496,   908,   167,   898,   319,   323,   895,  1018,   500,
     775,   977,   871,  1061,   536,   325,   594,  1014,   970,   381,
     319,   799,   134,   315,   522,   340,   489,   491,  1024,   802,
    -274,  1099,   628,   384,   264,   525,   875,  -328,  1066,   611,
     354,   385,  1067,   274,   274,   500,  -275,   980,   551,   386,
     387,   388,   167,   557,  1140,   507,   167,   167,  1212,   631,
     703,   373,    23,   214,   873,   779,   214,   512,   539,   214,
     500,   134,   629,   522,   217,   134,   134,   274,   167,  1221,
       3,  1221,   167,   776,   498,   693,   695,  1213,   981,   384,
    -522,   440,   874,   274,   153,  -349,   194,   134,  1068,   446,
     374,   134,   499,  -296,  -524,   487,   387,   388,   367,   500,
     795,   198,   540,   522,    56,  1102,   630,   153,   860,  -522,
    -257,  1133,   363,   214,  1182,   218,    65,   389,   375,   214,
     214,   735,   737,  -524,   526,   384,   922,   742,   384,   570,
    -258,   214,   982,   385,  -296,   229,  1178,  -524,   319,    84,
    1061,   386,   387,   388,   386,   387,   388,   963,   529,  -524,
     707,   265,  -264,  -522,  1176,   571,   153,   172,   523,  -524,
     153,   153,   793,   363,   897,  -522,  1095,  -524,  -324,   949,
     363,   363,   390,  1100,   319,  1148,  1149,   862,   576,  -524,
     172,   363,   153,   167,   777,  -524,   153,   492,  -296,  1103,
    1125,   436,  -324,   632,  -263,   733,   734,   498,  1122,   319,
     358,  1222,   134,   222,   363,   498,   363,   523,   363,   363,
     778,   395,  -522,   528,   816,   530,   709,  -522,   850,   389,
     363,   682,   389,   548,   921,   863,  -524,  1017,   488,   172,
     224,  -524,   257,   172,   172,   635,   929,   167,   319,   861,
     867,  1061,   167,   864,   989,   877,   881,   523,  -524,   214,
     437,   500,   500,   923,   438,   172,   134,  1035,   593,   172,
     974,   134,   555,  1208,   882,   893,  -524,   362,   229,   566,
     804,   404,   334,   337,   575,   293,   863,   390,  -596,   636,
     930,   703,   282,   931,  1187,  -670,   500,  -670,  1171,    82,
     883,   894,   405,   567,   864,   295,   764,   153,   704,   297,
     440,  -272,   406,   790,   865,   444,  1177,   686,   446,   932,
    -262,   705,   449,  1085,   293,   304,   214,   167,   760,   565,
     777,  -259,   960,  1153,   298,   687,   500,   299,   777,    58,
     526,   167,   198,  1101,   295,   265,   134,   606,   297,   300,
     167,   706,   283,   498,  1153,   617,   961,   -70,   689,   500,
     134,   153,   266,   167,   973,   825,   153,   302,   891,   134,
     483,   284,   303,   298,   484,   853,   299,   -70,    58,  1054,
     172,   990,   134,   826,  1154,   481,   285,   482,   300,   319,
     319,   707,   892,  1163,  1164,   319,   -70,   708,   319,   319,
     319,   319,   781,   683,   869,  1154,   302,   822,  -266,   728,
     729,   303,   992,    37,   870,   730,  -335,  1116,   731,   732,
     175,  1083,   764,  1155,   -70,   498,   -69,  1150,   975,  1019,
     624,   764,   759,  1156,   172,   319,   289,  -335,   993,   172,
      54,   153,   764,   175,  -609,   741,   -69,  -335,  1077,   262,
    -270,  1131,   263,  1184,  1156,   153,  -608,  1146,  -267,   867,
     877,   881,   436,   829,   153,   -69,   526,   304,  1170,  -414,
    -609,   290,   595,   884,   309,   319,  1060,   153,   311,   681,
     885,   830,  -608,  1019,   947,   167,   946,   167,   319,   319,
     484,   369,   175,   -69,   526,  1019,   175,   175,   319,   686,
    -671,  1020,  -671,  -747,   134,  -261,   134,  -747,   812,   812,
    1032,   526,  1098,  1120,   172,  1209,  1019,  1127,   175,   176,
     500,   437,   175,  1186,   703,   438,  -748,  1049,   172,  1128,
    -748,   440,   815,  -670,  1236,  -670,   444,   172,   933,   446,
     328,   934,   176,   449,   500,   194,   935,   856,   857,   194,
     172,   563,   335,   335,  -265,   823,   196,   876,   880,   537,
     538,   167,   293,   565,   868,  -749,   936,  -271,  -744,  -749,
    1078,   440,  -744,   937,  1081,   198,   444,   329,   445,   446,
     134,  -273,   295,   449,   938,  -667,   297,  -667,   436,  -260,
     456,   176,   939,   693,   735,   176,   176,  1223,   462,   153,
    -669,   153,  -669,  -668,   911,  -668,   912,   995,   709,   940,
    -268,   298,     7,  1060,   299,   370,   356,   176,   397,   181,
     941,   176,   969,   372,   707,  1214,   300,   398,   811,   814,
     401,   942,  -593,   175,   402,   431,   943,  1111,   490,  1113,
    -592,   432,   181,  -597,   302,  -598,  1223,   437,  1147,   303,
    -595,   438,  1215,  1151,  1216,  1217,  -594,  1218,  1219,   319,
     494,   497,   363,   374,   515,   500,   436,   543,   544,   547,
     516,   553,   172,   532,   172,   153,  1134,  1136,  1211,   408,
     533,   319,   562,   319,   556,  1026,   569,   175,   572,   573,
     578,   181,   175,  1142,  1145,   181,   181,   440,   441,   409,
     319,   948,   444,   410,   445,   446,   589,   500,   579,   449,
     600,   618,  1074,   684,  1060,   688,   456,   181,   446,   727,
     965,   181,   460,   461,   462,   437,   740,   762,   411,   438,
     763,   412,   176,   436,   764,   944,   765,  1249,  1250,  1251,
    1252,  1253,  1254,   413,   766,   768,   167,   771,   172,   167,
     773,   774,   781,   791,   796,  1134,  1136,  1142,  1145,   805,
     806,   414,   876,   880,   851,   134,   415,   175,   134,   854,
     858,   498,   500,   890,   888,   440,   441,   900,   442,   443,
     444,   175,   445,   446,   447,   902,   176,   449,   905,  -139,
     175,   176,   437,   455,   456,  -160,   438,   459,   913,   919,
     460,   461,   462,   175,   319,  -167,   417,  -166,  -165,  -162,
    1059,   463,  -140,  -169,  -170,  1029,   304,  -164,  -168,  -141,
    1174,  1175,  -163,  1046,  -171,   304,   418,  1051,   920,   906,
     419,   914,   181,   924,   927,   971,   952,   915,   167,   916,
     928,   917,   440,   167,   954,  1019,   319,   444,   979,   445,
     446,   984,   918,   167,   449,   420,   167,   134,   421,  1022,
     153,   456,   134,   153,  1028,  1023,   176,   460,   461,   462,
     422,  1031,   134,  1038,  1039,   134,   500,   500,  1040,  1041,
     176,  1076,  1055,  1092,  1093,  1094,   181,  1075,   423,   176,
    1096,   181,   293,   424,  1097,  1109,  1162,  1121,   893,  1165,
    1168,  1172,   176,  1180,  1194,  1207,  1237,  1232,  1198,  1233,
    1261,   319,   295,  1213,  1262,   364,   297,   818,   583,  1115,
     513,   994,   225,  1086,  1087,   175,   577,   175,   601,   561,
     167,   369,   167,   172,  1052,   167,   172,  1108,  1107,   785,
    1073,   298,   794,   598,   299,   512,    58,  1059,  1070,   134,
     541,   134,   153,  1065,   134,  1246,   300,   153,   167,  1152,
    1255,  1248,  1025,   910,   378,   787,   181,   153,   167,   784,
     153,   221,   962,  1084,   302,     0,     0,  1106,     0,   303,
     181,     0,     0,     0,     0,     0,     0,   134,     0,   181,
       0,     0,     0,     0,     0,  1141,  1144,     0,     0,     0,
       0,   175,   181,     0,     0,     0,     0,     0,     0,   167,
       0,     0,     0,     0,     0,   319,   319,   167,   369,     0,
       0,     0,   369,     0,   176,   172,   176,     0,   134,     0,
     172,     0,     0,     0,     0,     0,   134,     0,     0,     0,
     172,     0,     0,   172,   153,   512,   153,   512,  1059,   153,
     512,   512,     0,  1179,     0,     0,     0,     0,     0,  1141,
    1144,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     167,     0,   153,     0,     0,     0,  1190,     0,     0,     0,
       0,     0,   153,     0,     0,     0,     0,     0,  1203,   134,
       0,  1206,     0,     0,     0,     0,     0,     0,     0,     0,
     176,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   167,     0,   167,     0,     0,     0,   172,   167,   172,
       0,   167,   172,   153,   181,     0,   181,     0,     0,     0,
     134,   153,   134,     0,     0,     0,     0,   134,     0,     0,
     134,     0,     0,     0,     0,   172,     0,     0,     0,     0,
     167,     0,     0,     0,   183,   172,   167,     0,     0,     0,
     167,   167,     0,     0,     0,     0,     0,     0,     0,   134,
       0,     0,   167,     0,     0,   134,   167,   183,   167,   134,
     134,     0,     0,     0,   153,     0,   175,     0,     0,   175,
       0,   134,     0,     0,     0,   134,   172,   134,     0,   167,
     181,  1256,     0,     0,   172,     0,   436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   134,     0,
       0,     0,     0,     0,     0,   153,   183,   153,     0,     0,
     183,   183,   153,     0,     0,   153,     0,     0,     0,     0,
       0,   167,   167,   167,   167,     0,     0,     0,     0,     0,
       0,     0,   183,     0,     0,     0,   183,   172,     0,     0,
     134,   134,   134,   134,   153,   437,     0,     0,     0,   438,
     153,     0,     0,     0,   153,   153,     0,     0,   175,     0,
       0,     0,     0,   175,     0,   176,   153,     0,   176,     0,
     153,     0,   153,   175,     0,     0,   175,     0,   172,     0,
     172,     0,     0,     0,     0,   172,     0,     0,   172,     0,
       0,     0,     0,   153,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,     0,     0,     0,   449,     0,     0,
       0,     0,     0,     0,   456,     0,     0,   172,     0,     0,
     460,   461,   462,   172,     0,     0,     0,   172,   172,     0,
       0,     0,     0,     0,     0,   153,   153,   153,   153,   172,
     184,     0,   226,   172,     0,   172,     0,   183,     0,     0,
     175,     0,   175,     0,     0,   175,     0,   176,     0,     0,
       0,     0,   176,   184,     0,   181,   172,     0,   181,     0,
       0,     0,   176,     0,     0,   176,     0,     0,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   175,     0,
       0,   330,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   183,     0,     0,     0,     0,   183,     0,   172,   172,
     172,   172,   184,     0,     0,     0,   184,   184,     0,     0,
       0,   357,     0,     0,     0,     0,     0,     0,     0,   175,
       0,     0,     0,     0,     0,     0,     0,   175,   184,     0,
       0,     0,   184,     0,     0,     0,     0,     0,     0,   176,
     429,   176,   230,     0,   176,     0,     0,   181,     0,     0,
       0,     0,   181,     0,     0,     0,   231,   232,     0,   233,
       0,     0,   181,     0,   234,   181,     0,   176,     0,     0,
       0,   183,   235,     0,     0,     0,     0,   176,   236,     0,
     175,     0,     0,     0,   237,   183,     0,     0,   238,     0,
       0,   239,     0,   240,   183,     0,     0,     0,     0,     0,
       0,     0,   241,     0,     0,     0,     0,   183,     0,   242,
     243,     0,     0,     0,     0,     0,     0,   244,   176,     0,
       0,   175,   493,   175,     0,     0,   176,   245,   175,     0,
       0,   175,     0,     0,     0,     0,   246,     0,   247,   181,
     248,   181,   249,   184,   181,   250,     0,     0,     0,   251,
       0,   252,     0,     0,   253,     0,     0,     0,     0,     0,
     175,     0,     0,     0,     0,     0,   175,   181,     0,     0,
     175,   175,     0,     0,     0,     0,   517,   181,     0,   176,
       0,   527,   175,     0,     0,     0,   175,     0,   175,     0,
       0,     0,     0,     0,     0,     0,   139,   184,     0,     0,
       0,     0,   184,     0,     0,     0,     0,     0,     0,   175,
     270,     0,     0,     0,     0,     0,     0,     0,   181,   139,
     176,     0,   176,     0,     0,     0,   181,   176,     0,   183,
     176,   183,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   175,   175,   175,   175,     0,     0,     0,     0,   176,
       0,     0,     0,     0,     0,   176,     0,     0,   139,   176,
     176,     0,     0,     0,     0,     0,     0,   184,     0,   181,
       0,   176,     0,     0,     0,   176,     0,   176,     0,     0,
       0,   184,   599,     0,   139,     0,     0,     0,   139,     0,
     184,     0,     0,     0,     0,   183,     0,     0,   176,     0,
       0,     0,     0,   184,     0,     0,     0,     0,     0,     0,
     181,     0,   181,     0,     0,     0,     0,   181,     0,     0,
     181,     0,     0,   464,   465,   466,   467,   468,   469,   470,
     471,   472,   473,   474,   475,   476,   477,   478,     0,     0,
     176,   176,   176,   176,     0,     0,     0,     0,     0,   181,
       0,     0,     0,     0,     0,   181,     0,     0,     0,   181,
     181,     0,     0,     0,     0,     0,     0,   479,   255,     0,
    -667,   181,  -667,     0,     0,   181,     0,   181,     0,     0,
     267,   268,   269,     0,     0,     0,   886,     0,     0,     0,
    -315,     0,     0,     0,     0,     0,     0,     0,   181,   139,
       0,     0,     0,     0,   767,     0,   769,  -315,     0,     0,
       0,  -315,  -315,     0,     0,     0,     0,     0,     0,     0,
    -315,     0,  -315,  -315,     0,   184,     0,   184,  -315,     0,
       0,     0,     0,     0,     0,  -315,     0,     0,  -315,     0,
     181,   181,   181,   181,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   139,     0,     0,     0,  -315,   139,  -315,
       0,     0,  -315,     0,  -315,     0,  -315,     0,  -315,  -315,
       0,  -315,     0,     0,  -315,     0,  -315,     0,     0,     0,
     183,     0,     0,   183,     0,     0,     0,     0,     0,   400,
       0,     0,     0,     0,  -315,     0,  -315,     0,     0,  -315,
       0,   184,     0,   426,     0,     0,     0,     0,   230,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   231,   232,     0,   233,     0,     0,     0,     0,
     234,     0,     0,   139,     0,     0,     0,     0,   235,   486,
       0,     0,     0,     0,   236,     0,     0,   139,     0,  -315,
     237,     0,     0,     0,   238,  -315,   139,   239,     0,   240,
       0,     0,     0,     0,     0,     0,     0,     0,   241,   139,
       0,     0,   183,     0,     0,   242,   243,   183,     0,     0,
       0,     0,     0,   244,     0,     0,     0,   183,     0,     0,
     183,     0,     0,   245,     0,     0,     0,     0,   505,   506,
       0,     0,   246,     0,   247,     0,   248,     0,   249,     0,
     519,   250,   230,     0,     0,   251,     0,   252,     0,     0,
     253,     0,     0,     0,     0,     0,   231,   232,     0,   233,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,     0,     0,     0,     0,     0,   236,     0,
     546,     0,     0,     0,   237,     0,     0,     0,   238,     0,
       0,   239,     0,   240,   183,   953,   183,     0,   956,   183,
       0,     0,   241,     0,   363,     0,   985,     0,     0,   242,
     243,     0,     0,     0,     0,     0,   184,   244,     0,   184,
       0,   139,   183,   139,     0,     0,     0,   245,     0,   255,
       0,   436,   183,     0,     0,     0,   246,     0,   247,     0,
     248,     0,   249,     0,     0,   250,     0,     0,     0,   251,
       0,   252,     0,     0,   253,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   613,     0,   616,     0,     0,
       0,   621,     0,   183,     0,     0,     0,     0,     0,     0,
       0,   183,     0,     0,     0,     0,     0,  1030,     0,     0,
     437,     0,  1036,     0,   438,   678,     0,   139,     0,     0,
       0,     0,   527,     0,     0,   527,     0,     0,   184,     0,
     270,     0,     0,   184,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   184,     0,     0,   184,     0,     0,     0,
       0,     0,     0,     0,   183,     0,     0,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,     0,   449,   450,   451,     0,   453,   454,   455,   456,
     744,     0,   459,     0,     0,   460,   461,   462,     0,     0,
       0,     0,     0,     0,     0,   183,   463,   183,     0,  1088,
       0,  1089,   183,     0,  1091,   183,     0,     0,   519,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     184,   786,   184,     0,     0,   184,     0,  1105,     0,     0,
       0,   792,     0,     0,   183,   519,     0,  1036,     0,     0,
     183,     0,     0,     0,   183,   183,     0,     0,   184,     0,
       0,     0,     0,     0,     0,   230,   183,     0,   184,     0,
     183,     0,   183,     0,     0,     0,     0,     0,     0,   231,
     232,     0,   233,     0,     0,     0,     0,   234,  1132,   834,
       0,     0,     0,   183,     0,   235,  1137,     0,     0,     0,
     855,   236,     0,     0,     0,     0,     0,   237,     0,   184,
       0,   238,   139,     0,   239,   139,   240,   184,     0,     0,
     621,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,   242,   243,     0,   183,   183,   183,   183,   358,
     244,     0,     0,    22,    23,     0,     0,     0,     0,  1173,
     245,     0,   359,     0,    29,   360,     0,     0,     0,   246,
      32,   247,     0,   248,     0,   249,     0,    37,   250,     0,
     184,     0,   251,     0,   252,     0,     0,   253,     0,     0,
       0,     0,     0,     0,     0,     0,   426,     0,     0,   361,
    1192,     0,  1196,     0,    54,     0,    56,   527,     0,     0,
     527,     0,     0,     0,   139,     0,   362,     0,    65,   139,
       0,   184,     0,   184,     0,     0,     0,     0,   184,   139,
       0,   184,   139,     0,     0,     0,    80,     0,    82,  1231,
       0,    84,     0,   518,     0,  1234,     0,     0,     0,  1192,
    1196,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     184,  1240,     0,     0,     0,  1242,   184,   769,     0,     0,
     184,   184,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   184,   959,     0,     0,   184,   519,   184,     0,
       0,     0,     0,     0,     0,     0,     0,   966,     0,     0,
       0,     0,     0,     0,     0,     0,   139,     0,   139,   184,
       0,   139,     0,     0,   436,     0,   987,     0,     0,     0,
       0,     0,     0,     0,     0,   210,     0,     0,     0,   220,
     769,   769,   769,   769,   139,     0,     0,     0,   228,     0,
       0,     0,     0,     0,   139,   261,     0,     0,     0,     0,
       0,   184,   184,   184,   184,     0,     0,   277,     0,   280,
     281,     0,   426,     0,     0,   286,     0,   287,     0,   288,
       0,     0,     0,   437,   314,     0,     0,   438,   228,   322,
     324,   326,   327,     0,     0,   139,   331,     0,   332,     0,
       0,     0,   613,   139,     0,   339,     0,     0,   322,     0,
     341,     0,   342,     0,     0,   343,   344,     0,   345,     0,
       0,     0,   228,   322,   355,     0,   678,     0,     0,     0,
       0,     0,     0,   440,   441,     0,     0,   443,   444,     0,
     445,   446,     0,     0,     7,   449,     0,     0,     0,     0,
       0,   379,   456,   382,     0,     0,   139,     0,   460,   461,
     462,   358,     0,     0,     0,    22,    23,     0,     0,   275,
       0,     0,     0,     0,   359,     0,    29,   360,     0,     0,
     436,     0,    32,     0,     0,     0,     0,     0,     0,    37,
       0,     0,     0,   275,     0,     0,   433,   139,     0,   139,
       0,     0,   336,   338,   139,   987,     0,   139,     0,     0,
       0,   361,     0,     0,     0,     0,    54,     0,    56,   485,
      58,     0,  1056,   331,   228,  1057,     0,     0,   362,     0,
      65,     0,     0,   368,     0,     0,   139,     0,     0,   437,
       0,     0,   139,   438,     0,     0,   139,   139,    80,     0,
      82,   322,     0,    84,     0,     0,     0,     0,   139,     0,
       0,   396,   139,     0,   139,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   621,     0,   139,     0,   322,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   434,     0,
       0,   449,     0,    98,     0,   435,     0,   455,   456,  1058,
       0,   459,   322,     0,   460,   461,   462,   436,   878,     0,
       0,     0,     0,   331,     0,     0,     0,   139,   139,   139,
     139,     0,     0,   879,     0,     0,     0,     0,   549,   550,
       0,     0,     0,     0,     0,   495,     0,     0,     0,     0,
       0,   322,     0,     0,     0,   559,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   621,     0,   568,     0,     0,   437,     0,     0,     0,
     438,     0,     0,   574,   508,   511,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   586,     0,   588,     0,     0,
       0,   275,     0,   275,     0,   214,     0,     0,     0,   439,
     275,   602,   609,   612,     0,     0,     0,     0,     0,     0,
     228,     0,     0,     0,     0,     0,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,     0,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
     275,   460,   461,   462,     0,   363,   690,   691,   692,   694,
     696,   697,   463,   698,   699,   700,   701,   702,     0,   710,
     711,   712,   713,   714,   715,   716,   717,   718,   719,   720,
     721,   722,   723,   724,   725,   726,     0,     0,     0,     0,
       0,     0,   322,   322,     0,     0,     0,     0,   322,     0,
       0,   322,   322,   322,   322,   736,   738,     0,     0,     0,
     609,   743,     0,     0,   758,     0,     0,     0,     0,     0,
     761,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   770,     0,     0,   772,     0,     0,     0,   322,     0,
       0,     0,     0,     0,     0,   780,     0,   783,     0,     0,
       0,   280,     0,     0,   788,   789,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   314,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,     0,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   761,     0,
       0,   322,   322,     0,     0,     0,     0,   228,   817,     0,
       0,   322,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   847,   847,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   859,     0,     0,   228,
       0,     0,     0,     0,     0,     0,     0,     6,     0,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
       0,    17,     0,     0,   314,   195,     0,  -209,     0,     0,
     293,     0,     0,     0,     0,     0,     0,     0,   196,     0,
       0,     0,     0,  -209,  -209,  -209,     0,   197,     0,     0,
     295,     0,     0,   275,   297,     0,     0,     0,   434,   199,
       0,     0,    43,    44,     0,   435,     0,     0,    48,   904,
      49,    50,     0,     0,     0,     0,   909,   436,   274,   298,
       0,    55,   299,    57,    58,    59,     0,     0,     0,     0,
      62,     0,   200,     0,   300,   275,   275,     0,     0,     0,
      70,    71,     0,    73,     0,   275,   275,     0,     0,     0,
       0,     0,   302,     0,     0,    83,     0,   303,     0,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,   770,   322,     0,     0,     0,   437,     0,     0,     0,
     438,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,   322,     0,   322,   950,   951,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,   439,
      99,     0,   202,   322,   101,  -339,  -209,     0,     0,   102,
       0,     0,     0,     0,     0,     0,   440,   441,   314,   442,
     443,   444,   978,   445,   446,   447,     0,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
       0,   460,   461,   462,     0,   363,     0,   586,     0,     0,
       0,  1015,   463,  1016,   847,   847,     0,   341,     0,   342,
       0,     0,   344,   345,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1027,     0,     0,     0,
     228,     0,     0,     0,  1033,   761,     0,  1037,     0,     0,
       0,     0,     0,     0,  1042,  1044,   958,  1045,     0,     0,
    1048,  1050,     0,     0,     0,     0,     0,   322,     0,     0,
       0,     0,  1062,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     986,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     275,   275,     0,     0,  1079,  1037,  1045,  1048,  1082,   322,
       0,     0,     0,   758,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   694,   736,     0,
     230,     0,     0,     0,     0,     0,     0,  1090,     0,     0,
       0,     0,     0,     0,   231,   232,     0,   233,     0,     0,
       0,     0,   234,   314,     0,     0,     0,  1104,     0,     0,
     235,   275,     0,   228,     0,   275,   236,  1053,     0,     0,
       0,  1112,   237,  1114,   322,     0,   238,     0,     0,   239,
       0,   240,     0,     0,     0,     0,     0,     0,     0,     0,
     241,     0,     0,     0,   847,     0,     0,   242,   243,     0,
       0,  1123,     0,  1124,     0,   244,     0,     0,     0,     0,
    1130,     0,     0,     0,     0,   245,     0,     0,     0,     0,
       0,     0,   761,     0,   246,     0,   247,     0,   248,  1139,
     249,     0,     0,   250,     0,     0,     0,   251,     0,   252,
       0,     0,   253,     0,     0,     0,     0,     0,     0,     0,
    1160,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1167,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1110,     0,     0,     0,     0,     0,   322,   322,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1181,     0,     0,     0,     0,     0,     0,   545,  1185,
     847,   847,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1193,     0,  1197,     0,     0,
    1199,     0,  1202,     0,     0,  1205,     0,     0,     0,     0,
       0,     0,     0,   275,   275,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,  1193,  1197,     0,
    1202,  1205,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
    1238,     0,     0,  1239,     0,     0,     0,  1241,     0,     0,
       0,     0,  1243,  1244,     0,     0,  1245,     0,     0,     0,
       0,   275,     0,     0,  1247,     0,     0,   275,   275,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1257,  1258,
    1243,  1259,  1260,     0,   275,     0,     0,     0,     0,     0,
       0,     0,     0,   511,   437,     0,   275,     0,   438,   275,
       0,     0,     0,     0,     0,     0,   509,     0,     0,     0,
       0,     0,     0,     0,     0,  1271,  1272,  1273,  1274,  1271,
    1272,  1273,  1274,     0,     0,     0,     0,   439,   510,   465,
     466,   467,   468,   469,  1235,     0,   472,   473,   474,   475,
       0,   477,   478,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   363,     0,     0,     0,     0,     0,     0,
     463,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   511,     0,   511,     0,     0,   511,   511,
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
    -209,  -209,  -209,    32,  1191,    34,   -68,     0,    35,    36,
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
       0,    29,    30,    31,  -209,  -209,  -209,    32,  1195,    34,
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
    -209,    32,  1201,    34,   -68,     0,    35,    36,    37,     0,
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
      30,    31,  -209,  -209,  -209,    32,  1204,    34,   -68,     0,
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
      99,     0,   202,     0,   101,   754,  -209,     0,   896,   102,
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
      59,     0,     0,     0,     0,    62,    63,   200,  -743,     0,
       0,     0,  -743,     0,     0,    70,    71,    72,    73,   201,
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
       0,     0,    46,     0,    48,     0,    49,    50,   964,    51,
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
       0,     0,    62,    63,   200,  -743,     0,     0,     0,  -743,
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
       0,   101,     0,     0,  1034,     0,   102,     5,     6,     0,
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
       0,     0,    31,     0,     0,     0,     0,  1226,    34,     0,
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
       0,     0,     0,    31,     0,     0,     0,     0,  1227,    34,
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
     196,     0,     0,     0,    31,     0,     0,     0,     0,  1229,
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
    1230,    34,     0,     0,     0,    36,     0,     0,    38,     0,
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
       0,     0,     0,     0,     0,     0,     0,     0,   637,     0,
     638,     0,    88,    89,    90,    91,    92,     0,     0,   639,
       0,    93,     0,     0,   640,   232,   641,   642,     0,     0,
      94,     0,   643,    95,    96,    97,     0,     0,     0,    99,
     235,   202,   196,   101,     0,  -209,   236,     0,   102,     0,
       0,     0,   644,     0,     0,     0,   238,     0,     0,   645,
       0,   646,   647,     0,     0,     0,     0,     0,     0,     0,
     648,     0,     0,     0,     0,     0,     0,   242,   649,     0,
       0,     0,     0,     0,     0,   244,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   650,     0,     0,     0,     0,
       0,     0,     0,     0,   246,     0,   651,     0,   248,     0,
     652,     0,     0,   250,     0,     0,     0,   653,     0,   252,
       0,     0,   654,     0,     0,     0,     0,     0,     0,     0,
       0,   510,   465,   466,   467,   468,   469,     0,     0,   472,
     473,   474,   475,     0,   477,   478,     0,   655,   656,   657,
     658,   659,   660,     0,   661,     0,     0,     0,   662,   663,
     664,   665,   666,   667,   668,   669,   670,   671,     0,   672,
       0,     0,   673,   674,   675,   676,     5,     6,   677,     8,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,    96,    97,     0,     0,     0,
      99,     0,   202,     0,   101,     0,  -209,     5,     6,     0,
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
       0,     0,     0,     0,    88,     0,    90,     0,     0,     5,
       6,     0,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,    97,   195,     0,
    -209,    21,     0,   202,     0,   101,     0,  -209,     0,    27,
       0,   196,     0,     0,     0,    31,  -209,  -209,  -209,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,   201,   703,    75,
       0,     0,    77,     0,     0,     0,    81,   639,    83,     0,
       0,    85,   996,   997,   641,   998,     0,     0,     0,     0,
     999,     0,     0,     0,     0,     0,     0,     0,   705,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    90,     0,
    1000,     0,     0,     0,  1001,     0,     0,  1002,     0,   198,
     647,     0,     0,     0,     0,     0,     0,     0,   706,    97,
       0,     0,     0,     0,     0,     0,  1003,   101,     0,  -209,
       0,     0,     0,  1004,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1005,     0,     0,     0,     0,     0,     0,
       0,     0,  1006,     0,  1007,     0,  1008,     0,   707,     0,
       0,     0,     0,     0,   708,  1009,     0,  1010,     0,     0,
    1011,     0,     0,     0,     0,     0,     0,     0,     0,   510,
     465,   466,   467,   468,   469,     0,     0,   472,   473,   474,
     475,     0,   477,   478,     0,   655,   656,   657,   658,   659,
     660,     0,   661,     0,     0,     0,   662,   663,   664,   665,
     666,   667,   668,   669,   670,   671,   230,   672,     0,     0,
     673,   674,   675,   676,  1012,   639,     0,     0,     0,     0,
     231,   232,   641,   233,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,   235,     0,     0,     0,
       0,     0,   236,     0,     0,     0,     0,     0,   237,     0,
       0,     0,   238,     0,     0,   239,     0,   240,   647,     0,
       0,     0,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,   242,   243,     0,     0,     0,     0,     0,
       0,   244,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
     246,     0,   247,     0,   248,     0,   249,     0,     0,   250,
       0,     0,     0,   251,     0,   252,     0,     0,   253,     0,
       0,     0,     0,     0,     0,     0,     0,   510,   465,   466,
     467,   468,   469,     0,     0,   472,   473,   474,   475,     0,
     477,   478,     0,   655,   656,   657,   658,   659,   660,     0,
     661,     0,     0,     0,   662,   663,   664,   665,   666,   667,
     668,   669,   670,   671,     7,   672,     0,     0,   673,   674,
     675,   676,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   358,     0,     0,     0,    22,    23,     0,     0,     0,
       0,     0,     0,     0,   359,     0,    29,   360,     0,     0,
       0,     0,    32,     0,     0,     0,     0,     0,     0,    37,
       0,     7,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   358,     0,
       0,   361,    22,    23,     0,     0,    54,     0,    56,     0,
      58,   359,  1056,    29,   360,  1057,   434,     0,   362,    32,
      65,     0,     0,   435,     0,     0,    37,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,    80,     0,
      82,     0,     0,    84,     0,     0,     0,     0,   361,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,  1056,
       0,     0,  1057,     0,     0,   362,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    82,     0,     0,
      84,     0,     0,    98,   437,     0,   434,     0,   438,  1126,
       0,     0,     0,   435,     0,     0,  1200,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,   510,   465,
     466,   467,   468,   469,     0,     0,   472,   473,   474,   475,
      98,   477,   478,     0,   440,   441,  1189,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   363,   437,     0,   434,     0,   438,     0,
     463,     0,     0,   435,     0,     0,  1263,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,   510,   465,
     466,   467,   468,   469,     0,     0,   472,   473,   474,   475,
       0,   477,   478,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   363,   437,     0,   434,     0,   438,     0,
     463,     0,     0,   435,     0,     0,  1264,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,   510,   465,
     466,   467,   468,   469,     0,     0,   472,   473,   474,   475,
       0,   477,   478,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   363,   437,     0,   434,     0,   438,     0,
     463,     0,     0,   435,     0,     0,  1265,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,   510,   465,
     466,   467,   468,   469,     0,     0,   472,   473,   474,   475,
       0,   477,   478,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   363,   437,     0,     0,     0,   438,     0,
     463,     0,     0,     0,     0,   434,  1266,     0,     0,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,   274,     0,   439,   510,   465,
     466,   467,   468,   469,     0,     0,   472,   473,   474,   475,
     991,   477,   478,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   363,     0,     0,     0,     0,     0,     0,
     463,     0,     0,   437,     0,     0,     0,   438,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   214,     0,   436,     0,   439,     0,     0,   590,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   591,     0,   460,   461,
     462,     0,   363,     0,     0,     0,     0,     0,     0,   463,
       0,     0,     0,   437,     0,     0,     0,   438,     0,     0,
       0,     0,     0,     0,     0,   434,   831,     0,     0,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,   439,     0,     0,   832,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   833,   592,   460,   461,
     462,     0,     0,     0,     0,     0,     0,     0,     0,   463,
       0,     0,     0,   437,   434,     0,     0,   438,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,  1143,     0,     0,     0,     0,     0,
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
       0,     0,     0,   436,   925,     0,     0,     0,     0,     0,
       0,   214,     0,     0,     0,   439,     0,     0,     0,   926,
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
       0,     0,     0,   957,   434,     0,     0,     0,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   439,     0,     0,     0,  1118,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,  1119,   460,   461,   462,     0,
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
    1135,   437,   463,     0,     0,   438,     0,     0,     0,     0,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   439,     0,   460,   461,   462,     0,
       0,     0,   434,     0,     0,  1183,     0,   463,     0,   435,
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
     440,   441,   739,   442,   443,   444,     0,   445,   446,   447,
     436,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   439,   558,   460,   461,   462,     0,     0,
       0,   434,   827,     0,     0,     0,   463,     0,   435,     0,
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
     437,   809,     0,     0,   438,   463,     0,     0,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   828,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   872,   439,   460,   461,   462,     0,     0,     0,
       0,     0,     0,     0,     0,   463,     0,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   434,     0,   460,   461,   462,     0,     0,
     435,     0,     0,     0,     0,     0,   463,     0,     0,     0,
       0,     0,   436,     0,     0,   955,     0,     0,     0,     0,
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
       0,   437,   434,  1021,     0,   438,     0,   463,     0,   435,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   436,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   439,     0,   460,   461,   462,     0,
       0,     0,   434,   967,     0,     0,     0,   463,     0,   435,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   436,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,     0,   460,   461,   462,     0,
     437,     0,     0,   968,   438,     0,     0,   463,     0,     0,
     434,  1117,     0,     0,     0,     0,     0,   435,     0,     0,
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
     435,   439,  1072,     0,     0,     0,   463,     0,     0,     0,
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
     456,   457,   458,   459,     0,  1129,   460,   461,   462,     0,
       0,  1138,   439,     0,     0,     0,     0,   463,     0,   437,
       0,     0,     0,   438,     0,     0,     0,     0,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   439,     0,   460,   461,   462,     0,     0,     0,
       0,     0,     0,     0,     0,   463,     0,     0,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   434,     0,   460,   461,   462,     0,     0,   435,
       0,     0,   619,  1166,   230,   463,     7,     0,     0,     0,
       0,   436,  1169,     0,     0,     0,     0,     0,   231,   232,
       0,   233,     0,     0,     0,     0,   234,     0,     0,     0,
       0,     0,   434,  1188,   235,     0,     0,     0,     0,   435,
     236,     0,     0,     0,     0,     0,   237,     0,     0,     0,
     238,   436,     0,   239,     0,   240,     0,     0,     0,     0,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
     437,   242,   243,   434,   438,     0,     0,     0,     0,   244,
     435,     0,     0,     0,     0,     0,     0,     0,     0,   245,
       0,     0,   436,     0,     0,     0,     0,     0,   246,     0,
     247,     0,   248,   439,   249,     0,     0,   250,     0,     0,
     437,   251,     0,   252,   438,     0,   253,     0,     0,     0,
     440,   441,  1220,   442,   443,   444,     0,   445,   446,   447,
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
     438,     0,     0,     0,     0,     0,   434,     0,  1228,     0,
       0,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,   439,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,     0,   440,   441,   438,   442,
     443,   444,     0,   445,   446,   447,  1267,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
       0,   460,   461,   462,     0,     0,     0,   439,     0,     0,
       0,     0,   463,     0,   437,   434,     0,     0,   438,     0,
       0,     0,   435,     0,   440,   441,  1268,   442,   443,   444,
       0,   445,   446,   447,   436,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   439,     0,   460,
     461,   462,     0,     0,     0,   434,     0,     0,     0,     0,
     463,     0,   435,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   436,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   437,     0,   434,     0,   438,     0,     0,
     463,     0,   435,     0,     0,  1269,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,     0,     0,     0,
       0,     0,     0,   437,     0,     0,     0,   438,     0,     0,
       0,     0,     0,   440,   441,  1270,   442,   443,   444,     0,
     445,   446,   447,   436,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   439,     0,   460,   461,
     462,     0,     0,   437,   434,     0,     0,   438,     0,   463,
       0,   435,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   436,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   439,     0,   460,   461,
     462,     0,   437,     0,     0,     0,   438,     0,     0,   463,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,     0,   460,   461,
     462,     0,   945,     0,     0,     0,   438,     0,     0,   463,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,     0,   453,   454,
     455,   456,   457,     0,   459,   439,     0,   460,   461,   462,
       0,     0,     0,     0,     0,     0,     0,     0,   463,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,     7,     0,   460,   461,   462,
       0,     0,     0,     0,     0,     0,     0,     0,   463,     0,
       0,     0,   358,     0,     0,     0,    22,    23,     0,     0,
       0,     0,     0,     0,     0,   359,     0,    29,   360,     0,
       0,     0,     0,    32,     0,     0,     0,     0,     0,     0,
      37,     0,     7,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   358,
       0,     0,   361,    22,    23,     0,     0,    54,     0,    56,
       0,   356,   359,  1056,    29,   360,  1057,     0,     0,   362,
      32,    65,     0,     0,     0,     0,     0,    37,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,    80,
       0,    82,     0,     0,    84,     0,   358,     0,     0,   361,
      22,    23,     0,     0,    54,     0,    56,     0,     0,   359,
    1056,    29,   360,  1057,     0,     0,   362,    32,    65,     0,
       0,     0,     0,     0,    37,     0,     0,  -335,     0,     0,
       0,    41,     0,     0,     0,     0,    80,     0,    82,   437,
     -68,    84,     0,   438,    98,     0,   361,     0,  -335,     0,
       0,    54,     0,    56,   436,     0,     0,    60,  -335,   -68,
      61,     0,     0,   362,     0,    65,     0,     0,     0,     0,
       0,     0,   439,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    80,     0,    82,     0,     0,    84,   440,
     441,    98,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,     0,   453,   454,   455,   456,   457,
     458,   459,     0,   437,   460,   461,   462,   438,     0,     0,
       0,     0,     0,     0,     0,   463,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,     0,     0,   363,     0,     0,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   230,   453,
     454,   455,   456,   457,     0,   459,     0,     0,   460,   461,
     462,     0,   231,   232,     0,   233,     0,     0,     0,   463,
     234,     0,    23,     0,     0,     0,     0,     0,   235,     0,
       0,     0,     0,     0,   236,     0,     0,     0,     0,     0,
     237,     0,     0,     0,   238,     0,     0,   239,     0,   240,
       0,     0,     0,     0,     0,     0,     0,     0,   241,     0,
       0,     0,     0,     0,     0,   242,   243,     0,     0,     0,
       0,     0,     0,   244,    56,     0,     0,     0,     0,     0,
       0,     0,     0,   245,     0,     0,    65,     0,     0,   230,
       0,     7,   246,     0,   247,     0,   248,     0,   249,     0,
       0,   250,     0,   231,   232,   251,   233,   252,     0,    84,
     253,   234,     0,     0,     0,     0,     0,     0,     0,   235,
       0,     0,     0,     0,     0,   236,     0,     0,     0,     0,
       0,   237,     0,     0,     0,   238,     0,     0,   239,     0,
     240,     0,     0,     0,     0,     0,     0,     0,  -310,   241,
       0,     0,     0,     0,     0,     0,   242,   243,     0,     0,
       0,     0,  -310,  -310,   244,  -310,     0,     0,     0,     0,
    -310,     0,     0,     0,   245,     0,     0,     0,  -310,     0,
       0,     0,     0,   246,  -310,   247,     0,   248,     0,   249,
    -310,     0,   250,     0,  -310,     0,   251,  -310,   252,  -310,
       0,   253,     0,     0,     0,     0,     0,   230,  -310,     0,
       0,     0,     0,     0,     0,  -310,  -310,     0,     0,     0,
       0,   231,   232,  -310,   233,     0,     0,     0,     0,   234,
       0,     0,     0,  -310,     0,     0,     0,   235,     0,     0,
       0,     0,  -310,   236,  -310,     0,  -310,     0,  -310,   237,
       0,  -310,     0,   238,     0,  -310,   239,  -310,   240,     0,
    -310,     0,     0,     0,     0,     0,  -311,   241,     0,     0,
       0,     0,     0,     0,   242,   243,     0,     0,     0,     0,
    -311,  -311,   244,  -311,     0,     0,     0,     0,  -311,     0,
       0,     0,   245,     0,     0,     0,  -311,     0,     0,     0,
       0,   246,  -311,   247,     0,   248,     0,   249,  -311,     0,
     250,     0,  -311,     0,   251,  -311,   252,  -311,     0,   253,
       0,     0,     0,     0,     0,     0,  -311,     0,     0,     0,
       0,     0,     0,  -311,  -311,     0,     0,     0,     0,     0,
       0,  -311,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -311,     0,     0,     0,     0,     0,     0,     0,     0,
    -311,     0,  -311,     0,  -311,     0,  -311,     0,     0,  -311,
       0,     0,     0,  -311,     0,  -311,     0,     0,  -311
};

static const yytype_int16 yycheck[] =
{
       2,   110,     2,    26,   620,    62,    67,   181,   552,   265,
     401,    45,    62,   492,   579,   590,    62,   165,   425,     2,
     680,   223,   682,    25,   632,    86,    67,   628,   833,   229,
     516,   798,   608,   887,   290,    69,   381,   828,   792,   131,
     101,   547,    25,    66,     1,    86,   203,     1,   851,     1,
      56,   970,   403,    24,    35,    47,    32,     5,   895,   391,
     101,    32,    61,    32,    32,   265,    56,    75,   122,    40,
      41,    42,    74,   329,    32,   102,    78,    79,   117,     1,
       3,    46,    27,   119,   146,   523,   119,   261,    47,   119,
     290,    74,    47,     1,     3,    78,    79,    32,   100,    32,
       0,    32,   104,   518,   146,   437,   438,   146,   116,    24,
      32,   140,   174,    32,     2,   169,   164,   100,   117,   148,
      85,   104,   164,    75,    32,    40,    41,    42,   109,   329,
     545,    54,    91,     1,    79,   972,    91,    25,   174,    61,
      56,   174,   169,   119,   174,    54,    91,   118,   113,   119,
     119,   483,   484,    61,   146,    24,    47,   489,    24,   146,
      56,   119,   170,    32,   116,   171,    32,   124,   229,   114,
    1024,    40,    41,    42,    40,    41,    42,   778,   146,    47,
     103,   171,    56,   105,  1103,   172,    74,     2,   145,   146,
      78,    79,   543,   169,   632,   117,   963,   105,   146,   764,
     169,   169,   173,   970,   265,  1059,  1060,    47,   365,   117,
      25,   169,   100,   215,   146,   172,   104,   171,   170,   973,
    1023,    31,   170,   145,    56,   481,   482,   146,  1019,   290,
      22,   164,   215,   109,   169,   146,   169,   145,   169,   169,
     172,   171,   164,   277,   576,   279,   446,   169,   593,   118,
     169,   425,   118,   164,   740,   146,   124,   832,   173,    74,
      91,   169,    11,    78,    79,    47,    47,   269,   329,   605,
     606,  1125,   274,   164,    47,   611,   612,   145,   146,   119,
      90,   481,   482,   174,    94,   100,   269,   863,   380,   104,
     796,   274,   326,  1147,   146,   146,   164,    89,   171,   146,
     556,    53,    78,    79,   173,    27,   146,   173,   164,    91,
      91,     3,    18,   146,  1119,   171,   516,   173,  1085,   111,
     172,   172,    74,   170,   164,    47,   146,   215,    20,    51,
     140,    56,    84,   535,   174,   145,  1103,   146,   148,   172,
      56,    33,   152,   944,    27,   402,   119,   349,   496,   349,
     146,    56,   172,     3,    76,   164,   556,    79,   146,    81,
     146,   363,    54,   971,    47,   171,   349,   390,    51,    91,
     372,    63,    78,   146,     3,   398,   172,    46,   164,   579,
     363,   269,   103,   385,   172,   146,   274,   109,   146,   372,
      90,    97,   114,    76,    94,   597,    79,    66,    81,   885,
     215,   174,   385,   164,    54,   171,   112,   173,    91,   470,
     471,   103,   170,  1073,  1074,   476,    85,   109,   479,   480,
     481,   482,   124,   425,   164,    54,   109,   992,    56,   470,
     471,   114,   146,    50,   174,   476,    53,  1012,   479,   480,
       2,   927,   146,    93,   113,   146,    46,  1063,   150,   146,
     172,   146,   931,   103,   269,   516,     9,    74,   172,   274,
      77,   349,   146,    25,   146,   488,    66,    84,   172,    83,
      56,   172,    86,   170,   103,   363,   146,   172,    56,   815,
     816,   817,    31,   146,   372,    85,   146,   544,   172,   172,
     172,   171,   109,   164,   544,   556,   887,   385,   544,   906,
     171,   164,   172,   146,   164,   507,    90,   509,   569,   570,
      94,   620,    74,   113,   146,   146,    78,    79,   579,   146,
     171,   164,   173,    90,   507,    56,   509,    94,   569,   570,
     862,   146,   164,   164,   349,  1151,   146,   164,   100,     2,
     740,    90,   104,  1118,     3,    94,    90,   879,   363,   164,
      94,   140,   575,   171,   164,   173,   145,   372,    17,   148,
     100,    20,    25,   152,   764,   164,    25,   601,   602,   164,
     385,   170,    78,    79,    56,   170,    35,   611,   612,   291,
     292,   583,    27,   583,   607,    90,    45,    56,    90,    94,
     922,   140,    94,    52,   926,    54,   145,   171,   147,   148,
     583,    56,    47,   152,    63,   171,    51,   173,    31,    56,
     159,    74,    71,   945,   946,    78,    79,  1161,   167,   507,
     171,   509,   173,   171,   171,   173,   173,   827,   828,    88,
      56,    76,     5,  1024,    79,   148,    81,   100,    56,     2,
      99,   104,   790,   169,   103,   124,    91,   145,   569,   570,
     169,   110,   164,   215,   171,   163,   115,   989,    56,   991,
     164,   164,    25,   164,   109,   164,  1210,    90,  1059,   114,
     164,    94,   151,  1064,   153,   154,   164,   156,   157,   740,
     164,   164,   169,    85,   164,   885,    31,   172,   146,   145,
     171,   164,   507,   171,   509,   583,  1032,  1033,  1155,    27,
     171,   762,   164,   764,   171,   853,   146,   269,   172,     9,
       3,    74,   274,  1049,  1050,    78,    79,   140,   141,    47,
     781,   762,   145,    51,   147,   148,    66,   927,   171,   152,
      79,   169,   906,   124,  1125,   164,   159,   100,   148,    69,
     781,   104,   165,   166,   167,    90,   171,   124,    76,    94,
     172,    79,   215,    31,   146,   757,   124,  1214,  1215,  1216,
    1217,  1218,  1219,    91,   124,    34,   768,    34,   583,   771,
      66,   172,   124,   172,   145,  1111,  1112,  1113,  1114,   172,
      21,   109,   816,   817,   169,   768,   114,   349,   771,   118,
     171,   146,   992,   124,   170,   140,   141,   105,   143,   144,
     145,   363,   147,   148,   149,   171,   269,   152,   140,   148,
     372,   274,    90,   158,   159,   148,    94,   162,   164,   172,
     165,   166,   167,   385,   885,   148,    27,   148,   148,   148,
     887,   176,   148,   148,   148,   858,   893,   148,   148,   148,
    1096,  1097,   148,   877,   148,   902,    47,   881,   174,   148,
      51,   164,   215,    32,   171,   145,    34,   164,   860,   164,
     172,   164,   140,   865,    34,   146,   927,   145,   170,   147,
     148,   172,   164,   875,   152,    76,   878,   860,    79,   164,
     768,   159,   865,   771,    47,   169,   349,   165,   166,   167,
      91,   174,   875,   164,   174,   878,  1096,  1097,   146,   174,
     363,   174,   170,    34,   164,   164,   269,   172,   109,   372,
     171,   274,    27,   114,   171,   145,   172,   164,   146,   174,
      32,    34,   385,   172,   174,   164,   164,   172,   174,   172,
     163,   992,    47,   146,   163,   109,    51,   578,   372,   992,
     264,   825,    24,   945,   946,   507,   365,   509,   389,   338,
     952,  1060,   954,   768,   882,   957,   771,   985,   981,   529,
     906,    76,   544,   383,    79,  1139,    81,  1024,   902,   952,
     304,   954,   860,   893,   957,  1210,    91,   865,   980,  1066,
    1220,  1213,   852,   686,   129,   531,   349,   875,   990,   526,
     878,    20,   777,   931,   109,    -1,    -1,   980,    -1,   114,
     363,    -1,    -1,    -1,    -1,    -1,    -1,   990,    -1,   372,
      -1,    -1,    -1,    -1,    -1,  1049,  1050,    -1,    -1,    -1,
      -1,   583,   385,    -1,    -1,    -1,    -1,    -1,    -1,  1031,
      -1,    -1,    -1,    -1,    -1,  1096,  1097,  1039,  1147,    -1,
      -1,    -1,  1151,    -1,   507,   860,   509,    -1,  1031,    -1,
     865,    -1,    -1,    -1,    -1,    -1,  1039,    -1,    -1,    -1,
     875,    -1,    -1,   878,   952,  1239,   954,  1241,  1125,   957,
    1244,  1245,    -1,  1107,    -1,    -1,    -1,    -1,    -1,  1113,
    1114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1092,    -1,   980,    -1,    -1,    -1,  1130,    -1,    -1,    -1,
      -1,    -1,   990,    -1,    -1,    -1,    -1,    -1,  1142,  1092,
      -1,  1145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     583,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1133,    -1,  1135,    -1,    -1,    -1,   952,  1140,   954,
      -1,  1143,   957,  1031,   507,    -1,   509,    -1,    -1,    -1,
    1133,  1039,  1135,    -1,    -1,    -1,    -1,  1140,    -1,    -1,
    1143,    -1,    -1,    -1,    -1,   980,    -1,    -1,    -1,    -1,
    1172,    -1,    -1,    -1,     2,   990,  1178,    -1,    -1,    -1,
    1182,  1183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1172,
      -1,    -1,  1194,    -1,    -1,  1178,  1198,    25,  1200,  1182,
    1183,    -1,    -1,    -1,  1092,    -1,   768,    -1,    -1,   771,
      -1,  1194,    -1,    -1,    -1,  1198,  1031,  1200,    -1,  1221,
     583,  1221,    -1,    -1,  1039,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1221,    -1,
      -1,    -1,    -1,    -1,    -1,  1133,    74,  1135,    -1,    -1,
      78,    79,  1140,    -1,    -1,  1143,    -1,    -1,    -1,    -1,
      -1,  1263,  1264,  1265,  1266,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,    -1,    -1,    -1,   104,  1092,    -1,    -1,
    1263,  1264,  1265,  1266,  1172,    90,    -1,    -1,    -1,    94,
    1178,    -1,    -1,    -1,  1182,  1183,    -1,    -1,   860,    -1,
      -1,    -1,    -1,   865,    -1,   768,  1194,    -1,   771,    -1,
    1198,    -1,  1200,   875,    -1,    -1,   878,    -1,  1133,    -1,
    1135,    -1,    -1,    -1,    -1,  1140,    -1,    -1,  1143,    -1,
      -1,    -1,    -1,  1221,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,    -1,    -1,    -1,   152,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,  1172,    -1,    -1,
     165,   166,   167,  1178,    -1,    -1,    -1,  1182,  1183,    -1,
      -1,    -1,    -1,    -1,    -1,  1263,  1264,  1265,  1266,  1194,
       2,    -1,    25,  1198,    -1,  1200,    -1,   215,    -1,    -1,
     952,    -1,   954,    -1,    -1,   957,    -1,   860,    -1,    -1,
      -1,    -1,   865,    25,    -1,   768,  1221,    -1,   771,    -1,
      -1,    -1,   875,    -1,    -1,   878,    -1,    -1,   980,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   990,    -1,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   269,    -1,    -1,    -1,    -1,   274,    -1,  1263,  1264,
    1265,  1266,    74,    -1,    -1,    -1,    78,    79,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1031,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1039,   100,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,   952,
       1,   954,     3,    -1,   957,    -1,    -1,   860,    -1,    -1,
      -1,    -1,   865,    -1,    -1,    -1,    17,    18,    -1,    20,
      -1,    -1,   875,    -1,    25,   878,    -1,   980,    -1,    -1,
      -1,   349,    33,    -1,    -1,    -1,    -1,   990,    39,    -1,
    1092,    -1,    -1,    -1,    45,   363,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,   372,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,   385,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,  1031,    -1,
      -1,  1133,   215,  1135,    -1,    -1,  1039,    88,  1140,    -1,
      -1,  1143,    -1,    -1,    -1,    -1,    97,    -1,    99,   952,
     101,   954,   103,   215,   957,   106,    -1,    -1,    -1,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
    1172,    -1,    -1,    -1,    -1,    -1,  1178,   980,    -1,    -1,
    1182,  1183,    -1,    -1,    -1,    -1,   269,   990,    -1,  1092,
      -1,   274,  1194,    -1,    -1,    -1,  1198,    -1,  1200,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     2,   269,    -1,    -1,
      -1,    -1,   274,    -1,    -1,    -1,    -1,    -1,    -1,  1221,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1031,    25,
    1133,    -1,  1135,    -1,    -1,    -1,  1039,  1140,    -1,   507,
    1143,   509,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1263,  1264,  1265,  1266,    -1,    -1,    -1,    -1,  1172,
      -1,    -1,    -1,    -1,    -1,  1178,    -1,    -1,    74,  1182,
    1183,    -1,    -1,    -1,    -1,    -1,    -1,   349,    -1,  1092,
      -1,  1194,    -1,    -1,    -1,  1198,    -1,  1200,    -1,    -1,
      -1,   363,   385,    -1,   100,    -1,    -1,    -1,   104,    -1,
     372,    -1,    -1,    -1,    -1,   583,    -1,    -1,  1221,    -1,
      -1,    -1,    -1,   385,    -1,    -1,    -1,    -1,    -1,    -1,
    1133,    -1,  1135,    -1,    -1,    -1,    -1,  1140,    -1,    -1,
    1143,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    -1,    -1,
    1263,  1264,  1265,  1266,    -1,    -1,    -1,    -1,    -1,  1172,
      -1,    -1,    -1,    -1,    -1,  1178,    -1,    -1,    -1,  1182,
    1183,    -1,    -1,    -1,    -1,    -1,    -1,   168,    28,    -1,
     171,  1194,   173,    -1,    -1,  1198,    -1,  1200,    -1,    -1,
      40,    41,    42,    -1,    -1,    -1,     1,    -1,    -1,    -1,
       5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1221,   215,
      -1,    -1,    -1,    -1,   507,    -1,   509,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    37,    38,    -1,   507,    -1,   509,    43,    -1,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
    1263,  1264,  1265,  1266,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   269,    -1,    -1,    -1,    72,   274,    74,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    83,    84,
      -1,    86,    -1,    -1,    89,    -1,    91,    -1,    -1,    -1,
     768,    -1,    -1,   771,    -1,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,
      -1,   583,    -1,   163,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,   349,    -1,    -1,    -1,    -1,    33,   199,
      -1,    -1,    -1,    -1,    39,    -1,    -1,   363,    -1,   164,
      45,    -1,    -1,    -1,    49,   170,   372,    52,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,   385,
      -1,    -1,   860,    -1,    -1,    70,    71,   865,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,   875,    -1,    -1,
     878,    -1,    -1,    88,    -1,    -1,    -1,    -1,   258,   259,
      -1,    -1,    97,    -1,    99,    -1,   101,    -1,   103,    -1,
     270,   106,     3,    -1,    -1,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
     310,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,   952,   768,   954,    -1,   771,   957,
      -1,    -1,    63,    -1,   169,    -1,   171,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,   768,    78,    -1,   771,
      -1,   507,   980,   509,    -1,    -1,    -1,    88,    -1,   359,
      -1,    31,   990,    -1,    -1,    -1,    97,    -1,    99,    -1,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   395,    -1,   397,    -1,    -1,
      -1,   401,    -1,  1031,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1039,    -1,    -1,    -1,    -1,    -1,   860,    -1,    -1,
      90,    -1,   865,    -1,    94,   425,    -1,   583,    -1,    -1,
      -1,    -1,   875,    -1,    -1,   878,    -1,    -1,   860,    -1,
     171,    -1,    -1,   865,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   875,    -1,    -1,   878,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1092,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,    -1,   152,   153,   154,    -1,   156,   157,   158,   159,
     490,    -1,   162,    -1,    -1,   165,   166,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1133,   176,  1135,    -1,   952,
      -1,   954,  1140,    -1,   957,  1143,    -1,    -1,   518,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     952,   531,   954,    -1,    -1,   957,    -1,   980,    -1,    -1,
      -1,   541,    -1,    -1,  1172,   545,    -1,   990,    -1,    -1,
    1178,    -1,    -1,    -1,  1182,  1183,    -1,    -1,   980,    -1,
      -1,    -1,    -1,    -1,    -1,     3,  1194,    -1,   990,    -1,
    1198,    -1,  1200,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,  1031,   589,
      -1,    -1,    -1,  1221,    -1,    33,  1039,    -1,    -1,    -1,
     600,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,  1031,
      -1,    49,   768,    -1,    52,   771,    54,  1039,    -1,    -1,
     620,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,  1263,  1264,  1265,  1266,    22,
      78,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,  1092,
      88,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,    97,
      43,    99,    -1,   101,    -1,   103,    -1,    50,   106,    -1,
    1092,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   686,    -1,    -1,    72,
    1133,    -1,  1135,    -1,    77,    -1,    79,  1140,    -1,    -1,
    1143,    -1,    -1,    -1,   860,    -1,    89,    -1,    91,   865,
      -1,  1133,    -1,  1135,    -1,    -1,    -1,    -1,  1140,   875,
      -1,  1143,   878,    -1,    -1,    -1,   109,    -1,   111,  1172,
      -1,   114,    -1,   171,    -1,  1178,    -1,    -1,    -1,  1182,
    1183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1172,  1194,    -1,    -1,    -1,  1198,  1178,  1200,    -1,    -1,
    1182,  1183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1194,   773,    -1,    -1,  1198,   777,  1200,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   787,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   952,    -1,   954,  1221,
      -1,   957,    -1,    -1,    31,    -1,   806,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    17,
    1263,  1264,  1265,  1266,   980,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,   990,    33,    -1,    -1,    -1,    -1,
      -1,  1263,  1264,  1265,  1266,    -1,    -1,    45,    -1,    47,
      48,    -1,   852,    -1,    -1,    53,    -1,    55,    -1,    57,
      -1,    -1,    -1,    90,    62,    -1,    -1,    94,    66,    67,
      68,    69,    70,    -1,    -1,  1031,    74,    -1,    76,    -1,
      -1,    -1,   882,  1039,    -1,    83,    -1,    -1,    86,    -1,
      88,    -1,    90,    -1,    -1,    93,    94,    -1,    96,    -1,
      -1,    -1,   100,   101,   102,    -1,   906,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,    -1,   144,   145,    -1,
     147,   148,    -1,    -1,     5,   152,    -1,    -1,    -1,    -1,
      -1,   129,   159,   131,    -1,    -1,  1092,    -1,   165,   166,
     167,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,    45,
      -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,
      31,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    69,    -1,    -1,   174,  1133,    -1,  1135,
      -1,    -1,    78,    79,  1140,   985,    -1,  1143,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    77,    -1,    79,   197,
      81,    -1,    83,   201,   202,    86,    -1,    -1,    89,    -1,
      91,    -1,    -1,   109,    -1,    -1,  1172,    -1,    -1,    90,
      -1,    -1,  1178,    94,    -1,    -1,  1182,  1183,   109,    -1,
     111,   229,    -1,   114,    -1,    -1,    -1,    -1,  1194,    -1,
      -1,   137,  1198,    -1,  1200,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1063,    -1,  1221,    -1,   265,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,    12,    -1,
      -1,   152,    -1,   164,    -1,    19,    -1,   158,   159,   170,
      -1,   162,   290,    -1,   165,   166,   167,    31,    32,    -1,
      -1,    -1,    -1,   301,    -1,    -1,    -1,  1263,  1264,  1265,
    1266,    -1,    -1,    47,    -1,    -1,    -1,    -1,   316,   317,
      -1,    -1,    -1,    -1,    -1,   221,    -1,    -1,    -1,    -1,
      -1,   329,    -1,    -1,    -1,   333,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1151,    -1,   351,    -1,    -1,    90,    -1,    -1,    -1,
      94,    -1,    -1,   361,   260,   261,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   373,    -1,   375,    -1,    -1,
      -1,   277,    -1,   279,    -1,   119,    -1,    -1,    -1,   123,
     286,   389,   390,   391,    -1,    -1,    -1,    -1,    -1,    -1,
     398,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
     326,   165,   166,   167,    -1,   169,   434,   435,   436,   437,
     438,   439,   176,   441,   442,   443,   444,   445,    -1,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,    -1,    -1,    -1,    -1,
      -1,    -1,   470,   471,    -1,    -1,    -1,    -1,   476,    -1,
      -1,   479,   480,   481,   482,   483,   484,    -1,    -1,    -1,
     488,   489,    -1,    -1,   492,    -1,    -1,    -1,    -1,    -1,
     498,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   509,    -1,    -1,   512,    -1,    -1,    -1,   516,    -1,
      -1,    -1,    -1,    -1,    -1,   523,    -1,   525,    -1,    -1,
      -1,   529,    -1,    -1,   532,   533,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   544,    -1,    -1,   547,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   556,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   566,    -1,
      -1,   569,   570,    -1,    -1,    -1,    -1,   575,   576,    -1,
      -1,   579,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   590,   591,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   604,    -1,    -1,   607,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     4,    -1,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    18,    -1,    -1,   632,    22,    -1,    24,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    42,    -1,    44,    -1,    -1,
      47,    -1,    -1,   559,    51,    -1,    -1,    -1,    12,    56,
      -1,    -1,    59,    60,    -1,    19,    -1,    -1,    65,   677,
      67,    68,    -1,    -1,    -1,    -1,   684,    31,    32,    76,
      -1,    78,    79,    80,    81,    82,    -1,    -1,    -1,    -1,
      87,    -1,    89,    -1,    91,   601,   602,    -1,    -1,    -1,
      97,    98,    -1,   100,    -1,   611,   612,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,   739,   740,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      94,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,   762,    -1,   764,   765,   766,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,   123,
     167,    -1,   169,   781,   171,   172,   173,    -1,    -1,   176,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   796,   143,
     144,   145,   800,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    -1,   825,    -1,    -1,
      -1,   829,   176,   831,   832,   833,    -1,   835,    -1,   837,
      -1,    -1,   840,   841,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   854,    -1,    -1,    -1,
     858,    -1,    -1,    -1,   862,   863,    -1,   865,    -1,    -1,
      -1,    -1,    -1,    -1,   872,   873,   772,   875,    -1,    -1,
     878,   879,    -1,    -1,    -1,    -1,    -1,   885,    -1,    -1,
      -1,    -1,   890,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     806,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     816,   817,    -1,    -1,   922,   923,   924,   925,   926,   927,
      -1,    -1,    -1,   931,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   945,   946,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    -1,   955,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,   971,    -1,    -1,    -1,   975,    -1,    -1,
      33,   877,    -1,   981,    -1,   881,    39,   883,    -1,    -1,
      -1,   989,    45,   991,   992,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,  1012,    -1,    -1,    70,    71,    -1,
      -1,  1019,    -1,  1021,    -1,    78,    -1,    -1,    -1,    -1,
    1028,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,  1040,    -1,    97,    -1,    99,    -1,   101,  1047,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1068,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1080,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   988,    -1,    -1,    -1,    -1,    -1,  1096,  1097,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1109,    -1,    -1,    -1,    -1,    -1,    -1,   171,  1117,
    1118,  1119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1133,    -1,  1135,    -1,    -1,
    1138,    -1,  1140,    -1,    -1,  1143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1049,  1050,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,  1165,  1166,    -1,
    1168,  1169,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
    1188,    -1,    -1,  1191,    -1,    -1,    -1,  1195,    -1,    -1,
      -1,    -1,  1200,  1201,    -1,    -1,  1204,    -1,    -1,    -1,
      -1,  1107,    -1,    -1,  1212,    -1,    -1,  1113,  1114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1226,  1227,
    1228,  1229,  1230,    -1,  1130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1139,    90,    -1,  1142,    -1,    94,  1145,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1263,  1264,  1265,  1266,  1267,
    1268,  1269,  1270,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,   129,  1180,    -1,   132,   133,   134,   135,
      -1,   137,   138,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,
     176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1239,    -1,  1241,    -1,    -1,  1244,  1245,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,    -1,   140,   141,   142,   143,   144,    -1,    -1,    12,
      -1,   149,    -1,    -1,    17,    18,    19,    20,    -1,    -1,
     158,    -1,    25,   161,   162,   163,    -1,    -1,    -1,   167,
      33,   169,    35,   171,    -1,   173,    39,    -1,   176,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    99,    -1,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,   128,   129,    -1,    -1,   132,
     133,   134,   135,    -1,   137,   138,    -1,   140,   141,   142,
     143,   144,   145,    -1,   147,    -1,    -1,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,    -1,   162,
      -1,    -1,   165,   166,   167,   168,     3,     4,   171,     6,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,    -1,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,     3,     4,    -1,
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
      -1,    -1,    -1,    -1,   140,    -1,   142,    -1,    -1,     3,
       4,    -1,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,   163,    22,    -1,
      24,    25,    -1,   169,    -1,   171,    -1,   173,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    42,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,     3,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    12,   112,    -1,
      -1,   115,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,    -1,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    71,   171,    -1,   173,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    99,    -1,   101,    -1,   103,    -1,
      -1,    -1,    -1,    -1,   109,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,    -1,    -1,   132,   133,   134,
     135,    -1,   137,   138,    -1,   140,   141,   142,   143,   144,
     145,    -1,   147,    -1,    -1,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,     3,   162,    -1,    -1,
     165,   166,   167,   168,   169,    12,    -1,    -1,    -1,    -1,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,    -1,    -1,   132,   133,   134,   135,    -1,
     137,   138,    -1,   140,   141,   142,   143,   144,   145,    -1,
     147,    -1,    -1,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,     5,   162,    -1,    -1,   165,   166,
     167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      -1,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    72,    26,    27,    -1,    -1,    77,    -1,    79,    -1,
      81,    35,    83,    37,    38,    86,    12,    -1,    89,    43,
      91,    -1,    -1,    19,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,   109,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    86,    -1,    -1,    89,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,   164,    90,    -1,    12,    -1,    94,   170,
      -1,    -1,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,   129,    -1,    -1,   132,   133,   134,   135,
     164,   137,   138,    -1,   140,   141,   170,   143,   144,   145,
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
     166,   167,    -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,
     176,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,    -1,    31,    -1,   123,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    73,    -1,   165,   166,
     167,    -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    13,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,   123,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    73,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,    -1,    90,    12,    -1,    -1,    94,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    90,    -1,    12,    -1,    94,    -1,    -1,   176,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    90,    -1,    12,    -1,    94,    -1,   176,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    90,    12,    -1,    -1,    94,    -1,   176,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    90,    -1,    -1,    -1,    94,    -1,    -1,   176,    -1,
      -1,    -1,    -1,   102,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,   123,    -1,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,    73,   165,   166,   167,    -1,
     169,    -1,    -1,    -1,    -1,    -1,    -1,   176,    12,    -1,
      -1,    -1,    90,    -1,    -1,    19,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    31,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    90,   165,   166,   167,
      94,    -1,    -1,    12,    13,    -1,    -1,    -1,   176,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,   119,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    94,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
     119,   165,   166,   167,   123,    -1,    -1,    -1,    -1,    -1,
     174,    90,   176,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   123,    -1,   165,   166,   167,    -1,
      -1,    -1,    12,    -1,    -1,   174,    -1,   176,    -1,    19,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    31,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,   176,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    12,    -1,    94,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   123,    -1,   165,   166,   167,    -1,   169,
      90,    12,    -1,    -1,    94,    -1,   176,    -1,    19,    -1,
     140,   141,   102,   143,   144,   145,    -1,   147,   148,   149,
      31,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   123,   164,   165,   166,   167,    -1,    -1,
      -1,    12,    13,    -1,    -1,    -1,   176,    -1,    19,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      31,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    90,
      -1,    -1,    12,    94,    -1,    -1,   176,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   123,    -1,   165,   166,   167,    -1,    -1,    -1,
      90,   172,    -1,    -1,    94,   176,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   122,   123,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    12,    -1,   165,   166,   167,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    12,    -1,    94,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   123,    -1,   165,   166,   167,    -1,
      -1,    90,    12,    13,    -1,    94,    -1,   176,    -1,    19,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    31,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   123,    -1,   165,   166,   167,    -1,
      -1,    -1,    12,   172,    -1,    -1,    -1,   176,    -1,    19,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    31,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      90,    -1,    -1,   172,    94,    -1,    -1,   176,    -1,    -1,
      12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   123,    -1,   165,   166,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    90,    -1,
     140,   141,    94,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    12,    -1,   165,   166,   167,    -1,    -1,
      19,   123,   172,    -1,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    12,    -1,   165,   166,   167,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,   140,   141,    94,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,   164,   165,   166,   167,    -1,
      -1,   122,   123,    -1,    -1,    -1,    -1,   176,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   123,    -1,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,    12,    -1,   165,   166,   167,    -1,    -1,    19,
      -1,    -1,     1,   174,     3,   176,     5,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    12,    13,    33,    -1,    -1,    -1,    -1,    19,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    31,    -1,    52,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      90,    70,    71,    12,    94,    -1,    -1,    -1,    -1,    78,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      99,    -1,   101,   123,   103,    -1,    -1,   106,    -1,    -1,
      90,   110,    -1,   112,    94,    -1,   115,    -1,    -1,    -1,
     140,   141,    61,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   123,    -1,   165,   166,   167,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    94,   176,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,   123,   165,   166,   167,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,   176,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    31,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,   176,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    12,    -1,   102,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,   140,   141,    94,   143,
     144,   145,    -1,   147,   148,   149,   102,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,   176,    -1,    90,    12,    -1,    -1,    94,    -1,
      -1,    -1,    19,    -1,   140,   141,   102,   143,   144,   145,
      -1,   147,   148,   149,    31,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   123,    -1,   165,
     166,   167,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
     176,    -1,    19,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    31,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    90,    -1,    12,    -1,    94,    -1,    -1,
     176,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,   140,   141,   102,   143,   144,   145,    -1,
     147,   148,   149,    31,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   123,    -1,   165,   166,
     167,    -1,    -1,    90,    12,    -1,    -1,    94,    -1,   176,
      -1,    19,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    31,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   123,    -1,   165,   166,
     167,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,   176,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,   176,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,    -1,   162,   123,    -1,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,     5,    -1,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    -1,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    72,    26,    27,    -1,    -1,    77,    -1,    79,
      -1,    81,    35,    83,    37,    38,    86,    -1,    -1,    89,
      43,    91,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    22,    -1,    -1,    72,
      26,    27,    -1,    -1,    77,    -1,    79,    -1,    -1,    35,
      83,    37,    38,    86,    -1,    -1,    89,    43,    91,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      -1,    57,    -1,    -1,    -1,    -1,   109,    -1,   111,    90,
      66,   114,    -1,    94,   164,    -1,    72,    -1,    74,    -1,
      -1,    77,    -1,    79,    31,    -1,    -1,    83,    84,    85,
      86,    -1,    -1,    89,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,   140,
     141,   164,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    90,   165,   166,   167,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,
      -1,    -1,    -1,   169,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,     3,   156,
     157,   158,   159,   160,    -1,   162,    -1,    -1,   165,   166,
     167,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,   176,
      25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    91,    -1,    -1,     3,
      -1,     5,    97,    -1,    99,    -1,   101,    -1,   103,    -1,
      -1,   106,    -1,    17,    18,   110,    20,   112,    -1,   114,
     115,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    17,    18,    78,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    88,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    97,    39,    99,    -1,   101,    -1,   103,
      45,    -1,   106,    -1,    49,    -1,   110,    52,   112,    54,
      -1,   115,    -1,    -1,    -1,    -1,    -1,     3,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    17,    18,    78,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    97,    39,    99,    -1,   101,    -1,   103,    45,
      -1,   106,    -1,    49,    -1,   110,    52,   112,    54,    -1,
     115,    -1,    -1,    -1,    -1,    -1,     3,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      17,    18,    78,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    97,    39,    99,    -1,   101,    -1,   103,    45,    -1,
     106,    -1,    49,    -1,   110,    52,   112,    54,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115
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
     174,   361,    47,   146,   164,   174,   345,   361,   342,   164,
     174,   345,   122,   146,   174,    32,   224,   361,    32,    47,
     224,   361,   146,   172,   164,   171,     1,   269,   170,   275,
     124,   146,   170,   146,   172,   338,     1,   223,   334,   335,
     105,   311,   171,   300,   372,   140,   148,   300,   300,   372,
     322,   171,   173,   164,   164,   164,   164,   164,   164,   172,
     174,   349,    47,   174,    32,    32,    47,   171,   172,    47,
      91,   146,   172,    17,    20,    25,    45,    52,    63,    71,
      88,    99,   110,   115,   351,    90,    90,   164,   374,   350,
     372,   372,    34,   185,    34,    34,   185,   102,   205,   219,
     172,   172,   331,   338,    69,   374,   219,   172,   172,   329,
     341,   145,   284,   172,   339,   150,   299,   333,   372,   170,
      75,   116,   170,   261,   172,   171,   205,   219,   257,    47,
     174,    47,   146,   172,   213,   220,    17,    18,    20,    25,
      45,    49,    52,    71,    78,    88,    97,    99,   101,   110,
     112,   115,   169,   212,   298,   372,   372,   207,   209,   146,
     164,    13,   164,   169,   269,   321,   329,   372,    47,   342,
     185,   174,   243,   372,   174,   345,   185,   372,   164,   174,
     146,   174,   372,   174,   372,   372,   224,    44,   372,   243,
     372,   224,   249,   205,   349,   170,    83,    86,   170,   184,
     192,   226,   372,   273,   274,   304,   311,    61,   117,   315,
     302,   303,   172,   295,   297,   172,   174,   172,   243,   372,
      44,   243,   372,   349,   363,   338,   351,   351,   185,   185,
     372,   185,    34,   164,   164,   333,   171,   171,   164,   299,
     333,   334,   311,   341,   372,   185,   239,   342,   257,   145,
     205,   243,   372,   243,   372,   198,   207,    13,    36,    73,
     164,   164,   298,   372,   372,   269,   170,   164,   164,   164,
     372,   172,   185,   174,   361,   174,   361,   185,   122,   372,
      32,   224,   361,    32,   224,   361,   172,   192,   226,   226,
     275,   192,   315,     3,    54,    93,   103,   316,   317,   318,
     372,   291,   172,   300,   300,   174,   174,   372,    32,    32,
     172,   333,    34,   185,   348,   348,   299,   333,    32,   224,
     172,   372,   174,   174,   170,   372,   207,   209,    13,   170,
     224,    44,   185,   372,   174,    44,   185,   372,   174,   372,
     102,    44,   372,   224,    44,   372,   224,   164,   226,   275,
     278,   318,   117,   146,   124,   151,   153,   154,   156,   157,
      61,    32,   164,   204,   312,   313,    44,    44,   102,    44,
      44,   185,   172,   172,   185,   205,   164,   164,   372,   372,
     185,   372,   185,   372,   372,   372,   313,   372,   317,   318,
     318,   318,   318,   318,   318,   316,   183,   372,   372,   372,
     372,   163,   163,   102,   102,   102,   102,   102,   102,   102,
     102,   372,   372,   372,   372
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
     382,   382,   382,   383,   383,   384,   384,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   385,   385,
     385,   385,   385,   385,   385,   386,   386,   386,   386,   387,
     387,   387,   387,   388,   388,   388,   388,   388,   388,   388
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
#line 685 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 5984 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 689 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 5990 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 690 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 5996 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 697 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6004 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 705 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6012 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 709 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6021 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 719 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6029 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 724 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6037 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 731 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6043 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 732 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6049 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 733 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6055 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 734 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6061 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 735 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6067 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 736 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6073 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 737 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6079 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 738 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6085 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: TDEFER stmt  */
#line 740 "chpl.ypp"
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
#line 6107 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: try_stmt  */
#line 757 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6113 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: return_stmt  */
#line 758 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6119 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: TINIT TTHIS TSEMI  */
#line 760 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr));
    auto node = Init::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6131 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 768 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6144 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 777 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6157 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 786 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6165 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 790 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6176 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 797 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6185 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 804 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6191 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 805 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6197 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 806 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6203 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 807 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6209 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 808 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6215 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 809 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6221 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 810 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6227 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 812 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6233 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 814 "chpl.ypp"
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
#line 6257 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 834 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6269 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 842 "chpl.ypp"
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
#line 6290 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL do_stmt  */
#line 859 "chpl.ypp"
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
#line 6310 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TON expr do_stmt  */
#line 875 "chpl.ypp"
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
#line 6331 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 892 "chpl.ypp"
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
#line 6352 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL do_stmt  */
#line 909 "chpl.ypp"
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
#line 6372 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSYNC stmt  */
#line 925 "chpl.ypp"
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
#line 6394 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 951 "chpl.ypp"
  {
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 6402 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 972 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6410 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 979 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6419 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 986 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6427 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 990 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6438 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 1000 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6444 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 1002 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6452 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 1006 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6460 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 1013 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6469 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 1018 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6479 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 1031 "chpl.ypp"
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
#line 6501 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 1052 "chpl.ypp"
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
#line 6522 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 1069 "chpl.ypp"
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
#line 6543 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1086 "chpl.ypp"
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
#line 6562 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1103 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6568 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1104 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6575 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1106 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6582 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1111 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6588 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1112 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6595 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1118 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6605 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1136 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6614 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1141 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6623 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1146 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6636 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1158 "chpl.ypp"
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
#line 6655 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1176 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6661 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1177 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6667 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1182 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6675 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1186 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6683 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1190 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6692 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1195 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6700 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1199 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6708 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1203 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6717 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1211 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6726 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1216 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6736 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1222 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6745 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1227 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6756 "bison-chpl-lib.cpp"
    break;

  case 90: /* opt_only_ls: %empty  */
#line 1235 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6762 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: renames_ls  */
#line 1236 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6768 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1241 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6777 "bison-chpl-lib.cpp"
    break;

  case 93: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1246 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6789 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1254 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6802 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1263 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6814 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1271 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6827 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1282 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6836 "bison-chpl-lib.cpp"
    break;

  case 114: /* import_expr: expr  */
#line 1317 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6844 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr TDOT all_op_name  */
#line 1321 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6853 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1326 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6862 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TAS ident_use  */
#line 1331 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6872 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1337 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6882 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_ls: import_expr  */
#line 1345 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6888 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_ls TCOMMA import_expr  */
#line 1346 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6894 "bison-chpl-lib.cpp"
    break;

  case 121: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1351 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6904 "bison-chpl-lib.cpp"
    break;

  case 122: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1360 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6912 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1364 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6920 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1368 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6928 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1372 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6936 "bison-chpl-lib.cpp"
    break;

  case 126: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1376 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6944 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1380 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6952 "bison-chpl-lib.cpp"
    break;

  case 128: /* opt_label_ident: %empty  */
#line 1388 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6958 "bison-chpl-lib.cpp"
    break;

  case 129: /* opt_label_ident: TIDENT  */
#line 1389 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6964 "bison-chpl-lib.cpp"
    break;

  case 130: /* opt_label_ident: TINIT  */
#line 1390 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6970 "bison-chpl-lib.cpp"
    break;

  case 139: /* ident_use: TIDENT  */
#line 1416 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6976 "bison-chpl-lib.cpp"
    break;

  case 140: /* ident_use: TINIT  */
#line 1417 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6982 "bison-chpl-lib.cpp"
    break;

  case 141: /* ident_use: TTHIS  */
#line 1418 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6988 "bison-chpl-lib.cpp"
    break;

  case 160: /* scalar_type: TBOOL  */
#line 1444 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6994 "bison-chpl-lib.cpp"
    break;

  case 161: /* scalar_type: TENUM  */
#line 1445 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7000 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TINT  */
#line 1446 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7006 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TUINT  */
#line 1447 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7012 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TREAL  */
#line 1448 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7018 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TIMAG  */
#line 1449 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7024 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TCOMPLEX  */
#line 1450 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7030 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TBYTES  */
#line 1451 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7036 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TSTRING  */
#line 1452 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7042 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TLOCALE  */
#line 1453 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7048 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TNOTHING  */
#line 1454 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7054 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TVOID  */
#line 1455 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7060 "bison-chpl-lib.cpp"
    break;

  case 175: /* do_stmt: TDO stmt  */
#line 1468 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7066 "bison-chpl-lib.cpp"
    break;

  case 176: /* do_stmt: block_stmt  */
#line 1469 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7072 "bison-chpl-lib.cpp"
    break;

  case 177: /* return_stmt: TRETURN TSEMI  */
#line 1474 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7083 "bison-chpl-lib.cpp"
    break;

  case 178: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1481 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7094 "bison-chpl-lib.cpp"
    break;

  case 179: /* class_level_stmt: TSEMI  */
#line 1491 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7102 "bison-chpl-lib.cpp"
    break;

  case 180: /* class_level_stmt: inner_class_level_stmt  */
#line 1495 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7112 "bison-chpl-lib.cpp"
    break;

  case 181: /* $@1: %empty  */
#line 1500 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7119 "bison-chpl-lib.cpp"
    break;

  case 182: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1502 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7128 "bison-chpl-lib.cpp"
    break;

  case 183: /* $@2: %empty  */
#line 1506 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7135 "bison-chpl-lib.cpp"
    break;

  case 184: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1508 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7144 "bison-chpl-lib.cpp"
    break;

  case 192: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1526 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7153 "bison-chpl-lib.cpp"
    break;

  case 193: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1531 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7162 "bison-chpl-lib.cpp"
    break;

  case 194: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1536 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7171 "bison-chpl-lib.cpp"
    break;

  case 195: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1541 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7179 "bison-chpl-lib.cpp"
    break;

  case 196: /* forwarding_decl_start: TFORWARDING  */
#line 1548 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7188 "bison-chpl-lib.cpp"
    break;

  case 197: /* extern_or_export: TEXTERN  */
#line 1555 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7194 "bison-chpl-lib.cpp"
    break;

  case 198: /* extern_or_export: TEXPORT  */
#line 1556 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7200 "bison-chpl-lib.cpp"
    break;

  case 199: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1561 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7210 "bison-chpl-lib.cpp"
    break;

  case 200: /* extern_export_kind_inner: TTYPE  */
#line 1574 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7219 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1583 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7229 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1590 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7241 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1598 "chpl.ypp"
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
#line 7263 "bison-chpl-lib.cpp"
    break;

  case 204: /* $@3: %empty  */
#line 1616 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7271 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 1620 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-4])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7280 "bison-chpl-lib.cpp"
    break;

  case 206: /* $@4: %empty  */
#line 1625 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7289 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr extern_export_kind_inner $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1630 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yylsp[-5]), (yyvsp[-3].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7298 "bison-chpl-lib.cpp"
    break;

  case 208: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1638 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7306 "bison-chpl-lib.cpp"
    break;

  case 209: /* no_loop_attributes: %empty  */
#line 1644 "chpl.ypp"
  {
    // Even though there are no attributes, there could be loop pragmas.
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 7315 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1650 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7321 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1651 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7327 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1668 "chpl.ypp"
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
#line 7354 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1691 "chpl.ypp"
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
#line 7377 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1710 "chpl.ypp"
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
#line 7400 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1729 "chpl.ypp"
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
#line 7429 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1754 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7438 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: for_loop_kw expr TIN expr task_intent_clause do_stmt  */
#line 1759 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7447 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1764 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7456 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator task_intent_clause do_stmt  */
#line 1769 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7465 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1774 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7474 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw expr task_intent_clause do_stmt  */
#line 1779 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7483 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1784 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7492 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: for_loop_kw zippered_iterator task_intent_clause do_stmt  */
#line 1789 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7501 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1794 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7510 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TLSBR expr_ls TIN expr task_intent_clause TRSBR stmt  */
#line 1799 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7519 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1804 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7528 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator task_intent_clause TRSBR stmt  */
#line 1809 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7537 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1814 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7546 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR expr_ls task_intent_clause TRSBR stmt  */
#line 1819 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7555 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1824 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7565 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TLSBR zippered_iterator task_intent_clause TRSBR stmt  */
#line 1830 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7575 "bison-chpl-lib.cpp"
    break;

  case 236: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1839 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7585 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr TTHEN stmt  */
#line 1848 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7593 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr block_stmt  */
#line 1852 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7601 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1856 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7609 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1860 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7617 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1864 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7625 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF ifvar block_stmt  */
#line 1868 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7633 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1872 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7641 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1876 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7649 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1880 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7658 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1885 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7667 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1890 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7676 "bison-chpl-lib.cpp"
    break;

  case 248: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1895 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7685 "bison-chpl-lib.cpp"
    break;

  case 249: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1903 "chpl.ypp"
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
#line 7705 "bison-chpl-lib.cpp"
    break;

  case 250: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1919 "chpl.ypp"
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
#line 7725 "bison-chpl-lib.cpp"
    break;

  case 251: /* interface_start: TINTERFACE ident_def  */
#line 1938 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Interface);
  }
#line 7733 "bison-chpl-lib.cpp"
    break;

  case 252: /* interface_stmt: interface_start TLP ifc_formal_ls TRP block_stmt  */
#line 1945 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-4].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), parts, (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7743 "bison-chpl-lib.cpp"
    break;

  case 253: /* interface_stmt: interface_start block_stmt  */
#line 1951 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-1].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), parts, nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7753 "bison-chpl-lib.cpp"
    break;

  case 254: /* ifc_formal_ls: ifc_formal  */
#line 1959 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7759 "bison-chpl-lib.cpp"
    break;

  case 255: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1960 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7765 "bison-chpl-lib.cpp"
    break;

  case 256: /* ifc_formal: ident_def  */
#line 1965 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7771 "bison-chpl-lib.cpp"
    break;

  case 276: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 2008 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7779 "bison-chpl-lib.cpp"
    break;

  case 277: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 2012 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7787 "bison-chpl-lib.cpp"
    break;

  case 278: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 2016 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7795 "bison-chpl-lib.cpp"
    break;

  case 279: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2023 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7803 "bison-chpl-lib.cpp"
    break;

  case 280: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 2027 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7811 "bison-chpl-lib.cpp"
    break;

  case 281: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2031 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7819 "bison-chpl-lib.cpp"
    break;

  case 282: /* try_stmt: TTRY tryable_stmt  */
#line 2038 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7827 "bison-chpl-lib.cpp"
    break;

  case 283: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2042 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7835 "bison-chpl-lib.cpp"
    break;

  case 284: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2046 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7843 "bison-chpl-lib.cpp"
    break;

  case 285: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2050 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7851 "bison-chpl-lib.cpp"
    break;

  case 286: /* catch_expr_ls: %empty  */
#line 2056 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7857 "bison-chpl-lib.cpp"
    break;

  case 287: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2057 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7863 "bison-chpl-lib.cpp"
    break;

  case 288: /* catch_expr: TCATCH block_stmt  */
#line 2062 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7871 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2066 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7879 "bison-chpl-lib.cpp"
    break;

  case 290: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2070 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7887 "bison-chpl-lib.cpp"
    break;

  case 291: /* catch_expr_inner: ident_def  */
#line 2077 "chpl.ypp"
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
#line 7906 "bison-chpl-lib.cpp"
    break;

  case 292: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2092 "chpl.ypp"
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
#line 7925 "bison-chpl-lib.cpp"
    break;

  case 293: /* throw_stmt: TTHROW expr TSEMI  */
#line 2110 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7935 "bison-chpl-lib.cpp"
    break;

  case 294: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2118 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7944 "bison-chpl-lib.cpp"
    break;

  case 295: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2123 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7955 "bison-chpl-lib.cpp"
    break;

  case 296: /* when_stmt_ls: %empty  */
#line 2132 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7961 "bison-chpl-lib.cpp"
    break;

  case 297: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2133 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7967 "bison-chpl-lib.cpp"
    break;

  case 298: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2138 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7975 "bison-chpl-lib.cpp"
    break;

  case 299: /* when_stmt: TOTHERWISE stmt  */
#line 2142 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7984 "bison-chpl-lib.cpp"
    break;

  case 300: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2147 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 7994 "bison-chpl-lib.cpp"
    break;

  case 301: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2156 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8003 "bison-chpl-lib.cpp"
    break;

  case 302: /* manager_expr: expr TAS ident_def  */
#line 2161 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8011 "bison-chpl-lib.cpp"
    break;

  case 303: /* manager_expr: expr  */
#line 2165 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8019 "bison-chpl-lib.cpp"
    break;

  case 304: /* manager_expr_ls: manager_expr  */
#line 2171 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8025 "bison-chpl-lib.cpp"
    break;

  case 305: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2172 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8031 "bison-chpl-lib.cpp"
    break;

  case 306: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2177 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8040 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2187 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8050 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2193 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8062 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_start: class_tag ident_def  */
#line 2205 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8070 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_tag: TCLASS  */
#line 2211 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8076 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_tag: TRECORD  */
#line 2212 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8082 "bison-chpl-lib.cpp"
    break;

  case 312: /* class_tag: TUNION  */
#line 2213 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8088 "bison-chpl-lib.cpp"
    break;

  case 313: /* opt_inherit: %empty  */
#line 2217 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8094 "bison-chpl-lib.cpp"
    break;

  case 314: /* opt_inherit: TCOLON expr_ls  */
#line 2218 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8100 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_level_stmt_ls: %empty  */
#line 2222 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8109 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2227 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8117 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2231 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8125 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2235 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8133 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2239 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8141 "bison-chpl-lib.cpp"
    break;

  case 320: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
#line 2247 "chpl.ypp"
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
#line 8164 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
#line 2266 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8178 "bison-chpl-lib.cpp"
    break;

  case 322: /* enum_header: TENUM ident_def  */
#line 2279 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8186 "bison-chpl-lib.cpp"
    break;

  case 323: /* enum_ls: enum_item  */
#line 2286 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8195 "bison-chpl-lib.cpp"
    break;

  case 324: /* enum_ls: enum_ls TCOMMA  */
#line 2291 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8205 "bison-chpl-lib.cpp"
    break;

  case 325: /* $@5: %empty  */
#line 2297 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8214 "bison-chpl-lib.cpp"
    break;

  case 326: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2302 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8223 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2307 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8232 "bison-chpl-lib.cpp"
    break;

  case 328: /* $@6: %empty  */
#line 2312 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8241 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2317 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8250 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_item: ident_def  */
#line 2327 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[0])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8262 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_item: ident_def TASSIGN expr  */
#line 2335 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[-2])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8276 "bison-chpl-lib.cpp"
    break;

  case 332: /* lambda_decl_start: TLAMBDA  */
#line 2348 "chpl.ypp"
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
#line 8292 "bison-chpl-lib.cpp"
    break;

  case 333: /* $@7: %empty  */
#line 2365 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8302 "bison-chpl-lib.cpp"
    break;

  case 334: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2371 "chpl.ypp"
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
#line 8325 "bison-chpl-lib.cpp"
    break;

  case 336: /* linkage_spec: linkage_spec_empty  */
#line 2395 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8331 "bison-chpl-lib.cpp"
    break;

  case 337: /* linkage_spec: TINLINE  */
#line 2396 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8338 "bison-chpl-lib.cpp"
    break;

  case 338: /* linkage_spec: TOVERRIDE  */
#line 2398 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8345 "bison-chpl-lib.cpp"
    break;

  case 339: /* opt_fn_type_formal_ls: %empty  */
#line 2403 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8351 "bison-chpl-lib.cpp"
    break;

  case 340: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2404 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8357 "bison-chpl-lib.cpp"
    break;

  case 341: /* fn_type_formal_ls: fn_type_formal  */
#line 2408 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8363 "bison-chpl-lib.cpp"
    break;

  case 342: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2409 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8369 "bison-chpl-lib.cpp"
    break;

  case 343: /* fn_type_formal: named_formal  */
#line 2414 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8375 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2417 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8381 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_type_formal: formal_type  */
#line 2419 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8387 "bison-chpl-lib.cpp"
    break;

  case 346: /* opt_fn_type_ret_type: %empty  */
#line 2423 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8393 "bison-chpl-lib.cpp"
    break;

  case 347: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2424 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8399 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2437 "chpl.ypp"
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
#line 8419 "bison-chpl-lib.cpp"
    break;

  case 349: /* $@8: %empty  */
#line 2456 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8429 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2462 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8442 "bison-chpl-lib.cpp"
    break;

  case 351: /* $@9: %empty  */
#line 2471 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8452 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2477 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8465 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2489 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8473 "bison-chpl-lib.cpp"
    break;

  case 354: /* $@10: %empty  */
#line 2498 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8481 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2502 "chpl.ypp"
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
#line 8504 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2524 "chpl.ypp"
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

  case 357: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2533 "chpl.ypp"
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

  case 358: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2542 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.thisIntentLoc = (yylsp[-4]);
      fp.receiver = context->buildThisFormal((yylsp[-3]), (yylsp[-4]), (yyvsp[-4].intentTag), (yyvsp[-3].expr), /*initExpr*/ nullptr);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8544 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2552 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.thisIntentLoc = (yylsp[-4]);
      fp.receiver = context->buildThisFormal((yylsp[-3]), (yylsp[-4]), (yyvsp[-4].intentTag), (yyvsp[-3].expr), /*initExpr*/ nullptr);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8558 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2562 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8568 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2571 "chpl.ypp"
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
#line 8584 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2587 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 8593 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_ident: ident_def TBANG  */
#line 2597 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8603 "bison-chpl-lib.cpp"
    break;

  case 406: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2654 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8609 "bison-chpl-lib.cpp"
    break;

  case 407: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2655 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8615 "bison-chpl-lib.cpp"
    break;

  case 408: /* opt_formal_ls: %empty  */
#line 2659 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8621 "bison-chpl-lib.cpp"
    break;

  case 409: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2660 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8627 "bison-chpl-lib.cpp"
    break;

  case 410: /* req_formal_ls: TLP TRP  */
#line 2664 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8633 "bison-chpl-lib.cpp"
    break;

  case 411: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2665 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8639 "bison-chpl-lib.cpp"
    break;

  case 412: /* formal_ls_inner: formal  */
#line 2669 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8645 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2670 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8651 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_ls: %empty  */
#line 2674 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8657 "bison-chpl-lib.cpp"
    break;

  case 415: /* formal_ls: formal_ls_inner  */
#line 2675 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8663 "bison-chpl-lib.cpp"
    break;

  case 417: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2684 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8671 "bison-chpl-lib.cpp"
    break;

  case 418: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2689 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8679 "bison-chpl-lib.cpp"
    break;

  case 419: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2694 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8687 "bison-chpl-lib.cpp"
    break;

  case 420: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2699 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8695 "bison-chpl-lib.cpp"
    break;

  case 421: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2704 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yylsp[-5]), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8703 "bison-chpl-lib.cpp"
    break;

  case 422: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2709 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8711 "bison-chpl-lib.cpp"
    break;

  case 423: /* opt_formal_intent_tag: %empty  */
#line 2715 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);
  }
#line 8720 "bison-chpl-lib.cpp"
    break;

  case 424: /* opt_formal_intent_tag: required_intent_tag  */
#line 2720 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8729 "bison-chpl-lib.cpp"
    break;

  case 425: /* required_intent_tag: TIN  */
#line 2727 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::IN);            }
#line 8735 "bison-chpl-lib.cpp"
    break;

  case 426: /* required_intent_tag: TINOUT  */
#line 2728 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::INOUT);         }
#line 8741 "bison-chpl-lib.cpp"
    break;

  case 427: /* required_intent_tag: TOUT  */
#line 2729 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::OUT);           }
#line 8747 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TCONST TIN  */
#line 2730 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_IN);      }
#line 8753 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TCONST TREF  */
#line 2731 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);     }
#line 8759 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TCONST  */
#line 2732 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);         }
#line 8765 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TPARAM  */
#line 2733 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);         }
#line 8771 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TREF  */
#line 2734 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);           }
#line 8777 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TTYPE  */
#line 2735 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);          }
#line 8783 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TVAR  */
#line 2736 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR); }
#line 8789 "bison-chpl-lib.cpp"
    break;

  case 435: /* opt_this_intent_tag: %empty  */
#line 2740 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);     }
#line 8795 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_this_intent_tag: TPARAM  */
#line 2741 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);              }
#line 8801 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: TREF  */
#line 2742 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);                }
#line 8807 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TCONST TREF  */
#line 2743 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);          }
#line 8813 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TCONST  */
#line 2744 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);              }
#line 8819 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TTYPE  */
#line 2745 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);               }
#line 8825 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TIN  */
#line 2746 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::IN);       }
#line 8831 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TCONST TIN  */
#line 2747 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::CONST_IN); }
#line 8837 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_this_intent_tag: TVAR  */
#line 2748 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR);      }
#line 8843 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_this_intent_tag: TOUT  */
#line 2749 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::OUT);      }
#line 8849 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_this_intent_tag: TINOUT  */
#line 2750 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::INOUT);    }
#line 8855 "bison-chpl-lib.cpp"
    break;

  case 446: /* proc_iter_or_op: TPROC  */
#line 2754 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8861 "bison-chpl-lib.cpp"
    break;

  case 447: /* proc_iter_or_op: TITER  */
#line 2755 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8867 "bison-chpl-lib.cpp"
    break;

  case 448: /* proc_iter_or_op: TOPERATOR  */
#line 2756 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8873 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: %empty  */
#line 2760 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::DEFAULT_RETURN_INTENT); }
#line 8879 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TOUT  */
#line 2761 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::OUT);                   }
#line 8885 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TCONST  */
#line 2762 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST);                 }
#line 8891 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TCONST TREF  */
#line 2763 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST_REF);             }
#line 8897 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_ret_tag: TREF  */
#line 2764 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::REF);                   }
#line 8903 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: TPARAM  */
#line 2765 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::PARAM);                 }
#line 8909 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: TTYPE  */
#line 2766 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::TYPE);                  }
#line 8915 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: TIN  */
#line 2767 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::IN);            }
#line 8921 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_ret_tag: TCONST TIN  */
#line 2768 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::CONST_IN);      }
#line 8927 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_ret_tag: TVAR  */
#line 2769 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::VAR);           }
#line 8933 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_ret_tag: TINOUT  */
#line 2770 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::INOUT);         }
#line 8939 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_throws_error: %empty  */
#line 2774 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8945 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_throws_error: TTHROWS  */
#line 2775 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8951 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_function_body_stmt: TSEMI  */
#line 2778 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8957 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_function_body_stmt: function_body_stmt  */
#line 2779 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8963 "bison-chpl-lib.cpp"
    break;

  case 464: /* function_body_stmt: block_stmt_body  */
#line 2783 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8969 "bison-chpl-lib.cpp"
    break;

  case 465: /* function_body_stmt: TDO toplevel_stmt  */
#line 2784 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 8975 "bison-chpl-lib.cpp"
    break;

  case 466: /* query_expr: TQUERIEDIDENT  */
#line 2788 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8981 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_lifetime_where: %empty  */
#line 2793 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8987 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_lifetime_where: TWHERE expr  */
#line 2795 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8993 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2797 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8999 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2799 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9005 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2801 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9011 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_components_expr: lifetime_expr  */
#line 2806 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9017 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2808 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9023 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2813 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9029 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2815 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9035 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2817 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9041 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2819 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9047 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2821 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9053 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2823 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9059 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2825 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9065 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_ident: TIDENT  */
#line 2829 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9071 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_ident: TINIT  */
#line 2830 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9077 "bison-chpl-lib.cpp"
    break;

  case 483: /* lifetime_ident: TTHIS  */
#line 2831 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9083 "bison-chpl-lib.cpp"
    break;

  case 484: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2836 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9092 "bison-chpl-lib.cpp"
    break;

  case 485: /* type_alias_decl_stmt_start: TTYPE  */
#line 2845 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9102 "bison-chpl-lib.cpp"
    break;

  case 486: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2851 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9113 "bison-chpl-lib.cpp"
    break;

  case 487: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2861 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9121 "bison-chpl-lib.cpp"
    break;

  case 488: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2865 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9129 "bison-chpl-lib.cpp"
    break;

  case 489: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2872 "chpl.ypp"
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
#line 9152 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_init_type: %empty  */
#line 2893 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9158 "bison-chpl-lib.cpp"
    break;

  case 491: /* opt_init_type: TASSIGN expr  */
#line 2895 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9164 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_type: TPARAM  */
#line 2899 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9170 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_type: TCONST TREF  */
#line 2900 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9176 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_type: TREF  */
#line 2901 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9182 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_type: TCONST  */
#line 2902 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9188 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_type: TVAR  */
#line 2903 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9194 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_start: var_decl_type  */
#line 2908 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9203 "bison-chpl-lib.cpp"
    break;

  case 498: /* $@11: %empty  */
#line 2916 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9212 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt: TCONFIG $@11 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2920 "chpl.ypp"
                                              {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-4]), (yylsp[-3])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9221 "bison-chpl-lib.cpp"
    break;

  case 500: /* $@12: %empty  */
#line 2925 "chpl.ypp"
  {
    // Use a mid-rule action to preserve the 'on' expr
    context->noteVarDestinationExpr((yyvsp[0].expr));
  }
#line 9230 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt: TON expr $@12 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2930 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-4])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9239 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt: var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2935 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9248 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt: var_decl_start error TSEMI  */
#line 2940 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    delete (yyvsp[-2].attributeGroup);
    context->resetDeclStateOnError();
  }
#line 9258 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2949 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9266 "bison-chpl-lib.cpp"
    break;

  case 505: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2953 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9274 "bison-chpl-lib.cpp"
    break;

  case 506: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2960 "chpl.ypp"
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
#line 9297 "bison-chpl-lib.cpp"
    break;

  case 507: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2979 "chpl.ypp"
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
#line 9317 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_var_decl_component: ident_def  */
#line 2998 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9325 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 3002 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9333 "bison-chpl-lib.cpp"
    break;

  case 510: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 3009 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9339 "bison-chpl-lib.cpp"
    break;

  case 511: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 3011 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9345 "bison-chpl-lib.cpp"
    break;

  case 512: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 3013 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9351 "bison-chpl-lib.cpp"
    break;

  case 513: /* opt_init_expr: %empty  */
#line 3019 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9357 "bison-chpl-lib.cpp"
    break;

  case 514: /* opt_init_expr: TASSIGN TNOINIT  */
#line 3020 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9363 "bison-chpl-lib.cpp"
    break;

  case 515: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 3021 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9369 "bison-chpl-lib.cpp"
    break;

  case 516: /* formal_or_ret_type_expr: expr  */
#line 3025 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9375 "bison-chpl-lib.cpp"
    break;

  case 517: /* ret_type: formal_or_ret_type_expr  */
#line 3029 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9381 "bison-chpl-lib.cpp"
    break;

  case 518: /* ret_type: reserved_type_ident_use  */
#line 3030 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9387 "bison-chpl-lib.cpp"
    break;

  case 519: /* ret_type: error  */
#line 3031 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9393 "bison-chpl-lib.cpp"
    break;

  case 520: /* colon_ret_type: TCOLON ret_type  */
#line 3035 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9399 "bison-chpl-lib.cpp"
    break;

  case 521: /* colon_ret_type: error  */
#line 3036 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9405 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_ret_type: %empty  */
#line 3040 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9411 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_type: %empty  */
#line 3045 "chpl.ypp"
                                 { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9417 "bison-chpl-lib.cpp"
    break;

  case 525: /* opt_type: TCOLON expr  */
#line 3046 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9423 "bison-chpl-lib.cpp"
    break;

  case 526: /* opt_type: TCOLON reserved_type_ident_use  */
#line 3047 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9429 "bison-chpl-lib.cpp"
    break;

  case 527: /* opt_type: error  */
#line 3048 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9435 "bison-chpl-lib.cpp"
    break;

  case 528: /* formal_type: formal_or_ret_type_expr  */
#line 3052 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9441 "bison-chpl-lib.cpp"
    break;

  case 529: /* formal_type: reserved_type_ident_use  */
#line 3053 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9447 "bison-chpl-lib.cpp"
    break;

  case 530: /* colon_formal_type: TCOLON formal_type  */
#line 3057 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9453 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_colon_formal_type: %empty  */
#line 3061 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9459 "bison-chpl-lib.cpp"
    break;

  case 532: /* opt_colon_formal_type: colon_formal_type  */
#line 3062 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9465 "bison-chpl-lib.cpp"
    break;

  case 533: /* expr_ls: expr  */
#line 3068 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9471 "bison-chpl-lib.cpp"
    break;

  case 534: /* expr_ls: expr_ls TCOMMA expr  */
#line 3069 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9477 "bison-chpl-lib.cpp"
    break;

  case 535: /* expr_ls_end_semi: expr_ls semicolon_list  */
#line 3073 "chpl.ypp"
                         {
    (yyval.ndArrayList) = context->makeNDArrayList();
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElement((yyvsp[-1].exprList)));
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter)));
  }
#line 9487 "bison-chpl-lib.cpp"
    break;

  case 536: /* expr_ls_end_semi: expr_ls TCOMMA semicolon_list  */
#line 3078 "chpl.ypp"
                                {
    (yyval.ndArrayList) = context->makeNDArrayList();
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElement((yyvsp[-2].exprList)));
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter)));
  }
#line 9497 "bison-chpl-lib.cpp"
    break;

  case 537: /* expr_ls_end_semi: expr_ls_semi semicolon_list  */
#line 3084 "chpl.ypp"
  {
    (yyval.ndArrayList) = (yyvsp[-1].ndArrayList);
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter)));
  }
#line 9506 "bison-chpl-lib.cpp"
    break;

  case 538: /* expr_ls_semi: expr_ls_end_semi expr_ls  */
#line 3091 "chpl.ypp"
                           {
    (yyval.ndArrayList) = context->appendNDArrayList((yyvsp[-1].ndArrayList), makeNDArrayElement((yyvsp[0].exprList)));
  }
#line 9514 "bison-chpl-lib.cpp"
    break;

  case 539: /* expr_ls_semi: expr_ls_end_semi expr_ls TCOMMA  */
#line 3094 "chpl.ypp"
                                  {
    (yyval.ndArrayList) = context->appendNDArrayList((yyvsp[-2].ndArrayList), makeNDArrayElement((yyvsp[-1].exprList)));
  }
#line 9522 "bison-chpl-lib.cpp"
    break;

  case 540: /* semicolon_list: TSEMI  */
#line 3100 "chpl.ypp"
                       { (yyval.counter) = 1; }
#line 9528 "bison-chpl-lib.cpp"
    break;

  case 541: /* semicolon_list: semicolon_list TSEMI  */
#line 3101 "chpl.ypp"
                       { (yyval.counter) = (yyvsp[-1].counter) + 1; }
#line 9534 "bison-chpl-lib.cpp"
    break;

  case 542: /* tuple_component: opt_try_expr  */
#line 3107 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9540 "bison-chpl-lib.cpp"
    break;

  case 543: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3112 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9548 "bison-chpl-lib.cpp"
    break;

  case 544: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3116 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9556 "bison-chpl-lib.cpp"
    break;

  case 545: /* opt_actual_ls: %empty  */
#line 3122 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9562 "bison-chpl-lib.cpp"
    break;

  case 546: /* opt_actual_ls: actual_ls  */
#line 3123 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9568 "bison-chpl-lib.cpp"
    break;

  case 547: /* actual_ls: actual_expr  */
#line 3128 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9577 "bison-chpl-lib.cpp"
    break;

  case 548: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3133 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9587 "bison-chpl-lib.cpp"
    break;

  case 549: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3141 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9593 "bison-chpl-lib.cpp"
    break;

  case 550: /* actual_expr: opt_try_expr  */
#line 3142 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9599 "bison-chpl-lib.cpp"
    break;

  case 551: /* ident_expr: ident_use  */
#line 3146 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9605 "bison-chpl-lib.cpp"
    break;

  case 552: /* ident_expr: scalar_type  */
#line 3147 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9611 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3160 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9617 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3162 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9623 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3164 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9629 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3166 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9639 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TATOMIC expr  */
#line 3172 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9645 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TSYNC expr  */
#line 3174 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9651 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TOWNED  */
#line 3177 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9657 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TOWNED expr  */
#line 3179 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9663 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TUNMANAGED  */
#line 3181 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9669 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3183 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9675 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TSHARED  */
#line 3185 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9681 "bison-chpl-lib.cpp"
    break;

  case 569: /* sub_type_level_expr: TSHARED expr  */
#line 3187 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9687 "bison-chpl-lib.cpp"
    break;

  case 570: /* sub_type_level_expr: TBORROWED  */
#line 3189 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9693 "bison-chpl-lib.cpp"
    break;

  case 571: /* sub_type_level_expr: TBORROWED expr  */
#line 3191 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9699 "bison-chpl-lib.cpp"
    break;

  case 572: /* sub_type_level_expr: TCLASS  */
#line 3193 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9705 "bison-chpl-lib.cpp"
    break;

  case 573: /* sub_type_level_expr: TRECORD  */
#line 3195 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9711 "bison-chpl-lib.cpp"
    break;

  case 574: /* for_expr: no_loop_attributes for_expr_base  */
#line 3199 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9717 "bison-chpl-lib.cpp"
    break;

  case 575: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3204 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9725 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3208 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9733 "bison-chpl-lib.cpp"
    break;

  case 577: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3212 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9741 "bison-chpl-lib.cpp"
    break;

  case 578: /* for_expr_base: for_loop_kw zippered_iterator TDO expr  */
#line 3216 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "expression-level loops with 'zip' must have an index.");
  }
#line 9749 "bison-chpl-lib.cpp"
    break;

  case 579: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3220 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9762 "bison-chpl-lib.cpp"
    break;

  case 580: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3229 "chpl.ypp"
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

  case 581: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3238 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9788 "bison-chpl-lib.cpp"
    break;

  case 582: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3249 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9794 "bison-chpl-lib.cpp"
    break;

  case 583: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3254 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9802 "bison-chpl-lib.cpp"
    break;

  case 584: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3258 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9810 "bison-chpl-lib.cpp"
    break;

  case 585: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3262 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9818 "bison-chpl-lib.cpp"
    break;

  case 586: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3266 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9826 "bison-chpl-lib.cpp"
    break;

  case 587: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3270 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9834 "bison-chpl-lib.cpp"
    break;

  case 588: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3274 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9842 "bison-chpl-lib.cpp"
    break;

  case 589: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3278 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9850 "bison-chpl-lib.cpp"
    break;

  case 590: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3285 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9864 "bison-chpl-lib.cpp"
    break;

  case 591: /* nil_expr: TNIL  */
#line 3302 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9870 "bison-chpl-lib.cpp"
    break;

  case 599: /* opt_task_intent_ls: %empty  */
#line 3320 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9876 "bison-chpl-lib.cpp"
    break;

  case 600: /* opt_task_intent_ls: task_intent_clause  */
#line 3321 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9882 "bison-chpl-lib.cpp"
    break;

  case 601: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3326 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9890 "bison-chpl-lib.cpp"
    break;

  case 602: /* task_intent_clause: TWITH TLP TRP  */
#line 3330 "chpl.ypp"
  {
    auto err = context->syntax((yyloc), "'with' clause cannot be empty.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-2]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9901 "bison-chpl-lib.cpp"
    break;

  case 603: /* task_intent_clause: TWITH TLP error TRP  */
#line 3337 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[-2]), "invalid intent expression in 'with' clause.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9912 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_intent_clause: TWITH error  */
#line 3344 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[0]), "missing parentheses around 'with' clause intents.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-1]), dummyLoc, dummyLoc, dummyLoc, exprs);
  }
#line 9923 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_intent_ls: intent_expr  */
#line 3353 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9929 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3354 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9935 "bison-chpl-lib.cpp"
    break;

  case 607: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3359 "chpl.ypp"
  {
    (yyval.expr) = context->buildTaskIntent((yyloc), (yylsp[-2]), nullptr, (yyvsp[-2].expr), (yyvsp[-3].taskIntent), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9943 "bison-chpl-lib.cpp"
    break;

  case 608: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3363 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9951 "bison-chpl-lib.cpp"
    break;

  case 609: /* intent_expr: expr TREDUCE ident_expr  */
#line 3367 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9959 "bison-chpl-lib.cpp"
    break;

  case 610: /* task_var_prefix: TCONST  */
#line 3373 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST);          }
#line 9965 "bison-chpl-lib.cpp"
    break;

  case 611: /* task_var_prefix: TIN  */
#line 3374 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::IN);             }
#line 9971 "bison-chpl-lib.cpp"
    break;

  case 612: /* task_var_prefix: TCONST TIN  */
#line 3375 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_IN);       }
#line 9977 "bison-chpl-lib.cpp"
    break;

  case 613: /* task_var_prefix: TREF  */
#line 3376 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::REF);            }
#line 9983 "bison-chpl-lib.cpp"
    break;

  case 614: /* task_var_prefix: TCONST TREF  */
#line 3377 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_REF);      }
#line 9989 "bison-chpl-lib.cpp"
    break;

  case 615: /* task_var_prefix: TVAR  */
#line 3378 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::VAR);            }
#line 9995 "bison-chpl-lib.cpp"
    break;

  case 616: /* task_var_prefix: TOUT  */
#line 3379 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::OUT);   }
#line 10001 "bison-chpl-lib.cpp"
    break;

  case 617: /* task_var_prefix: TINOUT  */
#line 3380 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::INOUT); }
#line 10007 "bison-chpl-lib.cpp"
    break;

  case 618: /* task_var_prefix: TPARAM  */
#line 3381 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::PARAM); }
#line 10013 "bison-chpl-lib.cpp"
    break;

  case 619: /* task_var_prefix: TTYPE  */
#line 3382 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::TYPE);  }
#line 10019 "bison-chpl-lib.cpp"
    break;

  case 620: /* new_maybe_decorated: TNEW  */
#line 3387 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10025 "bison-chpl-lib.cpp"
    break;

  case 621: /* new_maybe_decorated: TNEW TOWNED  */
#line 3389 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10031 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_maybe_decorated: TNEW TSHARED  */
#line 3391 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10037 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3393 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10043 "bison-chpl-lib.cpp"
    break;

  case 624: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3395 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10049 "bison-chpl-lib.cpp"
    break;

  case 625: /* new_expr: new_maybe_decorated expr  */
#line 3401 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10057 "bison-chpl-lib.cpp"
    break;

  case 626: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3408 "chpl.ypp"
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
#line 10074 "bison-chpl-lib.cpp"
    break;

  case 627: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3421 "chpl.ypp"
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
#line 10091 "bison-chpl-lib.cpp"
    break;

  case 628: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3434 "chpl.ypp"
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
#line 10109 "bison-chpl-lib.cpp"
    break;

  case 629: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3448 "chpl.ypp"
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
#line 10128 "bison-chpl-lib.cpp"
    break;

  case 630: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3466 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10136 "bison-chpl-lib.cpp"
    break;

  case 631: /* range_literal_expr: expr TDOTDOT expr  */
#line 3473 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10145 "bison-chpl-lib.cpp"
    break;

  case 632: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3478 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10154 "bison-chpl-lib.cpp"
    break;

  case 633: /* range_literal_expr: expr TDOTDOT  */
#line 3483 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10163 "bison-chpl-lib.cpp"
    break;

  case 634: /* range_literal_expr: TDOTDOT expr  */
#line 3488 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10173 "bison-chpl-lib.cpp"
    break;

  case 635: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3494 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10183 "bison-chpl-lib.cpp"
    break;

  case 636: /* range_literal_expr: TDOTDOT  */
#line 3500 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10193 "bison-chpl-lib.cpp"
    break;

  case 637: /* cast_expr: expr TCOLON expr  */
#line 3530 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10201 "bison-chpl-lib.cpp"
    break;

  case 638: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3537 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10209 "bison-chpl-lib.cpp"
    break;

  case 639: /* super_expr: fn_expr  */
#line 3543 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10215 "bison-chpl-lib.cpp"
    break;

  case 643: /* expr: sub_type_level_expr TQUESTION  */
#line 3552 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10221 "bison-chpl-lib.cpp"
    break;

  case 644: /* expr: TQUESTION  */
#line 3554 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10227 "bison-chpl-lib.cpp"
    break;

  case 648: /* expr: fn_type  */
#line 3559 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10233 "bison-chpl-lib.cpp"
    break;

  case 658: /* opt_expr: %empty  */
#line 3573 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10239 "bison-chpl-lib.cpp"
    break;

  case 659: /* opt_expr: expr  */
#line 3574 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10245 "bison-chpl-lib.cpp"
    break;

  case 660: /* opt_try_expr: TTRY expr  */
#line 3578 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10251 "bison-chpl-lib.cpp"
    break;

  case 661: /* opt_try_expr: TTRYBANG expr  */
#line 3579 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10257 "bison-chpl-lib.cpp"
    break;

  case 662: /* opt_try_expr: super_expr  */
#line 3580 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10263 "bison-chpl-lib.cpp"
    break;

  case 668: /* call_base_expr: expr TBANG  */
#line 3597 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10271 "bison-chpl-lib.cpp"
    break;

  case 669: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3600 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10277 "bison-chpl-lib.cpp"
    break;

  case 672: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3607 "chpl.ypp"
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
#line 10293 "bison-chpl-lib.cpp"
    break;

  case 673: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3619 "chpl.ypp"
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
#line 10309 "bison-chpl-lib.cpp"
    break;

  case 674: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3631 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10317 "bison-chpl-lib.cpp"
    break;

  case 675: /* dot_expr: expr TDOT ident_use  */
#line 3638 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10323 "bison-chpl-lib.cpp"
    break;

  case 676: /* dot_expr: expr TDOT TTYPE  */
#line 3640 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10329 "bison-chpl-lib.cpp"
    break;

  case 677: /* dot_expr: expr TDOT TDOMAIN  */
#line 3642 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10335 "bison-chpl-lib.cpp"
    break;

  case 678: /* dot_expr: expr TDOT TLOCALE  */
#line 3644 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10341 "bison-chpl-lib.cpp"
    break;

  case 679: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3646 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10347 "bison-chpl-lib.cpp"
    break;

  case 680: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3648 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10353 "bison-chpl-lib.cpp"
    break;

  case 681: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3657 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10363 "bison-chpl-lib.cpp"
    break;

  case 682: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3663 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10371 "bison-chpl-lib.cpp"
    break;

  case 683: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3667 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10379 "bison-chpl-lib.cpp"
    break;

  case 684: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3671 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10387 "bison-chpl-lib.cpp"
    break;

  case 685: /* bool_literal: TFALSE  */
#line 3677 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10393 "bison-chpl-lib.cpp"
    break;

  case 686: /* bool_literal: TTRUE  */
#line 3678 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10399 "bison-chpl-lib.cpp"
    break;

  case 687: /* str_bytes_literal: STRINGLITERAL  */
#line 3682 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10405 "bison-chpl-lib.cpp"
    break;

  case 688: /* str_bytes_literal: BYTESLITERAL  */
#line 3683 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10411 "bison-chpl-lib.cpp"
    break;

  case 691: /* literal_expr: INTLITERAL  */
#line 3689 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10417 "bison-chpl-lib.cpp"
    break;

  case 692: /* literal_expr: REALLITERAL  */
#line 3690 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10423 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: IMAGLITERAL  */
#line 3691 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10429 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: TNONE  */
#line 3692 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10435 "bison-chpl-lib.cpp"
    break;

  case 695: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3694 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10444 "bison-chpl-lib.cpp"
    break;

  case 696: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3699 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10453 "bison-chpl-lib.cpp"
    break;

  case 697: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3704 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10462 "bison-chpl-lib.cpp"
    break;

  case 698: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3709 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10471 "bison-chpl-lib.cpp"
    break;

  case 699: /* literal_expr: no_loop_attributes TLSBR expr_ls_semi TRSBR  */
#line 3714 "chpl.ypp"
  {
    (yyval.expr) = context->buildNDArray((yyloc), (yyvsp[-1].ndArrayList));
    context->popLoopAttributeGroup();
  }
#line 10480 "bison-chpl-lib.cpp"
    break;

  case 700: /* literal_expr: no_loop_attributes TLSBR expr_ls_semi TSEMI TRSBR  */
#line 3719 "chpl.ypp"
  {
    (yyval.expr) = context->buildNDArray((yyloc), (yyvsp[-2].ndArrayList));
    context->popLoopAttributeGroup();
  }
#line 10489 "bison-chpl-lib.cpp"
    break;

  case 701: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3724 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10498 "bison-chpl-lib.cpp"
    break;

  case 702: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3729 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10507 "bison-chpl-lib.cpp"
    break;

  case 703: /* assoc_expr_ls: expr TALIAS expr  */
#line 3738 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10516 "bison-chpl-lib.cpp"
    break;

  case 704: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3743 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10526 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TPLUS expr  */
#line 3751 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10532 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TMINUS expr  */
#line 3752 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10538 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TSTAR expr  */
#line 3753 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10544 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TDIVIDE expr  */
#line 3754 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10550 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3755 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10556 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3756 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10562 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TMOD expr  */
#line 3757 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10568 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TEQUAL expr  */
#line 3758 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10574 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3759 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10580 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3760 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10586 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3761 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10592 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TLESS expr  */
#line 3762 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10598 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TGREATER expr  */
#line 3763 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10604 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TBAND expr  */
#line 3764 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10610 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TBOR expr  */
#line 3765 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10616 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TBXOR expr  */
#line 3766 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10622 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr TAND expr  */
#line 3767 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10628 "bison-chpl-lib.cpp"
    break;

  case 722: /* binary_op_expr: expr TOR expr  */
#line 3768 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10634 "bison-chpl-lib.cpp"
    break;

  case 723: /* binary_op_expr: expr TEXP expr  */
#line 3769 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10640 "bison-chpl-lib.cpp"
    break;

  case 724: /* binary_op_expr: expr TBY expr  */
#line 3770 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10646 "bison-chpl-lib.cpp"
    break;

  case 725: /* binary_op_expr: expr TALIGN expr  */
#line 3771 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10652 "bison-chpl-lib.cpp"
    break;

  case 726: /* binary_op_expr: expr THASH expr  */
#line 3772 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10658 "bison-chpl-lib.cpp"
    break;

  case 727: /* binary_op_expr: expr TDMAPPED expr  */
#line 3773 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10664 "bison-chpl-lib.cpp"
    break;

  case 728: /* unary_op_expr: TPLUS expr  */
#line 3777 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10670 "bison-chpl-lib.cpp"
    break;

  case 729: /* unary_op_expr: TMINUS expr  */
#line 3778 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10676 "bison-chpl-lib.cpp"
    break;

  case 730: /* unary_op_expr: TMINUSMINUS expr  */
#line 3779 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10682 "bison-chpl-lib.cpp"
    break;

  case 731: /* unary_op_expr: TPLUSPLUS expr  */
#line 3780 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10688 "bison-chpl-lib.cpp"
    break;

  case 732: /* unary_op_expr: TBANG expr  */
#line 3781 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10694 "bison-chpl-lib.cpp"
    break;

  case 733: /* unary_op_expr: expr TBANG  */
#line 3782 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10702 "bison-chpl-lib.cpp"
    break;

  case 734: /* unary_op_expr: TBNOT expr  */
#line 3785 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10708 "bison-chpl-lib.cpp"
    break;

  case 735: /* reduce_expr: expr TREDUCE expr  */
#line 3790 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10716 "bison-chpl-lib.cpp"
    break;

  case 736: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3794 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10724 "bison-chpl-lib.cpp"
    break;

  case 737: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3798 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10732 "bison-chpl-lib.cpp"
    break;

  case 738: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3802 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10740 "bison-chpl-lib.cpp"
    break;

  case 739: /* scan_expr: expr TSCAN expr  */
#line 3809 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10748 "bison-chpl-lib.cpp"
    break;

  case 740: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3813 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10756 "bison-chpl-lib.cpp"
    break;

  case 741: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3817 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10764 "bison-chpl-lib.cpp"
    break;

  case 742: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3821 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10772 "bison-chpl-lib.cpp"
    break;


#line 10776 "bison-chpl-lib.cpp"

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
