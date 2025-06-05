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
#define YYLAST   17998

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  180
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  209
/* YYNRULES -- Number of rules.  */
#define YYNRULES  751
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1272

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
    1347,  1351,  1360,  1364,  1368,  1372,  1381,  1382,  1383,  1387,
    1388,  1389,  1390,  1391,  1395,  1396,  1397,  1409,  1410,  1411,
    1416,  1417,  1418,  1419,  1420,  1421,  1422,  1423,  1424,  1425,
    1426,  1427,  1428,  1429,  1430,  1431,  1432,  1433,  1437,  1438,
    1439,  1440,  1441,  1442,  1443,  1444,  1445,  1446,  1447,  1448,
    1455,  1456,  1457,  1461,  1462,  1466,  1473,  1483,  1487,  1493,
    1493,  1499,  1499,  1508,  1509,  1510,  1511,  1512,  1513,  1514,
    1518,  1523,  1528,  1533,  1540,  1548,  1549,  1553,  1566,  1574,
    1581,  1590,  1609,  1608,  1618,  1617,  1630,  1637,  1643,  1644,
    1653,  1654,  1655,  1656,  1660,  1683,  1702,  1721,  1746,  1751,
    1756,  1761,  1766,  1771,  1776,  1781,  1786,  1791,  1796,  1801,
    1806,  1811,  1816,  1822,  1831,  1840,  1844,  1848,  1852,  1856,
    1860,  1864,  1868,  1872,  1877,  1882,  1887,  1895,  1911,  1930,
    1937,  1943,  1952,  1953,  1958,  1963,  1964,  1965,  1966,  1967,
    1968,  1969,  1970,  1971,  1972,  1973,  1974,  1979,  1984,  1985,
    1986,  1987,  1995,  1996,  2000,  2004,  2008,  2015,  2019,  2023,
    2030,  2034,  2038,  2042,  2049,  2050,  2054,  2058,  2062,  2069,
    2084,  2102,  2110,  2115,  2125,  2126,  2130,  2134,  2139,  2148,
    2153,  2157,  2164,  2165,  2169,  2179,  2185,  2197,  2204,  2205,
    2206,  2210,  2211,  2215,  2219,  2223,  2227,  2231,  2239,  2258,
    2271,  2278,  2283,  2290,  2289,  2299,  2305,  2304,  2319,  2327,
    2340,  2358,  2355,  2384,  2388,  2389,  2391,  2396,  2397,  2401,
    2402,  2406,  2409,  2411,  2416,  2417,  2428,  2449,  2448,  2464,
    2463,  2481,  2491,  2488,  2516,  2525,  2534,  2544,  2554,  2563,
    2578,  2579,  2587,  2588,  2589,  2598,  2599,  2600,  2601,  2602,
    2603,  2604,  2605,  2606,  2607,  2608,  2609,  2610,  2611,  2612,
    2613,  2614,  2615,  2616,  2617,  2618,  2619,  2620,  2621,  2622,
    2626,  2627,  2628,  2629,  2630,  2631,  2632,  2633,  2634,  2635,
    2636,  2637,  2638,  2639,  2644,  2645,  2649,  2650,  2654,  2655,
    2659,  2660,  2664,  2665,  2669,  2670,  2674,  2678,  2682,  2687,
    2692,  2697,  2702,  2710,  2714,  2722,  2723,  2724,  2725,  2726,
    2727,  2728,  2729,  2730,  2731,  2735,  2736,  2737,  2738,  2739,
    2740,  2741,  2742,  2743,  2744,  2745,  2749,  2750,  2751,  2755,
    2756,  2757,  2758,  2759,  2760,  2761,  2762,  2763,  2764,  2765,
    2769,  2770,  2773,  2774,  2778,  2779,  2783,  2788,  2789,  2791,
    2793,  2795,  2800,  2802,  2807,  2809,  2811,  2813,  2815,  2817,
    2819,  2824,  2825,  2826,  2830,  2839,  2845,  2855,  2859,  2866,
    2888,  2889,  2894,  2895,  2896,  2897,  2898,  2902,  2911,  2910,
    2920,  2919,  2929,  2934,  2943,  2947,  2954,  2973,  2992,  2996,
    3003,  3005,  3007,  3014,  3015,  3016,  3020,  3024,  3025,  3026,
    3030,  3031,  3035,  3036,  3040,  3041,  3042,  3043,  3047,  3048,
    3052,  3056,  3057,  3063,  3064,  3068,  3073,  3078,  3086,  3089,
    3095,  3096,  3102,  3106,  3110,  3117,  3118,  3122,  3127,  3136,
    3137,  3141,  3142,  3149,  3150,  3151,  3152,  3153,  3154,  3156,
    3158,  3160,  3166,  3168,  3171,  3173,  3175,  3177,  3179,  3181,
    3183,  3185,  3187,  3189,  3194,  3198,  3202,  3206,  3210,  3214,
    3223,  3232,  3244,  3248,  3252,  3256,  3260,  3264,  3268,  3272,
    3279,  3297,  3305,  3306,  3307,  3308,  3309,  3310,  3311,  3315,
    3316,  3320,  3324,  3331,  3338,  3348,  3349,  3353,  3357,  3361,
    3368,  3369,  3370,  3371,  3372,  3373,  3374,  3375,  3376,  3377,
    3381,  3383,  3385,  3387,  3389,  3395,  3402,  3415,  3428,  3442,
    3460,  3467,  3472,  3477,  3482,  3488,  3494,  3524,  3531,  3538,
    3539,  3543,  3545,  3546,  3548,  3550,  3551,  3552,  3553,  3556,
    3557,  3558,  3559,  3560,  3561,  3562,  3563,  3564,  3568,  3569,
    3573,  3574,  3575,  3579,  3580,  3581,  3582,  3591,  3592,  3595,
    3596,  3597,  3601,  3613,  3625,  3632,  3634,  3636,  3638,  3640,
    3642,  3651,  3657,  3661,  3665,  3672,  3673,  3677,  3678,  3682,
    3683,  3684,  3685,  3686,  3687,  3688,  3693,  3698,  3703,  3708,
    3713,  3721,  3731,  3741,  3746,  3755,  3760,  3769,  3770,  3771,
    3772,  3773,  3774,  3775,  3776,  3777,  3778,  3779,  3780,  3781,
    3782,  3783,  3784,  3785,  3786,  3787,  3788,  3789,  3790,  3791,
    3795,  3796,  3797,  3798,  3799,  3800,  3803,  3807,  3811,  3815,
    3819,  3826,  3830,  3834,  3838,  3846,  3847,  3848,  3849,  3850,
    3851,  3852
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

#define YYPACT_NINF (-913)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-752)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -913,   260,  3913,  -913,   113,   149,  -913,  -913,  -913,  -913,
    -913,  -913,  -913, 12013,   121,   124,   290, 13828,   294, 17765,
     121,   317,   324,   319,   124,  5305, 12013,   232, 17824,  -913,
     409,   384,  -913,  9946,   392,   423,   297,  -913,   424,   401,
   17824, 17824, 17824,  -913,  2252, 11152,   463, 12013, 12013,   145,
    -913,   469,   472, 12013,  -913, 13828,  -913, 12013,   524,   369,
      58,   258, 13185,   493, 17883,  -913, 12013,  8062, 12013, 11152,
   13828,   456,   504,   419,  5305,   507, 12013,   539,  7036,  7036,
    -913,   540,  -913, 13828,  -913,   544, 10117,  -913, 12013,  -913,
   12013,  -913,  -913, 13657, 12013,  -913, 10288,  -913,  -913,  -913,
    4261,  8233, 12013,  -913,  4957,  -913,  -913,  -913,  -913, 17486,
     593,  -913,   454,   434,  -913,    41,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  7378,
    -913,  7549,  -913,  -913,   141,  -913,  -913,   193,  -913,   548,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,   462, 17824,
    -913,   439,   438,   -13,  -913,   254,  -913,  -913,  -913,   741,
     832,  -913,  -913, 17824,  -913,  1918,  -913,   450,   452,  -913,
    -913,  -913,   459,   461, 12013,   466,   470,  -913,  -913,  -913,
   17265,  1548,   -54,   471,   473,  -913,  -913,   225,  -913,  -913,
    -913,  -913,  -913,   323,  -913,  -913,  -913, 12013,  -913, 17824,
    -913, 12013, 12013,    72,   560,   382,  -913,  -913,  -913,  -913,
   17265,   386,  -913,  -913,    46,  5305,  -913,  -913,  -913,   475,
     -66,   449,  -913,    43,  -913,   476,  -913,   109, 17265,  8404,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913, 17824, 17824,
     -21,  3725,  -913,  -913,   541,  8404,   477,   479,  -913,  5305,
    2435,   117,    10,  -913,  5305,  -913,  -913, 15314,  -913,    39,
   15723,   542,  -913,   481,   482,  -913, 15314,   -66,   542,  -913,
    8404,  1823,  1823,   199,    -5,  -913,    13,  -913,  -913,  -913,
    -913,  7720,  -913,  -913,   358,  -913,   483,   501,  -913,  -913,
    3124,   509,  -913,  -913, 17265,   223, 12013, 12013,  -913,   -30,
    -913,  -913, 17265,   492, 15842,  -913, 15314,   -66,   487,  8404,
    -913, 17265, 15882, 12013,  -913,  -913,  -913,  -913,  -913,   -66,
     497,   321,   321,  1503,   542,   542,   312,  -913,  -913,  4435,
       7, 12013,   514,   -57,   491,  1503,   656,  -913,  -913, 17824,
    -913, 12013,  -913,  4261,  -913,   270,  -913,   541,  -913,  -913,
     663,   499,  4609, 12013,  -913, 12013,   601,  -913,  -913, 14985,
      36,   462, 17265,   256,  -913,  5305,   590,  -913,  -913,  9946,
   10459, 11323,  -913,  -913,  -913, 17824,  -913, 17824, 12013,   502,
    -913, 16843,   597,   741,  -913,  -913,  -913,  -913,   200,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,    89,   217,  -913,  -913,
    -913,  -913,  -913,  -913,  -913, 13491,   550,   262,  -913,   511,
     298,   391,  -913,   529, 12013, 12013, 12013, 11323, 11323, 12013,
     398, 12013, 12013, 12013, 12013, 12013,   263, 13657, 12013, 12013,
   12013, 12013, 12013, 12013, 12013, 12013, 12013, 12013, 12013, 12013,
   12013, 12013, 12013, 12013,   611,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913, 10117,  -913,  -913, 10117,
   10117,  8404,  8575, 11323, 11323, 15922,   510,  -913, 11494, 11323,
   17824,  -913,  6346,  -913,  -913,  -913,  2252,  -913, 12013,  -913,
     562,   512,   545,  -913,  -913,   568,   569,  5305,   664,  5305,
    -913,   665, 12013,   631,   532,  -913, 10117,  -913,  2435,  -913,
    -913,    93,  -913, 12184,   581, 12013,  2252,  -913,  -913, 12013,
    -913, 17615, 12013, 12013,  -913,    43,   538,  -913,  -913,  -913,
    -913, 17824,  -913,   741, 13356,  2435,   567, 12184,  -913, 17265,
   17265,  -913,   449,  -913,    45,  -913,  8404,   543,  -913, 15314,
     692,   692,  -913,  -913,  -913,  -913, 11668,  -913, 16001,  8749,
    8920,  -913,  -913,  -913, 17265, 11323, 11323,  -913,   454,  9091,
    -913,   314,  -913,  4783,  -913,   299, 16041,   300, 15075, 17824,
    6862,  6691,  -913,   462,   547,  -913,  -913,    43,  -913,   599,
   17824,    67, 15314,   553, 13970,   -44,   151, 12013,   -12, 16082,
     -68,    27, 14820,  -913,   106,  -913,   118,   572,  2687,   549,
   17706,   607,   122,  -913,  -913,   123,  -913,  -913,    62,  -913,
    -913,  -913,  6520,  -913,   630,  -913,  -913,   573,   591,  -913,
     595,  -913,   598,   602,   604,   610,   612,  -913,   613,   615,
     618,   622,   624,   625,   627,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913, 12013,   605,   628,
     573,  -913,   573,  -913, 12013,  -913, 17824,  -913,  -913,  -913,
    2231,  2231,   402,  -913,   402,  -913,   402, 17304,   555,  1301,
    1465,   -66,   321,  -913,   399,  -913,  -913,  -913,  -913,  -913,
    1503,   854,   402,   589,   589,  2231,   589,   589,   954,   321,
     854, 17463,   954,   542,   542,   321,  1503,   614,   616,   619,
     620,   587,   608,  -913,   402,  -913,   402, 12013, 10117,    97,
     753, 15394,   623,   617,   253,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,   129,  -913,  1749, 17344,   342,   301, 17265,
   10117,  -913, 10117, 12013, 12013,   752,  5305,   757, 16233,  5305,
   15473, 17824,  -913,   170,   181,  2435,   117,  -913, 17265,  9262,
    -913, 17265,  -913,  -913,  -913, 17824, 16273, 16313,  2252,  -913,
     567,   642,  -913,   185, 12184,  -913,   208,  -913, 12013,  -913,
     629,    22,   621,  -913,  2068,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,    26,    30, 14894,  -913,  -913,   188,  -913,
    -913,  -913,  -913, 12013,  -913,    88, 14069, 12013,  -913, 12013,
    6862,  6691,  -913,  9433,   389,  9604,   395,   426,  9775,  7891,
     437,   306,  -913,  -913,  -913, 16392,   651,   636,   633,  -913,
   17824,  2252, 12013,   756,  -913,  -913, 12013, 17265,  5305,   634,
   11323, 10633,  -913,  4087,   171,   640,   658,  -913,   230, 12013,
   11839,  -913,  5305,  -913,    67,  5479, 11323,  -913,    67, 17824,
     449,  -913, 10117,   635,  3482,  -913,  -913, 12013,    51,  -913,
    1163,  -913,   630,  -913,  -913,  -913,  -913,  -913,    76,   730,
    -913, 16432,  -913, 14227,  -913,  -913, 17265,  -913,   638,   641,
    -913,  -913,  -913,  -913,  -913,  -913,   191, 11323, 13970, 12013,
   12355, 11323, 10117,  -913,  -913,  -913,  7207,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,    44,
   11323, 11323,  -913,  -913,  -913, 17265, 17265,  5305,  -913,  5305,
   12013,  -913,  5305,   773,   652,   654,  -913,  -913,   581,  -913,
    -913,  -913,   648,   653,   307,   208, 12013,   630,   567,  -913,
   10807,  -913,  -913, 17265,  -913,  5305, 12013,  -913,  -913,  -913,
   17824,  -913,   680,   449, 11323,  5305, 11323, 10117,  -913,  -913,
    -913,  -913,  -913,   399,  -913,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  6862,  -913,  -913,
   16480, 15564,   311,   662,  6862,  -913, 12013,  -913,  -913,  3595,
     346,   347, 16583, 12013,   209,  -913,  5305,   -34, 15638,  -913,
     233,  -913, 17265,  -913,  -913,  5305, 10978,  -913, 17265,  -913,
   16631, 17265,  -913,  9946, 17265,    56, 15154,  -913,  -913,  -913,
     221,  -913,  -913,  -913,  -913,  2795,   508,  -913, 17265, 17824,
     593,  -913,    76,    77, 12013,  -913,   681,   668,  -913,   573,
     573,  -913,  -913,  -913,   660, 16671, 12013,   796, 16822,   228,
    -913,   581,   234,   240,  -913,  -913, 17265,   801,  5305,  -913,
    -913,  -913,  8404,  8404,  -913,  -913,  -913,  -913,  -913,   208,
   17265,  -913,   281,    63,   669, 12013,  -913,    40, 15683,    30,
   15234,  -913,   205, 12013,  6862,  6691,  -913,  -913,  -913, 16862,
   17265, 14383,  -913,  -913,  -913,  -913, 15314,  -913,  -913,  5653,
     671,  5827,   673,  -913,  -913, 12013, 14411,  6001,  -913,    67,
    6175,  -913,    67,   672,   508,  -913,  -913,  -913, 17706,  -913,
    -913,  -913,   132,  -913,    84,  -913,   265, 16903,    69,  -913,
    -913,  -913, 12526, 12697, 17018, 12868, 13039,  -913,  -913,  5305,
    -913,   676,   677,  -913,  -913,  5305,  -913,   449, 17265,  5305,
    5305,  -913, 17265,   362,   679, 12013,  -913,  -913,  9946,  -913,
   17265,  5305,  9946,  -913, 17265,  5305, 17265,  5305,  9946, 17265,
    -913,  9946, 17265,  -913,  -913,  -913,  -913,    68,  -913, 12013,
      77,   132,   132,   132,   132,   132,   132,    77,  5131,  -913,
    -913,  -913,  -913, 12013, 12013, 12013, 12013, 12013,  -913,   688,
     689,   599,  -913,  -913,  -913, 17265, 14491,  -913, 14571,  -913,
   16233, 14651, 14731,  -913, 17265,  -913,  -913,  -913,  -913,  -913,
    -913,  -913,   707,  -913, 17066, 17106, 17185, 17225,  -913,  -913,
    5305,  5305,  5305,  5305, 12013, 12013, 12013, 12013, 16233, 16233,
   16233, 16233
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   137,   466,    55,   691,   692,
     693,   687,   688,   207,   599,   126,   158,   570,   165,   572,
     599,   164,   498,   495,   126,     0,   207,   272,   159,   196,
     195,   685,   194,   207,   163,    68,   273,   335,   160,   138,
       0,     0,     0,   330,     0,   207,   167,   207,   207,   620,
     591,   694,   168,   207,   336,   564,   492,   207,     0,     0,
     181,   179,   423,   162,   573,   494,   207,   207,   207,   207,
     568,     0,   166,     0,     0,   139,   207,   686,   207,   207,
     485,   161,   310,   566,   496,   169,   207,   747,   207,   749,
     207,   750,   751,   636,   207,   748,   207,   644,   177,   746,
       0,   207,   207,     4,     0,     5,    10,    11,    12,    48,
      51,    52,    56,     0,    45,    71,    13,    77,    14,    15,
      16,    17,    29,   551,   552,    22,    46,   178,   188,   207,
     197,   658,   189,    18,     0,    31,    30,     0,    47,     0,
     267,    19,   654,    21,    35,    32,    33,   187,   311,     0,
     185,     0,     0,   651,   334,     0,   648,   183,   351,   449,
     435,   646,   186,     0,   497,     0,   184,   663,   642,   641,
     645,   555,   553,     0,   207,   652,   653,   657,   656,   655,
       0,   554,     0,   664,   665,   666,   689,   690,   647,   557,
     556,   649,   650,     0,    28,   572,   159,   207,   138,     0,
     573,   207,   207,     0,     0,   651,   663,   553,   652,   653,
     562,   554,   664,   665,     0,     0,   600,   127,   128,     0,
     571,     0,   486,     0,   493,     0,    20,     0,   533,   207,
     134,   140,   151,   146,   145,   153,   131,   144,   154,   141,
     135,   155,   129,   156,   149,   143,   150,   147,   148,   130,
     132,   142,   152,   157,   136,   320,   133,   206,     0,     0,
       0,     0,    70,    69,    71,   207,     0,     0,   249,     0,
       0,     0,     0,   504,     0,   174,    40,     0,   302,     0,
     301,   732,   624,   621,   622,   623,   500,   565,   733,     7,
     207,   333,   333,   430,   171,   425,   172,   426,   427,   431,
     432,   170,   433,   434,   423,   529,     0,   338,   339,   341,
       0,   424,   528,   343,   516,     0,   207,   207,   175,   648,
     639,   662,   640,     0,     0,    43,     0,   569,     0,   207,
      44,   563,     0,   207,   280,    48,   284,   281,   284,   567,
       0,   734,   736,   634,   731,   730,     0,    74,    78,     0,
       0,   207,     0,     0,   542,   635,     0,     6,   308,     0,
     195,   207,   309,     0,    49,     0,     9,    71,    50,    53,
       0,    60,     0,   207,    72,   207,     0,   498,   193,     0,
     687,   311,   659,   202,   213,     0,   210,   211,   212,   207,
     207,   207,   209,   574,   582,     0,   251,     0,   207,     0,
     307,     0,   423,   449,   447,   448,   446,   359,   451,   456,
     459,   450,   454,   453,   455,   458,     0,   439,   441,   445,
     444,   436,   437,   440,   443,     0,   490,     0,   487,     0,
       0,   643,    34,   625,   207,   207,   207,   207,   207,   207,
     735,   207,   207,   207,   207,   207,     0,   633,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   390,   397,   398,   399,   394,   396,
     400,   401,   392,   395,   393,   391,   207,   403,   402,   207,
     207,   207,   207,   207,   207,     0,     0,   210,   207,   207,
       0,   604,     0,    36,    24,    37,     0,    25,   207,    38,
     551,     0,   546,   547,   550,     0,     0,     0,   240,     0,
     390,   236,   207,     0,     0,    23,   207,    26,     0,   508,
     510,     0,   527,   207,   513,   207,     0,   173,    39,   207,
     304,     0,   207,   207,    41,     0,     0,   182,   180,   428,
     429,     0,   424,   449,   423,     0,   531,   207,   121,   660,
     661,   349,     0,   176,     0,    42,   207,     0,   291,     0,
     282,   283,    27,    76,    75,    79,   207,   695,     0,   207,
     207,   683,   681,     8,   500,   207,   207,   208,    56,   207,
      54,     0,    65,     0,   119,     0,   114,     0,    86,     0,
     207,   207,   190,   311,     0,   198,   204,     0,   201,     0,
       0,     0,     0,     0,   583,     0,     0,   207,     0,   533,
       0,     0,     0,   254,     0,   252,   278,   312,     0,     0,
       0,   328,     0,   321,   410,     0,   412,   416,     0,   457,
     452,   521,     0,   523,   460,   442,   438,   408,   134,   386,
     140,   384,   146,   145,   144,   141,   135,   388,   155,   156,
     143,   147,   130,   142,   157,   383,   365,   368,   366,   367,
     389,   378,   369,   382,   374,   372,   385,   373,   371,   376,
     381,   370,   375,   379,   380,   377,   387,   207,   362,     0,
     408,   363,   408,   360,   207,   489,     0,   484,   503,   502,
     727,   726,   729,   738,   737,   742,   741,   723,   720,   721,
     722,   637,   710,   137,     0,   677,   678,   139,   676,   675,
     631,   714,   725,   719,   717,   728,   718,   716,   708,   713,
     715,   724,   707,   711,   712,   709,   632,     0,     0,     0,
       0,     0,     0,   740,   739,   744,   743,   207,   207,     0,
       0,     0,   278,     0,   610,   611,   617,   616,   618,   613,
     619,   615,   602,     0,   605,     0,     0,     0,     0,   534,
     207,   559,   207,   207,   207,   239,     0,   235,     0,     0,
       0,     0,   558,     0,     0,   511,     0,   526,   525,   207,
     506,   630,   505,   303,   300,     0,     0,     0,     0,   674,
     531,   344,   340,     0,   207,   532,   513,   342,   207,   348,
       0,     0,     0,   560,     0,   285,   696,   638,   682,   543,
     542,   684,   544,     0,     0,     0,    57,    58,     0,    61,
      63,    67,    66,   207,    97,     0,     0,   207,    92,   207,
     207,   207,    64,   207,   365,   207,   366,   367,   207,   207,
     377,     0,   404,   405,    81,    80,    91,     0,     0,   313,
       0,     0,   207,     0,   216,   215,   207,   584,     0,     0,
     207,   207,   540,     0,   535,     0,   538,   699,   537,   207,
     207,   703,     0,   224,     0,     0,   207,   222,     0,     0,
       0,   275,   207,     0,   333,   319,   325,   207,   323,   318,
     423,   411,   460,   519,   518,   517,   520,   461,   467,   423,
     358,     0,   364,     0,   354,   355,   491,   488,     0,     0,
     125,   124,   123,   122,   672,   673,     0,   207,   697,   207,
     207,   207,   207,   603,   612,   614,   207,   601,   158,   165,
     164,   163,   160,   167,   168,   162,   166,   161,   169,     0,
     207,   207,   499,   549,   548,   248,   247,     0,   242,     0,
     207,   238,     0,   244,     0,   277,   509,   512,   513,   514,
     515,   299,     0,     0,     0,   513,   207,   460,   531,   530,
     207,   419,   417,   350,   293,     0,   207,   292,   295,   561,
       0,   286,   289,     0,   207,     0,   207,   207,    59,   120,
     117,    98,   109,   104,   103,   102,   111,    99,   112,   107,
     101,   108,   105,   106,   100,   110,   113,   207,   116,   115,
      88,    87,     0,     0,   207,   191,   207,   192,   313,   333,
       0,     0,     0,   207,     0,   232,     0,     0,     0,   698,
     536,   230,   585,   541,   701,     0,   207,   700,   705,   704,
       0,   578,   225,   207,   577,     0,     0,   223,   253,   250,
       0,   306,   181,   179,   305,   333,   333,   314,   329,     0,
       0,   413,   467,     0,   207,   352,   415,     0,   361,   408,
     408,   679,   680,   277,     0,     0,   207,     0,     0,     0,
     606,   513,   663,   663,   241,   237,   590,   243,     0,    73,
     274,   507,   207,   207,   501,   420,   418,   345,   346,   513,
     407,   297,     0,     0,     0,   207,   287,     0,     0,     0,
       0,    62,     0,   207,   207,   207,    93,    95,    84,    83,
      82,   333,   199,   205,   203,   214,     0,   234,   233,     0,
       0,     0,     0,   702,   231,   207,     0,     0,   220,     0,
       0,   218,     0,   279,   333,   316,   315,   324,     0,   331,
     481,   482,     0,   483,   469,   472,     0,   468,     0,   409,
     356,   357,   207,   207,     0,   207,   207,   279,   607,     0,
     246,     0,     0,   422,   421,     0,   296,     0,   290,     0,
       0,   118,    89,     0,     0,   207,   200,   217,   207,   228,
     587,     0,   207,   226,   586,     0,   706,     0,   207,   576,
     221,   207,   575,   219,   276,   317,   327,     0,   480,   207,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   462,
     464,   353,   463,   207,   207,   207,   207,   207,   245,   626,
     627,   298,   288,    94,    96,    85,     0,   229,     0,   227,
     581,     0,     0,   332,   471,   473,   474,   477,   478,   479,
     475,   476,   470,   465,     0,     0,     0,     0,   628,   629,
       0,     0,     0,     0,   207,   207,   207,   207,   589,   588,
     580,   579
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -913,  -913,  -913,    -1,   -53,  2171,  -913,   496,   745,  -913,
    -913,   498,  -398,  -105,  -913,   279,  -913,  -913,  -129,  -913,
    -913,    32,   596,  -913,  -548,  2670,   490,  -582,  -913,  -795,
    -913,  -913,  -913,    42,  -913,  -913,  -913,   840,  -913,  1883,
    -197,  -913,  -913,  -463,   211,  -913,  -845,  -913,  -913,   287,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,     5,
    -913,  -153,   506,  -373,  -913,   478,  -913,  -913,  -913,   -10,
    1484,  -913,  -913,  -913,  -913,   528,  -913,  -108,  -913,  -913,
    -913,  -913,   345,  -913,  -913,  -913,   -94,  -913,  -339,  -794,
    -913,  -913,  -913,  -913,  -913,  -609,  -913,    29,  -913,  -913,
    -913,  -913,  -913,   331,  -913,  1337,  -913,  -913,  -913,  -913,
     495,  -913,  -913,  -913,  -913,   -22,  -409,  -175,  -788,  -912,
    -639,  -913,   -17,  -913,     1,   -49,   584,   -48,  -913,  -913,
    -355,  -831,  -913,  -318,  -913,  -169,  -323,  -313,  -714,  -913,
    -913,    48,   214,  -913,   372,  -206,   776,  -913,  -913,  -155,
     380,   134,  -390,  -763,  -610,  -913,  -913,  -913,  -600,  -507,
    -913,  -746,   -14,  -913,  -913,  -573,    14,  -913,  -263,  -487,
    -558,    -2,  -913,  -913,  -913,  -913,  -913,  -913,   377,  -913,
     887,  -586,  -913,   -16,  -913,  -913,   527,   564,  -913,  -913,
    -913,  -913,  2526,  -913,   -52,  1026,  -913,  1033,  1385,  -913,
    -913,  -913,  -913,  -913,  -913,  -913,  -913,  -913,  -468
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   348,   104,   105,   106,   107,   108,   365,
     366,   109,   110,   111,   112,   371,   580,   818,   819,   113,
     114,   115,   376,   116,   117,   118,   349,   846,   587,   847,
     119,   120,  1008,   584,   585,   121,   122,   219,   254,   271,
     123,   256,   124,   305,   534,   125,   126,   292,   291,   127,
     128,   129,   130,   131,   596,   132,   597,   850,   133,   203,
     135,   391,   392,   605,   136,   260,   137,   138,   614,   615,
     204,   140,   141,   142,   143,   560,   805,   983,   144,   145,
     801,   978,   278,   279,   146,   147,   148,   149,   399,   884,
     150,   151,   622,  1059,  1060,   623,   152,   205,  1207,   154,
     155,   306,   307,   308,   967,   156,   320,   552,   798,   157,
     158,  1158,   159,   160,   679,   680,   842,   843,   844,   971,
     900,   403,   625,  1067,   626,   627,   310,   542,   425,   407,
     416,   898,  1221,  1222,   161,  1065,  1154,  1155,  1156,   162,
     163,   427,   428,   685,   164,   165,   166,   223,   535,   272,
     273,   520,   521,   780,   312,   896,   633,   634,   524,   313,
     795,   796,   350,   607,   608,   864,   352,   353,   501,   502,
     503,   206,   168,   169,   393,   170,   394,   171,   207,   173,
     215,   216,   753,   754,   755,   174,   208,   209,   177,   178,
     179,   321,   180,   383,   504,   211,   182,   212,   213,   185,
     186,   187,   188,   610,   189,   190,   191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   103,   514,   620,   799,   369,   480,   134,   841,   304,
     430,   886,   227,   309,   311,   323,   681,   496,   611,   859,
     865,   820,   895,   167,   757,   874,   878,   536,   892,   773,
     134,   153,   500,   972,   340,   868,  1013,   381,  1009,  1057,
     797,   904,   594,   905,   965,   522,   800,   491,   628,   354,
     489,  -272,   315,  1095,   153,  1019,  -326,   525,   358,   872,
     777,  1062,   274,   522,   693,   695,   557,   264,   500,  -273,
      23,   274,   167,   984,   440,   214,   167,   167,   870,   134,
    1150,   507,   446,   134,   134,   214,   512,   373,  1137,   570,
     631,   703,   551,   500,  -524,   274,   384,   975,   167,   274,
    1218,  1218,   167,   153,   -70,   134,   871,   153,   153,   134,
     733,   735,   487,   387,   388,   571,   740,   481,   522,   482,
    -294,  -522,    56,  -524,   -70,   362,   374,   217,   774,   153,
     858,  1151,   500,   153,    65,  1150,  1098,  1063,   976,  -347,
    1129,   367,   198,   -70,   917,   214,   214,    82,   363,   214,
    -522,  -596,   862,   566,   375,   793,   526,    84,  -670,   214,
    -670,  -294,   867,   282,  -524,   384,   229,  -524,  -524,   894,
    1152,   -70,   498,   385,  1057,   214,   958,   567,   218,  -524,
    1153,   386,   387,   388,   265,   529,  1151,  1173,   791,   523,
    -524,   707,   977,  1064,  -522,  1091,   363,  -322,   860,   363,
     985,  1209,  1096,   814,   944,  -255,  -522,   523,   363,   498,
    1145,  1146,   576,   167,  1179,  -294,  -524,   492,   731,   732,
     134,  -322,  1099,   283,  1121,   363,  1118,   865,   874,   878,
    1210,  -524,   363,  1219,   632,  1153,   363,   363,   363,   775,
     214,  -524,   284,   861,   153,   488,   539,   629,  1012,   709,
     682,   916,   879,  -522,   848,   498,   276,   285,  -522,   389,
       3,   862,   523,  -524,   635,   776,   703,   167,   888,   890,
     214,   918,   167,   499,   134,   926,  1057,   194,   880,   134,
     325,  -524,   881,   704,   500,   500,   593,   969,  1030,   882,
     540,   630,   889,   802,   384,   891,   705,   861,   153,  1205,
     924,   927,   385,   153,   -69,   384,    37,   404,   636,  -333,
     386,   387,   388,  1175,   390,   862,   762,   198,  1168,   500,
    1184,   386,   387,   388,   -69,   863,   706,   775,   405,   788,
    -333,   775,   779,    54,   987,  1033,  1174,   762,   406,  1081,
    -333,   758,   955,   -69,   925,  1034,  -256,   167,   565,   304,
    -262,  1014,   436,   956,   134,   498,  1097,   968,   970,   500,
     988,   167,   363,  1073,   395,   595,   707,   762,   134,   498,
     167,   -69,   708,  -261,   762,  1181,   606,   134,   153,   172,
    -609,  1127,   500,   167,   617,   293,  -608,   548,   389,  1211,
     134,   851,   153,  1143,  1033,  1050,  -671,  1033,  -671,   389,
    1167,   153,   172,   229,  1037,   295,  -609,  1133,   686,   297,
     224,   437,  -608,   483,   153,   438,  1212,   484,  1213,  1214,
     257,  1215,  1216,   683,   728,  1112,   687,   729,   730,   820,
    1160,  1161,   941,   222,   298,  1079,   484,   299,  1208,   356,
    -270,  1130,  1132,   575,   526,   823,   827,   526,  -260,   300,
    1147,   172,  1014,   526,   390,   172,   172,  1014,   757,  1139,
    1142,   440,   689,   824,   828,   942,   444,   302,   265,   446,
    1015,  1094,   303,   449,   739,  1116,   194,   172,   194,  -749,
    -257,   172,   563,  -749,   821,  -750,  1056,  1027,   528,  -750,
     530,   304,   686,   526,   681,   309,   311,  1246,  1247,  1248,
    1249,  1250,  1251,  1045,   266,   167,   262,   167,  1014,   263,
    1123,  1124,   134,     7,   134,   369,  -751,   810,   810,  -264,
    -751,  1130,  1132,  1139,  1142,  -268,  1233,  -746,  -265,   175,
     358,  -746,  1183,   289,    22,    23,   153,   555,   153,  1206,
     290,   500,   440,   359,  1074,    29,   360,   444,  1077,  -259,
     446,    32,   175,  -670,   449,  -670,   328,  -667,    37,  -667,
    -263,   813,  -669,  -269,  -669,   500,   176,   693,   733,  -668,
     908,  -668,   909,   436,   334,   337,   335,   335,   537,   538,
     361,   167,   565,   809,   812,    54,   436,    56,   134,   176,
     329,  1052,   172,   866,  1053,  -271,  -258,   362,     7,    65,
    -266,   175,   370,   372,   397,   175,   175,   398,   401,   402,
    1220,  1107,   153,  1109,  -593,   431,   490,    80,   363,    82,
     436,  1056,    84,  -592,   293,   432,   374,   175,   990,   709,
    -597,   175,   437,   964,  -598,  -595,   438,  -594,   176,   494,
     497,   515,   176,   176,   295,   437,   172,   544,   297,   438,
     516,   172,   532,   533,   547,   543,   553,  1144,   556,  1220,
     569,   562,  1148,   572,   176,   573,   578,   589,   176,   600,
     579,   618,    98,   298,   684,   688,   299,   446,    58,   437,
     727,   738,   440,   438,   761,   500,   760,   444,   300,   445,
     446,   762,   763,   764,   449,   440,  1021,   771,   766,   769,
     444,   456,   445,   446,   772,   779,   302,   449,   943,   462,
     789,   303,   794,   804,   456,   803,   849,   852,   498,   885,
     460,   461,   462,  1056,   856,   500,   172,   960,  1070,   440,
     441,   887,   442,   443,   444,   897,   445,   446,   447,  -137,
     172,   449,   175,  -158,   899,   902,  -165,   455,   456,   172,
    -164,   459,  -163,   939,   460,   461,   462,   293,  -160,   914,
    -138,  -167,   172,  -168,   167,   463,  -162,   167,   408,   624,
    -166,   134,  -139,  -161,   134,  -169,   903,   295,   910,   176,
     911,   297,   915,   912,   913,   919,   947,   966,   409,   923,
     500,   949,   410,   979,   922,   153,   175,  1014,   153,   974,
    1017,   175,  1018,  1023,  1036,  1051,   298,  1088,  1026,   299,
    1071,    58,   854,   855,  1035,  1072,  1089,   411,  1090,  1092,
     412,   300,   873,   877,  1093,  1105,  1117,   890,  1165,  1171,
    1172,  1055,   413,   176,  1162,  1169,  1204,   304,   176,   302,
    1159,  1177,  1024,  1234,   303,  1191,   304,  1195,  1229,  1230,
     414,  1258,  1259,  1210,   364,   415,   167,   816,  1111,   417,
     513,   167,   583,   134,   225,   989,   561,   601,   134,  1048,
     167,   577,  1104,   167,   783,   792,   175,   134,   598,   418,
     134,  1069,  1066,   419,   172,   436,   172,   153,   541,  1243,
     175,  1061,   153,  1149,  1252,   500,   500,  1245,  1020,   175,
     907,   153,  -414,   785,   153,   378,   782,   221,   420,   957,
    1080,   421,   175,   176,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   422,     0,     0,     0,   176,     0,     0,
       0,     0,     0,     0,     0,     0,   176,     0,  1082,  1083,
       0,   423,     0,     0,   437,   167,   424,   167,   438,   176,
     167,   369,   134,     0,   134,     0,     0,   134,     0,     0,
     172,   512,  1103,     0,     0,     0,  1055,     0,     0,     0,
       0,     0,     0,   167,     0,     0,   153,     0,   153,     0,
    1102,   153,     0,   167,     0,   436,     0,     0,     0,     0,
     134,     0,     0,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   153,     0,   449,   450,   451,     0,
     453,   454,   455,   456,   153,     0,   459,     0,     0,   460,
     461,   462,     0,     0,   167,   873,   877,     0,   181,     0,
     463,   134,     0,   167,   175,   183,   175,     0,     0,   369,
     134,     0,     0,   369,   437,     0,     0,     0,   438,     0,
       0,   181,     0,     0,     0,   153,     0,     0,   183,     0,
       0,   512,     0,   512,   153,     0,   512,   512,  1055,     0,
       0,   176,     0,   176,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1042,   167,     0,     0,  1047,
       0,     0,     0,   134,   440,   441,     0,   442,   443,   444,
     181,   445,   446,     0,   181,   181,   449,   183,     0,     0,
     175,   183,   183,   456,     0,     0,     0,   153,     0,   460,
     461,   462,     0,     0,     0,     0,   181,   167,     0,   167,
     181,     0,     0,   183,   134,   167,   134,   183,   167,     0,
       0,     0,   134,   172,     0,   134,   172,   176,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   153,     0,
     153,     0,     0,     0,     0,     0,   153,   167,     0,   153,
       0,     0,     0,   167,   134,     0,     0,   167,   167,     0,
     134,     0,     0,     0,   134,   134,     0,     0,     0,   167,
     293,     0,     0,   167,     0,   167,   134,     0,   153,     0,
     134,     0,   134,     0,   153,     0,     0,     0,   153,   153,
     295,     0,     0,     0,   297,     0,   167,  1253,     0,     0,
     153,     0,     0,   134,   153,     0,   153,     0,     0,     0,
       0,     0,     0,     0,     0,   172,     0,     0,     0,   298,
     172,   181,   299,     0,    58,     0,     0,   153,   183,   172,
       0,     0,   172,     0,   300,     0,  1138,  1141,   167,   167,
     167,   167,     0,     0,     0,   134,   134,   134,   134,     0,
       0,     0,   302,     0,     0,     0,     0,   303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   153,
     153,   153,   153,   175,     0,   181,   175,     0,     0,     0,
     181,     0,   183,     0,     0,     0,     0,   183,     0,     0,
       0,     0,     0,     0,  1176,     0,     0,     0,     0,     0,
    1138,  1141,     0,     0,   172,     0,   172,     0,     0,   172,
     176,     0,   436,   176,     0,     0,     0,  1187,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1200,     0,   172,  1203,     0,     0,     0,     0,     0,     0,
       0,     0,   172,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   181,     0,     0,     0,     0,
       0,     0,   183,     0,     0,   175,     0,   184,     0,   181,
     175,   437,     0,     0,     0,   438,   183,     0,   181,   175,
       0,     0,   175,   172,   319,   183,     0,     0,     0,     0,
     184,   181,   172,     0,     0,     0,     0,     0,   183,     0,
       0,     0,   176,   319,     0,     0,     0,   176,     0,     0,
       0,     0,     0,     0,     0,     0,   176,     0,   319,   176,
       0,   440,   441,     0,     0,   443,   444,     0,   445,   446,
       0,     0,     0,   449,     0,     0,     0,     0,     0,   184,
     456,     0,     0,   184,   184,   172,   460,   461,   462,     0,
       0,     0,     0,     0,   175,     0,   175,     0,     0,   175,
       0,     0,     0,     0,     0,   184,   139,     0,     0,   184,
       0,     0,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,   175,     0,     0,     0,   172,     0,   172,   139,
       0,   176,   175,   176,   172,     0,   176,   172,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   181,   436,   181,     0,     0,     0,   176,
     183,     0,   183,     0,     0,     0,   172,     0,     0,   176,
       0,     0,   172,   175,     0,   437,   172,   172,   139,   438,
       0,     0,   175,     0,     0,     0,   319,     0,   172,     0,
       0,     0,   172,     0,   172,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   139,     0,     0,     0,   139,     0,
     176,     0,     0,   437,     0,   172,     0,   438,     0,   176,
     184,     0,   319,     0,     0,   440,   441,     0,     0,   181,
     444,     0,   445,   446,     0,   175,   183,   449,     0,     0,
       0,     0,     0,     0,   456,     0,     0,   319,     0,     0,
     460,   461,   462,     0,     0,     0,     0,   172,   172,   172,
     172,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   176,     0,   184,   449,   175,     0,   175,   184,
       0,   455,   456,     0,   175,   459,   319,   175,   460,   461,
     462,     0,   464,   465,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,   476,   477,   478,     0,     0,     0,
       0,     0,     0,   176,     0,   176,   175,     0,     0,   139,
       0,   176,   175,     0,   176,     0,   175,   175,     0,     0,
       0,     0,     0,     0,     0,     0,   479,     0,   175,  -667,
       0,  -667,   175,     0,   175,     0,     0,     0,     0,     0,
       0,     0,     0,   176,   184,     0,     0,     0,     0,   176,
       0,     0,     0,   176,   176,   175,     0,     0,   184,     0,
       0,     0,   703,   139,     0,   176,     0,   184,   139,   176,
       0,   176,     0,     0,     0,     0,   928,     0,     0,   929,
     184,     0,     0,     0,   930,     0,     0,     0,     0,     0,
       0,     0,   176,     0,   196,     0,     0,   175,   175,   175,
     175,     0,   181,     0,   931,   181,     0,     0,     0,   183,
       0,   932,   183,   198,     0,     0,     0,     0,     0,     0,
       0,     0,   933,   319,     0,     0,   319,   319,   319,   319,
     934,     0,     0,     0,   176,   176,   176,   176,     0,     0,
       0,     0,     0,   139,     0,     0,     0,   935,     0,     0,
       0,     0,     0,     0,     0,   358,     0,   139,   936,    22,
      23,     0,   707,   319,     0,     0,   139,     0,   359,   937,
      29,   360,     0,     0,   938,     0,    32,     0,     0,   139,
       0,     0,     0,    37,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   181,     0,     0,     0,     0,   181,
       0,   183,   184,   319,   184,   361,   183,     0,   181,     0,
      54,   181,    56,     0,     0,   183,   319,   319,   183,     0,
       0,   255,   362,     0,    65,     0,   319,     0,     0,   429,
       0,   230,     0,   267,   268,   269,     0,     0,     0,     0,
       0,     0,    80,     0,    82,   231,   232,    84,   233,     0,
       0,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,   235,     0,     0,     0,     0,     0,   236,     0,     0,
       0,     0,     0,   237,     0,     0,     0,   238,   184,     0,
     239,     0,   240,   181,     0,   181,     0,     0,   181,     0,
     183,   241,   183,     0,     0,   183,     0,     0,   242,   243,
       0,   139,     0,   139,     0,     0,   244,     0,     0,     0,
       0,   181,     0,     0,     0,     0,   245,     0,   183,     0,
       0,   181,     0,     0,     0,   246,     0,   247,   183,   248,
       0,   249,     0,     0,   250,     0,     0,     0,   251,     0,
     252,     0,   400,   253,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   426,     0,     0,     0,
       0,     0,   181,     0,     0,     0,     0,     0,     0,   183,
       0,   181,     0,     0,     0,     0,     0,   139,   183,     0,
       0,   230,     0,     0,     0,   319,     0,     0,     0,     0,
       0,     0,   486,     0,     0,   231,   232,     0,   233,   270,
       0,     0,     0,   234,     0,     0,     0,   319,     0,   319,
       0,   235,     0,     0,     0,     0,     0,   236,     0,     0,
       0,     0,     0,   237,   181,     0,   319,   238,     0,     0,
     239,   183,   240,     0,     0,     0,     0,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,     0,   242,   243,
       0,   505,   506,     0,     0,     0,   244,     0,     0,     0,
       0,   184,     0,   519,   184,   181,   245,   181,     0,     0,
       0,     0,   183,   181,   183,   246,   181,   247,     0,   248,
     183,   249,     0,   183,   250,     0,     0,     0,   251,     0,
     252,     0,     0,   253,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   546,     0,   181,   226,     0,     0,     0,
       0,   181,   183,     0,     0,   181,   181,     0,   183,     0,
       0,     0,   183,   183,     0,     0,     0,   181,     0,   319,
       0,   181,     0,   181,   183,     0,     0,     0,   183,     0,
     183,     0,     0,     0,     0,     0,     0,   363,     0,   980,
       0,     0,   255,   184,   181,   330,     0,     0,   184,     0,
     139,   183,     0,   139,     0,   230,     0,   184,     0,   319,
     184,     0,   436,     0,     0,     0,     0,     0,     0,   231,
     232,     0,   233,     0,     0,   357,     0,   234,   613,     0,
     616,     0,     0,     0,   621,   235,   181,   181,   181,   181,
       0,   236,     0,   183,   183,   183,   183,   237,     0,     0,
       0,   238,     0,     0,   239,     0,   240,     0,   678,     0,
       0,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,   437,   242,   243,   319,   438,     0,     0,     0,     0,
     244,     0,   184,     0,   184,     0,     0,   184,     0,     0,
     245,     0,   139,     0,     0,     0,     0,   139,     0,   246,
       0,   247,     0,   248,   439,   249,   139,     0,   250,   139,
     184,     0,   251,     0,   252,     0,     0,   253,     0,     0,
     184,   440,   441,   742,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   493,   453,   454,   455,
     456,   457,   458,   459,     0,     0,   460,   461,   462,     0,
       0,   519,     0,     0,     0,     0,     0,   463,     0,     0,
       0,   184,     0,     0,   784,     0,     0,     0,     0,     0,
     184,     0,     0,   270,   790,     0,     0,     0,   519,   319,
     319,   139,     0,   139,     0,     0,   139,     0,   230,     0,
     517,     0,     0,     0,     0,   527,     0,     0,     0,     0,
       0,     0,   231,   232,     0,   233,     0,     0,     0,   139,
     234,     0,     0,     0,     0,     0,     0,     0,   235,   139,
       0,     0,   832,   184,   236,     0,     0,     0,     0,     0,
     237,     0,     0,   853,   238,     0,     0,   239,     0,   240,
       0,     0,     0,     0,     0,     0,     0,     0,   241,     0,
       0,     0,     0,   621,     0,   242,   243,     0,     0,     0,
     139,     0,     0,   244,   184,     0,   184,     0,     0,   139,
       0,     0,   184,   245,     0,   184,     0,     0,     0,     0,
       0,     0,   246,     0,   247,     0,   248,     0,   249,   210,
       0,   250,     0,   220,     0,   251,     0,   252,     0,     0,
     253,     0,   228,     0,   184,     0,   599,     0,     0,   261,
     184,     0,     0,     0,   184,   184,     0,     0,     0,   426,
       0,   277,   139,   280,   281,     0,   184,     0,     0,   286,
     184,   287,   184,   288,     0,     0,     0,     0,   314,     0,
       0,     0,   228,   322,   324,   326,   327,     0,     0,     0,
     331,     0,   332,   184,     0,     0,   518,     0,     0,   339,
       0,     0,   322,   139,   341,   139,   342,     0,     0,   343,
     344,   139,   345,     0,   139,     0,   228,   322,   355,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   184,   184,   184,   184,     0,
       0,     0,     0,   139,   954,   379,     0,   382,   519,   139,
       0,     0,     0,   139,   139,     0,     0,     0,   961,     0,
       0,     0,     0,     0,     0,   139,     0,     0,   765,   139,
     767,   139,     0,     0,     0,     0,     0,   982,   883,     0,
       0,     0,  -313,     0,     0,     0,     0,     0,     0,     0,
     433,     0,   139,     0,     0,     0,     0,     0,     0,  -313,
       0,     0,     0,  -313,  -313,   275,     0,     0,     0,     0,
       0,     0,  -313,   485,  -313,  -313,     0,   331,   228,     0,
    -313,     0,     0,   426,     0,     0,     0,  -313,     0,   275,
    -313,     0,     0,     0,   139,   139,   139,   139,   336,   338,
       0,     0,     0,     0,     0,   322,     0,     0,     0,  -313,
       0,  -313,   613,     0,  -313,     0,  -313,     0,  -313,     0,
    -313,  -313,     0,  -313,     0,     0,  -313,     0,  -313,   368,
       0,     0,     0,     0,     0,     0,   678,     0,     0,     0,
       0,   322,     0,     0,     0,     0,  -313,     0,  -313,     0,
       7,  -313,     0,     0,     0,     0,     0,   396,     0,     0,
       0,     0,     0,     0,     0,     0,   322,   358,     0,     0,
       0,    22,    23,     0,     0,     0,     0,   331,     0,     0,
     359,     0,    29,   360,     0,     0,     0,     0,    32,     0,
       0,     0,   549,   550,     0,    37,     0,     0,     0,     0,
       0,  -313,     0,     0,     0,   322,     0,  -313,     0,   559,
       0,     0,     0,   982,     0,     0,     0,   361,     0,     0,
       0,     0,    54,     0,    56,     0,   356,   568,  1052,     0,
       0,  1053,     0,     0,   362,     0,    65,   574,     0,     0,
       0,   495,     0,     0,     0,     0,     0,     0,     0,   586,
       0,   588,     0,     0,    80,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,   602,   609,   612,     0,     0,
       0,     0,     0,     0,   228,     0,     0,     0,     0,     0,
     508,   511,     0,     0,     0,     0,     0,   948,     0,     0,
     951,     0,   621,     0,     0,     0,     0,   275,     0,   275,
       0,     0,     0,     0,     0,     0,   275,     0,     0,    98,
     690,   691,   692,   694,   696,   697,     0,   698,   699,   700,
     701,   702,     0,   710,   711,   712,   713,   714,   715,   716,
     717,   718,   719,   720,   721,   722,   723,   724,   725,   726,
       0,     0,     0,     0,     0,     0,   275,     0,     0,     0,
       0,     0,   322,     0,     0,   322,   322,   322,   322,   734,
     736,     0,     0,     0,   609,   741,     0,     0,   756,     0,
       0,     0,     0,     0,   759,     0,     0,     0,     0,  1025,
       0,   621,     0,     0,  1031,   768,     0,     0,   770,     0,
       0,     0,   322,   527,     0,     0,   527,     0,     0,   778,
       0,   781,     0,     0,     0,   280,     0,     0,   786,   787,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     314,     0,     0,   314,     0,     0,     0,     0,     0,     0,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   759,     0,     0,   322,   322,     0,     0,     0,
       0,   228,   815,     0,     0,   322,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   845,   845,  1084,     0,
    1085,     0,     0,  1087,     0,     0,     0,   230,     0,     0,
     857,     0,     0,   228,     0,     0,     0,     0,     0,     0,
       0,   231,   232,     0,   233,     0,  1101,     0,     0,   234,
       0,     0,     0,     0,     0,     0,  1031,   235,   314,     0,
       0,     0,     0,   236,     0,     0,     0,     0,     0,   237,
       0,     0,     0,   238,     0,     0,   239,     0,   240,     0,
       0,     0,     0,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,   242,   243,     0,  1128,     0,     0,
       0,     0,   244,   901,     0,     0,  1134,     0,     0,     0,
     906,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,   246,     0,   247,     0,   248,     0,   249,     0,   275,
     250,     0,     0,     0,   251,     0,   252,     0,     0,   253,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1170,
       0,     0,     0,   768,   322,     0,     0,     0,     0,     0,
       0,   275,   275,     0,     0,     0,     0,     0,     0,     0,
       0,   275,   275,     0,     0,     0,   322,     0,   322,   945,
     946,     0,     0,     0,     0,   545,     0,     0,     0,     0,
    1189,     0,  1193,     0,     0,   322,     0,     0,   527,     0,
       0,   527,     0,     0,     0,     0,     0,     0,     0,     0,
     314,     0,     0,     0,   973,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1228,     0,     0,     0,     0,     0,  1231,     0,     0,   586,
    1189,  1193,     0,  1010,     0,  1011,   845,   845,     0,   341,
       0,   342,  1237,     0,   344,   345,  1239,     0,   767,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1022,     0,
       0,     0,   228,     0,     0,     0,  1028,   759,     0,  1032,
       0,     0,     0,     0,     0,  1038,  1040,     0,  1041,     0,
       0,  1044,  1046,     0,     0,     0,     0,     0,   322,     0,
       0,     0,     0,  1058,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   767,   767,   767,   767,     0,     0,     0,     0,     0,
     953,     0,     0,  1075,  1032,  1041,  1044,  1078,   322,     0,
       0,     0,   756,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   694,   734,     0,     0,
       0,     0,     0,     0,   981,     0,  1086,     0,     0,     0,
       0,     0,     0,     0,   275,   275,     0,     7,     0,     0,
       0,     0,   314,     0,     0,     0,  1100,     0,     0,     0,
       0,     0,   228,     0,   358,     0,     0,     0,    22,    23,
    1108,     0,  1110,   322,     0,     0,     0,   359,     0,    29,
     360,     0,     0,     0,     0,    32,     0,     0,     0,     0,
       0,     0,    37,   845,     0,     0,     0,     0,     0,     0,
    1119,     0,  1120,     0,   275,     0,     0,     0,   275,  1126,
    1049,     0,     0,     0,   361,     0,     0,     0,     0,    54,
       0,    56,   759,    58,     0,  1052,     0,     0,  1053,  1136,
       0,   362,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1157,    80,     0,    82,     0,     0,    84,     0,     0,     0,
       7,     0,  1164,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   358,   322,   322,
       0,    22,    23,     0,     0,     0,     0,     0,     0,     0,
     359,  1178,    29,   360,     0,     0,     0,     0,    32,  1182,
     845,   845,     0,     0,     0,    37,    98,     0,     0,     0,
       0,     0,  1054,  1106,     0,  1190,     0,  1194,     0,     0,
       0,  1196,     0,  1199,     0,     0,  1202,   361,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,  1052,     0,
       0,  1053,     0,     0,   362,     0,    65,     0,  1190,  1194,
       0,  1199,  1202,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    82,     0,     0,    84,
       0,  1235,     0,     0,  1236,   275,   275,     0,  1238,     0,
       0,     0,     0,  1240,  1241,     0,     0,  1242,     0,     0,
       0,     0,     0,     0,     0,  1244,     0,   434,     0,     0,
       0,     0,     0,     0,   435,     0,     0,     0,     0,  1254,
    1255,  1240,  1256,  1257,     0,     0,   436,     0,     0,    98,
       0,     0,     0,     0,     0,  1122,     0,     0,     0,     0,
       0,     0,     0,   275,     0,     0,     0,     0,     0,   275,
     275,     0,     0,     0,     0,     0,  1268,  1269,  1270,  1271,
    1268,  1269,  1270,  1271,     0,     0,   275,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   511,     0,     0,   275,
       0,     0,   275,     0,     0,   437,     0,     0,     0,   438,
       0,     0,     0,     0,     0,     0,     0,   509,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1232,   439,   510,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,     0,   477,   478,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,     0,     0,
     460,   461,   462,     0,   363,     0,     0,     0,     0,     0,
       0,   463,     0,     0,     0,     0,   511,     0,   511,     0,
       0,   511,   511,    -2,     4,     0,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,  -207,    21,    22,
      23,    24,    25,    26,     0,  -207,    27,     0,    28,     0,
      29,    30,    31,  -207,  -207,  -207,    32,    33,    34,   -68,
       0,    35,    36,    37,     0,    38,  -333,    39,     0,    40,
      41,    42,    43,    44,     0,    45,    46,    47,    48,   -68,
      49,    50,     0,    51,    52,    53,     0,  -333,     0,     0,
      54,    55,    56,    57,    58,    59,    60,  -333,   -68,    61,
      62,    63,    64,     0,    65,    66,    67,     0,    68,    69,
      70,    71,    72,    73,    74,     0,    75,    76,     0,    77,
      78,    79,    80,    81,    82,    83,   -68,    84,    85,     0,
       0,  -207,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,    98,     0,     0,
      99,     0,   100,     0,   101,     0,  -207,     0,     4,   102,
       5,     6,     7,     8,     9,    10,    11,    12,     0,  -697,
       0,    13,    14,    15,    16,    17,  -697,    18,     0,    19,
      20,  -207,    21,    22,    23,    24,    25,    26,  -697,  -207,
      27,  -697,    28,     0,    29,    30,    31,  -207,  -207,  -207,
      32,    33,    34,   -68,     0,    35,    36,    37,     0,    38,
    -333,    39,     0,    40,    41,    42,    43,    44,     0,    45,
      46,    47,    48,   -68,    49,    50,     0,    51,    52,    53,
       0,  -333,     0,     0,    54,    55,    56,    57,     0,    59,
      60,  -333,   -68,    61,    62,    63,    64,  -697,    65,    66,
      67,  -697,    68,    69,    70,    71,    72,    73,    74,     0,
      75,    76,     0,    77,    78,    79,    80,    81,    82,    83,
     -68,    84,    85,     0,     0,  -207,     0,    86,     0,     0,
    -697,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,  -697,    90,
    -697,  -697,  -697,  -697,  -697,  -697,  -697,     0,  -697,  -697,
    -697,  -697,  -697,  -697,  -697,  -697,  -697,  -697,  -697,  -697,
      97,    98,  -697,  -697,  -697,     0,   100,  -697,   101,     0,
    -207,     0,   346,  -697,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -207,    21,    22,    23,    24,
      25,    26,     0,  -207,    27,     0,    28,     0,    29,    30,
      31,  -207,  -207,  -207,    32,    33,    34,   -68,     0,    35,
      36,    37,     0,    38,  -333,    39,     0,    40,    41,    42,
      43,    44,     0,    45,    46,    47,    48,   -68,    49,    50,
       0,    51,    52,    53,     0,  -333,     0,     0,    54,    55,
      56,    57,    58,    59,    60,  -333,   -68,    61,    62,    63,
      64,     0,    65,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,     0,    75,    76,     0,    77,    78,    79,
      80,    81,    82,    83,   -68,    84,    85,     0,     0,  -207,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,    98,     0,     0,    99,     0,
     100,   347,   101,     0,  -207,     0,     4,   102,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -207,
      21,    22,    23,    24,    25,    26,     0,  -207,    27,     0,
      28,     0,    29,    30,    31,  -207,  -207,  -207,    32,    33,
      34,   -68,     0,    35,    36,    37,     0,    38,  -333,    39,
       0,    40,    41,    42,    43,    44,     0,    45,    46,    47,
      48,   -68,    49,    50,     0,    51,    52,    53,     0,  -333,
       0,     0,    54,    55,    56,    57,    58,    59,    60,  -333,
     -68,    61,    62,    63,    64,     0,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,     0,    75,    76,
       0,    77,    78,    79,    80,    81,    82,    83,   -68,    84,
      85,     0,     0,  -207,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,    98,
       0,     0,    99,     0,   100,   564,   101,     0,  -207,     0,
     581,   102,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -207,    21,    22,    23,    24,    25,    26,
       0,  -207,    27,     0,    28,     0,    29,    30,    31,  -207,
    -207,  -207,    32,    33,    34,   -68,     0,    35,    36,    37,
       0,    38,  -333,    39,     0,    40,    41,    42,    43,    44,
       0,    45,    46,    47,    48,   -68,    49,    50,     0,    51,
      52,    53,     0,  -333,     0,     0,    54,    55,    56,    57,
      58,    59,    60,  -333,   -68,    61,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
      82,    83,   -68,    84,    85,     0,     0,  -207,     0,    86,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,    98,     0,     0,    99,     0,   100,   582,
     101,     0,  -207,     0,     4,   102,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,  -207,    21,    22,
      23,    24,    25,    26,     0,  -207,    27,     0,    28,     0,
      29,    30,    31,  -207,  -207,  -207,    32,    33,    34,   -68,
       0,    35,    36,    37,     0,    38,  -333,    39,     0,    40,
      41,    42,    43,    44,     0,    45,    46,    47,    48,   -68,
      49,    50,     0,    51,    52,    53,     0,  -333,     0,     0,
      54,    55,    56,    57,    58,    59,    60,  -333,   -68,    61,
      62,    63,    64,     0,    65,    66,    67,     0,    68,    69,
      70,    71,    72,    73,    74,     0,    75,    76,     0,    77,
      78,    79,    80,    81,    82,    83,   -68,    84,    85,     0,
       0,  -207,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,    98,     0,     0,
      99,     0,   100,   822,   101,     0,  -207,     0,     4,   102,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,  -207,    21,    22,    23,    24,    25,    26,     0,  -207,
      27,     0,    28,     0,    29,    30,    31,  -207,  -207,  -207,
      32,    33,    34,   -68,     0,    35,    36,    37,     0,    38,
    -333,    39,     0,    40,    41,    42,    43,    44,     0,    45,
      46,    47,    48,   -68,    49,    50,     0,    51,    52,    53,
       0,  -333,     0,     0,    54,    55,    56,    57,   356,    59,
      60,  -333,   -68,    61,    62,    63,    64,     0,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,     0,
      75,    76,     0,    77,    78,    79,    80,    81,    82,    83,
     -68,    84,    85,     0,     0,  -207,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,     0,   100,     0,   101,     0,
    -207,     0,     4,   102,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -207,    21,    22,    23,    24,
      25,    26,     0,  -207,    27,     0,    28,     0,    29,    30,
      31,  -207,  -207,  -207,    32,    33,    34,   -68,     0,    35,
      36,    37,     0,    38,  -333,    39,     0,    40,    41,    42,
      43,    44,     0,    45,    46,    47,    48,   -68,    49,    50,
       0,    51,    52,    53,     0,  -333,     0,     0,    54,    55,
      56,    57,    58,    59,    60,  -333,   -68,    61,    62,    63,
      64,     0,    65,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,     0,    75,    76,     0,    77,    78,    79,
      80,    81,    82,    83,   -68,    84,    85,     0,     0,  -207,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,    98,     0,     0,    99,     0,
     100,     0,   101,     0,  -207,     0,     4,   102,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -207,
      21,    22,    23,    24,    25,    26,     0,  -207,    27,     0,
      28,     0,    29,    30,    31,  -207,  -207,  -207,    32,    33,
      34,   -68,     0,    35,    36,    37,     0,    38,  -333,    39,
       0,    40,    41,    42,    43,    44,     0,    45,    46,    47,
      48,   -68,    49,    50,     0,    51,    52,    53,     0,  -333,
       0,     0,    54,    55,    56,    57,     0,    59,    60,  -333,
     -68,    61,    62,    63,    64,     0,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,     0,    75,    76,
       0,    77,    78,    79,    80,    81,    82,    83,   -68,    84,
      85,     0,     0,  -207,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,    98,
       0,     0,    99,     0,   100,     0,   101,     0,  -207,     0,
       4,   102,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -207,    21,    22,    23,    24,    25,    26,
       0,  -207,    27,     0,    28,     0,    29,    30,    31,  -207,
    -207,  -207,    32,  1043,    34,   -68,     0,    35,    36,    37,
       0,    38,  -333,    39,     0,    40,    41,    42,    43,    44,
       0,    45,    46,    47,    48,   -68,    49,    50,     0,    51,
      52,    53,     0,  -333,     0,     0,    54,    55,    56,    57,
       0,    59,    60,  -333,   -68,    61,    62,    63,    64,     0,
      65,    66,    67,     0,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
      82,    83,   -68,    84,    85,     0,     0,  -207,     0,    86,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,    98,     0,     0,    99,     0,   100,     0,
     101,     0,  -207,     0,     4,   102,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,  -207,    21,    22,
      23,    24,    25,    26,     0,  -207,    27,     0,    28,     0,
      29,    30,    31,  -207,  -207,  -207,    32,  1188,    34,   -68,
       0,    35,    36,    37,     0,    38,  -333,    39,     0,    40,
      41,    42,    43,    44,     0,    45,    46,    47,    48,   -68,
      49,    50,     0,    51,    52,    53,     0,  -333,     0,     0,
      54,    55,    56,    57,     0,    59,    60,  -333,   -68,    61,
      62,    63,    64,     0,    65,    66,    67,     0,    68,    69,
      70,    71,    72,    73,    74,     0,    75,    76,     0,    77,
      78,    79,    80,    81,    82,    83,   -68,    84,    85,     0,
       0,  -207,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,    98,     0,     0,
      99,     0,   100,     0,   101,     0,  -207,     0,     4,   102,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,  -207,    21,    22,    23,    24,    25,    26,     0,  -207,
      27,     0,    28,     0,    29,    30,    31,  -207,  -207,  -207,
      32,  1192,    34,   -68,     0,    35,    36,    37,     0,    38,
    -333,    39,     0,    40,    41,    42,    43,    44,     0,    45,
      46,    47,    48,   -68,    49,    50,     0,    51,    52,    53,
       0,  -333,     0,     0,    54,    55,    56,    57,     0,    59,
      60,  -333,   -68,    61,    62,    63,    64,     0,    65,    66,
      67,     0,    68,    69,    70,    71,    72,    73,    74,     0,
      75,    76,     0,    77,    78,    79,    80,    81,    82,    83,
     -68,    84,    85,     0,     0,  -207,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,     0,   100,     0,   101,     0,
    -207,     0,     4,   102,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -207,    21,    22,    23,    24,
      25,    26,     0,  -207,    27,     0,    28,     0,    29,    30,
      31,  -207,  -207,  -207,    32,  1198,    34,   -68,     0,    35,
      36,    37,     0,    38,  -333,    39,     0,    40,    41,    42,
      43,    44,     0,    45,    46,    47,    48,   -68,    49,    50,
       0,    51,    52,    53,     0,  -333,     0,     0,    54,    55,
      56,    57,     0,    59,    60,  -333,   -68,    61,    62,    63,
      64,     0,    65,    66,    67,     0,    68,    69,    70,    71,
      72,    73,    74,     0,    75,    76,     0,    77,    78,    79,
      80,    81,    82,    83,   -68,    84,    85,     0,     0,  -207,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,    98,     0,     0,    99,     0,
     100,     0,   101,     0,  -207,     0,     4,   102,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -207,
      21,    22,    23,    24,    25,    26,     0,  -207,    27,     0,
      28,     0,    29,    30,    31,  -207,  -207,  -207,    32,  1201,
      34,   -68,     0,    35,    36,    37,     0,    38,  -333,    39,
       0,    40,    41,    42,    43,    44,     0,    45,    46,    47,
      48,   -68,    49,    50,     0,    51,    52,    53,     0,  -333,
       0,     0,    54,    55,    56,    57,     0,    59,    60,  -333,
     -68,    61,    62,    63,    64,     0,    65,    66,    67,     0,
      68,    69,    70,    71,    72,    73,    74,     0,    75,    76,
       0,    77,    78,    79,    80,    81,    82,    83,   -68,    84,
      85,     0,     0,  -207,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,    98,
       0,     0,    99,     0,   100,     0,   101,   743,  -207,     5,
       6,   102,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
    -207,    21,     0,   744,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,  -207,  -207,  -207,     0,
     197,    34,     0,   745,     0,    36,     0,   746,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,   747,     0,    55,   748,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,   749,     0,     0,
       0,     0,     0,    70,    71,    72,    73,   201,     0,    75,
       0,     0,    77,     0,     0,   750,    81,     0,    83,     0,
     751,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   202,     0,   101,   752,  -207,
       0,   893,   102,     5,     6,     0,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,  -207,    21,     0,     0,     0,     0,
       0,     0,     0,   294,     0,   196,     0,     0,     0,    31,
    -207,  -207,  -207,     0,   197,    34,     0,     0,     0,   296,
       0,     0,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   200,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,   301,     0,    75,     0,     0,    77,     0,     0,     0,
      81,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   202,
       0,   101,     0,  -207,     5,     6,   102,     8,     9,    10,
      11,    12,     0,   639,     0,    13,     0,     0,    16,    17,
     641,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,   647,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,     0,     0,     0,     0,     0,     0,     0,    70,    71,
      72,    73,   201,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,   510,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475,     0,   477,   478,
       0,   833,   834,   835,   836,   837,   660,     0,   661,     0,
      93,     0,   662,   663,   664,   665,   666,   667,   668,   838,
     670,   671,    95,   839,    97,   -90,   673,   674,   840,   676,
     202,     0,   101,     0,     0,     5,     6,   102,     8,     9,
      10,    11,    12,     0,   639,     0,    13,     0,     0,    16,
      17,   641,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,   647,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,     0,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,   201,     0,    75,     0,     0,    77,     0,
       0,     0,    81,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,   510,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   474,   475,     0,   477,
     478,     0,   833,   834,   835,   836,   837,   660,     0,   661,
       0,    93,     0,   662,   663,   664,   665,   666,   667,   668,
     838,   670,   671,    95,   839,    97,     0,   673,   674,   840,
     676,   202,     0,   101,     0,     0,     0,     0,   102,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,     0,    16,    17,     0,    18,     0,   195,    20,
       0,    21,     0,     0,     0,     0,    26,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
      33,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,    50,     0,    51,    52,   333,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,    73,    74,     0,    75,
      76,     0,    77,     0,     0,     0,    81,     0,    83,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   100,     0,   101,     0,     0,
       5,     6,   102,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,     0,    21,     0,   744,     0,     0,     0,     0,     0,
      27,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,   197,    34,     0,   745,     0,    36,     0,   746,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,   747,     0,    55,   748,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,   749,     0,
       0,     0,     0,     0,    70,    71,    72,    73,   201,     0,
      75,     0,     0,    77,     0,     0,   750,    81,     0,    83,
       0,   751,    85,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   202,     0,   101,     0,
       0,     5,     6,   102,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,     0,    21,   377,    23,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
     361,     0,     0,     0,     0,     0,    55,    56,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,    65,
       0,     0,     0,     0,     0,    70,    71,    72,    73,   201,
       0,    75,     0,     0,    77,     0,     0,     0,    81,     0,
      83,     0,    84,    85,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   202,     0,   101,
       0,     0,     5,     6,   102,     8,     9,    10,   380,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,    19,     0,  -207,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   196,     0,     0,     0,    31,  -207,
    -207,  -207,     0,   197,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,    64,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
      82,    83,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   202,     0,
     101,     0,  -207,     5,     6,   102,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   195,     0,  -207,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   196,     0,     0,     0,    31,
    -207,  -207,  -207,     0,   197,    34,     0,     0,     0,    36,
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
       0,   101,     0,  -207,     5,     6,   102,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   195,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   196,     0,     0,     0,
      31,     0,     0,     0,     0,   197,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   198,     0,   199,     0,     0,
      43,    44,     0,     0,    46,     0,    48,     0,    49,    50,
       0,    51,    52,     0,     0,     0,     0,     0,     0,    55,
       0,    57,     0,    59,     0,     0,     0,     0,    62,    63,
     200,  -745,     0,     0,     0,  -745,     0,     0,    70,    71,
      72,    73,   201,     0,    75,     0,     0,    77,     0,     0,
       0,    81,     0,    83,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,  -375,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,  -375,     0,     0,    99,     0,
     202,  -375,   101,     0,     0,     5,     6,   102,     8,     9,
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
      94,     0,     0,    95,    96,    97,   318,     0,     0,    99,
       0,   202,     0,   101,     0,     0,     5,     6,   102,     8,
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
       0,     0,    93,   351,     0,     0,     0,     0,     0,     0,
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
       0,    99,     0,   202,     0,   101,  -545,     0,     5,     6,
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
       0,     0,    99,     0,   202,     0,   101,     0,     0,  -545,
       0,   102,     5,     6,     0,     8,     9,    10,    11,    12,
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
     101,   808,     0,     5,     6,   102,     8,     9,    10,    11,
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
       0,   101,   811,     0,     5,     6,   102,     8,     9,    10,
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
     202,     0,   101,   817,     0,     5,     6,   102,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   196,     0,     0,
       0,    31,     0,     0,     0,     0,   197,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   198,     0,   199,     0,
       0,    43,    44,     0,     0,    46,     0,    48,     0,    49,
      50,   959,    51,    52,     0,     0,     0,     0,     0,     0,
      55,     0,    57,     0,    59,     0,     0,     0,     0,    62,
      63,   200,     0,     0,     0,     0,     0,     0,     0,    70,
      71,    72,    73,   201,     0,    75,     0,     0,    77,   316,
     317,     0,    81,     0,    83,     0,     0,    85,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   202,     0,   101,     0,     0,     5,     6,   102,     8,
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
       0,     0,     0,    88,    89,    90,    91,    92,     0,  -383,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,  -383,     0,     0,
      99,     0,   202,  -383,   101,     0,     0,     5,     6,   102,
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
    -368,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,  -368,     0,
       0,    99,     0,   202,  -368,   101,     0,     0,     5,     6,
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
       0,  -376,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,  -376,
       0,     0,    99,     0,   202,  -376,   101,     0,     0,     5,
       6,   102,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   195,     0,
       0,    21,     0,   258,     0,     0,     0,     0,     0,    27,
       0,   196,     0,     0,     0,    31,     0,     0,     0,     0,
     197,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     198,     0,   199,     0,     0,    43,    44,     0,     0,    46,
       0,    48,     0,    49,    50,     0,    51,    52,     0,     0,
       0,     0,     0,     0,    55,     0,    57,     0,    59,     0,
       0,     0,     0,    62,    63,   200,     0,     0,     0,     0,
       0,     0,     0,    70,    71,    72,    73,   201,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,    83,     0,
     259,    85,     0,     0,     0,     0,     0,     0,     0,    87,
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
      75,     0,     0,    77,   316,   317,     0,    81,     0,    83,
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
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   202,     0,   101,
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
     603,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   202,     0,
     101,     0,     0,   604,     0,   102,     5,     6,     0,     8,
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
       0,    94,     0,     0,    95,    96,    97,   862,     0,     0,
      99,     0,   202,     0,   101,     0,     0,  1029,     0,   102,
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
      91,    92,     0,  -406,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   202,     0,   101,  -406,
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
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,  -539,     0,     0,    99,     0,   202,     0,   101,
       0,     0,  -539,     0,   102,     5,     6,     0,     8,     9,
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
     101,     0,     0,  1039,     0,   102,     5,     6,     0,     8,
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
      99,     0,   202,     0,   101,     0,     0,     5,     6,   102,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   294,     0,   196,
       0,     0,     0,    31,     0,     0,     0,     0,   197,    34,
       0,     0,     0,   296,     0,     0,    38,     0,   198,     0,
     199,     0,     0,    43,    44,     0,     0,    46,     0,    48,
       0,    49,    50,     0,    51,    52,     0,     0,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,     0,     0,
       0,    62,    63,   200,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    73,   301,     0,    75,     0,     0,
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
     196,     0,     0,     0,    31,     0,     0,     0,     0,  1076,
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
    1223,    34,     0,     0,     0,    36,     0,     0,    38,     0,
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
      27,     0,   196,     0,     0,     0,    31,     0,     0,     0,
       0,  1224,    34,     0,     0,     0,    36,     0,     0,    38,
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
       0,    27,     0,   196,     0,     0,     0,    31,     0,     0,
       0,     0,  1226,    34,     0,     0,     0,    36,     0,     0,
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
       0,     0,     0,  1227,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   198,     0,   199,     0,     0,    43,    44,
       0,     0,    46,     0,    48,     0,    49,    50,     0,    51,
      52,     0,     0,     0,     0,     0,     0,    55,     0,    57,
       0,    59,     0,     0,     0,     0,    62,    63,   200,     0,
       0,     0,     0,     0,     0,     0,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,    83,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     6,
       0,     8,     9,    10,    11,    12,     0,    94,     0,    13,
      95,    96,    97,    17,     0,     0,    99,   195,   202,  -207,
     101,     0,   293,     0,     0,   102,     0,     0,     0,     0,
     196,     0,     0,     0,     0,  -207,  -207,  -207,     0,   197,
       0,     0,   295,     0,     0,     0,   297,     0,     0,     0,
       0,   199,     0,     0,    43,    44,     0,     0,     0,     0,
      48,     0,    49,    50,     0,     0,     0,     0,     0,     0,
       0,   298,     0,    55,   299,    57,    58,    59,     0,     0,
       0,     0,    62,     0,   200,     0,   300,     0,     0,     0,
       0,     0,    70,    71,     0,    73,     0,     0,     0,     0,
       0,     0,     0,     0,   302,     0,     0,    83,     0,   303,
       0,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   202,     0,   101,  -337,  -207,     0,
       6,   102,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,     0,    17,     0,     0,     0,   195,     0,
    -207,     0,     0,   293,     0,     0,     0,     0,     0,     0,
       0,   196,     0,     0,     0,     0,  -207,  -207,  -207,     0,
     197,     0,     0,   295,     0,     0,     0,   297,     0,     0,
       0,     0,   199,     0,     0,    43,    44,     0,     0,     0,
       0,    48,     0,    49,    50,     0,     0,     0,     0,     0,
       0,     0,   298,     0,    55,   299,    57,    58,    59,     0,
       0,     0,     0,    62,     0,   200,     0,   300,     0,     0,
       0,     0,     0,    70,    71,     0,    73,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,     0,    83,     0,
     303,     0,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   637,     0,   638,     0,    88,    89,    90,    91,
      92,     0,     0,   639,     0,    93,     0,     0,   640,   232,
     641,   642,     0,     0,    94,     0,   643,    95,    96,    97,
       0,     0,     0,    99,   235,   202,   196,   101,     0,  -207,
     236,     0,   102,     0,     0,     0,   644,     0,     0,     0,
     238,     0,     0,   645,     0,   646,   647,     0,     0,     0,
       0,     0,     0,     0,   648,     0,     0,     0,     0,     0,
       0,   242,   649,     0,     0,     0,     0,     0,     0,   244,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   650,
       0,     0,     0,     0,     0,     0,     0,     0,   246,     0,
     651,     0,   248,     0,   652,     0,     0,   250,     0,     0,
       0,   653,     0,   252,     0,     0,   654,     0,     0,     0,
       0,     0,     0,     0,     0,   510,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475,     0,   477,   478,
       0,   655,   656,   657,   658,   659,   660,     0,   661,     0,
       0,     0,   662,   663,   664,   665,   666,   667,   668,   669,
     670,   671,     0,   672,     0,     0,   673,   674,   675,   676,
       5,     6,   677,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   195,
       0,  -207,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   196,     0,     0,     0,    31,  -207,  -207,  -207,
       0,   197,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   198,     0,   199,     0,     0,    43,    44,     0,     0,
      46,     0,    48,     0,    49,    50,     0,    51,    52,     0,
       0,     0,     0,     0,     0,    55,     0,    57,     0,    59,
       0,     0,     0,     0,    62,    63,   200,     0,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    73,   201,     0,
      75,     0,     0,    77,     0,     0,     0,    81,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,     0,    96,
      97,     0,     0,     0,    99,     0,   202,     0,   101,     0,
    -207,     5,     6,     0,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     195,     0,  -207,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   196,     0,     0,     0,    31,  -207,  -207,
    -207,     0,   197,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   198,     0,   199,     0,     0,    43,    44,     0,
       0,    46,     0,    48,     0,    49,    50,     0,    51,    52,
       0,     0,     0,     0,     0,     0,    55,     0,    57,     0,
      59,     0,     0,     0,     0,    62,    63,   200,     0,     0,
       0,     0,     0,     0,     0,    70,    71,    72,    73,   201,
       0,    75,     0,     0,    77,     0,     0,     0,    81,     0,
      83,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
      90,     0,     0,     5,     6,     0,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,    97,   195,     0,  -207,    21,     0,   202,     0,   101,
       0,  -207,     0,    27,     0,   196,     0,     0,     0,    31,
    -207,  -207,  -207,     0,   197,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   198,     0,   199,     0,     0,    43,
      44,     0,     0,    46,     0,    48,     0,    49,    50,     0,
      51,    52,     0,     0,     0,     0,     0,     0,    55,     0,
      57,     0,    59,     0,     0,     0,     0,    62,    63,   200,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      73,   201,   703,    75,     0,     0,    77,     0,     0,     0,
      81,   639,    83,     0,     0,    85,   991,   992,   641,   993,
       0,     0,     0,     0,   994,     0,     0,     0,     0,     0,
       0,     0,   705,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    90,     0,   995,     0,     0,     0,   996,     0,
       0,   997,     0,   198,   647,     0,     0,     0,     0,     0,
       0,     0,   706,    97,     0,     0,     0,     0,     0,     0,
     998,   101,     0,  -207,     0,     0,     0,   999,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1000,     0,     0,
       0,     0,     0,     0,     0,     0,  1001,     0,  1002,     0,
    1003,     0,   707,     0,     0,     0,     0,     0,   708,  1004,
       0,  1005,     0,     0,  1006,     0,     0,     0,     0,     0,
       0,     0,     0,   510,   465,   466,   467,   468,   469,   470,
     471,   472,   473,   474,   475,     0,   477,   478,     0,   655,
     656,   657,   658,   659,   660,     0,   661,     0,     0,     0,
     662,   663,   664,   665,   666,   667,   668,   669,   670,   671,
     230,   672,     0,     0,   673,   674,   675,   676,  1007,   639,
       0,     0,     0,     0,   231,   232,   641,   233,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
     235,     0,     0,     0,     0,     0,   236,     0,     0,     0,
       0,     0,   237,     0,     0,     0,   238,     0,     0,   239,
       0,   240,   647,     0,     0,     0,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,   242,   243,     0,
       0,     0,     0,     0,     0,   244,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   245,     0,     0,     0,     0,
       0,     0,     0,     0,   246,     0,   247,     0,   248,     0,
     249,     0,     0,   250,     0,     0,     0,   251,     0,   252,
       0,     0,   253,     0,     0,     0,     0,     0,     0,     0,
       0,   510,   465,   466,   467,   468,   469,   470,   471,   472,
     473,   474,   475,     0,   477,   478,     0,   655,   656,   657,
     658,   659,   660,     0,   661,     0,     0,     0,   662,   663,
     664,   665,   666,   667,   668,   669,   670,   671,     7,   672,
       0,     0,   673,   674,   675,   676,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   358,     0,     0,     0,    22,
      23,     0,     0,     0,     0,     0,     0,     0,   359,     0,
      29,   360,     0,   434,     0,     0,    32,     0,     0,     0,
     435,     0,     0,    37,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   361,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,  1052,     0,     0,  1053,
       0,     0,   362,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    80,     0,    82,     0,     0,    84,     0,     0,
       0,   437,     0,   434,     0,   438,     0,     0,     0,     0,
     435,     0,     0,  1197,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,   510,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475,    98,   477,   478,
       0,   440,   441,  1186,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,     0,   460,   461,   462,     0,
     363,   437,     0,   434,     0,   438,     0,   463,     0,     0,
     435,     0,     0,  1260,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,   510,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475,     0,   477,   478,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,     0,   460,   461,   462,     0,
     363,   437,     0,   434,     0,   438,     0,   463,     0,     0,
     435,     0,     0,  1261,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,   510,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475,     0,   477,   478,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,     0,   460,   461,   462,     0,
     363,   437,     0,   434,     0,   438,     0,   463,     0,     0,
     435,     0,     0,  1262,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,   510,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475,     0,   477,   478,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,     0,   460,   461,   462,     0,
     363,   437,     0,     0,     0,   438,     0,   463,     0,     0,
       0,     0,   434,  1263,     0,     0,     0,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   875,     0,   439,   510,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475,   876,   477,   478,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,     0,   460,   461,   462,     0,
     363,     0,     0,     0,     0,     0,   434,   463,     0,     0,
     437,     0,     0,   435,   438,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,   274,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   214,
       0,   986,     0,   439,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,     0,   437,   460,   461,   462,   438,   363,
       0,     0,     0,     0,     0,     0,   463,   434,     0,     0,
       0,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,   214,     0,     0,   436,   439,     0,     0,
       0,   590,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,   591,   460,
     461,   462,     0,   363,     0,     0,     0,     0,     0,     0,
     463,     0,     0,     0,     0,   437,     0,     0,     0,   438,
       0,     0,     0,     0,     0,     0,     0,   434,   829,     0,
       0,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,     0,   439,     0,
       0,   830,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   831,   592,
     460,   461,   462,     0,     0,     0,     0,     0,     0,     0,
       0,   463,     0,     0,     0,   437,   434,     0,     0,   438,
       0,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,  1140,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   439,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,     0,     0,
     460,   461,   462,     0,   437,     0,   434,     0,   438,     0,
       0,   463,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,   274,     0,     0,     0,
       0,     0,     0,   214,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   363,   437,     0,   434,     0,   438,     0,
     463,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,   274,     0,     0,     0,
       0,     0,     0,   214,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   363,   437,     0,   434,     0,   438,     0,
     463,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,   920,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,     0,     0,
       0,   921,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   363,   437,   434,     0,     0,   438,     0,
     463,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,   437,     0,     0,     0,   438,     0,     0,
     463,     0,     0,     0,     0,   952,   434,     0,     0,     0,
       0,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,   439,     0,     0,     0,
    1114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,  1115,   460,   461,
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
       0,     0,  1131,   437,   463,     0,     0,   438,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   439,     0,   460,   461,
     462,     0,     0,     0,   434,     0,     0,  1180,     0,   463,
       0,   435,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   436,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,     0,   460,   461,
     462,     0,     0,     0,   434,     0,     0,     0,     0,   463,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,     0,   434,     0,   438,     0,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   439,     0,     0,     0,     0,
       0,     0,   437,     0,     0,     0,   438,     0,     0,     0,
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
     456,   457,   458,   459,   869,   439,   460,   461,   462,     0,
       0,     0,     0,     0,     0,     0,     0,   463,     0,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   434,     0,   460,   461,   462,
       0,     0,   435,     0,     0,     0,     0,     0,   463,     0,
       0,     0,     0,     0,   436,     0,     0,   950,     0,     0,
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
     462,     0,     0,   437,   434,  1016,     0,   438,     0,   463,
       0,   435,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   436,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   439,     0,   460,   461,
     462,     0,     0,     0,   434,   962,     0,     0,     0,   463,
       0,   435,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   436,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,     0,   460,   461,
     462,     0,   437,     0,     0,   963,   438,     0,     0,   463,
       0,     0,   434,  1113,     0,     0,     0,     0,     0,   435,
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
       0,     0,   435,   439,  1068,     0,     0,     0,   463,     0,
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
     454,   455,   456,   457,   458,   459,     0,  1125,   460,   461,
     462,     0,     0,  1135,   439,     0,     0,     0,     0,   463,
       0,   437,     0,     0,     0,   438,     0,     0,     0,     0,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   439,     0,   460,   461,   462,     0,
       0,     0,     0,     0,     0,     0,     0,   463,     0,     0,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   434,     0,   460,   461,   462,     0,
       0,   435,     0,     0,   619,  1163,   230,   463,     7,     0,
       0,     0,     0,   436,  1166,     0,     0,     0,     0,     0,
     231,   232,     0,   233,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,   434,  1185,   235,     0,     0,     0,
       0,   435,   236,     0,     0,     0,     0,     0,   237,     0,
       0,     0,   238,   436,     0,   239,     0,   240,     0,     0,
       0,     0,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,   437,   242,   243,   434,   438,     0,     0,     0,
       0,   244,   435,     0,     0,     0,     0,     0,     0,     0,
       0,   245,     0,     0,   436,     0,     0,     0,     0,     0,
     246,     0,   247,     0,   248,   439,   249,     0,     0,   250,
       0,     0,   437,   251,     0,   252,   438,     0,   253,     0,
       0,     0,   440,   441,  1217,   442,   443,   444,     0,   445,
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
    1225,     0,     0,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   437,     0,   440,   441,
     438,   442,   443,   444,     0,   445,   446,   447,  1264,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,     0,     0,   460,   461,   462,     0,     0,     0,   439,
       0,     0,     0,     0,   463,     0,   437,   434,     0,     0,
     438,     0,     0,     0,   435,     0,   440,   441,  1265,   442,
     443,   444,     0,   445,   446,   447,   436,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   439,
       0,   460,   461,   462,     0,     0,     0,   434,     0,     0,
       0,     0,   463,     0,   435,     0,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   436,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,     0,
       0,   460,   461,   462,     0,   437,     0,   434,     0,   438,
       0,     0,   463,     0,   435,     0,     0,  1266,     0,     0,
       0,     0,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   439,     0,
       0,     0,     0,     0,     0,   437,     0,     0,     0,   438,
       0,     0,     0,     0,     0,   440,   441,  1267,   442,   443,
     444,     0,   445,   446,   447,   436,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   439,     0,
     460,   461,   462,     0,     0,   437,   434,     0,     0,   438,
       0,   463,     0,   435,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   436,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   439,     0,
     460,   461,   462,     0,   437,     0,     0,     0,   438,     0,
       0,   463,     0,     0,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,     0,     0,
     460,   461,   462,     0,   940,     0,     0,     0,   438,     0,
       0,   463,     0,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,     0,
     453,   454,   455,   456,   457,     0,   459,   439,     0,   460,
     461,   462,     0,     0,     0,     0,     0,     0,     0,     0,
     463,     0,     0,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   436,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,   358,   460,
     461,   462,    22,    23,     0,     0,     0,     0,     0,     0,
     463,   359,     0,    29,   360,     0,     0,     0,     0,    32,
       0,     0,     0,     0,     0,     0,    37,     0,     0,  -333,
       0,     0,     0,    41,     0,     0,     0,     0,     0,     0,
       0,     0,   -68,   437,     0,     0,     0,   438,   361,     0,
    -333,     0,     0,    54,     0,    56,     0,     0,     0,    60,
    -333,   -68,    61,     0,     0,   362,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,   439,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    82,     0,     0,
      84,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   230,   453,
     454,   455,   456,   457,     0,   459,     0,     0,   460,   461,
     462,     0,   231,   232,     0,   233,     0,     0,     0,   463,
     234,     0,    23,     0,     0,     0,     0,     0,   235,     0,
      98,     0,     0,     0,   236,   363,     0,     0,     0,     0,
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
     240,     0,     0,     0,     0,     0,     0,     0,  -308,   241,
       0,     0,     0,     0,     0,     0,   242,   243,     0,     0,
       0,     0,  -308,  -308,   244,  -308,     0,     0,     0,     0,
    -308,     0,     0,     0,   245,     0,     0,     0,  -308,     0,
       0,     0,     0,   246,  -308,   247,     0,   248,     0,   249,
    -308,     0,   250,     0,  -308,     0,   251,  -308,   252,  -308,
       0,   253,     0,     0,     0,     0,     0,   230,  -308,     0,
       0,     0,     0,     0,     0,  -308,  -308,     0,     0,     0,
       0,   231,   232,  -308,   233,     0,     0,     0,     0,   234,
       0,     0,     0,  -308,     0,     0,     0,   235,     0,     0,
       0,     0,  -308,   236,  -308,     0,  -308,     0,  -308,   237,
       0,  -308,     0,   238,     0,  -308,   239,  -308,   240,     0,
    -308,     0,     0,     0,     0,     0,  -309,   241,     0,     0,
       0,     0,     0,     0,   242,   243,     0,     0,     0,     0,
    -309,  -309,   244,  -309,     0,     0,     0,     0,  -309,     0,
       0,     0,   245,     0,     0,     0,  -309,     0,     0,     0,
       0,   246,  -309,   247,     0,   248,     0,   249,  -309,     0,
     250,     0,  -309,     0,   251,  -309,   252,  -309,     0,   253,
       0,     0,     0,     0,     0,     0,  -309,     0,     0,     0,
       0,     0,     0,  -309,  -309,     0,     0,     0,     0,     0,
       0,  -309,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -309,     0,     0,     0,     0,     0,     0,     0,     0,
    -309,     0,  -309,     0,  -309,     0,  -309,     0,     0,  -309,
       0,     0,     0,  -309,     0,  -309,     0,     0,  -309
};

static const yytype_int16 yycheck[] =
{
       2,     2,   265,   401,   552,   110,   181,     2,   590,    62,
     165,   620,    26,    62,    62,    67,   425,   223,   391,   605,
     606,   579,   632,    25,   492,   611,   612,   290,   628,   516,
      25,     2,   229,   796,    86,   608,   831,   131,   826,   884,
     547,   680,   381,   682,   790,     1,     1,     1,   403,   101,
     203,    56,    66,   965,    25,   849,     5,    47,    22,    32,
     523,   892,    32,     1,   437,   438,   329,    35,   265,    56,
      27,    32,    74,    47,   140,   119,    78,    79,   146,    74,
       3,   102,   148,    78,    79,   119,   261,    46,    32,   146,
       1,     3,   122,   290,    32,    32,    24,    75,   100,    32,
      32,    32,   104,    74,    46,   100,   174,    78,    79,   104,
     483,   484,    40,    41,    42,   172,   489,   171,     1,   173,
      75,    32,    79,    61,    66,    89,    85,     3,   518,   100,
     174,    54,   329,   104,    91,     3,   967,    61,   116,   169,
     174,   109,    54,    85,    47,   119,   119,   111,   169,   119,
      61,   164,   164,   146,   113,   545,   146,   114,   171,   119,
     173,   116,   174,    18,    47,    24,   171,   105,   124,   632,
      93,   113,   146,    32,  1019,   119,   776,   170,    54,   117,
     103,    40,    41,    42,   171,   146,    54,  1099,   543,   145,
     146,   103,   170,   117,   105,   958,   169,   146,    47,   169,
     174,   117,   965,   576,   762,    56,   117,   145,   169,   146,
    1055,  1056,   365,   215,   174,   170,   172,   171,   481,   482,
     215,   170,   968,    78,  1018,   169,  1014,   813,   814,   815,
     146,   169,   169,   164,   145,   103,   169,   169,   169,   146,
     119,   124,    97,   146,   215,   173,    47,    47,   830,   446,
     425,   738,   146,   164,   593,   146,    45,   112,   169,   118,
       0,   164,   145,   146,    47,   172,     3,   269,   146,   146,
     119,   174,   274,   164,   269,   146,  1121,   164,   172,   274,
      69,   164,   164,    20,   481,   482,   380,   794,   861,   171,
      91,    91,   170,   556,    24,   172,    33,   146,   269,  1144,
      47,   172,    32,   274,    46,    24,    50,    53,    91,    53,
      40,    41,    42,    32,   173,   164,   146,    54,  1081,   516,
    1115,    40,    41,    42,    66,   174,    63,   146,    74,   535,
      74,   146,   124,    77,   146,   164,  1099,   146,    84,   939,
      84,   496,   172,    85,    91,   174,    56,   349,   349,   402,
      56,   146,    31,   172,   349,   146,   966,   172,   150,   556,
     172,   363,   169,   172,   171,   109,   103,   146,   363,   146,
     372,   113,   109,    56,   146,   170,   390,   372,   349,     2,
     146,   172,   579,   385,   398,    27,   146,   164,   118,   124,
     385,   597,   363,   172,   164,   882,   171,   164,   173,   118,
     172,   372,    25,   171,   174,    47,   172,   174,   146,    51,
      91,    90,   172,    90,   385,    94,   151,    94,   153,   154,
      11,   156,   157,   425,   476,  1007,   164,   479,   480,   987,
    1069,  1070,    90,   109,    76,   922,    94,    79,  1152,    81,
      56,  1027,  1028,   173,   146,   146,   146,   146,    56,    91,
    1059,    74,   146,   146,   173,    78,    79,   146,   926,  1045,
    1046,   140,   164,   164,   164,   164,   145,   109,   171,   148,
     164,   164,   114,   152,   488,   164,   164,   100,   164,    90,
      56,   104,   170,    94,   170,    90,   884,   860,   277,    94,
     279,   544,   146,   146,   903,   544,   544,  1211,  1212,  1213,
    1214,  1215,  1216,   876,   103,   507,    83,   509,   146,    86,
     164,   164,   507,     5,   509,   620,    90,   569,   570,    56,
      94,  1107,  1108,  1109,  1110,    56,   164,    90,    56,     2,
      22,    94,  1114,     9,    26,    27,   507,   326,   509,  1148,
     171,   738,   140,    35,   917,    37,    38,   145,   921,    56,
     148,    43,    25,   171,   152,   173,   100,   171,    50,   173,
      56,   575,   171,    56,   173,   762,     2,   940,   941,   171,
     171,   173,   173,    31,    78,    79,    78,    79,   291,   292,
      72,   583,   583,   569,   570,    77,    31,    79,   583,    25,
     171,    83,   215,   607,    86,    56,    56,    89,     5,    91,
      56,    74,   148,   169,    56,    78,    79,   145,   169,   171,
    1158,   984,   583,   986,   164,   163,    56,   109,   169,   111,
      31,  1019,   114,   164,    27,   164,    85,   100,   825,   826,
     164,   104,    90,   788,   164,   164,    94,   164,    74,   164,
     164,   164,    78,    79,    47,    90,   269,   146,    51,    94,
     171,   274,   171,   171,   145,   172,   164,  1055,   171,  1207,
     146,   164,  1060,   172,   100,     9,     3,    66,   104,    79,
     171,   169,   164,    76,   124,   164,    79,   148,    81,    90,
      69,   171,   140,    94,   172,   882,   124,   145,    91,   147,
     148,   146,   124,   124,   152,   140,   851,    66,    34,    34,
     145,   159,   147,   148,   172,   124,   109,   152,   760,   167,
     172,   114,   145,    21,   159,   172,   169,   118,   146,   170,
     165,   166,   167,  1121,   171,   922,   349,   779,   903,   140,
     141,   124,   143,   144,   145,   105,   147,   148,   149,   148,
     363,   152,   215,   148,   171,   140,   148,   158,   159,   372,
     148,   162,   148,   755,   165,   166,   167,    27,   148,   172,
     148,   148,   385,   148,   766,   176,   148,   769,    27,   172,
     148,   766,   148,   148,   769,   148,   148,    47,   164,   215,
     164,    51,   174,   164,   164,    32,    34,   145,    47,   172,
     987,    34,    51,   172,   171,   766,   269,   146,   769,   170,
     164,   274,   169,    47,   146,   170,    76,    34,   174,    79,
     172,    81,   601,   602,   174,   174,   164,    76,   164,   171,
      79,    91,   611,   612,   171,   145,   164,   146,    32,  1092,
    1093,   884,    91,   269,   174,    34,   164,   890,   274,   109,
     172,   172,   856,   164,   114,   174,   899,   174,   172,   172,
     109,   163,   163,   146,   109,   114,   858,   578,   987,    27,
     264,   863,   372,   858,    24,   823,   338,   389,   863,   879,
     872,   365,   980,   875,   529,   544,   349,   872,   383,    47,
     875,   903,   899,    51,   507,    31,   509,   858,   304,  1207,
     363,   890,   863,  1062,  1217,  1092,  1093,  1210,   850,   372,
     686,   872,   172,   531,   875,   129,   526,    20,    76,   775,
     926,    79,   385,   349,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,   363,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   372,    -1,   940,   941,
      -1,   109,    -1,    -1,    90,   947,   114,   949,    94,   385,
     952,  1056,   947,    -1,   949,    -1,    -1,   952,    -1,    -1,
     583,  1136,   976,    -1,    -1,    -1,  1019,    -1,    -1,    -1,
      -1,    -1,    -1,   975,    -1,    -1,   947,    -1,   949,    -1,
     975,   952,    -1,   985,    -1,    31,    -1,    -1,    -1,    -1,
     985,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,   975,    -1,   152,   153,   154,    -1,
     156,   157,   158,   159,   985,    -1,   162,    -1,    -1,   165,
     166,   167,    -1,    -1,  1026,   814,   815,    -1,     2,    -1,
     176,  1026,    -1,  1035,   507,     2,   509,    -1,    -1,  1144,
    1035,    -1,    -1,  1148,    90,    -1,    -1,    -1,    94,    -1,
      -1,    25,    -1,    -1,    -1,  1026,    -1,    -1,    25,    -1,
      -1,  1236,    -1,  1238,  1035,    -1,  1241,  1242,  1121,    -1,
      -1,   507,    -1,   509,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   874,  1088,    -1,    -1,   878,
      -1,    -1,    -1,  1088,   140,   141,    -1,   143,   144,   145,
      74,   147,   148,    -1,    78,    79,   152,    74,    -1,    -1,
     583,    78,    79,   159,    -1,    -1,    -1,  1088,    -1,   165,
     166,   167,    -1,    -1,    -1,    -1,   100,  1129,    -1,  1131,
     104,    -1,    -1,   100,  1129,  1137,  1131,   104,  1140,    -1,
      -1,    -1,  1137,   766,    -1,  1140,   769,   583,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1129,    -1,
    1131,    -1,    -1,    -1,    -1,    -1,  1137,  1169,    -1,  1140,
      -1,    -1,    -1,  1175,  1169,    -1,    -1,  1179,  1180,    -1,
    1175,    -1,    -1,    -1,  1179,  1180,    -1,    -1,    -1,  1191,
      27,    -1,    -1,  1195,    -1,  1197,  1191,    -1,  1169,    -1,
    1195,    -1,  1197,    -1,  1175,    -1,    -1,    -1,  1179,  1180,
      47,    -1,    -1,    -1,    51,    -1,  1218,  1218,    -1,    -1,
    1191,    -1,    -1,  1218,  1195,    -1,  1197,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   858,    -1,    -1,    -1,    76,
     863,   215,    79,    -1,    81,    -1,    -1,  1218,   215,   872,
      -1,    -1,   875,    -1,    91,    -1,  1045,  1046,  1260,  1261,
    1262,  1263,    -1,    -1,    -1,  1260,  1261,  1262,  1263,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1260,
    1261,  1262,  1263,   766,    -1,   269,   769,    -1,    -1,    -1,
     274,    -1,   269,    -1,    -1,    -1,    -1,   274,    -1,    -1,
      -1,    -1,    -1,    -1,  1103,    -1,    -1,    -1,    -1,    -1,
    1109,  1110,    -1,    -1,   947,    -1,   949,    -1,    -1,   952,
     766,    -1,    31,   769,    -1,    -1,    -1,  1126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1139,    -1,   975,  1142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   985,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   349,    -1,    -1,    -1,    -1,
      -1,    -1,   349,    -1,    -1,   858,    -1,     2,    -1,   363,
     863,    90,    -1,    -1,    -1,    94,   363,    -1,   372,   872,
      -1,    -1,   875,  1026,    67,   372,    -1,    -1,    -1,    -1,
      25,   385,  1035,    -1,    -1,    -1,    -1,    -1,   385,    -1,
      -1,    -1,   858,    86,    -1,    -1,    -1,   863,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   872,    -1,   101,   875,
      -1,   140,   141,    -1,    -1,   144,   145,    -1,   147,   148,
      -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    74,
     159,    -1,    -1,    78,    79,  1088,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,   947,    -1,   949,    -1,    -1,   952,
      -1,    -1,    -1,    -1,    -1,   100,     2,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   975,    -1,    -1,    -1,  1129,    -1,  1131,    25,
      -1,   947,   985,   949,  1137,    -1,   952,  1140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   507,    31,   509,    -1,    -1,    -1,   975,
     507,    -1,   509,    -1,    -1,    -1,  1169,    -1,    -1,   985,
      -1,    -1,  1175,  1026,    -1,    90,  1179,  1180,    74,    94,
      -1,    -1,  1035,    -1,    -1,    -1,   229,    -1,  1191,    -1,
      -1,    -1,  1195,    -1,  1197,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,   104,    -1,
    1026,    -1,    -1,    90,    -1,  1218,    -1,    94,    -1,  1035,
     215,    -1,   265,    -1,    -1,   140,   141,    -1,    -1,   583,
     145,    -1,   147,   148,    -1,  1088,   583,   152,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   290,    -1,    -1,
     165,   166,   167,    -1,    -1,    -1,    -1,  1260,  1261,  1262,
    1263,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,  1088,    -1,   269,   152,  1129,    -1,  1131,   274,
      -1,   158,   159,    -1,  1137,   162,   329,  1140,   165,   166,
     167,    -1,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    -1,  1129,    -1,  1131,  1169,    -1,    -1,   215,
      -1,  1137,  1175,    -1,  1140,    -1,  1179,  1180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,  1191,   171,
      -1,   173,  1195,    -1,  1197,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1169,   349,    -1,    -1,    -1,    -1,  1175,
      -1,    -1,    -1,  1179,  1180,  1218,    -1,    -1,   363,    -1,
      -1,    -1,     3,   269,    -1,  1191,    -1,   372,   274,  1195,
      -1,  1197,    -1,    -1,    -1,    -1,    17,    -1,    -1,    20,
     385,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1218,    -1,    35,    -1,    -1,  1260,  1261,  1262,
    1263,    -1,   766,    -1,    45,   769,    -1,    -1,    -1,   766,
      -1,    52,   769,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,   476,    -1,    -1,   479,   480,   481,   482,
      71,    -1,    -1,    -1,  1260,  1261,  1262,  1263,    -1,    -1,
      -1,    -1,    -1,   349,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,   363,    99,    26,
      27,    -1,   103,   516,    -1,    -1,   372,    -1,    35,   110,
      37,    38,    -1,    -1,   115,    -1,    43,    -1,    -1,   385,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   858,    -1,    -1,    -1,    -1,   863,
      -1,   858,   507,   556,   509,    72,   863,    -1,   872,    -1,
      77,   875,    79,    -1,    -1,   872,   569,   570,   875,    -1,
      -1,    28,    89,    -1,    91,    -1,   579,    -1,    -1,     1,
      -1,     3,    -1,    40,    41,    42,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,   111,    17,    18,   114,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,   583,    -1,
      52,    -1,    54,   947,    -1,   949,    -1,    -1,   952,    -1,
     947,    63,   949,    -1,    -1,   952,    -1,    -1,    70,    71,
      -1,   507,    -1,   509,    -1,    -1,    78,    -1,    -1,    -1,
      -1,   975,    -1,    -1,    -1,    -1,    88,    -1,   975,    -1,
      -1,   985,    -1,    -1,    -1,    97,    -1,    99,   985,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
     112,    -1,   149,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,
      -1,    -1,  1026,    -1,    -1,    -1,    -1,    -1,    -1,  1026,
      -1,  1035,    -1,    -1,    -1,    -1,    -1,   583,  1035,    -1,
      -1,     3,    -1,    -1,    -1,   738,    -1,    -1,    -1,    -1,
      -1,    -1,   199,    -1,    -1,    17,    18,    -1,    20,   171,
      -1,    -1,    -1,    25,    -1,    -1,    -1,   760,    -1,   762,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,  1088,    -1,   779,    49,    -1,    -1,
      52,  1088,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,   258,   259,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,   766,    -1,   270,   769,  1129,    88,  1131,    -1,    -1,
      -1,    -1,  1129,  1137,  1131,    97,  1140,    99,    -1,   101,
    1137,   103,    -1,  1140,   106,    -1,    -1,    -1,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   310,    -1,  1169,    25,    -1,    -1,    -1,
      -1,  1175,  1169,    -1,    -1,  1179,  1180,    -1,  1175,    -1,
      -1,    -1,  1179,  1180,    -1,    -1,    -1,  1191,    -1,   882,
      -1,  1195,    -1,  1197,  1191,    -1,    -1,    -1,  1195,    -1,
    1197,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,   171,
      -1,    -1,   359,   858,  1218,    74,    -1,    -1,   863,    -1,
     766,  1218,    -1,   769,    -1,     3,    -1,   872,    -1,   922,
     875,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,    -1,   104,    -1,    25,   395,    -1,
     397,    -1,    -1,    -1,   401,    33,  1260,  1261,  1262,  1263,
      -1,    39,    -1,  1260,  1261,  1262,  1263,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,   425,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    90,    70,    71,   987,    94,    -1,    -1,    -1,    -1,
      78,    -1,   947,    -1,   949,    -1,    -1,   952,    -1,    -1,
      88,    -1,   858,    -1,    -1,    -1,    -1,   863,    -1,    97,
      -1,    99,    -1,   101,   123,   103,   872,    -1,   106,   875,
     975,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
     985,   140,   141,   490,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   215,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      -1,   518,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,  1026,    -1,    -1,   531,    -1,    -1,    -1,    -1,    -1,
    1035,    -1,    -1,   171,   541,    -1,    -1,    -1,   545,  1092,
    1093,   947,    -1,   949,    -1,    -1,   952,    -1,     3,    -1,
     269,    -1,    -1,    -1,    -1,   274,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,   975,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,   985,
      -1,    -1,   589,  1088,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,   600,    49,    -1,    -1,    52,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,   620,    -1,    70,    71,    -1,    -1,    -1,
    1026,    -1,    -1,    78,  1129,    -1,  1131,    -1,    -1,  1035,
      -1,    -1,  1137,    88,    -1,  1140,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    99,    -1,   101,    -1,   103,    13,
      -1,   106,    -1,    17,    -1,   110,    -1,   112,    -1,    -1,
     115,    -1,    26,    -1,  1169,    -1,   385,    -1,    -1,    33,
    1175,    -1,    -1,    -1,  1179,  1180,    -1,    -1,    -1,   686,
      -1,    45,  1088,    47,    48,    -1,  1191,    -1,    -1,    53,
    1195,    55,  1197,    57,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,
      74,    -1,    76,  1218,    -1,    -1,   171,    -1,    -1,    83,
      -1,    -1,    86,  1129,    88,  1131,    90,    -1,    -1,    93,
      94,  1137,    96,    -1,  1140,    -1,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1260,  1261,  1262,  1263,    -1,
      -1,    -1,    -1,  1169,   771,   129,    -1,   131,   775,  1175,
      -1,    -1,    -1,  1179,  1180,    -1,    -1,    -1,   785,    -1,
      -1,    -1,    -1,    -1,    -1,  1191,    -1,    -1,   507,  1195,
     509,  1197,    -1,    -1,    -1,    -1,    -1,   804,     1,    -1,
      -1,    -1,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,    -1,  1218,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    26,    27,    45,    -1,    -1,    -1,    -1,
      -1,    -1,    35,   197,    37,    38,    -1,   201,   202,    -1,
      43,    -1,    -1,   850,    -1,    -1,    -1,    50,    -1,    69,
      53,    -1,    -1,    -1,  1260,  1261,  1262,  1263,    78,    79,
      -1,    -1,    -1,    -1,    -1,   229,    -1,    -1,    -1,    72,
      -1,    74,   879,    -1,    77,    -1,    79,    -1,    81,    -1,
      83,    84,    -1,    86,    -1,    -1,    89,    -1,    91,   109,
      -1,    -1,    -1,    -1,    -1,    -1,   903,    -1,    -1,    -1,
      -1,   265,    -1,    -1,    -1,    -1,   109,    -1,   111,    -1,
       5,   114,    -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   290,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,   301,    -1,    -1,
      35,    -1,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    -1,   316,   317,    -1,    50,    -1,    -1,    -1,    -1,
      -1,   164,    -1,    -1,    -1,   329,    -1,   170,    -1,   333,
      -1,    -1,    -1,   980,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,   351,    83,    -1,
      -1,    86,    -1,    -1,    89,    -1,    91,   361,    -1,    -1,
      -1,   221,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   373,
      -1,   375,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,   389,   390,   391,    -1,    -1,
      -1,    -1,    -1,    -1,   398,    -1,    -1,    -1,    -1,    -1,
     260,   261,    -1,    -1,    -1,    -1,    -1,   766,    -1,    -1,
     769,    -1,  1059,    -1,    -1,    -1,    -1,   277,    -1,   279,
      -1,    -1,    -1,    -1,    -1,    -1,   286,    -1,    -1,   164,
     434,   435,   436,   437,   438,   439,    -1,   441,   442,   443,
     444,   445,    -1,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
      -1,    -1,    -1,    -1,    -1,    -1,   326,    -1,    -1,    -1,
      -1,    -1,   476,    -1,    -1,   479,   480,   481,   482,   483,
     484,    -1,    -1,    -1,   488,   489,    -1,    -1,   492,    -1,
      -1,    -1,    -1,    -1,   498,    -1,    -1,    -1,    -1,   858,
      -1,  1148,    -1,    -1,   863,   509,    -1,    -1,   512,    -1,
      -1,    -1,   516,   872,    -1,    -1,   875,    -1,    -1,   523,
      -1,   525,    -1,    -1,    -1,   529,    -1,    -1,   532,   533,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     544,    -1,    -1,   547,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   556,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   566,    -1,    -1,   569,   570,    -1,    -1,    -1,
      -1,   575,   576,    -1,    -1,   579,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   590,   591,   947,    -1,
     949,    -1,    -1,   952,    -1,    -1,    -1,     3,    -1,    -1,
     604,    -1,    -1,   607,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    18,    -1,    20,    -1,   975,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,   985,    33,   632,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,  1026,    -1,    -1,
      -1,    -1,    78,   677,    -1,    -1,  1035,    -1,    -1,    -1,
     684,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    -1,    99,    -1,   101,    -1,   103,    -1,   559,
     106,    -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1088,
      -1,    -1,    -1,   737,   738,    -1,    -1,    -1,    -1,    -1,
      -1,   601,   602,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   611,   612,    -1,    -1,    -1,   760,    -1,   762,   763,
     764,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
    1129,    -1,  1131,    -1,    -1,   779,    -1,    -1,  1137,    -1,
      -1,  1140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     794,    -1,    -1,    -1,   798,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1169,    -1,    -1,    -1,    -1,    -1,  1175,    -1,    -1,   823,
    1179,  1180,    -1,   827,    -1,   829,   830,   831,    -1,   833,
      -1,   835,  1191,    -1,   838,   839,  1195,    -1,  1197,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   852,    -1,
      -1,    -1,   856,    -1,    -1,    -1,   860,   861,    -1,   863,
      -1,    -1,    -1,    -1,    -1,   869,   870,    -1,   872,    -1,
      -1,   875,   876,    -1,    -1,    -1,    -1,    -1,   882,    -1,
      -1,    -1,    -1,   887,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1260,  1261,  1262,  1263,    -1,    -1,    -1,    -1,    -1,
     770,    -1,    -1,   917,   918,   919,   920,   921,   922,    -1,
      -1,    -1,   926,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   940,   941,    -1,    -1,
      -1,    -1,    -1,    -1,   804,    -1,   950,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   814,   815,    -1,     5,    -1,    -1,
      -1,    -1,   966,    -1,    -1,    -1,   970,    -1,    -1,    -1,
      -1,    -1,   976,    -1,    22,    -1,    -1,    -1,    26,    27,
     984,    -1,   986,   987,    -1,    -1,    -1,    35,    -1,    37,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    -1,    50,  1007,    -1,    -1,    -1,    -1,    -1,    -1,
    1014,    -1,  1016,    -1,   874,    -1,    -1,    -1,   878,  1023,
     880,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    77,
      -1,    79,  1036,    81,    -1,    83,    -1,    -1,    86,  1043,
      -1,    89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1064,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
       5,    -1,  1076,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,  1092,  1093,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,  1105,    37,    38,    -1,    -1,    -1,    -1,    43,  1113,
    1114,  1115,    -1,    -1,    -1,    50,   164,    -1,    -1,    -1,
      -1,    -1,   170,   983,    -1,  1129,    -1,  1131,    -1,    -1,
      -1,  1135,    -1,  1137,    -1,    -1,  1140,    72,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    83,    -1,
      -1,    86,    -1,    -1,    89,    -1,    91,    -1,  1162,  1163,
      -1,  1165,  1166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,
      -1,  1185,    -1,    -1,  1188,  1045,  1046,    -1,  1192,    -1,
      -1,    -1,    -1,  1197,  1198,    -1,    -1,  1201,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1209,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,  1223,
    1224,  1225,  1226,  1227,    -1,    -1,    31,    -1,    -1,   164,
      -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1103,    -1,    -1,    -1,    -1,    -1,  1109,
    1110,    -1,    -1,    -1,    -1,    -1,  1260,  1261,  1262,  1263,
    1264,  1265,  1266,  1267,    -1,    -1,  1126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1136,    -1,    -1,  1139,
      -1,    -1,  1142,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1177,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,   137,   138,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,   169,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    -1,    -1,    -1,  1236,    -1,  1238,    -1,
      -1,  1241,  1242,     0,     1,    -1,     3,     4,     5,     6,
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
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,   169,   170,   171,    -1,
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
     161,   162,   163,   164,    -1,    -1,   167,    -1,   169,   170,
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
     163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
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
      -1,    -1,   167,    -1,   169,    -1,   171,     1,   173,     3,
       4,   176,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      24,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    42,    -1,
      44,    45,    -1,    47,    -1,    49,    -1,    51,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    78,    79,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,   109,   110,    -1,   112,    -1,
     114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,   172,   173,
      -1,     1,   176,     3,     4,    -1,     6,     7,     8,     9,
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
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,     3,     4,   176,     6,     7,     8,
       9,    10,    -1,    12,    -1,    14,    -1,    -1,    17,    18,
      19,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    55,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,   137,   138,
      -1,   140,   141,   142,   143,   144,   145,    -1,   147,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,   171,    -1,    -1,     3,     4,   176,     6,     7,
       8,     9,    10,    -1,    12,    -1,    14,    -1,    -1,    17,
      18,    19,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    55,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,   137,
     138,    -1,   140,   141,   142,   143,   144,   145,    -1,   147,
      -1,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,   165,   166,   167,
     168,   169,    -1,   171,    -1,    -1,    -1,    -1,   176,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    -1,    17,    18,    -1,    20,    -1,    22,    23,
      -1,    25,    -1,    -1,    -1,    -1,    30,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    62,    63,
      64,    65,    -1,    67,    68,    -1,    70,    71,    72,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,
       3,     4,   176,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    -1,    51,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    78,    79,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,   109,   110,    -1,   112,
      -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
      -1,     3,     4,   176,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
     112,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,    -1,     3,     4,   176,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    24,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    42,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
     111,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,     3,     4,   176,     6,     7,     8,     9,
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
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,     3,     4,   176,     6,     7,     8,
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
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,   107,
     108,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,
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
     107,   108,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   106,   107,   108,    -1,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,    -1,   174,
      -1,   176,     3,     4,    -1,     6,     7,     8,     9,    10,
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
      68,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,   107,
     108,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
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
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,   146,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,
     167,    -1,   169,   170,   171,    -1,    -1,     3,     4,   176,
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
      -1,   106,    -1,    -1,    -1,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,   146,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,   170,   171,    -1,    -1,     3,
       4,   176,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
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
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
      -1,     3,     4,   176,     6,     7,     8,     9,    10,    -1,
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
     121,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,
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
     143,   144,    -1,   146,    -1,    -1,   149,    -1,    -1,    -1,
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
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,    -1,   174,    -1,   176,     3,     4,    -1,     6,     7,
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
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,     4,
      -1,     6,     7,     8,     9,    10,    -1,   158,    -1,    14,
     161,   162,   163,    18,    -1,    -1,   167,    22,   169,    24,
     171,    -1,    27,    -1,    -1,   176,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,
      65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    79,    80,    81,    82,    -1,    -1,
      -1,    -1,    87,    -1,    89,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,   172,   173,    -1,
       4,   176,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    18,    -1,    -1,    -1,    22,    -1,
      24,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    79,    80,    81,    82,    -1,
      -1,    -1,    -1,    87,    -1,    89,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    97,    98,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    12,    -1,   149,    -1,    -1,    17,    18,
      19,    20,    -1,    -1,   158,    -1,    25,   161,   162,   163,
      -1,    -1,    -1,   167,    33,   169,    35,   171,    -1,   173,
      39,    -1,   176,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,   137,   138,
      -1,   140,   141,   142,   143,   144,   145,    -1,   147,    -1,
      -1,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,    -1,   162,    -1,    -1,   165,   166,   167,   168,
       3,     4,   171,     6,     7,     8,     9,    10,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,     3,     4,    -1,     6,     7,     8,     9,    10,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
     142,    -1,    -1,     3,     4,    -1,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,   163,    22,    -1,    24,    25,    -1,   169,    -1,   171,
      -1,   173,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    42,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,     3,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    12,   112,    -1,    -1,   115,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,   142,    -1,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,   163,    -1,    -1,    -1,    -1,    -1,    -1,
      71,   171,    -1,   173,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    99,    -1,
     101,    -1,   103,    -1,    -1,    -1,    -1,    -1,   109,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,   137,   138,    -1,   140,
     141,   142,   143,   144,   145,    -1,   147,    -1,    -1,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
       3,   162,    -1,    -1,   165,   166,   167,   168,   169,    12,
      -1,    -1,    -1,    -1,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    99,    -1,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,    -1,   137,   138,    -1,   140,   141,   142,
     143,   144,   145,    -1,   147,    -1,    -1,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,     5,   162,
      -1,    -1,   165,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,    38,    -1,    12,    -1,    -1,    43,    -1,    -1,    -1,
      19,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    86,
      -1,    -1,    89,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    90,    -1,    12,    -1,    94,    -1,    -1,    -1,    -1,
      19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   164,   137,   138,
      -1,   140,   141,   170,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
     169,    90,    -1,    12,    -1,    94,    -1,   176,    -1,    -1,
      19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,   137,   138,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
     169,    90,    -1,    12,    -1,    94,    -1,   176,    -1,    -1,
      19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,   137,   138,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
     169,    90,    -1,    12,    -1,    94,    -1,   176,    -1,    -1,
      19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,   137,   138,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
     169,    90,    -1,    -1,    -1,    94,    -1,   176,    -1,    -1,
      -1,    -1,    12,   102,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    47,   137,   138,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
     169,    -1,    -1,    -1,    -1,    -1,    12,   176,    -1,    -1,
      90,    -1,    -1,    19,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,    47,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,    90,   165,   166,   167,    94,   169,
      -1,    -1,    -1,    -1,    -1,    -1,   176,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,    -1,    31,   123,    -1,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    73,   165,
     166,   167,    -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,
     176,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,   123,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    73,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    -1,    -1,    90,    12,    -1,    -1,    94,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    90,    -1,    12,    -1,    94,    -1,
      -1,   176,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    90,    -1,    12,    -1,    94,    -1,
     176,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    90,    -1,    12,    -1,    94,    -1,
     176,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    90,    12,    -1,    -1,    94,    -1,
     176,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   123,    -1,   165,   166,
     167,    -1,    -1,    -1,    12,    -1,    -1,   174,    -1,   176,
      -1,    19,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    31,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,   176,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    12,    -1,    94,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,
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
     145,    -1,   147,   148,   149,    31,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   123,    -1,
     165,   166,   167,    -1,    -1,    90,    12,    -1,    -1,    94,
      -1,   176,    -1,    19,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    31,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   123,    -1,
     165,   166,   167,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,   176,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,   176,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,    -1,   162,   123,    -1,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    31,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    22,   165,
     166,   167,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
     176,    35,    -1,    37,    38,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    90,    -1,    -1,    -1,    94,    72,    -1,
      74,    -1,    -1,    77,    -1,    79,    -1,    -1,    -1,    83,
      84,    85,    86,    -1,    -1,    89,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,     3,   156,
     157,   158,   159,   160,    -1,   162,    -1,    -1,   165,   166,
     167,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,   176,
      25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,
     164,    -1,    -1,    -1,    39,   169,    -1,    -1,    -1,    -1,
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
     374,   348,   348,   243,   372,   243,   372,   102,   171,   342,
     243,   372,   219,     1,    27,    47,    51,    76,    79,    91,
     109,   114,   172,   362,   363,   364,   372,   388,   329,   372,
     124,   172,   146,   124,   124,   185,    34,   185,   372,    34,
     372,    66,   172,   349,   332,   146,   172,   223,   372,   124,
     333,   372,   330,   262,   219,   324,   372,   372,   325,   172,
     219,   310,   283,   332,   145,   340,   341,   339,   288,   204,
       1,   260,   348,   172,    21,   256,   170,   172,   172,   346,
     374,   172,   346,   342,   243,   372,   195,   172,   197,   198,
     350,   170,   170,   146,   164,    13,   148,   146,   164,    13,
      36,    73,   219,   140,   141,   142,   143,   144,   158,   162,
     167,   207,   296,   297,   298,   372,   207,   209,   268,   169,
     237,   325,   118,   219,   224,   224,   171,   372,   174,   361,
      47,   146,   164,   174,   345,   361,   342,   174,   345,   122,
     146,   174,    32,   224,   361,    32,    47,   224,   361,   146,
     172,   164,   171,     1,   269,   170,   275,   124,   146,   170,
     146,   172,   338,     1,   223,   334,   335,   105,   311,   171,
     300,   372,   140,   148,   300,   300,   372,   322,   171,   173,
     164,   164,   164,   164,   172,   174,   349,    47,   174,    32,
      32,    47,   171,   172,    47,    91,   146,   172,    17,    20,
      25,    45,    52,    63,    71,    88,    99,   110,   115,   351,
      90,    90,   164,   374,   350,   372,   372,    34,   185,    34,
      34,   185,   102,   205,   219,   172,   172,   331,   338,    69,
     374,   219,   172,   172,   329,   341,   145,   284,   172,   339,
     150,   299,   333,   372,   170,    75,   116,   170,   261,   172,
     171,   205,   219,   257,    47,   174,    47,   146,   172,   213,
     220,    17,    18,    20,    25,    45,    49,    52,    71,    78,
      88,    97,    99,   101,   110,   112,   115,   169,   212,   298,
     372,   372,   207,   209,   146,   164,    13,   164,   169,   269,
     321,   329,   372,    47,   342,   185,   174,   243,   372,   174,
     345,   185,   372,   164,   174,   174,   146,   174,   372,   174,
     372,   372,   224,    44,   372,   243,   372,   224,   249,   205,
     349,   170,    83,    86,   170,   184,   192,   226,   372,   273,
     274,   304,   311,    61,   117,   315,   302,   303,   172,   295,
     297,   172,   174,   172,   243,   372,    44,   243,   372,   349,
     363,   338,   351,   351,   185,   185,   372,   185,    34,   164,
     164,   333,   171,   171,   164,   299,   333,   334,   311,   341,
     372,   185,   239,   342,   257,   145,   205,   243,   372,   243,
     372,   198,   207,    13,    36,    73,   164,   164,   298,   372,
     372,   269,   170,   164,   164,   164,   372,   172,   185,   174,
     361,   174,   361,   174,   185,   122,   372,    32,   224,   361,
      32,   224,   361,   172,   192,   226,   226,   275,   192,   315,
       3,    54,    93,   103,   316,   317,   318,   372,   291,   172,
     300,   300,   174,   174,   372,    32,    32,   172,   333,    34,
     185,   348,   348,   299,   333,    32,   224,   172,   372,   174,
     174,   170,   372,   207,   209,    13,   170,   224,    44,   185,
     372,   174,    44,   185,   372,   174,   372,   102,    44,   372,
     224,    44,   372,   224,   164,   226,   275,   278,   318,   117,
     146,   124,   151,   153,   154,   156,   157,    61,    32,   164,
     204,   312,   313,    44,    44,   102,    44,    44,   185,   172,
     172,   185,   205,   164,   164,   372,   372,   185,   372,   185,
     372,   372,   372,   313,   372,   317,   318,   318,   318,   318,
     318,   318,   316,   183,   372,   372,   372,   372,   163,   163,
     102,   102,   102,   102,   102,   102,   102,   102,   372,   372,
     372,   372
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
     214,   215,   216,   216,   216,   216,   217,   217,   217,   218,
     218,   218,   218,   218,   219,   219,   219,   220,   220,   220,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     223,   223,   223,   224,   224,   225,   225,   226,   226,   227,
     226,   228,   226,   229,   229,   229,   229,   229,   229,   229,
     230,   230,   230,   230,   231,   232,   232,   233,   234,   235,
     235,   235,   236,   235,   237,   235,   238,   239,   240,   240,
     241,   241,   241,   241,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   243,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   245,   245,   246,
     247,   247,   248,   248,   249,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   251,   251,
     251,   251,   251,   251,   252,   252,   252,   253,   253,   253,
     254,   254,   254,   254,   255,   255,   256,   256,   256,   257,
     257,   258,   259,   259,   260,   260,   261,   261,   261,   262,
     262,   262,   263,   263,   264,   265,   265,   266,   267,   267,
     267,   268,   268,   269,   269,   269,   269,   269,   270,   270,
     271,   272,   272,   273,   272,   272,   274,   272,   275,   275,
     276,   278,   277,   279,   280,   280,   280,   281,   281,   282,
     282,   283,   283,   283,   284,   284,   285,   287,   286,   288,
     286,   289,   291,   290,   292,   292,   292,   292,   292,   293,
     294,   294,   295,   295,   295,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   297,
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
       3,     3,     4,     4,     4,     4,     0,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     2,     3,     1,     1,     0,
       3,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       3,     5,     5,     2,     1,     1,     1,     1,     1,     6,
       7,     3,     0,     6,     0,     6,     2,     0,     3,     2,
       1,     1,     1,     1,     5,     3,     3,     6,     5,     6,
       5,     6,     3,     4,     3,     4,     6,     7,     6,     7,
       4,     5,     4,     5,     4,     4,     3,     6,     5,     4,
       3,     6,     5,     6,     5,     8,     7,     4,     4,     2,
       5,     2,     1,     3,     1,     1,     1,     1,     1,     1,
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
#line 5999 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 690 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6005 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 691 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6011 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 698 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6019 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 706 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6027 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 710 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6036 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 720 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6044 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 725 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6052 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 732 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6058 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 733 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6064 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 734 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6070 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 735 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6076 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 736 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6082 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 737 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6088 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 738 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6094 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 739 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6100 "bison-chpl-lib.cpp"
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
#line 6122 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: try_stmt  */
#line 758 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6128 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: return_stmt  */
#line 759 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6134 "bison-chpl-lib.cpp"
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
#line 6146 "bison-chpl-lib.cpp"
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
#line 6159 "bison-chpl-lib.cpp"
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
#line 6172 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 787 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6180 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 791 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6191 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 798 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6200 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 805 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6206 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 806 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6212 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 807 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6218 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 808 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6224 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 809 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6230 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 810 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6236 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 811 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6242 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 813 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6248 "bison-chpl-lib.cpp"
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
#line 6272 "bison-chpl-lib.cpp"
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
#line 6284 "bison-chpl-lib.cpp"
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
#line 6305 "bison-chpl-lib.cpp"
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
#line 6325 "bison-chpl-lib.cpp"
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
#line 6346 "bison-chpl-lib.cpp"
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
#line 6367 "bison-chpl-lib.cpp"
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
#line 6387 "bison-chpl-lib.cpp"
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
#line 6409 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 952 "chpl.ypp"
  {
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 6417 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 973 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6425 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 980 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6434 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 987 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6442 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 991 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6453 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 1001 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6459 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 1003 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6467 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 1007 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6475 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 1014 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6484 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 1019 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6494 "bison-chpl-lib.cpp"
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
#line 6516 "bison-chpl-lib.cpp"
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
#line 6537 "bison-chpl-lib.cpp"
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
#line 6558 "bison-chpl-lib.cpp"
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
#line 6577 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1104 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6583 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1105 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6590 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1107 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6597 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1112 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6603 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1113 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6610 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1119 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6620 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1137 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6629 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1142 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6638 "bison-chpl-lib.cpp"
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
#line 6651 "bison-chpl-lib.cpp"
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
#line 6670 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1177 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6676 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1178 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6682 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1183 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6690 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1187 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6698 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1191 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6707 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1196 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6715 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1200 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6723 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1204 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6732 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1212 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6741 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1217 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6751 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1223 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6760 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1228 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6771 "bison-chpl-lib.cpp"
    break;

  case 90: /* opt_only_ls: %empty  */
#line 1236 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6777 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: renames_ls  */
#line 1237 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6783 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1242 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6792 "bison-chpl-lib.cpp"
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
#line 6804 "bison-chpl-lib.cpp"
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
#line 6817 "bison-chpl-lib.cpp"
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
#line 6829 "bison-chpl-lib.cpp"
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
#line 6842 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1283 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6851 "bison-chpl-lib.cpp"
    break;

  case 114: /* import_expr: expr  */
#line 1318 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6859 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr TDOT all_op_name  */
#line 1322 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6868 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1327 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6877 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TAS ident_use  */
#line 1332 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6887 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1338 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6897 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_ls: import_expr  */
#line 1346 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6903 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_ls TCOMMA import_expr  */
#line 1347 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6909 "bison-chpl-lib.cpp"
    break;

  case 121: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1352 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6919 "bison-chpl-lib.cpp"
    break;

  case 122: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1361 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6927 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1365 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6935 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1369 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6943 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1373 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6951 "bison-chpl-lib.cpp"
    break;

  case 126: /* opt_label_ident: %empty  */
#line 1381 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6957 "bison-chpl-lib.cpp"
    break;

  case 127: /* opt_label_ident: TIDENT  */
#line 1382 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6963 "bison-chpl-lib.cpp"
    break;

  case 128: /* opt_label_ident: TINIT  */
#line 1383 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6969 "bison-chpl-lib.cpp"
    break;

  case 137: /* ident_use: TIDENT  */
#line 1409 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6975 "bison-chpl-lib.cpp"
    break;

  case 138: /* ident_use: TINIT  */
#line 1410 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6981 "bison-chpl-lib.cpp"
    break;

  case 139: /* ident_use: TTHIS  */
#line 1411 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6987 "bison-chpl-lib.cpp"
    break;

  case 158: /* scalar_type: TBOOL  */
#line 1437 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6993 "bison-chpl-lib.cpp"
    break;

  case 159: /* scalar_type: TENUM  */
#line 1438 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6999 "bison-chpl-lib.cpp"
    break;

  case 160: /* scalar_type: TINT  */
#line 1439 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7005 "bison-chpl-lib.cpp"
    break;

  case 161: /* scalar_type: TUINT  */
#line 1440 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7011 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TREAL  */
#line 1441 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7017 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TIMAG  */
#line 1442 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7023 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TCOMPLEX  */
#line 1443 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7029 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TBYTES  */
#line 1444 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7035 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TSTRING  */
#line 1445 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7041 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TLOCALE  */
#line 1446 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7047 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TNOTHING  */
#line 1447 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7053 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TVOID  */
#line 1448 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7059 "bison-chpl-lib.cpp"
    break;

  case 173: /* do_stmt: TDO stmt  */
#line 1461 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7065 "bison-chpl-lib.cpp"
    break;

  case 174: /* do_stmt: block_stmt  */
#line 1462 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7071 "bison-chpl-lib.cpp"
    break;

  case 175: /* return_stmt: TRETURN TSEMI  */
#line 1467 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7082 "bison-chpl-lib.cpp"
    break;

  case 176: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1474 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7093 "bison-chpl-lib.cpp"
    break;

  case 177: /* class_level_stmt: TSEMI  */
#line 1484 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7101 "bison-chpl-lib.cpp"
    break;

  case 178: /* class_level_stmt: inner_class_level_stmt  */
#line 1488 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7111 "bison-chpl-lib.cpp"
    break;

  case 179: /* $@1: %empty  */
#line 1493 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7118 "bison-chpl-lib.cpp"
    break;

  case 180: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1495 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7127 "bison-chpl-lib.cpp"
    break;

  case 181: /* $@2: %empty  */
#line 1499 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7134 "bison-chpl-lib.cpp"
    break;

  case 182: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1501 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7143 "bison-chpl-lib.cpp"
    break;

  case 190: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1519 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7152 "bison-chpl-lib.cpp"
    break;

  case 191: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1524 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7161 "bison-chpl-lib.cpp"
    break;

  case 192: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1529 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7170 "bison-chpl-lib.cpp"
    break;

  case 193: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1534 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7178 "bison-chpl-lib.cpp"
    break;

  case 194: /* forwarding_decl_start: TFORWARDING  */
#line 1541 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7187 "bison-chpl-lib.cpp"
    break;

  case 195: /* extern_or_export: TEXTERN  */
#line 1548 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7193 "bison-chpl-lib.cpp"
    break;

  case 196: /* extern_or_export: TEXPORT  */
#line 1549 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7199 "bison-chpl-lib.cpp"
    break;

  case 197: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1554 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7209 "bison-chpl-lib.cpp"
    break;

  case 198: /* extern_export_kind_inner: TTYPE  */
#line 1567 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7218 "bison-chpl-lib.cpp"
    break;

  case 199: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1576 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7228 "bison-chpl-lib.cpp"
    break;

  case 200: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1583 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7240 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1591 "chpl.ypp"
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
#line 7262 "bison-chpl-lib.cpp"
    break;

  case 202: /* $@3: %empty  */
#line 1609 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7270 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 1613 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-4])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7279 "bison-chpl-lib.cpp"
    break;

  case 204: /* $@4: %empty  */
#line 1618 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7288 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr extern_export_kind_inner $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1623 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yylsp[-5]), (yyvsp[-3].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7297 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1631 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7305 "bison-chpl-lib.cpp"
    break;

  case 207: /* no_loop_attributes: %empty  */
#line 1637 "chpl.ypp"
  {
    // Even though there are no attributes, there could be loop pragmas.
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 7314 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1643 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7320 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1644 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7326 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1661 "chpl.ypp"
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
#line 7353 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1684 "chpl.ypp"
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
#line 7376 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1703 "chpl.ypp"
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
#line 7399 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1722 "chpl.ypp"
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
#line 7428 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1747 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7437 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: for_loop_kw expr TIN expr task_intent_clause do_stmt  */
#line 1752 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7446 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1757 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7455 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator task_intent_clause do_stmt  */
#line 1762 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7464 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1767 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7473 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: for_loop_kw expr task_intent_clause do_stmt  */
#line 1772 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7482 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1777 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7491 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw zippered_iterator task_intent_clause do_stmt  */
#line 1782 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7500 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1787 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7509 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TLSBR expr_ls TIN expr task_intent_clause TRSBR stmt  */
#line 1792 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7518 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1797 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7527 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator task_intent_clause TRSBR stmt  */
#line 1802 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7536 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1807 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7545 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TLSBR expr_ls task_intent_clause TRSBR stmt  */
#line 1812 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7554 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1817 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7564 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR zippered_iterator task_intent_clause TRSBR stmt  */
#line 1823 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7574 "bison-chpl-lib.cpp"
    break;

  case 234: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1832 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7584 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF expr TTHEN stmt  */
#line 1841 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7592 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF expr block_stmt  */
#line 1845 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7600 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1849 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7608 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1853 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7616 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1857 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7624 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF ifvar block_stmt  */
#line 1861 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7632 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1865 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7640 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1869 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7648 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1873 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7657 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1878 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7666 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1883 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7675 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1888 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7684 "bison-chpl-lib.cpp"
    break;

  case 247: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1896 "chpl.ypp"
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
#line 7704 "bison-chpl-lib.cpp"
    break;

  case 248: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1912 "chpl.ypp"
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
#line 7724 "bison-chpl-lib.cpp"
    break;

  case 249: /* interface_start: TINTERFACE ident_def  */
#line 1931 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Interface);
  }
#line 7732 "bison-chpl-lib.cpp"
    break;

  case 250: /* interface_stmt: interface_start TLP ifc_formal_ls TRP block_stmt  */
#line 1938 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-4].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), parts, (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7742 "bison-chpl-lib.cpp"
    break;

  case 251: /* interface_stmt: interface_start block_stmt  */
#line 1944 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-1].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), parts, nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7752 "bison-chpl-lib.cpp"
    break;

  case 252: /* ifc_formal_ls: ifc_formal  */
#line 1952 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7758 "bison-chpl-lib.cpp"
    break;

  case 253: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1953 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7764 "bison-chpl-lib.cpp"
    break;

  case 254: /* ifc_formal: ident_def  */
#line 1958 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7770 "bison-chpl-lib.cpp"
    break;

  case 274: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 2001 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7778 "bison-chpl-lib.cpp"
    break;

  case 275: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 2005 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7786 "bison-chpl-lib.cpp"
    break;

  case 276: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 2009 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7794 "bison-chpl-lib.cpp"
    break;

  case 277: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2016 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7802 "bison-chpl-lib.cpp"
    break;

  case 278: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 2020 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7810 "bison-chpl-lib.cpp"
    break;

  case 279: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2024 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7818 "bison-chpl-lib.cpp"
    break;

  case 280: /* try_stmt: TTRY tryable_stmt  */
#line 2031 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7826 "bison-chpl-lib.cpp"
    break;

  case 281: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2035 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7834 "bison-chpl-lib.cpp"
    break;

  case 282: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2039 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7842 "bison-chpl-lib.cpp"
    break;

  case 283: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2043 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7850 "bison-chpl-lib.cpp"
    break;

  case 284: /* catch_expr_ls: %empty  */
#line 2049 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7856 "bison-chpl-lib.cpp"
    break;

  case 285: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2050 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7862 "bison-chpl-lib.cpp"
    break;

  case 286: /* catch_expr: TCATCH block_stmt  */
#line 2055 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7870 "bison-chpl-lib.cpp"
    break;

  case 287: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2059 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7878 "bison-chpl-lib.cpp"
    break;

  case 288: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2063 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7886 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr_inner: ident_def  */
#line 2070 "chpl.ypp"
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
#line 7905 "bison-chpl-lib.cpp"
    break;

  case 290: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2085 "chpl.ypp"
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
#line 7924 "bison-chpl-lib.cpp"
    break;

  case 291: /* throw_stmt: TTHROW expr TSEMI  */
#line 2103 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7934 "bison-chpl-lib.cpp"
    break;

  case 292: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2111 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7943 "bison-chpl-lib.cpp"
    break;

  case 293: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2116 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7954 "bison-chpl-lib.cpp"
    break;

  case 294: /* when_stmt_ls: %empty  */
#line 2125 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7960 "bison-chpl-lib.cpp"
    break;

  case 295: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2126 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7966 "bison-chpl-lib.cpp"
    break;

  case 296: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2131 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7974 "bison-chpl-lib.cpp"
    break;

  case 297: /* when_stmt: TOTHERWISE stmt  */
#line 2135 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7983 "bison-chpl-lib.cpp"
    break;

  case 298: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2140 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 7993 "bison-chpl-lib.cpp"
    break;

  case 299: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2149 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8002 "bison-chpl-lib.cpp"
    break;

  case 300: /* manager_expr: expr TAS ident_def  */
#line 2154 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8010 "bison-chpl-lib.cpp"
    break;

  case 301: /* manager_expr: expr  */
#line 2158 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8018 "bison-chpl-lib.cpp"
    break;

  case 302: /* manager_expr_ls: manager_expr  */
#line 2164 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8024 "bison-chpl-lib.cpp"
    break;

  case 303: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2165 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8030 "bison-chpl-lib.cpp"
    break;

  case 304: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2170 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8039 "bison-chpl-lib.cpp"
    break;

  case 305: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2180 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8049 "bison-chpl-lib.cpp"
    break;

  case 306: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2186 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8061 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_start: class_tag ident_def  */
#line 2198 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8069 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_tag: TCLASS  */
#line 2204 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8075 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_tag: TRECORD  */
#line 2205 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8081 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_tag: TUNION  */
#line 2206 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8087 "bison-chpl-lib.cpp"
    break;

  case 311: /* opt_inherit: %empty  */
#line 2210 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8093 "bison-chpl-lib.cpp"
    break;

  case 312: /* opt_inherit: TCOLON expr_ls  */
#line 2211 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8099 "bison-chpl-lib.cpp"
    break;

  case 313: /* class_level_stmt_ls: %empty  */
#line 2215 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8108 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2220 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8116 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2224 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8124 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2228 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8132 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2232 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8140 "bison-chpl-lib.cpp"
    break;

  case 318: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
#line 2240 "chpl.ypp"
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
#line 8163 "bison-chpl-lib.cpp"
    break;

  case 319: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
#line 2259 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8177 "bison-chpl-lib.cpp"
    break;

  case 320: /* enum_header: TENUM ident_def  */
#line 2272 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8185 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_ls: enum_item  */
#line 2279 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8194 "bison-chpl-lib.cpp"
    break;

  case 322: /* enum_ls: enum_ls TCOMMA  */
#line 2284 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8204 "bison-chpl-lib.cpp"
    break;

  case 323: /* $@5: %empty  */
#line 2290 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8213 "bison-chpl-lib.cpp"
    break;

  case 324: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2295 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8222 "bison-chpl-lib.cpp"
    break;

  case 325: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2300 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8231 "bison-chpl-lib.cpp"
    break;

  case 326: /* $@6: %empty  */
#line 2305 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8240 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2310 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8249 "bison-chpl-lib.cpp"
    break;

  case 328: /* enum_item: ident_def  */
#line 2320 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[0])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8261 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_item: ident_def TASSIGN expr  */
#line 2328 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[-2])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8275 "bison-chpl-lib.cpp"
    break;

  case 330: /* lambda_decl_start: TLAMBDA  */
#line 2341 "chpl.ypp"
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
#line 8291 "bison-chpl-lib.cpp"
    break;

  case 331: /* $@7: %empty  */
#line 2358 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8301 "bison-chpl-lib.cpp"
    break;

  case 332: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2364 "chpl.ypp"
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
#line 8324 "bison-chpl-lib.cpp"
    break;

  case 334: /* linkage_spec: linkage_spec_empty  */
#line 2388 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8330 "bison-chpl-lib.cpp"
    break;

  case 335: /* linkage_spec: TINLINE  */
#line 2389 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8337 "bison-chpl-lib.cpp"
    break;

  case 336: /* linkage_spec: TOVERRIDE  */
#line 2391 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8344 "bison-chpl-lib.cpp"
    break;

  case 337: /* opt_fn_type_formal_ls: %empty  */
#line 2396 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8350 "bison-chpl-lib.cpp"
    break;

  case 338: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2397 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8356 "bison-chpl-lib.cpp"
    break;

  case 339: /* fn_type_formal_ls: fn_type_formal  */
#line 2401 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8362 "bison-chpl-lib.cpp"
    break;

  case 340: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2402 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8368 "bison-chpl-lib.cpp"
    break;

  case 341: /* fn_type_formal: named_formal  */
#line 2407 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8374 "bison-chpl-lib.cpp"
    break;

  case 342: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2410 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8380 "bison-chpl-lib.cpp"
    break;

  case 343: /* fn_type_formal: formal_type  */
#line 2412 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8386 "bison-chpl-lib.cpp"
    break;

  case 344: /* opt_fn_type_ret_type: %empty  */
#line 2416 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8392 "bison-chpl-lib.cpp"
    break;

  case 345: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2417 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8398 "bison-chpl-lib.cpp"
    break;

  case 346: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2430 "chpl.ypp"
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
#line 8418 "bison-chpl-lib.cpp"
    break;

  case 347: /* $@8: %empty  */
#line 2449 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8428 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2455 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8441 "bison-chpl-lib.cpp"
    break;

  case 349: /* $@9: %empty  */
#line 2464 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8451 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2470 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8464 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2482 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8472 "bison-chpl-lib.cpp"
    break;

  case 352: /* $@10: %empty  */
#line 2491 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8480 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2495 "chpl.ypp"
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
#line 8503 "bison-chpl-lib.cpp"
    break;

  case 354: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2517 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.thisIntentLoc = (yylsp[-2]);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8516 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2526 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.thisIntentLoc = (yylsp[-2]);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8529 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2535 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.thisIntentLoc = (yylsp[-4]);
      fp.receiver = context->buildThisFormal((yylsp[-3]), (yylsp[-4]), (yyvsp[-4].intentTag), (yyvsp[-3].expr), /*initExpr*/ nullptr);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8543 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2545 "chpl.ypp"
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

  case 358: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2555 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8567 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2564 "chpl.ypp"
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
#line 8583 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2580 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 8592 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_ident: ident_def TBANG  */
#line 2590 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8602 "bison-chpl-lib.cpp"
    break;

  case 406: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2649 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8608 "bison-chpl-lib.cpp"
    break;

  case 407: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2650 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8614 "bison-chpl-lib.cpp"
    break;

  case 408: /* opt_formal_ls: %empty  */
#line 2654 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8620 "bison-chpl-lib.cpp"
    break;

  case 409: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2655 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8626 "bison-chpl-lib.cpp"
    break;

  case 410: /* req_formal_ls: TLP TRP  */
#line 2659 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8632 "bison-chpl-lib.cpp"
    break;

  case 411: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2660 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8638 "bison-chpl-lib.cpp"
    break;

  case 412: /* formal_ls_inner: formal  */
#line 2664 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8644 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2665 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8650 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_ls: %empty  */
#line 2669 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8656 "bison-chpl-lib.cpp"
    break;

  case 415: /* formal_ls: formal_ls_inner  */
#line 2670 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8662 "bison-chpl-lib.cpp"
    break;

  case 417: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2679 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8670 "bison-chpl-lib.cpp"
    break;

  case 418: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2684 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8678 "bison-chpl-lib.cpp"
    break;

  case 419: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2689 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8686 "bison-chpl-lib.cpp"
    break;

  case 420: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2694 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8694 "bison-chpl-lib.cpp"
    break;

  case 421: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2699 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yylsp[-5]), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8702 "bison-chpl-lib.cpp"
    break;

  case 422: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2704 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8710 "bison-chpl-lib.cpp"
    break;

  case 423: /* opt_formal_intent_tag: %empty  */
#line 2710 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);
  }
#line 8719 "bison-chpl-lib.cpp"
    break;

  case 424: /* opt_formal_intent_tag: required_intent_tag  */
#line 2715 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8728 "bison-chpl-lib.cpp"
    break;

  case 425: /* required_intent_tag: TIN  */
#line 2722 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::IN);            }
#line 8734 "bison-chpl-lib.cpp"
    break;

  case 426: /* required_intent_tag: TINOUT  */
#line 2723 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::INOUT);         }
#line 8740 "bison-chpl-lib.cpp"
    break;

  case 427: /* required_intent_tag: TOUT  */
#line 2724 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::OUT);           }
#line 8746 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TCONST TIN  */
#line 2725 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_IN);      }
#line 8752 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TCONST TREF  */
#line 2726 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);     }
#line 8758 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TCONST  */
#line 2727 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);         }
#line 8764 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TPARAM  */
#line 2728 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);         }
#line 8770 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TREF  */
#line 2729 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);           }
#line 8776 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TTYPE  */
#line 2730 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);          }
#line 8782 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TVAR  */
#line 2731 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR); }
#line 8788 "bison-chpl-lib.cpp"
    break;

  case 435: /* opt_this_intent_tag: %empty  */
#line 2735 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);     }
#line 8794 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_this_intent_tag: TPARAM  */
#line 2736 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);              }
#line 8800 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: TREF  */
#line 2737 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);                }
#line 8806 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TCONST TREF  */
#line 2738 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);          }
#line 8812 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TCONST  */
#line 2739 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);              }
#line 8818 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TTYPE  */
#line 2740 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);               }
#line 8824 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TIN  */
#line 2741 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::IN);       }
#line 8830 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TCONST TIN  */
#line 2742 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::CONST_IN); }
#line 8836 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_this_intent_tag: TVAR  */
#line 2743 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR);      }
#line 8842 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_this_intent_tag: TOUT  */
#line 2744 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::OUT);      }
#line 8848 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_this_intent_tag: TINOUT  */
#line 2745 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::INOUT);    }
#line 8854 "bison-chpl-lib.cpp"
    break;

  case 446: /* proc_iter_or_op: TPROC  */
#line 2749 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8860 "bison-chpl-lib.cpp"
    break;

  case 447: /* proc_iter_or_op: TITER  */
#line 2750 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8866 "bison-chpl-lib.cpp"
    break;

  case 448: /* proc_iter_or_op: TOPERATOR  */
#line 2751 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8872 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: %empty  */
#line 2755 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::DEFAULT_RETURN_INTENT); }
#line 8878 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TOUT  */
#line 2756 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::OUT);                   }
#line 8884 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TCONST  */
#line 2757 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST);                 }
#line 8890 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TCONST TREF  */
#line 2758 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST_REF);             }
#line 8896 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_ret_tag: TREF  */
#line 2759 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::REF);                   }
#line 8902 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: TPARAM  */
#line 2760 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::PARAM);                 }
#line 8908 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: TTYPE  */
#line 2761 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::TYPE);                  }
#line 8914 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: TIN  */
#line 2762 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::IN);            }
#line 8920 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_ret_tag: TCONST TIN  */
#line 2763 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::CONST_IN);      }
#line 8926 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_ret_tag: TVAR  */
#line 2764 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::VAR);           }
#line 8932 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_ret_tag: TINOUT  */
#line 2765 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::INOUT);         }
#line 8938 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_throws_error: %empty  */
#line 2769 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8944 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_throws_error: TTHROWS  */
#line 2770 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8950 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_function_body_stmt: TSEMI  */
#line 2773 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8956 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_function_body_stmt: function_body_stmt  */
#line 2774 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8962 "bison-chpl-lib.cpp"
    break;

  case 464: /* function_body_stmt: block_stmt_body  */
#line 2778 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8968 "bison-chpl-lib.cpp"
    break;

  case 465: /* function_body_stmt: TDO toplevel_stmt  */
#line 2779 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 8974 "bison-chpl-lib.cpp"
    break;

  case 466: /* query_expr: TQUERIEDIDENT  */
#line 2783 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8980 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_lifetime_where: %empty  */
#line 2788 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8986 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_lifetime_where: TWHERE expr  */
#line 2790 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8992 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2792 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8998 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2794 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9004 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2796 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9010 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_components_expr: lifetime_expr  */
#line 2801 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9016 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2803 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9022 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2808 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9028 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2810 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9034 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2812 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9040 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2814 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9046 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2816 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9052 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2818 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9058 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2820 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9064 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_ident: TIDENT  */
#line 2824 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9070 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_ident: TINIT  */
#line 2825 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9076 "bison-chpl-lib.cpp"
    break;

  case 483: /* lifetime_ident: TTHIS  */
#line 2826 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9082 "bison-chpl-lib.cpp"
    break;

  case 484: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2831 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9091 "bison-chpl-lib.cpp"
    break;

  case 485: /* type_alias_decl_stmt_start: TTYPE  */
#line 2840 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9101 "bison-chpl-lib.cpp"
    break;

  case 486: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2846 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9112 "bison-chpl-lib.cpp"
    break;

  case 487: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2856 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9120 "bison-chpl-lib.cpp"
    break;

  case 488: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2860 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9128 "bison-chpl-lib.cpp"
    break;

  case 489: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2867 "chpl.ypp"
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
#line 9151 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_init_type: %empty  */
#line 2888 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9157 "bison-chpl-lib.cpp"
    break;

  case 491: /* opt_init_type: TASSIGN expr  */
#line 2890 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9163 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_type: TPARAM  */
#line 2894 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9169 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_type: TCONST TREF  */
#line 2895 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9175 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_type: TREF  */
#line 2896 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9181 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_type: TCONST  */
#line 2897 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9187 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_type: TVAR  */
#line 2898 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9193 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_start: var_decl_type  */
#line 2903 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9202 "bison-chpl-lib.cpp"
    break;

  case 498: /* $@11: %empty  */
#line 2911 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9211 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt: TCONFIG $@11 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2915 "chpl.ypp"
                                              {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-4]), (yylsp[-3])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9220 "bison-chpl-lib.cpp"
    break;

  case 500: /* $@12: %empty  */
#line 2920 "chpl.ypp"
  {
    // Use a mid-rule action to preserve the 'on' expr
    context->noteVarDestinationExpr((yyvsp[0].expr));
  }
#line 9229 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt: TON expr $@12 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2925 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-4])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9238 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt: var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2930 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9247 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt: var_decl_start error TSEMI  */
#line 2935 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    delete (yyvsp[-2].attributeGroup);
    context->resetDeclStateOnError();
  }
#line 9257 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2944 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9265 "bison-chpl-lib.cpp"
    break;

  case 505: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2948 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9273 "bison-chpl-lib.cpp"
    break;

  case 506: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2955 "chpl.ypp"
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
#line 9296 "bison-chpl-lib.cpp"
    break;

  case 507: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2974 "chpl.ypp"
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
#line 9316 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_var_decl_component: ident_def  */
#line 2993 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9324 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2997 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9332 "bison-chpl-lib.cpp"
    break;

  case 510: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 3004 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9338 "bison-chpl-lib.cpp"
    break;

  case 511: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 3006 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9344 "bison-chpl-lib.cpp"
    break;

  case 512: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 3008 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9350 "bison-chpl-lib.cpp"
    break;

  case 513: /* opt_init_expr: %empty  */
#line 3014 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9356 "bison-chpl-lib.cpp"
    break;

  case 514: /* opt_init_expr: TASSIGN TNOINIT  */
#line 3015 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9362 "bison-chpl-lib.cpp"
    break;

  case 515: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 3016 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9368 "bison-chpl-lib.cpp"
    break;

  case 516: /* formal_or_ret_type_expr: expr  */
#line 3020 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9374 "bison-chpl-lib.cpp"
    break;

  case 517: /* ret_type: formal_or_ret_type_expr  */
#line 3024 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9380 "bison-chpl-lib.cpp"
    break;

  case 518: /* ret_type: reserved_type_ident_use  */
#line 3025 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9386 "bison-chpl-lib.cpp"
    break;

  case 519: /* ret_type: error  */
#line 3026 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9392 "bison-chpl-lib.cpp"
    break;

  case 520: /* colon_ret_type: TCOLON ret_type  */
#line 3030 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9398 "bison-chpl-lib.cpp"
    break;

  case 521: /* colon_ret_type: error  */
#line 3031 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9404 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_ret_type: %empty  */
#line 3035 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9410 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_type: %empty  */
#line 3040 "chpl.ypp"
                                 { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9416 "bison-chpl-lib.cpp"
    break;

  case 525: /* opt_type: TCOLON expr  */
#line 3041 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9422 "bison-chpl-lib.cpp"
    break;

  case 526: /* opt_type: TCOLON reserved_type_ident_use  */
#line 3042 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9428 "bison-chpl-lib.cpp"
    break;

  case 527: /* opt_type: error  */
#line 3043 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9434 "bison-chpl-lib.cpp"
    break;

  case 528: /* formal_type: formal_or_ret_type_expr  */
#line 3047 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9440 "bison-chpl-lib.cpp"
    break;

  case 529: /* formal_type: reserved_type_ident_use  */
#line 3048 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9446 "bison-chpl-lib.cpp"
    break;

  case 530: /* colon_formal_type: TCOLON formal_type  */
#line 3052 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9452 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_colon_formal_type: %empty  */
#line 3056 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9458 "bison-chpl-lib.cpp"
    break;

  case 532: /* opt_colon_formal_type: colon_formal_type  */
#line 3057 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9464 "bison-chpl-lib.cpp"
    break;

  case 533: /* expr_ls: expr  */
#line 3063 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9470 "bison-chpl-lib.cpp"
    break;

  case 534: /* expr_ls: expr_ls TCOMMA expr  */
#line 3064 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9476 "bison-chpl-lib.cpp"
    break;

  case 535: /* expr_ls_end_semi: expr_ls semicolon_list  */
#line 3068 "chpl.ypp"
                         {
    (yyval.ndArrayList) = context->makeNDArrayList();
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElement((yyvsp[-1].exprList), (yylsp[-1])));
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9486 "bison-chpl-lib.cpp"
    break;

  case 536: /* expr_ls_end_semi: expr_ls TCOMMA semicolon_list  */
#line 3073 "chpl.ypp"
                                {
    (yyval.ndArrayList) = context->makeNDArrayList();
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElement((yyvsp[-2].exprList), (yylsp[-2])));
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9496 "bison-chpl-lib.cpp"
    break;

  case 537: /* expr_ls_end_semi: expr_ls_semi semicolon_list  */
#line 3079 "chpl.ypp"
  {
    (yyval.ndArrayList) = (yyvsp[-1].ndArrayList);
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9505 "bison-chpl-lib.cpp"
    break;

  case 538: /* expr_ls_semi: expr_ls_end_semi expr_ls  */
#line 3086 "chpl.ypp"
                           {
    (yyval.ndArrayList) = context->appendNDArrayList((yyvsp[-1].ndArrayList), makeNDArrayElement((yyvsp[0].exprList), (yylsp[0])));
  }
#line 9513 "bison-chpl-lib.cpp"
    break;

  case 539: /* expr_ls_semi: expr_ls_end_semi expr_ls TCOMMA  */
#line 3089 "chpl.ypp"
                                  {
    (yyval.ndArrayList) = context->appendNDArrayList((yyvsp[-2].ndArrayList), makeNDArrayElement((yyvsp[-1].exprList), (yylsp[-1])));
  }
#line 9521 "bison-chpl-lib.cpp"
    break;

  case 540: /* semicolon_list: TSEMI  */
#line 3095 "chpl.ypp"
                       { (yyval.counter) = 1; }
#line 9527 "bison-chpl-lib.cpp"
    break;

  case 541: /* semicolon_list: semicolon_list TSEMI  */
#line 3096 "chpl.ypp"
                       { (yyval.counter) = (yyvsp[-1].counter) + 1; }
#line 9533 "bison-chpl-lib.cpp"
    break;

  case 542: /* tuple_component: opt_try_expr  */
#line 3102 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9539 "bison-chpl-lib.cpp"
    break;

  case 543: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3107 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9547 "bison-chpl-lib.cpp"
    break;

  case 544: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3111 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9555 "bison-chpl-lib.cpp"
    break;

  case 545: /* opt_actual_ls: %empty  */
#line 3117 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9561 "bison-chpl-lib.cpp"
    break;

  case 546: /* opt_actual_ls: actual_ls  */
#line 3118 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9567 "bison-chpl-lib.cpp"
    break;

  case 547: /* actual_ls: actual_expr  */
#line 3123 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9576 "bison-chpl-lib.cpp"
    break;

  case 548: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3128 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9586 "bison-chpl-lib.cpp"
    break;

  case 549: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3136 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9592 "bison-chpl-lib.cpp"
    break;

  case 550: /* actual_expr: opt_try_expr  */
#line 3137 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9598 "bison-chpl-lib.cpp"
    break;

  case 551: /* ident_expr: ident_use  */
#line 3141 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9604 "bison-chpl-lib.cpp"
    break;

  case 552: /* ident_expr: scalar_type  */
#line 3142 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9610 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3155 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9616 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3157 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9622 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3159 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9628 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3161 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9638 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TATOMIC expr  */
#line 3167 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9644 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TSYNC expr  */
#line 3169 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9650 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TOWNED  */
#line 3172 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9656 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TOWNED expr  */
#line 3174 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9662 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TUNMANAGED  */
#line 3176 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9668 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3178 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9674 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TSHARED  */
#line 3180 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9680 "bison-chpl-lib.cpp"
    break;

  case 569: /* sub_type_level_expr: TSHARED expr  */
#line 3182 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9686 "bison-chpl-lib.cpp"
    break;

  case 570: /* sub_type_level_expr: TBORROWED  */
#line 3184 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9692 "bison-chpl-lib.cpp"
    break;

  case 571: /* sub_type_level_expr: TBORROWED expr  */
#line 3186 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9698 "bison-chpl-lib.cpp"
    break;

  case 572: /* sub_type_level_expr: TCLASS  */
#line 3188 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9704 "bison-chpl-lib.cpp"
    break;

  case 573: /* sub_type_level_expr: TRECORD  */
#line 3190 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9710 "bison-chpl-lib.cpp"
    break;

  case 574: /* for_expr: no_loop_attributes for_expr_base  */
#line 3194 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9716 "bison-chpl-lib.cpp"
    break;

  case 575: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3199 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9724 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3203 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9732 "bison-chpl-lib.cpp"
    break;

  case 577: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3207 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9740 "bison-chpl-lib.cpp"
    break;

  case 578: /* for_expr_base: for_loop_kw zippered_iterator TDO expr  */
#line 3211 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "expression-level loops with 'zip' must have an index.");
  }
#line 9748 "bison-chpl-lib.cpp"
    break;

  case 579: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3215 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9761 "bison-chpl-lib.cpp"
    break;

  case 580: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3224 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9774 "bison-chpl-lib.cpp"
    break;

  case 581: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3233 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9787 "bison-chpl-lib.cpp"
    break;

  case 582: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3244 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9793 "bison-chpl-lib.cpp"
    break;

  case 583: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3249 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9801 "bison-chpl-lib.cpp"
    break;

  case 584: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3253 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9809 "bison-chpl-lib.cpp"
    break;

  case 585: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3257 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9817 "bison-chpl-lib.cpp"
    break;

  case 586: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3261 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9825 "bison-chpl-lib.cpp"
    break;

  case 587: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3265 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9833 "bison-chpl-lib.cpp"
    break;

  case 588: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3269 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9841 "bison-chpl-lib.cpp"
    break;

  case 589: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3273 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9849 "bison-chpl-lib.cpp"
    break;

  case 590: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3280 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9863 "bison-chpl-lib.cpp"
    break;

  case 591: /* nil_expr: TNIL  */
#line 3297 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9869 "bison-chpl-lib.cpp"
    break;

  case 599: /* opt_task_intent_ls: %empty  */
#line 3315 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9875 "bison-chpl-lib.cpp"
    break;

  case 600: /* opt_task_intent_ls: task_intent_clause  */
#line 3316 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9881 "bison-chpl-lib.cpp"
    break;

  case 601: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3321 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9889 "bison-chpl-lib.cpp"
    break;

  case 602: /* task_intent_clause: TWITH TLP TRP  */
#line 3325 "chpl.ypp"
  {
    auto err = context->syntax((yyloc), "'with' clause cannot be empty.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-2]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9900 "bison-chpl-lib.cpp"
    break;

  case 603: /* task_intent_clause: TWITH TLP error TRP  */
#line 3332 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[-2]), "invalid intent expression in 'with' clause.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9911 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_intent_clause: TWITH error  */
#line 3339 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[0]), "missing parentheses around 'with' clause intents.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-1]), dummyLoc, dummyLoc, dummyLoc, exprs);
  }
#line 9922 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_intent_ls: intent_expr  */
#line 3348 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9928 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3349 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9934 "bison-chpl-lib.cpp"
    break;

  case 607: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3354 "chpl.ypp"
  {
    (yyval.expr) = context->buildTaskIntent((yyloc), (yylsp[-2]), nullptr, (yyvsp[-2].expr), (yyvsp[-3].taskIntent), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9942 "bison-chpl-lib.cpp"
    break;

  case 608: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3358 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9950 "bison-chpl-lib.cpp"
    break;

  case 609: /* intent_expr: expr TREDUCE ident_expr  */
#line 3362 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9958 "bison-chpl-lib.cpp"
    break;

  case 610: /* task_var_prefix: TCONST  */
#line 3368 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST);          }
#line 9964 "bison-chpl-lib.cpp"
    break;

  case 611: /* task_var_prefix: TIN  */
#line 3369 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::IN);             }
#line 9970 "bison-chpl-lib.cpp"
    break;

  case 612: /* task_var_prefix: TCONST TIN  */
#line 3370 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_IN);       }
#line 9976 "bison-chpl-lib.cpp"
    break;

  case 613: /* task_var_prefix: TREF  */
#line 3371 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::REF);            }
#line 9982 "bison-chpl-lib.cpp"
    break;

  case 614: /* task_var_prefix: TCONST TREF  */
#line 3372 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_REF);      }
#line 9988 "bison-chpl-lib.cpp"
    break;

  case 615: /* task_var_prefix: TVAR  */
#line 3373 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::VAR);            }
#line 9994 "bison-chpl-lib.cpp"
    break;

  case 616: /* task_var_prefix: TOUT  */
#line 3374 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::OUT);   }
#line 10000 "bison-chpl-lib.cpp"
    break;

  case 617: /* task_var_prefix: TINOUT  */
#line 3375 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::INOUT); }
#line 10006 "bison-chpl-lib.cpp"
    break;

  case 618: /* task_var_prefix: TPARAM  */
#line 3376 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::PARAM); }
#line 10012 "bison-chpl-lib.cpp"
    break;

  case 619: /* task_var_prefix: TTYPE  */
#line 3377 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::TYPE);  }
#line 10018 "bison-chpl-lib.cpp"
    break;

  case 620: /* new_maybe_decorated: TNEW  */
#line 3382 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10024 "bison-chpl-lib.cpp"
    break;

  case 621: /* new_maybe_decorated: TNEW TOWNED  */
#line 3384 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10030 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_maybe_decorated: TNEW TSHARED  */
#line 3386 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10036 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3388 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10042 "bison-chpl-lib.cpp"
    break;

  case 624: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3390 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10048 "bison-chpl-lib.cpp"
    break;

  case 625: /* new_expr: new_maybe_decorated expr  */
#line 3396 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10056 "bison-chpl-lib.cpp"
    break;

  case 626: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3403 "chpl.ypp"
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
#line 10073 "bison-chpl-lib.cpp"
    break;

  case 627: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3416 "chpl.ypp"
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
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 628: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3429 "chpl.ypp"
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
#line 10108 "bison-chpl-lib.cpp"
    break;

  case 629: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3443 "chpl.ypp"
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
#line 10127 "bison-chpl-lib.cpp"
    break;

  case 630: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3461 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10135 "bison-chpl-lib.cpp"
    break;

  case 631: /* range_literal_expr: expr TDOTDOT expr  */
#line 3468 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10144 "bison-chpl-lib.cpp"
    break;

  case 632: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3473 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10153 "bison-chpl-lib.cpp"
    break;

  case 633: /* range_literal_expr: expr TDOTDOT  */
#line 3478 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10162 "bison-chpl-lib.cpp"
    break;

  case 634: /* range_literal_expr: TDOTDOT expr  */
#line 3483 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10172 "bison-chpl-lib.cpp"
    break;

  case 635: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3489 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10182 "bison-chpl-lib.cpp"
    break;

  case 636: /* range_literal_expr: TDOTDOT  */
#line 3495 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10192 "bison-chpl-lib.cpp"
    break;

  case 637: /* cast_expr: expr TCOLON expr  */
#line 3525 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10200 "bison-chpl-lib.cpp"
    break;

  case 638: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3532 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10208 "bison-chpl-lib.cpp"
    break;

  case 639: /* super_expr: fn_expr  */
#line 3538 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10214 "bison-chpl-lib.cpp"
    break;

  case 643: /* expr: sub_type_level_expr TQUESTION  */
#line 3547 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10220 "bison-chpl-lib.cpp"
    break;

  case 644: /* expr: TQUESTION  */
#line 3549 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10226 "bison-chpl-lib.cpp"
    break;

  case 648: /* expr: fn_type  */
#line 3554 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10232 "bison-chpl-lib.cpp"
    break;

  case 658: /* opt_expr: %empty  */
#line 3568 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10238 "bison-chpl-lib.cpp"
    break;

  case 659: /* opt_expr: expr  */
#line 3569 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10244 "bison-chpl-lib.cpp"
    break;

  case 660: /* opt_try_expr: TTRY expr  */
#line 3573 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10250 "bison-chpl-lib.cpp"
    break;

  case 661: /* opt_try_expr: TTRYBANG expr  */
#line 3574 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10256 "bison-chpl-lib.cpp"
    break;

  case 662: /* opt_try_expr: super_expr  */
#line 3575 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10262 "bison-chpl-lib.cpp"
    break;

  case 668: /* call_base_expr: expr TBANG  */
#line 3592 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10270 "bison-chpl-lib.cpp"
    break;

  case 669: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3595 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10276 "bison-chpl-lib.cpp"
    break;

  case 672: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3602 "chpl.ypp"
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
#line 10292 "bison-chpl-lib.cpp"
    break;

  case 673: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3614 "chpl.ypp"
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
#line 10308 "bison-chpl-lib.cpp"
    break;

  case 674: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3626 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10316 "bison-chpl-lib.cpp"
    break;

  case 675: /* dot_expr: expr TDOT ident_use  */
#line 3633 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10322 "bison-chpl-lib.cpp"
    break;

  case 676: /* dot_expr: expr TDOT TTYPE  */
#line 3635 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10328 "bison-chpl-lib.cpp"
    break;

  case 677: /* dot_expr: expr TDOT TDOMAIN  */
#line 3637 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10334 "bison-chpl-lib.cpp"
    break;

  case 678: /* dot_expr: expr TDOT TLOCALE  */
#line 3639 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10340 "bison-chpl-lib.cpp"
    break;

  case 679: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3641 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10346 "bison-chpl-lib.cpp"
    break;

  case 680: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3643 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10352 "bison-chpl-lib.cpp"
    break;

  case 681: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3652 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10362 "bison-chpl-lib.cpp"
    break;

  case 682: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3658 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10370 "bison-chpl-lib.cpp"
    break;

  case 683: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3662 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10378 "bison-chpl-lib.cpp"
    break;

  case 684: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3666 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10386 "bison-chpl-lib.cpp"
    break;

  case 685: /* bool_literal: TFALSE  */
#line 3672 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10392 "bison-chpl-lib.cpp"
    break;

  case 686: /* bool_literal: TTRUE  */
#line 3673 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10398 "bison-chpl-lib.cpp"
    break;

  case 687: /* str_bytes_literal: STRINGLITERAL  */
#line 3677 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10404 "bison-chpl-lib.cpp"
    break;

  case 688: /* str_bytes_literal: BYTESLITERAL  */
#line 3678 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10410 "bison-chpl-lib.cpp"
    break;

  case 691: /* literal_expr: INTLITERAL  */
#line 3684 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10416 "bison-chpl-lib.cpp"
    break;

  case 692: /* literal_expr: REALLITERAL  */
#line 3685 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10422 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: IMAGLITERAL  */
#line 3686 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10428 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: TNONE  */
#line 3687 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10434 "bison-chpl-lib.cpp"
    break;

  case 695: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3689 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10443 "bison-chpl-lib.cpp"
    break;

  case 696: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3694 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10452 "bison-chpl-lib.cpp"
    break;

  case 697: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3699 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10461 "bison-chpl-lib.cpp"
    break;

  case 698: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3704 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10470 "bison-chpl-lib.cpp"
    break;

  case 699: /* literal_expr: no_loop_attributes TLSBR expr_ls_semi TRSBR  */
#line 3709 "chpl.ypp"
  {
    (yyval.expr) = context->buildNDArray((yyloc), (yyvsp[-1].ndArrayList));
    context->popLoopAttributeGroup();
  }
#line 10479 "bison-chpl-lib.cpp"
    break;

  case 700: /* literal_expr: no_loop_attributes TLSBR expr_ls_semi semicolon_list TRSBR  */
#line 3714 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    (yyval.expr) = context->buildNDArray((yyloc), (yyvsp[-2].ndArrayList));
    context->popLoopAttributeGroup();
  }
#line 10491 "bison-chpl-lib.cpp"
    break;

  case 701: /* literal_expr: no_loop_attributes TLSBR expr_ls semicolon_list TRSBR  */
#line 3722 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    AstList lst;
    lst.push_back(ArrayRow::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))));
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), std::move(lst)).release();
    context->popLoopAttributeGroup();
  }
#line 10505 "bison-chpl-lib.cpp"
    break;

  case 702: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA semicolon_list TRSBR  */
#line 3732 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    AstList lst;
    lst.push_back(ArrayRow::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-3].exprList))));
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), std::move(lst)).release();
    context->popLoopAttributeGroup();
  }
#line 10519 "bison-chpl-lib.cpp"
    break;

  case 703: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3742 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10528 "bison-chpl-lib.cpp"
    break;

  case 704: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3747 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10537 "bison-chpl-lib.cpp"
    break;

  case 705: /* assoc_expr_ls: expr TALIAS expr  */
#line 3756 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10546 "bison-chpl-lib.cpp"
    break;

  case 706: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3761 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10556 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TPLUS expr  */
#line 3769 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10562 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TMINUS expr  */
#line 3770 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10568 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TSTAR expr  */
#line 3771 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10574 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TDIVIDE expr  */
#line 3772 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10580 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3773 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10586 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3774 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10592 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TMOD expr  */
#line 3775 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10598 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TEQUAL expr  */
#line 3776 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10604 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3777 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10610 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3778 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10616 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3779 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10622 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TLESS expr  */
#line 3780 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10628 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TGREATER expr  */
#line 3781 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10634 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TBAND expr  */
#line 3782 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10640 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr TBOR expr  */
#line 3783 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10646 "bison-chpl-lib.cpp"
    break;

  case 722: /* binary_op_expr: expr TBXOR expr  */
#line 3784 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10652 "bison-chpl-lib.cpp"
    break;

  case 723: /* binary_op_expr: expr TAND expr  */
#line 3785 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10658 "bison-chpl-lib.cpp"
    break;

  case 724: /* binary_op_expr: expr TOR expr  */
#line 3786 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10664 "bison-chpl-lib.cpp"
    break;

  case 725: /* binary_op_expr: expr TEXP expr  */
#line 3787 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10670 "bison-chpl-lib.cpp"
    break;

  case 726: /* binary_op_expr: expr TBY expr  */
#line 3788 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10676 "bison-chpl-lib.cpp"
    break;

  case 727: /* binary_op_expr: expr TALIGN expr  */
#line 3789 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10682 "bison-chpl-lib.cpp"
    break;

  case 728: /* binary_op_expr: expr THASH expr  */
#line 3790 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10688 "bison-chpl-lib.cpp"
    break;

  case 729: /* binary_op_expr: expr TDMAPPED expr  */
#line 3791 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10694 "bison-chpl-lib.cpp"
    break;

  case 730: /* unary_op_expr: TPLUS expr  */
#line 3795 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10700 "bison-chpl-lib.cpp"
    break;

  case 731: /* unary_op_expr: TMINUS expr  */
#line 3796 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10706 "bison-chpl-lib.cpp"
    break;

  case 732: /* unary_op_expr: TMINUSMINUS expr  */
#line 3797 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10712 "bison-chpl-lib.cpp"
    break;

  case 733: /* unary_op_expr: TPLUSPLUS expr  */
#line 3798 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10718 "bison-chpl-lib.cpp"
    break;

  case 734: /* unary_op_expr: TBANG expr  */
#line 3799 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10724 "bison-chpl-lib.cpp"
    break;

  case 735: /* unary_op_expr: expr TBANG  */
#line 3800 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10732 "bison-chpl-lib.cpp"
    break;

  case 736: /* unary_op_expr: TBNOT expr  */
#line 3803 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10738 "bison-chpl-lib.cpp"
    break;

  case 737: /* reduce_expr: expr TREDUCE expr  */
#line 3808 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10746 "bison-chpl-lib.cpp"
    break;

  case 738: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3812 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10754 "bison-chpl-lib.cpp"
    break;

  case 739: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3816 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10762 "bison-chpl-lib.cpp"
    break;

  case 740: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3820 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10770 "bison-chpl-lib.cpp"
    break;

  case 741: /* scan_expr: expr TSCAN expr  */
#line 3827 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10778 "bison-chpl-lib.cpp"
    break;

  case 742: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3831 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10786 "bison-chpl-lib.cpp"
    break;

  case 743: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3835 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10794 "bison-chpl-lib.cpp"
    break;

  case 744: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3839 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10802 "bison-chpl-lib.cpp"
    break;


#line 10806 "bison-chpl-lib.cpp"

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
