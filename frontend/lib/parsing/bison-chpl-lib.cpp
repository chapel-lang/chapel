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
  YYSYMBOL_TSPARSE = 97,                   /* TSPARSE  */
  YYSYMBOL_TSTRING = 98,                   /* TSTRING  */
  YYSYMBOL_TSUBDOMAIN = 99,                /* TSUBDOMAIN  */
  YYSYMBOL_TSYNC = 100,                    /* TSYNC  */
  YYSYMBOL_TTHEN = 101,                    /* TTHEN  */
  YYSYMBOL_TTHIS = 102,                    /* TTHIS  */
  YYSYMBOL_TTHROW = 103,                   /* TTHROW  */
  YYSYMBOL_TTHROWS = 104,                  /* TTHROWS  */
  YYSYMBOL_TTRUE = 105,                    /* TTRUE  */
  YYSYMBOL_TTRY = 106,                     /* TTRY  */
  YYSYMBOL_TTRYBANG = 107,                 /* TTRYBANG  */
  YYSYMBOL_TTYPE = 108,                    /* TTYPE  */
  YYSYMBOL_TUINT = 109,                    /* TUINT  */
  YYSYMBOL_TUNION = 110,                   /* TUNION  */
  YYSYMBOL_TUNMANAGED = 111,               /* TUNMANAGED  */
  YYSYMBOL_TUSE = 112,                     /* TUSE  */
  YYSYMBOL_TVAR = 113,                     /* TVAR  */
  YYSYMBOL_TVOID = 114,                    /* TVOID  */
  YYSYMBOL_TWHEN = 115,                    /* TWHEN  */
  YYSYMBOL_TWHERE = 116,                   /* TWHERE  */
  YYSYMBOL_TWHILE = 117,                   /* TWHILE  */
  YYSYMBOL_TWITH = 118,                    /* TWITH  */
  YYSYMBOL_TYIELD = 119,                   /* TYIELD  */
  YYSYMBOL_TZIP = 120,                     /* TZIP  */
  YYSYMBOL_TALIAS = 121,                   /* TALIAS  */
  YYSYMBOL_TAND = 122,                     /* TAND  */
  YYSYMBOL_TASSIGN = 123,                  /* TASSIGN  */
  YYSYMBOL_TASSIGNBAND = 124,              /* TASSIGNBAND  */
  YYSYMBOL_TASSIGNBOR = 125,               /* TASSIGNBOR  */
  YYSYMBOL_TASSIGNBXOR = 126,              /* TASSIGNBXOR  */
  YYSYMBOL_TASSIGNDIVIDE = 127,            /* TASSIGNDIVIDE  */
  YYSYMBOL_TASSIGNEXP = 128,               /* TASSIGNEXP  */
  YYSYMBOL_TASSIGNLAND = 129,              /* TASSIGNLAND  */
  YYSYMBOL_TASSIGNLOR = 130,               /* TASSIGNLOR  */
  YYSYMBOL_TASSIGNMINUS = 131,             /* TASSIGNMINUS  */
  YYSYMBOL_TASSIGNMOD = 132,               /* TASSIGNMOD  */
  YYSYMBOL_TASSIGNMULTIPLY = 133,          /* TASSIGNMULTIPLY  */
  YYSYMBOL_TASSIGNPLUS = 134,              /* TASSIGNPLUS  */
  YYSYMBOL_TASSIGNREDUCE = 135,            /* TASSIGNREDUCE  */
  YYSYMBOL_TASSIGNSL = 136,                /* TASSIGNSL  */
  YYSYMBOL_TASSIGNSR = 137,                /* TASSIGNSR  */
  YYSYMBOL_TATMARK = 138,                  /* TATMARK  */
  YYSYMBOL_TBANG = 139,                    /* TBANG  */
  YYSYMBOL_TBAND = 140,                    /* TBAND  */
  YYSYMBOL_TBNOT = 141,                    /* TBNOT  */
  YYSYMBOL_TBOR = 142,                     /* TBOR  */
  YYSYMBOL_TBXOR = 143,                    /* TBXOR  */
  YYSYMBOL_TCOLON = 144,                   /* TCOLON  */
  YYSYMBOL_TCOMMA = 145,                   /* TCOMMA  */
  YYSYMBOL_TDIVIDE = 146,                  /* TDIVIDE  */
  YYSYMBOL_TDOT = 147,                     /* TDOT  */
  YYSYMBOL_TDOTDOT = 148,                  /* TDOTDOT  */
  YYSYMBOL_TDOTDOTDOT = 149,               /* TDOTDOTDOT  */
  YYSYMBOL_TEQUAL = 150,                   /* TEQUAL  */
  YYSYMBOL_TEXP = 151,                     /* TEXP  */
  YYSYMBOL_TGREATER = 152,                 /* TGREATER  */
  YYSYMBOL_TGREATEREQUAL = 153,            /* TGREATEREQUAL  */
  YYSYMBOL_THASH = 154,                    /* THASH  */
  YYSYMBOL_TLESS = 155,                    /* TLESS  */
  YYSYMBOL_TLESSEQUAL = 156,               /* TLESSEQUAL  */
  YYSYMBOL_TMINUS = 157,                   /* TMINUS  */
  YYSYMBOL_TMOD = 158,                     /* TMOD  */
  YYSYMBOL_TNOTEQUAL = 159,                /* TNOTEQUAL  */
  YYSYMBOL_TOR = 160,                      /* TOR  */
  YYSYMBOL_TPLUS = 161,                    /* TPLUS  */
  YYSYMBOL_TQUESTION = 162,                /* TQUESTION  */
  YYSYMBOL_TSEMI = 163,                    /* TSEMI  */
  YYSYMBOL_TSHIFTLEFT = 164,               /* TSHIFTLEFT  */
  YYSYMBOL_TSHIFTRIGHT = 165,              /* TSHIFTRIGHT  */
  YYSYMBOL_TSTAR = 166,                    /* TSTAR  */
  YYSYMBOL_TSWAP = 167,                    /* TSWAP  */
  YYSYMBOL_TLCBR = 168,                    /* TLCBR  */
  YYSYMBOL_TRCBR = 169,                    /* TRCBR  */
  YYSYMBOL_TLP = 170,                      /* TLP  */
  YYSYMBOL_TRP = 171,                      /* TRP  */
  YYSYMBOL_TLSBR = 172,                    /* TLSBR  */
  YYSYMBOL_TRSBR = 173,                    /* TRSBR  */
  YYSYMBOL_TNOELSE = 174,                  /* TNOELSE  */
  YYSYMBOL_TDOTDOTOPENHIGH = 175,          /* TDOTDOTOPENHIGH  */
  YYSYMBOL_TUPLUS = 176,                   /* TUPLUS  */
  YYSYMBOL_TUMINUS = 177,                  /* TUMINUS  */
  YYSYMBOL_TLNOT = 178,                    /* TLNOT  */
  YYSYMBOL_YYACCEPT = 179,                 /* $accept  */
  YYSYMBOL_program = 180,                  /* program  */
  YYSYMBOL_toplevel_stmt_ls = 181,         /* toplevel_stmt_ls  */
  YYSYMBOL_toplevel_stmt = 182,            /* toplevel_stmt  */
  YYSYMBOL_pragma_ls = 183,                /* pragma_ls  */
  YYSYMBOL_stmt = 184,                     /* stmt  */
  YYSYMBOL_stmt_base = 185,                /* stmt_base  */
  YYSYMBOL_tryable_stmt = 186,             /* tryable_stmt  */
  YYSYMBOL_decl_base = 187,                /* decl_base  */
  YYSYMBOL_collect_attributes = 188,       /* collect_attributes  */
  YYSYMBOL_attribute_receiver = 189,       /* attribute_receiver  */
  YYSYMBOL_attribute_group = 190,          /* attribute_group  */
  YYSYMBOL_attribute_decl_stmt_ls = 191,   /* attribute_decl_stmt_ls  */
  YYSYMBOL_attribute_decl_stmt = 192,      /* attribute_decl_stmt  */
  YYSYMBOL_attribute_decl_begin = 193,     /* attribute_decl_begin  */
  YYSYMBOL_opt_attribute_decl_toolspace = 194, /* opt_attribute_decl_toolspace  */
  YYSYMBOL_opt_attribute_actuals = 195,    /* opt_attribute_actuals  */
  YYSYMBOL_attribute_actuals_ls = 196,     /* attribute_actuals_ls  */
  YYSYMBOL_attribute_actual = 197,         /* attribute_actual  */
  YYSYMBOL_module_decl_start = 198,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 199,         /* module_decl_stmt  */
  YYSYMBOL_opt_access_control = 200,       /* opt_access_control  */
  YYSYMBOL_opt_prototype = 201,            /* opt_prototype  */
  YYSYMBOL_include_module_stmt = 202,      /* include_module_stmt  */
  YYSYMBOL_block_stmt_body = 203,          /* block_stmt_body  */
  YYSYMBOL_block_stmt = 204,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 205,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 206,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 207,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 208,              /* opt_only_ls  */
  YYSYMBOL_use_stmt = 209,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 210,              /* import_stmt  */
  YYSYMBOL_import_internal_type_ident = 211, /* import_internal_type_ident  */
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
  YYSYMBOL_extern_export_kind_inner = 233, /* extern_export_kind_inner  */
  YYSYMBOL_extern_export_decl_stmt = 234,  /* extern_export_decl_stmt  */
  YYSYMBOL_235_3 = 235,                    /* $@3  */
  YYSYMBOL_236_4 = 236,                    /* $@4  */
  YYSYMBOL_extern_block_stmt = 237,        /* extern_block_stmt  */
  YYSYMBOL_no_loop_attributes = 238,       /* no_loop_attributes  */
  YYSYMBOL_loop_stmt = 239,                /* loop_stmt  */
  YYSYMBOL_for_loop_kw = 240,              /* for_loop_kw  */
  YYSYMBOL_loop_stmt_base = 241,           /* loop_stmt_base  */
  YYSYMBOL_zippered_iterator = 242,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 243,                  /* if_stmt  */
  YYSYMBOL_ifvar = 244,                    /* ifvar  */
  YYSYMBOL_interface_start = 245,          /* interface_start  */
  YYSYMBOL_interface_stmt = 246,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 247,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 248,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 249,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 250, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 251,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 252,           /* ifc_constraint  */
  YYSYMBOL_try_stmt = 253,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 254,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 255,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 256,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 257,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 258,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 259,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 260,                /* when_stmt  */
  YYSYMBOL_match_select_stmt = 261,        /* match_select_stmt  */
  YYSYMBOL_match_case_stmt_ls = 262,       /* match_case_stmt_ls  */
  YYSYMBOL_match_case_stmt = 263,          /* match_case_stmt  */
  YYSYMBOL_match_otherwise_stmt = 264,     /* match_otherwise_stmt  */
  YYSYMBOL_manager_expr = 265,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 266,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 267,              /* manage_stmt  */
  YYSYMBOL_class_decl_stmt = 268,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 269,              /* class_start  */
  YYSYMBOL_class_tag = 270,                /* class_tag  */
  YYSYMBOL_opt_inherit = 271,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 272,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 273,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header = 274,              /* enum_header  */
  YYSYMBOL_enum_ls = 275,                  /* enum_ls  */
  YYSYMBOL_276_5 = 276,                    /* $@5  */
  YYSYMBOL_277_6 = 277,                    /* $@6  */
  YYSYMBOL_enum_item = 278,                /* enum_item  */
  YYSYMBOL_linkage_spec_empty = 279,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 280,             /* linkage_spec  */
  YYSYMBOL_opt_fn_type_formal_ls = 281,    /* opt_fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal_ls = 282,        /* fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal = 283,           /* fn_type_formal  */
  YYSYMBOL_opt_fn_type_ret_type = 284,     /* opt_fn_type_ret_type  */
  YYSYMBOL_fn_type = 285,                  /* fn_type  */
  YYSYMBOL_fn_expr = 286,                  /* fn_expr  */
  YYSYMBOL_287_7 = 287,                    /* $@7  */
  YYSYMBOL_288_8 = 288,                    /* $@8  */
  YYSYMBOL_fn_decl_stmt_complete = 289,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 290,             /* fn_decl_stmt  */
  YYSYMBOL_291_9 = 291,                    /* $@9  */
  YYSYMBOL_fn_decl_stmt_inner = 292,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 293,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 294,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 295,                 /* fn_ident  */
  YYSYMBOL_op_ident = 296,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 297,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 298,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 299,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 300,            /* opt_formal_ls  */
  YYSYMBOL_formal_ls_inner = 301,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 302,                /* formal_ls  */
  YYSYMBOL_formal = 303,                   /* formal  */
  YYSYMBOL_named_formal = 304,             /* named_formal  */
  YYSYMBOL_opt_formal_intent_tag = 305,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 306,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 307,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 308,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 309,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 310,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 311,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 312,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 313,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 314,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 315, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 316,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 317,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 318,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 319, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 320, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 321, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 322,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 323,            /* var_decl_type  */
  YYSYMBOL_var_decl_start = 324,           /* var_decl_start  */
  YYSYMBOL_var_decl_stmt = 325,            /* var_decl_stmt  */
  YYSYMBOL_326_10 = 326,                   /* $@10  */
  YYSYMBOL_327_11 = 327,                   /* $@11  */
  YYSYMBOL_var_decl_stmt_inner_ls = 328,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 329,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 330, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 331, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 332,            /* opt_init_expr  */
  YYSYMBOL_formal_or_ret_type_expr = 333,  /* formal_or_ret_type_expr  */
  YYSYMBOL_ret_type = 334,                 /* ret_type  */
  YYSYMBOL_colon_ret_type = 335,           /* colon_ret_type  */
  YYSYMBOL_opt_ret_type = 336,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 337,                 /* opt_type  */
  YYSYMBOL_formal_type = 338,              /* formal_type  */
  YYSYMBOL_colon_formal_type = 339,        /* colon_formal_type  */
  YYSYMBOL_opt_colon_formal_type = 340,    /* opt_colon_formal_type  */
  YYSYMBOL_expr_ls = 341,                  /* expr_ls  */
  YYSYMBOL_expr_ls_end_semi = 342,         /* expr_ls_end_semi  */
  YYSYMBOL_expr_ls_semi = 343,             /* expr_ls_semi  */
  YYSYMBOL_semicolon_list = 344,           /* semicolon_list  */
  YYSYMBOL_tuple_component = 345,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 346,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 347,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 348,                /* actual_ls  */
  YYSYMBOL_actual_expr = 349,              /* actual_expr  */
  YYSYMBOL_ident_expr = 350,               /* ident_expr  */
  YYSYMBOL_sub_type_level_expr = 351,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 352,                 /* for_expr  */
  YYSYMBOL_for_expr_base = 353,            /* for_expr_base  */
  YYSYMBOL_bracket_loop_expr = 354,        /* bracket_loop_expr  */
  YYSYMBOL_bracket_loop_expr_base = 355,   /* bracket_loop_expr_base  */
  YYSYMBOL_cond_expr = 356,                /* cond_expr  */
  YYSYMBOL_nil_expr = 357,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 358,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 359,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 360,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 361,           /* task_intent_ls  */
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
#line 383 "chpl.ypp"

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
#define YYLAST   18020

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  179
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  209
/* YYNRULES -- Number of rules.  */
#define YYNRULES  754
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1281

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   433


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
     175,   176,   177,   178
};

#if YYCHPL_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   692,   692,   696,   697,   702,   703,   711,   715,   725,
     730,   737,   743,   745,   746,   747,   748,   749,   750,   751,
     752,   769,   770,   771,   779,   788,   797,   801,   808,   816,
     817,   818,   819,   820,   821,   822,   823,   824,   826,   845,
     853,   869,   884,   900,   916,   931,   951,   952,   953,   957,
     963,   964,   968,   972,   973,   977,   984,   992,   995,  1005,
    1007,  1012,  1018,  1023,  1032,  1036,  1057,  1075,  1093,  1112,
    1113,  1115,  1120,  1121,  1126,  1144,  1149,  1154,  1166,  1189,
    1190,  1194,  1198,  1202,  1207,  1211,  1215,  1223,  1228,  1234,
    1239,  1248,  1249,  1253,  1258,  1266,  1275,  1283,  1294,  1302,
    1303,  1304,  1305,  1306,  1307,  1308,  1309,  1310,  1311,  1312,
    1313,  1314,  1315,  1316,  1317,  1329,  1333,  1338,  1343,  1349,
    1358,  1359,  1363,  1372,  1376,  1380,  1384,  1393,  1394,  1395,
    1399,  1400,  1401,  1402,  1403,  1407,  1408,  1409,  1421,  1422,
    1423,  1428,  1429,  1430,  1431,  1432,  1433,  1434,  1435,  1436,
    1437,  1438,  1439,  1440,  1441,  1442,  1443,  1444,  1445,  1449,
    1450,  1451,  1452,  1453,  1454,  1455,  1456,  1457,  1458,  1459,
    1460,  1467,  1468,  1469,  1473,  1474,  1478,  1485,  1495,  1499,
    1505,  1505,  1512,  1512,  1522,  1523,  1524,  1525,  1526,  1527,
    1528,  1532,  1537,  1542,  1547,  1554,  1562,  1563,  1567,  1580,
    1588,  1596,  1606,  1625,  1624,  1634,  1633,  1646,  1653,  1659,
    1660,  1669,  1670,  1671,  1672,  1676,  1698,  1716,  1734,  1758,
    1763,  1768,  1773,  1778,  1783,  1788,  1793,  1798,  1803,  1808,
    1813,  1818,  1823,  1828,  1834,  1843,  1852,  1856,  1860,  1864,
    1868,  1872,  1876,  1880,  1884,  1889,  1894,  1899,  1907,  1922,
    1940,  1947,  1954,  1964,  1965,  1970,  1975,  1976,  1977,  1978,
    1979,  1980,  1981,  1982,  1983,  1984,  1985,  1986,  1991,  1996,
    1997,  1998,  1999,  2007,  2008,  2012,  2016,  2020,  2027,  2031,
    2035,  2042,  2046,  2050,  2054,  2061,  2062,  2066,  2070,  2074,
    2081,  2096,  2114,  2122,  2127,  2137,  2138,  2142,  2146,  2151,
    2160,  2165,  2170,  2179,  2180,  2184,  2191,  2196,  2205,  2210,
    2214,  2221,  2222,  2226,  2236,  2243,  2256,  2263,  2264,  2265,
    2269,  2270,  2274,  2278,  2282,  2286,  2290,  2298,  2318,  2328,
    2342,  2349,  2354,  2361,  2360,  2370,  2376,  2375,  2390,  2398,
    2410,  2414,  2415,  2417,  2422,  2423,  2427,  2428,  2432,  2435,
    2437,  2442,  2443,  2454,  2475,  2474,  2494,  2493,  2511,  2521,
    2518,  2553,  2566,  2579,  2593,  2607,  2620,  2635,  2636,  2644,
    2645,  2646,  2655,  2656,  2657,  2658,  2659,  2660,  2661,  2662,
    2663,  2664,  2665,  2666,  2667,  2668,  2669,  2670,  2671,  2672,
    2673,  2674,  2675,  2676,  2677,  2678,  2679,  2683,  2684,  2685,
    2686,  2687,  2688,  2689,  2690,  2691,  2692,  2693,  2694,  2695,
    2696,  2701,  2702,  2706,  2707,  2711,  2712,  2716,  2717,  2721,
    2722,  2726,  2730,  2734,  2739,  2744,  2749,  2754,  2762,  2766,
    2774,  2775,  2776,  2777,  2778,  2779,  2780,  2781,  2782,  2783,
    2787,  2788,  2789,  2790,  2791,  2792,  2793,  2794,  2795,  2796,
    2797,  2801,  2802,  2803,  2807,  2808,  2809,  2810,  2811,  2812,
    2813,  2814,  2815,  2816,  2817,  2821,  2822,  2825,  2826,  2830,
    2831,  2835,  2840,  2841,  2843,  2845,  2847,  2852,  2854,  2859,
    2861,  2863,  2865,  2867,  2869,  2871,  2876,  2877,  2878,  2882,
    2891,  2897,  2907,  2911,  2918,  2939,  2940,  2945,  2946,  2947,
    2948,  2949,  2953,  2962,  2961,  2972,  2971,  2981,  2986,  2995,
    2999,  3006,  3024,  3043,  3047,  3054,  3056,  3058,  3065,  3066,
    3067,  3071,  3075,  3076,  3077,  3081,  3082,  3086,  3087,  3091,
    3092,  3093,  3094,  3098,  3099,  3103,  3107,  3108,  3114,  3115,
    3119,  3124,  3129,  3137,  3140,  3146,  3147,  3153,  3157,  3161,
    3168,  3169,  3173,  3178,  3187,  3188,  3192,  3193,  3200,  3201,
    3202,  3203,  3204,  3205,  3207,  3209,  3211,  3217,  3219,  3221,
    3223,  3225,  3227,  3229,  3231,  3233,  3235,  3237,  3239,  3241,
    3246,  3250,  3254,  3258,  3262,  3266,  3275,  3284,  3296,  3300,
    3304,  3308,  3312,  3316,  3320,  3324,  3331,  3349,  3357,  3358,
    3359,  3360,  3361,  3362,  3366,  3367,  3371,  3375,  3382,  3389,
    3399,  3400,  3404,  3408,  3412,  3419,  3420,  3421,  3422,  3423,
    3424,  3425,  3426,  3427,  3428,  3432,  3434,  3436,  3438,  3440,
    3446,  3453,  3466,  3479,  3493,  3511,  3518,  3523,  3528,  3533,
    3539,  3545,  3575,  3582,  3589,  3590,  3594,  3596,  3597,  3599,
    3601,  3602,  3603,  3604,  3607,  3608,  3609,  3610,  3611,  3612,
    3613,  3614,  3618,  3619,  3623,  3624,  3625,  3629,  3630,  3631,
    3632,  3641,  3642,  3645,  3646,  3650,  3662,  3674,  3681,  3683,
    3685,  3687,  3689,  3691,  3700,  3706,  3710,  3714,  3721,  3722,
    3726,  3727,  3731,  3732,  3733,  3734,  3735,  3736,  3737,  3742,
    3747,  3752,  3757,  3762,  3770,  3780,  3790,  3795,  3804,  3809,
    3818,  3819,  3820,  3821,  3822,  3823,  3824,  3825,  3826,  3827,
    3828,  3829,  3830,  3831,  3832,  3833,  3834,  3835,  3836,  3837,
    3838,  3839,  3840,  3844,  3845,  3846,  3847,  3848,  3849,  3852,
    3856,  3860,  3864,  3868,  3875,  3879,  3883,  3887,  3895,  3896,
    3897,  3898,  3899,  3900,  3901
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
  "TINIT", "TINITEQUALS", "TIMPLEMENTS", "TINTERFACE", "TLABEL", "TLET",
  "TLIFETIME", "TLOCAL", "TLOCALE", "TMANAGE", "TMINUSMINUS", "TMODULE",
  "TNEW", "TNIL", "TNOINIT", "TNONE", "TNOTHING", "TON", "TONLY",
  "TOPERATOR", "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED", "TPARAM",
  "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE",
  "TPUBLIC", "TPROCLP", "TREAL", "TRECORD", "TREDUCE", "TREF", "TREQUIRE",
  "TRETURN", "TSCAN", "TSELECT", "TSERIAL", "TSHARED", "TSPARSE",
  "TSTRING", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS", "TTHROW", "TTHROWS",
  "TTRUE", "TTRY", "TTRYBANG", "TTYPE", "TUINT", "TUNION", "TUNMANAGED",
  "TUSE", "TVAR", "TVOID", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD",
  "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR",
  "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND",
  "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY",
  "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL", "TASSIGNSR", "TATMARK",
  "TBANG", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA",
  "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER",
  "TGREATEREQUAL", "THASH", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD",
  "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT",
  "TSHIFTRIGHT", "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR",
  "TRSBR", "TNOELSE", "TDOTDOTOPENHIGH", "TUPLUS", "TUMINUS", "TLNOT",
  "$accept", "program", "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls",
  "stmt", "stmt_base", "tryable_stmt", "decl_base", "collect_attributes",
  "attribute_receiver", "attribute_group", "attribute_decl_stmt_ls",
  "attribute_decl_stmt", "attribute_decl_begin",
  "opt_attribute_decl_toolspace", "opt_attribute_actuals",
  "attribute_actuals_ls", "attribute_actual", "module_decl_start",
  "module_decl_stmt", "opt_access_control", "opt_prototype",
  "include_module_stmt", "block_stmt_body", "block_stmt", "stmt_ls",
  "renames_ls", "use_renames_ls", "opt_only_ls", "use_stmt", "import_stmt",
  "import_internal_type_ident", "import_expr", "import_ls", "require_stmt",
  "assignment_stmt", "opt_label_ident", "reserved_word_ident", "ident_def",
  "ident_use", "internal_type_ident_def", "scalar_type",
  "reserved_type_ident_use", "do_stmt", "return_stmt", "class_level_stmt",
  "$@1", "$@2", "inner_class_level_stmt", "forwarding_decl_stmt",
  "forwarding_decl_start", "extern_or_export",
  "extern_export_decl_stmt_start", "extern_export_kind_inner",
  "extern_export_decl_stmt", "$@3", "$@4", "extern_block_stmt",
  "no_loop_attributes", "loop_stmt", "for_loop_kw", "loop_stmt_base",
  "zippered_iterator", "if_stmt", "ifvar", "interface_start",
  "interface_stmt", "ifc_formal_ls", "ifc_formal", "implements_type_ident",
  "implements_type_error_ident", "implements_stmt", "ifc_constraint",
  "try_stmt", "catch_expr_ls", "catch_expr", "catch_expr_inner",
  "throw_stmt", "select_stmt", "when_stmt_ls", "when_stmt",
  "match_select_stmt", "match_case_stmt_ls", "match_case_stmt",
  "match_otherwise_stmt", "manager_expr", "manager_expr_ls", "manage_stmt",
  "class_decl_stmt", "class_start", "class_tag", "opt_inherit",
  "class_level_stmt_ls", "enum_decl_stmt", "enum_header", "enum_ls", "$@5",
  "$@6", "enum_item", "linkage_spec_empty", "linkage_spec",
  "opt_fn_type_formal_ls", "fn_type_formal_ls", "fn_type_formal",
  "opt_fn_type_ret_type", "fn_type", "fn_expr", "$@7", "$@8",
  "fn_decl_stmt_complete", "fn_decl_stmt", "$@9", "fn_decl_stmt_inner",
  "fn_decl_stmt_start", "fn_decl_receiver_expr", "fn_ident", "op_ident",
  "assignop_ident", "all_op_name", "formal_var_arg_expr", "opt_formal_ls",
  "formal_ls_inner", "formal_ls", "formal", "named_formal",
  "opt_formal_intent_tag", "required_intent_tag", "opt_this_intent_tag",
  "proc_iter_or_op", "opt_ret_tag", "opt_throws_error",
  "opt_function_body_stmt", "function_body_stmt", "query_expr",
  "opt_lifetime_where", "lifetime_components_expr", "lifetime_expr",
  "lifetime_ident", "type_alias_decl_stmt", "type_alias_decl_stmt_start",
  "type_alias_decl_stmt_inner_ls", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_type", "var_decl_start", "var_decl_stmt",
  "$@10", "$@11", "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
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

#define YYPACT_NINF (-923)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-755)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -923,   105,  3890,  -923,   -46,    54,  -923,  -923,  -923,  -923,
    -923,  -923,  -923, 11946,    73,    66,    65, 13750,    99, 17732,
      73,   215,    94,    84,    66,  5277, 11946,   129, 17790,  -923,
     310,   262,  -923,  9891,   284,   416,   175,  -923,   301,   272,
   17790, 17790, 17790,  3033, 11090,   362, 11946, 11946,   269,  -923,
     370,   377, 11946,  -923, 13750,  -923, 11946,   429,   329,   195,
     238, 13111,   450, 17848,  -923, 11946,  8018, 11946, 11090, 13750,
     379,   478,   369,  5277,   486, 11946,   506,  6998,  6998,  -923,
     525, 16706, 13750,  -923,   528, 10061,  -923, 11946,  -923, 11946,
    -923,  -923, 13580, 11946,  -923, 10231,  -923,  -923,  -923,  4239,
    8188, 11946,  -923,  4931,  -923,  -923,  -923,  -923, 17394,   580,
    -923,   447,   430,  -923,   207,  -923,  -923,  -923,  -923,  -923,
    -923,  -923,  -923,  -923,  -923,  -923,  -923,  -923,  7338,  -923,
    7508,  -923,  -923,    26,  -923,  -923,   375,  -923,   541,  -923,
    -923,  -923,  -923,  -923,  -923,  -923,  -923,  -923,   459, 17790,
    -923,   441,  -923,   242,  -923,  -923,  -923,   668,   901,  -923,
    -923, 17790,  -923,  1282,  -923,   457,   448,  -923,  -923,  -923,
     458,   460, 11946,   463,   465,  -923,  -923,  -923, 17151,  3091,
     391,   467,   473,  -923,  -923,   396,  -923,  -923,  -923,  -923,
    -923,   381,  -923,  -923,  -923, 11946,  -923, 17790,  -923, 11946,
    -923, 11946,    21,   566,  -923,  -923,  -923,  -923, 17151,   403,
    -923,  -923,    40,  5277,  -923,  -923,  -923,   475,   254,   474,
    -923,   297,  -923,   481,  -923,   225, 17151,  8358,  -923,  -923,
    -923,  -923,  -923,  -923,  -923,  -923,  -923,  -923,  -923,  -923,
    -923,  -923,  -923,  -923,  -923,  -923,  -923,  -923,  -923,  -923,
    -923,  -923,  -923,  -923,  -923,  -923, 17790, 17790,   -29, 14332,
    -923,  -923,   557,  8358,   489,   485,  -923,  5277,  3496,    98,
      87,  -923,  5277,  -923,  -923, 15111,  -923,    28, 15504,   307,
    -923,   487,   490,  -923, 15111,   254,   307,  -923,  8358,  1447,
    1447,    56,    -5,  -923,    14,  -923,  -923,  -923,  -923,  7678,
    -923,  -923,   389,  -923,   488,   511,  -923,  -923,  3702,   518,
    -923,  -923, 17151,   246, 11946, 11946,  -923,    12,  -923,  -923,
   17151,   501, 15544,  -923, 15111,   254,   496,  8358,  -923, 17151,
   15662, 11946,   575,  -923,  -923,  -923,  -923,  -923, 11946,   254,
     513,   217,   217,   609,   307,   307,   286,  -923,  -923,  4412,
     159, 11946,   532,   -64,   499,   609,   669,  -923,  -923, 17790,
    -923, 11946,  -923,  -923,  4239,  -923,   116,  -923,   557,  -923,
    -923,   676,   510,  4585, 11946,  -923, 11946,   616,  -923,  -923,
   14876,    34, 17906,   459, 17151,   247,  -923,  5277,   604,  -923,
    -923,  9891, 10401, 11260,  -923,  -923,  -923, 17790,  -923, 17790,
   11946,   515,  -923,  1592,  -923,  -923,  -923,  -923,    69,  -923,
    -923,  -923,  -923,  -923,  -923,  -923,   146,   141,  -923,  -923,
    -923,  -923,  -923,  -923,  -923, 13415,   561,   252,  -923,   522,
     259,   407,  -923,   539, 11946, 11946, 11946, 11260, 11260, 11946,
     423, 11946, 11946, 11946, 11946, 11946,   340, 13580, 11946, 11946,
   11946, 11946, 11946, 11946, 11946, 11946, 11946, 11946, 11946, 11946,
   11946, 11946, 11946, 11946,   619,  -923,  -923,  -923,  -923,  -923,
    -923,  -923,  -923,  -923,  -923,  -923, 10061,  -923,  -923, 10061,
   10061,  8358,  8528, 11260, 11260, 15702,   519,  -923, 11430, 11260,
   17790,  -923,  6312,  -923,  -923,  -923,  3033,  -923, 11946,  -923,
     565,   520,   545,  -923,  -923,   569,   570,  5277,   660,  5277,
    -923,   665, 11946,   638,   533,  -923, 10061,  -923,  3496,  -923,
    -923,   -53,  -923, 12116,   584, 11946,  3033,  -923,  -923, 11946,
    -923,  2737, 11946, 11946,  -923,   297,   537,  -923,  -923,  -923,
    -923, 17790,  -923,   668, 13281,  3496,   568, 12116,  -923, 17151,
   17151,  -923,    15,  -923,    38,  -923,  8358,   538,  -923, 15111,
     689,   689, 15742,  -923,  -923,  -923,  -923, 11603,  -923, 15820,
    8701,  8871,  -923,  -923,  -923, 17151, 11260, 11260,  -923,   447,
    9041,  -923,   308,  -923,  4758,  -923,   294, 15860,   296, 14916,
   17790,  6825,  6655,  -923,   459,   546,  -923,  -923,   297,  -923,
     596, 17790,    55, 15111,   547, 13891,   -24,   119, 11946,   -67,
   15908,   -50,    32,   765,  -923,   -47,  -923,   123,   571,  1874,
     549,  -923, 17674,   600,   160,  -923,  -923,  -923,  -923,  6485,
    -923,   621,  -923,  -923,   558,   582,  -923,   585,  -923,   587,
     589,   591,   592,   593,  -923,   594,   595,   598,   607,   610,
     612,   615,  -923,  -923,  -923,  -923,  -923,  -923,  -923,  -923,
    -923,  -923,  -923,  -923,  -923,  -923,  -923,  -923,  -923,  -923,
    -923,  -923,  -923,  -923, 11946,   608,   618,   558,  -923,   558,
    -923, 11946,  -923, 17790,  -923,  -923,  -923, 17373, 17373,   468,
    -923,   468,  -923,   468, 17458,   644,  1274,   937,   254,   217,
    -923,   444,  -923,  -923,  -923,  -923,  -923,   609, 17510,   468,
    1644,  1644, 17373,  1644,  1644,   507,   217, 17510, 17421,   507,
     307,   307,   217,   609,   617,   622,   624,   629,   560,   590,
    -923,   468,  -923,   468, 11946, 10061,    82,   740, 15151,   628,
     611,   211,  -923,  -923,  -923,  -923,  -923,  -923,  -923,  -923,
     118,  -923,  2037, 17191,   415,   298, 17151, 10061,  -923, 10061,
   11946, 11946,   744,  5277,   752, 16015,  5277, 15269, 17790,  -923,
     166,   182,  3496,    98,  -923, 17151,  9211,  -923, 17151,  -923,
    -923,  -923, 17790, 16063, 16103,  3033,  -923,   568,   656,  -923,
     196, 12116,  -923,   -35,  -923, 11946,  5104,  -923,  -923,   634,
       0,   633,  -923,  2239,  -923,    53,  -923,  -923,  -923,  -923,
    -923,  -923,  -923,    83,    57,  3692,  -923,  -923,   206,  -923,
    -923,  -923,  -923, 11946,  -923,    74, 13992, 11946,  -923, 11946,
    6825,  6655,  -923,  9381,   422,  9551,   428,   433,  9721,  7848,
     439,   305,  -923,  -923,  -923, 16253,   661,   642,   639,  -923,
   17790,  3033, 11946,   764,  -923,  -923, 11946, 17151,  5277,   640,
   11260, 10574,  -923,  4066,   103,   641,   673,  -923,   153, 11946,
   11773,  -923,  5277,  -923,    55,  5450, 11260,  -923,    55, 17790,
     474,  -923, 10061,   650,   498,  -923,  -923, 11946,    47,  -923,
    -923,  -923,  -923,  -923,  -923,    25,   339,  -923, 16293,  -923,
   14149,  -923,  -923, 17151,  -923,   652,   648,  -923,  -923,  -923,
    -923,  -923,  -923,   218, 11260, 13891, 11946, 12286, 11260, 10061,
    -923,  -923,  -923,  7168,  -923,  -923,  -923,  -923,  -923,  -923,
    -923,  -923,  -923,  -923,  -923,  -923,    37, 11260, 11260,  -923,
    -923,  -923, 17151, 17151,  5277,  -923,  5277, 11946,  -923,  5277,
     790,   666,   670,  -923,  -923,   584,  -923,  -923,  -923,   658,
     664,   317,   -35, 11946,   621,   568,  -923, 10747,  -923,  -923,
   17151,  -923,  -923,  5277, 11946,  -923,  -923,  -923, 17790,  -923,
     691,   474,   672,   170, 11260,  5277, 11260, 10061,  -923,  -923,
    -923,  -923,  -923,   444,  -923,  -923,  -923,  -923,  -923,  -923,
    -923,  -923,  -923,  -923,  -923,  -923,  -923,  6825,  -923,  -923,
   16338, 15309,   318,   674,  6825,  -923, 11946,  -923,  -923,  2544,
     321,   330, 16443, 11946,   234,  -923,  5277,    -9, 15354,  -923,
     323,  -923, 17151,  -923,  -923,  5277, 10917,  -923, 17151,  -923,
   16495, 17151,  -923,  9891, 17151,    61, 14961,  -923,  -923,  -923,
     236,  -923,  -923,  -923,  -923, 17348,  1686,  -923, 17151, 17790,
     580,    97, 11946,  -923,   693,   671,  -923,  -923,  -923,   558,
     558,  -923,  -923,  -923,   675, 16535, 11946,   811, 16685,   250,
    -923,   584,   261,   266,  -923,  -923, 17151,   810,  5277,  -923,
    -923,  -923,  8358,  8358,  -923,  -923,  -923,  -923,  -923,   -35,
   17151,  -923,   130,    81,   678, 11946,  -923,  -923,  5277, 11946,
    -923,  -923,   677,    67, 15459,    57, 15066,  -923,   167, 11946,
    6825,  6655,  -923,  -923,  -923, 16725, 17151, 14304,  -923,  -923,
    -923,  -923, 15111,  -923,  -923,  5623,   679,  5796,   680,  -923,
    -923, 11946, 14410,  5969,  -923,    55,  6142,  -923,    55,   682,
    1686,  -923,  -923,  -923, 17674,  -923,  -923,   132,  -923,   -43,
    -923,   449, 16770,    27,   742,  -923,  -923,  -923, 12456, 12626,
   16875, 12796, 12966,  -923,  -923,  5277,  -923,   686,   688,  -923,
    -923,  5277,  -923,   474, 17151,  -923,   227, 15111,  -923,  5277,
    5277,  -923, 17151,   331,   684, 11946,  -923,  -923,  9891,  -923,
   17151,  5277,  9891,  -923, 17151,  5277, 17151,  5277,  9891, 17151,
    -923,  9891, 17151,  -923,  -923,  -923,  -923,  -923, 11946,    97,
     132,   132,   132,   132,   132,   132,    97,  -923,  -923,  -923,
    -923, 11946, 11946, 11946, 11946, 11946,  -923,   698,   700,   596,
    -923,  5277,  -923,  -923,  -923, 17151, 14490,  -923, 14568,  -923,
   16015, 14648, 14726, 17151,  -923,  -923,  -923,  -923,  -923,  -923,
    -923,   706, 16953, 16993, 17033, 17111,  -923,  -923,   596,  5277,
    5277,  5277,  5277, 11946, 11946, 11946, 11946, 16015, 16015, 16015,
   16015
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   138,   471,    56,   694,   695,
     696,   690,   691,   208,   604,   127,   159,   575,   166,   577,
     604,   165,   503,   500,   127,     0,   208,   273,   160,   197,
     196,   688,   195,   208,   164,    69,   274,   342,   161,   139,
       0,     0,     0,     0,   208,   168,   208,   208,   625,   597,
     697,   169,   208,   343,   569,   497,   208,     0,     0,   182,
     180,   428,   163,   578,   499,   208,   208,   208,   208,   573,
       0,   167,     0,     0,   140,   208,   689,   208,   208,   490,
     162,   579,   571,   501,   170,   208,   750,   208,   752,   208,
     753,   754,   641,   208,   751,   208,   649,   178,   749,     0,
     208,   208,     4,     0,     5,    10,    11,    12,    49,    52,
      53,    57,     0,    46,    72,    13,    78,    14,    15,    16,
      17,    29,   556,   557,    22,    47,   179,   189,   208,   198,
     662,   190,    18,     0,    31,    30,     0,    48,     0,   268,
      19,   658,    21,    36,    32,    33,    34,   188,   320,     0,
     186,     0,   341,     0,   653,   184,   358,   454,   440,   651,
     187,     0,   502,     0,   185,   667,   647,   646,   650,   560,
     558,     0,   208,   656,   657,   661,   660,   659,     0,   559,
       0,   668,   669,   670,   692,   693,   652,   562,   561,   654,
     655,     0,    28,   577,   160,   208,   139,     0,   578,   208,
     579,   208,     0,     0,   667,   558,   656,   657,   567,   559,
     668,   669,     0,     0,   605,   128,   129,     0,   576,     0,
     491,     0,   498,     0,    20,     0,   538,   208,   135,   141,
     152,   147,   146,   154,   132,   145,   155,   142,   136,   156,
     130,   157,   150,   144,   151,   148,   149,   131,   133,   143,
     153,   158,   137,   330,   134,   207,     0,     0,     0,     0,
      71,    70,    72,   208,     0,     0,   250,     0,     0,     0,
       0,   509,     0,   175,    41,     0,   311,     0,   310,   735,
     629,   626,   627,   628,   505,   570,   736,     7,   208,   340,
     340,   435,   172,   430,   173,   431,   432,   436,   437,   171,
     438,   439,   428,   534,     0,   345,   346,   348,     0,   429,
     533,   350,   521,     0,   208,   208,   176,   653,   644,   666,
     645,     0,     0,    44,     0,   574,     0,   208,    45,   568,
       0,   208,   579,   281,    49,   285,   282,   285,   208,   572,
       0,   737,   739,   639,   734,   733,     0,    75,    79,     0,
       0,   208,     0,     0,   547,   640,     0,     6,   317,     0,
     196,   208,   318,   319,     0,    50,     0,     9,    72,    51,
      54,     0,    61,     0,   208,    73,   208,     0,   503,   194,
       0,   690,   579,   320,   663,   203,   214,     0,   211,   212,
     213,   208,   208,   208,   210,   580,   588,     0,   252,     0,
     208,     0,   316,     0,   452,   453,   451,   366,   456,   461,
     464,   455,   459,   458,   460,   463,     0,   444,   446,   450,
     449,   441,   442,   445,   448,     0,   495,     0,   492,     0,
       0,   648,    35,   630,   208,   208,   208,   208,   208,   208,
     738,   208,   208,   208,   208,   208,     0,   638,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   397,   404,   405,   406,   401,   403,
     407,   408,   399,   402,   400,   398,   208,   410,   409,   208,
     208,   208,   208,   208,   208,     0,     0,   211,   208,   208,
       0,   609,     0,    37,    24,    38,     0,    25,   208,    39,
     556,     0,   551,   552,   555,     0,     0,     0,   241,     0,
     397,   237,   208,     0,     0,    23,   208,    26,     0,   513,
     515,     0,   532,   208,   518,   208,     0,   174,    40,   208,
     313,     0,   208,   208,    42,     0,     0,   183,   181,   433,
     434,     0,   429,   454,   428,     0,   536,   208,   122,   664,
     665,   356,     0,   177,     0,    43,   208,     0,   292,     0,
     283,   284,     0,    27,    77,    76,    80,   208,   698,     0,
     208,   208,   686,   684,     8,   505,   208,   208,   209,    57,
     208,    55,     0,    66,     0,   120,     0,   115,     0,    87,
       0,   208,   208,   191,   320,     0,   199,   205,     0,   202,
       0,     0,     0,     0,     0,   589,     0,     0,   208,     0,
     538,     0,     0,     0,   255,     0,   253,   279,   321,     0,
       0,   329,     0,   338,     0,   331,   462,   457,   526,     0,
     528,   465,   447,   443,   415,   135,   393,   141,   391,   147,
     146,   145,   142,   136,   395,   156,   157,   144,   148,   131,
     143,   158,   390,   372,   375,   373,   374,   396,   385,   376,
     389,   381,   379,   392,   380,   378,   383,   388,   377,   382,
     386,   387,   384,   394,   208,   369,     0,   415,   370,   415,
     367,   208,   494,     0,   489,   508,   507,   730,   729,   732,
     741,   740,   745,   744,   726,   723,   724,   725,   642,   713,
     138,     0,   680,   681,   140,   679,   678,   636,   717,   728,
     722,   720,   731,   721,   719,   711,   716,   718,   727,   710,
     714,   715,   712,   637,     0,     0,     0,     0,     0,     0,
     743,   742,   747,   746,   208,   208,     0,     0,     0,   279,
       0,   615,   616,   622,   621,   623,   618,   624,   620,   607,
       0,   610,     0,     0,     0,     0,   539,   208,   564,   208,
     208,   208,   240,     0,   236,     0,     0,     0,     0,   563,
       0,     0,   516,     0,   531,   530,   208,   511,   635,   510,
     312,   309,     0,     0,     0,     0,   677,   536,   351,   347,
       0,   208,   537,   518,   349,   208,     0,   469,   355,     0,
       0,     0,   565,     0,   286,     0,   699,   643,   685,   548,
     547,   687,   549,     0,     0,     0,    58,    59,     0,    62,
      64,    68,    67,   208,    98,     0,     0,   208,    93,   208,
     208,   208,    65,   208,   372,   208,   373,   374,   208,   208,
     384,     0,   411,   412,    82,    81,    92,     0,     0,   322,
       0,     0,   208,     0,   217,   216,   208,   590,     0,     0,
     208,   208,   545,     0,   540,     0,   543,   702,   542,   208,
     208,   706,     0,   225,     0,     0,   208,   223,     0,     0,
       0,   276,   208,     0,   340,   328,   335,   208,   333,   327,
     524,   523,   522,   525,   466,   472,   428,   365,     0,   371,
       0,   361,   362,   496,   493,     0,     0,   126,   125,   124,
     123,   675,   676,     0,   208,   700,   208,   208,   208,   208,
     608,   617,   619,   208,   606,   159,   166,   165,   164,   161,
     168,   169,   163,   167,   162,   170,     0,   208,   208,   504,
     554,   553,   249,   248,     0,   243,     0,   208,   239,     0,
     245,     0,   278,   514,   517,   518,   519,   520,   308,     0,
       0,     0,   518,   208,   465,   536,   535,   208,   424,   422,
     357,   470,   294,     0,   208,   293,   296,   566,     0,   287,
     290,     0,     0,     0,   208,     0,   208,   208,    60,   121,
     118,    99,   110,   105,   104,   103,   112,   100,   113,   108,
     102,   109,   106,   107,   101,   111,   114,   208,   117,   116,
      89,    88,     0,     0,   208,   192,   208,   193,   322,   340,
       0,     0,     0,   208,     0,   233,     0,     0,     0,   701,
     541,   231,   591,   546,   704,     0,   208,   703,   708,   707,
       0,   584,   226,   208,   583,     0,     0,   224,   254,   251,
       0,   315,   182,   180,   314,   340,   340,   323,   339,     0,
       0,     0,   208,   359,   420,     0,   417,   421,   368,   415,
     415,   682,   683,   278,     0,     0,   208,     0,     0,     0,
     611,   518,   667,   667,   242,   238,   596,   244,     0,    74,
     275,   512,   208,   208,   506,   425,   423,   352,   353,   518,
     414,   298,     0,     0,     0,   208,   288,   302,     0,   208,
     300,   304,     0,     0,     0,     0,     0,    63,     0,   208,
     208,   208,    94,    96,    85,    84,    83,   340,   200,   206,
     204,   215,     0,   235,   234,     0,     0,     0,     0,   705,
     232,   208,     0,     0,   221,     0,     0,   219,     0,   280,
     340,   325,   324,   334,     0,   486,   487,     0,   488,   474,
     477,     0,   473,     0,   428,   416,   363,   364,   208,   208,
       0,   208,   208,   280,   612,     0,   247,     0,     0,   427,
     426,     0,   297,     0,   291,   306,     0,     0,   301,     0,
       0,   119,    90,     0,     0,   208,   201,   218,   208,   229,
     593,     0,   208,   227,   592,     0,   709,     0,   208,   582,
     222,   208,   581,   220,   277,   326,   337,   485,   208,     0,
       0,     0,     0,     0,     0,     0,     0,   467,   360,   468,
     418,   208,   208,   208,   208,   208,   246,   631,   632,   299,
     289,     0,   305,    95,    97,    86,     0,   230,     0,   228,
     587,     0,     0,   476,   478,   479,   482,   483,   484,   480,
     481,   475,     0,     0,     0,     0,   633,   634,   307,     0,
       0,     0,     0,   208,   208,   208,   208,   595,   594,   586,
     585
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -923,  -923,  -923,     3,   -57,  2496,  -923,   405,   757,  -923,
    -923,   453,  -392,  -106,  -923,   288,  -923,  -923,  -118,  -923,
    -923,    44,   613,  -923,  -550,  1922,   503,  -576,  -923,  -804,
    -923,  -923,  -923,    48,  -923,  -923,  -923,   848,  -923,  2987,
    -208,  -923,  -923,  -466,   213,  -923,  -852,  -923,  -923,   260,
    -923,  -923,  -923,  -923,  -923,  -923,  -923,  -923,  -923,     5,
    -923,  -149,   508,  -347,  -923,   491,  -923,  -923,  -923,    -1,
    1503,  -923,  -923,  -923,  -923,   542,  -923,   -97,  -923,  -923,
    -923,  -923,  -923,  -923,  -923,  -923,   359,  -923,  -923,  -923,
    -101,  -923,  -348,  -806,  -923,  -923,  -923,  -923,  -923,  -614,
    -923,  -923,  -923,  -923,   342,  -923,  1634,  -923,  -923,  -923,
    -923,   504,  -923,  -923,  -923,  -923,    -8,  -409,  -173,  -790,
    -922,  -657,  -923,  -923,  -273,  -868,   599,   -49,  -923,  -923,
     350,   -70,  -923,  -267,  -923,  -923,  -329,  -321,  -668,  -923,
    -923,    50,   220,  -923,   383,  -200,   778,  -923,  -923,  -154,
     384,   155,  -434,  -768,  -605,  -923,  -923,  -923,  -731,  -513,
    -923,  -750,   -16,  -923,  -923,  -578,   -12,  -923,  -262,  -499,
    -562,    -2,  -923,  -923,  -923,  -923,  -923,  -923,   136,  -923,
     914,  -481,  -923,    16,  -923,  -923,   357,   623,  -923,  -923,
    -923,  -923,  2418,  -923,   -52,   959,  -923,  1144,  1157,  -923,
    -923,  -923,  -923,  -923,  -923,  -923,  -923,  -923,  -479
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   348,   103,   104,   105,   106,   107,   366,
     367,   108,   109,   110,   111,   372,   581,   818,   819,   112,
     113,   114,   377,   115,   116,   117,   349,   846,   588,   847,
     118,   119,  1008,   585,   586,   120,   121,   217,   252,   269,
     122,   254,   123,   303,   534,   124,   125,   290,   289,   126,
     127,   128,   129,   130,   597,   131,   598,   850,   132,   202,
     134,   393,   394,   606,   135,   258,   136,   137,   615,   616,
     203,   139,   140,   141,   142,   560,   804,   981,   143,   144,
     800,   976,   145,   983,  1111,  1112,   276,   277,   146,   147,
     148,   149,   401,   884,   150,   151,   624,  1059,  1060,   625,
     152,   153,   304,   305,   306,   964,   154,   318,   552,   795,
     155,   156,  1163,   157,   158,   676,   677,   842,   843,   844,
     968,   897,  1064,  1065,  1066,   307,   308,   542,   425,   407,
     416,   895,  1228,   798,   159,  1063,  1159,  1160,  1161,   160,
     161,   427,   428,   682,   162,   163,   164,   221,   535,   270,
     271,   520,   521,   777,   310,   893,   630,   631,   524,   311,
     792,   793,   350,   608,   609,   864,   352,   353,   501,   502,
     503,   204,   166,   167,   395,   168,   396,   169,   205,   171,
     213,   214,   750,   751,   752,   172,   206,   207,   175,   176,
     177,   319,   178,   385,   504,   209,   180,   210,   211,   183,
     184,   185,   186,   611,   187,   188,   189,   190,   191
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     165,   514,   797,   370,   302,   102,   480,   133,   886,   430,
     225,   622,   309,   754,   321,   841,   678,   770,   820,   500,
     901,   496,   902,   165,   892,   969,   536,  1013,  1067,   383,
     133,   868,  1057,   340,   794,   595,  1009,   962,   522,   799,
    1095,   491,   955,  1019,  -354,   386,   612,   796,   354,   313,
     386,  -273,  -336,   489,   982,   500,   358,   774,   387,   796,
     272,   487,   389,   390,   872,   557,   388,   389,   390,   215,
    -274,   165,   507,  1218,   973,   165,   165,   700,   133,   262,
     500,   571,   133,   133,   771,  1061,   512,   272,   776,   272,
     690,   692,   772,  1143,   212,   870,   862,   165,   879,   522,
    1155,   165,  1219,   539,   133,     3,   867,   572,   133,   212,
    -256,   790,  -295,   272,   967,   974,   626,   192,   773,   500,
     216,  -257,   362,   871,   880,   859,   865,  -303,   196,   914,
     984,   874,   878,   551,   525,  1155,   730,   732,   170,   364,
     386,  1062,   737,   391,   363,  -529,   540,   628,   387,   858,
     212,  1156,   368,  -295,   386,  -263,   388,   389,   390,   627,
    -529,   170,  1181,   891,  1135,   227,   860,  1057,  -303,   975,
     388,   389,   390,   529,   222,   212,   704,  1179,  -527,   212,
    -354,   523,  -529,   364,   263,   212,  1156,  1091,   632,  1157,
    1227,   212,  -332,   488,  1096,   364,   364,   941,   392,  1158,
     364,   212,   220,  1151,  1152,  1081,  -527,  -295,  -529,   170,
     492,   165,  1127,   170,   170,  1099,  -332,   577,   133,   728,
     729,  -529,  -303,   364,  1124,   364,   498,   861,   498,   364,
     814,   633,   526,   391,  1158,   170,   913,   212,   706,   170,
    1189,   -71,   523,  -529,  1108,   862,   848,   391,   436,   364,
    -527,   386,   679,   374,  1012,   915,   985,   274,   921,  1241,
     -71,  -529,  -527,   923,   861,   165,  1033,   388,   389,   390,
     165,  -262,   133,   500,   500,  1057,  1034,   133,   966,   -71,
     594,   323,   862,  1030,   -70,  1109,   881,   280,   576,   924,
     629,   375,   863,   882,   801,   404,  1067,    37,  1215,   227,
    -340,   922,   392,   -70,   567,   888,   437,   -71,   500,  -527,
     438,   759,  1014,  1174,  -527,   405,  1033,  1194,  -271,   376,
    -340,   255,   -70,    53,    23,   406,  1037,   772,   568,   889,
    -340,  1180,   865,   874,   878,   785,  1191,   952,   436,  1110,
    -261,   772,   755,   700,   391,   263,   281,   165,   500,   170,
     -70,   987,   566,   953,   133,   596,   440,  -258,  1097,   173,
     701,   444,   165,   759,   446,   282,   291,   965,   449,   133,
     498,   165,   500,   702,   264,    55,   607,   988,   133,   498,
     283,   759,   173,  1050,   618,   165,   293,    64,   499,  1073,
     295,   498,   133,   440,   196,   759,   437,   683,   851,   392,
     438,   446,   703,   170,   526,  1133,  -614,  1149,   170,   548,
      83,  -613,  1166,  1167,   296,   684,   291,   297,  -265,    57,
    1079,  1173,   686,   680,   725,   820,  -269,   726,   727,   298,
     173,  1118,  -614,  -266,   173,   173,   293,  -613,   287,   823,
     295,   827,   704,   526,   754,  1153,   440,   300,   705,   192,
    1014,   444,   301,   445,   446,   564,   173,   824,   449,   828,
     173,   939,   526,  1014,   296,   456,   683,   297,  1015,   356,
     483,   192,   736,   462,   484,   526,  1014,   821,   326,   298,
    1094,  1122,   333,   336,  1129,   170,  1033,   302,   528,  1217,
     530,   678,  1056,  1130,  1243,   309,  1139,   300,   260,   288,
     170,   261,   301,     7,   938,   165,  -260,   165,   484,   170,
    -419,  -752,   133,  1027,   133,  -752,   370,  -753,   810,   810,
     358,  -753,  -754,   170,    22,    23,  -754,   500,  -749,  1045,
     334,   334,  -749,   359,  -264,    29,   360,   555,   436,   327,
    1216,    32,  -270,   364,  1193,   397,  1136,  1138,    37,   537,
     538,   500,  1255,  1256,  1257,  1258,  1259,  1260,   809,   812,
     813,   481,  -272,   482,  1145,  1148,  -674,  1074,  -674,   361,
     173,  1077,  1220,  -671,    53,  -671,    55,  -673,    57,  -673,
    1052,  -259,   165,  1053,  -267,     7,   362,   566,    64,   133,
     690,   730,   866,  -672,   371,  -672,   437,   399,   373,  1221,
     438,  1222,  1223,   400,  1224,  1225,    79,   440,   363,   403,
     431,    83,   444,   797,   905,   446,   906,   990,   706,   449,
    -599,  -598,   490,   432,   173,   174,  -602,  1056,  -603,   173,
    -601,   961,  1136,  1138,  1145,  1148,  -600,  1113,   494,  1115,
     436,   375,   364,   170,   497,   170,   440,   441,   174,   442,
     443,   444,   515,   445,   446,   516,   544,   532,   449,   543,
     533,    97,   547,  1150,   553,   456,   556,  1054,  1154,   338,
     573,   460,   461,   462,   500,   436,   563,   570,   574,   579,
     580,   590,   601,   619,   681,   685,   446,   724,   757,   735,
     759,   758,   760,   761,   763,   408,   174,  1021,   437,   766,
     174,   174,   438,   768,   769,   940,   173,   776,   786,   802,
     803,   500,   791,   852,   849,   409,   498,   856,   885,   410,
     170,   173,   174,   887,   957,   894,   174,  1070,   896,  -138,
     173,   911,  -159,   437,  -166,  1056,  -165,   438,  -164,  -161,
    -139,  -168,  -169,   411,   173,  -163,   412,   899,   440,   441,
     936,   442,   443,   444,  -167,   445,   446,  -140,   413,  -162,
     449,   165,  -170,   912,   165,   900,   455,   456,   133,   291,
     459,   133,   916,   460,   461,   462,   414,   434,   944,   500,
     907,   415,   920,   440,   435,   908,   946,   909,   444,   293,
     445,   446,   910,   295,   165,   449,   436,   875,   919,   971,
     963,   133,   456,   972,   977,  1017,  1014,  1018,   460,   461,
     462,  1023,   876,  1026,  1035,   854,   855,   296,  1036,  1051,
     297,  1072,    57,  1071,  1088,   873,   877,  1055,  1092,  1089,
    1177,  1178,   298,  1090,  1093,  1105,   174,  1123,  1164,   302,
    1024,  1107,  1165,  1171,  1175,  1214,  1188,  1244,  1168,  1183,
     300,  1219,  1201,  1205,   437,   301,   165,  1237,   438,  1238,
    1266,   165,  1267,   133,   173,   365,   173,   816,   133,  1117,
     165,   989,   223,   165,   578,   513,   584,   133,  1048,   561,
     133,  1104,   602,   212,   500,   500,   789,   439,   780,   599,
     174,  1230,  1069,   788,  1098,   174,  1229,  1261,  1254,   170,
    1020,   541,   170,   904,   440,   441,   379,   442,   443,   444,
     779,   445,   446,   447,   782,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   954,   417,   460,
     461,   462,   170,   364,   219,  1082,  1083,     0,     0,  1080,
     463,   173,   165,     0,   165,     0,     0,   165,   418,   133,
     370,   133,   419,     0,   133,     0,     0,     0,  1103,     0,
       0,   179,  1055,     0,     0,     0,     0,     0,   436,   512,
       0,   165,   174,     0,     0,     0,   420,     0,  1102,   421,
       0,     0,     0,   165,   179,     0,     0,   174,     0,     0,
     133,   422,     0,     0,   170,     0,   174,     0,     0,   170,
       0,     0,     0,     0,     0,     0,     0,     0,   170,   423,
     174,   170,     0,     0,   424,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   165,     0,   437,   873,   877,     0,
     438,   133,   179,   165,     0,     0,   179,   179,     0,     0,
     133,     0,     0,     0,   370,     0,     0,     0,   370,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   179,     0,
       0,     0,   179,     0,     0,     0,     0,     0,     0,     0,
    1055,     0,     0,   512,     0,   512,   440,   441,   512,   512,
     170,   444,   170,   445,   446,   170,   165,  1042,   449,     0,
       0,  1047,     0,   133,     0,   456,     0,     0,     0,     0,
       0,   460,   461,   462,     0,     0,   165,   302,     0,   170,
       0,     0,     0,  1186,     0,     0,     0,     0,     0,     0,
     173,   170,     0,   173,     0,     0,     0,     0,     0,     0,
     174,     0,   174,   165,     0,   165,     0,     0,     0,     0,
     133,   165,   133,     0,   165,     0,   181,     0,   133,     0,
       0,   133,     0,   173,     0,     0,     0,     0,     0,   182,
       0,     0,   170,     0,     0,     0,     0,     0,     0,   181,
       0,   170,   179,   165,     0,     0,     0,     0,     0,   165,
     133,     0,   182,     0,     0,     0,   133,   165,   165,     0,
       0,     0,     0,     0,   133,   133,     0,     0,     0,   165,
       0,     0,     0,   165,     0,   165,   133,   174,     0,     0,
     133,     0,   133,     0,     0,   173,     0,   181,     0,     0,
     173,   181,   181,     0,   170,     0,   179,     0,     0,   173,
     182,   179,   173,     0,   182,   182,     0,     0,     0,   165,
       0,     0,     0,   181,   170,     0,   133,   181,     0,     0,
       0,     0,     0,     0,     0,     0,   182,     0,  1144,  1147,
     182,     0,     0,     0,     0,     0,     0,   165,   165,   165,
     165,   170,     0,   170,   133,   133,   133,   133,     0,   170,
       0,     0,   170,   429,     0,   228,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   229,
     230,   173,   231,   173,     0,   436,   173,   232,   179,     0,
       0,   170,     0,     0,     0,   233,  1182,   170,     0,     0,
       0,   234,     0,   179,     0,   170,   170,   235,  1144,  1147,
     173,   236,   179,     0,   237,     0,   238,   170,     0,     0,
       0,   170,   173,   170,   239,  1197,   179,     0,     0,     0,
       0,   240,   241,     0,     0,     0,     0,   181,  1210,   242,
       0,  1213,     0,   437,     0,     0,     0,   438,     0,   243,
     182,     0,     0,     0,     0,     0,     0,   170,   244,     0,
     245,     0,   246,   173,   247,     0,   174,   248,     0,   174,
       0,   249,   173,   250,     0,     0,   251,     0,     0,     0,
    1242,     0,     0,     0,     0,   170,   170,   170,   170,     0,
       0,   181,     0,   440,   441,     0,   181,   443,   444,   174,
     445,   446,     0,     0,   182,   449,     0,     0,     0,   182,
       0,     0,   456,     0,     0,     0,     0,     0,   460,   461,
     462,     0,     0,     0,     0,   173,     0,     0,     0,     0,
       0,     0,   268,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   173,   179,     0,   179,   358,
       0,     0,     0,    22,    23,     0,     0,     0,     0,     0,
       0,   174,   359,     0,    29,   360,   174,     0,     0,     0,
      32,     0,   173,   181,   173,   174,     0,    37,   174,     0,
     173,     0,     0,   173,     0,   138,   182,     0,   181,     0,
       0,     0,     0,     0,     0,     0,     0,   181,   361,     0,
       0,   182,     0,    53,     0,    55,     0,     0,   138,     0,
     182,   181,   173,     0,     0,   362,     0,    64,   173,     0,
       0,     0,     0,   179,   182,     0,   173,   173,     0,     0,
       0,     0,     0,     0,     0,    79,     0,   363,   173,     0,
      83,     0,   173,     0,   173,     0,     0,   174,     0,   174,
       0,     0,   174,     0,     0,     0,   138,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   620,     0,   228,   174,     7,   173,     0,
       0,     0,   138,     0,     0,     0,   138,     0,   174,   229,
     230,     0,   231,     0,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,   233,   173,   173,   173,   173,
       0,   234,     0,     0,     0,     0,     0,   235,     0,     0,
       0,   236,     0,     0,   237,     0,   238,     0,     0,   174,
       0,   181,     0,   181,   239,     0,     0,     0,   174,     0,
       0,   240,   241,     0,   182,     0,   182,     0,     0,   242,
       0,     0,     0,     0,     0,   436,     0,     0,     0,   243,
       0,     0,     0,     0,     0,     0,     0,     0,   244,     0,
     245,     7,   246,     0,   247,     0,     0,   248,     0,     0,
     317,   249,     0,   250,     0,     0,   251,     0,   358,     0,
       0,   174,    22,    23,     0,     0,   138,     0,     0,   317,
       0,   359,   179,    29,   360,   179,     0,     0,   181,    32,
       0,   174,     0,   437,   317,     0,    37,   438,     0,     0,
       0,   182,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   179,     0,   361,   174,     0,
     174,   621,    53,     0,    55,     0,   174,     0,  1052,   174,
     138,  1053,     0,     0,   362,   138,    64,     0,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,    79,   449,   363,     0,   174,    83,
       0,   455,   456,     0,   174,   459,     0,     0,   460,   461,
     462,     0,   174,   174,     0,     0,     0,   179,     0,   463,
       0,     0,   179,     0,   174,     0,     0,     0,   174,     0,
     174,   179,     0,     0,   179,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,   138,     0,     0,     0,     0,     0,     0,     0,
       0,   317,     0,     0,   174,     0,     0,   138,     0,     0,
       0,     0,     0,     0,     0,   883,   138,     0,     0,  -322,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     138,     0,   174,   174,   174,   174,  -322,   317,     0,     0,
    -322,  -322,     0,   179,     0,   179,     0,   181,   179,  -322,
     181,  -322,  -322,     0,     0,     0,     0,  -322,     0,     0,
     182,     0,   317,   182,  -322,     0,     0,  -322,     0,     0,
       0,     0,   179,     0,     0,     0,     0,     0,     0,     0,
     181,     0,     0,     0,   179,  -322,     0,  -322,     0,     0,
    -322,     0,  -322,   182,  -322,     0,  -322,  -322,     0,  -322,
       0,   317,  -322,     0,  -322,     0,   273,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -322,     0,  -322,   179,     0,  -322,     0,     0,
     273,     0,     0,     0,   179,     0,     0,     0,     0,   335,
     337,     0,   181,     0,     0,     0,     0,   181,     0,     0,
     138,     0,   138,     0,     0,   182,   181,     0,     0,   181,
     182,     0,     0,     0,     0,     0,     0,     0,     0,   182,
     369,     0,   182,     0,     0,     0,     0,  -322,     0,     0,
     700,     0,     0,  -322,     0,     0,     0,   179,     0,     0,
       0,     0,     0,     0,   925,     0,     0,   926,   398,     0,
       0,     0,   927,     0,     0,     0,     0,   179,     0,     0,
       0,     0,   194,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   928,     0,     0,     0,     0,   138,   181,   929,
     181,   196,     0,   181,   179,     0,   179,     0,     0,   930,
       0,   182,   179,   182,     0,   179,   182,   931,     0,     0,
     317,     0,     0,   317,   317,   317,   317,   181,     0,     0,
       0,     0,     0,     0,   932,     0,     0,     0,     0,   181,
     182,     0,     0,     0,   179,   933,     0,     0,     0,   704,
     179,   495,   182,     0,     0,     0,   934,     0,   179,   179,
     317,   935,     0,     0,     0,     0,     0,     0,     0,     0,
     179,     0,     0,     0,   179,     0,   179,     0,     0,     0,
     181,     0,     0,     0,     0,     0,     0,     0,     0,   181,
     508,   511,     0,   182,     0,     0,     0,     0,     0,     0,
     317,     0,   182,     0,     0,     0,     0,   273,     0,   273,
     179,     0,     0,     0,   317,   317,   273,     0,     0,     0,
       0,     0,     0,     0,   317,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   179,   179,
     179,   179,   181,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   228,     0,     0,   182,   273,     0,     0,     0,
       0,     0,   181,     0,     0,     0,   229,   230,     0,   231,
       0,     0,     0,     0,   232,   182,   138,     0,     0,   138,
       0,     0,   233,     0,     0,     0,     0,     0,   234,   181,
       0,   181,     0,     0,   235,     0,     0,   181,   236,     0,
     181,   237,   182,   238,   182,     0,     0,     0,     0,   138,
     182,   239,     0,   182,     0,     0,     0,     0,   240,   241,
       0,     0,     0,     0,     0,     0,   242,     0,     0,   181,
       0,     0,     0,     0,     0,   181,   243,     0,     0,     0,
       0,     0,   182,   181,   181,   244,     0,   245,   182,   246,
       0,   247,     0,     0,   248,   181,   182,   182,   249,   181,
     250,   181,     0,   251,     0,     0,     0,     0,   182,     0,
       0,   138,   182,     0,   182,     0,   138,     0,     0,   317,
       0,     0,     0,     0,     0,   138,     0,     0,   138,     0,
       0,     0,     0,     0,     0,   181,     0,     0,     0,     0,
       0,   317,     0,   317,     0,     0,     0,     0,   182,     0,
       0,     0,     0,     0,     0,     0,     0,   364,     0,   978,
     317,     0,     0,   181,   181,   181,   181,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   182,   182,   182,   182,
       0,   208,     0,     0,     0,   218,     0,     0,     0,     0,
       0,     0,     0,     0,   226,     0,     0,   138,     0,   138,
       0,   259,   138,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   275,     0,   278,   279,     0,     0,     0,     0,
     284,     0,   285,     0,   286,     0,   138,     0,     0,   312,
       0,   273,     0,   226,   320,   322,   324,   325,   138,     0,
       0,   329,     0,   330,     0,     0,     0,     0,     0,     0,
     339,     0,     0,   320,     0,   341,     0,   342,     0,     0,
     343,   344,     0,   345,     0,     0,   317,   226,   320,   355,
       0,   224,     0,     0,   273,   273,     0,     0,     0,   138,
       0,     0,     0,     0,   273,   273,     0,     0,   138,     0,
       0,     0,     0,     0,     0,     0,   380,     0,   384,     7,
       0,     0,     0,   317,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   358,     0,     0,   328,
      22,    23,     0,     0,     0,     0,     0,     0,     0,   359,
       0,    29,   360,     0,     0,     0,     0,    32,     0,     0,
     433,   138,     0,     0,    37,     0,     0,     0,     0,   357,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   138,     0,   485,     0,   361,     0,   329,     0,   226,
      53,   317,    55,     0,    57,     0,  1052,     0,     0,  1053,
       0,     0,   362,     0,    64,     0,     0,     0,   138,     0,
     138,     0,     0,     0,     0,   320,   138,     0,     0,   138,
       0,     0,    79,     0,   363,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   138,     0,
       0,   320,     0,     0,   138,     0,     0,     0,     0,   950,
       0,     0,   138,   138,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   138,     0,   320,    97,   138,   493,
     138,     0,     0,  1128,     0,     0,     0,   329,     0,     0,
       0,     0,     0,     0,     0,   979,   317,   317,     0,     0,
       0,     0,   549,   550,     0,     0,   273,   273,     0,     0,
     228,     0,     0,     0,   138,   320,     0,     0,     0,   559,
       0,     0,     0,     0,   229,   230,   562,   231,     0,     0,
       0,     0,   232,   517,    23,     0,     0,     0,   527,   569,
     233,     0,   138,   138,   138,   138,   234,     0,     0,   575,
       0,     0,   235,     0,     0,     0,   236,     0,     0,   237,
       0,   238,   587,     0,   589,     0,   273,     0,     0,   239,
     273,     0,  1049,     0,     0,     0,   240,   241,     0,   603,
     610,   613,     0,     0,   242,    55,     0,     0,   226,     0,
       0,     0,     0,     0,   243,     0,     0,    64,     0,     0,
       0,     0,     0,   244,     0,   245,     0,   246,     0,   247,
       0,     0,   248,     0,     0,     0,   249,     0,   250,     0,
      83,   251,   687,   688,   689,   691,   693,   694,     0,   695,
     696,   697,   698,   699,     0,   707,   708,   709,   710,   711,
     712,   713,   714,   715,   716,   717,   718,   719,   720,   721,
     722,   723,     0,   600,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   320,     0,     0,   320,   320,   320,
     320,   731,   733,  1106,     0,     0,   610,   738,     0,     0,
     753,     0,     0,     0,     0,     0,   756,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   765,     0,     0,
     767,     0,     0,     0,   320,     0,     0,     0,     0,     0,
       0,   775,     0,   778,     0,     0,     0,   278,     0,     0,
     783,   784,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,     0,     0,   312,     0,   273,   273,     0,
       0,     0,     0,     0,   320,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   756,     0,     0,   320,   320,
       0,     0,     0,     0,   226,   815,     0,     0,   320,     0,
       0,     0,     0,   762,     0,   764,     0,     0,     0,   845,
     845,     0,     0,     0,     0,   253,     0,     0,     0,     0,
       0,     0,     0,   857,     0,   273,   226,   265,   266,   267,
       0,     0,     0,     0,     0,     0,   228,   273,   273,     0,
       0,     0,     0,     0,     0,     0,     0,   312,     0,     0,
     229,   230,     0,   231,   273,     0,     0,     0,   232,     0,
       0,     0,     0,     0,   511,     0,   233,   273,     0,     0,
     273,     0,   234,     0,     0,     0,     0,     0,   235,     0,
       0,     0,   236,     0,     0,   237,     0,   238,     0,     0,
       0,     0,   898,     0,     0,   239,     0,     0,     0,   903,
       0,     0,   240,   241,     0,  1240,     0,     0,     0,   273,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     243,     0,     0,     0,     0,     0,     0,     0,     0,   244,
       0,   245,     0,   246,     0,   247,   402,     0,   248,     0,
       0,     0,   249,     0,   250,     0,     0,   251,   426,     0,
       0,     0,   765,   320,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   511,     0,
     511,     0,     0,   511,   511,   320,     0,   320,   942,   943,
       0,     0,     0,     0,   486,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   320,     0,     0,     0,     0,     0,
       0,     0,     0,   268,     0,     0,     0,     0,     0,   312,
       0,     0,     0,   970,   464,   465,   466,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   478,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   587,     0,   505,   506,  1010,     0,  1011,   845,   845,
       0,   341,     0,   342,     0,   519,   344,   345,   479,   945,
       0,  -671,   948,  -671,     0,     0,     0,     0,     0,     0,
    1022,     0,     0,     0,   226,     0,     0,     0,  1028,   756,
       0,  1032,     0,     0,     0,     0,     0,  1038,  1040,     0,
    1041,     0,     0,  1044,  1046,   546,     0,     0,     0,     0,
     320,     0,     0,     0,     0,  1058,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1075,  1032,  1041,  1044,  1078,   320,     0,     0,
       0,   753,     0,     0,     0,     0,   253,     0,     0,     0,
       0,     0,     0,     0,  1025,   691,   731,     0,     0,  1031,
       0,     0,     0,     0,     0,  1086,     0,     0,   527,     0,
       0,   527,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   312,     0,     0,   614,  1100,   617,     0,     0,     0,
     623,     0,   226,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1114,     0,  1116,   320,     0,     0,     0,     0,
       0,     0,   675,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   845,     0,     0,     0,     0,
       0,     0,  1125,     0,  1126,     0,     0,     0,     0,     0,
    1084,  1132,  1085,     0,     0,  1087,     0,     0,     0,     0,
       0,     0,     0,     0,   756,     0,     0,     0,     0,     0,
       0,  1142,     0,     0,     0,     0,     0,     0,     0,  1101,
       0,     0,     0,     0,     0,     0,     0,   739,     0,     0,
    1162,  1031,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1170,     0,     0,     0,     0,   228,
       0,     0,     0,     0,     0,   519,     0,     0,     0,     0,
     320,   320,     0,   229,   230,     0,   231,     0,   781,     0,
       0,   232,  1134,  1184,     0,     0,     0,  1187,   787,   233,
       0,  1140,   519,     0,     0,   234,     0,  1192,   845,   845,
       0,   235,     0,     0,     0,   236,     0,     0,   237,     0,
     238,     0,     0,  1200,     0,  1204,     0,     0,   239,  1206,
       0,  1209,     0,     0,  1212,   240,   241,     0,     0,     0,
       0,     0,     0,   242,     0,     0,     0,   832,     0,     0,
       0,     0,     0,   243,  1176,     0,  1200,  1204,   853,  1209,
    1212,     0,   244,     0,   245,     0,   246,     0,   247,     0,
       0,   248,     0,     0,  1185,   249,     0,   250,     0,   623,
     251,     0,     0,  1245,     0,     0,  1246,     0,     0,     0,
    1248,     0,     0,     0,     0,  1250,  1251,     0,     0,  1252,
       0,  1199,     0,  1203,     0,     0,  1253,     0,     0,   527,
       0,     0,   527,     0,     0,     0,     0,     0,     0,  1262,
    1263,  1250,  1264,  1265,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   518,     0,     0,     0,
     426,  1236,     0,     0,     0,     0,     0,  1239,     0,     0,
       0,     0,     0,     0,     0,  1199,  1203,  1277,  1278,  1279,
    1280,  1277,  1278,  1279,  1280,     0,     0,  1247,     0,     0,
       0,  1249,     0,   764,   434,   228,     0,     0,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,   229,
     230,     0,   231,   436,   272,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,   233,     0,  1268,     0,   986,
       0,   234,     0,     0,     0,     0,     0,   235,     0,     0,
       0,   236,     0,     0,   237,   951,   238,     0,     0,   519,
       0,     0,     0,     0,   239,   764,   764,   764,   764,   958,
       0,   240,   241,     0,     0,     0,     0,     0,     0,   242,
       0,   437,     0,     0,     0,   438,     0,     0,     0,   243,
     980,     0,     0,     0,     0,     0,     0,     0,   244,     0,
     245,     0,   246,     0,   247,     0,     0,   248,     0,     0,
     212,   249,     0,   250,   439,     0,   251,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   440,   441,     0,   442,   443,   444,   426,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,     0,   460,   461,   462,     0,
     364,     0,     0,     0,     0,     0,   614,   463,     0,     0,
       0,     0,   545,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   675,     0,     0,
      -2,     4,     0,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -208,    21,    22,    23,    24,    25,
      26,     0,  -208,    27,     0,    28,     0,    29,    30,    31,
    -208,  -208,  -208,    32,    33,    34,   -69,     0,    35,    36,
      37,     0,    38,  -340,    39,     0,    40,    41,    42,    43,
       0,    44,    45,    46,    47,   -69,    48,    49,     0,    50,
      51,    52,     0,  -340,     0,   980,    53,    54,    55,    56,
      57,    58,    59,  -340,   -69,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
      81,    82,   -69,    83,    84,     0,     0,  -208,     0,    85,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,   623,    93,     0,     0,
      94,    95,    96,    97,     0,     0,    98,     0,    99,     0,
     100,     0,  -208,     0,     0,   101,     0,     4,     0,     5,
       6,     7,     8,     9,    10,    11,    12,     0,  -700,     0,
      13,    14,    15,    16,    17,  -700,    18,     0,    19,    20,
    -208,    21,    22,    23,    24,    25,    26,  -700,  -208,    27,
    -700,    28,     0,    29,    30,    31,  -208,  -208,  -208,    32,
      33,    34,   -69,     0,    35,    36,    37,     0,    38,  -340,
      39,     0,    40,    41,    42,    43,     0,    44,    45,    46,
      47,   -69,    48,    49,     0,    50,    51,    52,     0,  -340,
       0,   623,    53,    54,    55,    56,     0,    58,    59,  -340,
     -69,    60,    61,    62,    63,  -700,    64,    65,    66,  -700,
      67,    68,    69,    70,    71,    72,    73,     0,    74,    75,
       0,    76,    77,    78,    79,    80,    81,    82,   -69,    83,
      84,     0,     0,  -208,     0,    85,     0,     0,  -700,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,  -700,    89,  -700,  -700,
    -700,  -700,  -700,  -700,  -700,     0,  -700,  -700,  -700,  -700,
    -700,  -700,  -700,  -700,  -700,  -700,  -700,  -700,    96,    97,
    -700,  -700,  -700,     0,    99,  -700,   100,     0,  -208,     0,
     346,  -700,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -208,    21,    22,    23,    24,    25,    26,
       0,  -208,    27,     0,    28,     0,    29,    30,    31,  -208,
    -208,  -208,    32,    33,    34,   -69,     0,    35,    36,    37,
       0,    38,  -340,    39,     0,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -69,    48,    49,     0,    50,    51,
      52,     0,  -340,     0,     0,    53,    54,    55,    56,    57,
      58,    59,  -340,   -69,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
       0,    74,    75,     0,    76,    77,    78,    79,    80,    81,
      82,   -69,    83,    84,     0,     0,  -208,     0,    85,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,    97,     0,     0,    98,     0,    99,   347,   100,
       0,  -208,     0,     4,   101,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,  -208,    21,    22,    23,
      24,    25,    26,     0,  -208,    27,     0,    28,     0,    29,
      30,    31,  -208,  -208,  -208,    32,    33,    34,   -69,     0,
      35,    36,    37,     0,    38,  -340,    39,     0,    40,    41,
      42,    43,     0,    44,    45,    46,    47,   -69,    48,    49,
       0,    50,    51,    52,     0,  -340,     0,     0,    53,    54,
      55,    56,    57,    58,    59,  -340,   -69,    60,    61,    62,
      63,     0,    64,    65,    66,     0,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,    81,    82,   -69,    83,    84,     0,     0,  -208,
       0,    85,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,    97,     0,     0,    98,     0,
      99,   565,   100,     0,  -208,     0,   582,   101,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -208,
      21,    22,    23,    24,    25,    26,     0,  -208,    27,     0,
      28,     0,    29,    30,    31,  -208,  -208,  -208,    32,    33,
      34,   -69,     0,    35,    36,    37,     0,    38,  -340,    39,
       0,    40,    41,    42,    43,     0,    44,    45,    46,    47,
     -69,    48,    49,     0,    50,    51,    52,     0,  -340,     0,
       0,    53,    54,    55,    56,    57,    58,    59,  -340,   -69,
      60,    61,    62,    63,     0,    64,    65,    66,     0,    67,
      68,    69,    70,    71,    72,    73,     0,    74,    75,     0,
      76,    77,    78,    79,    80,    81,    82,   -69,    83,    84,
       0,     0,  -208,     0,    85,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,    97,     0,
       0,    98,     0,    99,   583,   100,     0,  -208,     0,     4,
     101,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,  -208,    21,    22,    23,    24,    25,    26,     0,
    -208,    27,     0,    28,     0,    29,    30,    31,  -208,  -208,
    -208,    32,    33,    34,   -69,     0,    35,    36,    37,     0,
      38,  -340,    39,     0,    40,    41,    42,    43,     0,    44,
      45,    46,    47,   -69,    48,    49,     0,    50,    51,    52,
       0,  -340,     0,     0,    53,    54,    55,    56,    57,    58,
      59,  -340,   -69,    60,    61,    62,    63,     0,    64,    65,
      66,     0,    67,    68,    69,    70,    71,    72,    73,     0,
      74,    75,     0,    76,    77,    78,    79,    80,    81,    82,
     -69,    83,    84,     0,     0,  -208,     0,    85,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,    97,     0,     0,    98,     0,    99,   822,   100,     0,
    -208,     0,     4,   101,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -208,    21,    22,    23,    24,
      25,    26,     0,  -208,    27,     0,    28,     0,    29,    30,
      31,  -208,  -208,  -208,    32,    33,    34,   -69,     0,    35,
      36,    37,     0,    38,  -340,    39,     0,    40,    41,    42,
      43,     0,    44,    45,    46,    47,   -69,    48,    49,     0,
      50,    51,    52,     0,  -340,     0,     0,    53,    54,    55,
      56,   356,    58,    59,  -340,   -69,    60,    61,    62,    63,
       0,    64,    65,    66,     0,    67,    68,    69,    70,    71,
      72,    73,     0,    74,    75,     0,    76,    77,    78,    79,
      80,    81,    82,   -69,    83,    84,     0,     0,  -208,     0,
      85,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,    97,     0,     0,    98,     0,    99,
       0,   100,     0,  -208,     0,     4,   101,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -208,    21,
      22,    23,    24,    25,    26,     0,  -208,    27,     0,    28,
       0,    29,    30,    31,  -208,  -208,  -208,    32,    33,    34,
     -69,     0,    35,    36,    37,     0,    38,  -340,    39,     0,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -69,
      48,    49,     0,    50,    51,    52,     0,  -340,     0,     0,
      53,    54,    55,    56,    57,    58,    59,  -340,   -69,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,     0,    74,    75,     0,    76,
      77,    78,    79,    80,    81,    82,   -69,    83,    84,     0,
       0,  -208,     0,    85,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,    97,     0,     0,
      98,     0,    99,     0,   100,     0,  -208,     0,     4,   101,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,  -208,    21,    22,    23,    24,    25,    26,     0,  -208,
      27,     0,    28,     0,    29,    30,    31,  -208,  -208,  -208,
      32,    33,    34,   -69,     0,    35,    36,    37,     0,    38,
    -340,    39,     0,    40,    41,    42,    43,     0,    44,    45,
      46,    47,   -69,    48,    49,     0,    50,    51,    52,     0,
    -340,     0,     0,    53,    54,    55,    56,     0,    58,    59,
    -340,   -69,    60,    61,    62,    63,     0,    64,    65,    66,
       0,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,    81,    82,   -69,
      83,    84,     0,     0,  -208,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,     0,   100,     0,  -208,
       0,     4,   101,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -208,    21,    22,    23,    24,    25,
      26,     0,  -208,    27,     0,    28,     0,    29,    30,    31,
    -208,  -208,  -208,    32,  1043,    34,   -69,     0,    35,    36,
      37,     0,    38,  -340,    39,     0,    40,    41,    42,    43,
       0,    44,    45,    46,    47,   -69,    48,    49,     0,    50,
      51,    52,     0,  -340,     0,     0,    53,    54,    55,    56,
       0,    58,    59,  -340,   -69,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
      81,    82,   -69,    83,    84,     0,     0,  -208,     0,    85,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,    97,     0,     0,    98,     0,    99,     0,
     100,     0,  -208,     0,     4,   101,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,  -208,    21,    22,
      23,    24,    25,    26,     0,  -208,    27,     0,    28,     0,
      29,    30,    31,  -208,  -208,  -208,    32,  1198,    34,   -69,
       0,    35,    36,    37,     0,    38,  -340,    39,     0,    40,
      41,    42,    43,     0,    44,    45,    46,    47,   -69,    48,
      49,     0,    50,    51,    52,     0,  -340,     0,     0,    53,
      54,    55,    56,     0,    58,    59,  -340,   -69,    60,    61,
      62,    63,     0,    64,    65,    66,     0,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,    81,    82,   -69,    83,    84,     0,     0,
    -208,     0,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,    97,     0,     0,    98,
       0,    99,     0,   100,     0,  -208,     0,     4,   101,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -208,    21,    22,    23,    24,    25,    26,     0,  -208,    27,
       0,    28,     0,    29,    30,    31,  -208,  -208,  -208,    32,
    1202,    34,   -69,     0,    35,    36,    37,     0,    38,  -340,
      39,     0,    40,    41,    42,    43,     0,    44,    45,    46,
      47,   -69,    48,    49,     0,    50,    51,    52,     0,  -340,
       0,     0,    53,    54,    55,    56,     0,    58,    59,  -340,
     -69,    60,    61,    62,    63,     0,    64,    65,    66,     0,
      67,    68,    69,    70,    71,    72,    73,     0,    74,    75,
       0,    76,    77,    78,    79,    80,    81,    82,   -69,    83,
      84,     0,     0,  -208,     0,    85,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,    97,
       0,     0,    98,     0,    99,     0,   100,     0,  -208,     0,
       4,   101,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -208,    21,    22,    23,    24,    25,    26,
       0,  -208,    27,     0,    28,     0,    29,    30,    31,  -208,
    -208,  -208,    32,  1208,    34,   -69,     0,    35,    36,    37,
       0,    38,  -340,    39,     0,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -69,    48,    49,     0,    50,    51,
      52,     0,  -340,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -340,   -69,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
       0,    74,    75,     0,    76,    77,    78,    79,    80,    81,
      82,   -69,    83,    84,     0,     0,  -208,     0,    85,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,    97,     0,     0,    98,     0,    99,     0,   100,
       0,  -208,     0,     4,   101,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,  -208,    21,    22,    23,
      24,    25,    26,     0,  -208,    27,     0,    28,     0,    29,
      30,    31,  -208,  -208,  -208,    32,  1211,    34,   -69,     0,
      35,    36,    37,     0,    38,  -340,    39,     0,    40,    41,
      42,    43,     0,    44,    45,    46,    47,   -69,    48,    49,
       0,    50,    51,    52,     0,  -340,     0,     0,    53,    54,
      55,    56,     0,    58,    59,  -340,   -69,    60,    61,    62,
      63,     0,    64,    65,    66,     0,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,    81,    82,   -69,    83,    84,     0,     0,  -208,
       0,    85,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,    97,     0,     0,    98,     0,
      99,     0,   100,   740,  -208,     5,     6,   101,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   193,     0,  -208,    21,     0,   741,
       0,     0,     0,     0,     0,    27,     0,   194,     0,     0,
       0,    31,  -208,  -208,  -208,     0,   195,    34,     0,   742,
       0,    36,     0,   743,    38,     0,   196,     0,   197,     0,
       0,    43,     0,     0,    45,     0,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,   744,     0,    54,
     745,    56,     0,    58,     0,     0,     0,     0,    61,    62,
     198,     0,   746,     0,     0,     0,     0,     0,    69,    70,
      71,    72,   199,     0,    74,     0,     0,    76,     0,     0,
     747,    80,   200,    82,     0,   748,    84,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     201,     0,   100,   749,  -208,     0,   890,   101,     5,     6,
       0,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   193,     0,  -208,
      21,     0,     0,     0,     0,     0,     0,     0,   292,     0,
     194,     0,     0,     0,    31,  -208,  -208,  -208,     0,   195,
      34,     0,     0,     0,   294,     0,     0,    38,     0,   196,
       0,   197,     0,     0,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   198,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,   299,     0,    74,     0,     0,
      76,     0,     0,     0,    80,   200,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   201,     0,   100,     0,  -208,     5,     6,
     101,     8,     9,    10,    11,    12,     0,   636,     0,    13,
       0,     0,    16,    17,   638,    18,     0,   193,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     194,     0,     0,     0,    31,     0,     0,     0,     0,   195,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   196,
     644,   197,     0,     0,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   198,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,   199,     0,    74,     0,     0,
      76,     0,     0,     0,    80,   200,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    86,   510,   465,
     466,   467,   468,   469,   470,   471,   472,   473,   474,   475,
       0,   477,   478,     0,   833,   834,   835,   836,   837,   657,
       0,   658,     0,    92,     0,   659,   660,   661,   662,   663,
     664,   665,   838,   667,   668,    94,   839,    96,   -91,   670,
     671,   840,   673,   201,     0,   100,     0,     0,     5,     6,
     101,     8,     9,    10,    11,    12,     0,   636,     0,    13,
       0,     0,    16,    17,   638,    18,     0,   193,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     194,     0,     0,     0,    31,     0,     0,     0,     0,   195,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   196,
     644,   197,     0,     0,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   198,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,   199,     0,    74,     0,     0,
      76,     0,     0,     0,    80,   200,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    86,   510,   465,
     466,   467,   468,   469,   470,   471,   472,   473,   474,   475,
       0,   477,   478,     0,   833,   834,   835,   836,   837,   657,
       0,   658,     0,    92,     0,   659,   660,   661,   662,   663,
     664,   665,   838,   667,   668,    94,   839,    96,     0,   670,
     671,   840,   673,   201,     0,   100,     0,     0,     0,     0,
     101,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,     0,    16,    17,     0,    18,     0,
     193,    20,     0,    21,     0,     0,     0,     0,    26,     0,
       0,    27,     0,   194,     0,     0,     0,    31,     0,     0,
       0,     0,    33,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   196,     0,   197,     0,     0,    43,     0,    44,
      45,    46,    47,     0,    48,    49,     0,    50,    51,   331,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   198,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,    72,    73,     0,
      74,    75,     0,    76,     0,     0,     0,    80,   332,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,    99,     0,   100,     0,
       0,     5,     6,   101,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     193,     0,     0,    21,     0,   741,     0,     0,     0,     0,
       0,    27,     0,   194,     0,     0,     0,    31,     0,     0,
       0,     0,   195,    34,     0,   742,     0,    36,     0,   743,
      38,     0,   196,     0,   197,     0,     0,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,   744,     0,    54,   745,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   198,     0,   746,     0,
       0,     0,     0,     0,    69,    70,    71,    72,   199,     0,
      74,     0,     0,    76,     0,     0,   747,    80,   200,    82,
       0,   748,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   201,     0,   100,     0,
       0,     5,     6,   101,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     193,     0,     0,    21,   378,    23,     0,     0,     0,     0,
       0,    27,     0,   194,     0,     0,     0,    31,     0,     0,
       0,     0,   195,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   196,     0,   197,     0,     0,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,   361,
       0,     0,     0,     0,     0,    54,    55,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   198,     0,    64,     0,
       0,     0,     0,     0,    69,    70,    71,    72,   199,     0,
      74,     0,     0,    76,     0,     0,     0,    80,   200,    82,
       0,    83,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   201,     0,   100,     0,
       0,     5,     6,   101,     8,     9,    10,   381,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
      19,     0,  -208,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   194,     0,     0,     0,    31,  -208,  -208,
    -208,     0,   195,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   196,     0,   197,     0,     0,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,    63,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,   199,     0,
      74,     0,     0,    76,     0,     0,     0,    80,   382,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   201,     0,   100,     0,
    -208,     5,     6,   101,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     193,     0,  -208,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   194,     0,     0,     0,    31,  -208,  -208,
    -208,     0,   195,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   196,     0,   197,     0,     0,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   198,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,   199,     0,
      74,     0,     0,    76,     0,     0,     0,    80,   200,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   201,     0,   100,     0,
    -208,     5,     6,   101,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     193,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   194,     0,     0,     0,    31,     0,     0,
       0,     0,   195,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   196,     0,   197,     0,     0,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   198,  -748,     0,     0,
       0,  -748,     0,     0,    69,    70,    71,    72,   199,     0,
      74,     0,     0,    76,     0,     0,     0,    80,   200,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,  -382,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,  -382,     0,     0,    98,     0,   201,  -382,   100,     0,
       0,     5,     6,   101,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     193,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   194,     0,     0,     0,    31,     0,     0,
       0,     0,   195,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   196,     0,   197,     0,     0,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   198,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,   199,     0,
      74,     0,     0,    76,   314,   315,     0,    80,   200,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,   316,     0,     0,    98,     0,   201,     0,   100,     0,
       0,     5,     6,   101,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     193,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   194,     0,     0,     0,    31,     0,     0,
       0,     0,   195,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   196,     0,   197,     0,     0,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   198,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,   199,     0,
      74,     0,     0,    76,   314,   315,     0,    80,   200,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,   351,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   201,     0,   100,     0,
       0,     5,     6,   101,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     193,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   194,     0,     0,     0,    31,     0,     0,
       0,     0,   195,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   196,     0,   197,     0,     0,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   198,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,   199,     0,
      74,     0,     0,    76,   314,   315,     0,    80,   200,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   201,     0,   100,  -550,
       0,     5,     6,   101,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     193,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   194,     0,     0,     0,    31,     0,     0,
       0,     0,   195,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   196,     0,   197,     0,     0,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   198,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,   199,     0,
      74,     0,     0,    76,   314,   315,     0,    80,   200,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   201,     0,   100,     0,
       0,  -550,     0,   101,     5,     6,     0,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   193,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   194,     0,     0,     0,
      31,     0,     0,     0,     0,   195,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   196,     0,   197,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   198,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   199,     0,    74,     0,     0,    76,   314,   315,     0,
      80,   200,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   201,
       0,   100,   808,     0,     5,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   193,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   194,     0,     0,     0,
      31,     0,     0,     0,     0,   195,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   196,     0,   197,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   198,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   199,     0,    74,     0,     0,    76,   314,   315,     0,
      80,   200,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   201,
       0,   100,   811,     0,     5,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   193,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   194,     0,     0,     0,
      31,     0,     0,     0,     0,   195,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   196,     0,   197,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   198,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   199,     0,    74,     0,     0,    76,   314,   315,     0,
      80,   200,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   201,
       0,   100,   817,     0,     5,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   193,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   194,     0,     0,     0,
      31,     0,     0,     0,     0,   195,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   196,     0,   197,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,   956,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   198,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   199,     0,    74,     0,     0,    76,   314,   315,     0,
      80,   200,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   201,
       0,   100,     0,     0,     5,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   193,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   194,     0,     0,     0,
      31,     0,     0,     0,     0,   195,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   196,     0,   197,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   198,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   199,     0,    74,     0,     0,    76,     0,     0,     0,
      80,   200,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,  -390,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,  -390,     0,     0,    98,     0,   201,
    -390,   100,     0,     0,     5,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   193,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   194,     0,     0,     0,
      31,     0,     0,     0,     0,   195,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   196,     0,   197,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   198,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   199,     0,    74,     0,     0,    76,     0,     0,     0,
      80,   200,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,  -375,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,  -375,     0,     0,    98,     0,   201,
    -375,   100,     0,     0,     5,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   193,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   194,     0,     0,     0,
      31,     0,     0,     0,     0,   195,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   196,     0,   197,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   198,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   199,     0,    74,     0,     0,    76,     0,     0,     0,
      80,   200,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,  -383,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,  -383,     0,     0,    98,     0,   201,
    -383,   100,     0,     0,     5,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   193,     0,     0,    21,     0,   256,     0,
       0,     0,     0,     0,    27,     0,   194,     0,     0,     0,
      31,     0,     0,     0,     0,   195,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   196,     0,   197,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   198,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   199,     0,    74,     0,     0,    76,     0,     0,     0,
      80,   200,    82,     0,   257,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   201,
       0,   100,     0,     0,     5,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   193,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   194,     0,     0,     0,
      31,     0,     0,     0,     0,   195,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   196,     0,   197,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   198,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   199,     0,    74,     0,     0,    76,   314,   315,     0,
      80,   200,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   201,
       0,   100,     0,     0,     5,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   193,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   194,     0,     0,     0,
      31,     0,     0,     0,     0,   195,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   196,     0,   197,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   198,
    -748,     0,     0,     0,  -748,     0,     0,    69,    70,    71,
      72,   199,     0,    74,     0,     0,    76,     0,     0,     0,
      80,   200,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   201,
       0,   100,     0,     0,     5,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   193,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   194,     0,     0,     0,
      31,     0,     0,     0,     0,   195,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   196,     0,   197,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   198,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   199,     0,    74,     0,     0,    76,     0,     0,     0,
      80,   200,    82,     0,     0,    84,     0,     0,     0,     0,
       0,   604,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   201,
       0,   100,     0,     0,   605,     0,   101,     5,     6,     0,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   193,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   194,
       0,     0,     0,    31,     0,     0,     0,     0,   195,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   196,     0,
     197,     0,     0,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   198,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,   199,     0,    74,     0,     0,    76,
       0,     0,     0,    80,   200,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,   862,     0,     0,
      98,     0,   201,     0,   100,     0,     0,  1029,     0,   101,
       5,     6,     0,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   193,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   194,     0,     0,     0,    31,     0,     0,     0,
       0,   195,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   196,     0,   197,     0,     0,    43,     0,     0,    45,
       0,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,    62,   198,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,   199,     0,    74,
       0,     0,    76,     0,     0,     0,    80,   200,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,  -413,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,   201,     0,   100,  -413,     0,
       5,     6,   101,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   193,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   194,     0,     0,     0,    31,     0,     0,     0,
       0,   195,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   196,     0,   197,     0,     0,    43,     0,     0,    45,
       0,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,    62,   198,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,   199,     0,    74,
       0,     0,    76,     0,     0,     0,    80,   200,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
    -544,     0,     0,    98,     0,   201,     0,   100,     0,     0,
    -544,     0,   101,     5,     6,     0,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   193,     0,     0,    21,     0,     0,     0,     0,
       0,     0,   272,    27,     0,   194,     0,     0,     0,    31,
       0,     0,     0,     0,   195,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   196,     0,   197,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   198,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     199,     0,    74,     0,     0,    76,     0,     0,     0,    80,
     200,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,    99,     0,
     100,     0,     0,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   193,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   194,     0,     0,     0,    31,
       0,     0,     0,     0,   195,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   196,     0,   197,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   198,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     199,     0,    74,     0,     0,    76,     0,     0,     0,    80,
     200,    82,     0,     0,    84,     0,     0,     0,     0,     0,
     604,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   201,     0,
     100,     0,     0,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   193,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   194,     0,     0,     0,    31,
       0,     0,     0,     0,   195,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   196,     0,   197,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   198,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     199,     0,    74,     0,     0,    76,     0,     0,     0,    80,
     200,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   201,     0,
     100,     0,     0,   605,     0,   101,     5,     6,     0,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   193,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   194,     0,
       0,     0,    31,     0,     0,     0,     0,   195,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   196,     0,   197,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   198,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,   199,     0,    74,     0,     0,    76,     0,
       0,     0,    80,   200,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   201,   806,   100,     0,     0,     5,     6,   101,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   193,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   194,     0,
       0,     0,    31,     0,     0,     0,     0,   195,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   196,     0,   197,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   198,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,   199,     0,    74,     0,     0,    76,     0,
       0,     0,    80,   200,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   201,     0,   100,     0,     0,  1039,     0,   101,     5,
       6,     0,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   193,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   194,     0,     0,     0,    31,     0,     0,     0,     0,
     195,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     196,     0,   197,     0,     0,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   198,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,   199,     0,    74,     0,
       0,    76,     0,     0,     0,    80,   200,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   201,     0,   100,     0,     0,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   193,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,   292,
       0,   194,     0,     0,     0,    31,     0,     0,     0,     0,
     195,    34,     0,     0,     0,   294,     0,     0,    38,     0,
     196,     0,   197,     0,     0,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   198,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,   299,     0,    74,     0,
       0,    76,     0,     0,     0,    80,   200,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   201,     0,   100,     0,     0,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   193,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   194,     0,     0,     0,    31,     0,     0,     0,     0,
    1076,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     196,     0,   197,     0,     0,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   198,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,   199,     0,    74,     0,
       0,    76,     0,     0,     0,    80,   200,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   201,     0,   100,     0,     0,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   193,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   194,     0,     0,     0,    31,     0,     0,     0,     0,
    1231,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     196,     0,   197,     0,     0,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   198,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,   199,     0,    74,     0,
       0,    76,     0,     0,     0,    80,   200,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   201,     0,   100,     0,     0,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   193,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   194,     0,     0,     0,    31,     0,     0,     0,     0,
    1232,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     196,     0,   197,     0,     0,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   198,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,   199,     0,    74,     0,
       0,    76,     0,     0,     0,    80,   200,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   201,     0,   100,     0,     0,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   193,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   194,     0,     0,     0,    31,     0,     0,     0,     0,
    1234,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     196,     0,   197,     0,     0,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   198,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,   199,     0,    74,     0,
       0,    76,     0,     0,     0,    80,   200,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   201,     0,   100,     0,     0,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   193,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   194,     0,     0,     0,    31,     0,     0,     0,     0,
    1235,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     196,     0,   197,     0,     0,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   198,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,   199,     0,    74,     0,
       0,    76,     0,     0,     0,    80,   200,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     6,     0,     8,     9,    10,
      11,    12,     0,    93,     0,    13,    94,    95,    96,    17,
       0,     0,    98,   193,   201,  -208,   100,     0,   291,     0,
       0,   101,     0,     0,     0,     0,   194,     0,     0,     0,
       0,  -208,  -208,  -208,     0,   195,     0,     0,   293,     0,
       0,     0,   295,     0,     0,     0,     0,   197,     0,     0,
      43,     0,     0,     0,     0,    47,     0,    48,    49,     0,
       0,     0,     0,     0,     0,     0,   296,     0,    54,   297,
      56,    57,    58,     0,     0,     0,     0,    61,     0,   198,
       0,   298,     0,     0,     0,     0,     0,    69,    70,     0,
      72,     0,     0,     0,     0,     0,     0,     0,     0,   300,
       0,   200,    82,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   201,
       0,   100,  -344,  -208,     0,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,     0,    17,
       0,     0,     0,   193,     0,  -208,     0,     0,   291,     0,
       0,     0,     0,     0,     0,     0,   194,     0,     0,     0,
       0,  -208,  -208,  -208,     0,   195,     0,     0,   293,     0,
       0,     0,   295,     0,     0,     0,     0,   197,     0,     0,
      43,     0,     0,     0,     0,    47,     0,    48,    49,     0,
       0,     0,     0,     0,     0,     0,   296,     0,    54,   297,
      56,    57,    58,     0,     0,     0,     0,    61,     0,   198,
       0,   298,     0,     0,     0,     0,     0,    69,    70,     0,
      72,     0,     0,     0,     0,     0,     0,     0,     0,   300,
       0,   200,    82,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   634,     0,   635,     0,
      87,    88,    89,    90,    91,     0,     0,   636,     0,    92,
       0,     0,   637,   230,   638,   639,     0,     0,    93,     0,
     640,    94,    95,    96,     0,     0,     0,    98,   233,   201,
     194,   100,     0,  -208,   234,     0,   101,     0,     0,     0,
     641,     0,     0,     0,   236,     0,     0,   642,     0,   643,
     644,     0,     0,     0,     0,     0,     0,   645,     0,     0,
       0,     0,     0,     0,   240,   646,     0,     0,     0,     0,
       0,     0,   242,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   647,     0,     0,     0,     0,     0,     0,     0,
       0,   244,     0,   648,     0,   246,     0,   649,     0,     0,
     248,     0,     0,     0,   650,     0,   250,     0,     0,   651,
       0,     0,     0,     0,     0,     0,     0,     0,   510,   465,
     466,   467,   468,   469,   470,   471,   472,   473,   474,   475,
       0,   477,   478,     0,   652,   653,   654,   655,   656,   657,
       0,   658,     0,     0,     0,   659,   660,   661,   662,   663,
     664,   665,   666,   667,   668,     0,   669,     0,     0,   670,
     671,   672,   673,     5,     6,   674,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   193,     0,  -208,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   194,     0,     0,     0,    31,
    -208,  -208,  -208,     0,   195,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   196,     0,   197,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   198,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     199,     0,    74,     0,     0,    76,     0,     0,     0,    80,
     200,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,    95,    96,     0,     0,     0,    98,     0,   201,     0,
     100,     0,  -208,     5,     6,     0,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   193,     0,  -208,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   194,     0,     0,     0,    31,
    -208,  -208,  -208,     0,   195,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   196,     0,   197,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   198,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     199,     0,    74,     0,     0,    76,     0,     0,     0,    80,
     200,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
       0,    89,     0,     0,     5,     6,     0,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,    96,   193,     0,  -208,    21,     0,   201,     0,
     100,     0,  -208,     0,    27,     0,   194,     0,     0,     0,
      31,  -208,  -208,  -208,     0,   195,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   196,     0,   197,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   198,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   199,     0,    74,     0,   700,    76,     0,     0,     0,
      80,   200,    82,     0,   636,    84,     0,     0,     0,   991,
     992,   638,   993,     0,     0,     0,     0,   994,     0,     0,
       0,     0,     0,     0,     0,   702,     0,     0,     0,     0,
      87,     0,    89,     0,     0,     0,     0,   995,     0,     0,
       0,   996,     0,     0,   997,     0,   196,   644,     0,     0,
       0,     0,     0,    96,   703,     0,     0,     0,     0,     0,
       0,   100,   998,  -208,     0,     0,     0,     0,     0,   999,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1000,
       0,     0,     0,     0,     0,     0,     0,     0,  1001,     0,
    1002,     0,  1003,     0,   704,     0,     0,     0,     0,     0,
     705,  1004,     0,  1005,     0,     0,  1006,     0,     0,     0,
       0,     0,     0,     0,     0,   510,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475,     0,   477,   478,
       0,   652,   653,   654,   655,   656,   657,     0,   658,     0,
       0,     0,   659,   660,   661,   662,   663,   664,   665,   666,
     667,   668,   228,   669,     0,     0,   670,   671,   672,   673,
    1007,   636,     0,     0,     0,     0,   229,   230,   638,   231,
       0,     0,     0,     0,   232,     0,     0,     0,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,   235,     0,     0,     0,   236,     0,
       0,   237,     0,   238,   644,     0,     0,     0,     0,     0,
       0,   239,     0,     0,     0,     0,     0,     0,   240,   241,
       0,     0,     0,     0,     0,     0,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,     0,     0,
       0,     0,     0,     0,     0,   244,     0,   245,     0,   246,
       0,   247,     0,     0,   248,     0,     0,     0,   249,     0,
     250,     0,     0,   251,     0,     0,     0,     0,     0,     0,
       0,     0,   510,   465,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,     0,   477,   478,     0,   652,   653,
     654,   655,   656,   657,     0,   658,     0,     0,     0,   659,
     660,   661,   662,   663,   664,   665,   666,   667,   668,     7,
     669,     0,     0,   670,   671,   672,   673,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   358,     0,     0,     0,
      22,    23,     0,     0,     0,     0,     0,     0,     0,   359,
       0,    29,   360,     0,   434,     0,     0,    32,     0,     0,
       0,   435,     0,     0,    37,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   361,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,  1052,     0,     0,  1053,
       0,     0,   362,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    79,     0,   363,     0,     0,    83,     0,     0,
       0,   437,   434,     0,     0,   438,     0,     0,     0,   435,
       0,     0,     0,   509,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,   510,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475,    97,   477,   478,
       0,   440,   441,  1196,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,     0,   460,   461,   462,   437,
     364,     0,   434,   438,     0,     0,     0,   463,     0,   435,
       0,  1207,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   439,   510,   465,   466,   467,   468,   469,   470,
     471,   472,   473,   474,   475,     0,   477,   478,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,     0,     0,   460,   461,   462,     0,   364,   437,
     434,     0,     0,   438,     0,   463,     0,   435,     0,     0,
       0,  1269,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   439,   510,   465,   466,   467,   468,   469,   470,
     471,   472,   473,   474,   475,     0,   477,   478,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,     0,     0,   460,   461,   462,   437,   364,     0,
     434,   438,     0,     0,     0,   463,     0,   435,     0,  1270,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,   510,   465,   466,   467,   468,   469,   470,   471,   472,
     473,   474,   475,     0,   477,   478,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
       0,     0,   460,   461,   462,     0,   364,   437,   434,     0,
       0,   438,     0,   463,     0,   435,     0,     0,     0,  1271,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     439,   510,   465,   466,   467,   468,   469,   470,   471,   472,
     473,   474,   475,     0,   477,   478,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
       0,     0,   460,   461,   462,   437,   364,     0,     0,   438,
       0,     0,     0,   463,     0,     0,     0,  1272,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   439,   510,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,     0,   477,   478,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   434,     0,
     460,   461,   462,     0,   364,   435,     0,     0,     0,     0,
       0,   463,     0,     0,     0,     0,     0,   436,     0,     0,
       0,     0,   591,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   829,
       0,     0,     0,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,   592,     0,
       0,     0,   830,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,     0,     0,     0,   438,
       0,     0,     0,   434,     0,     0,     0,     0,     0,     0,
     435,     0,     0,     0,     0,     0,     0,     0,   831,     0,
       0,     0,   436,  1146,     0,     0,     0,     0,   439,     0,
       0,     0,     0,     0,     0,   437,     0,     0,     0,   438,
       0,     0,     0,     0,     0,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   439,   593,
     460,   461,   462,     0,     0,     0,     0,     0,     0,     0,
     437,   463,     0,     0,   438,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   434,   212,
     460,   461,   462,   439,     0,   435,     0,     0,     0,     0,
       0,   463,     0,     0,     0,     0,     0,   436,   272,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   434,     0,   460,   461,   462,     0,   364,
     435,     0,     0,     0,     0,     0,   463,     0,     0,     0,
       0,     0,   436,   272,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,     0,     0,     0,   438,
       0,     0,     0,   434,     0,     0,     0,     0,     0,     0,
     435,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   917,   212,     0,     0,     0,   439,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   918,     0,
     437,     0,     0,     0,   438,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,     0,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,     0,     0,
     460,   461,   462,   439,   364,     0,     0,     0,     0,     0,
     437,   463,     0,     0,   438,     0,     0,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   439,     0,   460,   461,   462,     0,   364,
       0,   434,     0,     0,     0,     0,   463,     0,   435,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     436,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,     0,     0,   460,   461,   462,     0,     0,
       0,   434,     0,     0,     0,     0,   463,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,  1120,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,     0,
       0,     0,   438,     0,     0,     0,   434,     0,     0,     0,
     949,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,  1121,     0,     0,     0,   436,     0,     0,     0,     0,
       0,   439,     0,     0,     0,     0,     0,     0,   437,     0,
       0,     0,   438,     0,     0,     0,     0,     0,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,     0,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   439,     0,   460,   461,   462,     0,   364,     0,     0,
       0,     0,     0,   437,   463,     0,     0,   438,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,     0,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   434,   212,   460,   461,   462,   439,     0,   435,     0,
       0,     0,     0,     0,   463,     0,     0,     0,     0,     0,
     436,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   434,   531,   460,   461,
     462,     0,     0,   435,     0,     0,     0,  1137,     0,   463,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,     0,
       0,     0,   438,     0,     0,     0,   434,     0,     0,     0,
       0,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,   212,     0,     0,
       0,   439,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,     0,     0,     0,   438,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,     0,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,     0,     0,   460,   461,   462,   439,     0,     0,     0,
       0,     0,  1190,   437,   463,     0,     0,   438,     0,     0,
       0,     0,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,     0,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   439,     0,   460,   461,
     462,     0,     0,     0,   434,     0,     0,     0,     0,   463,
       0,   435,     0,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   436,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,     0,     0,   460,   461,
     462,     0,   554,     0,   434,     0,     0,     0,     0,   463,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,     0,     0,   434,   438,     0,     0,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,     0,     0,     0,     0,     0,
       0,   437,     0,     0,     0,   438,     0,     0,     0,     0,
       0,   440,   441,   734,   442,   443,   444,     0,   445,   446,
     447,     0,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   439,   558,   460,   461,   462,     0,
       0,   437,   434,     0,     0,   438,     0,   463,     0,   435,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   436,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   439,     0,   460,   461,   462,     0,
       0,     0,   434,   825,     0,     0,     0,   463,     0,   435,
       0,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   436,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,     0,     0,   460,   461,   462,   437,
     805,     0,     0,   438,     0,     0,     0,   463,     0,     0,
     434,     0,     0,     0,     0,     0,     0,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,   439,     0,     0,     0,     0,     0,     0,   437,
       0,     0,     0,   438,     0,     0,     0,     0,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   439,     0,   460,   461,   462,     0,     0,     0,
       0,   807,     0,     0,     0,   463,     0,   437,     0,   440,
     441,   438,   442,   443,   444,     0,   445,   826,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,     0,     0,   460,   461,   462,   434,     0,   869,
     439,     0,     0,     0,   435,   463,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,   440,   441,   947,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
       0,     0,   460,   461,   462,   434,     0,     0,     0,     0,
       0,     0,   435,   463,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,     0,     0,     0,   438,     0,
       0,     0,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,   439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,     0,   440,   441,   438,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,     0,     0,     0,   439,     0,     0,     0,     0,
     463,     0,   437,     0,     0,     0,   438,     0,     0,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   439,     0,   460,   461,   462,
       0,     0,     0,     0,   959,     0,     0,     0,   463,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   434,  1016,   460,   461,   462,
       0,     0,   435,     0,   960,     0,     0,     0,   463,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,     0,     0,     0,   438,     0,     0,     0,
     434,  1119,     0,     0,     0,     0,     0,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,     0,     0,   439,     0,     0,     0,     0,
       0,     0,   437,     0,     0,     0,   438,     0,     0,     0,
       0,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   439,     0,   460,   461,   462,
       0,     0,     0,     0,     0,     0,     0,   437,   463,     0,
       0,   438,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   434,     0,   460,   461,   462,
     439,     0,   435,     0,  1068,     0,     0,     0,   463,     0,
       0,     0,     0,     0,   436,     0,     0,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,     0,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
       0,     0,   460,   461,   462,     0,     0,   434,     0,     0,
       0,     0,     0,   463,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,   437,     0,     0,     0,   438,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   434,     0,     0,
       0,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   439,   436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   440,   441,   437,   442,   443,   444,   438,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,     0,  1131,   460,   461,   462,
       0,     0,     0,     0,     0,     0,  1141,   439,   463,     0,
       0,     0,     0,     0,   437,     0,     0,     0,   438,     0,
       0,     0,     0,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   439,     0,   460,
     461,   462,     0,     0,     0,     0,     0,     0,     0,     0,
     463,     0,     0,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   434,     0,   460,
     461,   462,     0,     0,   435,     0,     0,     0,  1169,  -319,
     463,     0,     0,     0,     0,     0,   436,  1172,     0,     0,
       0,     0,     0,  -319,  -319,     0,  -319,     0,     0,     0,
       0,  -319,     0,     0,     0,     0,     0,   434,  1195,  -319,
       0,     0,     0,     0,   435,  -319,     0,     0,     0,     0,
       0,  -319,     0,     0,     0,  -319,   436,     0,  -319,     0,
    -319,     0,     0,     0,     0,     0,     0,     0,  -319,     0,
       0,     0,     0,     0,   437,  -319,  -319,     0,   438,     0,
       0,     0,   434,  -319,     0,     0,     0,     0,     0,   435,
       0,     0,     0,  -319,     0,     0,     0,     0,     0,     0,
     338,   436,  -319,     0,  -319,     0,  -319,   439,  -319,     0,
       0,  -319,     0,     0,   437,  -319,     0,  -319,   438,     0,
    -319,     0,     0,     0,   440,   441,     0,   442,   443,   444,
    1226,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   439,     0,   460,
     461,   462,     0,     0,     0,     0,     0,     0,     0,   437,
     463,     0,     0,   438,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,     0,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   434,     0,   460,
     461,   462,   439,     0,   435,     0,     0,     0,     0,     0,
     463,     0,     0,     0,     0,     0,   436,     0,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,     0,     0,   460,   461,   462,     0,     0,     0,
       0,     0,     0,     0,     0,   463,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,   434,     0,     0,   438,     0,
       0,     0,   435,     0,     0,     0,  1233,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   439,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,   435,     0,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   436,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,     0,   460,
     461,   462,   437,     0,     0,   434,   438,     0,     0,     0,
     463,     0,   435,     0,  1273,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   439,     0,     0,     0,     0,
       0,     0,   437,     0,     0,     0,   438,     0,     0,     0,
       0,     0,   440,   441,  1274,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   439,     0,   460,   461,   462,
       0,     0,   437,   434,     0,     0,   438,     0,   463,     0,
     435,     0,   440,   441,  1275,   442,   443,   444,     0,   445,
     446,   447,   436,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   439,     0,   460,   461,   462,
       0,     0,     0,   434,     0,     0,     0,     0,   463,     0,
     435,     0,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   436,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,     0,     0,   460,   461,   462,
     437,     0,     0,   434,   438,     0,     0,     0,   463,     0,
     435,     0,  1276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   439,     0,     0,     0,     0,     0,     0,
     437,     0,     0,     0,   438,     0,     0,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   439,     0,   460,   461,   462,     0,     0,
     937,     0,     0,     0,   438,     0,   463,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   439,     0,   460,   461,   462,     0,     0,
       0,     0,     0,     0,     0,     0,   463,     0,     0,     0,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,     7,     0,   460,   461,   462,     0,     0,
       0,     0,     0,     0,     0,     0,   463,     0,     0,     0,
     358,     0,     0,     0,    22,    23,     0,     0,     0,     0,
       0,     0,     0,   359,     0,    29,   360,     0,     0,     0,
       0,    32,     0,     0,     0,     0,     0,     0,    37,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   358,     0,     0,   361,
      22,    23,     0,     0,    53,     0,    55,     0,   356,   359,
    1052,    29,   360,  1053,     0,     0,   362,    32,    64,     0,
       0,     0,     0,     0,    37,     0,     0,  -340,     0,     0,
       0,    41,   436,     0,     0,     0,    79,     0,   363,   -69,
       0,    83,   437,     0,     0,   361,   438,  -340,     0,     0,
      53,     0,    55,     0,     0,     0,    59,  -340,   -69,    60,
       0,     0,   362,     0,    64,     0,     0,     0,     0,   436,
       0,     0,     0,     0,     0,   439,     0,     0,     0,     0,
       0,     0,    79,     0,   363,     0,     0,    83,     0,     0,
     437,    97,   440,   441,   438,   442,   443,   444,     0,   445,
     446,   447,     0,   448,   449,   450,   451,     0,   453,   454,
     455,   456,   457,   458,   459,     0,     0,   460,   461,   462,
       0,   436,     0,   439,     0,     0,     0,   437,   463,     0,
       0,   438,     0,     0,     0,     0,     0,    97,     0,     0,
     440,   441,   364,   442,   443,   444,     0,   445,   446,   447,
       0,   448,   449,   450,   451,     0,   453,   454,   455,   456,
     457,     0,   459,     0,     0,   460,   461,   462,     0,     0,
       0,     0,     0,     0,     0,     0,   463,   440,   441,   437,
     442,   443,   444,   438,   445,   446,   447,     0,   448,   449,
     450,   451,     0,   453,   454,   455,   456,   457,     0,   459,
       0,     0,   460,   461,   462,     0,     0,     0,     0,     0,
       0,     0,     0,   463,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,     0,
       0,   449,   450,   451,     0,   453,   454,   455,   456,     0,
       0,   459,     0,     0,   460,   461,   462,   228,     0,     7,
       0,     0,     0,     0,     0,   463,     0,     0,     0,     0,
       0,   229,   230,     0,   231,     0,     0,     0,     0,   232,
       0,     0,     0,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   234,     0,     0,     0,     0,     0,   235,
       0,     0,     0,   236,     0,     0,   237,     0,   238,     0,
       0,     0,     0,     0,     0,  -317,   239,     0,     0,     0,
       0,     0,     0,   240,   241,     0,     0,     0,     0,  -317,
    -317,   242,  -317,     0,     0,     0,     0,  -317,     0,     0,
       0,   243,     0,     0,     0,  -317,     0,     0,     0,     0,
     244,  -317,   245,     0,   246,     0,   247,  -317,     0,   248,
       0,  -317,     0,   249,  -317,   250,  -317,     0,   251,     0,
       0,     0,     0,   228,  -317,     0,     0,     0,     0,     0,
       0,  -317,  -317,     0,     0,     0,     0,   229,   230,  -317,
     231,     0,     0,     0,     0,   232,     0,     0,     0,  -317,
       0,     0,     0,   233,     0,     0,     0,     0,  -317,   234,
    -317,     0,  -317,     0,  -317,   235,     0,  -317,     0,   236,
       0,  -317,   237,  -317,   238,     0,  -317,     0,     0,     0,
       0,  -318,   239,     0,     0,     0,     0,     0,     0,   240,
     241,     0,     0,     0,     0,  -318,  -318,   242,  -318,     0,
       0,     0,     0,  -318,     0,     0,     0,   243,     0,     0,
       0,  -318,     0,     0,     0,     0,   244,  -318,   245,     0,
     246,     0,   247,  -318,     0,   248,     0,  -318,     0,   249,
    -318,   250,  -318,     0,   251,     0,     0,     0,     0,  -319,
    -318,     0,     0,     0,     0,     0,     0,  -318,  -318,     0,
       0,     0,     0,  -319,  -319,  -318,  -319,     0,     0,     0,
       0,  -319,     0,     0,     0,  -318,     0,     0,     0,  -319,
       0,     0,     0,     0,  -318,  -319,  -318,     0,  -318,     0,
    -318,  -319,     0,  -318,     0,  -319,     0,  -318,  -319,  -318,
    -319,     0,  -318,     0,     0,     0,     0,     0,  -319,     0,
       0,     0,     0,     0,     0,  -319,  -319,     0,     0,     0,
       0,     0,     0,  -319,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -319,     0,     0,     0,     0,     0,     0,
       0,     0,  -319,     0,  -319,     0,  -319,     0,  -319,     0,
       0,  -319,     0,     0,     0,  -319,     0,  -319,     0,     0,
    -319
};

static const yytype_int16 yycheck[] =
{
       2,   263,   552,   109,    61,     2,   179,     2,   622,   163,
      26,   403,    61,   492,    66,   591,   425,   516,   580,   227,
     677,   221,   679,    25,   629,   793,   288,   831,   896,   130,
      25,   609,   884,    85,   547,   383,   826,   787,     1,     1,
     962,     1,   773,   849,    32,    24,   393,    32,   100,    65,
      24,    56,     5,   202,     1,   263,    22,   523,    32,    32,
      32,    40,    41,    42,    32,   327,    40,    41,    42,     3,
      56,    73,   101,   116,    74,    77,    78,     3,    73,    35,
     288,   145,    77,    78,   518,    60,   259,    32,   123,    32,
     437,   438,   145,    32,   118,   145,   163,    99,   145,     1,
       3,   103,   145,    47,    99,     0,   173,   171,   103,   118,
      56,   545,    74,    32,   149,   115,    47,   163,   171,   327,
      54,    56,    88,   173,   171,   606,   607,    74,    54,    47,
      47,   612,   613,   121,    47,     3,   483,   484,     2,   168,
      24,   116,   489,   117,   110,    47,    90,     1,    32,   173,
     118,    54,   108,   115,    24,    56,    40,    41,    42,    90,
     123,    25,    32,   629,   173,   170,    47,  1019,   115,   169,
      40,    41,    42,   145,    90,   118,   102,  1099,    32,   118,
     168,   144,   145,   168,   170,   118,    54,   955,    47,    92,
     163,   118,   145,   172,   962,   168,   168,   759,   172,   102,
     168,   118,   108,  1055,  1056,   936,    60,   169,   171,    73,
     170,   213,  1018,    77,    78,   965,   169,   366,   213,   481,
     482,   123,   169,   168,  1014,   168,   145,   145,   145,   168,
     577,    90,   145,   117,   102,    99,   735,   118,   446,   103,
     173,    46,   144,   145,    74,   163,   594,   117,    31,   168,
     104,    24,   425,    46,   830,   173,   173,    44,    47,    32,
      65,   163,   116,   145,   145,   267,   163,    40,    41,    42,
     272,    56,   267,   481,   482,  1127,   173,   272,   791,    84,
     381,    68,   163,   861,    46,   115,   163,    18,   172,   171,
     144,    84,   173,   170,   556,    53,  1164,    50,  1150,   170,
      53,    90,   172,    65,   145,   145,    89,   112,   516,   163,
      93,   145,   145,  1081,   168,    73,   163,  1121,    56,   112,
      73,    11,    84,    76,    27,    83,   173,   145,   169,   169,
      83,  1099,   813,   814,   815,   535,   169,   171,    31,   169,
      56,   145,   496,     3,   117,   170,    77,   349,   556,   213,
     112,   145,   349,   171,   349,   108,   139,    56,   963,     2,
      20,   144,   364,   145,   147,    96,    27,   171,   151,   364,
     145,   373,   580,    33,   102,    78,   392,   171,   373,   145,
     111,   145,    25,   882,   400,   387,    47,    90,   163,   171,
      51,   145,   387,   139,    54,   145,    89,   145,   598,   172,
      93,   147,    62,   267,   145,   171,   145,   171,   272,   163,
     113,   145,  1069,  1070,    75,   163,    27,    78,    56,    80,
     919,   171,   163,   425,   476,   987,    56,   479,   480,    90,
      73,  1007,   171,    56,    77,    78,    47,   171,     9,   145,
      51,   145,   102,   145,   923,  1059,   139,   108,   108,   163,
     145,   144,   113,   146,   147,   169,    99,   163,   151,   163,
     103,   163,   145,   145,    75,   158,   145,    78,   163,    80,
      89,   163,   488,   166,    93,   145,   145,   169,    99,    90,
     163,   163,    77,    78,   163,   349,   163,   544,   275,  1157,
     277,   900,   884,   163,   163,   544,   173,   108,    82,   170,
     364,    85,   113,     5,    89,   507,    56,   509,    93,   373,
     171,    89,   507,   860,   509,    93,   622,    89,   570,   571,
      22,    93,    89,   387,    26,    27,    93,   735,    89,   876,
      77,    78,    93,    35,    56,    37,    38,   324,    31,   170,
    1154,    43,    56,   168,  1120,   170,  1027,  1028,    50,   289,
     290,   759,  1220,  1221,  1222,  1223,  1224,  1225,   570,   571,
     576,   170,    56,   172,  1045,  1046,   170,   914,   172,    71,
     213,   918,   123,   170,    76,   172,    78,   170,    80,   172,
      82,    56,   584,    85,    56,     5,    88,   584,    90,   584,
     937,   938,   608,   170,   147,   172,    89,    56,   168,   150,
      93,   152,   153,   144,   155,   156,   108,   139,   110,   168,
     162,   113,   144,  1163,   170,   147,   172,   825,   826,   151,
     163,   163,    56,   163,   267,     2,   163,  1019,   163,   272,
     163,   785,  1113,  1114,  1115,  1116,   163,   984,   163,   986,
      31,    84,   168,   507,   163,   509,   139,   140,    25,   142,
     143,   144,   163,   146,   147,   170,   145,   170,   151,   171,
     170,   163,   144,  1055,   163,   158,   170,   169,  1060,    94,
     171,   164,   165,   166,   882,    31,   163,   145,     9,     3,
     170,    65,    78,   168,   123,   163,   147,    68,   123,   170,
     145,   171,   123,   123,    34,    27,    73,   851,    89,    34,
      77,    78,    93,    65,   171,   757,   349,   123,   171,   171,
      21,   919,   144,   117,   168,    47,   145,   170,   169,    51,
     584,   364,    99,   123,   776,   104,   103,   900,   170,   147,
     373,   171,   147,    89,   147,  1127,   147,    93,   147,   147,
     147,   147,   147,    75,   387,   147,    78,   139,   139,   140,
     752,   142,   143,   144,   147,   146,   147,   147,    90,   147,
     151,   763,   147,   173,   766,   147,   157,   158,   763,    27,
     161,   766,    32,   164,   165,   166,   108,    12,    34,   987,
     163,   113,   171,   139,    19,   163,    34,   163,   144,    47,
     146,   147,   163,    51,   796,   151,    31,    32,   170,   796,
     144,   796,   158,   169,   171,   163,   145,   168,   164,   165,
     166,    47,    47,   173,   173,   602,   603,    75,   145,   169,
      78,   173,    80,   171,    34,   612,   613,   884,   170,   163,
    1092,  1093,    90,   163,   170,   144,   213,   163,   145,   896,
     856,   169,   171,    32,    34,   163,   169,   163,   173,   171,
     108,   145,   173,   173,    89,   113,   858,   171,    93,   171,
     162,   863,   162,   858,   507,   108,   509,   579,   863,   987,
     872,   823,    24,   875,   366,   262,   373,   872,   879,   337,
     875,   978,   391,   118,  1092,  1093,   544,   122,   529,   385,
     267,  1164,   900,   543,   964,   272,  1163,  1226,  1219,   763,
     850,   302,   766,   683,   139,   140,   128,   142,   143,   144,
     526,   146,   147,   148,   531,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   772,    27,   164,
     165,   166,   796,   168,    20,   937,   938,    -1,    -1,   923,
     175,   584,   944,    -1,   946,    -1,    -1,   949,    47,   944,
    1056,   946,    51,    -1,   949,    -1,    -1,    -1,   974,    -1,
      -1,     2,  1019,    -1,    -1,    -1,    -1,    -1,    31,  1142,
      -1,   973,   349,    -1,    -1,    -1,    75,    -1,   973,    78,
      -1,    -1,    -1,   985,    25,    -1,    -1,   364,    -1,    -1,
     985,    90,    -1,    -1,   858,    -1,   373,    -1,    -1,   863,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   872,   108,
     387,   875,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1026,    -1,    89,   814,   815,    -1,
      93,  1026,    73,  1035,    -1,    -1,    77,    78,    -1,    -1,
    1035,    -1,    -1,    -1,  1150,    -1,    -1,    -1,  1154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
      -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1127,    -1,    -1,  1246,    -1,  1248,   139,   140,  1251,  1252,
     944,   144,   946,   146,   147,   949,  1088,   874,   151,    -1,
      -1,   878,    -1,  1088,    -1,   158,    -1,    -1,    -1,    -1,
      -1,   164,   165,   166,    -1,    -1,  1108,  1164,    -1,   973,
      -1,    -1,    -1,  1108,    -1,    -1,    -1,    -1,    -1,    -1,
     763,   985,    -1,   766,    -1,    -1,    -1,    -1,    -1,    -1,
     507,    -1,   509,  1135,    -1,  1137,    -1,    -1,    -1,    -1,
    1135,  1143,  1137,    -1,  1146,    -1,     2,    -1,  1143,    -1,
      -1,  1146,    -1,   796,    -1,    -1,    -1,    -1,    -1,     2,
      -1,    -1,  1026,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,  1035,   213,  1175,    -1,    -1,    -1,    -1,    -1,  1181,
    1175,    -1,    25,    -1,    -1,    -1,  1181,  1189,  1190,    -1,
      -1,    -1,    -1,    -1,  1189,  1190,    -1,    -1,    -1,  1201,
      -1,    -1,    -1,  1205,    -1,  1207,  1201,   584,    -1,    -1,
    1205,    -1,  1207,    -1,    -1,   858,    -1,    73,    -1,    -1,
     863,    77,    78,    -1,  1088,    -1,   267,    -1,    -1,   872,
      73,   272,   875,    -1,    77,    78,    -1,    -1,    -1,  1241,
      -1,    -1,    -1,    99,  1108,    -1,  1241,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,  1045,  1046,
     103,    -1,    -1,    -1,    -1,    -1,    -1,  1269,  1270,  1271,
    1272,  1135,    -1,  1137,  1269,  1270,  1271,  1272,    -1,  1143,
      -1,    -1,  1146,     1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,   944,    20,   946,    -1,    31,   949,    25,   349,    -1,
      -1,  1175,    -1,    -1,    -1,    33,  1103,  1181,    -1,    -1,
      -1,    39,    -1,   364,    -1,  1189,  1190,    45,  1115,  1116,
     973,    49,   373,    -1,    52,    -1,    54,  1201,    -1,    -1,
      -1,  1205,   985,  1207,    62,  1132,   387,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,   213,  1145,    77,
      -1,  1148,    -1,    89,    -1,    -1,    -1,    93,    -1,    87,
     213,    -1,    -1,    -1,    -1,    -1,    -1,  1241,    96,    -1,
      98,    -1,   100,  1026,   102,    -1,   763,   105,    -1,   766,
      -1,   109,  1035,   111,    -1,    -1,   114,    -1,    -1,    -1,
    1187,    -1,    -1,    -1,    -1,  1269,  1270,  1271,  1272,    -1,
      -1,   267,    -1,   139,   140,    -1,   272,   143,   144,   796,
     146,   147,    -1,    -1,   267,   151,    -1,    -1,    -1,   272,
      -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,   164,   165,
     166,    -1,    -1,    -1,    -1,  1088,    -1,    -1,    -1,    -1,
      -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1108,   507,    -1,   509,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,   858,    35,    -1,    37,    38,   863,    -1,    -1,    -1,
      43,    -1,  1135,   349,  1137,   872,    -1,    50,   875,    -1,
    1143,    -1,    -1,  1146,    -1,     2,   349,    -1,   364,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   373,    71,    -1,
      -1,   364,    -1,    76,    -1,    78,    -1,    -1,    25,    -1,
     373,   387,  1175,    -1,    -1,    88,    -1,    90,  1181,    -1,
      -1,    -1,    -1,   584,   387,    -1,  1189,  1190,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,   110,  1201,    -1,
     113,    -1,  1205,    -1,  1207,    -1,    -1,   944,    -1,   946,
      -1,    -1,   949,    -1,    -1,    -1,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,   973,     5,  1241,    -1,
      -1,    -1,    99,    -1,    -1,    -1,   103,    -1,   985,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,  1269,  1270,  1271,  1272,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,  1026,
      -1,   507,    -1,   509,    62,    -1,    -1,    -1,  1035,    -1,
      -1,    69,    70,    -1,   507,    -1,   509,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      98,     5,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      66,   109,    -1,   111,    -1,    -1,   114,    -1,    22,    -1,
      -1,  1088,    26,    27,    -1,    -1,   213,    -1,    -1,    85,
      -1,    35,   763,    37,    38,   766,    -1,    -1,   584,    43,
      -1,  1108,    -1,    89,   100,    -1,    50,    93,    -1,    -1,
      -1,   584,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   796,    -1,    71,  1135,    -1,
    1137,   169,    76,    -1,    78,    -1,  1143,    -1,    82,  1146,
     267,    85,    -1,    -1,    88,   272,    90,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   108,   151,   110,    -1,  1175,   113,
      -1,   157,   158,    -1,  1181,   161,    -1,    -1,   164,   165,
     166,    -1,  1189,  1190,    -1,    -1,    -1,   858,    -1,   175,
      -1,    -1,   863,    -1,  1201,    -1,    -1,    -1,  1205,    -1,
    1207,   872,    -1,    -1,   875,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
      -1,    -1,   349,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   227,    -1,    -1,  1241,    -1,    -1,   364,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,   373,    -1,    -1,     5,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     387,    -1,  1269,  1270,  1271,  1272,    22,   263,    -1,    -1,
      26,    27,    -1,   944,    -1,   946,    -1,   763,   949,    35,
     766,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
     763,    -1,   288,   766,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,   973,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     796,    -1,    -1,    -1,   985,    71,    -1,    73,    -1,    -1,
      76,    -1,    78,   796,    80,    -1,    82,    83,    -1,    85,
      -1,   327,    88,    -1,    90,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,   110,  1026,    -1,   113,    -1,    -1,
      68,    -1,    -1,    -1,  1035,    -1,    -1,    -1,    -1,    77,
      78,    -1,   858,    -1,    -1,    -1,    -1,   863,    -1,    -1,
     507,    -1,   509,    -1,    -1,   858,   872,    -1,    -1,   875,
     863,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   872,
     108,    -1,   875,    -1,    -1,    -1,    -1,   163,    -1,    -1,
       3,    -1,    -1,   169,    -1,    -1,    -1,  1088,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    20,   136,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,  1108,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    -1,   584,   944,    52,
     946,    54,    -1,   949,  1135,    -1,  1137,    -1,    -1,    62,
      -1,   944,  1143,   946,    -1,  1146,   949,    70,    -1,    -1,
     476,    -1,    -1,   479,   480,   481,   482,   973,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,   985,
     973,    -1,    -1,    -1,  1175,    98,    -1,    -1,    -1,   102,
    1181,   219,   985,    -1,    -1,    -1,   109,    -1,  1189,  1190,
     516,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1201,    -1,    -1,    -1,  1205,    -1,  1207,    -1,    -1,    -1,
    1026,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1035,
     258,   259,    -1,  1026,    -1,    -1,    -1,    -1,    -1,    -1,
     556,    -1,  1035,    -1,    -1,    -1,    -1,   275,    -1,   277,
    1241,    -1,    -1,    -1,   570,   571,   284,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   580,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1269,  1270,
    1271,  1272,  1088,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,    -1,    -1,  1088,   324,    -1,    -1,    -1,
      -1,    -1,  1108,    -1,    -1,    -1,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,  1108,   763,    -1,    -1,   766,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,  1135,
      -1,  1137,    -1,    -1,    45,    -1,    -1,  1143,    49,    -1,
    1146,    52,  1135,    54,  1137,    -1,    -1,    -1,    -1,   796,
    1143,    62,    -1,  1146,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,  1175,
      -1,    -1,    -1,    -1,    -1,  1181,    87,    -1,    -1,    -1,
      -1,    -1,  1175,  1189,  1190,    96,    -1,    98,  1181,   100,
      -1,   102,    -1,    -1,   105,  1201,  1189,  1190,   109,  1205,
     111,  1207,    -1,   114,    -1,    -1,    -1,    -1,  1201,    -1,
      -1,   858,  1205,    -1,  1207,    -1,   863,    -1,    -1,   735,
      -1,    -1,    -1,    -1,    -1,   872,    -1,    -1,   875,    -1,
      -1,    -1,    -1,    -1,    -1,  1241,    -1,    -1,    -1,    -1,
      -1,   757,    -1,   759,    -1,    -1,    -1,    -1,  1241,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,   170,
     776,    -1,    -1,  1269,  1270,  1271,  1272,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1269,  1270,  1271,  1272,
      -1,    13,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,    -1,   944,    -1,   946,
      -1,    33,   949,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    46,    47,    -1,    -1,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,   973,    -1,    -1,    61,
      -1,   559,    -1,    65,    66,    67,    68,    69,   985,    -1,
      -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    -1,    -1,    85,    -1,    87,    -1,    89,    -1,    -1,
      92,    93,    -1,    95,    -1,    -1,   882,    99,   100,   101,
      -1,    25,    -1,    -1,   602,   603,    -1,    -1,    -1,  1026,
      -1,    -1,    -1,    -1,   612,   613,    -1,    -1,  1035,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,   130,     5,
      -1,    -1,    -1,   919,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    73,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
     172,  1088,    -1,    -1,    50,    -1,    -1,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1108,    -1,   195,    -1,    71,    -1,   199,    -1,   201,
      76,   987,    78,    -1,    80,    -1,    82,    -1,    -1,    85,
      -1,    -1,    88,    -1,    90,    -1,    -1,    -1,  1135,    -1,
    1137,    -1,    -1,    -1,    -1,   227,  1143,    -1,    -1,  1146,
      -1,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1175,    -1,
      -1,   263,    -1,    -1,  1181,    -1,    -1,    -1,    -1,   767,
      -1,    -1,  1189,  1190,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1201,    -1,   288,   163,  1205,   213,
    1207,    -1,    -1,   169,    -1,    -1,    -1,   299,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   803,  1092,  1093,    -1,    -1,
      -1,    -1,   314,   315,    -1,    -1,   814,   815,    -1,    -1,
       3,    -1,    -1,    -1,  1241,   327,    -1,    -1,    -1,   331,
      -1,    -1,    -1,    -1,    17,    18,   338,    20,    -1,    -1,
      -1,    -1,    25,   267,    27,    -1,    -1,    -1,   272,   351,
      33,    -1,  1269,  1270,  1271,  1272,    39,    -1,    -1,   361,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,   374,    -1,   376,    -1,   874,    -1,    -1,    62,
     878,    -1,   880,    -1,    -1,    -1,    69,    70,    -1,   391,
     392,   393,    -1,    -1,    77,    78,    -1,    -1,   400,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    98,    -1,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,   111,    -1,
     113,   114,   434,   435,   436,   437,   438,   439,    -1,   441,
     442,   443,   444,   445,    -1,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,    -1,   387,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   476,    -1,    -1,   479,   480,   481,
     482,   483,   484,   981,    -1,    -1,   488,   489,    -1,    -1,
     492,    -1,    -1,    -1,    -1,    -1,   498,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   509,    -1,    -1,
     512,    -1,    -1,    -1,   516,    -1,    -1,    -1,    -1,    -1,
      -1,   523,    -1,   525,    -1,    -1,    -1,   529,    -1,    -1,
     532,   533,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   544,    -1,    -1,   547,    -1,  1045,  1046,    -1,
      -1,    -1,    -1,    -1,   556,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   567,    -1,    -1,   570,   571,
      -1,    -1,    -1,    -1,   576,   577,    -1,    -1,   580,    -1,
      -1,    -1,    -1,   507,    -1,   509,    -1,    -1,    -1,   591,
     592,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   605,    -1,  1103,   608,    40,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,     3,  1115,  1116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   629,    -1,    -1,
      17,    18,    -1,    20,  1132,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,  1142,    -1,    33,  1145,    -1,    -1,
    1148,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,
      -1,    -1,   674,    -1,    -1,    62,    -1,    -1,    -1,   681,
      -1,    -1,    69,    70,    -1,  1183,    -1,    -1,    -1,  1187,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    98,    -1,   100,    -1,   102,   149,    -1,   105,    -1,
      -1,    -1,   109,    -1,   111,    -1,    -1,   114,   161,    -1,
      -1,    -1,   734,   735,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1246,    -1,
    1248,    -1,    -1,  1251,  1252,   757,    -1,   759,   760,   761,
      -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   776,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,   791,
      -1,    -1,    -1,   795,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   823,    -1,   256,   257,   827,    -1,   829,   830,   831,
      -1,   833,    -1,   835,    -1,   268,   838,   839,   167,   763,
      -1,   170,   766,   172,    -1,    -1,    -1,    -1,    -1,    -1,
     852,    -1,    -1,    -1,   856,    -1,    -1,    -1,   860,   861,
      -1,   863,    -1,    -1,    -1,    -1,    -1,   869,   870,    -1,
     872,    -1,    -1,   875,   876,   308,    -1,    -1,    -1,    -1,
     882,    -1,    -1,    -1,    -1,   887,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   914,   915,   916,   917,   918,   919,    -1,    -1,
      -1,   923,    -1,    -1,    -1,    -1,   359,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   858,   937,   938,    -1,    -1,   863,
      -1,    -1,    -1,    -1,    -1,   947,    -1,    -1,   872,    -1,
      -1,   875,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   963,    -1,    -1,   397,   967,   399,    -1,    -1,    -1,
     403,    -1,   974,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   984,    -1,   986,   987,    -1,    -1,    -1,    -1,
      -1,    -1,   425,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1007,    -1,    -1,    -1,    -1,
      -1,    -1,  1014,    -1,  1016,    -1,    -1,    -1,    -1,    -1,
     944,  1023,   946,    -1,    -1,   949,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1036,    -1,    -1,    -1,    -1,    -1,
      -1,  1043,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   973,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   490,    -1,    -1,
    1062,   985,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1076,    -1,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,   518,    -1,    -1,    -1,    -1,
    1092,  1093,    -1,    17,    18,    -1,    20,    -1,   531,    -1,
      -1,    25,  1026,  1105,    -1,    -1,    -1,  1109,   541,    33,
      -1,  1035,   545,    -1,    -1,    39,    -1,  1119,  1120,  1121,
      -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    -1,  1135,    -1,  1137,    -1,    -1,    62,  1141,
      -1,  1143,    -1,    -1,  1146,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,   590,    -1,    -1,
      -1,    -1,    -1,    87,  1088,    -1,  1168,  1169,   601,  1171,
    1172,    -1,    96,    -1,    98,    -1,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,  1108,   109,    -1,   111,    -1,   622,
     114,    -1,    -1,  1195,    -1,    -1,  1198,    -1,    -1,    -1,
    1202,    -1,    -1,    -1,    -1,  1207,  1208,    -1,    -1,  1211,
      -1,  1135,    -1,  1137,    -1,    -1,  1218,    -1,    -1,  1143,
      -1,    -1,  1146,    -1,    -1,    -1,    -1,    -1,    -1,  1231,
    1232,  1233,  1234,  1235,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,
     683,  1175,    -1,    -1,    -1,    -1,    -1,  1181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1189,  1190,  1269,  1270,  1271,
    1272,  1273,  1274,  1275,  1276,    -1,    -1,  1201,    -1,    -1,
      -1,  1205,    -1,  1207,    12,     3,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    31,    32,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,  1241,    -1,    47,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,   768,    54,    -1,    -1,   772,
      -1,    -1,    -1,    -1,    62,  1269,  1270,  1271,  1272,   782,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    87,
     803,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
     118,   109,    -1,   111,   122,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,   850,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    -1,
     168,    -1,    -1,    -1,    -1,    -1,   879,   175,    -1,    -1,
      -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   900,    -1,    -1,
       0,     1,    -1,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    54,    -1,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,   978,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,    -1,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1059,   157,    -1,    -1,
     160,   161,   162,   163,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,    -1,    -1,   175,    -1,     1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    19,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,  1154,    76,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,    -1,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,   168,   169,   170,    -1,   172,    -1,
       1,   175,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
      -1,   102,   103,    -1,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    -1,    -1,   117,    -1,   119,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,   163,    -1,    -1,   166,    -1,   168,   169,   170,
      -1,   172,    -1,     1,   175,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    54,    -1,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,    -1,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    -1,    -1,   117,
      -1,   119,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,
     168,   169,   170,    -1,   172,    -1,     1,   175,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    54,
      -1,    56,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    94,
      95,    96,    97,    98,    99,   100,    -1,   102,   103,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      -1,    -1,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,    -1,
      -1,   166,    -1,   168,   169,   170,    -1,   172,    -1,     1,
     175,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    54,    -1,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,    -1,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,   117,    -1,   119,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,   163,    -1,    -1,   166,    -1,   168,   169,   170,    -1,
     172,    -1,     1,   175,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    54,    -1,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,    -1,   102,   103,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    -1,    -1,   117,    -1,
     119,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,   172,    -1,     1,   175,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,    -1,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    -1,
      -1,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,   163,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,   172,    -1,     1,   175,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    54,    -1,    56,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,    -1,   102,
     103,    -1,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    -1,    -1,   117,    -1,   119,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
     163,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,
      -1,     1,   175,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    54,    -1,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,    -1,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,   163,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,    -1,     1,   175,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    54,    -1,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,    -1,   102,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    -1,    -1,
     117,    -1,   119,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,   172,    -1,     1,   175,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,    -1,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,
       1,   175,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    54,    -1,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
      -1,   102,   103,    -1,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    -1,    -1,   117,    -1,   119,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,   163,    -1,    -1,   166,    -1,   168,    -1,   170,
      -1,   172,    -1,     1,   175,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    49,    50,    -1,    52,    53,    54,    -1,    56,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,    -1,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    -1,    -1,   117,
      -1,   119,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,
     168,    -1,   170,     1,   172,     3,     4,   175,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    24,    25,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    42,    -1,    44,    45,    -1,    47,
      -1,    49,    -1,    51,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    77,
      78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
     108,   109,   110,   111,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,   171,   172,    -1,     1,   175,     3,     4,
      -1,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    24,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    42,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,   110,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,   172,     3,     4,
     175,     6,     7,     8,     9,    10,    -1,    12,    -1,    14,
      -1,    -1,    17,    18,    19,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      55,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,   110,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,   136,   137,    -1,   139,   140,   141,   142,   143,   144,
      -1,   146,    -1,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,    -1,   170,    -1,    -1,     3,     4,
     175,     6,     7,     8,     9,    10,    -1,    12,    -1,    14,
      -1,    -1,    17,    18,    19,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      55,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,   110,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,   136,   137,    -1,   139,   140,   141,   142,   143,   144,
      -1,   146,    -1,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    -1,   164,
     165,   166,   167,   168,    -1,   170,    -1,    -1,    -1,    -1,
     175,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    -1,    17,    18,    -1,    20,    -1,
      22,    23,    -1,    25,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,    61,
      62,    63,    64,    -1,    66,    67,    -1,    69,    70,    71,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,   100,    -1,
     102,   103,    -1,   105,    -1,    -1,    -1,   109,   110,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
      -1,     3,     4,   175,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    47,    -1,    49,    -1,    51,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    77,    78,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,   108,   109,   110,   111,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
      -1,     3,     4,   175,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    71,
      -1,    -1,    -1,    -1,    -1,    77,    78,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,   110,   111,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
      -1,     3,     4,   175,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      42,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,   110,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
     172,     3,     4,   175,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      42,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,   110,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
     172,     3,     4,   175,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    89,    -1,    -1,
      -1,    93,    -1,    -1,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,   110,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,   145,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,   163,    -1,    -1,   166,    -1,   168,   169,   170,    -1,
      -1,     3,     4,   175,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,   163,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
      -1,     3,     4,   175,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
      -1,     3,     4,   175,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,   171,
      -1,     3,     4,   175,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
      -1,   173,    -1,   175,     3,     4,    -1,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,   171,    -1,     3,     4,   175,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,   171,    -1,     3,     4,   175,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,   171,    -1,     3,     4,   175,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,    -1,     3,     4,   175,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,   145,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,   168,
     169,   170,    -1,    -1,     3,     4,   175,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,   145,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,   168,
     169,   170,    -1,    -1,     3,     4,   175,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,   145,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,   168,
     169,   170,    -1,    -1,     3,     4,   175,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,   110,   111,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,    -1,     3,     4,   175,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,    -1,     3,     4,   175,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      89,    -1,    -1,    -1,    93,    -1,    -1,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,    -1,     3,     4,   175,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,   120,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,    -1,   173,    -1,   175,     3,     4,    -1,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,   110,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,   163,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,    -1,   173,    -1,   175,
       3,     4,    -1,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,   110,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,   145,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
      -1,    -1,    -1,   166,    -1,   168,    -1,   170,   171,    -1,
       3,     4,   175,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,   110,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
     163,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,
     173,    -1,   175,     3,     4,    -1,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
     110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,    -1,     3,     4,   175,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
     110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,    -1,     3,     4,   175,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
     110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,    -1,   173,    -1,   175,     3,     4,    -1,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,   110,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,   169,   170,    -1,    -1,     3,     4,   175,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,   110,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,    -1,   173,    -1,   175,     3,
       4,    -1,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,   110,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,     3,
       4,   175,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,   110,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,     3,
       4,   175,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,   110,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,     3,
       4,   175,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,   110,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,     3,
       4,   175,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,   110,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,     3,
       4,   175,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,   110,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,     3,
       4,   175,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,   110,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,     4,    -1,     6,     7,     8,
       9,    10,    -1,   157,    -1,    14,   160,   161,   162,    18,
      -1,    -1,   166,    22,   168,    24,   170,    -1,    27,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    42,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    64,    -1,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    78,
      79,    80,    81,    -1,    -1,    -1,    -1,    86,    -1,    88,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,   110,   111,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,
      -1,   170,   171,   172,    -1,     4,   175,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    22,    -1,    24,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    42,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    64,    -1,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    78,
      79,    80,    81,    -1,    -1,    -1,    -1,    86,    -1,    88,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,   110,   111,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    12,    -1,   148,
      -1,    -1,    17,    18,    19,    20,    -1,    -1,   157,    -1,
      25,   160,   161,   162,    -1,    -1,    -1,   166,    33,   168,
      35,   170,    -1,   172,    39,    -1,   175,    -1,    -1,    -1,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    98,    -1,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
      -1,   136,   137,    -1,   139,   140,   141,   142,   143,   144,
      -1,   146,    -1,    -1,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,    -1,   161,    -1,    -1,   164,
     165,   166,   167,     3,     4,   170,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    24,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    42,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
     110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
      -1,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,     3,     4,    -1,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    24,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    42,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
     110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
      -1,   141,    -1,    -1,     3,     4,    -1,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,   162,    22,    -1,    24,    25,    -1,   168,    -1,
     170,    -1,   172,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    42,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,    -1,   102,    -1,     3,   105,    -1,    -1,    -1,
     109,   110,   111,    -1,    12,   114,    -1,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
     139,    -1,   141,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,   162,    62,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    70,   172,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      98,    -1,   100,    -1,   102,    -1,    -1,    -1,    -1,    -1,
     108,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,   136,   137,
      -1,   139,   140,   141,   142,   143,   144,    -1,   146,    -1,
      -1,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,     3,   161,    -1,    -1,   164,   165,   166,   167,
     168,    12,    -1,    -1,    -1,    -1,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    -1,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,   136,   137,    -1,   139,   140,
     141,   142,   143,   144,    -1,   146,    -1,    -1,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,     5,
     161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    37,    38,    -1,    12,    -1,    -1,    43,    -1,    -1,
      -1,    19,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    85,
      -1,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    89,    12,    -1,    -1,    93,    -1,    -1,    -1,    19,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   163,   136,   137,
      -1,   139,   140,   169,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    89,
     168,    -1,    12,    93,    -1,    -1,    -1,   175,    -1,    19,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,   137,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,    -1,   168,    89,
      12,    -1,    -1,    93,    -1,   175,    -1,    19,    -1,    -1,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,   137,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,    89,   168,    -1,
      12,    93,    -1,    -1,    -1,   175,    -1,    19,    -1,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,   137,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,    -1,   168,    89,    12,    -1,
      -1,    93,    -1,   175,    -1,    19,    -1,    -1,    -1,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,   137,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,    89,   168,    -1,    -1,    93,
      -1,    -1,    -1,   175,    -1,    -1,    -1,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,   136,   137,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    12,    -1,
     164,   165,   166,    -1,   168,    19,    -1,    -1,    -1,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    72,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   122,   163,
     164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,   175,    -1,    -1,    93,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    12,   118,
     164,   165,   166,   122,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    12,    -1,   164,   165,   166,    -1,   168,
      19,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,   118,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,
      89,    -1,    -1,    -1,    93,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,   122,   168,    -1,    -1,    -1,    -1,    -1,
      89,   175,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   122,    -1,   164,   165,   166,    -1,   168,
      -1,    12,    -1,    -1,    -1,    -1,   175,    -1,    19,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      31,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,   175,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    12,    -1,    -1,    -1,
     101,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   122,    -1,   164,   165,   166,    -1,   168,    -1,    -1,
      -1,    -1,    -1,    89,   175,    -1,    -1,    93,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    12,   118,   164,   165,   166,   122,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    12,    13,   164,   165,
     166,    -1,    -1,    19,    -1,    -1,    -1,   173,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,   118,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,   122,    -1,    -1,    -1,
      -1,    -1,   173,    89,   175,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   122,    -1,   164,   165,
     166,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,   175,
      -1,    19,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    31,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,   168,    -1,    12,    -1,    -1,    -1,    -1,   175,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,   139,   140,   101,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   122,   163,   164,   165,   166,    -1,
      -1,    89,    12,    -1,    -1,    93,    -1,   175,    -1,    19,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    31,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   122,    -1,   164,   165,   166,    -1,
      -1,    -1,    12,    13,    -1,    -1,    -1,   175,    -1,    19,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    31,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    89,
     168,    -1,    -1,    93,    -1,    -1,    -1,   175,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   122,    -1,   164,   165,   166,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,   175,    -1,    89,    -1,   139,
     140,    93,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,    12,    -1,   121,
     122,    -1,    -1,    -1,    19,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,   139,   140,    34,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,   139,   140,    93,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
     175,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   122,    -1,   164,   165,   166,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,   175,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    12,    13,   164,   165,   166,
      -1,    -1,    19,    -1,   171,    -1,    -1,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   122,    -1,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,   175,    -1,
      -1,    93,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    12,    -1,   164,   165,   166,
     122,    -1,    19,    -1,   171,    -1,    -1,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   175,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    89,   142,   143,   144,    93,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,   163,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,   121,   122,   175,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   122,    -1,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    12,    -1,   164,
     165,   166,    -1,    -1,    19,    -1,    -1,    -1,   173,     3,
     175,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    12,    13,    33,
      -1,    -1,    -1,    -1,    19,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    49,    31,    -1,    52,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    89,    69,    70,    -1,    93,    -1,
      -1,    -1,    12,    77,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    31,    96,    -1,    98,    -1,   100,   122,   102,    -1,
      -1,   105,    -1,    -1,    89,   109,    -1,   111,    93,    -1,
     114,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      60,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   122,    -1,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
     175,    -1,    -1,    93,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    12,    -1,   164,
     165,   166,   122,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    12,    -1,    -1,    93,    -1,
      -1,    -1,    19,    -1,    -1,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    31,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,
     175,    -1,    19,    -1,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,   139,   140,   101,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   122,    -1,   164,   165,   166,
      -1,    -1,    89,    12,    -1,    -1,    93,    -1,   175,    -1,
      19,    -1,   139,   140,   101,   142,   143,   144,    -1,   146,
     147,   148,    31,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   122,    -1,   164,   165,   166,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,   175,    -1,
      19,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    31,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
      89,    -1,    -1,    12,    93,    -1,    -1,    -1,   175,    -1,
      19,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   122,    -1,   164,   165,   166,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,   175,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   122,    -1,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,     5,    -1,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    71,
      26,    27,    -1,    -1,    76,    -1,    78,    -1,    80,    35,
      82,    37,    38,    85,    -1,    -1,    88,    43,    90,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      -1,    57,    31,    -1,    -1,    -1,   108,    -1,   110,    65,
      -1,   113,    89,    -1,    -1,    71,    93,    73,    -1,    -1,
      76,    -1,    78,    -1,    -1,    -1,    82,    83,    84,    85,
      -1,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
      89,   163,   139,   140,    93,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,    -1,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
      -1,    31,    -1,   122,    -1,    -1,    -1,    89,   175,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,
     139,   140,   168,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,    -1,   155,   156,   157,   158,
     159,    -1,   161,    -1,    -1,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,   139,   140,    89,
     142,   143,   144,    93,   146,   147,   148,    -1,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,    -1,   161,
      -1,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
      -1,   151,   152,   153,    -1,   155,   156,   157,   158,    -1,
      -1,   161,    -1,    -1,   164,   165,   166,     3,    -1,     5,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    17,
      18,    77,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      96,    39,    98,    -1,   100,    -1,   102,    45,    -1,   105,
      -1,    49,    -1,   109,    52,   111,    54,    -1,   114,    -1,
      -1,    -1,    -1,     3,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    17,    18,    77,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    96,    39,
      98,    -1,   100,    -1,   102,    45,    -1,   105,    -1,    49,
      -1,   109,    52,   111,    54,    -1,   114,    -1,    -1,    -1,
      -1,     3,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    17,    18,    77,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    96,    39,    98,    -1,
     100,    -1,   102,    45,    -1,   105,    -1,    49,    -1,   109,
      52,   111,    54,    -1,   114,    -1,    -1,    -1,    -1,     3,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    17,    18,    77,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    87,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    96,    39,    98,    -1,   100,    -1,
     102,    45,    -1,   105,    -1,    49,    -1,   109,    52,   111,
      54,    -1,   114,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    98,    -1,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,
     114
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   180,   181,     0,     1,     3,     4,     5,     6,     7,
       8,     9,    10,    14,    15,    16,    17,    18,    20,    22,
      23,    25,    26,    27,    28,    29,    30,    33,    35,    37,
      38,    39,    43,    44,    45,    48,    49,    50,    52,    54,
      56,    57,    58,    59,    61,    62,    63,    64,    66,    67,
      69,    70,    71,    76,    77,    78,    79,    80,    81,    82,
      85,    86,    87,    88,    90,    91,    92,    94,    95,    96,
      97,    98,    99,   100,   102,   103,   105,   106,   107,   108,
     109,   110,   111,   113,   114,   119,   122,   139,   140,   141,
     142,   143,   148,   157,   160,   161,   162,   163,   166,   168,
     170,   175,   182,   183,   184,   185,   186,   187,   190,   191,
     192,   193,   198,   199,   200,   202,   203,   204,   209,   210,
     214,   215,   219,   221,   224,   225,   228,   229,   230,   231,
     232,   234,   237,   238,   239,   243,   245,   246,   249,   250,
     251,   252,   253,   257,   258,   261,   267,   268,   269,   270,
     273,   274,   279,   280,   285,   289,   290,   292,   293,   313,
     318,   319,   323,   324,   325,   350,   351,   352,   354,   356,
     357,   358,   364,   365,   366,   367,   368,   369,   371,   374,
     375,   376,   377,   378,   379,   380,   381,   383,   384,   385,
     386,   387,   163,    22,    35,    44,    54,    56,    88,   100,
     110,   168,   238,   249,   350,   357,   365,   366,   371,   374,
     376,   377,   118,   359,   360,     3,    54,   216,   371,   359,
     108,   326,    90,   216,   184,   341,   371,   170,     3,    17,
      18,    20,    25,    33,    39,    45,    49,    52,    54,    62,
      69,    70,    77,    87,    96,    98,   100,   102,   105,   109,
     111,   114,   217,   218,   220,    11,    27,   113,   244,   371,
      82,    85,   200,   170,   102,   218,   218,   218,   170,   218,
     328,   329,    32,   204,   223,   371,   265,   266,   371,   371,
      18,    77,    96,   111,   371,   371,   371,     9,   170,   227,
     226,    27,    33,    47,    49,    51,    75,    78,    90,   100,
     108,   113,   183,   222,   281,   282,   283,   304,   305,   306,
     333,   338,   371,   341,   106,   107,   163,   285,   286,   370,
     371,   373,   371,   223,   371,   371,    99,   170,   184,   371,
     371,    71,   110,   186,   190,   204,   186,   204,    94,   371,
     373,   371,   371,   371,   371,   371,     1,   169,   182,   205,
     341,   149,   345,   346,   373,   371,    80,   184,    22,    35,
      38,    71,    88,   110,   168,   187,   188,   189,   200,   204,
     192,   147,   194,   168,    46,    84,   112,   201,    26,   325,
     371,     9,   110,   269,   371,   372,    24,    32,    40,    41,
      42,   117,   172,   240,   241,   353,   355,   170,   204,    56,
     144,   271,   218,   168,    53,    73,    83,   308,    27,    47,
      51,    75,    78,    90,   108,   113,   309,    27,    47,    51,
      75,    78,    90,   108,   113,   307,   218,   320,   321,     1,
     328,   162,   163,   371,    12,    19,    31,    89,    93,   122,
     139,   140,   142,   143,   144,   146,   147,   148,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     164,   165,   166,   175,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   167,
     297,   170,   172,    89,    93,   371,   218,    40,   172,   240,
      56,     1,   170,   184,   163,   204,   324,   163,   145,   163,
     219,   347,   348,   349,   373,   218,   218,   101,   204,   101,
     123,   204,   297,   201,   347,   163,   170,   184,   170,   218,
     330,   331,     1,   144,   337,    47,   145,   184,   223,   145,
     223,    13,   170,   170,   223,   327,   347,   228,   228,    47,
      90,   305,   306,   171,   145,   170,   218,   144,   163,   371,
     371,   121,   287,   163,   168,   223,   170,   347,   163,   371,
     254,   254,   371,   163,   169,   169,   182,   145,   169,   371,
     145,   145,   171,   171,     9,   371,   172,   240,   241,     3,
     170,   195,     1,   169,   205,   212,   213,   371,   207,   371,
      65,    36,    72,   163,   269,   271,   108,   233,   235,   290,
     184,    78,   244,   371,   120,   173,   242,   341,   342,   343,
     371,   382,   242,   371,   218,   247,   248,   218,   341,   168,
       1,   169,   191,   218,   275,   278,    47,    90,     1,   144,
     335,   336,    47,    90,     1,     3,    12,    17,    19,    20,
      25,    45,    52,    54,    55,    62,    70,    87,    98,   102,
     109,   114,   139,   140,   141,   142,   143,   144,   146,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   161,
     164,   165,   166,   167,   170,   218,   294,   295,   296,   297,
     350,   123,   322,   145,   163,   163,   163,   371,   371,   371,
     242,   371,   242,   371,   371,   371,   371,   371,   371,   371,
       3,    20,    33,    62,   102,   108,   219,   371,   371,   371,
     371,   371,   371,   371,   371,   371,   371,   371,   371,   371,
     371,   371,   371,   371,    68,   373,   373,   373,   347,   347,
     242,   371,   242,   371,   101,   170,   341,   242,   371,   218,
       1,    27,    47,    51,    75,    78,    90,   108,   113,   171,
     361,   362,   363,   371,   387,   328,   371,   123,   171,   145,
     123,   123,   184,    34,   184,   371,    34,   371,    65,   171,
     348,   331,   145,   171,   222,   371,   123,   332,   371,   329,
     265,   218,   323,   371,   371,   324,   171,   218,   309,   283,
     331,   144,   339,   340,   338,   288,    32,   203,   312,     1,
     259,   347,   171,    21,   255,   168,   169,   171,   171,   345,
     373,   171,   345,   341,   242,   371,   194,   171,   196,   197,
     349,   169,   169,   145,   163,    13,   147,   145,   163,    13,
      36,    72,   218,   139,   140,   141,   142,   143,   157,   161,
     166,   206,   296,   297,   298,   371,   206,   208,   271,   168,
     236,   324,   117,   218,   223,   223,   170,   371,   173,   360,
      47,   145,   163,   173,   344,   360,   341,   173,   344,   121,
     145,   173,    32,   223,   360,    32,    47,   223,   360,   145,
     171,   163,   170,     1,   272,   169,   278,   123,   145,   169,
       1,   222,   333,   334,   104,   310,   170,   300,   371,   139,
     147,   300,   300,   371,   321,   170,   172,   163,   163,   163,
     163,   171,   173,   348,    47,   173,    32,    32,    47,   170,
     171,    47,    90,   145,   171,    17,    20,    25,    45,    52,
      62,    70,    87,    98,   109,   114,   350,    89,    89,   163,
     373,   349,   371,   371,    34,   184,    34,    34,   184,   101,
     204,   218,   171,   171,   330,   337,    68,   373,   218,   171,
     171,   328,   340,   144,   284,   171,   338,   149,   299,   332,
     371,   182,   169,    74,   115,   169,   260,   171,   170,   204,
     218,   256,     1,   262,    47,   173,    47,   145,   171,   212,
     219,    17,    18,    20,    25,    45,    49,    52,    70,    77,
      87,    96,    98,   100,   109,   111,   114,   168,   211,   298,
     371,   371,   206,   208,   145,   163,    13,   163,   168,   272,
     320,   328,   371,    47,   341,   184,   173,   242,   371,   173,
     344,   184,   371,   163,   173,   173,   145,   173,   371,   173,
     371,   371,   223,    44,   371,   242,   371,   223,   248,   204,
     348,   169,    82,    85,   169,   183,   191,   225,   371,   276,
     277,    60,   116,   314,   301,   302,   303,   304,   171,   295,
     297,   171,   173,   171,   242,   371,    44,   242,   371,   348,
     362,   337,   350,   350,   184,   184,   371,   184,    34,   163,
     163,   332,   170,   170,   163,   299,   332,   333,   310,   340,
     371,   184,   238,   341,   256,   144,   204,   169,    74,   115,
     169,   263,   264,   242,   371,   242,   371,   197,   206,    13,
      36,    72,   163,   163,   298,   371,   371,   272,   169,   163,
     163,   163,   371,   171,   184,   173,   360,   173,   360,   173,
     184,   121,   371,    32,   223,   360,    32,   223,   360,   171,
     191,   225,   225,   278,   191,     3,    54,    92,   102,   315,
     316,   317,   371,   291,   145,   171,   300,   300,   173,   173,
     371,    32,    32,   171,   332,    34,   184,   347,   347,   299,
     332,    32,   223,   171,   371,   184,   238,   371,   169,   173,
     173,   169,   371,   206,   208,    13,   169,   223,    44,   184,
     371,   173,    44,   184,   371,   173,   371,   101,    44,   371,
     223,    44,   371,   223,   163,   225,   278,   317,   116,   145,
     123,   150,   152,   153,   155,   156,    60,   163,   311,   312,
     303,    44,    44,   101,    44,    44,   184,   171,   171,   184,
     204,    32,   223,   163,   163,   371,   371,   184,   371,   184,
     371,   371,   371,   371,   316,   317,   317,   317,   317,   317,
     317,   315,   371,   371,   371,   371,   162,   162,   184,   101,
     101,   101,   101,   101,   101,   101,   101,   371,   371,   371,
     371
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   179,   180,   181,   181,   182,   182,   183,   183,   184,
     184,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   187,   187,   187,   188,
     189,   189,   190,   191,   191,   192,   193,   194,   194,   195,
     195,   195,   196,   196,   197,   198,   199,   199,   199,   200,
     200,   200,   201,   201,   202,   203,   203,   203,   204,   205,
     205,   206,   206,   206,   206,   206,   206,   207,   207,   207,
     207,   208,   208,   209,   209,   209,   209,   209,   210,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   212,   212,   212,   212,   212,
     213,   213,   214,   215,   215,   215,   215,   216,   216,   216,
     217,   217,   217,   217,   217,   218,   218,   218,   219,   219,
     219,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   222,   222,   222,   223,   223,   224,   224,   225,   225,
     226,   225,   227,   225,   228,   228,   228,   228,   228,   228,
     228,   229,   229,   229,   229,   230,   231,   231,   232,   233,
     234,   234,   234,   235,   234,   236,   234,   237,   238,   239,
     239,   240,   240,   240,   240,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   242,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   244,   244,
     245,   246,   246,   247,   247,   248,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   250,
     250,   250,   250,   250,   250,   251,   251,   251,   252,   252,
     252,   253,   253,   253,   253,   254,   254,   255,   255,   255,
     256,   256,   257,   258,   258,   259,   259,   260,   260,   260,
     261,   261,   261,   262,   262,   263,   264,   264,   265,   265,
     265,   266,   266,   267,   268,   268,   269,   270,   270,   270,
     271,   271,   272,   272,   272,   272,   272,   273,   273,   273,
     274,   275,   275,   276,   275,   275,   277,   275,   278,   278,
     279,   280,   280,   280,   281,   281,   282,   282,   283,   283,
     283,   284,   284,   285,   287,   286,   288,   286,   289,   291,
     290,   292,   292,   292,   292,   292,   293,   294,   294,   295,
     295,   295,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   298,   298,   299,   299,   300,   300,   301,   301,   302,
     302,   303,   304,   304,   304,   304,   304,   304,   305,   305,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   308,   308,   308,   309,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   309,   310,   310,   311,   311,   312,
     312,   313,   314,   314,   314,   314,   314,   315,   315,   316,
     316,   316,   316,   316,   316,   316,   317,   317,   317,   318,
     319,   319,   320,   320,   321,   322,   322,   323,   323,   323,
     323,   323,   324,   326,   325,   327,   325,   325,   325,   328,
     328,   329,   329,   330,   330,   331,   331,   331,   332,   332,
     332,   333,   334,   334,   334,   335,   335,   336,   336,   337,
     337,   337,   337,   338,   338,   339,   340,   340,   341,   341,
     342,   342,   342,   343,   343,   344,   344,   345,   346,   346,
     347,   347,   348,   348,   349,   349,   350,   350,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     352,   353,   353,   353,   353,   353,   353,   353,   354,   355,
     355,   355,   355,   355,   355,   355,   356,   357,   358,   358,
     358,   358,   358,   358,   359,   359,   360,   360,   360,   360,
     361,   361,   362,   362,   362,   363,   363,   363,   363,   363,
     363,   363,   363,   363,   363,   364,   364,   364,   364,   364,
     365,   365,   365,   365,   365,   366,   367,   367,   367,   367,
     367,   367,   368,   369,   370,   370,   371,   371,   371,   371,
     371,   371,   371,   371,   371,   371,   371,   371,   371,   371,
     371,   371,   372,   372,   373,   373,   373,   374,   374,   374,
     374,   375,   375,   375,   375,   376,   376,   376,   377,   377,
     377,   377,   377,   377,   378,   378,   378,   378,   379,   379,
     380,   380,   381,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   381,   381,   382,   382,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   384,   384,   384,   384,   384,   384,   384,
     385,   385,   385,   385,   386,   386,   386,   386,   387,   387,
     387,   387,   387,   387,   387
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     3,     3,     3,     3,     3,     2,     1,
       1,     1,     1,     1,     1,     2,     1,     3,     3,     3,
       3,     2,     3,     3,     2,     2,     1,     1,     1,     0,
       1,     1,     1,     1,     2,     3,     1,     0,     3,     2,
       3,     0,     1,     3,     1,     4,     3,     4,     4,     0,
       1,     1,     0,     1,     6,     2,     3,     3,     1,     1,
       2,     1,     1,     3,     3,     3,     5,     1,     3,     3,
       5,     0,     1,     4,     6,     8,     6,     8,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     5,
       1,     3,     3,     4,     4,     4,     4,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     2,     3,     1,     1,
       0,     3,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     3,     5,     5,     2,     1,     1,     1,     1,     1,
       6,     7,     3,     0,     6,     0,     6,     2,     0,     3,
       2,     1,     1,     1,     1,     5,     3,     3,     6,     5,
       6,     5,     6,     3,     4,     3,     4,     6,     7,     6,
       7,     4,     5,     4,     5,     4,     4,     3,     6,     5,
       4,     3,     6,     5,     6,     5,     8,     7,     4,     4,
       2,     5,     2,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     6,     4,     7,     5,     3,
       6,     2,     2,     3,     3,     0,     2,     2,     3,     5,
       1,     3,     3,     5,     5,     0,     2,     3,     2,     4,
       6,     7,     6,     0,     2,     3,     2,     4,     4,     3,
       1,     1,     3,     3,     5,     5,     2,     1,     1,     1,
       0,     2,     0,     2,     3,     3,     4,     4,     4,     3,
       2,     1,     2,     0,     4,     2,     0,     5,     1,     3,
       0,     1,     1,     1,     0,     1,     1,     3,     1,     3,
       1,     0,     2,     6,     0,     3,     0,     4,     1,     0,
       7,     4,     4,     6,     6,     4,     2,     1,     3,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     0,     3,     1,     3,     0,
       1,     1,     4,     5,     4,     5,     6,     6,     0,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       0,     1,     1,     2,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     0,     1,     1,     2,     1,     1,
       1,     1,     2,     1,     1,     0,     1,     1,     1,     1,
       2,     1,     0,     2,     2,     4,     4,     1,     3,     3,
       3,     3,     3,     3,     3,     2,     1,     1,     1,     3,
       1,     2,     1,     3,     2,     0,     2,     1,     2,     1,
       1,     1,     1,     0,     5,     0,     6,     3,     3,     1,
       3,     3,     5,     1,     3,     1,     2,     3,     0,     2,
       2,     1,     1,     1,     1,     2,     1,     0,     1,     0,
       2,     2,     1,     1,     1,     2,     0,     1,     1,     3,
       2,     3,     2,     2,     3,     1,     2,     1,     3,     3,
       0,     1,     1,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     4,     5,     2,     2,     1,
       2,     1,     2,     1,     2,     1,     2,     1,     1,     1,
       2,     6,     6,     4,     4,     9,     9,     7,     2,     2,
       3,     4,     6,     6,     9,     9,     6,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     4,     3,     4,     2,
       1,     3,     4,     3,     3,     1,     1,     2,     1,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       2,     8,     8,     9,     9,     4,     3,     3,     2,     2,
       2,     1,     3,     4,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     2,     2,     1,     4,     4,     4,     3,     3,
       3,     3,     5,     5,     3,     4,     3,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     4,
       4,     5,     4,     5,     5,     6,     4,     5,     3,     5,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1
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
#line 692 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6006 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 696 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6012 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 697 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6018 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 704 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6026 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 712 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6034 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 716 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6043 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 726 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6051 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 731 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6059 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 743 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)); }
#line 6065 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 745 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6071 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 746 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6077 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 747 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6083 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 748 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6089 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 749 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6095 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 750 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6101 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 751 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6107 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: TDEFER stmt  */
#line 753 "chpl.ypp"
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
    if (context->isValidCurlyBracesLoc((yyvsp[0].commentsAndStmt).curlyLoc)) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC((yyvsp[0].commentsAndStmt).curlyLoc));
    }
    CommentsAndStmt ret = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6128 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: try_stmt  */
#line 769 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6134 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: return_stmt  */
#line 770 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6140 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: TINIT TTHIS TSEMI  */
#line 772 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr));
    auto node = Init::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6152 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 780 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6165 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 789 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6178 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 798 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6186 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 802 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6197 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 809 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6206 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 816 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6212 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 817 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6218 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 818 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6224 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 819 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6230 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: match_select_stmt  */
#line 820 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6236 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: manage_stmt  */
#line 821 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6242 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 822 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6248 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: throw_stmt  */
#line 823 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6254 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 825 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6260 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 827 "chpl.ypp"
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
    if (context->isValidCurlyBracesLoc((yyvsp[0].commentsAndStmt).curlyLoc)) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC((yyvsp[0].commentsAndStmt).curlyLoc));
    }
    CommentsAndStmt ret = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6283 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 846 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6295 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 854 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Local::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)), blockStyle,
                             std::move(stmts));
    BUILDER->noteBlockHeaderLocation(node.get(), LOC2((yylsp[-2]), (yylsp[-1])));
    if (context->isValidCurlyBracesLoc((yyvsp[0].blockOrDo).cs.curlyLoc)) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC((yyvsp[0].blockOrDo).cs.curlyLoc));
    }
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6315 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TLOCAL do_stmt  */
#line 870 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-1]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Local::build(BUILDER, LOC((yyloc)), blockStyle, std::move(stmts));
    BUILDER->noteBlockHeaderLocation(node.get(), LOC((yylsp[-1])));
    if (context->isValidCurlyBracesLoc((yyvsp[0].blockOrDo).cs.curlyLoc)) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC((yyvsp[0].blockOrDo).cs.curlyLoc));
    }
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6334 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TON expr do_stmt  */
#line 885 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = On::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)), blockStyle,
                          std::move(stmts));
    BUILDER->noteBlockHeaderLocation(node.get(), LOC2((yylsp[-2]), (yylsp[-1])));
    if (context->isValidCurlyBracesLoc((yyvsp[0].blockOrDo).cs.curlyLoc)) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC((yyvsp[0].blockOrDo).cs.curlyLoc));
    }
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6354 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 901 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Serial::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)), blockStyle,
                              std::move(stmts));
    BUILDER->noteBlockHeaderLocation(node.get(), LOC2((yylsp[-2]), (yylsp[-1])));
    if (context->isValidCurlyBracesLoc((yyvsp[0].blockOrDo).cs.curlyLoc)) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC((yyvsp[0].blockOrDo).cs.curlyLoc));
    }
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6374 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSERIAL do_stmt  */
#line 917 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-1]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Serial::build(BUILDER, LOC((yyloc)), blockStyle, std::move(stmts));
    BUILDER->noteBlockHeaderLocation(node.get(), LOC((yylsp[-1])));
    if (context->isValidCurlyBracesLoc((yyvsp[0].blockOrDo).cs.curlyLoc)) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC((yyvsp[0].blockOrDo).cs.curlyLoc));
    }
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6393 "bison-chpl-lib.cpp"
    break;

  case 45: /* tryable_stmt: TSYNC stmt  */
#line 932 "chpl.ypp"
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
    if (context->isValidCurlyBracesLoc((yyvsp[0].commentsAndStmt).curlyLoc)) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC((yyvsp[0].commentsAndStmt).curlyLoc));
    }
    CommentsAndStmt ret = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6414 "bison-chpl-lib.cpp"
    break;

  case 49: /* collect_attributes: %empty  */
#line 957 "chpl.ypp"
  {
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 6422 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 978 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6430 "bison-chpl-lib.cpp"
    break;

  case 56: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 985 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6439 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: %empty  */
#line 992 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6447 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 996 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6458 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP TRP  */
#line 1006 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6464 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 1008 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6472 "bison-chpl-lib.cpp"
    break;

  case 61: /* opt_attribute_actuals: %empty  */
#line 1012 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6480 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actual  */
#line 1019 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6489 "bison-chpl-lib.cpp"
    break;

  case 63: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 1024 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6499 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 1037 "chpl.ypp"
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
#line 6521 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 1058 "chpl.ypp"
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
      BUILDER->noteCurlyBracesLocation(mod.get(), LOC2((yylsp[-1]), (yylsp[0])));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6543 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 1076 "chpl.ypp"
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
      BUILDER->noteCurlyBracesLocation(mod.get(), LOC2((yylsp[-2]), (yylsp[0])));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6565 "bison-chpl-lib.cpp"
    break;

  case 68: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1094 "chpl.ypp"
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
      BUILDER->noteCurlyBracesLocation(mod.get(), LOC2((yylsp[-2]), (yylsp[0])));
      CommentsAndStmt cs = makeCommentsAndStmt(parts.comments, mod.release());
      (yyval.commentsAndStmt) = cs;
    }
#line 6585 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: %empty  */
#line 1112 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6591 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPUBLIC  */
#line 1113 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6598 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_access_control: TPRIVATE  */
#line 1115 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6605 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: %empty  */
#line 1120 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6611 "bison-chpl-lib.cpp"
    break;

  case 73: /* opt_prototype: TPROTOTYPE  */
#line 1121 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6618 "bison-chpl-lib.cpp"
    break;

  case 74: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1127 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr), LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6628 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR TRCBR  */
#line 1145 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6637 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1150 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6646 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt_body: TLCBR error TRCBR  */
#line 1155 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6659 "bison-chpl-lib.cpp"
    break;

  case 78: /* block_stmt: block_stmt_body  */
#line 1167 "chpl.ypp"
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
    if (context->hasCurlyBracesLoc()) {
      cs.curlyLoc = context->curlyBracesLoc();
      context->resetCurlyBracesLoc();
    }
    (yyval.commentsAndStmt) = cs;
  }
#line 6682 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: toplevel_stmt  */
#line 1189 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6688 "bison-chpl-lib.cpp"
    break;

  case 80: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1190 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6694 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: expr  */
#line 1195 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6702 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: all_op_name  */
#line 1199 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6710 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: expr TAS expr  */
#line 1203 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6719 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA expr  */
#line 1208 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6727 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1212 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6735 "bison-chpl-lib.cpp"
    break;

  case 86: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1216 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6744 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr  */
#line 1224 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6753 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: expr TAS expr  */
#line 1229 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6763 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1235 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6772 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1240 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6783 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: %empty  */
#line 1248 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6789 "bison-chpl-lib.cpp"
    break;

  case 92: /* opt_only_ls: renames_ls  */
#line 1249 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6795 "bison-chpl-lib.cpp"
    break;

  case 93: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1254 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6804 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1259 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6816 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1267 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6829 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1276 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6841 "bison-chpl-lib.cpp"
    break;

  case 97: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1284 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6854 "bison-chpl-lib.cpp"
    break;

  case 98: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1295 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6863 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr  */
#line 1330 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6871 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT all_op_name  */
#line 1334 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6880 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1339 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6889 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TAS ident_use  */
#line 1344 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6899 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1350 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6909 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_expr  */
#line 1358 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6915 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_ls: import_ls TCOMMA import_expr  */
#line 1359 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6921 "bison-chpl-lib.cpp"
    break;

  case 122: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1364 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = makeCommentsAndStmt(comments, node.release());
  }
#line 6931 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1373 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6939 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1377 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6947 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1381 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6955 "bison-chpl-lib.cpp"
    break;

  case 126: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1385 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6963 "bison-chpl-lib.cpp"
    break;

  case 127: /* opt_label_ident: %empty  */
#line 1393 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6969 "bison-chpl-lib.cpp"
    break;

  case 128: /* opt_label_ident: TIDENT  */
#line 1394 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6975 "bison-chpl-lib.cpp"
    break;

  case 129: /* opt_label_ident: TINIT  */
#line 1395 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6981 "bison-chpl-lib.cpp"
    break;

  case 138: /* ident_use: TIDENT  */
#line 1421 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6987 "bison-chpl-lib.cpp"
    break;

  case 139: /* ident_use: TINIT  */
#line 1422 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6993 "bison-chpl-lib.cpp"
    break;

  case 140: /* ident_use: TTHIS  */
#line 1423 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6999 "bison-chpl-lib.cpp"
    break;

  case 159: /* scalar_type: TBOOL  */
#line 1449 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7005 "bison-chpl-lib.cpp"
    break;

  case 160: /* scalar_type: TENUM  */
#line 1450 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7011 "bison-chpl-lib.cpp"
    break;

  case 161: /* scalar_type: TINT  */
#line 1451 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7017 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TUINT  */
#line 1452 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7023 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TREAL  */
#line 1453 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7029 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TIMAG  */
#line 1454 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7035 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TCOMPLEX  */
#line 1455 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7041 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TBYTES  */
#line 1456 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7047 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TSTRING  */
#line 1457 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7053 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TLOCALE  */
#line 1458 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7059 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TNOTHING  */
#line 1459 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7065 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TVOID  */
#line 1460 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7071 "bison-chpl-lib.cpp"
    break;

  case 174: /* do_stmt: TDO stmt  */
#line 1473 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7077 "bison-chpl-lib.cpp"
    break;

  case 175: /* do_stmt: block_stmt  */
#line 1474 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7083 "bison-chpl-lib.cpp"
    break;

  case 176: /* return_stmt: TRETURN TSEMI  */
#line 1479 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7094 "bison-chpl-lib.cpp"
    break;

  case 177: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1486 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7105 "bison-chpl-lib.cpp"
    break;

  case 178: /* class_level_stmt: TSEMI  */
#line 1496 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7113 "bison-chpl-lib.cpp"
    break;

  case 179: /* class_level_stmt: inner_class_level_stmt  */
#line 1500 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7123 "bison-chpl-lib.cpp"
    break;

  case 180: /* $@1: %empty  */
#line 1505 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7130 "bison-chpl-lib.cpp"
    break;

  case 181: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1507 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->adjustLocation((yyval.commentsAndStmt), (yylsp[-2]), (yylsp[0]));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7140 "bison-chpl-lib.cpp"
    break;

  case 182: /* $@2: %empty  */
#line 1512 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7147 "bison-chpl-lib.cpp"
    break;

  case 183: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1514 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->adjustLocation((yyval.commentsAndStmt), (yylsp[-2]), (yylsp[0]));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7157 "bison-chpl-lib.cpp"
    break;

  case 191: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1533 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7166 "bison-chpl-lib.cpp"
    break;

  case 192: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1538 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7175 "bison-chpl-lib.cpp"
    break;

  case 193: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1543 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7184 "bison-chpl-lib.cpp"
    break;

  case 194: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1548 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7192 "bison-chpl-lib.cpp"
    break;

  case 195: /* forwarding_decl_start: TFORWARDING  */
#line 1555 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7201 "bison-chpl-lib.cpp"
    break;

  case 196: /* extern_or_export: TEXTERN  */
#line 1562 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7207 "bison-chpl-lib.cpp"
    break;

  case 197: /* extern_or_export: TEXPORT  */
#line 1563 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7213 "bison-chpl-lib.cpp"
    break;

  case 198: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1568 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7223 "bison-chpl-lib.cpp"
    break;

  case 199: /* extern_export_kind_inner: TTYPE  */
#line 1581 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7232 "bison-chpl-lib.cpp"
    break;

  case 200: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1590 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7243 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1598 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7256 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1607 "chpl.ypp"
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
#line 7278 "bison-chpl-lib.cpp"
    break;

  case 203: /* $@3: %empty  */
#line 1625 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7286 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 1629 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7295 "bison-chpl-lib.cpp"
    break;

  case 205: /* $@4: %empty  */
#line 1634 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7304 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr extern_export_kind_inner $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1639 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-1])), (yyvsp[-3].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7313 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yyloc), (yyvsp[0].sizedStr));
  }
#line 7321 "bison-chpl-lib.cpp"
    break;

  case 208: /* no_loop_attributes: %empty  */
#line 1653 "chpl.ypp"
  {
    // Even though there are no attributes, there could be loop pragmas.
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 7330 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1659 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7336 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1660 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7342 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1677 "chpl.ypp"
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
    if (context->isValidCurlyBracesLoc((yyvsp[-3].commentsAndStmt).curlyLoc)) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC((yyvsp[-3].commentsAndStmt).curlyLoc));
    }
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7368 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1699 "chpl.ypp"
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
    if (context->isValidCurlyBracesLoc((yyvsp[0].blockOrDo).cs.curlyLoc)) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC((yyvsp[0].blockOrDo).cs.curlyLoc));
    }
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7390 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1717 "chpl.ypp"
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
    if (context->isValidCurlyBracesLoc((yyvsp[0].blockOrDo).cs.curlyLoc)) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC((yyvsp[0].blockOrDo).cs.curlyLoc));
    }
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7412 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1735 "chpl.ypp"
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
    if (context->isValidCurlyBracesLoc((yyvsp[0].blockOrDo).cs.curlyLoc)) {
      BUILDER->noteCurlyBracesLocation(node.get(), LOC((yyvsp[0].blockOrDo).cs.curlyLoc));
    }
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    context->resetAttributeGroupPartsState();
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7440 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1759 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7449 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: for_loop_kw expr TIN expr task_intent_clause do_stmt  */
#line 1764 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7458 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1769 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7467 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator task_intent_clause do_stmt  */
#line 1774 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7476 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1779 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7485 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw expr task_intent_clause do_stmt  */
#line 1784 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7494 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1789 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7503 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: for_loop_kw zippered_iterator task_intent_clause do_stmt  */
#line 1794 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7512 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1799 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7521 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TLSBR expr_ls TIN expr task_intent_clause TRSBR stmt  */
#line 1804 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7530 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1809 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7539 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator task_intent_clause TRSBR stmt  */
#line 1814 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7548 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1819 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7557 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR expr_ls task_intent_clause TRSBR stmt  */
#line 1824 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7566 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1829 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7576 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TLSBR zippered_iterator task_intent_clause TRSBR stmt  */
#line 1835 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7586 "bison-chpl-lib.cpp"
    break;

  case 235: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1844 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7596 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF expr TTHEN stmt  */
#line 1853 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7604 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr block_stmt  */
#line 1857 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7612 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1861 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7620 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1865 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7628 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1869 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7636 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar block_stmt  */
#line 1873 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7644 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1877 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7652 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1881 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7660 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1885 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7669 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1890 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7678 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1895 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7687 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1900 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7696 "bison-chpl-lib.cpp"
    break;

  case 248: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1908 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)), LOC((yylsp[-2])),
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
#line 7715 "bison-chpl-lib.cpp"
    break;

  case 249: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1923 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)), LOC((yylsp[-2])),
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
#line 7734 "bison-chpl-lib.cpp"
    break;

  case 250: /* interface_start: TINTERFACE ident_def  */
#line 1941 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Interface);
  }
#line 7742 "bison-chpl-lib.cpp"
    break;

  case 251: /* interface_stmt: interface_start TLP ifc_formal_ls TRP block_stmt  */
#line 1948 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-4].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), parts, (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC((yyvsp[0].commentsAndStmt).curlyLoc));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7753 "bison-chpl-lib.cpp"
    break;

  case 252: /* interface_stmt: interface_start block_stmt  */
#line 1955 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-1].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), parts, nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC((yyvsp[0].commentsAndStmt).curlyLoc));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7764 "bison-chpl-lib.cpp"
    break;

  case 253: /* ifc_formal_ls: ifc_formal  */
#line 1964 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7770 "bison-chpl-lib.cpp"
    break;

  case 254: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1965 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7776 "bison-chpl-lib.cpp"
    break;

  case 255: /* ifc_formal: ident_def  */
#line 1970 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7782 "bison-chpl-lib.cpp"
    break;

  case 275: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 2013 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7790 "bison-chpl-lib.cpp"
    break;

  case 276: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 2017 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7798 "bison-chpl-lib.cpp"
    break;

  case 277: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 2021 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7806 "bison-chpl-lib.cpp"
    break;

  case 278: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2028 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7814 "bison-chpl-lib.cpp"
    break;

  case 279: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 2032 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7822 "bison-chpl-lib.cpp"
    break;

  case 280: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2036 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7830 "bison-chpl-lib.cpp"
    break;

  case 281: /* try_stmt: TTRY tryable_stmt  */
#line 2043 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7838 "bison-chpl-lib.cpp"
    break;

  case 282: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2047 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7846 "bison-chpl-lib.cpp"
    break;

  case 283: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2051 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7854 "bison-chpl-lib.cpp"
    break;

  case 284: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2055 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7862 "bison-chpl-lib.cpp"
    break;

  case 285: /* catch_expr_ls: %empty  */
#line 2061 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7868 "bison-chpl-lib.cpp"
    break;

  case 286: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2062 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7874 "bison-chpl-lib.cpp"
    break;

  case 287: /* catch_expr: TCATCH block_stmt  */
#line 2067 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false, TextLocation::create());
  }
#line 7882 "bison-chpl-lib.cpp"
    break;

  case 288: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2071 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false, TextLocation::create());
  }
#line 7890 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2075 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true, context->makeSpannedLocation((yylsp[-3]), (yylsp[-1])));
  }
#line 7898 "bison-chpl-lib.cpp"
    break;

  case 290: /* catch_expr_inner: ident_def  */
#line 2082 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)), LOC((yylsp[0])),
                         /*attributeGroup*/ nullptr,
                         Decl::DEFAULT_VISIBILITY,
                         Decl::DEFAULT_LINKAGE,
                         /*linkageName*/ nullptr,
                         /*name*/ (yyvsp[0].uniqueStr),
                         Variable::VAR,
                         /*isConfig*/ false,
                         /*isField*/ false,
                         /*typeExpression*/ nullptr,
                         /*initExpression*/ nullptr);
    (yyval.expr) = varDecl.release();
  }
#line 7917 "bison-chpl-lib.cpp"
    break;

  case 291: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2097 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)), LOC((yylsp[-2])),
                         /*attributeGroup*/ nullptr,
                         Decl::DEFAULT_VISIBILITY,
                         Decl::DEFAULT_LINKAGE,
                         /*linkageName*/ nullptr,
                         /*name*/ (yyvsp[-2].uniqueStr),
                         Variable::VAR,
                         /*isConfig*/ false,
                         /*isField*/ false,
                         /*typeExpression*/ toOwned((yyvsp[0].expr)),
                         /*initExpression*/ nullptr);
    (yyval.expr) = varDecl.release();
  }
#line 7936 "bison-chpl-lib.cpp"
    break;

  case 292: /* throw_stmt: TTHROW expr TSEMI  */
#line 2115 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = makeCommentsAndStmt(comments, node.release());
  }
#line 7946 "bison-chpl-lib.cpp"
    break;

  case 293: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2123 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7955 "bison-chpl-lib.cpp"
    break;

  case 294: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2128 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = makeCommentsAndStmt(comments, node.release());
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7966 "bison-chpl-lib.cpp"
    break;

  case 295: /* when_stmt_ls: %empty  */
#line 2137 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7972 "bison-chpl-lib.cpp"
    break;

  case 296: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2138 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7978 "bison-chpl-lib.cpp"
    break;

  case 297: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2143 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7986 "bison-chpl-lib.cpp"
    break;

  case 298: /* when_stmt: TOTHERWISE stmt  */
#line 2147 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7995 "bison-chpl-lib.cpp"
    break;

  case 299: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2152 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 8005 "bison-chpl-lib.cpp"
    break;

  case 300: /* match_select_stmt: TUNION TSELECT expr TLCBR match_case_stmt_ls TRCBR  */
#line 2161 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMatchStmt((yyloc), context->makeSpannedLocation((yylsp[-5]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList), nullptr);
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 8014 "bison-chpl-lib.cpp"
    break;

  case 301: /* match_select_stmt: TUNION TSELECT expr TLCBR match_case_stmt_ls match_otherwise_stmt TRCBR  */
#line 2166 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMatchStmt((yyloc), context->makeSpannedLocation((yylsp[-6]), (yylsp[-4])), toOwned((yyvsp[-4].expr)), (yyvsp[-2].exprList), toOwned((yyvsp[-1].commentsAndStmt).stmt));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-3]), (yylsp[0])));
  }
#line 8023 "bison-chpl-lib.cpp"
    break;

  case 302: /* match_select_stmt: TUNION TSELECT expr TLCBR error TRCBR  */
#line 2171 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = makeCommentsAndStmt(comments, node.release());
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 8034 "bison-chpl-lib.cpp"
    break;

  case 303: /* match_case_stmt_ls: %empty  */
#line 2179 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 8040 "bison-chpl-lib.cpp"
    break;

  case 304: /* match_case_stmt_ls: match_case_stmt_ls match_case_stmt  */
#line 2180 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8046 "bison-chpl-lib.cpp"
    break;

  case 305: /* match_case_stmt: TWHEN expr do_stmt  */
#line 2185 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMatchCaseStmt((yyloc), YLOC2((yylsp[-2]), (yylsp[-1])), (yylsp[-1]), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8054 "bison-chpl-lib.cpp"
    break;

  case 306: /* match_otherwise_stmt: TOTHERWISE stmt  */
#line 2192 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildMatchCaseStmt((yyloc), (yylsp[-1]), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 8063 "bison-chpl-lib.cpp"
    break;

  case 307: /* match_otherwise_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2197 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildMatchCaseStmt((yyloc), (yylsp[-3]), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 8073 "bison-chpl-lib.cpp"
    break;

  case 308: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2206 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8082 "bison-chpl-lib.cpp"
    break;

  case 309: /* manager_expr: expr TAS ident_def  */
#line 2211 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8090 "bison-chpl-lib.cpp"
    break;

  case 310: /* manager_expr: expr  */
#line 2215 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8098 "bison-chpl-lib.cpp"
    break;

  case 311: /* manager_expr_ls: manager_expr  */
#line 2221 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8104 "bison-chpl-lib.cpp"
    break;

  case 312: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2222 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8110 "bison-chpl-lib.cpp"
    break;

  case 313: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2227 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8119 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2237 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8130 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2244 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8143 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_start: class_tag ident_def  */
#line 2257 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8151 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_tag: TCLASS  */
#line 2263 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8157 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_tag: TRECORD  */
#line 2264 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8163 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_tag: TUNION  */
#line 2265 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8169 "bison-chpl-lib.cpp"
    break;

  case 320: /* opt_inherit: %empty  */
#line 2269 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8175 "bison-chpl-lib.cpp"
    break;

  case 321: /* opt_inherit: TCOLON expr_ls  */
#line 2270 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8181 "bison-chpl-lib.cpp"
    break;

  case 322: /* class_level_stmt_ls: %empty  */
#line 2274 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8190 "bison-chpl-lib.cpp"
    break;

  case 323: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2279 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8198 "bison-chpl-lib.cpp"
    break;

  case 324: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2283 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8206 "bison-chpl-lib.cpp"
    break;

  case 325: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2287 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8214 "bison-chpl-lib.cpp"
    break;

  case 326: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2291 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8222 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
#line 2299 "chpl.ypp"
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
    BUILDER->noteCurlyBracesLocation(decl.get(), LOC2((yylsp[-2]), (yylsp[0])));
    CommentsAndStmt cs = makeCommentsAndStmt(parts.comments, decl.release());
    (yyval.commentsAndStmt) = cs;
    context->exitScope(asttags::Enum, parts.name);
    context->resetDeclState();
    context->clearComments();
  }
#line 8246 "bison-chpl-lib.cpp"
    break;

  case 328: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
#line 2319 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
    auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    CommentsAndStmt cs = makeCommentsAndStmt(parts.comments, err.release());
    (yyval.commentsAndStmt) = cs;
    context->exitScope(asttags::Enum, parts.name);
    context->resetDeclState();
    context->clearComments();
  }
#line 8260 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_decl_stmt: enum_header TLCBR TRCBR  */
#line 2329 "chpl.ypp"
  {
    CHPL_PARSER_REPORT(context, EmptyEnum, (yyloc));
    TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
    auto err = ErroneousExpression::build(BUILDER, LOC((yyloc)));
    CommentsAndStmt cs = makeCommentsAndStmt(parts.comments, err.release());
    (yyval.commentsAndStmt) = cs;
    context->exitScope(asttags::Enum, parts.name);
    context->resetDeclState();
    context->clearComments();
  }
#line 8275 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_header: TENUM ident_def  */
#line 2343 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8283 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_ls: enum_item  */
#line 2350 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8292 "bison-chpl-lib.cpp"
    break;

  case 332: /* enum_ls: enum_ls TCOMMA  */
#line 2355 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8302 "bison-chpl-lib.cpp"
    break;

  case 333: /* $@5: %empty  */
#line 2361 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8311 "bison-chpl-lib.cpp"
    break;

  case 334: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2366 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8320 "bison-chpl-lib.cpp"
    break;

  case 335: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2371 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8329 "bison-chpl-lib.cpp"
    break;

  case 336: /* $@6: %empty  */
#line 2376 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8338 "bison-chpl-lib.cpp"
    break;

  case 337: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2381 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8347 "bison-chpl-lib.cpp"
    break;

  case 338: /* enum_item: ident_def  */
#line 2391 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[0])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8359 "bison-chpl-lib.cpp"
    break;

  case 339: /* enum_item: ident_def TASSIGN expr  */
#line 2399 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[-2])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8373 "bison-chpl-lib.cpp"
    break;

  case 341: /* linkage_spec: linkage_spec_empty  */
#line 2414 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8379 "bison-chpl-lib.cpp"
    break;

  case 342: /* linkage_spec: TINLINE  */
#line 2415 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8386 "bison-chpl-lib.cpp"
    break;

  case 343: /* linkage_spec: TOVERRIDE  */
#line 2417 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8393 "bison-chpl-lib.cpp"
    break;

  case 344: /* opt_fn_type_formal_ls: %empty  */
#line 2422 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8399 "bison-chpl-lib.cpp"
    break;

  case 345: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2423 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8405 "bison-chpl-lib.cpp"
    break;

  case 346: /* fn_type_formal_ls: fn_type_formal  */
#line 2427 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8411 "bison-chpl-lib.cpp"
    break;

  case 347: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2428 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8417 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_type_formal: named_formal  */
#line 2433 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8423 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2436 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8429 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_type_formal: formal_type  */
#line 2438 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8435 "bison-chpl-lib.cpp"
    break;

  case 351: /* opt_fn_type_ret_type: %empty  */
#line 2442 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8441 "bison-chpl-lib.cpp"
    break;

  case 352: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2443 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8447 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2456 "chpl.ypp"
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
#line 8467 "bison-chpl-lib.cpp"
    break;

  case 354: /* $@7: %empty  */
#line 2475 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8477 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_expr: fn_type $@7 function_body_stmt  */
#line 2481 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    if (context->hasCurlyBracesLoc()) {
      fp.curlyLoc = context->curlyBracesLoc();
      context->resetCurlyBracesLoc();
    }
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8494 "bison-chpl-lib.cpp"
    break;

  case 356: /* $@8: %empty  */
#line 2494 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8504 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_expr: fn_type TALIAS $@8 expr  */
#line 2500 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8517 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2512 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8525 "bison-chpl-lib.cpp"
    break;

  case 359: /* $@9: %empty  */
#line 2521 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8533 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@9 opt_function_body_stmt  */
#line 2525 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-6].functionParts);
    fp.returnIntent = (yyvsp[-5].returnTag);
    fp.returnIntentLoc = (yylsp[-5]);
    fp.returnType = (yyvsp[-4].expr);
    fp.throws = ((yyvsp[-3].throwsTag) == ThrowsTag_THROWS);
    if ((yyvsp[-3].throwsTag) == ThrowsTag_THROWS) {
      fp.throwsLoc = (yylsp[-3]);
    }
    WhereAndLifetime wl = (yyvsp[-2].lifetimeAndWhere);
    fp.body = (yyvsp[0].exprList);
    if (context->hasCurlyBracesLoc()) {
      fp.curlyLoc = context->curlyBracesLoc();
      context->resetCurlyBracesLoc();
    }
    fp.lifetime = wl.lifetime;
    fp.where = wl.where;
    // conditionals are needed since bison has weird behavior when the rule is empty
    fp.headerLoc = context->makeSpannedLocation(fp.headerLoc,
      (yyvsp[-3].throwsTag) != ThrowsTag_DEFAULT ? (yylsp[-3]) :
        ((yyvsp[-4].expr) != nullptr ? (yylsp[-4]) :
          ((yyvsp[-5].returnTag).isValid && (Function::ReturnIntent)(yyvsp[-5].returnTag).intent != Function::DEFAULT_RETURN_INTENT ? (yylsp[-5]) : (yylsp[-6]))));
    context->exitScopeForFunctionDecl((yylsp[0]), fp);
    (yyval.functionParts) = fp;
  }
#line 8563 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2554 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.thisIntentLoc = (yylsp[-2]);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      // store location of the header to handle weird bison location issues
      // with empty rules
      fp.headerLoc =
        context->makeSpannedLocation((yylsp[-3]), (yyvsp[0].exprList) != context->parenlessMarker ? (yylsp[0]) : (yylsp[-1]));
      (yyval.functionParts) = fp;
    }
#line 8580 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2567 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.thisIntentLoc = (yylsp[-2]);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      // store location of the header to handle weird bison location issues
      // with empty rules
      fp.headerLoc =
        context->makeSpannedLocation((yylsp[-3]), (yyvsp[0].exprList) != context->parenlessMarker ? (yylsp[0]) : (yylsp[-1]));
      (yyval.functionParts) = fp;
    }
#line 8597 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2580 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.thisIntentLoc = (yylsp[-4]);
      fp.receiver = context->buildThisFormal((yylsp[-3]), (yylsp[-4]), (yyvsp[-4].intentTag), (yyvsp[-3].expr), /*initExpr*/ nullptr);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      // store location of the header to handle weird bison location issues
      // with empty rules
      fp.headerLoc =
        context->makeSpannedLocation((yylsp[-5]), (yyvsp[0].exprList) != context->parenlessMarker ? (yylsp[0]) : (yylsp[-1]));
      (yyval.functionParts) = fp;
    }
#line 8615 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2594 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.thisIntentLoc = (yylsp[-4]);
      fp.receiver = context->buildThisFormal((yylsp[-3]), (yylsp[-4]), (yyvsp[-4].intentTag), (yyvsp[-3].expr), /*initExpr*/ nullptr);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      // store location of the header to handle weird bison location issues
      // with empty rules
      fp.headerLoc =
        context->makeSpannedLocation((yylsp[-5]), (yyvsp[0].exprList) != context->parenlessMarker ? (yylsp[0]) : (yylsp[-1]));
      (yyval.functionParts) = fp;
    }
#line 8633 "bison-chpl-lib.cpp"
    break;

  case 365: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2608 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      // store location of the header to handle weird bison location issues
      // with empty rules
      fp.headerLoc =
        context->makeSpannedLocation((yylsp[-3]), (yyvsp[0].exprList) != context->parenlessMarker ? (yylsp[0]) : (yylsp[-1]));
      (yyval.functionParts) = fp;
    }
#line 8647 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2621 "chpl.ypp"
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
#line 8663 "bison-chpl-lib.cpp"
    break;

  case 368: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2637 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 8672 "bison-chpl-lib.cpp"
    break;

  case 371: /* fn_ident: ident_def TBANG  */
#line 2647 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8682 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2706 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8688 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2707 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8694 "bison-chpl-lib.cpp"
    break;

  case 415: /* opt_formal_ls: %empty  */
#line 2711 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; CHPL_NULL_YYLLOC((yyloc)); }
#line 8700 "bison-chpl-lib.cpp"
    break;

  case 416: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2712 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8706 "bison-chpl-lib.cpp"
    break;

  case 417: /* formal_ls_inner: formal  */
#line 2716 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8712 "bison-chpl-lib.cpp"
    break;

  case 418: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2717 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8718 "bison-chpl-lib.cpp"
    break;

  case 419: /* formal_ls: %empty  */
#line 2721 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8724 "bison-chpl-lib.cpp"
    break;

  case 420: /* formal_ls: formal_ls_inner  */
#line 2722 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8730 "bison-chpl-lib.cpp"
    break;

  case 422: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2731 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8738 "bison-chpl-lib.cpp"
    break;

  case 423: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2736 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8746 "bison-chpl-lib.cpp"
    break;

  case 424: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2741 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8754 "bison-chpl-lib.cpp"
    break;

  case 425: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2746 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8762 "bison-chpl-lib.cpp"
    break;

  case 426: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2751 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yylsp[-5]), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8770 "bison-chpl-lib.cpp"
    break;

  case 427: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2756 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8778 "bison-chpl-lib.cpp"
    break;

  case 428: /* opt_formal_intent_tag: %empty  */
#line 2762 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);
  }
#line 8787 "bison-chpl-lib.cpp"
    break;

  case 429: /* opt_formal_intent_tag: required_intent_tag  */
#line 2767 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8796 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TIN  */
#line 2774 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::IN);            }
#line 8802 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TINOUT  */
#line 2775 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::INOUT);         }
#line 8808 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TOUT  */
#line 2776 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::OUT);           }
#line 8814 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TCONST TIN  */
#line 2777 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_IN);      }
#line 8820 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TCONST TREF  */
#line 2778 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);     }
#line 8826 "bison-chpl-lib.cpp"
    break;

  case 435: /* required_intent_tag: TCONST  */
#line 2779 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);         }
#line 8832 "bison-chpl-lib.cpp"
    break;

  case 436: /* required_intent_tag: TPARAM  */
#line 2780 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);         }
#line 8838 "bison-chpl-lib.cpp"
    break;

  case 437: /* required_intent_tag: TREF  */
#line 2781 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);           }
#line 8844 "bison-chpl-lib.cpp"
    break;

  case 438: /* required_intent_tag: TTYPE  */
#line 2782 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);          }
#line 8850 "bison-chpl-lib.cpp"
    break;

  case 439: /* required_intent_tag: TVAR  */
#line 2783 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR); }
#line 8856 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: %empty  */
#line 2787 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);     }
#line 8862 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TPARAM  */
#line 2788 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);              }
#line 8868 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TREF  */
#line 2789 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);                }
#line 8874 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_this_intent_tag: TCONST TREF  */
#line 2790 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);          }
#line 8880 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_this_intent_tag: TCONST  */
#line 2791 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);              }
#line 8886 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_this_intent_tag: TTYPE  */
#line 2792 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);               }
#line 8892 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_this_intent_tag: TIN  */
#line 2793 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::IN);       }
#line 8898 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_this_intent_tag: TCONST TIN  */
#line 2794 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::CONST_IN); }
#line 8904 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_this_intent_tag: TVAR  */
#line 2795 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR);      }
#line 8910 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_this_intent_tag: TOUT  */
#line 2796 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::OUT);      }
#line 8916 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_this_intent_tag: TINOUT  */
#line 2797 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::INOUT);    }
#line 8922 "bison-chpl-lib.cpp"
    break;

  case 451: /* proc_iter_or_op: TPROC  */
#line 2801 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8928 "bison-chpl-lib.cpp"
    break;

  case 452: /* proc_iter_or_op: TITER  */
#line 2802 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8934 "bison-chpl-lib.cpp"
    break;

  case 453: /* proc_iter_or_op: TOPERATOR  */
#line 2803 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8940 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: %empty  */
#line 2807 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::DEFAULT_RETURN_INTENT); }
#line 8946 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: TOUT  */
#line 2808 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::OUT);                   }
#line 8952 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: TCONST  */
#line 2809 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST);                 }
#line 8958 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_ret_tag: TCONST TREF  */
#line 2810 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST_REF);             }
#line 8964 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_ret_tag: TREF  */
#line 2811 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::REF);                   }
#line 8970 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_ret_tag: TPARAM  */
#line 2812 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::PARAM);                 }
#line 8976 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_ret_tag: TTYPE  */
#line 2813 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::TYPE);                  }
#line 8982 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_ret_tag: TIN  */
#line 2814 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::IN);            }
#line 8988 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_ret_tag: TCONST TIN  */
#line 2815 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::CONST_IN);      }
#line 8994 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_ret_tag: TVAR  */
#line 2816 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::VAR);           }
#line 9000 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_ret_tag: TINOUT  */
#line 2817 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::INOUT);         }
#line 9006 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_throws_error: %empty  */
#line 2821 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9012 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_throws_error: TTHROWS  */
#line 2822 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9018 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_function_body_stmt: TSEMI  */
#line 2825 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9024 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_function_body_stmt: function_body_stmt  */
#line 2826 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 469: /* function_body_stmt: block_stmt_body  */
#line 2830 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9036 "bison-chpl-lib.cpp"
    break;

  case 470: /* function_body_stmt: TDO toplevel_stmt  */
#line 2831 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yylsp[0]), (yyvsp[0].commentsAndStmt)); }
#line 9042 "bison-chpl-lib.cpp"
    break;

  case 471: /* query_expr: TQUERIEDIDENT  */
#line 2835 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9048 "bison-chpl-lib.cpp"
    break;

  case 472: /* opt_lifetime_where: %empty  */
#line 2840 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9054 "bison-chpl-lib.cpp"
    break;

  case 473: /* opt_lifetime_where: TWHERE expr  */
#line 2842 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9060 "bison-chpl-lib.cpp"
    break;

  case 474: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2844 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9066 "bison-chpl-lib.cpp"
    break;

  case 475: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2846 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9072 "bison-chpl-lib.cpp"
    break;

  case 476: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2848 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9078 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_components_expr: lifetime_expr  */
#line 2853 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9084 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2855 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9090 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2860 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9096 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2862 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9102 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2864 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9108 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2866 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9114 "bison-chpl-lib.cpp"
    break;

  case 483: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2868 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9120 "bison-chpl-lib.cpp"
    break;

  case 484: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2870 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9126 "bison-chpl-lib.cpp"
    break;

  case 485: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2872 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9132 "bison-chpl-lib.cpp"
    break;

  case 486: /* lifetime_ident: TIDENT  */
#line 2876 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9138 "bison-chpl-lib.cpp"
    break;

  case 487: /* lifetime_ident: TINIT  */
#line 2877 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9144 "bison-chpl-lib.cpp"
    break;

  case 488: /* lifetime_ident: TTHIS  */
#line 2878 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9150 "bison-chpl-lib.cpp"
    break;

  case 489: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2883 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9159 "bison-chpl-lib.cpp"
    break;

  case 490: /* type_alias_decl_stmt_start: TTYPE  */
#line 2892 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9169 "bison-chpl-lib.cpp"
    break;

  case 491: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2898 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true, (yylsp[-1]));
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9180 "bison-chpl-lib.cpp"
    break;

  case 492: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2908 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9188 "bison-chpl-lib.cpp"
    break;

  case 493: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2912 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9196 "bison-chpl-lib.cpp"
    break;

  case 494: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2919 "chpl.ypp"
  {
    // TODO (dlongnecke-cray): Add a helper to build this and var_decl_stmt.
    auto node = Variable::build(BUILDER, LOC((yyloc)), LOC((yylsp[-1])),
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
#line 9218 "bison-chpl-lib.cpp"
    break;

  case 495: /* opt_init_type: %empty  */
#line 2939 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9224 "bison-chpl-lib.cpp"
    break;

  case 496: /* opt_init_type: TASSIGN expr  */
#line 2941 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9230 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_type: TPARAM  */
#line 2945 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9236 "bison-chpl-lib.cpp"
    break;

  case 498: /* var_decl_type: TCONST TREF  */
#line 2946 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9242 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_type: TREF  */
#line 2947 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9248 "bison-chpl-lib.cpp"
    break;

  case 500: /* var_decl_type: TCONST  */
#line 2948 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9254 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_type: TVAR  */
#line 2949 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9260 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_start: var_decl_type  */
#line 2954 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9269 "bison-chpl-lib.cpp"
    break;

  case 503: /* $@10: %empty  */
#line 2962 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true, (yylsp[0]));
  }
#line 9278 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_stmt: TCONFIG $@10 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2966 "chpl.ypp"
                                              {
    CHPL_ASSERT(context->isVarDeclConfig);
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9288 "bison-chpl-lib.cpp"
    break;

  case 505: /* $@11: %empty  */
#line 2972 "chpl.ypp"
  {
    // Use a mid-rule action to preserve the 'on' expr
    context->noteVarDestinationExpr((yyvsp[0].expr));
  }
#line 9297 "bison-chpl-lib.cpp"
    break;

  case 506: /* var_decl_stmt: TON expr $@11 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2977 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9306 "bison-chpl-lib.cpp"
    break;

  case 507: /* var_decl_stmt: var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2982 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9315 "bison-chpl-lib.cpp"
    break;

  case 508: /* var_decl_stmt: var_decl_start error TSEMI  */
#line 2987 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = makeCommentsAndStmt(nullptr, ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release());
    delete (yyvsp[-2].attributeGroup);
    context->resetDeclStateOnError();
  }
#line 9325 "bison-chpl-lib.cpp"
    break;

  case 509: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2996 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9333 "bison-chpl-lib.cpp"
    break;

  case 510: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 3000 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9341 "bison-chpl-lib.cpp"
    break;

  case 511: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 3007 "chpl.ypp"
    {
      // delay building the attributes for a variable, then update it later
      // in the context during the call to buildVarOrMultiDeclStmt
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)), LOC((yylsp[-2])),
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
#line 9363 "bison-chpl-lib.cpp"
    break;

  case 512: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 3025 "chpl.ypp"
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
#line 9383 "bison-chpl-lib.cpp"
    break;

  case 513: /* tuple_var_decl_component: ident_def  */
#line 3044 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9391 "bison-chpl-lib.cpp"
    break;

  case 514: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 3048 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9399 "bison-chpl-lib.cpp"
    break;

  case 515: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 3055 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9405 "bison-chpl-lib.cpp"
    break;

  case 516: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 3057 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9411 "bison-chpl-lib.cpp"
    break;

  case 517: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 3059 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9417 "bison-chpl-lib.cpp"
    break;

  case 518: /* opt_init_expr: %empty  */
#line 3065 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9423 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_init_expr: TASSIGN TNOINIT  */
#line 3066 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9429 "bison-chpl-lib.cpp"
    break;

  case 520: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 3067 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9435 "bison-chpl-lib.cpp"
    break;

  case 521: /* formal_or_ret_type_expr: expr  */
#line 3071 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9441 "bison-chpl-lib.cpp"
    break;

  case 522: /* ret_type: formal_or_ret_type_expr  */
#line 3075 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9447 "bison-chpl-lib.cpp"
    break;

  case 523: /* ret_type: reserved_type_ident_use  */
#line 3076 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9453 "bison-chpl-lib.cpp"
    break;

  case 524: /* ret_type: error  */
#line 3077 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9459 "bison-chpl-lib.cpp"
    break;

  case 525: /* colon_ret_type: TCOLON ret_type  */
#line 3081 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9465 "bison-chpl-lib.cpp"
    break;

  case 526: /* colon_ret_type: error  */
#line 3082 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9471 "bison-chpl-lib.cpp"
    break;

  case 527: /* opt_ret_type: %empty  */
#line 3086 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9477 "bison-chpl-lib.cpp"
    break;

  case 529: /* opt_type: %empty  */
#line 3091 "chpl.ypp"
                                 { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9483 "bison-chpl-lib.cpp"
    break;

  case 530: /* opt_type: TCOLON expr  */
#line 3092 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9489 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_type: TCOLON reserved_type_ident_use  */
#line 3093 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9495 "bison-chpl-lib.cpp"
    break;

  case 532: /* opt_type: error  */
#line 3094 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9501 "bison-chpl-lib.cpp"
    break;

  case 533: /* formal_type: formal_or_ret_type_expr  */
#line 3098 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9507 "bison-chpl-lib.cpp"
    break;

  case 534: /* formal_type: reserved_type_ident_use  */
#line 3099 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9513 "bison-chpl-lib.cpp"
    break;

  case 535: /* colon_formal_type: TCOLON formal_type  */
#line 3103 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9519 "bison-chpl-lib.cpp"
    break;

  case 536: /* opt_colon_formal_type: %empty  */
#line 3107 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9525 "bison-chpl-lib.cpp"
    break;

  case 537: /* opt_colon_formal_type: colon_formal_type  */
#line 3108 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9531 "bison-chpl-lib.cpp"
    break;

  case 538: /* expr_ls: expr  */
#line 3114 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9537 "bison-chpl-lib.cpp"
    break;

  case 539: /* expr_ls: expr_ls TCOMMA expr  */
#line 3115 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9543 "bison-chpl-lib.cpp"
    break;

  case 540: /* expr_ls_end_semi: expr_ls semicolon_list  */
#line 3119 "chpl.ypp"
                         {
    (yyval.ndArrayList) = context->makeNDArrayList();
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElement((yyvsp[-1].exprList), (yylsp[-1])));
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9553 "bison-chpl-lib.cpp"
    break;

  case 541: /* expr_ls_end_semi: expr_ls TCOMMA semicolon_list  */
#line 3124 "chpl.ypp"
                                {
    (yyval.ndArrayList) = context->makeNDArrayList();
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElement((yyvsp[-2].exprList), (yylsp[-2])));
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9563 "bison-chpl-lib.cpp"
    break;

  case 542: /* expr_ls_end_semi: expr_ls_semi semicolon_list  */
#line 3130 "chpl.ypp"
  {
    (yyval.ndArrayList) = (yyvsp[-1].ndArrayList);
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9572 "bison-chpl-lib.cpp"
    break;

  case 543: /* expr_ls_semi: expr_ls_end_semi expr_ls  */
#line 3137 "chpl.ypp"
                           {
    (yyval.ndArrayList) = context->appendNDArrayList((yyvsp[-1].ndArrayList), makeNDArrayElement((yyvsp[0].exprList), (yylsp[0])));
  }
#line 9580 "bison-chpl-lib.cpp"
    break;

  case 544: /* expr_ls_semi: expr_ls_end_semi expr_ls TCOMMA  */
#line 3140 "chpl.ypp"
                                  {
    (yyval.ndArrayList) = context->appendNDArrayList((yyvsp[-2].ndArrayList), makeNDArrayElement((yyvsp[-1].exprList), (yylsp[-1])));
  }
#line 9588 "bison-chpl-lib.cpp"
    break;

  case 545: /* semicolon_list: TSEMI  */
#line 3146 "chpl.ypp"
                       { (yyval.counter) = 1; }
#line 9594 "bison-chpl-lib.cpp"
    break;

  case 546: /* semicolon_list: semicolon_list TSEMI  */
#line 3147 "chpl.ypp"
                       { (yyval.counter) = (yyvsp[-1].counter) + 1; }
#line 9600 "bison-chpl-lib.cpp"
    break;

  case 547: /* tuple_component: opt_try_expr  */
#line 3153 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9606 "bison-chpl-lib.cpp"
    break;

  case 548: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3158 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9614 "bison-chpl-lib.cpp"
    break;

  case 549: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3162 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9622 "bison-chpl-lib.cpp"
    break;

  case 550: /* opt_actual_ls: %empty  */
#line 3168 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9628 "bison-chpl-lib.cpp"
    break;

  case 551: /* opt_actual_ls: actual_ls  */
#line 3169 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9634 "bison-chpl-lib.cpp"
    break;

  case 552: /* actual_ls: actual_expr  */
#line 3174 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9643 "bison-chpl-lib.cpp"
    break;

  case 553: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3179 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9653 "bison-chpl-lib.cpp"
    break;

  case 554: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3187 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9659 "bison-chpl-lib.cpp"
    break;

  case 555: /* actual_expr: opt_try_expr  */
#line 3188 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9665 "bison-chpl-lib.cpp"
    break;

  case 556: /* ident_expr: ident_use  */
#line 3192 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9671 "bison-chpl-lib.cpp"
    break;

  case 557: /* ident_expr: scalar_type  */
#line 3193 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9677 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3206 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9683 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3208 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9689 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3210 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9695 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3212 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9705 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TATOMIC expr  */
#line 3218 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9711 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TSYNC expr  */
#line 3220 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9717 "bison-chpl-lib.cpp"
    break;

  case 569: /* sub_type_level_expr: TOWNED  */
#line 3222 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9723 "bison-chpl-lib.cpp"
    break;

  case 570: /* sub_type_level_expr: TOWNED expr  */
#line 3224 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9729 "bison-chpl-lib.cpp"
    break;

  case 571: /* sub_type_level_expr: TUNMANAGED  */
#line 3226 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9735 "bison-chpl-lib.cpp"
    break;

  case 572: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3228 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9741 "bison-chpl-lib.cpp"
    break;

  case 573: /* sub_type_level_expr: TSHARED  */
#line 3230 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9747 "bison-chpl-lib.cpp"
    break;

  case 574: /* sub_type_level_expr: TSHARED expr  */
#line 3232 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9753 "bison-chpl-lib.cpp"
    break;

  case 575: /* sub_type_level_expr: TBORROWED  */
#line 3234 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9759 "bison-chpl-lib.cpp"
    break;

  case 576: /* sub_type_level_expr: TBORROWED expr  */
#line 3236 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9765 "bison-chpl-lib.cpp"
    break;

  case 577: /* sub_type_level_expr: TCLASS  */
#line 3238 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9771 "bison-chpl-lib.cpp"
    break;

  case 578: /* sub_type_level_expr: TRECORD  */
#line 3240 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9777 "bison-chpl-lib.cpp"
    break;

  case 579: /* sub_type_level_expr: TUNION  */
#line 3242 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9783 "bison-chpl-lib.cpp"
    break;

  case 580: /* for_expr: no_loop_attributes for_expr_base  */
#line 3246 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9789 "bison-chpl-lib.cpp"
    break;

  case 581: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3251 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9797 "bison-chpl-lib.cpp"
    break;

  case 582: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3255 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9805 "bison-chpl-lib.cpp"
    break;

  case 583: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3259 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9813 "bison-chpl-lib.cpp"
    break;

  case 584: /* for_expr_base: for_loop_kw zippered_iterator TDO expr  */
#line 3263 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "expression-level loops with 'zip' must have an index.");
  }
#line 9821 "bison-chpl-lib.cpp"
    break;

  case 585: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3267 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9834 "bison-chpl-lib.cpp"
    break;

  case 586: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3276 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9847 "bison-chpl-lib.cpp"
    break;

  case 587: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3285 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9860 "bison-chpl-lib.cpp"
    break;

  case 588: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3296 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9866 "bison-chpl-lib.cpp"
    break;

  case 589: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3301 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9874 "bison-chpl-lib.cpp"
    break;

  case 590: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3305 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9882 "bison-chpl-lib.cpp"
    break;

  case 591: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3309 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9890 "bison-chpl-lib.cpp"
    break;

  case 592: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3313 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9898 "bison-chpl-lib.cpp"
    break;

  case 593: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3317 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9906 "bison-chpl-lib.cpp"
    break;

  case 594: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3321 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9914 "bison-chpl-lib.cpp"
    break;

  case 595: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3325 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9922 "bison-chpl-lib.cpp"
    break;

  case 596: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3332 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9936 "bison-chpl-lib.cpp"
    break;

  case 597: /* nil_expr: TNIL  */
#line 3349 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9942 "bison-chpl-lib.cpp"
    break;

  case 604: /* opt_task_intent_ls: %empty  */
#line 3366 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9948 "bison-chpl-lib.cpp"
    break;

  case 605: /* opt_task_intent_ls: task_intent_clause  */
#line 3367 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9954 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3372 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9962 "bison-chpl-lib.cpp"
    break;

  case 607: /* task_intent_clause: TWITH TLP TRP  */
#line 3376 "chpl.ypp"
  {
    auto err = context->syntax((yyloc), "'with' clause cannot be empty.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-2]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9973 "bison-chpl-lib.cpp"
    break;

  case 608: /* task_intent_clause: TWITH TLP error TRP  */
#line 3383 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[-2]), "invalid intent expression in 'with' clause.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9984 "bison-chpl-lib.cpp"
    break;

  case 609: /* task_intent_clause: TWITH error  */
#line 3390 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[0]), "missing parentheses around 'with' clause intents.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-1]), dummyLoc, dummyLoc, dummyLoc, exprs);
  }
#line 9995 "bison-chpl-lib.cpp"
    break;

  case 610: /* task_intent_ls: intent_expr  */
#line 3399 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10001 "bison-chpl-lib.cpp"
    break;

  case 611: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3400 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10007 "bison-chpl-lib.cpp"
    break;

  case 612: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3405 "chpl.ypp"
  {
    (yyval.expr) = context->buildTaskIntent((yyloc), (yylsp[-2]), nullptr, (yyvsp[-2].expr), (yyvsp[-3].taskIntent), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 10015 "bison-chpl-lib.cpp"
    break;

  case 613: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3409 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 10023 "bison-chpl-lib.cpp"
    break;

  case 614: /* intent_expr: expr TREDUCE ident_expr  */
#line 3413 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 10031 "bison-chpl-lib.cpp"
    break;

  case 615: /* task_var_prefix: TCONST  */
#line 3419 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST);          }
#line 10037 "bison-chpl-lib.cpp"
    break;

  case 616: /* task_var_prefix: TIN  */
#line 3420 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::IN);             }
#line 10043 "bison-chpl-lib.cpp"
    break;

  case 617: /* task_var_prefix: TCONST TIN  */
#line 3421 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_IN);       }
#line 10049 "bison-chpl-lib.cpp"
    break;

  case 618: /* task_var_prefix: TREF  */
#line 3422 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::REF);            }
#line 10055 "bison-chpl-lib.cpp"
    break;

  case 619: /* task_var_prefix: TCONST TREF  */
#line 3423 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_REF);      }
#line 10061 "bison-chpl-lib.cpp"
    break;

  case 620: /* task_var_prefix: TVAR  */
#line 3424 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::VAR);            }
#line 10067 "bison-chpl-lib.cpp"
    break;

  case 621: /* task_var_prefix: TOUT  */
#line 3425 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::OUT);   }
#line 10073 "bison-chpl-lib.cpp"
    break;

  case 622: /* task_var_prefix: TINOUT  */
#line 3426 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::INOUT); }
#line 10079 "bison-chpl-lib.cpp"
    break;

  case 623: /* task_var_prefix: TPARAM  */
#line 3427 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::PARAM); }
#line 10085 "bison-chpl-lib.cpp"
    break;

  case 624: /* task_var_prefix: TTYPE  */
#line 3428 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::TYPE);  }
#line 10091 "bison-chpl-lib.cpp"
    break;

  case 625: /* new_maybe_decorated: TNEW  */
#line 3433 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10097 "bison-chpl-lib.cpp"
    break;

  case 626: /* new_maybe_decorated: TNEW TOWNED  */
#line 3435 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10103 "bison-chpl-lib.cpp"
    break;

  case 627: /* new_maybe_decorated: TNEW TSHARED  */
#line 3437 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10109 "bison-chpl-lib.cpp"
    break;

  case 628: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3439 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10115 "bison-chpl-lib.cpp"
    break;

  case 629: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3441 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10121 "bison-chpl-lib.cpp"
    break;

  case 630: /* new_expr: new_maybe_decorated expr  */
#line 3447 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10129 "bison-chpl-lib.cpp"
    break;

  case 631: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3454 "chpl.ypp"
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
#line 10146 "bison-chpl-lib.cpp"
    break;

  case 632: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3467 "chpl.ypp"
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
#line 10163 "bison-chpl-lib.cpp"
    break;

  case 633: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3480 "chpl.ypp"
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
#line 10181 "bison-chpl-lib.cpp"
    break;

  case 634: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3494 "chpl.ypp"
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
#line 10200 "bison-chpl-lib.cpp"
    break;

  case 635: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3512 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10208 "bison-chpl-lib.cpp"
    break;

  case 636: /* range_literal_expr: expr TDOTDOT expr  */
#line 3519 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10217 "bison-chpl-lib.cpp"
    break;

  case 637: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3524 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10226 "bison-chpl-lib.cpp"
    break;

  case 638: /* range_literal_expr: expr TDOTDOT  */
#line 3529 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10235 "bison-chpl-lib.cpp"
    break;

  case 639: /* range_literal_expr: TDOTDOT expr  */
#line 3534 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10245 "bison-chpl-lib.cpp"
    break;

  case 640: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3540 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10255 "bison-chpl-lib.cpp"
    break;

  case 641: /* range_literal_expr: TDOTDOT  */
#line 3546 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10265 "bison-chpl-lib.cpp"
    break;

  case 642: /* cast_expr: expr TCOLON expr  */
#line 3576 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10273 "bison-chpl-lib.cpp"
    break;

  case 643: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3583 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10281 "bison-chpl-lib.cpp"
    break;

  case 644: /* super_expr: fn_expr  */
#line 3589 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10287 "bison-chpl-lib.cpp"
    break;

  case 648: /* expr: sub_type_level_expr TQUESTION  */
#line 3598 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10293 "bison-chpl-lib.cpp"
    break;

  case 649: /* expr: TQUESTION  */
#line 3600 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10299 "bison-chpl-lib.cpp"
    break;

  case 653: /* expr: fn_type  */
#line 3605 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10305 "bison-chpl-lib.cpp"
    break;

  case 662: /* opt_expr: %empty  */
#line 3618 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10311 "bison-chpl-lib.cpp"
    break;

  case 663: /* opt_expr: expr  */
#line 3619 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10317 "bison-chpl-lib.cpp"
    break;

  case 664: /* opt_try_expr: TTRY expr  */
#line 3623 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10323 "bison-chpl-lib.cpp"
    break;

  case 665: /* opt_try_expr: TTRYBANG expr  */
#line 3624 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10329 "bison-chpl-lib.cpp"
    break;

  case 666: /* opt_try_expr: super_expr  */
#line 3625 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10335 "bison-chpl-lib.cpp"
    break;

  case 672: /* call_base_expr: expr TBANG  */
#line 3642 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10343 "bison-chpl-lib.cpp"
    break;

  case 673: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3645 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10349 "bison-chpl-lib.cpp"
    break;

  case 675: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3651 "chpl.ypp"
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
#line 10365 "bison-chpl-lib.cpp"
    break;

  case 676: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3663 "chpl.ypp"
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
#line 10381 "bison-chpl-lib.cpp"
    break;

  case 677: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3675 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10389 "bison-chpl-lib.cpp"
    break;

  case 678: /* dot_expr: expr TDOT ident_use  */
#line 3682 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10395 "bison-chpl-lib.cpp"
    break;

  case 679: /* dot_expr: expr TDOT TTYPE  */
#line 3684 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10401 "bison-chpl-lib.cpp"
    break;

  case 680: /* dot_expr: expr TDOT TDOMAIN  */
#line 3686 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10407 "bison-chpl-lib.cpp"
    break;

  case 681: /* dot_expr: expr TDOT TLOCALE  */
#line 3688 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10413 "bison-chpl-lib.cpp"
    break;

  case 682: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3690 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10419 "bison-chpl-lib.cpp"
    break;

  case 683: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3692 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10425 "bison-chpl-lib.cpp"
    break;

  case 684: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3701 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10435 "bison-chpl-lib.cpp"
    break;

  case 685: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3707 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10443 "bison-chpl-lib.cpp"
    break;

  case 686: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3711 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10451 "bison-chpl-lib.cpp"
    break;

  case 687: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3715 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10459 "bison-chpl-lib.cpp"
    break;

  case 688: /* bool_literal: TFALSE  */
#line 3721 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10465 "bison-chpl-lib.cpp"
    break;

  case 689: /* bool_literal: TTRUE  */
#line 3722 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10471 "bison-chpl-lib.cpp"
    break;

  case 690: /* str_bytes_literal: STRINGLITERAL  */
#line 3726 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10477 "bison-chpl-lib.cpp"
    break;

  case 691: /* str_bytes_literal: BYTESLITERAL  */
#line 3727 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10483 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: INTLITERAL  */
#line 3733 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10489 "bison-chpl-lib.cpp"
    break;

  case 695: /* literal_expr: REALLITERAL  */
#line 3734 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10495 "bison-chpl-lib.cpp"
    break;

  case 696: /* literal_expr: IMAGLITERAL  */
#line 3735 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10501 "bison-chpl-lib.cpp"
    break;

  case 697: /* literal_expr: TNONE  */
#line 3736 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10507 "bison-chpl-lib.cpp"
    break;

  case 698: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3738 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10516 "bison-chpl-lib.cpp"
    break;

  case 699: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3743 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10525 "bison-chpl-lib.cpp"
    break;

  case 700: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3748 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10534 "bison-chpl-lib.cpp"
    break;

  case 701: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3753 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10543 "bison-chpl-lib.cpp"
    break;

  case 702: /* literal_expr: no_loop_attributes TLSBR expr_ls_semi TRSBR  */
#line 3758 "chpl.ypp"
  {
    (yyval.expr) = context->buildNDArray((yyloc), (yyvsp[-1].ndArrayList));
    context->popLoopAttributeGroup();
  }
#line 10552 "bison-chpl-lib.cpp"
    break;

  case 703: /* literal_expr: no_loop_attributes TLSBR expr_ls_semi semicolon_list TRSBR  */
#line 3763 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    (yyval.expr) = context->buildNDArray((yyloc), (yyvsp[-2].ndArrayList));
    context->popLoopAttributeGroup();
  }
#line 10564 "bison-chpl-lib.cpp"
    break;

  case 704: /* literal_expr: no_loop_attributes TLSBR expr_ls semicolon_list TRSBR  */
#line 3771 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    AstList lst;
    lst.push_back(ArrayRow::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))));
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), std::move(lst)).release();
    context->popLoopAttributeGroup();
  }
#line 10578 "bison-chpl-lib.cpp"
    break;

  case 705: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA semicolon_list TRSBR  */
#line 3781 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    AstList lst;
    lst.push_back(ArrayRow::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-3].exprList))));
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), std::move(lst)).release();
    context->popLoopAttributeGroup();
  }
#line 10592 "bison-chpl-lib.cpp"
    break;

  case 706: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3791 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10601 "bison-chpl-lib.cpp"
    break;

  case 707: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3796 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10610 "bison-chpl-lib.cpp"
    break;

  case 708: /* assoc_expr_ls: expr TALIAS expr  */
#line 3805 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10619 "bison-chpl-lib.cpp"
    break;

  case 709: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3810 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10629 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TPLUS expr  */
#line 3818 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10635 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TMINUS expr  */
#line 3819 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10641 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TSTAR expr  */
#line 3820 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10647 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TDIVIDE expr  */
#line 3821 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10653 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3822 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10659 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3823 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10665 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TMOD expr  */
#line 3824 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10671 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TEQUAL expr  */
#line 3825 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10677 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3826 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10683 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3827 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10689 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3828 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10695 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr TLESS expr  */
#line 3829 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10701 "bison-chpl-lib.cpp"
    break;

  case 722: /* binary_op_expr: expr TGREATER expr  */
#line 3830 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10707 "bison-chpl-lib.cpp"
    break;

  case 723: /* binary_op_expr: expr TBAND expr  */
#line 3831 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10713 "bison-chpl-lib.cpp"
    break;

  case 724: /* binary_op_expr: expr TBOR expr  */
#line 3832 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10719 "bison-chpl-lib.cpp"
    break;

  case 725: /* binary_op_expr: expr TBXOR expr  */
#line 3833 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10725 "bison-chpl-lib.cpp"
    break;

  case 726: /* binary_op_expr: expr TAND expr  */
#line 3834 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10731 "bison-chpl-lib.cpp"
    break;

  case 727: /* binary_op_expr: expr TOR expr  */
#line 3835 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10737 "bison-chpl-lib.cpp"
    break;

  case 728: /* binary_op_expr: expr TEXP expr  */
#line 3836 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10743 "bison-chpl-lib.cpp"
    break;

  case 729: /* binary_op_expr: expr TBY expr  */
#line 3837 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10749 "bison-chpl-lib.cpp"
    break;

  case 730: /* binary_op_expr: expr TALIGN expr  */
#line 3838 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10755 "bison-chpl-lib.cpp"
    break;

  case 731: /* binary_op_expr: expr THASH expr  */
#line 3839 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10761 "bison-chpl-lib.cpp"
    break;

  case 732: /* binary_op_expr: expr TDMAPPED expr  */
#line 3840 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10767 "bison-chpl-lib.cpp"
    break;

  case 733: /* unary_op_expr: TPLUS expr  */
#line 3844 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10773 "bison-chpl-lib.cpp"
    break;

  case 734: /* unary_op_expr: TMINUS expr  */
#line 3845 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10779 "bison-chpl-lib.cpp"
    break;

  case 735: /* unary_op_expr: TMINUSMINUS expr  */
#line 3846 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10785 "bison-chpl-lib.cpp"
    break;

  case 736: /* unary_op_expr: TPLUSPLUS expr  */
#line 3847 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10791 "bison-chpl-lib.cpp"
    break;

  case 737: /* unary_op_expr: TBANG expr  */
#line 3848 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10797 "bison-chpl-lib.cpp"
    break;

  case 738: /* unary_op_expr: expr TBANG  */
#line 3849 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10805 "bison-chpl-lib.cpp"
    break;

  case 739: /* unary_op_expr: TBNOT expr  */
#line 3852 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10811 "bison-chpl-lib.cpp"
    break;

  case 740: /* reduce_expr: expr TREDUCE expr  */
#line 3857 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10819 "bison-chpl-lib.cpp"
    break;

  case 741: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3861 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10827 "bison-chpl-lib.cpp"
    break;

  case 742: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3865 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10835 "bison-chpl-lib.cpp"
    break;

  case 743: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3869 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10843 "bison-chpl-lib.cpp"
    break;

  case 744: /* scan_expr: expr TSCAN expr  */
#line 3876 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10851 "bison-chpl-lib.cpp"
    break;

  case 745: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3880 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10859 "bison-chpl-lib.cpp"
    break;

  case 746: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3884 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10867 "bison-chpl-lib.cpp"
    break;

  case 747: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3888 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10875 "bison-chpl-lib.cpp"
    break;


#line 10879 "bison-chpl-lib.cpp"

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
