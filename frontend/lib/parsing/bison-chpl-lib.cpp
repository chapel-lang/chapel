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
  YYSYMBOL_extern_export_kind = 235,       /* extern_export_kind  */
  YYSYMBOL_extern_export_decl_stmt = 236,  /* extern_export_decl_stmt  */
  YYSYMBOL_237_3 = 237,                    /* $@3  */
  YYSYMBOL_238_4 = 238,                    /* $@4  */
  YYSYMBOL_extern_block_stmt = 239,        /* extern_block_stmt  */
  YYSYMBOL_no_loop_attributes = 240,       /* no_loop_attributes  */
  YYSYMBOL_loop_stmt = 241,                /* loop_stmt  */
  YYSYMBOL_for_loop_kw = 242,              /* for_loop_kw  */
  YYSYMBOL_loop_stmt_base = 243,           /* loop_stmt_base  */
  YYSYMBOL_zippered_iterator = 244,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 245,                  /* if_stmt  */
  YYSYMBOL_ifvar = 246,                    /* ifvar  */
  YYSYMBOL_interface_start = 247,          /* interface_start  */
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
  YYSYMBOL_named_formal = 306,             /* named_formal  */
  YYSYMBOL_opt_formal_intent_tag = 307,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 308,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 309,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 310,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 311,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 312,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 313,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 314,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 315,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 316,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 317, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 318,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 319,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 320,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 321, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 322, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 323, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 324,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 325,            /* var_decl_type  */
  YYSYMBOL_var_decl_start = 326,           /* var_decl_start  */
  YYSYMBOL_var_decl_stmt = 327,            /* var_decl_stmt  */
  YYSYMBOL_328_11 = 328,                   /* $@11  */
  YYSYMBOL_329_12 = 329,                   /* $@12  */
  YYSYMBOL_var_decl_stmt_inner_ls = 330,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 331,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 332, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 333, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 334,            /* opt_init_expr  */
  YYSYMBOL_formal_or_ret_type_expr = 335,  /* formal_or_ret_type_expr  */
  YYSYMBOL_ret_type = 336,                 /* ret_type  */
  YYSYMBOL_colon_ret_type = 337,           /* colon_ret_type  */
  YYSYMBOL_opt_ret_type = 338,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 339,                 /* opt_type  */
  YYSYMBOL_formal_type = 340,              /* formal_type  */
  YYSYMBOL_colon_formal_type = 341,        /* colon_formal_type  */
  YYSYMBOL_opt_colon_formal_type = 342,    /* opt_colon_formal_type  */
  YYSYMBOL_expr_ls = 343,                  /* expr_ls  */
  YYSYMBOL_tuple_component = 344,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 345,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 346,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 347,                /* actual_ls  */
  YYSYMBOL_actual_expr = 348,              /* actual_expr  */
  YYSYMBOL_ident_expr = 349,               /* ident_expr  */
  YYSYMBOL_sub_type_level_expr = 350,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 351,                 /* for_expr  */
  YYSYMBOL_for_expr_base = 352,            /* for_expr_base  */
  YYSYMBOL_bracket_loop_expr = 353,        /* bracket_loop_expr  */
  YYSYMBOL_bracket_loop_expr_base = 354,   /* bracket_loop_expr_base  */
  YYSYMBOL_cond_expr = 355,                /* cond_expr  */
  YYSYMBOL_nil_expr = 356,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 357,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 358,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 359,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 360,           /* task_intent_ls  */
  YYSYMBOL_intent_expr = 361,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 362,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 363,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 364,                 /* new_expr  */
  YYSYMBOL_let_expr = 365,                 /* let_expr  */
  YYSYMBOL_range_literal_expr = 366,       /* range_literal_expr  */
  YYSYMBOL_cast_expr = 367,                /* cast_expr  */
  YYSYMBOL_tuple_expand_expr = 368,        /* tuple_expand_expr  */
  YYSYMBOL_super_expr = 369,               /* super_expr  */
  YYSYMBOL_expr = 370,                     /* expr  */
  YYSYMBOL_opt_expr = 371,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 372,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 373,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 374,           /* call_base_expr  */
  YYSYMBOL_call_expr = 375,                /* call_expr  */
  YYSYMBOL_dot_expr = 376,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 377,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 378,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 379,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 380,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 381,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 382,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 383,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 384,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 385,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 386       /* reduce_scan_op_expr  */
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

#line 510 "bison-chpl-lib.cpp"

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
#define YYLAST   17686

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  206
/* YYNRULES -- Number of rules.  */
#define YYNRULES  741
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1264

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
       0,   665,   665,   669,   670,   675,   676,   684,   688,   698,
     703,   710,   711,   712,   713,   714,   715,   716,   717,   718,
     719,   737,   738,   739,   747,   756,   765,   769,   776,   784,
     785,   786,   787,   788,   789,   790,   791,   793,   813,   821,
     838,   854,   871,   888,   904,   925,   926,   927,   931,   937,
     938,   942,   946,   947,   951,   958,   966,   969,   979,   981,
     986,   992,   997,  1006,  1010,  1031,  1048,  1065,  1083,  1084,
    1086,  1091,  1092,  1097,  1115,  1120,  1125,  1137,  1156,  1157,
    1161,  1165,  1169,  1174,  1178,  1182,  1190,  1195,  1201,  1206,
    1215,  1216,  1220,  1225,  1233,  1242,  1250,  1261,  1269,  1270,
    1271,  1272,  1273,  1274,  1275,  1276,  1277,  1278,  1279,  1280,
    1281,  1282,  1283,  1284,  1296,  1300,  1305,  1310,  1316,  1325,
    1326,  1330,  1339,  1343,  1347,  1351,  1355,  1359,  1368,  1369,
    1370,  1374,  1375,  1376,  1377,  1378,  1382,  1383,  1384,  1396,
    1397,  1398,  1403,  1404,  1405,  1406,  1407,  1408,  1409,  1410,
    1411,  1412,  1413,  1414,  1415,  1416,  1417,  1418,  1419,  1420,
    1424,  1425,  1426,  1427,  1428,  1429,  1430,  1431,  1432,  1433,
    1434,  1435,  1442,  1443,  1444,  1448,  1449,  1453,  1460,  1470,
    1474,  1480,  1480,  1486,  1486,  1495,  1496,  1497,  1498,  1499,
    1500,  1501,  1505,  1510,  1515,  1520,  1527,  1535,  1536,  1540,
    1549,  1557,  1564,  1573,  1592,  1591,  1601,  1600,  1613,  1620,
    1626,  1627,  1636,  1637,  1638,  1639,  1643,  1666,  1685,  1704,
    1729,  1734,  1739,  1744,  1749,  1754,  1759,  1764,  1769,  1774,
    1779,  1784,  1789,  1794,  1799,  1805,  1814,  1823,  1827,  1831,
    1835,  1839,  1843,  1847,  1851,  1855,  1860,  1865,  1870,  1878,
    1893,  1911,  1918,  1924,  1933,  1934,  1939,  1944,  1945,  1946,
    1947,  1948,  1949,  1950,  1951,  1952,  1953,  1954,  1955,  1960,
    1965,  1966,  1967,  1968,  1976,  1977,  1981,  1985,  1989,  1996,
    2000,  2004,  2011,  2015,  2019,  2023,  2030,  2031,  2035,  2039,
    2043,  2050,  2063,  2079,  2087,  2092,  2102,  2103,  2107,  2111,
    2116,  2125,  2130,  2134,  2141,  2142,  2146,  2156,  2162,  2174,
    2181,  2182,  2183,  2187,  2188,  2192,  2196,  2200,  2204,  2208,
    2216,  2235,  2248,  2255,  2260,  2267,  2266,  2276,  2282,  2281,
    2296,  2304,  2317,  2335,  2332,  2361,  2365,  2366,  2368,  2373,
    2374,  2378,  2379,  2383,  2386,  2388,  2393,  2394,  2405,  2426,
    2425,  2441,  2440,  2458,  2468,  2465,  2493,  2502,  2511,  2521,
    2531,  2540,  2555,  2556,  2564,  2565,  2566,  2575,  2576,  2577,
    2578,  2579,  2580,  2581,  2582,  2583,  2584,  2585,  2586,  2587,
    2588,  2589,  2590,  2591,  2592,  2593,  2594,  2595,  2596,  2597,
    2598,  2599,  2603,  2604,  2605,  2606,  2607,  2608,  2609,  2610,
    2611,  2612,  2613,  2614,  2619,  2620,  2624,  2625,  2629,  2630,
    2634,  2635,  2639,  2640,  2644,  2645,  2649,  2653,  2657,  2662,
    2667,  2672,  2677,  2685,  2689,  2697,  2698,  2699,  2700,  2701,
    2702,  2703,  2704,  2705,  2706,  2710,  2711,  2712,  2713,  2714,
    2715,  2716,  2717,  2718,  2719,  2720,  2724,  2725,  2726,  2730,
    2731,  2732,  2733,  2734,  2735,  2736,  2737,  2738,  2739,  2740,
    2744,  2745,  2748,  2749,  2753,  2754,  2758,  2763,  2764,  2766,
    2768,  2770,  2775,  2777,  2782,  2784,  2786,  2788,  2790,  2792,
    2794,  2799,  2800,  2801,  2805,  2814,  2820,  2830,  2834,  2841,
    2863,  2864,  2869,  2870,  2871,  2872,  2873,  2877,  2886,  2885,
    2895,  2894,  2904,  2909,  2918,  2922,  2929,  2948,  2967,  2971,
    2978,  2980,  2982,  2989,  2990,  2991,  2995,  2999,  3000,  3001,
    3005,  3006,  3010,  3011,  3015,  3016,  3017,  3018,  3022,  3023,
    3027,  3031,  3032,  3038,  3039,  3043,  3047,  3051,  3058,  3059,
    3063,  3068,  3077,  3078,  3082,  3083,  3090,  3091,  3092,  3093,
    3094,  3095,  3097,  3099,  3101,  3107,  3109,  3112,  3114,  3116,
    3118,  3120,  3122,  3124,  3126,  3128,  3130,  3135,  3139,  3143,
    3147,  3151,  3155,  3164,  3173,  3185,  3189,  3193,  3197,  3201,
    3205,  3209,  3213,  3220,  3238,  3246,  3247,  3248,  3249,  3250,
    3251,  3252,  3256,  3257,  3261,  3265,  3272,  3279,  3289,  3290,
    3294,  3298,  3302,  3309,  3310,  3311,  3312,  3313,  3314,  3315,
    3316,  3317,  3318,  3322,  3324,  3326,  3328,  3330,  3336,  3343,
    3356,  3369,  3383,  3401,  3408,  3413,  3418,  3423,  3429,  3435,
    3465,  3472,  3479,  3480,  3484,  3486,  3487,  3489,  3491,  3492,
    3493,  3494,  3497,  3498,  3499,  3500,  3501,  3502,  3503,  3504,
    3505,  3509,  3510,  3514,  3515,  3516,  3520,  3521,  3522,  3523,
    3532,  3533,  3536,  3537,  3538,  3542,  3554,  3566,  3573,  3575,
    3577,  3579,  3581,  3583,  3592,  3598,  3602,  3606,  3613,  3614,
    3618,  3619,  3623,  3624,  3625,  3626,  3627,  3628,  3629,  3630,
    3635,  3640,  3645,  3650,  3655,  3664,  3669,  3678,  3679,  3680,
    3681,  3682,  3683,  3684,  3685,  3686,  3687,  3688,  3689,  3690,
    3691,  3692,  3693,  3694,  3695,  3696,  3697,  3698,  3699,  3700,
    3704,  3705,  3706,  3707,  3708,  3709,  3712,  3716,  3720,  3724,
    3728,  3735,  3739,  3743,  3747,  3755,  3756,  3757,  3758,  3759,
    3760,  3761
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
  "extern_export_decl_stmt_start", "extern_export_kind",
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

#define YYPACT_NINF (-911)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-742)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -911,    68,  3825,  -911,   -77,    54,  -911,  -911,  -911,  -911,
    -911,  -911,  -911,  -911, 11797,     5,    53,   104, 13647,   139,
   17452,     5,   142,    26,   140,    53,  5225, 11797,   105, 17511,
    -911,   262,   227,  -911,  9893,   267,    -8,   159,  -911,   306,
     215, 17511, 17511, 17511,  -911,  2288, 10756,   311, 11797, 11797,
     272,  -911,   320,   376, 11797,  -911, 13647,  -911, 11797,   349,
     183,    34,   387, 13001,   398, 17570,  -911, 11797,  7998, 11797,
   10756, 13647,   343,   426,   330,  5225,   448, 11797,   455,  6966,
    6966,  -911,   480,  -911, 13647,  -911,   508, 10065,  -911, 11797,
    -911, 11797,  -911,  -911, 13475, 11797,  -911, 10237,  -911,  -911,
    -911,  4175,  8170, 11797,  -911,  4875,  -911,  -911,  -911,  -911,
    1623,   512,  -911,   431,   435,  -911,    37,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,
    7310,  -911,  7482,  -911,  -911,   112,  -911,  -911,   325,  -911,
     549,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,   461,
   17511,  -911,   443,   444,   369,  -911,   281,  -911,  -911,  -911,
     899,   992,  -911,  -911, 17511,  -911,  1776,  -911,   463,   456,
    -911,  -911,  -911,   466,   471, 11797,   476,   485,  -911,  -911,
    -911, 16990, 13468,   375,   488,   489,  -911,  -911,   382,  -911,
    -911,  -911,  -911,  -911,   340,  -911,  -911,  -911, 11797,  -911,
   17511,  -911, 11797, 11797,    91,   588,   417,  -911,  -911,  -911,
    -911, 16990,   418,  -911,  -911,    41,  5225,  -911,  -911,  -911,
     491,   250,   477,  -911,   238,  -911,   493,  -911,   -76, 16990,
    8342,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911, 17511,
   17511,   -64, 14136,  -911,  -911,   562,  8342,   498,   495,  -911,
    5225,  2735,    79,    16,  -911,  5225,  -911,  -911,  2821,  -911,
      11, 15448,   462,  -911,   497,   500,  -911,  2821,   250,   462,
    -911,  8342,  1727,  1727,   186,    -6,  -911,     3,  -911,  -911,
    -911,  -911,  7654,  -911,  -911,   627,  -911,   503,   521,  -911,
    -911,  3097,   524,  -911,  -911, 16990,   -56, 11797, 11797,  -911,
     -58,  -911,  -911, 16990,   515, 15567,  -911,  2821,   250,   505,
    8342,  -911, 16990, 15607, 11797,  -911,  -911,  -911,  -911,  -911,
     250,   518,   377,   377,  1199,   462,   462,   342,  -911,  -911,
    4350,   -21, 11797,   538,   -73,   516,  1199,   681,  -911,  -911,
   17511,  -911, 11797,  -911,  4175,  -911,   131,  -911,   562,  -911,
    -911,   688,   523,  4525, 11797,  -911, 11797,   626,  -911,  -911,
   14790,    40,   461, 16990,   436,  -911,  5225,   617,  -911,  -911,
    9893, 10409, 10928,  -911,  -911,  -911, 17511,  -911, 17511, 11797,
     533,  -911,  2484,   265,   899,  -911,  -911,  -911,  -911,   205,
    -911,  -911,  -911,  -911,  -911,  -911,  -911,    82,   210,  -911,
    -911,  -911,  -911,  -911,  -911,  -911, 13308,   580,   114,  -911,
     543,   125,   440,  -911,   561, 11797, 11797, 11797, 10928, 10928,
   11797,   445, 11797, 11797, 11797, 11797, 11797,   517, 13475, 11797,
   11797, 11797, 11797, 11797, 11797, 11797, 11797, 11797, 11797, 11797,
   11797, 11797, 11797, 11797, 11797,   641,  -911,  -911,  -911,  -911,
    -911, 10065, 10065,  -911,  -911,  -911,  -911, 10065,  -911,  -911,
   10065, 10065,  8342,  8514, 10928, 10928, 15647,   542,  -911, 11100,
   10928, 17511,  -911,  6272,  -911,  -911,  -911,  2288,  -911, 11797,
    -911,   590,   544,   575,  -911,  -911,   601,   602,  5225,   694,
    5225,  -911,   695, 11797,   664,   559,  -911, 10065,  -911,  2735,
    -911,  -911,   102,  -911, 11969,   608, 11797,  2288,  -911,  -911,
   11797,  -911, 17302, 11797, 11797,  -911,   238,   565,  -911,  -911,
    -911,  -911, 17511,  -911,   899, 13173,  2735,   593, 11969,  -911,
   16990, 16990,  -911,   477,  -911,    48,  -911,  8342,   570,  -911,
    2821,   722,   722,  -911,  -911,  -911,  -911, 11275,  -911, 15726,
    8689,  8861,  -911,  -911,  -911, 16990, 10928, 10928,  -911,   431,
    9033,  -911,   371,  -911,  4700,  -911,   161, 15766,   176, 14880,
   17511,  6791,  6619,  -911,   461,   577,  -911,  -911,   238,  -911,
     629, 17511,    28,  2821,   578, 13789,   -34,   212, 15807,   -54,
      15, 14625,  -911,   133,  -911,   156,   605,  1923,   582, 17393,
     624,    98,  -911,  -911,   141,  -911,  -911,   194,  -911,  -911,
    -911,  6447,  -911,   649,  -911,  -911,   584,   609,  -911,   610,
    -911,   611,   612,   613,   614,   615,  -911,   618,   620,   621,
     622,   623,   630,   631,  -911,  -911,  -911,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,  -911,  -911, 11797,   616,   632,   584,
    -911,   584,  -911, 11797,  -911, 17511,  -911,  -911,  -911, 17109,
   17109,   525,  -911,   525,  -911,   525,   650,  2284,  1777,  1955,
     250,   377,  -911,   460,  -911,  -911,  -911,  -911,  -911,  1199,
    1932,   525,   745,   745, 17109,   745,   745,  1411,   377,  1932,
   17149,  1411,   462,   462,   377,  1199,   625,   628,   647,   654,
     656,   657,   633,   591,  -911,   525,  -911,   525, 11797, 10065,
      95,   742, 15119,   606,   653,   211,  -911,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,   147,  -911,   962, 17069,   433,   213,
   16990, 10065,  -911, 10065, 11797, 11797,   747,  5225,   748, 15958,
    5225, 15198, 17511,  -911,   222,   225,  2735,    79,  -911, 16990,
    9205,  -911, 16990,  -911,  -911,  -911, 17511, 15998, 16038,  2288,
    -911,   593,   639,  -911,   245, 11969,  -911,   268,  -911, 11797,
    -911,   643,    -4,   660,  -911,  2133,  -911,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,   214,    19, 14699,  -911,  -911,   263,
    -911,  -911,  -911,  -911, 11797,  -911,   240, 13894, 11797,  -911,
   11797,  6791,  6619,  -911,  9377,   457,  9549,   472,   482,  9721,
    7826,   483,   236,  -911,  -911,  -911, 16117,   642,   658,   669,
    -911, 17511,  2288, 11797,   738,  -911,  -911, 11797, 16990,  5225,
     659, 10928, 11447,  4000,   666, 11797, 11622,  -911,  5225,  -911,
      28,  5400, 10928,  -911,    28, 17511,   477,  -911, 10065,   671,
    1369,  -911,  -911, 11797,    36,  -911,   636,  -911,   649,  -911,
    -911,  -911,  -911,  -911,    13,   360,  -911, 16157,  -911, 14053,
    -911,  -911, 16990,  -911,   670,   672,  -911,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,   292, 10928, 13789, 11797, 12141, 10928,
   10065,  -911,  -911,  -911,  7138,  -911,  -911,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,  -911,  -911,  -911,    44, 10928, 10928,
    -911,  -911,  -911, 16990, 16990,  5225,  -911,  5225, 11797,  -911,
    5225,   753,   679,   680,  -911,  -911,   608,  -911,  -911,  -911,
     676,   677,   255,   268, 11797,   649,   593,  -911, 10584,  -911,
    -911, 16990,  -911,  5225, 11797,  -911,  -911,  -911, 17511,  -911,
     651,   477, 10928,  5225, 10928, 10065,  -911,  -911,  -911,  -911,
    -911,   460,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,  -911,  6791,  -911,  -911, 16205, 15289,
     285,   685,  6791,  -911, 11797,  -911,  -911,  3184,   301,   338,
   16308, 11797,   296,  -911,  5225,   -26, 15363,  -911,  -911, 16990,
    5225, 16990,  -911, 16356, 16990,  -911,  9893, 16990,    22, 14959,
    -911,  -911,  -911,   298,  -911,  -911,  -911,  -911,  3296,  3626,
    -911, 16990, 17511,   512,  -911,    13,   221, 11797,  -911,   706,
     686,  -911,   584,   584,  -911,  -911,  -911,   687, 16396, 11797,
     827, 16547,   315,  -911,   608,   331,   346,  -911,  -911, 16990,
     828,  5225,  -911,  -911,  -911,  8342,  8342,  -911,  -911,  -911,
    -911,  -911,   268, 16990,  -911,   135,    24,   697, 11797,  -911,
     126, 15408,    19, 15039,  -911,   110, 11797,  6791,  6619,  -911,
    -911,  -911, 16587, 16990,  3525,  -911,  -911,  -911,  -911,  2821,
    -911,  -911,  5575,   692,  5750,   693,  -911, 11797, 14216,  5925,
    -911,    28,  6100,  -911,    28,   707,  3626,  -911,  -911,  -911,
   17393,  -911,  -911,  -911,   261,  -911,   -33,  -911,   322, 16628,
      65,  -911,  -911,  -911, 12313, 12485, 16743, 12657, 12829,  -911,
    -911,  5225,  -911,   698,   700,  -911,  -911,  5225,  -911,   477,
   16990,  5225,  5225,  -911, 16990,   385,   709, 11797,  -911,  -911,
    9893,  -911, 16990,  5225,  9893,  -911, 16990,  5225, 16990,  5225,
    9893, 16990,  -911,  9893, 16990,  -911,  -911,  -911,  -911,    69,
    -911, 11797,   221,   261,   261,   261,   261,   261,   261,   221,
    5050,  -911,  -911,  -911,  -911, 11797, 11797, 11797, 11797, 11797,
    -911,   711,   714,   629,  -911,  -911,  -911, 16990, 14296,  -911,
   14376,  -911, 15958, 14456, 14536,  -911, 16990,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,   734,  -911, 16791, 16831, 16910, 16950,
    -911,  -911,  5225,  5225,  5225,  5225, 11797, 11797, 11797, 11797,
   15958, 15958, 15958, 15958
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   139,   466,    55,   684,   685,
     686,   680,   681,   687,   209,   592,   128,   160,   563,   167,
     565,   592,   166,   498,   495,   128,     0,   209,   274,   161,
     198,   197,   678,   196,   209,   165,    68,   275,   337,   162,
     140,     0,     0,     0,   332,     0,   209,   169,   209,   209,
     613,   584,   688,   170,   209,   338,   557,   492,   209,     0,
       0,   183,   181,   423,   164,   566,   494,   209,   209,   209,
     209,   561,     0,   168,     0,     0,   141,   209,   679,   209,
     209,   485,   163,   312,   559,   496,   171,   209,   737,   209,
     739,   209,   740,   741,   629,   209,   738,   209,   637,   179,
     736,     0,   209,   209,     4,     0,     5,    10,    11,    12,
      48,    51,    52,    56,     0,    45,    71,    13,    77,    14,
      15,    16,    17,    29,   544,   545,    22,    46,   180,   190,
     209,   199,   651,   191,    18,     0,    31,    30,     0,    47,
       0,   269,    19,   647,    21,    35,    32,    33,   189,   313,
       0,   187,     0,     0,   644,   336,     0,   641,   185,   353,
     449,   435,   639,   188,     0,   497,     0,   186,   656,   635,
     634,   638,   548,   546,     0,   209,   645,   646,   650,   649,
     648,     0,   547,     0,   657,   658,   659,   682,   683,   640,
     550,   549,   642,   643,     0,    28,   565,   161,   209,   140,
       0,   566,   209,   209,     0,     0,   644,   656,   546,   645,
     646,   555,   547,   657,   658,     0,     0,   593,   129,   130,
       0,   564,     0,   486,     0,   493,     0,    20,     0,   533,
     209,   136,   142,   153,   148,   147,   155,   133,   146,   156,
     143,   137,   157,   131,   158,   151,   145,   152,   149,   150,
     132,   134,   144,   154,   159,   138,   322,   135,   208,     0,
       0,     0,     0,    70,    69,    71,   209,     0,     0,   251,
       0,     0,     0,     0,   504,     0,   176,    40,     0,   304,
       0,   303,   722,   617,   614,   615,   616,   500,   558,   723,
       7,   209,   335,   335,   430,   173,   425,   174,   426,   427,
     431,   432,   172,   433,   434,   423,   529,     0,   340,   341,
     343,     0,   424,   528,   345,   516,     0,   209,   209,   177,
     641,   632,   655,   633,     0,     0,    43,     0,   562,     0,
     209,    44,   556,     0,   209,   282,    48,   286,   283,   286,
     560,     0,   724,   726,   627,   721,   720,     0,    74,    78,
       0,     0,   209,     0,     0,   535,   628,     0,     6,   310,
       0,   197,   209,   311,     0,    49,     0,     9,    71,    50,
      53,     0,    60,     0,   209,    72,   209,     0,   498,   195,
       0,   680,   313,   652,   204,   215,     0,   212,   213,   214,
     209,   209,   209,   211,   567,   575,     0,   253,     0,   209,
       0,   309,     0,   423,   449,   447,   448,   446,   361,   451,
     456,   459,   450,   454,   453,   455,   458,     0,   439,   441,
     445,   444,   436,   437,   440,   443,     0,   490,     0,   487,
       0,     0,   636,    34,   618,   209,   209,   209,   209,   209,
     209,   725,   209,   209,   209,   209,   209,     0,   626,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   392,   399,   400,   401,   396,
     398,   209,   209,   394,   397,   395,   393,   209,   403,   402,
     209,   209,   209,   209,   209,   209,     0,     0,   212,   209,
     209,     0,   597,     0,    36,    24,    37,     0,    25,   209,
      38,   544,     0,   539,   540,   543,     0,     0,     0,   242,
       0,   392,   238,   209,     0,     0,    23,   209,    26,     0,
     508,   510,     0,   527,   209,   513,   209,     0,   175,    39,
     209,   306,     0,   209,   209,    41,     0,     0,   184,   182,
     428,   429,     0,   424,   449,   423,     0,   531,   209,   121,
     653,   654,   351,     0,   178,     0,    42,   209,     0,   293,
       0,   284,   285,    27,    76,    75,    79,   209,   689,     0,
     209,   209,   676,   674,     8,   500,   209,   209,   210,    56,
     209,    54,     0,    65,     0,   119,     0,   114,     0,    86,
       0,   209,   209,   192,   313,     0,   200,   206,     0,   203,
       0,     0,     0,     0,     0,   576,     0,     0,   533,     0,
       0,     0,   256,     0,   254,   280,   314,     0,     0,     0,
     330,     0,   323,   410,     0,   412,   416,     0,   457,   452,
     521,     0,   523,   460,   442,   438,   408,   136,   388,   142,
     386,   148,   147,   146,   143,   137,   390,   157,   158,   145,
     149,   132,   144,   159,   385,   367,   370,   368,   369,   391,
     380,   371,   384,   376,   374,   387,   375,   373,   378,   383,
     372,   377,   381,   382,   379,   389,   209,   364,     0,   408,
     365,   408,   362,   209,   489,     0,   484,   503,   502,   717,
     716,   719,   728,   727,   732,   731,   713,   710,   711,   712,
     630,   700,   139,     0,   670,   671,   141,   669,   668,   624,
     704,   715,   709,   707,   718,   708,   706,   698,   703,   705,
     714,   697,   701,   702,   699,   625,     0,     0,     0,     0,
       0,     0,     0,     0,   730,   729,   734,   733,   209,   209,
       0,     0,     0,   280,     0,   603,   604,   610,   609,   611,
     606,   612,   608,   595,     0,   598,     0,     0,     0,     0,
     534,   209,   552,   209,   209,   209,   241,     0,   237,     0,
       0,     0,     0,   551,     0,     0,   511,     0,   526,   525,
     209,   506,   623,   505,   305,   302,     0,     0,     0,     0,
     667,   531,   346,   342,     0,   209,   532,   513,   344,   209,
     350,     0,     0,     0,   553,     0,   287,   690,   631,   675,
     536,   535,   677,   537,     0,     0,     0,    57,    58,     0,
      61,    63,    67,    66,   209,    97,     0,     0,   209,    92,
     209,   209,   209,    64,   209,   367,   209,   368,   369,   209,
     209,   379,     0,   404,   405,    81,    80,    91,     0,     0,
     315,     0,     0,   209,     0,   218,   217,   209,   577,     0,
       0,   209,   209,     0,     0,   209,   209,   693,     0,   226,
       0,     0,   209,   224,     0,     0,     0,   277,   209,     0,
     335,   321,   327,   209,   325,   320,   423,   411,   460,   519,
     518,   517,   520,   461,   467,   423,   360,     0,   366,     0,
     356,   357,   491,   488,     0,     0,   127,   125,   126,   124,
     123,   122,   665,   666,     0,   209,   691,   209,   209,   209,
     209,   596,   605,   607,   209,   594,   160,   167,   166,   165,
     162,   169,   170,   164,   168,   163,   171,     0,   209,   209,
     499,   542,   541,   250,   249,     0,   244,     0,   209,   240,
       0,   246,     0,   279,   509,   512,   513,   514,   515,   301,
       0,     0,     0,   513,   209,   460,   531,   530,   209,   419,
     417,   352,   295,     0,   209,   294,   297,   554,     0,   288,
     291,     0,   209,     0,   209,   209,    59,   120,   117,    98,
     109,   104,   103,   102,   111,    99,   112,   107,   101,   108,
     105,   106,   100,   110,   113,   209,   116,   115,    88,    87,
       0,     0,   209,   193,   209,   194,   315,   335,     0,     0,
       0,   209,     0,   234,     0,     0,     0,   692,   232,   578,
       0,   695,   694,     0,   571,   227,   209,   570,     0,     0,
     225,   255,   252,     0,   308,   183,   181,   307,   335,   335,
     316,   331,     0,     0,   413,   467,     0,   209,   354,   415,
       0,   363,   408,   408,   672,   673,   279,     0,     0,   209,
       0,     0,     0,   599,   513,   656,   656,   243,   239,   583,
     245,     0,    73,   276,   507,   209,   209,   501,   420,   418,
     347,   348,   513,   407,   299,     0,     0,     0,   209,   289,
       0,     0,     0,     0,    62,     0,   209,   209,   209,    93,
      95,    84,    83,    82,   335,   201,   207,   205,   216,     0,
     236,   235,     0,     0,     0,     0,   233,   209,     0,     0,
     222,     0,     0,   220,     0,   281,   335,   318,   317,   326,
       0,   333,   481,   482,     0,   483,   469,   472,     0,   468,
       0,   409,   358,   359,   209,   209,     0,   209,   209,   281,
     600,     0,   248,     0,     0,   422,   421,     0,   298,     0,
     292,     0,     0,   118,    89,     0,     0,   209,   202,   219,
     209,   230,   580,     0,   209,   228,   579,     0,   696,     0,
     209,   569,   223,   209,   568,   221,   278,   319,   329,     0,
     480,   209,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   462,   464,   355,   463,   209,   209,   209,   209,   209,
     247,   619,   620,   300,   290,    94,    96,    85,     0,   231,
       0,   229,   574,     0,     0,   334,   471,   473,   474,   477,
     478,   479,   475,   476,   470,   465,     0,     0,     0,     0,
     621,   622,     0,     0,     0,     0,   209,   209,   209,   209,
     582,   581,   573,   572
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -911,  -911,  -911,    -1,   -49,  2335,  -911,   384,   772,  -911,
    -911,   496,  -391,  -109,  -911,   304,  -911,  -911,  -101,  -911,
    -911,    30,   634,  -911,  -550,    49,   519,  -576,  -911,  -801,
    -911,  -911,  -911,    73,  -911,  -911,  -911,   875,  -911,  1592,
    -184,  -911,  -911,  -474,   -42,  -911,  -847,  -911,  -911,   302,
    -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,  -911,    17,
    -911,  -137,   541,  -380,  -911,   526,  -911,  -911,  -911,    39,
    1698,  -911,  -911,  -911,  -911,   576,  -911,   -69,  -911,  -911,
    -911,  -911,   388,  -911,  -911,  -911,   -98,  -911,  -346,  -796,
    -911,  -911,  -911,  -911,  -911,  -611,  -911,   136,  -911,  -911,
    -911,  -911,  -911,   372,  -911,   116,  -911,  -911,  -911,  -911,
     536,  -911,  -911,  -911,  -911,    27,  -417,  -175,  -789,  -910,
    -658,  -911,    33,  -911,    35,   -47,   637,   -46,  -911,  -911,
    -364,  -818,  -911,  -270,  -911,  -125,  -278,  -268,  -747,  -911,
    -911,    84,   253,  -911,   407,  -198,   811,  -911,  -911,  -160,
     422,   168,  -429,  -770,  -621,  -911,  -911,  -911,  -598,  -513,
    -911,  -754,    -5,    14,  -911,  -261,  -499,  -555,    -2,  -911,
    -911,  -911,  -911,  -911,  -911,   348,  -911,   929,  -401,  -911,
      31,  -911,  -911,   465,   560,  -911,  -911,  -911,  -911,  2565,
    -911,   -55,  1031,  -911,  1274,  1548,  -911,  -911,  -911,  -911,
    -911,  -911,  -911,  -911,  -911,  -473
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   349,   105,   106,   107,   108,   109,   366,
     367,   110,   111,   112,   113,   372,   581,   819,   820,   114,
     115,   116,   377,   117,   118,   119,   350,   847,   588,   848,
     120,   121,  1006,   585,   586,   122,   123,   220,   255,   272,
     124,   257,   125,   306,   535,   126,   127,   293,   292,   128,
     129,   130,   131,   132,   597,   133,   598,   851,   134,   204,
     136,   392,   393,   606,   137,   261,   138,   139,   613,   614,
     205,   141,   142,   143,   144,   561,   806,   981,   145,   146,
     802,   976,   279,   280,   147,   148,   149,   150,   400,   880,
     151,   152,   621,  1052,  1053,   622,   153,   206,  1199,   155,
     156,   307,   308,   309,   965,   157,   321,   553,   799,   158,
     159,  1150,   160,   161,   678,   679,   843,   844,   845,   969,
     896,   404,   624,  1060,   625,   626,   311,   543,   426,   408,
     417,   894,  1213,  1214,   162,  1058,  1146,  1147,  1148,   163,
     164,   428,   429,   684,   165,   166,   167,   224,   536,   273,
     274,   521,   522,   781,   313,   892,   632,   633,   525,   314,
     796,   797,   351,   353,   354,   502,   503,   504,   207,   169,
     170,   394,   171,   395,   172,   208,   174,   216,   217,   754,
     755,   756,   175,   209,   210,   178,   179,   180,   322,   181,
     384,   505,   212,   183,   213,   214,   186,   187,   188,   189,
     609,   190,   191,   192,   193,   194
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     168,   104,   370,   800,   277,   515,   431,   481,   882,   680,
     891,   619,   610,   324,   305,   842,   310,   312,   774,   135,
     758,   900,   228,   901,   168,   821,   497,   970,   326,   888,
     537,  1011,   341,  1050,   382,   798,   595,   963,  1007,   508,
     627,  -328,   492,   135,   275,   523,   501,   355,   868,   801,
     778,  -274,   275,  1088,  1017,  1129,   218,   275,   692,   694,
    -275,   275,   316,   359,   526,   552,   265,   490,     3,   558,
    1055,   499,   973,   168,   571,  1056,   263,   168,   168,   264,
     523,   -70,   501,   630,   374,  1201,   215,   513,   195,   500,
     775,   499,   135,   866,   215,   276,   135,   135,  1210,   168,
     572,   -70,  1210,   168,   734,   736,   364,   501,   219,   549,
     741,  -257,  -349,   974,  1202,  -522,   385,   794,   135,   276,
     -70,   867,   135,   375,  -296,   215,   567,  -524,   337,   339,
     363,  1057,   488,   388,   389,   215,   223,   385,   154,   215,
     368,   859,   215,   915,  -522,   386,   501,  1091,   -70,  1122,
     568,   376,    83,   387,   388,   389,   385,   890,   530,   369,
     385,  -258,   154,   527,   386,  -296,   230,   975,  1167,  -524,
    1050,   499,   387,   388,   389,   266,   387,   388,   389,   956,
     792,   364,  1165,  -324,   320,   364,  1084,   397,  -522,   364,
     524,  -524,   364,  1089,   364,   523,  -264,   815,   364,  -263,
    -522,  1137,  1138,   320,  -524,   860,   864,  -324,   942,   870,
     874,   154,  1092,   493,   168,   154,   154,  -524,   320,  -296,
    1114,   732,   733,  1111,  1142,   524,  -524,  -524,   631,   577,
    1211,   390,   225,   135,   540,   364,   529,   154,   531,   364,
     914,   154,   862,   702,  -524,   884,   215,  -522,   849,   776,
     390,   681,  -522,   628,   390,  1010,  -524,  1012,   634,   922,
     861,   685,   982,   708,  1142,   489,    24,  1050,   168,   885,
     916,   496,   527,   168,   258,   777,  1143,   230,   541,   686,
     875,  1173,   967,   594,  -272,   556,   391,   135,   886,  1197,
     688,   283,   135,   294,   924,   199,   803,   629,   501,   501,
    -524,  1171,   635,   923,  1160,   576,   876,  1176,   824,   391,
     509,   512,  -524,   296,   887,  1144,  1143,   298,    57,   267,
     925,   877,  1166,   828,  -262,  1145,   825,   276,   878,   276,
      66,   266,   215,   501,   215,   405,   276,   759,   789,  1074,
     524,   829,   299,  1090,   706,   300,   320,    59,   168,   566,
     173,   284,   154,    85,   305,   291,   406,   301,   290,   862,
     527,   499,   168,  -259,  -524,  1145,   407,   135,  -266,   763,
     285,   168,   776,   501,   173,   303,   276,  -270,   940,  1043,
     304,   135,   320,  1012,   168,   286,   607,   863,   294,   983,
     135,   441,   776,   780,   616,   953,   501,  1200,   954,   447,
     852,  1013,   527,   135,  1152,  1153,   154,   320,   296,   437,
     985,   154,   298,   864,   870,   874,   727,   728,   966,   968,
    1087,  1072,   729,   173,   682,   730,   731,   173,   173,  1105,
     821,   484,  1012,  -267,   -69,   485,   986,   299,   623,   763,
     300,  1139,    59,   499,   329,   763,   320,  1203,   685,   173,
    1109,   758,   301,   173,   -69,  -261,  1238,  1239,  1240,  1241,
    1242,  1243,   763,   335,   338,  1066,  1116,   176,   438,  1120,
     303,  1135,   439,   -69,  1204,   304,  1205,  1206,  -602,  1207,
    1208,  1025,   680,  -265,   740,   527,   154,    38,  1159,  1049,
    -335,   176,  1038,  -601,   437,   364,   305,   396,   310,   312,
     154,   -69,   330,  1117,  -602,  -271,   168,   195,   168,   154,
     370,  -335,  -273,   564,    55,   811,   811,     7,   441,  -601,
     702,  -335,   154,   445,   939,   135,   447,   135,   485,  1198,
     450,  1175,  1012,  -414,  -589,  1067,   195,  -260,   703,  1070,
     176,  -663,   822,  -663,   176,   176,   596,   482,  -739,   483,
    1225,   704,  -739,   438,  -664,   501,  -664,   439,   692,   734,
     855,   856,   177,  -740,   173,  -268,   176,  -740,   869,   873,
     176,   814,   199,  -741,  -736,   336,   336,  -741,  -736,   501,
     371,   705,   168,   566,   810,   813,   177,   320,   320,  -663,
    -660,  -663,  -660,   320,   538,   539,   320,   320,   320,   320,
    1212,   135,  1100,   441,  1102,   373,   398,   399,   445,   276,
     446,   447,  -662,   402,  -662,   450,   403,  -661,   173,  -661,
     432,   706,   457,   173,  1123,  1125,  1049,   707,  -586,   962,
     463,  -585,   904,   320,   905,   177,   433,  1131,  1134,   177,
     177,  -590,   988,   708,   154,   491,   154,   364,   375,  1212,
    -591,   276,   276,  -588,  -587,   294,   495,  1136,   498,   276,
     276,   177,  1140,   516,   294,   177,   441,   517,   545,   533,
     548,   445,   534,   320,   447,   296,   544,   557,   450,   298,
     554,   176,   437,   563,   296,   570,   320,   320,   298,   573,
     574,   579,  1019,   590,   501,   580,   320,   601,   173,  1123,
    1125,  1131,  1134,   617,   299,   683,   941,   300,   687,   357,
     447,   726,   173,   299,   739,   761,   300,   762,    59,   301,
     154,   173,   763,  1049,  1063,   958,   764,   765,   301,   767,
     770,   772,   773,   780,   173,   176,   501,   303,   790,   795,
     176,   438,   304,   804,   805,   439,   303,   850,   853,   883,
     857,   304,   499,   881,   937,   893,   895,   898,  -139,  -160,
    -167,  -166,  -165,  -162,  -140,   168,   913,  -169,   168,  -170,
    -164,  -168,  -141,   869,   873,   917,   177,   437,   920,  -163,
    -171,   899,   945,   947,   135,   964,  1021,   135,  1081,  1012,
     906,   441,   442,   907,   443,   444,   445,  1098,   446,   447,
     448,   501,   449,   450,   451,   452,   912,   454,   455,   456,
     457,   458,   908,   460,   972,   176,   461,   462,   463,   909,
     951,   910,   911,  1015,  1163,  1164,   921,   464,  1035,   176,
     177,  1048,  1040,   977,  1024,   177,   438,   305,   176,  1016,
     439,  1030,  1044,  1064,  1082,  1083,   305,  1065,  1085,  1086,
    1110,   176,  1022,   886,   979,   320,   173,   168,   173,  1151,
    1157,   168,  1154,  1161,   276,   276,   168,  1183,  1187,   168,
    1169,  1221,  1196,  1222,  1226,  1250,   135,   320,  1251,   320,
     135,  1202,   365,   817,  1104,   135,   441,   442,   135,   443,
     444,   445,   584,   446,   447,   448,   320,   987,   450,   514,
     226,   501,   501,   154,   456,   457,   154,   578,   460,  1097,
     177,   461,   462,   463,  1041,   562,   602,   793,   784,   276,
     599,  1054,   464,   276,   177,  1042,  1062,   409,  1059,  1235,
    1141,  1244,   173,   177,  1237,  1018,  1075,  1076,   903,   786,
     370,   379,   542,   168,   955,   168,   177,   410,   168,   783,
     222,   411,     0,   513,     0,  1073,     0,     0,     0,     0,
       0,     0,   135,     0,   135,   702,     0,   135,  1048,  1096,
       0,   168,     0,   176,     0,   176,   412,     0,     0,   413,
     926,   168,     0,   927,     0,     0,     0,     0,   928,     0,
    1095,   414,     0,     0,   320,   154,  1130,  1133,   197,   154,
     135,     0,     0,     0,   154,     0,     0,   154,   929,   415,
       0,     0,     0,     0,   416,   930,     0,   199,     0,     0,
     418,     0,   168,     0,     0,     0,   931,   370,   168,     0,
    1099,   370,     0,   182,   932,     0,   320,     0,     0,     0,
     419,   135,     0,     0,   420,     0,     0,   135,     0,   176,
       0,   933,     0,   513,  1168,   513,     0,   182,   513,   513,
    1130,  1133,   934,     0,     0,  1048,   706,     0,   177,   421,
     177,     0,   422,   935,     0,     0,     0,  1179,   936,   168,
       0,   154,     0,   154,   423,     0,   154,   276,   276,  1192,
       0,     0,  1195,     0,     0,     0,     0,     0,   135,     0,
       0,   320,   424,     0,     0,     0,   182,   425,     0,   154,
     182,   182,     0,     0,     0,   173,     0,     0,   173,   154,
     168,     0,   168,     0,     0,     0,     0,   168,     0,     0,
     168,     0,   182,     0,     0,     0,   182,     0,     0,   135,
       0,   135,     0,     0,   177,   276,   135,     0,     0,   135,
       0,   276,   276,     0,     0,     0,     0,     0,     0,   168,
     154,     0,     0,     0,     0,   168,   154,     0,   276,   168,
     168,     0,     0,     0,     0,     0,     0,   512,   135,     0,
     276,   168,     0,   276,   135,   168,     0,   168,   135,   135,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     135,   320,   320,     0,   135,     0,   135,   173,   168,  1245,
       0,   173,     0,     0,     0,     0,   173,   154,  1224,   173,
       0,     0,     0,     0,     0,     0,     0,   135,     0,     0,
       0,   437,   176,     0,     0,   176,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   182,     0,     0,
     168,   168,   168,   168,     0,     0,     0,     0,   154,     0,
     154,     0,     0,     0,     0,   154,     0,     0,   154,   135,
     135,   135,   135,     0,     0,     0,   184,   512,     0,   512,
       0,     0,   512,   512,     0,     0,     0,     0,     0,     0,
     438,     0,     0,   173,   439,   173,     0,   154,   173,     0,
     184,   182,     0,   154,     0,     0,   182,   154,   154,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   154,
       0,   173,     0,   154,   176,   154,     0,   177,   176,     0,
     177,   173,     0,   176,     0,     0,   176,     0,     0,     0,
     441,   442,     0,   443,   444,   445,   154,   446,   447,   184,
       0,     0,   450,   184,   184,     0,     0,     0,   456,   457,
       0,     0,   460,     0,     0,   461,   462,   463,     0,     0,
       0,     0,   173,     0,     7,   184,     0,     0,   173,   184,
       0,   182,     0,     0,     0,     0,     0,     0,   154,   154,
     154,   154,   359,     0,     0,   182,    23,    24,     0,     0,
       0,     0,     0,     0,   182,   360,     0,    30,   361,     0,
     176,     0,   176,    33,     0,   176,     0,   182,     0,   177,
      38,     0,     0,   177,     0,     0,     0,     0,   177,   173,
       0,   177,     0,     0,     0,     0,     0,     0,   176,     0,
       0,     0,   362,   437,     0,     0,     0,    55,   176,    57,
       0,    59,     0,  1045,     0,     0,  1046,     0,     0,   363,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
     173,     0,   173,     0,     0,     0,     0,   173,     0,    81,
     173,    83,     0,     0,    85,     0,     0,     0,     0,   176,
     184,     0,     0,     0,     0,   176,     0,     0,     0,     0,
       0,     0,   438,     0,     0,   177,   439,   177,     0,   173,
     177,     0,     0,     0,     0,   173,     0,     0,     0,   173,
     173,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   173,     0,   177,    99,   173,     0,   173,     0,   182,
    1047,   182,     0,   177,   184,     0,   176,     0,     0,   184,
     185,     0,   441,   442,     0,   443,   444,   445,   173,   446,
     447,     0,     0,     0,   450,     0,     0,     0,     0,     0,
       0,   457,     0,     0,   185,     0,     0,   461,   462,   463,
       0,     0,     0,     0,   177,     0,     0,   176,     0,   176,
     177,     0,     0,     0,   176,     0,     0,   176,     0,     0,
     173,   173,   173,   173,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   182,     0,     0,     0,     0,
       0,   256,     0,   185,   184,     0,   176,   185,   185,     0,
       0,     0,   176,   268,   269,   270,   176,   176,   184,     0,
       0,   177,     0,     0,     0,     0,   359,   184,   176,   185,
      23,    24,   176,   185,   176,     0,     0,     0,     0,   360,
     184,    30,   361,     0,     0,     0,     0,    33,     0,     0,
       0,     0,     0,     0,    38,   176,     0,  -335,     0,     0,
       0,    42,   177,     0,   177,     0,     0,     0,     0,   177,
     -68,     0,   177,     0,     0,     0,   362,     0,  -335,     0,
     140,    55,     0,    57,     0,     0,     0,    61,  -335,   -68,
      62,     0,     0,   363,     0,    66,     0,   176,   176,   176,
     176,   177,     0,     0,   140,     0,     0,   177,     0,     0,
       0,   177,   177,    81,     0,    83,     0,     0,    85,     0,
       0,     0,   401,   177,     0,     0,     0,   177,     0,   177,
     359,     0,     0,     0,    23,    24,   427,     0,     0,     0,
       0,     0,     0,   360,   185,    30,   361,     0,     0,     0,
     177,    33,     0,   140,     0,     0,     0,   430,    38,   231,
       0,     0,   184,     0,   184,     0,     0,     0,    99,     0,
       0,     0,   487,   364,   232,   233,     0,   234,   182,   140,
     362,   182,   235,   140,     0,    55,     0,    57,     0,   437,
     236,     0,   177,   177,   177,   177,   237,   363,   185,    66,
       0,     0,   238,   185,     0,     0,   239,     0,     0,   240,
       0,   241,     0,     0,     0,     0,     0,    81,     0,    83,
     242,     0,    85,     0,     0,     0,     0,   243,   244,     0,
       0,   506,   507,     0,     0,   245,     0,     0,   184,     0,
       0,     0,     0,   520,     0,   246,     0,     0,   438,     0,
       0,     0,   439,     0,   247,     0,   248,     0,   249,     0,
     250,     0,     0,   251,     0,     0,     0,   252,     0,   253,
     182,     0,   254,     0,   182,     0,     0,     0,   185,   182,
       0,     0,   182,   547,     0,     0,     0,     0,     0,     0,
       0,     0,   185,     0,   140,     0,     0,     0,   441,   442,
       0,   185,   444,   445,   879,   446,   447,     0,  -315,     0,
     450,     0,     0,     0,   185,     0,     0,   457,     0,     0,
       0,     0,     0,   461,   462,   463,  -315,     0,   271,     0,
    -315,  -315,   256,     0,     0,     0,     0,     0,     0,  -315,
       0,  -315,  -315,     0,   437,     0,     0,  -315,   140,     0,
       0,     0,     0,   140,  -315,     0,   182,  -315,   182,     0,
       0,   182,     0,     0,     0,     0,     0,   437,   612,     0,
     615,     0,     0,     0,   620,     0,  -315,     0,  -315,     0,
       0,  -315,     0,  -315,   182,  -315,     0,  -315,  -315,     0,
    -315,     0,     0,  -315,   182,  -315,     0,     0,   677,     0,
       0,     0,     0,   438,     0,     0,     0,   439,     0,     0,
       0,     0,     0,  -315,     0,  -315,     0,     0,  -315,     0,
       0,   184,     0,     0,   184,     0,   438,     0,   140,     0,
     439,     0,     0,     0,     0,   182,   185,     0,   185,     0,
       0,   182,   140,     0,     0,     0,     0,     0,     0,     0,
       0,   140,     0,   441,   442,     0,   443,   444,   445,     0,
     446,   447,   448,   743,   140,   450,   451,   452,  -315,   454,
     455,   456,   457,     0,  -315,   460,   441,   442,   461,   462,
     463,   445,     0,   446,   447,     0,     0,     0,   450,   464,
       0,   520,   182,     0,     0,   457,     0,     0,     0,     0,
       0,   461,   462,   463,   785,     0,     0,     0,     0,     0,
       0,     0,   185,   184,   791,     0,   231,   184,   520,     0,
       0,     0,   184,     0,     0,   184,     0,     0,     0,     0,
       0,   232,   233,   182,   234,   182,     0,     0,     0,   235,
     182,     0,     0,   182,     0,     0,     0,   236,     0,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,   238,
       0,     0,   833,   239,     0,     0,   240,     0,   241,     0,
       0,     0,   182,   854,     0,     0,     0,   242,   182,     0,
       0,     0,   182,   182,   243,   244,   140,     0,   140,     0,
       0,   620,   245,     0,   182,     0,     0,     0,   182,   184,
     182,   184,   246,     0,   184,     0,     0,     0,     0,     0,
       0,   247,     0,   248,     0,   249,     0,   250,     0,     0,
     251,   182,     0,     0,   252,     0,   253,   184,     0,   254,
       0,     0,     0,     0,     0,     0,     0,   184,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   427,     0,     0,
       0,     0,   140,   182,   182,   182,   182,     0,     0,     0,
       0,   231,     0,     0,     0,     0,     0,     0,   184,     0,
       0,     0,     0,   364,   184,   978,   232,   233,     0,   234,
       0,     0,     0,     0,   235,   185,   437,     0,   185,     0,
       0,     0,   236,     0,     0,     0,     0,     0,   237,     0,
       0,     0,     0,     0,   238,     0,     0,     0,   239,     0,
       0,   240,     0,   241,     0,     0,     0,     0,     0,     0,
       0,     0,   242,     0,     0,   184,     0,     0,     0,   243,
     244,   227,     0,     0,   952,     0,     0,   245,   520,     0,
       0,     0,     0,     0,     0,   438,     0,   246,   959,   439,
       0,     0,     0,     0,     0,     0,   247,     0,   248,     0,
     249,     0,   250,     0,     0,   251,   184,   980,   184,   252,
       0,   253,     0,   184,   254,     0,   184,   185,     0,     0,
     331,   185,     0,     0,     0,     0,   185,     0,     0,   185,
       0,     0,     0,     0,     0,   441,     0,     0,     0,     0,
     445,     0,   446,   447,     0,   184,     0,   450,     0,     0,
     358,   184,     0,   427,   457,   184,   184,     0,     0,     0,
     461,   462,   463,     0,     0,     0,     0,   184,     0,     0,
     271,   184,     0,   184,     0,   140,     0,   612,   140,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   184,   618,     0,   231,     0,     7,
       0,   677,     0,   185,     0,   185,     0,     0,   185,     0,
       0,     0,   232,   233,     0,   234,     0,     0,     0,     0,
     235,     0,     0,     0,     0,     0,     0,     0,   236,     0,
       0,   185,     0,     0,   237,     0,   184,   184,   184,   184,
     238,   185,     0,     0,   239,     0,     0,   240,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,   242,     0,
       0,   494,     0,     0,     0,   243,   244,   140,     0,     0,
       0,   140,     0,   245,     0,     0,   140,     0,     0,   140,
     980,     0,   185,   246,     0,     0,     0,     0,   185,   211,
       0,     0,   247,   221,   248,     0,   249,     0,   250,     0,
       0,   251,   229,     0,     0,   252,     0,   253,     0,   262,
     254,     0,     0,     0,     0,   518,     0,     0,     0,     0,
     528,   278,     0,   281,   282,     0,     0,     0,     0,   287,
       0,   288,     0,   289,     0,     0,     0,     0,   315,   185,
       0,     0,   229,   323,   325,   327,   328,     0,     0,     0,
     332,     0,   333,   140,   620,   140,     0,     0,   140,   340,
       0,     0,   323,     0,   342,     0,   343,     0,     0,   344,
     345,     0,   346,     0,     0,     0,   229,   323,   356,     0,
     185,   140,   185,     0,     0,     0,     0,   185,     0,     0,
     185,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   380,     0,   383,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   185,
       0,     0,     0,     0,     0,   185,     0,     0,     0,   185,
     185,   600,   140,     0,     0,     0,     0,     0,   140,     0,
       0,   185,   620,     0,     0,   185,     0,   185,   231,     0,
     434,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   232,   233,     0,   234,     0,   185,     0,
       0,   235,     0,   486,     0,     0,     0,   332,   229,   236,
       0,     0,     0,     0,     0,   237,     0,     0,     0,   140,
       0,   238,     0,     0,     0,   239,     0,     0,   240,     0,
     241,     0,     0,     0,     0,   323,     0,     0,     0,   242,
     185,   185,   185,   185,     0,     0,   243,   244,     0,     0,
       0,     0,     0,     0,   245,     0,     0,     0,     0,     0,
     140,     0,   140,     0,   246,     0,     0,   140,     0,     0,
     140,   323,     0,   247,   435,   248,     0,   249,     0,   250,
       0,   436,   251,   766,     0,   768,   252,     0,   253,     0,
       0,   254,     0,   437,   275,     0,   323,     0,     0,   140,
       0,     0,     0,     0,     0,   140,     0,   332,     0,   140,
     140,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   140,   550,   551,     0,   140,     0,   140,     0,     0,
       0,     0,     0,     0,     0,   323,     0,     0,     0,   560,
       0,     0,     0,     0,     0,     0,     0,   519,   140,     0,
       0,     0,   438,     0,     0,     0,   439,   569,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   575,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   587,
       0,   589,     0,     0,     0,   440,     0,     0,     0,     0,
     140,   140,   140,   140,     0,   603,   608,   611,     0,     0,
       0,     0,   441,   442,   229,   443,   444,   445,     0,   446,
     447,   448,     0,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   460,     0,     0,   461,   462,   463,
       0,   364,     0,     0,     0,     0,     0,     0,   464,     0,
     689,   690,   691,   693,   695,   696,     0,   697,   698,   699,
     700,   701,     0,   709,   710,   711,   712,   713,   714,   715,
     716,   717,   718,   719,   720,   721,   722,   723,   724,   725,
       0,     0,     0,     0,     0,     0,   323,   323,     0,     0,
       0,     0,   323,     0,     0,   323,   323,   323,   323,   735,
     737,     0,     0,     0,   608,   742,     0,     0,   757,     0,
       0,     0,     0,     0,   760,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   769,     0,     0,   771,     0,
       0,     0,   323,     0,     0,     0,     0,     0,     0,   779,
       0,   782,     0,     0,     0,   281,     0,     0,   787,   788,
     231,     0,   946,     0,     0,   949,     0,     0,     0,     0,
     315,     0,     0,   315,     0,   232,   233,     0,   234,     0,
       0,     0,   323,   235,     0,     0,     0,     0,     0,     0,
       0,   236,   760,     0,     0,   323,   323,   237,     0,     0,
       0,   229,   816,   238,     0,   323,     0,   239,     0,     0,
     240,     0,   241,     0,     0,     0,   846,   846,     0,     0,
       0,   242,     0,     0,     0,     0,     0,     0,   243,   244,
     858,     0,     0,     0,     0,     0,   245,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   246,     0,     0,     7,
       0,     0,     0,     0,  1023,   247,   315,   248,  1028,   249,
       0,   250,     0,   528,   251,     0,   528,   359,   252,     0,
     253,    23,    24,   254,     0,     0,     0,     0,     0,     0,
     360,     0,    30,   361,     0,     0,     0,     0,    33,     0,
       0,     0,     0,     0,     0,    38,     0,     0,     0,     0,
       0,   897,     0,     0,     0,     0,     0,     0,   902,     0,
       0,     0,     0,     0,     0,     0,     0,   362,     0,     0,
       0,     0,    55,     0,    57,     0,    59,     0,  1045,   546,
       0,  1046,     0,     0,   363,     0,    66,     0,     0,     0,
    1077,     0,  1078,     0,     0,  1080,     0,     0,     0,     0,
       0,     0,     0,     0,    81,     0,    83,     0,     0,    85,
       0,     7,     0,   769,   323,     0,     0,     0,  1094,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1028,   359,
       0,     0,     0,    23,    24,     0,   323,     0,   323,   943,
     944,     0,   360,     0,    30,   361,     0,     0,     0,     0,
      33,     0,     0,     0,     0,   323,     0,    38,     0,    99,
       0,     0,     0,     0,     0,  1115,     0,     0,     0,  1121,
     315,     0,     0,     0,   971,  1126,     0,     0,     0,   362,
       0,     0,     0,     0,    55,     0,    57,     0,   357,     0,
    1045,     0,     0,  1046,     0,     0,   363,     0,    66,   587,
       0,     0,     0,  1008,     0,  1009,   846,   846,     0,   342,
       0,   343,     0,     0,   345,   346,    81,     0,    83,     0,
       0,    85,     0,     0,     0,     0,  1162,     0,  1020,     0,
       0,     0,   229,     0,     0,     0,  1026,   760,  1029,     0,
    1031,  1033,     0,  1034,     0,     0,  1037,  1039,     0,     0,
       0,     0,     0,   323,     0,     0,     0,     0,  1051,     0,
       0,     0,     0,     0,     0,     0,     0,  1181,     0,  1185,
       0,    99,     0,     0,   528,     0,     0,   528,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1068,  1029,  1034,  1037,  1071,   323,     0,     0,     0,   757,
       0,     0,     0,     0,     0,     0,  1220,     0,     0,     0,
       0,     0,  1223,   693,   735,     0,  1181,  1185,     0,     0,
       0,     0,     0,  1079,     0,     0,     0,     0,  1229,     0,
       0,     0,  1231,     0,   768,     0,     0,     0,     0,   315,
       7,     0,     0,  1093,     0,     0,     0,     0,     0,   229,
       0,     0,     0,     0,     0,     0,     0,  1101,   359,  1103,
     323,     0,    23,    24,     0,     0,     0,     0,     0,     0,
       0,   360,     0,    30,   361,     0,     0,     0,     0,    33,
     846,     0,     0,     0,     0,     0,    38,  1112,     0,  1113,
       0,     0,     0,     0,     0,     0,  1119,   768,   768,   768,
     768,     0,     0,     0,     0,     0,     0,     0,   362,     0,
       0,  1128,     0,    55,     0,    57,     0,    59,     0,  1045,
       0,     0,  1046,     0,     0,   363,     0,    66,     0,     0,
       0,     0,  1149,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     0,     0,  1156,    81,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,   359,
     323,   323,     0,    23,    24,     0,     0,     0,     0,     0,
       0,     0,   360,  1170,    30,   361,     0,     0,     0,     0,
      33,  1174,   846,   846,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1182,     0,  1186,
      99,     0,  1188,     0,  1191,     0,  1178,  1194,     0,   362,
       0,     0,     0,     0,    55,     0,    57,     0,     0,     0,
    1045,     0,     0,  1046,     0,     0,   363,     0,    66,  1182,
    1186,     0,  1191,  1194,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    81,     0,    83,     0,
       0,    85,  1227,     0,     0,  1228,     0,     0,     0,  1230,
       0,     0,     0,     0,  1232,  1233,     0,     0,  1234,     0,
       0,     0,     0,     0,     0,     0,  1236,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1246,  1247,  1232,  1248,  1249,     0,     0,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1260,  1261,  1262,
    1263,  1260,  1261,  1262,  1263,    -2,     4,     0,     5,     6,
       7,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,    15,    16,    17,    18,     0,    19,     0,    20,    21,
    -209,    22,    23,    24,    25,    26,    27,     0,  -209,    28,
       0,    29,     0,    30,    31,    32,  -209,  -209,  -209,    33,
      34,    35,   -68,     0,    36,    37,    38,     0,    39,  -335,
      40,     0,    41,    42,    43,    44,    45,     0,    46,    47,
      48,    49,   -68,    50,    51,     0,    52,    53,    54,     0,
    -335,     0,     0,    55,    56,    57,    58,    59,    60,    61,
    -335,   -68,    62,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,    83,    84,   -68,
      85,    86,     0,     0,  -209,     0,    87,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
      99,     0,     0,   100,     0,   101,     0,   102,     0,  -209,
       0,     4,   103,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     0,  -691,     0,    14,    15,    16,    17,    18,
    -691,    19,     0,    20,    21,  -209,    22,    23,    24,    25,
      26,    27,  -691,  -209,    28,  -691,    29,     0,    30,    31,
      32,  -209,  -209,  -209,    33,    34,    35,   -68,     0,    36,
      37,    38,     0,    39,  -335,    40,     0,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,   -68,    50,    51,
       0,    52,    53,    54,     0,  -335,     0,     0,    55,    56,
      57,    58,     0,    60,    61,  -335,   -68,    62,    63,    64,
      65,  -691,    66,    67,    68,  -691,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,    83,    84,   -68,    85,    86,     0,     0,  -209,
       0,    87,     0,     0,  -691,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,  -691,    91,  -691,  -691,  -691,  -691,  -691,  -691,
    -691,     0,  -691,  -691,  -691,  -691,  -691,  -691,  -691,  -691,
    -691,  -691,  -691,  -691,    98,    99,  -691,  -691,  -691,     0,
     101,  -691,   102,     0,  -209,     0,   347,  -691,     5,     6,
       7,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,    15,    16,    17,    18,     0,    19,     0,    20,    21,
    -209,    22,    23,    24,    25,    26,    27,     0,  -209,    28,
       0,    29,     0,    30,    31,    32,  -209,  -209,  -209,    33,
      34,    35,   -68,     0,    36,    37,    38,     0,    39,  -335,
      40,     0,    41,    42,    43,    44,    45,     0,    46,    47,
      48,    49,   -68,    50,    51,     0,    52,    53,    54,     0,
    -335,     0,     0,    55,    56,    57,    58,    59,    60,    61,
    -335,   -68,    62,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,    83,    84,   -68,
      85,    86,     0,     0,  -209,     0,    87,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
      99,     0,     0,   100,     0,   101,   348,   102,     0,  -209,
       0,     4,   103,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,    15,    16,    17,    18,
       0,    19,     0,    20,    21,  -209,    22,    23,    24,    25,
      26,    27,     0,  -209,    28,     0,    29,     0,    30,    31,
      32,  -209,  -209,  -209,    33,    34,    35,   -68,     0,    36,
      37,    38,     0,    39,  -335,    40,     0,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,   -68,    50,    51,
       0,    52,    53,    54,     0,  -335,     0,     0,    55,    56,
      57,    58,    59,    60,    61,  -335,   -68,    62,    63,    64,
      65,     0,    66,    67,    68,     0,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,    83,    84,   -68,    85,    86,     0,     0,  -209,
       0,    87,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,    99,     0,     0,   100,     0,
     101,   565,   102,     0,  -209,     0,   582,   103,     5,     6,
       7,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,    15,    16,    17,    18,     0,    19,     0,    20,    21,
    -209,    22,    23,    24,    25,    26,    27,     0,  -209,    28,
       0,    29,     0,    30,    31,    32,  -209,  -209,  -209,    33,
      34,    35,   -68,     0,    36,    37,    38,     0,    39,  -335,
      40,     0,    41,    42,    43,    44,    45,     0,    46,    47,
      48,    49,   -68,    50,    51,     0,    52,    53,    54,     0,
    -335,     0,     0,    55,    56,    57,    58,    59,    60,    61,
    -335,   -68,    62,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,    83,    84,   -68,
      85,    86,     0,     0,  -209,     0,    87,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
      99,     0,     0,   100,     0,   101,   583,   102,     0,  -209,
       0,     4,   103,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,    15,    16,    17,    18,
       0,    19,     0,    20,    21,  -209,    22,    23,    24,    25,
      26,    27,     0,  -209,    28,     0,    29,     0,    30,    31,
      32,  -209,  -209,  -209,    33,    34,    35,   -68,     0,    36,
      37,    38,     0,    39,  -335,    40,     0,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,   -68,    50,    51,
       0,    52,    53,    54,     0,  -335,     0,     0,    55,    56,
      57,    58,    59,    60,    61,  -335,   -68,    62,    63,    64,
      65,     0,    66,    67,    68,     0,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,    83,    84,   -68,    85,    86,     0,     0,  -209,
       0,    87,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,    99,     0,     0,   100,     0,
     101,   823,   102,     0,  -209,     0,     4,   103,     5,     6,
       7,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,    15,    16,    17,    18,     0,    19,     0,    20,    21,
    -209,    22,    23,    24,    25,    26,    27,     0,  -209,    28,
       0,    29,     0,    30,    31,    32,  -209,  -209,  -209,    33,
      34,    35,   -68,     0,    36,    37,    38,     0,    39,  -335,
      40,     0,    41,    42,    43,    44,    45,     0,    46,    47,
      48,    49,   -68,    50,    51,     0,    52,    53,    54,     0,
    -335,     0,     0,    55,    56,    57,    58,   357,    60,    61,
    -335,   -68,    62,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,    83,    84,   -68,
      85,    86,     0,     0,  -209,     0,    87,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
      99,     0,     0,   100,     0,   101,     0,   102,     0,  -209,
       0,     4,   103,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,    15,    16,    17,    18,
       0,    19,     0,    20,    21,  -209,    22,    23,    24,    25,
      26,    27,     0,  -209,    28,     0,    29,     0,    30,    31,
      32,  -209,  -209,  -209,    33,    34,    35,   -68,     0,    36,
      37,    38,     0,    39,  -335,    40,     0,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,   -68,    50,    51,
       0,    52,    53,    54,     0,  -335,     0,     0,    55,    56,
      57,    58,    59,    60,    61,  -335,   -68,    62,    63,    64,
      65,     0,    66,    67,    68,     0,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,    83,    84,   -68,    85,    86,     0,     0,  -209,
       0,    87,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,    99,     0,     0,   100,     0,
     101,     0,   102,     0,  -209,     0,     4,   103,     5,     6,
       7,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,    15,    16,    17,    18,     0,    19,     0,    20,    21,
    -209,    22,    23,    24,    25,    26,    27,     0,  -209,    28,
       0,    29,     0,    30,    31,    32,  -209,  -209,  -209,    33,
      34,    35,   -68,     0,    36,    37,    38,     0,    39,  -335,
      40,     0,    41,    42,    43,    44,    45,     0,    46,    47,
      48,    49,   -68,    50,    51,     0,    52,    53,    54,     0,
    -335,     0,     0,    55,    56,    57,    58,     0,    60,    61,
    -335,   -68,    62,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,    83,    84,   -68,
      85,    86,     0,     0,  -209,     0,    87,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
      99,     0,     0,   100,     0,   101,     0,   102,     0,  -209,
       0,     4,   103,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,    15,    16,    17,    18,
       0,    19,     0,    20,    21,  -209,    22,    23,    24,    25,
      26,    27,     0,  -209,    28,     0,    29,     0,    30,    31,
      32,  -209,  -209,  -209,    33,  1036,    35,   -68,     0,    36,
      37,    38,     0,    39,  -335,    40,     0,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,   -68,    50,    51,
       0,    52,    53,    54,     0,  -335,     0,     0,    55,    56,
      57,    58,     0,    60,    61,  -335,   -68,    62,    63,    64,
      65,     0,    66,    67,    68,     0,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,    83,    84,   -68,    85,    86,     0,     0,  -209,
       0,    87,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,    99,     0,     0,   100,     0,
     101,     0,   102,     0,  -209,     0,     4,   103,     5,     6,
       7,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,    15,    16,    17,    18,     0,    19,     0,    20,    21,
    -209,    22,    23,    24,    25,    26,    27,     0,  -209,    28,
       0,    29,     0,    30,    31,    32,  -209,  -209,  -209,    33,
    1180,    35,   -68,     0,    36,    37,    38,     0,    39,  -335,
      40,     0,    41,    42,    43,    44,    45,     0,    46,    47,
      48,    49,   -68,    50,    51,     0,    52,    53,    54,     0,
    -335,     0,     0,    55,    56,    57,    58,     0,    60,    61,
    -335,   -68,    62,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,    83,    84,   -68,
      85,    86,     0,     0,  -209,     0,    87,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
      99,     0,     0,   100,     0,   101,     0,   102,     0,  -209,
       0,     4,   103,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,    15,    16,    17,    18,
       0,    19,     0,    20,    21,  -209,    22,    23,    24,    25,
      26,    27,     0,  -209,    28,     0,    29,     0,    30,    31,
      32,  -209,  -209,  -209,    33,  1184,    35,   -68,     0,    36,
      37,    38,     0,    39,  -335,    40,     0,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,   -68,    50,    51,
       0,    52,    53,    54,     0,  -335,     0,     0,    55,    56,
      57,    58,     0,    60,    61,  -335,   -68,    62,    63,    64,
      65,     0,    66,    67,    68,     0,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,    83,    84,   -68,    85,    86,     0,     0,  -209,
       0,    87,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,    99,     0,     0,   100,     0,
     101,     0,   102,     0,  -209,     0,     4,   103,     5,     6,
       7,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,    15,    16,    17,    18,     0,    19,     0,    20,    21,
    -209,    22,    23,    24,    25,    26,    27,     0,  -209,    28,
       0,    29,     0,    30,    31,    32,  -209,  -209,  -209,    33,
    1190,    35,   -68,     0,    36,    37,    38,     0,    39,  -335,
      40,     0,    41,    42,    43,    44,    45,     0,    46,    47,
      48,    49,   -68,    50,    51,     0,    52,    53,    54,     0,
    -335,     0,     0,    55,    56,    57,    58,     0,    60,    61,
    -335,   -68,    62,    63,    64,    65,     0,    66,    67,    68,
       0,    69,    70,    71,    72,    73,    74,    75,     0,    76,
      77,     0,    78,    79,    80,    81,    82,    83,    84,   -68,
      85,    86,     0,     0,  -209,     0,    87,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
      99,     0,     0,   100,     0,   101,     0,   102,     0,  -209,
       0,     4,   103,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,    15,    16,    17,    18,
       0,    19,     0,    20,    21,  -209,    22,    23,    24,    25,
      26,    27,     0,  -209,    28,     0,    29,     0,    30,    31,
      32,  -209,  -209,  -209,    33,  1193,    35,   -68,     0,    36,
      37,    38,     0,    39,  -335,    40,     0,    41,    42,    43,
      44,    45,     0,    46,    47,    48,    49,   -68,    50,    51,
       0,    52,    53,    54,     0,  -335,     0,     0,    55,    56,
      57,    58,     0,    60,    61,  -335,   -68,    62,    63,    64,
      65,     0,    66,    67,    68,     0,    69,    70,    71,    72,
      73,    74,    75,     0,    76,    77,     0,    78,    79,    80,
      81,    82,    83,    84,   -68,    85,    86,     0,     0,  -209,
       0,    87,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,    99,     0,     0,   100,     0,
     101,     0,   102,   744,  -209,     5,     6,   103,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   196,     0,  -209,    22,     0,
     745,     0,     0,     0,     0,     0,    28,     0,   197,     0,
       0,     0,    32,  -209,  -209,  -209,     0,   198,    35,     0,
     746,     0,    37,     0,   747,    39,     0,   199,     0,   200,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,   748,
       0,    56,   749,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   201,     0,   750,     0,     0,     0,     0,     0,
      71,    72,    73,    74,   202,     0,    76,     0,     0,    78,
       0,     0,   751,    82,     0,    84,     0,   752,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,     0,     0,     0,
     100,     0,   203,     0,   102,   753,  -209,     0,   889,   103,
       5,     6,     0,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     196,     0,  -209,    22,     0,     0,     0,     0,     0,     0,
       0,   295,     0,   197,     0,     0,     0,    32,  -209,  -209,
    -209,     0,   198,    35,     0,     0,     0,   297,     0,     0,
      39,     0,   199,     0,   200,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   201,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,   302,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,     0,     0,     0,   100,     0,   203,     0,   102,
       0,  -209,     5,     6,   103,     8,     9,    10,    11,    12,
      13,     0,   638,     0,    14,     0,     0,    17,    18,   640,
      19,     0,   196,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   197,     0,     0,     0,    32,
       0,     0,     0,     0,   198,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   199,   646,   200,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   201,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,   202,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,   511,   466,   467,   468,   469,   470,
       0,     0,   473,   474,   475,   476,     0,   478,   479,     0,
     834,   835,   836,   837,   838,   659,     0,   660,     0,    94,
       0,   661,   662,   663,   664,   665,   666,   667,   839,   669,
     670,    96,   840,    98,   -90,   672,   673,   841,   675,   203,
       0,   102,     0,     0,     5,     6,   103,     8,     9,    10,
      11,    12,    13,     0,   638,     0,    14,     0,     0,    17,
      18,   640,    19,     0,   196,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   197,     0,     0,
       0,    32,     0,     0,     0,     0,   198,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   199,   646,   200,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   201,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,   202,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,   511,   466,   467,   468,
     469,   470,     0,     0,   473,   474,   475,   476,     0,   478,
     479,     0,   834,   835,   836,   837,   838,   659,     0,   660,
       0,    94,     0,   661,   662,   663,   664,   665,   666,   667,
     839,   669,   670,    96,   840,    98,     0,   672,   673,   841,
     675,   203,     0,   102,     0,     0,     0,     0,   103,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,    15,     0,    17,    18,     0,    19,     0,   196,
      21,     0,    22,     0,     0,     0,     0,    27,     0,     0,
      28,     0,   197,     0,     0,     0,    32,     0,     0,     0,
       0,    34,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   199,     0,   200,     0,     0,    44,    45,     0,    46,
      47,    48,    49,     0,    50,    51,     0,    52,    53,   334,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   201,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,    74,    75,     0,
      76,    77,     0,    78,     0,     0,     0,    82,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   101,     0,   102,     0,
       0,     5,     6,   103,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   196,     0,     0,    22,     0,   745,     0,     0,     0,
       0,     0,    28,     0,   197,     0,     0,     0,    32,     0,
       0,     0,     0,   198,    35,     0,   746,     0,    37,     0,
     747,    39,     0,   199,     0,   200,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,   748,     0,    56,   749,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   201,     0,
     750,     0,     0,     0,     0,     0,    71,    72,    73,    74,
     202,     0,    76,     0,     0,    78,     0,     0,   751,    82,
       0,    84,     0,   752,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,     0,     0,     0,     0,    94,     0,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
      96,    97,    98,     0,     0,     0,   100,     0,   203,     0,
     102,     0,     0,     5,     6,   103,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   196,     0,     0,    22,   378,    24,     0,
       0,     0,     0,     0,    28,     0,   197,     0,     0,     0,
      32,     0,     0,     0,     0,   198,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   199,     0,   200,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,   362,     0,     0,     0,     0,     0,    56,
      57,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     201,     0,    66,     0,     0,     0,     0,     0,    71,    72,
      73,    74,   202,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,    84,     0,    85,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     203,     0,   102,     0,     0,     5,     6,   103,     8,     9,
      10,   381,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,    20,     0,  -209,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   197,     0,
       0,     0,    32,  -209,  -209,  -209,     0,   198,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   199,     0,   200,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,    65,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,   202,     0,    76,     0,     0,    78,
       0,     0,     0,    82,    83,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,     0,     0,     0,
     100,     0,   203,     0,   102,     0,  -209,     5,     6,   103,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   196,     0,  -209,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     197,     0,     0,     0,    32,  -209,  -209,  -209,     0,   198,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   199,
       0,   200,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   201,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,   202,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   203,     0,   102,     0,  -209,     5,
       6,   103,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   196,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   197,     0,     0,     0,    32,     0,     0,     0,
       0,   198,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   199,     0,   200,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   201,  -735,     0,     0,
       0,  -735,     0,     0,    71,    72,    73,    74,   202,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,  -377,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,  -377,     0,     0,   100,     0,   203,  -377,   102,     0,
       0,     5,     6,   103,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   196,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   197,     0,     0,     0,    32,     0,
       0,     0,     0,   198,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   199,     0,   200,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   201,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
     202,     0,    76,     0,     0,    78,   317,   318,     0,    82,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,     0,     0,     0,     0,    94,     0,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
      96,    97,    98,   319,     0,     0,   100,     0,   203,     0,
     102,     0,     0,     5,     6,   103,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   196,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   197,     0,     0,     0,
      32,     0,     0,     0,     0,   198,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   199,     0,   200,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     201,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,   202,     0,    76,     0,     0,    78,   317,   318,
       0,    82,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,   352,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     203,     0,   102,     0,     0,     5,     6,   103,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   196,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   197,     0,
       0,     0,    32,     0,     0,     0,     0,   198,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   199,     0,   200,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   201,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,   202,     0,    76,     0,     0,    78,
     317,   318,     0,    82,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,     0,     0,     0,
     100,     0,   203,     0,   102,  -538,     0,     5,     6,   103,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   196,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     197,     0,     0,     0,    32,     0,     0,     0,     0,   198,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   199,
       0,   200,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   201,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,   202,     0,    76,     0,
       0,    78,   317,   318,     0,    82,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   203,     0,   102,     0,     0,  -538,
       0,   103,     5,     6,     0,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   196,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   197,     0,     0,     0,    32,
       0,     0,     0,     0,   198,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   199,     0,   200,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   201,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,   202,     0,    76,     0,     0,    78,   317,   318,     0,
      82,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   203,
       0,   102,   809,     0,     5,     6,   103,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   196,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   197,     0,     0,
       0,    32,     0,     0,     0,     0,   198,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   199,     0,   200,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   201,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,   202,     0,    76,     0,     0,    78,   317,
     318,     0,    82,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   203,     0,   102,   812,     0,     5,     6,   103,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   196,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   197,
       0,     0,     0,    32,     0,     0,     0,     0,   198,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   199,     0,
     200,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   201,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,   202,     0,    76,     0,     0,
      78,   317,   318,     0,    82,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    91,    92,    93,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,    96,    97,    98,     0,     0,
       0,   100,     0,   203,     0,   102,   818,     0,     5,     6,
     103,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   196,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   197,     0,     0,     0,    32,     0,     0,     0,     0,
     198,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     199,     0,   200,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,   957,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   201,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,   202,     0,    76,
       0,     0,    78,   317,   318,     0,    82,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   203,     0,   102,     0,     0,
       5,     6,   103,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     196,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   197,     0,     0,     0,    32,     0,     0,
       0,     0,   198,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   199,     0,   200,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   201,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,   202,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,  -385,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,  -385,     0,     0,   100,     0,   203,  -385,   102,
       0,     0,     5,     6,   103,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   196,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   197,     0,     0,     0,    32,
       0,     0,     0,     0,   198,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   199,     0,   200,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   201,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,   202,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,  -370,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,  -370,     0,     0,   100,     0,   203,
    -370,   102,     0,     0,     5,     6,   103,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   196,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   197,     0,     0,
       0,    32,     0,     0,     0,     0,   198,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   199,     0,   200,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   201,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,   202,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,  -378,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,  -378,     0,     0,   100,
       0,   203,  -378,   102,     0,     0,     5,     6,   103,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   196,     0,     0,    22,
       0,   259,     0,     0,     0,     0,     0,    28,     0,   197,
       0,     0,     0,    32,     0,     0,     0,     0,   198,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   199,     0,
     200,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   201,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,   202,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,    84,     0,   260,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    91,    92,    93,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,    96,    97,    98,     0,     0,
       0,   100,     0,   203,     0,   102,     0,     0,     5,     6,
     103,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   196,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   197,     0,     0,     0,    32,     0,     0,     0,     0,
     198,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     199,     0,   200,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   201,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,   202,     0,    76,
       0,     0,    78,   317,   318,     0,    82,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   203,     0,   102,     0,     0,
       5,     6,   103,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     196,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   197,     0,     0,     0,    32,     0,     0,
       0,     0,   198,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   199,     0,   200,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   201,  -735,     0,
       0,     0,  -735,     0,     0,    71,    72,    73,    74,   202,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,     0,     0,     0,   100,     0,   203,     0,   102,
       0,     0,     5,     6,   103,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   196,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   197,     0,     0,     0,    32,
       0,     0,     0,     0,   198,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   199,     0,   200,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   201,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,   202,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,   604,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   203,
       0,   102,     0,     0,   605,     0,   103,     5,     6,     0,
       8,     9,    10,    11,    12,    13,     0,     0,     0,    14,
       0,     0,    17,    18,     0,    19,     0,   196,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     197,     0,     0,     0,    32,     0,     0,     0,     0,   198,
      35,     0,     0,     0,    37,     0,     0,    39,     0,   199,
       0,   200,     0,     0,    44,    45,     0,     0,    47,     0,
      49,     0,    50,    51,     0,    52,    53,     0,     0,     0,
       0,     0,     0,    56,     0,    58,     0,    60,     0,     0,
       0,     0,    63,    64,   201,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,   202,     0,    76,     0,
       0,    78,     0,     0,     0,    82,     0,    84,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,  -406,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   203,     0,   102,  -406,     0,     5,
       6,   103,     8,     9,    10,    11,    12,    13,     0,     0,
       0,    14,     0,     0,    17,    18,     0,    19,     0,   196,
       0,     0,    22,     0,     0,     0,     0,     0,     0,   275,
      28,     0,   197,     0,     0,     0,    32,     0,     0,     0,
       0,   198,    35,     0,     0,     0,    37,     0,     0,    39,
       0,   199,     0,   200,     0,     0,    44,    45,     0,     0,
      47,     0,    49,     0,    50,    51,     0,    52,    53,     0,
       0,     0,     0,     0,     0,    56,     0,    58,     0,    60,
       0,     0,     0,     0,    63,    64,   201,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    73,    74,   202,     0,
      76,     0,     0,    78,     0,     0,     0,    82,     0,    84,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   101,     0,   102,     0,
       0,     5,     6,   103,     8,     9,    10,    11,    12,    13,
       0,     0,     0,    14,     0,     0,    17,    18,     0,    19,
       0,   196,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   197,     0,     0,     0,    32,     0,
       0,     0,     0,   198,    35,     0,     0,     0,    37,     0,
       0,    39,     0,   199,     0,   200,     0,     0,    44,    45,
       0,     0,    47,     0,    49,     0,    50,    51,     0,    52,
      53,     0,     0,     0,     0,     0,     0,    56,     0,    58,
       0,    60,     0,     0,     0,     0,    63,    64,   201,     0,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    74,
     202,     0,    76,     0,     0,    78,     0,     0,     0,    82,
       0,    84,     0,     0,    86,     0,     0,     0,     0,     0,
     604,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,    91,    92,    93,     0,     0,     0,     0,    94,     0,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
      96,    97,    98,     0,     0,     0,   100,     0,   203,     0,
     102,     0,     0,     5,     6,   103,     8,     9,    10,    11,
      12,    13,     0,     0,     0,    14,     0,     0,    17,    18,
       0,    19,     0,   196,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   197,     0,     0,     0,
      32,     0,     0,     0,     0,   198,    35,     0,     0,     0,
      37,     0,     0,    39,     0,   199,     0,   200,     0,     0,
      44,    45,     0,     0,    47,     0,    49,     0,    50,    51,
       0,    52,    53,     0,     0,     0,     0,     0,     0,    56,
       0,    58,     0,    60,     0,     0,     0,     0,    63,    64,
     201,     0,     0,     0,     0,     0,     0,     0,    71,    72,
      73,    74,   202,     0,    76,     0,     0,    78,     0,     0,
       0,    82,     0,    84,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,    91,    92,    93,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,    97,    98,     0,     0,     0,   100,     0,
     203,     0,   102,     0,     0,   605,     0,   103,     5,     6,
       0,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   196,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   197,     0,     0,     0,    32,     0,     0,     0,     0,
     198,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     199,     0,   200,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   201,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,   202,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   203,   807,   102,     0,     0,
       5,     6,   103,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     196,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   197,     0,     0,     0,    32,     0,     0,
       0,     0,   198,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   199,     0,   200,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   201,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,   202,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,     0,     0,     0,   100,     0,   203,     0,   102,
       0,     0,  1027,     0,   103,     5,     6,     0,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,     0,     0,
      17,    18,     0,    19,     0,   196,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   197,     0,
       0,     0,    32,     0,     0,     0,     0,   198,    35,     0,
       0,     0,    37,     0,     0,    39,     0,   199,     0,   200,
       0,     0,    44,    45,     0,     0,    47,     0,    49,     0,
      50,    51,     0,    52,    53,     0,     0,     0,     0,     0,
       0,    56,     0,    58,     0,    60,     0,     0,     0,     0,
      63,    64,   201,     0,     0,     0,     0,     0,     0,     0,
      71,    72,    73,    74,   202,     0,    76,     0,     0,    78,
       0,     0,     0,    82,     0,    84,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,    91,    92,    93,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,    96,    97,    98,     0,     0,     0,
     100,     0,   203,     0,   102,     0,     0,  1032,     0,   103,
       5,     6,     0,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     196,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   197,     0,     0,     0,    32,     0,     0,
       0,     0,   198,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   199,     0,   200,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   201,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,   202,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,     0,     0,     0,   100,     0,   203,     0,   102,
       0,     0,     5,     6,   103,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   196,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,   295,     0,   197,     0,     0,     0,    32,
       0,     0,     0,     0,   198,    35,     0,     0,     0,   297,
       0,     0,    39,     0,   199,     0,   200,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   201,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,   302,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   203,
       0,   102,     0,     0,     5,     6,   103,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,    17,
      18,     0,    19,     0,   196,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   197,     0,     0,
       0,    32,     0,     0,     0,     0,  1069,    35,     0,     0,
       0,    37,     0,     0,    39,     0,   199,     0,   200,     0,
       0,    44,    45,     0,     0,    47,     0,    49,     0,    50,
      51,     0,    52,    53,     0,     0,     0,     0,     0,     0,
      56,     0,    58,     0,    60,     0,     0,     0,     0,    63,
      64,   201,     0,     0,     0,     0,     0,     0,     0,    71,
      72,    73,    74,   202,     0,    76,     0,     0,    78,     0,
       0,     0,    82,     0,    84,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   203,     0,   102,     0,     0,     5,     6,   103,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,    17,    18,     0,    19,     0,   196,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   197,
       0,     0,     0,    32,     0,     0,     0,     0,  1215,    35,
       0,     0,     0,    37,     0,     0,    39,     0,   199,     0,
     200,     0,     0,    44,    45,     0,     0,    47,     0,    49,
       0,    50,    51,     0,    52,    53,     0,     0,     0,     0,
       0,     0,    56,     0,    58,     0,    60,     0,     0,     0,
       0,    63,    64,   201,     0,     0,     0,     0,     0,     0,
       0,    71,    72,    73,    74,   202,     0,    76,     0,     0,
      78,     0,     0,     0,    82,     0,    84,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    91,    92,    93,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,    96,    97,    98,     0,     0,
       0,   100,     0,   203,     0,   102,     0,     0,     5,     6,
     103,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   196,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   197,     0,     0,     0,    32,     0,     0,     0,     0,
    1216,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     199,     0,   200,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   201,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,   202,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,    84,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,    96,    97,    98,
       0,     0,     0,   100,     0,   203,     0,   102,     0,     0,
       5,     6,   103,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     196,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   197,     0,     0,     0,    32,     0,     0,
       0,     0,  1218,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   199,     0,   200,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   201,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,   202,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,    96,
      97,    98,     0,     0,     0,   100,     0,   203,     0,   102,
       0,     0,     5,     6,   103,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,     0,   196,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   197,     0,     0,     0,    32,
       0,     0,     0,     0,  1219,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   199,     0,   200,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   201,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,   202,     0,    76,     0,     0,    78,     0,     0,     0,
      82,     0,    84,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   203,
       0,   102,     0,     0,     0,     6,   103,     8,     9,    10,
      11,    12,    13,     0,     0,     0,    14,     0,     0,     0,
      18,     0,     0,     0,   196,     0,  -209,     0,     0,   294,
       0,     0,     0,     0,     0,     0,     0,   197,     0,     0,
       0,     0,  -209,  -209,  -209,     0,   198,     0,     0,   296,
       0,     0,     0,   298,     0,     0,     0,     0,   200,     0,
       0,    44,    45,     0,     0,     0,     0,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,     0,   299,     0,
      56,   300,    58,    59,    60,     0,     0,     0,     0,    63,
       0,   201,     0,   301,     0,     0,     0,     0,     0,    71,
      72,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,   303,     0,     0,    84,     0,   304,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,    96,    97,    98,     0,     0,     0,   100,
       0,   203,     0,   102,  -339,  -209,     0,     6,   103,     8,
       9,    10,    11,    12,    13,     0,     0,     0,    14,     0,
       0,     0,    18,     0,     0,     0,   196,     0,  -209,     0,
       0,   294,     0,     0,     0,     0,     0,     0,     0,   197,
       0,     0,     0,     0,  -209,  -209,  -209,     0,   198,     0,
       0,   296,     0,     0,     0,   298,     0,     0,     0,     0,
     200,     0,     0,    44,    45,     0,     0,     0,     0,    49,
       0,    50,    51,     0,     0,     0,     0,     0,     0,     0,
     299,     0,    56,   300,    58,    59,    60,     0,     0,     0,
       0,    63,     0,   201,     0,   301,     0,     0,     0,     0,
       0,    71,    72,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,   303,     0,     0,    84,     0,   304,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   636,
       0,   637,     0,     0,    89,    90,    91,    92,    93,     0,
       0,   638,     0,    94,     0,     0,   639,   233,   640,   641,
       0,     0,    95,     0,   642,    96,    97,    98,     0,     0,
       0,   100,   236,   203,   197,   102,     0,  -209,   237,     0,
     103,     0,     0,     0,   643,     0,     0,     0,   239,     0,
       0,   644,     0,   645,   646,     0,     0,     0,     0,     0,
       0,     0,   647,     0,     0,     0,     0,     0,     0,   243,
     648,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   649,     0,     0,
       0,     0,     0,     0,     0,     0,   247,     0,   650,     0,
     249,     0,   651,     0,     0,   251,     0,     0,     0,   652,
       0,   253,     0,     0,   653,     0,     0,     0,     0,     0,
       0,     0,     0,   511,   466,   467,   468,   469,   470,     0,
       0,   473,   474,   475,   476,     0,   478,   479,     0,   654,
     655,   656,   657,   658,   659,     0,   660,     0,     0,     0,
     661,   662,   663,   664,   665,   666,   667,   668,   669,   670,
       0,   671,     0,     0,   672,   673,   674,   675,     5,     6,
     676,     8,     9,    10,    11,    12,    13,     0,     0,     0,
      14,     0,     0,    17,    18,     0,    19,     0,   196,     0,
    -209,    22,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   197,     0,     0,     0,    32,  -209,  -209,  -209,     0,
     198,    35,     0,     0,     0,    37,     0,     0,    39,     0,
     199,     0,   200,     0,     0,    44,    45,     0,     0,    47,
       0,    49,     0,    50,    51,     0,    52,    53,     0,     0,
       0,     0,     0,     0,    56,     0,    58,     0,    60,     0,
       0,     0,     0,    63,    64,   201,     0,     0,     0,     0,
       0,     0,     0,    71,    72,    73,    74,   202,     0,    76,
       0,     0,    78,     0,     0,     0,    82,     0,    84,     0,
       0,    86,     0,   465,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,   476,   477,   478,   479,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    90,    91,    92,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,   480,    97,    98,
    -660,     0,  -660,   100,     0,   203,     0,   102,     0,  -209,
       5,     6,     0,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,     0,     0,    17,    18,     0,    19,     0,
     196,     0,  -209,    22,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   197,     0,     0,     0,    32,  -209,  -209,
    -209,     0,   198,    35,     0,     0,     0,    37,     0,     0,
      39,     0,   199,     0,   200,     0,     0,    44,    45,     0,
       0,    47,     0,    49,     0,    50,    51,     0,    52,    53,
       0,     0,     0,     0,     0,     0,    56,     0,    58,     0,
      60,     0,     0,     0,     0,    63,    64,   201,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    74,   202,
       0,    76,     0,     0,    78,     0,     0,     0,    82,     0,
      84,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,     0,
      91,     0,     5,     6,     0,     8,     9,    10,    11,    12,
      13,     0,     0,     0,    14,     0,     0,    17,    18,     0,
      19,    98,   196,     0,  -209,    22,     0,   203,     0,   102,
       0,  -209,     0,    28,     0,   197,     0,     0,     0,    32,
    -209,  -209,  -209,     0,   198,    35,     0,     0,     0,    37,
       0,     0,    39,     0,   199,     0,   200,     0,     0,    44,
      45,     0,     0,    47,     0,    49,     0,    50,    51,     0,
      52,    53,     0,     0,     0,     0,     0,     0,    56,     0,
      58,     0,    60,     0,     0,     0,     0,    63,    64,   201,
       0,     0,     0,     0,     0,     0,     0,    71,    72,    73,
      74,   202,     0,    76,     0,     0,    78,   702,     0,     0,
      82,     0,    84,     0,     0,    86,     0,   638,     0,     0,
       0,     0,   989,   990,   640,   991,     0,     0,     0,     0,
     992,     0,     0,     0,     0,     0,     0,     0,   704,     0,
      89,     0,    91,     0,     0,     0,     0,     0,     0,     0,
     993,     0,     0,     0,   994,     0,     0,   995,     0,   199,
     646,     0,     0,    98,     0,     0,     0,     0,   705,     0,
       0,   102,     0,  -209,     0,     0,   996,     0,     0,     0,
       0,     0,     0,   997,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   998,     0,     0,     0,     0,     0,     0,
       0,     0,   999,     0,  1000,     0,  1001,     0,   706,     0,
       0,     0,     0,     0,   707,  1002,     0,  1003,     0,     0,
    1004,     0,     0,     0,     0,     0,     0,     0,     0,   511,
     466,   467,   468,   469,   470,     0,     0,   473,   474,   475,
     476,     0,   478,   479,     0,   654,   655,   656,   657,   658,
     659,     0,   660,     0,     0,     0,   661,   662,   663,   664,
     665,   666,   667,   668,   669,   670,   231,   671,     0,     0,
     672,   673,   674,   675,  1005,     0,   638,     0,     0,     0,
       0,   232,   233,   640,   234,     0,     0,     0,     0,   235,
       0,     0,     0,     0,     0,     0,     0,   236,     0,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,   238,
       0,     0,     0,   239,     0,     0,   240,     0,   241,   646,
       0,     0,     0,     0,     0,     0,     0,   242,     0,     0,
       0,     0,     0,     0,   243,   244,     0,     0,     0,     0,
       0,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   246,     0,     0,     0,     0,     0,     0,   435,
       0,   247,     0,   248,     0,   249,   436,   250,     0,     0,
     251,     0,     0,     0,   252,     0,   253,     0,   437,   254,
       0,     0,     0,     0,     0,     0,     0,     0,   511,   466,
     467,   468,   469,   470,     0,     0,   473,   474,   475,   476,
       0,   478,   479,     0,   654,   655,   656,   657,   658,   659,
       0,   660,     0,     0,     0,   661,   662,   663,   664,   665,
     666,   667,   668,   669,   670,     0,   671,     0,     0,   672,
     673,   674,   675,     0,     0,     0,     0,   438,     0,   435,
       0,   439,     0,     0,     0,     0,   436,     0,     0,   510,
       0,     0,     0,     0,     0,     0,     0,     0,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     440,   511,   466,   467,   468,   469,   470,     0,     0,   473,
     474,   475,   476,     0,   478,   479,     0,   441,   442,     0,
     443,   444,   445,     0,   446,   447,   448,     0,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
       0,     0,   461,   462,   463,     0,   364,   438,     0,   435,
       0,   439,     0,   464,     0,     0,   436,     0,     0,  1189,
       0,     0,     0,     0,     0,     0,     0,     0,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     440,   511,   466,   467,   468,   469,   470,     0,     0,   473,
     474,   475,   476,     0,   478,   479,     0,   441,   442,     0,
     443,   444,   445,     0,   446,   447,   448,     0,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
       0,     0,   461,   462,   463,     0,   364,   438,     0,   435,
       0,   439,     0,   464,     0,     0,   436,     0,     0,  1252,
       0,     0,     0,     0,     0,     0,     0,     0,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     440,   511,   466,   467,   468,   469,   470,     0,     0,   473,
     474,   475,   476,     0,   478,   479,     0,   441,   442,     0,
     443,   444,   445,     0,   446,   447,   448,     0,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
       0,     0,   461,   462,   463,     0,   364,   438,     0,   435,
       0,   439,     0,   464,     0,     0,   436,     0,     0,  1253,
       0,     0,     0,     0,     0,     0,     0,     0,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     440,   511,   466,   467,   468,   469,   470,     0,     0,   473,
     474,   475,   476,     0,   478,   479,     0,   441,   442,     0,
     443,   444,   445,     0,   446,   447,   448,     0,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
       0,     0,   461,   462,   463,     0,   364,   438,     0,   435,
       0,   439,     0,   464,     0,     0,   436,     0,     0,  1254,
       0,     0,     0,     0,     0,     0,     0,     0,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     440,   511,   466,   467,   468,   469,   470,     0,     0,   473,
     474,   475,   476,     0,   478,   479,     0,   441,   442,     0,
     443,   444,   445,     0,   446,   447,   448,     0,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
       0,     0,   461,   462,   463,     0,   364,   438,     0,     0,
       0,   439,     0,   464,     0,     0,     0,     0,   435,  1255,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   871,     0,
     440,   511,   466,   467,   468,   469,   470,     0,     0,   473,
     474,   475,   476,   872,   478,   479,     0,   441,   442,     0,
     443,   444,   445,     0,   446,   447,   448,     0,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
       0,     0,   461,   462,   463,     0,   364,     0,     0,     0,
       0,     0,   435,   464,     0,     0,   438,     0,     0,   436,
     439,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   275,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   215,     0,   984,     0,   440,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   441,   442,     0,   443,
     444,   445,     0,   446,   447,   448,     0,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,     0,
     438,   461,   462,   463,   439,   364,     0,     0,     0,     0,
       0,     0,   464,   435,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,   215,
       0,     0,   437,   440,     0,     0,     0,   591,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     441,   442,     0,   443,   444,   445,     0,   446,   447,   448,
       0,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,     0,   592,   461,   462,   463,     0,   364,
       0,     0,     0,     0,     0,     0,   464,     0,     0,     0,
       0,   438,     0,     0,     0,   439,     0,     0,     0,     0,
       0,     0,     0,   435,   830,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,     0,   440,     0,     0,   831,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   441,   442,     0,   443,   444,   445,     0,   446,   447,
     448,     0,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   832,   593,   461,   462,   463,     0,
       0,     0,     0,     0,     0,     0,     0,   464,     0,     0,
       0,   438,   435,     0,     0,   439,     0,     0,     0,   436,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,  1132,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   440,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   441,   442,     0,   443,   444,   445,     0,   446,   447,
     448,     0,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,     0,     0,   461,   462,   463,     0,
     438,     0,   435,     0,   439,     0,     0,   464,     0,   436,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   275,     0,     0,     0,     0,     0,     0,   215,
       0,     0,     0,   440,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     441,   442,     0,   443,   444,   445,     0,   446,   447,   448,
       0,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,     0,     0,   461,   462,   463,     0,   364,
     438,     0,   435,     0,   439,     0,   464,     0,     0,   436,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   918,     0,     0,     0,     0,     0,     0,   215,
       0,     0,     0,   440,     0,     0,     0,   919,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     441,   442,     0,   443,   444,   445,     0,   446,   447,   448,
       0,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,     0,     0,   461,   462,   463,     0,   364,
     438,   435,     0,     0,   439,     0,   464,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   440,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     441,   442,     0,   443,   444,   445,     0,   446,   447,   448,
       0,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,     0,     0,   461,   462,   463,     0,   438,
       0,     0,     0,   439,     0,     0,   464,     0,     0,     0,
       0,   950,   435,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   440,     0,     0,     0,  1107,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
     442,     0,   443,   444,   445,     0,   446,   447,   448,     0,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,     0,  1108,   461,   462,   463,     0,   364,     0,
       0,     0,     0,     0,     0,   464,   435,     0,     0,     0,
     438,     0,     0,   436,   439,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   440,     0,     0,     0,     0,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,   436,     0,
     441,   442,     0,   443,   444,   445,     0,   446,   447,   448,
     437,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,     0,   438,   461,   462,   463,   439,     0,
       0,   435,   532,     0,     0,     0,   464,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,     0,     0,   215,     0,     0,     0,   440,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   438,
       0,     0,     0,   439,   441,   442,     0,   443,   444,   445,
       0,   446,   447,   448,     0,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,     0,   215,   461,
     462,   463,   440,     0,     0,     0,     0,     0,  1124,   438,
     464,     0,     0,   439,     0,     0,     0,     0,     0,   441,
     442,     0,   443,   444,   445,     0,   446,   447,   448,     0,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   440,     0,   461,   462,   463,     0,     0,     0,
     435,     0,     0,  1172,     0,   464,     0,   436,     0,   441,
     442,     0,   443,   444,   445,     0,   446,   447,   448,   437,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,     0,     0,   461,   462,   463,     0,     0,     0,
     435,     0,     0,     0,     0,   464,     0,   436,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   438,     0,
     435,     0,   439,     0,     0,     0,     0,   436,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   440,     0,     0,     0,     0,     0,     0,   438,     0,
       0,     0,   439,     0,     0,     0,     0,     0,   441,   442,
       0,   443,   444,   445,     0,   446,   447,   448,     0,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   440,     0,   461,   462,   463,     0,   555,   438,   435,
       0,     0,   439,     0,   464,     0,   436,     0,   441,   442,
     738,   443,   444,   445,     0,   446,   447,   448,   437,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   440,   559,   461,   462,   463,     0,     0,     0,   435,
     826,     0,     0,     0,   464,     0,   436,     0,   441,   442,
       0,   443,   444,   445,     0,   446,   447,   448,   437,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,     0,     0,   461,   462,   463,     0,   438,     0,     0,
     435,   439,     0,     0,   464,     0,     0,   436,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     440,     0,     0,     0,     0,     0,     0,   438,     0,     0,
       0,   439,     0,     0,     0,     0,     0,   441,   442,     0,
     443,   444,   445,     0,   446,   447,   448,     0,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
     440,     0,   461,   462,   463,     0,     0,     0,   438,   808,
       0,     0,   439,   464,     0,     0,     0,   441,   442,     0,
     443,   444,   445,     0,   446,   827,   448,     0,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
     865,   440,   461,   462,   463,     0,     0,     0,     0,     0,
       0,     0,     0,   464,     0,     0,     0,     0,   441,   442,
       0,   443,   444,   445,     0,   446,   447,   448,     0,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   435,     0,   461,   462,   463,     0,     0,   436,     0,
       0,     0,     0,     0,   464,     0,     0,     0,     0,     0,
     437,     0,     0,   948,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   438,
       0,   435,     0,   439,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   440,     0,     0,     0,     0,     0,     0,   438,
       0,     0,     0,   439,     0,     0,     0,     0,     0,   441,
     442,     0,   443,   444,   445,     0,   446,   447,   448,     0,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   440,     0,   461,   462,   463,     0,     0,   438,
     435,  1014,     0,   439,     0,   464,     0,   436,     0,   441,
     442,     0,   443,   444,   445,     0,   446,   447,   448,   437,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   440,     0,   461,   462,   463,     0,     0,     0,
     435,   960,     0,     0,     0,   464,     0,   436,     0,   441,
     442,     0,   443,   444,   445,     0,   446,   447,   448,   437,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,     0,     0,   461,   462,   463,     0,   438,     0,
       0,   961,   439,     0,     0,   464,     0,     0,   435,  1106,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,     0,     0,
       0,   440,     0,     0,     0,     0,     0,     0,   438,     0,
       0,     0,   439,     0,     0,     0,     0,     0,   441,   442,
       0,   443,   444,   445,     0,   446,   447,   448,     0,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   440,     0,   461,   462,   463,     0,     0,     0,     0,
       0,     0,     0,     0,   464,     0,   438,     0,   441,   442,
     439,   443,   444,   445,     0,   446,   447,   448,     0,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   435,     0,   461,   462,   463,     0,     0,   436,   440,
    1061,     0,     0,     0,   464,     0,     0,     0,     0,     0,
     437,     0,     0,     0,     0,     0,   441,   442,     0,   443,
     444,   445,     0,   446,   447,   448,     0,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   435,
       0,   461,   462,   463,     0,     0,   436,     0,     0,     0,
       0,     0,   464,     0,     0,     0,     0,     0,   437,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   438,
       0,     0,     0,   439,     0,     0,     0,     0,     0,   435,
       0,     0,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,     0,
       0,     0,   440,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   438,     0,   441,
     442,   439,   443,   444,   445,     0,   446,   447,   448,     0,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,     0,  1118,   461,   462,   463,     0,     0,  1127,
     440,     0,     0,     0,     0,   464,     0,   438,     0,     0,
       0,   439,     0,     0,     0,     0,     0,   441,   442,     0,
     443,   444,   445,     0,   446,   447,   448,     0,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
     440,     0,   461,   462,   463,     0,     0,     0,     0,     0,
       0,     0,     0,   464,     0,     0,     0,   441,   442,     0,
     443,   444,   445,     0,   446,   447,   448,     0,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
     435,     0,   461,   462,   463,     0,     0,   436,     0,     0,
       0,  1155,     0,   464,     0,     0,     0,     0,     0,   437,
    1158,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     435,  1177,     0,     0,     0,     0,     0,   436,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   438,     0,
       0,   435,   439,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   440,     0,     0,     0,     0,     0,     0,   438,     0,
       0,     0,   439,     0,     0,     0,     0,     0,   441,   442,
    1209,   443,   444,   445,     0,   446,   447,   448,     0,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   440,     0,   461,   462,   463,     0,     0,     0,   438,
       0,     0,     0,   439,   464,     0,     0,     0,   441,   442,
       0,   443,   444,   445,     0,   446,   447,   448,     0,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,     0,   440,   461,   462,   463,   435,     0,     0,     0,
       0,     0,     0,   436,   464,     0,     0,     0,     0,   441,
     442,     0,   443,   444,   445,   437,   446,   447,   448,     0,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,     0,     0,   461,   462,   463,     0,     0,     0,
       0,     0,     0,     0,   435,   464,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   438,     0,     0,     0,   439,     0,
       0,     0,     0,     0,   435,     0,  1217,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,     0,     0,     0,   440,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   438,     0,   441,   442,   439,   443,   444,   445,
       0,   446,   447,   448,  1256,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,     0,     0,   461,
     462,   463,     0,     0,     0,   440,     0,     0,     0,     0,
     464,     0,   438,   435,     0,     0,   439,     0,     0,     0,
     436,     0,   441,   442,  1257,   443,   444,   445,     0,   446,
     447,   448,   437,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   460,   440,     0,   461,   462,   463,
       0,     0,     0,   435,     0,     0,     0,     0,   464,     0,
     436,     0,   441,   442,     0,   443,   444,   445,     0,   446,
     447,   448,   437,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   460,     0,     0,   461,   462,   463,
       0,   438,     0,   435,     0,   439,     0,     0,   464,     0,
     436,     0,     0,  1258,     0,     0,     0,     0,     0,     0,
       0,     0,   437,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   440,     0,     0,     0,     0,     0,
       0,   438,     0,     0,     0,   439,     0,     0,     0,     0,
       0,   441,   442,  1259,   443,   444,   445,     0,   446,   447,
     448,     0,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   440,     0,   461,   462,   463,     0,
       0,   438,   435,     0,     0,   439,     0,   464,     0,   436,
       0,   441,   442,     0,   443,   444,   445,     0,   446,   447,
     448,   437,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   440,     0,   461,   462,   463,     0,
       0,     0,     0,     0,     0,     0,     0,   464,     0,     0,
       0,   441,   442,     0,   443,   444,   445,     0,   446,   447,
     448,   437,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,     0,     0,   461,   462,   463,     0,
     938,     0,     0,     0,   439,     0,     0,   464,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   440,     0,     0,     0,     0,     0,     0,
     438,     0,     0,     0,   439,     0,     0,     0,     0,     0,
     441,   442,     0,   443,   444,   445,     0,   446,   447,   448,
       0,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   440,     0,   461,   462,   463,     0,     0,
     438,     0,     0,     0,   439,     0,   464,     0,     0,     0,
     441,   442,     0,   443,   444,   445,     0,   446,   447,   448,
       0,   449,   450,   451,   452,     0,   454,   455,   456,   457,
     458,   459,   460,   440,     0,   461,   462,   463,     0,     0,
       0,     0,     0,     0,     0,     0,   464,     0,     0,     0,
     441,   442,     0,   443,   444,   445,     0,   446,   447,   448,
       0,   449,   450,   451,   452,   231,   454,   455,   456,   457,
     458,     0,   460,     0,     0,   461,   462,   463,     0,     0,
     232,   233,     0,   234,     0,     0,   464,     0,   235,     0,
      24,     0,     0,     0,     0,     0,   236,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,   238,     0,
       0,     0,   239,     0,     0,   240,     0,   241,     0,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,     0,
       0,     0,     0,   243,   244,     0,     0,     0,     0,     0,
       0,   245,    57,     0,     0,     0,     0,     0,     0,     0,
       0,   246,     0,     0,    66,     0,   231,     0,     7,     0,
     247,     0,   248,     0,   249,     0,   250,     0,     0,   251,
       0,   232,   233,   252,   234,   253,     0,    85,   254,   235,
       0,     0,     0,     0,     0,     0,     0,   236,     0,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,   238,
       0,     0,     0,   239,     0,     0,   240,     0,   241,     0,
       0,     0,     0,     0,     0,  -310,     0,   242,     0,     0,
       0,     0,     0,     0,   243,   244,     0,     0,     0,     0,
    -310,  -310,   245,  -310,     0,     0,     0,     0,  -310,     0,
       0,     0,   246,     0,     0,     0,  -310,     0,     0,     0,
       0,   247,  -310,   248,     0,   249,     0,   250,  -310,     0,
     251,     0,  -310,     0,   252,  -310,   253,  -310,     0,   254,
       0,     0,     0,     0,   231,     0,  -310,     0,     0,     0,
       0,     0,     0,  -310,  -310,     0,     0,     0,     0,   232,
     233,  -310,   234,     0,     0,     0,     0,   235,     0,     0,
       0,  -310,     0,     0,     0,   236,     0,     0,     0,     0,
    -310,   237,  -310,     0,  -310,     0,  -310,   238,     0,  -310,
       0,   239,     0,  -310,   240,  -310,   241,     0,  -310,     0,
       0,     0,     0,  -311,     0,   242,     0,     0,     0,     0,
       0,     0,   243,   244,     0,     0,     0,     0,  -311,  -311,
     245,  -311,     0,     0,     0,     0,  -311,     0,     0,     0,
     246,     0,     0,     0,  -311,     0,     0,     0,     0,   247,
    -311,   248,     0,   249,     0,   250,  -311,     0,   251,     0,
    -311,     0,   252,  -311,   253,  -311,     0,   254,     0,     0,
       0,     0,     0,     0,  -311,     0,     0,     0,     0,     0,
       0,  -311,  -311,     0,     0,     0,     0,     0,     0,  -311,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -311,
       0,     0,     0,     0,     0,     0,     0,     0,  -311,     0,
    -311,     0,  -311,     0,  -311,     0,     0,  -311,     0,     0,
       0,  -311,     0,  -311,     0,     0,  -311
};

static const yytype_int16 yycheck[] =
{
       2,     2,   111,   553,    46,   266,   166,   182,   619,   426,
     631,   402,   392,    68,    63,   591,    63,    63,   517,     2,
     493,   679,    27,   681,    26,   580,   224,   797,    70,   627,
     291,   832,    87,   880,   132,   548,   382,   791,   827,   103,
     404,     5,     1,    26,    33,     1,   230,   102,    33,     1,
     524,    57,    33,   963,   850,    33,     3,    33,   438,   439,
      57,    33,    67,    23,    48,   123,    36,   204,     0,   330,
     888,   147,    76,    75,   147,    62,    84,    79,    80,    87,
       1,    47,   266,     1,    47,   118,   120,   262,   165,   165,
     519,   147,    75,   147,   120,    46,    79,    80,    33,   101,
     173,    67,    33,   105,   484,   485,   170,   291,    55,   165,
     490,    57,   170,   117,   147,    33,    25,   546,   101,    70,
      86,   175,   105,    86,    76,   120,   147,    48,    79,    80,
      90,   118,    41,    42,    43,   120,   110,    25,     2,   120,
     110,   175,   120,    48,    62,    33,   330,   965,   114,   175,
     171,   114,   112,    41,    42,    43,    25,   631,   147,   110,
      25,    57,    26,   147,    33,   117,   172,   171,    33,   125,
    1017,   147,    41,    42,    43,   172,    41,    42,    43,   777,
     544,   170,  1092,   147,    68,   170,   956,   138,   106,   170,
     146,   147,   170,   963,   170,     1,    57,   577,   170,    57,
     118,  1048,  1049,    87,   125,   606,   607,   171,   763,   610,
     611,    75,   966,   172,   216,    79,    80,   173,   102,   171,
    1016,   482,   483,  1012,     3,   146,   147,    33,   146,   366,
     165,   119,    92,   216,    48,   170,   278,   101,   280,   170,
     739,   105,   147,     3,   165,   147,   120,   165,   594,   147,
     119,   426,   170,    48,   119,   831,    62,   147,    48,    48,
      48,   147,    48,   447,     3,   174,    28,  1114,   270,   171,
     175,   222,   147,   275,    12,   173,    55,   172,    92,   165,
     147,   171,   795,   381,    57,   327,   174,   270,   147,  1136,
     165,    19,   275,    28,   147,    55,   557,    92,   482,   483,
     106,   175,    92,    92,  1074,   174,   173,  1108,   147,   174,
     261,   262,   118,    48,   173,    94,    55,    52,    80,   104,
     173,   165,  1092,   147,    57,   104,   165,   278,   172,   280,
      92,   172,   120,   517,   120,    54,   287,   497,   536,   937,
     146,   165,    77,   964,   104,    80,   230,    82,   350,   350,
       2,    79,   216,   115,   403,   172,    75,    92,     9,   147,
     147,   147,   364,    57,   170,   104,    85,   350,    57,   147,
      98,   373,   147,   557,    26,   110,   327,    57,   165,   878,
     115,   364,   266,   147,   386,   113,   391,   175,    28,   175,
     373,   141,   147,   125,   399,   173,   580,  1144,   173,   149,
     598,   165,   147,   386,  1062,  1063,   270,   291,    48,    32,
     147,   275,    52,   814,   815,   816,   471,   472,   173,   151,
     165,   920,   477,    75,   426,   480,   481,    79,    80,  1005,
     985,    91,   147,    57,    47,    95,   173,    77,   173,   147,
      80,  1052,    82,   147,   101,   147,   330,   125,   147,   101,
     165,   924,    92,   105,    67,    57,  1203,  1204,  1205,  1206,
    1207,  1208,   147,    79,    80,   173,   165,     2,    91,   173,
     110,   173,    95,    86,   152,   115,   154,   155,   147,   157,
     158,   861,   899,    57,   489,   147,   350,    51,   173,   880,
      54,    26,   872,   147,    32,   170,   545,   172,   545,   545,
     364,   114,   172,   165,   173,    57,   508,   165,   510,   373,
     619,    75,    57,   171,    78,   570,   571,     5,   141,   173,
       3,    85,   386,   146,    91,   508,   149,   510,    95,  1140,
     153,  1107,   147,   173,   165,   915,   165,    57,    21,   919,
      75,   172,   171,   174,    79,    80,   110,   172,    91,   174,
     165,    34,    95,    91,   172,   739,   174,    95,   938,   939,
     602,   603,     2,    91,   216,    57,   101,    95,   610,   611,
     105,   576,    55,    91,    91,    79,    80,    95,    95,   763,
     149,    64,   584,   584,   570,   571,    26,   471,   472,   172,
     172,   174,   174,   477,   292,   293,   480,   481,   482,   483,
    1150,   584,   982,   141,   984,   170,    57,   146,   146,   560,
     148,   149,   172,   170,   174,   153,   172,   172,   270,   174,
     164,   104,   160,   275,  1025,  1026,  1017,   110,   165,   789,
     168,   165,   172,   517,   174,    75,   165,  1038,  1039,    79,
      80,   165,   826,   827,   508,    57,   510,   170,    86,  1199,
     165,   602,   603,   165,   165,    28,   165,  1048,   165,   610,
     611,   101,  1053,   165,    28,   105,   141,   172,   147,   172,
     146,   146,   172,   557,   149,    48,   173,   172,   153,    52,
     165,   216,    32,   165,    48,   147,   570,   571,    52,   173,
       9,     3,   852,    67,   878,   172,   580,    80,   350,  1100,
    1101,  1102,  1103,   170,    77,   125,   761,    80,   165,    82,
     149,    70,   364,    77,   172,   125,    80,   173,    82,    92,
     584,   373,   147,  1114,   899,   780,   125,   125,    92,    35,
      35,    67,   173,   125,   386,   270,   920,   110,   173,   146,
     275,    91,   115,   173,    22,    95,   110,   170,   119,   125,
     172,   115,   147,   171,   756,   106,   172,   141,   149,   149,
     149,   149,   149,   149,   149,   767,   175,   149,   770,   149,
     149,   149,   149,   815,   816,    33,   216,    32,   172,   149,
     149,   149,    35,    35,   767,   146,    48,   770,    35,   147,
     165,   141,   142,   165,   144,   145,   146,   146,   148,   149,
     150,   985,   152,   153,   154,   155,   173,   157,   158,   159,
     160,   161,   165,   163,   171,   350,   166,   167,   168,   165,
     771,   165,   165,   165,  1085,  1086,   173,   177,   870,   364,
     270,   880,   874,   173,   175,   275,    91,   886,   373,   170,
      95,   175,   171,   173,   165,   165,   895,   175,   172,   172,
     165,   386,   857,   147,   805,   739,   508,   859,   510,   173,
      33,   863,   175,    35,   815,   816,   868,   175,   175,   871,
     173,   173,   165,   173,   165,   164,   859,   761,   164,   763,
     863,   147,   110,   579,   985,   868,   141,   142,   871,   144,
     145,   146,   373,   148,   149,   150,   780,   824,   153,   265,
      25,  1085,  1086,   767,   159,   160,   770,   366,   163,   978,
     350,   166,   167,   168,   875,   339,   390,   545,   530,   870,
     384,   886,   177,   874,   364,   876,   899,    28,   895,  1199,
    1055,  1209,   584,   373,  1202,   851,   938,   939,   685,   532,
    1049,   130,   305,   945,   776,   947,   386,    48,   950,   527,
      21,    52,    -1,  1128,    -1,   924,    -1,    -1,    -1,    -1,
      -1,    -1,   945,    -1,   947,     3,    -1,   950,  1017,   974,
      -1,   973,    -1,   508,    -1,   510,    77,    -1,    -1,    80,
      18,   983,    -1,    21,    -1,    -1,    -1,    -1,    26,    -1,
     973,    92,    -1,    -1,   878,   859,  1038,  1039,    36,   863,
     983,    -1,    -1,    -1,   868,    -1,    -1,   871,    46,   110,
      -1,    -1,    -1,    -1,   115,    53,    -1,    55,    -1,    -1,
      28,    -1,  1024,    -1,    -1,    -1,    64,  1136,  1030,    -1,
     981,  1140,    -1,     2,    72,    -1,   920,    -1,    -1,    -1,
      48,  1024,    -1,    -1,    52,    -1,    -1,  1030,    -1,   584,
      -1,    89,    -1,  1228,  1096,  1230,    -1,    26,  1233,  1234,
    1102,  1103,   100,    -1,    -1,  1114,   104,    -1,   508,    77,
     510,    -1,    80,   111,    -1,    -1,    -1,  1119,   116,  1081,
      -1,   945,    -1,   947,    92,    -1,   950,  1038,  1039,  1131,
      -1,    -1,  1134,    -1,    -1,    -1,    -1,    -1,  1081,    -1,
      -1,   985,   110,    -1,    -1,    -1,    75,   115,    -1,   973,
      79,    80,    -1,    -1,    -1,   767,    -1,    -1,   770,   983,
    1122,    -1,  1124,    -1,    -1,    -1,    -1,  1129,    -1,    -1,
    1132,    -1,   101,    -1,    -1,    -1,   105,    -1,    -1,  1122,
      -1,  1124,    -1,    -1,   584,  1096,  1129,    -1,    -1,  1132,
      -1,  1102,  1103,    -1,    -1,    -1,    -1,    -1,    -1,  1161,
    1024,    -1,    -1,    -1,    -1,  1167,  1030,    -1,  1119,  1171,
    1172,    -1,    -1,    -1,    -1,    -1,    -1,  1128,  1161,    -1,
    1131,  1183,    -1,  1134,  1167,  1187,    -1,  1189,  1171,  1172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1183,  1085,  1086,    -1,  1187,    -1,  1189,   859,  1210,  1210,
      -1,   863,    -1,    -1,    -1,    -1,   868,  1081,  1169,   871,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1210,    -1,    -1,
      -1,    32,   767,    -1,    -1,   770,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,
    1252,  1253,  1254,  1255,    -1,    -1,    -1,    -1,  1122,    -1,
    1124,    -1,    -1,    -1,    -1,  1129,    -1,    -1,  1132,  1252,
    1253,  1254,  1255,    -1,    -1,    -1,     2,  1228,    -1,  1230,
      -1,    -1,  1233,  1234,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,   945,    95,   947,    -1,  1161,   950,    -1,
      26,   270,    -1,  1167,    -1,    -1,   275,  1171,  1172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1183,
      -1,   973,    -1,  1187,   859,  1189,    -1,   767,   863,    -1,
     770,   983,    -1,   868,    -1,    -1,   871,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,  1210,   148,   149,    75,
      -1,    -1,   153,    79,    80,    -1,    -1,    -1,   159,   160,
      -1,    -1,   163,    -1,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,  1024,    -1,     5,   101,    -1,    -1,  1030,   105,
      -1,   350,    -1,    -1,    -1,    -1,    -1,    -1,  1252,  1253,
    1254,  1255,    23,    -1,    -1,   364,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,   373,    36,    -1,    38,    39,    -1,
     945,    -1,   947,    44,    -1,   950,    -1,   386,    -1,   859,
      51,    -1,    -1,   863,    -1,    -1,    -1,    -1,   868,  1081,
      -1,   871,    -1,    -1,    -1,    -1,    -1,    -1,   973,    -1,
      -1,    -1,    73,    32,    -1,    -1,    -1,    78,   983,    80,
      -1,    82,    -1,    84,    -1,    -1,    87,    -1,    -1,    90,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1122,    -1,  1124,    -1,    -1,    -1,    -1,  1129,    -1,   110,
    1132,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,  1024,
     216,    -1,    -1,    -1,    -1,  1030,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,   945,    95,   947,    -1,  1161,
     950,    -1,    -1,    -1,    -1,  1167,    -1,    -1,    -1,  1171,
    1172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1183,    -1,   973,   165,  1187,    -1,  1189,    -1,   508,
     171,   510,    -1,   983,   270,    -1,  1081,    -1,    -1,   275,
       2,    -1,   141,   142,    -1,   144,   145,   146,  1210,   148,
     149,    -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,    26,    -1,    -1,   166,   167,   168,
      -1,    -1,    -1,    -1,  1024,    -1,    -1,  1122,    -1,  1124,
    1030,    -1,    -1,    -1,  1129,    -1,    -1,  1132,    -1,    -1,
    1252,  1253,  1254,  1255,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   584,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    75,   350,    -1,  1161,    79,    80,    -1,
      -1,    -1,  1167,    41,    42,    43,  1171,  1172,   364,    -1,
      -1,  1081,    -1,    -1,    -1,    -1,    23,   373,  1183,   101,
      27,    28,  1187,   105,  1189,    -1,    -1,    -1,    -1,    36,
     386,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    51,  1210,    -1,    54,    -1,    -1,
      -1,    58,  1122,    -1,  1124,    -1,    -1,    -1,    -1,  1129,
      67,    -1,  1132,    -1,    -1,    -1,    73,    -1,    75,    -1,
       2,    78,    -1,    80,    -1,    -1,    -1,    84,    85,    86,
      87,    -1,    -1,    90,    -1,    92,    -1,  1252,  1253,  1254,
    1255,  1161,    -1,    -1,    26,    -1,    -1,  1167,    -1,    -1,
      -1,  1171,  1172,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,   150,  1183,    -1,    -1,    -1,  1187,    -1,  1189,
      23,    -1,    -1,    -1,    27,    28,   164,    -1,    -1,    -1,
      -1,    -1,    -1,    36,   216,    38,    39,    -1,    -1,    -1,
    1210,    44,    -1,    75,    -1,    -1,    -1,     1,    51,     3,
      -1,    -1,   508,    -1,   510,    -1,    -1,    -1,   165,    -1,
      -1,    -1,   200,   170,    18,    19,    -1,    21,   767,   101,
      73,   770,    26,   105,    -1,    78,    -1,    80,    -1,    32,
      34,    -1,  1252,  1253,  1254,  1255,    40,    90,   270,    92,
      -1,    -1,    46,   275,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,
      64,    -1,   115,    -1,    -1,    -1,    -1,    71,    72,    -1,
      -1,   259,   260,    -1,    -1,    79,    -1,    -1,   584,    -1,
      -1,    -1,    -1,   271,    -1,    89,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    98,    -1,   100,    -1,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,
     859,    -1,   116,    -1,   863,    -1,    -1,    -1,   350,   868,
      -1,    -1,   871,   311,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   364,    -1,   216,    -1,    -1,    -1,   141,   142,
      -1,   373,   145,   146,     1,   148,   149,    -1,     5,    -1,
     153,    -1,    -1,    -1,   386,    -1,    -1,   160,    -1,    -1,
      -1,    -1,    -1,   166,   167,   168,    23,    -1,   172,    -1,
      27,    28,   360,    -1,    -1,    -1,    -1,    -1,    -1,    36,
      -1,    38,    39,    -1,    32,    -1,    -1,    44,   270,    -1,
      -1,    -1,    -1,   275,    51,    -1,   945,    54,   947,    -1,
      -1,   950,    -1,    -1,    -1,    -1,    -1,    32,   396,    -1,
     398,    -1,    -1,    -1,   402,    -1,    73,    -1,    75,    -1,
      -1,    78,    -1,    80,   973,    82,    -1,    84,    85,    -1,
      87,    -1,    -1,    90,   983,    92,    -1,    -1,   426,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,   767,    -1,    -1,   770,    -1,    91,    -1,   350,    -1,
      95,    -1,    -1,    -1,    -1,  1024,   508,    -1,   510,    -1,
      -1,  1030,   364,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   373,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,   491,   386,   153,   154,   155,   165,   157,
     158,   159,   160,    -1,   171,   163,   141,   142,   166,   167,
     168,   146,    -1,   148,   149,    -1,    -1,    -1,   153,   177,
      -1,   519,  1081,    -1,    -1,   160,    -1,    -1,    -1,    -1,
      -1,   166,   167,   168,   532,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   584,   859,   542,    -1,     3,   863,   546,    -1,
      -1,    -1,   868,    -1,    -1,   871,    -1,    -1,    -1,    -1,
      -1,    18,    19,  1122,    21,  1124,    -1,    -1,    -1,    26,
    1129,    -1,    -1,  1132,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,   590,    50,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,  1161,   601,    -1,    -1,    -1,    64,  1167,    -1,
      -1,    -1,  1171,  1172,    71,    72,   508,    -1,   510,    -1,
      -1,   619,    79,    -1,  1183,    -1,    -1,    -1,  1187,   945,
    1189,   947,    89,    -1,   950,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,   100,    -1,   102,    -1,   104,    -1,    -1,
     107,  1210,    -1,    -1,   111,    -1,   113,   973,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   983,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   685,    -1,    -1,
      -1,    -1,   584,  1252,  1253,  1254,  1255,    -1,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,  1024,    -1,
      -1,    -1,    -1,   170,  1030,   172,    18,    19,    -1,    21,
      -1,    -1,    -1,    -1,    26,   767,    32,    -1,   770,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,  1081,    -1,    -1,    -1,    71,
      72,    26,    -1,    -1,   772,    -1,    -1,    79,   776,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    89,   786,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,    -1,
     102,    -1,   104,    -1,    -1,   107,  1122,   805,  1124,   111,
      -1,   113,    -1,  1129,   116,    -1,  1132,   859,    -1,    -1,
      75,   863,    -1,    -1,    -1,    -1,   868,    -1,    -1,   871,
      -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,
     146,    -1,   148,   149,    -1,  1161,    -1,   153,    -1,    -1,
     105,  1167,    -1,   851,   160,  1171,  1172,    -1,    -1,    -1,
     166,   167,   168,    -1,    -1,    -1,    -1,  1183,    -1,    -1,
     172,  1187,    -1,  1189,    -1,   767,    -1,   875,   770,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1210,     1,    -1,     3,    -1,     5,
      -1,   899,    -1,   945,    -1,   947,    -1,    -1,   950,    -1,
      -1,    -1,    18,    19,    -1,    21,    -1,    -1,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,   973,    -1,    -1,    40,    -1,  1252,  1253,  1254,  1255,
      46,   983,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,   216,    -1,    -1,    -1,    71,    72,   859,    -1,    -1,
      -1,   863,    -1,    79,    -1,    -1,   868,    -1,    -1,   871,
     978,    -1,  1024,    89,    -1,    -1,    -1,    -1,  1030,    14,
      -1,    -1,    98,    18,   100,    -1,   102,    -1,   104,    -1,
      -1,   107,    27,    -1,    -1,   111,    -1,   113,    -1,    34,
     116,    -1,    -1,    -1,    -1,   270,    -1,    -1,    -1,    -1,
     275,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    54,
      -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    63,  1081,
      -1,    -1,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    77,   945,  1052,   947,    -1,    -1,   950,    84,
      -1,    -1,    87,    -1,    89,    -1,    91,    -1,    -1,    94,
      95,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
    1122,   973,  1124,    -1,    -1,    -1,    -1,  1129,    -1,    -1,
    1132,   983,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1161,
      -1,    -1,    -1,    -1,    -1,  1167,    -1,    -1,    -1,  1171,
    1172,   386,  1024,    -1,    -1,    -1,    -1,    -1,  1030,    -1,
      -1,  1183,  1140,    -1,    -1,  1187,    -1,  1189,     3,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    19,    -1,    21,    -1,  1210,    -1,
      -1,    26,    -1,   198,    -1,    -1,    -1,   202,   203,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,  1081,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,   230,    -1,    -1,    -1,    64,
    1252,  1253,  1254,  1255,    -1,    -1,    71,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
    1122,    -1,  1124,    -1,    89,    -1,    -1,  1129,    -1,    -1,
    1132,   266,    -1,    98,    13,   100,    -1,   102,    -1,   104,
      -1,    20,   107,   508,    -1,   510,   111,    -1,   113,    -1,
      -1,   116,    -1,    32,    33,    -1,   291,    -1,    -1,  1161,
      -1,    -1,    -1,    -1,    -1,  1167,    -1,   302,    -1,  1171,
    1172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1183,   317,   318,    -1,  1187,    -1,  1189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   330,    -1,    -1,    -1,   334,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,  1210,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    95,   352,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   362,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   374,
      -1,   376,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
    1252,  1253,  1254,  1255,    -1,   390,   391,   392,    -1,    -1,
      -1,    -1,   141,   142,   399,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
     435,   436,   437,   438,   439,   440,    -1,   442,   443,   444,
     445,   446,    -1,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
      -1,    -1,    -1,    -1,    -1,    -1,   471,   472,    -1,    -1,
      -1,    -1,   477,    -1,    -1,   480,   481,   482,   483,   484,
     485,    -1,    -1,    -1,   489,   490,    -1,    -1,   493,    -1,
      -1,    -1,    -1,    -1,   499,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   510,    -1,    -1,   513,    -1,
      -1,    -1,   517,    -1,    -1,    -1,    -1,    -1,    -1,   524,
      -1,   526,    -1,    -1,    -1,   530,    -1,    -1,   533,   534,
       3,    -1,   767,    -1,    -1,   770,    -1,    -1,    -1,    -1,
     545,    -1,    -1,   548,    -1,    18,    19,    -1,    21,    -1,
      -1,    -1,   557,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,   567,    -1,    -1,   570,   571,    40,    -1,    -1,
      -1,   576,   577,    46,    -1,   580,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,   591,   592,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
     605,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,     5,
      -1,    -1,    -1,    -1,   859,    98,   631,   100,   863,   102,
      -1,   104,    -1,   868,   107,    -1,   871,    23,   111,    -1,
     113,    27,    28,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,   676,    -1,    -1,    -1,    -1,    -1,    -1,   683,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    84,   172,
      -1,    87,    -1,    -1,    90,    -1,    92,    -1,    -1,    -1,
     945,    -1,   947,    -1,    -1,   950,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,   112,    -1,    -1,   115,
      -1,     5,    -1,   738,   739,    -1,    -1,    -1,   973,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   983,    23,
      -1,    -1,    -1,    27,    28,    -1,   761,    -1,   763,   764,
     765,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    -1,   780,    -1,    51,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,  1024,
     795,    -1,    -1,    -1,   799,  1030,    -1,    -1,    -1,    73,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      84,    -1,    -1,    87,    -1,    -1,    90,    -1,    92,   824,
      -1,    -1,    -1,   828,    -1,   830,   831,   832,    -1,   834,
      -1,   836,    -1,    -1,   839,   840,   110,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,  1081,    -1,   853,    -1,
      -1,    -1,   857,    -1,    -1,    -1,   861,   862,   863,    -1,
     865,   866,    -1,   868,    -1,    -1,   871,   872,    -1,    -1,
      -1,    -1,    -1,   878,    -1,    -1,    -1,    -1,   883,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1122,    -1,  1124,
      -1,   165,    -1,    -1,  1129,    -1,    -1,  1132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     915,   916,   917,   918,   919,   920,    -1,    -1,    -1,   924,
      -1,    -1,    -1,    -1,    -1,    -1,  1161,    -1,    -1,    -1,
      -1,    -1,  1167,   938,   939,    -1,  1171,  1172,    -1,    -1,
      -1,    -1,    -1,   948,    -1,    -1,    -1,    -1,  1183,    -1,
      -1,    -1,  1187,    -1,  1189,    -1,    -1,    -1,    -1,   964,
       5,    -1,    -1,   968,    -1,    -1,    -1,    -1,    -1,   974,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   982,    23,   984,
     985,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,
    1005,    -1,    -1,    -1,    -1,    -1,    51,  1012,    -1,  1014,
      -1,    -1,    -1,    -1,    -1,    -1,  1021,  1252,  1253,  1254,
    1255,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,
      -1,  1036,    -1,    78,    -1,    80,    -1,    82,    -1,    84,
      -1,    -1,    87,    -1,    -1,    90,    -1,    92,    -1,    -1,
      -1,    -1,  1057,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     5,    -1,    -1,  1069,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
    1085,  1086,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    36,  1098,    38,    39,    -1,    -1,    -1,    -1,
      44,  1106,  1107,  1108,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1122,    -1,  1124,
     165,    -1,  1127,    -1,  1129,    -1,   171,  1132,    -1,    73,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    -1,    -1,
      84,    -1,    -1,    87,    -1,    -1,    90,    -1,    92,  1154,
    1155,    -1,  1157,  1158,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,    -1,
      -1,   115,  1177,    -1,    -1,  1180,    -1,    -1,    -1,  1184,
      -1,    -1,    -1,    -1,  1189,  1190,    -1,    -1,  1193,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1201,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1215,  1216,  1217,  1218,  1219,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1252,  1253,  1254,
    1255,  1256,  1257,  1258,  1259,     0,     1,    -1,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    16,    17,    18,    19,    -1,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    66,    67,    68,    69,    -1,    71,    72,    73,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,    -1,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    13,    -1,    15,    16,    17,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,    69,
      -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,    -1,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
     170,   171,   172,    -1,   174,    -1,     1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    16,    17,    18,    19,    -1,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    66,    67,    68,    69,    -1,    71,    72,    73,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,    -1,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,   174,
      -1,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    16,    17,    18,    19,
      -1,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,    69,
      -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,    -1,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,   171,   172,    -1,   174,    -1,     1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    16,    17,    18,    19,    -1,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    66,    67,    68,    69,    -1,    71,    72,    73,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,    -1,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,   174,
      -1,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    16,    17,    18,    19,
      -1,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,    69,
      -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,    -1,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,   171,   172,    -1,   174,    -1,     1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    16,    17,    18,    19,    -1,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    66,    67,    68,    69,    -1,    71,    72,    73,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,    -1,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    16,    17,    18,    19,
      -1,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,    69,
      -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,    -1,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    16,    17,    18,    19,    -1,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    66,    67,    68,    69,    -1,    71,    72,    73,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,    -1,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    16,    17,    18,    19,
      -1,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,    69,
      -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,    -1,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    16,    17,    18,    19,    -1,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    66,    67,    68,    69,    -1,    71,    72,    73,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,    -1,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    16,    17,    18,    19,
      -1,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,    69,
      -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,    -1,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     1,   177,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
      15,    16,    17,    18,    19,    -1,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    -1,    63,    64,
      65,    66,    67,    68,    69,    -1,    71,    72,    73,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    84,
      85,    86,    87,    88,    89,    90,    -1,    92,    93,    94,
      -1,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,    -1,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    16,    17,    18,    19,
      -1,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    61,    -1,    63,    64,    65,    66,    67,    68,    69,
      -1,    71,    72,    73,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    84,    85,    86,    87,    88,    89,
      90,    -1,    92,    93,    94,    -1,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,    -1,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    -1,    -1,   119,
      -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,     1,   174,     3,     4,   177,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      18,    19,    -1,    21,    -1,    23,    -1,    25,    26,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    43,    -1,    45,    46,    -1,
      48,    -1,    50,    -1,    52,    53,    -1,    55,    -1,    57,
      -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,
      68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,    77,
      -1,    79,    80,    81,    -1,    83,    -1,    -1,    -1,    -1,
      88,    89,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,   110,   111,    -1,   113,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,   173,   174,    -1,     1,   177,
       3,     4,    -1,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      43,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
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
      -1,   174,     3,     4,   177,     6,     7,     8,     9,    10,
      11,    -1,    13,    -1,    15,    -1,    -1,    18,    19,    20,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    56,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,    -1,
     141,   142,   143,   144,   145,   146,    -1,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
      -1,   172,    -1,    -1,     3,     4,   177,     6,     7,     8,
       9,    10,    11,    -1,    13,    -1,    15,    -1,    -1,    18,
      19,    20,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    56,    57,    -1,
      -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,
      69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,    -1,   141,   142,   143,   144,   145,   146,    -1,   148,
      -1,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,   166,   167,   168,
     169,   170,    -1,   172,    -1,    -1,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    16,    -1,    18,    19,    -1,    21,    -1,    23,
      24,    -1,    26,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,    63,
      64,    65,    66,    -1,    68,    69,    -1,    71,    72,    73,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
      -1,     3,     4,   177,     6,     7,     8,     9,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,
      -1,    23,    -1,    -1,    26,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    45,    46,    -1,    48,    -1,    50,    -1,
      52,    53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,
      -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,
      72,    -1,    -1,    -1,    -1,    77,    -1,    79,    80,    81,
      -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,   111,
      -1,   113,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,    -1,     3,     4,   177,     6,     7,     8,     9,
      10,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,
      -1,    21,    -1,    23,    -1,    -1,    26,    27,    28,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,
      60,    61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,
      -1,    71,    72,    73,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,
      90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,     3,     4,   177,     6,     7,
       8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      18,    19,    -1,    21,    -1,    23,    -1,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    43,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    57,
      -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,    -1,
      68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,    -1,
      88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,   112,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,     3,     4,   177,
       6,     7,     8,     9,    10,    11,    -1,    -1,    -1,    15,
      -1,    -1,    18,    19,    -1,    21,    -1,    23,    -1,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    43,    -1,    45,
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
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,     3,
       4,   177,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,    -1,
      64,    -1,    66,    -1,    68,    69,    -1,    71,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    -1,    -1,    88,    89,    90,    91,    -1,    -1,
      -1,    95,    -1,    -1,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,   147,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,
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
     102,    -1,   104,    -1,    -1,   107,   108,   109,    -1,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
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
     100,   101,   102,    -1,   104,    -1,    -1,   107,   108,   109,
      -1,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
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
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,   175,
      -1,   177,     3,     4,    -1,     6,     7,     8,     9,    10,
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
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,   173,    -1,     3,     4,   177,     6,     7,     8,
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
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,   173,    -1,     3,     4,   177,     6,
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
      -1,    66,    -1,    68,    69,    70,    71,    72,    -1,    -1,
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
       3,     4,   177,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
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
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,   147,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
     171,   172,    -1,    -1,     3,     4,   177,     6,     7,     8,
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
      -1,    -1,   141,   142,   143,   144,   145,    -1,   147,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,    -1,     3,     4,   177,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    -1,    21,    -1,    23,    -1,    -1,    26,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    64,    -1,    66,
      -1,    68,    69,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      -1,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,   113,    -1,   115,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,    -1,     3,     4,
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
      -1,   172,    -1,    -1,   175,    -1,   177,     3,     4,    -1,
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
      -1,   147,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,   173,    -1,     3,
       4,   177,     6,     7,     8,     9,    10,    11,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,    23,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    33,
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
     122,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,   171,   172,    -1,    -1,
       3,     4,   177,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
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
      -1,    -1,   175,    -1,   177,     3,     4,    -1,     6,     7,
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
     168,    -1,   170,    -1,   172,    -1,    -1,   175,    -1,   177,
       3,     4,    -1,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
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
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
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
     107,    -1,    -1,    -1,   111,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,    -1,     3,     4,
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
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,
       3,     4,   177,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    -1,    -1,
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
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,    -1,     4,   177,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    23,    -1,    25,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    41,    42,    43,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    57,    -1,
      -1,    60,    61,    -1,    -1,    -1,    -1,    66,    -1,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    80,    81,    82,    83,    -1,    -1,    -1,    -1,    88,
      -1,    90,    -1,    92,    -1,    -1,    -1,    -1,    -1,    98,
      99,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,   173,   174,    -1,     4,   177,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    23,    -1,    25,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    41,    42,    43,    -1,    45,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      57,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    66,
      -1,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    80,    81,    82,    83,    -1,    -1,    -1,
      -1,    88,    -1,    90,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    13,    -1,   150,    -1,    -1,    18,    19,    20,    21,
      -1,    -1,   159,    -1,    26,   162,   163,   164,    -1,    -1,
      -1,   168,    34,   170,    36,   172,    -1,   174,    40,    -1,
     177,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,    -1,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,   143,   144,   145,   146,    -1,   148,    -1,    -1,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,   163,    -1,    -1,   166,   167,   168,   169,     3,     4,
     172,     6,     7,     8,     9,    10,    11,    -1,    -1,    -1,
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
      -1,   116,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   169,   163,   164,
     172,    -1,   174,   168,    -1,   170,    -1,   172,    -1,   174,
       3,     4,    -1,     6,     7,     8,     9,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    -1,    21,    -1,
      23,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      43,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    57,    -1,    -1,    60,    61,    -1,
      -1,    64,    -1,    66,    -1,    68,    69,    -1,    71,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    -1,    -1,    88,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
     143,    -1,     3,     4,    -1,     6,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    -1,
      21,   164,    23,    -1,    25,    26,    -1,   170,    -1,   172,
      -1,   174,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    43,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    -1,    57,    -1,    -1,    60,
      61,    -1,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    88,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,     3,    -1,    -1,
     111,    -1,   113,    -1,    -1,   116,    -1,    13,    -1,    -1,
      -1,    -1,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
     141,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      56,    -1,    -1,   164,    -1,    -1,    -1,    -1,    64,    -1,
      -1,   172,    -1,   174,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    -1,   100,    -1,   102,    -1,   104,    -1,
      -1,    -1,    -1,    -1,   110,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,    -1,   138,   139,    -1,   141,   142,   143,   144,   145,
     146,    -1,   148,    -1,    -1,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,     3,   163,    -1,    -1,
     166,   167,   168,   169,   170,    -1,    13,    -1,    -1,    -1,
      -1,    18,    19,    20,    21,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      -1,    98,    -1,   100,    -1,   102,    20,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,   113,    -1,    32,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,    -1,   141,   142,   143,   144,   145,   146,
      -1,   148,    -1,    -1,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,   163,    -1,    -1,   166,
     167,   168,   169,    -1,    -1,    -1,    -1,    91,    -1,    13,
      -1,    95,    -1,    -1,    -1,    -1,    20,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,    -1,   138,   139,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,    -1,   170,    91,    -1,    13,
      -1,    95,    -1,   177,    -1,    -1,    20,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,    -1,   138,   139,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,    -1,   170,    91,    -1,    13,
      -1,    95,    -1,   177,    -1,    -1,    20,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,    -1,   138,   139,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,    -1,   170,    91,    -1,    13,
      -1,    95,    -1,   177,    -1,    -1,    20,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,    -1,   138,   139,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,    -1,   170,    91,    -1,    13,
      -1,    95,    -1,   177,    -1,    -1,    20,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,    -1,   138,   139,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,    -1,   170,    91,    -1,    -1,
      -1,    95,    -1,   177,    -1,    -1,    -1,    -1,    13,   103,
      -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
     124,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,    48,   138,   139,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    13,   177,    -1,    -1,    91,    -1,    -1,    20,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    48,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      91,   166,   167,   168,    95,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    32,   124,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    74,   166,   167,   168,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,    14,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,   124,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    74,   165,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    91,    13,    -1,    -1,    95,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
      91,    -1,    13,    -1,    95,    -1,    -1,   177,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,   170,
      91,    -1,    13,    -1,    95,    -1,   177,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,   170,
      91,    13,    -1,    -1,    95,    -1,   177,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,   177,    -1,    -1,    -1,
      -1,   103,    13,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,   124,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    74,   166,   167,   168,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    13,    -1,    -1,    -1,
      91,    -1,    -1,    20,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      32,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    91,   166,   167,   168,    95,    -1,
      -1,    13,    14,    -1,    -1,    -1,   177,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,   120,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,   120,   166,
     167,   168,   124,    -1,    -1,    -1,    -1,    -1,   175,    91,
     177,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      13,    -1,    -1,   175,    -1,   177,    -1,    20,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    32,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,   177,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      13,    -1,    95,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,   170,    91,    13,
      -1,    -1,    95,    -1,   177,    -1,    20,    -1,   141,   142,
     103,   144,   145,   146,    -1,   148,   149,   150,    32,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   124,   165,   166,   167,   168,    -1,    -1,    -1,    13,
      14,    -1,    -1,    -1,   177,    -1,    20,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    32,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,   166,   167,   168,    -1,    91,    -1,    -1,
      13,    95,    -1,    -1,   177,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,    -1,    -1,    91,   173,
      -1,    -1,    95,   177,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     123,   124,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    13,    -1,   166,   167,   168,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,
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
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    91,
      13,    14,    -1,    95,    -1,   177,    -1,    20,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    32,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      13,   173,    -1,    -1,    -1,   177,    -1,    20,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    32,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,    91,    -1,
      -1,   173,    95,    -1,    -1,   177,    -1,    -1,    13,    14,
      -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    91,    -1,   141,   142,
      95,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    13,    -1,   166,   167,   168,    -1,    -1,    20,   124,
     173,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    13,
      -1,   166,   167,   168,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,   141,
     142,    95,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,   165,   166,   167,   168,    -1,    -1,   123,
     124,    -1,    -1,    -1,    -1,   177,    -1,    91,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      13,    -1,   166,   167,   168,    -1,    -1,    20,    -1,    -1,
      -1,   175,    -1,   177,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    14,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    13,    95,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,   141,   142,
      62,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,   177,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,   124,   166,   167,   168,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    20,   177,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    32,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,   177,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    13,    -1,   103,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,   141,   142,    95,   144,   145,   146,
      -1,   148,   149,   150,   103,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
     177,    -1,    91,    13,    -1,    -1,    95,    -1,    -1,    -1,
      20,    -1,   141,   142,   103,   144,   145,   146,    -1,   148,
     149,   150,    32,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   124,    -1,   166,   167,   168,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,   177,    -1,
      20,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    32,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,    91,    -1,    13,    -1,    95,    -1,    -1,   177,    -1,
      20,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,   141,   142,   103,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    91,    13,    -1,    -1,    95,    -1,   177,    -1,    20,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    32,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    32,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,   177,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,     3,   157,   158,   159,   160,
     161,    -1,   163,    -1,    -1,   166,   167,   168,    -1,    -1,
      18,    19,    -1,    21,    -1,    -1,   177,    -1,    26,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    92,    -1,     3,    -1,     5,    -1,
      98,    -1,   100,    -1,   102,    -1,   104,    -1,    -1,   107,
      -1,    18,    19,   111,    21,   113,    -1,   115,   116,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,
      18,    19,    79,    21,    -1,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    98,    40,   100,    -1,   102,    -1,   104,    46,    -1,
     107,    -1,    50,    -1,   111,    53,   113,    55,    -1,   116,
      -1,    -1,    -1,    -1,     3,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    18,
      19,    79,    21,    -1,    -1,    -1,    -1,    26,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      98,    40,   100,    -1,   102,    -1,   104,    46,    -1,   107,
      -1,    50,    -1,   111,    53,   113,    55,    -1,   116,    -1,
      -1,    -1,    -1,     3,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    -1,    -1,    -1,    -1,    18,    19,
      79,    21,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    98,
      40,   100,    -1,   102,    -1,   104,    46,    -1,   107,    -1,
      50,    -1,   111,    53,   113,    55,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
     100,    -1,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,   113,    -1,    -1,   116
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
     232,   233,   234,   236,   239,   240,   241,   245,   247,   248,
     251,   252,   253,   254,   255,   259,   260,   265,   266,   267,
     268,   271,   272,   277,   278,   280,   281,   286,   290,   291,
     293,   294,   315,   320,   321,   325,   326,   327,   349,   350,
     351,   353,   355,   356,   357,   363,   364,   365,   366,   367,
     368,   370,   373,   374,   375,   376,   377,   378,   379,   380,
     382,   383,   384,   385,   386,   165,    23,    36,    45,    55,
      57,    90,   102,   170,   240,   251,   278,   349,   356,   364,
     365,   370,   373,   375,   376,   120,   358,   359,     3,    55,
     218,   370,   358,   110,   328,    92,   218,   186,   343,   370,
     172,     3,    18,    19,    21,    26,    34,    40,    46,    50,
      53,    55,    64,    71,    72,    79,    89,    98,   100,   102,
     104,   107,   111,   113,   116,   219,   220,   222,    12,    28,
     115,   246,   370,    84,    87,   202,   172,   104,   220,   220,
     220,   172,   220,   330,   331,    33,   206,   225,   370,   263,
     264,   370,   370,    19,    79,    98,   113,   370,   370,   370,
       9,   172,   229,   228,    28,    34,    48,    50,    52,    77,
      80,    92,   102,   110,   115,   185,   224,   282,   283,   284,
     306,   307,   308,   335,   340,   370,   343,   108,   109,   165,
     286,   287,   369,   370,   372,   370,   225,   370,   370,   101,
     172,   186,   370,   370,    73,   188,   192,   206,   188,   206,
     370,   372,   370,   370,   370,   370,   370,     1,   171,   184,
     207,   343,   151,   344,   345,   372,   370,    82,   186,    23,
      36,    39,    73,    90,   170,   189,   190,   191,   202,   206,
     194,   149,   196,   170,    47,    86,   114,   203,    27,   327,
     370,     9,   267,   370,   371,    25,    33,    41,    42,    43,
     119,   174,   242,   243,   352,   354,   172,   206,    57,   146,
     269,   220,   170,   172,   302,    54,    75,    85,   310,    28,
      48,    52,    77,    80,    92,   110,   115,   311,    28,    48,
      52,    77,    80,    92,   110,   115,   309,   220,   322,   323,
       1,   330,   164,   165,   370,    13,    20,    32,    91,    95,
     124,   141,   142,   144,   145,   146,   148,   149,   150,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   166,   167,   168,   177,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     169,   298,   172,   174,    91,    95,   370,   220,    41,   174,
     242,    57,     1,   172,   186,   165,   206,   326,   165,   147,
     165,   221,   346,   347,   348,   372,   220,   220,   103,   206,
     103,   125,   206,   298,   203,   346,   165,   172,   186,   172,
     220,   332,   333,     1,   146,   339,    48,   147,   186,   225,
     147,   225,    14,   172,   172,   225,   329,   346,   230,   230,
      48,    92,   307,   308,   173,   147,   172,   220,   146,   165,
     370,   370,   123,   288,   165,   170,   225,   172,   346,   165,
     370,   256,   256,   165,   171,   171,   184,   147,   171,   370,
     147,   147,   173,   173,     9,   370,   174,   242,   243,     3,
     172,   197,     1,   171,   207,   214,   215,   370,   209,   370,
      67,    37,    74,   165,   267,   269,   110,   235,   237,   291,
     186,    80,   246,   370,   122,   175,   244,   343,   370,   381,
     244,   370,   220,   249,   250,   220,   343,   170,     1,   193,
     220,   273,   276,   173,   303,   305,   306,   311,    48,    92,
       1,   146,   337,   338,    48,    92,     1,     3,    13,    18,
      20,    21,    26,    46,    53,    55,    56,    64,    72,    89,
     100,   104,   111,   116,   141,   142,   143,   144,   145,   146,
     148,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   163,   166,   167,   168,   169,   172,   220,   295,   296,
     297,   298,   349,   125,   324,   147,   165,   165,   165,   370,
     370,   370,   244,   370,   244,   370,   370,   370,   370,   370,
     370,   370,     3,    21,    34,    64,   104,   110,   221,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   370,    70,   372,   372,   372,
     372,   372,   346,   346,   244,   370,   244,   370,   103,   172,
     343,   244,   370,   220,     1,    28,    48,    52,    77,    80,
      92,   110,   115,   173,   360,   361,   362,   370,   386,   330,
     370,   125,   173,   147,   125,   125,   186,    35,   186,   370,
      35,   370,    67,   173,   347,   333,   147,   173,   224,   370,
     125,   334,   370,   331,   263,   220,   325,   370,   370,   326,
     173,   220,   311,   284,   333,   146,   341,   342,   340,   289,
     205,     1,   261,   346,   173,    22,   257,   171,   173,   173,
     344,   372,   173,   344,   343,   244,   370,   196,   173,   198,
     199,   348,   171,   171,   147,   165,    14,   149,   147,   165,
      14,    37,    74,   220,   141,   142,   143,   144,   145,   159,
     163,   168,   208,   297,   298,   299,   370,   208,   210,   269,
     170,   238,   326,   119,   220,   225,   225,   172,   370,   175,
     359,    48,   147,   175,   359,   123,   147,   175,    33,   225,
     359,    33,    48,   225,   359,   147,   173,   165,   172,     1,
     270,   171,   276,   125,   147,   171,   147,   173,   339,     1,
     224,   335,   336,   106,   312,   172,   301,   370,   141,   149,
     301,   301,   370,   323,   172,   174,   165,   165,   165,   165,
     165,   165,   173,   175,   347,    48,   175,    33,    33,    48,
     172,   173,    48,    92,   147,   173,    18,    21,    26,    46,
      53,    64,    72,    89,   100,   111,   116,   349,    91,    91,
     165,   372,   348,   370,   370,    35,   186,    35,    35,   186,
     103,   206,   220,   173,   173,   332,   339,    70,   372,   220,
     173,   173,   330,   342,   146,   285,   173,   340,   151,   300,
     334,   370,   171,    76,   117,   171,   262,   173,   172,   206,
     220,   258,    48,   175,    48,   147,   173,   214,   221,    18,
      19,    21,    26,    46,    50,    53,    72,    79,    89,    98,
     100,   102,   111,   113,   116,   170,   213,   299,   370,   370,
     208,   210,   147,   165,    14,   165,   170,   270,   322,   330,
     370,    48,   343,   186,   175,   244,   370,   175,   186,   370,
     175,   370,   175,   370,   370,   225,    45,   370,   244,   370,
     225,   250,   206,   347,   171,    84,    87,   171,   185,   193,
     227,   370,   274,   275,   305,   312,    62,   118,   316,   303,
     304,   173,   296,   298,   173,   175,   173,   244,   370,    45,
     244,   370,   347,   361,   339,   349,   349,   186,   186,   370,
     186,    35,   165,   165,   334,   172,   172,   165,   300,   334,
     335,   312,   342,   370,   186,   240,   343,   258,   146,   206,
     244,   370,   244,   370,   199,   208,    14,    37,    74,   165,
     165,   299,   370,   370,   270,   171,   165,   165,   165,   370,
     173,   186,   175,   359,   175,   359,   186,   123,   370,    33,
     225,   359,    33,   225,   359,   173,   193,   227,   227,   276,
     193,   316,     3,    55,    94,   104,   317,   318,   319,   370,
     292,   173,   301,   301,   175,   175,   370,    33,    33,   173,
     334,    35,   186,   346,   346,   300,   334,    33,   225,   173,
     370,   175,   175,   171,   370,   208,   210,    14,   171,   225,
      45,   186,   370,   175,    45,   186,   370,   175,   370,   103,
      45,   370,   225,    45,   370,   225,   165,   227,   276,   279,
     319,   118,   147,   125,   152,   154,   155,   157,   158,    62,
      33,   165,   205,   313,   314,    45,    45,   103,    45,    45,
     186,   173,   173,   186,   206,   165,   165,   370,   370,   186,
     370,   186,   370,   370,   370,   314,   370,   318,   319,   319,
     319,   319,   319,   319,   317,   184,   370,   370,   370,   370,
     164,   164,   103,   103,   103,   103,   103,   103,   103,   103,
     370,   370,   370,   370
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   181,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   189,   189,   189,   190,   191,
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
     235,   236,   236,   236,   237,   236,   238,   236,   239,   240,
     241,   241,   242,   242,   242,   242,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   244,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   246,
     246,   247,   248,   248,   249,   249,   250,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     252,   252,   252,   252,   252,   252,   253,   253,   253,   254,
     254,   254,   255,   255,   255,   255,   256,   256,   257,   257,
     257,   258,   258,   259,   260,   260,   261,   261,   262,   262,
     262,   263,   263,   263,   264,   264,   265,   266,   266,   267,
     268,   268,   268,   269,   269,   270,   270,   270,   270,   270,
     271,   271,   272,   273,   273,   274,   273,   273,   275,   273,
     276,   276,   277,   279,   278,   280,   281,   281,   281,   282,
     282,   283,   283,   284,   284,   284,   285,   285,   286,   288,
     287,   289,   287,   290,   292,   291,   293,   293,   293,   293,
     293,   294,   295,   295,   296,   296,   296,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   299,   299,   300,   300,   301,   301,
     302,   302,   303,   303,   304,   304,   305,   306,   306,   306,
     306,   306,   306,   307,   307,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   309,   310,   310,   310,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     312,   312,   313,   313,   314,   314,   315,   316,   316,   316,
     316,   316,   317,   317,   318,   318,   318,   318,   318,   318,
     318,   319,   319,   319,   320,   321,   321,   322,   322,   323,
     324,   324,   325,   325,   325,   325,   325,   326,   328,   327,
     329,   327,   327,   327,   330,   330,   331,   331,   332,   332,
     333,   333,   333,   334,   334,   334,   335,   336,   336,   336,
     337,   337,   338,   338,   339,   339,   339,   339,   340,   340,
     341,   342,   342,   343,   343,   344,   345,   345,   346,   346,
     347,   347,   348,   348,   349,   349,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   351,   352,   352,
     352,   352,   352,   352,   352,   353,   354,   354,   354,   354,
     354,   354,   354,   355,   356,   357,   357,   357,   357,   357,
     357,   357,   358,   358,   359,   359,   359,   359,   360,   360,
     361,   361,   361,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   363,   363,   363,   363,   363,   364,   364,
     364,   364,   364,   365,   366,   366,   366,   366,   366,   366,
     367,   368,   369,   369,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   371,   371,   372,   372,   372,   373,   373,   373,   373,
     374,   374,   374,   374,   374,   375,   375,   375,   376,   376,
     376,   376,   376,   376,   377,   377,   377,   377,   378,   378,
     379,   379,   380,   380,   380,   380,   380,   380,   380,   380,
     380,   380,   380,   380,   380,   381,   381,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     383,   383,   383,   383,   383,   383,   383,   384,   384,   384,
     384,   385,   385,   385,   385,   386,   386,   386,   386,   386,
     386,   386
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       4,     4,     5,     4,     5,     3,     5,     3,     3,     3,
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
#line 665 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 5928 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 669 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 5934 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 670 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 5940 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 677 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 5948 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 685 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 5956 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 689 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 5965 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 699 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 5973 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 704 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 5981 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 711 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5987 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 712 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5993 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 713 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5999 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 714 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6005 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 715 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6011 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 716 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6017 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 717 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6023 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 718 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6029 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: TDEFER stmt  */
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
#line 6051 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: try_stmt  */
#line 737 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6057 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: return_stmt  */
#line 738 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6063 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: TINIT TTHIS TSEMI  */
#line 740 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr));
    auto node = Init::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6075 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 748 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6088 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 757 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6101 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 766 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6109 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 770 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6120 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 777 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6129 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 784 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6135 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 785 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6141 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 786 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6147 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 787 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6153 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 788 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6159 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 789 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6165 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 790 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6171 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 792 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6177 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
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
#line 6201 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 814 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6213 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL expr do_stmt  */
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
#line 6234 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL do_stmt  */
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
#line 6254 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TON expr do_stmt  */
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
#line 6275 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSERIAL expr do_stmt  */
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
#line 6296 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL do_stmt  */
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
#line 6316 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSYNC stmt  */
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
#line 6338 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 931 "chpl.ypp"
  {
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 6346 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 952 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6354 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 959 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6363 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 966 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6371 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 970 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6382 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 980 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6388 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 982 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6396 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 986 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6404 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 993 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6413 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 998 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6423 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 1011 "chpl.ypp"
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
#line 6445 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 1032 "chpl.ypp"
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
#line 6466 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 1049 "chpl.ypp"
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
#line 6487 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1066 "chpl.ypp"
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
#line 6506 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1083 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6512 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1084 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6519 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1086 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6526 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1091 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6532 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1092 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6539 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1098 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6549 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1116 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6558 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1121 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6567 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1126 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6580 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1138 "chpl.ypp"
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
#line 6599 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1156 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6605 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1157 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6611 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1162 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6619 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1166 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6627 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1170 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6636 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1175 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6644 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1179 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6652 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1183 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6661 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1191 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6670 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1196 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6680 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1202 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6689 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1207 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6700 "bison-chpl-lib.cpp"
    break;

  case 90: /* opt_only_ls: %empty  */
#line 1215 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6706 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: renames_ls  */
#line 1216 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6712 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1221 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6721 "bison-chpl-lib.cpp"
    break;

  case 93: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1226 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6733 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1234 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6746 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1243 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6758 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1251 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6771 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1262 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6780 "bison-chpl-lib.cpp"
    break;

  case 114: /* import_expr: expr  */
#line 1297 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6788 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr TDOT all_op_name  */
#line 1301 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6797 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1306 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6806 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TAS ident_use  */
#line 1311 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6816 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1317 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6826 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_ls: import_expr  */
#line 1325 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6832 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_ls TCOMMA import_expr  */
#line 1326 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6838 "bison-chpl-lib.cpp"
    break;

  case 121: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1331 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6848 "bison-chpl-lib.cpp"
    break;

  case 122: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1340 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6856 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1344 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6864 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1348 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6872 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1352 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6880 "bison-chpl-lib.cpp"
    break;

  case 126: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1356 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6888 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1360 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6896 "bison-chpl-lib.cpp"
    break;

  case 128: /* opt_label_ident: %empty  */
#line 1368 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6902 "bison-chpl-lib.cpp"
    break;

  case 129: /* opt_label_ident: TIDENT  */
#line 1369 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6908 "bison-chpl-lib.cpp"
    break;

  case 130: /* opt_label_ident: TINIT  */
#line 1370 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6914 "bison-chpl-lib.cpp"
    break;

  case 139: /* ident_use: TIDENT  */
#line 1396 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6920 "bison-chpl-lib.cpp"
    break;

  case 140: /* ident_use: TINIT  */
#line 1397 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6926 "bison-chpl-lib.cpp"
    break;

  case 141: /* ident_use: TTHIS  */
#line 1398 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6932 "bison-chpl-lib.cpp"
    break;

  case 160: /* scalar_type: TBOOL  */
#line 1424 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6938 "bison-chpl-lib.cpp"
    break;

  case 161: /* scalar_type: TENUM  */
#line 1425 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6944 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TINT  */
#line 1426 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6950 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TUINT  */
#line 1427 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6956 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TREAL  */
#line 1428 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6962 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TIMAG  */
#line 1429 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6968 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TCOMPLEX  */
#line 1430 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6974 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TBYTES  */
#line 1431 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6980 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TSTRING  */
#line 1432 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6986 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TLOCALE  */
#line 1433 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6992 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TNOTHING  */
#line 1434 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6998 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TVOID  */
#line 1435 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7004 "bison-chpl-lib.cpp"
    break;

  case 175: /* do_stmt: TDO stmt  */
#line 1448 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7010 "bison-chpl-lib.cpp"
    break;

  case 176: /* do_stmt: block_stmt  */
#line 1449 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7016 "bison-chpl-lib.cpp"
    break;

  case 177: /* return_stmt: TRETURN TSEMI  */
#line 1454 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7027 "bison-chpl-lib.cpp"
    break;

  case 178: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1461 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7038 "bison-chpl-lib.cpp"
    break;

  case 179: /* class_level_stmt: TSEMI  */
#line 1471 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7046 "bison-chpl-lib.cpp"
    break;

  case 180: /* class_level_stmt: inner_class_level_stmt  */
#line 1475 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7056 "bison-chpl-lib.cpp"
    break;

  case 181: /* $@1: %empty  */
#line 1480 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7063 "bison-chpl-lib.cpp"
    break;

  case 182: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1482 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7072 "bison-chpl-lib.cpp"
    break;

  case 183: /* $@2: %empty  */
#line 1486 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7079 "bison-chpl-lib.cpp"
    break;

  case 184: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1488 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7088 "bison-chpl-lib.cpp"
    break;

  case 192: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1506 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7097 "bison-chpl-lib.cpp"
    break;

  case 193: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1511 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7106 "bison-chpl-lib.cpp"
    break;

  case 194: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1516 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7115 "bison-chpl-lib.cpp"
    break;

  case 195: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1521 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7123 "bison-chpl-lib.cpp"
    break;

  case 196: /* forwarding_decl_start: TFORWARDING  */
#line 1528 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7132 "bison-chpl-lib.cpp"
    break;

  case 197: /* extern_or_export: TEXTERN  */
#line 1535 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7138 "bison-chpl-lib.cpp"
    break;

  case 198: /* extern_or_export: TEXPORT  */
#line 1536 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7144 "bison-chpl-lib.cpp"
    break;

  case 199: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1541 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7154 "bison-chpl-lib.cpp"
    break;

  case 200: /* extern_export_kind: TTYPE  */
#line 1550 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7163 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1559 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7173 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1566 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7185 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1574 "chpl.ypp"
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
#line 7207 "bison-chpl-lib.cpp"
    break;

  case 204: /* $@3: %empty  */
#line 1592 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7215 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 1596 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-4])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7224 "bison-chpl-lib.cpp"
    break;

  case 206: /* $@4: %empty  */
#line 1601 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7233 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr extern_export_kind $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1606 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yylsp[-5]), (yyvsp[-3].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7242 "bison-chpl-lib.cpp"
    break;

  case 208: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1614 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7250 "bison-chpl-lib.cpp"
    break;

  case 209: /* no_loop_attributes: %empty  */
#line 1620 "chpl.ypp"
  {
    // Even though there are no attributes, there could be loop pragmas.
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 7259 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1626 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7265 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1627 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7271 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1644 "chpl.ypp"
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
#line 7298 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1667 "chpl.ypp"
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
#line 7321 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1686 "chpl.ypp"
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
#line 7344 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1705 "chpl.ypp"
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
#line 7373 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1730 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7382 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: for_loop_kw expr TIN expr task_intent_clause do_stmt  */
#line 1735 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7391 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1740 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7400 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator task_intent_clause do_stmt  */
#line 1745 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7409 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1750 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7418 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw expr task_intent_clause do_stmt  */
#line 1755 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7427 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1760 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7436 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: for_loop_kw zippered_iterator task_intent_clause do_stmt  */
#line 1765 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7445 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1770 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7454 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TLSBR expr_ls TIN expr task_intent_clause TRSBR stmt  */
#line 1775 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7463 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1780 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7472 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator task_intent_clause TRSBR stmt  */
#line 1785 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7481 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1790 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7490 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR expr_ls task_intent_clause TRSBR stmt  */
#line 1795 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7499 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1800 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7509 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TLSBR zippered_iterator task_intent_clause TRSBR stmt  */
#line 1806 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7519 "bison-chpl-lib.cpp"
    break;

  case 236: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1815 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7529 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr TTHEN stmt  */
#line 1824 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7537 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr block_stmt  */
#line 1828 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7545 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1832 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7553 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1836 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7561 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1840 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7569 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF ifvar block_stmt  */
#line 1844 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7577 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1848 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7585 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1852 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7593 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1856 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7602 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1861 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7611 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1866 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7620 "bison-chpl-lib.cpp"
    break;

  case 248: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1871 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7629 "bison-chpl-lib.cpp"
    break;

  case 249: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1879 "chpl.ypp"
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
#line 7648 "bison-chpl-lib.cpp"
    break;

  case 250: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1894 "chpl.ypp"
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
#line 7667 "bison-chpl-lib.cpp"
    break;

  case 251: /* interface_start: TINTERFACE ident_def  */
#line 1912 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Interface);
  }
#line 7675 "bison-chpl-lib.cpp"
    break;

  case 252: /* interface_stmt: interface_start TLP ifc_formal_ls TRP block_stmt  */
#line 1919 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-4].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), parts, (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7685 "bison-chpl-lib.cpp"
    break;

  case 253: /* interface_stmt: interface_start block_stmt  */
#line 1925 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-1].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), parts, nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7695 "bison-chpl-lib.cpp"
    break;

  case 254: /* ifc_formal_ls: ifc_formal  */
#line 1933 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7701 "bison-chpl-lib.cpp"
    break;

  case 255: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1934 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7707 "bison-chpl-lib.cpp"
    break;

  case 256: /* ifc_formal: ident_def  */
#line 1939 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7713 "bison-chpl-lib.cpp"
    break;

  case 276: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1982 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7721 "bison-chpl-lib.cpp"
    break;

  case 277: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1986 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7729 "bison-chpl-lib.cpp"
    break;

  case 278: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1990 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7737 "bison-chpl-lib.cpp"
    break;

  case 279: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1997 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7745 "bison-chpl-lib.cpp"
    break;

  case 280: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 2001 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7753 "bison-chpl-lib.cpp"
    break;

  case 281: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2005 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7761 "bison-chpl-lib.cpp"
    break;

  case 282: /* try_stmt: TTRY tryable_stmt  */
#line 2012 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7769 "bison-chpl-lib.cpp"
    break;

  case 283: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2016 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7777 "bison-chpl-lib.cpp"
    break;

  case 284: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2020 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7785 "bison-chpl-lib.cpp"
    break;

  case 285: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2024 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7793 "bison-chpl-lib.cpp"
    break;

  case 286: /* catch_expr_ls: %empty  */
#line 2030 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7799 "bison-chpl-lib.cpp"
    break;

  case 287: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2031 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7805 "bison-chpl-lib.cpp"
    break;

  case 288: /* catch_expr: TCATCH block_stmt  */
#line 2036 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7813 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2040 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7821 "bison-chpl-lib.cpp"
    break;

  case 290: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2044 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7829 "bison-chpl-lib.cpp"
    break;

  case 291: /* catch_expr_inner: ident_def  */
#line 2051 "chpl.ypp"
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
#line 7846 "bison-chpl-lib.cpp"
    break;

  case 292: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2064 "chpl.ypp"
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
#line 7863 "bison-chpl-lib.cpp"
    break;

  case 293: /* throw_stmt: TTHROW expr TSEMI  */
#line 2080 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7873 "bison-chpl-lib.cpp"
    break;

  case 294: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2088 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7882 "bison-chpl-lib.cpp"
    break;

  case 295: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2093 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7893 "bison-chpl-lib.cpp"
    break;

  case 296: /* when_stmt_ls: %empty  */
#line 2102 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7899 "bison-chpl-lib.cpp"
    break;

  case 297: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2103 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7905 "bison-chpl-lib.cpp"
    break;

  case 298: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2108 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7913 "bison-chpl-lib.cpp"
    break;

  case 299: /* when_stmt: TOTHERWISE stmt  */
#line 2112 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7922 "bison-chpl-lib.cpp"
    break;

  case 300: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2117 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 7932 "bison-chpl-lib.cpp"
    break;

  case 301: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2126 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 7941 "bison-chpl-lib.cpp"
    break;

  case 302: /* manager_expr: expr TAS ident_def  */
#line 2131 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 7949 "bison-chpl-lib.cpp"
    break;

  case 303: /* manager_expr: expr  */
#line 2135 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 7957 "bison-chpl-lib.cpp"
    break;

  case 304: /* manager_expr_ls: manager_expr  */
#line 2141 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7963 "bison-chpl-lib.cpp"
    break;

  case 305: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2142 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7969 "bison-chpl-lib.cpp"
    break;

  case 306: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2147 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 7978 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2157 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 7988 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2163 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8000 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_start: class_tag ident_def  */
#line 2175 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8008 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_tag: TCLASS  */
#line 2181 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8014 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_tag: TRECORD  */
#line 2182 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8020 "bison-chpl-lib.cpp"
    break;

  case 312: /* class_tag: TUNION  */
#line 2183 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8026 "bison-chpl-lib.cpp"
    break;

  case 313: /* opt_inherit: %empty  */
#line 2187 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8032 "bison-chpl-lib.cpp"
    break;

  case 314: /* opt_inherit: TCOLON expr_ls  */
#line 2188 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8038 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_level_stmt_ls: %empty  */
#line 2192 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8047 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2197 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8055 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2201 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8063 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2205 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8071 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2209 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8079 "bison-chpl-lib.cpp"
    break;

  case 320: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
#line 2217 "chpl.ypp"
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
#line 8102 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
#line 2236 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8116 "bison-chpl-lib.cpp"
    break;

  case 322: /* enum_header: TENUM ident_def  */
#line 2249 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8124 "bison-chpl-lib.cpp"
    break;

  case 323: /* enum_ls: enum_item  */
#line 2256 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8133 "bison-chpl-lib.cpp"
    break;

  case 324: /* enum_ls: enum_ls TCOMMA  */
#line 2261 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8143 "bison-chpl-lib.cpp"
    break;

  case 325: /* $@5: %empty  */
#line 2267 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8152 "bison-chpl-lib.cpp"
    break;

  case 326: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2272 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8161 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2277 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8170 "bison-chpl-lib.cpp"
    break;

  case 328: /* $@6: %empty  */
#line 2282 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8179 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2287 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8188 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_item: ident_def  */
#line 2297 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[0])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8200 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_item: ident_def TASSIGN expr  */
#line 2305 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[-2])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8214 "bison-chpl-lib.cpp"
    break;

  case 332: /* lambda_decl_start: TLAMBDA  */
#line 2318 "chpl.ypp"
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
#line 8230 "bison-chpl-lib.cpp"
    break;

  case 333: /* $@7: %empty  */
#line 2335 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8240 "bison-chpl-lib.cpp"
    break;

  case 334: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2341 "chpl.ypp"
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
#line 8263 "bison-chpl-lib.cpp"
    break;

  case 336: /* linkage_spec: linkage_spec_empty  */
#line 2365 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8269 "bison-chpl-lib.cpp"
    break;

  case 337: /* linkage_spec: TINLINE  */
#line 2366 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8276 "bison-chpl-lib.cpp"
    break;

  case 338: /* linkage_spec: TOVERRIDE  */
#line 2368 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8283 "bison-chpl-lib.cpp"
    break;

  case 339: /* opt_fn_type_formal_ls: %empty  */
#line 2373 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8289 "bison-chpl-lib.cpp"
    break;

  case 340: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2374 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8295 "bison-chpl-lib.cpp"
    break;

  case 341: /* fn_type_formal_ls: fn_type_formal  */
#line 2378 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8301 "bison-chpl-lib.cpp"
    break;

  case 342: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2379 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8307 "bison-chpl-lib.cpp"
    break;

  case 343: /* fn_type_formal: named_formal  */
#line 2384 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8313 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2387 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8319 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_type_formal: formal_type  */
#line 2389 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8325 "bison-chpl-lib.cpp"
    break;

  case 346: /* opt_fn_type_ret_type: %empty  */
#line 2393 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8331 "bison-chpl-lib.cpp"
    break;

  case 347: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2394 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8337 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2407 "chpl.ypp"
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
#line 8357 "bison-chpl-lib.cpp"
    break;

  case 349: /* $@8: %empty  */
#line 2426 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8367 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2432 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8380 "bison-chpl-lib.cpp"
    break;

  case 351: /* $@9: %empty  */
#line 2441 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8390 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2447 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8403 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2459 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8411 "bison-chpl-lib.cpp"
    break;

  case 354: /* $@10: %empty  */
#line 2468 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8419 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2472 "chpl.ypp"
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
#line 8442 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2494 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.thisIntentLoc = (yylsp[-2]);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8455 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2503 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.thisIntentLoc = (yylsp[-2]);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8468 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2512 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.thisIntentLoc = (yylsp[-4]);
      fp.receiver = context->buildThisFormal((yylsp[-3]), (yylsp[-4]), (yyvsp[-4].intentTag), (yyvsp[-3].expr), /*initExpr*/ nullptr);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8482 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2522 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.thisIntentLoc = (yylsp[-4]);
      fp.receiver = context->buildThisFormal((yylsp[-3]), (yylsp[-4]), (yyvsp[-4].intentTag), (yyvsp[-3].expr), /*initExpr*/ nullptr);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8496 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2532 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8506 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2541 "chpl.ypp"
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
#line 8522 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2557 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenthLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 8531 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_ident: ident_def TBANG  */
#line 2567 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8541 "bison-chpl-lib.cpp"
    break;

  case 406: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2624 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8547 "bison-chpl-lib.cpp"
    break;

  case 407: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2625 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8553 "bison-chpl-lib.cpp"
    break;

  case 408: /* opt_formal_ls: %empty  */
#line 2629 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8559 "bison-chpl-lib.cpp"
    break;

  case 409: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2630 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8565 "bison-chpl-lib.cpp"
    break;

  case 410: /* req_formal_ls: TLP TRP  */
#line 2634 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8571 "bison-chpl-lib.cpp"
    break;

  case 411: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2635 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8577 "bison-chpl-lib.cpp"
    break;

  case 412: /* formal_ls_inner: formal  */
#line 2639 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8583 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2640 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8589 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_ls: %empty  */
#line 2644 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8595 "bison-chpl-lib.cpp"
    break;

  case 415: /* formal_ls: formal_ls_inner  */
#line 2645 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8601 "bison-chpl-lib.cpp"
    break;

  case 417: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2654 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8609 "bison-chpl-lib.cpp"
    break;

  case 418: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2659 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8617 "bison-chpl-lib.cpp"
    break;

  case 419: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2664 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8625 "bison-chpl-lib.cpp"
    break;

  case 420: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2669 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8633 "bison-chpl-lib.cpp"
    break;

  case 421: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2674 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yylsp[-5]), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8641 "bison-chpl-lib.cpp"
    break;

  case 422: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2679 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8649 "bison-chpl-lib.cpp"
    break;

  case 423: /* opt_formal_intent_tag: %empty  */
#line 2685 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);
  }
#line 8658 "bison-chpl-lib.cpp"
    break;

  case 424: /* opt_formal_intent_tag: required_intent_tag  */
#line 2690 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8667 "bison-chpl-lib.cpp"
    break;

  case 425: /* required_intent_tag: TIN  */
#line 2697 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::IN);            }
#line 8673 "bison-chpl-lib.cpp"
    break;

  case 426: /* required_intent_tag: TINOUT  */
#line 2698 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::INOUT);         }
#line 8679 "bison-chpl-lib.cpp"
    break;

  case 427: /* required_intent_tag: TOUT  */
#line 2699 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::OUT);           }
#line 8685 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TCONST TIN  */
#line 2700 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_IN);      }
#line 8691 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TCONST TREF  */
#line 2701 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);     }
#line 8697 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TCONST  */
#line 2702 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);         }
#line 8703 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TPARAM  */
#line 2703 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);         }
#line 8709 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TREF  */
#line 2704 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);           }
#line 8715 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TTYPE  */
#line 2705 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);          }
#line 8721 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TVAR  */
#line 2706 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR); }
#line 8727 "bison-chpl-lib.cpp"
    break;

  case 435: /* opt_this_intent_tag: %empty  */
#line 2710 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);     }
#line 8733 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_this_intent_tag: TPARAM  */
#line 2711 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);              }
#line 8739 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: TREF  */
#line 2712 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);                }
#line 8745 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TCONST TREF  */
#line 2713 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);          }
#line 8751 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TCONST  */
#line 2714 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);              }
#line 8757 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TTYPE  */
#line 2715 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);               }
#line 8763 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TIN  */
#line 2716 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::IN);       }
#line 8769 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TCONST TIN  */
#line 2717 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::CONST_IN); }
#line 8775 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_this_intent_tag: TVAR  */
#line 2718 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR);      }
#line 8781 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_this_intent_tag: TOUT  */
#line 2719 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::OUT);      }
#line 8787 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_this_intent_tag: TINOUT  */
#line 2720 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::INOUT);    }
#line 8793 "bison-chpl-lib.cpp"
    break;

  case 446: /* proc_iter_or_op: TPROC  */
#line 2724 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8799 "bison-chpl-lib.cpp"
    break;

  case 447: /* proc_iter_or_op: TITER  */
#line 2725 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8805 "bison-chpl-lib.cpp"
    break;

  case 448: /* proc_iter_or_op: TOPERATOR  */
#line 2726 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8811 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: %empty  */
#line 2730 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::DEFAULT_RETURN_INTENT); }
#line 8817 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TOUT  */
#line 2731 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::OUT);                   }
#line 8823 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TCONST  */
#line 2732 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST);                 }
#line 8829 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TCONST TREF  */
#line 2733 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST_REF);             }
#line 8835 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_ret_tag: TREF  */
#line 2734 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::REF);                   }
#line 8841 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: TPARAM  */
#line 2735 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::PARAM);                 }
#line 8847 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: TTYPE  */
#line 2736 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::TYPE);                  }
#line 8853 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: TIN  */
#line 2737 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::IN);            }
#line 8859 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_ret_tag: TCONST TIN  */
#line 2738 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::CONST_IN);      }
#line 8865 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_ret_tag: TVAR  */
#line 2739 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::VAR);           }
#line 8871 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_ret_tag: TINOUT  */
#line 2740 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::INOUT);         }
#line 8877 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_throws_error: %empty  */
#line 2744 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8883 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_throws_error: TTHROWS  */
#line 2745 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8889 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_function_body_stmt: TSEMI  */
#line 2748 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8895 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_function_body_stmt: function_body_stmt  */
#line 2749 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8901 "bison-chpl-lib.cpp"
    break;

  case 464: /* function_body_stmt: block_stmt_body  */
#line 2753 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8907 "bison-chpl-lib.cpp"
    break;

  case 465: /* function_body_stmt: TDO toplevel_stmt  */
#line 2754 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 8913 "bison-chpl-lib.cpp"
    break;

  case 466: /* query_expr: TQUERIEDIDENT  */
#line 2758 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8919 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_lifetime_where: %empty  */
#line 2763 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8925 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_lifetime_where: TWHERE expr  */
#line 2765 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8931 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2767 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8937 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2769 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 8943 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2771 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 8949 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_components_expr: lifetime_expr  */
#line 2776 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8955 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2778 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8961 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2783 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8967 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2785 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8973 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2787 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8979 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2789 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8985 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2791 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8991 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2793 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8997 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2795 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9003 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_ident: TIDENT  */
#line 2799 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9009 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_ident: TINIT  */
#line 2800 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9015 "bison-chpl-lib.cpp"
    break;

  case 483: /* lifetime_ident: TTHIS  */
#line 2801 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9021 "bison-chpl-lib.cpp"
    break;

  case 484: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2806 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 485: /* type_alias_decl_stmt_start: TTYPE  */
#line 2815 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9040 "bison-chpl-lib.cpp"
    break;

  case 486: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2821 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9051 "bison-chpl-lib.cpp"
    break;

  case 487: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2831 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9059 "bison-chpl-lib.cpp"
    break;

  case 488: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2835 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9067 "bison-chpl-lib.cpp"
    break;

  case 489: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2842 "chpl.ypp"
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
#line 9090 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_init_type: %empty  */
#line 2863 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9096 "bison-chpl-lib.cpp"
    break;

  case 491: /* opt_init_type: TASSIGN expr  */
#line 2865 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9102 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_type: TPARAM  */
#line 2869 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9108 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_type: TCONST TREF  */
#line 2870 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9114 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_type: TREF  */
#line 2871 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9120 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_type: TCONST  */
#line 2872 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9126 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_type: TVAR  */
#line 2873 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9132 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_start: var_decl_type  */
#line 2878 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9141 "bison-chpl-lib.cpp"
    break;

  case 498: /* $@11: %empty  */
#line 2886 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9150 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt: TCONFIG $@11 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2890 "chpl.ypp"
                                              {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-4]), (yylsp[-3])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9159 "bison-chpl-lib.cpp"
    break;

  case 500: /* $@12: %empty  */
#line 2895 "chpl.ypp"
  {
    // Use a mid-rule action to preserve the 'on' expr
    context->noteVarDestinationExpr((yyvsp[0].expr));
  }
#line 9168 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt: TON expr $@12 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2900 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-4])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9177 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt: var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2905 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9186 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt: var_decl_start error TSEMI  */
#line 2910 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = { .comments=nullptr, .stmt=ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release() };
    delete (yyvsp[-2].attributeGroup);
    context->resetDeclStateOnError();
  }
#line 9196 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2919 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9204 "bison-chpl-lib.cpp"
    break;

  case 505: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2923 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9212 "bison-chpl-lib.cpp"
    break;

  case 506: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2930 "chpl.ypp"
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
#line 9235 "bison-chpl-lib.cpp"
    break;

  case 507: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2949 "chpl.ypp"
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
#line 9255 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_var_decl_component: ident_def  */
#line 2968 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9263 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2972 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9271 "bison-chpl-lib.cpp"
    break;

  case 510: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2979 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9277 "bison-chpl-lib.cpp"
    break;

  case 511: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2981 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9283 "bison-chpl-lib.cpp"
    break;

  case 512: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2983 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9289 "bison-chpl-lib.cpp"
    break;

  case 513: /* opt_init_expr: %empty  */
#line 2989 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9295 "bison-chpl-lib.cpp"
    break;

  case 514: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2990 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9301 "bison-chpl-lib.cpp"
    break;

  case 515: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2991 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9307 "bison-chpl-lib.cpp"
    break;

  case 516: /* formal_or_ret_type_expr: expr  */
#line 2995 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9313 "bison-chpl-lib.cpp"
    break;

  case 517: /* ret_type: formal_or_ret_type_expr  */
#line 2999 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9319 "bison-chpl-lib.cpp"
    break;

  case 518: /* ret_type: reserved_type_ident_use  */
#line 3000 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9325 "bison-chpl-lib.cpp"
    break;

  case 519: /* ret_type: error  */
#line 3001 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9331 "bison-chpl-lib.cpp"
    break;

  case 520: /* colon_ret_type: TCOLON ret_type  */
#line 3005 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9337 "bison-chpl-lib.cpp"
    break;

  case 521: /* colon_ret_type: error  */
#line 3006 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9343 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_ret_type: %empty  */
#line 3010 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9349 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_type: %empty  */
#line 3015 "chpl.ypp"
                                 { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9355 "bison-chpl-lib.cpp"
    break;

  case 525: /* opt_type: TCOLON expr  */
#line 3016 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9361 "bison-chpl-lib.cpp"
    break;

  case 526: /* opt_type: TCOLON reserved_type_ident_use  */
#line 3017 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9367 "bison-chpl-lib.cpp"
    break;

  case 527: /* opt_type: error  */
#line 3018 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9373 "bison-chpl-lib.cpp"
    break;

  case 528: /* formal_type: formal_or_ret_type_expr  */
#line 3022 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9379 "bison-chpl-lib.cpp"
    break;

  case 529: /* formal_type: reserved_type_ident_use  */
#line 3023 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9385 "bison-chpl-lib.cpp"
    break;

  case 530: /* colon_formal_type: TCOLON formal_type  */
#line 3027 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9391 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_colon_formal_type: %empty  */
#line 3031 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9397 "bison-chpl-lib.cpp"
    break;

  case 532: /* opt_colon_formal_type: colon_formal_type  */
#line 3032 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9403 "bison-chpl-lib.cpp"
    break;

  case 533: /* expr_ls: expr  */
#line 3038 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9409 "bison-chpl-lib.cpp"
    break;

  case 534: /* expr_ls: expr_ls TCOMMA expr  */
#line 3039 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9415 "bison-chpl-lib.cpp"
    break;

  case 535: /* tuple_component: opt_try_expr  */
#line 3043 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9421 "bison-chpl-lib.cpp"
    break;

  case 536: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3048 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9429 "bison-chpl-lib.cpp"
    break;

  case 537: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3052 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9437 "bison-chpl-lib.cpp"
    break;

  case 538: /* opt_actual_ls: %empty  */
#line 3058 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9443 "bison-chpl-lib.cpp"
    break;

  case 539: /* opt_actual_ls: actual_ls  */
#line 3059 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9449 "bison-chpl-lib.cpp"
    break;

  case 540: /* actual_ls: actual_expr  */
#line 3064 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9458 "bison-chpl-lib.cpp"
    break;

  case 541: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3069 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9468 "bison-chpl-lib.cpp"
    break;

  case 542: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3077 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9474 "bison-chpl-lib.cpp"
    break;

  case 543: /* actual_expr: opt_try_expr  */
#line 3078 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9480 "bison-chpl-lib.cpp"
    break;

  case 544: /* ident_expr: ident_use  */
#line 3082 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9486 "bison-chpl-lib.cpp"
    break;

  case 545: /* ident_expr: scalar_type  */
#line 3083 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9492 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3096 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9498 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3098 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9504 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3100 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9510 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3102 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9520 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TATOMIC expr  */
#line 3108 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9526 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TSYNC expr  */
#line 3110 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9532 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TOWNED  */
#line 3113 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9538 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TOWNED expr  */
#line 3115 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9544 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TUNMANAGED  */
#line 3117 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9550 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3119 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9556 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TSHARED  */
#line 3121 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9562 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TSHARED expr  */
#line 3123 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9568 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TBORROWED  */
#line 3125 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9574 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TBORROWED expr  */
#line 3127 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9580 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TCLASS  */
#line 3129 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9586 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TRECORD  */
#line 3131 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9592 "bison-chpl-lib.cpp"
    break;

  case 567: /* for_expr: no_loop_attributes for_expr_base  */
#line 3135 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9598 "bison-chpl-lib.cpp"
    break;

  case 568: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3140 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9606 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3144 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9614 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3148 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9622 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr_base: for_loop_kw zippered_iterator TDO expr  */
#line 3152 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "expression-level loops with 'zip' must have an index.");
  }
#line 9630 "bison-chpl-lib.cpp"
    break;

  case 572: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3156 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9643 "bison-chpl-lib.cpp"
    break;

  case 573: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3165 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9656 "bison-chpl-lib.cpp"
    break;

  case 574: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3174 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9669 "bison-chpl-lib.cpp"
    break;

  case 575: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3185 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9675 "bison-chpl-lib.cpp"
    break;

  case 576: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3190 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9683 "bison-chpl-lib.cpp"
    break;

  case 577: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3194 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9691 "bison-chpl-lib.cpp"
    break;

  case 578: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3198 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9699 "bison-chpl-lib.cpp"
    break;

  case 579: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3202 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9707 "bison-chpl-lib.cpp"
    break;

  case 580: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3206 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9715 "bison-chpl-lib.cpp"
    break;

  case 581: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3210 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9723 "bison-chpl-lib.cpp"
    break;

  case 582: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3214 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9731 "bison-chpl-lib.cpp"
    break;

  case 583: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3221 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9745 "bison-chpl-lib.cpp"
    break;

  case 584: /* nil_expr: TNIL  */
#line 3238 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9751 "bison-chpl-lib.cpp"
    break;

  case 592: /* opt_task_intent_ls: %empty  */
#line 3256 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9757 "bison-chpl-lib.cpp"
    break;

  case 593: /* opt_task_intent_ls: task_intent_clause  */
#line 3257 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9763 "bison-chpl-lib.cpp"
    break;

  case 594: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3262 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9771 "bison-chpl-lib.cpp"
    break;

  case 595: /* task_intent_clause: TWITH TLP TRP  */
#line 3266 "chpl.ypp"
  {
    auto err = context->syntax((yyloc), "'with' clause cannot be empty.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-2]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9782 "bison-chpl-lib.cpp"
    break;

  case 596: /* task_intent_clause: TWITH TLP error TRP  */
#line 3273 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[-2]), "invalid intent expression in 'with' clause.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9793 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_intent_clause: TWITH error  */
#line 3280 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[0]), "missing parentheses around 'with' clause intents.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-1]), dummyLoc, dummyLoc, dummyLoc, exprs);
  }
#line 9804 "bison-chpl-lib.cpp"
    break;

  case 598: /* task_intent_ls: intent_expr  */
#line 3289 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9810 "bison-chpl-lib.cpp"
    break;

  case 599: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3290 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9816 "bison-chpl-lib.cpp"
    break;

  case 600: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3295 "chpl.ypp"
  {
    (yyval.expr) = context->buildTaskIntent((yyloc), (yylsp[-2]), nullptr, (yyvsp[-2].expr), (yyvsp[-3].taskIntent), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9824 "bison-chpl-lib.cpp"
    break;

  case 601: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3299 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9832 "bison-chpl-lib.cpp"
    break;

  case 602: /* intent_expr: expr TREDUCE ident_expr  */
#line 3303 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9840 "bison-chpl-lib.cpp"
    break;

  case 603: /* task_var_prefix: TCONST  */
#line 3309 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST);          }
#line 9846 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_var_prefix: TIN  */
#line 3310 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::IN);             }
#line 9852 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_var_prefix: TCONST TIN  */
#line 3311 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_IN);       }
#line 9858 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_var_prefix: TREF  */
#line 3312 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::REF);            }
#line 9864 "bison-chpl-lib.cpp"
    break;

  case 607: /* task_var_prefix: TCONST TREF  */
#line 3313 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_REF);      }
#line 9870 "bison-chpl-lib.cpp"
    break;

  case 608: /* task_var_prefix: TVAR  */
#line 3314 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::VAR);            }
#line 9876 "bison-chpl-lib.cpp"
    break;

  case 609: /* task_var_prefix: TOUT  */
#line 3315 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::OUT);   }
#line 9882 "bison-chpl-lib.cpp"
    break;

  case 610: /* task_var_prefix: TINOUT  */
#line 3316 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::INOUT); }
#line 9888 "bison-chpl-lib.cpp"
    break;

  case 611: /* task_var_prefix: TPARAM  */
#line 3317 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::PARAM); }
#line 9894 "bison-chpl-lib.cpp"
    break;

  case 612: /* task_var_prefix: TTYPE  */
#line 3318 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::TYPE);  }
#line 9900 "bison-chpl-lib.cpp"
    break;

  case 613: /* new_maybe_decorated: TNEW  */
#line 3323 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9906 "bison-chpl-lib.cpp"
    break;

  case 614: /* new_maybe_decorated: TNEW TOWNED  */
#line 3325 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9912 "bison-chpl-lib.cpp"
    break;

  case 615: /* new_maybe_decorated: TNEW TSHARED  */
#line 3327 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9918 "bison-chpl-lib.cpp"
    break;

  case 616: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3329 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9924 "bison-chpl-lib.cpp"
    break;

  case 617: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3331 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9930 "bison-chpl-lib.cpp"
    break;

  case 618: /* new_expr: new_maybe_decorated expr  */
#line 3337 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 9938 "bison-chpl-lib.cpp"
    break;

  case 619: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3344 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenthLocation((yyvsp[-4].expr), LOC2((yylsp[-5]), (yylsp[-3])));
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
#line 9955 "bison-chpl-lib.cpp"
    break;

  case 620: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3357 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenthLocation((yyvsp[-4].expr), LOC2((yylsp[-5]), (yylsp[-3])));
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
#line 9972 "bison-chpl-lib.cpp"
    break;

  case 621: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3370 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenthLocation((yyvsp[-5].expr), LOC2((yylsp[-6]), (yylsp[-4])));
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
#line 9990 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3384 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenthLocation((yyvsp[-5].expr), LOC2((yylsp[-6]), (yylsp[-4])));
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
#line 10009 "bison-chpl-lib.cpp"
    break;

  case 623: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3402 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10017 "bison-chpl-lib.cpp"
    break;

  case 624: /* range_literal_expr: expr TDOTDOT expr  */
#line 3409 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10026 "bison-chpl-lib.cpp"
    break;

  case 625: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3414 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10035 "bison-chpl-lib.cpp"
    break;

  case 626: /* range_literal_expr: expr TDOTDOT  */
#line 3419 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10044 "bison-chpl-lib.cpp"
    break;

  case 627: /* range_literal_expr: TDOTDOT expr  */
#line 3424 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10054 "bison-chpl-lib.cpp"
    break;

  case 628: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3430 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10064 "bison-chpl-lib.cpp"
    break;

  case 629: /* range_literal_expr: TDOTDOT  */
#line 3436 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10074 "bison-chpl-lib.cpp"
    break;

  case 630: /* cast_expr: expr TCOLON expr  */
#line 3466 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10082 "bison-chpl-lib.cpp"
    break;

  case 631: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3473 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 632: /* super_expr: fn_expr  */
#line 3479 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10096 "bison-chpl-lib.cpp"
    break;

  case 636: /* expr: sub_type_level_expr TQUESTION  */
#line 3488 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10102 "bison-chpl-lib.cpp"
    break;

  case 637: /* expr: TQUESTION  */
#line 3490 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10108 "bison-chpl-lib.cpp"
    break;

  case 641: /* expr: fn_type  */
#line 3495 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10114 "bison-chpl-lib.cpp"
    break;

  case 651: /* opt_expr: %empty  */
#line 3509 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10120 "bison-chpl-lib.cpp"
    break;

  case 652: /* opt_expr: expr  */
#line 3510 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10126 "bison-chpl-lib.cpp"
    break;

  case 653: /* opt_try_expr: TTRY expr  */
#line 3514 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10132 "bison-chpl-lib.cpp"
    break;

  case 654: /* opt_try_expr: TTRYBANG expr  */
#line 3515 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10138 "bison-chpl-lib.cpp"
    break;

  case 655: /* opt_try_expr: super_expr  */
#line 3516 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10144 "bison-chpl-lib.cpp"
    break;

  case 661: /* call_base_expr: expr TBANG  */
#line 3533 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10152 "bison-chpl-lib.cpp"
    break;

  case 662: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3536 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10158 "bison-chpl-lib.cpp"
    break;

  case 665: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3543 "chpl.ypp"
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
#line 10174 "bison-chpl-lib.cpp"
    break;

  case 666: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3555 "chpl.ypp"
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
#line 10190 "bison-chpl-lib.cpp"
    break;

  case 667: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3567 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10198 "bison-chpl-lib.cpp"
    break;

  case 668: /* dot_expr: expr TDOT ident_use  */
#line 3574 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10204 "bison-chpl-lib.cpp"
    break;

  case 669: /* dot_expr: expr TDOT TTYPE  */
#line 3576 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10210 "bison-chpl-lib.cpp"
    break;

  case 670: /* dot_expr: expr TDOT TDOMAIN  */
#line 3578 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10216 "bison-chpl-lib.cpp"
    break;

  case 671: /* dot_expr: expr TDOT TLOCALE  */
#line 3580 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10222 "bison-chpl-lib.cpp"
    break;

  case 672: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3582 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10228 "bison-chpl-lib.cpp"
    break;

  case 673: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3584 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10234 "bison-chpl-lib.cpp"
    break;

  case 674: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3593 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenthLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10244 "bison-chpl-lib.cpp"
    break;

  case 675: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3599 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10252 "bison-chpl-lib.cpp"
    break;

  case 676: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3603 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10260 "bison-chpl-lib.cpp"
    break;

  case 677: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3607 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10268 "bison-chpl-lib.cpp"
    break;

  case 678: /* bool_literal: TFALSE  */
#line 3613 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10274 "bison-chpl-lib.cpp"
    break;

  case 679: /* bool_literal: TTRUE  */
#line 3614 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10280 "bison-chpl-lib.cpp"
    break;

  case 680: /* str_bytes_literal: STRINGLITERAL  */
#line 3618 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10286 "bison-chpl-lib.cpp"
    break;

  case 681: /* str_bytes_literal: BYTESLITERAL  */
#line 3619 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10292 "bison-chpl-lib.cpp"
    break;

  case 684: /* literal_expr: INTLITERAL  */
#line 3625 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10298 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: REALLITERAL  */
#line 3626 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10304 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: IMAGLITERAL  */
#line 3627 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10310 "bison-chpl-lib.cpp"
    break;

  case 687: /* literal_expr: CSTRINGLITERAL  */
#line 3628 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10316 "bison-chpl-lib.cpp"
    break;

  case 688: /* literal_expr: TNONE  */
#line 3629 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10322 "bison-chpl-lib.cpp"
    break;

  case 689: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3631 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10331 "bison-chpl-lib.cpp"
    break;

  case 690: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3636 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10340 "bison-chpl-lib.cpp"
    break;

  case 691: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3641 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10349 "bison-chpl-lib.cpp"
    break;

  case 692: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3646 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10358 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3651 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10367 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3656 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10376 "bison-chpl-lib.cpp"
    break;

  case 695: /* assoc_expr_ls: expr TALIAS expr  */
#line 3665 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10385 "bison-chpl-lib.cpp"
    break;

  case 696: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3670 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10395 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TPLUS expr  */
#line 3678 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10401 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TMINUS expr  */
#line 3679 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10407 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TSTAR expr  */
#line 3680 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10413 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TDIVIDE expr  */
#line 3681 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10419 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3682 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10425 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3683 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10431 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TMOD expr  */
#line 3684 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10437 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TEQUAL expr  */
#line 3685 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10443 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3686 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10449 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3687 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10455 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3688 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10461 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TLESS expr  */
#line 3689 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10467 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TGREATER expr  */
#line 3690 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10473 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TBAND expr  */
#line 3691 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10479 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TBOR expr  */
#line 3692 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10485 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TBXOR expr  */
#line 3693 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10491 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TAND expr  */
#line 3694 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10497 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TOR expr  */
#line 3695 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10503 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TEXP expr  */
#line 3696 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10509 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TBY expr  */
#line 3697 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10515 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TALIGN expr  */
#line 3698 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10521 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr THASH expr  */
#line 3699 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10527 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TDMAPPED expr  */
#line 3700 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10533 "bison-chpl-lib.cpp"
    break;

  case 720: /* unary_op_expr: TPLUS expr  */
#line 3704 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10539 "bison-chpl-lib.cpp"
    break;

  case 721: /* unary_op_expr: TMINUS expr  */
#line 3705 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10545 "bison-chpl-lib.cpp"
    break;

  case 722: /* unary_op_expr: TMINUSMINUS expr  */
#line 3706 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10551 "bison-chpl-lib.cpp"
    break;

  case 723: /* unary_op_expr: TPLUSPLUS expr  */
#line 3707 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10557 "bison-chpl-lib.cpp"
    break;

  case 724: /* unary_op_expr: TBANG expr  */
#line 3708 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10563 "bison-chpl-lib.cpp"
    break;

  case 725: /* unary_op_expr: expr TBANG  */
#line 3709 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10571 "bison-chpl-lib.cpp"
    break;

  case 726: /* unary_op_expr: TBNOT expr  */
#line 3712 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10577 "bison-chpl-lib.cpp"
    break;

  case 727: /* reduce_expr: expr TREDUCE expr  */
#line 3717 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10585 "bison-chpl-lib.cpp"
    break;

  case 728: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3721 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10593 "bison-chpl-lib.cpp"
    break;

  case 729: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3725 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10601 "bison-chpl-lib.cpp"
    break;

  case 730: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3729 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10609 "bison-chpl-lib.cpp"
    break;

  case 731: /* scan_expr: expr TSCAN expr  */
#line 3736 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10617 "bison-chpl-lib.cpp"
    break;

  case 732: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3740 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10625 "bison-chpl-lib.cpp"
    break;

  case 733: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3744 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10633 "bison-chpl-lib.cpp"
    break;

  case 734: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3748 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10641 "bison-chpl-lib.cpp"
    break;


#line 10645 "bison-chpl-lib.cpp"

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
