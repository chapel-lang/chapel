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
#define YYLAST   19054

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  198
/* YYNRULES -- Number of rules.  */
#define YYNRULES  723
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1294

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
    2636,  2637,  2638,  2639,  2643,  2644,  2647,  2648,  2652,  2653,
    2654,  2658,  2663,  2664,  2666,  2668,  2670,  2675,  2677,  2682,
    2684,  2686,  2688,  2690,  2692,  2694,  2699,  2700,  2704,  2713,
    2717,  2725,  2729,  2736,  2757,  2758,  2763,  2764,  2765,  2766,
    2767,  2772,  2771,  2780,  2788,  2792,  2799,  2817,  2836,  2840,
    2844,  2851,  2853,  2855,  2862,  2863,  2864,  2868,  2872,  2873,
    2874,  2878,  2879,  2883,  2884,  2888,  2889,  2890,  2891,  2895,
    2896,  2900,  2904,  2905,  2911,  2912,  2916,  2917,  2921,  2925,
    2932,  2933,  2937,  2942,  2951,  2952,  2956,  2957,  2964,  2965,
    2966,  2967,  2968,  2969,  2971,  2973,  2975,  2977,  2983,  2985,
    2988,  2990,  2992,  2994,  2996,  2998,  3000,  3002,  3004,  3006,
    3011,  3022,  3033,  3043,  3059,  3075,  3091,  3100,  3109,  3117,
    3131,  3145,  3162,  3166,  3170,  3174,  3178,  3182,  3186,  3193,
    3211,  3219,  3220,  3221,  3222,  3223,  3224,  3225,  3229,  3230,
    3234,  3243,  3244,  3248,  3257,  3258,  3262,  3276,  3280,  3287,
    3288,  3289,  3290,  3291,  3292,  3296,  3298,  3300,  3302,  3304,
    3310,  3317,  3329,  3341,  3354,  3371,  3378,  3383,  3388,  3393,
    3399,  3405,  3435,  3442,  3449,  3450,  3454,  3456,  3457,  3459,
    3461,  3462,  3463,  3464,  3467,  3468,  3469,  3470,  3471,  3472,
    3473,  3474,  3475,  3479,  3480,  3484,  3485,  3486,  3490,  3491,
    3492,  3493,  3502,  3503,  3506,  3507,  3508,  3512,  3524,  3536,
    3543,  3545,  3547,  3549,  3551,  3557,  3570,  3571,  3575,  3579,
    3586,  3587,  3591,  3592,  3596,  3597,  3598,  3599,  3600,  3601,
    3602,  3603,  3608,  3613,  3617,  3621,  3625,  3633,  3638,  3647,
    3648,  3649,  3650,  3651,  3652,  3653,  3654,  3655,  3656,  3657,
    3658,  3659,  3660,  3661,  3662,  3663,  3664,  3665,  3666,  3667,
    3668,  3669,  3673,  3674,  3675,  3676,  3677,  3678,  3681,  3685,
    3689,  3693,  3697,  3704,  3708,  3712,  3716,  3724,  3725,  3726,
    3727,  3728,  3729,  3730
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

#define YYPACT_NINF (-1084)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-724)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1084,   155,  3939, -1084,   106,   225, -1084, -1084, -1084, -1084,
   -1084, -1084, -1084, 11289,   165,   297,   265, 13690,   274, 18808,
     165, 11289,   301,   243,   271,   297,  5339, 11289,  5339,   227,
   18863, -1084,   379,   341,  8839,  9714,  9714, -1084,  9014,   380,
      38,   291, -1084,   437, 18863, 18863, 18863, -1084,  3488,  9889,
     446, 11289, 11289,   149, -1084,   461,   505, 11289, -1084, 13690,
   -1084, 11289,   397,   313,   249,   288, 13038,   525, 18938, -1084,
   11289,  8314, 11289,  9889, 13690, 11289,   424,   535,   411,  5339,
     573, 11289,   576,  7264,  7264, -1084,   579, -1084, 13690, -1084,
     582,  9014,  9189, -1084,   530, 11289, -1084, 11289, -1084, -1084,
   13518, 11289, -1084, 11289, -1084, -1084, -1084,  4289,  7614,  9364,
   11289, -1084,  4989, -1084, -1084, -1084, -1084,  3124,   500, -1084,
      56,   479, -1084,    31, -1084, -1084, -1084, -1084, -1084, -1084,
   -1084, -1084, -1084, -1084, -1084, -1084, -1084,  7439, -1084, 10064,
   -1084, -1084, -1084, -1084, -1084,   586, -1084, -1084, -1084, -1084,
   -1084, -1084, -1084, -1084,   504, 18863, -1084, 18548,   481,   226,
   -1084,   203, -1084, -1084, -1084,   257,    49, -1084, -1084, 18863,
    3488, -1084,   491,   495, -1084, -1084, -1084,   499,   506, 11289,
     507,   508, -1084, -1084, -1084, 18067,  1879,   -65,   514,   519,
   -1084, -1084,   232, -1084, -1084, -1084, -1084, -1084,   188, -1084,
   -1084, -1084, 11289, 11289, 11289, 18863, -1084, 11289, 11289, 10239,
     610,   298, -1084, -1084, -1084, -1084, 18067,   321, -1084, -1084,
     517,  5339, -1084, -1084,   521,    21,   520, 15453, -1084,   247,
   -1084,   526, -1084,   -25, 18067,   575,  9189, -1084, -1084, -1084,
   -1084, -1084, -1084, -1084, -1084, -1084, -1084, -1084, -1084, -1084,
   -1084, -1084, -1084, -1084, -1084, -1084, -1084, -1084, -1084, -1084,
   -1084, -1084,   529, -1084, -1084, 18863,   533,    66,  3210,    79,
    3548,    79, 14885, 18863, 18863,   -17, 14145, -1084, -1084,   614,
    9189,   537,   330,  5339,  2719,    88,    34, -1084,  5339, -1084,
   -1084, 15529, -1084,    41, 16159,   528, -1084,   540,   541, -1084,
   15529,    21,   528, -1084,  9189,  1756,  1756,    89,    78, -1084,
      82, -1084, -1084, -1084, -1084, 11289, 11289, -1084,   558, -1084,
     527,   567, -1084, -1084,  2982,   572, -1084, -1084, 18067,   248,
   11289, 11289, -1084,   -32, -1084, -1084, 18067,   554, 16239, -1084,
   15529,    21, 18067,   548,  9189, -1084, 18067, 16279, -1084, -1084,
   -1084, -1084,    21,    66, 15529,   556, -1084,    23,    23,  1333,
     528,   528,    95, -1084, -1084,  4464,   111, -1084, 11289,   100,
     169, -1084, 13833,   -28,   131, 16325,   108,  1333,   715, -1084,
   -1084, 18863, -1084,  8839,  9714, -1084,  4289,  9714, -1084, -1084,
     614, -1084, -1084, -1084, -1084,   721,   559,  4639, 11289, -1084,
   11289,   661, -1084, -1084, 15053,   224,   504, 18067,   469, 18863,
   11289,   563, -1084,   561, 18603,   609,   172, -1084,   150,   257,
   -1084, -1084, -1084, -1084,   645, -1084, -1084, -1084, -1084,    68,
     646, -1084, -1084, -1084, 13348,   612,   263, -1084,   350,   444,
   -1084,   589, 11289, 11289, 11289,  9714,  9714, 11289,   471, 11289,
   11289, 11289, 11289, 11289,   478, 13518, 11289, 11289, 11289, 11289,
   11289, 11289, 11289, 11289, 11289, 11289, 11289, 11289, 11289, 11289,
   11289, 11289,   672, -1084, -1084, -1084, -1084, -1084,  9189,  9189,
   -1084, -1084, -1084, -1084,  9189, -1084, -1084,  9189,  9189,  9189,
    9189,  9714,  9714, 15605, 15681, 16431,   569,     9, 18863,  7789,
   -1084, -1084, -1084,  9714,    66,  3488, -1084, 11289, -1084, 11289,
     617,   566,   596, -1084, -1084, -1084,   708, 11289, -1084,  5514,
    9714, -1084,   584, -1084,    66,  5689,  9714, -1084,    66, -1084,
      66,  9714, -1084,    66,   633,   634,  5339,   727,  5339, -1084,
     731, 11289,   701,   594,  9189, 18863, -1084, -1084, -1084,  2719,
   -1084, -1084,   192, -1084, 11464,   643, 11289,  3488, -1084, -1084,
   11289, -1084, 18678, 11289, 11289, -1084,   598, -1084, -1084, -1084,
   -1084, 18733, -1084,   257, 13212, -1084,  2719, -1084,   623, 11464,
   -1084, 18067, 18067, -1084,   520, -1084,    51, -1084,  9189,   601,
   -1084,   755,   755, -1084, -1084, -1084, -1084, -1084, -1084, 10414,
   -1084, 16477,  7964, -1084,  8139, -1084, 18067,  5339,   602,  9714,
   10589,  4114,   613, 11289, 10764, -1084, -1084, 15133, 14961,   174,
     635,  8489, -1084,    99, -1084,  4814, -1084,   408, 16517,   423,
   15225, 18863,  7089,  7089, -1084,   504,   615, -1084,   247, -1084,
     -55,   642,  1554, -1084, -1084, 11289,   210, -1084, -1084,   194,
   -1084, -1084,    69, -1084, -1084,  3758, -1084,   687, -1084,   621,
     647, -1084,   649, -1084,   655,   656,   658,   659, -1084,   665,
     669,   673,   677,   679,   680,   681, -1084, -1084, -1084, -1084,
   -1084, -1084, -1084, -1084, -1084, -1084, -1084, -1084, -1084, -1084,
   -1084, -1084, -1084, -1084, -1084, -1084, -1084, -1084, 11289,   653,
     684,   621, -1084,   621, -1084, 11289, -1084, 18863, -1084, -1084,
   12909, 12909,   534, -1084,   534, -1084,   534, 16119,  1044,   671,
     884,    21,    23, -1084,   480, -1084, -1084, -1084, -1084, -1084,
    1333, 18301,   534,   852,   852, 12909,   852,   852,   562,    23,
   18301, 13083,   562,   528,   528,    23,  1333,   630,   670,   675,
     676,   678,   682,   637,   667, -1084,   534, -1084,   534, 11639,
    9714, 11814,  9714, 11289,  9189,  9714, 13833,   664,   218, -1084,
   -1084, -1084,   213, -1084,  1685, 18112,   375,   165, 16593, -1084,
     429, 18067, 16669,  9189, -1084,  9189, 11289,   236,  9014, 18067,
      72, 15757,  7789, -1084,  9014, 18067,    87, 15301, -1084, -1084,
      79, 15377, -1084, 11289, 11289,   811,  5339,   812, 16752,  5339,
   15833, 18863, -1084,   241, -1084,   245, -1084,   246,  2719,    88,
   -1084, 18067,  8664, -1084, 18067, -1084, -1084, -1084, 18863, 16828,
   16868, -1084,   623,   703, -1084,   294, 11464, -1084,   337, -1084,
   11289, -1084,   685,   187,   688, -1084,  3401, -1084, -1084, -1084,
   -1084, -1084, -1084, -1084, -1084,  5339,   -12, 15909, -1084, -1084,
   18067,  5339, 18067, -1084, 16914,  9714,  9714,  9714,  5339, -1084,
   -1084,   326, -1084, -1084, -1084, -1084, 11289, -1084,    80, 13905,
   11289, -1084, 10939,  7089,  7089, -1084, 11289,   391, 11289,   398,
     443, 11289,  9539,   510,   430, -1084, -1084, -1084, 17020,   704,
     690,   692, -1084, 18863,  3488, -1084,  9189,   689,  2501, 18067,
   18863,   500,   588, -1084,   687, -1084, -1084, -1084, -1084, -1084,
      33,   221, -1084, 17066, -1084, 14064, -1084, -1084, 18067, -1084,
     693,   702, -1084, -1084, -1084, -1084, -1084, -1084, -1084, -1084,
   11289,   818, 17106, 11289,   833, 17258,   336,   706, 17182,  9189,
   -1084, -1084,  7789, -1084, -1084, -1084, -1084, -1084, -1084, -1084,
   -1084, -1084, -1084, -1084, -1084,    46,  9714,  9714,    66,    66,
   -1084, -1084, -1084, -1084, 15529, -1084, 14225,  5864, -1084,  6039,
   -1084,   361, -1084, 14305,  6214, -1084,    66,  6389, -1084,    66,
   -1084,    66, -1084,    66, 18067, 18067,  5339, -1084,  5339, 11289,
   -1084,  5339,   840,   711,   717, 18863,   520, -1084, -1084,   643,
   -1084, -1084, -1084,   712,   716,   337, 11289,   687,   623, -1084,
   11289, -1084, -1084, 18067, -1084,  6564, 11289, -1084, -1084, -1084,
   18863, -1084,   741,   520, -1084,  6739,   714,  6914,   719, -1084,
   11289,    66, 15529,    79, 15377,   132, 15985,  9189, -1084, -1084,
   -1084,  7089, -1084, 17298,   113, 16077,   445,   725,  7089, -1084,
   11289, -1084, -1084, 18255,   457,   459,   390, -1084, -1084, -1084,
   -1084, 18444, 18462, -1084, -1084, 18603, -1084,    33,    62, 11289,
   -1084,   744,   722, -1084,   621,   621, -1084, -1084, 17343, 11989,
   12164, 17450, 12339, 12514, -1084, 12689, 12864,   399, -1084,   643,
     401,   409, -1084, -1084, -1084,  5339,  9014, 18067,  9014, 18067,
    7789, -1084,  5339,  9014, 18067, -1084,  9014, 18067, -1084, -1084,
   -1084, -1084, -1084, 18067,   858,  5339, -1084, -1084, -1084, -1084,
   -1084,  9189,  9189, -1084, -1084, -1084, -1084,   337, 18067,  5339,
   -1084,    71,   723, 11289, -1084,  9014, -1084, 18067,  5339,  9014,
   -1084, 18067,  5339, 18067,  5339,  5339, -1084,   230, 11114,  7089,
    7089,  7089,  7089, -1084, -1084, -1084, 17495, 18067, 18273, -1084,
   -1084, -1084,   732, 18462, -1084, -1084, -1084, -1084, -1084,   141,
   -1084,    -4, -1084,   410, 17541,    67, -1084, -1084, -1084, 11289,
   11289, 11289, 11289, 11289, 11289, 11289, 11289, -1084, -1084, 16752,
   14385, 14465, -1084, 16752, 14545, 14625,  5339, -1084,   726,   728,
   -1084, -1084,   575, -1084,   520, 18067, 14705, -1084, 14785, -1084,
   -1084, -1084, 18067,   460,   733,   465,   735, 11289, -1084, -1084,
   -1084,    54, -1084, 11289,    62,   141,   141,   141,   141,   141,
     141,    62,  5164, -1084, -1084, -1084, -1084, -1084, 17647, 17723,
   17799, 17875, 17951, 18027,  5339,  5339,  5339,  5339, -1084,   738,
     740, -1084,  5339,  5339, -1084, -1084, -1084, -1084, 18067, -1084,
   18067, -1084, -1084, -1084, -1084, -1084, -1084, -1084,   758, -1084,
   11289, 11289, 11289, 11289, 11289, 11289, 16752, 16752, 16752, 16752,
   -1084, -1084, 16752, 16752
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   124,   451,   666,   667,   668,
     662,   663,   669,     0,   578,   115,   145,   546,   152,   548,
     578,     0,   151,   481,   479,   115,     0,     0,     0,   266,
     146,   184,   183,   660,     0,     0,     0,   182,     0,   150,
      68,   267,   329,   147,     0,     0,     0,   324,     0,     0,
     154,     0,     0,   595,   570,   670,   155,     0,   330,   540,
     476,     0,     0,     0,   169,   167,   417,   149,   549,   478,
       0,     0,     0,     0,   544,     0,     0,   153,     0,     0,
     125,     0,   661,     0,     0,   469,   148,   304,   542,   480,
     156,     0,     0,   719,     0,     0,   721,     0,   722,   723,
     611,     0,   720,   717,   619,   165,   718,     0,     0,     0,
       0,     4,     0,     5,    10,    11,    12,   327,    50,    51,
      57,     0,    45,    71,    13,    77,    14,    15,    16,    17,
      29,   526,   527,    23,    46,   166,   176,     0,   185,   633,
     177,    18,    31,    30,    20,     0,   261,    19,   629,    22,
      35,    32,    33,   175,   305,     0,   173,     0,     0,   626,
     328,     0,   623,   171,   345,   437,   428,   621,   174,     0,
       0,   172,   638,   617,   616,   620,   530,   528,     0,     0,
     627,   628,   632,   631,   630,     0,   529,     0,   639,   640,
     641,   664,   665,   622,   532,   531,   624,   625,     0,    28,
     548,   146,     0,     0,     0,     0,   549,     0,     0,     0,
       0,   626,   638,   528,   627,   628,   538,   529,   639,   640,
       0,     0,   579,   116,     0,   547,     0,   578,   470,     0,
     477,     0,    21,     0,   514,     0,   520,   122,   126,   138,
     132,   131,   140,   119,   130,   141,   127,   142,   117,   143,
     136,   129,   137,   135,   133,   134,   118,   120,   128,   139,
     144,   123,     0,   121,   193,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    70,    69,    71,
     520,     0,     0,     0,     0,     0,     0,   484,     0,   162,
      40,     0,   296,     0,   295,   704,   599,   596,   597,   598,
       0,   541,   705,     7,   520,   327,   327,   424,   159,   419,
     160,   420,   421,   425,   426,   158,   157,   427,   417,   510,
       0,   332,   333,   335,     0,   418,   509,   337,   497,     0,
       0,     0,   163,   623,   614,   637,   615,     0,     0,    43,
       0,   545,   533,     0,   520,    44,   539,     0,   274,   278,
     275,   278,   543,     0,     0,     0,    56,   706,   708,   609,
     703,   702,     0,    74,    78,     0,     0,   516,     0,     0,
       0,   517,   562,     0,     0,   514,     0,   610,     0,     6,
     302,     0,   183,     0,     0,   303,     0,     0,    48,     9,
      71,    49,    47,    52,    55,     0,    54,     0,     0,    72,
       0,     0,   481,   181,     0,   662,   305,   634,   189,     0,
       0,     0,   301,     0,     0,   322,     0,   315,   417,   437,
     435,   436,   434,   353,   439,   438,   442,   441,   443,     0,
     432,   429,   430,   433,     0,   474,     0,   471,     0,   618,
      34,   600,     0,     0,     0,     0,     0,     0,   707,     0,
       0,     0,     0,     0,     0,   608,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   384,   391,   392,   393,   388,   390,     0,     0,
     386,   389,   387,   385,     0,   395,   394,     0,     0,   520,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      36,    24,    37,     0,     0,     0,    25,     0,    38,     0,
     526,     0,   521,   522,   525,   314,     0,     0,   203,     0,
       0,   202,     0,   211,     0,     0,     0,   209,     0,   219,
       0,     0,   217,     0,     0,     0,     0,   235,     0,   384,
     231,     0,     0,     0,     0,     0,   245,    26,   488,     0,
     489,   491,     0,   508,     0,   494,     0,     0,   161,    39,
       0,   298,     0,     0,     0,    41,     0,   170,   168,   422,
     423,     0,   418,   437,   417,   410,     0,   409,   512,     0,
     108,   635,   636,   343,     0,   164,     0,    42,     0,     0,
     285,   276,   277,   196,   195,    27,    76,    75,    79,     0,
     671,     0,     0,   656,     0,   658,   563,     0,     0,     0,
       0,     0,     0,     0,     0,   675,     8,     0,     0,     0,
      57,     0,    53,     0,    65,     0,   106,     0,   102,     0,
      86,     0,     0,    92,   178,   305,     0,   191,     0,   188,
     272,   306,     0,   313,   319,     0,   317,   312,   402,     0,
     404,   408,     0,   440,   502,     0,   504,   444,   431,   400,
     122,   380,   126,   378,   132,   131,   130,   127,   382,   142,
     143,   129,   133,   118,   128,   144,   377,   359,   362,   360,
     361,   383,   372,   363,   376,   368,   366,   379,   367,   365,
     370,   375,   364,   369,   373,   374,   371,   381,     0,   356,
       0,   400,   357,   400,   354,     0,   473,     0,   468,   483,
     699,   698,   701,   710,   709,   714,   713,   695,   692,   693,
     694,   612,   682,   124,     0,   652,   653,   125,   651,   650,
     606,   686,   697,   691,   689,   700,   690,   688,   680,   685,
     687,   696,   679,   683,   684,   681,   607,     0,     0,     0,
       0,     0,     0,     0,     0,   712,   711,   716,   715,     0,
       0,     0,     0,     0,     0,     0,   673,   272,   589,   590,
     592,   594,     0,   581,     0,     0,     0,   578,   578,   199,
       0,   515,     0,     0,   535,     0,     0,     0,     0,   552,
       0,     0,     0,   212,     0,   558,     0,     0,   210,   220,
       0,     0,   218,     0,     0,   234,     0,   230,     0,     0,
       0,     0,   534,     0,   248,     0,   246,     0,   492,     0,
     507,   506,     0,   486,   605,   485,   297,   294,     0,     0,
       0,   649,   512,   338,   334,     0,     0,   513,   494,   336,
       0,   342,     0,     0,     0,   536,     0,   279,   672,   613,
     657,   518,   659,   519,   227,     0,     0,     0,   674,   225,
     564,     0,   677,   676,     0,     0,     0,     0,     0,    58,
      59,     0,    61,    63,    67,    66,     0,   101,     0,     0,
       0,    94,     0,     0,    92,    64,   377,   359,   362,   360,
     361,   370,   369,   371,     0,   396,   397,    81,    80,    93,
       0,     0,   307,     0,     0,   269,     0,     0,   327,   323,
       0,     0,   417,   403,   444,   500,   499,   498,   501,   445,
     452,   417,   352,     0,   358,     0,   348,   349,   475,   472,
       0,     0,   114,   112,   113,   111,   110,   109,   647,   648,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     591,   593,     0,   580,   145,   152,   151,   150,   147,   154,
     155,   149,   153,   148,   156,     0,     0,     0,     0,     0,
     482,   194,   524,   523,     0,   229,     0,     0,   201,     0,
     200,     0,   584,     0,     0,   207,     0,     0,   205,     0,
     215,     0,   213,     0,   243,   242,     0,   237,     0,     0,
     233,     0,   239,     0,   271,     0,     0,   490,   493,   494,
     495,   496,   293,     0,     0,   494,     0,   444,   512,   511,
     398,   413,   411,   344,   287,     0,     0,   286,   289,   537,
       0,   280,   283,     0,   228,     0,     0,     0,     0,   226,
       0,     0,     0,     0,     0,     0,     0,     0,    60,   107,
     104,     0,   103,    89,    88,    87,     0,     0,     0,   179,
       0,   180,   307,   327,     0,     0,     0,   300,   169,   167,
     299,   327,   327,   308,   318,     0,   405,   452,     0,     0,
     346,   407,     0,   355,   400,   400,   654,   655,     0,     0,
       0,     0,     0,     0,   271,     0,     0,     0,   582,   494,
     638,   638,   198,   197,   204,     0,     0,   551,     0,   550,
       0,   583,     0,     0,   557,   208,     0,   556,   206,   216,
     214,   236,   232,   569,   238,     0,    73,   268,   247,   244,
     487,   520,   520,   414,   412,   339,   340,   494,   399,     0,
     291,     0,     0,     0,   281,     0,   223,   566,     0,     0,
     221,   565,     0,   678,     0,     0,    62,     0,     0,     0,
      92,     0,    92,    95,    98,    84,    83,    82,   327,   186,
     192,   190,   273,   327,   310,   309,   321,   325,   466,     0,
     467,   454,   457,     0,   453,     0,   401,   350,   351,     0,
       0,     0,     0,     0,     0,     0,     0,   273,   586,   555,
       0,     0,   585,   561,     0,     0,     0,   241,     0,     0,
     416,   415,   292,   290,     0,   284,     0,   224,     0,   222,
     105,    91,    90,     0,     0,     0,     0,     0,   187,   270,
     311,     0,   465,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   446,   448,   450,   347,   447,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   240,   601,
     602,   282,     0,     0,    97,   100,    96,    99,    85,   326,
     456,   458,   459,   462,   463,   464,   460,   461,   455,   449,
       0,     0,     0,     0,     0,     0,   554,   553,   560,   559,
     603,   604,   568,   567
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1084, -1084, -1084,     4,   -64,  2407, -1084,   328,   789, -1084,
   -1084,  -154,  -117, -1084,   290, -1084, -1084,  -139, -1084, -1084,
      40,   632, -1084,  -574,  2693,   515,  -590, -1084,  -847, -1084,
   -1084,    37, -1084, -1084, -1084,   889, -1084,  2911,  -162, -1084,
   -1084,  -503,  2149, -1083,  -863, -1084, -1084,   233, -1084, -1084,
   -1084, -1084, -1084, -1084, -1084, -1084,   799,  1266, -1084,   827,
   -1084, -1084,   -86,  2076, -1084, -1084, -1084, -1084,   571, -1084,
    -107, -1084, -1084, -1084, -1084,   364, -1084, -1084, -1084,  -126,
   -1084,  -366,  -852, -1084, -1084, -1084, -1084, -1084,  -405, -1084,
     296, -1084, -1084, -1084, -1084, -1084,   351, -1084,  1399, -1084,
   -1084, -1084, -1084,   518, -1084, -1084, -1084, -1084,     3,  -422,
     -45,  -838,  -894,  -669, -1084,     8, -1084,    20,   362,   -51,
     616,   -47, -1084, -1084,  -370,  -842, -1084,  -296, -1084,  -140,
    -301,  -292,  -815, -1084, -1084,    43,   237, -1084,  -211,   806,
   -1084,  -159,   392,   129,  -444,  -811,  -638, -1084, -1084, -1084,
    -617,  -546, -1084,  -789,    -3,    58, -1084,  -273,  -489,  -558,
     261, -1084, -1084, -1084, -1084,   699, -1084,   -15, -1084, -1084,
    -243, -1084,  -772, -1084, -1084,  1060,  1124, -1084, -1084, -1084,
   -1084,   -13, -1084,    42,  1312, -1084,  1545,  1865, -1084, -1084,
   -1084, -1084, -1084, -1084, -1084, -1084, -1084,  -483
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   364,   112,   807,   114,   115,   116,   389,
     117,   118,   119,   120,   396,   622,   871,   872,   121,   122,
     123,   401,   124,   125,   126,   365,   899,   629,   900,   127,
     128,   626,   627,   129,   130,   224,   261,   285,   131,   263,
     132,   319,   521,   133,   134,   306,   305,   135,   136,   137,
     138,   139,   140,   638,   903,   141,   142,   267,   143,   275,
     144,   815,   816,   210,   146,   147,   148,   149,   591,   847,
    1033,   150,   151,   843,  1028,   292,   293,   152,   153,   154,
     155,   411,   908,   156,   157,   416,   910,   911,   417,   158,
     211,  1231,   160,   161,   320,   321,   322,  1017,   162,   334,
     584,   840,   163,   164,  1185,   165,   166,   700,   701,   895,
     896,   897,  1021,   922,   419,   649,  1082,   650,   578,   651,
     324,   572,   434,   423,   429,   920,  1246,  1247,   167,  1080,
    1181,  1182,  1183,   168,   169,   436,   437,   706,   170,   171,
     229,   286,   287,   551,   552,   823,   326,   918,   656,   657,
     555,   327,   837,   838,   366,   369,   370,   511,   512,   513,
     212,   173,   174,   175,   176,   213,   178,   221,   222,   772,
     528,   981,   773,   774,   179,   214,   215,   182,   183,   184,
     335,   185,   408,   514,   217,   187,   218,   219,   190,   191,
     192,   193,   376,   194,   195,   196,   197,   198
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     216,   393,   318,   414,   225,   226,   111,   543,   227,   644,
     841,   438,   702,   406,   234,   323,   776,   917,   505,   325,
     982,   268,   270,   272,   233,   276,   524,  1022,   530,   533,
     844,   566,   926,   839,   927,   914,   291,  1057,   294,   295,
     636,  1052,   894,  1015,   300,  1073,   301,   553,   302,   652,
    1063,   820,   842,   328,   444,   813,   765,   234,   336,   338,
     340,   341,   342,   873,   394,  1178,   346,   329,   347,   654,
     553,   589,  1077,   288,   510,   352,   430,   398,   354,   336,
     279,   556,   357,   723,   358,   536,  1242,   359,   360,   553,
     361,   583,   522,  1078,   234,   336,   375,   377,   288,  1242,
    -503,  -505,  1245,   288,   977,   817,   374,   489,   522,   490,
     905,   288,   445,   337,  1233,   399,   446,   906,   510,   984,
     277,  1133,   507,   278,   404,  -288,   407,   431,  -503,  -505,
     608,   612,   835,  -266,   355,  -505,   569,  -267,  -341,   432,
     508,   488,   510,  1234,  1178,   400,    71,   607,  1245,  1159,
     371,  1079,   916,   386,  1179,     3,   610,   390,   433,    71,
    -503,  -505,   448,  1035,   448,  1180,   441,   296,  -288,   452,
     454,  -505,   454,  -503,  -505,  1136,   457,   307,   609,   570,
    1098,   557,   510,   727,   766,  1160,  -503,  -505,   560,   493,
     494,   495,   554,  -505,   346,   234,   375,   309,  1130,   522,
    1073,   311,  1009,   833,  1134,   395,   497,   522,  1174,  1175,
    1168,   386,   504,  -505,   655,   554,   753,   754,   507,  -505,
    1165,   867,  -288,   336,   386,   312,   297,   973,   313,  1137,
      62,   541,  1243,  -503,   554,  -505,   386,   386,  -503,  -505,
     314,   386,   386,  1210,  1180,   298,   380,   602,   307,   386,
     236,   522,   522,  -505,   280,   614,   420,   386,   599,   317,
     199,  1025,   299,   172,   199,   950,   596,   336,   309,   901,
     874,   199,   311,   603,    24,   946,   421,   491,   610,   635,
    -249,   492,   600,   615,   424,   220,   422,   172,  1198,   172,
    1019,   336,   729,  1056,   522,   -70,   312,   393,   159,   313,
     223,    62,   342,   346,  1026,  1073,   611,  1154,   951,   776,
    1230,   314,   385,  1224,   -70,  1226,   604,   581,   582,   646,
    -250,   507,   159,   648,   159,    60,  1211,   510,   510,  -256,
     317,   336,   425,   -70,   -69,   426,    87,    69,  1202,   818,
     172,   912,   605,   647,   172,   172,   780,   427,  1099,   868,
    -320,   828,   228,   -69,   318,   601,  -255,  -316,  1027,   606,
     952,   230,    89,   -70,  1232,   819,   428,   913,   172,   598,
     617,   618,   -69,   172,   234,   159,   612,  1058,  1135,   159,
     159,  -316,   510,   507,   619,   628,   953,   630,   785,   703,
     264,  -575,  1005,   818,  -406,   507,  -264,   234,  -645,   236,
    -645,  1220,   -69,   159,  -646,   303,  -646,   641,   159,   975,
     707,   348,   350,   580,  1004,  1187,  1188,  1066,  1006,  1007,
    1272,  1273,  1274,  1275,  1276,  1277,   510,   904,   708,   710,
     711,   712,   714,   716,   717,  -254,   718,   719,   720,   721,
     722,   818,   730,   731,   732,   733,   734,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   510,
    1097,  1157,   822,   280,   967,   336,   336,  1018,   492,   776,
    -645,   336,  -645,  1047,   336,   336,   336,   336,   756,   758,
    -721,   723,   172,   785,  -721,   304,   775,  -722,  1020,   873,
     778,  -722,  -251,  -642,   781,  -642,   782,   557,   724,  1048,
     386,  -258,   545,   702,   234,  1074,   789,   791,  1110,  1094,
     318,   725,   795,   797,   787,   709,  -262,   159,   801,    42,
     748,   749,  -327,   323,   343,   808,   750,   325,   810,   751,
     752,   336,  -723,   356,  1111,  1235,  -723,   785,   567,   568,
     726,   821,  -327,   824,   172,    58,   785,   294,  -588,   172,
     829,   830,  -327,   986,   989,   876,  -587,   991,   993,   444,
    -259,   328,  1236,  1172,  1237,  1238,   328,  1239,  1240,  1223,
     880,  1225,  1197,   877,  -588,   336,   557,  1058,   637,   159,
    -253,   727,  -587,   344,   159,   307,   781,   728,   881,   336,
    -257,   336,  1058,   444,   970,  1059,   857,   781,   860,  -718,
     862,   864,   510,  -718,   707,   309,   557,  1058,   336,   311,
    1163,  1244,  1058,  1036,  1038,   307,  -644,   445,  -644,   898,
     898,   446,  1170,   510,  1171,  1264,   172,   776,  -263,   598,
    1266,  -265,   909,   312,  -252,   309,   313,  -260,   378,   311,
      94,   409,   328,  -643,   371,  -643,   371,   172,   314,   397,
     410,   445,   930,   418,   931,   446,  -572,  1244,   172,   439,
     851,   159,   853,   312,  -571,   498,   313,   317,    62,   448,
    1176,   440,  -576,  -577,   452,   448,   453,   454,   314,  -574,
     452,   457,   159,   454,  -573,   923,   501,   457,   464,   499,
     386,   506,   928,   159,   509,   704,   470,   317,   399,   515,
     573,   177,   444,   448,   449,   517,   450,   451,   452,   544,
     453,   454,   563,   564,   574,   457,  1050,   729,   579,   585,
     588,   595,   464,   616,   620,   177,   631,   177,   468,   469,
     470,   621,   643,   642,   645,   653,   658,   705,   454,   784,
     747,   764,   783,   785,   510,  1065,   789,   942,   795,   945,
     808,   336,   948,   860,  1072,   786,   792,  1075,   803,   804,
     445,   806,   968,   969,   446,   809,   811,   812,   822,   836,
     336,   831,   336,   974,   845,   976,   846,   855,   177,   775,
     172,   983,   177,   177,   395,   902,   172,   510,   861,   507,
     994,   995,   919,   921,   924,   932,  -124,   172,  -145,   172,
     986,   989,  1036,  1038,  -152,  -151,   177,  -150,  -147,   336,
     938,   177,   448,   449,  -154,   159,   451,   452,  -155,   453,
     454,   159,  -149,   328,   457,   972,  -153,  1023,  -125,  -148,
    -156,   464,   159,   925,   159,   933,   949,   468,   469,   470,
     934,   935,   939,   936,  1071,   996,   998,   937,   318,  1016,
    1089,  1058,  1042,  1044,  1046,  1061,  1024,   318,  1208,  1209,
    1067,  1029,  1062,   628,  1011,  1092,  1086,  1053,   172,  1055,
     898,   898,   172,   357,  1125,   358,  1126,  1087,   360,   361,
    1085,  1095,  1127,   444,  1131,   510,   172,  1143,  1132,  1148,
    1164,   912,  1206,   336,  1152,  1186,  1214,  1229,  1265,  1259,
    1267,  1260,  1290,   159,  1291,  1234,   388,   159,  1156,  1072,
     869,   542,   625,  1049,   231,   444,   392,  1173,   353,  1128,
     177,   159,   592,  1142,   826,   834,   639,  1088,  1084,  1081,
    1091,   541,  1076,   832,   571,  1269,   336,  1177,   541,   775,
    1278,   445,  1271,   403,   929,   446,  1064,  1008,     0,   825,
       0,     0,     0,   714,   756,   393,     0,     0,   393,     0,
       0,     0,     0,     0,  1107,     0,  1109,     0,     0,   510,
     510,  1114,     0,   445,  1117,     0,     0,   446,     0,     0,
       0,     0,   177,     0,     0,     0,  1123,   177,     0,     0,
       0,     0,     0,   448,   449,     0,   450,   451,   452,  1071,
     453,   454,   455,   328,     0,   457,     0,  1138,     0,     0,
       0,   463,   464,   234,  1072,   467,     0,     0,   468,   469,
     470,     0,  1147,  1141,  1151,   448,   449,  1153,     0,   471,
     452,     0,   453,   454,   336,   965,     0,   457,   898,     0,
       0,     0,     0,     0,   464,  1166,     0,  1167,     0,     0,
     468,   469,   470,     0,     0,     0,   393,     0,     0,     0,
       0,     0,   180,     0,   177,     0,  1184,   172,     0,     0,
     172,     0,     0,     0,     0,   444,  1107,  1109,     0,  1114,
    1117,     0,  1147,  1151,     0,   177,   180,     0,   180,     0,
       0,     0,  1199,  1200,     0,  1201,   177,   775,     0,  1203,
    1204,     0,   159,  1205,  1071,   159,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   172,     0,   336,   336,
       0,     0,   172,     0,     0,     0,   181,     0,     0,   172,
    1215,     0,  1216,   445,     0,     0,  1218,   446,     0,   180,
       0,     0,     0,   180,   180,  1222,   898,   898,   898,   898,
     181,   159,   181,     0,     0,   541,   541,   159,     0,   541,
     541,     0,     0,     0,   159,     0,     0,   180,     0,     0,
       0,   541,   180,   541,     0,     0,  1199,  1248,  1249,  1203,
    1250,  1251,  1252,  1253,     0,   448,     0,     0,     0,     0,
     452,     0,   453,   454,     0,     0,     0,   457,     0,     0,
       0,     0,     0,   181,   464,     0,     0,   181,   181,     0,
     468,   469,   470,     0,  1268,     0,     0,     0,   177,     0,
    1270,     0,     0,     0,   177,     0,     0,  1100,  1101,     0,
       0,   181,     0,     0,     0,   177,   181,   177,   172,     0,
     172,  1286,  1287,  1288,  1289,   172,  1279,     0,   172,  1292,
    1293,     0,     0,     0,     0,     0,     0,   172,     0,   172,
       0,     0,   172,     0,     0,     0,     0,  1286,  1287,  1288,
    1289,  1292,  1293,   159,     0,   159,     0,     0,     0,     0,
     159,   180,     0,   159,     0,     0,   172,     0,     0,     0,
       0,     0,   159,     0,   159,     0,   172,   159,   172,     0,
       0,   269,   271,     0,     0,     0,   177,     0,     0,     0,
     177,     0,     0,     0,   186,     0,     0,     0,     0,     0,
       0,   159,     0,     0,   177,     0,     0,     0,     0,     0,
       0,   159,     0,   159,     0,     0,     0,     0,   186,     0,
     186,     0,     0,   180,     0,   181,     0,     0,   180,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   444,     0,   172,     0,     0,     0,
       0,     0,     0,   172,     0,   373,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   172,     0,     0,     0,
       0,   186,     0,     0,     0,   186,   186,     0,     0,     0,
     172,   159,     0,     0,     0,     0,     0,   181,   159,   172,
       0,     0,   181,   172,     0,   172,   172,     0,     0,   186,
       0,   159,   445,     0,   186,   180,   446,     0,     0,     0,
       0,     0,     0,     0,     0,   159,     0,     0,     0,     0,
       0,     0,     0,     0,   159,     0,   180,     0,   159,     0,
     159,   159,     0,     0,     0,     0,     0,   180,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   172,     0,     0,
     333,     0,     0,     0,   448,   449,     0,   450,   451,   452,
       0,   453,   454,     0,     0,     0,   457,     0,     0,   181,
       0,   333,   463,   464,     0,     0,   467,     0,     0,   468,
     469,   470,   159,   172,     0,   177,     0,   333,   177,     0,
     181,     0,     0,     0,     0,   172,   172,   172,   172,     0,
       0,   181,     0,   172,   172,     0,     0,     0,     0,     0,
       0,     0,     0,   186,     0,     0,     0,     0,   159,     0,
       0,     0,     0,     0,     0,     0,     0,   188,     0,     0,
     159,   159,   159,   159,   177,   907,     0,     0,   159,   159,
     177,     0,     0,     0,     0,     0,     0,   177,     0,     0,
       0,   188,     0,   188,     0,     0,  -307,     0,     0,   180,
    -307,  -307,     0,     0,     0,   180,     0,     0,     0,  -307,
       0,  -307,  -307,     0,     0,   186,   180,  -307,   180,     0,
     186,     0,     0,     0,  -307,     0,     0,  -307,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   188,     0,     0,  -307,   188,   188,
    -307,     0,  -307,     0,  -307,   333,  -307,  -307,     0,  -307,
       0,     0,  -307,   181,  -307,     0,     0,     0,     0,   181,
     269,     0,   188,   373,     0,     0,     0,   188,     0,     0,
     181,     0,   181,  -307,     0,     0,  -307,   180,     0,  -307,
       0,   180,     0,     0,     0,     0,   177,   186,   177,   333,
       0,     0,     0,   177,     0,   180,   177,     0,   723,     0,
       0,     0,     0,     0,  -307,   177,     0,   177,   186,     0,
     177,     0,   954,   333,     0,   955,     0,     0,     0,   186,
     956,   713,   715,     0,     0,     0,     0,     0,     0,  -307,
     201,     0,     0,     0,   177,  -307,     0,     0,     0,     0,
     957,   181,     0,     0,   177,   181,   177,   958,     0,     0,
       0,     0,     0,   333,     0,     0,     0,   959,     0,   181,
       0,     0,     0,     0,     0,   960,     0,   755,   757,     0,
       0,     0,     0,     0,     0,     0,   188,     0,     0,   777,
       0,     0,   961,     0,     0,     0,     0,     0,   380,     0,
       0,     0,    23,    24,   962,     0,   790,     0,   727,     0,
       0,   381,   796,    31,   382,   963,     0,   800,     0,    37,
       0,   964,     0,     0,   177,     0,    42,     0,     0,     0,
       0,   177,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   177,     0,     0,     0,   188,     0,
       0,   186,    58,   188,    60,     0,     0,   186,   177,     0,
       0,     0,     0,     0,   385,     0,    69,   177,   186,     0,
     186,   177,     0,   177,   177,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    85,   180,   189,    87,   180,
       0,    89,     0,     0,     0,   856,     0,   333,   333,     0,
       0,     0,     0,   333,     0,     0,   333,   333,   333,   333,
       0,   189,     0,   189,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   177,     0,     0,     0,     0,
     188,     0,     0,     0,     0,   180,     0,     0,     0,   186,
       0,   180,     0,   186,     0,     0,     0,     0,   180,     0,
     181,   188,     0,   181,     0,     0,     0,   186,     0,     0,
       0,   177,   188,   333,   189,     0,     0,     0,   189,   189,
       0,     0,     0,   177,   177,   177,   177,     0,     0,     0,
       0,   177,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   189,     0,     0,     0,     0,   189,     0,   181,
       0,     0,     0,     0,     0,   181,     0,   333,     0,     0,
       0,     0,   181,     0,     0,     0,     0,     0,     0,     0,
       0,   333,     0,   333,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,     0,
     333,     0,     0,     0,     0,     0,   941,     0,   944,     0,
       0,   947,     0,     0,     0,     0,     0,   180,     0,   180,
       0,     0,     0,     0,   180,     0,     0,   180,   487,     0,
       0,  -642,     0,  -642,     0,     0,   180,     0,   180,     0,
       0,   180,     0,     0,   188,     0,     0,     0,     0,     0,
     188,     0,     0,     0,     0,     0,     0,     0,   145,     0,
       0,   188,     0,   188,     0,   180,   189,     0,     0,     0,
       0,     0,     0,     0,     0,   180,     0,   180,     0,     0,
       0,   181,   145,   181,   145,     0,     0,     0,   181,     0,
       0,   181,     0,     0,     0,     0,     0,     0,   186,     0,
     181,   186,   181,     0,     0,   181,     0,     0,     0,     0,
       0,  1041,  1043,  1045,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   189,   181,
       0,     0,   188,   189,     0,   145,   188,     0,     0,   181,
       0,   181,     0,   333,     0,   180,     0,   186,     0,     0,
     188,     0,   180,   186,     0,     0,     0,     0,     0,     0,
     186,     0,   333,   145,   333,   180,     0,     0,   145,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   290,   180,
       0,     0,     0,     0,     0,     0,     0,     0,   180,     0,
       0,     0,   180,     0,   180,   180,     0,     0,     0,     0,
       0,   333,   339,     0,     0,     0,     0,     0,     0,   181,
     189,     0,   713,   755,     0,     0,   181,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   181,
       0,   189,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   189,   181,     0,     0,   180,     0,     0,     0,
       0,     0,   181,     0,     0,     0,   181,     0,   181,   181,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   186,
       0,   186,     0,     0,     0,     0,   186,   145,     0,   186,
       0,     0,   180,     0,     0,   333,     0,     0,   186,     0,
     186,     0,     0,   186,   180,   180,   180,   180,     0,     0,
       0,     0,   180,   180,     0,     0,     0,     0,     0,     0,
     181,     0,     0,     0,     0,     0,     0,   186,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   186,   333,   186,
       0,   188,     0,     0,   188,     0,     0,     0,     0,   145,
       0,     0,     0,     0,   145,     0,   181,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   181,   181,
     181,   181,     0,     0,   189,     0,   181,   181,     0,     0,
     189,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     188,   189,     0,   189,     0,     0,   188,     0,     0,   113,
       0,     0,     0,   188,     0,     0,   518,   186,   523,   527,
     529,   532,     0,     0,   186,     0,     0,     0,     0,     0,
       0,     0,     0,   232,     0,   235,     0,   186,     0,     0,
     559,   145,   561,     0,     0,     0,   333,     0,     0,   565,
       0,   186,     0,     0,     0,     0,     0,     0,     0,     0,
     186,     0,   145,     0,   186,     0,   186,   186,     0,     0,
       0,     0,   189,   145,     0,     0,   189,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   345,     0,     0,   587,
     189,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   593,   594,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   113,     0,     0,     0,   186,   379,
       0,     0,   188,   380,   188,     0,     0,    23,    24,   188,
     333,   333,   188,     0,     0,     0,   381,     0,    31,   382,
       0,   188,     0,   188,    37,     0,   188,     0,     0,     0,
       0,    42,     0,     0,   186,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   186,   186,   186,   186,
     188,     0,     0,     0,   186,   186,     0,    58,     0,    60,
     188,    62,   188,  1068,     0,     0,  1069,     0,     0,   385,
       0,    69,     0,     0,     0,   145,     0,     0,     0,     0,
       0,   145,     0,     0,     0,     0,     0,     0,     0,     0,
      85,     0,   145,    87,   145,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   500,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
     188,     0,     0,   779,     0,     0,     0,   188,     0,     0,
       0,     0,     0,     0,     0,     0,   105,     0,     0,     0,
     188,   189,  1070,   793,   189,     0,     0,   798,     0,   799,
       0,     0,   802,   145,   188,     0,     0,   145,     0,     0,
     547,     0,     0,   188,     0,   558,     0,   188,     0,   188,
     188,   145,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     189,     0,   237,     0,     0,     0,   189,     0,     0,     0,
       0,     0,     0,   189,     0,     0,   238,   239,     0,   240,
       0,     0,   289,     0,   241,     0,     0,     0,     0,     0,
       0,   188,   242,     0,     0,     0,     0,     0,   243,     0,
       0,     0,     0,     0,   244,     0,   289,   527,   245,     0,
       0,   246,   113,     0,     0,     0,   349,   351,     0,     0,
       0,   247,     0,     0,     0,     0,     0,   188,   248,   249,
       0,     0,     0,   113,     0,     0,   250,     0,     0,   188,
     188,   188,   188,     0,   113,     0,   251,   188,   188,     0,
     391,     0,     0,     0,     0,   252,   253,     0,   254,     0,
     255,     0,   256,     0,     0,   257,     0,     0,     0,   258,
     548,     0,   259,     0,     0,   260,     0,     0,     0,     0,
       0,     0,   189,     0,   189,     0,     0,     0,     0,   189,
       0,     0,   189,     0,     0,     0,     0,     0,     0,     0,
       0,   189,     0,   189,     0,     0,   189,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   145,     0,     0,   145,     0,     0,     0,     0,
     189,   549,     0,     0,     0,     0,     0,     0,     0,     0,
     189,     0,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   502,
       0,     0,     0,     0,     0,     0,   558,     0,     0,     0,
       0,   145,   558,     0,     0,     0,     0,   145,     0,   978,
     980,   262,     0,   805,   145,   985,   988,     0,     0,   990,
     992,     0,     0,     0,     0,   281,   282,   283,     0,     0,
     289,   289,   289,   289,   289,   289,     0,     0,   537,   540,
     189,     0,     0,     0,     0,   546,     0,   189,     0,     0,
       0,     0,     0,     0,   289,   237,   289,     0,     0,     0,
     189,     0,     0,   289,     0,     0,     0,     0,     0,   238,
     239,     0,   240,     0,   189,     0,     0,   241,     0,     0,
       0,     0,     0,   189,   854,   242,     0,   189,   859,   189,
     189,   243,     0,     0,     0,     0,     0,   244,     0,     0,
       0,   245,   113,   289,   246,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   247,     0,   289,   289,     0,     0,
       0,   248,   249,   145,     0,   145,     0,     0,     0,   250,
     145,     0,     0,   145,     0,     0,   412,     0,   415,   251,
       0,   189,   145,     0,   145,     0,     0,   145,   252,   253,
     435,   254,     0,   255,     0,   256,     0,     0,   257,     0,
       0,     0,   258,   575,     0,   259,     0,     0,   260,     0,
       0,   145,     0,     0,     0,     0,     0,   189,     0,     0,
       0,   145,     0,   145,     0,     0,   496,  1102,  1103,   189,
     189,   189,   189,  1104,     0,     0,     0,   189,   189,     0,
       0,     0,     0,     0,     0,  1115,     0,     0,  1118,     0,
    1119,     0,  1120,     0,     0,     0,   380,     0,    21,     0,
      23,    24,     0,     0,   576,     0,    28,     0,     0,   381,
       0,    31,   382,     0,   383,   384,    36,    37,     0,     0,
       0,     0,     0,     0,    42,     0,   516,     0,     0,     0,
       0,   145,     0,     0,   534,   535,     0,     0,   145,   -68,
     978,   980,   985,   988,     0,   550,     0,   289,     0,     0,
      58,   145,    60,     0,     0,     0,    64,     0,   -68,    65,
       0,     0,   385,   997,    69,   145,  1000,   289,     0,     0,
       0,   289,   442,   289,   145,     0,   289,     0,   145,   443,
     145,   145,     0,    85,     0,   577,    87,     0,     0,    89,
       0,   444,   519,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   520,     0,     0,
       0,     0,  1034,     0,     0,     0,     0,     0,  1039,     0,
       0,     0,     0,     0,     0,   859,     0,     0,     0,     0,
       0,     0,   145,     0,     0,     0,     0,     0,     0,   105,
    1213,     0,   262,     0,   386,     0,     0,     0,   387,   445,
       0,     0,     0,   446,     0,     0,     0,     0,     0,     0,
     289,   289,     0,     0,     0,     0,     0,     0,   145,     0,
     640,     0,     0,     0,     0,   415,     0,     0,     0,     0,
     145,   145,   145,   145,   447,     0,     0,     0,   145,   145,
       0,     0,     0,     0,     0,   699,     0,     0,     0,     0,
       0,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,     0,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,     0,   468,   469,   470,     0,
     386,     0,     0,     0,   558,     0,   558,   471,     0,     0,
       0,   558,     0,     0,   558,     0,     0,     0,     0,     0,
       0,     0,     0,  1121,   237,  1122,     0,     0,  1124,   767,
       0,     0,     0,     0,     0,     0,     0,     0,   238,   239,
       0,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,  1140,     0,   242,     0,     0,     0,     0,     0,
     243,     0,  1146,     0,  1150,     0,   244,     0,     0,     0,
     245,     0,     0,   246,     0,     0,   814,     0,     0,     0,
     550,     0,     0,   247,     0,     0,     0,     0,     0,     0,
     248,   249,     0,   827,     0,     0,     0,     0,   250,     0,
       0,     0,   577,   289,   289,     0,     0,   550,   251,   289,
     289,   237,     0,   289,   289,     0,     0,   252,   253,     0,
     254,     0,   255,  1002,   256,   238,   239,   257,   240,     0,
       0,   258,     0,   241,   259,     0,     0,   260,     0,     0,
       0,   242,     0,     0,     0,     0,     0,   243,     0,     0,
       0,     0,  1207,   244,     0,     0,     0,   245,     0,  1031,
     246,     0,   885,     0,     0,     0,  1212,     0,     0,     0,
     247,     0,     0,     0,     0,  1217,     0,   248,   249,  1219,
     442,  1146,  1150,     0,     0,   250,     0,   443,     0,     0,
       0,   386,     0,  1030,     0,   251,     0,     0,     0,   444,
     525,     0,     0,     0,   252,   253,     0,   254,     0,   255,
       0,   256,     0,     0,   257,   526,     0,     0,   258,     0,
       0,   259,     0,     0,   260,     0,     0,     0,     0,     0,
       0,     0,     0,  1258,     0,     0,     0,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   445,     0,     0,
       0,   446,     0,     0,     0,     0,     0,     0,     0,   113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     284,   289,   289,     0,     0,     0,     0,   289,   522,   540,
       0,     0,   447,     0,     0,     0,   540,     0,     0,   289,
       0,     0,   289,     0,   289,     0,   289,     0,     0,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,  1129,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,     0,     0,   468,   469,   470,     0,   386,     0,
       0,     0,  1003,     0,     0,   471,  1144,     0,     0,   550,
       0,     0,     0,     0,   289,   289,   289,   289,     0,  1012,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1032,     0,   915,
       0,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     200,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,   308,     0,   201,     0,     0,     0,    33,   202,   203,
       0,     0,   204,    39,     0,     0,     0,   310,     0,     0,
      43,     0,     0,   205,   435,     0,    47,    48,     0,     0,
      50,   415,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,   289,    59,   699,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   206,     0,     0,     0,
       0,     0,     0,     0,    74,   315,    76,    77,    78,   316,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   540,   540,     0,     0,   540,   540,    95,
      96,    97,    98,    99,     0,     0,     0,  1261,   100,   540,
       0,   540,     0,     0,     0,     0,   814,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     0,     0,   110,     0,     0,     0,    -2,
       4,  1032,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -68,   415,    40,    41,    42,
       0,    43,  -327,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -68,    53,    54,     0,    55,    56,
      57,     0,  -327,     0,     0,    58,    59,    60,    61,    62,
      63,    64,  -327,   -68,    65,    66,    67,    68,     0,    69,
      70,    71,     0,    72,    73,    74,    75,    76,    77,    78,
      79,     0,    80,    81,     0,    82,    83,    84,    85,    86,
       0,    87,    88,   -68,    89,    90,     0,     0,    91,     0,
      92,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,   105,     0,     0,   106,     0,   107,
       0,   108,     0,   109,     0,     4,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,  -673,     0,    13,    14,
      15,    16,    17,  -673,    18,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,  -673,    28,    29,  -673,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -68,     0,    40,    41,    42,     0,    43,  -327,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -68,
      53,    54,     0,    55,    56,    57,     0,  -327,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -327,   -68,    65,
      66,    67,    68,  -673,    69,    70,    71,  -673,    72,    73,
      74,    75,    76,    77,    78,    79,     0,    80,    81,     0,
      82,    83,    84,    85,    86,     0,    87,    88,   -68,    89,
      90,     0,     0,    91,     0,    92,     0,     0,  -673,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,  -673,    97,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,     0,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,   104,   105,
    -673,  -673,  -673,     0,   107,  -673,   108,     0,   109,     0,
     362,  -673,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -68,     0,    40,    41,    42,
       0,    43,  -327,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -68,    53,    54,     0,    55,    56,
      57,     0,  -327,     0,     0,    58,    59,    60,    61,    62,
      63,    64,  -327,   -68,    65,    66,    67,    68,     0,    69,
      70,    71,     0,    72,    73,    74,    75,    76,    77,    78,
      79,     0,    80,    81,     0,    82,    83,    84,    85,    86,
       0,    87,    88,   -68,    89,    90,     0,     0,    91,     0,
      92,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,   105,     0,     0,   106,     0,   107,
     363,   108,     0,   109,     0,     4,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -68,     0,    40,    41,    42,     0,    43,  -327,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -68,
      53,    54,     0,    55,    56,    57,     0,  -327,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -327,   -68,    65,
      66,    67,    68,     0,    69,    70,    71,     0,    72,    73,
      74,    75,    76,    77,    78,    79,     0,    80,    81,     0,
      82,    83,    84,    85,    86,     0,    87,    88,   -68,    89,
      90,     0,     0,    91,     0,    92,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,   105,
       0,     0,   106,     0,   107,   597,   108,     0,   109,     0,
     623,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -68,     0,    40,    41,    42,
       0,    43,  -327,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -68,    53,    54,     0,    55,    56,
      57,     0,  -327,     0,     0,    58,    59,    60,    61,    62,
      63,    64,  -327,   -68,    65,    66,    67,    68,     0,    69,
      70,    71,     0,    72,    73,    74,    75,    76,    77,    78,
      79,     0,    80,    81,     0,    82,    83,    84,    85,    86,
       0,    87,    88,   -68,    89,    90,     0,     0,    91,     0,
      92,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,   105,     0,     0,   106,     0,   107,
     624,   108,     0,   109,     0,     4,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -68,     0,    40,    41,    42,     0,    43,  -327,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -68,
      53,    54,     0,    55,    56,    57,     0,  -327,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -327,   -68,    65,
      66,    67,    68,     0,    69,    70,    71,     0,    72,    73,
      74,    75,    76,    77,    78,    79,     0,    80,    81,     0,
      82,    83,    84,    85,    86,     0,    87,    88,   -68,    89,
      90,     0,     0,    91,     0,    92,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,   105,
       0,     0,   106,     0,   107,   875,   108,     0,   109,     0,
       4,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -68,     0,    40,    41,    42,
       0,    43,  -327,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -68,    53,    54,     0,    55,    56,
      57,     0,  -327,     0,     0,    58,    59,    60,    61,   378,
      63,    64,  -327,   -68,    65,    66,    67,    68,     0,    69,
      70,    71,     0,    72,    73,    74,    75,    76,    77,    78,
      79,     0,    80,    81,     0,    82,    83,    84,    85,    86,
       0,    87,    88,   -68,    89,    90,     0,     0,    91,     0,
      92,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,   105,     0,     0,   106,     0,   107,
       0,   108,     0,   109,     0,     4,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -68,     0,    40,    41,    42,     0,    43,  -327,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -68,
      53,    54,     0,    55,    56,    57,     0,  -327,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -327,   -68,    65,
      66,    67,    68,     0,    69,    70,    71,     0,    72,    73,
      74,    75,    76,    77,    78,    79,     0,    80,    81,     0,
      82,    83,    84,    85,    86,     0,    87,    88,   -68,    89,
      90,     0,     0,    91,     0,    92,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,   105,
       0,     0,   106,     0,   107,     0,   108,     0,   109,     0,
       4,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -68,     0,    40,    41,    42,
       0,    43,  -327,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -68,    53,    54,     0,    55,    56,
      57,     0,  -327,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -327,   -68,    65,    66,    67,    68,     0,    69,
      70,    71,     0,    72,    73,    74,    75,    76,    77,    78,
      79,     0,    80,    81,     0,    82,    83,    84,    85,    86,
       0,    87,    88,   -68,    89,    90,     0,     0,    91,     0,
      92,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,   105,     0,     0,   106,     0,   107,
       0,   108,     0,   109,     0,     4,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,   788,    39,
     -68,     0,    40,    41,    42,     0,    43,  -327,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -68,
      53,    54,     0,    55,    56,    57,     0,  -327,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -327,   -68,    65,
      66,    67,    68,     0,    69,    70,    71,     0,    72,    73,
      74,    75,    76,    77,    78,    79,     0,    80,    81,     0,
      82,    83,    84,    85,    86,     0,    87,    88,   -68,    89,
      90,     0,     0,    91,     0,    92,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,   105,
       0,     0,   106,     0,   107,     0,   108,     0,   109,     0,
       4,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,   794,    39,   -68,     0,    40,    41,    42,
       0,    43,  -327,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -68,    53,    54,     0,    55,    56,
      57,     0,  -327,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -327,   -68,    65,    66,    67,    68,     0,    69,
      70,    71,     0,    72,    73,    74,    75,    76,    77,    78,
      79,     0,    80,    81,     0,    82,    83,    84,    85,    86,
       0,    87,    88,   -68,    89,    90,     0,     0,    91,     0,
      92,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,   105,     0,     0,   106,     0,   107,
       0,   108,     0,   109,     0,     4,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1106,    39,
     -68,     0,    40,    41,    42,     0,    43,  -327,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -68,
      53,    54,     0,    55,    56,    57,     0,  -327,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -327,   -68,    65,
      66,    67,    68,     0,    69,    70,    71,     0,    72,    73,
      74,    75,    76,    77,    78,    79,     0,    80,    81,     0,
      82,    83,    84,    85,    86,     0,    87,    88,   -68,    89,
      90,     0,     0,    91,     0,    92,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,   105,
       0,     0,   106,     0,   107,     0,   108,     0,   109,     0,
       4,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,  1108,    39,   -68,     0,    40,    41,    42,
       0,    43,  -327,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -68,    53,    54,     0,    55,    56,
      57,     0,  -327,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -327,   -68,    65,    66,    67,    68,     0,    69,
      70,    71,     0,    72,    73,    74,    75,    76,    77,    78,
      79,     0,    80,    81,     0,    82,    83,    84,    85,    86,
       0,    87,    88,   -68,    89,    90,     0,     0,    91,     0,
      92,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,   105,     0,     0,   106,     0,   107,
       0,   108,     0,   109,     0,     4,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1113,    39,
     -68,     0,    40,    41,    42,     0,    43,  -327,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -68,
      53,    54,     0,    55,    56,    57,     0,  -327,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -327,   -68,    65,
      66,    67,    68,     0,    69,    70,    71,     0,    72,    73,
      74,    75,    76,    77,    78,    79,     0,    80,    81,     0,
      82,    83,    84,    85,    86,     0,    87,    88,   -68,    89,
      90,     0,     0,    91,     0,    92,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,   105,
       0,     0,   106,     0,   107,     0,   108,     0,   109,     0,
       4,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,  1116,    39,   -68,     0,    40,    41,    42,
       0,    43,  -327,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -68,    53,    54,     0,    55,    56,
      57,     0,  -327,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -327,   -68,    65,    66,    67,    68,     0,    69,
      70,    71,     0,    72,    73,    74,    75,    76,    77,    78,
      79,     0,    80,    81,     0,    82,    83,    84,    85,    86,
       0,    87,    88,   -68,    89,    90,     0,     0,    91,     0,
      92,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,   105,     0,     0,   106,     0,   107,
       0,   108,     0,   109,     0,     4,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,  1139,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -68,     0,    40,    41,    42,     0,    43,  -327,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -68,
      53,    54,     0,    55,    56,    57,     0,  -327,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -327,   -68,    65,
      66,    67,    68,     0,    69,    70,    71,     0,    72,    73,
      74,    75,    76,    77,    78,    79,     0,    80,    81,     0,
      82,    83,    84,    85,    86,     0,    87,    88,   -68,    89,
      90,     0,     0,    91,     0,    92,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,   105,
       0,     0,   106,     0,   107,     0,   108,     0,   109,     0,
       4,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,  1145,    39,   -68,     0,    40,    41,    42,
       0,    43,  -327,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -68,    53,    54,     0,    55,    56,
      57,     0,  -327,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -327,   -68,    65,    66,    67,    68,     0,    69,
      70,    71,     0,    72,    73,    74,    75,    76,    77,    78,
      79,     0,    80,    81,     0,    82,    83,    84,    85,    86,
       0,    87,    88,   -68,    89,    90,     0,     0,    91,     0,
      92,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,   105,     0,     0,   106,     0,   107,
       0,   108,     0,   109,     0,     4,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1149,    39,
     -68,     0,    40,    41,    42,     0,    43,  -327,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -68,
      53,    54,     0,    55,    56,    57,     0,  -327,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -327,   -68,    65,
      66,    67,    68,     0,    69,    70,    71,     0,    72,    73,
      74,    75,    76,    77,    78,    79,     0,    80,    81,     0,
      82,    83,    84,    85,    86,     0,    87,    88,   -68,    89,
      90,     0,     0,    91,     0,    92,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,   105,
       0,     0,   106,     0,   107,     0,   108,     0,   109,     0,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,   661,     0,    13,     0,     0,    16,    17,   663,    18,
       0,   200,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,   204,    39,     0,     0,     0,    41,     0,
       0,    43,     0,   668,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,     0,     0,     0,    86,
       0,     0,    88,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,   539,   473,   474,   475,   476,   477,
       0,     0,   480,   481,   482,   483,     0,   485,   486,     0,
     886,   887,   888,   889,   890,   681,     0,   682,     0,   100,
       0,   683,   684,   685,   686,   687,   688,   689,   891,   691,
     692,   102,   892,   104,     0,   694,   695,   893,   697,   208,
       0,   108,     0,   209,     0,     0,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
       0,    16,    17,     0,    18,     0,   200,    20,    21,    22,
       0,     0,     0,     0,    27,     0,    28,    29,     0,   201,
       0,     0,     0,    33,    34,    35,    36,     0,    38,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   205,
       0,     0,    47,    48,     0,    49,    50,    51,    52,     0,
      53,    54,     0,    55,    56,    57,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   206,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,    77,    78,    79,     0,    80,    81,     0,
      82,     0,     0,     0,    86,     0,     0,    88,     0,     0,
      90,     0,     0,    91,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   107,     0,   108,     0,   109,     0,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   200,     0,     0,    22,   402,    24,     0,     0,     0,
       0,     0,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,   204,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,    60,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,     0,    69,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,     0,     0,     0,    86,
       0,     0,    88,     0,    89,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   208,
       0,   108,     0,   209,     0,     0,   110,     5,     6,     7,
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
      82,   330,   331,     0,    86,   367,     0,    88,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,   368,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   208,     0,   108,     0,   209,     0,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   200,     0,     0,    22,     0,   768,     0,     0,     0,
       0,     0,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,   204,    39,     0,   769,     0,    41,     0,
       0,    43,     0,     0,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,     0,   770,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,     0,     0,     0,    86,
       0,     0,    88,     0,   771,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   208,
       0,   108,     0,   209,     0,     0,   110,     5,     6,     7,
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
      82,   330,   331,     0,    86,   367,     0,    88,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   208,     0,   108,   850,   209,     0,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   200,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,   204,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,   330,   331,     0,    86,
     367,     0,    88,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   208,
       0,   108,   852,   209,     0,     0,   110,     5,     6,     7,
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
      82,   330,   331,     0,    86,     0,     0,    88,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,   332,
       0,     0,   106,     0,   208,     0,   108,     0,   209,     0,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   200,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,   204,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,   330,   331,     0,    86,
       0,     0,    88,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   208,
       0,   108,   870,   209,     0,     0,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   200,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   201,
       0,     0,     0,    33,   202,   203,     0,     0,   204,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   205,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,  1010,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   206,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   207,     0,    80,     0,     0,
      82,   330,   331,     0,    86,     0,     0,    88,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   208,     0,   108,     0,   209,     0,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   200,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,   204,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,   265,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,     0,     0,     0,    86,
       0,     0,    88,     0,     0,    90,     0,     0,     0,     0,
       0,   266,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   208,
       0,   108,     0,   209,     0,     0,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   200,     0,     0,    22,
       0,   273,     0,     0,     0,     0,     0,    29,     0,   201,
       0,     0,     0,    33,   202,   203,     0,     0,   204,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   205,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   206,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   207,     0,    80,     0,     0,
      82,     0,     0,     0,    86,     0,     0,    88,     0,   274,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   208,     0,   108,     0,   209,     0,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   200,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,   204,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,   330,   331,     0,    86,
       0,     0,    88,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   208,
       0,   108,     0,   209,     0,     0,   110,     5,     6,     7,
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
      90,     0,     0,     0,     0,     0,   266,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   208,     0,   108,     0,   209,   372,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   200,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,   204,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,  -717,     0,
       0,     0,  -717,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,     0,     0,     0,    86,
       0,     0,    88,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   208,
       0,   108,     0,   209,     0,     0,   110,     5,     6,     7,
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
      90,     0,     0,     0,     0,     0,   266,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   208,     0,   108,     0,   209,     0,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   200,     0,     0,    22,     0,     0,     0,     0,     0,
       0,   288,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,   204,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,     0,     0,     0,    86,
       0,     0,    88,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   107,
       0,   108,     0,   209,     0,     0,   110,     5,     6,     7,
       8,     9,   405,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,    19,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   201,
       0,     0,     0,    33,   202,   203,     0,     0,   204,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   205,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,    68,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   207,     0,    80,     0,     0,
      82,     0,     0,     0,    86,     0,    87,    88,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   208,     0,   108,     0,   209,     0,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   200,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,   204,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,     0,     0,     0,    86,
       0,     0,    88,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   208,
       0,   108,     0,   209,   372,     0,   110,     5,     6,     7,
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
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   208,   848,   108,     0,   209,     0,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   200,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,   204,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,     0,     0,     0,    86,
       0,     0,    88,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   208,
       0,   108,     0,   209,   858,     0,   110,     5,     6,     7,
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
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   208,     0,   108,     0,   209,   863,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   200,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,   204,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,     0,     0,     0,    86,
    1054,     0,    88,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   208,
       0,   108,     0,   209,     0,     0,   110,     5,     6,     7,
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
      82,     0,     0,     0,    86,  1221,     0,    88,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   208,     0,   108,     0,   209,     0,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   200,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,   204,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,     0,     0,     0,    86,
       0,     0,    88,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   208,
       0,   108,     0,   209,     0,     0,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   200,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,   308,     0,   201,
       0,     0,     0,    33,   202,   203,     0,     0,   204,    39,
       0,     0,     0,   310,     0,     0,    43,     0,     0,   205,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   206,     0,     0,     0,     0,     0,     0,     0,
      74,   315,    76,    77,    78,   316,     0,    80,     0,     0,
      82,     0,     0,     0,    86,     0,     0,    88,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   208,     0,   108,     0,   209,     0,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   200,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,   940,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,     0,     0,     0,    86,
       0,     0,    88,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   208,
       0,   108,     0,   209,     0,     0,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   200,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   201,
       0,     0,     0,    33,   202,   203,     0,     0,   943,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   205,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   206,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   207,     0,    80,     0,     0,
      82,     0,     0,     0,    86,     0,     0,    88,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   208,     0,   108,     0,   209,     0,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   200,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,  1190,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,     0,     0,     0,    86,
       0,     0,    88,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   208,
       0,   108,     0,   209,     0,     0,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   200,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   201,
       0,     0,     0,    33,   202,   203,     0,     0,  1191,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   205,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   206,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   207,     0,    80,     0,     0,
      82,     0,     0,     0,    86,     0,     0,    88,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   208,     0,   108,     0,   209,     0,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   200,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,  1193,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,     0,     0,     0,    86,
       0,     0,    88,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   208,
       0,   108,     0,   209,     0,     0,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   200,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   201,
       0,     0,     0,    33,   202,   203,     0,     0,  1194,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   205,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   206,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   207,     0,    80,     0,     0,
      82,     0,     0,     0,    86,     0,     0,    88,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   208,     0,   108,     0,   209,     0,
       0,   110,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   200,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   201,     0,     0,     0,    33,   202,
     203,     0,     0,  1195,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   205,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   206,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     207,     0,    80,     0,     0,    82,     0,     0,     0,    86,
       0,     0,    88,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   208,
       0,   108,     0,   209,     0,     0,   110,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   200,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   201,
       0,     0,     0,    33,   202,   203,     0,     0,  1196,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   205,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
     444,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   206,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   207,     0,    80,     0,     0,
      82,     0,     0,     0,    86,     0,     0,    88,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   445,     0,
       0,     0,   446,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,   447,   208,     0,   108,     0,   209,     0,
       0,   110,     6,     7,     8,     9,    10,    11,    12,     0,
     448,   449,    13,   450,   451,   452,    17,   453,   454,   455,
     200,   456,   457,   458,   459,   307,   461,   462,   463,   464,
     465,   466,   467,   201,     0,   468,   469,   470,   202,   203,
       0,     0,   204,     0,     0,   309,   471,     0,     0,   311,
       0,     0,     0,   205,     0,     0,    47,    48,     0,     0,
       0,     0,    52,     0,    53,    54,     0,     0,     0,     0,
       0,     0,     0,   312,   444,    59,   313,    61,    62,    63,
       0,     0,     0,     0,    66,     0,   206,     0,   314,     0,
       0,     0,     0,     0,    74,     0,    76,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,   317,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   445,     0,     0,     0,   446,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,   447,   208,     0,
     108,  -331,   209,     0,     0,   110,     6,     7,     8,     9,
      10,    11,    12,     0,   448,   449,    13,   450,   451,   452,
      17,   453,   454,   455,   200,   456,   457,   458,   459,   307,
     461,   462,   463,   464,   465,     0,   467,   201,     0,   468,
     469,   470,   202,   203,     0,     0,   204,     0,     0,   309,
     471,     0,     0,   311,     0,     0,     0,   205,     0,     0,
      47,    48,     0,     0,     0,     0,    52,     0,    53,    54,
       0,     0,     0,     0,     0,     0,     0,   312,     0,    59,
     313,    61,    62,    63,     0,     0,     0,     0,    66,     0,
     206,     0,   314,     0,     0,     0,     0,     0,    74,     0,
      76,     0,    78,     0,     0,     0,     0,     0,     0,     0,
       0,   317,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   659,
       0,   660,     0,    95,    96,    97,    98,    99,     0,     0,
     661,     0,   100,     0,     0,   662,   239,   663,   664,     0,
       0,   101,     0,   665,   102,   103,   104,     0,     0,     0,
     106,   242,   208,   201,   108,     0,   209,   243,     0,   110,
       0,     0,     0,   666,     0,     0,     0,   245,     0,     0,
     667,     0,   668,     0,     0,     0,     0,     0,     0,     0,
     669,     0,     0,     0,     0,     0,     0,   248,   670,     0,
       0,     0,     0,     0,     0,   250,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   671,     0,     0,     0,     0,
       0,     0,     0,     0,   252,   253,     0,   672,     0,   255,
       0,   673,     0,     0,   257,     0,     0,     0,   674,     0,
       0,   259,     0,     0,   675,     0,     0,     0,     0,     0,
       0,     0,     0,   539,   473,   474,   475,   476,   477,     0,
       0,   480,   481,   482,   483,     0,   485,   486,     0,   676,
     677,   678,   679,   680,   681,     0,   682,     0,     0,     0,
     683,   684,   685,   686,   687,   688,   689,   690,   691,   692,
       0,   693,     0,     0,   694,   695,   696,   697,     0,     0,
     698,     5,     6,     7,     8,     9,    10,    11,    12,     0,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
       0,   103,   104,     0,     0,     0,   106,     0,   208,     0,
     108,     0,   209,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   200,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   201,     0,     0,     0,    33,
     202,   203,     0,     0,   204,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   205,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,    67,   206,     0,
       0,     0,     0,     0,     0,     0,    74,    75,    76,    77,
      78,   207,     0,    80,     0,     0,    82,     0,     0,     0,
      86,     0,     0,    88,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,    97,     0,     0,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,   104,   200,     0,     0,    22,     0,
     208,     0,   108,     0,   209,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,   723,     0,
      59,     0,    61,     0,    63,     0,     0,   661,     0,    66,
      67,   206,     0,     0,   663,   724,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,   725,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   668,
       0,     0,     0,     0,     0,     0,     0,   726,     0,     0,
       0,     0,     0,     0,    95,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   104,     0,     0,
       0,     0,     0,     0,     0,   108,     0,   209,   727,     0,
       0,     0,     0,     0,   728,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     539,   473,   474,   475,   476,   477,     0,     0,   480,   481,
     482,   483,     0,   485,   486,     0,   676,   677,   678,   679,
     680,   681,     0,   682,     0,     0,     0,   683,   684,   685,
     686,   687,   688,   689,   690,   691,   692,   237,   693,     0,
       0,   694,   695,   696,   697,  1051,   661,     0,     0,     0,
       0,   238,   239,   663,   240,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,   242,     0,     0,
       0,     0,     0,   243,     0,     0,     0,     0,     0,   244,
       0,     0,     0,   245,     0,     0,   246,     0,   668,     0,
       0,     0,     0,     0,     0,     0,   247,     0,     0,     0,
       0,     0,     0,   248,   249,     0,     0,     0,     0,     0,
       0,   250,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   251,     0,     0,     0,     0,     0,   442,     0,     0,
     252,   253,     0,   254,   443,   255,     0,   256,     0,     0,
     257,     0,     0,     0,   258,     0,   444,   259,     0,     0,
     260,     0,     0,     0,     0,     0,     0,     0,     0,   539,
     473,   474,   475,   476,   477,     0,     0,   480,   481,   482,
     483,     0,   485,   486,     0,   676,   677,   678,   679,   680,
     681,     0,   682,     0,     0,     0,   683,   684,   685,   686,
     687,   688,   689,   690,   691,   692,     0,   693,     0,     0,
     694,   695,   696,   697,   445,     0,     0,   442,   446,     0,
       0,     0,     0,     0,   443,     0,     0,   538,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   447,
     539,   473,   474,   475,   476,   477,     0,     0,   480,   481,
     482,   483,     0,   485,   486,     0,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,   468,   469,   470,   445,   386,     0,   442,   446,     0,
       0,     0,   471,     0,   443,     0,     0,  1105,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   447,
     539,   473,   474,   475,   476,   477,     0,     0,   480,   481,
     482,   483,     0,   485,   486,     0,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,   468,   469,   470,   445,   386,     0,   442,   446,     0,
       0,     0,   471,     0,   443,     0,     0,  1112,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   447,
     539,   473,   474,   475,   476,   477,     0,     0,   480,   481,
     482,   483,     0,   485,   486,     0,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,   468,   469,   470,   445,   386,     0,   442,   446,     0,
       0,     0,   471,     0,   443,     0,     0,  1254,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   447,
     539,   473,   474,   475,   476,   477,     0,     0,   480,   481,
     482,   483,     0,   485,   486,     0,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,   468,   469,   470,   445,   386,     0,   442,   446,     0,
       0,     0,   471,     0,   443,     0,     0,  1255,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   447,
     539,   473,   474,   475,   476,   477,     0,     0,   480,   481,
     482,   483,     0,   485,   486,     0,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,   468,   469,   470,   445,   386,     0,   442,   446,     0,
       0,     0,   471,     0,   443,     0,     0,  1256,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   447,
     539,   473,   474,   475,   476,   477,     0,     0,   480,   481,
     482,   483,     0,   485,   486,     0,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,   468,   469,   470,   445,   386,     0,   442,   446,     0,
       0,     0,   471,     0,   443,     0,     0,  1257,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   447,
     539,   473,   474,   475,   476,   477,     0,     0,   480,   481,
     482,   483,     0,   485,   486,     0,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,   468,   469,   470,   445,   386,     0,   442,   446,     0,
       0,     0,   471,     0,   443,     0,     0,  1262,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   447,
     539,   473,   474,   475,   476,   477,     0,     0,   480,   481,
     482,   483,     0,   485,   486,     0,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,   468,   469,   470,   445,   386,     0,     0,   446,     0,
       0,     0,   471,     0,     0,     0,     0,  1263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   442,     0,     0,
       0,     0,     0,     0,   443,     0,     0,     0,     0,   447,
     539,   473,   474,   475,   476,   477,   444,   288,   480,   481,
     482,   483,     0,   485,   486,     0,   448,   449,     0,   450,
     451,   452,   531,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
       0,   468,   469,   470,     0,   386,     0,     0,     0,     0,
       0,     0,   471,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   442,   445,     0,     0,     0,   446,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   444,   288,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   522,     0,     0,   866,   447,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,     0,
     445,   468,   469,   470,   446,   386,     0,     0,     0,     0,
       0,     0,   471,     0,     0,   442,     0,     0,     0,     0,
       0,     0,   443,     0,     0,     0,     0,     0,     0,     0,
       0,   522,     0,     0,   444,   447,     0,     0,     0,   632,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   448,   449,     0,   450,   451,   452,     0,   453,
     454,   455,     0,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   633,     0,   468,   469,   470,
       0,   386,     0,     0,     0,     0,     0,     0,   471,     0,
       0,     0,   445,     0,     0,   442,   446,     0,     0,     0,
       0,     0,   443,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   444,   288,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   447,     0,     0,
     865,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   448,   449,     0,   450,   451,   452,
       0,   453,   454,   455,     0,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,   634,   468,
     469,   470,   445,     0,     0,     0,   446,     0,     0,     0,
     471,     0,     0,     0,     0,     0,     0,   442,   882,     0,
       0,     0,     0,     0,   443,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,   447,     0,     0,
       0,   883,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   448,   449,     0,   450,   451,   452,
       0,   453,   454,   455,     0,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   884,     0,   468,
     469,   470,     0,   386,     0,     0,     0,     0,     0,     0,
     471,     0,     0,   442,   445,     0,     0,     0,   446,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   444,   987,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   447,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   442,
     445,   468,   469,   470,   446,     0,   443,     0,     0,     0,
       0,     0,   471,     0,     0,     0,     0,     0,   444,   288,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   522,     0,     0,     0,   447,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   448,   449,     0,   450,   451,   452,     0,   453,
     454,   455,     0,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   442,   445,   468,   469,   470,
     446,   386,   443,     0,     0,     0,     0,     0,   471,     0,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   522,     0,     0,
     503,   447,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   448,   449,
       0,   450,   451,   452,     0,   453,   454,   455,     0,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   442,   445,   468,   469,   470,   446,   386,   443,     0,
       0,     0,     0,     0,   471,     0,     0,     0,     0,     0,
     444,   288,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   220,     0,     0,     0,   447,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   448,   449,     0,   450,   451,   452,
       0,   453,   454,   455,     0,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   442,   445,   468,
     469,   470,   446,     0,   443,     0,     0,     0,     0,     0,
     471,     0,     0,     0,     0,     0,   444,   759,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   760,   447,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     448,   449,     0,   450,   451,   452,     0,   453,   454,   455,
       0,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   442,   445,   468,   469,   470,   446,   386,
     443,     0,     0,     0,     0,     0,   471,     0,     0,     0,
       0,     0,   444,   761,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   762,   447,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   442,
     445,   468,   469,   470,   446,     0,   443,     0,     0,     0,
       0,     0,   471,     0,     0,     0,     0,     0,   444,   979,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   447,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   448,   449,     0,   450,   451,   452,     0,   453,
     454,   455,     0,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   442,   445,   468,   469,   470,
     446,     0,   443,     0,     0,     0,     0,     0,   471,     0,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   447,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   448,   449,
       0,   450,   451,   452,     0,   453,   454,   455,     0,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   442,   445,   468,   469,   470,   446,   386,   443,     0,
       0,     0,     0,     0,   471,  1001,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   447,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   448,   449,     0,   450,   451,   452,
       0,   453,   454,   455,     0,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   442,   445,   468,
     469,   470,   446,   386,   443,     0,     0,     0,     0,     0,
     471,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   522,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     448,   449,     0,   450,   451,   452,     0,   453,   454,   455,
       0,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,     0,   445,   468,   469,   470,   446,     0,
       0,     0,     0,     0,  1037,     0,   471,     0,     0,   442,
       0,     0,     0,     0,     0,     0,   443,     0,     0,     0,
       0,     0,     0,     0,     0,   522,     0,     0,   444,   447,
       0,     0,     0,  1161,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,  1162,
     444,   468,   469,   470,     0,     0,     0,     0,     0,     0,
    1155,     0,   471,     0,     0,     0,   445,     0,     0,     0,
     446,   442,   562,     0,     0,     0,     0,     0,   443,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   447,     0,     0,     0,     0,     0,     0,   445,     0,
       0,     0,   446,     0,     0,     0,     0,     0,   448,   449,
       0,   450,   451,   452,     0,   453,   454,   455,     0,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,   468,   469,   470,     0,     0,   445,     0,
       0,   442,   446,     0,   471,     0,     0,     0,   443,     0,
     448,   449,     0,   450,   451,   452,     0,   453,   454,   455,
     444,   456,   457,   458,   459,     0,   461,   462,   463,   464,
     465,     0,   467,   447,     0,   468,   469,   470,     0,     0,
       0,   442,     0,     0,     0,     0,   471,     0,   443,     0,
     448,   449,     0,   450,   451,   452,     0,   453,   454,   455,
     444,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,     0,     0,   468,   469,   470,   445,     0,
       0,     0,   446,     0,     0,     0,   471,   442,     0,     0,
       0,     0,     0,     0,   443,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,   447,     0,     0,     0,     0,   445,     0,
       0,     0,   446,     0,     0,     0,     0,     0,     0,     0,
     448,   449,     0,   450,   451,   452,     0,   453,   454,   455,
       0,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   447,     0,   468,   469,   470,     0,   586,
       0,     0,     0,     0,   445,     0,   471,     0,   446,     0,
     448,   449,     0,   450,   451,   452,     0,   453,   454,   455,
       0,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   442,   590,   468,   469,   470,   613,   447,
     443,     0,     0,     0,     0,     0,   471,     0,     0,     0,
       0,     0,   444,     0,     0,     0,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   442,
       0,   468,   469,   470,     0,     0,   443,     0,     0,     0,
       0,     0,   471,     0,     0,     0,     0,     0,   444,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     445,     0,     0,     0,   446,     0,     0,     0,     0,   442,
     878,     0,     0,   763,     0,     0,   443,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   444,     0,
       0,     0,     0,     0,     0,   447,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,     0,     0,     0,
     446,     0,   448,   449,     0,   450,   451,   452,     0,   453,
     454,   455,     0,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,   468,   469,   470,
       0,   447,     0,     0,     0,   442,   445,     0,   471,     0,
     446,     0,   443,     0,     0,     0,     0,     0,   448,   449,
       0,   450,   451,   452,   444,   453,   454,   455,     0,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   447,     0,   468,   469,   470,     0,     0,     0,     0,
     849,     0,     0,     0,   471,     0,     0,     0,   448,   449,
       0,   450,   451,   452,     0,   453,   879,   455,     0,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   442,   445,   468,   469,   470,   446,     0,   443,     0,
       0,     0,     0,     0,   471,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   220,     0,     0,     0,   447,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   448,   449,     0,   450,   451,   452,
       0,   453,   454,   455,     0,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,   445,   468,
     469,   470,   446,     0,   442,     0,     0,     0,     0,     0,
     471,   443,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,   999,     0,     0,     0,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     448,   449,     0,   450,   451,   452,     0,   453,   454,   455,
       0,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,     0,   971,   468,   469,   470,     0,     0,
     442,   445,     0,     0,     0,   446,   471,   443,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   444,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,     0,     0,     0,
     442,     0,     0,     0,     0,     0,     0,   443,     0,     0,
       0,     0,     0,   448,   449,     0,   450,   451,   452,   444,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,   445,   468,   469,
     470,   446,     0,     0,     0,     0,   442,     0,     0,   471,
       0,     0,     0,   443,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   444,     0,     0,     0,     0,
       0,     0,   447,     0,     0,     0,     0,   445,     0,     0,
       0,   446,     0,     0,     0,     0,     0,     0,     0,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,     0,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   447,     0,   468,   469,   470,     0,     0,     0,
       0,  1013,     0,   445,     0,   471,     0,   446,     0,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,     0,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   442,  1060,   468,   469,   470,  1040,   447,   443,
       0,  1014,     0,     0,     0,   471,     0,     0,     0,     0,
       0,   444,     0,     0,     0,   448,   449,     0,   450,   451,
     452,     0,   453,   454,   455,     0,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   442,     0,
     468,   469,   470,     0,     0,   443,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,   444,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   445,
       0,     0,     0,   446,     0,     0,     0,     0,   442,     0,
       0,     0,     0,     0,     0,   443,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   444,  1090,     0,
       0,     0,     0,     0,   447,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   445,     0,     0,     0,   446,
       0,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,     0,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,     0,   468,   469,   470,     0,
     447,     0,     0,     0,   442,   445,     0,   471,     0,   446,
       0,   443,     0,     0,     0,     0,     0,   448,   449,     0,
     450,   451,   452,   444,   453,   454,   455,     0,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     447,     0,   468,   469,   470,     0,     0,     0,     0,  1083,
       0,     0,     0,   471,     0,     0,     0,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,     0,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     442,   445,   468,   469,   470,   446,     0,   443,     0,     0,
       0,     0,     0,   471,     0,     0,     0,     0,     0,   444,
    1093,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,     0,     0,     0,
     442,  1158,     0,     0,     0,     0,     0,   443,     0,     0,
       0,     0,     0,   448,   449,     0,   450,   451,   452,   444,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,   445,   468,   469,
     470,   446,     0,     0,     0,   442,     0,  1096,     0,   471,
       0,     0,   443,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
       0,     0,   447,     0,     0,     0,     0,   445,     0,     0,
       0,   446,     0,     0,     0,     0,     0,     0,     0,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,     0,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   447,     0,   468,   469,   470,     0,     0,     0,
       0,     0,   445,     0,     0,   471,   446,     0,     0,   448,
     449,     0,   450,   451,   452,  1189,   453,   454,   455,     0,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   442,     0,   468,   469,   470,   447,     0,   443,
       0,     0,     0,     0,     0,   471,     0,     0,     0,     0,
       0,   444,     0,     0,   448,   449,     0,   450,   451,   452,
       0,   453,   454,   455,     0,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   442,  1227,   468,
     469,   470,     0,     0,   443,     0,     0,     0,     0,     0,
     471,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   445,
       0,     0,     0,   446,     0,     0,     0,     0,     0,     0,
       0,     0,  1192,   442,     0,     0,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   444,     0,   447,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   445,     0,     0,     0,   446,     0,
       0,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,  1241,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,     0,   468,   469,   470,   447,
       0,     0,     0,     0,     0,     0,     0,   471,     0,     0,
     445,     0,     0,     0,   446,     0,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   442,
       0,   468,   469,   470,     0,   447,   443,     0,     0,     0,
       0,     0,   471,     0,     0,     0,     0,     0,   444,     0,
       0,     0,   448,   449,     0,   450,   451,   452,     0,   453,
     454,   455,     0,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,   468,   469,   470,
       0,     0,     0,     0,     0,     0,     0,     0,   471,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   442,   445,     0,     0,     0,
     446,     0,   443,     0,     0,     0,     0,     0,     0,  1280,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   447,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   448,   449,
       0,   450,   451,   452,     0,   453,   454,   455,     0,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   442,   445,   468,   469,   470,   446,     0,   443,     0,
       0,     0,     0,     0,   471,  1281,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   447,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   448,   449,     0,   450,   451,   452,
       0,   453,   454,   455,     0,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   442,   445,   468,
     469,   470,   446,     0,   443,     0,     0,     0,     0,     0,
     471,  1282,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     448,   449,     0,   450,   451,   452,     0,   453,   454,   455,
       0,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   442,   445,   468,   469,   470,   446,     0,
     443,     0,     0,     0,     0,     0,   471,  1283,     0,     0,
       0,     0,   444,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   447,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   442,
     445,   468,   469,   470,   446,     0,   443,     0,     0,     0,
       0,     0,   471,  1284,     0,     0,     0,     0,   444,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   447,     0,     0,     0,   442,
       0,     0,     0,     0,     0,     0,   443,     0,     0,     0,
       0,     0,   448,   449,     0,   450,   451,   452,   444,   453,
     454,   455,     0,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,   445,   468,   469,   470,
     446,     0,     0,     0,   442,     0,     0,     0,   471,  1285,
       0,   443,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,     0,     0,     0,     0,
       0,   447,     0,     0,     0,     0,   445,     0,     0,     0,
     446,     0,     0,     0,     0,     0,     0,     0,   448,   449,
       0,   450,   451,   452,     0,   453,   454,   455,     0,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   447,     0,   468,   469,   470,     0,     0,     0,     0,
       0,   966,     0,     0,   471,   446,     0,     0,   448,   449,
       0,   450,   451,   452,     0,   453,   454,   455,     0,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,     0,     0,   468,   469,   470,   447,     0,     0,     0,
       0,     0,     0,     0,   471,     0,     0,     0,     0,     0,
       0,     0,     0,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,   380,   468,   469,
     470,    23,    24,     0,     0,     0,     0,     0,     0,   471,
     381,     0,    31,   382,     0,   380,     0,     0,    37,    23,
      24,     0,     0,     0,     0,    42,     0,     0,   381,     0,
      31,   382,     0,     0,     0,     0,    37,     0,     0,     0,
       0,     0,     0,    42,     0,     0,     0,     0,     0,     0,
       0,    58,   444,    60,     0,    62,     0,  1068,     0,     0,
    1069,     0,     0,   385,     0,    69,     0,     0,     0,    58,
       0,    60,     0,    62,     0,  1068,     0,     0,  1069,     0,
       0,   385,     0,    69,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
     445,     0,     0,     0,   446,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
     105,     0,     0,     0,     0,     0,  1169,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   105,     0,
       0,     0,   448,   449,  1228,   450,   451,   452,     0,   453,
     454,   455,     0,     0,   457,   458,   459,     0,   461,   462,
     463,   464,     0,     0,   467,     0,   380,   468,   469,   470,
      23,    24,     0,     0,     0,     0,     0,     0,   471,   381,
       0,    31,   382,     0,   380,     0,     0,    37,    23,    24,
       0,     0,     0,     0,    42,     0,     0,   381,     0,    31,
     382,     0,     0,     0,     0,    37,     0,     0,     0,     0,
       0,     0,    42,     0,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,   378,     0,  1068,     0,     0,  1069,
       0,     0,   385,     0,    69,     0,     0,     0,    58,     0,
      60,     0,     0,     0,  1068,     0,     0,  1069,     0,   413,
     385,   237,    69,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,   238,   239,     0,   240,     0,
       0,    85,     0,   241,    87,     0,     0,    89,     0,     0,
       0,   242,     0,     0,    94,     0,     0,   243,     0,     0,
       0,     0,     0,   244,     0,     0,     0,   245,     0,     0,
     246,     0,    94,     0,     0,     0,   237,     0,     0,   105,
     247,     0,     0,     0,     0,     0,     0,   248,   249,     0,
     238,   239,     0,   240,     0,   250,     0,   105,   241,     0,
       0,     0,     0,     0,     0,   251,   242,     0,     0,     0,
       0,     0,   243,     0,   252,   253,     0,   254,   244,   255,
       0,   256,   245,     0,   257,   246,     0,     0,   258,     0,
       0,   259,     0,     0,   260,   247,     0,     0,     0,     0,
       0,     0,   248,   249,     0,     0,     0,     0,     0,     0,
     250,   237,     0,     0,     0,     0,     0,     0,    94,     0,
     251,     0,     0,     0,     0,   238,   239,     0,   240,   252,
     253,     0,   254,   241,   255,    24,   256,     0,     0,   257,
       0,   242,     0,   258,     0,     0,   259,   243,     0,   260,
       0,     0,     0,   244,     0,     0,     0,   245,     0,     0,
     246,     0,     0,     0,     0,     0,   237,     0,     0,     0,
     247,     0,     0,    94,     0,     0,     0,   248,   249,     0,
     238,   239,     0,   240,     0,   250,    60,     0,   241,     0,
       0,     0,     0,     0,     0,   251,   242,     0,    69,     0,
       0,     0,   243,     0,   252,   253,     0,   254,   244,   255,
       0,   256,   245,     0,   257,   246,     0,     0,   258,     0,
       0,   259,     0,    89,   260,   247,     0,     0,     0,     0,
       0,     0,   248,   249,     0,     0,     0,     0,     0,     0,
     250,  -302,     0,     0,     0,     0,     0,     0,     0,     0,
     251,     0,     0,     0,     0,  -302,  -302,     0,  -302,   252,
     253,     0,   254,  -302,   255,     0,   256,     0,     0,   257,
       0,  -302,     0,   258,   575,     0,   259,  -302,     0,   260,
       0,     0,     0,  -302,     0,     0,     0,  -302,     0,     0,
    -302,     0,     0,     0,     0,     0,   237,     0,     0,     0,
    -302,     0,     0,     0,     0,     0,     0,  -302,  -302,     0,
     238,   239,     0,   240,     0,  -302,     0,     0,   241,     0,
       0,     0,     0,     0,     0,  -302,   242,     0,     0,     0,
       0,     0,   243,     0,  -302,  -302,     0,  -302,   244,  -302,
       0,  -302,   245,     0,  -302,   246,     0,     0,  -302,     0,
       0,  -302,     0,     0,  -302,   247,     0,     0,     0,     0,
       0,     0,   248,   249,     0,     0,     0,     0,     0,     0,
     250,  -303,     0,     0,     0,     0,     0,     0,     0,     0,
     251,     0,     0,     0,     0,  -303,  -303,     0,  -303,   252,
     253,     0,   254,  -303,   255,     0,   256,     0,     0,   257,
       0,  -303,     0,   258,     0,     0,   259,  -303,     0,   260,
       0,     0,     0,  -303,     0,     0,     0,  -303,     0,     0,
    -303,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -303,     0,     0,     0,     0,     0,     0,  -303,  -303,     0,
       0,     0,     0,     0,     0,  -303,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -303,     0,     0,     0,     0,
       0,     0,     0,     0,  -303,  -303,     0,  -303,     0,  -303,
       0,  -303,     0,     0,  -303,     0,     0,     0,  -303,     0,
       0,  -303,     0,     0,  -303
};

static const yytype_int16 yycheck[] =
{
      13,   118,    66,   157,    17,    20,     2,   280,    21,   414,
     584,   170,   434,   139,    27,    66,   499,   655,   229,    66,
     792,    34,    35,    36,    27,    38,   269,   838,   271,   272,
     588,   304,   701,   579,   703,   652,    49,   884,    51,    52,
     406,   879,   632,   832,    57,   908,    59,     1,    61,   419,
     902,   554,     1,    66,    31,   544,    47,    70,    71,    72,
      73,    74,    75,   621,     8,     3,    79,    70,    81,     1,
       1,   344,   914,    32,   236,    88,    27,    46,    91,    92,
      40,    47,    95,     3,    97,   102,    32,   100,   101,     1,
     103,   123,   120,    60,   107,   108,   109,   110,    32,    32,
      32,    32,  1185,    32,    32,   549,   109,   172,   120,   174,
     165,    32,    89,    71,   118,    84,    93,   172,   280,    32,
      82,  1015,   147,    85,   137,    74,   139,    78,    60,    60,
     373,   374,   576,    55,    92,    47,    47,    55,   170,    90,
     165,   186,   304,   147,     3,   114,    92,   175,  1231,    36,
     108,   118,   655,   170,    92,     0,   147,   117,   109,    92,
      92,    92,   141,   175,   141,   103,   179,    18,   117,   146,
     149,   125,   149,   105,   105,  1017,   153,    27,    47,    90,
     952,   147,   344,   103,   175,    72,   118,   118,   147,   202,
     203,   204,   146,   147,   207,   208,   209,    47,  1009,   120,
    1063,    51,   819,   573,  1015,   149,   209,   120,  1071,  1072,
    1062,   170,   227,   125,   146,   146,   489,   490,   147,   173,
    1058,    47,   171,   236,   170,    75,    77,   785,    78,  1018,
      80,   276,   165,   165,   146,   147,   170,   170,   170,   170,
      90,   170,   170,  1137,   103,    96,    22,   147,    27,   170,
     172,   120,   120,   165,   172,   147,    53,   170,   147,   109,
     165,    74,   113,     2,   165,    47,   171,   280,    47,   635,
     171,   165,    51,   173,    27,   764,    73,    89,   147,   405,
      55,    93,   171,   175,    27,   120,    83,    26,  1099,    28,
     836,   304,   454,   883,   120,    46,    75,   414,     2,    78,
       3,    80,   315,   316,   117,  1168,   175,   175,    90,   792,
    1173,    90,    88,  1160,    65,  1162,   147,   330,   331,   147,
      55,   147,    26,   173,    28,    78,  1137,   489,   490,    55,
     109,   344,    75,    84,    46,    78,   112,    90,  1110,   147,
      79,   147,   173,   171,    83,    84,   505,    90,   965,   175,
     140,   562,   109,    65,   418,   368,    55,   147,   171,   372,
     147,    90,   115,   114,  1179,   173,   109,   173,   107,   365,
     383,   384,    84,   112,   387,    79,   619,   147,  1016,    83,
      84,   171,   544,   147,   387,   398,   173,   400,   147,   434,
      11,   165,   147,   147,   173,   147,    55,   410,   172,   172,
     174,   171,   114,   107,   172,     8,   174,   410,   112,   173,
     147,    83,    84,   165,   173,  1084,  1085,   906,   173,   173,
    1235,  1236,  1237,  1238,  1239,  1240,   588,   638,   165,   442,
     443,   444,   445,   446,   447,    55,   449,   450,   451,   452,
     453,   147,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   621,
     949,  1051,   125,   172,    89,   478,   479,   173,    93,   952,
     172,   484,   174,   147,   487,   488,   489,   490,   491,   492,
      89,     3,   221,   147,    93,   172,   499,    89,   151,  1047,
     503,    93,    55,   172,   507,   174,   509,   147,    20,   173,
     170,    55,   172,   925,   517,   910,   519,   520,   147,   173,
     574,    33,   525,   526,   517,   165,    55,   221,   531,    50,
     478,   479,    53,   574,   100,   538,   484,   574,   541,   487,
     488,   544,    89,     3,   173,   125,    93,   147,   305,   306,
      62,   554,    73,   556,   283,    76,   147,   560,   147,   288,
     563,   564,    83,   796,   797,   147,   147,   800,   801,    31,
      55,   574,   152,   173,   154,   155,   579,   157,   158,  1159,
     147,  1161,   173,   165,   173,   588,   147,   147,   109,   283,
      55,   103,   173,   172,   288,    27,   599,   109,   165,   602,
      55,   604,   147,    31,   165,   165,   609,   610,   611,    89,
     613,   614,   764,    93,   147,    47,   147,   147,   621,    51,
     165,  1185,   147,   856,   857,    27,   172,    89,   174,   632,
     633,    93,   165,   785,   165,   165,   365,  1110,    55,   625,
     165,    55,   645,    75,    55,    47,    78,    55,    80,    51,
     140,    55,   655,   172,   602,   174,   604,   386,    90,   170,
     146,    89,   172,   172,   174,    93,   165,  1231,   397,   164,
     602,   365,   604,    75,   165,    55,    78,   109,    80,   141,
    1075,   165,   165,   165,   146,   141,   148,   149,    90,   165,
     146,   153,   386,   149,   165,   698,   165,   153,   160,   172,
     170,   165,   705,   397,   119,   434,   168,   109,    84,   170,
     173,     2,    31,   141,   142,   172,   144,   145,   146,   172,
     148,   149,   172,   172,   147,   153,   878,   879,   146,   165,
     172,   165,   160,     8,     3,    26,    65,    28,   166,   167,
     168,   172,   171,   170,   125,    90,    90,   125,   149,   173,
      68,   172,   125,   147,   906,   904,   759,   760,   761,   762,
     763,   764,   765,   766,   908,    47,   172,   911,   125,   125,
      89,    34,   777,   778,    93,    34,    65,   173,   125,   146,
     783,   173,   785,   786,   173,   788,    21,   175,    79,   792,
     519,   794,    83,    84,   149,   170,   525,   949,   175,   147,
     803,   804,   105,   172,   141,   165,   149,   536,   149,   538,
    1043,  1044,  1045,  1046,   149,   149,   107,   149,   149,   822,
     173,   112,   141,   142,   149,   519,   145,   146,   149,   148,
     149,   525,   149,   836,   153,   783,   149,   840,   149,   149,
     149,   160,   536,   149,   538,   165,   172,   166,   167,   168,
     165,   165,   175,   165,   908,    34,    34,   165,   912,   146,
      32,   147,   865,   866,   867,   165,   171,   921,  1131,  1132,
     171,   173,   170,   876,   822,    32,   173,   880,   607,   882,
     883,   884,   611,   886,    34,   888,   165,   175,   891,   892,
     925,   175,   165,    31,   172,  1047,   625,   146,   172,   175,
     165,   147,    34,   906,   175,   173,   173,   165,   165,   173,
     165,   173,   164,   607,   164,   147,   117,   611,  1047,  1063,
     620,   279,   397,   876,    25,    31,   117,  1071,    91,  1005,
     221,   625,   351,  1030,   560,   574,   408,   940,   925,   921,
     943,   976,   912,   571,   318,  1231,   949,  1077,   983,   952,
    1241,    89,  1234,   137,   707,    93,   903,   818,    -1,   557,
      -1,    -1,    -1,   966,   967,  1072,    -1,    -1,  1075,    -1,
      -1,    -1,    -1,    -1,   977,    -1,   979,    -1,    -1,  1131,
    1132,   984,    -1,    89,   987,    -1,    -1,    93,    -1,    -1,
      -1,    -1,   283,    -1,    -1,    -1,   999,   288,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,  1063,
     148,   149,   150,  1016,    -1,   153,    -1,  1020,    -1,    -1,
      -1,   159,   160,  1026,  1168,   163,    -1,    -1,   166,   167,
     168,    -1,  1035,  1026,  1037,   141,   142,  1040,    -1,   177,
     146,    -1,   148,   149,  1047,   774,    -1,   153,  1051,    -1,
      -1,    -1,    -1,    -1,   160,  1058,    -1,  1060,    -1,    -1,
     166,   167,   168,    -1,    -1,    -1,  1173,    -1,    -1,    -1,
      -1,    -1,     2,    -1,   365,    -1,  1079,   806,    -1,    -1,
     809,    -1,    -1,    -1,    -1,    31,  1089,  1090,    -1,  1092,
    1093,    -1,  1095,  1096,    -1,   386,    26,    -1,    28,    -1,
      -1,    -1,  1105,  1106,    -1,  1108,   397,  1110,    -1,  1112,
    1113,    -1,   806,  1116,  1168,   809,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   855,    -1,  1131,  1132,
      -1,    -1,   861,    -1,    -1,    -1,     2,    -1,    -1,   868,
    1143,    -1,  1145,    89,    -1,    -1,  1149,    93,    -1,    79,
      -1,    -1,    -1,    83,    84,  1158,  1159,  1160,  1161,  1162,
      26,   855,    28,    -1,    -1,  1200,  1201,   861,    -1,  1204,
    1205,    -1,    -1,    -1,   868,    -1,    -1,   107,    -1,    -1,
      -1,  1216,   112,  1218,    -1,    -1,  1189,  1190,  1191,  1192,
    1193,  1194,  1195,  1196,    -1,   141,    -1,    -1,    -1,    -1,
     146,    -1,   148,   149,    -1,    -1,    -1,   153,    -1,    -1,
      -1,    -1,    -1,    79,   160,    -1,    -1,    83,    84,    -1,
     166,   167,   168,    -1,  1227,    -1,    -1,    -1,   519,    -1,
    1233,    -1,    -1,    -1,   525,    -1,    -1,   966,   967,    -1,
      -1,   107,    -1,    -1,    -1,   536,   112,   538,   977,    -1,
     979,  1254,  1255,  1256,  1257,   984,  1242,    -1,   987,  1262,
    1263,    -1,    -1,    -1,    -1,    -1,    -1,   996,    -1,   998,
      -1,    -1,  1001,    -1,    -1,    -1,    -1,  1280,  1281,  1282,
    1283,  1284,  1285,   977,    -1,   979,    -1,    -1,    -1,    -1,
     984,   221,    -1,   987,    -1,    -1,  1025,    -1,    -1,    -1,
      -1,    -1,   996,    -1,   998,    -1,  1035,  1001,  1037,    -1,
      -1,    35,    36,    -1,    -1,    -1,   607,    -1,    -1,    -1,
     611,    -1,    -1,    -1,     2,    -1,    -1,    -1,    -1,    -1,
      -1,  1025,    -1,    -1,   625,    -1,    -1,    -1,    -1,    -1,
      -1,  1035,    -1,  1037,    -1,    -1,    -1,    -1,    26,    -1,
      28,    -1,    -1,   283,    -1,   221,    -1,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,  1105,    -1,    -1,    -1,
      -1,    -1,    -1,  1112,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1125,    -1,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,
    1139,  1105,    -1,    -1,    -1,    -1,    -1,   283,  1112,  1148,
      -1,    -1,   288,  1152,    -1,  1154,  1155,    -1,    -1,   107,
      -1,  1125,    89,    -1,   112,   365,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1148,    -1,   386,    -1,  1152,    -1,
    1154,  1155,    -1,    -1,    -1,    -1,    -1,   397,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1206,    -1,    -1,
      71,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,    -1,    -1,    -1,   153,    -1,    -1,   365,
      -1,    92,   159,   160,    -1,    -1,   163,    -1,    -1,   166,
     167,   168,  1206,  1242,    -1,   806,    -1,   108,   809,    -1,
     386,    -1,    -1,    -1,    -1,  1254,  1255,  1256,  1257,    -1,
      -1,   397,    -1,  1262,  1263,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   221,    -1,    -1,    -1,    -1,  1242,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,
    1254,  1255,  1256,  1257,   855,     1,    -1,    -1,  1262,  1263,
     861,    -1,    -1,    -1,    -1,    -1,    -1,   868,    -1,    -1,
      -1,    26,    -1,    28,    -1,    -1,    22,    -1,    -1,   519,
      26,    27,    -1,    -1,    -1,   525,    -1,    -1,    -1,    35,
      -1,    37,    38,    -1,    -1,   283,   536,    43,   538,    -1,
     288,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    73,    83,    84,
      76,    -1,    78,    -1,    80,   236,    82,    83,    -1,    85,
      -1,    -1,    88,   519,    90,    -1,    -1,    -1,    -1,   525,
     384,    -1,   107,   387,    -1,    -1,    -1,   112,    -1,    -1,
     536,    -1,   538,   109,    -1,    -1,   112,   607,    -1,   115,
      -1,   611,    -1,    -1,    -1,    -1,   977,   365,   979,   280,
      -1,    -1,    -1,   984,    -1,   625,   987,    -1,     3,    -1,
      -1,    -1,    -1,    -1,   140,   996,    -1,   998,   386,    -1,
    1001,    -1,    17,   304,    -1,    20,    -1,    -1,    -1,   397,
      25,   445,   446,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      35,    -1,    -1,    -1,  1025,   171,    -1,    -1,    -1,    -1,
      45,   607,    -1,    -1,  1035,   611,  1037,    52,    -1,    -1,
      -1,    -1,    -1,   344,    -1,    -1,    -1,    62,    -1,   625,
      -1,    -1,    -1,    -1,    -1,    70,    -1,   491,   492,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   221,    -1,    -1,   503,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    26,    27,    99,    -1,   520,    -1,   103,    -1,
      -1,    35,   526,    37,    38,   110,    -1,   531,    -1,    43,
      -1,   116,    -1,    -1,  1105,    -1,    50,    -1,    -1,    -1,
      -1,  1112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1125,    -1,    -1,    -1,   283,    -1,
      -1,   519,    76,   288,    78,    -1,    -1,   525,  1139,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    90,  1148,   536,    -1,
     538,  1152,    -1,  1154,  1155,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   806,     2,   112,   809,
      -1,   115,    -1,    -1,    -1,   609,    -1,   478,   479,    -1,
      -1,    -1,    -1,   484,    -1,    -1,   487,   488,   489,   490,
      -1,    26,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1206,    -1,    -1,    -1,    -1,
     365,    -1,    -1,    -1,    -1,   855,    -1,    -1,    -1,   607,
      -1,   861,    -1,   611,    -1,    -1,    -1,    -1,   868,    -1,
     806,   386,    -1,   809,    -1,    -1,    -1,   625,    -1,    -1,
      -1,  1242,   397,   544,    79,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,  1254,  1255,  1256,  1257,    -1,    -1,    -1,
      -1,  1262,  1263,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,    -1,    -1,    -1,   112,    -1,   855,
      -1,    -1,    -1,    -1,    -1,   861,    -1,   588,    -1,    -1,
      -1,    -1,   868,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   602,    -1,   604,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,    -1,
     621,    -1,    -1,    -1,    -1,    -1,   760,    -1,   762,    -1,
      -1,   765,    -1,    -1,    -1,    -1,    -1,   977,    -1,   979,
      -1,    -1,    -1,    -1,   984,    -1,    -1,   987,   169,    -1,
      -1,   172,    -1,   174,    -1,    -1,   996,    -1,   998,    -1,
      -1,  1001,    -1,    -1,   519,    -1,    -1,    -1,    -1,    -1,
     525,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,
      -1,   536,    -1,   538,    -1,  1025,   221,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1035,    -1,  1037,    -1,    -1,
      -1,   977,    26,   979,    28,    -1,    -1,    -1,   984,    -1,
      -1,   987,    -1,    -1,    -1,    -1,    -1,    -1,   806,    -1,
     996,   809,   998,    -1,    -1,  1001,    -1,    -1,    -1,    -1,
      -1,   865,   866,   867,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   283,  1025,
      -1,    -1,   607,   288,    -1,    79,   611,    -1,    -1,  1035,
      -1,  1037,    -1,   764,    -1,  1105,    -1,   855,    -1,    -1,
     625,    -1,  1112,   861,    -1,    -1,    -1,    -1,    -1,    -1,
     868,    -1,   783,   107,   785,  1125,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,  1139,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1148,    -1,
      -1,    -1,  1152,    -1,  1154,  1155,    -1,    -1,    -1,    -1,
      -1,   822,    73,    -1,    -1,    -1,    -1,    -1,    -1,  1105,
     365,    -1,   966,   967,    -1,    -1,  1112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1125,
      -1,   386,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   397,  1139,    -1,    -1,  1206,    -1,    -1,    -1,
      -1,    -1,  1148,    -1,    -1,    -1,  1152,    -1,  1154,  1155,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   977,
      -1,   979,    -1,    -1,    -1,    -1,   984,   221,    -1,   987,
      -1,    -1,  1242,    -1,    -1,   906,    -1,    -1,   996,    -1,
     998,    -1,    -1,  1001,  1254,  1255,  1256,  1257,    -1,    -1,
      -1,    -1,  1262,  1263,    -1,    -1,    -1,    -1,    -1,    -1,
    1206,    -1,    -1,    -1,    -1,    -1,    -1,  1025,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1035,   949,  1037,
      -1,   806,    -1,    -1,   809,    -1,    -1,    -1,    -1,   283,
      -1,    -1,    -1,    -1,   288,    -1,  1242,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1254,  1255,
    1256,  1257,    -1,    -1,   519,    -1,  1262,  1263,    -1,    -1,
     525,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     855,   536,    -1,   538,    -1,    -1,   861,    -1,    -1,     2,
      -1,    -1,    -1,   868,    -1,    -1,   267,  1105,   269,   270,
     271,   272,    -1,    -1,  1112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,    28,    -1,  1125,    -1,    -1,
     291,   365,   293,    -1,    -1,    -1,  1047,    -1,    -1,   300,
      -1,  1139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1148,    -1,   386,    -1,  1152,    -1,  1154,  1155,    -1,    -1,
      -1,    -1,   607,   397,    -1,    -1,   611,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,   340,
     625,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   353,   354,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,  1206,   112,
      -1,    -1,   977,    22,   979,    -1,    -1,    26,    27,   984,
    1131,  1132,   987,    -1,    -1,    -1,    35,    -1,    37,    38,
      -1,   996,    -1,   998,    43,    -1,  1001,    -1,    -1,    -1,
      -1,    50,    -1,    -1,  1242,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1254,  1255,  1256,  1257,
    1025,    -1,    -1,    -1,  1262,  1263,    -1,    76,    -1,    78,
    1035,    80,  1037,    82,    -1,    -1,    85,    -1,    -1,    88,
      -1,    90,    -1,    -1,    -1,   519,    -1,    -1,    -1,    -1,
      -1,   525,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,   536,   112,   538,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   221,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1105,    -1,    -1,   504,    -1,    -1,    -1,  1112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
    1125,   806,   171,   524,   809,    -1,    -1,   528,    -1,   530,
      -1,    -1,   533,   607,  1139,    -1,    -1,   611,    -1,    -1,
     283,    -1,    -1,  1148,    -1,   288,    -1,  1152,    -1,  1154,
    1155,   625,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     855,    -1,     3,    -1,    -1,    -1,   861,    -1,    -1,    -1,
      -1,    -1,    -1,   868,    -1,    -1,    17,    18,    -1,    20,
      -1,    -1,    49,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,  1206,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    73,   618,    49,    -1,
      -1,    52,   365,    -1,    -1,    -1,    83,    84,    -1,    -1,
      -1,    62,    -1,    -1,    -1,    -1,    -1,  1242,    69,    70,
      -1,    -1,    -1,   386,    -1,    -1,    77,    -1,    -1,  1254,
    1255,  1256,  1257,    -1,   397,    -1,    87,  1262,  1263,    -1,
     117,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,   977,    -1,   979,    -1,    -1,    -1,    -1,   984,
      -1,    -1,   987,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   996,    -1,   998,    -1,    -1,  1001,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   806,    -1,    -1,   809,    -1,    -1,    -1,    -1,
    1025,   172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1035,    -1,  1037,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,    -1,    -1,    -1,    -1,    -1,   519,    -1,    -1,    -1,
      -1,   855,   525,    -1,    -1,    -1,    -1,   861,    -1,   790,
     791,    30,    -1,   536,   868,   796,   797,    -1,    -1,   800,
     801,    -1,    -1,    -1,    -1,    44,    45,    46,    -1,    -1,
     267,   268,   269,   270,   271,   272,    -1,    -1,   275,   276,
    1105,    -1,    -1,    -1,    -1,   282,    -1,  1112,    -1,    -1,
      -1,    -1,    -1,    -1,   291,     3,   293,    -1,    -1,    -1,
    1125,    -1,    -1,   300,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,  1139,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,  1148,   607,    33,    -1,  1152,   611,  1154,
    1155,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,   625,   340,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,   353,   354,    -1,    -1,
      -1,    69,    70,   977,    -1,   979,    -1,    -1,    -1,    77,
     984,    -1,    -1,   987,    -1,    -1,   155,    -1,   157,    87,
      -1,  1206,   996,    -1,   998,    -1,    -1,  1001,    96,    97,
     169,    99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,  1025,    -1,    -1,    -1,    -1,    -1,  1242,    -1,    -1,
      -1,  1035,    -1,  1037,    -1,    -1,   205,   968,   969,  1254,
    1255,  1256,  1257,   974,    -1,    -1,    -1,  1262,  1263,    -1,
      -1,    -1,    -1,    -1,    -1,   986,    -1,    -1,   989,    -1,
     991,    -1,   993,    -1,    -1,    -1,    22,    -1,    24,    -1,
      26,    27,    -1,    -1,   172,    -1,    32,    -1,    -1,    35,
      -1,    37,    38,    -1,    40,    41,    42,    43,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    -1,   265,    -1,    -1,    -1,
      -1,  1105,    -1,    -1,   273,   274,    -1,    -1,  1112,    65,
    1041,  1042,  1043,  1044,    -1,   284,    -1,   504,    -1,    -1,
      76,  1125,    78,    -1,    -1,    -1,    82,    -1,    84,    85,
      -1,    -1,    88,   806,    90,  1139,   809,   524,    -1,    -1,
      -1,   528,    12,   530,  1148,    -1,   533,    -1,  1152,    19,
    1154,  1155,    -1,   109,    -1,   324,   112,    -1,    -1,   115,
      -1,    31,    32,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,   855,    -1,    -1,    -1,    -1,    -1,   861,    -1,
      -1,    -1,    -1,    -1,    -1,   868,    -1,    -1,    -1,    -1,
      -1,    -1,  1206,    -1,    -1,    -1,    -1,    -1,    -1,   165,
    1141,    -1,   381,    -1,   170,    -1,    -1,    -1,   174,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
     617,   618,    -1,    -1,    -1,    -1,    -1,    -1,  1242,    -1,
     409,    -1,    -1,    -1,    -1,   414,    -1,    -1,    -1,    -1,
    1254,  1255,  1256,  1257,   124,    -1,    -1,    -1,  1262,  1263,
      -1,    -1,    -1,    -1,    -1,   434,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
     170,    -1,    -1,    -1,   977,    -1,   979,   177,    -1,    -1,
      -1,   984,    -1,    -1,   987,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   996,     3,   998,    -1,    -1,  1001,   498,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,  1025,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,  1035,    -1,  1037,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,   545,    -1,    -1,    -1,
     549,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,   562,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,   571,   790,   791,    -1,    -1,   576,    87,   796,
     797,     3,    -1,   800,   801,    -1,    -1,    96,    97,    -1,
      99,    -1,   101,   810,   103,    17,    18,   106,    20,    -1,
      -1,   110,    -1,    25,   113,    -1,    -1,   116,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,  1125,    45,    -1,    -1,    -1,    49,    -1,   846,
      52,    -1,   631,    -1,    -1,    -1,  1139,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,  1148,    -1,    69,    70,  1152,
      12,  1154,  1155,    -1,    -1,    77,    -1,    19,    -1,    -1,
      -1,   170,    -1,   172,    -1,    87,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    -1,    -1,   106,    47,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1206,    -1,    -1,    -1,    -1,   707,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1242,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     172,   968,   969,    -1,    -1,    -1,    -1,   974,   120,   976,
      -1,    -1,   124,    -1,    -1,    -1,   983,    -1,    -1,   986,
      -1,    -1,   989,    -1,   991,    -1,   993,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,  1006,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,   170,    -1,
      -1,    -1,   811,    -1,    -1,   177,  1033,    -1,    -1,   818,
      -1,    -1,    -1,    -1,  1041,  1042,  1043,  1044,    -1,   828,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   846,    -1,     1,
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,   903,    -1,    58,    59,    -1,    -1,
      62,   910,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,  1141,    77,   925,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1200,  1201,    -1,    -1,  1204,  1205,   141,
     142,   143,   144,   145,    -1,    -1,    -1,  1214,   150,  1216,
      -1,  1218,    -1,    -1,    -1,    -1,  1005,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,    -1,    -1,    -1,     0,
       1,  1030,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,  1075,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    18,    19,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,    -1,   170,   171,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
     171,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
     171,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    12,    -1,    14,    -1,    -1,    17,    18,    19,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,    -1,
     141,   142,   143,   144,   145,   146,    -1,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,   166,   167,   168,   169,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      -1,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    30,    -1,    32,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    42,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    61,    62,    63,    64,    -1,
      66,    67,    -1,    69,    70,    71,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,   119,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
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
     106,   107,   108,    -1,   110,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    47,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
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
     106,   107,   108,    -1,   110,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,   173,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,   173,   174,    -1,    -1,   177,     3,     4,     5,
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
     106,   107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,   173,   174,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,   107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,   122,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
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
     116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,   175,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    -1,
      -1,    -1,    93,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
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
     116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
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
     106,    -1,    -1,    -1,   110,    -1,   112,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,   175,    -1,   177,     3,     4,     5,
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
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,   175,    -1,   177,     3,     4,     5,
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
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,   175,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
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
     106,    -1,    -1,    -1,   110,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
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
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
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
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
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
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
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
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      31,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,   124,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     4,     5,     6,     7,     8,     9,    10,    -1,
     141,   142,    14,   144,   145,   146,    18,   148,   149,   150,
      22,   152,   153,   154,   155,    27,   157,   158,   159,   160,
     161,   162,   163,    35,    -1,   166,   167,   168,    40,    41,
      -1,    -1,    44,    -1,    -1,    47,   177,    -1,    -1,    51,
      -1,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      -1,    -1,    64,    -1,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    31,    77,    78,    79,    80,    81,
      -1,    -1,    -1,    -1,    86,    -1,    88,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    98,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,   124,   170,    -1,
     172,   173,   174,    -1,    -1,   177,     4,     5,     6,     7,
       8,     9,    10,    -1,   141,   142,    14,   144,   145,   146,
      18,   148,   149,   150,    22,   152,   153,   154,   155,    27,
     157,   158,   159,   160,   161,    -1,   163,    35,    -1,   166,
     167,   168,    40,    41,    -1,    -1,    44,    -1,    -1,    47,
     177,    -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    -1,    -1,    64,    -1,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,
      78,    79,    80,    81,    -1,    -1,    -1,    -1,    86,    -1,
      88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      12,    -1,   150,    -1,    -1,    17,    18,    19,    20,    -1,
      -1,   159,    -1,    25,   162,   163,   164,    -1,    -1,    -1,
     168,    33,   170,    35,   172,    -1,   174,    39,    -1,   177,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,   143,   144,   145,   146,    -1,   148,    -1,    -1,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,   163,    -1,    -1,   166,   167,   168,   169,    -1,    -1,
     172,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
      -1,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,   143,    -1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,   164,    22,    -1,    -1,    25,    -1,
     170,    -1,   172,    -1,   174,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,     3,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    12,    -1,    86,
      87,    88,    -1,    -1,    19,    20,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    33,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   172,    -1,   174,   103,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,   143,   144,
     145,   146,    -1,   148,    -1,    -1,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,     3,   163,    -1,
      -1,   166,   167,   168,   169,   170,    12,    -1,    -1,    -1,
      -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      96,    97,    -1,    99,    19,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    31,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,    -1,   138,   139,    -1,   141,   142,   143,   144,   145,
     146,    -1,   148,    -1,    -1,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,   163,    -1,    -1,
     166,   167,   168,   169,    89,    -1,    -1,    12,    93,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    89,   170,    -1,    12,    93,    -1,
      -1,    -1,   177,    -1,    19,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    89,   170,    -1,    12,    93,    -1,
      -1,    -1,   177,    -1,    19,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    89,   170,    -1,    12,    93,    -1,
      -1,    -1,   177,    -1,    19,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    89,   170,    -1,    12,    93,    -1,
      -1,    -1,   177,    -1,    19,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    89,   170,    -1,    12,    93,    -1,
      -1,    -1,   177,    -1,    19,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    89,   170,    -1,    12,    93,    -1,
      -1,    -1,   177,    -1,    19,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    89,   170,    -1,    12,    93,    -1,
      -1,    -1,   177,    -1,    19,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    89,   170,    -1,    -1,    93,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    31,    32,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    47,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    89,    -1,    -1,    -1,    93,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      31,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,   177,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,    -1,    -1,    89,    -1,
      -1,    12,    93,    -1,   177,    -1,    -1,    -1,    19,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      31,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,    -1,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,   177,    -1,    19,    -1,
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
     158,   159,   160,   161,   162,   163,    -1,    22,   166,   167,
     168,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      35,    -1,    37,    38,    -1,    22,    -1,    -1,    43,    26,
      27,    -1,    -1,    -1,    -1,    50,    -1,    -1,    35,    -1,
      37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    31,    78,    -1,    80,    -1,    82,    -1,    -1,
      85,    -1,    -1,    88,    -1,    90,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    85,    -1,
      -1,    88,    -1,    90,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      89,    -1,    -1,    -1,    93,   140,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
      -1,    -1,   141,   142,   171,   144,   145,   146,    -1,   148,
     149,   150,    -1,    -1,   153,   154,   155,    -1,   157,   158,
     159,   160,    -1,    -1,   163,    -1,    22,   166,   167,   168,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,   177,    35,
      -1,    37,    38,    -1,    22,    -1,    -1,    43,    26,    27,
      -1,    -1,    -1,    -1,    50,    -1,    -1,    35,    -1,    37,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    85,
      -1,    -1,    88,    -1,    90,    -1,    -1,    -1,    76,    -1,
      78,    -1,    -1,    -1,    82,    -1,    -1,    85,    -1,     1,
      88,     3,    90,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,
      -1,   109,    -1,    25,   112,    -1,    -1,   115,    -1,    -1,
      -1,    33,    -1,    -1,   140,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,   140,    -1,    -1,    -1,     3,    -1,    -1,   165,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      17,    18,    -1,    20,    -1,    77,    -1,   165,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    96,    97,    -1,    99,    45,   101,
      -1,   103,    49,    -1,   106,    52,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,     3,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
      87,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,    96,
      97,    -1,    99,    25,   101,    27,   103,    -1,    -1,   106,
      -1,    33,    -1,   110,    -1,    -1,   113,    39,    -1,   116,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,
      62,    -1,    -1,   140,    -1,    -1,    -1,    69,    70,    -1,
      17,    18,    -1,    20,    -1,    77,    78,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    33,    -1,    90,    -1,
      -1,    -1,    39,    -1,    96,    97,    -1,    99,    45,   101,
      -1,   103,    49,    -1,   106,    52,    -1,    -1,   110,    -1,
      -1,   113,    -1,   115,   116,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,    96,
      97,    -1,    99,    25,   101,    -1,   103,    -1,    -1,   106,
      -1,    33,    -1,   110,   111,    -1,   113,    39,    -1,   116,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      17,    18,    -1,    20,    -1,    77,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    96,    97,    -1,    99,    45,   101,
      -1,   103,    49,    -1,   106,    52,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,    96,
      97,    -1,    99,    25,   101,    -1,   103,    -1,    -1,   106,
      -1,    33,    -1,   110,    -1,    -1,   113,    39,    -1,   116,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116
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
      53,    73,    83,   304,    27,    75,    78,    90,   109,   305,
      27,    78,    90,   109,   303,   218,   316,   317,   322,   164,
     165,   362,    12,    19,    31,    89,    93,   124,   141,   142,
     144,   145,   146,   148,   149,   150,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   166,   167,
     168,   177,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   169,   291,   172,
     174,    89,    93,   362,   362,   362,   218,   335,    55,   172,
     186,   165,   205,    47,   348,   319,   165,   147,   165,   119,
     219,   338,   339,   340,   364,   170,   218,   172,   223,    32,
      47,   223,   120,   223,   351,    32,    47,   223,   351,   223,
     351,    47,   223,   351,   218,   218,   102,   205,   102,   125,
     205,   291,   202,   338,   172,   172,   205,   186,   111,   172,
     218,   324,   325,     1,   146,   331,    47,   147,   186,   223,
     147,   223,    13,   172,   172,   223,   338,   228,   228,    47,
      90,   301,   302,   173,   147,   111,   172,   218,   299,   146,
     165,   362,   362,   123,   281,   165,   170,   223,   172,   338,
     165,   249,   249,   223,   223,   165,   171,   171,   184,   147,
     171,   362,   147,   173,   147,   173,   362,   175,   351,    47,
     147,   175,   351,   123,   147,   175,     8,   362,   362,   335,
       3,   172,   196,     1,   171,   206,   212,   213,   362,   208,
     362,    65,    36,    72,   165,   260,   262,   109,   234,   284,
     218,   335,   170,   171,   269,   125,   147,   171,   173,   296,
     298,   300,   305,    90,     1,   146,   329,   330,    90,     1,
       3,    12,    17,    19,    20,    25,    45,    52,    54,    62,
      70,    87,    99,   103,   110,   116,   141,   142,   143,   144,
     145,   146,   148,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   163,   166,   167,   168,   169,   172,   218,
     288,   289,   290,   291,   341,   125,   318,   147,   165,   165,
     362,   362,   362,   238,   362,   238,   362,   362,   362,   362,
     362,   362,   362,     3,    20,    33,    62,   103,   109,   219,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,    68,   364,   364,
     364,   364,   364,   338,   338,   238,   362,   238,   362,    32,
      47,    32,    47,   102,   172,    47,   175,   218,    27,    47,
      90,   115,   350,   353,   354,   362,   378,   238,   362,   223,
     322,   362,   362,   125,   173,   147,    47,   335,    44,   362,
     238,   362,   172,   223,    44,   362,   238,   362,   223,   223,
     238,   362,   223,   125,   125,   186,    34,   186,   362,    34,
     362,    65,   173,   339,   218,   242,   243,   325,   147,   173,
     222,   362,   125,   326,   362,   323,   256,   218,   319,   362,
     362,   173,   299,   305,   277,   325,   146,   333,   334,   332,
     282,   204,     1,   254,   340,   173,    21,   250,   171,   173,
     173,   336,   173,   336,   186,   175,   238,   362,   175,   186,
     362,   175,   362,   175,   362,    47,    47,    47,   175,   195,
     173,   197,   198,   340,   171,   171,   147,   165,    13,   149,
     147,   165,    13,    36,    72,   218,   141,   142,   143,   144,
     145,   159,   163,   168,   207,   290,   291,   292,   362,   207,
     209,   262,   170,   235,   319,   165,   172,     1,   263,   362,
     267,   268,   147,   173,   331,     1,   222,   327,   328,   105,
     306,   172,   294,   362,   141,   149,   294,   294,   362,   317,
     172,   174,   165,   165,   165,   165,   165,   165,   173,   175,
      44,   238,   362,    44,   238,   362,   339,   238,   362,   172,
      47,    90,   147,   173,    17,    20,    25,    45,    52,    62,
      70,    87,    99,   110,   116,   341,    89,    89,   348,   348,
     165,   165,   364,   340,   362,   173,   362,    32,   223,    32,
     223,   352,   353,   362,    32,   223,   351,    32,   223,   351,
     223,   351,   223,   351,   362,   362,    34,   186,    34,    34,
     186,   102,   205,   218,   173,   147,   173,   173,   324,   331,
      68,   364,   218,   173,   173,   334,   146,   278,   173,   332,
     151,   293,   326,   362,   171,    74,   117,   171,   255,   173,
     172,   205,   218,   251,   186,   175,   351,   175,   351,   186,
     123,   238,   362,   238,   362,   238,   362,   147,   173,   212,
     219,   170,   292,   362,   111,   362,   207,   209,   147,   165,
      13,   165,   170,   263,   316,   322,   339,   171,    82,    85,
     171,   185,   192,   225,   269,   192,   298,   306,    60,   118,
     310,   296,   297,   173,   289,   291,   173,   175,   362,    32,
      32,   362,    32,    32,   173,   175,   175,   339,   353,   331,
     341,   341,   223,   223,   223,   102,    44,   362,    44,   362,
     147,   173,   102,    44,   362,   223,    44,   362,   223,   223,
     223,   186,   186,   362,   186,    34,   165,   165,   243,   205,
     326,   172,   172,   293,   326,   327,   306,   334,   362,    32,
     186,   335,   251,   146,   205,    44,   186,   362,   175,    44,
     186,   362,   175,   362,   175,   175,   198,   207,    13,    36,
      72,    36,    72,   165,   165,   292,   362,   362,   263,   171,
     165,   165,   173,   192,   225,   225,   269,   310,     3,    92,
     103,   311,   312,   313,   362,   285,   173,   294,   294,   102,
      44,    44,   102,    44,    44,    44,    44,   173,   326,   362,
     362,   362,   353,   362,   362,   362,    34,   186,   338,   338,
     293,   326,   186,   223,   173,   362,   362,   186,   362,   186,
     171,   111,   362,   207,   209,   207,   209,    13,   171,   165,
     225,   272,   313,   118,   147,   125,   152,   154,   155,   157,
     158,    60,    32,   165,   204,   224,   307,   308,   362,   362,
     362,   362,   362,   362,   102,   102,   102,   102,   186,   173,
     173,   205,   102,   102,   165,   165,   165,   165,   362,   308,
     362,   312,   313,   313,   313,   313,   313,   313,   311,   184,
     102,   102,   102,   102,   102,   102,   362,   362,   362,   362,
     164,   164,   362,   362
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
     305,   305,   305,   305,   306,   306,   307,   307,   308,   308,
     308,   309,   310,   310,   310,   310,   310,   311,   311,   312,
     312,   312,   312,   312,   312,   312,   313,   313,   314,   315,
     315,   316,   316,   317,   318,   318,   319,   319,   319,   319,
     319,   321,   320,   320,   322,   322,   323,   323,   324,   324,
     324,   325,   325,   325,   326,   326,   326,   327,   328,   328,
     328,   329,   329,   330,   330,   331,   331,   331,   331,   332,
     332,   333,   334,   334,   335,   335,   336,   336,   337,   337,
     338,   338,   339,   339,   340,   340,   341,   341,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   344,   344,   344,   344,   344,   344,   344,   345,
     346,   347,   347,   347,   347,   347,   347,   347,   348,   348,
     349,   350,   350,   351,   352,   352,   353,   353,   353,   354,
     354,   354,   354,   354,   354,   355,   355,   355,   355,   355,
     356,   356,   356,   356,   356,   357,   358,   358,   358,   358,
     358,   358,   359,   360,   361,   361,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   363,   363,   364,   364,   364,   365,   365,
     365,   365,   366,   366,   366,   366,   366,   367,   367,   367,
     368,   368,   368,   368,   368,   368,   369,   369,   369,   369,
     370,   370,   371,   371,   372,   372,   372,   372,   372,   372,
     372,   372,   372,   372,   372,   372,   372,   373,   373,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   375,   375,   375,   375,   375,   375,   375,   376,
     376,   376,   376,   377,   377,   377,   377,   378,   378,   378,
     378,   378,   378,   378
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
       1,     2,     1,     1,     1,     1,     1,     0,     1,     1,
       2,     1,     1,     1,     0,     1,     1,     1,     1,     2,
       1,     1,     0,     2,     2,     4,     4,     1,     3,     3,
       3,     3,     3,     3,     3,     2,     1,     1,     3,     1,
       2,     1,     3,     2,     0,     2,     1,     2,     1,     1,
       1,     0,     5,     3,     1,     3,     3,     5,     1,     1,
       3,     1,     2,     3,     0,     2,     2,     1,     1,     1,
       1,     2,     1,     0,     1,     0,     2,     2,     1,     1,
       1,     2,     0,     1,     1,     3,     1,     1,     3,     3,
       0,     1,     1,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     4,     4,     4,     5,     2,     2,
       1,     2,     1,     2,     1,     2,     1,     2,     1,     1,
       6,     6,     4,     9,     9,     7,     6,     6,     4,     9,
       9,     7,     2,     3,     4,     6,     6,     9,     9,     6,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       4,     1,     3,     4,     1,     3,     4,     3,     3,     1,
       1,     2,     1,     2,     1,     1,     2,     2,     2,     2,
       2,     8,     8,     9,     9,     4,     3,     3,     2,     2,
       2,     1,     3,     4,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     4,     4,     4,
       3,     3,     3,     3,     5,     5,     3,     4,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     4,     3,     4,     3,     4,     3,     5,     3,
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
#line 634 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6193 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 638 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6199 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 639 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6205 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 646 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6213 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 654 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6221 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 658 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6230 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 668 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6238 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 673 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6246 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 680 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6252 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 681 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6258 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6264 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6270 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6276 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6282 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6288 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6294 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6300 "bison-chpl-lib.cpp"
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
#line 6317 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 702 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6323 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 703 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6329 "bison-chpl-lib.cpp"
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
#line 6342 "bison-chpl-lib.cpp"
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
#line 6355 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 723 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6363 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 727 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6374 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 734 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6383 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 741 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6389 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 742 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6395 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 743 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6401 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 744 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6407 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 745 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6413 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 746 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6419 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 747 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6425 "bison-chpl-lib.cpp"
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
#line 6443 "bison-chpl-lib.cpp"
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
#line 6462 "bison-chpl-lib.cpp"
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
#line 6474 "bison-chpl-lib.cpp"
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
#line 6490 "bison-chpl-lib.cpp"
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
#line 6505 "bison-chpl-lib.cpp"
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
#line 6521 "bison-chpl-lib.cpp"
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
#line 6537 "bison-chpl-lib.cpp"
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
#line 6552 "bison-chpl-lib.cpp"
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
#line 6569 "bison-chpl-lib.cpp"
    break;

  case 53: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 880 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), true, (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6577 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace  */
#line 884 "chpl.ypp"
 {
  // this allows us to distinguish between @attribute() and @attribute
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, (yyvsp[0].exprList), lst);
 }
#line 6588 "bison-chpl-lib.cpp"
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
#line 6601 "bison-chpl-lib.cpp"
    break;

  case 56: /* attribute_decl_begin: TATMARK TIDENT  */
#line 903 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6610 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: %empty  */
#line 910 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6618 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 914 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6629 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP TRP  */
#line 924 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6635 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 926 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6643 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 933 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6652 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 938 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6662 "bison-chpl-lib.cpp"
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
#line 6683 "bison-chpl-lib.cpp"
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
#line 6702 "bison-chpl-lib.cpp"
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
#line 6721 "bison-chpl-lib.cpp"
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
#line 6738 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1016 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6744 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1017 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6751 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1019 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6758 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1024 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6764 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1025 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6771 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1031 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6781 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1049 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6789 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1053 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6797 "bison-chpl-lib.cpp"
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
#line 6809 "bison-chpl-lib.cpp"
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
#line 6828 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1086 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6834 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1087 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6840 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1092 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6848 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1096 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6856 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1100 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6865 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1105 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6873 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1109 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6881 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1113 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6890 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1123 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6899 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1128 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6909 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1134 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6920 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1141 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6929 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1146 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6940 "bison-chpl-lib.cpp"
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
#line 6952 "bison-chpl-lib.cpp"
    break;

  case 92: /* opt_only_ls: %empty  */
#line 1163 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6958 "bison-chpl-lib.cpp"
    break;

  case 93: /* opt_only_ls: renames_ls  */
#line 1164 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6964 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1169 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6973 "bison-chpl-lib.cpp"
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
#line 6985 "bison-chpl-lib.cpp"
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
#line 6998 "bison-chpl-lib.cpp"
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
#line 7012 "bison-chpl-lib.cpp"
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
#line 7024 "bison-chpl-lib.cpp"
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
#line 7037 "bison-chpl-lib.cpp"
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
#line 7051 "bison-chpl-lib.cpp"
    break;

  case 101: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1231 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7060 "bison-chpl-lib.cpp"
    break;

  case 102: /* import_expr: expr  */
#line 1239 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7068 "bison-chpl-lib.cpp"
    break;

  case 103: /* import_expr: expr TDOT all_op_name  */
#line 1243 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7077 "bison-chpl-lib.cpp"
    break;

  case 104: /* import_expr: expr TAS ident_use  */
#line 1248 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7087 "bison-chpl-lib.cpp"
    break;

  case 105: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1254 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7097 "bison-chpl-lib.cpp"
    break;

  case 106: /* import_ls: import_expr  */
#line 1262 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7103 "bison-chpl-lib.cpp"
    break;

  case 107: /* import_ls: import_ls TCOMMA import_expr  */
#line 1263 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7109 "bison-chpl-lib.cpp"
    break;

  case 108: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1268 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7119 "bison-chpl-lib.cpp"
    break;

  case 109: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1277 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7127 "bison-chpl-lib.cpp"
    break;

  case 110: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1281 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7135 "bison-chpl-lib.cpp"
    break;

  case 111: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1285 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7143 "bison-chpl-lib.cpp"
    break;

  case 112: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1289 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7151 "bison-chpl-lib.cpp"
    break;

  case 113: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1293 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7159 "bison-chpl-lib.cpp"
    break;

  case 114: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1297 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7167 "bison-chpl-lib.cpp"
    break;

  case 115: /* opt_label_ident: %empty  */
#line 1305 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7173 "bison-chpl-lib.cpp"
    break;

  case 116: /* opt_label_ident: TIDENT  */
#line 1306 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7179 "bison-chpl-lib.cpp"
    break;

  case 124: /* ident_use: TIDENT  */
#line 1331 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7185 "bison-chpl-lib.cpp"
    break;

  case 125: /* ident_use: TTHIS  */
#line 1332 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7191 "bison-chpl-lib.cpp"
    break;

  case 145: /* scalar_type: TBOOL  */
#line 1359 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7197 "bison-chpl-lib.cpp"
    break;

  case 146: /* scalar_type: TENUM  */
#line 1360 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7203 "bison-chpl-lib.cpp"
    break;

  case 147: /* scalar_type: TINT  */
#line 1361 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7209 "bison-chpl-lib.cpp"
    break;

  case 148: /* scalar_type: TUINT  */
#line 1362 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7215 "bison-chpl-lib.cpp"
    break;

  case 149: /* scalar_type: TREAL  */
#line 1363 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7221 "bison-chpl-lib.cpp"
    break;

  case 150: /* scalar_type: TIMAG  */
#line 1364 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7227 "bison-chpl-lib.cpp"
    break;

  case 151: /* scalar_type: TCOMPLEX  */
#line 1365 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7233 "bison-chpl-lib.cpp"
    break;

  case 152: /* scalar_type: TBYTES  */
#line 1366 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7239 "bison-chpl-lib.cpp"
    break;

  case 153: /* scalar_type: TSTRING  */
#line 1367 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7245 "bison-chpl-lib.cpp"
    break;

  case 154: /* scalar_type: TLOCALE  */
#line 1368 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7251 "bison-chpl-lib.cpp"
    break;

  case 155: /* scalar_type: TNOTHING  */
#line 1369 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7257 "bison-chpl-lib.cpp"
    break;

  case 156: /* scalar_type: TVOID  */
#line 1370 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7263 "bison-chpl-lib.cpp"
    break;

  case 161: /* do_stmt: TDO stmt  */
#line 1384 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7269 "bison-chpl-lib.cpp"
    break;

  case 162: /* do_stmt: block_stmt  */
#line 1385 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7275 "bison-chpl-lib.cpp"
    break;

  case 163: /* return_stmt: TRETURN TSEMI  */
#line 1390 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7286 "bison-chpl-lib.cpp"
    break;

  case 164: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1397 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7297 "bison-chpl-lib.cpp"
    break;

  case 165: /* class_level_stmt: TSEMI  */
#line 1407 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7305 "bison-chpl-lib.cpp"
    break;

  case 166: /* class_level_stmt: inner_class_level_stmt  */
#line 1411 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7315 "bison-chpl-lib.cpp"
    break;

  case 167: /* $@1: %empty  */
#line 1416 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7322 "bison-chpl-lib.cpp"
    break;

  case 168: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1418 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7331 "bison-chpl-lib.cpp"
    break;

  case 169: /* $@2: %empty  */
#line 1422 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7338 "bison-chpl-lib.cpp"
    break;

  case 170: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1424 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7347 "bison-chpl-lib.cpp"
    break;

  case 178: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1442 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7356 "bison-chpl-lib.cpp"
    break;

  case 179: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1447 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7365 "bison-chpl-lib.cpp"
    break;

  case 180: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1452 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7374 "bison-chpl-lib.cpp"
    break;

  case 181: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1457 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7382 "bison-chpl-lib.cpp"
    break;

  case 182: /* forwarding_decl_start: TFORWARDING  */
#line 1464 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7391 "bison-chpl-lib.cpp"
    break;

  case 183: /* extern_or_export: TEXTERN  */
#line 1471 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7397 "bison-chpl-lib.cpp"
    break;

  case 184: /* extern_or_export: TEXPORT  */
#line 1472 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7403 "bison-chpl-lib.cpp"
    break;

  case 185: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1477 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7413 "bison-chpl-lib.cpp"
    break;

  case 186: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1487 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7422 "bison-chpl-lib.cpp"
    break;

  case 187: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1493 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7433 "bison-chpl-lib.cpp"
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
#line 7455 "bison-chpl-lib.cpp"
    break;

  case 189: /* $@3: %empty  */
#line 1518 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7463 "bison-chpl-lib.cpp"
    break;

  case 190: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1522 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7472 "bison-chpl-lib.cpp"
    break;

  case 191: /* $@4: %empty  */
#line 1527 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7481 "bison-chpl-lib.cpp"
    break;

  case 192: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1532 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7490 "bison-chpl-lib.cpp"
    break;

  case 193: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1540 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7498 "bison-chpl-lib.cpp"
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
#line 7520 "bison-chpl-lib.cpp"
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
#line 7537 "bison-chpl-lib.cpp"
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
#line 7554 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1591 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7562 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1595 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7570 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1599 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7578 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1603 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7586 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1607 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7594 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFOR expr do_stmt  */
#line 1611 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7602 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1615 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7610 "bison-chpl-lib.cpp"
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
#line 7634 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1639 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7642 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1643 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7650 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7658 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1651 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7666 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFORALL expr do_stmt  */
#line 1655 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7674 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1659 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7682 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1663 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7690 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1667 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7698 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1671 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7706 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1675 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7714 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1679 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7722 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1683 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7730 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1687 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7738 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1691 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7746 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1695 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7754 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1699 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7762 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1703 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7770 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1707 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7778 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1711 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7786 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1715 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7794 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1719 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7802 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1723 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7810 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1727 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7819 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1732 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7828 "bison-chpl-lib.cpp"
    break;

  case 229: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1740 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7838 "bison-chpl-lib.cpp"
    break;

  case 230: /* if_stmt: TIF expr TTHEN stmt  */
#line 1749 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7846 "bison-chpl-lib.cpp"
    break;

  case 231: /* if_stmt: TIF expr block_stmt  */
#line 1753 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7854 "bison-chpl-lib.cpp"
    break;

  case 232: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1757 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7862 "bison-chpl-lib.cpp"
    break;

  case 233: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1761 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7870 "bison-chpl-lib.cpp"
    break;

  case 234: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1765 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7878 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF ifvar block_stmt  */
#line 1769 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7886 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1773 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7894 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1777 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7902 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1781 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7911 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1786 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7920 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1791 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7929 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1796 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7938 "bison-chpl-lib.cpp"
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
#line 7957 "bison-chpl-lib.cpp"
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
#line 7976 "bison-chpl-lib.cpp"
    break;

  case 244: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1837 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7984 "bison-chpl-lib.cpp"
    break;

  case 245: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1841 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7992 "bison-chpl-lib.cpp"
    break;

  case 246: /* ifc_formal_ls: ifc_formal  */
#line 1847 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7998 "bison-chpl-lib.cpp"
    break;

  case 247: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1848 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8004 "bison-chpl-lib.cpp"
    break;

  case 248: /* ifc_formal: ident_def  */
#line 1853 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8010 "bison-chpl-lib.cpp"
    break;

  case 261: /* implements_type_ident: implements_type_error_ident  */
#line 1871 "chpl.ypp"
  {
    context->syntax((yyloc), "type '%s' cannot implement an interface.", (yyvsp[0].uniqueStr).c_str());
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8019 "bison-chpl-lib.cpp"
    break;

  case 268: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1897 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8027 "bison-chpl-lib.cpp"
    break;

  case 269: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1901 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8035 "bison-chpl-lib.cpp"
    break;

  case 270: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1905 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8043 "bison-chpl-lib.cpp"
    break;

  case 271: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1912 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8051 "bison-chpl-lib.cpp"
    break;

  case 272: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1916 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8059 "bison-chpl-lib.cpp"
    break;

  case 273: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1920 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8067 "bison-chpl-lib.cpp"
    break;

  case 274: /* try_stmt: TTRY tryable_stmt  */
#line 1927 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8075 "bison-chpl-lib.cpp"
    break;

  case 275: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1931 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8083 "bison-chpl-lib.cpp"
    break;

  case 276: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1935 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8091 "bison-chpl-lib.cpp"
    break;

  case 277: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1939 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8099 "bison-chpl-lib.cpp"
    break;

  case 278: /* catch_expr_ls: %empty  */
#line 1945 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8105 "bison-chpl-lib.cpp"
    break;

  case 279: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1946 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8111 "bison-chpl-lib.cpp"
    break;

  case 280: /* catch_expr: TCATCH block_stmt  */
#line 1951 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8119 "bison-chpl-lib.cpp"
    break;

  case 281: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1955 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8127 "bison-chpl-lib.cpp"
    break;

  case 282: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1959 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8135 "bison-chpl-lib.cpp"
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
#line 8152 "bison-chpl-lib.cpp"
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
#line 8169 "bison-chpl-lib.cpp"
    break;

  case 285: /* throw_stmt: TTHROW expr TSEMI  */
#line 1995 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8179 "bison-chpl-lib.cpp"
    break;

  case 286: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2003 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8187 "bison-chpl-lib.cpp"
    break;

  case 287: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2007 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8197 "bison-chpl-lib.cpp"
    break;

  case 288: /* when_stmt_ls: %empty  */
#line 2015 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8203 "bison-chpl-lib.cpp"
    break;

  case 289: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2016 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8209 "bison-chpl-lib.cpp"
    break;

  case 290: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2021 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8217 "bison-chpl-lib.cpp"
    break;

  case 291: /* when_stmt: TOTHERWISE stmt  */
#line 2025 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8226 "bison-chpl-lib.cpp"
    break;

  case 292: /* when_stmt: TOTHERWISE TDO stmt  */
#line 2030 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8235 "bison-chpl-lib.cpp"
    break;

  case 293: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2038 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8244 "bison-chpl-lib.cpp"
    break;

  case 294: /* manager_expr: expr TAS ident_def  */
#line 2043 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8252 "bison-chpl-lib.cpp"
    break;

  case 295: /* manager_expr: expr  */
#line 2047 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8260 "bison-chpl-lib.cpp"
    break;

  case 296: /* manager_expr_ls: manager_expr  */
#line 2053 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8266 "bison-chpl-lib.cpp"
    break;

  case 297: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2054 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8272 "bison-chpl-lib.cpp"
    break;

  case 298: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2059 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8280 "bison-chpl-lib.cpp"
    break;

  case 299: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2068 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8289 "bison-chpl-lib.cpp"
    break;

  case 300: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2073 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8300 "bison-chpl-lib.cpp"
    break;

  case 301: /* class_start: class_tag ident_def  */
#line 2084 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8308 "bison-chpl-lib.cpp"
    break;

  case 302: /* class_tag: TCLASS  */
#line 2090 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8314 "bison-chpl-lib.cpp"
    break;

  case 303: /* class_tag: TRECORD  */
#line 2091 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8320 "bison-chpl-lib.cpp"
    break;

  case 304: /* class_tag: TUNION  */
#line 2092 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8326 "bison-chpl-lib.cpp"
    break;

  case 305: /* opt_inherit: %empty  */
#line 2096 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8332 "bison-chpl-lib.cpp"
    break;

  case 306: /* opt_inherit: TCOLON expr_ls  */
#line 2097 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8338 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_level_stmt_ls: %empty  */
#line 2101 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8347 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2106 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8355 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2110 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8363 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2114 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8371 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2118 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8379 "bison-chpl-lib.cpp"
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
#line 8400 "bison-chpl-lib.cpp"
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
#line 8414 "bison-chpl-lib.cpp"
    break;

  case 314: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2155 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8422 "bison-chpl-lib.cpp"
    break;

  case 315: /* enum_ls: enum_item  */
#line 2162 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8431 "bison-chpl-lib.cpp"
    break;

  case 316: /* enum_ls: enum_ls TCOMMA  */
#line 2167 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8441 "bison-chpl-lib.cpp"
    break;

  case 317: /* $@5: %empty  */
#line 2173 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8450 "bison-chpl-lib.cpp"
    break;

  case 318: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2178 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8459 "bison-chpl-lib.cpp"
    break;

  case 319: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2183 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8468 "bison-chpl-lib.cpp"
    break;

  case 320: /* $@6: %empty  */
#line 2188 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8477 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2193 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8486 "bison-chpl-lib.cpp"
    break;

  case 322: /* enum_item: ident_def  */
#line 2203 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8497 "bison-chpl-lib.cpp"
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
#line 8510 "bison-chpl-lib.cpp"
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
#line 8526 "bison-chpl-lib.cpp"
    break;

  case 325: /* $@7: %empty  */
#line 2239 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8536 "bison-chpl-lib.cpp"
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
#line 8558 "bison-chpl-lib.cpp"
    break;

  case 328: /* linkage_spec: linkage_spec_empty  */
#line 2268 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8564 "bison-chpl-lib.cpp"
    break;

  case 329: /* linkage_spec: TINLINE  */
#line 2269 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8571 "bison-chpl-lib.cpp"
    break;

  case 330: /* linkage_spec: TOVERRIDE  */
#line 2271 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8578 "bison-chpl-lib.cpp"
    break;

  case 331: /* opt_fn_type_formal_ls: %empty  */
#line 2276 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8584 "bison-chpl-lib.cpp"
    break;

  case 332: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2277 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8590 "bison-chpl-lib.cpp"
    break;

  case 333: /* fn_type_formal_ls: fn_type_formal  */
#line 2281 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8596 "bison-chpl-lib.cpp"
    break;

  case 334: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2282 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8602 "bison-chpl-lib.cpp"
    break;

  case 335: /* fn_type_formal: named_formal  */
#line 2287 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8608 "bison-chpl-lib.cpp"
    break;

  case 336: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2290 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8614 "bison-chpl-lib.cpp"
    break;

  case 337: /* fn_type_formal: formal_type  */
#line 2292 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8620 "bison-chpl-lib.cpp"
    break;

  case 338: /* opt_fn_type_ret_type: %empty  */
#line 2296 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8626 "bison-chpl-lib.cpp"
    break;

  case 339: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2297 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8632 "bison-chpl-lib.cpp"
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
#line 8651 "bison-chpl-lib.cpp"
    break;

  case 341: /* $@8: %empty  */
#line 2328 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8661 "bison-chpl-lib.cpp"
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
#line 8674 "bison-chpl-lib.cpp"
    break;

  case 343: /* $@9: %empty  */
#line 2343 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8684 "bison-chpl-lib.cpp"
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
#line 8697 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2361 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8705 "bison-chpl-lib.cpp"
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
#line 8719 "bison-chpl-lib.cpp"
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
#line 8742 "bison-chpl-lib.cpp"
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
#line 8754 "bison-chpl-lib.cpp"
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
#line 8766 "bison-chpl-lib.cpp"
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
#line 8781 "bison-chpl-lib.cpp"
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
#line 8796 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2440 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8806 "bison-chpl-lib.cpp"
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
#line 8822 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2464 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8828 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_ident: ident_def TBANG  */
#line 2471 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8838 "bison-chpl-lib.cpp"
    break;

  case 398: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2528 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8844 "bison-chpl-lib.cpp"
    break;

  case 399: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2529 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8850 "bison-chpl-lib.cpp"
    break;

  case 400: /* opt_formal_ls: %empty  */
#line 2533 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8856 "bison-chpl-lib.cpp"
    break;

  case 401: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2534 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8862 "bison-chpl-lib.cpp"
    break;

  case 402: /* req_formal_ls: TLP TRP  */
#line 2538 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8868 "bison-chpl-lib.cpp"
    break;

  case 403: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2539 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8874 "bison-chpl-lib.cpp"
    break;

  case 404: /* formal_ls_inner: formal  */
#line 2543 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8880 "bison-chpl-lib.cpp"
    break;

  case 405: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2544 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8886 "bison-chpl-lib.cpp"
    break;

  case 406: /* formal_ls: %empty  */
#line 2548 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8892 "bison-chpl-lib.cpp"
    break;

  case 407: /* formal_ls: formal_ls_inner  */
#line 2549 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8898 "bison-chpl-lib.cpp"
    break;

  case 411: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2563 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8906 "bison-chpl-lib.cpp"
    break;

  case 412: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2568 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8914 "bison-chpl-lib.cpp"
    break;

  case 413: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2573 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8922 "bison-chpl-lib.cpp"
    break;

  case 414: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2578 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8930 "bison-chpl-lib.cpp"
    break;

  case 415: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2583 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8938 "bison-chpl-lib.cpp"
    break;

  case 416: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2588 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8946 "bison-chpl-lib.cpp"
    break;

  case 417: /* opt_formal_intent_tag: %empty  */
#line 2594 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8955 "bison-chpl-lib.cpp"
    break;

  case 418: /* opt_formal_intent_tag: required_intent_tag  */
#line 2599 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8964 "bison-chpl-lib.cpp"
    break;

  case 419: /* required_intent_tag: TIN  */
#line 2606 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8970 "bison-chpl-lib.cpp"
    break;

  case 420: /* required_intent_tag: TINOUT  */
#line 2607 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8976 "bison-chpl-lib.cpp"
    break;

  case 421: /* required_intent_tag: TOUT  */
#line 2608 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8982 "bison-chpl-lib.cpp"
    break;

  case 422: /* required_intent_tag: TCONST TIN  */
#line 2609 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8988 "bison-chpl-lib.cpp"
    break;

  case 423: /* required_intent_tag: TCONST TREF  */
#line 2610 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 8994 "bison-chpl-lib.cpp"
    break;

  case 424: /* required_intent_tag: TCONST  */
#line 2611 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9000 "bison-chpl-lib.cpp"
    break;

  case 425: /* required_intent_tag: TPARAM  */
#line 2612 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9006 "bison-chpl-lib.cpp"
    break;

  case 426: /* required_intent_tag: TREF  */
#line 2613 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9012 "bison-chpl-lib.cpp"
    break;

  case 427: /* required_intent_tag: TTYPE  */
#line 2614 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9018 "bison-chpl-lib.cpp"
    break;

  case 428: /* opt_this_intent_tag: %empty  */
#line 2618 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9024 "bison-chpl-lib.cpp"
    break;

  case 429: /* opt_this_intent_tag: TPARAM  */
#line 2619 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 430: /* opt_this_intent_tag: TREF  */
#line 2620 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9036 "bison-chpl-lib.cpp"
    break;

  case 431: /* opt_this_intent_tag: TCONST TREF  */
#line 2621 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9042 "bison-chpl-lib.cpp"
    break;

  case 432: /* opt_this_intent_tag: TCONST  */
#line 2622 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9048 "bison-chpl-lib.cpp"
    break;

  case 433: /* opt_this_intent_tag: TTYPE  */
#line 2623 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9054 "bison-chpl-lib.cpp"
    break;

  case 434: /* proc_iter_or_op: TPROC  */
#line 2627 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9060 "bison-chpl-lib.cpp"
    break;

  case 435: /* proc_iter_or_op: TITER  */
#line 2628 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9066 "bison-chpl-lib.cpp"
    break;

  case 436: /* proc_iter_or_op: TOPERATOR  */
#line 2629 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9072 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_ret_tag: %empty  */
#line 2633 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9078 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_ret_tag: TOUT  */
#line 2634 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 9084 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_ret_tag: TCONST  */
#line 2635 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 9090 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_ret_tag: TCONST TREF  */
#line 2636 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 9096 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_ret_tag: TREF  */
#line 2637 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 9102 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_ret_tag: TPARAM  */
#line 2638 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 9108 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_ret_tag: TTYPE  */
#line 2639 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 9114 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_throws_error: %empty  */
#line 2643 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9120 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_throws_error: TTHROWS  */
#line 2644 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9126 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_function_body_stmt: TSEMI  */
#line 2647 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9132 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_function_body_stmt: function_body_stmt  */
#line 2648 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9138 "bison-chpl-lib.cpp"
    break;

  case 448: /* function_body_stmt: block_stmt_body  */
#line 2652 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9144 "bison-chpl-lib.cpp"
    break;

  case 449: /* function_body_stmt: TDO toplevel_stmt  */
#line 2653 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 9150 "bison-chpl-lib.cpp"
    break;

  case 450: /* function_body_stmt: return_stmt  */
#line 2654 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt), &((yylsp[0]))); }
#line 9156 "bison-chpl-lib.cpp"
    break;

  case 451: /* query_expr: TQUERIEDIDENT  */
#line 2658 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9162 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_lifetime_where: %empty  */
#line 2663 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9168 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_lifetime_where: TWHERE expr  */
#line 2665 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9174 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2667 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9180 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2669 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9186 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2671 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9192 "bison-chpl-lib.cpp"
    break;

  case 457: /* lifetime_components_expr: lifetime_expr  */
#line 2676 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9198 "bison-chpl-lib.cpp"
    break;

  case 458: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2678 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9204 "bison-chpl-lib.cpp"
    break;

  case 459: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2683 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9210 "bison-chpl-lib.cpp"
    break;

  case 460: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2685 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9216 "bison-chpl-lib.cpp"
    break;

  case 461: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2687 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9222 "bison-chpl-lib.cpp"
    break;

  case 462: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2689 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9228 "bison-chpl-lib.cpp"
    break;

  case 463: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2691 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9234 "bison-chpl-lib.cpp"
    break;

  case 464: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2693 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9240 "bison-chpl-lib.cpp"
    break;

  case 465: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2695 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9246 "bison-chpl-lib.cpp"
    break;

  case 466: /* lifetime_ident: TIDENT  */
#line 2699 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9252 "bison-chpl-lib.cpp"
    break;

  case 467: /* lifetime_ident: TTHIS  */
#line 2700 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9258 "bison-chpl-lib.cpp"
    break;

  case 468: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2705 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9267 "bison-chpl-lib.cpp"
    break;

  case 469: /* type_alias_decl_stmt_start: TTYPE  */
#line 2714 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9275 "bison-chpl-lib.cpp"
    break;

  case 470: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2718 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9284 "bison-chpl-lib.cpp"
    break;

  case 471: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2726 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9292 "bison-chpl-lib.cpp"
    break;

  case 472: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2730 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9300 "bison-chpl-lib.cpp"
    break;

  case 473: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2737 "chpl.ypp"
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
      // Gather the comments like a statement does.
      (yyval.commentsAndStmt) = STMT((yylsp[-1]), node.release());
      context->clearComments();
  }
#line 9322 "bison-chpl-lib.cpp"
    break;

  case 474: /* opt_init_type: %empty  */
#line 2757 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9328 "bison-chpl-lib.cpp"
    break;

  case 475: /* opt_init_type: TASSIGN expr  */
#line 2759 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9334 "bison-chpl-lib.cpp"
    break;

  case 476: /* var_decl_type: TPARAM  */
#line 2763 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9340 "bison-chpl-lib.cpp"
    break;

  case 477: /* var_decl_type: TCONST TREF  */
#line 2764 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9346 "bison-chpl-lib.cpp"
    break;

  case 478: /* var_decl_type: TREF  */
#line 2765 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9352 "bison-chpl-lib.cpp"
    break;

  case 479: /* var_decl_type: TCONST  */
#line 2766 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9358 "bison-chpl-lib.cpp"
    break;

  case 480: /* var_decl_type: TVAR  */
#line 2767 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9364 "bison-chpl-lib.cpp"
    break;

  case 481: /* $@11: %empty  */
#line 2772 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9373 "bison-chpl-lib.cpp"
    break;

  case 482: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2776 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9382 "bison-chpl-lib.cpp"
    break;

  case 483: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2781 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9391 "bison-chpl-lib.cpp"
    break;

  case 484: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2789 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9399 "bison-chpl-lib.cpp"
    break;

  case 485: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2793 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9407 "bison-chpl-lib.cpp"
    break;

  case 486: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2800 "chpl.ypp"
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
      // gather the comments like a statement does
      (yyval.commentsAndStmt) = STMT((yylsp[-2]), varDecl.release());
      context->clearComments();
    }
#line 9429 "bison-chpl-lib.cpp"
    break;

  case 487: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2818 "chpl.ypp"
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
#line 9449 "bison-chpl-lib.cpp"
    break;

  case 488: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2837 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9457 "bison-chpl-lib.cpp"
    break;

  case 489: /* tuple_var_decl_component: ident_def  */
#line 2841 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9465 "bison-chpl-lib.cpp"
    break;

  case 490: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2845 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9473 "bison-chpl-lib.cpp"
    break;

  case 491: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2852 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9479 "bison-chpl-lib.cpp"
    break;

  case 492: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2854 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9485 "bison-chpl-lib.cpp"
    break;

  case 493: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2856 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9491 "bison-chpl-lib.cpp"
    break;

  case 494: /* opt_init_expr: %empty  */
#line 2862 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9497 "bison-chpl-lib.cpp"
    break;

  case 495: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2863 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9503 "bison-chpl-lib.cpp"
    break;

  case 496: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2864 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9509 "bison-chpl-lib.cpp"
    break;

  case 497: /* formal_or_ret_type_expr: expr  */
#line 2868 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9515 "bison-chpl-lib.cpp"
    break;

  case 498: /* ret_type: formal_or_ret_type_expr  */
#line 2872 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9521 "bison-chpl-lib.cpp"
    break;

  case 499: /* ret_type: reserved_type_ident_use  */
#line 2873 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9527 "bison-chpl-lib.cpp"
    break;

  case 500: /* ret_type: error  */
#line 2874 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9533 "bison-chpl-lib.cpp"
    break;

  case 501: /* colon_ret_type: TCOLON ret_type  */
#line 2878 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9539 "bison-chpl-lib.cpp"
    break;

  case 502: /* colon_ret_type: error  */
#line 2879 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9545 "bison-chpl-lib.cpp"
    break;

  case 503: /* opt_ret_type: %empty  */
#line 2883 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9551 "bison-chpl-lib.cpp"
    break;

  case 505: /* opt_type: %empty  */
#line 2888 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9557 "bison-chpl-lib.cpp"
    break;

  case 506: /* opt_type: TCOLON expr  */
#line 2889 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9563 "bison-chpl-lib.cpp"
    break;

  case 507: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2890 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9569 "bison-chpl-lib.cpp"
    break;

  case 508: /* opt_type: error  */
#line 2891 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9575 "bison-chpl-lib.cpp"
    break;

  case 509: /* formal_type: formal_or_ret_type_expr  */
#line 2895 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9581 "bison-chpl-lib.cpp"
    break;

  case 510: /* formal_type: reserved_type_ident_use  */
#line 2896 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9587 "bison-chpl-lib.cpp"
    break;

  case 511: /* colon_formal_type: TCOLON formal_type  */
#line 2900 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9593 "bison-chpl-lib.cpp"
    break;

  case 512: /* opt_colon_formal_type: %empty  */
#line 2904 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9599 "bison-chpl-lib.cpp"
    break;

  case 513: /* opt_colon_formal_type: colon_formal_type  */
#line 2905 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9605 "bison-chpl-lib.cpp"
    break;

  case 514: /* expr_ls: expr  */
#line 2911 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9611 "bison-chpl-lib.cpp"
    break;

  case 515: /* expr_ls: expr_ls TCOMMA expr  */
#line 2912 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9617 "bison-chpl-lib.cpp"
    break;

  case 516: /* tuple_component: TUNDERSCORE  */
#line 2916 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9623 "bison-chpl-lib.cpp"
    break;

  case 517: /* tuple_component: opt_try_expr  */
#line 2917 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9629 "bison-chpl-lib.cpp"
    break;

  case 518: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2922 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9637 "bison-chpl-lib.cpp"
    break;

  case 519: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2926 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9645 "bison-chpl-lib.cpp"
    break;

  case 520: /* opt_actual_ls: %empty  */
#line 2932 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9651 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_actual_ls: actual_ls  */
#line 2933 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9657 "bison-chpl-lib.cpp"
    break;

  case 522: /* actual_ls: actual_expr  */
#line 2938 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9666 "bison-chpl-lib.cpp"
    break;

  case 523: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2943 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9676 "bison-chpl-lib.cpp"
    break;

  case 524: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2951 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9682 "bison-chpl-lib.cpp"
    break;

  case 525: /* actual_expr: opt_try_expr  */
#line 2952 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9688 "bison-chpl-lib.cpp"
    break;

  case 526: /* ident_expr: ident_use  */
#line 2956 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9694 "bison-chpl-lib.cpp"
    break;

  case 527: /* ident_expr: scalar_type  */
#line 2957 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9700 "bison-chpl-lib.cpp"
    break;

  case 533: /* sub_type_level_expr: TSINGLE expr  */
#line 2970 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9706 "bison-chpl-lib.cpp"
    break;

  case 534: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2972 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9712 "bison-chpl-lib.cpp"
    break;

  case 535: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2974 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9718 "bison-chpl-lib.cpp"
    break;

  case 536: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2976 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9724 "bison-chpl-lib.cpp"
    break;

  case 537: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2978 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9734 "bison-chpl-lib.cpp"
    break;

  case 538: /* sub_type_level_expr: TATOMIC expr  */
#line 2984 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9740 "bison-chpl-lib.cpp"
    break;

  case 539: /* sub_type_level_expr: TSYNC expr  */
#line 2986 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9746 "bison-chpl-lib.cpp"
    break;

  case 540: /* sub_type_level_expr: TOWNED  */
#line 2989 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9752 "bison-chpl-lib.cpp"
    break;

  case 541: /* sub_type_level_expr: TOWNED expr  */
#line 2991 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9758 "bison-chpl-lib.cpp"
    break;

  case 542: /* sub_type_level_expr: TUNMANAGED  */
#line 2993 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9764 "bison-chpl-lib.cpp"
    break;

  case 543: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2995 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9770 "bison-chpl-lib.cpp"
    break;

  case 544: /* sub_type_level_expr: TSHARED  */
#line 2997 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9776 "bison-chpl-lib.cpp"
    break;

  case 545: /* sub_type_level_expr: TSHARED expr  */
#line 2999 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9782 "bison-chpl-lib.cpp"
    break;

  case 546: /* sub_type_level_expr: TBORROWED  */
#line 3001 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9788 "bison-chpl-lib.cpp"
    break;

  case 547: /* sub_type_level_expr: TBORROWED expr  */
#line 3003 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9794 "bison-chpl-lib.cpp"
    break;

  case 548: /* sub_type_level_expr: TCLASS  */
#line 3005 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9800 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TRECORD  */
#line 3007 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9806 "bison-chpl-lib.cpp"
    break;

  case 550: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 3012 "chpl.ypp"
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
#line 9821 "bison-chpl-lib.cpp"
    break;

  case 551: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 3023 "chpl.ypp"
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
#line 9836 "bison-chpl-lib.cpp"
    break;

  case 552: /* for_expr: TFOR expr TDO expr  */
#line 3034 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->buildAttributeGroup((yyloc))).release();
                    context->resetAttributeGroupPartsState();
  }
#line 9850 "bison-chpl-lib.cpp"
    break;

  case 553: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3044 "chpl.ypp"
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
#line 9870 "bison-chpl-lib.cpp"
    break;

  case 554: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3060 "chpl.ypp"
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
#line 9890 "bison-chpl-lib.cpp"
    break;

  case 555: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 3076 "chpl.ypp"
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
#line 9910 "bison-chpl-lib.cpp"
    break;

  case 556: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3092 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 9923 "bison-chpl-lib.cpp"
    break;

  case 557: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3101 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 9936 "bison-chpl-lib.cpp"
    break;

  case 558: /* for_expr: TFORALL expr TDO expr  */
#line 3110 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 9948 "bison-chpl-lib.cpp"
    break;

  case 559: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3118 "chpl.ypp"
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
#line 9966 "bison-chpl-lib.cpp"
    break;

  case 560: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3132 "chpl.ypp"
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
#line 9984 "bison-chpl-lib.cpp"
    break;

  case 561: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3146 "chpl.ypp"
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
#line 10002 "bison-chpl-lib.cpp"
    break;

  case 562: /* bracket_loop_expr: TLSBR TRSBR  */
#line 3163 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 10010 "bison-chpl-lib.cpp"
    break;

  case 563: /* bracket_loop_expr: TLSBR TRSBR expr  */
#line 3167 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 10018 "bison-chpl-lib.cpp"
    break;

  case 564: /* bracket_loop_expr: TLSBR expr_ls TRSBR expr  */
#line 3171 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10026 "bison-chpl-lib.cpp"
    break;

  case 565: /* bracket_loop_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3175 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10034 "bison-chpl-lib.cpp"
    break;

  case 566: /* bracket_loop_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3179 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10042 "bison-chpl-lib.cpp"
    break;

  case 567: /* bracket_loop_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3183 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10050 "bison-chpl-lib.cpp"
    break;

  case 568: /* bracket_loop_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3187 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10058 "bison-chpl-lib.cpp"
    break;

  case 569: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3194 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10072 "bison-chpl-lib.cpp"
    break;

  case 570: /* nil_expr: TNIL  */
#line 3211 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10078 "bison-chpl-lib.cpp"
    break;

  case 578: /* opt_task_intent_ls: %empty  */
#line 3229 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10084 "bison-chpl-lib.cpp"
    break;

  case 579: /* opt_task_intent_ls: task_intent_clause  */
#line 3230 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 580: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3235 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10100 "bison-chpl-lib.cpp"
    break;

  case 581: /* task_intent_ls: intent_expr  */
#line 3243 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10106 "bison-chpl-lib.cpp"
    break;

  case 582: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3244 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10112 "bison-chpl-lib.cpp"
    break;

  case 583: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3249 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10122 "bison-chpl-lib.cpp"
    break;

  case 584: /* forall_intent_ls: intent_expr  */
#line 3257 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10128 "bison-chpl-lib.cpp"
    break;

  case 585: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3258 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10134 "bison-chpl-lib.cpp"
    break;

  case 586: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3263 "chpl.ypp"
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
#line 10152 "bison-chpl-lib.cpp"
    break;

  case 587: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3277 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10160 "bison-chpl-lib.cpp"
    break;

  case 588: /* intent_expr: expr TREDUCE ident_expr  */
#line 3281 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10168 "bison-chpl-lib.cpp"
    break;

  case 589: /* task_var_prefix: TCONST  */
#line 3287 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10174 "bison-chpl-lib.cpp"
    break;

  case 590: /* task_var_prefix: TIN  */
#line 3288 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10180 "bison-chpl-lib.cpp"
    break;

  case 591: /* task_var_prefix: TCONST TIN  */
#line 3289 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10186 "bison-chpl-lib.cpp"
    break;

  case 592: /* task_var_prefix: TREF  */
#line 3290 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10192 "bison-chpl-lib.cpp"
    break;

  case 593: /* task_var_prefix: TCONST TREF  */
#line 3291 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10198 "bison-chpl-lib.cpp"
    break;

  case 594: /* task_var_prefix: TVAR  */
#line 3292 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10204 "bison-chpl-lib.cpp"
    break;

  case 595: /* new_maybe_decorated: TNEW  */
#line 3297 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10210 "bison-chpl-lib.cpp"
    break;

  case 596: /* new_maybe_decorated: TNEW TOWNED  */
#line 3299 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10216 "bison-chpl-lib.cpp"
    break;

  case 597: /* new_maybe_decorated: TNEW TSHARED  */
#line 3301 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10222 "bison-chpl-lib.cpp"
    break;

  case 598: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3303 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10228 "bison-chpl-lib.cpp"
    break;

  case 599: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3305 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10234 "bison-chpl-lib.cpp"
    break;

  case 600: /* new_expr: new_maybe_decorated expr  */
#line 3311 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10242 "bison-chpl-lib.cpp"
    break;

  case 601: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3318 "chpl.ypp"
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
#line 10258 "bison-chpl-lib.cpp"
    break;

  case 602: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3330 "chpl.ypp"
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
#line 10274 "bison-chpl-lib.cpp"
    break;

  case 603: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
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
    (yyval.expr) = context->buildNewExpr((yyloc), New::OWNED, call.release());
  }
#line 10291 "bison-chpl-lib.cpp"
    break;

  case 604: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3355 "chpl.ypp"
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
#line 10309 "bison-chpl-lib.cpp"
    break;

  case 605: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3372 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10317 "bison-chpl-lib.cpp"
    break;

  case 606: /* range_literal_expr: expr TDOTDOT expr  */
#line 3379 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10326 "bison-chpl-lib.cpp"
    break;

  case 607: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3384 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10335 "bison-chpl-lib.cpp"
    break;

  case 608: /* range_literal_expr: expr TDOTDOT  */
#line 3389 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10344 "bison-chpl-lib.cpp"
    break;

  case 609: /* range_literal_expr: TDOTDOT expr  */
#line 3394 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10354 "bison-chpl-lib.cpp"
    break;

  case 610: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3400 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10364 "bison-chpl-lib.cpp"
    break;

  case 611: /* range_literal_expr: TDOTDOT  */
#line 3406 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10374 "bison-chpl-lib.cpp"
    break;

  case 612: /* cast_expr: expr TCOLON expr  */
#line 3436 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10382 "bison-chpl-lib.cpp"
    break;

  case 613: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3443 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10390 "bison-chpl-lib.cpp"
    break;

  case 614: /* super_expr: fn_expr  */
#line 3449 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10396 "bison-chpl-lib.cpp"
    break;

  case 618: /* expr: sub_type_level_expr TQUESTION  */
#line 3458 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10402 "bison-chpl-lib.cpp"
    break;

  case 619: /* expr: TQUESTION  */
#line 3460 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10408 "bison-chpl-lib.cpp"
    break;

  case 623: /* expr: fn_type  */
#line 3465 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10414 "bison-chpl-lib.cpp"
    break;

  case 633: /* opt_expr: %empty  */
#line 3479 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10420 "bison-chpl-lib.cpp"
    break;

  case 634: /* opt_expr: expr  */
#line 3480 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10426 "bison-chpl-lib.cpp"
    break;

  case 635: /* opt_try_expr: TTRY expr  */
#line 3484 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10432 "bison-chpl-lib.cpp"
    break;

  case 636: /* opt_try_expr: TTRYBANG expr  */
#line 3485 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10438 "bison-chpl-lib.cpp"
    break;

  case 637: /* opt_try_expr: super_expr  */
#line 3486 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10444 "bison-chpl-lib.cpp"
    break;

  case 643: /* call_base_expr: expr TBANG  */
#line 3503 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10452 "bison-chpl-lib.cpp"
    break;

  case 644: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3506 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10458 "bison-chpl-lib.cpp"
    break;

  case 647: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3513 "chpl.ypp"
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
#line 10474 "bison-chpl-lib.cpp"
    break;

  case 648: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3525 "chpl.ypp"
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
#line 10490 "bison-chpl-lib.cpp"
    break;

  case 649: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3537 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10498 "bison-chpl-lib.cpp"
    break;

  case 650: /* dot_expr: expr TDOT ident_use  */
#line 3544 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10504 "bison-chpl-lib.cpp"
    break;

  case 651: /* dot_expr: expr TDOT TTYPE  */
#line 3546 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10510 "bison-chpl-lib.cpp"
    break;

  case 652: /* dot_expr: expr TDOT TDOMAIN  */
#line 3548 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10516 "bison-chpl-lib.cpp"
    break;

  case 653: /* dot_expr: expr TDOT TLOCALE  */
#line 3550 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10522 "bison-chpl-lib.cpp"
    break;

  case 654: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3552 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10532 "bison-chpl-lib.cpp"
    break;

  case 655: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3558 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10542 "bison-chpl-lib.cpp"
    break;

  case 656: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3570 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10548 "bison-chpl-lib.cpp"
    break;

  case 657: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3572 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10556 "bison-chpl-lib.cpp"
    break;

  case 658: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3576 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10564 "bison-chpl-lib.cpp"
    break;

  case 659: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3580 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10572 "bison-chpl-lib.cpp"
    break;

  case 660: /* bool_literal: TFALSE  */
#line 3586 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10578 "bison-chpl-lib.cpp"
    break;

  case 661: /* bool_literal: TTRUE  */
#line 3587 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10584 "bison-chpl-lib.cpp"
    break;

  case 662: /* str_bytes_literal: STRINGLITERAL  */
#line 3591 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10590 "bison-chpl-lib.cpp"
    break;

  case 663: /* str_bytes_literal: BYTESLITERAL  */
#line 3592 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10596 "bison-chpl-lib.cpp"
    break;

  case 666: /* literal_expr: INTLITERAL  */
#line 3598 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10602 "bison-chpl-lib.cpp"
    break;

  case 667: /* literal_expr: REALLITERAL  */
#line 3599 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10608 "bison-chpl-lib.cpp"
    break;

  case 668: /* literal_expr: IMAGLITERAL  */
#line 3600 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10614 "bison-chpl-lib.cpp"
    break;

  case 669: /* literal_expr: CSTRINGLITERAL  */
#line 3601 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10620 "bison-chpl-lib.cpp"
    break;

  case 670: /* literal_expr: TNONE  */
#line 3602 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10626 "bison-chpl-lib.cpp"
    break;

  case 671: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3604 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10635 "bison-chpl-lib.cpp"
    break;

  case 672: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3609 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10644 "bison-chpl-lib.cpp"
    break;

  case 673: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3614 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10652 "bison-chpl-lib.cpp"
    break;

  case 674: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3618 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
  }
#line 10660 "bison-chpl-lib.cpp"
    break;

  case 675: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3622 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
  }
#line 10668 "bison-chpl-lib.cpp"
    break;

  case 676: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3626 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
  }
#line 10676 "bison-chpl-lib.cpp"
    break;

  case 677: /* assoc_expr_ls: expr TALIAS expr  */
#line 3634 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10685 "bison-chpl-lib.cpp"
    break;

  case 678: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3639 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10695 "bison-chpl-lib.cpp"
    break;

  case 679: /* binary_op_expr: expr TPLUS expr  */
#line 3647 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10701 "bison-chpl-lib.cpp"
    break;

  case 680: /* binary_op_expr: expr TMINUS expr  */
#line 3648 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10707 "bison-chpl-lib.cpp"
    break;

  case 681: /* binary_op_expr: expr TSTAR expr  */
#line 3649 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10713 "bison-chpl-lib.cpp"
    break;

  case 682: /* binary_op_expr: expr TDIVIDE expr  */
#line 3650 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10719 "bison-chpl-lib.cpp"
    break;

  case 683: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3651 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10725 "bison-chpl-lib.cpp"
    break;

  case 684: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3652 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10731 "bison-chpl-lib.cpp"
    break;

  case 685: /* binary_op_expr: expr TMOD expr  */
#line 3653 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10737 "bison-chpl-lib.cpp"
    break;

  case 686: /* binary_op_expr: expr TEQUAL expr  */
#line 3654 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10743 "bison-chpl-lib.cpp"
    break;

  case 687: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3655 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10749 "bison-chpl-lib.cpp"
    break;

  case 688: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3656 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10755 "bison-chpl-lib.cpp"
    break;

  case 689: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3657 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10761 "bison-chpl-lib.cpp"
    break;

  case 690: /* binary_op_expr: expr TLESS expr  */
#line 3658 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10767 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TGREATER expr  */
#line 3659 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10773 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TBAND expr  */
#line 3660 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10779 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TBOR expr  */
#line 3661 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10785 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TBXOR expr  */
#line 3662 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10791 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TAND expr  */
#line 3663 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10797 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TOR expr  */
#line 3664 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10803 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TEXP expr  */
#line 3665 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10809 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TBY expr  */
#line 3666 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10815 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TALIGN expr  */
#line 3667 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10821 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr THASH expr  */
#line 3668 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10827 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TDMAPPED expr  */
#line 3669 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10833 "bison-chpl-lib.cpp"
    break;

  case 702: /* unary_op_expr: TPLUS expr  */
#line 3673 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10839 "bison-chpl-lib.cpp"
    break;

  case 703: /* unary_op_expr: TMINUS expr  */
#line 3674 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10845 "bison-chpl-lib.cpp"
    break;

  case 704: /* unary_op_expr: TMINUSMINUS expr  */
#line 3675 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10851 "bison-chpl-lib.cpp"
    break;

  case 705: /* unary_op_expr: TPLUSPLUS expr  */
#line 3676 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10857 "bison-chpl-lib.cpp"
    break;

  case 706: /* unary_op_expr: TBANG expr  */
#line 3677 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10863 "bison-chpl-lib.cpp"
    break;

  case 707: /* unary_op_expr: expr TBANG  */
#line 3678 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10871 "bison-chpl-lib.cpp"
    break;

  case 708: /* unary_op_expr: TBNOT expr  */
#line 3681 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10877 "bison-chpl-lib.cpp"
    break;

  case 709: /* reduce_expr: expr TREDUCE expr  */
#line 3686 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10885 "bison-chpl-lib.cpp"
    break;

  case 710: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3690 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10893 "bison-chpl-lib.cpp"
    break;

  case 711: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3694 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10901 "bison-chpl-lib.cpp"
    break;

  case 712: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3698 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10909 "bison-chpl-lib.cpp"
    break;

  case 713: /* scan_expr: expr TSCAN expr  */
#line 3705 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10917 "bison-chpl-lib.cpp"
    break;

  case 714: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3709 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10925 "bison-chpl-lib.cpp"
    break;

  case 715: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3713 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10933 "bison-chpl-lib.cpp"
    break;

  case 716: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3717 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10941 "bison-chpl-lib.cpp"
    break;


#line 10945 "bison-chpl-lib.cpp"

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
