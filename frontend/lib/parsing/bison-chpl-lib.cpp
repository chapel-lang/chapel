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
#define YYLAST   19199

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  198
/* YYNRULES -- Number of rules.  */
#define YYNRULES  721
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1292

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
     870,   874,   875,   879,   886,   894,   897,   907,   908,   910,
     917,   922,   931,   935,   955,   970,   985,  1001,  1002,  1004,
    1009,  1010,  1015,  1033,  1037,  1041,  1052,  1071,  1072,  1076,
    1080,  1084,  1089,  1093,  1097,  1107,  1112,  1118,  1125,  1130,
    1137,  1148,  1149,  1153,  1158,  1166,  1175,  1185,  1193,  1202,
    1215,  1223,  1227,  1232,  1238,  1247,  1248,  1252,  1261,  1265,
    1269,  1273,  1277,  1281,  1290,  1291,  1295,  1296,  1297,  1298,
    1299,  1303,  1304,  1316,  1317,  1322,  1323,  1324,  1325,  1326,
    1327,  1328,  1329,  1330,  1331,  1332,  1333,  1334,  1335,  1336,
    1337,  1338,  1339,  1340,  1344,  1345,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,  1353,  1354,  1355,  1362,  1363,  1364,  1365,
    1369,  1370,  1374,  1381,  1391,  1395,  1401,  1401,  1407,  1407,
    1416,  1417,  1418,  1419,  1420,  1421,  1422,  1426,  1431,  1436,
    1441,  1448,  1456,  1457,  1461,  1470,  1476,  1484,  1503,  1502,
    1512,  1511,  1524,  1531,  1549,  1562,  1575,  1579,  1583,  1587,
    1591,  1595,  1599,  1603,  1623,  1627,  1631,  1635,  1639,  1643,
    1647,  1651,  1655,  1659,  1663,  1667,  1671,  1675,  1679,  1683,
    1687,  1691,  1695,  1699,  1703,  1707,  1711,  1716,  1724,  1733,
    1737,  1741,  1745,  1749,  1753,  1757,  1761,  1765,  1770,  1775,
    1780,  1788,  1803,  1821,  1825,  1832,  1833,  1838,  1843,  1844,
    1845,  1846,  1847,  1848,  1849,  1850,  1851,  1852,  1853,  1854,
    1855,  1864,  1865,  1866,  1867,  1876,  1877,  1881,  1885,  1889,
    1896,  1900,  1904,  1911,  1915,  1919,  1923,  1930,  1931,  1935,
    1939,  1943,  1950,  1963,  1979,  1987,  1991,  2000,  2001,  2005,
    2009,  2014,  2022,  2027,  2031,  2038,  2039,  2043,  2052,  2057,
    2068,  2075,  2076,  2077,  2081,  2082,  2086,  2090,  2094,  2098,
    2102,  2109,  2126,  2139,  2146,  2151,  2158,  2157,  2167,  2173,
    2172,  2187,  2194,  2206,  2224,  2221,  2249,  2253,  2254,  2256,
    2261,  2262,  2266,  2267,  2271,  2274,  2276,  2281,  2282,  2293,
    2313,  2312,  2328,  2327,  2345,  2355,  2352,  2386,  2394,  2402,
    2413,  2424,  2433,  2448,  2449,  2453,  2454,  2455,  2464,  2465,
    2466,  2467,  2468,  2469,  2470,  2471,  2472,  2473,  2474,  2475,
    2476,  2477,  2478,  2479,  2480,  2481,  2482,  2483,  2484,  2485,
    2486,  2487,  2488,  2492,  2493,  2494,  2495,  2496,  2497,  2498,
    2499,  2500,  2501,  2502,  2503,  2508,  2509,  2513,  2514,  2518,
    2519,  2523,  2524,  2528,  2529,  2533,  2534,  2538,  2542,  2543,
    2547,  2551,  2556,  2561,  2566,  2571,  2579,  2583,  2591,  2592,
    2593,  2594,  2595,  2596,  2597,  2598,  2599,  2603,  2604,  2605,
    2606,  2607,  2608,  2612,  2613,  2614,  2618,  2619,  2620,  2621,
    2622,  2623,  2627,  2628,  2631,  2632,  2636,  2637,  2638,  2642,
    2647,  2648,  2650,  2652,  2654,  2659,  2661,  2666,  2668,  2670,
    2672,  2674,  2676,  2678,  2683,  2684,  2688,  2697,  2701,  2709,
    2713,  2720,  2741,  2742,  2747,  2748,  2749,  2750,  2751,  2756,
    2755,  2764,  2772,  2776,  2783,  2799,  2816,  2820,  2824,  2831,
    2833,  2835,  2842,  2843,  2844,  2848,  2852,  2853,  2854,  2858,
    2859,  2863,  2864,  2868,  2869,  2870,  2871,  2875,  2876,  2880,
    2884,  2885,  2891,  2892,  2896,  2897,  2901,  2905,  2912,  2913,
    2917,  2922,  2931,  2932,  2936,  2937,  2944,  2945,  2946,  2947,
    2948,  2949,  2951,  2953,  2955,  2957,  2963,  2965,  2968,  2970,
    2972,  2974,  2976,  2978,  2980,  2982,  2984,  2986,  2991,  3002,
    3013,  3023,  3039,  3055,  3071,  3080,  3089,  3097,  3111,  3125,
    3142,  3146,  3150,  3154,  3158,  3162,  3166,  3173,  3191,  3199,
    3200,  3201,  3202,  3203,  3204,  3205,  3209,  3210,  3214,  3223,
    3224,  3228,  3237,  3238,  3242,  3256,  3260,  3267,  3268,  3269,
    3270,  3271,  3272,  3276,  3278,  3280,  3282,  3284,  3290,  3297,
    3309,  3321,  3334,  3351,  3358,  3363,  3368,  3373,  3379,  3385,
    3415,  3422,  3429,  3430,  3434,  3436,  3437,  3439,  3441,  3442,
    3443,  3444,  3447,  3448,  3449,  3450,  3451,  3452,  3453,  3454,
    3455,  3459,  3460,  3464,  3465,  3466,  3470,  3471,  3472,  3473,
    3482,  3483,  3486,  3487,  3488,  3492,  3504,  3516,  3523,  3525,
    3527,  3529,  3531,  3537,  3550,  3551,  3555,  3559,  3566,  3567,
    3571,  3572,  3576,  3577,  3578,  3579,  3580,  3581,  3582,  3583,
    3588,  3593,  3597,  3601,  3605,  3613,  3618,  3627,  3628,  3629,
    3630,  3631,  3632,  3633,  3634,  3635,  3636,  3637,  3638,  3639,
    3640,  3641,  3642,  3643,  3644,  3645,  3646,  3647,  3648,  3649,
    3653,  3654,  3655,  3656,  3657,  3658,  3661,  3665,  3669,  3673,
    3677,  3684,  3688,  3692,  3696,  3704,  3705,  3706,  3707,  3708,
    3709,  3710
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

#define YYPACT_NINF (-1108)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-722)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1108,   281,  3947, -1108,   -40,   219, -1108, -1108, -1108, -1108,
   -1108, -1108, -1108, 11472,   173,   325,   292, 13873,   306, 18953,
     173, 11472,   346,   295,   319,   325,  5347, 11472,  5347,   240,
   19008, -1108,   413,   403,  9022,  9897,  9897, -1108,  9197,   406,
     308,   327, -1108,   479, 19008, 19008, 19008, -1108, 14117, 10072,
     489, 11472, 11472,   244, -1108,   511,   514, 11472, -1108, 13873,
   -1108, 11472,   485,   402,    47,    58, 13221,   525, 19083, -1108,
   11472,  8497, 11472, 10072, 13873, 11472,   492,   542,   429,  5347,
     554, 11472,   555,  7447,  7447, -1108,   561, -1108, 13873, -1108,
     564,  9197,  9372, -1108,   619, 11472, -1108, 11472, -1108, -1108,
   13701, 11472, -1108, 11472, -1108, -1108, -1108,  4297,  7797,  9547,
   11472, -1108,  4997, -1108, -1108, -1108, -1108,  1966,   494, -1108,
     476,   458, -1108,    27, -1108, -1108, -1108, -1108, -1108, -1108,
   -1108, -1108, -1108, -1108, -1108, -1108, -1108,  7622, -1108, 10247,
   -1108, -1108, -1108, -1108, -1108,   577, -1108, -1108, -1108, -1108,
   -1108, -1108, -1108, -1108,   487, 19008, -1108, 18693,   463,   391,
   -1108,   195, -1108, -1108, -1108,   274,   298, -1108, -1108, 19008,
   14117, -1108,   480,   498, -1108, -1108, -1108,   484,   499, 11472,
     501,   503, -1108, -1108, -1108, 18334,  2534,   350,   504,   505,
   -1108, -1108,   416, -1108, -1108, -1108, -1108, -1108,   377, -1108,
   -1108, -1108, 11472, 11472, 11472, 19008, -1108, 11472, 11472, 10422,
     597,   419, -1108, -1108, -1108, -1108, 18334,   430, -1108, -1108,
     495,  5347, -1108, -1108,   506,   110,   509,  3641, -1108,   258,
   -1108,   507, -1108,   -30, 18334,   538,  9372, -1108, -1108, -1108,
   -1108, -1108, -1108, -1108, -1108, -1108, -1108, -1108, -1108, -1108,
   -1108, -1108, -1108, -1108, -1108, -1108, -1108, -1108, -1108, -1108,
   -1108, -1108,   516, -1108, -1108, 19008,   515,    73, 15304,    33,
    3737,    33, 15152, 19008, 19008,    99, 14412, -1108, -1108,   589,
    9372,   519,   116,  5347,  1471,   100,    34, -1108,  5347, -1108,
   -1108, 15796, -1108,    71, 16426,   490, -1108,   521,   523, -1108,
   15796,   110,   490, -1108,  9372,   684,   684,    74,    12, -1108,
      16, -1108, -1108, -1108, -1108, 11472, 11472, -1108,   573, -1108,
     524,   529, -1108, -1108,  3467,   550, -1108, -1108, 18334,   106,
   11472, 11472, -1108,   -25, -1108, -1108, 18334,   533, 16506, -1108,
   15796,   110, 18334,   528,  9372, -1108, 18334, 16546, -1108, -1108,
   -1108, -1108,   110,    73, 15796,   537, -1108,   246,   246,  2458,
     490,   490,   105, -1108, -1108,  4472,   206, -1108, 11472,     1,
     102, -1108, 14016,   -29,    23, 16592,   -20,  2458,   680, -1108,
   -1108, 19008, -1108,  9022,  9897, -1108,  4297,  9897, -1108, -1108,
     589, -1108, -1108, -1108,   700,   535,  4647, 11472, -1108, 11472,
     640, -1108, -1108, 15396,   202,   487, 18334,   502, 19008, 11472,
     539, -1108,   541, 18748,   593,   234, -1108,    59,   274, -1108,
   -1108, -1108, -1108,   624, -1108, -1108, -1108,    68,   630, -1108,
   -1108, -1108, 13531,   599,   179, -1108,   315,   433, -1108,   576,
   11472, 11472, 11472,  9897,  9897, 11472,   440, 11472, 11472, 11472,
   11472, 11472,   276, 13701, 11472, 11472, 11472, 11472, 11472, 11472,
   11472, 11472, 11472, 11472, 11472, 11472, 11472, 11472, 11472, 11472,
     658, -1108, -1108, -1108, -1108, -1108,  9372,  9372, -1108, -1108,
   -1108, -1108,  9372, -1108, -1108,  9372,  9372,  9372,  9372,  9897,
    9897, 15872, 15948, 16698,   556,    38, 19008,  7972, -1108, -1108,
   -1108,  9897,    73, 14117, -1108, 11472, -1108, 11472,   604,   557,
     584, -1108, -1108, -1108,   685, 11472, -1108,  5522,  9897, -1108,
     563, -1108,    73,  5697,  9897, -1108,    73, -1108,    73,  9897,
   -1108,    73,   611,   615,  5347,   705,  5347, -1108,   707, 11472,
     677,   570,  9372, 19008, -1108, -1108, -1108,  1471, -1108, -1108,
     109, -1108, 11647,   628, 11472, 14117, -1108, -1108, 11472, -1108,
   18823, 11472, 11472, -1108,   581, -1108, -1108, -1108, -1108, 18878,
   -1108,   274, 13395, -1108,  1471, -1108,   613, 11647, -1108, 18334,
   18334, -1108,   509, -1108,    41, -1108,  9372,   590, -1108,   743,
     743, -1108, -1108, -1108, -1108, -1108, -1108, 10597, -1108, 16744,
    8147, -1108,  8322, -1108, 18334,  5347,   592,  9897, 10772,  4122,
     594, 11472, 10947, -1108, -1108, 15476, 15228,    36,   476,  8672,
   -1108,   151, -1108,  4822, -1108,   320, 16784,   321, 15568, 19008,
    7272,  7272, -1108,   487,   605, -1108,   258, -1108,   101,   629,
    1316, -1108, -1108, 11472,   180, -1108, -1108,   114, -1108, -1108,
     137, -1108, -1108,  7097, -1108,   676, -1108,   606,   633, -1108,
     637, -1108,   638,   641,   642,   643, -1108,   645,   648,   649,
     651,   652,   654,   655, -1108, -1108, -1108, -1108, -1108, -1108,
   -1108, -1108, -1108, -1108, -1108, -1108, -1108, -1108, -1108, -1108,
   -1108, -1108, -1108, -1108, -1108, -1108, 11472,   664,   659,   606,
   -1108,   606, -1108, 11472, -1108, 19008, -1108, -1108, 13092, 13092,
     384, -1108,   384, -1108,   384, 14096,   822,  1872,   394,   110,
     246, -1108,   441, -1108, -1108, -1108, -1108, -1108,  2458,  3228,
     384,  2127,  2127, 13092,  2127,  2127,  1646,   246,  3228, 13266,
    1646,   490,   490,   246,  2458,   644,   646,   661,   662,   663,
     665,   647,   635, -1108,   384, -1108,   384, 11822,  9897, 11997,
    9897, 11472,  9372,  9897, 14016,   657,   205, -1108, -1108, -1108,
     125, -1108,  1210, 18379,   438,   173, 16860, -1108,   324, 18334,
   16936,  9372, -1108,  9372, 11472,   142,  9197, 18334,    75, 16024,
    7972, -1108,  9197, 18334,    57, 15644, -1108, -1108,    33, 15720,
   -1108, 11472, 11472,   780,  5347,   782, 17019,  5347, 16100, 19008,
   -1108,   177, -1108,   194, -1108,   196,  1471,   100, -1108, 18334,
    8847, -1108, 18334, -1108, -1108, -1108, 19008, 17095, 17135, -1108,
     613,   671, -1108,   225, 11647, -1108,   224, -1108, 11472, -1108,
     653,    45,   660, -1108,  3644, -1108, -1108, -1108, -1108, -1108,
   -1108, -1108, -1108,  5347,    31, 16176, -1108, -1108, 18334,  5347,
   18334, -1108, 17181,  9897,  9897,  9897,  5347, -1108, -1108,   235,
   -1108, -1108, -1108, -1108, 11472, -1108,    77, 14172, 11472, -1108,
   11122,  7272,  7272, -1108, 11472,   478, 11472,   483,   488, 11472,
    9722,   493,   335, -1108, -1108, -1108, 17287,   672,   666,   667,
   -1108, 19008, 14117, -1108,  9372,   668,  1603, 18334, 19008,   494,
     614, -1108,   676, -1108, -1108, -1108, -1108, -1108,    60,   233,
   -1108, 17333, -1108, 14331, -1108, -1108, 18334, -1108,   669,   670,
   -1108, -1108, -1108, -1108, -1108, -1108, -1108, -1108, 11472,   800,
   17373, 11472,   802, 17525,   247,   674, 17449,  9372, -1108, -1108,
    7972, -1108, -1108, -1108, -1108, -1108, -1108, -1108, -1108, -1108,
   -1108, -1108, -1108,    46,  9897,  9897,    73,    73, -1108, -1108,
   -1108, -1108, 15796, -1108, 14492,  5872, -1108,  6047, -1108,   250,
   -1108, 14572,  6222, -1108,    73,  6397, -1108,    73, -1108,    73,
   -1108,    73, 18334, 18334,  5347, -1108,  5347, 11472, -1108,  5347,
     801,   675,   679, 19008,   509, -1108, -1108,   628, -1108, -1108,
   -1108,   682,   683,   224, 11472,   676,   613, -1108, 11472, -1108,
   -1108, 18334, -1108,  6572, 11472, -1108, -1108, -1108, 19008, -1108,
     690,   509, -1108,  6747,   689,  6922,   691, -1108, 11472,    73,
   15796,    33, 15720,    32, 16252,  9372, -1108, -1108, -1108,  7272,
   -1108, 17565,    82, 16344,   348,   681,  7272, -1108, 11472, -1108,
   -1108,  3017,   349,   354,   253, -1108, -1108, -1108, -1108, 18540,
   18613, -1108, -1108, 18748, -1108,    60,   147, 11472, -1108,   701,
     686, -1108,   606,   606, -1108, -1108, 17610, 12172, 12347, 17717,
   12522, 12697, -1108, 12872, 13047,   266, -1108,   628,   286,   318,
   -1108, -1108, -1108,  5347,  9197, 18334,  9197, 18334,  7972, -1108,
    5347,  9197, 18334, -1108,  9197, 18334, -1108, -1108, -1108, -1108,
   -1108, 18334,   804,  5347, -1108, -1108, -1108, -1108, -1108,  9372,
    9372, -1108, -1108, -1108, -1108,   224, 18334,  5347, -1108,    84,
     687, 11472, -1108,  9197, -1108, 18334,  5347,  9197, -1108, 18334,
    5347, 18334,  5347,  5347, -1108,   310, 11297,  7272,  7272,  7272,
    7272, -1108, -1108, -1108, 17762, 18334, 18522, -1108, -1108, -1108,
     692, 18613, -1108, -1108, -1108, -1108, -1108,   141, -1108,    18,
   -1108,   208, 17808,    17, -1108, -1108, -1108, 11472, 11472, 11472,
   11472, 11472, 11472, 11472, 11472, -1108, -1108, 17019, 14652, 14732,
   -1108, 17019, 14812, 14892,  5347, -1108,   697,   699, -1108, -1108,
     538, -1108,   509, 18334, 14972, -1108, 15052, -1108, -1108, -1108,
   18334,   373,   693,   381,   709, 11472, -1108, -1108, -1108,    67,
   -1108, 11472,   147,   141,   141,   141,   141,   141,   141,   147,
    5172, -1108, -1108, -1108, -1108, -1108, 17914, 17990, 18066, 18142,
   18218, 18294,  5347,  5347,  5347,  5347, -1108,   711,   714, -1108,
    5347,  5347, -1108, -1108, -1108, -1108, 18334, -1108, 18334, -1108,
   -1108, -1108, -1108, -1108, -1108, -1108,   732, -1108, 11472, 11472,
   11472, 11472, 11472, 11472, 17019, 17019, 17019, 17019, -1108, -1108,
   17019, 17019
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   123,   449,   664,   665,   666,
     660,   661,   667,     0,   576,   114,   144,   544,   151,   546,
     576,     0,   150,   479,   477,   114,     0,     0,     0,   265,
     145,   183,   182,   658,     0,     0,     0,   181,     0,   149,
      67,   266,   328,   146,     0,     0,     0,   323,     0,     0,
     153,     0,     0,   593,   568,   668,   154,     0,   329,   538,
     474,     0,     0,     0,   168,   166,   416,   148,   547,   476,
       0,     0,     0,     0,   542,     0,     0,   152,     0,     0,
     124,     0,   659,     0,     0,   467,   147,   303,   540,   478,
     155,     0,     0,   717,     0,     0,   719,     0,   720,   721,
     609,     0,   718,   715,   617,   164,   716,     0,     0,     0,
       0,     4,     0,     5,    10,    11,    12,   326,    50,    51,
      55,     0,    45,    70,    13,    76,    14,    15,    16,    17,
      29,   524,   525,    23,    46,   165,   175,     0,   184,   631,
     176,    18,    31,    30,    20,     0,   260,    19,   627,    22,
      35,    32,    33,   174,   304,     0,   172,     0,     0,   624,
     327,     0,   621,   170,   344,   436,   427,   619,   173,     0,
       0,   171,   636,   615,   614,   618,   528,   526,     0,     0,
     625,   626,   630,   629,   628,     0,   527,     0,   637,   638,
     639,   662,   663,   620,   530,   529,   622,   623,     0,    28,
     546,   145,     0,     0,     0,     0,   547,     0,     0,     0,
       0,   624,   636,   526,   625,   626,   536,   527,   637,   638,
       0,     0,   577,   115,     0,   545,     0,   576,   468,     0,
     475,     0,    21,     0,   512,     0,   518,   121,   125,   137,
     131,   130,   139,   118,   129,   140,   126,   141,   116,   142,
     135,   128,   136,   134,   132,   133,   117,   119,   127,   138,
     143,   122,     0,   120,   192,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    68,    70,
     518,     0,     0,     0,     0,     0,     0,   482,     0,   161,
      40,     0,   295,     0,   294,   702,   597,   594,   595,   596,
       0,   539,   703,     7,   518,   326,   326,   423,   158,   418,
     159,   419,   420,   424,   425,   157,   156,   426,   416,   508,
       0,   331,   332,   334,     0,   417,   507,   336,   495,     0,
       0,     0,   162,   621,   612,   635,   613,     0,     0,    43,
       0,   543,   531,     0,   518,    44,   537,     0,   273,   277,
     274,   277,   541,     0,     0,     0,    54,   704,   706,   607,
     701,   700,     0,    73,    77,     0,     0,   514,     0,     0,
       0,   515,   560,     0,     0,   512,     0,   608,     0,     6,
     301,     0,   182,     0,     0,   302,     0,     0,    48,     9,
      70,    49,    47,    52,     0,    57,     0,     0,    71,     0,
       0,   479,   180,     0,   660,   304,   632,   188,     0,     0,
       0,   300,     0,     0,   321,     0,   314,   416,   436,   434,
     435,   433,   352,   437,   440,   439,   441,     0,   431,   428,
     429,   432,     0,   472,     0,   469,     0,   616,    34,   598,
       0,     0,     0,     0,     0,     0,   705,     0,     0,     0,
       0,     0,     0,   606,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     383,   390,   391,   392,   387,   389,     0,     0,   385,   388,
     386,   384,     0,   394,   393,     0,     0,   518,   518,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    36,    24,
      37,     0,     0,     0,    25,     0,    38,     0,   524,     0,
     519,   520,   523,   313,     0,     0,   202,     0,     0,   201,
       0,   210,     0,     0,     0,   208,     0,   218,     0,     0,
     216,     0,     0,     0,     0,   234,     0,   383,   230,     0,
       0,     0,     0,     0,   244,    26,   486,     0,   487,   489,
       0,   506,     0,   492,     0,     0,   160,    39,     0,   297,
       0,     0,     0,    41,     0,   169,   167,   421,   422,     0,
     417,   436,   416,   409,     0,   408,   510,     0,   107,   633,
     634,   342,     0,   163,     0,    42,     0,     0,   284,   275,
     276,   195,   194,    27,    75,    74,    78,     0,   669,     0,
       0,   654,     0,   656,   561,     0,     0,     0,     0,     0,
       0,     0,     0,   673,     8,     0,     0,     0,    55,     0,
      53,     0,    64,     0,   105,     0,   101,     0,    85,     0,
       0,    91,   177,   304,     0,   190,     0,   187,   271,   305,
       0,   312,   318,     0,   316,   311,   401,     0,   403,   407,
       0,   438,   500,     0,   502,   442,   430,   399,   121,   379,
     125,   377,   131,   130,   129,   126,   381,   141,   142,   128,
     132,   117,   127,   143,   376,   358,   361,   359,   360,   382,
     371,   362,   375,   367,   365,   378,   366,   364,   369,   374,
     363,   368,   372,   373,   370,   380,     0,   355,     0,   399,
     356,   399,   353,     0,   471,     0,   466,   481,   697,   696,
     699,   708,   707,   712,   711,   693,   690,   691,   692,   610,
     680,   123,     0,   650,   651,   124,   649,   648,   604,   684,
     695,   689,   687,   698,   688,   686,   678,   683,   685,   694,
     677,   681,   682,   679,   605,     0,     0,     0,     0,     0,
       0,     0,     0,   710,   709,   714,   713,     0,     0,     0,
       0,     0,     0,     0,   671,   271,   587,   588,   590,   592,
       0,   579,     0,     0,     0,   576,   576,   198,     0,   513,
       0,     0,   533,     0,     0,     0,     0,   550,     0,     0,
       0,   211,     0,   556,     0,     0,   209,   219,     0,     0,
     217,     0,     0,   233,     0,   229,     0,     0,     0,     0,
     532,     0,   247,     0,   245,     0,   490,     0,   505,   504,
       0,   484,   603,   483,   296,   293,     0,     0,     0,   647,
     510,   337,   333,     0,     0,   511,   492,   335,     0,   341,
       0,     0,     0,   534,     0,   278,   670,   611,   655,   516,
     657,   517,   226,     0,     0,     0,   672,   224,   562,     0,
     675,   674,     0,     0,     0,     0,     0,    56,    58,     0,
      60,    62,    66,    65,     0,   100,     0,     0,     0,    93,
       0,     0,    91,    63,   376,   358,   361,   359,   360,   369,
     368,   370,     0,   395,   396,    80,    79,    92,     0,     0,
     306,     0,     0,   268,     0,     0,   326,   322,     0,     0,
     416,   402,   442,   498,   497,   496,   499,   443,   450,   416,
     351,     0,   357,     0,   347,   348,   473,   470,     0,     0,
     113,   111,   112,   110,   109,   108,   645,   646,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   589,   591,
       0,   578,   144,   151,   150,   149,   146,   153,   154,   148,
     152,   147,   155,     0,     0,     0,     0,     0,   480,   193,
     522,   521,     0,   228,     0,     0,   200,     0,   199,     0,
     582,     0,     0,   206,     0,     0,   204,     0,   214,     0,
     212,     0,   242,   241,     0,   236,     0,     0,   232,     0,
     238,     0,   270,     0,     0,   488,   491,   492,   493,   494,
     292,     0,     0,   492,     0,   442,   510,   509,   397,   412,
     410,   343,   286,     0,     0,   285,   288,   535,     0,   279,
     282,     0,   227,     0,     0,     0,     0,   225,     0,     0,
       0,     0,     0,     0,     0,     0,    59,   106,   103,     0,
     102,    88,    87,    86,     0,     0,     0,   178,     0,   179,
     306,   326,     0,     0,     0,   299,   168,   166,   298,   326,
     326,   307,   317,     0,   404,   450,     0,     0,   345,   406,
       0,   354,   399,   399,   652,   653,     0,     0,     0,     0,
       0,     0,   270,     0,     0,     0,   580,   492,   636,   636,
     197,   196,   203,     0,     0,   549,     0,   548,     0,   581,
       0,     0,   555,   207,     0,   554,   205,   215,   213,   235,
     231,   567,   237,     0,    72,   267,   246,   243,   485,   518,
     518,   413,   411,   338,   339,   492,   398,     0,   290,     0,
       0,     0,   280,     0,   222,   564,     0,     0,   220,   563,
       0,   676,     0,     0,    61,     0,     0,     0,    91,     0,
      91,    94,    97,    83,    82,    81,   326,   185,   191,   189,
     272,   326,   309,   308,   320,   324,   464,     0,   465,   452,
     455,     0,   451,     0,   400,   349,   350,     0,     0,     0,
       0,     0,     0,     0,     0,   272,   584,   553,     0,     0,
     583,   559,     0,     0,     0,   240,     0,     0,   415,   414,
     291,   289,     0,   283,     0,   223,     0,   221,   104,    90,
      89,     0,     0,     0,     0,     0,   186,   269,   310,     0,
     463,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   444,   446,   448,   346,   445,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   239,   599,   600,   281,
       0,     0,    96,    99,    95,    98,    84,   325,   454,   456,
     457,   460,   461,   462,   458,   459,   453,   447,     0,     0,
       0,     0,     0,     0,   552,   551,   558,   557,   601,   602,
     566,   565
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1108, -1108, -1108,     3,   -63,  2357, -1108,   467,   763, -1108,
   -1108,  -148,  -116, -1108,   263, -1108, -1108,  -163, -1108, -1108,
     123,   607, -1108,  -575,  2634,   491,  -589, -1108,  -854, -1108,
   -1108,     9, -1108, -1108, -1108,   860, -1108,  3012,  -230, -1108,
   -1108,  -496,  2736, -1107,  -861, -1108, -1108,   252, -1108, -1108,
   -1108, -1108, -1108, -1108, -1108, -1108,   771,   294, -1108,   799,
   -1108, -1108,  -111,  2212, -1108, -1108, -1108, -1108,   544, -1108,
    -135, -1108, -1108, -1108, -1108,   338, -1108, -1108, -1108,  -105,
   -1108,  -370,  -845, -1108, -1108, -1108, -1108, -1108,  -396, -1108,
     673, -1108, -1108, -1108, -1108, -1108,   326, -1108,  2170, -1108,
   -1108, -1108, -1108,   500, -1108, -1108, -1108, -1108,   -26,  -408,
    -168,  -826,  -959,  -672, -1108,   -11, -1108,    -4,   340,   -55,
     598,   -54, -1108, -1108,  -366,  -840, -1108,  -319, -1108,  -158,
    -327,  -314,  -819, -1108, -1108,    19,   214, -1108,  -214,   785,
   -1108,  -169,   368,   108,  -407,  -803,  -623, -1108, -1108, -1108,
    -618,  -534, -1108,  -790,    -7,    42, -1108,  -267,  -526,  -555,
     601, -1108, -1108, -1108, -1108,   912, -1108,   -10, -1108, -1108,
    -139, -1108,  -771, -1108, -1108,  1223,  1335, -1108, -1108, -1108,
   -1108,   -13, -1108,    21,  1587, -1108,  1888,  2000, -1108, -1108,
   -1108, -1108, -1108, -1108, -1108, -1108, -1108,  -471
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   364,   112,   805,   114,   115,   116,   389,
     117,   118,   119,   120,   395,   620,   869,   870,   121,   122,
     123,   400,   124,   125,   126,   365,   897,   627,   898,   127,
     128,   624,   625,   129,   130,   224,   261,   285,   131,   263,
     132,   319,   519,   133,   134,   306,   305,   135,   136,   137,
     138,   139,   140,   636,   901,   141,   142,   267,   143,   275,
     144,   813,   814,   210,   146,   147,   148,   149,   589,   845,
    1031,   150,   151,   841,  1026,   292,   293,   152,   153,   154,
     155,   410,   906,   156,   157,   415,   908,   909,   416,   158,
     211,  1229,   160,   161,   320,   321,   322,  1015,   162,   334,
     582,   838,   163,   164,  1183,   165,   166,   698,   699,   893,
     894,   895,  1019,   920,   418,   647,  1080,   648,   576,   649,
     324,   570,   432,   422,   427,   918,  1244,  1245,   167,  1078,
    1179,  1180,  1181,   168,   169,   434,   435,   704,   170,   171,
     229,   286,   287,   549,   550,   821,   326,   916,   654,   655,
     553,   327,   835,   836,   366,   369,   370,   509,   510,   511,
     212,   173,   174,   175,   176,   213,   178,   221,   222,   770,
     526,   979,   771,   772,   179,   214,   215,   182,   183,   184,
     335,   185,   407,   512,   217,   187,   218,   219,   190,   191,
     192,   193,   376,   194,   195,   196,   197,   198
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     216,   436,   393,   318,   225,   111,   508,   839,   227,   413,
     226,   323,   325,   541,   234,   503,   811,   642,   486,   980,
     233,   268,   270,   272,   700,   276,   774,   924,  1055,   925,
     915,   842,   912,  1020,   405,   634,   291,   564,   294,   295,
    1013,   892,   840,   837,   300,  1071,   301,   551,   302,  1240,
     508,  1050,   650,   328,  1131,  1061,   818,   234,   336,   338,
     340,   341,   342,   329,   871,   288,   346,  -265,   347,   652,
     607,  -266,  1075,   397,   508,   352,  1243,   587,   354,   336,
     721,   554,   357,   865,   358,   763,   307,   359,   360,   982,
     361,   520,   337,   -69,   234,   336,   375,   377,   581,  1240,
    -501,   551,   374,   288,   -68,   288,   309,   975,   539,    71,
     311,   398,   -69,   355,   508,  -287,   288,   505,  1157,  1023,
    1076,   567,  1243,   -68,   403,   199,   406,   612,  -501,   371,
     522,   -69,   528,   531,   312,   506,  1231,   313,   551,    62,
     815,   399,   -68,   520,  1176,  -340,   605,  -503,   600,   314,
    1176,   520,   520,   520,  1158,   613,   520,   914,  -287,    71,
    -501,   -69,  1024,   279,   568,  1232,   439,   833,   317,  -503,
     608,  -503,   -68,  -501,   601,  1134,  1208,   520,  1077,  1096,
     725,   555,  1241,   505,   236,   608,  -501,   386,   280,   491,
     492,   493,   552,  -503,   346,   234,   375,  -503,   609,  1007,
    1071,   534,   495,   386,  1128,   831,  1033,  1152,  1172,  1173,
    1132,   866,  -287,   764,   653,  1166,  1025,   502,   558,  -503,
     751,   752,   727,   336,   380,  -503,  1135,   386,   971,  -503,
    1163,   505,   646,  -501,   606,   610,   944,   386,  -501,  1177,
     390,   386,  -503,   386,  1178,   386,   552,  -503,   419,   602,
    1178,   446,   948,   505,   386,  -503,   816,   508,   508,   452,
     307,   910,   296,   899,   701,  -503,   903,   336,   420,   386,
     199,   578,   950,   904,  -248,   603,   594,   442,   421,   721,
     309,     3,   817,   552,   311,    24,   386,   911,   543,   505,
     385,   336,  1054,   220,  1196,   949,   722,   393,   951,   633,
    1017,   423,   342,   346,  1222,  1071,  1224,  -503,   312,   723,
    1228,   313,   508,    62,    87,   973,   199,   579,   580,   774,
    -319,   297,   872,   314,   783,   428,   705,  -315,   223,   269,
     271,   336,  1209,  1233,   778,   443,    60,  1200,   724,   444,
     298,  1003,   317,   816,   706,  1097,   826,  -249,    69,   820,
    1002,  -315,   424,   597,   318,   599,   508,   299,  1230,   604,
    1234,  -255,  1235,  1236,   425,  1237,  1238,  1004,   596,  1005,
     615,   616,   816,    89,   234,  1018,   429,   598,  1064,   725,
     617,   644,  1045,   426,   626,   726,   628,   446,   430,   508,
     277,  1133,   450,   278,   783,   452,   234,  1108,  1016,   455,
     783,  -254,   639,   373,   228,   645,  -405,   431,  1046,   230,
    1185,  1186,   236,   783,  1270,  1271,  1272,  1273,  1274,  1275,
    1092,  1095,   902,  1109,   264,   442,  1170,   708,   709,   710,
     712,   714,   715,  -586,   716,   717,   718,   719,   720,  1195,
     728,   729,   730,   731,   732,   733,   734,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,  1056,  -263,  -586,
    1155,  -253,   555,   336,   336,  -585,   489,   874,   878,   336,
     490,   555,   336,   336,   336,   336,   754,   756,   610,   774,
     707,  1218,  1056,   443,   773,   875,   879,   444,   776,   968,
     871,  -585,   779,   303,   780,  1056,   705,   746,   747,   280,
    1057,   555,   234,   748,   787,   789,   749,   750,   785,   318,
     793,   795,  1072,  1161,  1168,   700,   799,   323,   325,  1169,
    1056,   442,   487,   806,   488,   446,   808,   965,  1056,   336,
     450,   490,   508,   452,  -250,   446,   447,   455,  1262,   819,
     450,   822,   451,   452,  -257,   294,  1264,   455,   827,   828,
     348,   350,    42,   508,   462,  -326,  -573,   565,   566,   328,
     466,   467,   468,  -643,   328,  -643,  -261,  -719,  1221,  -258,
    1223,  -719,  -720,   336,   304,  -326,  -720,  -721,    58,   443,
    -252,  -721,  -716,   444,   779,  -326,  -716,   336,  -644,   336,
    -644,  -643,   343,  -643,   855,   779,   858,  -256,   860,   862,
     307,   344,  -640,   172,  -640,  -642,   336,  -642,  1242,  -262,
    -264,   635,  -641,   928,  -641,   929,  -251,   896,   896,  -259,
     309,   371,   356,   371,   311,   394,   596,   172,   396,   172,
     907,   446,   408,   409,    94,   417,   450,   774,   451,   452,
     328,   307,   849,   455,   851,  -570,  1048,   727,   312,  -569,
     462,   313,   496,   378,  1242,   984,   987,   507,   468,   989,
     991,   309,   437,   314,   438,   311,  -574,   497,  -575,  -572,
    -571,   499,   504,   398,   508,   159,   572,  1174,   269,   386,
     172,   373,   317,   921,   172,   172,   513,   515,   614,   312,
     926,   542,   313,   561,    62,   562,   577,   571,   583,   159,
     586,   159,   593,   618,   314,   629,   380,   619,   172,   640,
      23,    24,   641,   172,   651,  1034,  1036,   508,   643,   381,
     656,    31,   382,   317,   703,   452,   745,    37,   762,   781,
     782,   783,   784,  1063,    42,   790,   801,   711,   713,   804,
     802,   807,   809,   810,   787,   940,   793,   943,   806,   336,
     946,   858,   159,   820,   829,  1083,   159,   159,  1070,   834,
      58,  1073,    60,   843,   844,   966,   967,   853,   336,   859,
     336,   972,   385,   974,    69,   900,   505,   773,   919,   981,
     159,   917,  -123,   753,   755,   159,  -144,  -151,   992,   993,
    -150,  -149,  -146,    85,  -153,   775,    87,  -154,  -148,    89,
    -152,  -124,   970,  -147,  -155,   922,   539,   336,   923,   930,
     937,   931,   788,   539,   994,   508,   996,  1014,   794,  1056,
     936,   328,   172,   798,  1022,  1021,   932,   933,   934,   947,
     935,  1059,  1087,  1027,  1090,  1123,  1141,  1060,  1204,  1065,
    1124,  1009,  1084,  1069,  1125,  1085,  1162,   318,   910,  1093,
    1040,  1042,  1044,   442,  1129,  1130,   318,  1227,  1263,  1184,
    1212,   626,  1206,  1207,  1146,  1051,  1150,  1053,   896,   896,
    1257,   357,  1258,   358,  1265,  1288,   360,   361,  1289,  1232,
     388,   867,  1154,  1047,   172,   231,   540,   623,   392,   172,
     353,   336,  1126,  1140,   159,   590,   824,  1082,   832,   508,
     508,   854,   984,   987,  1034,  1036,  1074,   637,  1079,   830,
    1267,   443,  1276,  1070,   177,   444,   569,  1175,  1269,   927,
    1062,  1171,   402,   823,  1006,  1086,     0,     0,  1089,     0,
       0,     0,     0,     0,   336,     0,     0,   773,   177,     0,
     177,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   712,   754,     0,   393,     0,   159,   393,     0,     0,
       0,   159,  1105,   446,  1107,     0,   172,     0,   450,  1112,
     451,   452,  1115,     0,     0,   455,     0,     0,     0,     0,
       0,     0,   462,     0,  1121,     0,     0,   172,   466,   467,
     468,   177,     0,     0,     0,   177,   177,   172,  1069,     0,
       0,   328,     0,     0,     0,  1136,     0,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,  1139,  1070,   177,
    1145,     0,  1149,     0,   177,  1151,     0,     0,     0,     0,
     539,   539,   336,   702,   539,   539,   896,     0,   159,     0,
       0,     0,     0,  1164,     0,  1165,   539,     0,   539,     0,
       0,     0,   939,     0,   942,   393,     0,   945,     0,   159,
       0,     0,     0,     0,  1182,     0,     0,     0,     0,   159,
       0,     0,     0,     0,  1105,  1107,     0,  1112,  1115,     0,
    1145,  1149,     0,     0,     0,     0,     0,     0,     0,     0,
    1197,  1198,     0,  1199,     0,   773,     0,  1201,  1202,     0,
       0,  1203,     0,  1069,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   336,   336,   172,     0,
       0,     0,     0,     0,   172,     0,     0,     0,  1213,     0,
    1214,     0,     0,   177,  1216,   172,     0,   172,     0,     0,
       0,     0,     0,  1220,   896,   896,   896,   896,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1039,  1041,  1043,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1197,  1246,  1247,  1201,  1248,  1249,
    1250,  1251,     0,     0,     0,     0,     0,     0,     0,     0,
     159,     0,     0,     0,     0,   177,   159,     0,     0,     0,
     177,     0,     0,     0,     0,     0,   172,   159,     0,   159,
     172,     0,  1266,   721,     0,     0,     0,     0,  1268,     0,
       0,     0,     0,     0,   172,   180,     0,   952,     0,     0,
     953,     0,     0,     0,     0,   954,     0,     0,     0,  1284,
    1285,  1286,  1287,  1277,     0,   201,     0,  1290,  1291,   180,
       0,   180,     0,     0,     0,   955,     0,     0,   711,   753,
       0,     0,   956,     0,     0,  1284,  1285,  1286,  1287,  1290,
    1291,     0,   957,     0,     0,     0,     0,   177,   159,     0,
     958,     0,   159,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   159,   959,   177,     0,
       0,     0,   180,     0,     0,     0,   180,   180,   177,   960,
       0,     0,     0,   725,     0,     0,     0,   905,     0,     0,
     961,     0,     0,     0,     0,     0,   962,     0,     0,     0,
     180,     0,     0,     0,     0,   180,     0,   181,  -306,     0,
       0,     0,  -306,  -306,     0,     0,     0,     0,     0,     0,
       0,  -306,     0,  -306,  -306,     0,     0,     0,     0,  -306,
       0,   181,     0,   181,     0,     0,  -306,     0,     0,  -306,
       0,     0,     0,   963,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -306,
       0,     0,  -306,     0,  -306,     0,  -306,     0,  -306,  -306,
       0,  -306,     0,     0,  -306,   172,  -306,     0,   172,     0,
       0,     0,     0,     0,   181,     0,     0,     0,   181,   181,
       0,     0,     0,     0,     0,  -306,     0,     0,  -306,   177,
       0,  -306,     0,     0,     0,   177,     0,     0,     0,     0,
       0,     0,   181,     0,   180,     0,   177,   181,   177,     0,
       0,     0,     0,     0,   172,     0,  -306,     0,     0,     0,
     172,     0,     0,     0,     0,     0,     0,   172,     0,     0,
       0,     0,     0,     0,   237,     0,     0,   159,     0,     0,
     159,  -306,     0,     0,     0,     0,     0,  -306,   238,   239,
       0,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   180,     0,     0,     0,
     243,   180,     0,     0,     0,     0,   244,   177,     0,     0,
     245,   177,     0,   246,     0,     0,   159,     0,     0,     0,
       0,     0,   159,   247,     0,   177,     0,     0,     0,   159,
     248,   249,     0,     0,     0,     0,     0,     0,   250,     0,
       0,     0,     0,     0,     0,     0,   181,     0,   251,     0,
       0,     0,     0,     0,     0,  1098,  1099,   252,   253,     0,
     254,     0,   255,     0,   256,     0,   172,   257,   172,     0,
       0,   258,   546,   172,   259,     0,   172,   260,   180,   186,
       0,     0,     0,     0,     0,   172,     0,   172,     0,     0,
     172,     0,     0,     0,     0,     0,     0,     0,     0,   180,
       0,     0,     0,   186,     0,   186,     0,     0,   181,   180,
       0,     0,     0,   181,   172,   380,     0,     0,     0,    23,
      24,     0,     0,     0,   172,     0,   172,     0,   381,     0,
      31,   382,     0,   547,     0,     0,    37,     0,   159,     0,
     159,     0,     0,    42,     0,   159,     0,     0,   159,     0,
       0,     0,     0,     0,     0,     0,   186,   159,     0,   159,
     186,   186,   159,     0,     0,     0,     0,   442,     0,    58,
       0,    60,     0,    62,     0,  1066,     0,     0,  1067,     0,
       0,   385,     0,    69,   186,     0,   159,     0,     0,   186,
     181,     0,     0,     0,   172,     0,   159,     0,   159,     0,
       0,   172,    85,     0,     0,    87,   177,     0,    89,   177,
       0,   181,     0,     0,   172,     0,     0,     0,     0,     0,
       0,   181,     0,     0,     0,   443,     0,     0,   172,   444,
     180,     0,     0,    94,     0,     0,   180,   172,     0,     0,
       0,   172,     0,   172,   172,     0,     0,   180,     0,   180,
       0,     0,     0,     0,     0,   177,     0,     0,   105,     0,
       0,   177,     0,     0,  1068,     0,   159,     0,   177,     0,
       0,     0,     0,   159,     0,     0,     0,   446,   447,     0,
     448,   449,   450,     0,   451,   452,   159,     0,     0,   455,
       0,     0,     0,     0,     0,   172,   462,     0,   186,     0,
     159,     0,   466,   467,   468,     0,     0,     0,     0,   159,
       0,     0,     0,   159,     0,   159,   159,     0,   180,     0,
       0,     0,   180,     0,     0,     0,     0,     0,     0,     0,
       0,   172,     0,     0,     0,     0,   180,     0,     0,     0,
       0,     0,   181,   172,   172,   172,   172,     0,   181,     0,
       0,   172,   172,     0,     0,     0,     0,     0,     0,   181,
     186,   181,     0,     0,     0,   186,     0,   159,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   177,     0,   177,
     188,     0,     0,     0,   177,     0,     0,   177,     0,     0,
       0,     0,     0,   442,     0,     0,   177,     0,   177,     0,
       0,   177,     0,   159,   188,     0,   188,     0,     0,     0,
       0,     0,     0,     0,     0,   159,   159,   159,   159,     0,
       0,     0,     0,   159,   159,   177,     0,     0,     0,     0,
     181,     0,     0,     0,   181,   177,     0,   177,     0,     0,
       0,     0,   186,     0,     0,     0,     0,     0,   181,     0,
       0,   443,     0,     0,     0,   444,     0,   188,     0,     0,
       0,   188,   188,   186,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   186,     0,     0,     0,     0,   380,     0,
      21,     0,    23,    24,     0,   188,     0,     0,    28,     0,
     188,   381,   189,    31,   382,     0,   383,   384,    36,    37,
       0,     0,     0,   446,   447,   177,    42,   449,   450,     0,
     451,   452,   177,     0,     0,   455,   189,   180,   189,     0,
     180,   -67,   462,     0,     0,   177,     0,     0,   466,   467,
     468,     0,    58,     0,    60,     0,     0,     0,    64,   177,
     -67,    65,     0,     0,   385,     0,    69,     0,   177,     0,
       0,     0,   177,     0,   177,   177,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    85,   180,     0,    87,   189,
       0,    89,   180,   189,   189,    91,     0,     0,     0,   180,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   186,     0,     0,   189,     0,   188,
     186,     0,   189,     0,     0,     0,   177,     0,     0,     0,
       0,   186,     0,   186,     0,     0,     0,     0,     0,     0,
       0,   105,     0,     0,     0,     0,   386,     0,     0,   181,
     387,     0,   181,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   177,     0,     0,     0,     0,     0,   442,     0,
       0,     0,     0,     0,   177,   177,   177,   177,     0,     0,
       0,   188,   177,   177,     0,     0,   188,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   181,     0,
       0,     0,   186,     0,   181,     0,   186,     0,   180,     0,
     180,   181,     0,     0,     0,   180,     0,     0,   180,     0,
     186,     0,     0,     0,   145,     0,   443,   180,     0,   180,
     444,   189,   180,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   145,     0,
     145,   333,     0,     0,     0,     0,   180,     0,     0,     0,
       0,     0,     0,   188,     0,     0,   180,     0,   180,     0,
       0,     0,   333,     0,     0,     0,     0,     0,   446,   447,
       0,   448,   449,   450,   188,   451,   452,   453,   333,     0,
     455,     0,     0,   189,   188,     0,   461,   462,   189,     0,
     465,   145,     0,   466,   467,   468,     0,     0,     0,     0,
       0,     0,     0,     0,   469,     0,     0,     0,     0,     0,
     181,     0,   181,     0,     0,     0,     0,   181,     0,   145,
     181,     0,     0,     0,   145,     0,   180,     0,     0,   181,
       0,   181,     0,   180,   181,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   180,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   181,   113,
     180,     0,     0,     0,     0,   189,     0,     0,   181,   180,
     181,     0,     0,   180,     0,   180,   180,     0,     0,     0,
       0,     0,     0,   232,     0,   235,   189,     0,     0,     0,
       0,   186,     0,     0,   186,     0,   189,     0,     0,     0,
       0,     0,     0,     0,     0,   188,   333,     0,     0,     0,
       0,   188,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   188,     0,   188,     0,     0,   180,     0,     0,
       0,     0,     0,   145,     0,     0,   345,     0,   181,     0,
     186,     0,     0,     0,     0,   181,   186,     0,     0,     0,
     333,     0,     0,   186,     0,     0,     0,     0,   181,     0,
       0,     0,     0,   180,   113,     0,     0,     0,     0,   379,
       0,     0,   181,     0,   333,   180,   180,   180,   180,     0,
       0,   181,     0,   180,   180,   181,     0,   181,   181,   442,
       0,     0,     0,   188,     0,   145,     0,   188,     0,     0,
     145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   188,     0,     0,   333,     0,     0,   189,     0,     0,
       0,     0,     0,   189,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   189,     0,   189,     0,     0,   181,
       0,     0,     0,     0,     0,     0,     0,   443,     0,     0,
       0,   444,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   186,     0,   186,     0,     0,     0,     0,   186,
       0,     0,   186,     0,     0,   181,     0,   145,   498,     0,
       0,   186,     0,   186,     0,     0,   186,   181,   181,   181,
     181,     0,     0,     0,     0,   181,   181,     0,   145,   446,
     447,     0,   448,   449,   450,   189,   451,   452,   145,   189,
     186,   455,     0,     0,     0,     0,     0,   461,   462,     0,
     186,   465,   186,   189,   466,   467,   468,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     545,     0,     0,     0,     0,   556,   333,   333,     0,     0,
       0,     0,   333,     0,     0,   333,   333,   333,   333,   470,
     471,   472,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   289,     0,     0,     0,     0,     0,     0,
     186,     0,   188,     0,     0,   188,     0,   186,     0,     0,
       0,     0,     0,   485,     0,     0,  -640,   289,  -640,     0,
     186,     0,   333,     0,     0,     0,     0,   349,   351,     0,
       0,     0,   113,     0,   186,     0,     0,     0,     0,   145,
       0,     0,     0,   186,     0,   145,     0,   186,     0,   186,
     186,   188,     0,   113,     0,     0,   145,   188,   145,     0,
       0,   391,     0,   113,   188,     0,   333,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     333,     0,   333,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   290,     0,     0,     0,   333,
       0,   186,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   189,     0,     0,   189,     0,   339,
       0,     0,     0,     0,     0,     0,     0,   145,     0,     0,
       0,   145,     0,     0,     0,     0,     0,   186,     0,     0,
       0,     0,     0,     0,     0,   145,     0,     0,     0,   186,
     186,   186,   186,     0,     0,     0,     0,   186,   186,     0,
       0,     0,     0,   189,     0,     0,     0,     0,     0,   189,
     500,     0,     0,   188,     0,   188,   189,     0,     0,     0,
     188,     0,     0,   188,   556,     0,     0,     0,     0,     0,
     556,     0,   188,     0,   188,     0,     0,   188,     0,     0,
       0,   803,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   289,   289,   289,   289,   289,   289,     0,     0,   535,
     538,   188,     0,     0,     0,     0,   544,     0,     0,     0,
       0,   188,     0,   188,     0,   289,     0,   289,     0,     0,
       0,     0,   333,     0,   289,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   333,     0,   333,     0,     0,     0,     0,     0,     0,
       0,     0,   852,     0,     0,     0,   857,     0,     0,     0,
       0,     0,     0,     0,   289,   189,     0,   189,     0,     0,
     113,     0,   189,     0,     0,   189,     0,   289,   289,     0,
     333,   188,     0,     0,   189,     0,   189,     0,   188,   189,
       0,     0,     0,   516,     0,   521,   525,   527,   530,     0,
       0,   188,     0,     0,     0,     0,   145,     0,     0,   145,
       0,     0,     0,   189,     0,   188,     0,   557,     0,   559,
       0,     0,     0,   189,   188,   189,   563,     0,   188,   380,
     188,   188,   262,    23,    24,     0,     0,     0,     0,     0,
       0,     0,   381,     0,    31,   382,   281,   282,   283,     0,
      37,     0,     0,     0,     0,   145,     0,    42,     0,     0,
       0,   145,     0,     0,   333,     0,   585,     0,   145,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   591,
     592,     0,   188,    58,     0,    60,     0,    62,     0,  1066,
       0,     0,  1067,   189,     0,   385,     0,    69,     0,     0,
     189,     0,     0,     0,     0,     0,     0,   333,     0,     0,
       0,     0,     0,   189,     0,     0,    85,     0,   188,    87,
       0,     0,    89,     0,     0,     0,   289,   189,     0,     0,
     188,   188,   188,   188,     0,     0,   189,     0,   188,   188,
     189,     0,   189,   189,     0,     0,   289,    94,     0,     0,
     289,   995,   289,     0,   998,   289,     0,   411,     0,   414,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,   105,     0,     0,     0,     0,   145,  1167,   145,
       0,     0,     0,     0,   145,     0,     0,   145,     0,     0,
       0,     0,     0,     0,   189,     0,   145,     0,   145,     0,
    1032,   145,     0,     0,     0,   333,  1037,   494,     0,     0,
       0,     0,     0,   857,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   145,     0,     0,   777,     0,
     189,     0,     0,     0,     0,   145,     0,   145,     0,   289,
     289,     0,   189,   189,   189,   189,     0,     0,   791,   442,
     189,   189,   796,     0,   797,     0,     0,   800,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   514,     0,     0,
       0,     0,     0,     0,     0,   532,   533,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   548,     0,     0,   333,
     333,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   145,     0,   443,     0,     0,
       0,   444,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   556,     0,   556,   145,   575,     0,     0,   556,
       0,     0,   556,     0,     0,     0,     0,     0,     0,   145,
       0,  1119,   525,  1120,     0,     0,  1122,     0,   145,     0,
       0,     0,   145,     0,   145,   145,     0,     0,     0,   446,
     447,     0,   448,   449,   450,     0,   451,   452,   453,     0,
    1138,   455,   456,   457,     0,   459,   460,   461,   462,     0,
    1144,   465,  1148,   262,   466,   467,   468,     0,     0,     0,
       0,     0,     0,     0,     0,   469,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   145,     0,     0,     0,
     638,     0,   289,   289,     0,   414,     0,     0,   289,   289,
       0,     0,   289,   289,     0,     0,     0,     0,     0,     0,
       0,     0,  1000,     0,   697,     0,     0,     0,     0,     0,
       0,     0,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   145,   145,   145,   145,     0,     0,
     237,     0,   145,   145,     0,     0,     0,     0,  1029,     0,
    1205,     0,     0,     0,   238,   239,     0,   240,     0,     0,
       0,     0,   241,     0,  1210,     0,     0,     0,     0,     0,
     242,     0,     0,  1215,     0,     0,   243,  1217,   765,  1144,
    1148,     0,   244,     0,     0,     0,   245,     0,     0,   246,
       0,     0,     0,     0,   976,   978,     0,     0,     0,   247,
     983,   986,     0,     0,   988,   990,   248,   249,     0,     0,
       0,     0,     0,     0,   250,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   251,   812,     0,     0,     0,   548,
       0,  1256,     0,   252,   253,     0,   254,     0,   255,     0,
     256,     0,   825,   257,     0,     0,     0,   258,   573,     0,
     259,   575,     0,   260,     0,     0,   548,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   113,     0,     0,
     289,   289,     0,     0,     0,     0,   289,     0,   538,     0,
       0,     0,     0,     0,     0,   538,     0,     0,   289,     0,
       0,   289,     0,   289,     0,   289,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1127,   574,
       0,   883,     0,     0,     0,     0,     0,   237,     0,     0,
       0,     0,     0,   440,     0,     0,     0,     0,     0,     0,
     441,   238,   239,     0,   240,  1142,     0,     0,     0,   241,
       0,     0,   442,   289,   289,   289,   289,   242,     0,     0,
       0,     0,     0,   243,     0,     0,     0,     0,   501,   244,
       0,     0,     0,   245,     0,     0,   246,     0,     0,     0,
       0,     0,  1100,  1101,     0,     0,   247,     0,  1102,     0,
       0,     0,     0,   248,   249,     0,     0,   433,     0,     0,
    1113,   250,     0,  1116,     0,  1117,     0,  1118,     0,     0,
     443,   251,     0,     0,   444,     0,     0,     0,     0,     0,
     252,   253,     0,   254,     0,   255,     0,   256,     0,   440,
     257,     0,     0,     0,   258,     0,   441,   259,     0,     0,
     260,   220,     0,     0,     0,   445,     0,     0,   442,   523,
       0,     0,     0,   289,     0,   976,   978,   983,   986,     0,
       0,     0,   446,   447,   524,   448,   449,   450,     0,   451,
     452,   453,     0,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,     0,     0,   466,   467,   468,
       0,     0,     0,     0,   386,     0,  1028,     0,   469,     0,
       0,  1001,     0,     0,     0,     0,   443,     0,   548,     0,
     444,     0,   538,   538,     0,     0,   538,   538,  1010,     0,
       0,     0,     0,     0,     0,     0,  1259,     0,   538,     0,
     538,     0,     0,     0,     0,     0,  1030,   520,     0,     0,
       0,   445,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1211,     0,     0,   446,   447,
       0,   448,   449,   450,     0,   451,   452,   453,     0,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,     0,     0,   466,   467,   468,     0,   386,     0,     0,
       0,     0,     0,   433,   469,     0,     0,     0,     0,     0,
     414,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   697,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    -2,     4,     0,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -67,     0,    40,    41,    42,     0,    43,
    -326,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -67,    53,    54,   812,    55,    56,    57,     0,
    -326,     0,     0,    58,    59,    60,    61,    62,    63,    64,
    -326,   -67,    65,    66,    67,    68,     0,    69,    70,    71,
    1030,    72,    73,    74,    75,    76,    77,    78,    79,     0,
      80,    81,     0,    82,    83,    84,    85,    86,     0,    87,
      88,   -67,    89,    90,     0,     0,    91,     0,    92,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   414,     0,    94,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,   105,     0,     0,   106,     0,   107,     0,   108,
       0,   109,     0,     4,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,  -671,     0,    13,    14,    15,    16,
      17,  -671,    18,     0,    19,    20,    21,    22,    23,    24,
      25,    26,    27,  -671,    28,    29,  -671,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -67,     0,
      40,    41,    42,     0,    43,  -326,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -67,    53,    54,
       0,    55,    56,    57,     0,  -326,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -326,   -67,    65,    66,    67,
      68,  -671,    69,    70,    71,  -671,    72,    73,    74,    75,
      76,    77,    78,    79,     0,    80,    81,     0,    82,    83,
      84,    85,    86,     0,    87,    88,   -67,    89,    90,     0,
       0,    91,     0,    92,     0,     0,  -671,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,  -671,    97,  -671,  -671,  -671,  -671,
    -671,  -671,  -671,     0,  -671,  -671,  -671,  -671,  -671,  -671,
    -671,  -671,  -671,  -671,  -671,  -671,   104,   105,  -671,  -671,
    -671,     0,   107,  -671,   108,     0,   109,     0,   362,  -671,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -67,     0,    40,    41,    42,     0,    43,
    -326,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -67,    53,    54,     0,    55,    56,    57,     0,
    -326,     0,     0,    58,    59,    60,    61,    62,    63,    64,
    -326,   -67,    65,    66,    67,    68,     0,    69,    70,    71,
       0,    72,    73,    74,    75,    76,    77,    78,    79,     0,
      80,    81,     0,    82,    83,    84,    85,    86,     0,    87,
      88,   -67,    89,    90,     0,     0,    91,     0,    92,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,   105,     0,     0,   106,     0,   107,   363,   108,
       0,   109,     0,     4,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -67,     0,
      40,    41,    42,     0,    43,  -326,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -67,    53,    54,
       0,    55,    56,    57,     0,  -326,     0,     0,    58,    59,
      60,    61,    62,    63,    64,  -326,   -67,    65,    66,    67,
      68,     0,    69,    70,    71,     0,    72,    73,    74,    75,
      76,    77,    78,    79,     0,    80,    81,     0,    82,    83,
      84,    85,    86,     0,    87,    88,   -67,    89,    90,     0,
       0,    91,     0,    92,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,   105,     0,     0,
     106,     0,   107,   595,   108,     0,   109,     0,   621,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -67,     0,    40,    41,    42,     0,    43,
    -326,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -67,    53,    54,     0,    55,    56,    57,     0,
    -326,     0,     0,    58,    59,    60,    61,    62,    63,    64,
    -326,   -67,    65,    66,    67,    68,     0,    69,    70,    71,
       0,    72,    73,    74,    75,    76,    77,    78,    79,     0,
      80,    81,     0,    82,    83,    84,    85,    86,     0,    87,
      88,   -67,    89,    90,     0,     0,    91,     0,    92,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,   105,     0,     0,   106,     0,   107,   622,   108,
       0,   109,     0,     4,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -67,     0,
      40,    41,    42,     0,    43,  -326,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -67,    53,    54,
       0,    55,    56,    57,     0,  -326,     0,     0,    58,    59,
      60,    61,    62,    63,    64,  -326,   -67,    65,    66,    67,
      68,     0,    69,    70,    71,     0,    72,    73,    74,    75,
      76,    77,    78,    79,     0,    80,    81,     0,    82,    83,
      84,    85,    86,     0,    87,    88,   -67,    89,    90,     0,
       0,    91,     0,    92,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,   105,     0,     0,
     106,     0,   107,   873,   108,     0,   109,     0,     4,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -67,     0,    40,    41,    42,     0,    43,
    -326,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -67,    53,    54,     0,    55,    56,    57,     0,
    -326,     0,     0,    58,    59,    60,    61,   378,    63,    64,
    -326,   -67,    65,    66,    67,    68,     0,    69,    70,    71,
       0,    72,    73,    74,    75,    76,    77,    78,    79,     0,
      80,    81,     0,    82,    83,    84,    85,    86,     0,    87,
      88,   -67,    89,    90,     0,     0,    91,     0,    92,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,   105,     0,     0,   106,     0,   107,     0,   108,
       0,   109,     0,     4,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -67,     0,
      40,    41,    42,     0,    43,  -326,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -67,    53,    54,
       0,    55,    56,    57,     0,  -326,     0,     0,    58,    59,
      60,    61,    62,    63,    64,  -326,   -67,    65,    66,    67,
      68,     0,    69,    70,    71,     0,    72,    73,    74,    75,
      76,    77,    78,    79,     0,    80,    81,     0,    82,    83,
      84,    85,    86,     0,    87,    88,   -67,    89,    90,     0,
       0,    91,     0,    92,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,   105,     0,     0,
     106,     0,   107,     0,   108,     0,   109,     0,     4,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -67,     0,    40,    41,    42,     0,    43,
    -326,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -67,    53,    54,     0,    55,    56,    57,     0,
    -326,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -326,   -67,    65,    66,    67,    68,     0,    69,    70,    71,
       0,    72,    73,    74,    75,    76,    77,    78,    79,     0,
      80,    81,     0,    82,    83,    84,    85,    86,     0,    87,
      88,   -67,    89,    90,     0,     0,    91,     0,    92,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,   105,     0,     0,   106,     0,   107,     0,   108,
       0,   109,     0,     4,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,   786,    39,   -67,     0,
      40,    41,    42,     0,    43,  -326,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -67,    53,    54,
       0,    55,    56,    57,     0,  -326,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -326,   -67,    65,    66,    67,
      68,     0,    69,    70,    71,     0,    72,    73,    74,    75,
      76,    77,    78,    79,     0,    80,    81,     0,    82,    83,
      84,    85,    86,     0,    87,    88,   -67,    89,    90,     0,
       0,    91,     0,    92,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,   105,     0,     0,
     106,     0,   107,     0,   108,     0,   109,     0,     4,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,   792,    39,   -67,     0,    40,    41,    42,     0,    43,
    -326,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -67,    53,    54,     0,    55,    56,    57,     0,
    -326,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -326,   -67,    65,    66,    67,    68,     0,    69,    70,    71,
       0,    72,    73,    74,    75,    76,    77,    78,    79,     0,
      80,    81,     0,    82,    83,    84,    85,    86,     0,    87,
      88,   -67,    89,    90,     0,     0,    91,     0,    92,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,   105,     0,     0,   106,     0,   107,     0,   108,
       0,   109,     0,     4,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,  1104,    39,   -67,     0,
      40,    41,    42,     0,    43,  -326,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -67,    53,    54,
       0,    55,    56,    57,     0,  -326,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -326,   -67,    65,    66,    67,
      68,     0,    69,    70,    71,     0,    72,    73,    74,    75,
      76,    77,    78,    79,     0,    80,    81,     0,    82,    83,
      84,    85,    86,     0,    87,    88,   -67,    89,    90,     0,
       0,    91,     0,    92,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,   105,     0,     0,
     106,     0,   107,     0,   108,     0,   109,     0,     4,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,  1106,    39,   -67,     0,    40,    41,    42,     0,    43,
    -326,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -67,    53,    54,     0,    55,    56,    57,     0,
    -326,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -326,   -67,    65,    66,    67,    68,     0,    69,    70,    71,
       0,    72,    73,    74,    75,    76,    77,    78,    79,     0,
      80,    81,     0,    82,    83,    84,    85,    86,     0,    87,
      88,   -67,    89,    90,     0,     0,    91,     0,    92,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,   105,     0,     0,   106,     0,   107,     0,   108,
       0,   109,     0,     4,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,  1111,    39,   -67,     0,
      40,    41,    42,     0,    43,  -326,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -67,    53,    54,
       0,    55,    56,    57,     0,  -326,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -326,   -67,    65,    66,    67,
      68,     0,    69,    70,    71,     0,    72,    73,    74,    75,
      76,    77,    78,    79,     0,    80,    81,     0,    82,    83,
      84,    85,    86,     0,    87,    88,   -67,    89,    90,     0,
       0,    91,     0,    92,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,   105,     0,     0,
     106,     0,   107,     0,   108,     0,   109,     0,     4,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,  1114,    39,   -67,     0,    40,    41,    42,     0,    43,
    -326,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -67,    53,    54,     0,    55,    56,    57,     0,
    -326,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -326,   -67,    65,    66,    67,    68,     0,    69,    70,    71,
       0,    72,    73,    74,    75,    76,    77,    78,    79,     0,
      80,    81,     0,    82,    83,    84,    85,    86,     0,    87,
      88,   -67,    89,    90,     0,     0,    91,     0,    92,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,   105,     0,     0,   106,     0,   107,     0,   108,
       0,   109,     0,     4,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,  1137,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -67,     0,
      40,    41,    42,     0,    43,  -326,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -67,    53,    54,
       0,    55,    56,    57,     0,  -326,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -326,   -67,    65,    66,    67,
      68,     0,    69,    70,    71,     0,    72,    73,    74,    75,
      76,    77,    78,    79,     0,    80,    81,     0,    82,    83,
      84,    85,    86,     0,    87,    88,   -67,    89,    90,     0,
       0,    91,     0,    92,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,   105,     0,     0,
     106,     0,   107,     0,   108,     0,   109,     0,     4,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,  1143,    39,   -67,     0,    40,    41,    42,     0,    43,
    -326,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -67,    53,    54,     0,    55,    56,    57,     0,
    -326,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -326,   -67,    65,    66,    67,    68,     0,    69,    70,    71,
       0,    72,    73,    74,    75,    76,    77,    78,    79,     0,
      80,    81,     0,    82,    83,    84,    85,    86,     0,    87,
      88,   -67,    89,    90,     0,     0,    91,     0,    92,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,   105,     0,     0,   106,     0,   107,     0,   108,
       0,   109,     0,     4,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,  1147,    39,   -67,     0,
      40,    41,    42,     0,    43,  -326,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -67,    53,    54,
       0,    55,    56,    57,     0,  -326,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -326,   -67,    65,    66,    67,
      68,     0,    69,    70,    71,     0,    72,    73,    74,    75,
      76,    77,    78,    79,     0,    80,    81,     0,    82,    83,
      84,    85,    86,     0,    87,    88,   -67,    89,    90,     0,
       0,    91,     0,    92,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,   105,     0,     0,
     106,     0,   107,     0,   108,     0,   109,     0,   913,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
     308,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   204,    39,     0,     0,     0,   310,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,   315,    76,    77,    78,   316,     0,
      80,     0,     0,    82,     0,     0,     0,    86,     0,     0,
      88,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   208,     0,   108,
       0,   209,     0,     0,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,   659,     0,    13,     0,     0,    16,
      17,   661,    18,     0,   200,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   201,     0,     0,
       0,    33,   202,   203,     0,     0,   204,    39,     0,     0,
       0,    41,     0,     0,    43,     0,   666,   205,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     206,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   207,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,   537,   471,   472,
     473,   474,   475,     0,     0,   478,   479,   480,   481,     0,
     483,   484,     0,   884,   885,   886,   887,   888,   679,     0,
     680,     0,   100,     0,   681,   682,   683,   684,   685,   686,
     687,   889,   689,   690,   102,   890,   104,     0,   692,   693,
     891,   695,   208,     0,   108,     0,   209,     0,     0,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,     0,    16,    17,     0,    18,     0,   200,
      20,    21,    22,     0,     0,     0,     0,    27,     0,    28,
      29,     0,   201,     0,     0,     0,    33,    34,    35,    36,
       0,    38,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,    49,    50,
      51,    52,     0,    53,    54,     0,    55,    56,    57,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,    77,    78,    79,     0,
      80,    81,     0,    82,     0,     0,     0,    86,     0,     0,
      88,     0,     0,    90,     0,     0,    91,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   107,     0,   108,
       0,   109,     0,     0,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   200,     0,     0,    22,   401,    24,
       0,     0,     0,     0,     0,    29,     0,   201,     0,     0,
       0,    33,   202,   203,     0,     0,   204,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   205,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
      60,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     206,     0,    69,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   207,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,    89,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,     0,   209,     0,     0,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   204,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,     0,    82,   330,   331,     0,    86,   367,     0,
      88,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,   368,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   208,     0,   108,
       0,   209,     0,     0,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   200,     0,     0,    22,     0,   766,
       0,     0,     0,     0,     0,    29,     0,   201,     0,     0,
       0,    33,   202,   203,     0,     0,   204,    39,     0,   767,
       0,    41,     0,     0,    43,     0,     0,   205,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     206,     0,   768,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   207,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,   769,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,     0,   209,     0,     0,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   204,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,     0,    82,   330,   331,     0,    86,   367,     0,
      88,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   208,     0,   108,
     848,   209,     0,     0,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   200,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   201,     0,     0,
       0,    33,   202,   203,     0,     0,   204,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   205,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     206,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   207,     0,    80,     0,     0,    82,   330,
     331,     0,    86,   367,     0,    88,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,   850,   209,     0,     0,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   204,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,     0,    82,   330,   331,     0,    86,     0,     0,
      88,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,   332,     0,     0,   106,     0,   208,     0,   108,
       0,   209,     0,     0,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   200,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   201,     0,     0,
       0,    33,   202,   203,     0,     0,   204,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   205,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     206,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   207,     0,    80,     0,     0,    82,   330,
     331,     0,    86,     0,     0,    88,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,   868,   209,     0,     0,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   204,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,  1008,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,     0,    82,   330,   331,     0,    86,     0,     0,
      88,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   208,     0,   108,
       0,   209,     0,     0,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   200,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   201,     0,     0,
       0,    33,   202,   203,     0,     0,   204,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   205,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
     265,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     206,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   207,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,    90,     0,
       0,     0,     0,     0,   266,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,     0,   209,     0,     0,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,   273,     0,     0,     0,     0,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   204,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,     0,    82,     0,     0,     0,    86,     0,     0,
      88,     0,   274,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   208,     0,   108,
       0,   209,     0,     0,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   200,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   201,     0,     0,
       0,    33,   202,   203,     0,     0,   204,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   205,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     206,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   207,     0,    80,     0,     0,    82,   330,
     331,     0,    86,     0,     0,    88,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,     0,   209,     0,     0,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   204,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,     0,    82,     0,     0,     0,    86,     0,     0,
      88,     0,     0,    90,     0,     0,     0,     0,     0,   266,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   208,     0,   108,
       0,   209,   372,     0,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   200,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   201,     0,     0,
       0,    33,   202,   203,     0,     0,   204,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   205,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     206,  -715,     0,     0,     0,  -715,     0,     0,    74,    75,
      76,    77,    78,   207,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,     0,   209,     0,     0,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   204,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,     0,    82,     0,     0,     0,    86,     0,     0,
      88,     0,     0,    90,     0,     0,     0,     0,     0,   266,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   208,     0,   108,
       0,   209,     0,     0,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   200,     0,     0,    22,     0,     0,
       0,     0,     0,     0,   288,    29,     0,   201,     0,     0,
       0,    33,   202,   203,     0,     0,   204,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   205,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     206,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   207,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   107,     0,   108,     0,   209,     0,     0,   110,
       5,     6,     7,     8,     9,   404,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,    19,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   204,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,    68,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,     0,    82,     0,     0,     0,    86,     0,    87,
      88,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   208,     0,   108,
       0,   209,     0,     0,   110,     5,     6,     7,     8,     9,
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
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,     0,   209,   372,     0,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   204,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,     0,    82,     0,     0,     0,    86,     0,     0,
      88,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   208,   846,   108,
       0,   209,     0,     0,   110,     5,     6,     7,     8,     9,
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
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,     0,   209,   856,     0,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   204,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,     0,    82,     0,     0,     0,    86,     0,     0,
      88,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   208,     0,   108,
       0,   209,   861,     0,   110,     5,     6,     7,     8,     9,
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
       0,     0,    86,  1052,     0,    88,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,     0,   209,     0,     0,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   204,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,     0,    82,     0,     0,     0,    86,  1219,     0,
      88,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   208,     0,   108,
       0,   209,     0,     0,   110,     5,     6,     7,     8,     9,
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
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,     0,   209,     0,     0,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
     308,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   204,    39,     0,     0,     0,   310,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,   315,    76,    77,    78,   316,     0,
      80,     0,     0,    82,     0,     0,     0,    86,     0,     0,
      88,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   208,     0,   108,
       0,   209,     0,     0,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   200,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   201,     0,     0,
       0,    33,   202,   203,     0,     0,   938,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   205,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     206,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   207,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,     0,   209,     0,     0,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,   941,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,     0,    82,     0,     0,     0,    86,     0,     0,
      88,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   208,     0,   108,
       0,   209,     0,     0,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   200,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   201,     0,     0,
       0,    33,   202,   203,     0,     0,  1188,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   205,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     206,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   207,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,     0,   209,     0,     0,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,  1189,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,     0,    82,     0,     0,     0,    86,     0,     0,
      88,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   208,     0,   108,
       0,   209,     0,     0,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   200,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   201,     0,     0,
       0,    33,   202,   203,     0,     0,  1191,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   205,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     206,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   207,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,     0,   209,     0,     0,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,  1192,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,     0,    82,     0,     0,     0,    86,     0,     0,
      88,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   208,     0,   108,
       0,   209,     0,     0,   110,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   200,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   201,     0,     0,
       0,    33,   202,   203,     0,     0,  1193,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   205,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     206,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   207,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   208,     0,   108,     0,   209,     0,     0,   110,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   200,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   201,     0,     0,     0,    33,   202,   203,     0,
       0,  1194,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   205,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,   442,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   206,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   207,     0,
      80,     0,     0,    82,     0,     0,     0,    86,     0,     0,
      88,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   443,     0,     0,     0,   444,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,   445,   208,     0,   108,
       0,   209,     0,     0,   110,     6,     7,     8,     9,    10,
      11,    12,     0,   446,   447,    13,   448,   449,   450,    17,
     451,   452,   453,   200,   454,   455,   456,   457,   307,   459,
     460,   461,   462,   463,   464,   465,   201,     0,   466,   467,
     468,   202,   203,     0,     0,   204,     0,     0,   309,   469,
       0,     0,   311,     0,     0,     0,   205,     0,     0,    47,
      48,     0,     0,     0,     0,    52,     0,    53,    54,     0,
       0,     0,     0,     0,     0,     0,   312,   442,    59,   313,
      61,    62,    63,     0,     0,     0,     0,    66,     0,   206,
       0,   314,     0,     0,     0,     0,     0,    74,     0,    76,
       0,    78,     0,     0,     0,     0,     0,     0,     0,     0,
     317,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   443,     0,     0,     0,   444,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
     445,   208,     0,   108,  -330,   209,     0,     0,   110,     6,
       7,     8,     9,    10,    11,    12,     0,   446,   447,    13,
     448,   449,   450,    17,   451,   452,   453,   200,   454,   455,
     456,   457,   307,   459,   460,   461,   462,   463,     0,   465,
     201,     0,   466,   467,   468,   202,   203,     0,     0,   204,
       0,     0,   309,   469,     0,     0,   311,     0,     0,     0,
     205,     0,     0,    47,    48,     0,     0,     0,     0,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,     0,
     312,     0,    59,   313,    61,    62,    63,     0,     0,     0,
       0,    66,     0,   206,     0,   314,     0,     0,     0,     0,
       0,    74,     0,    76,     0,    78,     0,     0,     0,     0,
       0,     0,     0,     0,   317,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   657,     0,   658,     0,    95,    96,    97,    98,
      99,     0,     0,   659,     0,   100,     0,     0,   660,   239,
     661,   662,     0,     0,   101,     0,   663,   102,   103,   104,
       0,     0,     0,   106,   242,   208,   201,   108,     0,   209,
     243,     0,   110,     0,     0,     0,   664,     0,     0,     0,
     245,     0,     0,   665,     0,   666,     0,     0,     0,     0,
       0,     0,     0,   667,     0,     0,     0,     0,     0,     0,
     248,   668,     0,     0,     0,     0,     0,     0,   250,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   669,     0,
       0,     0,     0,     0,     0,     0,     0,   252,   253,     0,
     670,     0,   255,     0,   671,     0,     0,   257,     0,     0,
       0,   672,     0,     0,   259,     0,     0,   673,     0,     0,
       0,     0,     0,     0,     0,     0,   537,   471,   472,   473,
     474,   475,     0,     0,   478,   479,   480,   481,     0,   483,
     484,     0,   674,   675,   676,   677,   678,   679,     0,   680,
       0,     0,     0,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,     0,   691,     0,     0,   692,   693,   694,
     695,     0,     0,   696,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   204,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,     0,     0,
       0,    86,     0,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,     0,   103,   104,     0,     0,     0,   106,
       0,   208,     0,   108,     0,   209,     5,     6,     7,     8,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,    97,     0,     0,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,   104,   200,     0,
       0,    22,     0,   208,     0,   108,     0,   209,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
     204,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
     237,     0,    82,     0,     0,     0,    86,   442,     0,    88,
       0,     0,    90,     0,   238,   239,     0,   240,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,   243,    95,     0,    97,
       0,     0,   244,     0,     0,     0,   245,     0,     0,   246,
       0,     0,     0,     0,     0,   721,     0,     0,     0,   247,
     104,     0,     0,     0,   659,   443,   248,   249,   108,   444,
     209,   661,   722,     0,   250,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   251,   723,     0,     0,     0,     0,
       0,     0,     0,   252,   253,     0,   254,     0,   255,     0,
     256,     0,     0,   257,     0,     0,   666,   258,     0,     0,
     259,     0,     0,   260,   724,     0,     0,   446,   447,     0,
     448,   449,   450,     0,   451,   452,   453,     0,   454,   455,
     456,   457,     0,   459,   460,   461,   462,   463,     0,   465,
       0,     0,   466,   467,   468,     0,     0,     0,     0,     0,
       0,     0,     0,   469,     0,   725,     0,     0,     0,     0,
       0,   726,     0,     0,     0,     0,     0,     0,     0,   284,
       0,     0,     0,     0,     0,     0,     0,   537,   471,   472,
     473,   474,   475,     0,     0,   478,   479,   480,   481,     0,
     483,   484,     0,   674,   675,   676,   677,   678,   679,     0,
     680,     0,     0,     0,   681,   682,   683,   684,   685,   686,
     687,   688,   689,   690,   237,   691,     0,     0,   692,   693,
     694,   695,  1049,   659,     0,     0,     0,     0,   238,   239,
     661,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,   242,     0,     0,     0,     0,     0,
     243,     0,     0,     0,     0,     0,   244,     0,     0,     0,
     245,     0,     0,   246,     0,   666,     0,     0,     0,     0,
       0,     0,     0,   247,     0,     0,     0,     0,     0,     0,
     248,   249,     0,     0,     0,     0,     0,     0,   250,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   251,     0,
       0,     0,     0,     0,   440,     0,     0,   252,   253,     0,
     254,   441,   255,     0,   256,     0,     0,   257,     0,     0,
       0,   258,     0,   442,   259,     0,     0,   260,     0,     0,
       0,     0,     0,     0,     0,     0,   537,   471,   472,   473,
     474,   475,     0,     0,   478,   479,   480,   481,     0,   483,
     484,     0,   674,   675,   676,   677,   678,   679,     0,   680,
       0,     0,     0,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,     0,   691,     0,     0,   692,   693,   694,
     695,   443,     0,     0,   440,   444,     0,     0,     0,     0,
       0,   441,     0,     0,   536,     0,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,   537,   471,   472,
     473,   474,   475,     0,     0,   478,   479,   480,   481,     0,
     483,   484,     0,   446,   447,     0,   448,   449,   450,     0,
     451,   452,   453,     0,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,     0,     0,   466,   467,
     468,   443,   386,     0,   440,   444,     0,     0,     0,   469,
       0,   441,     0,     0,  1103,     0,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,   537,   471,   472,
     473,   474,   475,     0,     0,   478,   479,   480,   481,     0,
     483,   484,     0,   446,   447,     0,   448,   449,   450,     0,
     451,   452,   453,     0,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,     0,     0,   466,   467,
     468,   443,   386,     0,   440,   444,     0,     0,     0,   469,
       0,   441,     0,     0,  1110,     0,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,   537,   471,   472,
     473,   474,   475,     0,     0,   478,   479,   480,   481,     0,
     483,   484,     0,   446,   447,     0,   448,   449,   450,     0,
     451,   452,   453,     0,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,     0,     0,   466,   467,
     468,   443,   386,     0,   440,   444,     0,     0,     0,   469,
       0,   441,     0,     0,  1252,     0,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,   537,   471,   472,
     473,   474,   475,     0,     0,   478,   479,   480,   481,     0,
     483,   484,     0,   446,   447,     0,   448,   449,   450,     0,
     451,   452,   453,     0,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,     0,     0,   466,   467,
     468,   443,   386,     0,   440,   444,     0,     0,     0,   469,
       0,   441,     0,     0,  1253,     0,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,   537,   471,   472,
     473,   474,   475,     0,     0,   478,   479,   480,   481,     0,
     483,   484,     0,   446,   447,     0,   448,   449,   450,     0,
     451,   452,   453,     0,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,     0,     0,   466,   467,
     468,   443,   386,     0,   440,   444,     0,     0,     0,   469,
       0,   441,     0,     0,  1254,     0,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,   537,   471,   472,
     473,   474,   475,     0,     0,   478,   479,   480,   481,     0,
     483,   484,     0,   446,   447,     0,   448,   449,   450,     0,
     451,   452,   453,     0,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,     0,     0,   466,   467,
     468,   443,   386,     0,   440,   444,     0,     0,     0,   469,
       0,   441,     0,     0,  1255,     0,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,   537,   471,   472,
     473,   474,   475,     0,     0,   478,   479,   480,   481,     0,
     483,   484,     0,   446,   447,     0,   448,   449,   450,     0,
     451,   452,   453,     0,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,     0,     0,   466,   467,
     468,   443,   386,     0,   440,   444,     0,     0,     0,   469,
       0,   441,     0,     0,  1260,     0,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,   537,   471,   472,
     473,   474,   475,     0,     0,   478,   479,   480,   481,     0,
     483,   484,     0,   446,   447,     0,   448,   449,   450,     0,
     451,   452,   453,     0,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,     0,     0,   466,   467,
     468,   443,   386,     0,     0,   444,     0,     0,     0,   469,
       0,     0,     0,     0,  1261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   440,     0,     0,     0,     0,     0,
       0,   441,     0,     0,     0,     0,   445,   537,   471,   472,
     473,   474,   475,   442,   288,   478,   479,   480,   481,     0,
     483,   484,     0,   446,   447,     0,   448,   449,   450,   529,
     451,   452,   453,     0,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,     0,     0,   466,   467,
     468,     0,   386,     0,     0,     0,     0,     0,     0,   469,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     440,   443,     0,     0,     0,   444,     0,   441,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   442,
     288,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   520,     0,     0,   864,   445,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   446,   447,     0,   448,   449,   450,     0,
     451,   452,   453,     0,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   440,   443,   466,   467,
     468,   444,   386,   441,     0,     0,     0,     0,     0,   469,
       0,     0,     0,     0,     0,   442,   517,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   520,     0,
       0,   518,   445,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   446,
     447,     0,   448,   449,   450,     0,   451,   452,   453,     0,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,     0,   443,   466,   467,   468,   444,   386,     0,
       0,     0,     0,     0,     0,   469,     0,     0,   440,     0,
       0,     0,     0,     0,     0,   441,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   442,   445,     0,
       0,     0,   630,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   446,   447,     0,   448,   449,
     450,     0,   451,   452,   453,     0,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   631,     0,
     466,   467,   468,     0,   386,     0,     0,     0,     0,     0,
       0,   469,     0,     0,     0,   443,     0,     0,   440,   444,
       0,     0,     0,     0,     0,   441,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   442,   288,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     445,     0,     0,   863,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   446,   447,     0,
     448,   449,   450,     0,   451,   452,   453,     0,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
       0,   632,   466,   467,   468,   443,     0,     0,     0,   444,
       0,     0,     0,   469,     0,     0,     0,     0,     0,     0,
     440,   880,     0,     0,     0,     0,     0,   441,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   442,
     445,     0,     0,     0,   881,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   446,   447,     0,
     448,   449,   450,     0,   451,   452,   453,     0,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     882,     0,   466,   467,   468,     0,   386,     0,     0,     0,
       0,     0,     0,   469,     0,     0,   440,   443,     0,     0,
       0,   444,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   442,   985,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   445,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   446,
     447,     0,   448,   449,   450,     0,   451,   452,   453,     0,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   440,   443,   466,   467,   468,   444,     0,   441,
       0,     0,     0,     0,     0,   469,     0,     0,     0,     0,
       0,   442,   288,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   520,     0,     0,     0,   445,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   446,   447,     0,   448,   449,
     450,     0,   451,   452,   453,     0,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   440,   443,
     466,   467,   468,   444,   386,   441,     0,     0,     0,     0,
       0,   469,     0,     0,     0,     0,     0,   442,   288,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     520,     0,     0,     0,   445,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   446,   447,     0,   448,   449,   450,     0,   451,   452,
     453,     0,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   440,   443,   466,   467,   468,   444,
     386,   441,     0,     0,     0,     0,     0,   469,     0,     0,
       0,     0,     0,   442,   757,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   758,
     445,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   446,   447,     0,
     448,   449,   450,     0,   451,   452,   453,     0,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     440,   443,   466,   467,   468,   444,   386,   441,     0,     0,
       0,     0,     0,   469,     0,     0,     0,     0,     0,   442,
     759,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   760,   445,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   446,   447,     0,   448,   449,   450,     0,
     451,   452,   453,     0,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   440,   443,   466,   467,
     468,   444,     0,   441,     0,     0,     0,     0,     0,   469,
       0,     0,     0,     0,     0,   442,   977,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   445,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   446,
     447,     0,   448,   449,   450,     0,   451,   452,   453,     0,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   440,   443,   466,   467,   468,   444,     0,   441,
       0,     0,     0,     0,     0,   469,     0,     0,     0,     0,
       0,   442,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   445,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   446,   447,     0,   448,   449,
     450,     0,   451,   452,   453,     0,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   440,   443,
     466,   467,   468,   444,   386,   441,     0,     0,     0,     0,
       0,   469,   999,     0,     0,     0,     0,   442,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   445,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   446,   447,     0,   448,   449,   450,     0,   451,   452,
     453,     0,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   440,   443,   466,   467,   468,   444,
     386,   441,     0,     0,     0,     0,     0,   469,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   520,     0,     0,     0,
     445,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   446,   447,     0,
     448,   449,   450,     0,   451,   452,   453,     0,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
       0,   443,   466,   467,   468,   444,     0,     0,     0,     0,
       0,  1035,     0,   469,     0,     0,   440,     0,     0,     0,
       0,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,   520,     0,     0,   442,   445,     0,     0,     0,
    1159,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   446,   447,     0,   448,   449,   450,     0,
     451,   452,   453,     0,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,  1160,     0,   466,   467,
     468,     0,     0,     0,     0,     0,     0,  1153,     0,   469,
       0,     0,     0,   443,     0,     0,     0,   444,   440,   560,
       0,     0,     0,     0,     0,   441,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   442,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   445,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   446,   447,     0,   448,   449,
     450,     0,   451,   452,   453,     0,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,     0,     0,
     466,   467,   468,     0,     0,   443,     0,     0,   440,   444,
       0,   469,     0,     0,     0,   441,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   442,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     445,     0,     0,     0,     0,     0,     0,     0,   440,     0,
       0,     0,     0,     0,     0,   441,     0,   446,   447,     0,
     448,   449,   450,     0,   451,   452,   453,   442,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
       0,     0,   466,   467,   468,   443,     0,     0,     0,   444,
       0,     0,     0,   469,   440,     0,     0,     0,     0,     0,
       0,   441,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
     445,     0,     0,     0,     0,   443,     0,     0,     0,   444,
       0,     0,     0,     0,     0,     0,     0,   446,   447,     0,
     448,   449,   450,     0,   451,   452,   453,     0,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     445,     0,   466,   467,   468,     0,   584,     0,     0,     0,
       0,   443,     0,   469,     0,   444,     0,   446,   447,     0,
     448,   449,   450,     0,   451,   452,   453,     0,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     440,   588,   466,   467,   468,   611,   445,   441,     0,     0,
       0,     0,     0,   469,     0,     0,     0,     0,     0,   442,
       0,     0,     0,   446,   447,     0,   448,   449,   450,     0,
     451,   452,   453,     0,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   440,     0,   466,   467,
     468,     0,     0,   441,     0,     0,     0,     0,     0,   469,
       0,     0,     0,     0,     0,   442,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   443,     0,     0,
       0,   444,     0,     0,     0,     0,   440,   876,     0,     0,
     761,     0,     0,   441,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   442,     0,     0,     0,     0,
       0,     0,   445,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   443,     0,     0,     0,   444,     0,   446,
     447,     0,   448,   449,   450,     0,   451,   452,   453,     0,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,     0,     0,   466,   467,   468,     0,   445,     0,
       0,     0,   440,   443,     0,   469,     0,   444,     0,   441,
       0,     0,     0,     0,     0,   446,   447,     0,   448,   449,
     450,   442,   451,   452,   453,     0,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   445,     0,
     466,   467,   468,     0,     0,     0,     0,   847,     0,     0,
       0,   469,     0,     0,     0,   446,   447,     0,   448,   449,
     450,     0,   451,   877,   453,     0,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   440,   443,
     466,   467,   468,   444,     0,   441,     0,     0,     0,     0,
       0,   469,     0,     0,     0,     0,     0,   442,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     220,     0,     0,     0,   445,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   446,   447,     0,   448,   449,   450,     0,   451,   452,
     453,     0,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,     0,   443,   466,   467,   468,   444,
       0,   440,     0,     0,     0,     0,     0,   469,   441,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     442,     0,     0,   997,     0,     0,     0,     0,     0,     0,
     445,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   446,   447,     0,
     448,   449,   450,     0,   451,   452,   453,     0,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
       0,   969,   466,   467,   468,     0,     0,   440,   443,     0,
       0,     0,   444,   469,   441,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   442,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   445,     0,     0,     0,   440,     0,     0,
       0,     0,     0,     0,   441,     0,     0,     0,     0,     0,
     446,   447,     0,   448,   449,   450,   442,   451,   452,   453,
       0,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,     0,   443,   466,   467,   468,   444,     0,
       0,     0,     0,   440,     0,     0,   469,     0,     0,     0,
     441,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   442,     0,     0,     0,     0,     0,     0,   445,
       0,     0,     0,     0,   443,     0,     0,     0,   444,     0,
       0,     0,     0,     0,     0,     0,   446,   447,     0,   448,
     449,   450,     0,   451,   452,   453,     0,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   445,
       0,   466,   467,   468,     0,     0,     0,     0,  1011,     0,
     443,     0,   469,     0,   444,     0,   446,   447,     0,   448,
     449,   450,     0,   451,   452,   453,     0,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   440,
    1058,   466,   467,   468,  1038,   445,   441,     0,  1012,     0,
       0,     0,   469,     0,     0,     0,     0,     0,   442,     0,
       0,     0,   446,   447,     0,   448,   449,   450,     0,   451,
     452,   453,     0,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   440,     0,   466,   467,   468,
       0,     0,   441,     0,     0,     0,     0,     0,   469,     0,
       0,     0,     0,     0,   442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   443,     0,     0,     0,
     444,     0,     0,     0,     0,   440,     0,     0,     0,     0,
       0,     0,   441,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   442,  1088,     0,     0,     0,     0,
       0,   445,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   443,     0,     0,     0,   444,     0,   446,   447,
       0,   448,   449,   450,     0,   451,   452,   453,     0,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,     0,     0,   466,   467,   468,     0,   445,     0,     0,
       0,   440,   443,     0,   469,     0,   444,     0,   441,     0,
       0,     0,     0,     0,   446,   447,     0,   448,   449,   450,
     442,   451,   452,   453,     0,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   445,     0,   466,
     467,   468,     0,     0,     0,     0,  1081,     0,     0,     0,
     469,     0,     0,     0,   446,   447,     0,   448,   449,   450,
       0,   451,   452,   453,     0,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   440,   443,   466,
     467,   468,   444,     0,   441,     0,     0,     0,     0,     0,
     469,     0,     0,     0,     0,     0,   442,  1091,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   445,     0,     0,     0,   440,  1156,     0,
       0,     0,     0,     0,   441,     0,     0,     0,     0,     0,
     446,   447,     0,   448,   449,   450,   442,   451,   452,   453,
       0,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,     0,   443,   466,   467,   468,   444,     0,
       0,     0,   440,     0,  1094,     0,   469,     0,     0,   441,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   442,     0,     0,     0,     0,     0,     0,     0,   445,
       0,     0,     0,     0,   443,     0,     0,     0,   444,     0,
       0,     0,     0,     0,     0,     0,   446,   447,     0,   448,
     449,   450,     0,   451,   452,   453,     0,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   445,
       0,   466,   467,   468,     0,     0,     0,     0,     0,   443,
       0,     0,   469,   444,     0,     0,   446,   447,     0,   448,
     449,   450,  1187,   451,   452,   453,     0,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   440,
       0,   466,   467,   468,   445,     0,   441,     0,     0,     0,
       0,     0,   469,     0,     0,     0,     0,     0,   442,     0,
       0,   446,   447,     0,   448,   449,   450,     0,   451,   452,
     453,     0,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   440,  1225,   466,   467,   468,     0,
       0,   441,     0,     0,     0,     0,     0,   469,     0,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   443,     0,     0,     0,
     444,     0,     0,     0,     0,     0,     0,     0,     0,  1190,
     440,     0,     0,     0,     0,     0,     0,   441,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   442,
       0,   445,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   443,     0,     0,     0,   444,     0,     0,   446,   447,
       0,   448,   449,   450,     0,   451,   452,   453,  1239,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,     0,     0,   466,   467,   468,   445,     0,     0,     0,
       0,     0,     0,     0,   469,     0,     0,   443,     0,     0,
       0,   444,     0,   446,   447,     0,   448,   449,   450,     0,
     451,   452,   453,     0,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   440,     0,   466,   467,
     468,     0,   445,   441,     0,     0,     0,     0,     0,   469,
       0,     0,     0,     0,     0,   442,     0,     0,     0,   446,
     447,     0,   448,   449,   450,     0,   451,   452,   453,     0,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,     0,     0,   466,   467,   468,     0,     0,     0,
       0,     0,     0,     0,     0,   469,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   440,   443,     0,     0,     0,   444,     0,   441,
       0,     0,     0,     0,     0,     0,  1278,     0,     0,     0,
       0,   442,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   445,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   446,   447,     0,   448,   449,
     450,     0,   451,   452,   453,     0,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   440,   443,
     466,   467,   468,   444,     0,   441,     0,     0,     0,     0,
       0,   469,  1279,     0,     0,     0,     0,   442,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   445,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   446,   447,     0,   448,   449,   450,     0,   451,   452,
     453,     0,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   440,   443,   466,   467,   468,   444,
       0,   441,     0,     0,     0,     0,     0,   469,  1280,     0,
       0,     0,     0,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     445,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   446,   447,     0,
     448,   449,   450,     0,   451,   452,   453,     0,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     440,   443,   466,   467,   468,   444,     0,   441,     0,     0,
       0,     0,     0,   469,  1281,     0,     0,     0,     0,   442,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   446,   447,     0,   448,   449,   450,     0,
     451,   452,   453,     0,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   440,   443,   466,   467,
     468,   444,     0,   441,     0,     0,     0,     0,     0,   469,
    1282,     0,     0,     0,     0,   442,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   445,     0,     0,     0,   440,     0,     0,     0,
       0,     0,     0,   441,     0,     0,     0,     0,     0,   446,
     447,     0,   448,   449,   450,   442,   451,   452,   453,     0,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,     0,   443,   466,   467,   468,   444,     0,     0,
       0,   440,     0,     0,     0,   469,  1283,     0,   441,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     442,     0,     0,     0,     0,     0,     0,     0,   445,     0,
       0,     0,     0,   443,     0,     0,     0,   444,     0,     0,
       0,     0,     0,     0,     0,   446,   447,     0,   448,   449,
     450,     0,   451,   452,   453,     0,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   445,     0,
     466,   467,   468,     0,     0,     0,     0,     0,   964,     0,
       0,   469,   444,     0,     0,   446,   447,     0,   448,   449,
     450,     0,   451,   452,   453,     0,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,     0,     0,
     466,   467,   468,   445,     0,     0,     0,     0,     0,     0,
       0,   469,     0,     0,     0,     0,     0,     0,     0,     0,
     446,   447,     0,   448,   449,   450,     0,   451,   452,   453,
       0,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,     0,   380,   466,   467,   468,    23,    24,
       0,     0,     0,     0,     0,     0,   469,   381,     0,    31,
     382,     0,   380,     0,     0,    37,    23,    24,     0,     0,
       0,     0,    42,     0,     0,   381,     0,    31,   382,     0,
       0,     0,     0,    37,     0,     0,     0,     0,     0,     0,
      42,     0,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,  1066,     0,     0,  1067,     0,     0,
     385,     0,    69,     0,     0,     0,    58,     0,    60,     0,
     378,     0,  1066,     0,     0,  1067,     0,     0,   385,     0,
      69,    85,     0,     0,    87,   380,     0,    89,     0,    23,
      24,     0,     0,     0,     0,     0,     0,     0,   381,    85,
      31,   382,    87,     0,     0,    89,    37,     0,     0,     0,
       0,     0,    94,    42,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,   105,     0,    58,
       0,    60,     0,  1226,   412,  1066,   237,     0,  1067,     0,
       0,   385,     0,    69,     0,   105,     0,     0,     0,     0,
     238,   239,     0,   240,     0,     0,     0,     0,   241,     0,
       0,     0,    85,     0,     0,    87,   242,     0,    89,     0,
       0,     0,   243,     0,     0,     0,     0,     0,   244,     0,
       0,     0,   245,     0,     0,   246,     0,     0,     0,     0,
       0,   237,     0,    94,     0,   247,     0,     0,     0,     0,
       0,     0,   248,   249,     0,   238,   239,     0,   240,     0,
     250,     0,     0,   241,     0,     0,     0,     0,   105,     0,
     251,   242,     0,     0,     0,     0,     0,   243,     0,   252,
     253,     0,   254,   244,   255,     0,   256,   245,     0,   257,
     246,     0,     0,   258,     0,     0,   259,     0,     0,   260,
     247,     0,     0,     0,     0,     0,     0,   248,   249,     0,
       0,     0,     0,     0,     0,   250,   237,     0,     0,     0,
       0,     0,     0,    94,     0,   251,     0,     0,     0,     0,
     238,   239,     0,   240,   252,   253,     0,   254,   241,   255,
      24,   256,     0,     0,   257,     0,   242,     0,   258,     0,
       0,   259,   243,     0,   260,     0,     0,     0,   244,     0,
       0,     0,   245,     0,     0,   246,     0,     0,     0,     0,
       0,   237,     0,     0,     0,   247,     0,     0,    94,     0,
       0,     0,   248,   249,     0,   238,   239,     0,   240,     0,
     250,    60,     0,   241,     0,     0,     0,     0,     0,     0,
     251,   242,     0,    69,     0,     0,     0,   243,     0,   252,
     253,     0,   254,   244,   255,     0,   256,   245,     0,   257,
     246,     0,     0,   258,     0,     0,   259,     0,    89,   260,
     247,     0,     0,     0,     0,     0,     0,   248,   249,     0,
       0,     0,     0,     0,     0,   250,  -301,     0,     0,     0,
       0,     0,     0,     0,     0,   251,     0,     0,     0,     0,
    -301,  -301,     0,  -301,   252,   253,     0,   254,  -301,   255,
       0,   256,     0,     0,   257,     0,  -301,     0,   258,   573,
       0,   259,  -301,     0,   260,     0,     0,     0,  -301,     0,
       0,     0,  -301,     0,     0,  -301,     0,     0,     0,     0,
       0,   237,     0,     0,     0,  -301,     0,     0,     0,     0,
       0,     0,  -301,  -301,     0,   238,   239,     0,   240,     0,
    -301,     0,     0,   241,     0,     0,     0,     0,     0,     0,
    -301,   242,     0,     0,     0,     0,     0,   243,     0,  -301,
    -301,     0,  -301,   244,  -301,     0,  -301,   245,     0,  -301,
     246,     0,     0,  -301,     0,     0,  -301,     0,     0,  -301,
     247,     0,     0,     0,     0,     0,     0,   248,   249,     0,
       0,     0,     0,     0,     0,   250,  -302,     0,     0,     0,
       0,     0,     0,     0,     0,   251,     0,     0,     0,     0,
    -302,  -302,     0,  -302,   252,   253,     0,   254,  -302,   255,
       0,   256,     0,     0,   257,     0,  -302,     0,   258,     0,
       0,   259,  -302,     0,   260,     0,     0,     0,  -302,     0,
       0,     0,  -302,     0,     0,  -302,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -302,     0,     0,     0,     0,
       0,     0,  -302,  -302,     0,     0,     0,     0,     0,     0,
    -302,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -302,     0,     0,     0,     0,     0,     0,     0,     0,  -302,
    -302,     0,  -302,     0,  -302,     0,  -302,     0,     0,  -302,
       0,     0,     0,  -302,     0,     0,  -302,     0,     0,  -302
};

static const yytype_int16 yycheck[] =
{
      13,   170,   118,    66,    17,     2,   236,   582,    21,   157,
      20,    66,    66,   280,    27,   229,   542,   413,   186,   790,
      27,    34,    35,    36,   432,    38,   497,   699,   882,   701,
     653,   586,   650,   836,   139,   405,    49,   304,    51,    52,
     830,   630,     1,   577,    57,   906,    59,     1,    61,    32,
     280,   877,   418,    66,  1013,   900,   552,    70,    71,    72,
      73,    74,    75,    70,   619,    32,    79,    55,    81,     1,
      47,    55,   912,    46,   304,    88,  1183,   344,    91,    92,
       3,    47,    95,    47,    97,    47,    27,   100,   101,    32,
     103,   120,    71,    46,   107,   108,   109,   110,   123,    32,
      32,     1,   109,    32,    46,    32,    47,    32,   276,    92,
      51,    84,    65,    92,   344,    74,    32,   147,    36,    74,
      60,    47,  1229,    65,   137,   165,   139,   147,    60,   108,
     269,    84,   271,   272,    75,   165,   118,    78,     1,    80,
     547,   114,    84,   120,     3,   170,   175,    47,   147,    90,
       3,   120,   120,   120,    72,   175,   120,   653,   117,    92,
      92,   114,   117,    40,    90,   147,   179,   574,   109,    32,
     147,   125,   114,   105,   173,  1015,  1135,   120,   118,   950,
     103,   147,   165,   147,   172,   147,   118,   170,   172,   202,
     203,   204,   146,   147,   207,   208,   209,    60,   175,   817,
    1061,   102,   209,   170,  1007,   571,   175,   175,  1069,  1070,
    1013,   175,   171,   175,   146,  1060,   171,   227,   147,   173,
     487,   488,   452,   236,    22,   125,  1016,   170,   783,    92,
    1056,   147,   173,   165,   373,   374,   762,   170,   170,    92,
     117,   170,   105,   170,   103,   170,   146,   147,    53,   147,
     103,   141,    47,   147,   170,   118,   147,   487,   488,   149,
      27,   147,    18,   633,   432,   165,   165,   280,    73,   170,
     165,   165,   147,   172,    55,   173,   171,    31,    83,     3,
      47,     0,   173,   146,    51,    27,   170,   173,   172,   147,
      88,   304,   881,   120,  1097,    90,    20,   413,   173,   404,
     834,    27,   315,   316,  1158,  1166,  1160,   170,    75,    33,
    1171,    78,   542,    80,   112,   173,   165,   330,   331,   790,
     140,    77,   171,    90,   147,    27,   147,   147,     3,    35,
      36,   344,  1135,   125,   503,    89,    78,  1108,    62,    93,
      96,   147,   109,   147,   165,   963,   560,    55,    90,   125,
     173,   171,    78,   147,   417,   368,   586,   113,  1177,   372,
     152,    55,   154,   155,    90,   157,   158,   173,   365,   173,
     383,   384,   147,   115,   387,   151,    78,   171,   904,   103,
     387,   147,   147,   109,   397,   109,   399,   141,    90,   619,
      82,  1014,   146,    85,   147,   149,   409,   147,   173,   153,
     147,    55,   409,   109,   109,   171,   173,   109,   173,    90,
    1082,  1083,   172,   147,  1233,  1234,  1235,  1236,  1237,  1238,
     173,   947,   636,   173,    11,    31,   173,   440,   441,   442,
     443,   444,   445,   147,   447,   448,   449,   450,   451,   173,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   147,    55,   173,
    1049,    55,   147,   476,   477,   147,    89,   147,   147,   482,
      93,   147,   485,   486,   487,   488,   489,   490,   617,   950,
     165,   171,   147,    89,   497,   165,   165,    93,   501,   165,
    1045,   173,   505,     8,   507,   147,   147,   476,   477,   172,
     165,   147,   515,   482,   517,   518,   485,   486,   515,   572,
     523,   524,   908,   165,   165,   923,   529,   572,   572,   165,
     147,    31,   172,   536,   174,   141,   539,    89,   147,   542,
     146,    93,   762,   149,    55,   141,   142,   153,   165,   552,
     146,   554,   148,   149,    55,   558,   165,   153,   561,   562,
      83,    84,    50,   783,   160,    53,   165,   305,   306,   572,
     166,   167,   168,   172,   577,   174,    55,    89,  1157,    55,
    1159,    93,    89,   586,   172,    73,    93,    89,    76,    89,
      55,    93,    89,    93,   597,    83,    93,   600,   172,   602,
     174,   172,   100,   174,   607,   608,   609,    55,   611,   612,
      27,   172,   172,     2,   174,   172,   619,   174,  1183,    55,
      55,   109,   172,   172,   174,   174,    55,   630,   631,    55,
      47,   600,     3,   602,    51,   149,   623,    26,   170,    28,
     643,   141,    55,   146,   140,   172,   146,  1108,   148,   149,
     653,    27,   600,   153,   602,   165,   876,   877,    75,   165,
     160,    78,    55,    80,  1229,   794,   795,   119,   168,   798,
     799,    47,   164,    90,   165,    51,   165,   172,   165,   165,
     165,   165,   165,    84,   904,     2,   147,  1073,   384,   170,
      79,   387,   109,   696,    83,    84,   170,   172,     8,    75,
     703,   172,    78,   172,    80,   172,   146,   173,   165,    26,
     172,    28,   165,     3,    90,    65,    22,   172,   107,   170,
      26,    27,   171,   112,    90,   854,   855,   947,   125,    35,
      90,    37,    38,   109,   125,   149,    68,    43,   172,   125,
     173,   147,    47,   902,    50,   172,   125,   443,   444,    34,
     125,    34,    65,   173,   757,   758,   759,   760,   761,   762,
     763,   764,    79,   125,   173,   923,    83,    84,   906,   146,
      76,   909,    78,   173,    21,   775,   776,   175,   781,   175,
     783,   784,    88,   786,    90,   170,   147,   790,   172,   792,
     107,   105,   149,   489,   490,   112,   149,   149,   801,   802,
     149,   149,   149,   109,   149,   501,   112,   149,   149,   115,
     149,   149,   781,   149,   149,   141,   974,   820,   149,   165,
     175,   165,   518,   981,    34,  1045,    34,   146,   524,   147,
     173,   834,   221,   529,   171,   838,   165,   165,   165,   172,
     165,   165,    32,   173,    32,    34,   146,   170,    34,   171,
     165,   820,   173,   906,   165,   175,   165,   910,   147,   175,
     863,   864,   865,    31,   172,   172,   919,   165,   165,   173,
     173,   874,  1129,  1130,   175,   878,   175,   880,   881,   882,
     173,   884,   173,   886,   165,   164,   889,   890,   164,   147,
     117,   618,  1045,   874,   283,    25,   279,   396,   117,   288,
      91,   904,  1003,  1028,   221,   351,   558,   923,   572,  1129,
    1130,   607,  1041,  1042,  1043,  1044,   910,   407,   919,   569,
    1229,    89,  1239,  1061,     2,    93,   318,  1075,  1232,   705,
     901,  1069,   137,   555,   816,   938,    -1,    -1,   941,    -1,
      -1,    -1,    -1,    -1,   947,    -1,    -1,   950,    26,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   964,   965,    -1,  1070,    -1,   283,  1073,    -1,    -1,
      -1,   288,   975,   141,   977,    -1,   365,    -1,   146,   982,
     148,   149,   985,    -1,    -1,   153,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,   997,    -1,    -1,   386,   166,   167,
     168,    79,    -1,    -1,    -1,    83,    84,   396,  1061,    -1,
      -1,  1014,    -1,    -1,    -1,  1018,    -1,    -1,    -1,    -1,
      -1,  1024,    -1,    -1,    -1,    -1,    -1,  1024,  1166,   107,
    1033,    -1,  1035,    -1,   112,  1038,    -1,    -1,    -1,    -1,
    1198,  1199,  1045,   432,  1202,  1203,  1049,    -1,   365,    -1,
      -1,    -1,    -1,  1056,    -1,  1058,  1214,    -1,  1216,    -1,
      -1,    -1,   758,    -1,   760,  1171,    -1,   763,    -1,   386,
      -1,    -1,    -1,    -1,  1077,    -1,    -1,    -1,    -1,   396,
      -1,    -1,    -1,    -1,  1087,  1088,    -1,  1090,  1091,    -1,
    1093,  1094,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1103,  1104,    -1,  1106,    -1,  1108,    -1,  1110,  1111,    -1,
      -1,  1114,    -1,  1166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1129,  1130,   517,    -1,
      -1,    -1,    -1,    -1,   523,    -1,    -1,    -1,  1141,    -1,
    1143,    -1,    -1,   221,  1147,   534,    -1,   536,    -1,    -1,
      -1,    -1,    -1,  1156,  1157,  1158,  1159,  1160,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   863,   864,   865,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1187,  1188,  1189,  1190,  1191,  1192,
    1193,  1194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     517,    -1,    -1,    -1,    -1,   283,   523,    -1,    -1,    -1,
     288,    -1,    -1,    -1,    -1,    -1,   605,   534,    -1,   536,
     609,    -1,  1225,     3,    -1,    -1,    -1,    -1,  1231,    -1,
      -1,    -1,    -1,    -1,   623,     2,    -1,    17,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,  1252,
    1253,  1254,  1255,  1240,    -1,    35,    -1,  1260,  1261,    26,
      -1,    28,    -1,    -1,    -1,    45,    -1,    -1,   964,   965,
      -1,    -1,    52,    -1,    -1,  1278,  1279,  1280,  1281,  1282,
    1283,    -1,    62,    -1,    -1,    -1,    -1,   365,   605,    -1,
      70,    -1,   609,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   623,    87,   386,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    83,    84,   396,    99,
      -1,    -1,    -1,   103,    -1,    -1,    -1,     1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
     107,    -1,    -1,    -1,    -1,   112,    -1,     2,    22,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    37,    38,    -1,    -1,    -1,    -1,    43,
      -1,    26,    -1,    28,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,   772,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    82,    83,
      -1,    85,    -1,    -1,    88,   804,    90,    -1,   807,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,   112,   517,
      -1,   115,    -1,    -1,    -1,   523,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   221,    -1,   534,   112,   536,    -1,
      -1,    -1,    -1,    -1,   853,    -1,   140,    -1,    -1,    -1,
     859,    -1,    -1,    -1,    -1,    -1,    -1,   866,    -1,    -1,
      -1,    -1,    -1,    -1,     3,    -1,    -1,   804,    -1,    -1,
     807,   165,    -1,    -1,    -1,    -1,    -1,   171,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,   283,    -1,    -1,    -1,
      39,   288,    -1,    -1,    -1,    -1,    45,   605,    -1,    -1,
      49,   609,    -1,    52,    -1,    -1,   853,    -1,    -1,    -1,
      -1,    -1,   859,    62,    -1,   623,    -1,    -1,    -1,   866,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   221,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,   964,   965,    96,    97,    -1,
      99,    -1,   101,    -1,   103,    -1,   975,   106,   977,    -1,
      -1,   110,   111,   982,   113,    -1,   985,   116,   365,     2,
      -1,    -1,    -1,    -1,    -1,   994,    -1,   996,    -1,    -1,
     999,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   386,
      -1,    -1,    -1,    26,    -1,    28,    -1,    -1,   283,   396,
      -1,    -1,    -1,   288,  1023,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,  1033,    -1,  1035,    -1,    35,    -1,
      37,    38,    -1,   172,    -1,    -1,    43,    -1,   975,    -1,
     977,    -1,    -1,    50,    -1,   982,    -1,    -1,   985,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,   994,    -1,   996,
      83,    84,   999,    -1,    -1,    -1,    -1,    31,    -1,    76,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    85,    -1,
      -1,    88,    -1,    90,   107,    -1,  1023,    -1,    -1,   112,
     365,    -1,    -1,    -1,  1103,    -1,  1033,    -1,  1035,    -1,
      -1,  1110,   109,    -1,    -1,   112,   804,    -1,   115,   807,
      -1,   386,    -1,    -1,  1123,    -1,    -1,    -1,    -1,    -1,
      -1,   396,    -1,    -1,    -1,    89,    -1,    -1,  1137,    93,
     517,    -1,    -1,   140,    -1,    -1,   523,  1146,    -1,    -1,
      -1,  1150,    -1,  1152,  1153,    -1,    -1,   534,    -1,   536,
      -1,    -1,    -1,    -1,    -1,   853,    -1,    -1,   165,    -1,
      -1,   859,    -1,    -1,   171,    -1,  1103,    -1,   866,    -1,
      -1,    -1,    -1,  1110,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,  1123,    -1,    -1,   153,
      -1,    -1,    -1,    -1,    -1,  1204,   160,    -1,   221,    -1,
    1137,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,  1146,
      -1,    -1,    -1,  1150,    -1,  1152,  1153,    -1,   605,    -1,
      -1,    -1,   609,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1240,    -1,    -1,    -1,    -1,   623,    -1,    -1,    -1,
      -1,    -1,   517,  1252,  1253,  1254,  1255,    -1,   523,    -1,
      -1,  1260,  1261,    -1,    -1,    -1,    -1,    -1,    -1,   534,
     283,   536,    -1,    -1,    -1,   288,    -1,  1204,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   975,    -1,   977,
       2,    -1,    -1,    -1,   982,    -1,    -1,   985,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,   994,    -1,   996,    -1,
      -1,   999,    -1,  1240,    26,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1252,  1253,  1254,  1255,    -1,
      -1,    -1,    -1,  1260,  1261,  1023,    -1,    -1,    -1,    -1,
     605,    -1,    -1,    -1,   609,  1033,    -1,  1035,    -1,    -1,
      -1,    -1,   365,    -1,    -1,    -1,    -1,    -1,   623,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    79,    -1,    -1,
      -1,    83,    84,   386,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   396,    -1,    -1,    -1,    -1,    22,    -1,
      24,    -1,    26,    27,    -1,   107,    -1,    -1,    32,    -1,
     112,    35,     2,    37,    38,    -1,    40,    41,    42,    43,
      -1,    -1,    -1,   141,   142,  1103,    50,   145,   146,    -1,
     148,   149,  1110,    -1,    -1,   153,    26,   804,    28,    -1,
     807,    65,   160,    -1,    -1,  1123,    -1,    -1,   166,   167,
     168,    -1,    76,    -1,    78,    -1,    -1,    -1,    82,  1137,
      84,    85,    -1,    -1,    88,    -1,    90,    -1,  1146,    -1,
      -1,    -1,  1150,    -1,  1152,  1153,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   853,    -1,   112,    79,
      -1,   115,   859,    83,    84,   119,    -1,    -1,    -1,   866,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   517,    -1,    -1,   107,    -1,   221,
     523,    -1,   112,    -1,    -1,    -1,  1204,    -1,    -1,    -1,
      -1,   534,    -1,   536,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,   170,    -1,    -1,   804,
     174,    -1,   807,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1240,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,  1252,  1253,  1254,  1255,    -1,    -1,
      -1,   283,  1260,  1261,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   853,    -1,
      -1,    -1,   605,    -1,   859,    -1,   609,    -1,   975,    -1,
     977,   866,    -1,    -1,    -1,   982,    -1,    -1,   985,    -1,
     623,    -1,    -1,    -1,     2,    -1,    89,   994,    -1,   996,
      93,   221,   999,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
      28,    71,    -1,    -1,    -1,    -1,  1023,    -1,    -1,    -1,
      -1,    -1,    -1,   365,    -1,    -1,  1033,    -1,  1035,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,   386,   148,   149,   150,   108,    -1,
     153,    -1,    -1,   283,   396,    -1,   159,   160,   288,    -1,
     163,    79,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
     975,    -1,   977,    -1,    -1,    -1,    -1,   982,    -1,   107,
     985,    -1,    -1,    -1,   112,    -1,  1103,    -1,    -1,   994,
      -1,   996,    -1,  1110,   999,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1023,     2,
    1137,    -1,    -1,    -1,    -1,   365,    -1,    -1,  1033,  1146,
    1035,    -1,    -1,  1150,    -1,  1152,  1153,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,    28,   386,    -1,    -1,    -1,
      -1,   804,    -1,    -1,   807,    -1,   396,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   517,   236,    -1,    -1,    -1,
      -1,   523,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   534,    -1,   536,    -1,    -1,  1204,    -1,    -1,
      -1,    -1,    -1,   221,    -1,    -1,    79,    -1,  1103,    -1,
     853,    -1,    -1,    -1,    -1,  1110,   859,    -1,    -1,    -1,
     280,    -1,    -1,   866,    -1,    -1,    -1,    -1,  1123,    -1,
      -1,    -1,    -1,  1240,   107,    -1,    -1,    -1,    -1,   112,
      -1,    -1,  1137,    -1,   304,  1252,  1253,  1254,  1255,    -1,
      -1,  1146,    -1,  1260,  1261,  1150,    -1,  1152,  1153,    31,
      -1,    -1,    -1,   605,    -1,   283,    -1,   609,    -1,    -1,
     288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   623,    -1,    -1,   344,    -1,    -1,   517,    -1,    -1,
      -1,    -1,    -1,   523,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   534,    -1,   536,    -1,    -1,  1204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   975,    -1,   977,    -1,    -1,    -1,    -1,   982,
      -1,    -1,   985,    -1,    -1,  1240,    -1,   365,   221,    -1,
      -1,   994,    -1,   996,    -1,    -1,   999,  1252,  1253,  1254,
    1255,    -1,    -1,    -1,    -1,  1260,  1261,    -1,   386,   141,
     142,    -1,   144,   145,   146,   605,   148,   149,   396,   609,
    1023,   153,    -1,    -1,    -1,    -1,    -1,   159,   160,    -1,
    1033,   163,  1035,   623,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     283,    -1,    -1,    -1,    -1,   288,   476,   477,    -1,    -1,
      -1,    -1,   482,    -1,    -1,   485,   486,   487,   488,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
    1103,    -1,   804,    -1,    -1,   807,    -1,  1110,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    -1,   172,    73,   174,    -1,
    1123,    -1,   542,    -1,    -1,    -1,    -1,    83,    84,    -1,
      -1,    -1,   365,    -1,  1137,    -1,    -1,    -1,    -1,   517,
      -1,    -1,    -1,  1146,    -1,   523,    -1,  1150,    -1,  1152,
    1153,   853,    -1,   386,    -1,    -1,   534,   859,   536,    -1,
      -1,   117,    -1,   396,   866,    -1,   586,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     600,    -1,   602,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,   619,
      -1,  1204,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   804,    -1,    -1,   807,    -1,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   605,    -1,    -1,
      -1,   609,    -1,    -1,    -1,    -1,    -1,  1240,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   623,    -1,    -1,    -1,  1252,
    1253,  1254,  1255,    -1,    -1,    -1,    -1,  1260,  1261,    -1,
      -1,    -1,    -1,   853,    -1,    -1,    -1,    -1,    -1,   859,
     226,    -1,    -1,   975,    -1,   977,   866,    -1,    -1,    -1,
     982,    -1,    -1,   985,   517,    -1,    -1,    -1,    -1,    -1,
     523,    -1,   994,    -1,   996,    -1,    -1,   999,    -1,    -1,
      -1,   534,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   267,   268,   269,   270,   271,   272,    -1,    -1,   275,
     276,  1023,    -1,    -1,    -1,    -1,   282,    -1,    -1,    -1,
      -1,  1033,    -1,  1035,    -1,   291,    -1,   293,    -1,    -1,
      -1,    -1,   762,    -1,   300,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   781,    -1,   783,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   605,    -1,    -1,    -1,   609,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   340,   975,    -1,   977,    -1,    -1,
     623,    -1,   982,    -1,    -1,   985,    -1,   353,   354,    -1,
     820,  1103,    -1,    -1,   994,    -1,   996,    -1,  1110,   999,
      -1,    -1,    -1,   267,    -1,   269,   270,   271,   272,    -1,
      -1,  1123,    -1,    -1,    -1,    -1,   804,    -1,    -1,   807,
      -1,    -1,    -1,  1023,    -1,  1137,    -1,   291,    -1,   293,
      -1,    -1,    -1,  1033,  1146,  1035,   300,    -1,  1150,    22,
    1152,  1153,    30,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    37,    38,    44,    45,    46,    -1,
      43,    -1,    -1,    -1,    -1,   853,    -1,    50,    -1,    -1,
      -1,   859,    -1,    -1,   904,    -1,   340,    -1,   866,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   353,
     354,    -1,  1204,    76,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    85,  1103,    -1,    88,    -1,    90,    -1,    -1,
    1110,    -1,    -1,    -1,    -1,    -1,    -1,   947,    -1,    -1,
      -1,    -1,    -1,  1123,    -1,    -1,   109,    -1,  1240,   112,
      -1,    -1,   115,    -1,    -1,    -1,   502,  1137,    -1,    -1,
    1252,  1253,  1254,  1255,    -1,    -1,  1146,    -1,  1260,  1261,
    1150,    -1,  1152,  1153,    -1,    -1,   522,   140,    -1,    -1,
     526,   804,   528,    -1,   807,   531,    -1,   155,    -1,   157,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,   165,    -1,    -1,    -1,    -1,   975,   171,   977,
      -1,    -1,    -1,    -1,   982,    -1,    -1,   985,    -1,    -1,
      -1,    -1,    -1,    -1,  1204,    -1,   994,    -1,   996,    -1,
     853,   999,    -1,    -1,    -1,  1045,   859,   205,    -1,    -1,
      -1,    -1,    -1,   866,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1023,    -1,    -1,   502,    -1,
    1240,    -1,    -1,    -1,    -1,  1033,    -1,  1035,    -1,   615,
     616,    -1,  1252,  1253,  1254,  1255,    -1,    -1,   522,    31,
    1260,  1261,   526,    -1,   528,    -1,    -1,   531,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   265,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   273,   274,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,  1129,
    1130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1103,    -1,    89,    -1,    -1,
      -1,    93,  1110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   975,    -1,   977,  1123,   324,    -1,    -1,   982,
      -1,    -1,   985,    -1,    -1,    -1,    -1,    -1,    -1,  1137,
      -1,   994,   616,   996,    -1,    -1,   999,    -1,  1146,    -1,
      -1,    -1,  1150,    -1,  1152,  1153,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
    1023,   153,   154,   155,    -1,   157,   158,   159,   160,    -1,
    1033,   163,  1035,   381,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1204,    -1,    -1,    -1,
     408,    -1,   788,   789,    -1,   413,    -1,    -1,   794,   795,
      -1,    -1,   798,   799,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   808,    -1,   432,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1240,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1252,  1253,  1254,  1255,    -1,    -1,
       3,    -1,  1260,  1261,    -1,    -1,    -1,    -1,   844,    -1,
    1123,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,  1137,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,  1146,    -1,    -1,    39,  1150,   496,  1152,
    1153,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    -1,    -1,   788,   789,    -1,    -1,    -1,    62,
     794,   795,    -1,    -1,   798,   799,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,   543,    -1,    -1,    -1,   547,
      -1,  1204,    -1,    96,    97,    -1,    99,    -1,   101,    -1,
     103,    -1,   560,   106,    -1,    -1,    -1,   110,   111,    -1,
     113,   569,    -1,   116,    -1,    -1,   574,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1240,    -1,    -1,
     966,   967,    -1,    -1,    -1,    -1,   972,    -1,   974,    -1,
      -1,    -1,    -1,    -1,    -1,   981,    -1,    -1,   984,    -1,
      -1,   987,    -1,   989,    -1,   991,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1004,   172,
      -1,   629,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    17,    18,    -1,    20,  1031,    -1,    -1,    -1,    25,
      -1,    -1,    31,  1039,  1040,  1041,  1042,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    47,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,   966,   967,    -1,    -1,    62,    -1,   972,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,   705,    -1,    -1,
     984,    77,    -1,   987,    -1,   989,    -1,   991,    -1,    -1,
      89,    87,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    99,    -1,   101,    -1,   103,    -1,    12,
     106,    -1,    -1,    -1,   110,    -1,    19,   113,    -1,    -1,
     116,   120,    -1,    -1,    -1,   124,    -1,    -1,    31,    32,
      -1,    -1,    -1,  1139,    -1,  1039,  1040,  1041,  1042,    -1,
      -1,    -1,   141,   142,    47,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,    -1,    -1,    -1,   170,    -1,   172,    -1,   177,    -1,
      -1,   809,    -1,    -1,    -1,    -1,    89,    -1,   816,    -1,
      93,    -1,  1198,  1199,    -1,    -1,  1202,  1203,   826,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1212,    -1,  1214,    -1,
    1216,    -1,    -1,    -1,    -1,    -1,   844,   120,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1139,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,    -1,   170,    -1,    -1,
      -1,    -1,    -1,   901,   177,    -1,    -1,    -1,    -1,    -1,
     908,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   923,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,     1,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,  1003,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
    1028,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1073,    -1,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,    -1,   170,   171,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,   171,   172,
      -1,   174,    -1,     1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,   171,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,   171,   172,
      -1,   174,    -1,     1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,   171,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    12,    -1,    14,    -1,    -1,    17,
      18,    19,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,   143,   144,   145,   146,    -1,
     148,    -1,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,   166,   167,
     168,   169,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    -1,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    30,    -1,    32,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    42,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    61,    62,
      63,    64,    -1,    66,    67,    -1,    69,    70,    71,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,   119,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    26,    27,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,   107,   108,    -1,   110,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,   107,   108,    -1,   110,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
     173,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,   107,
     108,    -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,   173,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,   107,   108,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,   107,
     108,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,   173,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,   107,   108,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,   107,
     108,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,   122,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,   175,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    89,    -1,    -1,    -1,    93,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,   122,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,
      58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
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
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,   175,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,   171,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
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
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,   175,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,   175,    -1,   177,     3,     4,     5,     6,     7,
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
      -1,    -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
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
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
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
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
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
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
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
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
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
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    31,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,   124,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     4,     5,     6,     7,     8,
       9,    10,    -1,   141,   142,    14,   144,   145,   146,    18,
     148,   149,   150,    22,   152,   153,   154,   155,    27,   157,
     158,   159,   160,   161,   162,   163,    35,    -1,   166,   167,
     168,    40,    41,    -1,    -1,    44,    -1,    -1,    47,   177,
      -1,    -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    -1,    -1,    64,    -1,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    31,    77,    78,
      79,    80,    81,    -1,    -1,    -1,    -1,    86,    -1,    88,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
     124,   170,    -1,   172,   173,   174,    -1,    -1,   177,     4,
       5,     6,     7,     8,     9,    10,    -1,   141,   142,    14,
     144,   145,   146,    18,   148,   149,   150,    22,   152,   153,
     154,   155,    27,   157,   158,   159,   160,   161,    -1,   163,
      35,    -1,   166,   167,   168,    40,    41,    -1,    -1,    44,
      -1,    -1,    47,   177,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    64,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    78,    79,    80,    81,    -1,    -1,    -1,
      -1,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    98,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    12,    -1,   150,    -1,    -1,    17,    18,
      19,    20,    -1,    -1,   159,    -1,    25,   162,   163,   164,
      -1,    -1,    -1,   168,    33,   170,    35,   172,    -1,   174,
      39,    -1,   177,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,    -1,   141,   142,   143,   144,   145,   146,    -1,   148,
      -1,    -1,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,   163,    -1,    -1,   166,   167,   168,
     169,    -1,    -1,   172,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,    -1,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,     3,     4,     5,     6,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,   143,    -1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,   164,    22,    -1,
      -1,    25,    -1,   170,    -1,   172,    -1,   174,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
       3,    -1,   106,    -1,    -1,    -1,   110,    31,    -1,   113,
      -1,    -1,   116,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,   141,    -1,   143,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    62,
     164,    -1,    -1,    -1,    12,    89,    69,    70,   172,    93,
     174,    19,    20,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    54,   110,    -1,    -1,
     113,    -1,    -1,   116,    62,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,    -1,   163,
      -1,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,   103,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,   143,   144,   145,   146,    -1,
     148,    -1,    -1,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,     3,   163,    -1,    -1,   166,   167,
     168,   169,   170,    12,    -1,    -1,    -1,    -1,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    96,    97,    -1,
      99,    19,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    31,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,    -1,   141,   142,   143,   144,   145,   146,    -1,   148,
      -1,    -1,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,   163,    -1,    -1,   166,   167,   168,
     169,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,   177,
      -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,   177,
      -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,   177,
      -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,   177,
      -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,   177,
      -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,   177,
      -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,   177,
      -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,   170,    -1,    -1,    93,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    31,    32,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    47,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    89,    -1,    -1,    -1,    93,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    47,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    47,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    89,   166,   167,   168,    93,   170,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,   124,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    72,    -1,
     166,   167,   168,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    89,    -1,    -1,    12,    93,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,   165,   166,   167,   168,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
     124,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      72,    -1,   166,   167,   168,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    12,    89,    -1,    -1,
      -1,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    12,    89,   166,   167,   168,    93,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    89,
     166,   167,   168,    93,   170,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
     170,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,    89,   166,   167,   168,    93,   170,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    47,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    12,    89,   166,   167,   168,    93,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    89,
     166,   167,   168,    93,   170,    19,    -1,    -1,    -1,    -1,
      -1,   177,   102,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
     170,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    89,   166,   167,   168,    93,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    31,   124,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    72,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,   177,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    12,    13,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,    -1,    -1,    89,    -1,    -1,    12,    93,
      -1,   177,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    31,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,   177,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,   170,    -1,    -1,    -1,
      -1,    89,    -1,   177,    -1,    93,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,   165,   166,   167,   168,   123,   124,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    -1,   166,   167,
     168,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    12,    13,    -1,    -1,
     102,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,   124,    -1,
      -1,    -1,    12,    89,    -1,   177,    -1,    93,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    31,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    -1,    -1,    -1,    -1,   173,    -1,    -1,
      -1,   177,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    89,
     166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    89,   166,   167,   168,    93,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   177,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,   165,   166,   167,   168,    -1,    -1,    12,    89,    -1,
      -1,    -1,    93,   177,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    89,   166,   167,   168,    93,    -1,
      -1,    -1,    -1,    12,    -1,    -1,   177,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   124,
      -1,   166,   167,   168,    -1,    -1,    -1,    -1,   173,    -1,
      89,    -1,   177,    -1,    93,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    12,
      13,   166,   167,   168,   123,   124,    19,    -1,   173,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    12,    -1,   166,   167,   168,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,    -1,   124,    -1,    -1,
      -1,    12,    89,    -1,   177,    -1,    93,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      31,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   124,    -1,   166,
     167,   168,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,
     177,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    12,    13,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    31,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    89,   166,   167,   168,    93,    -1,
      -1,    -1,    12,    -1,   175,    -1,   177,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   124,
      -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,   177,    93,    -1,    -1,   141,   142,    -1,   144,
     145,   146,   102,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    12,
      -1,   166,   167,   168,   124,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    12,    13,   166,   167,   168,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    60,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    -1,   166,   167,
     168,    -1,   124,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    89,    -1,    -1,    -1,    93,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    89,
     166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,   102,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,    89,   166,   167,   168,    93,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
     102,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    31,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    89,   166,   167,   168,    93,    -1,    -1,
      -1,    12,    -1,    -1,    -1,   177,   102,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,   177,    93,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    22,   166,   167,   168,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    35,    -1,    37,
      38,    -1,    22,    -1,    -1,    43,    26,    27,    -1,    -1,
      -1,    -1,    50,    -1,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    85,    -1,    -1,
      88,    -1,    90,    -1,    -1,    -1,    76,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    85,    -1,    -1,    88,    -1,
      90,   109,    -1,    -1,   112,    22,    -1,   115,    -1,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,   109,
      37,    38,   112,    -1,    -1,   115,    43,    -1,    -1,    -1,
      -1,    -1,   140,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    76,
      -1,    78,    -1,   171,     1,    82,     3,    -1,    85,    -1,
      -1,    88,    -1,    90,    -1,   165,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,   109,    -1,    -1,   112,    33,    -1,   115,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,     3,    -1,   140,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    17,    18,    -1,    20,    -1,
      77,    -1,    -1,    25,    -1,    -1,    -1,    -1,   165,    -1,
      87,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    96,
      97,    -1,    99,    45,   101,    -1,   103,    49,    -1,   106,
      52,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,     3,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,    87,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,    96,    97,    -1,    99,    25,   101,
      27,   103,    -1,    -1,   106,    -1,    33,    -1,   110,    -1,
      -1,   113,    39,    -1,   116,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    62,    -1,    -1,   140,    -1,
      -1,    -1,    69,    70,    -1,    17,    18,    -1,    20,    -1,
      77,    78,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    33,    -1,    90,    -1,    -1,    -1,    39,    -1,    96,
      97,    -1,    99,    45,   101,    -1,   103,    49,    -1,   106,
      52,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,   116,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,    96,    97,    -1,    99,    25,   101,
      -1,   103,    -1,    -1,   106,    -1,    33,    -1,   110,   111,
      -1,   113,    39,    -1,   116,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    17,    18,    -1,    20,    -1,
      77,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    96,
      97,    -1,    99,    45,   101,    -1,   103,    49,    -1,   106,
      52,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,    96,    97,    -1,    99,    25,   101,
      -1,   103,    -1,    -1,   106,    -1,    33,    -1,   110,    -1,
      -1,   113,    39,    -1,   116,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116
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
     201,   205,   237,   193,   149,   195,   170,    46,    84,   114,
     202,    26,   320,   362,     8,   260,   362,   363,    55,   146,
     262,   218,     1,   192,   218,   266,   269,   172,   295,    53,
      73,    83,   304,    27,    78,    90,   109,   305,    27,    78,
      90,   109,   303,   218,   316,   317,   322,   164,   165,   362,
      12,    19,    31,    89,    93,   124,   141,   142,   144,   145,
     146,   148,   149,   150,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   166,   167,   168,   177,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   169,   291,   172,   174,    89,
      93,   362,   362,   362,   218,   335,    55,   172,   186,   165,
     205,    47,   348,   319,   165,   147,   165,   119,   219,   338,
     339,   340,   364,   170,   218,   172,   223,    32,    47,   223,
     120,   223,   351,    32,    47,   223,   351,   223,   351,    47,
     223,   351,   218,   218,   102,   205,   102,   125,   205,   291,
     202,   338,   172,   172,   205,   186,   111,   172,   218,   324,
     325,     1,   146,   331,    47,   147,   186,   223,   147,   223,
      13,   172,   172,   223,   338,   228,   228,    47,    90,   301,
     302,   173,   147,   111,   172,   218,   299,   146,   165,   362,
     362,   123,   281,   165,   170,   223,   172,   338,   165,   249,
     249,   223,   223,   165,   171,   171,   184,   147,   171,   362,
     147,   173,   147,   173,   362,   175,   351,    47,   147,   175,
     351,   123,   147,   175,     8,   362,   362,   335,     3,   172,
     196,     1,   171,   206,   212,   213,   362,   208,   362,    65,
      36,    72,   165,   260,   262,   109,   234,   284,   218,   335,
     170,   171,   269,   125,   147,   171,   173,   296,   298,   300,
     305,    90,     1,   146,   329,   330,    90,     1,     3,    12,
      17,    19,    20,    25,    45,    52,    54,    62,    70,    87,
      99,   103,   110,   116,   141,   142,   143,   144,   145,   146,
     148,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   163,   166,   167,   168,   169,   172,   218,   288,   289,
     290,   291,   341,   125,   318,   147,   165,   165,   362,   362,
     362,   238,   362,   238,   362,   362,   362,   362,   362,   362,
     362,     3,    20,    33,    62,   103,   109,   219,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,    68,   364,   364,   364,   364,
     364,   338,   338,   238,   362,   238,   362,    32,    47,    32,
      47,   102,   172,    47,   175,   218,    27,    47,    90,   115,
     350,   353,   354,   362,   378,   238,   362,   223,   322,   362,
     362,   125,   173,   147,    47,   335,    44,   362,   238,   362,
     172,   223,    44,   362,   238,   362,   223,   223,   238,   362,
     223,   125,   125,   186,    34,   186,   362,    34,   362,    65,
     173,   339,   218,   242,   243,   325,   147,   173,   222,   362,
     125,   326,   362,   323,   256,   218,   319,   362,   362,   173,
     299,   305,   277,   325,   146,   333,   334,   332,   282,   204,
       1,   254,   340,   173,    21,   250,   171,   173,   173,   336,
     173,   336,   186,   175,   238,   362,   175,   186,   362,   175,
     362,   175,   362,    47,    47,    47,   175,   195,   173,   197,
     198,   340,   171,   171,   147,   165,    13,   149,   147,   165,
      13,    36,    72,   218,   141,   142,   143,   144,   145,   159,
     163,   168,   207,   290,   291,   292,   362,   207,   209,   262,
     170,   235,   319,   165,   172,     1,   263,   362,   267,   268,
     147,   173,   331,     1,   222,   327,   328,   105,   306,   172,
     294,   362,   141,   149,   294,   294,   362,   317,   172,   174,
     165,   165,   165,   165,   165,   165,   173,   175,    44,   238,
     362,    44,   238,   362,   339,   238,   362,   172,    47,    90,
     147,   173,    17,    20,    25,    45,    52,    62,    70,    87,
      99,   110,   116,   341,    89,    89,   348,   348,   165,   165,
     364,   340,   362,   173,   362,    32,   223,    32,   223,   352,
     353,   362,    32,   223,   351,    32,   223,   351,   223,   351,
     223,   351,   362,   362,    34,   186,    34,    34,   186,   102,
     205,   218,   173,   147,   173,   173,   324,   331,    68,   364,
     218,   173,   173,   334,   146,   278,   173,   332,   151,   293,
     326,   362,   171,    74,   117,   171,   255,   173,   172,   205,
     218,   251,   186,   175,   351,   175,   351,   186,   123,   238,
     362,   238,   362,   238,   362,   147,   173,   212,   219,   170,
     292,   362,   111,   362,   207,   209,   147,   165,    13,   165,
     170,   263,   316,   322,   339,   171,    82,    85,   171,   185,
     192,   225,   269,   192,   298,   306,    60,   118,   310,   296,
     297,   173,   289,   291,   173,   175,   362,    32,    32,   362,
      32,    32,   173,   175,   175,   339,   353,   331,   341,   341,
     223,   223,   223,   102,    44,   362,    44,   362,   147,   173,
     102,    44,   362,   223,    44,   362,   223,   223,   223,   186,
     186,   362,   186,    34,   165,   165,   243,   205,   326,   172,
     172,   293,   326,   327,   306,   334,   362,    32,   186,   335,
     251,   146,   205,    44,   186,   362,   175,    44,   186,   362,
     175,   362,   175,   175,   198,   207,    13,    36,    72,    36,
      72,   165,   165,   292,   362,   362,   263,   171,   165,   165,
     173,   192,   225,   225,   269,   310,     3,    92,   103,   311,
     312,   313,   362,   285,   173,   294,   294,   102,    44,    44,
     102,    44,    44,    44,    44,   173,   326,   362,   362,   362,
     353,   362,   362,   362,    34,   186,   338,   338,   293,   326,
     186,   223,   173,   362,   362,   186,   362,   186,   171,   111,
     362,   207,   209,   207,   209,    13,   171,   165,   225,   272,
     313,   118,   147,   125,   152,   154,   155,   157,   158,    60,
      32,   165,   204,   224,   307,   308,   362,   362,   362,   362,
     362,   362,   102,   102,   102,   102,   186,   173,   173,   205,
     102,   102,   165,   165,   165,   165,   362,   308,   362,   312,
     313,   313,   313,   313,   313,   313,   311,   184,   102,   102,
     102,   102,   102,   102,   362,   362,   362,   362,   164,   164,
     362,   362
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   181,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   189,   189,   190,   190,   190,
     191,   192,   192,   193,   194,   195,   195,   196,   196,   196,
     197,   197,   198,   199,   200,   200,   200,   201,   201,   201,
     202,   202,   203,   204,   204,   204,   205,   206,   206,   207,
     207,   207,   207,   207,   207,   208,   208,   208,   208,   208,
     208,   209,   209,   210,   210,   210,   210,   210,   210,   210,
     211,   212,   212,   212,   212,   213,   213,   214,   215,   215,
     215,   215,   215,   215,   216,   216,   217,   217,   217,   217,
     217,   218,   218,   219,   219,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   222,   222,   222,   222,
     223,   223,   224,   224,   225,   225,   226,   225,   227,   225,
     228,   228,   228,   228,   228,   228,   228,   229,   229,   229,
     229,   230,   231,   231,   232,   233,   233,   233,   234,   233,
     235,   233,   236,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   238,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   240,   240,   241,   241,   242,   242,   243,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   245,   245,   245,   245,   245,   245,   246,   246,   246,
     247,   247,   247,   248,   248,   248,   248,   249,   249,   250,
     250,   250,   251,   251,   252,   253,   253,   254,   254,   255,
     255,   255,   256,   256,   256,   257,   257,   258,   259,   259,
     260,   261,   261,   261,   262,   262,   263,   263,   263,   263,
     263,   264,   264,   265,   266,   266,   267,   266,   266,   268,
     266,   269,   269,   270,   272,   271,   273,   274,   274,   274,
     275,   275,   276,   276,   277,   277,   277,   278,   278,   279,
     281,   280,   282,   280,   283,   285,   284,   286,   286,   286,
     286,   286,   287,   288,   288,   289,   289,   289,   290,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   292,   292,   293,   293,   294,
     294,   295,   295,   296,   296,   297,   297,   298,   299,   299,
     300,   300,   300,   300,   300,   300,   301,   301,   302,   302,
     302,   302,   302,   302,   302,   302,   302,   303,   303,   303,
     303,   303,   303,   304,   304,   304,   305,   305,   305,   305,
     305,   305,   306,   306,   307,   307,   308,   308,   308,   309,
     310,   310,   310,   310,   310,   311,   311,   312,   312,   312,
     312,   312,   312,   312,   313,   313,   314,   315,   315,   316,
     316,   317,   318,   318,   319,   319,   319,   319,   319,   321,
     320,   320,   322,   322,   323,   323,   324,   324,   324,   325,
     325,   325,   326,   326,   326,   327,   328,   328,   328,   329,
     329,   330,   330,   331,   331,   331,   331,   332,   332,   333,
     334,   334,   335,   335,   336,   336,   337,   337,   338,   338,
     339,   339,   340,   340,   341,   341,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     344,   344,   344,   344,   344,   344,   344,   345,   346,   347,
     347,   347,   347,   347,   347,   347,   348,   348,   349,   350,
     350,   351,   352,   352,   353,   353,   353,   354,   354,   354,
     354,   354,   354,   355,   355,   355,   355,   355,   356,   356,
     356,   356,   356,   357,   358,   358,   358,   358,   358,   358,
     359,   360,   361,   361,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   363,   363,   364,   364,   364,   365,   365,   365,   365,
     366,   366,   366,   366,   366,   367,   367,   367,   368,   368,
     368,   368,   368,   368,   369,   369,   369,   369,   370,   370,
     371,   371,   372,   372,   372,   372,   372,   372,   372,   372,
     372,   372,   372,   372,   372,   373,   373,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     375,   375,   375,   375,   375,   375,   375,   376,   376,   376,
     376,   377,   377,   377,   377,   378,   378,   378,   378,   378,
     378,   378
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     3,     3,     3,     3,     2,     1,
       1,     1,     1,     1,     2,     1,     3,     3,     3,     3,
       2,     3,     3,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     2,     3,     2,     0,     3,     0,     2,     3,
       1,     3,     1,     4,     3,     4,     4,     0,     1,     1,
       0,     1,     6,     2,     3,     3,     1,     1,     2,     1,
       1,     3,     3,     3,     5,     1,     3,     3,     3,     5,
       5,     0,     1,     4,     6,     8,     8,     6,     8,     8,
       4,     1,     3,     3,     5,     1,     3,     3,     4,     4,
       4,     4,     4,     4,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     3,     1,     1,     0,     3,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     3,     5,     5,
       2,     1,     1,     1,     1,     6,     7,     3,     0,     6,
       0,     6,     2,     5,     3,     3,     6,     6,     4,     5,
       5,     3,     3,     6,     5,     6,     5,     6,     3,     4,
       3,     4,     5,     6,     5,     6,     3,     4,     3,     4,
       6,     7,     6,     7,     4,     5,     4,     5,     4,     4,
       3,     6,     5,     4,     3,     6,     5,     6,     5,     8,
       7,     4,     4,     6,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     6,     4,     7,
       5,     3,     6,     2,     2,     3,     3,     0,     2,     2,
       3,     5,     1,     3,     3,     5,     5,     0,     2,     3,
       2,     3,     4,     3,     1,     1,     3,     3,     5,     5,
       2,     1,     1,     1,     0,     2,     0,     2,     3,     3,
       4,     3,     3,     3,     1,     2,     0,     4,     2,     0,
       5,     1,     3,     1,     0,     8,     0,     1,     1,     1,
       0,     1,     1,     3,     1,     3,     1,     0,     2,     6,
       0,     3,     0,     4,     1,     0,     7,     4,     4,     6,
       6,     4,     2,     1,     3,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     0,
       3,     2,     3,     1,     3,     0,     1,     1,     1,     1,
       4,     5,     4,     5,     6,     6,     0,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     0,     1,     1,
       2,     1,     1,     1,     1,     1,     0,     1,     2,     1,
       1,     1,     0,     1,     1,     1,     1,     2,     1,     1,
       0,     2,     2,     4,     4,     1,     3,     3,     3,     3,
       3,     3,     3,     2,     1,     1,     3,     1,     2,     1,
       3,     2,     0,     2,     1,     2,     1,     1,     1,     0,
       5,     3,     1,     3,     3,     5,     1,     1,     3,     1,
       2,     3,     0,     2,     2,     1,     1,     1,     1,     2,
       1,     0,     1,     0,     2,     2,     1,     1,     1,     2,
       0,     1,     1,     3,     1,     1,     3,     3,     0,     1,
       1,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     4,     4,     4,     5,     2,     2,     1,     2,
       1,     2,     1,     2,     1,     2,     1,     1,     6,     6,
       4,     9,     9,     7,     6,     6,     4,     9,     9,     7,
       2,     3,     4,     6,     6,     9,     9,     6,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     4,     1,
       3,     4,     1,     3,     4,     3,     3,     1,     1,     2,
       1,     2,     1,     1,     2,     2,     2,     2,     2,     8,
       8,     9,     9,     4,     3,     3,     2,     2,     2,     1,
       3,     4,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     4,     4,     4,     3,     3,
       3,     3,     5,     5,     3,     4,     3,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       4,     3,     4,     3,     4,     3,     5,     3,     3,     3,
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
#line 634 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6221 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 638 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6227 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 639 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6233 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 646 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6241 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 654 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6249 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 658 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6258 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 668 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6266 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 673 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6274 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 680 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6280 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 681 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6286 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6292 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6298 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6304 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6310 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6316 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6322 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6328 "bison-chpl-lib.cpp"
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
#line 6345 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 702 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6351 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 703 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6357 "bison-chpl-lib.cpp"
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
#line 6370 "bison-chpl-lib.cpp"
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
#line 6383 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 723 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6391 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 727 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6402 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 734 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6411 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 741 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6417 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 742 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6423 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 743 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6429 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 744 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6435 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 745 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6441 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 746 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6447 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 747 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6453 "bison-chpl-lib.cpp"
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
#line 6471 "bison-chpl-lib.cpp"
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
#line 6490 "bison-chpl-lib.cpp"
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
#line 6502 "bison-chpl-lib.cpp"
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
#line 6518 "bison-chpl-lib.cpp"
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
#line 6533 "bison-chpl-lib.cpp"
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
#line 6549 "bison-chpl-lib.cpp"
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
#line 6565 "bison-chpl-lib.cpp"
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
#line 6580 "bison-chpl-lib.cpp"
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
#line 6597 "bison-chpl-lib.cpp"
    break;

  case 53: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 880 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6605 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_begin: TATMARK TIDENT  */
#line 887 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6614 "bison-chpl-lib.cpp"
    break;

  case 55: /* opt_attribute_decl_toolspace: %empty  */
#line 894 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6622 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 898 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6633 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_actuals: %empty  */
#line 907 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6639 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 909 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6645 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 911 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6653 "bison-chpl-lib.cpp"
    break;

  case 60: /* attribute_actuals_ls: attribute_actual  */
#line 918 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6662 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 923 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6672 "bison-chpl-lib.cpp"
    break;

  case 63: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 936 "chpl.ypp"
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
#line 6693 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 956 "chpl.ypp"
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
#line 6712 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 971 "chpl.ypp"
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
#line 6731 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 986 "chpl.ypp"
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
#line 6748 "bison-chpl-lib.cpp"
    break;

  case 67: /* opt_access_control: %empty  */
#line 1001 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6754 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: TPUBLIC  */
#line 1002 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6761 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPRIVATE  */
#line 1004 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6768 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_prototype: %empty  */
#line 1009 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6774 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: TPROTOTYPE  */
#line 1010 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6781 "bison-chpl-lib.cpp"
    break;

  case 72: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1016 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6791 "bison-chpl-lib.cpp"
    break;

  case 73: /* block_stmt_body: TLCBR TRCBR  */
#line 1034 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6799 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1038 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6807 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR error TRCBR  */
#line 1042 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6819 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt: block_stmt_body  */
#line 1053 "chpl.ypp"
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
#line 6838 "bison-chpl-lib.cpp"
    break;

  case 77: /* stmt_ls: toplevel_stmt  */
#line 1071 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6844 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1072 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6850 "bison-chpl-lib.cpp"
    break;

  case 79: /* renames_ls: expr  */
#line 1077 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6858 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: all_op_name  */
#line 1081 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6866 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: expr TAS expr  */
#line 1085 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6875 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: renames_ls TCOMMA expr  */
#line 1090 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6883 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1094 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6891 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1098 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6900 "bison-chpl-lib.cpp"
    break;

  case 85: /* use_renames_ls: expr  */
#line 1108 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6909 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr TAS expr  */
#line 1113 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6919 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1119 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6930 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1126 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6939 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1131 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6950 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1138 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6962 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: %empty  */
#line 1148 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6968 "bison-chpl-lib.cpp"
    break;

  case 92: /* opt_only_ls: renames_ls  */
#line 1149 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6974 "bison-chpl-lib.cpp"
    break;

  case 93: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1154 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6983 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1159 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6995 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1167 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7008 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1176 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7022 "bison-chpl-lib.cpp"
    break;

  case 97: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1186 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7034 "bison-chpl-lib.cpp"
    break;

  case 98: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1194 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7047 "bison-chpl-lib.cpp"
    break;

  case 99: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1203 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7061 "bison-chpl-lib.cpp"
    break;

  case 100: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1216 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7070 "bison-chpl-lib.cpp"
    break;

  case 101: /* import_expr: expr  */
#line 1224 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7078 "bison-chpl-lib.cpp"
    break;

  case 102: /* import_expr: expr TDOT all_op_name  */
#line 1228 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7087 "bison-chpl-lib.cpp"
    break;

  case 103: /* import_expr: expr TAS ident_use  */
#line 1233 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7097 "bison-chpl-lib.cpp"
    break;

  case 104: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1239 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7107 "bison-chpl-lib.cpp"
    break;

  case 105: /* import_ls: import_expr  */
#line 1247 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7113 "bison-chpl-lib.cpp"
    break;

  case 106: /* import_ls: import_ls TCOMMA import_expr  */
#line 1248 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7119 "bison-chpl-lib.cpp"
    break;

  case 107: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1253 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7129 "bison-chpl-lib.cpp"
    break;

  case 108: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1262 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7137 "bison-chpl-lib.cpp"
    break;

  case 109: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1266 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7145 "bison-chpl-lib.cpp"
    break;

  case 110: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1270 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7153 "bison-chpl-lib.cpp"
    break;

  case 111: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1274 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7161 "bison-chpl-lib.cpp"
    break;

  case 112: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1278 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7169 "bison-chpl-lib.cpp"
    break;

  case 113: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1282 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7177 "bison-chpl-lib.cpp"
    break;

  case 114: /* opt_label_ident: %empty  */
#line 1290 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7183 "bison-chpl-lib.cpp"
    break;

  case 115: /* opt_label_ident: TIDENT  */
#line 1291 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7189 "bison-chpl-lib.cpp"
    break;

  case 123: /* ident_use: TIDENT  */
#line 1316 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7195 "bison-chpl-lib.cpp"
    break;

  case 124: /* ident_use: TTHIS  */
#line 1317 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7201 "bison-chpl-lib.cpp"
    break;

  case 144: /* scalar_type: TBOOL  */
#line 1344 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7207 "bison-chpl-lib.cpp"
    break;

  case 145: /* scalar_type: TENUM  */
#line 1345 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7213 "bison-chpl-lib.cpp"
    break;

  case 146: /* scalar_type: TINT  */
#line 1346 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7219 "bison-chpl-lib.cpp"
    break;

  case 147: /* scalar_type: TUINT  */
#line 1347 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7225 "bison-chpl-lib.cpp"
    break;

  case 148: /* scalar_type: TREAL  */
#line 1348 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7231 "bison-chpl-lib.cpp"
    break;

  case 149: /* scalar_type: TIMAG  */
#line 1349 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7237 "bison-chpl-lib.cpp"
    break;

  case 150: /* scalar_type: TCOMPLEX  */
#line 1350 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7243 "bison-chpl-lib.cpp"
    break;

  case 151: /* scalar_type: TBYTES  */
#line 1351 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7249 "bison-chpl-lib.cpp"
    break;

  case 152: /* scalar_type: TSTRING  */
#line 1352 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7255 "bison-chpl-lib.cpp"
    break;

  case 153: /* scalar_type: TLOCALE  */
#line 1353 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7261 "bison-chpl-lib.cpp"
    break;

  case 154: /* scalar_type: TNOTHING  */
#line 1354 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7267 "bison-chpl-lib.cpp"
    break;

  case 155: /* scalar_type: TVOID  */
#line 1355 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7273 "bison-chpl-lib.cpp"
    break;

  case 160: /* do_stmt: TDO stmt  */
#line 1369 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7279 "bison-chpl-lib.cpp"
    break;

  case 161: /* do_stmt: block_stmt  */
#line 1370 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7285 "bison-chpl-lib.cpp"
    break;

  case 162: /* return_stmt: TRETURN TSEMI  */
#line 1375 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7296 "bison-chpl-lib.cpp"
    break;

  case 163: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1382 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7307 "bison-chpl-lib.cpp"
    break;

  case 164: /* class_level_stmt: TSEMI  */
#line 1392 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7315 "bison-chpl-lib.cpp"
    break;

  case 165: /* class_level_stmt: inner_class_level_stmt  */
#line 1396 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7325 "bison-chpl-lib.cpp"
    break;

  case 166: /* $@1: %empty  */
#line 1401 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7332 "bison-chpl-lib.cpp"
    break;

  case 167: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1403 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7341 "bison-chpl-lib.cpp"
    break;

  case 168: /* $@2: %empty  */
#line 1407 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7348 "bison-chpl-lib.cpp"
    break;

  case 169: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1409 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7357 "bison-chpl-lib.cpp"
    break;

  case 177: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1427 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7366 "bison-chpl-lib.cpp"
    break;

  case 178: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1432 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7375 "bison-chpl-lib.cpp"
    break;

  case 179: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1437 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7384 "bison-chpl-lib.cpp"
    break;

  case 180: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1442 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7392 "bison-chpl-lib.cpp"
    break;

  case 181: /* forwarding_decl_start: TFORWARDING  */
#line 1449 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7401 "bison-chpl-lib.cpp"
    break;

  case 182: /* extern_or_export: TEXTERN  */
#line 1456 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7407 "bison-chpl-lib.cpp"
    break;

  case 183: /* extern_or_export: TEXPORT  */
#line 1457 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7413 "bison-chpl-lib.cpp"
    break;

  case 184: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1462 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7423 "bison-chpl-lib.cpp"
    break;

  case 185: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1472 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7432 "bison-chpl-lib.cpp"
    break;

  case 186: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1478 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7443 "bison-chpl-lib.cpp"
    break;

  case 187: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1485 "chpl.ypp"
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
#line 7465 "bison-chpl-lib.cpp"
    break;

  case 188: /* $@3: %empty  */
#line 1503 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7473 "bison-chpl-lib.cpp"
    break;

  case 189: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1507 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7482 "bison-chpl-lib.cpp"
    break;

  case 190: /* $@4: %empty  */
#line 1512 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7491 "bison-chpl-lib.cpp"
    break;

  case 191: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1517 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7500 "bison-chpl-lib.cpp"
    break;

  case 192: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1525 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7508 "bison-chpl-lib.cpp"
    break;

  case 193: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1532 "chpl.ypp"
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
#line 7530 "bison-chpl-lib.cpp"
    break;

  case 194: /* loop_stmt: TWHILE expr do_stmt  */
#line 1550 "chpl.ypp"
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
#line 7547 "bison-chpl-lib.cpp"
    break;

  case 195: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1563 "chpl.ypp"
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
#line 7564 "bison-chpl-lib.cpp"
    break;

  case 196: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1576 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7572 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1580 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7580 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1584 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7588 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1588 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7596 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1592 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7604 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFOR expr do_stmt  */
#line 1596 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7612 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1600 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7620 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1604 "chpl.ypp"
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
#line 7644 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1624 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7652 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1628 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7660 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1632 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7668 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1636 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7676 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFORALL expr do_stmt  */
#line 1640 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7684 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1644 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7692 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1648 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7700 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1652 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7708 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1656 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7716 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1660 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7724 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1664 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7732 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1668 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7740 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1672 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7748 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1676 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7756 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1680 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7764 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1684 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7772 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1688 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7780 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1692 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7788 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1696 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7796 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1700 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7804 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1704 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7812 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1708 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7820 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1712 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7829 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1717 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7838 "bison-chpl-lib.cpp"
    break;

  case 228: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1725 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7848 "bison-chpl-lib.cpp"
    break;

  case 229: /* if_stmt: TIF expr TTHEN stmt  */
#line 1734 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7856 "bison-chpl-lib.cpp"
    break;

  case 230: /* if_stmt: TIF expr block_stmt  */
#line 1738 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7864 "bison-chpl-lib.cpp"
    break;

  case 231: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1742 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7872 "bison-chpl-lib.cpp"
    break;

  case 232: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1746 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7880 "bison-chpl-lib.cpp"
    break;

  case 233: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1750 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7888 "bison-chpl-lib.cpp"
    break;

  case 234: /* if_stmt: TIF ifvar block_stmt  */
#line 1754 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7896 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1758 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7904 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1762 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7912 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1766 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7921 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1771 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7930 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1776 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7939 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1781 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7948 "bison-chpl-lib.cpp"
    break;

  case 241: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1789 "chpl.ypp"
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
#line 7967 "bison-chpl-lib.cpp"
    break;

  case 242: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1804 "chpl.ypp"
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
#line 7986 "bison-chpl-lib.cpp"
    break;

  case 243: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1822 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7994 "bison-chpl-lib.cpp"
    break;

  case 244: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1826 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8002 "bison-chpl-lib.cpp"
    break;

  case 245: /* ifc_formal_ls: ifc_formal  */
#line 1832 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8008 "bison-chpl-lib.cpp"
    break;

  case 246: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1833 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8014 "bison-chpl-lib.cpp"
    break;

  case 247: /* ifc_formal: ident_def  */
#line 1838 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8020 "bison-chpl-lib.cpp"
    break;

  case 260: /* implements_type_ident: implements_type_error_ident  */
#line 1856 "chpl.ypp"
  {
    context->syntax((yyloc), "type '%s' cannot implement an interface.", (yyvsp[0].uniqueStr).c_str());
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8029 "bison-chpl-lib.cpp"
    break;

  case 267: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1882 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8037 "bison-chpl-lib.cpp"
    break;

  case 268: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1886 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8045 "bison-chpl-lib.cpp"
    break;

  case 269: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1890 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8053 "bison-chpl-lib.cpp"
    break;

  case 270: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1897 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8061 "bison-chpl-lib.cpp"
    break;

  case 271: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1901 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8069 "bison-chpl-lib.cpp"
    break;

  case 272: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1905 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8077 "bison-chpl-lib.cpp"
    break;

  case 273: /* try_stmt: TTRY tryable_stmt  */
#line 1912 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8085 "bison-chpl-lib.cpp"
    break;

  case 274: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1916 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8093 "bison-chpl-lib.cpp"
    break;

  case 275: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1920 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8101 "bison-chpl-lib.cpp"
    break;

  case 276: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1924 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8109 "bison-chpl-lib.cpp"
    break;

  case 277: /* catch_expr_ls: %empty  */
#line 1930 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8115 "bison-chpl-lib.cpp"
    break;

  case 278: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1931 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8121 "bison-chpl-lib.cpp"
    break;

  case 279: /* catch_expr: TCATCH block_stmt  */
#line 1936 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8129 "bison-chpl-lib.cpp"
    break;

  case 280: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1940 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8137 "bison-chpl-lib.cpp"
    break;

  case 281: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1944 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8145 "bison-chpl-lib.cpp"
    break;

  case 282: /* catch_expr_inner: ident_def  */
#line 1951 "chpl.ypp"
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
#line 8162 "bison-chpl-lib.cpp"
    break;

  case 283: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1964 "chpl.ypp"
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
#line 8179 "bison-chpl-lib.cpp"
    break;

  case 284: /* throw_stmt: TTHROW expr TSEMI  */
#line 1980 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8189 "bison-chpl-lib.cpp"
    break;

  case 285: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1988 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8197 "bison-chpl-lib.cpp"
    break;

  case 286: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1992 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8207 "bison-chpl-lib.cpp"
    break;

  case 287: /* when_stmt_ls: %empty  */
#line 2000 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8213 "bison-chpl-lib.cpp"
    break;

  case 288: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2001 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8219 "bison-chpl-lib.cpp"
    break;

  case 289: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2006 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8227 "bison-chpl-lib.cpp"
    break;

  case 290: /* when_stmt: TOTHERWISE stmt  */
#line 2010 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8236 "bison-chpl-lib.cpp"
    break;

  case 291: /* when_stmt: TOTHERWISE TDO stmt  */
#line 2015 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8245 "bison-chpl-lib.cpp"
    break;

  case 292: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2023 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8254 "bison-chpl-lib.cpp"
    break;

  case 293: /* manager_expr: expr TAS ident_def  */
#line 2028 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8262 "bison-chpl-lib.cpp"
    break;

  case 294: /* manager_expr: expr  */
#line 2032 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8270 "bison-chpl-lib.cpp"
    break;

  case 295: /* manager_expr_ls: manager_expr  */
#line 2038 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8276 "bison-chpl-lib.cpp"
    break;

  case 296: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2039 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8282 "bison-chpl-lib.cpp"
    break;

  case 297: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2044 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8290 "bison-chpl-lib.cpp"
    break;

  case 298: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2053 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8299 "bison-chpl-lib.cpp"
    break;

  case 299: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2058 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8310 "bison-chpl-lib.cpp"
    break;

  case 300: /* class_start: class_tag ident_def  */
#line 2069 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8318 "bison-chpl-lib.cpp"
    break;

  case 301: /* class_tag: TCLASS  */
#line 2075 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8324 "bison-chpl-lib.cpp"
    break;

  case 302: /* class_tag: TRECORD  */
#line 2076 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8330 "bison-chpl-lib.cpp"
    break;

  case 303: /* class_tag: TUNION  */
#line 2077 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8336 "bison-chpl-lib.cpp"
    break;

  case 304: /* opt_inherit: %empty  */
#line 2081 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8342 "bison-chpl-lib.cpp"
    break;

  case 305: /* opt_inherit: TCOLON expr_ls  */
#line 2082 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8348 "bison-chpl-lib.cpp"
    break;

  case 306: /* class_level_stmt_ls: %empty  */
#line 2086 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8357 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2091 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8365 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2095 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8373 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2099 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8381 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2103 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8389 "bison-chpl-lib.cpp"
    break;

  case 311: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2110 "chpl.ypp"
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
#line 8410 "bison-chpl-lib.cpp"
    break;

  case 312: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2127 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8424 "bison-chpl-lib.cpp"
    break;

  case 313: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2140 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8432 "bison-chpl-lib.cpp"
    break;

  case 314: /* enum_ls: enum_item  */
#line 2147 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8441 "bison-chpl-lib.cpp"
    break;

  case 315: /* enum_ls: enum_ls TCOMMA  */
#line 2152 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8451 "bison-chpl-lib.cpp"
    break;

  case 316: /* $@5: %empty  */
#line 2158 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8460 "bison-chpl-lib.cpp"
    break;

  case 317: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2163 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8469 "bison-chpl-lib.cpp"
    break;

  case 318: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2168 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8478 "bison-chpl-lib.cpp"
    break;

  case 319: /* $@6: %empty  */
#line 2173 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8487 "bison-chpl-lib.cpp"
    break;

  case 320: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2178 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8496 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_item: ident_def  */
#line 2188 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8507 "bison-chpl-lib.cpp"
    break;

  case 322: /* enum_item: ident_def TASSIGN expr  */
#line 2195 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8520 "bison-chpl-lib.cpp"
    break;

  case 323: /* lambda_decl_start: TLAMBDA  */
#line 2207 "chpl.ypp"
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
#line 8536 "bison-chpl-lib.cpp"
    break;

  case 324: /* $@7: %empty  */
#line 2224 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8546 "bison-chpl-lib.cpp"
    break;

  case 325: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2230 "chpl.ypp"
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
#line 8568 "bison-chpl-lib.cpp"
    break;

  case 327: /* linkage_spec: linkage_spec_empty  */
#line 2253 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8574 "bison-chpl-lib.cpp"
    break;

  case 328: /* linkage_spec: TINLINE  */
#line 2254 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8581 "bison-chpl-lib.cpp"
    break;

  case 329: /* linkage_spec: TOVERRIDE  */
#line 2256 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8588 "bison-chpl-lib.cpp"
    break;

  case 330: /* opt_fn_type_formal_ls: %empty  */
#line 2261 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8594 "bison-chpl-lib.cpp"
    break;

  case 331: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2262 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8600 "bison-chpl-lib.cpp"
    break;

  case 332: /* fn_type_formal_ls: fn_type_formal  */
#line 2266 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8606 "bison-chpl-lib.cpp"
    break;

  case 333: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2267 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8612 "bison-chpl-lib.cpp"
    break;

  case 334: /* fn_type_formal: named_formal  */
#line 2272 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8618 "bison-chpl-lib.cpp"
    break;

  case 335: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2275 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8624 "bison-chpl-lib.cpp"
    break;

  case 336: /* fn_type_formal: formal_type  */
#line 2277 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8630 "bison-chpl-lib.cpp"
    break;

  case 337: /* opt_fn_type_ret_type: %empty  */
#line 2281 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8636 "bison-chpl-lib.cpp"
    break;

  case 338: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2282 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8642 "bison-chpl-lib.cpp"
    break;

  case 339: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2295 "chpl.ypp"
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
#line 8661 "bison-chpl-lib.cpp"
    break;

  case 340: /* $@8: %empty  */
#line 2313 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8671 "bison-chpl-lib.cpp"
    break;

  case 341: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2319 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8684 "bison-chpl-lib.cpp"
    break;

  case 342: /* $@9: %empty  */
#line 2328 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8694 "bison-chpl-lib.cpp"
    break;

  case 343: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2334 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8707 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2346 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8715 "bison-chpl-lib.cpp"
    break;

  case 345: /* $@10: %empty  */
#line 2355 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-4].functionParts).errorExpr == nullptr) {
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
  }
#line 8729 "bison-chpl-lib.cpp"
    break;

  case 346: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2365 "chpl.ypp"
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
#line 8752 "bison-chpl-lib.cpp"
    break;

  case 347: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2387 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8764 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2395 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8776 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2403 "chpl.ypp"
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
#line 8791 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2414 "chpl.ypp"
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
#line 8806 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2425 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8816 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2434 "chpl.ypp"
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
#line 8832 "bison-chpl-lib.cpp"
    break;

  case 354: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2449 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8838 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_ident: ident_def TBANG  */
#line 2456 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8848 "bison-chpl-lib.cpp"
    break;

  case 397: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2513 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8854 "bison-chpl-lib.cpp"
    break;

  case 398: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2514 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8860 "bison-chpl-lib.cpp"
    break;

  case 399: /* opt_formal_ls: %empty  */
#line 2518 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8866 "bison-chpl-lib.cpp"
    break;

  case 400: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2519 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8872 "bison-chpl-lib.cpp"
    break;

  case 401: /* req_formal_ls: TLP TRP  */
#line 2523 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8878 "bison-chpl-lib.cpp"
    break;

  case 402: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2524 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8884 "bison-chpl-lib.cpp"
    break;

  case 403: /* formal_ls_inner: formal  */
#line 2528 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8890 "bison-chpl-lib.cpp"
    break;

  case 404: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2529 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8896 "bison-chpl-lib.cpp"
    break;

  case 405: /* formal_ls: %empty  */
#line 2533 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8902 "bison-chpl-lib.cpp"
    break;

  case 406: /* formal_ls: formal_ls_inner  */
#line 2534 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8908 "bison-chpl-lib.cpp"
    break;

  case 410: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2548 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8916 "bison-chpl-lib.cpp"
    break;

  case 411: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2553 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8924 "bison-chpl-lib.cpp"
    break;

  case 412: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2558 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8932 "bison-chpl-lib.cpp"
    break;

  case 413: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2563 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8940 "bison-chpl-lib.cpp"
    break;

  case 414: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2568 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8948 "bison-chpl-lib.cpp"
    break;

  case 415: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2573 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8956 "bison-chpl-lib.cpp"
    break;

  case 416: /* opt_formal_intent_tag: %empty  */
#line 2579 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8965 "bison-chpl-lib.cpp"
    break;

  case 417: /* opt_formal_intent_tag: required_intent_tag  */
#line 2584 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8974 "bison-chpl-lib.cpp"
    break;

  case 418: /* required_intent_tag: TIN  */
#line 2591 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8980 "bison-chpl-lib.cpp"
    break;

  case 419: /* required_intent_tag: TINOUT  */
#line 2592 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8986 "bison-chpl-lib.cpp"
    break;

  case 420: /* required_intent_tag: TOUT  */
#line 2593 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8992 "bison-chpl-lib.cpp"
    break;

  case 421: /* required_intent_tag: TCONST TIN  */
#line 2594 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8998 "bison-chpl-lib.cpp"
    break;

  case 422: /* required_intent_tag: TCONST TREF  */
#line 2595 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9004 "bison-chpl-lib.cpp"
    break;

  case 423: /* required_intent_tag: TCONST  */
#line 2596 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9010 "bison-chpl-lib.cpp"
    break;

  case 424: /* required_intent_tag: TPARAM  */
#line 2597 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9016 "bison-chpl-lib.cpp"
    break;

  case 425: /* required_intent_tag: TREF  */
#line 2598 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9022 "bison-chpl-lib.cpp"
    break;

  case 426: /* required_intent_tag: TTYPE  */
#line 2599 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9028 "bison-chpl-lib.cpp"
    break;

  case 427: /* opt_this_intent_tag: %empty  */
#line 2603 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9034 "bison-chpl-lib.cpp"
    break;

  case 428: /* opt_this_intent_tag: TPARAM  */
#line 2604 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9040 "bison-chpl-lib.cpp"
    break;

  case 429: /* opt_this_intent_tag: TREF  */
#line 2605 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9046 "bison-chpl-lib.cpp"
    break;

  case 430: /* opt_this_intent_tag: TCONST TREF  */
#line 2606 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9052 "bison-chpl-lib.cpp"
    break;

  case 431: /* opt_this_intent_tag: TCONST  */
#line 2607 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9058 "bison-chpl-lib.cpp"
    break;

  case 432: /* opt_this_intent_tag: TTYPE  */
#line 2608 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9064 "bison-chpl-lib.cpp"
    break;

  case 433: /* proc_iter_or_op: TPROC  */
#line 2612 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9070 "bison-chpl-lib.cpp"
    break;

  case 434: /* proc_iter_or_op: TITER  */
#line 2613 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9076 "bison-chpl-lib.cpp"
    break;

  case 435: /* proc_iter_or_op: TOPERATOR  */
#line 2614 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9082 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_ret_tag: %empty  */
#line 2618 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9088 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_ret_tag: TCONST  */
#line 2619 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 9094 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_ret_tag: TCONST TREF  */
#line 2620 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 9100 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_ret_tag: TREF  */
#line 2621 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 9106 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_ret_tag: TPARAM  */
#line 2622 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 9112 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_ret_tag: TTYPE  */
#line 2623 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 9118 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_throws_error: %empty  */
#line 2627 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9124 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_throws_error: TTHROWS  */
#line 2628 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9130 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_function_body_stmt: TSEMI  */
#line 2631 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9136 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_function_body_stmt: function_body_stmt  */
#line 2632 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9142 "bison-chpl-lib.cpp"
    break;

  case 446: /* function_body_stmt: block_stmt_body  */
#line 2636 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9148 "bison-chpl-lib.cpp"
    break;

  case 447: /* function_body_stmt: TDO toplevel_stmt  */
#line 2637 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 9154 "bison-chpl-lib.cpp"
    break;

  case 448: /* function_body_stmt: return_stmt  */
#line 2638 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt), &((yylsp[0]))); }
#line 9160 "bison-chpl-lib.cpp"
    break;

  case 449: /* query_expr: TQUERIEDIDENT  */
#line 2642 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9166 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_lifetime_where: %empty  */
#line 2647 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9172 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_lifetime_where: TWHERE expr  */
#line 2649 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9178 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2651 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9184 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2653 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9190 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2655 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9196 "bison-chpl-lib.cpp"
    break;

  case 455: /* lifetime_components_expr: lifetime_expr  */
#line 2660 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9202 "bison-chpl-lib.cpp"
    break;

  case 456: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2662 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9208 "bison-chpl-lib.cpp"
    break;

  case 457: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2667 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9214 "bison-chpl-lib.cpp"
    break;

  case 458: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2669 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9220 "bison-chpl-lib.cpp"
    break;

  case 459: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2671 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9226 "bison-chpl-lib.cpp"
    break;

  case 460: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2673 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9232 "bison-chpl-lib.cpp"
    break;

  case 461: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2675 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9238 "bison-chpl-lib.cpp"
    break;

  case 462: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2677 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9244 "bison-chpl-lib.cpp"
    break;

  case 463: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2679 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9250 "bison-chpl-lib.cpp"
    break;

  case 464: /* lifetime_ident: TIDENT  */
#line 2683 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9256 "bison-chpl-lib.cpp"
    break;

  case 465: /* lifetime_ident: TTHIS  */
#line 2684 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9262 "bison-chpl-lib.cpp"
    break;

  case 466: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2689 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9271 "bison-chpl-lib.cpp"
    break;

  case 467: /* type_alias_decl_stmt_start: TTYPE  */
#line 2698 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9279 "bison-chpl-lib.cpp"
    break;

  case 468: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2702 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9288 "bison-chpl-lib.cpp"
    break;

  case 469: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2710 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9296 "bison-chpl-lib.cpp"
    break;

  case 470: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2714 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9304 "bison-chpl-lib.cpp"
    break;

  case 471: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2721 "chpl.ypp"
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
#line 9326 "bison-chpl-lib.cpp"
    break;

  case 472: /* opt_init_type: %empty  */
#line 2741 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9332 "bison-chpl-lib.cpp"
    break;

  case 473: /* opt_init_type: TASSIGN expr  */
#line 2743 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9338 "bison-chpl-lib.cpp"
    break;

  case 474: /* var_decl_type: TPARAM  */
#line 2747 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9344 "bison-chpl-lib.cpp"
    break;

  case 475: /* var_decl_type: TCONST TREF  */
#line 2748 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9350 "bison-chpl-lib.cpp"
    break;

  case 476: /* var_decl_type: TREF  */
#line 2749 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9356 "bison-chpl-lib.cpp"
    break;

  case 477: /* var_decl_type: TCONST  */
#line 2750 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9362 "bison-chpl-lib.cpp"
    break;

  case 478: /* var_decl_type: TVAR  */
#line 2751 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9368 "bison-chpl-lib.cpp"
    break;

  case 479: /* $@11: %empty  */
#line 2756 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9377 "bison-chpl-lib.cpp"
    break;

  case 480: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2760 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9386 "bison-chpl-lib.cpp"
    break;

  case 481: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2765 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9395 "bison-chpl-lib.cpp"
    break;

  case 482: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2773 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9403 "bison-chpl-lib.cpp"
    break;

  case 483: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2777 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9411 "bison-chpl-lib.cpp"
    break;

  case 484: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2784 "chpl.ypp"
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
#line 9431 "bison-chpl-lib.cpp"
    break;

  case 485: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2800 "chpl.ypp"
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
#line 9449 "bison-chpl-lib.cpp"
    break;

  case 486: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2817 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9457 "bison-chpl-lib.cpp"
    break;

  case 487: /* tuple_var_decl_component: ident_def  */
#line 2821 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9465 "bison-chpl-lib.cpp"
    break;

  case 488: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2825 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9473 "bison-chpl-lib.cpp"
    break;

  case 489: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2832 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9479 "bison-chpl-lib.cpp"
    break;

  case 490: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2834 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9485 "bison-chpl-lib.cpp"
    break;

  case 491: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2836 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9491 "bison-chpl-lib.cpp"
    break;

  case 492: /* opt_init_expr: %empty  */
#line 2842 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9497 "bison-chpl-lib.cpp"
    break;

  case 493: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2843 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9503 "bison-chpl-lib.cpp"
    break;

  case 494: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2844 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9509 "bison-chpl-lib.cpp"
    break;

  case 495: /* formal_or_ret_type_expr: expr  */
#line 2848 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9515 "bison-chpl-lib.cpp"
    break;

  case 496: /* ret_type: formal_or_ret_type_expr  */
#line 2852 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9521 "bison-chpl-lib.cpp"
    break;

  case 497: /* ret_type: reserved_type_ident_use  */
#line 2853 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9527 "bison-chpl-lib.cpp"
    break;

  case 498: /* ret_type: error  */
#line 2854 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9533 "bison-chpl-lib.cpp"
    break;

  case 499: /* colon_ret_type: TCOLON ret_type  */
#line 2858 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9539 "bison-chpl-lib.cpp"
    break;

  case 500: /* colon_ret_type: error  */
#line 2859 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9545 "bison-chpl-lib.cpp"
    break;

  case 501: /* opt_ret_type: %empty  */
#line 2863 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9551 "bison-chpl-lib.cpp"
    break;

  case 503: /* opt_type: %empty  */
#line 2868 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9557 "bison-chpl-lib.cpp"
    break;

  case 504: /* opt_type: TCOLON expr  */
#line 2869 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9563 "bison-chpl-lib.cpp"
    break;

  case 505: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2870 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9569 "bison-chpl-lib.cpp"
    break;

  case 506: /* opt_type: error  */
#line 2871 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9575 "bison-chpl-lib.cpp"
    break;

  case 507: /* formal_type: formal_or_ret_type_expr  */
#line 2875 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9581 "bison-chpl-lib.cpp"
    break;

  case 508: /* formal_type: reserved_type_ident_use  */
#line 2876 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9587 "bison-chpl-lib.cpp"
    break;

  case 509: /* colon_formal_type: TCOLON formal_type  */
#line 2880 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9593 "bison-chpl-lib.cpp"
    break;

  case 510: /* opt_colon_formal_type: %empty  */
#line 2884 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9599 "bison-chpl-lib.cpp"
    break;

  case 511: /* opt_colon_formal_type: colon_formal_type  */
#line 2885 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9605 "bison-chpl-lib.cpp"
    break;

  case 512: /* expr_ls: expr  */
#line 2891 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9611 "bison-chpl-lib.cpp"
    break;

  case 513: /* expr_ls: expr_ls TCOMMA expr  */
#line 2892 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9617 "bison-chpl-lib.cpp"
    break;

  case 514: /* tuple_component: TUNDERSCORE  */
#line 2896 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9623 "bison-chpl-lib.cpp"
    break;

  case 515: /* tuple_component: opt_try_expr  */
#line 2897 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9629 "bison-chpl-lib.cpp"
    break;

  case 516: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2902 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9637 "bison-chpl-lib.cpp"
    break;

  case 517: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2906 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9645 "bison-chpl-lib.cpp"
    break;

  case 518: /* opt_actual_ls: %empty  */
#line 2912 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9651 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_actual_ls: actual_ls  */
#line 2913 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9657 "bison-chpl-lib.cpp"
    break;

  case 520: /* actual_ls: actual_expr  */
#line 2918 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9666 "bison-chpl-lib.cpp"
    break;

  case 521: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2923 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9676 "bison-chpl-lib.cpp"
    break;

  case 522: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2931 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9682 "bison-chpl-lib.cpp"
    break;

  case 523: /* actual_expr: opt_try_expr  */
#line 2932 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9688 "bison-chpl-lib.cpp"
    break;

  case 524: /* ident_expr: ident_use  */
#line 2936 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9694 "bison-chpl-lib.cpp"
    break;

  case 525: /* ident_expr: scalar_type  */
#line 2937 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9700 "bison-chpl-lib.cpp"
    break;

  case 531: /* sub_type_level_expr: TSINGLE expr  */
#line 2950 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9706 "bison-chpl-lib.cpp"
    break;

  case 532: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2952 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9712 "bison-chpl-lib.cpp"
    break;

  case 533: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2954 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9718 "bison-chpl-lib.cpp"
    break;

  case 534: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2956 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9724 "bison-chpl-lib.cpp"
    break;

  case 535: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2958 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9734 "bison-chpl-lib.cpp"
    break;

  case 536: /* sub_type_level_expr: TATOMIC expr  */
#line 2964 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9740 "bison-chpl-lib.cpp"
    break;

  case 537: /* sub_type_level_expr: TSYNC expr  */
#line 2966 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9746 "bison-chpl-lib.cpp"
    break;

  case 538: /* sub_type_level_expr: TOWNED  */
#line 2969 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9752 "bison-chpl-lib.cpp"
    break;

  case 539: /* sub_type_level_expr: TOWNED expr  */
#line 2971 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9758 "bison-chpl-lib.cpp"
    break;

  case 540: /* sub_type_level_expr: TUNMANAGED  */
#line 2973 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9764 "bison-chpl-lib.cpp"
    break;

  case 541: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2975 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9770 "bison-chpl-lib.cpp"
    break;

  case 542: /* sub_type_level_expr: TSHARED  */
#line 2977 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9776 "bison-chpl-lib.cpp"
    break;

  case 543: /* sub_type_level_expr: TSHARED expr  */
#line 2979 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9782 "bison-chpl-lib.cpp"
    break;

  case 544: /* sub_type_level_expr: TBORROWED  */
#line 2981 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9788 "bison-chpl-lib.cpp"
    break;

  case 545: /* sub_type_level_expr: TBORROWED expr  */
#line 2983 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9794 "bison-chpl-lib.cpp"
    break;

  case 546: /* sub_type_level_expr: TCLASS  */
#line 2985 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9800 "bison-chpl-lib.cpp"
    break;

  case 547: /* sub_type_level_expr: TRECORD  */
#line 2987 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9806 "bison-chpl-lib.cpp"
    break;

  case 548: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2992 "chpl.ypp"
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

  case 549: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 3003 "chpl.ypp"
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

  case 550: /* for_expr: TFOR expr TDO expr  */
#line 3014 "chpl.ypp"
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

  case 551: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3024 "chpl.ypp"
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

  case 552: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3040 "chpl.ypp"
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

  case 553: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 3056 "chpl.ypp"
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

  case 554: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3072 "chpl.ypp"
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

  case 555: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3081 "chpl.ypp"
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

  case 556: /* for_expr: TFORALL expr TDO expr  */
#line 3090 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 9948 "bison-chpl-lib.cpp"
    break;

  case 557: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3098 "chpl.ypp"
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

  case 558: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3112 "chpl.ypp"
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

  case 559: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3126 "chpl.ypp"
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

  case 560: /* bracket_loop_expr: TLSBR TRSBR  */
#line 3143 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 10010 "bison-chpl-lib.cpp"
    break;

  case 561: /* bracket_loop_expr: TLSBR TRSBR expr  */
#line 3147 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 10018 "bison-chpl-lib.cpp"
    break;

  case 562: /* bracket_loop_expr: TLSBR expr_ls TRSBR expr  */
#line 3151 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10026 "bison-chpl-lib.cpp"
    break;

  case 563: /* bracket_loop_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3155 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10034 "bison-chpl-lib.cpp"
    break;

  case 564: /* bracket_loop_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3159 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10042 "bison-chpl-lib.cpp"
    break;

  case 565: /* bracket_loop_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3163 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10050 "bison-chpl-lib.cpp"
    break;

  case 566: /* bracket_loop_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3167 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10058 "bison-chpl-lib.cpp"
    break;

  case 567: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3174 "chpl.ypp"
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

  case 568: /* nil_expr: TNIL  */
#line 3191 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10078 "bison-chpl-lib.cpp"
    break;

  case 576: /* opt_task_intent_ls: %empty  */
#line 3209 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10084 "bison-chpl-lib.cpp"
    break;

  case 577: /* opt_task_intent_ls: task_intent_clause  */
#line 3210 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 578: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3215 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10100 "bison-chpl-lib.cpp"
    break;

  case 579: /* task_intent_ls: intent_expr  */
#line 3223 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10106 "bison-chpl-lib.cpp"
    break;

  case 580: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3224 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10112 "bison-chpl-lib.cpp"
    break;

  case 581: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3229 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10122 "bison-chpl-lib.cpp"
    break;

  case 582: /* forall_intent_ls: intent_expr  */
#line 3237 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10128 "bison-chpl-lib.cpp"
    break;

  case 583: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3238 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10134 "bison-chpl-lib.cpp"
    break;

  case 584: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3243 "chpl.ypp"
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

  case 585: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3257 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10160 "bison-chpl-lib.cpp"
    break;

  case 586: /* intent_expr: expr TREDUCE ident_expr  */
#line 3261 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10168 "bison-chpl-lib.cpp"
    break;

  case 587: /* task_var_prefix: TCONST  */
#line 3267 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10174 "bison-chpl-lib.cpp"
    break;

  case 588: /* task_var_prefix: TIN  */
#line 3268 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10180 "bison-chpl-lib.cpp"
    break;

  case 589: /* task_var_prefix: TCONST TIN  */
#line 3269 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10186 "bison-chpl-lib.cpp"
    break;

  case 590: /* task_var_prefix: TREF  */
#line 3270 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10192 "bison-chpl-lib.cpp"
    break;

  case 591: /* task_var_prefix: TCONST TREF  */
#line 3271 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10198 "bison-chpl-lib.cpp"
    break;

  case 592: /* task_var_prefix: TVAR  */
#line 3272 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10204 "bison-chpl-lib.cpp"
    break;

  case 593: /* new_maybe_decorated: TNEW  */
#line 3277 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10210 "bison-chpl-lib.cpp"
    break;

  case 594: /* new_maybe_decorated: TNEW TOWNED  */
#line 3279 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10216 "bison-chpl-lib.cpp"
    break;

  case 595: /* new_maybe_decorated: TNEW TSHARED  */
#line 3281 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10222 "bison-chpl-lib.cpp"
    break;

  case 596: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3283 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10228 "bison-chpl-lib.cpp"
    break;

  case 597: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3285 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10234 "bison-chpl-lib.cpp"
    break;

  case 598: /* new_expr: new_maybe_decorated expr  */
#line 3291 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10242 "bison-chpl-lib.cpp"
    break;

  case 599: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3298 "chpl.ypp"
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

  case 600: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3310 "chpl.ypp"
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

  case 601: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3322 "chpl.ypp"
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

  case 602: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
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
    (yyval.expr) = context->buildNewExpr((yyloc), New::SHARED, call.release());

  }
#line 10309 "bison-chpl-lib.cpp"
    break;

  case 603: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3352 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10317 "bison-chpl-lib.cpp"
    break;

  case 604: /* range_literal_expr: expr TDOTDOT expr  */
#line 3359 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10326 "bison-chpl-lib.cpp"
    break;

  case 605: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3364 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10335 "bison-chpl-lib.cpp"
    break;

  case 606: /* range_literal_expr: expr TDOTDOT  */
#line 3369 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10344 "bison-chpl-lib.cpp"
    break;

  case 607: /* range_literal_expr: TDOTDOT expr  */
#line 3374 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10354 "bison-chpl-lib.cpp"
    break;

  case 608: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3380 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10364 "bison-chpl-lib.cpp"
    break;

  case 609: /* range_literal_expr: TDOTDOT  */
#line 3386 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10374 "bison-chpl-lib.cpp"
    break;

  case 610: /* cast_expr: expr TCOLON expr  */
#line 3416 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10382 "bison-chpl-lib.cpp"
    break;

  case 611: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3423 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10390 "bison-chpl-lib.cpp"
    break;

  case 612: /* super_expr: fn_expr  */
#line 3429 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10396 "bison-chpl-lib.cpp"
    break;

  case 616: /* expr: sub_type_level_expr TQUESTION  */
#line 3438 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10402 "bison-chpl-lib.cpp"
    break;

  case 617: /* expr: TQUESTION  */
#line 3440 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10408 "bison-chpl-lib.cpp"
    break;

  case 621: /* expr: fn_type  */
#line 3445 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10414 "bison-chpl-lib.cpp"
    break;

  case 631: /* opt_expr: %empty  */
#line 3459 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10420 "bison-chpl-lib.cpp"
    break;

  case 632: /* opt_expr: expr  */
#line 3460 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10426 "bison-chpl-lib.cpp"
    break;

  case 633: /* opt_try_expr: TTRY expr  */
#line 3464 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10432 "bison-chpl-lib.cpp"
    break;

  case 634: /* opt_try_expr: TTRYBANG expr  */
#line 3465 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10438 "bison-chpl-lib.cpp"
    break;

  case 635: /* opt_try_expr: super_expr  */
#line 3466 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10444 "bison-chpl-lib.cpp"
    break;

  case 641: /* call_base_expr: expr TBANG  */
#line 3483 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10452 "bison-chpl-lib.cpp"
    break;

  case 642: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3486 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10458 "bison-chpl-lib.cpp"
    break;

  case 645: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3493 "chpl.ypp"
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

  case 646: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3505 "chpl.ypp"
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

  case 647: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3517 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10498 "bison-chpl-lib.cpp"
    break;

  case 648: /* dot_expr: expr TDOT ident_use  */
#line 3524 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10504 "bison-chpl-lib.cpp"
    break;

  case 649: /* dot_expr: expr TDOT TTYPE  */
#line 3526 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10510 "bison-chpl-lib.cpp"
    break;

  case 650: /* dot_expr: expr TDOT TDOMAIN  */
#line 3528 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10516 "bison-chpl-lib.cpp"
    break;

  case 651: /* dot_expr: expr TDOT TLOCALE  */
#line 3530 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10522 "bison-chpl-lib.cpp"
    break;

  case 652: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3532 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10532 "bison-chpl-lib.cpp"
    break;

  case 653: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3538 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10542 "bison-chpl-lib.cpp"
    break;

  case 654: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3550 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10548 "bison-chpl-lib.cpp"
    break;

  case 655: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3552 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10556 "bison-chpl-lib.cpp"
    break;

  case 656: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3556 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10564 "bison-chpl-lib.cpp"
    break;

  case 657: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3560 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10572 "bison-chpl-lib.cpp"
    break;

  case 658: /* bool_literal: TFALSE  */
#line 3566 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10578 "bison-chpl-lib.cpp"
    break;

  case 659: /* bool_literal: TTRUE  */
#line 3567 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10584 "bison-chpl-lib.cpp"
    break;

  case 660: /* str_bytes_literal: STRINGLITERAL  */
#line 3571 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10590 "bison-chpl-lib.cpp"
    break;

  case 661: /* str_bytes_literal: BYTESLITERAL  */
#line 3572 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10596 "bison-chpl-lib.cpp"
    break;

  case 664: /* literal_expr: INTLITERAL  */
#line 3578 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10602 "bison-chpl-lib.cpp"
    break;

  case 665: /* literal_expr: REALLITERAL  */
#line 3579 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10608 "bison-chpl-lib.cpp"
    break;

  case 666: /* literal_expr: IMAGLITERAL  */
#line 3580 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10614 "bison-chpl-lib.cpp"
    break;

  case 667: /* literal_expr: CSTRINGLITERAL  */
#line 3581 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10620 "bison-chpl-lib.cpp"
    break;

  case 668: /* literal_expr: TNONE  */
#line 3582 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10626 "bison-chpl-lib.cpp"
    break;

  case 669: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3584 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10635 "bison-chpl-lib.cpp"
    break;

  case 670: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3589 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10644 "bison-chpl-lib.cpp"
    break;

  case 671: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3594 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10652 "bison-chpl-lib.cpp"
    break;

  case 672: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3598 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
  }
#line 10660 "bison-chpl-lib.cpp"
    break;

  case 673: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3602 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
  }
#line 10668 "bison-chpl-lib.cpp"
    break;

  case 674: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3606 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
  }
#line 10676 "bison-chpl-lib.cpp"
    break;

  case 675: /* assoc_expr_ls: expr TALIAS expr  */
#line 3614 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10685 "bison-chpl-lib.cpp"
    break;

  case 676: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3619 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10695 "bison-chpl-lib.cpp"
    break;

  case 677: /* binary_op_expr: expr TPLUS expr  */
#line 3627 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10701 "bison-chpl-lib.cpp"
    break;

  case 678: /* binary_op_expr: expr TMINUS expr  */
#line 3628 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10707 "bison-chpl-lib.cpp"
    break;

  case 679: /* binary_op_expr: expr TSTAR expr  */
#line 3629 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10713 "bison-chpl-lib.cpp"
    break;

  case 680: /* binary_op_expr: expr TDIVIDE expr  */
#line 3630 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10719 "bison-chpl-lib.cpp"
    break;

  case 681: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3631 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10725 "bison-chpl-lib.cpp"
    break;

  case 682: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3632 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10731 "bison-chpl-lib.cpp"
    break;

  case 683: /* binary_op_expr: expr TMOD expr  */
#line 3633 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10737 "bison-chpl-lib.cpp"
    break;

  case 684: /* binary_op_expr: expr TEQUAL expr  */
#line 3634 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10743 "bison-chpl-lib.cpp"
    break;

  case 685: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3635 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10749 "bison-chpl-lib.cpp"
    break;

  case 686: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3636 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10755 "bison-chpl-lib.cpp"
    break;

  case 687: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3637 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10761 "bison-chpl-lib.cpp"
    break;

  case 688: /* binary_op_expr: expr TLESS expr  */
#line 3638 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10767 "bison-chpl-lib.cpp"
    break;

  case 689: /* binary_op_expr: expr TGREATER expr  */
#line 3639 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10773 "bison-chpl-lib.cpp"
    break;

  case 690: /* binary_op_expr: expr TBAND expr  */
#line 3640 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10779 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TBOR expr  */
#line 3641 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10785 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TBXOR expr  */
#line 3642 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10791 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TAND expr  */
#line 3643 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10797 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TOR expr  */
#line 3644 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10803 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TEXP expr  */
#line 3645 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10809 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TBY expr  */
#line 3646 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10815 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TALIGN expr  */
#line 3647 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10821 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr THASH expr  */
#line 3648 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10827 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TDMAPPED expr  */
#line 3649 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10833 "bison-chpl-lib.cpp"
    break;

  case 700: /* unary_op_expr: TPLUS expr  */
#line 3653 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10839 "bison-chpl-lib.cpp"
    break;

  case 701: /* unary_op_expr: TMINUS expr  */
#line 3654 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10845 "bison-chpl-lib.cpp"
    break;

  case 702: /* unary_op_expr: TMINUSMINUS expr  */
#line 3655 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10851 "bison-chpl-lib.cpp"
    break;

  case 703: /* unary_op_expr: TPLUSPLUS expr  */
#line 3656 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10857 "bison-chpl-lib.cpp"
    break;

  case 704: /* unary_op_expr: TBANG expr  */
#line 3657 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10863 "bison-chpl-lib.cpp"
    break;

  case 705: /* unary_op_expr: expr TBANG  */
#line 3658 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10871 "bison-chpl-lib.cpp"
    break;

  case 706: /* unary_op_expr: TBNOT expr  */
#line 3661 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10877 "bison-chpl-lib.cpp"
    break;

  case 707: /* reduce_expr: expr TREDUCE expr  */
#line 3666 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10885 "bison-chpl-lib.cpp"
    break;

  case 708: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3670 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10893 "bison-chpl-lib.cpp"
    break;

  case 709: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3674 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10901 "bison-chpl-lib.cpp"
    break;

  case 710: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3678 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10909 "bison-chpl-lib.cpp"
    break;

  case 711: /* scan_expr: expr TSCAN expr  */
#line 3685 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10917 "bison-chpl-lib.cpp"
    break;

  case 712: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3689 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10925 "bison-chpl-lib.cpp"
    break;

  case 713: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3693 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10933 "bison-chpl-lib.cpp"
    break;

  case 714: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3697 "chpl.ypp"
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
