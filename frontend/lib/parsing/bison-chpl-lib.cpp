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
  YYSYMBOL_TSINGLE = 99,                   /* TSINGLE  */
  YYSYMBOL_TSPARSE = 100,                  /* TSPARSE  */
  YYSYMBOL_TSTRING = 101,                  /* TSTRING  */
  YYSYMBOL_TSUBDOMAIN = 102,               /* TSUBDOMAIN  */
  YYSYMBOL_TSYNC = 103,                    /* TSYNC  */
  YYSYMBOL_TTHEN = 104,                    /* TTHEN  */
  YYSYMBOL_TTHIS = 105,                    /* TTHIS  */
  YYSYMBOL_TTHROW = 106,                   /* TTHROW  */
  YYSYMBOL_TTHROWS = 107,                  /* TTHROWS  */
  YYSYMBOL_TTRUE = 108,                    /* TTRUE  */
  YYSYMBOL_TTRY = 109,                     /* TTRY  */
  YYSYMBOL_TTRYBANG = 110,                 /* TTRYBANG  */
  YYSYMBOL_TTYPE = 111,                    /* TTYPE  */
  YYSYMBOL_TUINT = 112,                    /* TUINT  */
  YYSYMBOL_TUNDERSCORE = 113,              /* TUNDERSCORE  */
  YYSYMBOL_TUNION = 114,                   /* TUNION  */
  YYSYMBOL_TUNMANAGED = 115,               /* TUNMANAGED  */
  YYSYMBOL_TUSE = 116,                     /* TUSE  */
  YYSYMBOL_TVAR = 117,                     /* TVAR  */
  YYSYMBOL_TVOID = 118,                    /* TVOID  */
  YYSYMBOL_TWHEN = 119,                    /* TWHEN  */
  YYSYMBOL_TWHERE = 120,                   /* TWHERE  */
  YYSYMBOL_TWHILE = 121,                   /* TWHILE  */
  YYSYMBOL_TWITH = 122,                    /* TWITH  */
  YYSYMBOL_TYIELD = 123,                   /* TYIELD  */
  YYSYMBOL_TZIP = 124,                     /* TZIP  */
  YYSYMBOL_TALIAS = 125,                   /* TALIAS  */
  YYSYMBOL_TAND = 126,                     /* TAND  */
  YYSYMBOL_TASSIGN = 127,                  /* TASSIGN  */
  YYSYMBOL_TASSIGNBAND = 128,              /* TASSIGNBAND  */
  YYSYMBOL_TASSIGNBOR = 129,               /* TASSIGNBOR  */
  YYSYMBOL_TASSIGNBXOR = 130,              /* TASSIGNBXOR  */
  YYSYMBOL_TASSIGNDIVIDE = 131,            /* TASSIGNDIVIDE  */
  YYSYMBOL_TASSIGNEXP = 132,               /* TASSIGNEXP  */
  YYSYMBOL_TASSIGNLAND = 133,              /* TASSIGNLAND  */
  YYSYMBOL_TASSIGNLOR = 134,               /* TASSIGNLOR  */
  YYSYMBOL_TASSIGNMINUS = 135,             /* TASSIGNMINUS  */
  YYSYMBOL_TASSIGNMOD = 136,               /* TASSIGNMOD  */
  YYSYMBOL_TASSIGNMULTIPLY = 137,          /* TASSIGNMULTIPLY  */
  YYSYMBOL_TASSIGNPLUS = 138,              /* TASSIGNPLUS  */
  YYSYMBOL_TASSIGNREDUCE = 139,            /* TASSIGNREDUCE  */
  YYSYMBOL_TASSIGNSL = 140,                /* TASSIGNSL  */
  YYSYMBOL_TASSIGNSR = 141,                /* TASSIGNSR  */
  YYSYMBOL_TATMARK = 142,                  /* TATMARK  */
  YYSYMBOL_TBANG = 143,                    /* TBANG  */
  YYSYMBOL_TBAND = 144,                    /* TBAND  */
  YYSYMBOL_TBNOT = 145,                    /* TBNOT  */
  YYSYMBOL_TBOR = 146,                     /* TBOR  */
  YYSYMBOL_TBXOR = 147,                    /* TBXOR  */
  YYSYMBOL_TCOLON = 148,                   /* TCOLON  */
  YYSYMBOL_TCOMMA = 149,                   /* TCOMMA  */
  YYSYMBOL_TDIVIDE = 150,                  /* TDIVIDE  */
  YYSYMBOL_TDOT = 151,                     /* TDOT  */
  YYSYMBOL_TDOTDOT = 152,                  /* TDOTDOT  */
  YYSYMBOL_TDOTDOTDOT = 153,               /* TDOTDOTDOT  */
  YYSYMBOL_TEQUAL = 154,                   /* TEQUAL  */
  YYSYMBOL_TEXP = 155,                     /* TEXP  */
  YYSYMBOL_TGREATER = 156,                 /* TGREATER  */
  YYSYMBOL_TGREATEREQUAL = 157,            /* TGREATEREQUAL  */
  YYSYMBOL_THASH = 158,                    /* THASH  */
  YYSYMBOL_TLESS = 159,                    /* TLESS  */
  YYSYMBOL_TLESSEQUAL = 160,               /* TLESSEQUAL  */
  YYSYMBOL_TMINUS = 161,                   /* TMINUS  */
  YYSYMBOL_TMOD = 162,                     /* TMOD  */
  YYSYMBOL_TNOTEQUAL = 163,                /* TNOTEQUAL  */
  YYSYMBOL_TOR = 164,                      /* TOR  */
  YYSYMBOL_TPLUS = 165,                    /* TPLUS  */
  YYSYMBOL_TQUESTION = 166,                /* TQUESTION  */
  YYSYMBOL_TSEMI = 167,                    /* TSEMI  */
  YYSYMBOL_TSHIFTLEFT = 168,               /* TSHIFTLEFT  */
  YYSYMBOL_TSHIFTRIGHT = 169,              /* TSHIFTRIGHT  */
  YYSYMBOL_TSTAR = 170,                    /* TSTAR  */
  YYSYMBOL_TSWAP = 171,                    /* TSWAP  */
  YYSYMBOL_TLCBR = 172,                    /* TLCBR  */
  YYSYMBOL_TRCBR = 173,                    /* TRCBR  */
  YYSYMBOL_TLP = 174,                      /* TLP  */
  YYSYMBOL_TRP = 175,                      /* TRP  */
  YYSYMBOL_TLSBR = 176,                    /* TLSBR  */
  YYSYMBOL_TRSBR = 177,                    /* TRSBR  */
  YYSYMBOL_TNOELSE = 178,                  /* TNOELSE  */
  YYSYMBOL_TDOTDOTOPENHIGH = 179,          /* TDOTDOTOPENHIGH  */
  YYSYMBOL_TUPLUS = 180,                   /* TUPLUS  */
  YYSYMBOL_TUMINUS = 181,                  /* TUMINUS  */
  YYSYMBOL_TLNOT = 182,                    /* TLNOT  */
  YYSYMBOL_YYACCEPT = 183,                 /* $accept  */
  YYSYMBOL_program = 184,                  /* program  */
  YYSYMBOL_toplevel_stmt_ls = 185,         /* toplevel_stmt_ls  */
  YYSYMBOL_toplevel_stmt = 186,            /* toplevel_stmt  */
  YYSYMBOL_pragma_ls = 187,                /* pragma_ls  */
  YYSYMBOL_stmt = 188,                     /* stmt  */
  YYSYMBOL_stmt_base = 189,                /* stmt_base  */
  YYSYMBOL_tryable_stmt = 190,             /* tryable_stmt  */
  YYSYMBOL_decl_base = 191,                /* decl_base  */
  YYSYMBOL_collect_attributes = 192,       /* collect_attributes  */
  YYSYMBOL_attribute_receiver = 193,       /* attribute_receiver  */
  YYSYMBOL_attribute_group = 194,          /* attribute_group  */
  YYSYMBOL_attribute_decl_stmt_ls = 195,   /* attribute_decl_stmt_ls  */
  YYSYMBOL_attribute_decl_stmt = 196,      /* attribute_decl_stmt  */
  YYSYMBOL_attribute_decl_begin = 197,     /* attribute_decl_begin  */
  YYSYMBOL_opt_attribute_decl_toolspace = 198, /* opt_attribute_decl_toolspace  */
  YYSYMBOL_opt_attribute_actuals = 199,    /* opt_attribute_actuals  */
  YYSYMBOL_attribute_actuals_ls = 200,     /* attribute_actuals_ls  */
  YYSYMBOL_attribute_actual = 201,         /* attribute_actual  */
  YYSYMBOL_module_decl_start = 202,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 203,         /* module_decl_stmt  */
  YYSYMBOL_opt_access_control = 204,       /* opt_access_control  */
  YYSYMBOL_opt_prototype = 205,            /* opt_prototype  */
  YYSYMBOL_include_module_stmt = 206,      /* include_module_stmt  */
  YYSYMBOL_block_stmt_body = 207,          /* block_stmt_body  */
  YYSYMBOL_block_stmt = 208,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 209,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 210,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 211,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 212,              /* opt_only_ls  */
  YYSYMBOL_use_stmt = 213,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 214,              /* import_stmt  */
  YYSYMBOL_import_internal_type_ident = 215, /* import_internal_type_ident  */
  YYSYMBOL_import_expr = 216,              /* import_expr  */
  YYSYMBOL_import_ls = 217,                /* import_ls  */
  YYSYMBOL_require_stmt = 218,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 219,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 220,          /* opt_label_ident  */
  YYSYMBOL_reserved_word_ident = 221,      /* reserved_word_ident  */
  YYSYMBOL_ident_def = 222,                /* ident_def  */
  YYSYMBOL_ident_use = 223,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 224,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 225,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 226,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 227,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 228,              /* return_stmt  */
  YYSYMBOL_class_level_stmt = 229,         /* class_level_stmt  */
  YYSYMBOL_230_1 = 230,                    /* $@1  */
  YYSYMBOL_231_2 = 231,                    /* $@2  */
  YYSYMBOL_inner_class_level_stmt = 232,   /* inner_class_level_stmt  */
  YYSYMBOL_forwarding_decl_stmt = 233,     /* forwarding_decl_stmt  */
  YYSYMBOL_forwarding_decl_start = 234,    /* forwarding_decl_start  */
  YYSYMBOL_extern_or_export = 235,         /* extern_or_export  */
  YYSYMBOL_extern_export_decl_stmt_start = 236, /* extern_export_decl_stmt_start  */
  YYSYMBOL_extern_export_decl_stmt = 237,  /* extern_export_decl_stmt  */
  YYSYMBOL_238_3 = 238,                    /* $@3  */
  YYSYMBOL_239_4 = 239,                    /* $@4  */
  YYSYMBOL_extern_block_stmt = 240,        /* extern_block_stmt  */
  YYSYMBOL_no_loop_attributes = 241,       /* no_loop_attributes  */
  YYSYMBOL_loop_stmt = 242,                /* loop_stmt  */
  YYSYMBOL_for_loop_kw = 243,              /* for_loop_kw  */
  YYSYMBOL_loop_stmt_base = 244,           /* loop_stmt_base  */
  YYSYMBOL_zippered_iterator = 245,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 246,                  /* if_stmt  */
  YYSYMBOL_ifvar = 247,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 248,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 249,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 250,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 251,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 252, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 253,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 254,           /* ifc_constraint  */
  YYSYMBOL_try_stmt = 255,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 256,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 257,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 258,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 259,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 260,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 261,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 262,                /* when_stmt  */
  YYSYMBOL_manager_expr = 263,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 264,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 265,              /* manage_stmt  */
  YYSYMBOL_class_decl_stmt = 266,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 267,              /* class_start  */
  YYSYMBOL_class_tag = 268,                /* class_tag  */
  YYSYMBOL_opt_inherit = 269,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 270,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 271,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header = 272,              /* enum_header  */
  YYSYMBOL_enum_ls = 273,                  /* enum_ls  */
  YYSYMBOL_274_5 = 274,                    /* $@5  */
  YYSYMBOL_275_6 = 275,                    /* $@6  */
  YYSYMBOL_enum_item = 276,                /* enum_item  */
  YYSYMBOL_lambda_decl_start = 277,        /* lambda_decl_start  */
  YYSYMBOL_lambda_decl_expr = 278,         /* lambda_decl_expr  */
  YYSYMBOL_279_7 = 279,                    /* $@7  */
  YYSYMBOL_linkage_spec_empty = 280,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 281,             /* linkage_spec  */
  YYSYMBOL_opt_fn_type_formal_ls = 282,    /* opt_fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal_ls = 283,        /* fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal = 284,           /* fn_type_formal  */
  YYSYMBOL_opt_fn_type_ret_type = 285,     /* opt_fn_type_ret_type  */
  YYSYMBOL_fn_type = 286,                  /* fn_type  */
  YYSYMBOL_fn_expr = 287,                  /* fn_expr  */
  YYSYMBOL_288_8 = 288,                    /* $@8  */
  YYSYMBOL_289_9 = 289,                    /* $@9  */
  YYSYMBOL_fn_decl_stmt_complete = 290,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 291,             /* fn_decl_stmt  */
  YYSYMBOL_292_10 = 292,                   /* $@10  */
  YYSYMBOL_fn_decl_stmt_inner = 293,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 294,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 295,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 296,                 /* fn_ident  */
  YYSYMBOL_op_ident = 297,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 298,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 299,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 300,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 301,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 302,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 303,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 304,                /* formal_ls  */
  YYSYMBOL_formal = 305,                   /* formal  */
  YYSYMBOL_formal_ident_def = 306,         /* formal_ident_def  */
  YYSYMBOL_named_formal = 307,             /* named_formal  */
  YYSYMBOL_opt_formal_intent_tag = 308,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 309,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 310,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 311,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 312,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 313,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 314,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 315,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 316,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 317,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 318, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 319,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 320,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 321,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 322, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 323, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 324, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 325,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 326,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 327,            /* var_decl_stmt  */
  YYSYMBOL_328_11 = 328,                   /* $@11  */
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
  YYSYMBOL_forall_intent_clause = 360,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 361,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 362,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 363,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 364,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 365,                 /* new_expr  */
  YYSYMBOL_let_expr = 366,                 /* let_expr  */
  YYSYMBOL_range_literal_expr = 367,       /* range_literal_expr  */
  YYSYMBOL_cast_expr = 368,                /* cast_expr  */
  YYSYMBOL_tuple_expand_expr = 369,        /* tuple_expand_expr  */
  YYSYMBOL_super_expr = 370,               /* super_expr  */
  YYSYMBOL_expr = 371,                     /* expr  */
  YYSYMBOL_opt_expr = 372,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 373,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 374,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 375,           /* call_base_expr  */
  YYSYMBOL_call_expr = 376,                /* call_expr  */
  YYSYMBOL_dot_expr = 377,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 378,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 379,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 380,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 381,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 382,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 383,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 384,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 385,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 386,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 387       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 326 "chpl.ypp"

  // include the definition of methods for ParserContext
  #include "ParserContextImpl.h"
  // include helper macros for use in writing productions
  #include "parser-help.h"
  // include implementation of error logging hook
  #include "parser-yyerror.h"

#line 511 "bison-chpl-lib.cpp"

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
#define YYLAST   18089

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  183
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  205
/* YYNRULES -- Number of rules.  */
#define YYNRULES  735
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1264

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   437


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
     175,   176,   177,   178,   179,   180,   181,   182
};

#if YYCHPL_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   636,   636,   640,   641,   646,   647,   655,   659,   669,
     674,   681,   682,   683,   684,   685,   686,   687,   688,   689,
     690,   691,   709,   710,   711,   719,   728,   737,   741,   748,
     756,   757,   758,   759,   760,   761,   762,   763,   765,   785,
     793,   810,   826,   843,   860,   876,   897,   898,   902,   912,
     913,   917,   921,   922,   926,   933,   941,   944,   954,   956,
     961,   967,   972,   981,   985,  1006,  1023,  1040,  1058,  1059,
    1061,  1066,  1067,  1072,  1090,  1095,  1100,  1112,  1131,  1132,
    1136,  1140,  1144,  1149,  1153,  1157,  1167,  1172,  1178,  1185,
    1190,  1197,  1208,  1209,  1213,  1218,  1226,  1235,  1245,  1253,
    1262,  1275,  1283,  1284,  1285,  1286,  1287,  1288,  1289,  1290,
    1291,  1292,  1293,  1294,  1295,  1296,  1297,  1298,  1299,  1311,
    1315,  1320,  1325,  1331,  1340,  1341,  1345,  1354,  1358,  1362,
    1366,  1370,  1374,  1383,  1384,  1385,  1389,  1390,  1391,  1392,
    1393,  1397,  1398,  1399,  1411,  1412,  1413,  1418,  1419,  1420,
    1421,  1422,  1423,  1424,  1425,  1426,  1427,  1428,  1429,  1430,
    1431,  1432,  1433,  1434,  1435,  1436,  1440,  1441,  1442,  1443,
    1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,  1458,  1459,
    1460,  1461,  1465,  1466,  1470,  1477,  1487,  1491,  1497,  1497,
    1503,  1503,  1512,  1513,  1514,  1515,  1516,  1517,  1518,  1522,
    1527,  1532,  1537,  1544,  1552,  1553,  1557,  1566,  1573,  1582,
    1601,  1600,  1610,  1609,  1622,  1628,  1631,  1632,  1641,  1642,
    1643,  1647,  1670,  1689,  1708,  1713,  1718,  1723,  1748,  1753,
    1758,  1763,  1768,  1773,  1778,  1783,  1788,  1793,  1798,  1803,
    1808,  1813,  1818,  1824,  1833,  1842,  1846,  1850,  1854,  1858,
    1862,  1866,  1870,  1874,  1879,  1884,  1889,  1897,  1912,  1930,
    1934,  1941,  1942,  1947,  1952,  1953,  1954,  1955,  1956,  1957,
    1958,  1959,  1960,  1961,  1962,  1963,  1968,  1973,  1974,  1975,
    1976,  1985,  1986,  1990,  1994,  1998,  2005,  2009,  2013,  2020,
    2024,  2028,  2032,  2039,  2040,  2044,  2048,  2052,  2059,  2072,
    2088,  2096,  2101,  2111,  2112,  2116,  2120,  2125,  2134,  2139,
    2143,  2150,  2151,  2155,  2165,  2171,  2183,  2190,  2191,  2192,
    2196,  2197,  2201,  2205,  2209,  2213,  2217,  2225,  2244,  2257,
    2264,  2269,  2276,  2275,  2285,  2291,  2290,  2305,  2312,  2324,
    2342,  2339,  2367,  2371,  2372,  2374,  2379,  2380,  2384,  2385,
    2389,  2392,  2394,  2399,  2400,  2411,  2431,  2430,  2446,  2445,
    2463,  2473,  2470,  2497,  2505,  2513,  2524,  2535,  2544,  2559,
    2560,  2564,  2565,  2566,  2575,  2576,  2577,  2578,  2579,  2580,
    2581,  2582,  2583,  2584,  2585,  2586,  2587,  2588,  2589,  2590,
    2591,  2592,  2593,  2594,  2595,  2596,  2597,  2598,  2599,  2603,
    2604,  2605,  2606,  2607,  2608,  2609,  2610,  2611,  2612,  2613,
    2614,  2619,  2620,  2624,  2625,  2629,  2630,  2634,  2635,  2639,
    2640,  2644,  2645,  2649,  2653,  2654,  2658,  2662,  2667,  2672,
    2677,  2682,  2690,  2694,  2702,  2703,  2704,  2705,  2706,  2707,
    2708,  2709,  2710,  2714,  2715,  2716,  2717,  2718,  2719,  2723,
    2724,  2725,  2729,  2730,  2731,  2732,  2733,  2734,  2735,  2739,
    2740,  2743,  2744,  2748,  2749,  2753,  2758,  2759,  2761,  2763,
    2765,  2770,  2772,  2777,  2779,  2781,  2783,  2785,  2787,  2789,
    2794,  2795,  2796,  2800,  2809,  2813,  2821,  2825,  2832,  2854,
    2855,  2860,  2861,  2862,  2863,  2864,  2869,  2868,  2877,  2882,
    2890,  2894,  2901,  2920,  2939,  2943,  2947,  2954,  2956,  2958,
    2965,  2966,  2967,  2971,  2975,  2976,  2977,  2981,  2982,  2986,
    2987,  2991,  2992,  2993,  2994,  2998,  2999,  3003,  3007,  3008,
    3014,  3015,  3019,  3020,  3024,  3028,  3035,  3036,  3040,  3045,
    3054,  3055,  3059,  3060,  3067,  3068,  3069,  3070,  3071,  3072,
    3074,  3076,  3078,  3080,  3086,  3088,  3091,  3093,  3095,  3097,
    3099,  3101,  3103,  3105,  3107,  3109,  3114,  3118,  3122,  3126,
    3130,  3139,  3148,  3160,  3164,  3168,  3172,  3176,  3180,  3184,
    3188,  3195,  3213,  3221,  3222,  3223,  3224,  3225,  3226,  3227,
    3231,  3232,  3236,  3243,  3244,  3248,  3257,  3258,  3262,  3277,
    3281,  3288,  3289,  3290,  3291,  3292,  3293,  3297,  3299,  3301,
    3303,  3305,  3311,  3318,  3330,  3342,  3355,  3372,  3379,  3384,
    3389,  3394,  3400,  3406,  3436,  3443,  3450,  3451,  3455,  3457,
    3458,  3460,  3462,  3463,  3464,  3465,  3468,  3469,  3470,  3471,
    3472,  3473,  3474,  3475,  3476,  3480,  3481,  3485,  3486,  3487,
    3491,  3492,  3493,  3494,  3503,  3504,  3507,  3508,  3509,  3513,
    3525,  3537,  3544,  3546,  3548,  3550,  3552,  3554,  3563,  3569,
    3573,  3577,  3584,  3585,  3589,  3590,  3594,  3595,  3596,  3597,
    3598,  3599,  3600,  3601,  3606,  3611,  3616,  3621,  3626,  3635,
    3640,  3649,  3650,  3651,  3652,  3653,  3654,  3655,  3656,  3657,
    3658,  3659,  3660,  3661,  3662,  3663,  3664,  3665,  3666,  3667,
    3668,  3669,  3670,  3671,  3675,  3676,  3677,  3678,  3679,  3680,
    3683,  3687,  3691,  3695,  3699,  3706,  3710,  3714,  3718,  3726,
    3727,  3728,  3729,  3730,  3731,  3732
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
  "TSCAN", "TSELECT", "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE",
  "TSTRING", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS", "TTHROW", "TTHROWS",
  "TTRUE", "TTRY", "TTRYBANG", "TTYPE", "TUINT", "TUNDERSCORE", "TUNION",
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
  "formal_ident_def", "named_formal", "opt_formal_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_iter_or_op",
  "opt_ret_tag", "opt_throws_error", "opt_function_body_stmt",
  "function_body_stmt", "query_expr", "opt_lifetime_where",
  "lifetime_components_expr", "lifetime_expr", "lifetime_ident",
  "type_alias_decl_stmt", "type_alias_decl_stmt_start",
  "type_alias_decl_stmt_inner_ls", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_type", "var_decl_stmt", "$@11",
  "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
  "tuple_var_decl_component", "tuple_var_decl_stmt_inner_ls",
  "opt_init_expr", "formal_or_ret_type_expr", "ret_type", "colon_ret_type",
  "opt_ret_type", "opt_type", "formal_type", "colon_formal_type",
  "opt_colon_formal_type", "expr_ls", "tuple_component", "tuple_expr_ls",
  "opt_actual_ls", "actual_ls", "actual_expr", "ident_expr",
  "sub_type_level_expr", "for_expr", "for_expr_base", "bracket_loop_expr",
  "bracket_loop_expr_base", "cond_expr", "nil_expr", "stmt_level_expr",
  "opt_task_intent_ls", "task_intent_clause", "task_intent_ls",
  "forall_intent_clause", "forall_intent_ls", "intent_expr",
  "task_var_prefix", "new_maybe_decorated", "new_expr", "let_expr",
  "range_literal_expr", "cast_expr", "tuple_expand_expr", "super_expr",
  "expr", "opt_expr", "opt_try_expr", "lhs_expr", "call_base_expr",
  "call_expr", "dot_expr", "parenthesized_expr", "bool_literal",
  "str_bytes_literal", "literal_expr", "assoc_expr_ls", "binary_op_expr",
  "unary_op_expr", "reduce_expr", "scan_expr", "reduce_scan_op_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-878)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-736)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -878,   123,  3714,  -878,     9,    80,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878, 11949,    76,   105,   224, 13821,   315,
   17841,    76,   340,   209,   269,   105,  5130, 11949,   267, 17883,
    -878,   393,   400,  -878,  9675,   403,   304,   292,  -878,   414,
     404, 17883, 17883, 17883,  -878,  3375, 10548,   462, 11949, 11949,
     231,  -878,   468,   475, 11949,  -878, 13821,  -878, 11949,   528,
     388,   217,   220, 13167,   488, 17971,  -878, 11949,  8280, 11949,
   10548, 13821, 11949,   483,   500,   416,  5130,   513, 11949,   539,
    6714,  6714,  -878,   552,  -878, 13821,  -878,   554,  9849,  -878,
   11949,  -878, 11949,  -878,  -878, 13647, 11949,  -878, 10023,  -878,
    -878,  -878,  4068,  7410, 11949,  -878,  4776,  -878,  -878,  -878,
    -878, 17420,   608,  -878,   466,   443,  -878,   235,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  6888,  -878,  7062,  -878,  -878,    21,  -878,  -878,  -878,
     562,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,   472,
   17883,  -878,   449,   448,   338,  -878,   284,  -878,  -878,  -878,
     234,   238,  -878,  -878, 17883,  1912,  -878,   458,   460,  -878,
    -878,  -878,   461,   463, 11949,   465,   467,  -878,  -878,  -878,
   17231,  3477,   317,   469,   470,  -878,  -878,   380,  -878,  -878,
    -878,  -878,  -878,   222,  -878,  -878,  -878, 11949,  -878, 17883,
    -878, 11949, 11949,    27,   567,   387,  -878,  -878,  -878,  -878,
   17231,   395,  -878,  -878,   453,  5130,  -878,  -878,  -878,   474,
     181,   476,  -878,   227,  -878,   479,  -878,   130, 17231,  8454,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878, 17883,
   17883,     6, 14283,  -878,  -878,   543,  8454,   482,   459,   318,
    5130,  2135,   110,    57,  -878,  5130,  -878,  -878, 15176,  -878,
      33, 15647,   684,  -878,   477,   480,  -878, 15176,   181,   684,
    -878,  8454,  2751,  2751,   200,    -1,  -878,    30,  -878,  -878,
    -878,  -878,  7584,  7584,  -878,   955,  -878,   481,   486,  -878,
    -878,  2981,   494,  -878,  -878, 17231,   185, 11949, 11949,  -878,
      -4,  -878,  -878, 17231,   490, 15693,  -878, 15176,   181, 17231,
     492,  8454,  -878, 17231, 15800,  -878,  -878,  -878,  -878,  -878,
     181,   493,   263,   263,  3173,   684,   684,   -34,  -878,  -878,
    4245,   -30,  -878, 11949,   506,   114,   487,  3173,   661,  -878,
    -878, 17883,  -878,  -878,  4068,  -878,    34,  -878,   543,  -878,
    -878,   689,   519,  4422, 11949,  -878, 11949,   627,  -878,  -878,
   14936,   164,   472, 17231,   362, 11949,  5130,   615,  -878,  -878,
    9675, 10197, 10722,  -878,  -878,  -878, 17883, 11949,   525,  -878,
   17567,   512,   234,  -878,  -878,  -878,  -878,   610,  -878,  -878,
    -878,  -878,    52,   611,  -878,  -878,  -878, 13478,   577,   226,
    -878,   538,   243,   417,  -878,   555, 11949, 11949, 11949, 10722,
   10722, 11949,   423, 11949, 11949, 11949, 11949, 11949,   334, 13647,
   11949, 11949, 11949, 11949, 11949, 11949, 11949, 11949, 11949, 11949,
   11949, 11949, 11949, 11949, 11949, 11949,   637,  -878,  -878,  -878,
    -878,  -878,  9849,  9849,  -878,  -878,  -878,  -878,  9849,  -878,
    -878,  9849,  9849,  8454,  8628, 10722, 10722, 15880,   535,  -878,
   10896, 11949, 17883,  7758,  -878,  -878,  -878,  3375,  -878, 11949,
    -878,   583,   536,   563,  -878,  -878,   586,   590,  5130,   687,
    5130,  -878,   688, 11949,   652,   549,  -878,  9849, 17883,  -878,
    -878,  -878,  2135,  -878,  -878,   150,  -878, 12123,   599, 11949,
    3375,  -878,  -878, 11949,  -878, 17609, 11949, 11949,  -878,   553,
    -878,  -878,  -878,  -878, 17697,  -878,   234, 13341,  -878,  2135,
    -878,   579, 12123,  -878, 17231, 17231,  -878,   476,  -878,    50,
    -878,  9849,   556,  -878,   707,   707,  -878,  -878,  -878,  -878,
   11073,  -878, 15921,  7932,  8106,  -878,  -878,  -878, 10722, 10722,
    -878,   466,  8805,  -878,   213,  -878,  4599,  -878,   301, 15967,
     313,   521, 17883,  6537,  6363,  -878,   472,   558,  -878,   227,
    -878, 15256,   614, 17883,    63, 15176,   564, 13939,   -24,     2,
   16074,   -15,    55, 14768,   109,   587,  2637,   569, 17753,   613,
     -19,  -878,  -878,   160,  -878,  -878,    60,  -878,  -878,  6189,
    -878,   630,  -878,   570,   595,  -878,   598,  -878,   601,   607,
     612,   617,   619,  -878,   620,   621,   623,   626,   629,   631,
     632,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878, 11949,   616,   634,   570,  -878,   570,  -878,
   11949,  -878, 17883,  -878,  -878,  -878, 15485, 15485,   396,  -878,
     396,  -878,   396, 13037,   933,  2200,   734,   181,   263,  -878,
     434,  -878,  -878,  -878,  -878,  -878,  3173,  2286,   396, 13211,
   13211, 15485, 13211, 13211,  1213,   263,  2286, 17397,  1213,   684,
     684,   263,  3173,   594,   597,   622,   624,   625,   628,   606,
     585,  -878,   396,  -878,   396, 11949,  9849,    97, 15336,   633,
     223,  -878,  -878,  -878,   167,  -878,  2019, 17276,   412,   316,
   17231,  9849,  -878,  9849, 11949, 11949,   751,  5130,   752, 16120,
    5130, 15377, 17883,  -878,   182,  -878,   192,  -878,   198,  2135,
     110,  -878, 17231,  8979,  -878, 17231,  -878,  -878,  -878, 17883,
   16160, 16241,  -878,   579,   640,  -878,   204, 12123,  -878,   229,
    -878, 11949,  -878,   636,    74,   618,  -878,  3111,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,     7, 14844,  -878,  -878,
     232,  -878,  -878,  -878,  -878, 11949,  -878,    86, 14040, 11949,
    -878, 11247,  6537,  6363,  -878,  9153,   447,  9327,   455,   457,
    9501,  7236,   464,   327,  -878,  -878,  -878, 16281,   641,   635,
     638,  -878, 17883,  3375, 10722,    63, 11949,   746,  -878,  -878,
   11949, 17231,   639,  5130,   642, 10722, 11421,  3891,   643, 11949,
   11598,  -878,  -878,    63,  5307, 10722,  -878,    63,  -878,  9849,
     645,  1636,  -878,  -878, 11949,    53,  -878,  1219,  -878,   630,
    -878,  -878,  -878,  -878,  -878,    51,   673,  -878, 16327,  -878,
   14201,  -878,  -878, 17231,  -878,   648,   647,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,   251, 10722, 13939, 12297, 10722,
    9849,  -878,  -878,  7758,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,    47, 10722, 10722,  -878,
    -878,  -878, 17231, 17231,  5130,  -878,  5130, 11949,  -878,  5130,
     761,   649,   666, 17883,   476,  -878,  -878,   599,  -878,  -878,
    -878,   654,   662,   229, 11949,   630,   579,  -878, 10374,  -878,
    -878, 17231,  -878,  5130, 11949,  -878,  -878,  -878, 17883,  -878,
     650,   476, 10722,  5130, 10722,  9849,  -878,  -878,  -878,  -878,
    -878,   434,  -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -878,  -878,  6537,  -878,  -878, 16434,
      83, 15530,   328,   670,  6537,  -878, 11949,  -878,  -878,  2302,
     330,   332,    76, 16488,  -878, 16529, 11949,   259,  7758,  -878,
    5130,    22, 15431,  -878,  -878, 17231,  5130, 17231,  -878, 16587,
    -878,  9675, 17231,    59, 15016,  -878,   278,  -878,  -878,  -878,
    -878,  3472,  1314,  -878, 17231, 17883,   608,  -878,    51,    81,
   11949,  -878,   651,   663,  -878,   570,   570,  -878,  -878,  -878,
     665, 16682, 11949,   764, 16758,   281,  -878,   599,   283,   286,
    -878,  -878, 17231,   768,  5130,  -878,  -878,  -878,  -878,  -878,
    8454,  8454,  -878,  -878,  -878,  -878,   229, 17231,  -878,    40,
      62,   674, 11949,  -878,    24, 15606,    55, 15096,  -878,   191,
   11775,  6537,  6363,  6537,  6363,  -878,  -878,  -878, 16799, 17231,
    2579,  -878,  -878,  -878,    63,    63,  -878, 15176,  -878,   297,
    -878,  -878,  5484,   678,  5661,   679,  -878, 11949, 14363,  5838,
    -878,    63,  6015,  -878,    63,   683,  1314,  -878,  -878,  -878,
   17753,  -878,  -878,  -878,   137,  -878,   -11,  -878,   134, 16845,
      66,  -878,  -878,  -878, 12471, 12645, 16952, 12819, 12993,  -878,
    -878,  5130,  -878,   676,   685,  -878,  -878,  5130,  -878,   476,
   17231,  5130,  5130,  -878,  -878, 17231,   333,   692,   337,   694,
   11949,  -878,  -878,  -878,  -878,  7758,  -878,  9675,  -878, 17231,
    5130,  9675,  -878, 17231,  5130, 17231,  5130,  9675, 17231,  -878,
    9675, 17231,  -878,  -878,  -878,  -878,    68,  -878, 11949,    81,
     137,   137,   137,   137,   137,   137,    81,  4953,  -878,  -878,
    -878,  -878, 11949, 11949, 11949, 11949, 11949,  -878,   696,   697,
     614,  -878,  -878,  -878,  -878,  -878, 17231,  -878, 14443,  -878,
   14523,  -878, 16120, 14603, 14683,  -878, 17231,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,   655,  -878, 17028, 17069, 17114, 17190,
    -878,  -878,  5130,  5130,  5130,  5130, 11949, 11949, 11949, 11949,
   16120, 16120, 16120, 16120
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   144,   465,    55,   678,   679,
     680,   674,   675,   681,   215,   590,   133,   166,   562,   173,
     564,   590,   172,   496,   494,   133,     0,   215,   281,   167,
     205,   204,   672,   203,   215,   171,    68,   282,   344,   168,
     145,     0,     0,     0,   339,     0,   215,   175,   215,   215,
     607,   582,   682,   176,   215,   345,   556,   491,   215,     0,
       0,   190,   188,   432,   170,   565,   493,   215,   215,   215,
     215,   560,   215,     0,   174,     0,     0,   146,   215,   673,
     215,   215,   484,   169,   319,   558,   495,   177,   215,   731,
     215,   733,   215,   734,   735,   623,   215,   732,   215,   631,
     186,   730,     0,   215,   215,     4,     0,     5,    10,    11,
      12,    48,    51,    52,    56,     0,    46,    71,    13,    77,
      14,    15,    16,    17,    30,   542,   543,    23,    47,   187,
     197,   215,   206,   645,   198,    18,     0,    32,    31,    20,
       0,   276,    19,   641,    22,    36,    33,    34,   196,   320,
       0,   194,     0,     0,   638,   343,     0,   635,   192,   360,
     452,   443,   633,   195,     0,     0,   193,   650,   629,   628,
     632,   546,   544,     0,   215,   639,   640,   644,   643,   642,
       0,   545,     0,   651,   652,   653,   676,   677,   634,   548,
     547,   636,   637,     0,    29,   564,   167,   215,   145,     0,
     565,   215,   215,     0,     0,   638,   650,   544,   639,   640,
     554,   545,   651,   652,     0,     0,   591,   134,   135,     0,
     563,     0,   485,     0,   492,     0,    21,     0,   530,   215,
     141,   147,   159,   153,   152,   161,   138,   151,   162,   148,
     142,   163,   136,   164,   157,   150,   158,   156,   154,   155,
     137,   139,   149,   160,   165,   143,   329,   140,   214,     0,
       0,     0,     0,    70,    69,    71,   215,     0,     0,     0,
       0,     0,     0,     0,   500,     0,   183,    41,     0,   311,
       0,   310,   716,   611,   608,   609,   610,     0,   557,   717,
       7,   215,   342,   342,   439,   180,   434,   181,   435,   436,
     440,   441,   179,   178,   442,   432,   526,     0,   347,   348,
     350,     0,   433,   525,   352,   513,     0,   215,   215,   184,
     635,   626,   649,   627,     0,     0,    44,     0,   561,   549,
       0,   215,    45,   555,     0,   289,    48,   293,   290,   293,
     559,     0,   718,   720,   621,   715,   714,     0,    74,    78,
       0,     0,   532,   215,     0,     0,   533,   622,     0,     6,
     317,     0,   204,   318,     0,    49,     0,     9,    71,    50,
      53,     0,    60,     0,   215,    72,   215,     0,   496,   202,
       0,   674,   320,   646,   210,   215,     0,   218,   219,   220,
     215,   215,   215,   217,   566,   573,     0,   215,     0,   316,
       0,   432,   452,   450,   451,   449,   368,   454,   453,   457,
     456,   458,     0,   447,   444,   445,   448,     0,   489,     0,
     486,     0,     0,   630,    35,   612,   215,   215,   215,   215,
     215,   215,   719,   215,   215,   215,   215,   215,     0,   620,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   399,   406,   407,   408,
     403,   405,   215,   215,   401,   404,   402,   400,   215,   410,
     409,   215,   215,   215,   215,   215,   215,     0,     0,   218,
     215,   215,     0,   215,    37,    25,    38,     0,    26,   215,
      39,   542,     0,   537,   538,   541,     0,     0,     0,   250,
       0,   399,   246,   215,     0,     0,    24,   215,     0,   260,
      27,   504,     0,   505,   507,     0,   524,   215,   510,   215,
       0,   182,    40,   215,   313,     0,   215,   215,    42,     0,
     191,   189,   437,   438,     0,   433,   452,   432,   425,     0,
     424,   528,   215,   126,   647,   648,   358,     0,   185,     0,
      43,   215,     0,   300,   291,   292,    28,    76,    75,    79,
     215,   683,     0,   215,   215,   670,   668,     8,   215,   215,
     216,    56,   215,    54,     0,    65,     0,   124,     0,   119,
       0,    86,     0,   215,   215,   199,   320,     0,   212,     0,
     209,   590,     0,     0,     0,     0,     0,   574,     0,     0,
     530,     0,     0,     0,   287,   321,     0,     0,     0,   337,
       0,   330,   417,     0,   419,   423,     0,   455,   518,     0,
     520,   459,   446,   415,   141,   395,   147,   393,   153,   152,
     151,   148,   142,   397,   163,   164,   150,   154,   137,   149,
     165,   392,   374,   377,   375,   376,   398,   387,   378,   391,
     383,   381,   394,   382,   380,   385,   390,   379,   384,   388,
     389,   386,   396,   215,   371,     0,   415,   372,   415,   369,
     215,   488,     0,   483,   499,   498,   711,   710,   713,   722,
     721,   726,   725,   707,   704,   705,   706,   624,   694,   144,
       0,   664,   665,   146,   663,   662,   618,   698,   709,   703,
     701,   712,   702,   700,   692,   697,   699,   708,   691,   695,
     696,   693,   619,     0,     0,     0,     0,     0,     0,     0,
       0,   724,   723,   728,   727,   215,   215,     0,     0,   287,
     601,   602,   604,   606,     0,   593,     0,     0,     0,     0,
     531,   215,   551,   215,   215,   215,   249,     0,   245,     0,
       0,     0,     0,   550,     0,   263,     0,   261,     0,   508,
       0,   523,   522,   215,   502,   617,   501,   312,   309,     0,
       0,     0,   661,   528,   353,   349,     0,   215,   529,   510,
     351,   215,   357,     0,     0,     0,   552,     0,   294,   684,
     625,   669,   534,   533,   671,   535,     0,     0,    57,    58,
       0,    61,    63,    67,    66,   215,   101,     0,     0,   215,
      94,   215,   215,   215,    64,   215,   374,   215,   375,   376,
     215,   215,   386,     0,   411,   412,    81,    80,    93,     0,
       0,   322,     0,     0,   215,     0,   215,     0,   223,   222,
     215,   575,     0,     0,     0,   215,   215,     0,     0,   215,
     215,   687,   234,     0,     0,   215,   232,     0,   284,   215,
       0,   342,   328,   334,   215,   332,   327,   432,   418,   459,
     516,   515,   514,   517,   460,   466,   432,   367,     0,   373,
       0,   363,   364,   490,   487,     0,     0,   132,   130,   131,
     129,   128,   127,   659,   660,     0,   215,   685,   215,   215,
     215,   603,   605,   215,   592,   166,   173,   172,   171,   168,
     175,   176,   170,   174,   169,   177,     0,   215,   215,   497,
     540,   539,   258,   257,     0,   252,     0,   215,   248,     0,
     254,     0,   286,     0,     0,   506,   509,   510,   511,   512,
     308,     0,     0,   510,   215,   459,   528,   527,   215,   428,
     426,   359,   302,     0,   215,   301,   304,   553,     0,   295,
     298,     0,   215,     0,   215,   215,    59,   125,   122,   102,
     114,   108,   107,   106,   116,   103,   117,   112,   105,   113,
     111,   109,   110,   104,   115,   118,   215,   121,   120,    89,
      88,    87,     0,     0,   215,   200,   215,   201,   322,   342,
       0,     0,   590,   590,   226,     0,   215,     0,   215,   242,
       0,     0,     0,   686,   240,   576,     0,   689,   688,     0,
     235,   215,   569,     0,     0,   233,     0,   315,   190,   188,
     314,   342,   342,   323,   338,     0,     0,   420,   466,     0,
     215,   361,   422,     0,   370,   415,   415,   666,   667,   286,
       0,     0,   215,     0,     0,     0,   594,   510,   650,   650,
     251,   247,   581,   253,     0,    73,   283,   262,   259,   503,
     215,   215,   429,   427,   354,   355,   510,   414,   306,     0,
       0,     0,   215,   296,     0,     0,     0,     0,    62,     0,
     215,   215,   215,   215,   215,    95,    98,    84,    83,    82,
     342,   207,   213,   211,     0,     0,   221,     0,   244,     0,
     596,   243,     0,     0,     0,     0,   241,   215,     0,     0,
     230,     0,     0,   228,     0,   288,   342,   325,   324,   333,
       0,   340,   480,   481,     0,   482,   468,   471,     0,   467,
       0,   416,   365,   366,   215,   215,     0,   215,   215,   288,
     598,     0,   256,     0,     0,   431,   430,     0,   305,     0,
     299,     0,     0,   123,    91,    90,     0,     0,     0,     0,
     215,   208,   225,   224,   227,   215,   595,   215,   238,   578,
       0,   215,   236,   577,     0,   690,     0,   215,   568,   231,
     215,   567,   229,   285,   326,   336,     0,   479,   215,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   461,   463,
     362,   462,   215,   215,   215,   215,   215,   255,   613,   614,
     307,   297,    97,   100,    96,    99,    85,   597,     0,   239,
       0,   237,   572,     0,     0,   341,   470,   472,   473,   476,
     477,   478,   474,   475,   469,   464,     0,     0,     0,     0,
     615,   616,     0,     0,     0,     0,   215,   215,   215,   215,
     580,   579,   571,   570
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -878,  -878,  -878,     0,   -62,   725,  -878,   322,   695,  -878,
    -878,   371,  -392,  -107,  -878,   244,  -878,  -878,  -125,  -878,
    -878,    79,   602,  -878,  -537,  2018,   497,  -563,  -878,  -790,
    -878,  -878,  -878,    67,  -878,  -878,  -878,   846,  -878,   106,
    1327,  -878,  -878,  -390,   -27,  -878,  -814,  -878,  -878,   273,
    -878,  -878,  -878,  -878,  -878,  -878,  -878,  -878,    10,  -878,
    -146,   508,  -312,  -878,   489,  -878,  -878,   -57,  1447,  -878,
    -878,  -878,  -878,   547,  -878,   -71,  -878,  -878,  -878,  -878,
     365,  -878,  -878,  -878,  -106,  -878,  -341,  -791,  -878,  -878,
    -878,  -878,  -878,  -599,  -878,    26,  -878,  -878,  -878,  -878,
    -878,   353,  -878,  1308,  -878,  -878,  -878,  -878,   507,  -878,
    -878,  -878,  -878,    12,  -402,  -165,  -773,  -840,  -624,  -878,
      18,  -878,    28,   363,   -50,   593,   -49,  -878,  -878,  -353,
    -775,  -878,  -297,  -878,  -138,  -305,  -294,  -623,  -878,  -878,
      75,   236,  -878,  -202,   775,  -878,  -159,   389,   152,  -387,
    -749,  -601,  -878,  -878,  -878,  -594,  -505,  -878,  -734,     4,
      37,  -878,  -259,  -496,  -534,    -2,  -878,  -878,  -878,  -878,
    -878,  -878,   163,  -878,   -18,  -878,  -878,  -569,  -878,  -877,
    -878,  -878,   368,   383,  -878,  -878,  -878,  -878,  2454,  -878,
     -43,   741,  -878,   965,  1242,  -878,  -878,  -878,  -878,  -878,
    -878,  -878,  -878,  -878,  -479
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   349,   106,   107,   108,   109,   110,   366,
     367,   111,   112,   113,   114,   372,   573,   800,   801,   115,
     116,   117,   377,   118,   119,   120,   350,   828,   580,   829,
     121,   122,   987,   577,   578,   123,   124,   219,   255,   272,
     125,   257,   126,   306,   856,   127,   128,   293,   292,   129,
     130,   131,   132,   133,   134,   589,   832,   135,   203,   137,
     392,   393,   598,   138,   261,   139,   756,   757,   204,   141,
     142,   143,   144,   554,   788,   961,   145,   146,   784,   956,
     279,   280,   147,   148,   149,   150,   398,   861,   151,   152,
     610,  1035,  1036,   611,   153,   205,  1196,   155,   156,   307,
     308,   309,   945,   157,   321,   547,   781,   158,   159,  1140,
     160,   161,   665,   666,   824,   825,   826,   949,   877,   402,
     613,  1043,   614,   541,   615,   311,   535,   417,   406,   412,
     875,  1210,  1211,   162,  1041,  1136,  1137,  1138,   163,   164,
     419,   420,   671,   165,   166,   223,   273,   274,   514,   515,
     764,   313,   873,   620,   621,   518,   314,   778,   779,   351,
     354,   355,   492,   493,   494,   206,   168,   169,   394,   170,
     395,   171,   207,   173,   215,   216,   734,   848,  1109,   735,
     736,   174,   208,   209,   177,   178,   179,   322,   180,   384,
     495,   211,   182,   212,   213,   185,   186,   187,   188,   601,
     189,   190,   191,   192,   193
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   305,   105,   221,   738,   370,   422,   505,   608,   863,
     782,   754,   136,   310,   312,   667,   472,   785,   872,   277,
     823,   487,   869,   993,   167,   324,  1056,   382,   154,   844,
     950,   227,   529,   853,   857,   988,   136,   780,   802,   943,
     999,   587,   881,   326,   882,   341,   385,  1033,   516,   616,
     845,   783,   154,   618,   386,   962,  -281,   481,  -335,   385,
     356,   516,   387,   388,   389,   385,   275,   386,   479,   388,
     389,   316,   552,  1157,   167,   387,   388,   389,   167,   167,
     602,   387,   388,   389,  1132,  -519,   136,  -282,   275,   689,
     136,   136,  1119,  -521,  1038,   275,   275,   503,   842,  1207,
     167,  1207,   154,  1072,   167,   519,   154,   154,   217,  1198,
     498,   516,   136,  1039,  -519,   265,   136,   679,   681,   560,
    1091,   546,  -521,     3,   842,   758,  -303,   761,   154,   842,
     865,  1110,   154,   194,   850,   256,  1133,  -264,  1199,   557,
    1132,   198,   390,   561,   842,   896,   842,   268,   269,   270,
     953,   846,   776,   843,   866,   390,   489,  1092,  -521,  -519,
     218,   390,   851,   721,   723,   172,   937,  -521,  -356,  -303,
    1075,  1040,  -519,   229,  -521,  1134,   194,   842,   364,   847,
    -521,   842,   523,   774,   963,  1033,  1135,   360,  1069,   172,
     368,   693,  1133,   954,  1073,   517,  -521,   391,   214,  1112,
     619,  1161,  -331,   480,   266,   364,   520,  1100,   517,   921,
     568,   489,  1076,   167,   719,   720,   391,  1127,  1128,  -519,
     569,  1097,  -521,  -303,  -519,   136,  -331,   364,   857,   871,
     895,   364,  -521,  1208,   364,   364,  1155,  -521,   364,   172,
     364,   154,  1135,   172,   172,   830,   846,   955,   532,   992,
     283,   522,   668,   524,   363,    24,   399,   602,   517,  -521,
     528,  1200,   407,   564,   -70,   172,   413,   -69,   167,   172,
     418,   901,   947,   167,   897,   586,   858,  -521,    84,   489,
     136,  -265,   374,   859,   -70,   136,  1033,   -69,  1201,   565,
    1202,  1203,   533,  1204,  1205,   428,   154,   490,  1227,   759,
     550,   154,  1167,   -70,  1169,   478,   -69,    57,  1150,   867,
     284,   408,  1194,   475,   409,   902,   903,   476,   414,    66,
     222,   375,  1057,   769,   432,   760,   410,  1156,   739,   285,
     415,   743,   438,   -70,   489,   868,   -69,   689,   403,   305,
     994,   933,   904,  1074,    86,   411,   286,   759,   167,   416,
     559,   376,   543,   759,   429,   690,   763,   932,   430,   404,
     136,   224,   167,  1026,  1163,   496,   497,   934,   691,   405,
     175,   167,  -271,   935,   136,   672,   154,   513,   172,   946,
     194,   965,   948,   136,   167,   176,   803,   833,   263,   198,
     154,   264,   520,   673,   175,   599,   136,  -270,   692,   154,
     743,   605,   335,   338,  1055,   258,   432,   966,   489,   176,
     675,   436,   154,    38,   438,   669,  -342,   540,   441,   714,
     715,  1142,  1143,  1089,   738,   716,  1049,   743,   717,   718,
     743,   802,  -600,   172,  1108,  -599,  1129,  -342,   172,   693,
      55,   229,  1113,  1115,   175,   694,  1175,  -342,   175,   175,
     805,   336,   336,  1125,  1121,  1124,  1149,  -279,  -600,   176,
    -269,  -599,   809,   176,   176,   520,   266,   256,   806,  1032,
     175,  -266,  1176,   588,   175,   305,   994,   994,   667,   672,
     810,   520,   994,   919,   727,   176,   994,   310,   312,   176,
     364,   473,   508,   474,   995,  1095,   167,  1102,   167,  1103,
    1222,   370,   604,   918,  1224,  -587,   609,   476,   136,   267,
     136,  1197,  -657,   172,  -657,  1113,  1115,  1121,  1124,  -273,
     793,   793,  1002,   664,   154,  -277,   154,   172,  1166,   738,
    1168,  1195,  -274,  1011,   426,   811,   172,   290,  -733,   432,
     294,   427,  -733,  1023,   436,  -268,  -734,   438,  -735,   172,
    -734,   441,  -735,   428,  -658,  -730,  -658,  -272,   812,  -730,
     296,  -657,   291,  -657,   298,   530,   531,   838,   839,  -654,
    -278,  -654,   796,   835,   167,   852,   559,  1238,  1239,  1240,
    1241,  1242,  1243,   175,  1050,   330,   136,  1053,   729,   299,
     331,  -656,   300,  -656,    59,   813,  -280,  -655,   176,  -655,
     792,   795,   154,  1209,   301,   679,   721,  1032,   885,  -267,
     886,  -275,   429,     7,   755,   373,   430,   371,   513,   396,
     397,   400,   401,   304,   482,  -584,   423,   483,  -583,   375,
     424,   768,  -588,   507,  -589,   537,  -586,  -585,   175,  1126,
     540,   485,   542,   175,  1130,   513,   488,   431,   364,   506,
    1084,   526,  1086,   176,   527,   563,   536,   548,   176,  1209,
     556,   172,   566,   172,   432,   433,   551,   434,   435,   436,
     567,   437,   438,   439,  1001,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   612,   814,   452,
     453,   454,   571,   572,   582,   593,   738,   606,   920,   837,
     455,   294,   617,   622,   670,   674,   438,   713,  1032,   726,
     741,   742,   743,   744,   609,  1046,   428,   745,   175,   752,
     939,   296,   747,   750,   753,   298,   763,   777,   772,   787,
     831,   786,   175,   176,   916,   836,   489,   874,   840,   172,
     864,   175,   862,   181,   876,   167,  -144,   176,   167,  -166,
     299,   226,  -173,   300,   175,    59,   176,   136,  -172,   879,
     136,   887,   894,  -171,   888,   301,   428,   181,  -168,   176,
    -145,  -175,  -176,   154,  -170,   429,   154,  -174,   418,   430,
    -146,   893,  -169,  -177,   304,   880,   924,   926,   944,   889,
     994,   890,   891,   957,  1006,   892,  1064,  1147,  1082,  1031,
     867,   332,   997,  1151,  1199,   305,   365,   900,  1004,   952,
     998,  1153,  1154,  1008,   305,   798,  1065,   181,  1027,  1010,
    1016,   181,   181,  1047,  1048,   429,  1020,   432,  1070,   430,
    1025,   359,   436,  1066,   437,   438,  1071,  1096,  1141,   441,
    1088,   167,  1144,   181,  1007,   167,   448,   181,  -421,  1159,
    1193,  1218,   167,   136,   454,  1180,  1184,   136,   931,  1223,
    1219,  1225,  1250,  1251,   136,   513,   175,   504,   175,   154,
     576,   225,   967,   154,   570,   940,  1067,   432,   433,   594,
     154,   176,   436,   176,   437,   438,   555,  1081,   767,   441,
     775,   590,  1045,   960,  1042,  1037,   448,   773,   534,  1235,
    1131,  1244,   452,   453,   454,  1237,   379,  1000,   884,   766,
     172,   936,     0,   172,     0,  1058,  1059,     0,     0,     0,
       0,     0,   167,     0,   167,   370,     0,   167,     0,     0,
       0,     0,     0,     0,   136,     0,   136,  1031,   418,   136,
     484,     0,     0,     0,   175,     0,     0,     0,     0,     0,
     154,   167,   154,   503,     0,   154,   181,     0,  1080,   176,
       0,   167,     0,  1079,     0,   428,     0,   183,     0,     0,
       0,     0,     0,   136,     0,     0,     0,     0,     0,   154,
       0,     0,     0,   294,  1104,  1105,   664,     0,     0,   154,
       0,   183,     0,     0,     0,   510,  1120,  1123,     0,     0,
     521,     0,     0,   296,     0,     0,   172,   298,   167,     0,
     172,   181,     0,     0,   167,     0,   181,   172,     0,   370,
     136,     0,     0,   370,   429,     0,   136,     0,   430,     0,
       0,     0,   299,     0,     0,   300,   154,   358,  1031,   755,
       0,   183,   154,     0,     0,   183,   183,   301,     0,     0,
       0,     0,     0,  1158,     0,     0,     0,     0,     0,  1120,
    1123,     0,   167,   503,   960,   503,   304,   183,   503,   503,
       0,   183,     0,     0,   136,     0,   432,  1172,  1173,     0,
    1174,   436,     0,   437,   438,     0,     0,   172,   441,   172,
     154,   181,   172,     0,  1189,   448,     0,  1192,     0,     0,
       0,   452,   453,   454,     0,   181,     0,     0,     0,     0,
     167,   592,   167,     0,   181,   175,   172,   167,   175,     0,
     167,     0,   136,     0,   136,     0,   172,   181,     0,   136,
     176,     0,   136,   176,     0,     0,     0,     0,   154,     0,
     154,   609,     0,     0,     0,   154,     0,     0,   154,   167,
       0,     0,     0,     0,     0,   167,     0,     0,     0,   167,
     167,   136,     0,     0,     0,     0,     0,   136,     0,     0,
       0,   136,   136,   172,     0,     0,     0,   154,   167,   172,
     183,     0,   167,   154,   167,     0,     0,   154,   154,     0,
     136,     0,     0,     0,   136,     0,   136,     0,     0,     0,
       0,     0,     0,     0,     0,   167,   154,  1245,     0,     0,
     154,   175,   154,     0,     0,   175,     0,   136,     0,     0,
       0,     0,   175,   746,     0,   748,   176,   172,     0,     0,
     176,     0,     0,   154,     0,   183,   609,   176,     0,   181,
     183,   181,     0,     0,   184,   428,     0,   294,     0,     0,
     167,   167,   167,   167,     0,     0,     0,     0,     0,     0,
       0,     0,   136,   136,   136,   136,     0,   296,   184,     0,
       0,   298,     0,     0,     0,   172,     0,   172,   154,   154,
     154,   154,   172,     0,     0,   172,     0,     0,     0,     0,
       0,     0,   175,     0,   175,     0,   299,   175,     0,   300,
       0,    59,     0,     0,   429,     0,     0,   176,   430,   176,
       0,   301,   176,     0,   172,   183,     0,   181,   184,     7,
     172,   175,   184,   184,   172,   172,     0,     0,     0,   183,
     304,   175,     0,     0,     0,     0,   176,   360,   183,     0,
       0,    23,    24,   172,   184,     0,   176,   172,   184,   172,
     361,   183,    30,   362,     0,     0,   432,   433,    33,   434,
     435,   436,     0,   437,   438,    38,     0,     0,   441,     0,
     172,     0,     0,     0,     0,   448,   320,     0,   175,     0,
       0,   452,   453,   454,   175,     0,     0,     0,     0,     0,
       0,     0,    55,   176,    57,     0,   320,     0,  1028,   176,
       0,  1029,     0,     0,   363,     0,    66,     0,     0,     0,
       0,   320,     0,     0,     0,   172,   172,   172,   172,     0,
       0,     0,     0,     0,     0,    82,     0,     0,    84,     0,
       0,    86,   175,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   176,     0,   140,
       0,     0,     0,     0,     0,     0,     0,   184,     0,     0,
       0,     0,     0,   183,     0,   183,     0,     0,     0,     0,
       0,     0,   925,   140,     0,   928,     0,     0,     0,     0,
     175,   100,   175,     0,     0,     0,     0,   175,   181,     0,
     175,   181,     0,     0,     0,   176,     0,   176,     0,     0,
       0,     0,   176,     0,     0,   176,     0,     0,     0,     0,
       0,     0,   184,     0,     0,     0,     0,   184,     0,   175,
       0,     0,     0,   140,     0,   175,     0,     0,     0,   175,
     175,     0,     0,     0,   176,     0,     0,   320,     0,     0,
     176,   183,     0,     0,   176,   176,     0,     0,   175,   140,
       0,     0,   175,   140,   175,     0,   491,     0,     0,     0,
       0,     0,     0,   176,     0,     0,     0,   176,  1009,   176,
       0,     0,  1014,     0,   320,   175,     0,     0,     0,   521,
       0,     0,     0,     0,   181,     0,     0,     0,   181,     0,
     176,     0,   184,   491,     0,   181,     0,     0,     0,   320,
       0,     0,     0,     0,     0,     0,   184,     0,     0,     0,
       0,     0,     0,     0,     0,   184,     0,     0,   491,     0,
     175,   175,   175,   175,     0,     0,     0,     0,   184,     0,
       0,     0,     0,     0,     0,   176,   176,   176,   176,   320,
       0,     7,     0,     0,     0,     0,     0,     0,     0,  1060,
       0,  1061,     0,     0,  1063,     0,     0,     0,   491,   360,
       0,     0,   140,    23,    24,   181,     0,   181,     0,     0,
     181,     0,   361,     0,    30,   362,     0,     0,  1078,     0,
      33,     0,     0,     0,     0,     0,     0,    38,  1014,     0,
       0,     0,     0,     0,   181,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   181,     0,     0,     0,     0,     0,
       0,     0,   183,     0,    55,   183,    57,   140,    59,     0,
    1028,     0,   140,  1029,     0,     0,   363,     0,    66,     0,
       0,     0,     0,     0,     0,  1111,     0,     0,     0,     0,
     184,  1116,   184,     0,     0,     0,     0,    82,     0,     0,
      84,   181,     0,    86,     0,     0,     0,   181,     0,     0,
       0,     0,     0,     0,     0,   695,     0,     0,     0,     0,
     320,   320,     0,     0,     0,     0,   320,     0,     0,   320,
     320,   320,   320,     0,     0,     0,     0,     0,     0,  1152,
       0,     0,     0,     0,     0,     0,     0,   140,     0,     0,
     491,   491,     0,   100,     0,   181,     0,     0,   183,  1030,
       0,   140,   183,     0,     0,   320,     0,     0,   184,   183,
     140,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   140,   491,     0,     0,  1178,     0,  1182,
       0,     0,     0,     0,   521,     0,     0,   521,     0,     0,
       0,     0,     0,   181,     0,   181,     0,     0,     0,   320,
     181,     0,     0,   181,     0,     0,     0,     0,     0,     0,
       0,   320,   320,     0,     0,     0,  1217,     0,   491,     0,
     320,     0,  1220,     0,     0,     0,  1178,  1182,     0,   183,
       0,   183,   181,     0,   183,     0,     0,     0,   181,   491,
       0,     0,   181,   181,     0,  1229,     0,     0,     0,  1231,
       0,   748,     0,   421,     0,   230,     0,     0,   183,     0,
       0,   181,     0,     0,     0,   181,     0,   181,   183,     0,
     231,   232,     0,   233,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,   140,   235,   140,   181,     0,
       0,     0,   236,     0,     0,     0,     0,     0,   237,     0,
       0,     0,   238,     0,     0,   239,     0,   240,     0,     0,
       0,     0,     0,     0,     0,   183,   241,   748,   748,   748,
     748,   183,     0,   242,   243,     0,     0,     0,     0,   184,
       0,   244,   184,   181,   181,   181,   181,     0,     0,     0,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
     246,   247,     0,   248,     0,   249,     0,   250,     0,     0,
     251,     0,   689,   140,   252,     0,     0,   253,     0,   183,
     254,     0,     0,     0,   320,     0,     0,   905,     0,     0,
     906,     0,     0,     0,     0,   907,     0,     0,     0,   320,
       0,   320,     0,   491,     0,   196,     0,     0,     0,     0,
       0,     0,     0,     0,   276,   908,     0,     0,     0,     0,
     491,   320,   909,     0,   198,     0,     0,   183,     0,   183,
       0,     0,     0,   910,   183,   184,   271,   183,   276,   184,
       0,   911,     0,     0,     0,     0,   184,     0,   337,   339,
       0,     0,     0,     0,     0,     0,     0,     0,   912,     0,
       0,     0,     0,     0,     0,     0,   183,     0,     0,     0,
     913,     0,   183,     0,   693,     0,   183,   183,     0,   369,
       0,   914,     0,     0,   968,   695,     0,   915,   230,     0,
       0,     0,     0,     0,     0,   183,     0,     0,     0,   183,
       0,   183,     0,   231,   232,     0,   233,     0,     0,     0,
       0,   234,     0,     0,     0,     0,   184,   320,   184,   235,
       0,   184,   183,     0,     0,   236,     0,     0,     0,     0,
       0,   237,     0,     0,     0,   238,   491,     0,   239,     0,
     240,     0,     0,     0,   140,   184,     0,   140,     0,   241,
       0,     0,     0,     0,     0,   184,   242,   243,   320,     0,
       0,     0,     0,     0,   244,     0,     0,   183,   183,   183,
     183,     0,     0,     0,   245,     0,     0,   491,     0,     0,
       0,     0,   428,   246,   247,     0,   248,     0,   249,   486,
     250,     0,     0,   251,     0,     0,     0,   252,   511,     0,
     253,     0,   184,   254,     0,     0,     0,     0,   184,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   320,     0,     0,     0,     0,     0,   499,
     502,     0,     0,     0,     0,     0,     0,   509,     0,     0,
     140,   429,   491,     0,   140,   430,   276,     0,   276,     0,
       0,   140,     0,     0,     0,   276,   184,     7,     0,   512,
       0,     0,     0,     0,     0,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,   360,     0,     0,     0,    23,
      24,     0,     0,     0,     0,     0,     0,     0,   361,     0,
      30,   362,     0,   432,   433,   276,    33,   435,   436,     0,
     437,   438,     0,    38,   184,   441,   184,     0,     0,     0,
       0,   184,   448,     0,   184,     0,     0,     0,   452,   453,
     454,   140,     0,   140,     0,     0,   140,   429,   320,   320,
      55,   430,    57,     0,    59,     0,  1028,     0,     0,  1029,
       0,     0,   363,   184,    66,     0,     0,   491,   491,   184,
     140,     0,     0,   184,   184,     0,     0,     0,     0,     0,
     140,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,   184,     0,     0,     0,   184,     0,   184,   432,
     433,     0,   434,   435,   436,     0,   437,   438,   439,     0,
       0,   441,   442,   443,     0,   445,   446,   447,   448,   184,
       0,   451,     0,     0,   452,   453,   454,   140,     0,     0,
       0,     0,     0,   140,     0,   455,     0,     0,   210,   100,
       0,     0,   220,     0,     0,  1101,     0,     0,     0,     0,
       0,   228,     0,     0,     0,     0,     0,     0,   262,     0,
       0,     0,     0,     0,   184,   184,   184,   184,     0,     0,
     278,     0,   281,   282,     0,     0,     0,     0,   287,     0,
     288,   140,   289,     0,     0,     0,     0,   315,     0,     0,
       0,   228,   323,   325,   327,   328,   329,     0,     0,     0,
     333,     0,   334,     0,     0,     0,     0,     0,     0,   340,
       0,     0,   323,     0,   342,     0,   343,     0,     0,   344,
     345,     0,   346,     0,     0,     0,   228,   323,   357,   140,
       0,   140,     0,     0,     0,     0,   140,     0,     0,   140,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     7,   380,     0,   383,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   140,     0,
       0,     0,   360,     0,   140,     0,    23,    24,   140,   140,
       0,     0,   276,   276,     0,   361,     0,    30,   362,     0,
     276,   276,     0,    33,     0,     0,     0,   140,   425,     0,
      38,   140,     0,   140,     0,     0,     0,     0,   860,     0,
       0,     0,  -322,     0,     0,     0,     0,     0,     0,     0,
       0,   477,     0,     0,   140,   333,   228,    55,     0,    57,
    -322,    59,     0,  1028,  -322,  -322,  1029,     0,     0,   363,
       0,    66,     0,  -322,     0,  -322,  -322,     0,     0,     0,
       0,  -322,     0,   323,     0,     0,     0,     0,  -322,     0,
      82,  -322,     0,    84,     0,     0,    86,     0,     0,   140,
     140,   140,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -322,     0,     0,  -322,     0,  -322,     0,  -322,
     323,  -322,  -322,     0,  -322,     0,     0,  -322,     0,  -322,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   323,   100,     0,  -322,     0,
       0,  -322,  1171,     0,  -322,     0,   329,   333,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   930,
       0,   544,   545,     0,   360,     0,     0,     0,    23,    24,
       0,     0,     0,     0,     0,   323,     0,   361,     0,    30,
     362,     0,     0,     0,     0,    33,     0,     0,     0,     0,
       0,     0,    38,     0,  -322,   959,     0,   562,     0,     0,
    -322,     0,     0,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   579,    55,
     581,    57,     0,     0,     0,     0,     0,     0,     0,   591,
       0,   363,     0,    66,   595,   600,   603,     0,     0,     0,
       0,   228,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,    84,     0,     0,    86,     0,
       0,   276,     0,     0,     0,   276,     0,     0,     0,     0,
     676,   677,   678,   680,   682,   683,     0,   684,   685,   686,
     687,   688,     0,   696,   697,   698,   699,   700,   701,   702,
     703,   704,   705,   706,   707,   708,   709,   710,   711,   712,
       0,     0,     0,     0,     0,     0,   323,   323,     0,     0,
       0,     0,   323,     0,     0,   323,   323,   323,   323,   722,
     724,     0,     0,     0,   600,   728,     0,   737,     0,     0,
       0,     0,     0,   740,     0,     0,     0,     0,     0,     0,
       0,     0,  1068,     0,   749,     0,     0,   751,     0,     0,
       0,   323,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   762,     0,   765,     0,     0,     0,   281,     0,  1083,
     770,   771,     0,     0,   230,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,   315,     0,     0,   231,
     232,     0,   233,     0,     0,   323,     0,   234,     0,     0,
       0,     0,     0,     0,   740,   235,     0,   323,   323,     0,
       0,   236,   228,   797,     0,     0,   323,   237,     0,     0,
       0,   238,     0,     0,   239,     0,   240,   827,   827,     0,
       0,   276,   276,     0,     0,   241,     0,     0,     0,     0,
       0,   841,   242,   243,     0,     0,     0,     0,     0,     0,
     244,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,     0,     0,   315,     0,     0,     0,     0,     0,   246,
     247,     0,   248,     0,   249,     0,   250,     0,     0,   251,
       0,     0,     0,   252,   538,     0,   253,     0,   276,   254,
       0,     0,     0,     0,   276,   276,     0,     0,     0,     0,
       0,     0,     0,     0,   230,     0,     0,   878,     0,     0,
       0,     0,   276,   276,   883,   276,     0,     0,     0,   231,
     232,     0,   233,     0,     0,     0,   502,   234,     0,   276,
       0,     0,   276,     0,     0,   235,     0,     0,     0,     0,
       0,   236,     0,     0,     0,   539,     0,   237,     0,     0,
       0,   238,     0,     0,   239,     0,   240,     0,     0,     0,
       0,     0,     0,     0,     0,   241,     0,  1221,     0,   749,
     323,     0,   242,   243,     0,     0,     0,     0,     0,     0,
     244,     0,     0,     0,     0,   323,     0,   323,   922,   923,
     245,     0,     0,     0,     0,   428,     0,     0,     0,   246,
     247,     0,   248,     0,   249,     0,   250,   323,     0,   251,
       0,     0,     0,   252,     0,     0,   253,     0,     0,   254,
       0,   315,     0,     0,     0,   951,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   502,     0,   502,     0,
       0,   502,   502,     0,     0,     0,     0,     0,     0,   579,
       0,     0,     0,   989,   429,   991,   827,   827,   430,   342,
       0,   343,     0,     0,   345,   346,     0,     0,     0,     0,
       0,     0,     0,   364,     0,   958,     0,     0,  1003,     0,
    1005,     0,     0,     0,   228,     0,     0,     0,     0,  1012,
     740,  1015,     0,  1017,  1019,     0,     0,     0,  1022,  1024,
       0,     0,     0,   323,     0,     0,   432,   433,  1034,   434,
     435,   436,     0,   437,   438,     0,     0,     0,   441,     0,
       0,     0,     0,     0,   447,   448,     0,     0,   451,     0,
       0,   452,   453,   454,     0,     0,     0,     0,     0,     0,
    1051,  1015,  1022,  1054,   323,     0,     0,   737,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   680,   722,     0,     0,     0,     0,     0,   230,     0,
       0,  1062,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   231,   232,     0,   233,     0,   315,     0,
       0,   234,  1077,     0,     0,     0,     0,     0,   228,   235,
       0,     0,     0,     0,     0,   236,  1085,     0,  1087,   323,
       0,   237,     0,     0,     0,   238,     0,     0,   239,     0,
     240,     0,     0,     0,     0,     0,     0,     0,     0,   241,
     827,     0,     0,     0,     0,     0,   242,   243,  1098,     0,
    1099,     0,     0,     0,   244,     0,     0,     0,     0,     0,
    1107,     0,   737,     0,   245,     0,     0,     0,     0,     0,
       0,     0,     0,   246,   247,  1118,   248,     7,   249,     0,
     250,     0,     0,   251,     0,     0,     0,   252,     0,     0,
     253,     0,     0,   254,  1139,   360,     0,     0,     0,    23,
      24,     0,     0,     0,     0,     0,  1146,     0,   361,     0,
      30,   362,     0,     0,     0,     0,    33,     0,     0,     0,
       0,     0,     0,    38,   323,   323,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1160,     0,     0,     0,
       0,     0,     0,     0,  1165,   827,   827,   827,   827,   271,
      55,     0,    57,     0,   358,     0,  1028,     0,     0,  1029,
       0,     0,   363,     0,    66,     0,  1179,     0,  1183,     0,
       0,  1185,     0,  1188,     0,     0,  1191,     0,     0,     0,
       0,     0,     0,    82,     0,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,  1179,  1183,
       0,  1188,  1191,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,   468,   469,   470,     0,
       0,     0,     0,     0,  1226,     0,     0,     0,     0,   737,
       0,  1228,     0,     0,     0,  1230,     0,     0,     0,   100,
    1232,  1233,     0,     0,  1234,     0,     0,     0,   471,     0,
       0,  -654,  1236,  -654,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1246,  1247,  1232,  1248,
    1249,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1260,  1261,  1262,  1263,
    1260,  1261,  1262,  1263,    -2,     4,     0,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
      15,    16,    17,    18,     0,    19,     0,    20,    21,  -215,
      22,    23,    24,    25,    26,    27,     0,  -215,    28,     0,
      29,     0,    30,    31,    32,  -215,  -215,  -215,    33,    34,
      35,   -68,     0,    36,    37,    38,     0,    39,  -342,    40,
       0,    41,    42,    43,    44,    45,     0,    46,    47,    48,
      49,   -68,    50,    51,     0,    52,    53,    54,     0,  -342,
       0,     0,    55,    56,    57,    58,    59,    60,    61,  -342,
     -68,    62,    63,    64,    65,     0,    66,    67,    68,     0,
      69,    70,    71,    72,    73,    74,    75,    76,     0,    77,
      78,     0,    79,    80,    81,    82,    83,     0,    84,    85,
     -68,    86,    87,     0,     0,  -215,     0,    88,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,   100,     0,     0,   101,     0,   102,     0,   103,     0,
    -215,     0,     4,   104,     5,     6,     7,     8,     9,    10,
      11,    12,    13,     0,  -685,     0,    14,    15,    16,    17,
      18,  -685,    19,     0,    20,    21,  -215,    22,    23,    24,
      25,    26,    27,  -685,  -215,    28,  -685,    29,     0,    30,
      31,    32,  -215,  -215,  -215,    33,    34,    35,   -68,     0,
      36,    37,    38,     0,    39,  -342,    40,     0,    41,    42,
      43,    44,    45,     0,    46,    47,    48,    49,   -68,    50,
      51,     0,    52,    53,    54,     0,  -342,     0,     0,    55,
      56,    57,    58,     0,    60,    61,  -342,   -68,    62,    63,
      64,    65,  -685,    66,    67,    68,  -685,    69,    70,    71,
      72,    73,    74,    75,    76,     0,    77,    78,     0,    79,
      80,    81,    82,    83,     0,    84,    85,   -68,    86,    87,
       0,     0,  -215,     0,    88,     0,     0,  -685,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,  -685,    92,  -685,  -685,  -685,
    -685,  -685,  -685,  -685,     0,  -685,  -685,  -685,  -685,  -685,
    -685,  -685,  -685,  -685,  -685,  -685,  -685,    99,   100,  -685,
    -685,  -685,     0,   102,  -685,   103,     0,  -215,     0,   347,
    -685,     5,     6,     7,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,    15,    16,    17,    18,     0,    19,
       0,    20,    21,  -215,    22,    23,    24,    25,    26,    27,
       0,  -215,    28,     0,    29,     0,    30,    31,    32,  -215,
    -215,  -215,    33,    34,    35,   -68,     0,    36,    37,    38,
       0,    39,  -342,    40,     0,    41,    42,    43,    44,    45,
       0,    46,    47,    48,    49,   -68,    50,    51,     0,    52,
      53,    54,     0,  -342,     0,     0,    55,    56,    57,    58,
      59,    60,    61,  -342,   -68,    62,    63,    64,    65,     0,
      66,    67,    68,     0,    69,    70,    71,    72,    73,    74,
      75,    76,     0,    77,    78,     0,    79,    80,    81,    82,
      83,     0,    84,    85,   -68,    86,    87,     0,     0,  -215,
       0,    88,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   100,     0,     0,   101,     0,
     102,   348,   103,     0,  -215,     0,     4,   104,     5,     6,
       7,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,    15,    16,    17,    18,     0,    19,     0,    20,    21,
    -215,    22,    23,    24,    25,    26,    27,     0,  -215,    28,
       0,    29,     0,    30,    31,    32,  -215,  -215,  -215,    33,
      34,    35,   -68,     0,    36,    37,    38,     0,    39,  -342,
      40,     0,    41,    42,    43,    44,    45,     0,    46,    47,
      48,    49,   -68,    50,    51,     0,    52,    53,    54,     0,
    -342,     0,     0,    55,    56,    57,    58,    59,    60,    61,
    -342,   -68,    62,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,    76,     0,
      77,    78,     0,    79,    80,    81,    82,    83,     0,    84,
      85,   -68,    86,    87,     0,     0,  -215,     0,    88,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,   558,   103,
       0,  -215,     0,   574,   104,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,    16,
      17,    18,     0,    19,     0,    20,    21,  -215,    22,    23,
      24,    25,    26,    27,     0,  -215,    28,     0,    29,     0,
      30,    31,    32,  -215,  -215,  -215,    33,    34,    35,   -68,
       0,    36,    37,    38,     0,    39,  -342,    40,     0,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,   -68,
      50,    51,     0,    52,    53,    54,     0,  -342,     0,     0,
      55,    56,    57,    58,    59,    60,    61,  -342,   -68,    62,
      63,    64,    65,     0,    66,    67,    68,     0,    69,    70,
      71,    72,    73,    74,    75,    76,     0,    77,    78,     0,
      79,    80,    81,    82,    83,     0,    84,    85,   -68,    86,
      87,     0,     0,  -215,     0,    88,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,   100,
       0,     0,   101,     0,   102,   575,   103,     0,  -215,     0,
       4,   104,     5,     6,     7,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,    15,    16,    17,    18,     0,
      19,     0,    20,    21,  -215,    22,    23,    24,    25,    26,
      27,     0,  -215,    28,     0,    29,     0,    30,    31,    32,
    -215,  -215,  -215,    33,    34,    35,   -68,     0,    36,    37,
      38,     0,    39,  -342,    40,     0,    41,    42,    43,    44,
      45,     0,    46,    47,    48,    49,   -68,    50,    51,     0,
      52,    53,    54,     0,  -342,     0,     0,    55,    56,    57,
      58,    59,    60,    61,  -342,   -68,    62,    63,    64,    65,
       0,    66,    67,    68,     0,    69,    70,    71,    72,    73,
      74,    75,    76,     0,    77,    78,     0,    79,    80,    81,
      82,    83,     0,    84,    85,   -68,    86,    87,     0,     0,
    -215,     0,    88,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,   100,     0,     0,   101,
       0,   102,   804,   103,     0,  -215,     0,     4,   104,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,    15,    16,    17,    18,     0,    19,     0,    20,
      21,  -215,    22,    23,    24,    25,    26,    27,     0,  -215,
      28,     0,    29,     0,    30,    31,    32,  -215,  -215,  -215,
      33,    34,    35,   -68,     0,    36,    37,    38,     0,    39,
    -342,    40,     0,    41,    42,    43,    44,    45,     0,    46,
      47,    48,    49,   -68,    50,    51,     0,    52,    53,    54,
       0,  -342,     0,     0,    55,    56,    57,    58,   358,    60,
      61,  -342,   -68,    62,    63,    64,    65,     0,    66,    67,
      68,     0,    69,    70,    71,    72,    73,    74,    75,    76,
       0,    77,    78,     0,    79,    80,    81,    82,    83,     0,
      84,    85,   -68,    86,    87,     0,     0,  -215,     0,    88,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,   100,     0,     0,   101,     0,   102,     0,
     103,     0,  -215,     0,     4,   104,     5,     6,     7,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,    15,
      16,    17,    18,     0,    19,     0,    20,    21,  -215,    22,
      23,    24,    25,    26,    27,     0,  -215,    28,     0,    29,
       0,    30,    31,    32,  -215,  -215,  -215,    33,    34,    35,
     -68,     0,    36,    37,    38,     0,    39,  -342,    40,     0,
      41,    42,    43,    44,    45,     0,    46,    47,    48,    49,
     -68,    50,    51,     0,    52,    53,    54,     0,  -342,     0,
       0,    55,    56,    57,    58,    59,    60,    61,  -342,   -68,
      62,    63,    64,    65,     0,    66,    67,    68,     0,    69,
      70,    71,    72,    73,    74,    75,    76,     0,    77,    78,
       0,    79,    80,    81,    82,    83,     0,    84,    85,   -68,
      86,    87,     0,     0,  -215,     0,    88,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
     100,     0,     0,   101,     0,   102,     0,   103,     0,  -215,
       0,     4,   104,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,    15,    16,    17,    18,
       0,    19,     0,    20,    21,  -215,    22,    23,    24,    25,
      26,    27,     0,  -215,    28,     0,    29,     0,    30,    31,
      32,  -215,  -215,  -215,    33,    34,    35,   -68,     0,    36,
      37,    38,     0,    39,  -342,    40,     0,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,   -68,    50,    51,
       0,    52,    53,    54,     0,  -342,     0,     0,    55,    56,
      57,    58,     0,    60,    61,  -342,   -68,    62,    63,    64,
      65,     0,    66,    67,    68,     0,    69,    70,    71,    72,
      73,    74,    75,    76,     0,    77,    78,     0,    79,    80,
      81,    82,    83,     0,    84,    85,   -68,    86,    87,     0,
       0,  -215,     0,    88,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,   100,     0,     0,
     101,     0,   102,     0,   103,     0,  -215,     0,     4,   104,
       5,     6,     7,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,    15,    16,    17,    18,     0,    19,     0,
      20,    21,  -215,    22,    23,    24,    25,    26,    27,     0,
    -215,    28,     0,    29,     0,    30,    31,    32,  -215,  -215,
    -215,    33,  1021,    35,   -68,     0,    36,    37,    38,     0,
      39,  -342,    40,     0,    41,    42,    43,    44,    45,     0,
      46,    47,    48,    49,   -68,    50,    51,     0,    52,    53,
      54,     0,  -342,     0,     0,    55,    56,    57,    58,     0,
      60,    61,  -342,   -68,    62,    63,    64,    65,     0,    66,
      67,    68,     0,    69,    70,    71,    72,    73,    74,    75,
      76,     0,    77,    78,     0,    79,    80,    81,    82,    83,
       0,    84,    85,   -68,    86,    87,     0,     0,  -215,     0,
      88,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,   100,     0,     0,   101,     0,   102,
       0,   103,     0,  -215,     0,     4,   104,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
      15,    16,    17,    18,     0,    19,     0,    20,    21,  -215,
      22,    23,    24,    25,    26,    27,     0,  -215,    28,     0,
      29,     0,    30,    31,    32,  -215,  -215,  -215,    33,  1177,
      35,   -68,     0,    36,    37,    38,     0,    39,  -342,    40,
       0,    41,    42,    43,    44,    45,     0,    46,    47,    48,
      49,   -68,    50,    51,     0,    52,    53,    54,     0,  -342,
       0,     0,    55,    56,    57,    58,     0,    60,    61,  -342,
     -68,    62,    63,    64,    65,     0,    66,    67,    68,     0,
      69,    70,    71,    72,    73,    74,    75,    76,     0,    77,
      78,     0,    79,    80,    81,    82,    83,     0,    84,    85,
     -68,    86,    87,     0,     0,  -215,     0,    88,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,   100,     0,     0,   101,     0,   102,     0,   103,     0,
    -215,     0,     4,   104,     5,     6,     7,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,    15,    16,    17,
      18,     0,    19,     0,    20,    21,  -215,    22,    23,    24,
      25,    26,    27,     0,  -215,    28,     0,    29,     0,    30,
      31,    32,  -215,  -215,  -215,    33,  1181,    35,   -68,     0,
      36,    37,    38,     0,    39,  -342,    40,     0,    41,    42,
      43,    44,    45,     0,    46,    47,    48,    49,   -68,    50,
      51,     0,    52,    53,    54,     0,  -342,     0,     0,    55,
      56,    57,    58,     0,    60,    61,  -342,   -68,    62,    63,
      64,    65,     0,    66,    67,    68,     0,    69,    70,    71,
      72,    73,    74,    75,    76,     0,    77,    78,     0,    79,
      80,    81,    82,    83,     0,    84,    85,   -68,    86,    87,
       0,     0,  -215,     0,    88,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,   100,     0,
       0,   101,     0,   102,     0,   103,     0,  -215,     0,     4,
     104,     5,     6,     7,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,    15,    16,    17,    18,     0,    19,
       0,    20,    21,  -215,    22,    23,    24,    25,    26,    27,
       0,  -215,    28,     0,    29,     0,    30,    31,    32,  -215,
    -215,  -215,    33,  1187,    35,   -68,     0,    36,    37,    38,
       0,    39,  -342,    40,     0,    41,    42,    43,    44,    45,
       0,    46,    47,    48,    49,   -68,    50,    51,     0,    52,
      53,    54,     0,  -342,     0,     0,    55,    56,    57,    58,
       0,    60,    61,  -342,   -68,    62,    63,    64,    65,     0,
      66,    67,    68,     0,    69,    70,    71,    72,    73,    74,
      75,    76,     0,    77,    78,     0,    79,    80,    81,    82,
      83,     0,    84,    85,   -68,    86,    87,     0,     0,  -215,
       0,    88,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,   100,     0,     0,   101,     0,
     102,     0,   103,     0,  -215,     0,     4,   104,     5,     6,
       7,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,    15,    16,    17,    18,     0,    19,     0,    20,    21,
    -215,    22,    23,    24,    25,    26,    27,     0,  -215,    28,
       0,    29,     0,    30,    31,    32,  -215,  -215,  -215,    33,
    1190,    35,   -68,     0,    36,    37,    38,     0,    39,  -342,
      40,     0,    41,    42,    43,    44,    45,     0,    46,    47,
      48,    49,   -68,    50,    51,     0,    52,    53,    54,     0,
    -342,     0,     0,    55,    56,    57,    58,     0,    60,    61,
    -342,   -68,    62,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,    76,     0,
      77,    78,     0,    79,    80,    81,    82,    83,     0,    84,
      85,   -68,    86,    87,     0,     0,  -215,     0,    88,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,   100,     0,     0,   101,     0,   102,     0,   103,
     870,  -215,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,   295,     0,   196,     0,     0,     0,    32,
    -215,  -215,  -215,     0,   197,    35,     0,     0,     0,   297,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,   302,    73,
      74,    75,   303,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,  -215,     5,     6,   104,     8,
       9,    10,    11,    12,    13,     0,   625,     0,    14,     0,
       0,    17,    18,   627,    19,     0,   195,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   196,
       0,     0,     0,    32,     0,     0,     0,     0,   197,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   198,   633,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   200,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   201,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    89,
     501,   457,   458,   459,   460,   461,     0,     0,   464,   465,
     466,   467,     0,   469,   470,     0,   815,   816,   817,   818,
     819,   646,     0,   647,     0,    95,     0,   648,   649,   650,
     651,   652,   653,   654,   820,   656,   657,    97,   821,    99,
     -92,   659,   660,   822,   662,   202,     0,   103,     0,     0,
       5,     6,   104,     8,     9,    10,    11,    12,    13,     0,
     625,     0,    14,     0,     0,    17,    18,   627,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,   197,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,   633,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     201,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    89,   501,   457,   458,   459,   460,   461,
       0,     0,   464,   465,   466,   467,     0,   469,   470,     0,
     815,   816,   817,   818,   819,   646,     0,   647,     0,    95,
       0,   648,   649,   650,   651,   652,   653,   654,   820,   656,
     657,    97,   821,    99,     0,   659,   660,   822,   662,   202,
       0,   103,     0,     0,     0,     0,   104,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
      15,     0,    17,    18,     0,    19,     0,   195,    21,     0,
      22,     0,     0,     0,     0,    27,     0,     0,    28,     0,
     196,     0,     0,     0,    32,     0,     0,     0,     0,    34,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   198,
       0,   199,     0,     0,    44,    45,     0,    46,    47,    48,
      49,     0,    50,    51,     0,    52,    53,    54,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   200,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,    74,    75,    76,     0,    77,
      78,     0,    79,     0,     0,     0,    83,     0,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   102,     0,   103,     0,
       0,     5,     6,   104,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   195,     0,     0,    22,   378,    24,     0,     0,     0,
       0,     0,    28,     0,   196,     0,     0,     0,    32,     0,
       0,     0,     0,   197,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   198,     0,   199,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,    57,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   200,     0,
      66,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   201,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,     0,    85,     0,    86,    87,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,     0,     0,     5,     6,   104,     8,     9,
      10,   381,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,    20,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   196,     0,
       0,     0,    32,  -215,  -215,  -215,     0,   197,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   198,     0,   199,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,    65,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   201,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,    84,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,     0,  -215,     5,
       6,   104,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   195,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   196,     0,     0,     0,    32,     0,     0,     0,
       0,   197,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   198,     0,   199,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   200,  -729,     0,     0,
       0,  -729,     0,     0,    71,    72,    73,    74,    75,   201,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,  -384,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,  -384,     0,     0,   101,     0,   202,  -384,
     103,     0,     0,     5,     6,   104,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   195,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   196,     0,     0,     0,
      32,     0,     0,     0,     0,   197,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   198,     0,   199,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     200,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   201,     0,    77,     0,     0,    79,   317,
     318,     0,    83,   352,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,   353,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,     0,     0,     0,
     101,     0,   202,     0,   103,     0,     0,     5,     6,   104,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   195,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     196,     0,     0,     0,    32,  -215,  -215,  -215,     0,   197,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   198,
       0,   199,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   200,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   201,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,     0,
    -215,     5,     6,   104,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   195,     0,     0,    22,     0,   730,     0,     0,     0,
       0,     0,    28,     0,   196,     0,     0,     0,    32,     0,
       0,     0,     0,   197,    35,     0,   731,     0,    37,     0,
       0,    39,     0,   198,     0,   199,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   200,     0,
     732,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   201,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,     0,    85,     0,   733,    87,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,     0,     0,     5,     6,   104,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   195,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   196,     0,
       0,     0,    32,     0,     0,     0,     0,   197,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   198,     0,   199,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   200,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   201,     0,    77,     0,     0,
      79,   317,   318,     0,    83,   352,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,   791,     0,     5,
       6,   104,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   195,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   196,     0,     0,     0,    32,     0,     0,     0,
       0,   197,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   198,     0,   199,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   200,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   201,
       0,    77,     0,     0,    79,   317,   318,     0,    83,   352,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,   794,     0,     5,     6,   104,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   195,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   196,     0,     0,     0,
      32,     0,     0,     0,     0,   197,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   198,     0,   199,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     200,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,   201,     0,    77,     0,     0,    79,   317,
     318,     0,    83,     0,     0,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    91,    92,    93,    94,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,    97,    98,    99,   319,     0,     0,
     101,     0,   202,     0,   103,     0,     0,     5,     6,   104,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   195,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     196,     0,     0,     0,    32,     0,     0,     0,     0,   197,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   198,
       0,   199,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   200,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   201,     0,    77,
       0,     0,    79,   317,   318,     0,    83,     0,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,  -536,
       0,     5,     6,   104,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   195,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   196,     0,     0,     0,    32,     0,
       0,     0,     0,   197,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   198,     0,   199,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   200,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   201,     0,    77,     0,     0,    79,   317,   318,     0,
      83,     0,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,     0,     0,  -536,     0,   104,     5,     6,
       0,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   195,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   196,     0,     0,     0,    32,     0,     0,     0,     0,
     197,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     198,     0,   199,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   200,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   201,     0,
      77,     0,     0,    79,   317,   318,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
     799,     0,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,   938,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   201,     0,    77,     0,     0,    79,   317,   318,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,     0,     5,     6,   104,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   195,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   196,
       0,     0,     0,    32,     0,     0,     0,     0,   197,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   198,     0,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   200,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   201,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,  -392,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
    -392,     0,     0,   101,     0,   202,  -392,   103,     0,     0,
       5,     6,   104,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,   197,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     201,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,  -377,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,  -377,     0,     0,   101,     0,   202,
    -377,   103,     0,     0,     5,     6,   104,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   201,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
    -385,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,  -385,     0,
       0,   101,     0,   202,  -385,   103,     0,     0,     5,     6,
     104,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   195,     0,
       0,    22,     0,   259,     0,     0,     0,     0,     0,    28,
       0,   196,     0,     0,     0,    32,     0,     0,     0,     0,
     197,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     198,     0,   199,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   200,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   201,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,   260,    87,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
       0,     0,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   201,     0,    77,     0,     0,    79,   317,   318,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,     0,     5,     6,   104,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   195,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   196,
       0,     0,     0,    32,     0,     0,     0,     0,   197,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   198,     0,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   200,  -729,     0,     0,     0,  -729,     0,
       0,    71,    72,    73,    74,    75,   201,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,     0,     0,
       5,     6,   104,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,   197,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     201,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,   596,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,     0,   597,     0,   104,     5,     6,     0,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   195,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     196,     0,     0,     0,    32,     0,     0,     0,     0,   197,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   198,
       0,   199,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   200,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,   201,     0,    77,
       0,     0,    79,     0,     0,     0,    83,     0,     0,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    90,    91,    92,
      93,    94,     0,  -413,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,    97,    98,
      99,     0,     0,     0,   101,     0,   202,     0,   103,  -413,
       0,     5,     6,   104,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   195,     0,     0,    22,     0,     0,     0,     0,     0,
       0,   275,    28,     0,   196,     0,     0,     0,    32,     0,
       0,     0,     0,   197,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   198,     0,   199,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   200,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   201,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     102,     0,   103,     0,     0,     5,     6,   104,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   195,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   196,     0,
       0,     0,    32,     0,     0,     0,     0,   197,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   198,     0,   199,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   200,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,   201,     0,    77,     0,     0,
      79,     0,     0,     0,    83,     0,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,   596,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,    98,    99,     0,
       0,     0,   101,     0,   202,     0,   103,     0,     0,     5,
       6,   104,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   195,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   196,     0,     0,     0,    32,     0,     0,     0,
       0,   197,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   198,     0,   199,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   200,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,    75,   201,
       0,    77,     0,     0,    79,     0,     0,     0,    83,     0,
       0,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    90,
      91,    92,    93,    94,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     0,     0,
      97,    98,    99,     0,     0,     0,   101,     0,   202,     0,
     103,     0,     0,   597,     0,   104,     5,     6,     0,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   195,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   196,
       0,     0,     0,    32,     0,     0,     0,     0,   197,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   198,     0,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   200,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   201,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,   789,   103,     0,     0,
       5,     6,   104,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,   197,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     201,     0,    77,     0,     0,    79,     0,     0,     0,    83,
     990,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,     0,     5,     6,   104,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   201,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   202,     0,   103,     0,     0,  1013,     0,
     104,     5,     6,     0,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   195,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   196,     0,     0,     0,    32,     0,
       0,     0,     0,   197,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   198,     0,   199,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   200,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,   201,     0,    77,     0,     0,    79,     0,     0,     0,
      83,     0,     0,    85,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,    97,    98,    99,     0,     0,     0,   101,     0,
     202,     0,   103,     0,     0,  1018,     0,   104,     5,     6,
       0,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   195,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   196,     0,     0,     0,    32,     0,     0,     0,     0,
     197,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     198,     0,   199,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   200,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   201,     0,
      77,     0,     0,    79,     0,     0,     0,    83,  1164,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
       0,     0,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,   197,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   201,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,     0,     5,     6,   104,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   195,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,   295,     0,   196,
       0,     0,     0,    32,     0,     0,     0,     0,   197,    35,
       0,     0,     0,   297,     0,     0,    39,     0,   198,     0,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   200,     0,     0,     0,     0,     0,     0,
       0,    71,   302,    73,    74,    75,   303,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,     0,     0,
       5,     6,   104,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,     0,     0,
       0,     0,  1052,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     201,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,     0,     5,     6,   104,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,     0,     0,     0,     0,  1212,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   201,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,    98,    99,     0,     0,
       0,   101,     0,   202,     0,   103,     0,     0,     5,     6,
     104,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   195,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   196,     0,     0,     0,    32,     0,     0,     0,     0,
    1213,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     198,     0,   199,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   200,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,    75,   201,     0,
      77,     0,     0,    79,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,    97,
      98,    99,     0,     0,     0,   101,     0,   202,     0,   103,
       0,     0,     5,     6,   104,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
       0,     0,     0,     0,  1215,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   198,     0,   199,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   201,     0,    77,     0,     0,    79,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     0,    97,    98,    99,     0,     0,     0,   101,
       0,   202,     0,   103,     0,     0,     5,     6,   104,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   195,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   196,
       0,     0,     0,    32,     0,     0,     0,     0,  1216,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   198,     0,
     199,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,   428,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   200,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,   201,     0,    77,     0,
       0,    79,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,   429,     0,
       0,     0,   430,     0,     0,     0,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,    97,    98,    99,
       0,     0,     0,   101,     0,   202,     0,   103,     0,     0,
       0,     6,   104,     8,     9,    10,    11,    12,    13,     0,
     432,   433,    14,   434,   435,   436,    18,   437,   438,   439,
     195,   440,   441,   442,   443,   294,   445,   446,   447,   448,
     449,     0,   451,   196,     0,   452,   453,   454,  -215,  -215,
    -215,     0,   197,     0,     0,   296,   455,     0,     0,   298,
       0,     0,     0,     0,   199,     0,     0,    44,    45,     0,
       0,     0,     0,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,   428,   299,     0,    56,   300,    58,    59,
      60,     0,     0,     0,     0,    63,     0,   200,     0,   301,
       0,     0,     0,     0,     0,    71,     0,    73,     0,    75,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,   430,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,    97,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,  -346,  -215,     0,     6,   104,     8,     9,    10,
      11,    12,    13,     0,   432,   433,    14,   434,   435,   436,
      18,   437,   438,   439,   195,     0,   441,     0,     0,   294,
       0,     0,   447,   448,     0,     0,   451,   196,     0,   452,
     453,   454,  -215,  -215,  -215,     0,   197,     0,     0,   296,
     455,     0,     0,   298,     0,     0,     0,     0,   199,     0,
       0,    44,    45,     0,     0,     0,     0,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,     0,   299,     0,
      56,   300,    58,    59,    60,     0,     0,     0,     0,    63,
       0,   200,     0,   301,     0,     0,     0,     0,     0,    71,
       0,    73,     0,    75,     0,     0,     0,     0,     0,     0,
       0,     0,   304,     0,     0,     0,    85,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   623,
       0,   624,     0,     0,    90,    91,    92,    93,    94,     0,
       0,   625,     0,    95,     0,     0,   626,   232,   627,   628,
       0,     0,    96,     0,   629,    97,    98,    99,     0,     0,
       0,   101,   235,   202,   196,   103,     0,  -215,   236,     0,
     104,     0,     0,     0,   630,     0,     0,     0,   238,     0,
       0,   631,     0,   632,   633,     0,     0,     0,     0,     0,
       0,     0,   634,     0,     0,     0,     0,     0,     0,   242,
     635,     0,     0,     0,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   636,     0,     0,
       0,     0,     0,     0,     0,     0,   246,   247,     0,   637,
       0,   249,     0,   638,     0,     0,   251,     0,     0,     0,
     639,     0,     0,   253,     0,     0,   640,     0,     0,     0,
       0,     0,     0,     0,     0,   501,   457,   458,   459,   460,
     461,     0,     0,   464,   465,   466,   467,     0,   469,   470,
       0,   641,   642,   643,   644,   645,   646,     0,   647,     0,
       0,     0,   648,   649,   650,   651,   652,   653,   654,   655,
     656,   657,     0,   658,     0,     0,   659,   660,   661,   662,
       5,     6,   663,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     195,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   196,     0,     0,     0,    32,  -215,  -215,
    -215,     0,   197,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   198,     0,   199,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   200,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,    75,
     201,     0,    77,     0,     0,    79,     0,     0,     0,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      90,    91,    92,    93,    94,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
       0,     0,    98,    99,     0,     0,     0,   101,     0,   202,
       0,   103,     0,  -215,     5,     6,     0,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   195,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   196,     0,     0,
       0,    32,  -215,  -215,  -215,     0,   197,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   198,     0,   199,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   200,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,    75,   201,     0,    77,     0,     0,    79,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     5,     6,     0,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   195,     0,    90,    22,    92,     0,     0,     0,
       0,     0,     0,    28,     0,   196,     0,     0,     0,    32,
    -215,  -215,  -215,     0,   197,    35,     0,    99,     0,    37,
       0,     0,    39,   202,   198,   103,   199,  -215,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   200,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,    75,   201,   689,    77,     0,     0,    79,     0,     0,
       0,    83,     0,   625,    85,     0,     0,    87,   969,   970,
     627,   971,     0,     0,     0,     0,   972,     0,     0,     0,
       0,     0,     0,     0,   691,     0,     0,     0,     0,     0,
       0,     0,    90,     0,    92,     0,   973,     0,     0,     0,
     974,     0,     0,   975,     0,   198,   633,     0,     0,     0,
       0,     0,     0,     0,   692,    99,     0,     0,     0,     0,
       0,     0,   976,   103,     0,  -215,     0,     0,     0,   977,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   978,
       0,     0,     0,     0,     0,     0,     0,     0,   979,   980,
       0,   981,     0,   982,     0,   693,     0,     0,     0,     0,
       0,   694,   983,     0,     0,   984,     0,     0,   985,     0,
       0,     0,     0,     0,     0,     0,     0,   501,   457,   458,
     459,   460,   461,     0,     0,   464,   465,   466,   467,     0,
     469,   470,     0,   641,   642,   643,   644,   645,   646,     0,
     647,     0,     0,     0,   648,   649,   650,   651,   652,   653,
     654,   655,   656,   657,   230,   658,     0,     0,   659,   660,
     661,   662,   986,     0,   625,     0,     0,     0,     0,   231,
     232,   627,   233,     0,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,   235,     0,     0,     0,     0,
       0,   236,     0,     0,     0,     0,     0,   237,     0,     0,
       0,   238,     0,     0,   239,     0,   240,   633,     0,     0,
       0,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,   242,   243,     0,     0,     0,     0,     0,     0,
     244,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,     0,     0,     0,     0,     0,   426,     0,     0,   246,
     247,     0,   248,   427,   249,     0,   250,     0,     0,   251,
       0,     0,     0,   252,     0,   428,   253,     0,     0,   254,
       0,     0,     0,     0,     0,     0,     0,     0,   501,   457,
     458,   459,   460,   461,     0,     0,   464,   465,   466,   467,
       0,   469,   470,     0,   641,   642,   643,   644,   645,   646,
       0,   647,     0,     0,     0,   648,   649,   650,   651,   652,
     653,   654,   655,   656,   657,     0,   658,     0,     0,   659,
     660,   661,   662,     0,   429,     0,   426,     0,   430,     0,
       0,     0,     0,   427,     0,     0,     0,   500,     0,     0,
       0,     0,     0,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
     501,   457,   458,   459,   460,   461,     0,     0,   464,   465,
     466,   467,     0,   469,   470,     0,   432,   433,     0,   434,
     435,   436,     0,   437,   438,   439,     0,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,     0,
       0,   452,   453,   454,   429,   364,   426,     0,   430,     0,
       0,     0,   455,   427,     0,     0,     0,  1186,     0,     0,
       0,     0,     0,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
     501,   457,   458,   459,   460,   461,     0,     0,   464,   465,
     466,   467,     0,   469,   470,     0,   432,   433,     0,   434,
     435,   436,     0,   437,   438,   439,     0,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,     0,
       0,   452,   453,   454,   429,   364,   426,     0,   430,     0,
       0,     0,   455,   427,     0,     0,     0,  1252,     0,     0,
       0,     0,     0,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
     501,   457,   458,   459,   460,   461,     0,     0,   464,   465,
     466,   467,     0,   469,   470,     0,   432,   433,     0,   434,
     435,   436,     0,   437,   438,   439,     0,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,     0,
       0,   452,   453,   454,   429,   364,   426,     0,   430,     0,
       0,     0,   455,   427,     0,     0,     0,  1253,     0,     0,
       0,     0,     0,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
     501,   457,   458,   459,   460,   461,     0,     0,   464,   465,
     466,   467,     0,   469,   470,     0,   432,   433,     0,   434,
     435,   436,     0,   437,   438,   439,     0,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,     0,
       0,   452,   453,   454,   429,   364,   426,     0,   430,     0,
       0,     0,   455,   427,     0,     0,     0,  1254,     0,     0,
       0,     0,     0,     0,     0,   428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
     501,   457,   458,   459,   460,   461,     0,     0,   464,   465,
     466,   467,     0,   469,   470,     0,   432,   433,     0,   434,
     435,   436,     0,   437,   438,   439,     0,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,     0,
       0,   452,   453,   454,   429,   364,     0,     0,   430,     0,
       0,   426,   455,     0,     0,     0,     0,  1255,   427,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     428,   854,     0,     0,     0,     0,     0,     0,     0,   431,
     501,   457,   458,   459,   460,   461,   855,     0,   464,   465,
     466,   467,     0,   469,   470,     0,   432,   433,     0,   434,
     435,   436,     0,   437,   438,   439,     0,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,     0,
       0,   452,   453,   454,     0,   364,     0,   426,     0,   429,
       0,     0,   455,   430,   427,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   428,   275,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     842,     0,   964,     0,   431,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,   433,     0,   434,   435,   436,     0,   437,   438,
     439,     0,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,     0,   429,   452,   453,   454,   430,
     364,     0,     0,     0,     0,     0,     0,   455,     0,   426,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   842,     0,   428,     0,
     431,     0,     0,   583,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   433,     0,
     434,   435,   436,     0,   437,   438,   439,     0,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     584,     0,   452,   453,   454,     0,   364,     0,     0,     0,
       0,     0,     0,   455,     0,     0,     0,   429,     0,   426,
       0,   430,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   428,  1122,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
     433,     0,   434,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,     0,   585,   452,   453,   454,   429,     0,   426,
       0,   430,     0,     0,     0,   455,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   428,   275,
       0,     0,     0,     0,     0,     0,     0,     0,   842,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
     433,     0,   434,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,     0,     0,   452,   453,   454,   429,   364,   426,
       0,   430,     0,     0,     0,   455,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   428,   275,
       0,     0,     0,     0,     0,     0,     0,     0,   842,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
     433,     0,   434,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,     0,     0,   452,   453,   454,   429,   364,   426,
       0,   430,     0,     0,     0,   455,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   431,     0,   834,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
     433,     0,   434,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,     0,     0,   452,   453,   454,   429,   364,   426,
       0,   430,     0,     0,     0,   455,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   428,   898,
       0,     0,     0,     0,     0,     0,     0,     0,   214,     0,
       0,     0,   431,     0,   899,     0,     0,     0,     0,     0,
     426,     0,     0,     0,     0,     0,     0,   427,     0,   432,
     433,     0,   434,   435,   436,     0,   437,   438,   439,   428,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,     0,     0,   452,   453,   454,   429,     0,     0,
       0,   430,     0,     0,     0,   455,     0,     0,     0,     0,
       0,     0,     0,     0,   426,     0,     0,     0,     0,     0,
       0,   427,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   431,   428,     0,     0,     0,     0,   429,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,   432,
     433,   929,   434,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,     0,   431,   452,   453,   454,     0,     0,     0,
       0,     0,     0,     0,     0,   455,     0,   428,     0,     0,
     432,   433,   429,   434,   435,   436,   430,   437,   438,   439,
       0,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   426,     0,   452,   453,   454,     0,   364,
     427,     0,     0,   842,     0,     0,   455,   431,     0,     0,
       0,     0,   428,     0,     0,     0,     0,  1093,     0,     0,
       0,     0,     0,     0,   432,   433,   429,   434,   435,   436,
     430,   437,   438,   439,     0,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,     0,     0,   452,
     453,   454,     0,     0,  1094,     0,     0,     0,  1114,     0,
     455,   431,     0,     0,     0,     0,     0,     0,     0,   426,
       0,   429,     0,     0,     0,   430,   427,     0,   432,   433,
       0,   434,   435,   436,     0,   437,   438,   439,   428,   440,
     441,   442,   443,     0,   445,   446,   447,   448,   449,   450,
     451,     0,     0,   452,   453,   454,   431,     0,     0,     0,
     426,   525,     0,     0,   455,     0,     0,   427,     0,     0,
       0,     0,     0,   432,   433,     0,   434,   435,   436,   428,
     437,   438,   439,     0,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,     0,   429,   452,   453,
     454,   430,     0,     0,     0,     0,   426,     0,     0,   455,
       0,     0,     0,   427,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   428,     0,     0,   842,     0,
       0,     0,   431,     0,     0,     0,     0,     0,   429,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,   432,
     433,     0,   434,   435,   436,     0,   437,   438,   439,     0,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,     0,   431,   452,   453,   454,     0,     0,     0,
       0,     0,     0,  1162,   429,   455,     0,     0,   430,     0,
     432,   433,     0,   434,   435,   436,     0,   437,   438,   439,
       0,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   426,     0,   452,   453,   454,     0,   431,
     427,     0,     0,     0,     0,     0,   455,     0,     0,     0,
       0,     0,   428,     0,     0,     0,   432,   433,     0,   434,
     435,   436,     0,   437,   438,   439,     0,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,     0,
       0,   452,   453,   454,     0,   549,     0,     0,     0,     0,
       0,     0,   455,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   429,     0,   426,     0,   430,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   428,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,     0,   426,     0,     0,     0,     0,     0,
       0,   427,     0,   432,   433,     0,   434,   435,   436,     0,
     437,   438,   439,   428,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,     0,   553,   452,   453,
     454,   429,     0,     0,     0,   430,     0,     0,     0,   455,
     426,   807,     0,     0,   725,     0,     0,   427,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   428,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,   429,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,   432,   433,     0,   434,   435,   436,     0,
     437,   438,   439,     0,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,     0,   431,   452,   453,
     454,     0,     0,     0,     0,     0,     0,     0,   429,   455,
       0,     0,   430,     0,   432,   433,     0,   434,   435,   436,
       0,   437,   438,   439,     0,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   426,     0,   452,
     453,   454,     0,   431,   427,     0,   790,     0,     0,     0,
     455,     0,     0,     0,     0,     0,   428,     0,     0,     0,
     432,   433,     0,   434,   435,   436,     0,   437,   808,   439,
       0,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   426,     0,   452,   453,   454,     0,     0,
     427,     0,     0,     0,     0,     0,   455,     0,     0,     0,
       0,     0,   428,     0,     0,   927,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,   426,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   428,     0,     0,     0,     0,     0,     0,   849,
     431,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,   430,     0,   432,   433,     0,
     434,   435,   436,     0,   437,   438,   439,     0,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
       0,     0,   452,   453,   454,     0,   431,     0,     0,     0,
       0,   429,     0,   455,   426,   430,     0,     0,     0,     0,
       0,   427,     0,   432,   433,     0,   434,   435,   436,     0,
     437,   438,   439,   428,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   431,     0,   452,   453,
     454,     0,     0,     0,   426,   996,     0,     0,     0,   455,
       0,   427,     0,   432,   433,     0,   434,   435,   436,     0,
     437,   438,   439,   428,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,     0,     0,   452,   453,
     454,     0,   429,     0,     0,   941,   430,     0,     0,   455,
     426,     0,     0,     0,     0,     0,     0,   427,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   428,
       0,     0,     0,     0,     0,     0,     0,   431,     0,     0,
       0,     0,   429,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,     0,   432,   433,     0,   434,   435,   436,
       0,   437,   438,   439,     0,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   431,     0,   452,
     453,   454,     0,     0,     0,     0,   942,     0,   429,     0,
     455,     0,   430,     0,   432,   433,     0,   434,   435,   436,
       0,   437,   438,   439,     0,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   426,  1090,   452,
     453,   454,     0,   431,   427,     0,     0,     0,     0,     0,
     455,     0,     0,     0,     0,     0,   428,     0,     0,     0,
     432,   433,     0,   434,   435,   436,     0,   437,   438,   439,
       0,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,     0,     0,   452,   453,   454,     0,     0,
       0,   426,  1044,     0,     0,     0,   455,     0,   427,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     428,     0,     0,     0,     0,   429,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   426,     0,     0,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,   428,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   433,   429,
     434,   435,   436,   430,   437,   438,   439,     0,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     426,     0,   452,   453,   454,     0,     0,   427,     0,     0,
     214,     0,     0,   455,   431,     0,     0,     0,     0,   428,
     429,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,   432,   433,     0,   434,   435,   436,     0,   437,   438,
     439,     0,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,     0,   431,   452,   453,   454,     0,
       0,     0,     0,     0,     0,     0,     0,   455,     0,     0,
       0,     0,   432,   433,     0,   434,   435,   436,   429,   437,
     438,   439,   430,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   426,  1106,   452,   453,   454,
       0,     0,   427,     0,     0,     0,     0,     0,   455,     0,
       0,     0,  1117,   431,   428,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     432,   433,     0,   434,   435,   436,     0,   437,   438,   439,
       0,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,     0,     0,   452,   453,   454,     0,     0,
       0,     0,     0,     0,     0,     0,   455,     0,     0,     0,
       0,   426,     0,   429,     0,     0,     0,   430,   427,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     428,  1148,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431,     0,
       0,     0,   426,  1170,     0,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,   432,   433,     0,   434,   435,
     436,   428,   437,   438,   439,     0,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,     0,   429,
     452,   453,   454,   430,     0,     0,     0,     0,   426,  1145,
       0,   455,     0,     0,     0,   427,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   428,     0,     0,
       0,     0,     0,     0,   431,     0,     0,     0,     0,     0,
     429,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,   432,   433,     0,   434,   435,   436,  1206,   437,   438,
     439,     0,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,     0,   431,   452,   453,   454,     0,
       0,     0,     0,     0,     0,     0,   429,   455,     0,     0,
     430,     0,   432,   433,     0,   434,   435,   436,     0,   437,
     438,   439,     0,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   426,     0,   452,   453,   454,
       0,   431,   427,     0,     0,     0,     0,     0,   455,     0,
       0,     0,     0,     0,   428,     0,     0,     0,   432,   433,
       0,   434,   435,   436,     0,   437,   438,   439,     0,   440,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,     0,     0,   452,   453,   454,     0,     0,     0,     0,
       0,     0,     0,     0,   455,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   426,     0,   429,     0,     0,     0,   430,   427,     0,
       0,     0,     0,     0,     0,     0,  1214,     0,     0,     0,
     428,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431,     0,
       0,     0,   426,     0,     0,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,   432,   433,     0,   434,   435,
     436,   428,   437,   438,   439,     0,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,     0,   429,
     452,   453,   454,   430,     0,     0,     0,   426,     0,     0,
       0,   455,  1256,     0,   427,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   428,     0,     0,     0,
       0,     0,     0,     0,   431,     0,     0,     0,     0,     0,
     429,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,   432,   433,  1257,   434,   435,   436,     0,   437,   438,
     439,     0,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,     0,   431,   452,   453,   454,     0,
       0,     0,     0,   426,     0,   429,     0,   455,     0,   430,
     427,     0,   432,   433,     0,   434,   435,   436,  1258,   437,
     438,   439,   428,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,     0,     0,   452,   453,   454,
     431,     0,     0,     0,   426,     0,     0,     0,   455,     0,
       0,   427,     0,     0,     0,     0,     0,   432,   433,     0,
     434,   435,   436,   428,   437,   438,   439,     0,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
       0,   429,   452,   453,   454,   430,     0,     0,     0,   426,
       0,     0,     0,   455,  1259,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
       0,     0,   429,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,   432,   433,     0,   434,   435,   436,     0,
     437,   438,   439,     0,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,     0,   431,   452,   453,
     454,     0,     0,     0,     0,     0,     0,   917,     0,   455,
       0,   430,     0,     0,   432,   433,     0,   434,   435,   436,
       0,   437,   438,   439,     0,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,     0,     0,   452,
     453,   454,   431,     0,     0,     0,     0,     0,     0,     0,
     455,     0,     0,     0,     0,     0,     0,     0,     0,   432,
     433,     0,   434,   435,   436,     0,   437,   438,   439,   428,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,     0,   360,   452,   453,   454,    23,    24,     0,
       0,     0,     0,     0,     0,   455,   361,     0,    30,   362,
       0,     0,     0,     0,    33,     0,     0,     0,     0,     0,
       0,    38,     0,     0,  -342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   -68,   429,     0,
       0,     0,   430,     0,     0,  -342,     0,     0,    55,     0,
      57,     0,     0,     0,    61,  -342,   -68,    62,     0,     0,
     363,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,    84,     0,     0,    86,     0,     0,
     432,   433,     0,   434,   435,   436,     0,   437,   438,   439,
       0,   440,   441,   442,   443,     0,   445,   446,   447,   448,
     449,     0,   451,     0,     0,   452,   453,   454,   607,     0,
     230,     0,     7,     0,     0,     0,   455,     0,     0,     0,
       0,     0,     0,     0,     0,   231,   232,   100,   233,     0,
       0,     0,   364,   234,     0,     0,     0,     0,     0,     0,
       0,   235,     0,     0,     0,     0,     0,   236,     0,     0,
       0,     0,   230,   237,     0,     0,     0,   238,     0,     0,
     239,     0,   240,     0,     0,     0,     0,   231,   232,     0,
     233,   241,     0,     0,     0,   234,     0,    24,   242,   243,
       0,     0,     0,   235,     0,     0,   244,     0,     0,   236,
       0,     0,     0,     0,     0,   237,   245,     0,     0,   238,
       0,     0,   239,     0,   240,   246,   247,     0,   248,     0,
     249,     0,   250,   241,     0,   251,     0,     0,     0,   252,
     242,   243,   253,     0,     0,   254,     0,     0,   244,    57,
       0,     0,     0,     0,     0,     0,     0,     0,   245,     0,
     230,    66,     0,     0,     0,     0,     0,   246,   247,     0,
     248,     0,   249,     0,   250,   231,   232,   251,   233,     0,
       0,   252,     0,   234,   253,     0,    86,   254,     0,     0,
       0,   235,     0,     0,     0,     0,     0,   236,     0,     0,
       0,     0,     0,   237,     0,     0,     0,   238,     0,     0,
     239,     0,   240,     0,     0,     0,   230,     0,     7,     0,
       0,   241,     0,     0,     0,     0,     0,     0,   242,   243,
       0,   231,   232,     0,   233,     0,   244,     0,     0,   234,
       0,     0,     0,     0,     0,     0,   245,   235,     0,     0,
       0,     0,     0,   236,     0,   246,   247,     0,   248,   237,
     249,     0,   250,   238,     0,   251,   239,     0,   240,   252,
     538,     0,   253,     0,     0,   254,     0,   241,     0,     0,
       0,     0,     0,     0,   242,   243,     0,     0,     0,     0,
       0,     0,   244,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   245,     0,  -317,     0,     0,     0,     0,     0,
       0,   246,   247,     0,   248,     0,   249,     0,   250,  -317,
    -317,   251,  -317,     0,     0,   252,     0,  -317,   253,     0,
       0,   254,     0,     0,     0,  -317,     0,     0,     0,     0,
       0,  -317,     0,     0,     0,     0,   230,  -317,     0,     0,
       0,  -317,     0,     0,  -317,     0,  -317,     0,     0,     0,
       0,   231,   232,     0,   233,  -317,     0,     0,     0,   234,
       0,     0,  -317,  -317,     0,     0,     0,   235,     0,     0,
    -317,     0,     0,   236,     0,     0,     0,     0,     0,   237,
    -317,     0,     0,   238,     0,     0,   239,     0,   240,  -317,
    -317,     0,  -317,     0,  -317,     0,  -317,   241,     0,  -317,
       0,     0,     0,  -317,   242,   243,  -317,     0,     0,  -317,
       0,     0,   244,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   245,     0,  -318,     0,     0,     0,     0,     0,
       0,   246,   247,     0,   248,     0,   249,     0,   250,  -318,
    -318,   251,  -318,     0,     0,   252,     0,  -318,   253,     0,
       0,   254,     0,     0,     0,  -318,     0,     0,     0,     0,
       0,  -318,     0,     0,     0,     0,     0,  -318,     0,     0,
       0,  -318,     0,     0,  -318,     0,  -318,     0,     0,     0,
       0,     0,     0,     0,     0,  -318,     0,     0,     0,     0,
       0,     0,  -318,  -318,     0,     0,     0,     0,     0,     0,
    -318,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -318,     0,     0,     0,     0,     0,     0,     0,     0,  -318,
    -318,     0,  -318,     0,  -318,     0,  -318,     0,     0,  -318,
       0,     0,     0,  -318,     0,     0,  -318,     0,     0,  -318
};

static const yytype_int16 yycheck[] =
{
       2,    63,     2,    21,   483,   112,   165,   266,   400,   608,
     547,   507,     2,    63,    63,   417,   181,   551,   619,    46,
     583,   223,   616,   813,    26,    68,   903,   133,     2,   598,
     779,    27,   291,   602,   603,   808,    26,   542,   572,   773,
     831,   382,   666,    70,   668,    88,    25,   861,     1,   402,
      48,     1,    26,     1,    33,    48,    57,   203,     5,    25,
     103,     1,    41,    42,    43,    25,    33,    33,    41,    42,
      43,    67,   331,    33,    76,    41,    42,    43,    80,    81,
     392,    41,    42,    43,     3,    33,    76,    57,    33,     3,
      80,    81,    33,    33,   869,    33,    33,   262,   122,    33,
     102,    33,    76,   943,   106,    48,    80,    81,     3,   120,
     104,     1,   102,    62,    62,    36,   106,   429,   430,   149,
      37,   125,    62,     0,   122,   512,    76,   517,   102,   122,
     149,  1008,   106,   167,   149,    29,    55,    57,   149,   173,
       3,    55,   121,   173,   122,    48,   122,    41,    42,    43,
      76,   149,   539,   177,   173,   121,   149,    74,    48,   107,
      55,   121,   177,   475,   476,     2,   760,   107,   172,   119,
     945,   120,   120,   174,   127,    94,   167,   122,   172,   177,
     120,   122,   149,   536,   177,   999,   105,    23,   937,    26,
     111,   105,    55,   119,   943,   148,   149,   176,   122,   177,
     148,   177,   149,   176,   174,   172,   149,   998,   148,   743,
     176,   149,   946,   215,   473,   474,   176,  1031,  1032,   167,
     366,   994,   175,   173,   172,   215,   173,   172,   797,   619,
     726,   172,   172,   167,   172,   172,  1076,   127,   172,    76,
     172,   215,   105,    80,    81,   586,   149,   173,    48,   812,
      19,   278,   417,   280,    90,    28,   150,   569,   148,   149,
     287,   127,    28,   149,    47,   102,    28,    47,   270,   106,
     164,    48,   777,   275,   177,   381,   167,   167,   114,   149,
     270,    57,    47,   174,    67,   275,  1100,    67,   154,   175,
     156,   157,    92,   159,   160,    32,   270,   167,  1175,   149,
     327,   275,  1092,    86,  1094,   199,    86,    80,  1057,   149,
      79,    77,  1126,    91,    80,    92,   149,    95,    80,    92,
     111,    86,   916,   525,   143,   175,    92,  1076,   487,    98,
      92,   149,   151,   116,   149,   175,   116,     3,    54,   401,
     149,   149,   175,   944,   117,   111,   115,   149,   350,   111,
     350,   116,   167,   149,    91,    21,   127,   175,    95,    75,
     350,    92,   364,   859,   173,   259,   260,   175,    34,    85,
       2,   373,    57,   175,   364,   149,   350,   271,   215,   175,
     167,   149,   153,   373,   386,     2,   173,   589,    84,    55,
     364,    87,   149,   167,    26,   391,   386,    57,    64,   373,
     149,   397,    80,    81,   900,    12,   143,   175,   149,    26,
     167,   148,   386,    51,   151,   417,    54,   311,   155,   462,
     463,  1045,  1046,   986,   903,   468,   175,   149,   471,   472,
     149,   965,   149,   270,   175,   149,  1035,    75,   275,   105,
      78,   174,  1011,  1012,    76,   111,   149,    85,    80,    81,
     149,    80,    81,   175,  1023,  1024,   175,    57,   175,    76,
      57,   175,   149,    80,    81,   149,   174,   361,   167,   861,
     102,    57,   175,   111,   106,   537,   149,   149,   880,   149,
     167,   149,   149,   167,   480,   102,   149,   537,   537,   106,
     172,   174,   174,   176,   167,   167,   498,   167,   500,   167,
     167,   608,   396,    91,   167,   167,   400,    95,   498,   105,
     500,  1134,   174,   350,   176,  1084,  1085,  1086,  1087,    57,
     563,   564,   834,   417,   498,    57,   500,   364,  1091,  1008,
    1093,  1130,    57,   845,    13,    14,   373,     9,    91,   143,
      28,    20,    95,   855,   148,    57,    91,   151,    91,   386,
      95,   155,    95,    32,   174,    91,   176,    57,    37,    95,
      48,   174,   174,   176,    52,   292,   293,   594,   595,   174,
      57,   176,   568,   591,   576,   602,   576,  1200,  1201,  1202,
    1203,  1204,  1205,   215,   896,   102,   576,   899,   482,    77,
     174,   174,    80,   176,    82,    74,    57,   174,   215,   176,
     563,   564,   576,  1140,    92,   917,   918,   999,   174,    57,
     176,    57,    91,     5,   508,   172,    95,   151,   512,    57,
     148,   172,   174,   111,    57,   167,   166,   174,   167,    86,
     167,   525,   167,   174,   167,   149,   167,   167,   270,  1031,
     534,   167,   148,   275,  1036,   539,   167,   126,   172,   167,
     962,   174,   964,   270,   174,   149,   175,   167,   275,  1196,
     167,   498,   175,   500,   143,   144,   174,   146,   147,   148,
       9,   150,   151,   152,   833,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   175,   582,   168,
     169,   170,     3,   174,    67,    80,  1175,   172,   741,   593,
     179,    28,    92,    92,   127,   167,   151,    70,  1100,   174,
     127,   175,   149,   127,   608,   880,    32,   127,   350,    67,
     763,    48,    35,    35,   175,    52,   127,   148,   175,    22,
     172,   175,   364,   350,   736,   121,   149,   107,   174,   576,
     127,   373,   173,     2,   174,   747,   151,   364,   750,   151,
      77,    26,   151,    80,   386,    82,   373,   747,   151,   143,
     750,   167,   177,   151,   167,    92,    32,    26,   151,   386,
     151,   151,   151,   747,   151,    91,   750,   151,   672,    95,
     151,   175,   151,   151,   111,   151,    35,    35,   148,   167,
     149,   167,   167,   175,    48,   167,    35,    33,   148,   861,
     149,    76,   167,    35,   149,   867,   111,   174,   835,   173,
     172,  1070,  1071,   174,   876,   571,   167,    76,   173,   177,
     177,    80,    81,   175,   177,    91,   853,   143,   174,    95,
     857,   106,   148,   167,   150,   151,   174,   167,   175,   155,
     965,   843,   177,   102,   840,   847,   162,   106,   175,   175,
     167,   175,   854,   843,   170,   177,   177,   847,   752,   167,
     175,   167,   166,   166,   854,   759,   498,   265,   500,   843,
     373,    25,   805,   847,   366,   769,   933,   143,   144,   390,
     854,   498,   148,   500,   150,   151,   339,   958,   523,   155,
     537,   384,   880,   787,   876,   867,   162,   534,   305,  1196,
    1038,  1206,   168,   169,   170,  1199,   131,   832,   672,   520,
     747,   759,    -1,   750,    -1,   917,   918,    -1,    -1,    -1,
      -1,    -1,   924,    -1,   926,  1032,    -1,   929,    -1,    -1,
      -1,    -1,    -1,    -1,   924,    -1,   926,   999,   832,   929,
     215,    -1,    -1,    -1,   576,    -1,    -1,    -1,    -1,    -1,
     924,   953,   926,  1118,    -1,   929,   215,    -1,   954,   576,
      -1,   963,    -1,   953,    -1,    32,    -1,     2,    -1,    -1,
      -1,    -1,    -1,   963,    -1,    -1,    -1,    -1,    -1,   953,
      -1,    -1,    -1,    28,  1002,  1003,   880,    -1,    -1,   963,
      -1,    26,    -1,    -1,    -1,   270,  1023,  1024,    -1,    -1,
     275,    -1,    -1,    48,    -1,    -1,   843,    52,  1010,    -1,
     847,   270,    -1,    -1,  1016,    -1,   275,   854,    -1,  1126,
    1010,    -1,    -1,  1130,    91,    -1,  1016,    -1,    95,    -1,
      -1,    -1,    77,    -1,    -1,    80,  1010,    82,  1100,   933,
      -1,    76,  1016,    -1,    -1,    80,    81,    92,    -1,    -1,
      -1,    -1,    -1,  1080,    -1,    -1,    -1,    -1,    -1,  1086,
    1087,    -1,  1064,  1228,   958,  1230,   111,   102,  1233,  1234,
      -1,   106,    -1,    -1,  1064,    -1,   143,  1104,  1105,    -1,
    1107,   148,    -1,   150,   151,    -1,    -1,   924,   155,   926,
    1064,   350,   929,    -1,  1121,   162,    -1,  1124,    -1,    -1,
      -1,   168,   169,   170,    -1,   364,    -1,    -1,    -1,    -1,
    1112,   386,  1114,    -1,   373,   747,   953,  1119,   750,    -1,
    1122,    -1,  1112,    -1,  1114,    -1,   963,   386,    -1,  1119,
     747,    -1,  1122,   750,    -1,    -1,    -1,    -1,  1112,    -1,
    1114,  1035,    -1,    -1,    -1,  1119,    -1,    -1,  1122,  1151,
      -1,    -1,    -1,    -1,    -1,  1157,    -1,    -1,    -1,  1161,
    1162,  1151,    -1,    -1,    -1,    -1,    -1,  1157,    -1,    -1,
      -1,  1161,  1162,  1010,    -1,    -1,    -1,  1151,  1180,  1016,
     215,    -1,  1184,  1157,  1186,    -1,    -1,  1161,  1162,    -1,
    1180,    -1,    -1,    -1,  1184,    -1,  1186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1207,  1180,  1207,    -1,    -1,
    1184,   843,  1186,    -1,    -1,   847,    -1,  1207,    -1,    -1,
      -1,    -1,   854,   498,    -1,   500,   843,  1064,    -1,    -1,
     847,    -1,    -1,  1207,    -1,   270,  1130,   854,    -1,   498,
     275,   500,    -1,    -1,     2,    32,    -1,    28,    -1,    -1,
    1252,  1253,  1254,  1255,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1252,  1253,  1254,  1255,    -1,    48,    26,    -1,
      -1,    52,    -1,    -1,    -1,  1112,    -1,  1114,  1252,  1253,
    1254,  1255,  1119,    -1,    -1,  1122,    -1,    -1,    -1,    -1,
      -1,    -1,   924,    -1,   926,    -1,    77,   929,    -1,    80,
      -1,    82,    -1,    -1,    91,    -1,    -1,   924,    95,   926,
      -1,    92,   929,    -1,  1151,   350,    -1,   576,    76,     5,
    1157,   953,    80,    81,  1161,  1162,    -1,    -1,    -1,   364,
     111,   963,    -1,    -1,    -1,    -1,   953,    23,   373,    -1,
      -1,    27,    28,  1180,   102,    -1,   963,  1184,   106,  1186,
      36,   386,    38,    39,    -1,    -1,   143,   144,    44,   146,
     147,   148,    -1,   150,   151,    51,    -1,    -1,   155,    -1,
    1207,    -1,    -1,    -1,    -1,   162,    68,    -1,  1010,    -1,
      -1,   168,   169,   170,  1016,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,  1010,    80,    -1,    88,    -1,    84,  1016,
      -1,    87,    -1,    -1,    90,    -1,    92,    -1,    -1,    -1,
      -1,   103,    -1,    -1,    -1,  1252,  1253,  1254,  1255,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,   117,  1064,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1064,    -1,     2,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   215,    -1,    -1,
      -1,    -1,    -1,   498,    -1,   500,    -1,    -1,    -1,    -1,
      -1,    -1,   747,    26,    -1,   750,    -1,    -1,    -1,    -1,
    1112,   167,  1114,    -1,    -1,    -1,    -1,  1119,   747,    -1,
    1122,   750,    -1,    -1,    -1,  1112,    -1,  1114,    -1,    -1,
      -1,    -1,  1119,    -1,    -1,  1122,    -1,    -1,    -1,    -1,
      -1,    -1,   270,    -1,    -1,    -1,    -1,   275,    -1,  1151,
      -1,    -1,    -1,    76,    -1,  1157,    -1,    -1,    -1,  1161,
    1162,    -1,    -1,    -1,  1151,    -1,    -1,   229,    -1,    -1,
    1157,   576,    -1,    -1,  1161,  1162,    -1,    -1,  1180,   102,
      -1,    -1,  1184,   106,  1186,    -1,   229,    -1,    -1,    -1,
      -1,    -1,    -1,  1180,    -1,    -1,    -1,  1184,   843,  1186,
      -1,    -1,   847,    -1,   266,  1207,    -1,    -1,    -1,   854,
      -1,    -1,    -1,    -1,   843,    -1,    -1,    -1,   847,    -1,
    1207,    -1,   350,   266,    -1,   854,    -1,    -1,    -1,   291,
      -1,    -1,    -1,    -1,    -1,    -1,   364,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   373,    -1,    -1,   291,    -1,
    1252,  1253,  1254,  1255,    -1,    -1,    -1,    -1,   386,    -1,
      -1,    -1,    -1,    -1,    -1,  1252,  1253,  1254,  1255,   331,
      -1,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   924,
      -1,   926,    -1,    -1,   929,    -1,    -1,    -1,   331,    23,
      -1,    -1,   215,    27,    28,   924,    -1,   926,    -1,    -1,
     929,    -1,    36,    -1,    38,    39,    -1,    -1,   953,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    51,   963,    -1,
      -1,    -1,    -1,    -1,   953,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   963,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   747,    -1,    78,   750,    80,   270,    82,    -1,
      84,    -1,   275,    87,    -1,    -1,    90,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,  1010,    -1,    -1,    -1,    -1,
     498,  1016,   500,    -1,    -1,    -1,    -1,   111,    -1,    -1,
     114,  1010,    -1,   117,    -1,    -1,    -1,  1016,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   438,    -1,    -1,    -1,    -1,
     462,   463,    -1,    -1,    -1,    -1,   468,    -1,    -1,   471,
     472,   473,   474,    -1,    -1,    -1,    -1,    -1,    -1,  1064,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   350,    -1,    -1,
     473,   474,    -1,   167,    -1,  1064,    -1,    -1,   843,   173,
      -1,   364,   847,    -1,    -1,   507,    -1,    -1,   576,   854,
     373,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   386,   507,    -1,    -1,  1112,    -1,  1114,
      -1,    -1,    -1,    -1,  1119,    -1,    -1,  1122,    -1,    -1,
      -1,    -1,    -1,  1112,    -1,  1114,    -1,    -1,    -1,   551,
    1119,    -1,    -1,  1122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   563,   564,    -1,    -1,    -1,  1151,    -1,   551,    -1,
     572,    -1,  1157,    -1,    -1,    -1,  1161,  1162,    -1,   924,
      -1,   926,  1151,    -1,   929,    -1,    -1,    -1,  1157,   572,
      -1,    -1,  1161,  1162,    -1,  1180,    -1,    -1,    -1,  1184,
      -1,  1186,    -1,     1,    -1,     3,    -1,    -1,   953,    -1,
      -1,  1180,    -1,    -1,    -1,  1184,    -1,  1186,   963,    -1,
      18,    19,    -1,    21,    -1,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,   498,    34,   500,  1207,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1010,    64,  1252,  1253,  1254,
    1255,  1016,    -1,    71,    72,    -1,    -1,    -1,    -1,   747,
      -1,    79,   750,  1252,  1253,  1254,  1255,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,   101,    -1,   103,    -1,   105,    -1,    -1,
     108,    -1,     3,   576,   112,    -1,    -1,   115,    -1,  1064,
     118,    -1,    -1,    -1,   726,    -1,    -1,    18,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,   741,
      -1,   743,    -1,   726,    -1,    36,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    46,    -1,    -1,    -1,    -1,
     743,   763,    53,    -1,    55,    -1,    -1,  1112,    -1,  1114,
      -1,    -1,    -1,    64,  1119,   843,   174,  1122,    70,   847,
      -1,    72,    -1,    -1,    -1,    -1,   854,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1151,    -1,    -1,    -1,
     101,    -1,  1157,    -1,   105,    -1,  1161,  1162,    -1,   111,
      -1,   112,    -1,    -1,   807,   808,    -1,   118,     3,    -1,
      -1,    -1,    -1,    -1,    -1,  1180,    -1,    -1,    -1,  1184,
      -1,  1186,    -1,    18,    19,    -1,    21,    -1,    -1,    -1,
      -1,    26,    -1,    -1,    -1,    -1,   924,   859,   926,    34,
      -1,   929,  1207,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,   859,    -1,    53,    -1,
      55,    -1,    -1,    -1,   747,   953,    -1,   750,    -1,    64,
      -1,    -1,    -1,    -1,    -1,   963,    71,    72,   900,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,  1252,  1253,  1254,
    1255,    -1,    -1,    -1,    89,    -1,    -1,   900,    -1,    -1,
      -1,    -1,    32,    98,    99,    -1,   101,    -1,   103,   221,
     105,    -1,    -1,   108,    -1,    -1,    -1,   112,   113,    -1,
     115,    -1,  1010,   118,    -1,    -1,    -1,    -1,  1016,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   965,    -1,    -1,    -1,    -1,    -1,   261,
     262,    -1,    -1,    -1,    -1,    -1,    -1,   269,    -1,    -1,
     843,    91,   965,    -1,   847,    95,   278,    -1,   280,    -1,
      -1,   854,    -1,    -1,    -1,   287,  1064,     5,    -1,   174,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,
      38,    39,    -1,   143,   144,   327,    44,   147,   148,    -1,
     150,   151,    -1,    51,  1112,   155,  1114,    -1,    -1,    -1,
      -1,  1119,   162,    -1,  1122,    -1,    -1,    -1,   168,   169,
     170,   924,    -1,   926,    -1,    -1,   929,    91,  1070,  1071,
      78,    95,    80,    -1,    82,    -1,    84,    -1,    -1,    87,
      -1,    -1,    90,  1151,    92,    -1,    -1,  1070,  1071,  1157,
     953,    -1,    -1,  1161,  1162,    -1,    -1,    -1,    -1,    -1,
     963,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,
      -1,    -1,  1180,    -1,    -1,    -1,  1184,    -1,  1186,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,    -1,
      -1,   155,   156,   157,    -1,   159,   160,   161,   162,  1207,
      -1,   165,    -1,    -1,   168,   169,   170,  1010,    -1,    -1,
      -1,    -1,    -1,  1016,    -1,   179,    -1,    -1,    14,   167,
      -1,    -1,    18,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,  1252,  1253,  1254,  1255,    -1,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    54,    -1,
      56,  1064,    58,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    -1,    -1,    -1,
      76,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,    88,    -1,    90,    -1,    92,    -1,    -1,    95,
      96,    -1,    98,    -1,    -1,    -1,   102,   103,   104,  1112,
      -1,  1114,    -1,    -1,    -1,    -1,  1119,    -1,    -1,  1122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     5,   131,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1151,    -1,
      -1,    -1,    23,    -1,  1157,    -1,    27,    28,  1161,  1162,
      -1,    -1,   594,   595,    -1,    36,    -1,    38,    39,    -1,
     602,   603,    -1,    44,    -1,    -1,    -1,  1180,   174,    -1,
      51,  1184,    -1,  1186,    -1,    -1,    -1,    -1,     1,    -1,
      -1,    -1,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   197,    -1,    -1,  1207,   201,   202,    78,    -1,    80,
      23,    82,    -1,    84,    27,    28,    87,    -1,    -1,    90,
      -1,    92,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    44,    -1,   229,    -1,    -1,    -1,    -1,    51,    -1,
     111,    54,    -1,   114,    -1,    -1,   117,    -1,    -1,  1252,
    1253,  1254,  1255,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    78,    -1,    80,    -1,    82,
     266,    84,    85,    -1,    87,    -1,    -1,    90,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   291,   167,    -1,   111,    -1,
      -1,   114,   173,    -1,   117,    -1,   302,   303,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   751,
      -1,   317,   318,    -1,    23,    -1,    -1,    -1,    27,    28,
      -1,    -1,    -1,    -1,    -1,   331,    -1,    36,    -1,    38,
      39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,   167,   787,    -1,   353,    -1,    -1,
     173,    -1,    -1,    -1,    -1,   797,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   374,    78,
     376,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   385,
      -1,    90,    -1,    92,   390,   391,   392,    -1,    -1,    -1,
      -1,   397,    -1,   835,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,
      -1,   853,    -1,    -1,    -1,   857,    -1,    -1,    -1,    -1,
     426,   427,   428,   429,   430,   431,    -1,   433,   434,   435,
     436,   437,    -1,   439,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
      -1,    -1,    -1,    -1,    -1,    -1,   462,   463,    -1,    -1,
      -1,    -1,   468,    -1,    -1,   471,   472,   473,   474,   475,
     476,    -1,    -1,    -1,   480,   481,    -1,   483,    -1,    -1,
      -1,    -1,    -1,   489,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   934,    -1,   500,    -1,    -1,   503,    -1,    -1,
      -1,   507,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   517,    -1,   519,    -1,    -1,    -1,   523,    -1,   961,
     526,   527,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      -1,   537,    -1,    -1,    -1,    -1,   542,    -1,    -1,    18,
      19,    -1,    21,    -1,    -1,   551,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,   560,    34,    -1,   563,   564,    -1,
      -1,    40,   568,   569,    -1,    -1,   572,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,   583,   584,    -1,
      -1,  1023,  1024,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,   597,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,   619,    -1,    -1,    -1,    -1,    -1,    98,
      99,    -1,   101,    -1,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,   112,   113,    -1,   115,    -1,  1080,   118,
      -1,    -1,    -1,    -1,  1086,  1087,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,    -1,    -1,   663,    -1,    -1,
      -1,    -1,  1104,  1105,   670,  1107,    -1,    -1,    -1,    18,
      19,    -1,    21,    -1,    -1,    -1,  1118,    26,    -1,  1121,
      -1,    -1,  1124,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,   174,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,  1159,    -1,   725,
     726,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,   741,    -1,   743,   744,   745,
      89,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    98,
      99,    -1,   101,    -1,   103,    -1,   105,   763,    -1,   108,
      -1,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,   118,
      -1,   777,    -1,    -1,    -1,   781,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1228,    -1,  1230,    -1,
      -1,  1233,  1234,    -1,    -1,    -1,    -1,    -1,    -1,   805,
      -1,    -1,    -1,   809,    91,   811,   812,   813,    95,   815,
      -1,   817,    -1,    -1,   820,   821,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,   174,    -1,    -1,   834,    -1,
     836,    -1,    -1,    -1,   840,    -1,    -1,    -1,    -1,   845,
     846,   847,    -1,   849,   850,    -1,    -1,    -1,   854,   855,
      -1,    -1,    -1,   859,    -1,    -1,   143,   144,   864,   146,
     147,   148,    -1,   150,   151,    -1,    -1,    -1,   155,    -1,
      -1,    -1,    -1,    -1,   161,   162,    -1,    -1,   165,    -1,
      -1,   168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,
     896,   897,   898,   899,   900,    -1,    -1,   903,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   917,   918,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,   927,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    19,    -1,    21,    -1,   944,    -1,
      -1,    26,   948,    -1,    -1,    -1,    -1,    -1,   954,    34,
      -1,    -1,    -1,    -1,    -1,    40,   962,    -1,   964,   965,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
     986,    -1,    -1,    -1,    -1,    -1,    71,    72,   994,    -1,
     996,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
    1006,    -1,  1008,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,  1021,   101,     5,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,   118,  1040,    23,    -1,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,  1052,    -1,    36,    -1,
      38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    51,  1070,  1071,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1082,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1090,  1091,  1092,  1093,  1094,   174,
      78,    -1,    80,    -1,    82,    -1,    84,    -1,    -1,    87,
      -1,    -1,    90,    -1,    92,    -1,  1112,    -1,  1114,    -1,
      -1,  1117,    -1,  1119,    -1,    -1,  1122,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1144,  1145,
      -1,  1147,  1148,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,    -1,
      -1,    -1,    -1,    -1,  1170,    -1,    -1,    -1,    -1,  1175,
      -1,  1177,    -1,    -1,    -1,  1181,    -1,    -1,    -1,   167,
    1186,  1187,    -1,    -1,  1190,    -1,    -1,    -1,   171,    -1,
      -1,   174,  1198,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1212,  1213,  1214,  1215,
    1216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1252,  1253,  1254,  1255,
    1256,  1257,  1258,  1259,     0,     1,    -1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      16,    17,    18,    19,    -1,    21,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    -1,    63,    64,    65,
      66,    67,    68,    69,    -1,    71,    72,    73,    -1,    75,
      -1,    -1,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    94,    -1,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,   105,
     106,    -1,   108,   109,   110,   111,   112,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,
     146,   147,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,   164,   165,
     166,   167,    -1,    -1,   170,    -1,   172,    -1,   174,    -1,
     176,    -1,     1,   179,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    13,    -1,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,
      79,    80,    81,    -1,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,   105,   106,    -1,   108,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,   118,
      -1,    -1,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,    -1,   172,   173,   174,    -1,   176,    -1,     1,
     179,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    16,    17,    18,    19,    -1,    21,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      -1,    63,    64,    65,    66,    67,    68,    69,    -1,    71,
      72,    73,    -1,    75,    -1,    -1,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,   105,   106,    -1,   108,   109,   110,   111,
     112,    -1,   114,   115,   116,   117,   118,    -1,    -1,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
     172,   173,   174,    -1,   176,    -1,     1,   179,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    16,    17,    18,    19,    -1,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    66,    67,    68,    69,    -1,    71,    72,    73,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
     105,   106,    -1,   108,   109,   110,   111,   112,    -1,   114,
     115,   116,   117,   118,    -1,    -1,   121,    -1,   123,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,
     145,   146,   147,    -1,    -1,    -1,    -1,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,   164,
     165,   166,   167,    -1,    -1,   170,    -1,   172,   173,   174,
      -1,   176,    -1,     1,   179,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    55,    -1,    57,
      58,    59,    60,    61,    -1,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    -1,    92,    93,    94,    -1,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,   105,   106,    -1,
     108,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
     118,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,   147,
      -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,    -1,   164,   165,   166,   167,
      -1,    -1,   170,    -1,   172,   173,   174,    -1,   176,    -1,
       1,   179,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    16,    17,    18,    19,    -1,
      21,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    55,    -1,    57,    58,    59,    60,
      61,    -1,    63,    64,    65,    66,    67,    68,    69,    -1,
      71,    72,    73,    -1,    75,    -1,    -1,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      -1,    92,    93,    94,    -1,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,   105,   106,    -1,   108,   109,   110,
     111,   112,    -1,   114,   115,   116,   117,   118,    -1,    -1,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   144,   145,   146,   147,    -1,    -1,    -1,
      -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,
      -1,   172,   173,   174,    -1,   176,    -1,     1,   179,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    17,    18,    19,    -1,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    55,    -1,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    69,    -1,    71,    72,    73,
      -1,    75,    -1,    -1,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    -1,    92,    93,
      94,    -1,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,   105,   106,    -1,   108,   109,   110,   111,   112,    -1,
     114,   115,   116,   117,   118,    -1,    -1,   121,    -1,   123,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,   145,   146,   147,    -1,    -1,    -1,    -1,   152,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,
     164,   165,   166,   167,    -1,    -1,   170,    -1,   172,    -1,
     174,    -1,   176,    -1,     1,   179,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    16,
      17,    18,    19,    -1,    21,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    55,    -1,
      57,    58,    59,    60,    61,    -1,    63,    64,    65,    66,
      67,    68,    69,    -1,    71,    72,    73,    -1,    75,    -1,
      -1,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    -1,    92,    93,    94,    -1,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,   105,   106,
      -1,   108,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,   118,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,
     147,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,   172,    -1,   174,    -1,   176,
      -1,     1,   179,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    16,    17,    18,    19,
      -1,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,    69,
      -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,   105,   106,    -1,   108,   109,
     110,   111,   112,    -1,   114,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,   144,   145,   146,   147,    -1,    -1,
      -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,
     170,    -1,   172,    -1,   174,    -1,   176,    -1,     1,   179,
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
     103,    -1,   105,   106,    -1,   108,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,   118,    -1,    -1,   121,    -1,
     123,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
      -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,   172,
      -1,   174,    -1,   176,    -1,     1,   179,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      16,    17,    18,    19,    -1,    21,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    55,
      -1,    57,    58,    59,    60,    61,    -1,    63,    64,    65,
      66,    67,    68,    69,    -1,    71,    72,    73,    -1,    75,
      -1,    -1,    78,    79,    80,    81,    -1,    83,    84,    85,
      86,    87,    88,    89,    90,    -1,    92,    93,    94,    -1,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,   105,
     106,    -1,   108,   109,   110,   111,   112,    -1,   114,   115,
     116,   117,   118,    -1,    -1,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,
     146,   147,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,   164,   165,
     166,   167,    -1,    -1,   170,    -1,   172,    -1,   174,    -1,
     176,    -1,     1,   179,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    16,    17,    18,
      19,    -1,    21,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    -1,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      69,    -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,
      79,    80,    81,    -1,    83,    84,    85,    86,    87,    88,
      89,    90,    -1,    92,    93,    94,    -1,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,   105,   106,    -1,   108,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,   118,
      -1,    -1,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,   144,   145,   146,   147,    -1,
      -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,    -1,   164,   165,   166,   167,    -1,
      -1,   170,    -1,   172,    -1,   174,    -1,   176,    -1,     1,
     179,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    16,    17,    18,    19,    -1,    21,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    55,    -1,    57,    58,    59,    60,    61,
      -1,    63,    64,    65,    66,    67,    68,    69,    -1,    71,
      72,    73,    -1,    75,    -1,    -1,    78,    79,    80,    81,
      -1,    83,    84,    85,    86,    87,    88,    89,    90,    -1,
      92,    93,    94,    -1,    96,    97,    98,    99,   100,   101,
     102,   103,    -1,   105,   106,    -1,   108,   109,   110,   111,
     112,    -1,   114,   115,   116,   117,   118,    -1,    -1,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
     172,    -1,   174,    -1,   176,    -1,     1,   179,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    16,    17,    18,    19,    -1,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    66,    67,    68,    69,    -1,    71,    72,    73,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
     105,   106,    -1,   108,   109,   110,   111,   112,    -1,   114,
     115,   116,   117,   118,    -1,    -1,   121,    -1,   123,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,
     145,   146,   147,    -1,    -1,    -1,    -1,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,   164,
     165,   166,   167,    -1,    -1,   170,    -1,   172,    -1,   174,
       1,   176,     3,     4,   179,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    43,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   144,   145,   146,   147,    -1,    -1,    -1,
      -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,   170,
      -1,   172,    -1,   174,    -1,   176,     3,     4,   179,     6,
       7,     8,     9,    10,    11,    -1,    13,    -1,    15,    -1,
      -1,    18,    19,    20,    21,    -1,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    56,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,   135,   136,
     137,   138,    -1,   140,   141,    -1,   143,   144,   145,   146,
     147,   148,    -1,   150,    -1,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,    -1,   174,    -1,    -1,
       3,     4,   179,     6,     7,     8,     9,    10,    11,    -1,
      13,    -1,    15,    -1,    -1,    18,    19,    20,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    56,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,   130,   131,   132,
      -1,    -1,   135,   136,   137,   138,    -1,   140,   141,    -1,
     143,   144,   145,   146,   147,   148,    -1,   150,    -1,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,   168,   169,   170,   171,   172,
      -1,   174,    -1,    -1,    -1,    -1,   179,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      16,    -1,    18,    19,    -1,    21,    -1,    23,    24,    -1,
      26,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    57,    -1,    -1,    60,    61,    -1,    63,    64,    65,
      66,    -1,    68,    69,    -1,    71,    72,    73,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,   105,
     106,    -1,   108,    -1,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,
     146,   147,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,   164,   165,
     166,    -1,    -1,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,     3,     4,   179,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    -1,    26,    27,    28,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,
      -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
     112,    -1,    -1,   115,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,    -1,   164,   165,   166,    -1,    -1,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,     3,     4,   179,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    43,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,
      -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,
      68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,    -1,   105,    -1,    -1,
     108,    -1,    -1,    -1,   112,    -1,   114,   115,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,   147,
      -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,    -1,   164,   165,   166,    -1,
      -1,    -1,   170,    -1,   172,    -1,   174,    -1,   176,     3,
       4,   179,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,
      64,    -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    -1,    -1,    88,    89,    90,    91,    -1,    -1,
      -1,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
      -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,   145,   146,   147,    -1,   149,    -1,    -1,   152,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,
     164,   165,   166,   167,    -1,    -1,   170,    -1,   172,   173,
     174,    -1,    -1,     3,     4,   179,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,
      -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,
      60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,   103,    -1,   105,    -1,    -1,   108,   109,
     110,    -1,   112,   113,    -1,   115,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,   144,   145,   146,   147,    -1,    -1,
      -1,    -1,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,
     170,    -1,   172,    -1,   174,    -1,    -1,     3,     4,   179,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    43,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,
      66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,
     146,   147,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,   164,   165,
     166,    -1,    -1,    -1,   170,    -1,   172,    -1,   174,    -1,
     176,     3,     4,   179,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    -1,    26,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    45,    46,    -1,    48,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,
      -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,
      -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
     112,    -1,    -1,   115,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,    -1,   164,   165,   166,    -1,    -1,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,     3,     4,   179,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,
      -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,
      68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,    -1,   105,    -1,    -1,
     108,   109,   110,    -1,   112,   113,    -1,   115,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,   147,
      -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,    -1,   164,   165,   166,    -1,
      -1,    -1,   170,    -1,   172,    -1,   174,   175,    -1,     3,
       4,   179,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,
      64,    -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
      -1,   105,    -1,    -1,   108,   109,   110,    -1,   112,   113,
      -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,   145,   146,   147,    -1,    -1,    -1,    -1,   152,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,
     164,   165,   166,    -1,    -1,    -1,   170,    -1,   172,    -1,
     174,   175,    -1,     3,     4,   179,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,
      -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,
      60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,   103,    -1,   105,    -1,    -1,   108,   109,
     110,    -1,   112,    -1,    -1,   115,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,   144,   145,   146,   147,    -1,    -1,
      -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,
     170,    -1,   172,    -1,   174,    -1,    -1,     3,     4,   179,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,
      66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,    -1,   105,
      -1,    -1,   108,   109,   110,    -1,   112,    -1,    -1,   115,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,
     146,   147,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,   164,   165,
     166,    -1,    -1,    -1,   170,    -1,   172,    -1,   174,   175,
      -1,     3,     4,   179,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,
      -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,
      -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,    -1,   105,    -1,    -1,   108,   109,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,    -1,   164,   165,   166,    -1,    -1,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,    -1,   179,     3,     4,
      -1,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,
      -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,
      -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,   103,    -1,
     105,    -1,    -1,   108,   109,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,
     145,   146,   147,    -1,    -1,    -1,    -1,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,   164,
     165,   166,    -1,    -1,    -1,   170,    -1,   172,    -1,   174,
     175,    -1,     3,     4,   179,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    70,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,    -1,   108,   109,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   144,   145,   146,   147,    -1,    -1,    -1,
      -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,     3,     4,   179,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,
     147,    -1,   149,    -1,    -1,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,   172,   173,   174,    -1,    -1,
       3,     4,   179,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,   144,   145,   146,   147,    -1,   149,    -1,    -1,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
      -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,   172,
     173,   174,    -1,    -1,     3,     4,   179,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,   144,   145,   146,   147,    -1,
     149,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,    -1,   164,   165,   166,   167,    -1,
      -1,   170,    -1,   172,   173,   174,    -1,    -1,     3,     4,
     179,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,
      -1,    26,    -1,    28,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,
      -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,
      -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,    -1,
     115,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,
     145,   146,   147,    -1,    -1,    -1,    -1,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,   164,
     165,   166,    -1,    -1,    -1,   170,    -1,   172,    -1,   174,
      -1,    -1,     3,     4,   179,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,    -1,   108,   109,   110,
      -1,   112,    -1,    -1,   115,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   144,   145,   146,   147,    -1,    -1,    -1,
      -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,     3,     4,   179,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    91,    -1,    -1,    -1,    95,    -1,
      -1,    98,    99,   100,   101,   102,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,
     147,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,    -1,   164,   165,   166,
      -1,    -1,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
       3,     4,   179,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
      -1,   164,   165,   166,    -1,    -1,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,    -1,   179,     3,     4,    -1,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,
      66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,   102,   103,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,
     146,   147,    -1,   149,    -1,    -1,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,   164,   165,
     166,    -1,    -1,    -1,   170,    -1,   172,    -1,   174,   175,
      -1,     3,     4,   179,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,
      -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,
      -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,    -1,   164,   165,   166,    -1,    -1,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,     3,     4,   179,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,
      -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,
      68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,    -1,   105,    -1,    -1,
     108,    -1,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,   147,
      -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,    -1,   164,   165,   166,    -1,
      -1,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,     3,
       4,   179,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,
      64,    -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
      -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,   145,   146,   147,    -1,    -1,    -1,    -1,   152,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,
     164,   165,   166,    -1,    -1,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,    -1,   179,     3,     4,    -1,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,
     147,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,    -1,   164,   165,   166,
      -1,    -1,    -1,   170,    -1,   172,   173,   174,    -1,    -1,
       3,     4,   179,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,
     113,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
      -1,   164,   165,   166,    -1,    -1,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,     3,     4,   179,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,   144,   145,   146,   147,    -1,
      -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,    -1,   164,   165,   166,    -1,    -1,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,    -1,
     179,     3,     4,    -1,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,
      -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,
      -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,    -1,   164,   165,   166,    -1,    -1,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,    -1,   179,     3,     4,
      -1,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,
      -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,
      -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,   112,   113,    -1,
     115,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,
     145,   146,   147,    -1,    -1,    -1,    -1,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,   164,
     165,   166,    -1,    -1,    -1,   170,    -1,   172,    -1,   174,
      -1,    -1,     3,     4,   179,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   144,   145,   146,   147,    -1,    -1,    -1,
      -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,     3,     4,   179,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,   144,   145,   146,
     147,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,    -1,   164,   165,   166,
      -1,    -1,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
       3,     4,   179,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
      -1,   164,   165,   166,    -1,    -1,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,     3,     4,   179,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,   144,   145,   146,   147,    -1,
      -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,    -1,   164,   165,   166,    -1,    -1,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,     3,     4,
     179,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,    64,
      -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,    -1,
      -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,   100,   101,   102,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,
     145,   146,   147,    -1,    -1,    -1,    -1,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,   164,
     165,   166,    -1,    -1,    -1,   170,    -1,   172,    -1,   174,
      -1,    -1,     3,     4,   179,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   144,   145,   146,   147,    -1,    -1,    -1,
      -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,     3,     4,   179,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    32,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    -1,   143,   144,   145,   146,
     147,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,    -1,   164,   165,   166,
      -1,    -1,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
      -1,     4,   179,     6,     7,     8,     9,    10,    11,    -1,
     143,   144,    15,   146,   147,   148,    19,   150,   151,   152,
      23,   154,   155,   156,   157,    28,   159,   160,   161,   162,
     163,    -1,   165,    36,    -1,   168,   169,   170,    41,    42,
      43,    -1,    45,    -1,    -1,    48,   179,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    -1,    -1,    66,    -1,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    77,    -1,    79,    80,    81,    82,
      83,    -1,    -1,    -1,    -1,    88,    -1,    90,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    98,    -1,   100,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,
     143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
      -1,   164,   165,   166,    -1,    -1,    -1,   170,    -1,   172,
      -1,   174,   175,   176,    -1,     4,   179,     6,     7,     8,
       9,    10,    11,    -1,   143,   144,    15,   146,   147,   148,
      19,   150,   151,   152,    23,    -1,   155,    -1,    -1,    28,
      -1,    -1,   161,   162,    -1,    -1,   165,    36,    -1,   168,
     169,   170,    41,    42,    43,    -1,    45,    -1,    -1,    48,
     179,    -1,    -1,    52,    -1,    -1,    -1,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    -1,    -1,    66,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,    88,
      -1,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    98,
      -1,   100,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,    -1,    -1,   143,   144,   145,   146,   147,    -1,
      -1,    13,    -1,   152,    -1,    -1,    18,    19,    20,    21,
      -1,    -1,   161,    -1,    26,   164,   165,   166,    -1,    -1,
      -1,   170,    34,   172,    36,   174,    -1,   176,    40,    -1,
     179,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,   101,
      -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,    -1,    -1,   135,   136,   137,   138,    -1,   140,   141,
      -1,   143,   144,   145,   146,   147,   148,    -1,   150,    -1,
      -1,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,   165,    -1,    -1,   168,   169,   170,   171,
       3,     4,   174,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      43,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,   144,   145,   146,   147,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
      -1,    -1,   165,   166,    -1,    -1,    -1,   170,    -1,   172,
      -1,   174,    -1,   176,     3,     4,    -1,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    43,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,   118,
      -1,    -1,     3,     4,    -1,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,    -1,    23,    -1,   143,    26,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    43,    -1,    45,    46,    -1,   166,    -1,    50,
      -1,    -1,    53,   172,    55,   174,    57,   176,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,     3,   105,    -1,    -1,   108,    -1,    -1,
      -1,   112,    -1,    13,   115,    -1,    -1,   118,    18,    19,
      20,    21,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,   145,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    72,   174,    -1,   176,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
      -1,   101,    -1,   103,    -1,   105,    -1,    -1,    -1,    -1,
      -1,   111,   112,    -1,    -1,   115,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,   131,   132,    -1,    -1,   135,   136,   137,   138,    -1,
     140,   141,    -1,   143,   144,   145,   146,   147,   148,    -1,
     150,    -1,    -1,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     3,   165,    -1,    -1,   168,   169,
     170,   171,   172,    -1,    13,    -1,    -1,    -1,    -1,    18,
      19,    20,    21,    -1,    -1,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    13,    -1,    -1,    98,
      99,    -1,   101,    20,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,   112,    -1,    32,   115,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,
     129,   130,   131,   132,    -1,    -1,   135,   136,   137,   138,
      -1,   140,   141,    -1,   143,   144,   145,   146,   147,   148,
      -1,   150,    -1,    -1,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,   165,    -1,    -1,   168,
     169,   170,   171,    -1,    91,    -1,    13,    -1,    95,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,   135,   136,
     137,   138,    -1,   140,   141,    -1,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,   168,   169,   170,    91,   172,    13,    -1,    95,    -1,
      -1,    -1,   179,    20,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,   135,   136,
     137,   138,    -1,   140,   141,    -1,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,   168,   169,   170,    91,   172,    13,    -1,    95,    -1,
      -1,    -1,   179,    20,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,   135,   136,
     137,   138,    -1,   140,   141,    -1,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,   168,   169,   170,    91,   172,    13,    -1,    95,    -1,
      -1,    -1,   179,    20,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,   135,   136,
     137,   138,    -1,   140,   141,    -1,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,   168,   169,   170,    91,   172,    13,    -1,    95,    -1,
      -1,    -1,   179,    20,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,   135,   136,
     137,   138,    -1,   140,   141,    -1,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,   168,   169,   170,    91,   172,    -1,    -1,    95,    -1,
      -1,    13,   179,    -1,    -1,    -1,    -1,   104,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,   132,    48,    -1,   135,   136,
     137,   138,    -1,   140,   141,    -1,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,   168,   169,   170,    -1,   172,    -1,    13,    -1,    91,
      -1,    -1,   179,    95,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    48,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    91,   168,   169,   170,    95,
     172,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    32,    -1,
     126,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      74,    -1,   168,   169,   170,    -1,   172,    -1,    -1,    -1,
      -1,    -1,    -1,   179,    -1,    -1,    -1,    91,    -1,    13,
      -1,    95,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,   167,   168,   169,   170,    91,    -1,    13,
      -1,    95,    -1,    -1,    -1,   179,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,   168,   169,   170,    91,   172,    13,
      -1,    95,    -1,    -1,    -1,   179,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,   168,   169,   170,    91,   172,    13,
      -1,    95,    -1,    -1,    -1,   179,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,   168,   169,   170,    91,   172,    13,
      -1,    95,    -1,    -1,    -1,   179,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,   126,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,    32,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,   168,   169,   170,    91,    -1,    -1,
      -1,    95,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    32,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,   104,   146,   147,   148,    -1,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,   126,   168,   169,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    32,    -1,    -1,
     143,   144,    91,   146,   147,   148,    95,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    13,    -1,   168,   169,   170,    -1,   172,
      20,    -1,    -1,   122,    -1,    -1,   179,   126,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    -1,    -1,   143,   144,    91,   146,   147,   148,
      95,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,   168,
     169,   170,    -1,    -1,    74,    -1,    -1,    -1,   177,    -1,
     179,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      -1,    91,    -1,    -1,    -1,    95,    20,    -1,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,    32,   154,
     155,   156,   157,    -1,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,   168,   169,   170,   126,    -1,    -1,    -1,
      13,    14,    -1,    -1,   179,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,   143,   144,    -1,   146,   147,   148,    32,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    91,   168,   169,
     170,    95,    -1,    -1,    -1,    -1,    13,    -1,    -1,   179,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,   122,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,   126,   168,   169,   170,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    91,   179,    -1,    -1,    95,    -1,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    13,    -1,   168,   169,   170,    -1,   126,
      20,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,   168,   169,   170,    -1,   172,    -1,    -1,    -1,    -1,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    13,    -1,    95,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    -1,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,    32,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,   167,   168,   169,
     170,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,   179,
      13,    14,    -1,    -1,   104,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,   126,   168,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,   179,
      -1,    -1,    95,    -1,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    13,    -1,   168,
     169,   170,    -1,   126,    20,    -1,   175,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    13,    -1,   168,   169,   170,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,   168,   169,   170,    -1,   126,    -1,    -1,    -1,
      -1,    91,    -1,   179,    13,    95,    -1,    -1,    -1,    -1,
      -1,    20,    -1,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,    32,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   126,    -1,   168,   169,
     170,    -1,    -1,    -1,    13,    14,    -1,    -1,    -1,   179,
      -1,    20,    -1,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,    32,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,   168,   169,
     170,    -1,    91,    -1,    -1,   175,    95,    -1,    -1,   179,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   126,    -1,   168,
     169,   170,    -1,    -1,    -1,    -1,   175,    -1,    91,    -1,
     179,    -1,    95,    -1,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    13,    14,   168,
     169,   170,    -1,   126,    20,    -1,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,   168,   169,   170,    -1,    -1,
      -1,    13,   175,    -1,    -1,    -1,   179,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,   144,    91,
     146,   147,   148,    95,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      13,    -1,   168,   169,   170,    -1,    -1,    20,    -1,    -1,
     122,    -1,    -1,   179,   126,    -1,    -1,    -1,    -1,    32,
      91,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,   126,   168,   169,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,
      -1,    -1,   143,   144,    -1,   146,   147,   148,    91,   150,
     151,   152,    95,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    13,   167,   168,   169,   170,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,   179,    -1,
      -1,    -1,   125,   126,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
      -1,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,   168,   169,   170,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    13,    -1,    91,    -1,    -1,    -1,    95,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    13,    14,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,   143,   144,    -1,   146,   147,
     148,    32,   150,   151,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    91,
     168,   169,   170,    95,    -1,    -1,    -1,    -1,    13,   177,
      -1,   179,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,    -1,   146,   147,   148,    62,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,   126,   168,   169,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,   179,    -1,    -1,
      95,    -1,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,    -1,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    13,    -1,   168,   169,   170,
      -1,   126,    20,    -1,    -1,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,    -1,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,   168,   169,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    -1,    91,    -1,    -1,    -1,    95,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,   143,   144,    -1,   146,   147,
     148,    32,   150,   151,   152,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    91,
     168,   169,   170,    95,    -1,    -1,    -1,    13,    -1,    -1,
      -1,   179,   104,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,   143,   144,   104,   146,   147,   148,    -1,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,   126,   168,   169,   170,    -1,
      -1,    -1,    -1,    13,    -1,    91,    -1,   179,    -1,    95,
      20,    -1,   143,   144,    -1,   146,   147,   148,   104,   150,
     151,   152,    32,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,   168,   169,   170,
     126,    -1,    -1,    -1,    13,    -1,    -1,    -1,   179,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,   143,   144,    -1,
     146,   147,   148,    32,   150,   151,   152,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    91,   168,   169,   170,    95,    -1,    -1,    -1,    13,
      -1,    -1,    -1,   179,   104,    -1,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,   126,   168,   169,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,   179,
      -1,    95,    -1,    -1,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,    -1,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,   168,
     169,   170,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,    32,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    23,   168,   169,   170,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,   179,    36,    -1,    38,    39,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    91,    -1,
      -1,    -1,    95,    -1,    -1,    75,    -1,    -1,    78,    -1,
      80,    -1,    -1,    -1,    84,    85,    86,    87,    -1,    -1,
      90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,   117,    -1,    -1,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
      -1,   154,   155,   156,   157,    -1,   159,   160,   161,   162,
     163,    -1,   165,    -1,    -1,   168,   169,   170,     1,    -1,
       3,    -1,     5,    -1,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    19,   167,    21,    -1,
      -1,    -1,   172,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,     3,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    18,    19,    -1,
      21,    64,    -1,    -1,    -1,    26,    -1,    28,    71,    72,
      -1,    -1,    -1,    34,    -1,    -1,    79,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    89,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    98,    99,    -1,   101,    -1,
     103,    -1,   105,    64,    -1,   108,    -1,    -1,    -1,   112,
      71,    72,   115,    -1,    -1,   118,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
       3,    92,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,
     101,    -1,   103,    -1,   105,    18,    19,   108,    21,    -1,
      -1,   112,    -1,    26,   115,    -1,   117,   118,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,     3,    -1,     5,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      -1,    18,    19,    -1,    21,    -1,    79,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    98,    99,    -1,   101,    46,
     103,    -1,   105,    50,    -1,   108,    53,    -1,    55,   112,
     113,    -1,   115,    -1,    -1,   118,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,   101,    -1,   103,    -1,   105,    18,
      19,   108,    21,    -1,    -1,   112,    -1,    26,   115,    -1,
      -1,   118,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,     3,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    18,    19,    -1,    21,    64,    -1,    -1,    -1,    26,
      -1,    -1,    71,    72,    -1,    -1,    -1,    34,    -1,    -1,
      79,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      89,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    98,
      99,    -1,   101,    -1,   103,    -1,   105,    64,    -1,   108,
      -1,    -1,    -1,   112,    71,    72,   115,    -1,    -1,   118,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,   101,    -1,   103,    -1,   105,    18,
      19,   108,    21,    -1,    -1,   112,    -1,    26,   115,    -1,
      -1,   118,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,    -1,   101,    -1,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,   118
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   184,   185,     0,     1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    15,    16,    17,    18,    19,    21,
      23,    24,    26,    27,    28,    29,    30,    31,    34,    36,
      38,    39,    40,    44,    45,    46,    49,    50,    51,    53,
      55,    57,    58,    59,    60,    61,    63,    64,    65,    66,
      68,    69,    71,    72,    73,    78,    79,    80,    81,    82,
      83,    84,    87,    88,    89,    90,    92,    93,    94,    96,
      97,    98,    99,   100,   101,   102,   103,   105,   106,   108,
     109,   110,   111,   112,   114,   115,   117,   118,   123,   126,
     143,   144,   145,   146,   147,   152,   161,   164,   165,   166,
     167,   170,   172,   174,   179,   186,   187,   188,   189,   190,
     191,   194,   195,   196,   197,   202,   203,   204,   206,   207,
     208,   213,   214,   218,   219,   223,   225,   228,   229,   232,
     233,   234,   235,   236,   237,   240,   241,   242,   246,   248,
     251,   252,   253,   254,   255,   259,   260,   265,   266,   267,
     268,   271,   272,   277,   278,   280,   281,   286,   290,   291,
     293,   294,   316,   321,   322,   326,   327,   348,   349,   350,
     352,   354,   355,   356,   364,   365,   366,   367,   368,   369,
     371,   374,   375,   376,   377,   378,   379,   380,   381,   383,
     384,   385,   386,   387,   167,    23,    36,    45,    55,    57,
      90,   103,   172,   241,   251,   278,   348,   355,   365,   366,
     371,   374,   376,   377,   122,   357,   358,     3,    55,   220,
     371,   357,   111,   328,    92,   220,   188,   342,   371,   174,
       3,    18,    19,    21,    26,    34,    40,    46,    50,    53,
      55,    64,    71,    72,    79,    89,    98,    99,   101,   103,
     105,   108,   112,   115,   118,   221,   222,   224,    12,    28,
     117,   247,   371,    84,    87,   204,   174,   105,   222,   222,
     222,   174,   222,   329,   330,    33,   208,   227,   371,   263,
     264,   371,   371,    19,    79,    98,   115,   371,   371,   371,
       9,   174,   231,   230,    28,    34,    48,    50,    52,    77,
      80,    92,    99,   103,   111,   187,   226,   282,   283,   284,
     307,   308,   309,   334,   339,   371,   342,   109,   110,   167,
     286,   287,   370,   371,   373,   371,   227,   371,   371,   371,
     102,   174,   188,   371,   371,   190,   194,   208,   190,   208,
     371,   373,   371,   371,   371,   371,   371,     1,   173,   186,
     209,   342,   113,   153,   343,   344,   373,   371,    82,   188,
      23,    36,    39,    90,   172,   191,   192,   193,   204,   208,
     196,   151,   198,   172,    47,    86,   116,   205,    27,   327,
     371,     9,   267,   371,   372,    25,    33,    41,    42,    43,
     121,   176,   243,   244,   351,   353,    57,   148,   269,   222,
     172,   174,   302,    54,    75,    85,   311,    28,    77,    80,
      92,   111,   312,    28,    80,    92,   111,   310,   222,   323,
     324,     1,   329,   166,   167,   371,    13,    20,    32,    91,
      95,   126,   143,   144,   146,   147,   148,   150,   151,   152,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   168,   169,   170,   179,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   171,   298,   174,   176,    91,    95,   371,   222,    41,
     176,   243,    57,   174,   188,   167,   208,   326,   167,   149,
     167,   223,   345,   346,   347,   373,   222,   222,   104,   208,
     104,   127,   208,   298,   205,   345,   167,   174,   174,   208,
     188,   113,   174,   222,   331,   332,     1,   148,   338,    48,
     149,   188,   227,   149,   227,    14,   174,   174,   227,   345,
     232,   232,    48,    92,   308,   309,   175,   149,   113,   174,
     222,   306,   148,   167,   371,   371,   125,   288,   167,   172,
     227,   174,   345,   167,   256,   256,   167,   173,   173,   186,
     149,   173,   371,   149,   149,   175,   175,     9,   176,   243,
     244,     3,   174,   199,     1,   173,   209,   216,   217,   371,
     211,   371,    67,    37,    74,   167,   267,   269,   111,   238,
     291,   371,   188,    80,   247,   371,   124,   177,   245,   342,
     371,   382,   245,   371,   222,   342,   172,     1,   195,   222,
     273,   276,   175,   303,   305,   307,   312,    92,     1,   148,
     336,   337,    92,     1,     3,    13,    18,    20,    21,    26,
      46,    53,    55,    56,    64,    72,    89,   101,   105,   112,
     118,   143,   144,   145,   146,   147,   148,   150,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   165,   168,
     169,   170,   171,   174,   222,   295,   296,   297,   298,   348,
     127,   325,   149,   167,   167,   167,   371,   371,   371,   245,
     371,   245,   371,   371,   371,   371,   371,   371,   371,     3,
      21,    34,    64,   105,   111,   223,   371,   371,   371,   371,
     371,   371,   371,   371,   371,   371,   371,   371,   371,   371,
     371,   371,   371,    70,   373,   373,   373,   373,   373,   345,
     345,   245,   371,   245,   371,   104,   174,   342,   371,   222,
      28,    48,    92,   117,   359,   362,   363,   371,   387,   329,
     371,   127,   175,   149,   127,   127,   188,    35,   188,   371,
      35,   371,    67,   175,   346,   222,   249,   250,   332,   149,
     175,   226,   371,   127,   333,   371,   330,   263,   222,   326,
     371,   371,   175,   306,   312,   284,   332,   148,   340,   341,
     339,   289,   207,     1,   261,   347,   175,    22,   257,   173,
     175,   175,   343,   373,   175,   343,   342,   371,   198,   175,
     200,   201,   347,   173,   173,   149,   167,    14,   151,   149,
     167,    14,    37,    74,   222,   143,   144,   145,   146,   147,
     161,   165,   170,   210,   297,   298,   299,   371,   210,   212,
     269,   172,   239,   326,    48,   357,   121,   222,   227,   227,
     174,   371,   122,   177,   360,    48,   149,   177,   360,   125,
     149,   177,   227,   360,    33,    48,   227,   360,   167,   174,
       1,   270,   173,   276,   127,   149,   173,   149,   175,   338,
       1,   226,   334,   335,   107,   313,   174,   301,   371,   143,
     151,   301,   301,   371,   324,   174,   176,   167,   167,   167,
     167,   167,   167,   175,   177,   346,    48,   177,    33,    48,
     174,    48,    92,   149,   175,    18,    21,    26,    46,    53,
      64,    72,    89,   101,   112,   118,   348,    91,    91,   167,
     373,   347,   371,   371,    35,   188,    35,    35,   188,   104,
     208,   222,   175,   149,   175,   175,   331,   338,    70,   373,
     222,   175,   175,   341,   148,   285,   175,   339,   153,   300,
     333,   371,   173,    76,   119,   173,   262,   175,   174,   208,
     222,   258,    48,   177,    48,   149,   175,   216,   223,    18,
      19,    21,    26,    46,    50,    53,    72,    79,    89,    98,
      99,   101,   103,   112,   115,   118,   172,   215,   299,   371,
     113,   371,   210,   212,   149,   167,    14,   167,   172,   270,
     323,   329,   245,   371,   227,   371,    48,   342,   174,   188,
     177,   245,   371,   177,   188,   371,   177,   371,   177,   371,
     227,    45,   371,   245,   371,   227,   346,   173,    84,    87,
     173,   187,   195,   229,   371,   274,   275,   305,   313,    62,
     120,   317,   303,   304,   175,   296,   298,   175,   177,   175,
     245,   371,    45,   245,   371,   346,   362,   338,   348,   348,
     188,   188,   371,   188,    35,   167,   167,   250,   208,   333,
     174,   174,   300,   333,   334,   313,   341,   371,   188,   241,
     342,   258,   148,   208,   245,   371,   245,   371,   201,   210,
      14,    37,    74,    37,    74,   167,   167,   299,   371,   371,
     270,   173,   167,   167,   357,   357,   167,   371,   175,   361,
     362,   188,   177,   360,   177,   360,   188,   125,   371,    33,
     227,   360,    33,   227,   360,   175,   195,   229,   229,   276,
     195,   317,     3,    55,    94,   105,   318,   319,   320,   371,
     292,   175,   301,   301,   177,   177,   371,    33,    33,   175,
     333,    35,   188,   345,   345,   300,   333,    33,   227,   175,
     371,   177,   177,   173,   113,   371,   210,   212,   210,   212,
      14,   173,   227,   227,   227,   149,   175,    45,   188,   371,
     177,    45,   188,   371,   177,   371,   104,    45,   371,   227,
      45,   371,   227,   167,   229,   276,   279,   320,   120,   149,
     127,   154,   156,   157,   159,   160,    62,    33,   167,   207,
     314,   315,    45,    45,   104,    45,    45,   188,   175,   175,
     188,   208,   167,   167,   167,   167,   371,   362,   371,   188,
     371,   188,   371,   371,   371,   315,   371,   319,   320,   320,
     320,   320,   320,   320,   318,   186,   371,   371,   371,   371,
     166,   166,   104,   104,   104,   104,   104,   104,   104,   104,
     371,   371,   371,   371
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   183,   184,   185,   185,   186,   186,   187,   187,   188,
     188,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   191,   191,   192,   193,
     193,   194,   195,   195,   196,   197,   198,   198,   199,   199,
     199,   200,   200,   201,   202,   203,   203,   203,   204,   204,
     204,   205,   205,   206,   207,   207,   207,   208,   209,   209,
     210,   210,   210,   210,   210,   210,   211,   211,   211,   211,
     211,   211,   212,   212,   213,   213,   213,   213,   213,   213,
     213,   214,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   216,
     216,   216,   216,   216,   217,   217,   218,   219,   219,   219,
     219,   219,   219,   220,   220,   220,   221,   221,   221,   221,
     221,   222,   222,   222,   223,   223,   223,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   226,   226,
     226,   226,   227,   227,   228,   228,   229,   229,   230,   229,
     231,   229,   232,   232,   232,   232,   232,   232,   232,   233,
     233,   233,   233,   234,   235,   235,   236,   237,   237,   237,
     238,   237,   239,   237,   240,   241,   242,   242,   243,   243,
     243,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   245,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   247,   247,   248,
     248,   249,   249,   250,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   252,   252,   252,
     252,   252,   252,   253,   253,   253,   254,   254,   254,   255,
     255,   255,   255,   256,   256,   257,   257,   257,   258,   258,
     259,   260,   260,   261,   261,   262,   262,   262,   263,   263,
     263,   264,   264,   265,   266,   266,   267,   268,   268,   268,
     269,   269,   270,   270,   270,   270,   270,   271,   271,   272,
     273,   273,   274,   273,   273,   275,   273,   276,   276,   277,
     279,   278,   280,   281,   281,   281,   282,   282,   283,   283,
     284,   284,   284,   285,   285,   286,   288,   287,   289,   287,
     290,   292,   291,   293,   293,   293,   293,   293,   294,   295,
     295,   296,   296,   296,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   299,   299,   300,   300,   301,   301,   302,   302,   303,
     303,   304,   304,   305,   306,   306,   307,   307,   307,   307,
     307,   307,   308,   308,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   310,   310,   310,   310,   310,   310,   311,
     311,   311,   312,   312,   312,   312,   312,   312,   312,   313,
     313,   314,   314,   315,   315,   316,   317,   317,   317,   317,
     317,   318,   318,   319,   319,   319,   319,   319,   319,   319,
     320,   320,   320,   321,   322,   322,   323,   323,   324,   325,
     325,   326,   326,   326,   326,   326,   328,   327,   327,   327,
     329,   329,   330,   330,   331,   331,   331,   332,   332,   332,
     333,   333,   333,   334,   335,   335,   335,   336,   336,   337,
     337,   338,   338,   338,   338,   339,   339,   340,   341,   341,
     342,   342,   343,   343,   344,   344,   345,   345,   346,   346,
     347,   347,   348,   348,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   350,   351,   351,   351,
     351,   351,   351,   352,   353,   353,   353,   353,   353,   353,
     353,   354,   355,   356,   356,   356,   356,   356,   356,   356,
     357,   357,   358,   359,   359,   360,   361,   361,   362,   362,
     362,   363,   363,   363,   363,   363,   363,   364,   364,   364,
     364,   364,   365,   365,   365,   365,   365,   366,   367,   367,
     367,   367,   367,   367,   368,   369,   370,   370,   371,   371,
     371,   371,   371,   371,   371,   371,   371,   371,   371,   371,
     371,   371,   371,   371,   371,   372,   372,   373,   373,   373,
     374,   374,   374,   374,   375,   375,   375,   375,   375,   376,
     376,   376,   377,   377,   377,   377,   377,   377,   378,   378,
     378,   378,   379,   379,   380,   380,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   381,   381,   382,
     382,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   384,   384,   384,   384,   384,   384,
     384,   385,   385,   385,   385,   386,   386,   386,   386,   387,
     387,   387,   387,   387,   387,   387
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
       1,     1,     3,     3,     3,     5,     1,     3,     3,     3,
       5,     5,     0,     1,     4,     6,     8,     8,     6,     8,
       8,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     5,     1,     3,     3,     4,     4,     4,
       4,     4,     4,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     3,     1,     1,     0,     3,
       0,     3,     1,     1,     1,     1,     1,     1,     1,     3,
       5,     5,     2,     1,     1,     1,     1,     6,     7,     3,
       0,     6,     0,     6,     2,     0,     3,     2,     1,     1,
       1,     5,     3,     3,     6,     6,     4,     6,     5,     6,
       5,     6,     3,     4,     3,     4,     6,     7,     6,     7,
       4,     5,     4,     5,     4,     4,     3,     6,     5,     4,
       3,     6,     5,     6,     5,     8,     7,     4,     4,     6,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     4,     7,     5,     3,     6,     2,
       2,     3,     3,     0,     2,     2,     3,     5,     1,     3,
       3,     5,     5,     0,     2,     3,     2,     4,     4,     3,
       1,     1,     3,     3,     5,     5,     2,     1,     1,     1,
       0,     2,     0,     2,     3,     3,     4,     4,     4,     2,
       1,     2,     0,     4,     2,     0,     5,     1,     3,     1,
       0,     8,     0,     1,     1,     1,     0,     1,     1,     3,
       1,     3,     1,     0,     2,     6,     0,     3,     0,     4,
       1,     0,     7,     4,     4,     6,     6,     4,     2,     1,
       3,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     0,     3,     2,     3,     1,
       3,     0,     1,     1,     1,     1,     4,     5,     4,     5,
       6,     6,     0,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     0,     1,     1,     2,     1,     1,     1,     0,
       1,     1,     1,     1,     2,     1,     0,     2,     2,     4,
       4,     1,     3,     3,     3,     3,     3,     3,     3,     2,
       1,     1,     1,     3,     1,     2,     1,     3,     2,     0,
       2,     1,     2,     1,     1,     1,     0,     5,     3,     3,
       1,     3,     3,     5,     1,     1,     3,     1,     2,     3,
       0,     2,     2,     1,     1,     1,     1,     2,     1,     0,
       1,     0,     2,     2,     1,     1,     1,     2,     0,     1,
       1,     3,     1,     1,     3,     3,     0,     1,     1,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       4,     4,     4,     5,     2,     2,     1,     2,     1,     2,
       1,     2,     1,     2,     1,     1,     2,     6,     6,     4,
       9,     9,     7,     2,     2,     3,     4,     6,     6,     9,
       9,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     4,     1,     3,     4,     1,     3,     4,     3,
       3,     1,     1,     2,     1,     2,     1,     1,     2,     2,
       2,     2,     2,     8,     8,     9,     9,     4,     3,     3,
       2,     2,     2,     1,     3,     4,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     4,
       4,     4,     3,     3,     3,     3,     5,     5,     3,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     4,     4,     5,     4,     5,     3,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1
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
#line 636 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6006 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 640 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6012 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 641 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6018 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 648 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6026 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 656 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6034 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 660 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6043 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 670 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6051 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 675 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6059 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6065 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6071 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6077 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6083 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6089 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6095 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6101 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 689 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6107 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 690 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6113 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: TDEFER stmt  */
#line 692 "chpl.ypp"
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
#line 6135 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 709 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6141 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 710 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6147 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TINIT TTHIS TSEMI  */
#line 712 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr));
    auto node = Init::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6159 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 720 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6172 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 729 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6185 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TLABEL ident_def stmt  */
#line 738 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6193 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 742 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6204 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt_base: error TSEMI  */
#line 749 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6213 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: assignment_stmt  */
#line 756 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6219 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: if_stmt  */
#line 757 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6225 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: loop_stmt  */
#line 758 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6231 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: select_stmt  */
#line 759 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6237 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: manage_stmt  */
#line 760 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6243 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 761 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6249 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: throw_stmt  */
#line 762 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6255 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 764 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6261 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 766 "chpl.ypp"
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
#line 6285 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 786 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6297 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 794 "chpl.ypp"
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
#line 6318 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TLOCAL do_stmt  */
#line 811 "chpl.ypp"
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
#line 6338 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TON expr do_stmt  */
#line 827 "chpl.ypp"
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
#line 6359 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 844 "chpl.ypp"
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
#line 6380 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSERIAL do_stmt  */
#line 861 "chpl.ypp"
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
#line 6400 "bison-chpl-lib.cpp"
    break;

  case 45: /* tryable_stmt: TSYNC stmt  */
#line 877 "chpl.ypp"
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
#line 6422 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 902 "chpl.ypp"
  {
    auto attributeGroup = context->buildAttributeGroup((yyloc));
    if (attributeGroup != nullptr) {
      context->resetAttributeGroupPartsState();
    }
    context->loopAttributes.push_back(std::move(attributeGroup));
  }
#line 6434 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 927 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6442 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 934 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6451 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 941 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6459 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 945 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6470 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 955 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6476 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 957 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6484 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 961 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6492 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 968 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6501 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 973 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6511 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 986 "chpl.ypp"
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
#line 6533 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 1007 "chpl.ypp"
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
#line 6554 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 1024 "chpl.ypp"
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
#line 6575 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1041 "chpl.ypp"
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
#line 6594 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1058 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6600 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1059 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6607 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1061 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6614 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1066 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6620 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1067 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6627 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1073 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6637 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1091 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6646 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1096 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6655 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1101 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6668 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1113 "chpl.ypp"
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
#line 6687 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1131 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6693 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1132 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6699 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1137 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6707 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1141 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6715 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1145 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6724 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1150 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6732 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1154 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6740 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1158 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6749 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1168 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6758 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1173 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6768 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1179 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6779 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1186 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6788 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1191 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6799 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1198 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6811 "bison-chpl-lib.cpp"
    break;

  case 92: /* opt_only_ls: %empty  */
#line 1208 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6817 "bison-chpl-lib.cpp"
    break;

  case 93: /* opt_only_ls: renames_ls  */
#line 1209 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6823 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1214 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6832 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1219 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6844 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1227 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6857 "bison-chpl-lib.cpp"
    break;

  case 97: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1236 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6871 "bison-chpl-lib.cpp"
    break;

  case 98: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1246 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6883 "bison-chpl-lib.cpp"
    break;

  case 99: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1254 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6896 "bison-chpl-lib.cpp"
    break;

  case 100: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1263 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6910 "bison-chpl-lib.cpp"
    break;

  case 101: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1276 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6919 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_expr: expr  */
#line 1312 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6927 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_expr: expr TDOT all_op_name  */
#line 1316 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6936 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1321 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6945 "bison-chpl-lib.cpp"
    break;

  case 122: /* import_expr: expr TAS ident_use  */
#line 1326 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6955 "bison-chpl-lib.cpp"
    break;

  case 123: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1332 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6965 "bison-chpl-lib.cpp"
    break;

  case 124: /* import_ls: import_expr  */
#line 1340 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6971 "bison-chpl-lib.cpp"
    break;

  case 125: /* import_ls: import_ls TCOMMA import_expr  */
#line 1341 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6977 "bison-chpl-lib.cpp"
    break;

  case 126: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1346 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6987 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1355 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6995 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1359 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7003 "bison-chpl-lib.cpp"
    break;

  case 129: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1363 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7011 "bison-chpl-lib.cpp"
    break;

  case 130: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1367 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7019 "bison-chpl-lib.cpp"
    break;

  case 131: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1371 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7027 "bison-chpl-lib.cpp"
    break;

  case 132: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1375 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7035 "bison-chpl-lib.cpp"
    break;

  case 133: /* opt_label_ident: %empty  */
#line 1383 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7041 "bison-chpl-lib.cpp"
    break;

  case 134: /* opt_label_ident: TIDENT  */
#line 1384 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7047 "bison-chpl-lib.cpp"
    break;

  case 135: /* opt_label_ident: TINIT  */
#line 1385 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7053 "bison-chpl-lib.cpp"
    break;

  case 144: /* ident_use: TIDENT  */
#line 1411 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7059 "bison-chpl-lib.cpp"
    break;

  case 145: /* ident_use: TINIT  */
#line 1412 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7065 "bison-chpl-lib.cpp"
    break;

  case 146: /* ident_use: TTHIS  */
#line 1413 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7071 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TBOOL  */
#line 1440 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7077 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TENUM  */
#line 1441 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7083 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TINT  */
#line 1442 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7089 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TUINT  */
#line 1443 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7095 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TREAL  */
#line 1444 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7101 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TIMAG  */
#line 1445 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7107 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TCOMPLEX  */
#line 1446 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7113 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TBYTES  */
#line 1447 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7119 "bison-chpl-lib.cpp"
    break;

  case 174: /* scalar_type: TSTRING  */
#line 1448 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7125 "bison-chpl-lib.cpp"
    break;

  case 175: /* scalar_type: TLOCALE  */
#line 1449 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7131 "bison-chpl-lib.cpp"
    break;

  case 176: /* scalar_type: TNOTHING  */
#line 1450 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7137 "bison-chpl-lib.cpp"
    break;

  case 177: /* scalar_type: TVOID  */
#line 1451 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7143 "bison-chpl-lib.cpp"
    break;

  case 182: /* do_stmt: TDO stmt  */
#line 1465 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7149 "bison-chpl-lib.cpp"
    break;

  case 183: /* do_stmt: block_stmt  */
#line 1466 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7155 "bison-chpl-lib.cpp"
    break;

  case 184: /* return_stmt: TRETURN TSEMI  */
#line 1471 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7166 "bison-chpl-lib.cpp"
    break;

  case 185: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1478 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7177 "bison-chpl-lib.cpp"
    break;

  case 186: /* class_level_stmt: TSEMI  */
#line 1488 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7185 "bison-chpl-lib.cpp"
    break;

  case 187: /* class_level_stmt: inner_class_level_stmt  */
#line 1492 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7195 "bison-chpl-lib.cpp"
    break;

  case 188: /* $@1: %empty  */
#line 1497 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7202 "bison-chpl-lib.cpp"
    break;

  case 189: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1499 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7211 "bison-chpl-lib.cpp"
    break;

  case 190: /* $@2: %empty  */
#line 1503 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7218 "bison-chpl-lib.cpp"
    break;

  case 191: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1505 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7227 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1523 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7236 "bison-chpl-lib.cpp"
    break;

  case 200: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1528 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7245 "bison-chpl-lib.cpp"
    break;

  case 201: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1533 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7254 "bison-chpl-lib.cpp"
    break;

  case 202: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1538 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7262 "bison-chpl-lib.cpp"
    break;

  case 203: /* forwarding_decl_start: TFORWARDING  */
#line 1545 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7271 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_or_export: TEXTERN  */
#line 1552 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7277 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_or_export: TEXPORT  */
#line 1553 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7283 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1558 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7293 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1568 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7303 "bison-chpl-lib.cpp"
    break;

  case 208: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1575 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7315 "bison-chpl-lib.cpp"
    break;

  case 209: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1583 "chpl.ypp"
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
#line 7337 "bison-chpl-lib.cpp"
    break;

  case 210: /* $@3: %empty  */
#line 1601 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7345 "bison-chpl-lib.cpp"
    break;

  case 211: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1605 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7354 "bison-chpl-lib.cpp"
    break;

  case 212: /* $@4: %empty  */
#line 1610 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7363 "bison-chpl-lib.cpp"
    break;

  case 213: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1615 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7372 "bison-chpl-lib.cpp"
    break;

  case 214: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1623 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7380 "bison-chpl-lib.cpp"
    break;

  case 215: /* no_loop_attributes: %empty  */
#line 1628 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7386 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1631 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7392 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1632 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7398 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1648 "chpl.ypp"
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
#line 7425 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1671 "chpl.ypp"
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
#line 7448 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1690 "chpl.ypp"
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
#line 7471 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1709 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7480 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1714 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7489 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1719 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7498 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1724 "chpl.ypp"
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
#line 7527 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1749 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7536 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: for_loop_kw expr TIN expr forall_intent_clause do_stmt  */
#line 1754 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7545 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1759 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7554 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1764 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7563 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1769 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7572 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: for_loop_kw expr forall_intent_clause do_stmt  */
#line 1774 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7581 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1779 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7590 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: for_loop_kw zippered_iterator forall_intent_clause do_stmt  */
#line 1784 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7599 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1789 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7608 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt_base: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1794 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7617 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1799 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7626 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1804 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7635 "bison-chpl-lib.cpp"
    break;

  case 240: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1809 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7644 "bison-chpl-lib.cpp"
    break;

  case 241: /* loop_stmt_base: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1814 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7653 "bison-chpl-lib.cpp"
    break;

  case 242: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1819 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7663 "bison-chpl-lib.cpp"
    break;

  case 243: /* loop_stmt_base: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1825 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7673 "bison-chpl-lib.cpp"
    break;

  case 244: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1834 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7683 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr TTHEN stmt  */
#line 1843 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7691 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr block_stmt  */
#line 1847 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7699 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1851 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7707 "bison-chpl-lib.cpp"
    break;

  case 248: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1855 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7715 "bison-chpl-lib.cpp"
    break;

  case 249: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1859 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7723 "bison-chpl-lib.cpp"
    break;

  case 250: /* if_stmt: TIF ifvar block_stmt  */
#line 1863 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7731 "bison-chpl-lib.cpp"
    break;

  case 251: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1867 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7739 "bison-chpl-lib.cpp"
    break;

  case 252: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1871 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7747 "bison-chpl-lib.cpp"
    break;

  case 253: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1875 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7756 "bison-chpl-lib.cpp"
    break;

  case 254: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1880 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7765 "bison-chpl-lib.cpp"
    break;

  case 255: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1885 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7774 "bison-chpl-lib.cpp"
    break;

  case 256: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1890 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7783 "bison-chpl-lib.cpp"
    break;

  case 257: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1898 "chpl.ypp"
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
#line 7802 "bison-chpl-lib.cpp"
    break;

  case 258: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1913 "chpl.ypp"
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
#line 7821 "bison-chpl-lib.cpp"
    break;

  case 259: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1931 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7829 "bison-chpl-lib.cpp"
    break;

  case 260: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1935 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7837 "bison-chpl-lib.cpp"
    break;

  case 261: /* ifc_formal_ls: ifc_formal  */
#line 1941 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7843 "bison-chpl-lib.cpp"
    break;

  case 262: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1942 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7849 "bison-chpl-lib.cpp"
    break;

  case 263: /* ifc_formal: ident_def  */
#line 1947 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7855 "bison-chpl-lib.cpp"
    break;

  case 283: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1991 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7863 "bison-chpl-lib.cpp"
    break;

  case 284: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1995 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7871 "bison-chpl-lib.cpp"
    break;

  case 285: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1999 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7879 "bison-chpl-lib.cpp"
    break;

  case 286: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2006 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7887 "bison-chpl-lib.cpp"
    break;

  case 287: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 2010 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7895 "bison-chpl-lib.cpp"
    break;

  case 288: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2014 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7903 "bison-chpl-lib.cpp"
    break;

  case 289: /* try_stmt: TTRY tryable_stmt  */
#line 2021 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7911 "bison-chpl-lib.cpp"
    break;

  case 290: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2025 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7919 "bison-chpl-lib.cpp"
    break;

  case 291: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2029 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7927 "bison-chpl-lib.cpp"
    break;

  case 292: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2033 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7935 "bison-chpl-lib.cpp"
    break;

  case 293: /* catch_expr_ls: %empty  */
#line 2039 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7941 "bison-chpl-lib.cpp"
    break;

  case 294: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2040 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7947 "bison-chpl-lib.cpp"
    break;

  case 295: /* catch_expr: TCATCH block_stmt  */
#line 2045 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7955 "bison-chpl-lib.cpp"
    break;

  case 296: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2049 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7963 "bison-chpl-lib.cpp"
    break;

  case 297: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2053 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7971 "bison-chpl-lib.cpp"
    break;

  case 298: /* catch_expr_inner: ident_def  */
#line 2060 "chpl.ypp"
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
#line 7988 "bison-chpl-lib.cpp"
    break;

  case 299: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2073 "chpl.ypp"
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
#line 8005 "bison-chpl-lib.cpp"
    break;

  case 300: /* throw_stmt: TTHROW expr TSEMI  */
#line 2089 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8015 "bison-chpl-lib.cpp"
    break;

  case 301: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2097 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 8024 "bison-chpl-lib.cpp"
    break;

  case 302: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2102 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 8035 "bison-chpl-lib.cpp"
    break;

  case 303: /* when_stmt_ls: %empty  */
#line 2111 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8041 "bison-chpl-lib.cpp"
    break;

  case 304: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2112 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8047 "bison-chpl-lib.cpp"
    break;

  case 305: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2117 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8055 "bison-chpl-lib.cpp"
    break;

  case 306: /* when_stmt: TOTHERWISE stmt  */
#line 2121 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 8064 "bison-chpl-lib.cpp"
    break;

  case 307: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2126 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 8074 "bison-chpl-lib.cpp"
    break;

  case 308: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2135 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8083 "bison-chpl-lib.cpp"
    break;

  case 309: /* manager_expr: expr TAS ident_def  */
#line 2140 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8091 "bison-chpl-lib.cpp"
    break;

  case 310: /* manager_expr: expr  */
#line 2144 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8099 "bison-chpl-lib.cpp"
    break;

  case 311: /* manager_expr_ls: manager_expr  */
#line 2150 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8105 "bison-chpl-lib.cpp"
    break;

  case 312: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2151 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8111 "bison-chpl-lib.cpp"
    break;

  case 313: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2156 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8120 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2166 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8130 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2172 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8142 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_start: class_tag ident_def  */
#line 2184 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8150 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_tag: TCLASS  */
#line 2190 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8156 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_tag: TRECORD  */
#line 2191 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8162 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_tag: TUNION  */
#line 2192 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8168 "bison-chpl-lib.cpp"
    break;

  case 320: /* opt_inherit: %empty  */
#line 2196 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8174 "bison-chpl-lib.cpp"
    break;

  case 321: /* opt_inherit: TCOLON expr_ls  */
#line 2197 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8180 "bison-chpl-lib.cpp"
    break;

  case 322: /* class_level_stmt_ls: %empty  */
#line 2201 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8189 "bison-chpl-lib.cpp"
    break;

  case 323: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2206 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8197 "bison-chpl-lib.cpp"
    break;

  case 324: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2210 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8205 "bison-chpl-lib.cpp"
    break;

  case 325: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2214 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8213 "bison-chpl-lib.cpp"
    break;

  case 326: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2218 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8221 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
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
#line 8244 "bison-chpl-lib.cpp"
    break;

  case 328: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
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
#line 8258 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_header: TENUM ident_def  */
#line 2258 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8266 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_ls: enum_item  */
#line 2265 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8275 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_ls: enum_ls TCOMMA  */
#line 2270 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8285 "bison-chpl-lib.cpp"
    break;

  case 332: /* $@5: %empty  */
#line 2276 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8294 "bison-chpl-lib.cpp"
    break;

  case 333: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2281 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8303 "bison-chpl-lib.cpp"
    break;

  case 334: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2286 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8312 "bison-chpl-lib.cpp"
    break;

  case 335: /* $@6: %empty  */
#line 2291 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8321 "bison-chpl-lib.cpp"
    break;

  case 336: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2296 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8330 "bison-chpl-lib.cpp"
    break;

  case 337: /* enum_item: ident_def  */
#line 2306 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8341 "bison-chpl-lib.cpp"
    break;

  case 338: /* enum_item: ident_def TASSIGN expr  */
#line 2313 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8354 "bison-chpl-lib.cpp"
    break;

  case 339: /* lambda_decl_start: TLAMBDA  */
#line 2325 "chpl.ypp"
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
#line 8370 "bison-chpl-lib.cpp"
    break;

  case 340: /* $@7: %empty  */
#line 2342 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8380 "bison-chpl-lib.cpp"
    break;

  case 341: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2348 "chpl.ypp"
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
#line 8402 "bison-chpl-lib.cpp"
    break;

  case 343: /* linkage_spec: linkage_spec_empty  */
#line 2371 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8408 "bison-chpl-lib.cpp"
    break;

  case 344: /* linkage_spec: TINLINE  */
#line 2372 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8415 "bison-chpl-lib.cpp"
    break;

  case 345: /* linkage_spec: TOVERRIDE  */
#line 2374 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8422 "bison-chpl-lib.cpp"
    break;

  case 346: /* opt_fn_type_formal_ls: %empty  */
#line 2379 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8428 "bison-chpl-lib.cpp"
    break;

  case 347: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2380 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8434 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_type_formal_ls: fn_type_formal  */
#line 2384 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8440 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2385 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8446 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_type_formal: named_formal  */
#line 2390 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8452 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2393 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8458 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_type_formal: formal_type  */
#line 2395 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8464 "bison-chpl-lib.cpp"
    break;

  case 353: /* opt_fn_type_ret_type: %empty  */
#line 2399 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8470 "bison-chpl-lib.cpp"
    break;

  case 354: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2400 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8476 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2413 "chpl.ypp"
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
#line 8495 "bison-chpl-lib.cpp"
    break;

  case 356: /* $@8: %empty  */
#line 2431 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8505 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2437 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8518 "bison-chpl-lib.cpp"
    break;

  case 358: /* $@9: %empty  */
#line 2446 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8528 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2452 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8541 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2464 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8549 "bison-chpl-lib.cpp"
    break;

  case 361: /* $@10: %empty  */
#line 2473 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8557 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2477 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-6].functionParts);
    fp.returnIntent = (yyvsp[-5].returnTag);
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
          ((yyvsp[-5].returnTag) != Function::DEFAULT_RETURN_INTENT ? (yylsp[-5]) : (yylsp[-6]))));
    context->exitScopeForFunctionDecl(fp);
    (yyval.functionParts) = fp;
  }
#line 8579 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2498 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8591 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2506 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8603 "bison-chpl-lib.cpp"
    break;

  case 365: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2514 "chpl.ypp"
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
#line 8618 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2525 "chpl.ypp"
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
#line 8633 "bison-chpl-lib.cpp"
    break;

  case 367: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2536 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8643 "bison-chpl-lib.cpp"
    break;

  case 368: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2545 "chpl.ypp"
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
#line 8659 "bison-chpl-lib.cpp"
    break;

  case 370: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2560 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8665 "bison-chpl-lib.cpp"
    break;

  case 373: /* fn_ident: ident_def TBANG  */
#line 2567 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8675 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2624 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8681 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2625 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8687 "bison-chpl-lib.cpp"
    break;

  case 415: /* opt_formal_ls: %empty  */
#line 2629 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8693 "bison-chpl-lib.cpp"
    break;

  case 416: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2630 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8699 "bison-chpl-lib.cpp"
    break;

  case 417: /* req_formal_ls: TLP TRP  */
#line 2634 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8705 "bison-chpl-lib.cpp"
    break;

  case 418: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2635 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8711 "bison-chpl-lib.cpp"
    break;

  case 419: /* formal_ls_inner: formal  */
#line 2639 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8717 "bison-chpl-lib.cpp"
    break;

  case 420: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2640 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8723 "bison-chpl-lib.cpp"
    break;

  case 421: /* formal_ls: %empty  */
#line 2644 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8729 "bison-chpl-lib.cpp"
    break;

  case 422: /* formal_ls: formal_ls_inner  */
#line 2645 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8735 "bison-chpl-lib.cpp"
    break;

  case 426: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2659 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8743 "bison-chpl-lib.cpp"
    break;

  case 427: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2664 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8751 "bison-chpl-lib.cpp"
    break;

  case 428: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2669 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8759 "bison-chpl-lib.cpp"
    break;

  case 429: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2674 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8767 "bison-chpl-lib.cpp"
    break;

  case 430: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2679 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8775 "bison-chpl-lib.cpp"
    break;

  case 431: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2684 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8783 "bison-chpl-lib.cpp"
    break;

  case 432: /* opt_formal_intent_tag: %empty  */
#line 2690 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8792 "bison-chpl-lib.cpp"
    break;

  case 433: /* opt_formal_intent_tag: required_intent_tag  */
#line 2695 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8801 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TIN  */
#line 2702 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8807 "bison-chpl-lib.cpp"
    break;

  case 435: /* required_intent_tag: TINOUT  */
#line 2703 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8813 "bison-chpl-lib.cpp"
    break;

  case 436: /* required_intent_tag: TOUT  */
#line 2704 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8819 "bison-chpl-lib.cpp"
    break;

  case 437: /* required_intent_tag: TCONST TIN  */
#line 2705 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8825 "bison-chpl-lib.cpp"
    break;

  case 438: /* required_intent_tag: TCONST TREF  */
#line 2706 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 8831 "bison-chpl-lib.cpp"
    break;

  case 439: /* required_intent_tag: TCONST  */
#line 2707 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 8837 "bison-chpl-lib.cpp"
    break;

  case 440: /* required_intent_tag: TPARAM  */
#line 2708 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 8843 "bison-chpl-lib.cpp"
    break;

  case 441: /* required_intent_tag: TREF  */
#line 2709 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 8849 "bison-chpl-lib.cpp"
    break;

  case 442: /* required_intent_tag: TTYPE  */
#line 2710 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 8855 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_this_intent_tag: %empty  */
#line 2714 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8861 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_this_intent_tag: TPARAM  */
#line 2715 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 8867 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_this_intent_tag: TREF  */
#line 2716 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 8873 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_this_intent_tag: TCONST TREF  */
#line 2717 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 8879 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_this_intent_tag: TCONST  */
#line 2718 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 8885 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_this_intent_tag: TTYPE  */
#line 2719 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 8891 "bison-chpl-lib.cpp"
    break;

  case 449: /* proc_iter_or_op: TPROC  */
#line 2723 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8897 "bison-chpl-lib.cpp"
    break;

  case 450: /* proc_iter_or_op: TITER  */
#line 2724 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8903 "bison-chpl-lib.cpp"
    break;

  case 451: /* proc_iter_or_op: TOPERATOR  */
#line 2725 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8909 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: %empty  */
#line 2729 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 8915 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_ret_tag: TOUT  */
#line 2730 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 8921 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: TCONST  */
#line 2731 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 8927 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: TCONST TREF  */
#line 2732 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 8933 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: TREF  */
#line 2733 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 8939 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_ret_tag: TPARAM  */
#line 2734 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 8945 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_ret_tag: TTYPE  */
#line 2735 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 8951 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_throws_error: %empty  */
#line 2739 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8957 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_throws_error: TTHROWS  */
#line 2740 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8963 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_function_body_stmt: TSEMI  */
#line 2743 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8969 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_function_body_stmt: function_body_stmt  */
#line 2744 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8975 "bison-chpl-lib.cpp"
    break;

  case 463: /* function_body_stmt: block_stmt_body  */
#line 2748 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8981 "bison-chpl-lib.cpp"
    break;

  case 464: /* function_body_stmt: TDO toplevel_stmt  */
#line 2749 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 8987 "bison-chpl-lib.cpp"
    break;

  case 465: /* query_expr: TQUERIEDIDENT  */
#line 2753 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8993 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_lifetime_where: %empty  */
#line 2758 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8999 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_lifetime_where: TWHERE expr  */
#line 2760 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9005 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2762 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9011 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2764 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9017 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2766 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9023 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_components_expr: lifetime_expr  */
#line 2771 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9029 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2773 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9035 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2778 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9041 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2780 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9047 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2782 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9053 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2784 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9059 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2786 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9065 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2788 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9071 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2790 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9077 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_ident: TIDENT  */
#line 2794 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9083 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_ident: TINIT  */
#line 2795 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9089 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_ident: TTHIS  */
#line 2796 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9095 "bison-chpl-lib.cpp"
    break;

  case 483: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2801 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9104 "bison-chpl-lib.cpp"
    break;

  case 484: /* type_alias_decl_stmt_start: TTYPE  */
#line 2810 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9112 "bison-chpl-lib.cpp"
    break;

  case 485: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2814 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9121 "bison-chpl-lib.cpp"
    break;

  case 486: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2822 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9129 "bison-chpl-lib.cpp"
    break;

  case 487: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2826 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9137 "bison-chpl-lib.cpp"
    break;

  case 488: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2833 "chpl.ypp"
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
#line 9160 "bison-chpl-lib.cpp"
    break;

  case 489: /* opt_init_type: %empty  */
#line 2854 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9166 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_init_type: TASSIGN expr  */
#line 2856 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9172 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_type: TPARAM  */
#line 2860 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9178 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_type: TCONST TREF  */
#line 2861 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9184 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_type: TREF  */
#line 2862 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9190 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_type: TCONST  */
#line 2863 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9196 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_type: TVAR  */
#line 2864 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9202 "bison-chpl-lib.cpp"
    break;

  case 496: /* $@11: %empty  */
#line 2869 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9211 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2873 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9220 "bison-chpl-lib.cpp"
    break;

  case 498: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2878 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9229 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt: var_decl_type error TSEMI  */
#line 2883 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    context->resetDeclStateOnError();
  }
#line 9238 "bison-chpl-lib.cpp"
    break;

  case 500: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2891 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9246 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2895 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9254 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2902 "chpl.ypp"
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
#line 9277 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2921 "chpl.ypp"
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
#line 9297 "bison-chpl-lib.cpp"
    break;

  case 504: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2940 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9305 "bison-chpl-lib.cpp"
    break;

  case 505: /* tuple_var_decl_component: ident_def  */
#line 2944 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9313 "bison-chpl-lib.cpp"
    break;

  case 506: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2948 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9321 "bison-chpl-lib.cpp"
    break;

  case 507: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2955 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9327 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2957 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9333 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2959 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9339 "bison-chpl-lib.cpp"
    break;

  case 510: /* opt_init_expr: %empty  */
#line 2965 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9345 "bison-chpl-lib.cpp"
    break;

  case 511: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2966 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9351 "bison-chpl-lib.cpp"
    break;

  case 512: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2967 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9357 "bison-chpl-lib.cpp"
    break;

  case 513: /* formal_or_ret_type_expr: expr  */
#line 2971 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9363 "bison-chpl-lib.cpp"
    break;

  case 514: /* ret_type: formal_or_ret_type_expr  */
#line 2975 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9369 "bison-chpl-lib.cpp"
    break;

  case 515: /* ret_type: reserved_type_ident_use  */
#line 2976 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9375 "bison-chpl-lib.cpp"
    break;

  case 516: /* ret_type: error  */
#line 2977 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9381 "bison-chpl-lib.cpp"
    break;

  case 517: /* colon_ret_type: TCOLON ret_type  */
#line 2981 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9387 "bison-chpl-lib.cpp"
    break;

  case 518: /* colon_ret_type: error  */
#line 2982 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9393 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_ret_type: %empty  */
#line 2986 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9399 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_type: %empty  */
#line 2991 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9405 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_type: TCOLON expr  */
#line 2992 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9411 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2993 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9417 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_type: error  */
#line 2994 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9423 "bison-chpl-lib.cpp"
    break;

  case 525: /* formal_type: formal_or_ret_type_expr  */
#line 2998 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9429 "bison-chpl-lib.cpp"
    break;

  case 526: /* formal_type: reserved_type_ident_use  */
#line 2999 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9435 "bison-chpl-lib.cpp"
    break;

  case 527: /* colon_formal_type: TCOLON formal_type  */
#line 3003 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9441 "bison-chpl-lib.cpp"
    break;

  case 528: /* opt_colon_formal_type: %empty  */
#line 3007 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9447 "bison-chpl-lib.cpp"
    break;

  case 529: /* opt_colon_formal_type: colon_formal_type  */
#line 3008 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9453 "bison-chpl-lib.cpp"
    break;

  case 530: /* expr_ls: expr  */
#line 3014 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9459 "bison-chpl-lib.cpp"
    break;

  case 531: /* expr_ls: expr_ls TCOMMA expr  */
#line 3015 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9465 "bison-chpl-lib.cpp"
    break;

  case 532: /* tuple_component: TUNDERSCORE  */
#line 3019 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9471 "bison-chpl-lib.cpp"
    break;

  case 533: /* tuple_component: opt_try_expr  */
#line 3020 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9477 "bison-chpl-lib.cpp"
    break;

  case 534: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3025 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9485 "bison-chpl-lib.cpp"
    break;

  case 535: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3029 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9493 "bison-chpl-lib.cpp"
    break;

  case 536: /* opt_actual_ls: %empty  */
#line 3035 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9499 "bison-chpl-lib.cpp"
    break;

  case 537: /* opt_actual_ls: actual_ls  */
#line 3036 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9505 "bison-chpl-lib.cpp"
    break;

  case 538: /* actual_ls: actual_expr  */
#line 3041 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9514 "bison-chpl-lib.cpp"
    break;

  case 539: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3046 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9524 "bison-chpl-lib.cpp"
    break;

  case 540: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3054 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9530 "bison-chpl-lib.cpp"
    break;

  case 541: /* actual_expr: opt_try_expr  */
#line 3055 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9536 "bison-chpl-lib.cpp"
    break;

  case 542: /* ident_expr: ident_use  */
#line 3059 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9542 "bison-chpl-lib.cpp"
    break;

  case 543: /* ident_expr: scalar_type  */
#line 3060 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9548 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TSINGLE expr  */
#line 3073 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9554 "bison-chpl-lib.cpp"
    break;

  case 550: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3075 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9560 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3077 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9566 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3079 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9572 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3081 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9582 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TATOMIC expr  */
#line 3087 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9588 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TSYNC expr  */
#line 3089 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9594 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TOWNED  */
#line 3092 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9600 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TOWNED expr  */
#line 3094 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9606 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TUNMANAGED  */
#line 3096 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9612 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3098 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9618 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TSHARED  */
#line 3100 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9624 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TSHARED expr  */
#line 3102 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9630 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TBORROWED  */
#line 3104 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9636 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TBORROWED expr  */
#line 3106 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9642 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TCLASS  */
#line 3108 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9648 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TRECORD  */
#line 3110 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9654 "bison-chpl-lib.cpp"
    break;

  case 566: /* for_expr: no_loop_attributes for_expr_base  */
#line 3114 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9660 "bison-chpl-lib.cpp"
    break;

  case 567: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3119 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9668 "bison-chpl-lib.cpp"
    break;

  case 568: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3123 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9676 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3127 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9684 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3131 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9697 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3140 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9710 "bison-chpl-lib.cpp"
    break;

  case 572: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3149 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9723 "bison-chpl-lib.cpp"
    break;

  case 573: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3160 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9729 "bison-chpl-lib.cpp"
    break;

  case 574: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3165 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9737 "bison-chpl-lib.cpp"
    break;

  case 575: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3169 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9745 "bison-chpl-lib.cpp"
    break;

  case 576: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3173 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9753 "bison-chpl-lib.cpp"
    break;

  case 577: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3177 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9761 "bison-chpl-lib.cpp"
    break;

  case 578: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3181 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9769 "bison-chpl-lib.cpp"
    break;

  case 579: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3185 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9777 "bison-chpl-lib.cpp"
    break;

  case 580: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3189 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9785 "bison-chpl-lib.cpp"
    break;

  case 581: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3196 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9799 "bison-chpl-lib.cpp"
    break;

  case 582: /* nil_expr: TNIL  */
#line 3213 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9805 "bison-chpl-lib.cpp"
    break;

  case 590: /* opt_task_intent_ls: %empty  */
#line 3231 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9811 "bison-chpl-lib.cpp"
    break;

  case 591: /* opt_task_intent_ls: task_intent_clause  */
#line 3232 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9817 "bison-chpl-lib.cpp"
    break;

  case 592: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3237 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9825 "bison-chpl-lib.cpp"
    break;

  case 593: /* task_intent_ls: intent_expr  */
#line 3243 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9831 "bison-chpl-lib.cpp"
    break;

  case 594: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3244 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9837 "bison-chpl-lib.cpp"
    break;

  case 595: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3249 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 9847 "bison-chpl-lib.cpp"
    break;

  case 596: /* forall_intent_ls: intent_expr  */
#line 3257 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9853 "bison-chpl-lib.cpp"
    break;

  case 597: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3258 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9859 "bison-chpl-lib.cpp"
    break;

  case 598: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3263 "chpl.ypp"
  {
    if (auto ident = (yyvsp[-2].expr)->toIdentifier()) {
      auto name = ident->name();
      auto node = TaskVar::build(BUILDER, LOC((yyloc)), /*attributeGroup*/ nullptr,
                                 name,
                                 /*intent*/ (yyvsp[-3].taskIntent),
                                 toOwned((yyvsp[-1].expr)),
                                 toOwned((yyvsp[0].expr)));
      BUILDER->noteDeclNameLocation(node.get(), LOC((yylsp[-2])));
      (yyval.expr) = node.release();
    } else {
      (yyval.expr) = context->syntax((yyloc), "expected identifier for task variable name.");
    }
  }
#line 9878 "bison-chpl-lib.cpp"
    break;

  case 599: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3278 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9886 "bison-chpl-lib.cpp"
    break;

  case 600: /* intent_expr: expr TREDUCE ident_expr  */
#line 3282 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9894 "bison-chpl-lib.cpp"
    break;

  case 601: /* task_var_prefix: TCONST  */
#line 3288 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 9900 "bison-chpl-lib.cpp"
    break;

  case 602: /* task_var_prefix: TIN  */
#line 3289 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 9906 "bison-chpl-lib.cpp"
    break;

  case 603: /* task_var_prefix: TCONST TIN  */
#line 3290 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 9912 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_var_prefix: TREF  */
#line 3291 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 9918 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_var_prefix: TCONST TREF  */
#line 3292 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 9924 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_var_prefix: TVAR  */
#line 3293 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 9930 "bison-chpl-lib.cpp"
    break;

  case 607: /* new_maybe_decorated: TNEW  */
#line 3298 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9936 "bison-chpl-lib.cpp"
    break;

  case 608: /* new_maybe_decorated: TNEW TOWNED  */
#line 3300 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9942 "bison-chpl-lib.cpp"
    break;

  case 609: /* new_maybe_decorated: TNEW TSHARED  */
#line 3302 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9948 "bison-chpl-lib.cpp"
    break;

  case 610: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3304 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9954 "bison-chpl-lib.cpp"
    break;

  case 611: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3306 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9960 "bison-chpl-lib.cpp"
    break;

  case 612: /* new_expr: new_maybe_decorated expr  */
#line 3312 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 9968 "bison-chpl-lib.cpp"
    break;

  case 613: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3319 "chpl.ypp"
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
#line 9984 "bison-chpl-lib.cpp"
    break;

  case 614: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3331 "chpl.ypp"
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
#line 10000 "bison-chpl-lib.cpp"
    break;

  case 615: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3343 "chpl.ypp"
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
#line 10017 "bison-chpl-lib.cpp"
    break;

  case 616: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3356 "chpl.ypp"
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
#line 10035 "bison-chpl-lib.cpp"
    break;

  case 617: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3373 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10043 "bison-chpl-lib.cpp"
    break;

  case 618: /* range_literal_expr: expr TDOTDOT expr  */
#line 3380 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10052 "bison-chpl-lib.cpp"
    break;

  case 619: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3385 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10061 "bison-chpl-lib.cpp"
    break;

  case 620: /* range_literal_expr: expr TDOTDOT  */
#line 3390 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10070 "bison-chpl-lib.cpp"
    break;

  case 621: /* range_literal_expr: TDOTDOT expr  */
#line 3395 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10080 "bison-chpl-lib.cpp"
    break;

  case 622: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3401 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 623: /* range_literal_expr: TDOTDOT  */
#line 3407 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10100 "bison-chpl-lib.cpp"
    break;

  case 624: /* cast_expr: expr TCOLON expr  */
#line 3437 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10108 "bison-chpl-lib.cpp"
    break;

  case 625: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3444 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10116 "bison-chpl-lib.cpp"
    break;

  case 626: /* super_expr: fn_expr  */
#line 3450 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10122 "bison-chpl-lib.cpp"
    break;

  case 630: /* expr: sub_type_level_expr TQUESTION  */
#line 3459 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10128 "bison-chpl-lib.cpp"
    break;

  case 631: /* expr: TQUESTION  */
#line 3461 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10134 "bison-chpl-lib.cpp"
    break;

  case 635: /* expr: fn_type  */
#line 3466 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10140 "bison-chpl-lib.cpp"
    break;

  case 645: /* opt_expr: %empty  */
#line 3480 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10146 "bison-chpl-lib.cpp"
    break;

  case 646: /* opt_expr: expr  */
#line 3481 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10152 "bison-chpl-lib.cpp"
    break;

  case 647: /* opt_try_expr: TTRY expr  */
#line 3485 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10158 "bison-chpl-lib.cpp"
    break;

  case 648: /* opt_try_expr: TTRYBANG expr  */
#line 3486 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10164 "bison-chpl-lib.cpp"
    break;

  case 649: /* opt_try_expr: super_expr  */
#line 3487 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10170 "bison-chpl-lib.cpp"
    break;

  case 655: /* call_base_expr: expr TBANG  */
#line 3504 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10178 "bison-chpl-lib.cpp"
    break;

  case 656: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3507 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10184 "bison-chpl-lib.cpp"
    break;

  case 659: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3514 "chpl.ypp"
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
#line 10200 "bison-chpl-lib.cpp"
    break;

  case 660: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3526 "chpl.ypp"
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
#line 10216 "bison-chpl-lib.cpp"
    break;

  case 661: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3538 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10224 "bison-chpl-lib.cpp"
    break;

  case 662: /* dot_expr: expr TDOT ident_use  */
#line 3545 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10230 "bison-chpl-lib.cpp"
    break;

  case 663: /* dot_expr: expr TDOT TTYPE  */
#line 3547 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10236 "bison-chpl-lib.cpp"
    break;

  case 664: /* dot_expr: expr TDOT TDOMAIN  */
#line 3549 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10242 "bison-chpl-lib.cpp"
    break;

  case 665: /* dot_expr: expr TDOT TLOCALE  */
#line 3551 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10248 "bison-chpl-lib.cpp"
    break;

  case 666: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3553 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10254 "bison-chpl-lib.cpp"
    break;

  case 667: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3555 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10260 "bison-chpl-lib.cpp"
    break;

  case 668: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3564 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenthLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10270 "bison-chpl-lib.cpp"
    break;

  case 669: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3570 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10278 "bison-chpl-lib.cpp"
    break;

  case 670: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3574 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10286 "bison-chpl-lib.cpp"
    break;

  case 671: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3578 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10294 "bison-chpl-lib.cpp"
    break;

  case 672: /* bool_literal: TFALSE  */
#line 3584 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10300 "bison-chpl-lib.cpp"
    break;

  case 673: /* bool_literal: TTRUE  */
#line 3585 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10306 "bison-chpl-lib.cpp"
    break;

  case 674: /* str_bytes_literal: STRINGLITERAL  */
#line 3589 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10312 "bison-chpl-lib.cpp"
    break;

  case 675: /* str_bytes_literal: BYTESLITERAL  */
#line 3590 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10318 "bison-chpl-lib.cpp"
    break;

  case 678: /* literal_expr: INTLITERAL  */
#line 3596 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10324 "bison-chpl-lib.cpp"
    break;

  case 679: /* literal_expr: REALLITERAL  */
#line 3597 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10330 "bison-chpl-lib.cpp"
    break;

  case 680: /* literal_expr: IMAGLITERAL  */
#line 3598 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10336 "bison-chpl-lib.cpp"
    break;

  case 681: /* literal_expr: CSTRINGLITERAL  */
#line 3599 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10342 "bison-chpl-lib.cpp"
    break;

  case 682: /* literal_expr: TNONE  */
#line 3600 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10348 "bison-chpl-lib.cpp"
    break;

  case 683: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3602 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10357 "bison-chpl-lib.cpp"
    break;

  case 684: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3607 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10366 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3612 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10375 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3617 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10384 "bison-chpl-lib.cpp"
    break;

  case 687: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3622 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10393 "bison-chpl-lib.cpp"
    break;

  case 688: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3627 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10402 "bison-chpl-lib.cpp"
    break;

  case 689: /* assoc_expr_ls: expr TALIAS expr  */
#line 3636 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10411 "bison-chpl-lib.cpp"
    break;

  case 690: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3641 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10421 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TPLUS expr  */
#line 3649 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10427 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TMINUS expr  */
#line 3650 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10433 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TSTAR expr  */
#line 3651 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10439 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TDIVIDE expr  */
#line 3652 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10445 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3653 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10451 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3654 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10457 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TMOD expr  */
#line 3655 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10463 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TEQUAL expr  */
#line 3656 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10469 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3657 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10475 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3658 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10481 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3659 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10487 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TLESS expr  */
#line 3660 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10493 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TGREATER expr  */
#line 3661 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10499 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TBAND expr  */
#line 3662 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10505 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TBOR expr  */
#line 3663 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10511 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TBXOR expr  */
#line 3664 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10517 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TAND expr  */
#line 3665 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10523 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TOR expr  */
#line 3666 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10529 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TEXP expr  */
#line 3667 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10535 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TBY expr  */
#line 3668 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10541 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TALIGN expr  */
#line 3669 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10547 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr THASH expr  */
#line 3670 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10553 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TDMAPPED expr  */
#line 3671 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10559 "bison-chpl-lib.cpp"
    break;

  case 714: /* unary_op_expr: TPLUS expr  */
#line 3675 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10565 "bison-chpl-lib.cpp"
    break;

  case 715: /* unary_op_expr: TMINUS expr  */
#line 3676 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10571 "bison-chpl-lib.cpp"
    break;

  case 716: /* unary_op_expr: TMINUSMINUS expr  */
#line 3677 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10577 "bison-chpl-lib.cpp"
    break;

  case 717: /* unary_op_expr: TPLUSPLUS expr  */
#line 3678 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10583 "bison-chpl-lib.cpp"
    break;

  case 718: /* unary_op_expr: TBANG expr  */
#line 3679 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10589 "bison-chpl-lib.cpp"
    break;

  case 719: /* unary_op_expr: expr TBANG  */
#line 3680 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10597 "bison-chpl-lib.cpp"
    break;

  case 720: /* unary_op_expr: TBNOT expr  */
#line 3683 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10603 "bison-chpl-lib.cpp"
    break;

  case 721: /* reduce_expr: expr TREDUCE expr  */
#line 3688 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10611 "bison-chpl-lib.cpp"
    break;

  case 722: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3692 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10619 "bison-chpl-lib.cpp"
    break;

  case 723: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3696 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10627 "bison-chpl-lib.cpp"
    break;

  case 724: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3700 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10635 "bison-chpl-lib.cpp"
    break;

  case 725: /* scan_expr: expr TSCAN expr  */
#line 3707 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10643 "bison-chpl-lib.cpp"
    break;

  case 726: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3711 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10651 "bison-chpl-lib.cpp"
    break;

  case 727: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3715 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10659 "bison-chpl-lib.cpp"
    break;

  case 728: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3719 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10667 "bison-chpl-lib.cpp"
    break;


#line 10671 "bison-chpl-lib.cpp"

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
