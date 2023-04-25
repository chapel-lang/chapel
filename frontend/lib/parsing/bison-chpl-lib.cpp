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
  YYSYMBOL_INTLITERAL = 5,                 /* INTLITERAL  */
  YYSYMBOL_REALLITERAL = 6,                /* REALLITERAL  */
  YYSYMBOL_IMAGLITERAL = 7,                /* IMAGLITERAL  */
  YYSYMBOL_STRINGLITERAL = 8,              /* STRINGLITERAL  */
  YYSYMBOL_BYTESLITERAL = 9,               /* BYTESLITERAL  */
  YYSYMBOL_CSTRINGLITERAL = 10,            /* CSTRINGLITERAL  */
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
  YYSYMBOL_TINITEQUALS = 54,               /* TINITEQUALS  */
  YYSYMBOL_TIMPLEMENTS = 55,               /* TIMPLEMENTS  */
  YYSYMBOL_TINTERFACE = 56,                /* TINTERFACE  */
  YYSYMBOL_TLABEL = 57,                    /* TLABEL  */
  YYSYMBOL_TLAMBDA = 58,                   /* TLAMBDA  */
  YYSYMBOL_TLET = 59,                      /* TLET  */
  YYSYMBOL_TLIFETIME = 60,                 /* TLIFETIME  */
  YYSYMBOL_TLOCAL = 61,                    /* TLOCAL  */
  YYSYMBOL_TLOCALE = 62,                   /* TLOCALE  */
  YYSYMBOL_TMANAGE = 63,                   /* TMANAGE  */
  YYSYMBOL_TMINUSMINUS = 64,               /* TMINUSMINUS  */
  YYSYMBOL_TMODULE = 65,                   /* TMODULE  */
  YYSYMBOL_TNEW = 66,                      /* TNEW  */
  YYSYMBOL_TNIL = 67,                      /* TNIL  */
  YYSYMBOL_TNOINIT = 68,                   /* TNOINIT  */
  YYSYMBOL_TNONE = 69,                     /* TNONE  */
  YYSYMBOL_TNOTHING = 70,                  /* TNOTHING  */
  YYSYMBOL_TON = 71,                       /* TON  */
  YYSYMBOL_TONLY = 72,                     /* TONLY  */
  YYSYMBOL_TOPERATOR = 73,                 /* TOPERATOR  */
  YYSYMBOL_TOTHERWISE = 74,                /* TOTHERWISE  */
  YYSYMBOL_TOUT = 75,                      /* TOUT  */
  YYSYMBOL_TOVERRIDE = 76,                 /* TOVERRIDE  */
  YYSYMBOL_TOWNED = 77,                    /* TOWNED  */
  YYSYMBOL_TPARAM = 78,                    /* TPARAM  */
  YYSYMBOL_TPLUSPLUS = 79,                 /* TPLUSPLUS  */
  YYSYMBOL_TPRAGMA = 80,                   /* TPRAGMA  */
  YYSYMBOL_TPRIMITIVE = 81,                /* TPRIMITIVE  */
  YYSYMBOL_TPRIVATE = 82,                  /* TPRIVATE  */
  YYSYMBOL_TPROC = 83,                     /* TPROC  */
  YYSYMBOL_TPROTOTYPE = 84,                /* TPROTOTYPE  */
  YYSYMBOL_TPUBLIC = 85,                   /* TPUBLIC  */
  YYSYMBOL_TPROCLP = 86,                   /* TPROCLP  */
  YYSYMBOL_TREAL = 87,                     /* TREAL  */
  YYSYMBOL_TRECORD = 88,                   /* TRECORD  */
  YYSYMBOL_TREDUCE = 89,                   /* TREDUCE  */
  YYSYMBOL_TREF = 90,                      /* TREF  */
  YYSYMBOL_TREQUIRE = 91,                  /* TREQUIRE  */
  YYSYMBOL_TRETURN = 92,                   /* TRETURN  */
  YYSYMBOL_TSCAN = 93,                     /* TSCAN  */
  YYSYMBOL_TSELECT = 94,                   /* TSELECT  */
  YYSYMBOL_TSERIAL = 95,                   /* TSERIAL  */
  YYSYMBOL_TSHARED = 96,                   /* TSHARED  */
  YYSYMBOL_TSINGLE = 97,                   /* TSINGLE  */
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
  YYSYMBOL_TUNDERSCORE = 111,              /* TUNDERSCORE  */
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
  YYSYMBOL_import_expr = 212,              /* import_expr  */
  YYSYMBOL_import_ls = 213,                /* import_ls  */
  YYSYMBOL_require_stmt = 214,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 215,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 216,          /* opt_label_ident  */
  YYSYMBOL_reserved_word_ident = 217,      /* reserved_word_ident  */
  YYSYMBOL_ident_def = 218,                /* ident_def  */
  YYSYMBOL_ident_use = 219,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 220,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 221,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 222,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 223,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 224,              /* return_stmt  */
  YYSYMBOL_class_level_stmt = 225,         /* class_level_stmt  */
  YYSYMBOL_226_1 = 226,                    /* $@1  */
  YYSYMBOL_227_2 = 227,                    /* $@2  */
  YYSYMBOL_inner_class_level_stmt = 228,   /* inner_class_level_stmt  */
  YYSYMBOL_forwarding_decl_stmt = 229,     /* forwarding_decl_stmt  */
  YYSYMBOL_forwarding_decl_start = 230,    /* forwarding_decl_start  */
  YYSYMBOL_extern_or_export = 231,         /* extern_or_export  */
  YYSYMBOL_extern_export_decl_stmt_start = 232, /* extern_export_decl_stmt_start  */
  YYSYMBOL_extern_export_decl_stmt = 233,  /* extern_export_decl_stmt  */
  YYSYMBOL_234_3 = 234,                    /* $@3  */
  YYSYMBOL_235_4 = 235,                    /* $@4  */
  YYSYMBOL_extern_block_stmt = 236,        /* extern_block_stmt  */
  YYSYMBOL_loop_stmt = 237,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 238,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 239,                  /* if_stmt  */
  YYSYMBOL_ifvar = 240,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 241,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 242,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 243,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 244,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 245, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 246,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 247,           /* ifc_constraint  */
  YYSYMBOL_try_stmt = 248,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 249,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 250,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 251,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 252,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 253,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 254,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 255,                /* when_stmt  */
  YYSYMBOL_manager_expr = 256,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 257,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 258,              /* manage_stmt  */
  YYSYMBOL_class_decl_stmt = 259,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 260,              /* class_start  */
  YYSYMBOL_class_tag = 261,                /* class_tag  */
  YYSYMBOL_opt_inherit = 262,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 263,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 264,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header_lcbr = 265,         /* enum_header_lcbr  */
  YYSYMBOL_enum_ls = 266,                  /* enum_ls  */
  YYSYMBOL_267_5 = 267,                    /* $@5  */
  YYSYMBOL_268_6 = 268,                    /* $@6  */
  YYSYMBOL_enum_item = 269,                /* enum_item  */
  YYSYMBOL_lambda_decl_start = 270,        /* lambda_decl_start  */
  YYSYMBOL_lambda_decl_expr = 271,         /* lambda_decl_expr  */
  YYSYMBOL_272_7 = 272,                    /* $@7  */
  YYSYMBOL_linkage_spec_empty = 273,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 274,             /* linkage_spec  */
  YYSYMBOL_opt_fn_type_formal_ls = 275,    /* opt_fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal_ls = 276,        /* fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal = 277,           /* fn_type_formal  */
  YYSYMBOL_opt_fn_type_ret_type = 278,     /* opt_fn_type_ret_type  */
  YYSYMBOL_fn_type = 279,                  /* fn_type  */
  YYSYMBOL_fn_expr = 280,                  /* fn_expr  */
  YYSYMBOL_281_8 = 281,                    /* $@8  */
  YYSYMBOL_282_9 = 282,                    /* $@9  */
  YYSYMBOL_fn_decl_stmt_complete = 283,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 284,             /* fn_decl_stmt  */
  YYSYMBOL_285_10 = 285,                   /* $@10  */
  YYSYMBOL_fn_decl_stmt_inner = 286,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 287,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 288,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 289,                 /* fn_ident  */
  YYSYMBOL_op_ident = 290,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 291,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 292,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 293,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 294,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 295,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 296,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 297,                /* formal_ls  */
  YYSYMBOL_formal = 298,                   /* formal  */
  YYSYMBOL_formal_ident_def = 299,         /* formal_ident_def  */
  YYSYMBOL_named_formal = 300,             /* named_formal  */
  YYSYMBOL_opt_formal_intent_tag = 301,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 302,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 303,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 304,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 305,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 306,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 307,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 308,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 309,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 310,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 311, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 312,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 313,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 314,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 315, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 316, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 317, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 318,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 319,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 320,            /* var_decl_stmt  */
  YYSYMBOL_321_11 = 321,                   /* $@11  */
  YYSYMBOL_var_decl_stmt_inner_ls = 322,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 323,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 324, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 325, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 326,            /* opt_init_expr  */
  YYSYMBOL_formal_or_ret_type_expr = 327,  /* formal_or_ret_type_expr  */
  YYSYMBOL_ret_type = 328,                 /* ret_type  */
  YYSYMBOL_colon_ret_type = 329,           /* colon_ret_type  */
  YYSYMBOL_opt_ret_type = 330,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 331,                 /* opt_type  */
  YYSYMBOL_formal_type = 332,              /* formal_type  */
  YYSYMBOL_colon_formal_type = 333,        /* colon_formal_type  */
  YYSYMBOL_opt_colon_formal_type = 334,    /* opt_colon_formal_type  */
  YYSYMBOL_expr_ls = 335,                  /* expr_ls  */
  YYSYMBOL_tuple_component = 336,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 337,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 338,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 339,                /* actual_ls  */
  YYSYMBOL_actual_expr = 340,              /* actual_expr  */
  YYSYMBOL_ident_expr = 341,               /* ident_expr  */
  YYSYMBOL_sub_type_level_expr = 342,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 343,                 /* for_expr  */
  YYSYMBOL_bracket_loop_expr = 344,        /* bracket_loop_expr  */
  YYSYMBOL_cond_expr = 345,                /* cond_expr  */
  YYSYMBOL_nil_expr = 346,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 347,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 348,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 349,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 350,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 351,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 352,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 353,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 354,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 355,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 356,                 /* new_expr  */
  YYSYMBOL_let_expr = 357,                 /* let_expr  */
  YYSYMBOL_range_literal_expr = 358,       /* range_literal_expr  */
  YYSYMBOL_cast_expr = 359,                /* cast_expr  */
  YYSYMBOL_tuple_expand_expr = 360,        /* tuple_expand_expr  */
  YYSYMBOL_super_expr = 361,               /* super_expr  */
  YYSYMBOL_expr = 362,                     /* expr  */
  YYSYMBOL_opt_expr = 363,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 364,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 365,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 366,           /* call_base_expr  */
  YYSYMBOL_call_expr = 367,                /* call_expr  */
  YYSYMBOL_dot_expr = 368,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 369,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 370,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 371,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 372,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 373,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 374,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 375,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 376,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 377,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 378       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 328 "chpl.ypp"

  // include the definition of methods for ParserContext
  #include "ParserContextImpl.h"
  // include helper macros for use in writing productions
  #include "parser-help.h"
  // include implementation of error logging hook
  #include "parser-yyerror.h"

#line 502 "bison-chpl-lib.cpp"

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
#define YYLAST   19116

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  198
/* YYNRULES -- Number of rules.  */
#define YYNRULES  722
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1293

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
       0,   634,   634,   638,   639,   644,   645,   653,   657,   667,
     672,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   702,   703,   704,   713,   722,   726,   733,   741,
     742,   743,   744,   745,   746,   747,   748,   762,   777,   785,
     797,   808,   820,   832,   843,   859,   860,   864,   865,   866,
     870,   874,   875,   879,   883,   890,   902,   910,   913,   923,
     925,   932,   937,   946,   950,   970,   985,  1000,  1016,  1017,
    1019,  1024,  1025,  1030,  1048,  1052,  1056,  1067,  1086,  1087,
    1091,  1095,  1099,  1104,  1108,  1112,  1122,  1127,  1133,  1140,
    1145,  1152,  1163,  1164,  1168,  1173,  1181,  1190,  1200,  1208,
    1217,  1230,  1238,  1242,  1247,  1253,  1262,  1263,  1267,  1276,
    1280,  1284,  1288,  1292,  1296,  1305,  1306,  1310,  1311,  1312,
    1313,  1314,  1318,  1319,  1331,  1332,  1337,  1338,  1339,  1340,
    1341,  1342,  1343,  1344,  1345,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,  1353,  1354,  1355,  1359,  1360,  1361,  1362,  1363,
    1364,  1365,  1366,  1367,  1368,  1369,  1370,  1377,  1378,  1379,
    1380,  1384,  1385,  1389,  1396,  1406,  1410,  1416,  1416,  1422,
    1422,  1431,  1432,  1433,  1434,  1435,  1436,  1437,  1441,  1446,
    1451,  1456,  1463,  1471,  1472,  1476,  1485,  1491,  1499,  1518,
    1517,  1527,  1526,  1539,  1546,  1564,  1577,  1590,  1594,  1598,
    1602,  1606,  1610,  1614,  1618,  1638,  1642,  1646,  1650,  1654,
    1658,  1662,  1666,  1670,  1674,  1678,  1682,  1686,  1690,  1694,
    1698,  1702,  1706,  1710,  1714,  1718,  1722,  1726,  1731,  1739,
    1748,  1752,  1756,  1760,  1764,  1768,  1772,  1776,  1780,  1785,
    1790,  1795,  1803,  1818,  1836,  1840,  1847,  1848,  1853,  1858,
    1859,  1860,  1861,  1862,  1863,  1864,  1865,  1866,  1867,  1868,
    1869,  1870,  1879,  1880,  1881,  1882,  1891,  1892,  1896,  1900,
    1904,  1911,  1915,  1919,  1926,  1930,  1934,  1938,  1945,  1946,
    1950,  1954,  1958,  1965,  1978,  1994,  2002,  2006,  2015,  2016,
    2020,  2024,  2029,  2037,  2042,  2046,  2053,  2054,  2058,  2067,
    2072,  2083,  2090,  2091,  2092,  2096,  2097,  2101,  2105,  2109,
    2113,  2117,  2124,  2141,  2154,  2161,  2166,  2173,  2172,  2182,
    2188,  2187,  2202,  2209,  2221,  2239,  2236,  2264,  2268,  2269,
    2271,  2276,  2277,  2281,  2282,  2286,  2289,  2291,  2296,  2297,
    2308,  2328,  2327,  2343,  2342,  2360,  2370,  2367,  2401,  2409,
    2417,  2428,  2439,  2448,  2463,  2464,  2468,  2469,  2470,  2479,
    2480,  2481,  2482,  2483,  2484,  2485,  2486,  2487,  2488,  2489,
    2490,  2491,  2492,  2493,  2494,  2495,  2496,  2497,  2498,  2499,
    2500,  2501,  2502,  2503,  2507,  2508,  2509,  2510,  2511,  2512,
    2513,  2514,  2515,  2516,  2517,  2518,  2523,  2524,  2528,  2529,
    2533,  2534,  2538,  2539,  2543,  2544,  2548,  2549,  2553,  2557,
    2558,  2562,  2566,  2571,  2576,  2581,  2586,  2594,  2598,  2606,
    2607,  2608,  2609,  2610,  2611,  2612,  2613,  2614,  2618,  2619,
    2620,  2621,  2622,  2623,  2627,  2628,  2629,  2633,  2634,  2635,
    2636,  2637,  2638,  2642,  2643,  2646,  2647,  2651,  2652,  2653,
    2657,  2662,  2663,  2665,  2667,  2669,  2674,  2676,  2681,  2683,
    2685,  2687,  2689,  2691,  2693,  2698,  2699,  2703,  2712,  2716,
    2724,  2728,  2735,  2756,  2757,  2762,  2763,  2764,  2765,  2766,
    2771,  2770,  2779,  2787,  2791,  2798,  2814,  2831,  2835,  2839,
    2846,  2848,  2850,  2857,  2858,  2859,  2863,  2867,  2868,  2869,
    2873,  2874,  2878,  2879,  2883,  2884,  2885,  2886,  2890,  2891,
    2895,  2899,  2900,  2906,  2907,  2911,  2912,  2916,  2920,  2927,
    2928,  2932,  2937,  2946,  2947,  2951,  2952,  2959,  2960,  2961,
    2962,  2963,  2964,  2966,  2968,  2970,  2972,  2978,  2980,  2983,
    2985,  2987,  2989,  2991,  2993,  2995,  2997,  2999,  3001,  3006,
    3017,  3028,  3038,  3054,  3070,  3086,  3095,  3104,  3112,  3126,
    3140,  3157,  3161,  3165,  3169,  3173,  3177,  3181,  3188,  3206,
    3214,  3215,  3216,  3217,  3218,  3219,  3220,  3224,  3225,  3229,
    3238,  3239,  3243,  3252,  3253,  3257,  3271,  3275,  3282,  3283,
    3284,  3285,  3286,  3287,  3291,  3293,  3295,  3297,  3299,  3305,
    3312,  3324,  3336,  3349,  3366,  3373,  3378,  3383,  3388,  3394,
    3400,  3430,  3437,  3444,  3445,  3449,  3451,  3452,  3454,  3456,
    3457,  3458,  3459,  3462,  3463,  3464,  3465,  3466,  3467,  3468,
    3469,  3470,  3474,  3475,  3479,  3480,  3481,  3485,  3486,  3487,
    3488,  3497,  3498,  3501,  3502,  3503,  3507,  3519,  3531,  3538,
    3540,  3542,  3544,  3546,  3552,  3565,  3566,  3570,  3574,  3581,
    3582,  3586,  3587,  3591,  3592,  3593,  3594,  3595,  3596,  3597,
    3598,  3603,  3608,  3612,  3616,  3620,  3628,  3633,  3642,  3643,
    3644,  3645,  3646,  3647,  3648,  3649,  3650,  3651,  3652,  3653,
    3654,  3655,  3656,  3657,  3658,  3659,  3660,  3661,  3662,  3663,
    3664,  3668,  3669,  3670,  3671,  3672,  3673,  3676,  3680,  3684,
    3688,  3692,  3699,  3703,  3707,  3711,  3719,  3720,  3721,  3722,
    3723,  3724,  3725
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
  "TQUERIEDIDENT", "INTLITERAL", "REALLITERAL", "IMAGLITERAL",
  "STRINGLITERAL", "BYTESLITERAL", "CSTRINGLITERAL", "EXTERNCODE",
  "TALIGN", "TAS", "TATOMIC", "TBEGIN", "TBREAK", "TBOOL", "TBORROWED",
  "TBY", "TBYTES", "TCATCH", "TCLASS", "TCOBEGIN", "TCOFORALL", "TCOMPLEX",
  "TCONFIG", "TCONST", "TCONTINUE", "TDEFER", "TDELETE", "TDMAPPED", "TDO",
  "TDOMAIN", "TELSE", "TENUM", "TEXCEPT", "TEXPORT", "TEXTERN", "TFALSE",
  "TFOR", "TFORALL", "TFOREACH", "TFORWARDING", "TIF", "TIMAG", "TIMPORT",
  "TIN", "TINCLUDE", "TINDEX", "TINLINE", "TINOUT", "TINT", "TITER",
  "TINITEQUALS", "TIMPLEMENTS", "TINTERFACE", "TLABEL", "TLAMBDA", "TLET",
  "TLIFETIME", "TLOCAL", "TLOCALE", "TMANAGE", "TMINUSMINUS", "TMODULE",
  "TNEW", "TNIL", "TNOINIT", "TNONE", "TNOTHING", "TON", "TONLY",
  "TOPERATOR", "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED", "TPARAM",
  "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE",
  "TPUBLIC", "TPROCLP", "TREAL", "TRECORD", "TREDUCE", "TREF", "TREQUIRE",
  "TRETURN", "TSCAN", "TSELECT", "TSERIAL", "TSHARED", "TSINGLE",
  "TSPARSE", "TSTRING", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS", "TTHROW",
  "TTHROWS", "TTRUE", "TTRY", "TTRYBANG", "TTYPE", "TUINT", "TUNDERSCORE",
  "TUNION", "TUNMANAGED", "TUSE", "TVAR", "TVOID", "TWHEN", "TWHERE",
  "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN",
  "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
  "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL",
  "TASSIGNSR", "TATMARK", "TBANG", "TBAND", "TBNOT", "TBOR", "TBXOR",
  "TCOLON", "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL",
  "TEXP", "TGREATER", "TGREATEREQUAL", "THASH", "TLESS", "TLESSEQUAL",
  "TMINUS", "TMOD", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI",
  "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP",
  "TRP", "TLSBR", "TRSBR", "TNOELSE", "TDOTDOTOPENHIGH", "TUPLUS",
  "TUMINUS", "TLNOT", "$accept", "program", "toplevel_stmt_ls",
  "toplevel_stmt", "pragma_ls", "stmt", "stmt_base", "tryable_stmt",
  "decl_base", "attribute_receiver", "attribute_group",
  "attribute_decl_stmt_ls", "attribute_decl_stmt", "attribute_decl_begin",
  "opt_attribute_decl_toolspace", "opt_attribute_actuals",
  "attribute_actuals_ls", "attribute_actual", "module_decl_start",
  "module_decl_stmt", "opt_access_control", "opt_prototype",
  "include_module_stmt", "block_stmt_body", "block_stmt", "stmt_ls",
  "renames_ls", "use_renames_ls", "opt_only_ls", "use_stmt", "import_stmt",
  "import_expr", "import_ls", "require_stmt", "assignment_stmt",
  "opt_label_ident", "reserved_word_ident", "ident_def", "ident_use",
  "internal_type_ident_def", "scalar_type", "reserved_type_ident_use",
  "do_stmt", "return_stmt", "class_level_stmt", "$@1", "$@2",
  "inner_class_level_stmt", "forwarding_decl_stmt",
  "forwarding_decl_start", "extern_or_export",
  "extern_export_decl_stmt_start", "extern_export_decl_stmt", "$@3", "$@4",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "try_stmt", "catch_expr_ls",
  "catch_expr", "catch_expr_inner", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "manager_expr", "manager_expr_ls",
  "manage_stmt", "class_decl_stmt", "class_start", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_header_lcbr", "enum_ls", "$@5", "$@6", "enum_item",
  "lambda_decl_start", "lambda_decl_expr", "$@7", "linkage_spec_empty",
  "linkage_spec", "opt_fn_type_formal_ls", "fn_type_formal_ls",
  "fn_type_formal", "opt_fn_type_ret_type", "fn_type", "fn_expr", "$@8",
  "$@9", "fn_decl_stmt_complete", "fn_decl_stmt", "$@10",
  "fn_decl_stmt_inner", "fn_decl_stmt_start", "fn_decl_receiver_expr",
  "fn_ident", "op_ident", "assignop_ident", "all_op_name",
  "formal_var_arg_expr", "opt_formal_ls", "req_formal_ls",
  "formal_ls_inner", "formal_ls", "formal", "formal_ident_def",
  "named_formal", "opt_formal_intent_tag", "required_intent_tag",
  "opt_this_intent_tag", "proc_iter_or_op", "opt_ret_tag",
  "opt_throws_error", "opt_function_body_stmt", "function_body_stmt",
  "query_expr", "opt_lifetime_where", "lifetime_components_expr",
  "lifetime_expr", "lifetime_ident", "type_alias_decl_stmt",
  "type_alias_decl_stmt_start", "type_alias_decl_stmt_inner_ls",
  "type_alias_decl_stmt_inner", "opt_init_type", "var_decl_type",
  "var_decl_stmt", "$@11", "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
  "tuple_var_decl_component", "tuple_var_decl_stmt_inner_ls",
  "opt_init_expr", "formal_or_ret_type_expr", "ret_type", "colon_ret_type",
  "opt_ret_type", "opt_type", "formal_type", "colon_formal_type",
  "opt_colon_formal_type", "expr_ls", "tuple_component", "tuple_expr_ls",
  "opt_actual_ls", "actual_ls", "actual_expr", "ident_expr",
  "sub_type_level_expr", "for_expr", "bracket_loop_expr", "cond_expr",
  "nil_expr", "stmt_level_expr", "opt_task_intent_ls",
  "task_intent_clause", "task_intent_ls", "forall_intent_clause",
  "forall_intent_ls", "intent_expr", "task_var_prefix",
  "new_maybe_decorated", "new_expr", "let_expr", "range_literal_expr",
  "cast_expr", "tuple_expand_expr", "super_expr", "expr", "opt_expr",
  "opt_try_expr", "lhs_expr", "call_base_expr", "call_expr", "dot_expr",
  "parenthesized_expr", "bool_literal", "str_bytes_literal",
  "literal_expr", "assoc_expr_ls", "binary_op_expr", "unary_op_expr",
  "reduce_expr", "scan_expr", "reduce_scan_op_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1078)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-723)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1078,   105,  4273, -1078,   -51,    91, -1078, -1078, -1078, -1078,
   -1078, -1078, -1078, 11623,     5,   194,   187, 14024,   226, 18870,
       5, 11623,   253,   263,   245,   194,  5673, 11623,  5673,   208,
   18925, -1078,   346,   333,  9173, 10048, 10048, -1078,  9348,   338,
     342,   287, -1078,   362, 18925, 18925, 18925, -1078,  2307, 10223,
     370, 11623, 11623,   266, -1078,   412,   417, 11623, -1078, 14024,
   -1078, 11623,   400,   307,   215,   249, 13372,   427, 19000, -1078,
   11623,  8648, 11623, 10223, 14024, 11623,   392,   446,   332,  5673,
     476, 11623,   490,  7598,  7598, -1078,   507, -1078, 14024, -1078,
     534,  9348,  9523, -1078,   553, 11623, -1078, 11623, -1078, -1078,
   13852, 11623, -1078, 11623, -1078, -1078, -1078,  4623,  7948,  9698,
   11623, -1078,  5323, -1078, -1078, -1078, -1078,  2559,   430, -1078,
      44,   422, -1078,   206, -1078, -1078, -1078, -1078, -1078, -1078,
   -1078, -1078, -1078, -1078, -1078, -1078, -1078,  7773, -1078, 10398,
   -1078, -1078, -1078, -1078, -1078,   550, -1078, -1078, -1078, -1078,
   -1078, -1078, -1078, -1078,   469, 18925, -1078, 18610,   463,   437,
   -1078,   213, -1078, -1078, -1078,   185,   244, -1078, -1078, 18925,
    2307, -1078,   478,   491, -1078, -1078, -1078,   479,   493, 11623,
     513,   518, -1078, -1078, -1078, 18397,  3495,   365,   521,   522,
   -1078, -1078,   401, -1078, -1078, -1078, -1078, -1078,    41, -1078,
   -1078, -1078, 11623, 11623, 11623, 18925, -1078, 11623, 11623, 10573,
     612,   464, -1078, -1078, -1078, -1078, 18397,   480, -1078, -1078,
     498,  5673, -1078, -1078,   523,   -32,   511, 15783, -1078,   233,
   -1078,   524, -1078,   202, 18397,   571,  9523, -1078, -1078, -1078,
   -1078, -1078, -1078, -1078, -1078, -1078, -1078, -1078, -1078, -1078,
   -1078, -1078, -1078, -1078, -1078, -1078, -1078, -1078, -1078, -1078,
   -1078, -1078,   528, -1078, -1078, 18925,   520,    40,  2848,    17,
   15139,    17, 15215, 18925, 18925,   -25,  3838, -1078, -1078,   609,
    9523,   529,   489,  5673,  1562,    88,    29, -1078,  5673, -1078,
   -1078, 15859, -1078,    38, 16489,   395, -1078,   530,   531, -1078,
   15859,   -32,   395, -1078,  9523,   905,   905,    39,    -4, -1078,
      66, -1078, -1078, -1078, -1078, 11623, 11623, -1078,   559, -1078,
     527,   547, -1078, -1078,  2032,   558, -1078, -1078, 18397,   257,
   11623, 11623, -1078,   -30, -1078, -1078, 18397,   545, 16569, -1078,
   15859,   -32, 18397,   542,  9523, -1078, 18397, 16609, -1078, -1078,
   -1078, -1078,   -32,    40, 15859,   564, -1078,   220,   220,  2772,
     395,   395,    13, -1078, -1078,  4798,   189, -1078, 11623,   131,
     146, -1078, 14167,    -1,   125, 16655,   110,  2772,   697, -1078,
   -1078, 18925, -1078,  9173, 10048, -1078,  4623, 10048, -1078, -1078,
     609, -1078, -1078, -1078, -1078,   715,   551,  4973, 11623, -1078,
   11623,   671, -1078, -1078, 15383,    63,   469, 18397,   449, 18925,
   11623,   568, -1078,   566, 18665,   615,   205, -1078,    53,   185,
   -1078, -1078, -1078, -1078,   651, -1078, -1078, -1078,   109,   653,
   -1078, -1078, -1078, 13682,   628,   293, -1078,   315,   501, -1078,
     606, 11623, 11623, 11623, 10048, 10048, 11623,   502, 11623, 11623,
   11623, 11623, 11623,   236, 13852, 11623, 11623, 11623, 11623, 11623,
   11623, 11623, 11623, 11623, 11623, 11623, 11623, 11623, 11623, 11623,
   11623,   689, -1078, -1078, -1078, -1078, -1078,  9523,  9523, -1078,
   -1078, -1078, -1078,  9523, -1078, -1078,  9523,  9523,  9523,  9523,
   10048, 10048, 15935, 16011, 16761,   586,   101, 18925,  8123, -1078,
   -1078, -1078, 10048,    40,  2307, -1078, 11623, -1078, 11623,   635,
     588,   616, -1078, -1078, -1078,   717, 11623, -1078,  5848, 10048,
   -1078,   593, -1078,    40,  6023, 10048, -1078,    40, -1078,    40,
   10048, -1078,    40,   643,   645,  5673,   739,  5673, -1078,   741,
   11623,   711,   604,  9523, 18925, -1078, -1078, -1078,  1562, -1078,
   -1078,   177, -1078, 11798,   656, 11623,  2307, -1078, -1078, 11623,
   -1078, 18740, 11623, 11623, -1078,   610, -1078, -1078, -1078, -1078,
   18795, -1078,   185, 13546, -1078,  1562, -1078,   636, 11798, -1078,
   18397, 18397, -1078,   511, -1078,    62, -1078,  9523,   611, -1078,
     764,   764, -1078, -1078, -1078, -1078, -1078, -1078, 10748, -1078,
   16807,  8298, -1078,  8473, -1078, 18397,  5673,   613, 10048, 10923,
    4448,   617, 11623, 11098, -1078, -1078, 15463, 15291,   130,   637,
    8823, -1078,   358, -1078,  5148, -1078,   316, 16847,   349, 15555,
   18925,  7423,  7423, -1078,   469,   621, -1078,   233, -1078,   240,
     640,  1254, -1078, -1078, 11623,   204, -1078, -1078,   183, -1078,
   -1078,   100, -1078, -1078,  4091, -1078,   688, -1078,   622,   646,
   -1078,   647, -1078,   648,   649,   652,   655, -1078,   657,   660,
     661,   662,   663,   664,   666, -1078, -1078, -1078, -1078, -1078,
   -1078, -1078, -1078, -1078, -1078, -1078, -1078, -1078, -1078, -1078,
   -1078, -1078, -1078, -1078, -1078, -1078, -1078, 11623,   659,   669,
     622, -1078,   622, -1078, 11623, -1078, 18925, -1078, -1078,  2541,
    2541,   516, -1078,   516, -1078,   516,   567,  1425,  2355,  1229,
     -32,   220, -1078,   505, -1078, -1078, -1078, -1078, -1078,  2772,
    3357,   516,  1616,  1616,  2541,  1616,  1616,  1043,   220,  3357,
    2998,  1043,   395,   395,   220,  2772,   654,   668,   670,   672,
     673,   676,   629,   630, -1078,   516, -1078,   516, 11973, 10048,
   12148, 10048, 11623,  9523, 10048, 14167,   674,    65, -1078, -1078,
   -1078,   217, -1078,  2752, 18442,   303,     5, 16923, -1078,   368,
   18397, 16999,  9523, -1078,  9523, 11623,   247,  9348, 18397,    70,
   16087,  8123, -1078,  9348, 18397,    51, 15631, -1078, -1078,    17,
   15707, -1078, 11623, 11623,   786,  5673,   790, 17082,  5673, 16163,
   18925, -1078,   248, -1078,   313, -1078,   314,  1562,    88, -1078,
   18397,  8998, -1078, 18397, -1078, -1078, -1078, 18925, 17158, 17198,
   -1078,   636,   679, -1078,   321, 11798, -1078,   207, -1078, 11623,
   -1078,   658,   170,   675, -1078,  1850, -1078, -1078, -1078, -1078,
   -1078, -1078, -1078, -1078,  5673,    68, 16239, -1078, -1078, 18397,
    5673, 18397, -1078, 17244, 10048, 10048, 10048,  5673, -1078, -1078,
     324, -1078, -1078, -1078, -1078, 11623, -1078,    96, 14239, 11623,
   -1078, 11273,  7423,  7423, -1078, 11623,   515, 11623,   537,   539,
   11623,  9873,   540,   387, -1078, -1078, -1078, 17350,   681,   680,
     684, -1078, 18925,  2307, -1078,  9523,   678,  1391, 18397, 18925,
     430,   752, -1078,   688, -1078, -1078, -1078, -1078, -1078,     9,
      64, -1078, 17396, -1078, 14398, -1078, -1078, 18397, -1078,   677,
     685, -1078, -1078, -1078, -1078, -1078, -1078, -1078, -1078, 11623,
     799, 17436, 11623,   812, 17588,   343,   692, 17512,  9523, -1078,
   -1078,  8123, -1078, -1078, -1078, -1078, -1078, -1078, -1078, -1078,
   -1078, -1078, -1078, -1078,    36, 10048, 10048,    40,    40, -1078,
   -1078, -1078, -1078, 15859, -1078, 14479,  6198, -1078,  6373, -1078,
     353, -1078, 14559,  6548, -1078,    40,  6723, -1078,    40, -1078,
      40, -1078,    40, 18397, 18397,  5673, -1078,  5673, 11623, -1078,
    5673,   821,   694,   699, 18925,   511, -1078, -1078,   656, -1078,
   -1078, -1078,   701,   703,   207, 11623,   688,   636, -1078, 11623,
   -1078, -1078, 18397, -1078,  6898, 11623, -1078, -1078, -1078, 18925,
   -1078,   725,   511, -1078,  7073,   704,  7248,   705, -1078, 11623,
      40, 15859,    17, 15707,   153, 16315,  9523, -1078, -1078, -1078,
    7423, -1078, 17628,    77, 16407,   404,   716,  7423, -1078, 11623,
   -1078, -1078,  2151,   419,   429,   374, -1078, -1078, -1078, -1078,
    2821,  2133, -1078, -1078, 18665, -1078,     9,    78, 11623, -1078,
     736,   712, -1078,   622,   622, -1078, -1078, 17673, 12323, 12498,
   17780, 12673, 12848, -1078, 13023, 13198,   381, -1078,   656,   391,
     406, -1078, -1078, -1078,  5673,  9348, 18397,  9348, 18397,  8123,
   -1078,  5673,  9348, 18397, -1078,  9348, 18397, -1078, -1078, -1078,
   -1078, -1078, 18397,   852,  5673, -1078, -1078, -1078, -1078, -1078,
    9523,  9523, -1078, -1078, -1078, -1078,   207, 18397,  5673, -1078,
      84,   718, 11623, -1078,  9348, -1078, 18397,  5673,  9348, -1078,
   18397,  5673, 18397,  5673,  5673, -1078,   218, 11448,  7423,  7423,
    7423,  7423, -1078, -1078, -1078, 17825, 18397,  3713, -1078, -1078,
   -1078,   723,  2133, -1078, -1078, -1078, -1078, -1078,   161, -1078,
       0, -1078,   261, 17871,    71, -1078, -1078, -1078, 11623, 11623,
   11623, 11623, 11623, 11623, 11623, 11623, -1078, -1078, 17082, 14639,
   14719, -1078, 17082, 14799, 14879,  5673, -1078,   724,   726, -1078,
   -1078,   571, -1078,   511, 18397, 14959, -1078, 15039, -1078, -1078,
   -1078, 18397,   436,   735,   456,   737, 11623, -1078, -1078, -1078,
      60, -1078, 11623,    78,   161,   161,   161,   161,   161,   161,
      78,  5498, -1078, -1078, -1078, -1078, -1078, 17977, 18053, 18129,
   18205, 18281, 18357,  5673,  5673,  5673,  5673, -1078,   740,   742,
   -1078,  5673,  5673, -1078, -1078, -1078, -1078, 18397, -1078, 18397,
   -1078, -1078, -1078, -1078, -1078, -1078, -1078,   743, -1078, 11623,
   11623, 11623, 11623, 11623, 11623, 17082, 17082, 17082, 17082, -1078,
   -1078, 17082, 17082
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   124,   450,   665,   666,   667,
     661,   662,   668,     0,   577,   115,   145,   545,   152,   547,
     577,     0,   151,   480,   478,   115,     0,     0,     0,   266,
     146,   184,   183,   659,     0,     0,     0,   182,     0,   150,
      68,   267,   329,   147,     0,     0,     0,   324,     0,     0,
     154,     0,     0,   594,   569,   669,   155,     0,   330,   539,
     475,     0,     0,     0,   169,   167,   417,   149,   548,   477,
       0,     0,     0,     0,   543,     0,     0,   153,     0,     0,
     125,     0,   660,     0,     0,   468,   148,   304,   541,   479,
     156,     0,     0,   718,     0,     0,   720,     0,   721,   722,
     610,     0,   719,   716,   618,   165,   717,     0,     0,     0,
       0,     4,     0,     5,    10,    11,    12,   327,    50,    51,
      57,     0,    45,    71,    13,    77,    14,    15,    16,    17,
      29,   525,   526,    23,    46,   166,   176,     0,   185,   632,
     177,    18,    31,    30,    20,     0,   261,    19,   628,    22,
      35,    32,    33,   175,   305,     0,   173,     0,     0,   625,
     328,     0,   622,   171,   345,   437,   428,   620,   174,     0,
       0,   172,   637,   616,   615,   619,   529,   527,     0,     0,
     626,   627,   631,   630,   629,     0,   528,     0,   638,   639,
     640,   663,   664,   621,   531,   530,   623,   624,     0,    28,
     547,   146,     0,     0,     0,     0,   548,     0,     0,     0,
       0,   625,   637,   527,   626,   627,   537,   528,   638,   639,
       0,     0,   578,   116,     0,   546,     0,   577,   469,     0,
     476,     0,    21,     0,   513,     0,   519,   122,   126,   138,
     132,   131,   140,   119,   130,   141,   127,   142,   117,   143,
     136,   129,   137,   135,   133,   134,   118,   120,   128,   139,
     144,   123,     0,   121,   193,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    70,    69,    71,
     519,     0,     0,     0,     0,     0,     0,   483,     0,   162,
      40,     0,   296,     0,   295,   703,   598,   595,   596,   597,
       0,   540,   704,     7,   519,   327,   327,   424,   159,   419,
     160,   420,   421,   425,   426,   158,   157,   427,   417,   509,
       0,   332,   333,   335,     0,   418,   508,   337,   496,     0,
       0,     0,   163,   622,   613,   636,   614,     0,     0,    43,
       0,   544,   532,     0,   519,    44,   538,     0,   274,   278,
     275,   278,   542,     0,     0,     0,    56,   705,   707,   608,
     702,   701,     0,    74,    78,     0,     0,   515,     0,     0,
       0,   516,   561,     0,     0,   513,     0,   609,     0,     6,
     302,     0,   183,     0,     0,   303,     0,     0,    48,     9,
      71,    49,    47,    52,    55,     0,    54,     0,     0,    72,
       0,     0,   480,   181,     0,   661,   305,   633,   189,     0,
       0,     0,   301,     0,     0,   322,     0,   315,   417,   437,
     435,   436,   434,   353,   438,   441,   440,   442,     0,   432,
     429,   430,   433,     0,   473,     0,   470,     0,   617,    34,
     599,     0,     0,     0,     0,     0,     0,   706,     0,     0,
       0,     0,     0,     0,   607,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   384,   391,   392,   393,   388,   390,     0,     0,   386,
     389,   387,   385,     0,   395,   394,     0,     0,   519,   519,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
      24,    37,     0,     0,     0,    25,     0,    38,     0,   525,
       0,   520,   521,   524,   314,     0,     0,   203,     0,     0,
     202,     0,   211,     0,     0,     0,   209,     0,   219,     0,
       0,   217,     0,     0,     0,     0,   235,     0,   384,   231,
       0,     0,     0,     0,     0,   245,    26,   487,     0,   488,
     490,     0,   507,     0,   493,     0,     0,   161,    39,     0,
     298,     0,     0,     0,    41,     0,   170,   168,   422,   423,
       0,   418,   437,   417,   410,     0,   409,   511,     0,   108,
     634,   635,   343,     0,   164,     0,    42,     0,     0,   285,
     276,   277,   196,   195,    27,    76,    75,    79,     0,   670,
       0,     0,   655,     0,   657,   562,     0,     0,     0,     0,
       0,     0,     0,     0,   674,     8,     0,     0,     0,    57,
       0,    53,     0,    65,     0,   106,     0,   102,     0,    86,
       0,     0,    92,   178,   305,     0,   191,     0,   188,   272,
     306,     0,   313,   319,     0,   317,   312,   402,     0,   404,
     408,     0,   439,   501,     0,   503,   443,   431,   400,   122,
     380,   126,   378,   132,   131,   130,   127,   382,   142,   143,
     129,   133,   118,   128,   144,   377,   359,   362,   360,   361,
     383,   372,   363,   376,   368,   366,   379,   367,   365,   370,
     375,   364,   369,   373,   374,   371,   381,     0,   356,     0,
     400,   357,   400,   354,     0,   472,     0,   467,   482,   698,
     697,   700,   709,   708,   713,   712,   694,   691,   692,   693,
     611,   681,   124,     0,   651,   652,   125,   650,   649,   605,
     685,   696,   690,   688,   699,   689,   687,   679,   684,   686,
     695,   678,   682,   683,   680,   606,     0,     0,     0,     0,
       0,     0,     0,     0,   711,   710,   715,   714,     0,     0,
       0,     0,     0,     0,     0,   672,   272,   588,   589,   591,
     593,     0,   580,     0,     0,     0,   577,   577,   199,     0,
     514,     0,     0,   534,     0,     0,     0,     0,   551,     0,
       0,     0,   212,     0,   557,     0,     0,   210,   220,     0,
       0,   218,     0,     0,   234,     0,   230,     0,     0,     0,
       0,   533,     0,   248,     0,   246,     0,   491,     0,   506,
     505,     0,   485,   604,   484,   297,   294,     0,     0,     0,
     648,   511,   338,   334,     0,     0,   512,   493,   336,     0,
     342,     0,     0,     0,   535,     0,   279,   671,   612,   656,
     517,   658,   518,   227,     0,     0,     0,   673,   225,   563,
       0,   676,   675,     0,     0,     0,     0,     0,    58,    59,
       0,    61,    63,    67,    66,     0,   101,     0,     0,     0,
      94,     0,     0,    92,    64,   377,   359,   362,   360,   361,
     370,   369,   371,     0,   396,   397,    81,    80,    93,     0,
       0,   307,     0,     0,   269,     0,     0,   327,   323,     0,
       0,   417,   403,   443,   499,   498,   497,   500,   444,   451,
     417,   352,     0,   358,     0,   348,   349,   474,   471,     0,
       0,   114,   112,   113,   111,   110,   109,   646,   647,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   590,
     592,     0,   579,   145,   152,   151,   150,   147,   154,   155,
     149,   153,   148,   156,     0,     0,     0,     0,     0,   481,
     194,   523,   522,     0,   229,     0,     0,   201,     0,   200,
       0,   583,     0,     0,   207,     0,     0,   205,     0,   215,
       0,   213,     0,   243,   242,     0,   237,     0,     0,   233,
       0,   239,     0,   271,     0,     0,   489,   492,   493,   494,
     495,   293,     0,     0,   493,     0,   443,   511,   510,   398,
     413,   411,   344,   287,     0,     0,   286,   289,   536,     0,
     280,   283,     0,   228,     0,     0,     0,     0,   226,     0,
       0,     0,     0,     0,     0,     0,     0,    60,   107,   104,
       0,   103,    89,    88,    87,     0,     0,     0,   179,     0,
     180,   307,   327,     0,     0,     0,   300,   169,   167,   299,
     327,   327,   308,   318,     0,   405,   451,     0,     0,   346,
     407,     0,   355,   400,   400,   653,   654,     0,     0,     0,
       0,     0,     0,   271,     0,     0,     0,   581,   493,   637,
     637,   198,   197,   204,     0,     0,   550,     0,   549,     0,
     582,     0,     0,   556,   208,     0,   555,   206,   216,   214,
     236,   232,   568,   238,     0,    73,   268,   247,   244,   486,
     519,   519,   414,   412,   339,   340,   493,   399,     0,   291,
       0,     0,     0,   281,     0,   223,   565,     0,     0,   221,
     564,     0,   677,     0,     0,    62,     0,     0,     0,    92,
       0,    92,    95,    98,    84,    83,    82,   327,   186,   192,
     190,   273,   327,   310,   309,   321,   325,   465,     0,   466,
     453,   456,     0,   452,     0,   401,   350,   351,     0,     0,
       0,     0,     0,     0,     0,     0,   273,   585,   554,     0,
       0,   584,   560,     0,     0,     0,   241,     0,     0,   416,
     415,   292,   290,     0,   284,     0,   224,     0,   222,   105,
      91,    90,     0,     0,     0,     0,     0,   187,   270,   311,
       0,   464,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   445,   447,   449,   347,   446,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   240,   600,   601,
     282,     0,     0,    97,   100,    96,    99,    85,   326,   455,
     457,   458,   461,   462,   463,   459,   460,   454,   448,     0,
       0,     0,     0,     0,     0,   553,   552,   559,   558,   602,
     603,   567,   566
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1078, -1078, -1078,     3,   -64,  2734, -1078,   175,   772, -1078,
   -1078,  -148,  -117, -1078,   282, -1078, -1078,  -143, -1078, -1078,
      33,   631, -1078,  -564,  3045,   510,  -567, -1078,  -849, -1078,
   -1078,    37, -1078, -1078, -1078,   883, -1078,  3235,  -206, -1078,
   -1078,  -498,  2909, -1077,  -851, -1078, -1078,   317, -1078, -1078,
   -1078, -1078, -1078, -1078, -1078, -1078,   794,   123, -1078,   822,
   -1078, -1078,   -87,  2433, -1078, -1078, -1078, -1078,   577, -1078,
    -111, -1078, -1078, -1078, -1078,   375, -1078, -1078, -1078,   -98,
   -1078,  -366,  -854, -1078, -1078, -1078, -1078, -1078,  -411, -1078,
     533, -1078, -1078, -1078, -1078, -1078,   360, -1078,  1792, -1078,
   -1078, -1078, -1078,   538, -1078, -1078, -1078, -1078,    12,  -416,
    -168,  -835,  -969,  -674, -1078,    19, -1078,    26,   371,   -55,
     627,   -54, -1078, -1078,  -369,  -859, -1078,  -283, -1078,  -126,
    -289,  -277,  -836, -1078, -1078,    56,   254, -1078,  -214,   824,
   -1078,  -164,   403,   145,  -286,  -810,  -638, -1078, -1078, -1078,
    -618,  -543, -1078,  -789,    -3,    79, -1078,  -273,  -514,  -555,
     299, -1078, -1078, -1078, -1078,   837, -1078,   -10, -1078, -1078,
    -149, -1078,  -771, -1078, -1078,  1219,  1405, -1078, -1078, -1078,
   -1078,   -13, -1078,    94,  1628, -1078,  1929,  2230, -1078, -1078,
   -1078, -1078, -1078, -1078, -1078, -1078, -1078,  -485
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   364,   112,   806,   114,   115,   116,   389,
     117,   118,   119,   120,   396,   621,   870,   871,   121,   122,
     123,   401,   124,   125,   126,   365,   898,   628,   899,   127,
     128,   625,   626,   129,   130,   224,   261,   285,   131,   263,
     132,   319,   520,   133,   134,   306,   305,   135,   136,   137,
     138,   139,   140,   637,   902,   141,   142,   267,   143,   275,
     144,   814,   815,   210,   146,   147,   148,   149,   590,   846,
    1032,   150,   151,   842,  1027,   292,   293,   152,   153,   154,
     155,   411,   907,   156,   157,   416,   909,   910,   417,   158,
     211,  1230,   160,   161,   320,   321,   322,  1016,   162,   334,
     583,   839,   163,   164,  1184,   165,   166,   699,   700,   894,
     895,   896,  1020,   921,   419,   648,  1081,   649,   577,   650,
     324,   571,   433,   423,   428,   919,  1245,  1246,   167,  1079,
    1180,  1181,  1182,   168,   169,   435,   436,   705,   170,   171,
     229,   286,   287,   550,   551,   822,   326,   917,   655,   656,
     554,   327,   836,   837,   366,   369,   370,   510,   511,   512,
     212,   173,   174,   175,   176,   213,   178,   221,   222,   771,
     527,   980,   772,   773,   179,   214,   215,   182,   183,   184,
     335,   185,   408,   513,   217,   187,   218,   219,   190,   191,
     192,   193,   376,   194,   195,   196,   197,   198
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     216,   393,   318,   643,   225,   111,   437,   542,   227,   414,
     226,   323,   325,   775,   234,   504,   916,   701,   487,   840,
     981,   268,   270,   272,   233,   276,   925,  1021,   926,   812,
     509,   565,   843,   913,  1056,   838,   291,   552,   294,   295,
     635,   406,  1014,  1051,   300,  1132,   301,  1062,   302,   288,
     651,  -266,   394,   328,  1076,   819,  1072,   234,   336,   338,
     340,   341,   342,   841,   893,   872,   346,   329,   347,  1077,
     288,   588,   288,   279,   509,   352,   555,   535,   354,   336,
     307,  1177,   357,   983,   358,   380,   568,   359,   360,   552,
     361,   307,  1241,   582,   234,   336,   375,   377,   509,   722,
     309,   552,   976,  1241,   311,     3,   374,  1244,   540,   447,
     653,   309,   949,  1158,   199,   311,   288,   453,  1232,   521,
     523,  -267,   529,   532,   404,   220,   407,  1078,   312,   569,
     490,   313,  -504,    62,   491,  -504,  -288,   521,   509,   312,
    -341,  -502,   313,   314,    62,   386,  -249,  1233,   764,  1159,
     390,   385,    71,  1244,   314,   950,   915,  1135,   269,   271,
    -504,  -504,   317,    71,  1177,   337,   440,  1209,   236,  -502,
    1178,   521,   608,   317,   606,    87,   556,   866,   199,  -288,
    1097,  1179,   553,  -504,   595,   559,   355,   386,   521,   492,
     493,   494,  -504,   395,   346,   234,   375,   223,  1129,   726,
    1008,  -502,   371,   832,  1133,  -504,   496,  1167,   386,  -504,
     386,  1072,   424,  -504,  -502,   752,   753,   503,  -504,  1173,
    1174,   386,  1164,   336,   607,   611,   647,  -502,  1136,   972,
     386,   506,   373,  -288,   553,  -504,  1242,  -406,   280,   722,
     386,   386,  -250,  1034,  1024,   521,   553,   728,   609,   945,
     521,   443,   398,  -504,   386,   654,   723,   613,   348,   350,
      24,   -70,   816,   425,  1179,   702,   420,   336,   900,   724,
    -504,   429,   609,   521,  -502,   426,   765,   506,   601,  -502,
     -70,  -256,   509,   509,   296,   614,   421,  1025,  1197,   834,
     399,   336,  1018,   603,   427,   -69,   422,   393,   725,   -70,
     610,   172,   342,   346,   602,   867,   775,   634,  -255,   444,
    1223,    60,  1225,   445,   -69,  1055,  1072,   580,   581,   604,
     400,  1229,   430,    69,   817,   172,  1210,   172,  1153,   -70,
     911,   336,   821,   -69,   431,   230,   598,   509,  1201,   726,
     779,  1026,  1231,   297,  -320,   727,  1098,   827,    89,   506,
     818,  -316,   645,   432,   318,   600,   912,   264,  1019,   605,
     599,   447,   298,   -69,   951,  1057,   451,   507,   597,   453,
     616,   617,   228,   456,   234,  -316,   646,  1134,   172,   299,
     236,   509,   172,   172,   618,   627,  1234,   629,  -264,  1219,
     952,  1065,   966,  -254,   506,   784,   491,   234,  1271,  1272,
    1273,  1274,  1275,  1276,   506,   904,   172,   640,   303,  1186,
    1187,   172,   905,  1235,   509,  1236,  1237,  -251,  1238,  1239,
     974,  1003,   579,   903,   277,  -258,   443,   278,   709,   710,
     711,   713,   715,   716,  1096,   717,   718,   719,   720,   721,
     706,   729,   730,   731,   732,   733,   734,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   707,   280,
    1004,   817,   556,   875,   336,   336,   775,  -262,   817,   611,
     336,  1046,  -259,   336,   336,   336,   336,   755,   757,   304,
     708,   876,  -253,  1156,   444,   774,  1005,  1006,   445,   777,
     784,   872,   343,   780,  1017,   781,   879,  1047,  1073,    42,
    1109,  -257,  -327,   234,   344,   788,   790,   269,   701,   318,
     373,   794,   796,   786,   880,   556,  1093,   800,   323,   325,
     172,   784,  -327,   199,   807,    58,  1110,   809,   784,   873,
     336,  -263,  -327,   969,  1057,   159,   447,   488,  -587,   489,
     820,   451,   823,   452,   453,  -265,   294,  1171,   456,   828,
     829,  1057,  1058,  -586,  1196,   463,   356,   509,   636,   159,
     328,   159,  -252,   469,  -587,   328,   706,   712,   714,  1162,
      94,   747,   748,  -645,   336,  -645,   556,   749,   509,  -586,
     750,   751,   172,  1057,  1169,   780,   307,   172,   336,  -260,
     336,  1222,   397,  1224,  1170,   856,   780,   859,   443,   861,
     863,  1263,  -574,  1057,  -720,   409,   309,   336,  -720,  -644,
     311,  -644,   159,   754,   756,   410,   159,   159,   897,   897,
    1243,  1265,   566,   567,   775,   776,  -721,   597,  -722,  -717,
    -721,   908,  -722,  -717,   312,   418,  -644,   313,  -644,   378,
     159,   328,   789,  -571,  -570,   159,   985,   988,   795,   314,
     990,   992,  -641,   799,  -641,   438,   444,   447,   439,   386,
     445,   544,   451,  1175,   172,   453,  1243,   497,   317,   456,
     498,  1049,   728,  -643,  -642,  -643,  -642,   929,  -575,   930,
     850,   386,   852,  -576,   922,   172,  -573,  -572,   500,   505,
     508,   927,   516,   399,   573,   371,   172,   371,   514,   509,
     572,   543,   562,   563,   578,   615,  1035,  1037,   447,   448,
     584,   449,   450,   451,   587,   452,   453,   454,   619,   455,
     456,   457,   458,   620,   460,   461,   462,   463,   464,   594,
     466,   855,   703,   467,   468,   469,   630,   642,   641,  1064,
     644,   652,   509,   657,   470,   788,   941,   794,   944,   807,
     336,   947,   859,   704,   159,   453,  1084,   746,   763,  1071,
     782,   783,  1074,   784,   785,   791,   967,   968,   802,   336,
     803,   336,   973,   805,   975,   808,   810,   811,   774,   307,
     982,   821,   835,   830,   844,   845,   395,   506,   854,   993,
     994,   901,   860,   918,   920,  -124,  -145,  -152,  -151,   309,
     923,  -150,   937,   311,  -147,   938,  -154,   540,   336,  -155,
    -149,  -153,  -125,  -148,   540,  -156,   159,   172,   924,   931,
     995,   159,   328,   172,   997,  1015,  1022,   312,  1057,  1023,
     313,  1088,    62,   932,   172,   933,   172,   934,   935,   177,
     509,   936,   314,  1070,  1091,  1060,   948,   318,  1028,  1066,
    1085,  1041,  1043,  1045,  1061,  1124,   318,  1207,  1208,  1125,
    1086,   317,   627,   177,  1126,   177,  1052,  1094,  1054,   897,
     897,  1142,   357,  1130,   358,  1131,   971,   360,   361,  1147,
    1151,  1163,   940,   911,   943,  1185,  1205,   946,  1228,   388,
    1233,  1213,   336,   985,   988,  1035,  1037,  1258,   159,  1259,
    1264,   868,  1266,  1155,  1289,   172,  1290,   624,   231,   172,
     541,   392,  1048,   353,  1071,  1010,   177,  1127,  1141,   159,
     177,   177,  1172,   172,   509,   509,  1087,   380,   591,  1090,
     159,    23,    24,   833,   825,   336,  1083,  1075,   774,  1080,
     381,   831,    31,   382,   177,   570,   638,  1268,    37,   177,
    1176,  1277,   713,   755,   393,    42,  1270,   393,  1063,   824,
     928,   403,  1007,  1106,     0,  1108,     0,     0,     0,     0,
    1113,     0,     0,  1116,     0,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,  1122,     0,  1040,  1042,  1044,
       0,     0,     0,   385,     0,    69,     0,     0,  1070,     0,
       0,     0,   328,     0,     0,     0,  1137,     0,     0,     0,
       0,     0,   234,     0,    85,     0,     0,    87,     0,  1071,
      89,  1146,  1140,  1150,     0,     0,  1152,     0,     0,     0,
       0,   540,   540,   336,     0,   540,   540,   897,     0,     0,
       0,     0,     0,     0,  1165,     0,  1166,   540,     0,   540,
       0,   159,     0,     0,     0,   393,     0,   159,   177,     0,
       0,     0,     0,     0,     0,  1183,     0,     0,   159,     0,
     159,     0,   964,     0,   443,  1106,  1108,     0,  1113,  1116,
       0,  1146,  1150,     0,     0,     0,     0,     0,   712,   754,
       0,  1198,  1199,     0,  1200,     0,   774,     0,  1202,  1203,
       0,     0,  1204,  1070,   172,     0,     0,   172,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   336,   336,     0,
     177,     0,     0,     0,     0,   177,     0,     0,     0,  1214,
       0,  1215,   444,     0,     0,  1217,   445,     0,     0,   159,
       0,     0,     0,   159,  1221,   897,   897,   897,   897,     0,
       0,     0,     0,   172,     0,     0,     0,   159,     0,   172,
       0,     0,     0,     0,     0,     0,   172,     0,     0,     0,
       0,     0,     0,     0,     0,  1198,  1247,  1248,  1202,  1249,
    1250,  1251,  1252,     0,   447,   448,     0,   449,   450,   451,
       0,   452,   453,     0,     0,     0,   456,     0,     0,     0,
       0,     0,   177,   463,     0,     0,     0,     0,     0,   467,
     468,   469,     0,  1267,     0,     0,     0,     0,     0,  1269,
       0,   180,     0,   177,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   177,     0,     0,     0,     0,     0,
    1285,  1286,  1287,  1288,  1278,   180,     0,   180,  1291,  1292,
       0,     0,     0,     0,     0,   906,     0,     0,     0,     0,
     443,     0,     0,     0,  1099,  1100,  1285,  1286,  1287,  1288,
    1291,  1292,     0,     0,     0,   172,  -307,   172,     0,     0,
    -307,  -307,   172,     0,     0,   172,     0,     0,     0,  -307,
       0,  -307,  -307,     0,   172,     0,   172,  -307,   180,   172,
       0,     0,   180,   180,  -307,     0,     0,  -307,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   444,     0,
       0,     0,   445,   172,     0,     0,   180,  -307,     0,     0,
    -307,   180,  -307,   172,  -307,   172,  -307,  -307,   159,  -307,
       0,   159,  -307,     0,  -307,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   177,     0,     0,     0,     0,
       0,   177,     0,  -307,     0,     0,  -307,     0,     0,  -307,
     447,   448,   177,     0,   177,   451,     0,   452,   453,     0,
       0,     0,   456,     0,     0,     0,     0,   159,     0,   463,
       0,     0,     0,   159,  -307,   467,   468,   469,     0,     0,
     159,     0,     0,   172,     0,     0,     0,   181,     0,     0,
     172,     0,     0,   380,     0,     0,     0,    23,    24,  -307,
       0,     0,     0,   172,     0,  -307,   381,     0,    31,   382,
       0,   181,     0,   181,    37,     0,     0,   172,     0,     0,
     180,    42,     0,   177,     0,     0,   172,   177,     0,     0,
     172,     0,   172,   172,     0,     0,   443,     0,     0,     0,
       0,   177,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,  1067,     0,     0,  1068,     0,     0,   385,
       0,    69,     0,     0,   181,     0,     0,     0,   181,   181,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      85,     0,   180,    87,   172,     0,    89,   180,     0,   159,
       0,   159,   181,     0,   444,     0,   159,   181,   445,   159,
       0,     0,     0,     0,     0,     0,     0,     0,   159,     0,
     159,    94,     0,   159,     0,     0,     0,     0,     0,     0,
     172,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   172,   172,   172,   172,   105,   159,     0,     0,
     172,   172,  1069,     0,     0,   237,   447,   159,     0,   159,
       0,   451,     0,   452,   453,     0,     0,     0,   456,   238,
     239,     0,   240,     0,   180,   463,     0,   241,     0,     0,
       0,   467,   468,   469,     0,   242,     0,     0,     0,     0,
       0,   243,     0,     0,     0,   180,     0,   244,     0,     0,
       0,   245,     0,     0,   246,     0,   180,     0,     0,     0,
       0,     0,     0,     0,   247,     0,   181,     0,     0,     0,
     186,   248,   249,     0,     0,     0,     0,   159,     0,   250,
       0,     0,   177,     0,   159,   177,     0,   443,     0,   251,
       0,     0,     0,     0,   186,     0,   186,   159,   252,   253,
       0,   254,     0,   255,     0,   256,     0,     0,   257,     0,
       0,   159,   258,   547,     0,   259,     0,     0,   260,     0,
     159,     0,     0,     0,   159,     0,   159,   159,   181,     0,
       0,   177,     0,   181,     0,     0,     0,   177,     0,     0,
       0,     0,     0,     0,   177,   444,     0,   186,     0,   445,
       0,   186,   186,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   548,   186,     0,   180,   159,     0,
     186,     0,     0,   180,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   180,     0,   180,   447,   448,     0,
     449,   450,   451,     0,   452,   453,   454,     0,     0,   456,
     181,     0,     0,     0,   159,   462,   463,     0,     0,   466,
       0,     0,   467,   468,   469,     0,   159,   159,   159,   159,
       0,   181,     0,   470,   159,   159,     0,     0,     0,     0,
       0,     0,   181,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   177,     0,   177,     0,     0,     0,     0,
     177,     0,     0,   177,     0,   180,     0,     0,     0,   180,
       0,     0,   177,     0,   177,     0,     0,   177,     0,     0,
       0,     0,     0,   180,     0,     0,     0,     0,     0,   186,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,   177,     0,   333,     0,     0,     0,   238,   239,     0,
     240,   177,     0,   177,     0,   241,     0,     0,     0,     0,
       0,     0,     0,   242,   333,     0,     0,     0,     0,   243,
       0,     0,     0,     0,     0,   244,     0,     0,     0,   245,
     333,     0,   246,     0,     0,     0,     0,     0,     0,     0,
       0,   186,   247,     0,     0,     0,   186,     0,     0,   248,
     249,     0,     0,   181,     0,     0,     0,   250,     0,   181,
       0,   188,     0,     0,     0,     0,     0,   251,     0,     0,
     181,   177,   181,     0,     0,     0,   252,   253,   177,   254,
       0,   255,     0,   256,     0,   188,   257,   188,     0,     0,
     258,   177,     0,   259,     0,     0,   260,     0,     0,     0,
       0,     0,     0,     0,     0,   177,     0,     0,     0,     0,
       0,     0,     0,     0,   177,     0,     0,     0,   177,     0,
     177,   177,     0,   186,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   188,     0,
       0,   181,   188,   188,   186,   181,     0,     0,     0,     0,
     386,     0,  1029,     0,   180,   186,     0,   180,   333,   181,
       0,     0,     0,     0,     0,   237,   188,     0,     0,     0,
       0,   188,   177,     0,     0,     0,     0,     0,     0,   238,
     239,     0,   240,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,   242,     0,     0,     0,     0,
       0,   243,   333,   180,     0,     0,     0,   244,   177,   180,
       0,   245,     0,     0,   246,     0,   180,     0,     0,     0,
     177,   177,   177,   177,   247,     0,   333,     0,   177,   177,
       0,   248,   249,     0,     0,     0,     0,     0,     0,   250,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   251,
       0,     0,     0,     0,     0,     0,     0,     0,   252,   253,
       0,   254,     0,   255,     0,   256,   333,     0,   257,     0,
       0,     0,   258,   574,     0,   259,   186,     0,   260,     0,
     188,     0,   186,     0,     0,   380,     0,     0,     0,    23,
      24,     0,     0,   186,     0,   186,     0,     0,   381,     0,
      31,   382,     0,   380,     0,     0,    37,    23,    24,     0,
       0,     0,     0,    42,     0,     0,   381,     0,    31,   382,
       0,     0,     0,     0,    37,   180,     0,   180,     0,     0,
       0,    42,   180,     0,   575,   180,     0,     0,     0,    58,
     181,    60,   188,   181,   180,  1067,   180,   188,  1068,   180,
       0,   385,     0,    69,     0,     0,     0,    58,     0,    60,
       0,    62,   189,  1067,   186,     0,  1068,     0,   186,   385,
       0,    69,    85,   180,     0,    87,     0,     0,    89,     0,
       0,     0,   186,   180,     0,   180,   189,     0,   189,   181,
      85,     0,     0,    87,     0,   181,    89,     0,     0,   333,
     333,     0,   181,    94,     0,   333,     0,     0,   333,   333,
     333,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,   188,     0,     0,     0,   105,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   189,
     237,     0,     0,   189,   189,   188,   105,     0,     0,     0,
       0,     0,  1168,   180,   238,   239,   188,   240,     0,     0,
     180,     0,   241,     0,     0,   333,     0,   189,     0,     0,
     242,     0,   189,   180,     0,     0,   243,     0,     0,     0,
       0,     0,   244,     0,     0,     0,   245,   180,     0,   246,
       0,     0,     0,     0,     0,     0,   180,     0,     0,   247,
     180,     0,   180,   180,     0,     0,   248,   249,     0,   333,
       0,   181,     0,   181,   250,     0,   443,     0,   181,     0,
       0,   181,     0,   333,   251,   333,     0,     0,     0,     0,
     181,     0,   181,   252,   253,   181,   254,     0,   255,     0,
     256,     0,   333,   257,     0,     0,     0,   258,     0,     0,
     259,     0,     0,   260,   180,     0,     0,     0,     0,   181,
       0,     0,     0,   186,     0,   145,   186,     0,     0,   181,
       0,   181,     0,     0,   444,     0,     0,   188,   445,     0,
       0,   189,     0,   188,     0,     0,     0,     0,     0,   145,
     180,   145,     0,     0,   188,     0,   188,     0,     0,     0,
       0,     0,   180,   180,   180,   180,     0,     0,     0,   284,
     180,   180,   186,     0,     0,     0,     0,     0,   186,     0,
       0,     0,     0,     0,     0,   186,   447,   448,     0,     0,
     450,   451,     0,   452,   453,     0,     0,     0,   456,   181,
       0,     0,   145,   189,     0,   463,   181,     0,   189,     0,
       0,   467,   468,   469,     0,     0,     0,     0,     0,   181,
       0,     0,     0,     0,     0,   188,     0,     0,     0,   188,
     145,     0,     0,   181,     0,   145,     0,     0,     0,     0,
       0,     0,   181,   188,     0,   333,   181,     0,   181,   181,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   443,     0,   333,     0,   333,     0,     0,     0,
       0,   380,     0,    21,     0,    23,    24,     0,     0,     0,
       0,    28,     0,     0,   381,   189,    31,   382,     0,   383,
     384,    36,    37,     0,   186,     0,   186,     0,     0,    42,
     181,   186,     0,   333,   186,     0,   189,     0,     0,     0,
       0,     0,     0,   186,   -68,   186,     0,   189,   186,     0,
     444,     0,     0,     0,   445,    58,     0,    60,     0,     0,
       0,    64,     0,   -68,    65,     0,   181,   385,     0,    69,
       0,     0,   186,     0,   145,     0,     0,     0,   181,   181,
     181,   181,   186,     0,   186,   446,   181,   181,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,    91,     0,
       0,     0,   447,   448,     0,   449,   450,   451,     0,   452,
     453,   454,     0,   455,   456,   457,   458,   333,   460,   461,
     462,   463,   464,   465,   466,     0,     0,   467,   468,   469,
       0,     0,     0,     0,     0,     0,   145,     0,   470,     0,
       0,   145,     0,     0,   105,     0,     0,     0,     0,   386,
       0,     0,   186,   387,   188,     0,   113,   188,     0,   186,
     333,     0,     0,     0,     0,     0,     0,     0,   189,     0,
       0,     0,   186,     0,   189,   722,     0,     0,     0,     0,
     232,     0,   235,     0,     0,   189,   186,   189,     0,   953,
       0,     0,   954,     0,     0,   186,     0,   955,     0,   186,
       0,   186,   186,   188,     0,     0,     0,   201,     0,   188,
       0,     0,     0,     0,     0,     0,   188,   956,   145,     0,
       0,     0,     0,   443,   957,     0,     0,     0,     0,     0,
       0,     0,     0,   345,   958,     0,     0,     0,     0,   145,
       0,     0,   959,     0,     0,     0,     0,     0,     0,     0,
     145,     0,     0,   186,     0,     0,   189,     0,   333,   960,
     189,   113,     0,   380,     0,     0,   379,    23,    24,     0,
       0,   961,     0,     0,   189,   726,   381,     0,    31,   382,
     441,   444,   962,     0,    37,   445,     0,   442,   963,   186,
       0,    42,     0,     0,     0,     0,     0,     0,     0,   443,
     518,   186,   186,   186,   186,     0,     0,     0,     0,   186,
     186,     0,     0,     0,     0,   519,     0,    58,     0,    60,
       0,   378,     0,  1067,     0,   188,  1068,   188,     0,   385,
       0,    69,   188,   447,   448,   188,   449,   450,   451,     0,
     452,   453,   333,   333,   188,   456,   188,     0,     0,   188,
      85,   462,   463,    87,     0,   466,    89,   444,   467,   468,
     469,   445,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   145,     0,   188,     0,   499,     0,   145,   290,     0,
       0,    94,     0,   188,     0,   188,     0,     0,   145,     0,
     145,     0,   446,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   339,     0,     0,     0,   105,     0,     0,   447,
     448,     0,   449,   450,   451,     0,   452,   453,   454,     0,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,     0,     0,   467,   468,   469,   546,   386,     0,
       0,     0,   557,     0,     0,   470,     0,     0,     0,   443,
       0,     0,     0,   188,     0,   189,     0,     0,   189,   145,
     188,     0,     0,   145,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   188,     0,     0,     0,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   188,     0,     0,
       0,     0,     0,     0,     0,     0,   188,     0,     0,     0,
     188,     0,   188,   188,   189,     0,     0,   444,     0,     0,
     189,   445,     0,     0,   289,     0,     0,   189,     0,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   289,     0,
     113,     0,   446,     0,     0,     0,     0,     0,   349,   351,
       0,   113,     0,     0,   188,     0,     0,     0,     0,   447,
     448,     0,   449,   450,   451,     0,   452,   453,   454,     0,
     455,   456,   457,   458,     0,   460,   461,   462,   463,   464,
       0,   466,   391,     0,   467,   468,   469,     0,     0,     0,
     188,     0,     0,     0,     0,   470,   517,     0,   522,   526,
     528,   531,   188,   188,   188,   188,     0,     0,     0,     0,
     188,   188,     0,     0,     0,     0,     0,     0,     0,     0,
     558,     0,   560,     0,     0,     0,   189,     0,   189,   564,
       0,     0,     0,   189,     0,     0,   189,     0,     0,     0,
       0,     0,     0,     0,     0,   189,     0,   189,     0,     0,
     189,     0,     0,     0,     0,     0,     0,     0,   145,     0,
       0,   145,     0,     0,     0,     0,     0,     0,     0,   586,
       0,     0,   557,     0,   189,     0,     0,     0,   557,     0,
       0,     0,   592,   593,   189,   262,   189,     0,     0,   804,
       0,   501,     0,     0,     0,     0,     0,     0,     0,   281,
     282,   283,     0,     0,     0,     0,     0,   145,     0,     0,
       0,     0,     0,   145,     0,     0,     0,     0,     0,     0,
     145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   289,   289,   289,   289,   289,   289,     0,     0,
     536,   539,     0,     0,     0,     0,     0,   545,     0,     0,
       0,     0,     0,     0,   189,     0,   289,     0,   289,     0,
     853,   189,     0,     0,   858,   289,     0,     0,     0,     0,
       0,     0,     0,     0,   189,     0,     0,     0,   113,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   189,     0,
       0,     0,     0,     0,     0,     0,     0,   189,     0,     0,
       0,   189,     0,   189,   189,   289,     0,     0,   443,     0,
     412,     0,   415,     0,     0,     0,     0,     0,   289,   289,
       0,     0,     0,     0,   434,     0,     0,     0,     0,   145,
       0,   145,   778,     0,     0,     0,   145,     0,     0,   145,
       0,     0,     0,     0,     0,     0,     0,     0,   145,     0,
     145,     0,   792,   145,     0,   189,   797,     0,   798,     0,
     495,   801,     0,     0,     0,     0,   444,     0,     0,     0,
     445,     0,     0,     0,     0,     0,     0,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   145,     0,   145,
       0,   189,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   189,   189,   189,   189,     0,     0,     0,
       0,   189,   189,     0,     0,     0,     0,     0,   447,   448,
     515,   449,   450,   451,     0,   452,   453,   454,   533,   534,
     456,   457,   458,     0,   460,   461,   462,   463,     0,   549,
     466,     0,     0,   467,   468,   469,   526,     0,     0,     0,
       0,     0,     0,     0,   470,     0,     0,   145,     0,   996,
       0,     0,   999,     0,   145,     0,     0,     0,   289,     0,
       0,     0,     0,     0,     0,     0,     0,   145,     0,   576,
       0,     0,     0,     0,     0,     0,     0,     0,   289,     0,
       0,   145,   289,     0,   289,     0,     0,   289,     0,     0,
     145,     0,     0,     0,   145,     0,   145,   145,  1033,     0,
       0,     0,     0,     0,  1038,     0,     0,     0,     0,     0,
       0,   858,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   262,     0,     0,     0,
     471,   472,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,   485,     0,     0,     0,   145,     0,
       0,     0,     0,     0,   639,     0,     0,     0,     0,   415,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   289,   289,     0,   486,     0,     0,  -641,   698,  -641,
       0,     0,     0,     0,   145,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   145,   145,   145,   145,
       0,     0,     0,     0,   145,   145,     0,     0,   977,   979,
       0,     0,     0,     0,   984,   987,     0,     0,   989,   991,
     557,     0,   557,     0,     0,     0,     0,   557,     0,     0,
     557,     0,     0,     0,     0,     0,     0,     0,     0,  1120,
       0,  1121,   766,     0,  1123,   380,     0,     0,     0,    23,
      24,     0,     0,     0,     0,     0,     0,     0,   381,     0,
      31,   382,     0,     0,     0,     0,    37,     0,  1139,     0,
       0,     0,     0,    42,     0,     0,     0,     0,  1145,     0,
    1149,     0,     0,     0,     0,     0,     0,     0,     0,   813,
       0,     0,     0,   549,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,  1067,   826,     0,  1068,     0,
       0,   385,     0,    69,     0,   576,     0,     0,     0,     0,
     549,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,   289,   289,     0,     0,     0,     0,
     289,   289,     0,     0,   289,   289,     0,     0,     0,     0,
     441,     0,     0,    94,  1001,     0,     0,   442,  1206,     0,
       0,     0,     0,     0,     0,   884,     0,     0,     0,   443,
       0,     0,  1211,     0,     0,     0,  1101,  1102,   105,     0,
       0,  1216,  1103,     0,  1227,  1218,     0,  1145,  1149,     0,
    1030,     0,     0,     0,  1114,     0,     0,  1117,     0,  1118,
       0,  1119,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   444,     0,     0,
       0,   445,     0,     0,     0,     0,     0,     0,     0,  1257,
     537,   434,     0,     0,     0,     0,     0,     0,     0,   977,
     979,   984,   987,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   446,   538,   472,   473,   474,   475,   476,     0,
       0,   479,   480,   481,   482,   113,   484,   485,     0,   447,
     448,     0,   449,   450,   451,     0,   452,   453,   454,     0,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,     0,     0,   467,   468,   469,     0,   386,     0,
       0,     0,   289,   289,     0,   470,     0,     0,   289,     0,
     539,     0,     0,     0,     0,     0,     0,   539,     0,     0,
     289,     0,     0,   289,     0,   289,     0,   289,     0,     0,
       0,     0,     0,     0,     0,  1002,     0,     0,     0,  1212,
    1128,     0,   549,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1011,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1143,     0,     0,
    1031,     0,     0,     0,     0,   289,   289,   289,   289,     0,
       0,     0,   914,     0,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,   308,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   204,    39,   434,     0,     0,
     310,     0,     0,    43,   415,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,   698,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,   289,     0,    74,   315,    76,
      77,    78,   316,     0,    80,     0,     0,    82,     0,     0,
       0,    86,     0,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,   813,
       0,   100,     0,     0,   539,   539,     0,     0,   539,   539,
     101,     0,     0,   102,   103,   104,     0,     0,  1260,   106,
     539,   208,   539,   108,  1031,   209,     0,     0,   110,     0,
       0,     0,     0,    -2,     4,     0,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,   415,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -68,
       0,    40,    41,    42,     0,    43,  -327,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -68,    53,
      54,     0,    55,    56,    57,     0,  -327,     0,     0,    58,
      59,    60,    61,    62,    63,    64,  -327,   -68,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -68,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,     0,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
    -672,     0,    13,    14,    15,    16,    17,  -672,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,  -672,
      28,    29,  -672,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -68,     0,    40,    41,    42,     0,
      43,  -327,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -68,    53,    54,     0,    55,    56,    57,
       0,  -327,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -327,   -68,    65,    66,    67,    68,  -672,    69,    70,
      71,  -672,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -68,    89,    90,     0,     0,    91,     0,    92,
       0,     0,  -672,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
    -672,    97,  -672,  -672,  -672,  -672,  -672,  -672,  -672,     0,
    -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,  -672,
    -672,  -672,   104,   105,  -672,  -672,  -672,     0,   107,  -672,
     108,     0,   109,     0,   362,  -672,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -68,
       0,    40,    41,    42,     0,    43,  -327,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -68,    53,
      54,     0,    55,    56,    57,     0,  -327,     0,     0,    58,
      59,    60,    61,    62,    63,    64,  -327,   -68,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -68,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,   363,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -68,     0,    40,    41,    42,     0,
      43,  -327,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -68,    53,    54,     0,    55,    56,    57,
       0,  -327,     0,     0,    58,    59,    60,    61,    62,    63,
      64,  -327,   -68,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -68,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,   596,
     108,     0,   109,     0,   622,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -68,
       0,    40,    41,    42,     0,    43,  -327,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -68,    53,
      54,     0,    55,    56,    57,     0,  -327,     0,     0,    58,
      59,    60,    61,    62,    63,    64,  -327,   -68,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -68,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,   623,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -68,     0,    40,    41,    42,     0,
      43,  -327,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -68,    53,    54,     0,    55,    56,    57,
       0,  -327,     0,     0,    58,    59,    60,    61,    62,    63,
      64,  -327,   -68,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -68,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,   874,
     108,     0,   109,     0,     4,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -68,
       0,    40,    41,    42,     0,    43,  -327,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -68,    53,
      54,     0,    55,    56,    57,     0,  -327,     0,     0,    58,
      59,    60,    61,   378,    63,    64,  -327,   -68,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -68,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,     0,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -68,     0,    40,    41,    42,     0,
      43,  -327,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -68,    53,    54,     0,    55,    56,    57,
       0,  -327,     0,     0,    58,    59,    60,    61,    62,    63,
      64,  -327,   -68,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -68,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     4,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -68,
       0,    40,    41,    42,     0,    43,  -327,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -68,    53,
      54,     0,    55,    56,    57,     0,  -327,     0,     0,    58,
      59,    60,    61,     0,    63,    64,  -327,   -68,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -68,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,     0,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,   787,    39,   -68,     0,    40,    41,    42,     0,
      43,  -327,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -68,    53,    54,     0,    55,    56,    57,
       0,  -327,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -327,   -68,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -68,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     4,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,   793,    39,   -68,
       0,    40,    41,    42,     0,    43,  -327,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -68,    53,
      54,     0,    55,    56,    57,     0,  -327,     0,     0,    58,
      59,    60,    61,     0,    63,    64,  -327,   -68,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -68,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,     0,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,  1105,    39,   -68,     0,    40,    41,    42,     0,
      43,  -327,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -68,    53,    54,     0,    55,    56,    57,
       0,  -327,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -327,   -68,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -68,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     4,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,  1107,    39,   -68,
       0,    40,    41,    42,     0,    43,  -327,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -68,    53,
      54,     0,    55,    56,    57,     0,  -327,     0,     0,    58,
      59,    60,    61,     0,    63,    64,  -327,   -68,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -68,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,     0,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,  1112,    39,   -68,     0,    40,    41,    42,     0,
      43,  -327,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -68,    53,    54,     0,    55,    56,    57,
       0,  -327,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -327,   -68,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -68,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     4,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,  1115,    39,   -68,
       0,    40,    41,    42,     0,    43,  -327,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -68,    53,
      54,     0,    55,    56,    57,     0,  -327,     0,     0,    58,
      59,    60,    61,     0,    63,    64,  -327,   -68,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -68,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,     0,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
    1138,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -68,     0,    40,    41,    42,     0,
      43,  -327,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -68,    53,    54,     0,    55,    56,    57,
       0,  -327,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -327,   -68,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -68,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     4,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,  1144,    39,   -68,
       0,    40,    41,    42,     0,    43,  -327,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -68,    53,
      54,     0,    55,    56,    57,     0,  -327,     0,     0,    58,
      59,    60,    61,     0,    63,    64,  -327,   -68,    65,    66,
      67,    68,     0,    69,    70,    71,     0,    72,    73,    74,
      75,    76,    77,    78,    79,     0,    80,    81,     0,    82,
      83,    84,    85,    86,     0,    87,    88,   -68,    89,    90,
       0,     0,    91,     0,    92,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,   105,     0,
       0,   106,     0,   107,     0,   108,     0,   109,     0,     4,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,  1148,    39,   -68,     0,    40,    41,    42,     0,
      43,  -327,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -68,    53,    54,     0,    55,    56,    57,
       0,  -327,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -327,   -68,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,   -68,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,   660,     0,    13,     0,     0,
      16,    17,   662,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,   667,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,   538,   472,
     473,   474,   475,   476,     0,     0,   479,   480,   481,   482,
       0,   484,   485,     0,   885,   886,   887,   888,   889,   680,
       0,   681,     0,   100,     0,   682,   683,   684,   685,   686,
     687,   688,   890,   690,   691,   102,   891,   104,     0,   693,
     694,   892,   696,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,     0,    16,    17,     0,    18,     0,
     200,    20,    21,    22,     0,     0,     0,     0,    27,     0,
      28,    29,     0,   201,     0,     0,     0,    33,    34,    35,
      36,     0,    38,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,    49,
      50,    51,    52,     0,    53,    54,     0,    55,    56,    57,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,    91,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,   402,
      24,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,    60,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,    69,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,    89,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,   330,   331,     0,    86,   367,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,   368,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
     767,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
     768,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,   769,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,   770,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,   330,   331,     0,    86,   367,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,   849,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
     330,   331,     0,    86,   367,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,   851,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,   330,   331,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   332,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
     330,   331,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,   869,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,  1009,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,   330,   331,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,   265,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,   266,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,   273,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,   274,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
     330,   331,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
     266,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,   372,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,  -716,     0,     0,     0,  -716,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
     266,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,   288,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   107,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,   405,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
      19,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,    68,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
      87,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,   372,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,   847,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,   857,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,   862,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,  1053,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,  1220,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,   308,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,   310,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,   315,    76,    77,    78,   316,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   939,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   942,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,  1189,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,  1190,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,  1192,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,  1193,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,  1194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,    98,    99,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   103,   104,     0,     0,
       0,   106,     0,   208,     0,   108,     0,   209,     0,     0,
     110,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,  1195,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   205,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   207,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,     0,
      17,     0,     0,     0,   200,     0,     0,     0,     0,   307,
       0,     0,     0,     0,     0,     0,     0,   201,     0,     0,
       0,     0,   202,   203,     0,     0,   204,     0,     0,   309,
       0,     0,     0,   311,     0,     0,     0,   205,     0,     0,
      47,    48,     0,     0,     0,     0,    52,     0,    53,    54,
       0,     0,     0,     0,     0,     0,     0,   312,     0,    59,
     313,    61,    62,    63,     0,     0,     0,     0,    66,     0,
     206,     0,   314,     0,     0,     0,     0,     0,    74,     0,
      76,     0,    78,     0,     0,     0,     0,     0,     0,     0,
       0,   317,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,  -331,   209,     0,     0,   110,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,     0,    17,     0,     0,     0,   200,     0,
       0,     0,     0,   307,     0,     0,     0,     0,     0,     0,
       0,   201,     0,     0,     0,     0,   202,   203,     0,     0,
     204,     0,     0,   309,     0,     0,     0,   311,     0,     0,
       0,   205,     0,     0,    47,    48,     0,     0,     0,     0,
      52,     0,    53,    54,     0,     0,     0,     0,     0,     0,
       0,   312,     0,    59,   313,    61,    62,    63,     0,     0,
       0,     0,    66,     0,   206,     0,   314,     0,     0,     0,
       0,     0,    74,     0,    76,     0,    78,     0,     0,     0,
       0,     0,     0,     0,     0,   317,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   658,     0,   659,     0,    95,    96,    97,
      98,    99,     0,     0,   660,     0,   100,     0,     0,   661,
     239,   662,   663,     0,     0,   101,     0,   664,   102,   103,
     104,     0,     0,     0,   106,   242,   208,   201,   108,     0,
     209,   243,     0,   110,     0,     0,     0,   665,     0,     0,
       0,   245,     0,     0,   666,     0,   667,     0,     0,     0,
       0,     0,     0,     0,   668,     0,     0,     0,     0,     0,
       0,   248,   669,     0,     0,     0,     0,     0,     0,   250,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   670,
       0,     0,     0,     0,     0,     0,     0,     0,   252,   253,
       0,   671,     0,   255,     0,   672,     0,     0,   257,     0,
       0,     0,   673,     0,     0,   259,     0,     0,   674,     0,
       0,     0,     0,     0,     0,     0,     0,   538,   472,   473,
     474,   475,   476,     0,     0,   479,   480,   481,   482,     0,
     484,   485,     0,   675,   676,   677,   678,   679,   680,     0,
     681,     0,     0,     0,   682,   683,   684,   685,   686,   687,
     688,   689,   690,   691,     0,   692,     0,     0,   693,   694,
     695,   696,     0,     0,   697,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   200,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   201,     0,     0,
       0,    33,   202,   203,     0,     0,   204,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   205,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     206,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   207,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,     0,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,     0,   209,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   200,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   201,
       0,     0,     0,    33,   202,   203,     0,     0,   204,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   205,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   206,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   207,     0,    80,     0,     0,
      82,     0,     0,     0,    86,     0,     0,    88,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,    97,     0,     0,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,   104,   200,
       0,     0,    22,     0,   208,     0,   108,     0,   209,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   204,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,   722,     0,    59,     0,    61,     0,    63,     0,
       0,   660,     0,    66,    67,   206,     0,     0,   662,   723,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,   724,    82,     0,     0,     0,    86,     0,     0,
      88,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   667,     0,     0,     0,     0,     0,     0,
       0,   725,     0,     0,     0,     0,     0,     0,    95,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   104,     0,     0,     0,     0,     0,     0,     0,   108,
       0,   209,   726,     0,     0,     0,     0,     0,   727,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   538,   472,   473,   474,   475,   476,
       0,     0,   479,   480,   481,   482,     0,   484,   485,     0,
     675,   676,   677,   678,   679,   680,     0,   681,     0,     0,
       0,   682,   683,   684,   685,   686,   687,   688,   689,   690,
     691,   237,   692,     0,     0,   693,   694,   695,   696,  1050,
     660,     0,     0,     0,     0,   238,   239,   662,   240,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,   242,     0,     0,     0,     0,     0,   243,     0,     0,
       0,     0,     0,   244,     0,     0,     0,   245,     0,     0,
     246,     0,   667,     0,     0,     0,     0,     0,     0,     0,
     247,     0,     0,     0,     0,     0,     0,   248,   249,     0,
       0,     0,     0,     0,     0,   250,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   251,     0,     0,     0,     0,
       0,   441,     0,     0,   252,   253,     0,   254,   442,   255,
       0,   256,     0,     0,   257,     0,     0,     0,   258,     0,
     443,   259,     0,     0,   260,     0,     0,     0,     0,     0,
       0,     0,     0,   538,   472,   473,   474,   475,   476,     0,
       0,   479,   480,   481,   482,     0,   484,   485,     0,   675,
     676,   677,   678,   679,   680,     0,   681,     0,     0,     0,
     682,   683,   684,   685,   686,   687,   688,   689,   690,   691,
       0,   692,     0,     0,   693,   694,   695,   696,   444,     0,
       0,   441,   445,     0,     0,     0,     0,     0,   442,     0,
       0,  1104,     0,     0,     0,     0,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   446,   538,   472,   473,   474,   475,   476,
       0,     0,   479,   480,   481,   482,     0,   484,   485,     0,
     447,   448,     0,   449,   450,   451,     0,   452,   453,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,     0,     0,   467,   468,   469,   444,   386,
       0,   441,   445,     0,     0,     0,   470,     0,   442,     0,
       0,  1111,     0,     0,     0,     0,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   446,   538,   472,   473,   474,   475,   476,
       0,     0,   479,   480,   481,   482,     0,   484,   485,     0,
     447,   448,     0,   449,   450,   451,     0,   452,   453,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,     0,     0,   467,   468,   469,   444,   386,
       0,   441,   445,     0,     0,     0,   470,     0,   442,     0,
       0,  1253,     0,     0,     0,     0,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   446,   538,   472,   473,   474,   475,   476,
       0,     0,   479,   480,   481,   482,     0,   484,   485,     0,
     447,   448,     0,   449,   450,   451,     0,   452,   453,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,     0,     0,   467,   468,   469,   444,   386,
       0,   441,   445,     0,     0,     0,   470,     0,   442,     0,
       0,  1254,     0,     0,     0,     0,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   446,   538,   472,   473,   474,   475,   476,
       0,     0,   479,   480,   481,   482,     0,   484,   485,     0,
     447,   448,     0,   449,   450,   451,     0,   452,   453,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,     0,     0,   467,   468,   469,   444,   386,
       0,   441,   445,     0,     0,     0,   470,     0,   442,     0,
       0,  1255,     0,     0,     0,     0,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   446,   538,   472,   473,   474,   475,   476,
       0,     0,   479,   480,   481,   482,     0,   484,   485,     0,
     447,   448,     0,   449,   450,   451,     0,   452,   453,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,     0,     0,   467,   468,   469,   444,   386,
       0,   441,   445,     0,     0,     0,   470,     0,   442,     0,
       0,  1256,     0,     0,     0,     0,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   446,   538,   472,   473,   474,   475,   476,
       0,     0,   479,   480,   481,   482,     0,   484,   485,     0,
     447,   448,     0,   449,   450,   451,     0,   452,   453,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,     0,     0,   467,   468,   469,   444,   386,
       0,   441,   445,     0,     0,     0,   470,     0,   442,     0,
       0,  1261,     0,     0,     0,     0,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   446,   538,   472,   473,   474,   475,   476,
       0,     0,   479,   480,   481,   482,     0,   484,   485,     0,
     447,   448,     0,   449,   450,   451,     0,   452,   453,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,     0,     0,   467,   468,   469,   444,   386,
       0,     0,   445,     0,     0,     0,   470,     0,     0,     0,
       0,  1262,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   441,     0,     0,     0,     0,     0,     0,   442,     0,
       0,     0,     0,   446,   538,   472,   473,   474,   475,   476,
     443,   524,   479,   480,   481,   482,     0,   484,   485,     0,
     447,   448,     0,   449,   450,   451,   525,   452,   453,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,     0,     0,   467,   468,   469,     0,   386,
       0,     0,     0,     0,     0,     0,   470,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   441,   444,     0,
       0,     0,   445,     0,   442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   443,   288,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   521,
       0,     0,   530,   446,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     447,   448,     0,   449,   450,   451,     0,   452,   453,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   441,   444,   467,   468,   469,   445,   386,
     442,     0,     0,     0,     0,     0,   470,     0,     0,     0,
       0,     0,   443,   288,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   521,     0,     0,   865,   446,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,   448,     0,   449,
     450,   451,     0,   452,   453,   454,     0,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,     0,
     444,   467,   468,   469,   445,   386,     0,     0,     0,     0,
       0,     0,   470,     0,     0,   441,     0,     0,     0,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
       0,   521,     0,     0,   443,   446,     0,     0,     0,   631,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   447,   448,     0,   449,   450,   451,     0,   452,
     453,   454,     0,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   632,     0,   467,   468,   469,
       0,   386,     0,     0,     0,     0,     0,     0,   470,     0,
       0,     0,   444,     0,     0,   441,   445,     0,     0,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   443,   288,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   446,     0,     0,
     864,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   447,   448,     0,   449,   450,   451,
       0,   452,   453,   454,     0,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,     0,   633,   467,
     468,   469,   444,     0,     0,     0,   445,     0,     0,     0,
     470,     0,     0,     0,     0,     0,     0,   441,   881,     0,
       0,     0,     0,     0,   442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   443,   446,     0,     0,
       0,   882,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   447,   448,     0,   449,   450,   451,
       0,   452,   453,   454,     0,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   883,     0,   467,
     468,   469,     0,   386,     0,     0,     0,     0,     0,     0,
     470,     0,     0,   441,   444,     0,     0,     0,   445,     0,
     442,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   443,   986,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   446,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,   448,     0,   449,
     450,   451,     0,   452,   453,   454,     0,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   441,
     444,   467,   468,   469,   445,     0,   442,     0,     0,     0,
       0,     0,   470,     0,     0,     0,     0,     0,   443,   288,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   521,     0,     0,     0,   446,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   447,   448,     0,   449,   450,   451,     0,   452,
     453,   454,     0,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   441,   444,   467,   468,   469,
     445,   386,   442,     0,     0,     0,     0,     0,   470,     0,
       0,     0,     0,     0,   443,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   521,     0,     0,
     502,   446,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   447,   448,
       0,   449,   450,   451,     0,   452,   453,   454,     0,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   441,   444,   467,   468,   469,   445,   386,   442,     0,
       0,     0,     0,     0,   470,     0,     0,     0,     0,     0,
     443,   288,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   220,     0,     0,     0,   446,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   447,   448,     0,   449,   450,   451,
       0,   452,   453,   454,     0,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   441,   444,   467,
     468,   469,   445,     0,   442,     0,     0,     0,     0,     0,
     470,     0,     0,     0,     0,     0,   443,   758,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   759,   446,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     447,   448,     0,   449,   450,   451,     0,   452,   453,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   441,   444,   467,   468,   469,   445,   386,
     442,     0,     0,     0,     0,     0,   470,     0,     0,     0,
       0,     0,   443,   760,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   761,   446,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,   448,     0,   449,
     450,   451,     0,   452,   453,   454,     0,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   441,
     444,   467,   468,   469,   445,     0,   442,     0,     0,     0,
       0,     0,   470,     0,     0,     0,     0,     0,   443,   978,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   446,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   447,   448,     0,   449,   450,   451,     0,   452,
     453,   454,     0,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   441,   444,   467,   468,   469,
     445,     0,   442,     0,     0,     0,     0,     0,   470,     0,
       0,     0,     0,     0,   443,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   446,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   447,   448,
       0,   449,   450,   451,     0,   452,   453,   454,     0,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   441,   444,   467,   468,   469,   445,   386,   442,     0,
       0,     0,     0,     0,   470,  1000,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   446,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   447,   448,     0,   449,   450,   451,
       0,   452,   453,   454,     0,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   441,   444,   467,
     468,   469,   445,   386,   442,     0,     0,     0,     0,     0,
     470,     0,     0,     0,     0,     0,   443,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   521,
       0,     0,     0,   446,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     447,   448,     0,   449,   450,   451,     0,   452,   453,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,     0,   444,   467,   468,   469,   445,     0,
       0,     0,     0,     0,  1036,     0,   470,     0,     0,   441,
       0,     0,     0,     0,     0,     0,   442,     0,     0,     0,
       0,     0,     0,     0,     0,   521,     0,     0,   443,   446,
       0,     0,     0,  1160,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,   448,     0,   449,
     450,   451,     0,   452,   453,   454,     0,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,  1161,
       0,   467,   468,   469,     0,     0,     0,     0,     0,     0,
    1154,     0,   470,     0,     0,     0,   444,     0,     0,     0,
     445,   441,   561,     0,     0,     0,     0,     0,   442,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   446,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   447,   448,
       0,   449,   450,   451,     0,   452,   453,   454,     0,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,     0,     0,   467,   468,   469,     0,     0,   444,     0,
       0,   441,   445,     0,   470,     0,     0,     0,   442,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   446,     0,     0,     0,     0,     0,     0,
       0,   441,     0,     0,     0,     0,     0,     0,   442,     0,
     447,   448,     0,   449,   450,   451,     0,   452,   453,   454,
     443,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,     0,     0,   467,   468,   469,   444,     0,
       0,     0,   445,     0,     0,     0,   470,   441,     0,     0,
       0,     0,     0,     0,   442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   443,     0,     0,     0,
       0,     0,     0,   446,     0,     0,     0,     0,   444,     0,
       0,     0,   445,     0,     0,     0,     0,     0,     0,     0,
     447,   448,     0,   449,   450,   451,     0,   452,   453,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   446,     0,   467,   468,   469,     0,   585,
       0,     0,     0,     0,   444,     0,   470,     0,   445,     0,
     447,   448,     0,   449,   450,   451,     0,   452,   453,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   441,   589,   467,   468,   469,   612,   446,
     442,     0,     0,     0,     0,     0,   470,     0,     0,     0,
       0,     0,   443,     0,     0,     0,   447,   448,     0,   449,
     450,   451,     0,   452,   453,   454,     0,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   441,
       0,   467,   468,   469,     0,     0,   442,     0,     0,     0,
       0,     0,   470,     0,     0,     0,     0,     0,   443,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,   445,     0,     0,     0,     0,   441,
     877,     0,     0,   762,     0,     0,   442,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   443,     0,
       0,     0,     0,     0,     0,   446,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
     445,     0,   447,   448,     0,   449,   450,   451,     0,   452,
     453,   454,     0,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,     0,     0,   467,   468,   469,
       0,   446,     0,     0,     0,   441,   444,     0,   470,     0,
     445,     0,   442,     0,     0,     0,     0,     0,   447,   448,
       0,   449,   450,   451,   443,   452,   453,   454,     0,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   446,     0,   467,   468,   469,     0,     0,     0,     0,
     848,     0,     0,     0,   470,     0,     0,     0,   447,   448,
       0,   449,   450,   451,     0,   452,   878,   454,     0,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   441,   444,   467,   468,   469,   445,     0,   442,     0,
       0,     0,     0,     0,   470,     0,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   220,     0,     0,     0,   446,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   447,   448,     0,   449,   450,   451,
       0,   452,   453,   454,     0,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,     0,   444,   467,
     468,   469,   445,     0,   441,     0,     0,     0,     0,     0,
     470,   442,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   443,     0,     0,   998,     0,     0,     0,
       0,     0,     0,   446,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     447,   448,     0,   449,   450,   451,     0,   452,   453,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,     0,   970,   467,   468,   469,     0,     0,
     441,   444,     0,     0,     0,   445,   470,   442,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   443,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   446,     0,     0,     0,
     441,     0,     0,     0,     0,     0,     0,   442,     0,     0,
       0,     0,     0,   447,   448,     0,   449,   450,   451,   443,
     452,   453,   454,     0,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,     0,   444,   467,   468,
     469,   445,     0,     0,     0,     0,   441,     0,     0,   470,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   443,     0,     0,     0,     0,
       0,     0,   446,     0,     0,     0,     0,   444,     0,     0,
       0,   445,     0,     0,     0,     0,     0,     0,     0,   447,
     448,     0,   449,   450,   451,     0,   452,   453,   454,     0,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   446,     0,   467,   468,   469,     0,     0,     0,
       0,  1012,     0,   444,     0,   470,     0,   445,     0,   447,
     448,     0,   449,   450,   451,     0,   452,   453,   454,     0,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   441,  1059,   467,   468,   469,  1039,   446,   442,
       0,  1013,     0,     0,     0,   470,     0,     0,     0,     0,
       0,   443,     0,     0,     0,   447,   448,     0,   449,   450,
     451,     0,   452,   453,   454,     0,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   441,     0,
     467,   468,   469,     0,     0,   442,     0,     0,     0,     0,
       0,   470,     0,     0,     0,     0,     0,   443,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   444,
       0,     0,     0,   445,     0,     0,     0,     0,   441,     0,
       0,     0,     0,     0,     0,   442,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   443,  1089,     0,
       0,     0,     0,     0,   446,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   444,     0,     0,     0,   445,
       0,   447,   448,     0,   449,   450,   451,     0,   452,   453,
     454,     0,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,     0,     0,   467,   468,   469,     0,
     446,     0,     0,     0,   441,   444,     0,   470,     0,   445,
       0,   442,     0,     0,     0,     0,     0,   447,   448,     0,
     449,   450,   451,   443,   452,   453,   454,     0,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     446,     0,   467,   468,   469,     0,     0,     0,     0,  1082,
       0,     0,     0,   470,     0,     0,     0,   447,   448,     0,
     449,   450,   451,     0,   452,   453,   454,     0,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     441,   444,   467,   468,   469,   445,     0,   442,     0,     0,
       0,     0,     0,   470,     0,     0,     0,     0,     0,   443,
    1092,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   446,     0,     0,     0,
     441,  1157,     0,     0,     0,     0,     0,   442,     0,     0,
       0,     0,     0,   447,   448,     0,   449,   450,   451,   443,
     452,   453,   454,     0,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,     0,   444,   467,   468,
     469,   445,     0,     0,     0,   441,     0,  1095,     0,   470,
       0,     0,   442,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   443,     0,     0,     0,     0,     0,
       0,     0,   446,     0,     0,     0,     0,   444,     0,     0,
       0,   445,     0,     0,     0,     0,     0,     0,     0,   447,
     448,     0,   449,   450,   451,     0,   452,   453,   454,     0,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   446,     0,   467,   468,   469,     0,     0,     0,
       0,     0,   444,     0,     0,   470,   445,     0,     0,   447,
     448,     0,   449,   450,   451,  1188,   452,   453,   454,     0,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   441,     0,   467,   468,   469,   446,     0,   442,
       0,     0,     0,     0,     0,   470,     0,     0,     0,     0,
       0,   443,     0,     0,   447,   448,     0,   449,   450,   451,
       0,   452,   453,   454,     0,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   441,  1226,   467,
     468,   469,     0,     0,   442,     0,     0,     0,     0,     0,
     470,     0,     0,     0,     0,     0,   443,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   444,
       0,     0,     0,   445,     0,     0,     0,     0,     0,     0,
       0,     0,  1191,   441,     0,     0,     0,     0,     0,     0,
     442,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   443,     0,   446,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,     0,   445,     0,
       0,   447,   448,     0,   449,   450,   451,     0,   452,   453,
     454,  1240,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,     0,     0,   467,   468,   469,   446,
       0,     0,     0,     0,     0,     0,     0,   470,     0,     0,
     444,     0,     0,     0,   445,     0,   447,   448,     0,   449,
     450,   451,     0,   452,   453,   454,     0,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   441,
       0,   467,   468,   469,     0,   446,   442,     0,     0,     0,
       0,     0,   470,     0,     0,     0,     0,     0,   443,     0,
       0,     0,   447,   448,     0,   449,   450,   451,     0,   452,
     453,   454,     0,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,     0,     0,   467,   468,   469,
       0,     0,     0,     0,     0,     0,     0,     0,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   441,   444,     0,     0,     0,
     445,     0,   442,     0,     0,     0,     0,     0,     0,  1279,
       0,     0,     0,     0,   443,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   446,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   447,   448,
       0,   449,   450,   451,     0,   452,   453,   454,     0,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   441,   444,   467,   468,   469,   445,     0,   442,     0,
       0,     0,     0,     0,   470,  1280,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   446,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   447,   448,     0,   449,   450,   451,
       0,   452,   453,   454,     0,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   441,   444,   467,
     468,   469,   445,     0,   442,     0,     0,     0,     0,     0,
     470,  1281,     0,     0,     0,     0,   443,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   446,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     447,   448,     0,   449,   450,   451,     0,   452,   453,   454,
       0,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   441,   444,   467,   468,   469,   445,     0,
     442,     0,     0,     0,     0,     0,   470,  1282,     0,     0,
       0,     0,   443,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   446,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,   448,     0,   449,
     450,   451,     0,   452,   453,   454,     0,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   441,
     444,   467,   468,   469,   445,     0,   442,     0,     0,     0,
       0,     0,   470,  1283,     0,     0,     0,     0,   443,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   446,     0,     0,     0,   441,
       0,     0,     0,     0,     0,     0,   442,     0,     0,     0,
       0,     0,   447,   448,     0,   449,   450,   451,   443,   452,
     453,   454,     0,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,     0,   444,   467,   468,   469,
     445,     0,     0,     0,   441,     0,     0,     0,   470,  1284,
       0,   442,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   443,     0,     0,     0,     0,     0,     0,
       0,   446,     0,     0,     0,     0,   444,     0,     0,     0,
     445,     0,     0,     0,     0,     0,     0,     0,   447,   448,
       0,   449,   450,   451,     0,   452,   453,   454,     0,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   446,     0,   467,   468,   469,     0,     0,     0,     0,
       0,   965,     0,     0,   470,   445,     0,     0,   447,   448,
       0,   449,   450,   451,     0,   452,   453,   454,     0,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,     0,     0,   467,   468,   469,   446,     0,     0,     0,
       0,     0,     0,     0,   470,     0,     0,     0,     0,     0,
       0,     0,     0,   447,   448,     0,   449,   450,   451,     0,
     452,   453,   454,     0,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,     0,     0,   467,   468,
     469,   413,     0,   237,     0,     0,     0,     0,     0,   470,
       0,     0,     0,     0,     0,     0,     0,   238,   239,     0,
     240,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,   242,     0,     0,     0,     0,     0,   243,
       0,     0,     0,     0,     0,   244,     0,     0,     0,   245,
       0,     0,   246,     0,     0,     0,     0,     0,   237,     0,
       0,     0,   247,     0,     0,     0,     0,     0,     0,   248,
     249,     0,   238,   239,     0,   240,     0,   250,     0,     0,
     241,     0,     0,     0,     0,     0,     0,   251,   242,     0,
       0,     0,     0,     0,   243,     0,   252,   253,     0,   254,
     244,   255,     0,   256,   245,     0,   257,   246,     0,     0,
     258,     0,     0,   259,     0,     0,   260,   247,     0,     0,
       0,     0,     0,     0,   248,   249,     0,     0,     0,     0,
       0,     0,   250,   237,     0,     0,     0,     0,     0,     0,
      94,     0,   251,     0,     0,     0,     0,   238,   239,     0,
     240,   252,   253,     0,   254,   241,   255,    24,   256,     0,
       0,   257,     0,   242,     0,   258,     0,     0,   259,   243,
       0,   260,     0,     0,     0,   244,     0,     0,     0,   245,
       0,     0,   246,     0,     0,     0,     0,     0,   237,     0,
       0,     0,   247,     0,     0,    94,     0,     0,     0,   248,
     249,     0,   238,   239,     0,   240,     0,   250,    60,     0,
     241,     0,     0,     0,     0,     0,     0,   251,   242,     0,
      69,     0,     0,     0,   243,     0,   252,   253,     0,   254,
     244,   255,     0,   256,   245,     0,   257,   246,     0,     0,
     258,     0,     0,   259,     0,    89,   260,   247,     0,     0,
       0,     0,     0,     0,   248,   249,     0,     0,     0,     0,
       0,     0,   250,  -302,     0,     0,     0,     0,     0,     0,
       0,     0,   251,     0,     0,     0,     0,  -302,  -302,     0,
    -302,   252,   253,     0,   254,  -302,   255,     0,   256,     0,
       0,   257,     0,  -302,     0,   258,   574,     0,   259,  -302,
       0,   260,     0,     0,     0,  -302,     0,     0,     0,  -302,
       0,     0,  -302,     0,     0,     0,     0,     0,   237,     0,
       0,     0,  -302,     0,     0,     0,     0,     0,     0,  -302,
    -302,     0,   238,   239,     0,   240,     0,  -302,     0,     0,
     241,     0,     0,     0,     0,     0,     0,  -302,   242,     0,
       0,     0,     0,     0,   243,     0,  -302,  -302,     0,  -302,
     244,  -302,     0,  -302,   245,     0,  -302,   246,     0,     0,
    -302,     0,     0,  -302,     0,     0,  -302,   247,     0,     0,
       0,     0,     0,     0,   248,   249,     0,     0,     0,     0,
       0,     0,   250,  -303,     0,     0,     0,     0,     0,     0,
       0,     0,   251,     0,     0,     0,     0,  -303,  -303,     0,
    -303,   252,   253,     0,   254,  -303,   255,     0,   256,     0,
       0,   257,     0,  -303,     0,   258,     0,     0,   259,  -303,
       0,   260,     0,     0,     0,  -303,     0,     0,     0,  -303,
       0,     0,  -303,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -303,     0,     0,     0,     0,     0,     0,  -303,
    -303,     0,     0,     0,     0,     0,     0,  -303,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -303,     0,     0,
       0,     0,     0,     0,     0,     0,  -303,  -303,     0,  -303,
       0,  -303,     0,  -303,     0,     0,  -303,     0,     0,     0,
    -303,     0,     0,  -303,     0,     0,  -303
};

static const yytype_int16 yycheck[] =
{
      13,   118,    66,   414,    17,     2,   170,   280,    21,   157,
      20,    66,    66,   498,    27,   229,   654,   433,   186,   583,
     791,    34,    35,    36,    27,    38,   700,   837,   702,   543,
     236,   304,   587,   651,   883,   578,    49,     1,    51,    52,
     406,   139,   831,   878,    57,  1014,    59,   901,    61,    32,
     419,    55,     8,    66,   913,   553,   907,    70,    71,    72,
      73,    74,    75,     1,   631,   620,    79,    70,    81,    60,
      32,   344,    32,    40,   280,    88,    47,   102,    91,    92,
      27,     3,    95,    32,    97,    22,    47,   100,   101,     1,
     103,    27,    32,   123,   107,   108,   109,   110,   304,     3,
      47,     1,    32,    32,    51,     0,   109,  1184,   276,   141,
       1,    47,    47,    36,   165,    51,    32,   149,   118,   120,
     269,    55,   271,   272,   137,   120,   139,   118,    75,    90,
      89,    78,    32,    80,    93,    47,    74,   120,   344,    75,
     170,    32,    78,    90,    80,   170,    55,   147,    47,    72,
     117,    88,    92,  1230,    90,    90,   654,  1016,    35,    36,
      60,   125,   109,    92,     3,    71,   179,  1136,   172,    60,
      92,   120,    47,   109,   175,   112,   147,    47,   165,   117,
     951,   103,   146,   147,   171,   147,    92,   170,   120,   202,
     203,   204,    92,   149,   207,   208,   209,     3,  1008,   103,
     818,    92,   108,   572,  1014,   105,   209,  1061,   170,   173,
     170,  1062,    27,   125,   105,   488,   489,   227,   118,  1070,
    1071,   170,  1057,   236,   373,   374,   173,   118,  1017,   784,
     170,   147,   109,   171,   146,   147,   165,   173,   172,     3,
     170,   170,    55,   175,    74,   120,   146,   453,   147,   763,
     120,    31,    46,   165,   170,   146,    20,   147,    83,    84,
      27,    46,   548,    78,   103,   433,    53,   280,   634,    33,
     170,    27,   147,   120,   165,    90,   175,   147,   147,   170,
      65,    55,   488,   489,    18,   175,    73,   117,  1098,   575,
      84,   304,   835,   147,   109,    46,    83,   414,    62,    84,
     175,     2,   315,   316,   173,   175,   791,   405,    55,    89,
    1159,    78,  1161,    93,    65,   882,  1167,   330,   331,   173,
     114,  1172,    78,    90,   147,    26,  1136,    28,   175,   114,
     147,   344,   125,    84,    90,    90,   147,   543,  1109,   103,
     504,   171,  1178,    77,   140,   109,   964,   561,   115,   147,
     173,   147,   147,   109,   418,   368,   173,    11,   151,   372,
     171,   141,    96,   114,   147,   147,   146,   165,   365,   149,
     383,   384,   109,   153,   387,   171,   171,  1015,    79,   113,
     172,   587,    83,    84,   387,   398,   125,   400,    55,   171,
     173,   905,    89,    55,   147,   147,    93,   410,  1234,  1235,
    1236,  1237,  1238,  1239,   147,   165,   107,   410,     8,  1083,
    1084,   112,   172,   152,   620,   154,   155,    55,   157,   158,
     173,   173,   165,   637,    82,    55,    31,    85,   441,   442,
     443,   444,   445,   446,   948,   448,   449,   450,   451,   452,
     147,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   165,   172,
     147,   147,   147,   147,   477,   478,   951,    55,   147,   618,
     483,   147,    55,   486,   487,   488,   489,   490,   491,   172,
     165,   165,    55,  1050,    89,   498,   173,   173,    93,   502,
     147,  1046,   100,   506,   173,   508,   147,   173,   909,    50,
     147,    55,    53,   516,   172,   518,   519,   384,   924,   573,
     387,   524,   525,   516,   165,   147,   173,   530,   573,   573,
     221,   147,    73,   165,   537,    76,   173,   540,   147,   171,
     543,    55,    83,   165,   147,     2,   141,   172,   147,   174,
     553,   146,   555,   148,   149,    55,   559,   173,   153,   562,
     563,   147,   165,   147,   173,   160,     3,   763,   109,    26,
     573,    28,    55,   168,   173,   578,   147,   444,   445,   165,
     140,   477,   478,   172,   587,   174,   147,   483,   784,   173,
     486,   487,   283,   147,   165,   598,    27,   288,   601,    55,
     603,  1158,   170,  1160,   165,   608,   609,   610,    31,   612,
     613,   165,   165,   147,    89,    55,    47,   620,    93,   172,
      51,   174,    79,   490,   491,   146,    83,    84,   631,   632,
    1184,   165,   305,   306,  1109,   502,    89,   624,    89,    89,
      93,   644,    93,    93,    75,   172,   172,    78,   174,    80,
     107,   654,   519,   165,   165,   112,   795,   796,   525,    90,
     799,   800,   172,   530,   174,   164,    89,   141,   165,   170,
      93,   172,   146,  1074,   365,   149,  1230,    55,   109,   153,
     172,   877,   878,   172,   172,   174,   174,   172,   165,   174,
     601,   170,   603,   165,   697,   386,   165,   165,   165,   165,
     119,   704,   172,    84,   147,   601,   397,   603,   170,   905,
     173,   172,   172,   172,   146,     8,   855,   856,   141,   142,
     165,   144,   145,   146,   172,   148,   149,   150,     3,   152,
     153,   154,   155,   172,   157,   158,   159,   160,   161,   165,
     163,   608,   433,   166,   167,   168,    65,   171,   170,   903,
     125,    90,   948,    90,   177,   758,   759,   760,   761,   762,
     763,   764,   765,   125,   221,   149,   924,    68,   172,   907,
     125,   173,   910,   147,    47,   172,   776,   777,   125,   782,
     125,   784,   785,    34,   787,    34,    65,   173,   791,    27,
     793,   125,   146,   173,   173,    21,   149,   147,   175,   802,
     803,   170,   175,   105,   172,   149,   149,   149,   149,    47,
     141,   149,   173,    51,   149,   175,   149,   975,   821,   149,
     149,   149,   149,   149,   982,   149,   283,   518,   149,   165,
      34,   288,   835,   524,    34,   146,   839,    75,   147,   171,
      78,    32,    80,   165,   535,   165,   537,   165,   165,     2,
    1046,   165,    90,   907,    32,   165,   172,   911,   173,   171,
     173,   864,   865,   866,   170,    34,   920,  1130,  1131,   165,
     175,   109,   875,    26,   165,    28,   879,   175,   881,   882,
     883,   146,   885,   172,   887,   172,   782,   890,   891,   175,
     175,   165,   759,   147,   761,   173,    34,   764,   165,   117,
     147,   173,   905,  1042,  1043,  1044,  1045,   173,   365,   173,
     165,   619,   165,  1046,   164,   606,   164,   397,    25,   610,
     279,   117,   875,    91,  1062,   821,    79,  1004,  1029,   386,
      83,    84,  1070,   624,  1130,  1131,   939,    22,   351,   942,
     397,    26,    27,   573,   559,   948,   924,   911,   951,   920,
      35,   570,    37,    38,   107,   318,   408,  1230,    43,   112,
    1076,  1240,   965,   966,  1071,    50,  1233,  1074,   902,   556,
     706,   137,   817,   976,    -1,   978,    -1,    -1,    -1,    -1,
     983,    -1,    -1,   986,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,   998,    -1,   864,   865,   866,
      -1,    -1,    -1,    88,    -1,    90,    -1,    -1,  1062,    -1,
      -1,    -1,  1015,    -1,    -1,    -1,  1019,    -1,    -1,    -1,
      -1,    -1,  1025,    -1,   109,    -1,    -1,   112,    -1,  1167,
     115,  1034,  1025,  1036,    -1,    -1,  1039,    -1,    -1,    -1,
      -1,  1199,  1200,  1046,    -1,  1203,  1204,  1050,    -1,    -1,
      -1,    -1,    -1,    -1,  1057,    -1,  1059,  1215,    -1,  1217,
      -1,   518,    -1,    -1,    -1,  1172,    -1,   524,   221,    -1,
      -1,    -1,    -1,    -1,    -1,  1078,    -1,    -1,   535,    -1,
     537,    -1,   773,    -1,    31,  1088,  1089,    -1,  1091,  1092,
      -1,  1094,  1095,    -1,    -1,    -1,    -1,    -1,   965,   966,
      -1,  1104,  1105,    -1,  1107,    -1,  1109,    -1,  1111,  1112,
      -1,    -1,  1115,  1167,   805,    -1,    -1,   808,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1130,  1131,    -1,
     283,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,  1142,
      -1,  1144,    89,    -1,    -1,  1148,    93,    -1,    -1,   606,
      -1,    -1,    -1,   610,  1157,  1158,  1159,  1160,  1161,    -1,
      -1,    -1,    -1,   854,    -1,    -1,    -1,   624,    -1,   860,
      -1,    -1,    -1,    -1,    -1,    -1,   867,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1188,  1189,  1190,  1191,  1192,
    1193,  1194,  1195,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,    -1,    -1,    -1,   153,    -1,    -1,    -1,
      -1,    -1,   365,   160,    -1,    -1,    -1,    -1,    -1,   166,
     167,   168,    -1,  1226,    -1,    -1,    -1,    -1,    -1,  1232,
      -1,     2,    -1,   386,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   397,    -1,    -1,    -1,    -1,    -1,
    1253,  1254,  1255,  1256,  1241,    26,    -1,    28,  1261,  1262,
      -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,   965,   966,  1279,  1280,  1281,  1282,
    1283,  1284,    -1,    -1,    -1,   976,    22,   978,    -1,    -1,
      26,    27,   983,    -1,    -1,   986,    -1,    -1,    -1,    35,
      -1,    37,    38,    -1,   995,    -1,   997,    43,    79,  1000,
      -1,    -1,    83,    84,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,  1024,    -1,    -1,   107,    73,    -1,    -1,
      76,   112,    78,  1034,    80,  1036,    82,    83,   805,    85,
      -1,   808,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   518,    -1,    -1,    -1,    -1,
      -1,   524,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
     141,   142,   535,    -1,   537,   146,    -1,   148,   149,    -1,
      -1,    -1,   153,    -1,    -1,    -1,    -1,   854,    -1,   160,
      -1,    -1,    -1,   860,   140,   166,   167,   168,    -1,    -1,
     867,    -1,    -1,  1104,    -1,    -1,    -1,     2,    -1,    -1,
    1111,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,   165,
      -1,    -1,    -1,  1124,    -1,   171,    35,    -1,    37,    38,
      -1,    26,    -1,    28,    43,    -1,    -1,  1138,    -1,    -1,
     221,    50,    -1,   606,    -1,    -1,  1147,   610,    -1,    -1,
    1151,    -1,  1153,  1154,    -1,    -1,    31,    -1,    -1,    -1,
      -1,   624,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    85,    -1,    -1,    88,
      -1,    90,    -1,    -1,    79,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,   283,   112,  1205,    -1,   115,   288,    -1,   976,
      -1,   978,   107,    -1,    89,    -1,   983,   112,    93,   986,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   995,    -1,
     997,   140,    -1,  1000,    -1,    -1,    -1,    -1,    -1,    -1,
    1241,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1253,  1254,  1255,  1256,   165,  1024,    -1,    -1,
    1261,  1262,   171,    -1,    -1,     3,   141,  1034,    -1,  1036,
      -1,   146,    -1,   148,   149,    -1,    -1,    -1,   153,    17,
      18,    -1,    20,    -1,   365,   160,    -1,    25,    -1,    -1,
      -1,   166,   167,   168,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,   386,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,   397,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,   221,    -1,    -1,    -1,
       2,    69,    70,    -1,    -1,    -1,    -1,  1104,    -1,    77,
      -1,    -1,   805,    -1,  1111,   808,    -1,    31,    -1,    87,
      -1,    -1,    -1,    -1,    26,    -1,    28,  1124,    96,    97,
      -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,  1138,   110,   111,    -1,   113,    -1,    -1,   116,    -1,
    1147,    -1,    -1,    -1,  1151,    -1,  1153,  1154,   283,    -1,
      -1,   854,    -1,   288,    -1,    -1,    -1,   860,    -1,    -1,
      -1,    -1,    -1,    -1,   867,    89,    -1,    79,    -1,    93,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   172,   107,    -1,   518,  1205,    -1,
     112,    -1,    -1,   524,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   535,    -1,   537,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,    -1,   153,
     365,    -1,    -1,    -1,  1241,   159,   160,    -1,    -1,   163,
      -1,    -1,   166,   167,   168,    -1,  1253,  1254,  1255,  1256,
      -1,   386,    -1,   177,  1261,  1262,    -1,    -1,    -1,    -1,
      -1,    -1,   397,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   976,    -1,   978,    -1,    -1,    -1,    -1,
     983,    -1,    -1,   986,    -1,   606,    -1,    -1,    -1,   610,
      -1,    -1,   995,    -1,   997,    -1,    -1,  1000,    -1,    -1,
      -1,    -1,    -1,   624,    -1,    -1,    -1,    -1,    -1,   221,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1024,    -1,    71,    -1,    -1,    -1,    17,    18,    -1,
      20,  1034,    -1,  1036,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    92,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,
     108,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   283,    62,    -1,    -1,    -1,   288,    -1,    -1,    69,
      70,    -1,    -1,   518,    -1,    -1,    -1,    77,    -1,   524,
      -1,     2,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
     535,  1104,   537,    -1,    -1,    -1,    96,    97,  1111,    99,
      -1,   101,    -1,   103,    -1,    26,   106,    28,    -1,    -1,
     110,  1124,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1147,    -1,    -1,    -1,  1151,    -1,
    1153,  1154,    -1,   365,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,   606,    83,    84,   386,   610,    -1,    -1,    -1,    -1,
     170,    -1,   172,    -1,   805,   397,    -1,   808,   236,   624,
      -1,    -1,    -1,    -1,    -1,     3,   107,    -1,    -1,    -1,
      -1,   112,  1205,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,   280,   854,    -1,    -1,    -1,    45,  1241,   860,
      -1,    49,    -1,    -1,    52,    -1,   867,    -1,    -1,    -1,
    1253,  1254,  1255,  1256,    62,    -1,   304,    -1,  1261,  1262,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,    -1,   103,   344,    -1,   106,    -1,
      -1,    -1,   110,   111,    -1,   113,   518,    -1,   116,    -1,
     221,    -1,   524,    -1,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,   535,    -1,   537,    -1,    -1,    35,    -1,
      37,    38,    -1,    22,    -1,    -1,    43,    26,    27,    -1,
      -1,    -1,    -1,    50,    -1,    -1,    35,    -1,    37,    38,
      -1,    -1,    -1,    -1,    43,   976,    -1,   978,    -1,    -1,
      -1,    50,   983,    -1,   172,   986,    -1,    -1,    -1,    76,
     805,    78,   283,   808,   995,    82,   997,   288,    85,  1000,
      -1,    88,    -1,    90,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,     2,    82,   606,    -1,    85,    -1,   610,    88,
      -1,    90,   109,  1024,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,   624,  1034,    -1,  1036,    26,    -1,    28,   854,
     109,    -1,    -1,   112,    -1,   860,   115,    -1,    -1,   477,
     478,    -1,   867,   140,    -1,   483,    -1,    -1,   486,   487,
     488,   489,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    -1,   365,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
       3,    -1,    -1,    83,    84,   386,   165,    -1,    -1,    -1,
      -1,    -1,   171,  1104,    17,    18,   397,    20,    -1,    -1,
    1111,    -1,    25,    -1,    -1,   543,    -1,   107,    -1,    -1,
      33,    -1,   112,  1124,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,  1138,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,  1147,    -1,    -1,    62,
    1151,    -1,  1153,  1154,    -1,    -1,    69,    70,    -1,   587,
      -1,   976,    -1,   978,    77,    -1,    31,    -1,   983,    -1,
      -1,   986,    -1,   601,    87,   603,    -1,    -1,    -1,    -1,
     995,    -1,   997,    96,    97,  1000,    99,    -1,   101,    -1,
     103,    -1,   620,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,  1205,    -1,    -1,    -1,    -1,  1024,
      -1,    -1,    -1,   805,    -1,     2,   808,    -1,    -1,  1034,
      -1,  1036,    -1,    -1,    89,    -1,    -1,   518,    93,    -1,
      -1,   221,    -1,   524,    -1,    -1,    -1,    -1,    -1,    26,
    1241,    28,    -1,    -1,   535,    -1,   537,    -1,    -1,    -1,
      -1,    -1,  1253,  1254,  1255,  1256,    -1,    -1,    -1,   172,
    1261,  1262,   854,    -1,    -1,    -1,    -1,    -1,   860,    -1,
      -1,    -1,    -1,    -1,    -1,   867,   141,   142,    -1,    -1,
     145,   146,    -1,   148,   149,    -1,    -1,    -1,   153,  1104,
      -1,    -1,    79,   283,    -1,   160,  1111,    -1,   288,    -1,
      -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,  1124,
      -1,    -1,    -1,    -1,    -1,   606,    -1,    -1,    -1,   610,
     107,    -1,    -1,  1138,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,  1147,   624,    -1,   763,  1151,    -1,  1153,  1154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,   782,    -1,   784,    -1,    -1,    -1,
      -1,    22,    -1,    24,    -1,    26,    27,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    35,   365,    37,    38,    -1,    40,
      41,    42,    43,    -1,   976,    -1,   978,    -1,    -1,    50,
    1205,   983,    -1,   821,   986,    -1,   386,    -1,    -1,    -1,
      -1,    -1,    -1,   995,    65,   997,    -1,   397,  1000,    -1,
      89,    -1,    -1,    -1,    93,    76,    -1,    78,    -1,    -1,
      -1,    82,    -1,    84,    85,    -1,  1241,    88,    -1,    90,
      -1,    -1,  1024,    -1,   221,    -1,    -1,    -1,  1253,  1254,
    1255,  1256,  1034,    -1,  1036,   124,  1261,  1262,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,   119,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   905,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,   283,    -1,   177,    -1,
      -1,   288,    -1,    -1,   165,    -1,    -1,    -1,    -1,   170,
      -1,    -1,  1104,   174,   805,    -1,     2,   808,    -1,  1111,
     948,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   518,    -1,
      -1,    -1,  1124,    -1,   524,     3,    -1,    -1,    -1,    -1,
      26,    -1,    28,    -1,    -1,   535,  1138,   537,    -1,    17,
      -1,    -1,    20,    -1,    -1,  1147,    -1,    25,    -1,  1151,
      -1,  1153,  1154,   854,    -1,    -1,    -1,    35,    -1,   860,
      -1,    -1,    -1,    -1,    -1,    -1,   867,    45,   365,    -1,
      -1,    -1,    -1,    31,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    62,    -1,    -1,    -1,    -1,   386,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     397,    -1,    -1,  1205,    -1,    -1,   606,    -1,  1046,    87,
     610,   107,    -1,    22,    -1,    -1,   112,    26,    27,    -1,
      -1,    99,    -1,    -1,   624,   103,    35,    -1,    37,    38,
      12,    89,   110,    -1,    43,    93,    -1,    19,   116,  1241,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,  1253,  1254,  1255,  1256,    -1,    -1,    -1,    -1,  1261,
    1262,    -1,    -1,    -1,    -1,    47,    -1,    76,    -1,    78,
      -1,    80,    -1,    82,    -1,   976,    85,   978,    -1,    88,
      -1,    90,   983,   141,   142,   986,   144,   145,   146,    -1,
     148,   149,  1130,  1131,   995,   153,   997,    -1,    -1,  1000,
     109,   159,   160,   112,    -1,   163,   115,    89,   166,   167,
     168,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   518,    -1,  1024,    -1,   221,    -1,   524,    49,    -1,
      -1,   140,    -1,  1034,    -1,  1036,    -1,    -1,   535,    -1,
     537,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    -1,   165,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,   283,   170,    -1,
      -1,    -1,   288,    -1,    -1,   177,    -1,    -1,    -1,    31,
      -1,    -1,    -1,  1104,    -1,   805,    -1,    -1,   808,   606,
    1111,    -1,    -1,   610,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1124,    -1,    -1,    -1,   624,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1138,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1147,    -1,    -1,    -1,
    1151,    -1,  1153,  1154,   854,    -1,    -1,    89,    -1,    -1,
     860,    93,    -1,    -1,    49,    -1,    -1,   867,    -1,   365,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,
     386,    -1,   124,    -1,    -1,    -1,    -1,    -1,    83,    84,
      -1,   397,    -1,    -1,  1205,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
      -1,   163,   117,    -1,   166,   167,   168,    -1,    -1,    -1,
    1241,    -1,    -1,    -1,    -1,   177,   267,    -1,   269,   270,
     271,   272,  1253,  1254,  1255,  1256,    -1,    -1,    -1,    -1,
    1261,  1262,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     291,    -1,   293,    -1,    -1,    -1,   976,    -1,   978,   300,
      -1,    -1,    -1,   983,    -1,    -1,   986,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   995,    -1,   997,    -1,    -1,
    1000,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   805,    -1,
      -1,   808,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   340,
      -1,    -1,   518,    -1,  1024,    -1,    -1,    -1,   524,    -1,
      -1,    -1,   353,   354,  1034,    30,  1036,    -1,    -1,   535,
      -1,   226,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      45,    46,    -1,    -1,    -1,    -1,    -1,   854,    -1,    -1,
      -1,    -1,    -1,   860,    -1,    -1,    -1,    -1,    -1,    -1,
     867,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   267,   268,   269,   270,   271,   272,    -1,    -1,
     275,   276,    -1,    -1,    -1,    -1,    -1,   282,    -1,    -1,
      -1,    -1,    -1,    -1,  1104,    -1,   291,    -1,   293,    -1,
     606,  1111,    -1,    -1,   610,   300,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1124,    -1,    -1,    -1,   624,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1147,    -1,    -1,
      -1,  1151,    -1,  1153,  1154,   340,    -1,    -1,    31,    -1,
     155,    -1,   157,    -1,    -1,    -1,    -1,    -1,   353,   354,
      -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,   976,
      -1,   978,   503,    -1,    -1,    -1,   983,    -1,    -1,   986,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   995,    -1,
     997,    -1,   523,  1000,    -1,  1205,   527,    -1,   529,    -1,
     205,   532,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,  1024,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1034,    -1,  1036,
      -1,  1241,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1253,  1254,  1255,  1256,    -1,    -1,    -1,
      -1,  1261,  1262,    -1,    -1,    -1,    -1,    -1,   141,   142,
     265,   144,   145,   146,    -1,   148,   149,   150,   273,   274,
     153,   154,   155,    -1,   157,   158,   159,   160,    -1,   284,
     163,    -1,    -1,   166,   167,   168,   617,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,  1104,    -1,   805,
      -1,    -1,   808,    -1,  1111,    -1,    -1,    -1,   503,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1124,    -1,   324,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   523,    -1,
      -1,  1138,   527,    -1,   529,    -1,    -1,   532,    -1,    -1,
    1147,    -1,    -1,    -1,  1151,    -1,  1153,  1154,   854,    -1,
      -1,    -1,    -1,    -1,   860,    -1,    -1,    -1,    -1,    -1,
      -1,   867,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   381,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,    -1,    -1,    -1,  1205,    -1,
      -1,    -1,    -1,    -1,   409,    -1,    -1,    -1,    -1,   414,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   616,   617,    -1,   169,    -1,    -1,   172,   433,   174,
      -1,    -1,    -1,    -1,  1241,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1253,  1254,  1255,  1256,
      -1,    -1,    -1,    -1,  1261,  1262,    -1,    -1,   789,   790,
      -1,    -1,    -1,    -1,   795,   796,    -1,    -1,   799,   800,
     976,    -1,   978,    -1,    -1,    -1,    -1,   983,    -1,    -1,
     986,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   995,
      -1,   997,   497,    -1,  1000,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,    38,    -1,    -1,    -1,    -1,    43,    -1,  1024,    -1,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,  1034,    -1,
    1036,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   544,
      -1,    -1,    -1,   548,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    82,   561,    -1,    85,    -1,
      -1,    88,    -1,    90,    -1,   570,    -1,    -1,    -1,    -1,
     575,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,   789,   790,    -1,    -1,    -1,    -1,
     795,   796,    -1,    -1,   799,   800,    -1,    -1,    -1,    -1,
      12,    -1,    -1,   140,   809,    -1,    -1,    19,  1124,    -1,
      -1,    -1,    -1,    -1,    -1,   630,    -1,    -1,    -1,    31,
      -1,    -1,  1138,    -1,    -1,    -1,   967,   968,   165,    -1,
      -1,  1147,   973,    -1,   171,  1151,    -1,  1153,  1154,    -1,
     845,    -1,    -1,    -1,   985,    -1,    -1,   988,    -1,   990,
      -1,   992,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1205,
     102,   706,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1040,
    1041,  1042,  1043,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,  1241,   138,   139,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,   170,    -1,
      -1,    -1,   967,   968,    -1,   177,    -1,    -1,   973,    -1,
     975,    -1,    -1,    -1,    -1,    -1,    -1,   982,    -1,    -1,
     985,    -1,    -1,   988,    -1,   990,    -1,   992,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   810,    -1,    -1,    -1,  1140,
    1005,    -1,   817,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   827,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1032,    -1,    -1,
     845,    -1,    -1,    -1,    -1,  1040,  1041,  1042,  1043,    -1,
      -1,    -1,     1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,   902,    -1,    -1,
      49,    -1,    -1,    52,   909,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,   924,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,  1140,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,  1004,
      -1,   150,    -1,    -1,  1199,  1200,    -1,    -1,  1203,  1204,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,  1213,   168,
    1215,   170,  1217,   172,  1029,   174,    -1,    -1,   177,    -1,
      -1,    -1,    -1,     0,     1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,  1074,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    -1,    14,    15,    16,    17,    18,    19,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,   170,   171,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,   171,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,   171,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    12,    -1,    14,    -1,    -1,
      17,    18,    19,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,    -1,   141,   142,   143,   144,   145,   146,
      -1,   148,    -1,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,   166,
     167,   168,   169,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    -1,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    -1,    -1,    -1,    -1,    30,    -1,
      32,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      42,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    61,
      62,    63,    64,    -1,    66,    67,    -1,    69,    70,    71,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,   119,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    26,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      47,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,   173,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
     107,   108,    -1,   110,   111,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,   173,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
     107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,   173,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
     107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
     122,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,   175,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    -1,    -1,    -1,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
     122,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
     112,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,   175,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,   171,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,   175,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,   175,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,   111,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,
      18,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    -1,    -1,    64,    -1,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      78,    79,    80,    81,    -1,    -1,    -1,    -1,    86,    -1,
      88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,   173,   174,    -1,    -1,   177,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    18,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,
      64,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    78,    79,    80,    81,    -1,    -1,
      -1,    -1,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    98,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    12,    -1,   150,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,   159,    -1,    25,   162,   163,
     164,    -1,    -1,    -1,   168,    33,   170,    35,   172,    -1,
     174,    39,    -1,   177,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,   143,   144,   145,   146,    -1,
     148,    -1,    -1,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,    -1,    -1,   166,   167,
     168,   169,    -1,    -1,   172,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,    -1,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,   143,    -1,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,   164,    22,
      -1,    -1,    25,    -1,   170,    -1,   172,    -1,   174,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,     3,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    12,    -1,    86,    87,    88,    -1,    -1,    19,    20,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    33,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,   174,   103,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,    -1,
     141,   142,   143,   144,   145,   146,    -1,   148,    -1,    -1,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,     3,   163,    -1,    -1,   166,   167,   168,   169,   170,
      12,    -1,    -1,    -1,    -1,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    96,    97,    -1,    99,    19,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      31,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,   143,   144,   145,   146,    -1,   148,    -1,    -1,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,   163,    -1,    -1,   166,   167,   168,   169,    89,    -1,
      -1,    12,    93,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    89,   170,
      -1,    12,    93,    -1,    -1,    -1,   177,    -1,    19,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    89,   170,
      -1,    12,    93,    -1,    -1,    -1,   177,    -1,    19,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    89,   170,
      -1,    12,    93,    -1,    -1,    -1,   177,    -1,    19,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    89,   170,
      -1,    12,    93,    -1,    -1,    -1,   177,    -1,    19,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    89,   170,
      -1,    12,    93,    -1,    -1,    -1,   177,    -1,    19,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    89,   170,
      -1,    12,    93,    -1,    -1,    -1,   177,    -1,    19,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    89,   170,
      -1,    -1,    93,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      31,    32,   133,   134,   135,   136,    -1,   138,   139,    -1,
     141,   142,    -1,   144,   145,   146,    47,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    89,    -1,
      -1,    -1,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    47,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,   170,
      19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    47,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      89,   166,   167,   168,    93,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    31,   124,    -1,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    72,    -1,   166,   167,   168,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,   165,   166,
     167,   168,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,   124,    -1,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    72,    -1,   166,
     167,   168,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    12,    89,    -1,    -1,    -1,    93,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      47,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,   170,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,   170,
      19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,   170,    19,    -1,
      -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,   170,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    89,   166,   167,   168,    93,    -1,
      -1,    -1,    -1,    -1,   175,    -1,   177,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    31,   124,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    72,
      -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,    -1,    -1,    89,    -1,
      -1,    12,    93,    -1,   177,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      31,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,   177,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,   170,
      -1,    -1,    -1,    -1,    89,    -1,   177,    -1,    93,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    12,   165,   166,   167,   168,   123,   124,
      19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    12,
      -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    12,
      13,    -1,    -1,   102,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,   124,    -1,    -1,    -1,    12,    89,    -1,   177,    -1,
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    31,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,   177,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    89,   166,
     167,   168,    93,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     177,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,   165,   166,   167,   168,    -1,    -1,
      12,    89,    -1,    -1,    -1,    93,   177,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    31,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    89,   166,   167,
     168,    93,    -1,    -1,    -1,    -1,    12,    -1,    -1,   177,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,   173,    -1,    89,    -1,   177,    -1,    93,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    12,    13,   166,   167,   168,   123,   124,    19,
      -1,   173,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    -1,
     166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
     124,    -1,    -1,    -1,    12,    89,    -1,   177,    -1,    93,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    31,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,   173,
      -1,    -1,    -1,   177,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,    89,   166,   167,   168,    93,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    31,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    89,   166,   167,
     168,    93,    -1,    -1,    -1,    12,    -1,   175,    -1,   177,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,   177,    93,    -1,    -1,   141,
     142,    -1,   144,   145,   146,   102,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,   124,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    12,    13,   166,
     167,   168,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    60,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    12,
      -1,   166,   167,   168,    -1,   124,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    89,    -1,    -1,    -1,
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     177,   102,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    31,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    89,   166,   167,   168,
      93,    -1,    -1,    -1,    12,    -1,    -1,    -1,   177,   102,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,   177,    93,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    17,    18,    -1,    20,    -1,    77,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    87,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    96,    97,    -1,    99,
      45,   101,    -1,   103,    49,    -1,   106,    52,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,     3,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,    87,    -1,    -1,    -1,    -1,    17,    18,    -1,
      20,    96,    97,    -1,    99,    25,   101,    27,   103,    -1,
      -1,   106,    -1,    33,    -1,   110,    -1,    -1,   113,    39,
      -1,   116,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    62,    -1,    -1,   140,    -1,    -1,    -1,    69,
      70,    -1,    17,    18,    -1,    20,    -1,    77,    78,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    87,    33,    -1,
      90,    -1,    -1,    -1,    39,    -1,    96,    97,    -1,    99,
      45,   101,    -1,   103,    49,    -1,   106,    52,    -1,    -1,
     110,    -1,    -1,   113,    -1,   115,   116,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    17,    18,    -1,
      20,    96,    97,    -1,    99,    25,   101,    -1,   103,    -1,
      -1,   106,    -1,    33,    -1,   110,   111,    -1,   113,    39,
      -1,   116,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    17,    18,    -1,    20,    -1,    77,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    87,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    96,    97,    -1,    99,
      45,   101,    -1,   103,    49,    -1,   106,    52,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    17,    18,    -1,
      20,    96,    97,    -1,    99,    25,   101,    -1,   103,    -1,
      -1,   106,    -1,    33,    -1,   110,    -1,    -1,   113,    39,
      -1,   116,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,
      -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   182,   183,     0,     1,     3,     4,     5,     6,     7,
       8,     9,    10,    14,    15,    16,    17,    18,    20,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    32,    33,
      35,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      48,    49,    50,    52,    55,    56,    57,    58,    59,    61,
      62,    63,    64,    66,    67,    69,    70,    71,    76,    77,
      78,    79,    80,    81,    82,    85,    86,    87,    88,    90,
      91,    92,    94,    95,    96,    97,    98,    99,   100,   101,
     103,   104,   106,   107,   108,   109,   110,   112,   113,   115,
     116,   119,   121,   124,   140,   141,   142,   143,   144,   145,
     150,   159,   162,   163,   164,   165,   168,   170,   172,   174,
     177,   184,   185,   186,   187,   188,   189,   191,   192,   193,
     194,   199,   200,   201,   203,   204,   205,   210,   211,   214,
     215,   219,   221,   224,   225,   228,   229,   230,   231,   232,
     233,   236,   237,   239,   241,   244,   245,   246,   247,   248,
     252,   253,   258,   259,   260,   261,   264,   265,   270,   271,
     273,   274,   279,   283,   284,   286,   287,   309,   314,   315,
     319,   320,   341,   342,   343,   344,   345,   346,   347,   355,
     356,   357,   358,   359,   360,   362,   365,   366,   367,   368,
     369,   370,   371,   372,   374,   375,   376,   377,   378,   165,
      22,    35,    40,    41,    44,    55,    88,   101,   170,   174,
     244,   271,   341,   346,   356,   357,   362,   365,   367,   368,
     120,   348,   349,     3,   216,   362,   348,   362,   109,   321,
      90,   216,   186,   335,   362,   186,   172,     3,    17,    18,
      20,    25,    33,    39,    45,    49,    52,    62,    69,    70,
      77,    87,    96,    97,    99,   101,   103,   106,   110,   113,
     116,   217,   218,   220,    11,    78,   122,   238,   362,   238,
     362,   238,   362,    27,   115,   240,   362,    82,    85,   201,
     172,   218,   218,   218,   172,   218,   322,   323,    32,   205,
     223,   362,   256,   257,   362,   362,    18,    77,    96,   113,
     362,   362,   362,     8,   172,   227,   226,    27,    33,    47,
      49,    51,    75,    78,    90,    97,   101,   109,   185,   222,
     275,   276,   277,   300,   301,   302,   327,   332,   362,   335,
     107,   108,   165,   279,   280,   361,   362,   364,   362,   223,
     362,   362,   362,   100,   172,   186,   362,   362,   188,   205,
     188,   205,   362,   240,   362,   364,     3,   362,   362,   362,
     362,   362,     1,   171,   184,   206,   335,   111,   151,   336,
     337,   364,   175,   238,   335,   362,   373,   362,    80,   186,
      22,    35,    38,    40,    41,    88,   170,   174,   189,   190,
     201,   205,   237,   193,     8,   149,   195,   170,    46,    84,
     114,   202,    26,   320,   362,     8,   260,   362,   363,    55,
     146,   262,   218,     1,   192,   218,   266,   269,   172,   295,
      53,    73,    83,   304,    27,    78,    90,   109,   305,    27,
      78,    90,   109,   303,   218,   316,   317,   322,   164,   165,
     362,    12,    19,    31,    89,    93,   124,   141,   142,   144,
     145,   146,   148,   149,   150,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   166,   167,   168,
     177,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   169,   291,   172,   174,
      89,    93,   362,   362,   362,   218,   335,    55,   172,   186,
     165,   205,    47,   348,   319,   165,   147,   165,   119,   219,
     338,   339,   340,   364,   170,   218,   172,   223,    32,    47,
     223,   120,   223,   351,    32,    47,   223,   351,   223,   351,
      47,   223,   351,   218,   218,   102,   205,   102,   125,   205,
     291,   202,   338,   172,   172,   205,   186,   111,   172,   218,
     324,   325,     1,   146,   331,    47,   147,   186,   223,   147,
     223,    13,   172,   172,   223,   338,   228,   228,    47,    90,
     301,   302,   173,   147,   111,   172,   218,   299,   146,   165,
     362,   362,   123,   281,   165,   170,   223,   172,   338,   165,
     249,   249,   223,   223,   165,   171,   171,   184,   147,   171,
     362,   147,   173,   147,   173,   362,   175,   351,    47,   147,
     175,   351,   123,   147,   175,     8,   362,   362,   335,     3,
     172,   196,     1,   171,   206,   212,   213,   362,   208,   362,
      65,    36,    72,   165,   260,   262,   109,   234,   284,   218,
     335,   170,   171,   269,   125,   147,   171,   173,   296,   298,
     300,   305,    90,     1,   146,   329,   330,    90,     1,     3,
      12,    17,    19,    20,    25,    45,    52,    54,    62,    70,
      87,    99,   103,   110,   116,   141,   142,   143,   144,   145,
     146,   148,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   163,   166,   167,   168,   169,   172,   218,   288,
     289,   290,   291,   341,   125,   318,   147,   165,   165,   362,
     362,   362,   238,   362,   238,   362,   362,   362,   362,   362,
     362,   362,     3,    20,    33,    62,   103,   109,   219,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,    68,   364,   364,   364,
     364,   364,   338,   338,   238,   362,   238,   362,    32,    47,
      32,    47,   102,   172,    47,   175,   218,    27,    47,    90,
     115,   350,   353,   354,   362,   378,   238,   362,   223,   322,
     362,   362,   125,   173,   147,    47,   335,    44,   362,   238,
     362,   172,   223,    44,   362,   238,   362,   223,   223,   238,
     362,   223,   125,   125,   186,    34,   186,   362,    34,   362,
      65,   173,   339,   218,   242,   243,   325,   147,   173,   222,
     362,   125,   326,   362,   323,   256,   218,   319,   362,   362,
     173,   299,   305,   277,   325,   146,   333,   334,   332,   282,
     204,     1,   254,   340,   173,    21,   250,   171,   173,   173,
     336,   173,   336,   186,   175,   238,   362,   175,   186,   362,
     175,   362,   175,   362,    47,    47,    47,   175,   195,   173,
     197,   198,   340,   171,   171,   147,   165,    13,   149,   147,
     165,    13,    36,    72,   218,   141,   142,   143,   144,   145,
     159,   163,   168,   207,   290,   291,   292,   362,   207,   209,
     262,   170,   235,   319,   165,   172,     1,   263,   362,   267,
     268,   147,   173,   331,     1,   222,   327,   328,   105,   306,
     172,   294,   362,   141,   149,   294,   294,   362,   317,   172,
     174,   165,   165,   165,   165,   165,   165,   173,   175,    44,
     238,   362,    44,   238,   362,   339,   238,   362,   172,    47,
      90,   147,   173,    17,    20,    25,    45,    52,    62,    70,
      87,    99,   110,   116,   341,    89,    89,   348,   348,   165,
     165,   364,   340,   362,   173,   362,    32,   223,    32,   223,
     352,   353,   362,    32,   223,   351,    32,   223,   351,   223,
     351,   223,   351,   362,   362,    34,   186,    34,    34,   186,
     102,   205,   218,   173,   147,   173,   173,   324,   331,    68,
     364,   218,   173,   173,   334,   146,   278,   173,   332,   151,
     293,   326,   362,   171,    74,   117,   171,   255,   173,   172,
     205,   218,   251,   186,   175,   351,   175,   351,   186,   123,
     238,   362,   238,   362,   238,   362,   147,   173,   212,   219,
     170,   292,   362,   111,   362,   207,   209,   147,   165,    13,
     165,   170,   263,   316,   322,   339,   171,    82,    85,   171,
     185,   192,   225,   269,   192,   298,   306,    60,   118,   310,
     296,   297,   173,   289,   291,   173,   175,   362,    32,    32,
     362,    32,    32,   173,   175,   175,   339,   353,   331,   341,
     341,   223,   223,   223,   102,    44,   362,    44,   362,   147,
     173,   102,    44,   362,   223,    44,   362,   223,   223,   223,
     186,   186,   362,   186,    34,   165,   165,   243,   205,   326,
     172,   172,   293,   326,   327,   306,   334,   362,    32,   186,
     335,   251,   146,   205,    44,   186,   362,   175,    44,   186,
     362,   175,   362,   175,   175,   198,   207,    13,    36,    72,
      36,    72,   165,   165,   292,   362,   362,   263,   171,   165,
     165,   173,   192,   225,   225,   269,   310,     3,    92,   103,
     311,   312,   313,   362,   285,   173,   294,   294,   102,    44,
      44,   102,    44,    44,    44,    44,   173,   326,   362,   362,
     362,   353,   362,   362,   362,    34,   186,   338,   338,   293,
     326,   186,   223,   173,   362,   362,   186,   362,   186,   171,
     111,   362,   207,   209,   207,   209,    13,   171,   165,   225,
     272,   313,   118,   147,   125,   152,   154,   155,   157,   158,
      60,    32,   165,   204,   224,   307,   308,   362,   362,   362,
     362,   362,   362,   102,   102,   102,   102,   186,   173,   173,
     205,   102,   102,   165,   165,   165,   165,   362,   308,   362,
     312,   313,   313,   313,   313,   313,   313,   311,   184,   102,
     102,   102,   102,   102,   102,   362,   362,   362,   362,   164,
     164,   362,   362
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   181,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   189,   189,   190,   190,   190,
     191,   192,   192,   193,   193,   193,   194,   195,   195,   196,
     196,   197,   197,   198,   199,   200,   200,   200,   201,   201,
     201,   202,   202,   203,   204,   204,   204,   205,   206,   206,
     207,   207,   207,   207,   207,   207,   208,   208,   208,   208,
     208,   208,   209,   209,   210,   210,   210,   210,   210,   210,
     210,   211,   212,   212,   212,   212,   213,   213,   214,   215,
     215,   215,   215,   215,   215,   216,   216,   217,   217,   217,
     217,   217,   218,   218,   219,   219,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   222,   222,   222,
     222,   223,   223,   224,   224,   225,   225,   226,   225,   227,
     225,   228,   228,   228,   228,   228,   228,   228,   229,   229,
     229,   229,   230,   231,   231,   232,   233,   233,   233,   234,
     233,   235,   233,   236,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   238,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   240,   240,   241,   241,   242,   242,   243,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   245,   245,   245,   245,   245,   245,   246,   246,
     246,   247,   247,   247,   248,   248,   248,   248,   249,   249,
     250,   250,   250,   251,   251,   252,   253,   253,   254,   254,
     255,   255,   255,   256,   256,   256,   257,   257,   258,   259,
     259,   260,   261,   261,   261,   262,   262,   263,   263,   263,
     263,   263,   264,   264,   265,   266,   266,   267,   266,   266,
     268,   266,   269,   269,   270,   272,   271,   273,   274,   274,
     274,   275,   275,   276,   276,   277,   277,   277,   278,   278,
     279,   281,   280,   282,   280,   283,   285,   284,   286,   286,
     286,   286,   286,   287,   288,   288,   289,   289,   289,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   292,   292,   293,   293,
     294,   294,   295,   295,   296,   296,   297,   297,   298,   299,
     299,   300,   300,   300,   300,   300,   300,   301,   301,   302,
     302,   302,   302,   302,   302,   302,   302,   302,   303,   303,
     303,   303,   303,   303,   304,   304,   304,   305,   305,   305,
     305,   305,   305,   306,   306,   307,   307,   308,   308,   308,
     309,   310,   310,   310,   310,   310,   311,   311,   312,   312,
     312,   312,   312,   312,   312,   313,   313,   314,   315,   315,
     316,   316,   317,   318,   318,   319,   319,   319,   319,   319,
     321,   320,   320,   322,   322,   323,   323,   324,   324,   324,
     325,   325,   325,   326,   326,   326,   327,   328,   328,   328,
     329,   329,   330,   330,   331,   331,   331,   331,   332,   332,
     333,   334,   334,   335,   335,   336,   336,   337,   337,   338,
     338,   339,   339,   340,   340,   341,   341,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   344,   344,   344,   344,   344,   344,   344,   345,   346,
     347,   347,   347,   347,   347,   347,   347,   348,   348,   349,
     350,   350,   351,   352,   352,   353,   353,   353,   354,   354,
     354,   354,   354,   354,   355,   355,   355,   355,   355,   356,
     356,   356,   356,   356,   357,   358,   358,   358,   358,   358,
     358,   359,   360,   361,   361,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   363,   363,   364,   364,   364,   365,   365,   365,
     365,   366,   366,   366,   366,   366,   367,   367,   367,   368,
     368,   368,   368,   368,   368,   369,   369,   369,   369,   370,
     370,   371,   371,   372,   372,   372,   372,   372,   372,   372,
     372,   372,   372,   372,   372,   372,   373,   373,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   375,   375,   375,   375,   375,   375,   375,   376,   376,
     376,   376,   377,   377,   377,   377,   378,   378,   378,   378,
     378,   378,   378
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     3,     3,     3,     3,     2,     1,
       1,     1,     1,     1,     2,     1,     3,     3,     3,     3,
       2,     3,     3,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     2,     3,     2,     2,     2,     0,     3,     2,
       3,     1,     3,     1,     4,     3,     4,     4,     0,     1,
       1,     0,     1,     6,     2,     3,     3,     1,     1,     2,
       1,     1,     3,     3,     3,     5,     1,     3,     3,     3,
       5,     5,     0,     1,     4,     6,     8,     8,     6,     8,
       8,     4,     1,     3,     3,     5,     1,     3,     3,     4,
       4,     4,     4,     4,     4,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     2,     3,     1,     1,     0,     3,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     3,     5,
       5,     2,     1,     1,     1,     1,     6,     7,     3,     0,
       6,     0,     6,     2,     5,     3,     3,     6,     6,     4,
       5,     5,     3,     3,     6,     5,     6,     5,     6,     3,
       4,     3,     4,     5,     6,     5,     6,     3,     4,     3,
       4,     6,     7,     6,     7,     4,     5,     4,     5,     4,
       4,     3,     6,     5,     4,     3,     6,     5,     6,     5,
       8,     7,     4,     4,     6,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     6,     4,
       7,     5,     3,     6,     2,     2,     3,     3,     0,     2,
       2,     3,     5,     1,     3,     3,     5,     5,     0,     2,
       3,     2,     3,     4,     3,     1,     1,     3,     3,     5,
       5,     2,     1,     1,     1,     0,     2,     0,     2,     3,
       3,     4,     3,     3,     3,     1,     2,     0,     4,     2,
       0,     5,     1,     3,     1,     0,     8,     0,     1,     1,
       1,     0,     1,     1,     3,     1,     3,     1,     0,     2,
       6,     0,     3,     0,     4,     1,     0,     7,     4,     4,
       6,     6,     4,     2,     1,     3,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       0,     3,     2,     3,     1,     3,     0,     1,     1,     1,
       1,     4,     5,     4,     5,     6,     6,     0,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     0,     1,
       1,     2,     1,     1,     1,     1,     1,     0,     1,     2,
       1,     1,     1,     0,     1,     1,     1,     1,     2,     1,
       1,     0,     2,     2,     4,     4,     1,     3,     3,     3,
       3,     3,     3,     3,     2,     1,     1,     3,     1,     2,
       1,     3,     2,     0,     2,     1,     2,     1,     1,     1,
       0,     5,     3,     1,     3,     3,     5,     1,     1,     3,
       1,     2,     3,     0,     2,     2,     1,     1,     1,     1,
       2,     1,     0,     1,     0,     2,     2,     1,     1,     1,
       2,     0,     1,     1,     3,     1,     1,     3,     3,     0,
       1,     1,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     4,     4,     4,     5,     2,     2,     1,
       2,     1,     2,     1,     2,     1,     2,     1,     1,     6,
       6,     4,     9,     9,     7,     6,     6,     4,     9,     9,
       7,     2,     3,     4,     6,     6,     9,     9,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     4,
       1,     3,     4,     1,     3,     4,     3,     3,     1,     1,
       2,     1,     2,     1,     1,     2,     2,     2,     2,     2,
       8,     8,     9,     9,     4,     3,     3,     2,     2,     2,
       1,     3,     4,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     2,     2,     1,     1,     4,     4,     4,     3,
       3,     3,     3,     5,     5,     3,     4,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     4,     3,     4,     3,     4,     3,     5,     3,     3,
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
#line 634 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6205 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 638 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6211 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 639 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6217 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 646 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6225 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 654 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6233 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 658 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6242 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 668 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6250 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 673 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6258 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 680 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6264 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 681 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6270 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6276 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6282 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6288 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6294 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6300 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6306 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6312 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: TDEFER stmt  */
#line 690 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-1]),
                               false, (yylsp[-1]), (yyvsp[0].commentsAndStmt));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Defer::build(BUILDER, LOC((yyloc)), blockStyle,
                             std::move(stmts));
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6329 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 702 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6335 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 703 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6341 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 705 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6354 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 714 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6367 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 723 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6375 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 727 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6386 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 734 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6395 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 741 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6401 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 742 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6407 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 743 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6413 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 744 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6419 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 745 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6425 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 746 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6431 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 747 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6437 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 749 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    YYLTYPE locBodyAnchor = context->makeLocationAtLast((yylsp[-1]));
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]),
                               false, locBodyAnchor, (yyvsp[0].commentsAndStmt));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Begin::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].withClause)), blockStyle,
                             std::move(stmts));
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6455 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 763 "chpl.ypp"
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
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6474 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 778 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6486 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 786 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Local::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)), blockStyle,
                             std::move(stmts));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6502 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL do_stmt  */
#line 798 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-1]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Local::build(BUILDER, LOC((yyloc)), blockStyle, std::move(stmts));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6517 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TON expr do_stmt  */
#line 809 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = On::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)), blockStyle,
                          std::move(stmts));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6533 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 821 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Serial::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)), blockStyle,
                              std::move(stmts));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6549 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL do_stmt  */
#line 833 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-1]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Serial::build(BUILDER, LOC((yyloc)), blockStyle, std::move(stmts));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6564 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSYNC stmt  */
#line 844 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-1]),
                               false, (yylsp[-1]), (yyvsp[0].commentsAndStmt));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Sync::build(BUILDER, LOC((yyloc)), blockStyle,
                            std::move(stmts));
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6581 "bison-chpl-lib.cpp"
    break;

  case 53: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 880 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), true, (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6589 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace  */
#line 884 "chpl.ypp"
 {
  // this allows us to distinguish between @attribute() and @attribute
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, (yyvsp[0].exprList), lst);
 }
#line 6600 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_stmt: attribute_decl_begin STRINGLITERAL  */
#line 891 "chpl.ypp"
 {
  // this allows us to maintain @unstable "message" without requiring parens
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  auto actual = makeMaybeNamedActual((yyvsp[0].expr));
  lst->push_back(actual);
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, context->makeList(), lst);
 }
#line 6613 "bison-chpl-lib.cpp"
    break;

  case 56: /* attribute_decl_begin: TATMARK TIDENT  */
#line 903 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6622 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: %empty  */
#line 910 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6630 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 914 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6641 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP TRP  */
#line 924 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6647 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 926 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6655 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 933 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6664 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 938 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6674 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 951 "chpl.ypp"
    {
      // take into account location of public/private, if any
      auto loc = context->declStartLoc((yylsp[-1]));

      ModuleParts parts = {
        .comments=context->gatherComments(loc),
        .attributeGroup=context->buildAttributeGroup((yyloc)).release(),
        .visibility=(yyvsp[-3].visibilityTag),
        .kind=(yyvsp[-2].moduleKind),
        .name=(yyvsp[0].uniqueStr)
      };

      (yyval.moduleParts) = parts;
      context->resetDeclState();
      context->clearComments();
    }
#line 6695 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 971 "chpl.ypp"
    {
      context->clearCommentsBefore((yylsp[-1]));

      ModuleParts parts = (yyvsp[-2].moduleParts);
      ParserExprList* body = context->makeList();
      context->appendList(body, context->gatherComments((yylsp[0])));
      auto mod = Module::build(BUILDER, LOC((yylsp[-2])), toOwned(parts.attributeGroup),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6714 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 986 "chpl.ypp"
    {
      context->clearCommentsBefore((yylsp[-2]));

      ModuleParts parts = (yyvsp[-3].moduleParts);
      ParserExprList* body = (yyvsp[-1].exprList);
      context->appendList(body, context->gatherComments((yylsp[0])));
      auto mod = Module::build(BUILDER, LOC((yylsp[-3])), toOwned(parts.attributeGroup),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6733 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1001 "chpl.ypp"
    {
      ModuleParts parts = (yyvsp[-3].moduleParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      ParserExprList* body = context->makeList(std::move(err));
      auto mod = Module::build(BUILDER, LOC((yylsp[-3])), toOwned(parts.attributeGroup),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6750 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1016 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6756 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1017 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6763 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1019 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6770 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1024 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6776 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1025 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6783 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1031 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6793 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1049 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6801 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1053 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6809 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1057 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6821 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1068 "chpl.ypp"
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
#line 6840 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1086 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6846 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1087 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6852 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1092 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6860 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1096 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6868 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1100 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6877 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1105 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6885 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1109 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6893 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1113 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6902 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1123 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6911 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1128 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6921 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1134 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6932 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1141 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6941 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1146 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6952 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1153 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6964 "bison-chpl-lib.cpp"
    break;

  case 92: /* opt_only_ls: %empty  */
#line 1163 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6970 "bison-chpl-lib.cpp"
    break;

  case 93: /* opt_only_ls: renames_ls  */
#line 1164 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6976 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1169 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6985 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1174 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6997 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1182 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7010 "bison-chpl-lib.cpp"
    break;

  case 97: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1191 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7024 "bison-chpl-lib.cpp"
    break;

  case 98: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1201 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7036 "bison-chpl-lib.cpp"
    break;

  case 99: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1209 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7049 "bison-chpl-lib.cpp"
    break;

  case 100: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1218 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7063 "bison-chpl-lib.cpp"
    break;

  case 101: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1231 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7072 "bison-chpl-lib.cpp"
    break;

  case 102: /* import_expr: expr  */
#line 1239 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7080 "bison-chpl-lib.cpp"
    break;

  case 103: /* import_expr: expr TDOT all_op_name  */
#line 1243 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7089 "bison-chpl-lib.cpp"
    break;

  case 104: /* import_expr: expr TAS ident_use  */
#line 1248 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7099 "bison-chpl-lib.cpp"
    break;

  case 105: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1254 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7109 "bison-chpl-lib.cpp"
    break;

  case 106: /* import_ls: import_expr  */
#line 1262 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7115 "bison-chpl-lib.cpp"
    break;

  case 107: /* import_ls: import_ls TCOMMA import_expr  */
#line 1263 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7121 "bison-chpl-lib.cpp"
    break;

  case 108: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1268 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7131 "bison-chpl-lib.cpp"
    break;

  case 109: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1277 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7139 "bison-chpl-lib.cpp"
    break;

  case 110: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1281 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7147 "bison-chpl-lib.cpp"
    break;

  case 111: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1285 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7155 "bison-chpl-lib.cpp"
    break;

  case 112: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1289 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7163 "bison-chpl-lib.cpp"
    break;

  case 113: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1293 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7171 "bison-chpl-lib.cpp"
    break;

  case 114: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1297 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7179 "bison-chpl-lib.cpp"
    break;

  case 115: /* opt_label_ident: %empty  */
#line 1305 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7185 "bison-chpl-lib.cpp"
    break;

  case 116: /* opt_label_ident: TIDENT  */
#line 1306 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7191 "bison-chpl-lib.cpp"
    break;

  case 124: /* ident_use: TIDENT  */
#line 1331 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7197 "bison-chpl-lib.cpp"
    break;

  case 125: /* ident_use: TTHIS  */
#line 1332 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7203 "bison-chpl-lib.cpp"
    break;

  case 145: /* scalar_type: TBOOL  */
#line 1359 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7209 "bison-chpl-lib.cpp"
    break;

  case 146: /* scalar_type: TENUM  */
#line 1360 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7215 "bison-chpl-lib.cpp"
    break;

  case 147: /* scalar_type: TINT  */
#line 1361 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7221 "bison-chpl-lib.cpp"
    break;

  case 148: /* scalar_type: TUINT  */
#line 1362 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7227 "bison-chpl-lib.cpp"
    break;

  case 149: /* scalar_type: TREAL  */
#line 1363 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7233 "bison-chpl-lib.cpp"
    break;

  case 150: /* scalar_type: TIMAG  */
#line 1364 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7239 "bison-chpl-lib.cpp"
    break;

  case 151: /* scalar_type: TCOMPLEX  */
#line 1365 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7245 "bison-chpl-lib.cpp"
    break;

  case 152: /* scalar_type: TBYTES  */
#line 1366 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7251 "bison-chpl-lib.cpp"
    break;

  case 153: /* scalar_type: TSTRING  */
#line 1367 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7257 "bison-chpl-lib.cpp"
    break;

  case 154: /* scalar_type: TLOCALE  */
#line 1368 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7263 "bison-chpl-lib.cpp"
    break;

  case 155: /* scalar_type: TNOTHING  */
#line 1369 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7269 "bison-chpl-lib.cpp"
    break;

  case 156: /* scalar_type: TVOID  */
#line 1370 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7275 "bison-chpl-lib.cpp"
    break;

  case 161: /* do_stmt: TDO stmt  */
#line 1384 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7281 "bison-chpl-lib.cpp"
    break;

  case 162: /* do_stmt: block_stmt  */
#line 1385 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7287 "bison-chpl-lib.cpp"
    break;

  case 163: /* return_stmt: TRETURN TSEMI  */
#line 1390 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7298 "bison-chpl-lib.cpp"
    break;

  case 164: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1397 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7309 "bison-chpl-lib.cpp"
    break;

  case 165: /* class_level_stmt: TSEMI  */
#line 1407 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7317 "bison-chpl-lib.cpp"
    break;

  case 166: /* class_level_stmt: inner_class_level_stmt  */
#line 1411 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7327 "bison-chpl-lib.cpp"
    break;

  case 167: /* $@1: %empty  */
#line 1416 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7334 "bison-chpl-lib.cpp"
    break;

  case 168: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1418 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7343 "bison-chpl-lib.cpp"
    break;

  case 169: /* $@2: %empty  */
#line 1422 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7350 "bison-chpl-lib.cpp"
    break;

  case 170: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1424 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7359 "bison-chpl-lib.cpp"
    break;

  case 178: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1442 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7368 "bison-chpl-lib.cpp"
    break;

  case 179: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1447 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7377 "bison-chpl-lib.cpp"
    break;

  case 180: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1452 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7386 "bison-chpl-lib.cpp"
    break;

  case 181: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1457 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7394 "bison-chpl-lib.cpp"
    break;

  case 182: /* forwarding_decl_start: TFORWARDING  */
#line 1464 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7403 "bison-chpl-lib.cpp"
    break;

  case 183: /* extern_or_export: TEXTERN  */
#line 1471 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7409 "bison-chpl-lib.cpp"
    break;

  case 184: /* extern_or_export: TEXPORT  */
#line 1472 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7415 "bison-chpl-lib.cpp"
    break;

  case 185: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1477 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7425 "bison-chpl-lib.cpp"
    break;

  case 186: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1487 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7434 "bison-chpl-lib.cpp"
    break;

  case 187: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1493 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7445 "bison-chpl-lib.cpp"
    break;

  case 188: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1500 "chpl.ypp"
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
#line 7467 "bison-chpl-lib.cpp"
    break;

  case 189: /* $@3: %empty  */
#line 1518 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7475 "bison-chpl-lib.cpp"
    break;

  case 190: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1522 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7484 "bison-chpl-lib.cpp"
    break;

  case 191: /* $@4: %empty  */
#line 1527 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7493 "bison-chpl-lib.cpp"
    break;

  case 192: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1532 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7502 "bison-chpl-lib.cpp"
    break;

  case 193: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1540 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7510 "bison-chpl-lib.cpp"
    break;

  case 194: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1547 "chpl.ypp"
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
                               toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7532 "bison-chpl-lib.cpp"
    break;

  case 195: /* loop_stmt: TWHILE expr do_stmt  */
#line 1565 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto body = context->consumeToBlock((yylsp[0]), exprLst);
    auto node = While::build(BUILDER, LOC((yylsp[-2])), toOwned((yyvsp[-1].expr)),
                             blockStyle,
                             std::move(body));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7549 "bison-chpl-lib.cpp"
    break;

  case 196: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1578 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto body = context->consumeToBlock((yylsp[0]), exprLst);
    auto node = While::build(BUILDER, LOC((yylsp[-2])), toOwned((yyvsp[-1].expr)),
                             blockStyle,
                             std::move(body));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7566 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1591 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7574 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1595 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7582 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1599 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7590 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1603 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7598 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1607 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7606 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFOR expr do_stmt  */
#line 1611 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7614 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1615 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7622 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1619 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-5]), (yylsp[0]), (yyvsp[0].blockOrDo));
    AstNode* ident = context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr));
    auto index = context->buildLoopIndexDecl((yylsp[-3]), toOwned(ident));
    auto body = context->consumeToBlock((yylsp[0]), exprLst);
    auto node = For::build(BUILDER, LOC((yylsp[-5])), std::move(index),
                           toOwned((yyvsp[-1].expr)),
                           blockStyle,
                           std::move(body),
                           /*isExpressionLevel*/ false,
                           /*isParam*/ true,
                           context->buildAttributeGroup((yyloc)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    context->resetAttributeGroupPartsState();
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7646 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1639 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7654 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1643 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7662 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7670 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1651 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7678 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFORALL expr do_stmt  */
#line 1655 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7686 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1659 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7694 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1663 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7702 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1667 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7710 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1671 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7718 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1675 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7726 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1679 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7734 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1683 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7742 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1687 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7750 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1691 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7758 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1695 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7766 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1699 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7774 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1703 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7782 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1707 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7790 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1711 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7798 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1715 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7806 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1719 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7814 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1723 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7822 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1727 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7831 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1732 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7840 "bison-chpl-lib.cpp"
    break;

  case 229: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1740 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7850 "bison-chpl-lib.cpp"
    break;

  case 230: /* if_stmt: TIF expr TTHEN stmt  */
#line 1749 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7858 "bison-chpl-lib.cpp"
    break;

  case 231: /* if_stmt: TIF expr block_stmt  */
#line 1753 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7866 "bison-chpl-lib.cpp"
    break;

  case 232: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1757 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7874 "bison-chpl-lib.cpp"
    break;

  case 233: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1761 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7882 "bison-chpl-lib.cpp"
    break;

  case 234: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1765 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7890 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF ifvar block_stmt  */
#line 1769 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7898 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1773 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7906 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1777 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7914 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1781 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7923 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1786 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7932 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1791 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7941 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1796 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7950 "bison-chpl-lib.cpp"
    break;

  case 242: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1804 "chpl.ypp"
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
#line 7969 "bison-chpl-lib.cpp"
    break;

  case 243: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1819 "chpl.ypp"
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
#line 7988 "bison-chpl-lib.cpp"
    break;

  case 244: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1837 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7996 "bison-chpl-lib.cpp"
    break;

  case 245: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1841 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8004 "bison-chpl-lib.cpp"
    break;

  case 246: /* ifc_formal_ls: ifc_formal  */
#line 1847 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8010 "bison-chpl-lib.cpp"
    break;

  case 247: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1848 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8016 "bison-chpl-lib.cpp"
    break;

  case 248: /* ifc_formal: ident_def  */
#line 1853 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8022 "bison-chpl-lib.cpp"
    break;

  case 261: /* implements_type_ident: implements_type_error_ident  */
#line 1871 "chpl.ypp"
  {
    context->syntax((yyloc), "type '%s' cannot implement an interface.", (yyvsp[0].uniqueStr).c_str());
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8031 "bison-chpl-lib.cpp"
    break;

  case 268: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1897 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8039 "bison-chpl-lib.cpp"
    break;

  case 269: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1901 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8047 "bison-chpl-lib.cpp"
    break;

  case 270: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1905 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8055 "bison-chpl-lib.cpp"
    break;

  case 271: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1912 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8063 "bison-chpl-lib.cpp"
    break;

  case 272: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1916 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8071 "bison-chpl-lib.cpp"
    break;

  case 273: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1920 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8079 "bison-chpl-lib.cpp"
    break;

  case 274: /* try_stmt: TTRY tryable_stmt  */
#line 1927 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8087 "bison-chpl-lib.cpp"
    break;

  case 275: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1931 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8095 "bison-chpl-lib.cpp"
    break;

  case 276: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1935 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8103 "bison-chpl-lib.cpp"
    break;

  case 277: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1939 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8111 "bison-chpl-lib.cpp"
    break;

  case 278: /* catch_expr_ls: %empty  */
#line 1945 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8117 "bison-chpl-lib.cpp"
    break;

  case 279: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1946 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8123 "bison-chpl-lib.cpp"
    break;

  case 280: /* catch_expr: TCATCH block_stmt  */
#line 1951 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8131 "bison-chpl-lib.cpp"
    break;

  case 281: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1955 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8139 "bison-chpl-lib.cpp"
    break;

  case 282: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1959 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8147 "bison-chpl-lib.cpp"
    break;

  case 283: /* catch_expr_inner: ident_def  */
#line 1966 "chpl.ypp"
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
#line 8164 "bison-chpl-lib.cpp"
    break;

  case 284: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1979 "chpl.ypp"
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
#line 8181 "bison-chpl-lib.cpp"
    break;

  case 285: /* throw_stmt: TTHROW expr TSEMI  */
#line 1995 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8191 "bison-chpl-lib.cpp"
    break;

  case 286: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2003 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8199 "bison-chpl-lib.cpp"
    break;

  case 287: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2007 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8209 "bison-chpl-lib.cpp"
    break;

  case 288: /* when_stmt_ls: %empty  */
#line 2015 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8215 "bison-chpl-lib.cpp"
    break;

  case 289: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2016 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8221 "bison-chpl-lib.cpp"
    break;

  case 290: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2021 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8229 "bison-chpl-lib.cpp"
    break;

  case 291: /* when_stmt: TOTHERWISE stmt  */
#line 2025 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8238 "bison-chpl-lib.cpp"
    break;

  case 292: /* when_stmt: TOTHERWISE TDO stmt  */
#line 2030 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8247 "bison-chpl-lib.cpp"
    break;

  case 293: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2038 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8256 "bison-chpl-lib.cpp"
    break;

  case 294: /* manager_expr: expr TAS ident_def  */
#line 2043 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8264 "bison-chpl-lib.cpp"
    break;

  case 295: /* manager_expr: expr  */
#line 2047 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8272 "bison-chpl-lib.cpp"
    break;

  case 296: /* manager_expr_ls: manager_expr  */
#line 2053 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8278 "bison-chpl-lib.cpp"
    break;

  case 297: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2054 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8284 "bison-chpl-lib.cpp"
    break;

  case 298: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2059 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8292 "bison-chpl-lib.cpp"
    break;

  case 299: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2068 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8301 "bison-chpl-lib.cpp"
    break;

  case 300: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2073 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8312 "bison-chpl-lib.cpp"
    break;

  case 301: /* class_start: class_tag ident_def  */
#line 2084 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8320 "bison-chpl-lib.cpp"
    break;

  case 302: /* class_tag: TCLASS  */
#line 2090 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8326 "bison-chpl-lib.cpp"
    break;

  case 303: /* class_tag: TRECORD  */
#line 2091 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8332 "bison-chpl-lib.cpp"
    break;

  case 304: /* class_tag: TUNION  */
#line 2092 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8338 "bison-chpl-lib.cpp"
    break;

  case 305: /* opt_inherit: %empty  */
#line 2096 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8344 "bison-chpl-lib.cpp"
    break;

  case 306: /* opt_inherit: TCOLON expr_ls  */
#line 2097 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8350 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_level_stmt_ls: %empty  */
#line 2101 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8359 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2106 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8367 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2110 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8375 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2114 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8383 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2118 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8391 "bison-chpl-lib.cpp"
    break;

  case 312: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2125 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      ParserExprList* list = (yyvsp[-1].exprList);
      // get any comments after the last element but before the closing brace
      context->appendList(list, context->gatherComments((yylsp[0])));

      auto decl = Enum::build(BUILDER, LOC((yyloc)), toOwned(parts.attributeGroup),
                              parts.visibility,
                              parts.name,
                              context->consumeList(list));
      CommentsAndStmt cs = {parts.comments, decl.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8412 "bison-chpl-lib.cpp"
    break;

  case 313: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2142 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8426 "bison-chpl-lib.cpp"
    break;

  case 314: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2155 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8434 "bison-chpl-lib.cpp"
    break;

  case 315: /* enum_ls: enum_item  */
#line 2162 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8443 "bison-chpl-lib.cpp"
    break;

  case 316: /* enum_ls: enum_ls TCOMMA  */
#line 2167 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8453 "bison-chpl-lib.cpp"
    break;

  case 317: /* $@5: %empty  */
#line 2173 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8462 "bison-chpl-lib.cpp"
    break;

  case 318: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2178 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8471 "bison-chpl-lib.cpp"
    break;

  case 319: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2183 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8480 "bison-chpl-lib.cpp"
    break;

  case 320: /* $@6: %empty  */
#line 2188 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8489 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2193 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8498 "bison-chpl-lib.cpp"
    break;

  case 322: /* enum_item: ident_def  */
#line 2203 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8509 "bison-chpl-lib.cpp"
    break;

  case 323: /* enum_item: ident_def TASSIGN expr  */
#line 2210 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8522 "bison-chpl-lib.cpp"
    break;

  case 324: /* lambda_decl_start: TLAMBDA  */
#line 2222 "chpl.ypp"
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
#line 8538 "bison-chpl-lib.cpp"
    break;

  case 325: /* $@7: %empty  */
#line 2239 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8548 "bison-chpl-lib.cpp"
    break;

  case 326: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2245 "chpl.ypp"
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
#line 8570 "bison-chpl-lib.cpp"
    break;

  case 328: /* linkage_spec: linkage_spec_empty  */
#line 2268 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8576 "bison-chpl-lib.cpp"
    break;

  case 329: /* linkage_spec: TINLINE  */
#line 2269 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8583 "bison-chpl-lib.cpp"
    break;

  case 330: /* linkage_spec: TOVERRIDE  */
#line 2271 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8590 "bison-chpl-lib.cpp"
    break;

  case 331: /* opt_fn_type_formal_ls: %empty  */
#line 2276 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8596 "bison-chpl-lib.cpp"
    break;

  case 332: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2277 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8602 "bison-chpl-lib.cpp"
    break;

  case 333: /* fn_type_formal_ls: fn_type_formal  */
#line 2281 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8608 "bison-chpl-lib.cpp"
    break;

  case 334: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2282 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8614 "bison-chpl-lib.cpp"
    break;

  case 335: /* fn_type_formal: named_formal  */
#line 2287 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8620 "bison-chpl-lib.cpp"
    break;

  case 336: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2290 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8626 "bison-chpl-lib.cpp"
    break;

  case 337: /* fn_type_formal: formal_type  */
#line 2292 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8632 "bison-chpl-lib.cpp"
    break;

  case 338: /* opt_fn_type_ret_type: %empty  */
#line 2296 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8638 "bison-chpl-lib.cpp"
    break;

  case 339: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2297 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8644 "bison-chpl-lib.cpp"
    break;

  case 340: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2310 "chpl.ypp"
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
#line 8663 "bison-chpl-lib.cpp"
    break;

  case 341: /* $@8: %empty  */
#line 2328 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8673 "bison-chpl-lib.cpp"
    break;

  case 342: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2334 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8686 "bison-chpl-lib.cpp"
    break;

  case 343: /* $@9: %empty  */
#line 2343 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8696 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2349 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8709 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2361 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8717 "bison-chpl-lib.cpp"
    break;

  case 346: /* $@10: %empty  */
#line 2370 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-4].functionParts).errorExpr == nullptr) {
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
  }
#line 8731 "bison-chpl-lib.cpp"
    break;

  case 347: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2380 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-6].functionParts);
    fp.returnIntent = (yyvsp[-5].returnTag);
    fp.returnType = (yyvsp[-4].expr);
    fp.throws = ((yyvsp[-3].throwsTag) == ThrowsTag_THROWS);
    WhereAndLifetime wl = (yyvsp[-2].lifetimeAndWhere);
    fp.body = (yyvsp[0].exprList);
    fp.lifetime = wl.lifetime;
    fp.where = wl.where;
    context->clearComments();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-6].functionParts).errorExpr == nullptr) {
      context->exitScope(asttags::Function, (yyvsp[-6].functionParts).name->name());
    }

    (yyval.functionParts) = fp;
  }
#line 8754 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2402 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8766 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2410 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8778 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2418 "chpl.ypp"
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
#line 8793 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2429 "chpl.ypp"
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
#line 8808 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2440 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8818 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2449 "chpl.ypp"
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
#line 8834 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2464 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8840 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_ident: ident_def TBANG  */
#line 2471 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8850 "bison-chpl-lib.cpp"
    break;

  case 398: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2528 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8856 "bison-chpl-lib.cpp"
    break;

  case 399: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2529 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8862 "bison-chpl-lib.cpp"
    break;

  case 400: /* opt_formal_ls: %empty  */
#line 2533 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8868 "bison-chpl-lib.cpp"
    break;

  case 401: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2534 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8874 "bison-chpl-lib.cpp"
    break;

  case 402: /* req_formal_ls: TLP TRP  */
#line 2538 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8880 "bison-chpl-lib.cpp"
    break;

  case 403: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2539 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8886 "bison-chpl-lib.cpp"
    break;

  case 404: /* formal_ls_inner: formal  */
#line 2543 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8892 "bison-chpl-lib.cpp"
    break;

  case 405: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2544 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8898 "bison-chpl-lib.cpp"
    break;

  case 406: /* formal_ls: %empty  */
#line 2548 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8904 "bison-chpl-lib.cpp"
    break;

  case 407: /* formal_ls: formal_ls_inner  */
#line 2549 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8910 "bison-chpl-lib.cpp"
    break;

  case 411: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2563 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8918 "bison-chpl-lib.cpp"
    break;

  case 412: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2568 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8926 "bison-chpl-lib.cpp"
    break;

  case 413: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2573 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8934 "bison-chpl-lib.cpp"
    break;

  case 414: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2578 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8942 "bison-chpl-lib.cpp"
    break;

  case 415: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2583 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8950 "bison-chpl-lib.cpp"
    break;

  case 416: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2588 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8958 "bison-chpl-lib.cpp"
    break;

  case 417: /* opt_formal_intent_tag: %empty  */
#line 2594 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8967 "bison-chpl-lib.cpp"
    break;

  case 418: /* opt_formal_intent_tag: required_intent_tag  */
#line 2599 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8976 "bison-chpl-lib.cpp"
    break;

  case 419: /* required_intent_tag: TIN  */
#line 2606 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8982 "bison-chpl-lib.cpp"
    break;

  case 420: /* required_intent_tag: TINOUT  */
#line 2607 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8988 "bison-chpl-lib.cpp"
    break;

  case 421: /* required_intent_tag: TOUT  */
#line 2608 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8994 "bison-chpl-lib.cpp"
    break;

  case 422: /* required_intent_tag: TCONST TIN  */
#line 2609 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9000 "bison-chpl-lib.cpp"
    break;

  case 423: /* required_intent_tag: TCONST TREF  */
#line 2610 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9006 "bison-chpl-lib.cpp"
    break;

  case 424: /* required_intent_tag: TCONST  */
#line 2611 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9012 "bison-chpl-lib.cpp"
    break;

  case 425: /* required_intent_tag: TPARAM  */
#line 2612 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9018 "bison-chpl-lib.cpp"
    break;

  case 426: /* required_intent_tag: TREF  */
#line 2613 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9024 "bison-chpl-lib.cpp"
    break;

  case 427: /* required_intent_tag: TTYPE  */
#line 2614 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 428: /* opt_this_intent_tag: %empty  */
#line 2618 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9036 "bison-chpl-lib.cpp"
    break;

  case 429: /* opt_this_intent_tag: TPARAM  */
#line 2619 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9042 "bison-chpl-lib.cpp"
    break;

  case 430: /* opt_this_intent_tag: TREF  */
#line 2620 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9048 "bison-chpl-lib.cpp"
    break;

  case 431: /* opt_this_intent_tag: TCONST TREF  */
#line 2621 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9054 "bison-chpl-lib.cpp"
    break;

  case 432: /* opt_this_intent_tag: TCONST  */
#line 2622 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9060 "bison-chpl-lib.cpp"
    break;

  case 433: /* opt_this_intent_tag: TTYPE  */
#line 2623 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9066 "bison-chpl-lib.cpp"
    break;

  case 434: /* proc_iter_or_op: TPROC  */
#line 2627 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9072 "bison-chpl-lib.cpp"
    break;

  case 435: /* proc_iter_or_op: TITER  */
#line 2628 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9078 "bison-chpl-lib.cpp"
    break;

  case 436: /* proc_iter_or_op: TOPERATOR  */
#line 2629 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9084 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_ret_tag: %empty  */
#line 2633 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9090 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_ret_tag: TCONST  */
#line 2634 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 9096 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_ret_tag: TCONST TREF  */
#line 2635 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 9102 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_ret_tag: TREF  */
#line 2636 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 9108 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_ret_tag: TPARAM  */
#line 2637 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 9114 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_ret_tag: TTYPE  */
#line 2638 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 9120 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_throws_error: %empty  */
#line 2642 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9126 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_throws_error: TTHROWS  */
#line 2643 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9132 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_function_body_stmt: TSEMI  */
#line 2646 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9138 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_function_body_stmt: function_body_stmt  */
#line 2647 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9144 "bison-chpl-lib.cpp"
    break;

  case 447: /* function_body_stmt: block_stmt_body  */
#line 2651 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9150 "bison-chpl-lib.cpp"
    break;

  case 448: /* function_body_stmt: TDO toplevel_stmt  */
#line 2652 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 9156 "bison-chpl-lib.cpp"
    break;

  case 449: /* function_body_stmt: return_stmt  */
#line 2653 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt), &((yylsp[0]))); }
#line 9162 "bison-chpl-lib.cpp"
    break;

  case 450: /* query_expr: TQUERIEDIDENT  */
#line 2657 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9168 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_lifetime_where: %empty  */
#line 2662 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9174 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_lifetime_where: TWHERE expr  */
#line 2664 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9180 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2666 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9186 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2668 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9192 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2670 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9198 "bison-chpl-lib.cpp"
    break;

  case 456: /* lifetime_components_expr: lifetime_expr  */
#line 2675 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9204 "bison-chpl-lib.cpp"
    break;

  case 457: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2677 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9210 "bison-chpl-lib.cpp"
    break;

  case 458: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2682 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9216 "bison-chpl-lib.cpp"
    break;

  case 459: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2684 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9222 "bison-chpl-lib.cpp"
    break;

  case 460: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2686 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9228 "bison-chpl-lib.cpp"
    break;

  case 461: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2688 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9234 "bison-chpl-lib.cpp"
    break;

  case 462: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2690 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9240 "bison-chpl-lib.cpp"
    break;

  case 463: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2692 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9246 "bison-chpl-lib.cpp"
    break;

  case 464: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2694 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9252 "bison-chpl-lib.cpp"
    break;

  case 465: /* lifetime_ident: TIDENT  */
#line 2698 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9258 "bison-chpl-lib.cpp"
    break;

  case 466: /* lifetime_ident: TTHIS  */
#line 2699 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9264 "bison-chpl-lib.cpp"
    break;

  case 467: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2704 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9273 "bison-chpl-lib.cpp"
    break;

  case 468: /* type_alias_decl_stmt_start: TTYPE  */
#line 2713 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9281 "bison-chpl-lib.cpp"
    break;

  case 469: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2717 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9290 "bison-chpl-lib.cpp"
    break;

  case 470: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2725 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9298 "bison-chpl-lib.cpp"
    break;

  case 471: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2729 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9306 "bison-chpl-lib.cpp"
    break;

  case 472: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2736 "chpl.ypp"
  {
    // TODO (dlongnecke-cray): Add a helper to build this and var_decl_stmt.
    auto node = Variable::build(BUILDER, LOC((yyloc)),
                                context->buildAttributeGroup((yyloc)),
                                context->visibility,
                                context->linkage,
                                context->consumeVarDeclLinkageName(),
                                /*name*/ (yyvsp[-1].uniqueStr),
                                context->varDeclKind,
                                context->isVarDeclConfig,
                                context->currentScopeIsAggregate(),
                                /*typeExpression*/ nullptr,
                                toOwned((yyvsp[0].expr)));
      // Gather the comments like a statement does.
      (yyval.commentsAndStmt) = STMT((yylsp[-1]), node.release());
      context->clearComments();
  }
#line 9328 "bison-chpl-lib.cpp"
    break;

  case 473: /* opt_init_type: %empty  */
#line 2756 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9334 "bison-chpl-lib.cpp"
    break;

  case 474: /* opt_init_type: TASSIGN expr  */
#line 2758 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9340 "bison-chpl-lib.cpp"
    break;

  case 475: /* var_decl_type: TPARAM  */
#line 2762 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9346 "bison-chpl-lib.cpp"
    break;

  case 476: /* var_decl_type: TCONST TREF  */
#line 2763 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9352 "bison-chpl-lib.cpp"
    break;

  case 477: /* var_decl_type: TREF  */
#line 2764 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9358 "bison-chpl-lib.cpp"
    break;

  case 478: /* var_decl_type: TCONST  */
#line 2765 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9364 "bison-chpl-lib.cpp"
    break;

  case 479: /* var_decl_type: TVAR  */
#line 2766 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9370 "bison-chpl-lib.cpp"
    break;

  case 480: /* $@11: %empty  */
#line 2771 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9379 "bison-chpl-lib.cpp"
    break;

  case 481: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2775 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9388 "bison-chpl-lib.cpp"
    break;

  case 482: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2780 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9397 "bison-chpl-lib.cpp"
    break;

  case 483: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2788 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9405 "bison-chpl-lib.cpp"
    break;

  case 484: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2792 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9413 "bison-chpl-lib.cpp"
    break;

  case 485: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2799 "chpl.ypp"
    {
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     context->visibility,
                                     context->linkage,
                                     context->consumeVarDeclLinkageName(),
                                     /*name*/ (yyvsp[-2].uniqueStr),
                                     context->varDeclKind,
                                     context->isVarDeclConfig,
                                     context->currentScopeIsAggregate(),
                                     toOwned((yyvsp[-1].expr)), toOwned((yyvsp[0].expr)));
      // gather the comments like a statement does
      (yyval.commentsAndStmt) = STMT((yylsp[-2]), varDecl.release());
      context->clearComments();
    }
#line 9433 "bison-chpl-lib.cpp"
    break;

  case 486: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2815 "chpl.ypp"
    {
      auto intentOrKind = (TupleDecl::IntentOrKind) context->varDeclKind;
      auto tupleDecl = TupleDecl::build(BUILDER, LOC((yyloc)),
                                        context->buildAttributeGroup((yyloc)),
                                        context->visibility,
                                        context->linkage,
                                        intentOrKind,
                                        context->consumeList((yyvsp[-3].exprList)),
                                        toOwned((yyvsp[-1].expr)),
                                        toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yylsp[-4]), tupleDecl.release());
      context->clearComments();
    }
#line 9451 "bison-chpl-lib.cpp"
    break;

  case 487: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2832 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9459 "bison-chpl-lib.cpp"
    break;

  case 488: /* tuple_var_decl_component: ident_def  */
#line 2836 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9467 "bison-chpl-lib.cpp"
    break;

  case 489: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2840 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9475 "bison-chpl-lib.cpp"
    break;

  case 490: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2847 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9481 "bison-chpl-lib.cpp"
    break;

  case 491: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2849 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9487 "bison-chpl-lib.cpp"
    break;

  case 492: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2851 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9493 "bison-chpl-lib.cpp"
    break;

  case 493: /* opt_init_expr: %empty  */
#line 2857 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9499 "bison-chpl-lib.cpp"
    break;

  case 494: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2858 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9505 "bison-chpl-lib.cpp"
    break;

  case 495: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2859 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9511 "bison-chpl-lib.cpp"
    break;

  case 496: /* formal_or_ret_type_expr: expr  */
#line 2863 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9517 "bison-chpl-lib.cpp"
    break;

  case 497: /* ret_type: formal_or_ret_type_expr  */
#line 2867 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9523 "bison-chpl-lib.cpp"
    break;

  case 498: /* ret_type: reserved_type_ident_use  */
#line 2868 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9529 "bison-chpl-lib.cpp"
    break;

  case 499: /* ret_type: error  */
#line 2869 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9535 "bison-chpl-lib.cpp"
    break;

  case 500: /* colon_ret_type: TCOLON ret_type  */
#line 2873 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9541 "bison-chpl-lib.cpp"
    break;

  case 501: /* colon_ret_type: error  */
#line 2874 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9547 "bison-chpl-lib.cpp"
    break;

  case 502: /* opt_ret_type: %empty  */
#line 2878 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9553 "bison-chpl-lib.cpp"
    break;

  case 504: /* opt_type: %empty  */
#line 2883 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9559 "bison-chpl-lib.cpp"
    break;

  case 505: /* opt_type: TCOLON expr  */
#line 2884 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9565 "bison-chpl-lib.cpp"
    break;

  case 506: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2885 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9571 "bison-chpl-lib.cpp"
    break;

  case 507: /* opt_type: error  */
#line 2886 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9577 "bison-chpl-lib.cpp"
    break;

  case 508: /* formal_type: formal_or_ret_type_expr  */
#line 2890 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9583 "bison-chpl-lib.cpp"
    break;

  case 509: /* formal_type: reserved_type_ident_use  */
#line 2891 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9589 "bison-chpl-lib.cpp"
    break;

  case 510: /* colon_formal_type: TCOLON formal_type  */
#line 2895 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9595 "bison-chpl-lib.cpp"
    break;

  case 511: /* opt_colon_formal_type: %empty  */
#line 2899 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9601 "bison-chpl-lib.cpp"
    break;

  case 512: /* opt_colon_formal_type: colon_formal_type  */
#line 2900 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9607 "bison-chpl-lib.cpp"
    break;

  case 513: /* expr_ls: expr  */
#line 2906 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9613 "bison-chpl-lib.cpp"
    break;

  case 514: /* expr_ls: expr_ls TCOMMA expr  */
#line 2907 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9619 "bison-chpl-lib.cpp"
    break;

  case 515: /* tuple_component: TUNDERSCORE  */
#line 2911 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9625 "bison-chpl-lib.cpp"
    break;

  case 516: /* tuple_component: opt_try_expr  */
#line 2912 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9631 "bison-chpl-lib.cpp"
    break;

  case 517: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2917 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9639 "bison-chpl-lib.cpp"
    break;

  case 518: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2921 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9647 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_actual_ls: %empty  */
#line 2927 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9653 "bison-chpl-lib.cpp"
    break;

  case 520: /* opt_actual_ls: actual_ls  */
#line 2928 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9659 "bison-chpl-lib.cpp"
    break;

  case 521: /* actual_ls: actual_expr  */
#line 2933 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9668 "bison-chpl-lib.cpp"
    break;

  case 522: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2938 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9678 "bison-chpl-lib.cpp"
    break;

  case 523: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2946 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9684 "bison-chpl-lib.cpp"
    break;

  case 524: /* actual_expr: opt_try_expr  */
#line 2947 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9690 "bison-chpl-lib.cpp"
    break;

  case 525: /* ident_expr: ident_use  */
#line 2951 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9696 "bison-chpl-lib.cpp"
    break;

  case 526: /* ident_expr: scalar_type  */
#line 2952 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9702 "bison-chpl-lib.cpp"
    break;

  case 532: /* sub_type_level_expr: TSINGLE expr  */
#line 2965 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9708 "bison-chpl-lib.cpp"
    break;

  case 533: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2967 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9714 "bison-chpl-lib.cpp"
    break;

  case 534: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2969 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9720 "bison-chpl-lib.cpp"
    break;

  case 535: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2971 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9726 "bison-chpl-lib.cpp"
    break;

  case 536: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2973 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9736 "bison-chpl-lib.cpp"
    break;

  case 537: /* sub_type_level_expr: TATOMIC expr  */
#line 2979 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9742 "bison-chpl-lib.cpp"
    break;

  case 538: /* sub_type_level_expr: TSYNC expr  */
#line 2981 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9748 "bison-chpl-lib.cpp"
    break;

  case 539: /* sub_type_level_expr: TOWNED  */
#line 2984 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9754 "bison-chpl-lib.cpp"
    break;

  case 540: /* sub_type_level_expr: TOWNED expr  */
#line 2986 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9760 "bison-chpl-lib.cpp"
    break;

  case 541: /* sub_type_level_expr: TUNMANAGED  */
#line 2988 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9766 "bison-chpl-lib.cpp"
    break;

  case 542: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2990 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9772 "bison-chpl-lib.cpp"
    break;

  case 543: /* sub_type_level_expr: TSHARED  */
#line 2992 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9778 "bison-chpl-lib.cpp"
    break;

  case 544: /* sub_type_level_expr: TSHARED expr  */
#line 2994 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9784 "bison-chpl-lib.cpp"
    break;

  case 545: /* sub_type_level_expr: TBORROWED  */
#line 2996 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9790 "bison-chpl-lib.cpp"
    break;

  case 546: /* sub_type_level_expr: TBORROWED expr  */
#line 2998 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9796 "bison-chpl-lib.cpp"
    break;

  case 547: /* sub_type_level_expr: TCLASS  */
#line 3000 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9802 "bison-chpl-lib.cpp"
    break;

  case 548: /* sub_type_level_expr: TRECORD  */
#line 3002 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9808 "bison-chpl-lib.cpp"
    break;

  case 549: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 3007 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->buildAttributeGroup((yyloc))).release();
    context->resetAttributeGroupPartsState();
  }
#line 9823 "bison-chpl-lib.cpp"
    break;

  case 550: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 3018 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->buildAttributeGroup((yyloc))).release();
                    context->resetAttributeGroupPartsState();
  }
#line 9838 "bison-chpl-lib.cpp"
    break;

  case 551: /* for_expr: TFOR expr TDO expr  */
#line 3029 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->buildAttributeGroup((yyloc))).release();
                    context->resetAttributeGroupPartsState();
  }
#line 9852 "bison-chpl-lib.cpp"
    break;

  case 552: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3039 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-7]), (yyvsp[-7].expr));
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-5].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock(ifLoc, ifExpr.release()),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->buildAttributeGroup((yyloc))).release();
                    context->resetAttributeGroupPartsState();
  }
#line 9872 "bison-chpl-lib.cpp"
    break;

  case 553: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3055 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-7]), (yyvsp[-7].expr));
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-5].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock(ifLoc, ifExpr.release()),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->buildAttributeGroup((yyloc))).release();
                    context->resetAttributeGroupPartsState();
  }
#line 9892 "bison-chpl-lib.cpp"
    break;

  case 554: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 3071 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr,
                    toOwned((yyvsp[-5].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock(ifLoc, ifExpr.release()),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->buildAttributeGroup((yyloc))).release();
                    context->resetAttributeGroupPartsState();
  }
#line 9912 "bison-chpl-lib.cpp"
    break;

  case 555: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3087 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 9925 "bison-chpl-lib.cpp"
    break;

  case 556: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3096 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 9938 "bison-chpl-lib.cpp"
    break;

  case 557: /* for_expr: TFORALL expr TDO expr  */
#line 3105 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 9950 "bison-chpl-lib.cpp"
    break;

  case 558: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3113 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-7]), (yyvsp[-7].expr));
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-5].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock(ifLoc, ifExpr.release()),
                       /*isExpressionLevel*/ true).release();
  }
#line 9968 "bison-chpl-lib.cpp"
    break;

  case 559: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3127 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-7]), (yyvsp[-7].expr));
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr)= Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-5].expr)),
                      /*withClause*/ nullptr,
                      BlockStyle::IMPLICIT,
                      context->consumeToBlock(ifLoc, ifExpr.release()),
                      /*isExpressionLevel*/ true).release();
  }
#line 9986 "bison-chpl-lib.cpp"
    break;

  case 560: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3141 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr,
                       toOwned((yyvsp[-5].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock(ifLoc, ifExpr.release()),
                       /*isExpressionLevel*/ true).release();
  }
#line 10004 "bison-chpl-lib.cpp"
    break;

  case 561: /* bracket_loop_expr: TLSBR TRSBR  */
#line 3158 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 10012 "bison-chpl-lib.cpp"
    break;

  case 562: /* bracket_loop_expr: TLSBR TRSBR expr  */
#line 3162 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 10020 "bison-chpl-lib.cpp"
    break;

  case 563: /* bracket_loop_expr: TLSBR expr_ls TRSBR expr  */
#line 3166 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10028 "bison-chpl-lib.cpp"
    break;

  case 564: /* bracket_loop_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3170 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10036 "bison-chpl-lib.cpp"
    break;

  case 565: /* bracket_loop_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3174 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10044 "bison-chpl-lib.cpp"
    break;

  case 566: /* bracket_loop_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3178 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10052 "bison-chpl-lib.cpp"
    break;

  case 567: /* bracket_loop_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3182 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10060 "bison-chpl-lib.cpp"
    break;

  case 568: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3189 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10074 "bison-chpl-lib.cpp"
    break;

  case 569: /* nil_expr: TNIL  */
#line 3206 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10080 "bison-chpl-lib.cpp"
    break;

  case 577: /* opt_task_intent_ls: %empty  */
#line 3224 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10086 "bison-chpl-lib.cpp"
    break;

  case 578: /* opt_task_intent_ls: task_intent_clause  */
#line 3225 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10092 "bison-chpl-lib.cpp"
    break;

  case 579: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3230 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10102 "bison-chpl-lib.cpp"
    break;

  case 580: /* task_intent_ls: intent_expr  */
#line 3238 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10108 "bison-chpl-lib.cpp"
    break;

  case 581: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3239 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10114 "bison-chpl-lib.cpp"
    break;

  case 582: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3244 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10124 "bison-chpl-lib.cpp"
    break;

  case 583: /* forall_intent_ls: intent_expr  */
#line 3252 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10130 "bison-chpl-lib.cpp"
    break;

  case 584: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3253 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10136 "bison-chpl-lib.cpp"
    break;

  case 585: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3258 "chpl.ypp"
  {
    if (auto ident = (yyvsp[-2].expr)->toIdentifier()) {
      auto name = ident->name();
      auto node = TaskVar::build(BUILDER, LOC((yyloc)), /*attributeGroup*/ nullptr,
                                 name,
                                 /*intent*/ (yyvsp[-3].taskIntent),
                                 toOwned((yyvsp[-1].expr)),
                                 toOwned((yyvsp[0].expr)));
      (yyval.expr) = node.release();
    } else {
      (yyval.expr) = context->syntax((yyloc), "expected identifier for task variable name.");
    }
  }
#line 10154 "bison-chpl-lib.cpp"
    break;

  case 586: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3272 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10162 "bison-chpl-lib.cpp"
    break;

  case 587: /* intent_expr: expr TREDUCE ident_expr  */
#line 3276 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10170 "bison-chpl-lib.cpp"
    break;

  case 588: /* task_var_prefix: TCONST  */
#line 3282 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10176 "bison-chpl-lib.cpp"
    break;

  case 589: /* task_var_prefix: TIN  */
#line 3283 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10182 "bison-chpl-lib.cpp"
    break;

  case 590: /* task_var_prefix: TCONST TIN  */
#line 3284 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10188 "bison-chpl-lib.cpp"
    break;

  case 591: /* task_var_prefix: TREF  */
#line 3285 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10194 "bison-chpl-lib.cpp"
    break;

  case 592: /* task_var_prefix: TCONST TREF  */
#line 3286 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10200 "bison-chpl-lib.cpp"
    break;

  case 593: /* task_var_prefix: TVAR  */
#line 3287 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10206 "bison-chpl-lib.cpp"
    break;

  case 594: /* new_maybe_decorated: TNEW  */
#line 3292 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10212 "bison-chpl-lib.cpp"
    break;

  case 595: /* new_maybe_decorated: TNEW TOWNED  */
#line 3294 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10218 "bison-chpl-lib.cpp"
    break;

  case 596: /* new_maybe_decorated: TNEW TSHARED  */
#line 3296 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10224 "bison-chpl-lib.cpp"
    break;

  case 597: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3298 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10230 "bison-chpl-lib.cpp"
    break;

  case 598: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3300 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10236 "bison-chpl-lib.cpp"
    break;

  case 599: /* new_expr: new_maybe_decorated expr  */
#line 3306 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10244 "bison-chpl-lib.cpp"
    break;

  case 600: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3313 "chpl.ypp"
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
#line 10260 "bison-chpl-lib.cpp"
    break;

  case 601: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3325 "chpl.ypp"
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
#line 10276 "bison-chpl-lib.cpp"
    break;

  case 602: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3337 "chpl.ypp"
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
#line 10293 "bison-chpl-lib.cpp"
    break;

  case 603: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3350 "chpl.ypp"
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
#line 10311 "bison-chpl-lib.cpp"
    break;

  case 604: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3367 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10319 "bison-chpl-lib.cpp"
    break;

  case 605: /* range_literal_expr: expr TDOTDOT expr  */
#line 3374 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10328 "bison-chpl-lib.cpp"
    break;

  case 606: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3379 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10337 "bison-chpl-lib.cpp"
    break;

  case 607: /* range_literal_expr: expr TDOTDOT  */
#line 3384 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10346 "bison-chpl-lib.cpp"
    break;

  case 608: /* range_literal_expr: TDOTDOT expr  */
#line 3389 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10356 "bison-chpl-lib.cpp"
    break;

  case 609: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3395 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10366 "bison-chpl-lib.cpp"
    break;

  case 610: /* range_literal_expr: TDOTDOT  */
#line 3401 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10376 "bison-chpl-lib.cpp"
    break;

  case 611: /* cast_expr: expr TCOLON expr  */
#line 3431 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10384 "bison-chpl-lib.cpp"
    break;

  case 612: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3438 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10392 "bison-chpl-lib.cpp"
    break;

  case 613: /* super_expr: fn_expr  */
#line 3444 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10398 "bison-chpl-lib.cpp"
    break;

  case 617: /* expr: sub_type_level_expr TQUESTION  */
#line 3453 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10404 "bison-chpl-lib.cpp"
    break;

  case 618: /* expr: TQUESTION  */
#line 3455 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10410 "bison-chpl-lib.cpp"
    break;

  case 622: /* expr: fn_type  */
#line 3460 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10416 "bison-chpl-lib.cpp"
    break;

  case 632: /* opt_expr: %empty  */
#line 3474 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10422 "bison-chpl-lib.cpp"
    break;

  case 633: /* opt_expr: expr  */
#line 3475 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10428 "bison-chpl-lib.cpp"
    break;

  case 634: /* opt_try_expr: TTRY expr  */
#line 3479 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10434 "bison-chpl-lib.cpp"
    break;

  case 635: /* opt_try_expr: TTRYBANG expr  */
#line 3480 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10440 "bison-chpl-lib.cpp"
    break;

  case 636: /* opt_try_expr: super_expr  */
#line 3481 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10446 "bison-chpl-lib.cpp"
    break;

  case 642: /* call_base_expr: expr TBANG  */
#line 3498 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10454 "bison-chpl-lib.cpp"
    break;

  case 643: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3501 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10460 "bison-chpl-lib.cpp"
    break;

  case 646: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3508 "chpl.ypp"
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
#line 10476 "bison-chpl-lib.cpp"
    break;

  case 647: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3520 "chpl.ypp"
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
#line 10492 "bison-chpl-lib.cpp"
    break;

  case 648: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3532 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10500 "bison-chpl-lib.cpp"
    break;

  case 649: /* dot_expr: expr TDOT ident_use  */
#line 3539 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10506 "bison-chpl-lib.cpp"
    break;

  case 650: /* dot_expr: expr TDOT TTYPE  */
#line 3541 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10512 "bison-chpl-lib.cpp"
    break;

  case 651: /* dot_expr: expr TDOT TDOMAIN  */
#line 3543 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10518 "bison-chpl-lib.cpp"
    break;

  case 652: /* dot_expr: expr TDOT TLOCALE  */
#line 3545 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10524 "bison-chpl-lib.cpp"
    break;

  case 653: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3547 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10534 "bison-chpl-lib.cpp"
    break;

  case 654: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3553 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10544 "bison-chpl-lib.cpp"
    break;

  case 655: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3565 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10550 "bison-chpl-lib.cpp"
    break;

  case 656: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3567 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10558 "bison-chpl-lib.cpp"
    break;

  case 657: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3571 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10566 "bison-chpl-lib.cpp"
    break;

  case 658: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3575 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10574 "bison-chpl-lib.cpp"
    break;

  case 659: /* bool_literal: TFALSE  */
#line 3581 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10580 "bison-chpl-lib.cpp"
    break;

  case 660: /* bool_literal: TTRUE  */
#line 3582 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10586 "bison-chpl-lib.cpp"
    break;

  case 661: /* str_bytes_literal: STRINGLITERAL  */
#line 3586 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10592 "bison-chpl-lib.cpp"
    break;

  case 662: /* str_bytes_literal: BYTESLITERAL  */
#line 3587 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10598 "bison-chpl-lib.cpp"
    break;

  case 665: /* literal_expr: INTLITERAL  */
#line 3593 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10604 "bison-chpl-lib.cpp"
    break;

  case 666: /* literal_expr: REALLITERAL  */
#line 3594 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10610 "bison-chpl-lib.cpp"
    break;

  case 667: /* literal_expr: IMAGLITERAL  */
#line 3595 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10616 "bison-chpl-lib.cpp"
    break;

  case 668: /* literal_expr: CSTRINGLITERAL  */
#line 3596 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10622 "bison-chpl-lib.cpp"
    break;

  case 669: /* literal_expr: TNONE  */
#line 3597 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10628 "bison-chpl-lib.cpp"
    break;

  case 670: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3599 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10637 "bison-chpl-lib.cpp"
    break;

  case 671: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3604 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10646 "bison-chpl-lib.cpp"
    break;

  case 672: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3609 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10654 "bison-chpl-lib.cpp"
    break;

  case 673: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3613 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
  }
#line 10662 "bison-chpl-lib.cpp"
    break;

  case 674: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3617 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
  }
#line 10670 "bison-chpl-lib.cpp"
    break;

  case 675: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3621 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
  }
#line 10678 "bison-chpl-lib.cpp"
    break;

  case 676: /* assoc_expr_ls: expr TALIAS expr  */
#line 3629 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10687 "bison-chpl-lib.cpp"
    break;

  case 677: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3634 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10697 "bison-chpl-lib.cpp"
    break;

  case 678: /* binary_op_expr: expr TPLUS expr  */
#line 3642 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10703 "bison-chpl-lib.cpp"
    break;

  case 679: /* binary_op_expr: expr TMINUS expr  */
#line 3643 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10709 "bison-chpl-lib.cpp"
    break;

  case 680: /* binary_op_expr: expr TSTAR expr  */
#line 3644 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10715 "bison-chpl-lib.cpp"
    break;

  case 681: /* binary_op_expr: expr TDIVIDE expr  */
#line 3645 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10721 "bison-chpl-lib.cpp"
    break;

  case 682: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3646 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10727 "bison-chpl-lib.cpp"
    break;

  case 683: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3647 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10733 "bison-chpl-lib.cpp"
    break;

  case 684: /* binary_op_expr: expr TMOD expr  */
#line 3648 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10739 "bison-chpl-lib.cpp"
    break;

  case 685: /* binary_op_expr: expr TEQUAL expr  */
#line 3649 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10745 "bison-chpl-lib.cpp"
    break;

  case 686: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3650 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10751 "bison-chpl-lib.cpp"
    break;

  case 687: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3651 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10757 "bison-chpl-lib.cpp"
    break;

  case 688: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3652 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10763 "bison-chpl-lib.cpp"
    break;

  case 689: /* binary_op_expr: expr TLESS expr  */
#line 3653 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10769 "bison-chpl-lib.cpp"
    break;

  case 690: /* binary_op_expr: expr TGREATER expr  */
#line 3654 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10775 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TBAND expr  */
#line 3655 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10781 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TBOR expr  */
#line 3656 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10787 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TBXOR expr  */
#line 3657 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10793 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TAND expr  */
#line 3658 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10799 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TOR expr  */
#line 3659 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10805 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TEXP expr  */
#line 3660 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10811 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TBY expr  */
#line 3661 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10817 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TALIGN expr  */
#line 3662 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10823 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr THASH expr  */
#line 3663 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10829 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TDMAPPED expr  */
#line 3664 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10835 "bison-chpl-lib.cpp"
    break;

  case 701: /* unary_op_expr: TPLUS expr  */
#line 3668 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10841 "bison-chpl-lib.cpp"
    break;

  case 702: /* unary_op_expr: TMINUS expr  */
#line 3669 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10847 "bison-chpl-lib.cpp"
    break;

  case 703: /* unary_op_expr: TMINUSMINUS expr  */
#line 3670 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10853 "bison-chpl-lib.cpp"
    break;

  case 704: /* unary_op_expr: TPLUSPLUS expr  */
#line 3671 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10859 "bison-chpl-lib.cpp"
    break;

  case 705: /* unary_op_expr: TBANG expr  */
#line 3672 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10865 "bison-chpl-lib.cpp"
    break;

  case 706: /* unary_op_expr: expr TBANG  */
#line 3673 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10873 "bison-chpl-lib.cpp"
    break;

  case 707: /* unary_op_expr: TBNOT expr  */
#line 3676 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10879 "bison-chpl-lib.cpp"
    break;

  case 708: /* reduce_expr: expr TREDUCE expr  */
#line 3681 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10887 "bison-chpl-lib.cpp"
    break;

  case 709: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3685 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10895 "bison-chpl-lib.cpp"
    break;

  case 710: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3689 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10903 "bison-chpl-lib.cpp"
    break;

  case 711: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3693 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10911 "bison-chpl-lib.cpp"
    break;

  case 712: /* scan_expr: expr TSCAN expr  */
#line 3700 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10919 "bison-chpl-lib.cpp"
    break;

  case 713: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3704 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10927 "bison-chpl-lib.cpp"
    break;

  case 714: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3708 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10935 "bison-chpl-lib.cpp"
    break;

  case 715: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3712 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10943 "bison-chpl-lib.cpp"
    break;


#line 10947 "bison-chpl-lib.cpp"

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
