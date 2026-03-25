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
  YYSYMBOL_manager_expr = 261,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 262,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 263,              /* manage_stmt  */
  YYSYMBOL_class_decl_stmt = 264,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 265,              /* class_start  */
  YYSYMBOL_class_tag = 266,                /* class_tag  */
  YYSYMBOL_opt_inherit = 267,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 268,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 269,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header = 270,              /* enum_header  */
  YYSYMBOL_enum_ls = 271,                  /* enum_ls  */
  YYSYMBOL_272_5 = 272,                    /* $@5  */
  YYSYMBOL_273_6 = 273,                    /* $@6  */
  YYSYMBOL_enum_item = 274,                /* enum_item  */
  YYSYMBOL_linkage_spec_empty = 275,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 276,             /* linkage_spec  */
  YYSYMBOL_opt_fn_type_formal_ls = 277,    /* opt_fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal_ls = 278,        /* fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal = 279,           /* fn_type_formal  */
  YYSYMBOL_opt_fn_type_ret_type = 280,     /* opt_fn_type_ret_type  */
  YYSYMBOL_fn_type = 281,                  /* fn_type  */
  YYSYMBOL_fn_expr = 282,                  /* fn_expr  */
  YYSYMBOL_283_7 = 283,                    /* $@7  */
  YYSYMBOL_284_8 = 284,                    /* $@8  */
  YYSYMBOL_fn_decl_stmt_complete = 285,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 286,             /* fn_decl_stmt  */
  YYSYMBOL_287_9 = 287,                    /* $@9  */
  YYSYMBOL_fn_decl_stmt_inner = 288,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 289,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 290,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 291,                 /* fn_ident  */
  YYSYMBOL_op_ident = 292,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 293,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 294,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 295,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 296,            /* opt_formal_ls  */
  YYSYMBOL_formal_ls_inner = 297,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 298,                /* formal_ls  */
  YYSYMBOL_formal = 299,                   /* formal  */
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
  YYSYMBOL_var_decl_start = 320,           /* var_decl_start  */
  YYSYMBOL_var_decl_stmt = 321,            /* var_decl_stmt  */
  YYSYMBOL_322_10 = 322,                   /* $@10  */
  YYSYMBOL_323_11 = 323,                   /* $@11  */
  YYSYMBOL_var_decl_stmt_inner_ls = 324,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 325,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 326, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 327, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 328,            /* opt_init_expr  */
  YYSYMBOL_formal_or_ret_type_expr = 329,  /* formal_or_ret_type_expr  */
  YYSYMBOL_ret_type = 330,                 /* ret_type  */
  YYSYMBOL_colon_ret_type = 331,           /* colon_ret_type  */
  YYSYMBOL_opt_ret_type = 332,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 333,                 /* opt_type  */
  YYSYMBOL_formal_type = 334,              /* formal_type  */
  YYSYMBOL_colon_formal_type = 335,        /* colon_formal_type  */
  YYSYMBOL_opt_colon_formal_type = 336,    /* opt_colon_formal_type  */
  YYSYMBOL_expr_ls = 337,                  /* expr_ls  */
  YYSYMBOL_expr_ls_end_semi = 338,         /* expr_ls_end_semi  */
  YYSYMBOL_expr_ls_semi = 339,             /* expr_ls_semi  */
  YYSYMBOL_semicolon_list = 340,           /* semicolon_list  */
  YYSYMBOL_tuple_component = 341,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 342,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 343,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 344,                /* actual_ls  */
  YYSYMBOL_actual_expr = 345,              /* actual_expr  */
  YYSYMBOL_ident_expr = 346,               /* ident_expr  */
  YYSYMBOL_sub_type_level_expr = 347,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 348,                 /* for_expr  */
  YYSYMBOL_for_expr_base = 349,            /* for_expr_base  */
  YYSYMBOL_bracket_loop_expr = 350,        /* bracket_loop_expr  */
  YYSYMBOL_bracket_loop_expr_base = 351,   /* bracket_loop_expr_base  */
  YYSYMBOL_cond_expr = 352,                /* cond_expr  */
  YYSYMBOL_nil_expr = 353,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 354,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 355,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 356,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 357,           /* task_intent_ls  */
  YYSYMBOL_intent_expr = 358,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 359,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 360,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 361,                 /* new_expr  */
  YYSYMBOL_let_expr = 362,                 /* let_expr  */
  YYSYMBOL_range_literal_expr = 363,       /* range_literal_expr  */
  YYSYMBOL_cast_expr = 364,                /* cast_expr  */
  YYSYMBOL_tuple_expand_expr = 365,        /* tuple_expand_expr  */
  YYSYMBOL_super_expr = 366,               /* super_expr  */
  YYSYMBOL_expr = 367,                     /* expr  */
  YYSYMBOL_opt_expr = 368,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 369,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 370,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 371,           /* call_base_expr  */
  YYSYMBOL_call_expr = 372,                /* call_expr  */
  YYSYMBOL_dot_expr = 373,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 374,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 375,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 376,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 377,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 378,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 379,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 380,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 381,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 382,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 383       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 382 "chpl.ypp"

  // include the definition of methods for ParserContext
  #include "ParserContextImpl.h"
  // include helper macros for use in writing productions
  #include "parser-help.h"
  // include implementation of error logging hook
  #include "parser-yyerror.h"

#line 507 "bison-chpl-lib.cpp"

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
#define YYLAST   17436

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  179
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  205
/* YYNRULES -- Number of rules.  */
#define YYNRULES  744
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1258

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
       0,   691,   691,   695,   696,   701,   702,   710,   714,   724,
     729,   736,   742,   744,   745,   746,   747,   748,   749,   750,
     751,   768,   769,   770,   778,   787,   796,   800,   807,   815,
     816,   817,   818,   819,   820,   821,   822,   824,   843,   851,
     867,   882,   898,   914,   929,   949,   950,   951,   955,   961,
     962,   966,   970,   971,   975,   982,   990,   993,  1003,  1005,
    1010,  1016,  1021,  1030,  1034,  1055,  1073,  1091,  1110,  1111,
    1113,  1118,  1119,  1124,  1142,  1147,  1152,  1164,  1187,  1188,
    1192,  1196,  1200,  1205,  1209,  1213,  1221,  1226,  1232,  1237,
    1246,  1247,  1251,  1256,  1264,  1273,  1281,  1292,  1300,  1301,
    1302,  1303,  1304,  1305,  1306,  1307,  1308,  1309,  1310,  1311,
    1312,  1313,  1314,  1315,  1327,  1331,  1336,  1341,  1347,  1356,
    1357,  1361,  1370,  1374,  1378,  1382,  1391,  1392,  1393,  1397,
    1398,  1399,  1400,  1401,  1405,  1406,  1407,  1419,  1420,  1421,
    1426,  1427,  1428,  1429,  1430,  1431,  1432,  1433,  1434,  1435,
    1436,  1437,  1438,  1439,  1440,  1441,  1442,  1443,  1447,  1448,
    1449,  1450,  1451,  1452,  1453,  1454,  1455,  1456,  1457,  1458,
    1465,  1466,  1467,  1471,  1472,  1476,  1483,  1493,  1497,  1503,
    1503,  1510,  1510,  1520,  1521,  1522,  1523,  1524,  1525,  1526,
    1530,  1535,  1540,  1545,  1552,  1560,  1561,  1565,  1578,  1586,
    1594,  1604,  1623,  1622,  1632,  1631,  1644,  1651,  1657,  1658,
    1667,  1668,  1669,  1670,  1674,  1696,  1714,  1732,  1756,  1761,
    1766,  1771,  1776,  1781,  1786,  1791,  1796,  1801,  1806,  1811,
    1816,  1821,  1826,  1832,  1841,  1850,  1854,  1858,  1862,  1866,
    1870,  1874,  1878,  1882,  1887,  1892,  1897,  1905,  1920,  1938,
    1945,  1952,  1962,  1963,  1968,  1973,  1974,  1975,  1976,  1977,
    1978,  1979,  1980,  1981,  1982,  1983,  1984,  1989,  1994,  1995,
    1996,  1997,  2005,  2006,  2010,  2014,  2018,  2025,  2029,  2033,
    2040,  2044,  2048,  2052,  2059,  2060,  2064,  2068,  2072,  2079,
    2094,  2112,  2120,  2125,  2135,  2136,  2140,  2144,  2149,  2158,
    2163,  2167,  2174,  2175,  2179,  2189,  2196,  2209,  2216,  2217,
    2218,  2222,  2223,  2227,  2231,  2235,  2239,  2243,  2251,  2271,
    2281,  2295,  2302,  2307,  2314,  2313,  2323,  2329,  2328,  2343,
    2351,  2363,  2367,  2368,  2370,  2375,  2376,  2380,  2381,  2385,
    2388,  2390,  2395,  2396,  2407,  2428,  2427,  2447,  2446,  2464,
    2474,  2471,  2503,  2516,  2529,  2543,  2557,  2570,  2585,  2586,
    2594,  2595,  2596,  2605,  2606,  2607,  2608,  2609,  2610,  2611,
    2612,  2613,  2614,  2615,  2616,  2617,  2618,  2619,  2620,  2621,
    2622,  2623,  2624,  2625,  2626,  2627,  2628,  2629,  2633,  2634,
    2635,  2636,  2637,  2638,  2639,  2640,  2641,  2642,  2643,  2644,
    2645,  2646,  2651,  2652,  2656,  2657,  2661,  2662,  2666,  2667,
    2671,  2672,  2676,  2680,  2684,  2689,  2694,  2699,  2704,  2712,
    2716,  2724,  2725,  2726,  2727,  2728,  2729,  2730,  2731,  2732,
    2733,  2737,  2738,  2739,  2740,  2741,  2742,  2743,  2744,  2745,
    2746,  2747,  2751,  2752,  2753,  2757,  2758,  2759,  2760,  2761,
    2762,  2763,  2764,  2765,  2766,  2767,  2771,  2772,  2775,  2776,
    2780,  2781,  2785,  2790,  2791,  2793,  2795,  2797,  2802,  2804,
    2809,  2811,  2813,  2815,  2817,  2819,  2821,  2826,  2827,  2828,
    2832,  2841,  2847,  2857,  2861,  2868,  2889,  2890,  2895,  2896,
    2897,  2898,  2899,  2903,  2912,  2911,  2922,  2921,  2931,  2936,
    2945,  2949,  2956,  2974,  2993,  2997,  3004,  3006,  3008,  3015,
    3016,  3017,  3021,  3025,  3026,  3027,  3031,  3032,  3036,  3037,
    3041,  3042,  3043,  3044,  3048,  3049,  3053,  3057,  3058,  3064,
    3065,  3069,  3074,  3079,  3087,  3090,  3096,  3097,  3103,  3107,
    3111,  3118,  3119,  3123,  3128,  3137,  3138,  3142,  3143,  3150,
    3151,  3152,  3153,  3154,  3155,  3157,  3159,  3161,  3167,  3169,
    3172,  3174,  3176,  3178,  3180,  3182,  3184,  3186,  3188,  3190,
    3195,  3199,  3203,  3207,  3211,  3215,  3224,  3233,  3245,  3249,
    3253,  3257,  3261,  3265,  3269,  3273,  3280,  3298,  3306,  3307,
    3308,  3309,  3310,  3311,  3315,  3316,  3320,  3324,  3331,  3338,
    3348,  3349,  3353,  3357,  3361,  3368,  3369,  3370,  3371,  3372,
    3373,  3374,  3375,  3376,  3377,  3381,  3383,  3385,  3387,  3389,
    3395,  3402,  3415,  3428,  3442,  3460,  3467,  3472,  3477,  3482,
    3488,  3494,  3524,  3531,  3538,  3539,  3543,  3545,  3546,  3548,
    3550,  3551,  3552,  3553,  3556,  3557,  3558,  3559,  3560,  3561,
    3562,  3563,  3567,  3568,  3572,  3573,  3574,  3578,  3579,  3580,
    3581,  3590,  3591,  3594,  3595,  3599,  3611,  3623,  3630,  3632,
    3634,  3636,  3638,  3640,  3649,  3655,  3659,  3663,  3670,  3671,
    3675,  3676,  3680,  3681,  3682,  3683,  3684,  3685,  3686,  3691,
    3696,  3701,  3706,  3711,  3719,  3729,  3739,  3744,  3753,  3758,
    3767,  3768,  3769,  3770,  3771,  3772,  3773,  3774,  3775,  3776,
    3777,  3778,  3779,  3780,  3781,  3782,  3783,  3784,  3785,  3786,
    3787,  3788,  3789,  3793,  3794,  3795,  3796,  3797,  3798,  3801,
    3805,  3809,  3813,  3817,  3824,  3828,  3832,  3836,  3844,  3845,
    3846,  3847,  3848,  3849,  3850
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
  "throw_stmt", "select_stmt", "when_stmt_ls", "when_stmt", "manager_expr",
  "manager_expr_ls", "manage_stmt", "class_decl_stmt", "class_start",
  "class_tag", "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_header", "enum_ls", "$@5", "$@6", "enum_item",
  "linkage_spec_empty", "linkage_spec", "opt_fn_type_formal_ls",
  "fn_type_formal_ls", "fn_type_formal", "opt_fn_type_ret_type", "fn_type",
  "fn_expr", "$@7", "$@8", "fn_decl_stmt_complete", "fn_decl_stmt", "$@9",
  "fn_decl_stmt_inner", "fn_decl_stmt_start", "fn_decl_receiver_expr",
  "fn_ident", "op_ident", "assignop_ident", "all_op_name",
  "formal_var_arg_expr", "opt_formal_ls", "formal_ls_inner", "formal_ls",
  "formal", "named_formal", "opt_formal_intent_tag", "required_intent_tag",
  "opt_this_intent_tag", "proc_iter_or_op", "opt_ret_tag",
  "opt_throws_error", "opt_function_body_stmt", "function_body_stmt",
  "query_expr", "opt_lifetime_where", "lifetime_components_expr",
  "lifetime_expr", "lifetime_ident", "type_alias_decl_stmt",
  "type_alias_decl_stmt_start", "type_alias_decl_stmt_inner_ls",
  "type_alias_decl_stmt_inner", "opt_init_type", "var_decl_type",
  "var_decl_start", "var_decl_stmt", "$@10", "$@11",
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

#define YYPACT_NINF (-871)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-745)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -871,    65,  3851,  -871,   -60,    66,  -871,  -871,  -871,  -871,
    -871,  -871,  -871, 11908,    10,    83,   128, 13542,   188, 17166,
      10,   213,    24,   191,    83,  5412, 11908,   120, 17224,  -871,
     297,   272,  -871,  9853,   286,   318,   183,  -871,   295,   309,
   17224, 17224, 17224, 13646, 11052,   374, 11908, 11908,   279,  -871,
     383,   422, 11908,  -871, 13542,  -871, 11908,   480,   335,    52,
     221, 13073,   461, 17282,  -871, 11908,  7980, 11908, 11052, 13542,
     455,   504,   398,  5412,   527, 11908,   530,  6960,  6960,  -871,
     534,  -871, 13542,  -871,   546, 10023,  -871, 11908,  -871, 11908,
    -871,  -871, 13372, 11908,  -871, 10193,  -871,  -871,  -871,  4374,
    8150, 11908,  -871,  5066,  -871,  -871,  -871,  -871,  2446,   602,
    -871,   475,   444,  -871,    35,  -871,  -871,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  7300,  -871,
    7470,  -871,  -871,    16,  -871,  -871,     2,  -871,   560,  -871,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,   482, 17224,  -871,
     456,  -871,   193,  -871,  -871,  -871,   429,   465,  -871,  -871,
   17224,  -871,  1468,  -871,   464,   466,  -871,  -871,  -871,   472,
     474, 11908,   476,   481,  -871,  -871,  -871, 16695,  1826,    59,
     484,   485,  -871,  -871,   151,  -871,  -871,  -871,  -871,  -871,
     446,  -871,  -871,  -871, 11908,  -871, 17224,  -871, 11908, 11908,
      53,   573,  -871,  -871,  -871,  -871, 16695,   399,  -871,  -871,
      42,  5412,  -871,  -871,  -871,   486,    14,   463,  -871,   314,
    -871,   488,  -871,   -20, 16695,  8320,  -871,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,
    -871,  -871,  -871,  -871, 17224, 17224,   -41,  3997,  -871,  -871,
     549,  8320,   489,   483,  -871,  5412, 13704,    91,    22,  -871,
    5412,  -871,  -871, 14851,  -871,    55, 15254,   499,  -871,   502,
     503,  -871, 14851,    14,   499,  -871,  8320, 17323, 17323,    12,
     -11,  -871,    -5,  -871,  -871,  -871,  -871,  7640,  -871,  -871,
     732,  -871,   487,   524,  -871,  -871, 13794,   490,  -871,  -871,
   16695,   168, 11908, 11908,  -871,    30,  -871,  -871, 16695,   511,
   15294,  -871, 14851,    14,   506,  8320,  -871, 16695, 15334, 11908,
    -871,  -871,  -871,  -871,  -871,    14,   525,   287,   287,  1298,
     499,   499,   331,  -871,  -871,  4547,   233, 11908,   538,   -36,
     508,  1298,   678,  -871,  -871, 17224,  -871, 11908,  -871,  4374,
    -871,   145,  -871,   549,  -871,  -871,   686,   521,  4720, 11908,
    -871, 11908,   627,  -871,  -871,  1894,    32,   482, 16695,   332,
    -871,  5412,   618,  -871,  -871,  9853, 10363, 11222,  -871,  -871,
    -871, 17224,  -871, 17224, 11908,   529,  -871,  1805,  -871,  -871,
    -871,  -871,    17,  -871,  -871,  -871,  -871,  -871,  -871,  -871,
      36,   206,  -871,  -871,  -871,  -871,  -871,  -871,  -871, 13207,
     576,   175,  -871,   539,   238,   425,  -871,   557, 11908, 11908,
   11908, 11222, 11222, 11908,   426, 11908, 11908, 11908, 11908, 11908,
     327, 13372, 11908, 11908, 11908, 11908, 11908, 11908, 11908, 11908,
   11908, 11908, 11908, 11908, 11908, 11908, 11908, 11908,   637,  -871,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,
   10023,  -871,  -871, 10023, 10023,  8320,  8490, 11222, 11222, 15412,
     537,  -871, 11392, 11222, 17224,  -871,  2775,  -871,  -871,  -871,
   13646,  -871, 11908,  -871,   586,   540,   565,  -871,  -871,   591,
     593,  5412,   684,  5412,  -871,   687, 11908,   655,   552,  -871,
   10023,  -871, 13704,  -871,  -871,   -21,  -871, 12078,   601, 11908,
   13646,  -871,  -871, 11908,  -871, 17018, 11908, 11908,  -871,   314,
     554,  -871,  -871,  -871,  -871, 17224,  -871,   429,  2066, 13704,
     582, 12078,  -871, 16695, 16695,  -871,    79,  -871,    41,  -871,
    8320,   559,  -871, 14851,   707,   707,  -871,  -871,  -871,  -871,
   11565,  -871, 15452,  8663,  8833,  -871,  -871,  -871, 16695, 11222,
   11222,  -871,   475,  9003,  -871,   384,  -871,  4893,  -871,   251,
   15504,   265,  2561, 17224,  6787,  6617,  -871,   482,   566,  -871,
    -871,   314,  -871,   616, 17224,    84, 14851,   567,  3120,   -44,
       0, 11908,   186, 15602,   -39,    56, 14535,  -871,    21,  -871,
     152,   594,  2344,   569,  -871, 17108,   617,   264,  -871,  -871,
    -871,  -871,  6447,  -871,   640,  -871,  -871,   575,   599,  -871,
     600,  -871,   603,   604,   606,   608,   611,  -871,   613,   615,
     619,   620,   622,   625,   626,  -871,  -871,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,  -871,  -871,  -871, 11908,   609,   630,
     575,  -871,   575,  -871, 11908,  -871, 17224,  -871,  -871,  -871,
   16775, 16775,   440,  -871,   440,  -871,   440,  3892,   624, 13624,
    2584,    14,   287,  -871,   451,  -871,  -871,  -871,  -871,  -871,
    1298, 16867,   440,   520,   520, 16775,   520,   520, 13809,   287,
   16867, 16827, 13809,   499,   499,   287,  1298,   621,   628,   632,
     633,   578,   592,  -871,   440,  -871,   440, 11908, 10023,    92,
     742, 14929,   623,   614,   214,  -871,  -871,  -871,  -871,  -871,
    -871,  -871,  -871,   111,  -871,  1597, 16735,   457,   274, 16695,
   10023,  -871, 10023, 11908, 11908,   763,  5412,   764, 15654,  5412,
    3670, 17224,  -871,   143,   148, 13704,    91,  -871, 16695,  9173,
    -871, 16695,  -871,  -871,  -871, 17224, 15694, 15812, 13646,  -871,
     582,   658,  -871,   153, 12078,  -871,   178,  -871, 11908,  5239,
    -871,  -871,   634,   177,   635,  -871,  3511,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,  -871,   171,    73, 14613,  -871,  -871,
     158,  -871,  -871,  -871,  -871, 11908,  -871,    60, 13892, 11908,
    -871, 11908,  6787,  6617,  -871,  9343,   468,  9513,   469,   470,
    9683,  7810,   492,   296,  -871,  -871,  -871, 15860,   654,   642,
     641,  -871, 17224, 13646, 11908,   761,  -871,  -871, 11908, 16695,
    5412,   644, 11222, 10536,  -871,  4201,   304,   645,   668,  -871,
     358, 11908, 11735,  -871,  5412,  -871,    84,  5585, 11222,  -871,
      84, 17224,   463,  -871, 10023,   646,  1708,  -871,  -871, 11908,
      34,  -871,  -871,  -871,  -871,  -871,  -871,    39,   385,  -871,
   15900,  -871, 14049,  -871,  -871, 16695,  -871,   643,   647,  -871,
    -871,  -871,  -871,  -871,  -871,   181, 11222,  3120, 11908, 12248,
   11222, 10023,  -871,  -871,  -871,  7130,  -871,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,    37, 11222,
   11222,  -871,  -871,  -871, 16695, 16695,  5412,  -871,  5412, 11908,
    -871,  5412,   787,   662,   663,  -871,  -871,   601,  -871,  -871,
    -871,   659,   664,   319,   178, 11908,   640,   582,  -871, 10709,
    -871,  -871, 16695,  -871,  -871,  5412, 11908,  -871,  -871,  -871,
   17224,  -871,   689,   463, 11222,  5412, 11222, 10023,  -871,  -871,
    -871,  -871,  -871,   451,  -871,  -871,  -871,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  6787,  -871,  -871,
   16018, 15019,   323,   672,  6787,  -871, 11908,  -871,  -871,  3390,
     366,   370, 16058, 11908,   187,  -871,  5412,   -29, 15097,  -871,
     363,  -871, 16695,  -871,  -871,  5412, 10879,  -871, 16695,  -871,
   16106, 16695,  -871,  9853, 16695,   109, 14693,  -871,  -871,  -871,
     190,  -871,  -871,  -871,  -871,  2225,  3230,  -871, 16695, 17224,
     602,    76, 11908,  -871,   691,   666,  -871,  -871,  -871,   575,
     575,  -871,  -871,  -871,   665, 16213, 11908,   810, 16261,   200,
    -871,   601,   222,   224,  -871,  -871, 16695,   812,  5412,  -871,
    -871,  -871,  8320,  8320,  -871,  -871,  -871,  -871,  -871,   178,
   16695,  -871,   270,    81,   673, 11908,  -871,   -27, 15176,    73,
   14771,  -871,   277, 11908,  6787,  6617,  -871,  -871,  -871, 16301,
   16695,  3626,  -871,  -871,  -871,  -871, 14851,  -871,  -871,  5758,
     676,  5931,   677,  -871,  -871, 11908, 14131,  6104,  -871,    84,
    6277,  -871,    84,   688,  3230,  -871,  -871,  -871, 17108,  -871,
    -871,   237,  -871,   -55,  -871,   372, 16379,    80,   796,  -871,
    -871,  -871, 12418, 12588, 16419, 12758, 12928,  -871,  -871,  5412,
    -871,   685,   692,  -871,  -871,  5412,  -871,   463, 16695,  5412,
    5412,  -871, 16695,   375,   696, 11908,  -871,  -871,  9853,  -871,
   16695,  5412,  9853,  -871, 16695,  5412, 16695,  5412,  9853, 16695,
    -871,  9853, 16695,  -871,  -871,  -871,  -871,  -871, 11908,    76,
     237,   237,   237,   237,   237,   237,    76,  -871,  -871,  -871,
    -871, 11908, 11908, 11908, 11908, 11908,  -871,   702,   704,   616,
    -871,  -871,  -871, 16695, 14209,  -871, 14289,  -871, 15654, 14367,
   14447, 16695,  -871,  -871,  -871,  -871,  -871,  -871,  -871,   722,
   16459, 16537, 16577, 16617,  -871,  -871,  5412,  5412,  5412,  5412,
   11908, 11908, 11908, 11908, 15654, 15654, 15654, 15654
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   137,   462,    55,   684,   685,
     686,   680,   681,   207,   594,   126,   158,   566,   165,   568,
     594,   164,   494,   491,   126,     0,   207,   272,   159,   196,
     195,   678,   194,   207,   163,    68,   273,   333,   160,   138,
       0,     0,     0,     0,   207,   167,   207,   207,   615,   587,
     687,   168,   207,   334,   560,   488,   207,     0,     0,   181,
     179,   419,   162,   569,   490,   207,   207,   207,   207,   564,
       0,   166,     0,     0,   139,   207,   679,   207,   207,   481,
     161,   310,   562,   492,   169,   207,   740,   207,   742,   207,
     743,   744,   631,   207,   741,   207,   639,   177,   739,     0,
     207,   207,     4,     0,     5,    10,    11,    12,    48,    51,
      52,    56,     0,    45,    71,    13,    77,    14,    15,    16,
      17,    29,   547,   548,    22,    46,   178,   188,   207,   197,
     652,   189,    18,     0,    31,    30,     0,    47,     0,   267,
      19,   648,    21,    35,    32,    33,   187,   311,     0,   185,
       0,   332,     0,   643,   183,   349,   445,   431,   641,   186,
       0,   493,     0,   184,   657,   637,   636,   640,   551,   549,
       0,   207,   646,   647,   651,   650,   649,     0,   550,     0,
     658,   659,   660,   682,   683,   642,   553,   552,   644,   645,
       0,    28,   568,   159,   207,   138,     0,   569,   207,   207,
       0,     0,   657,   549,   646,   647,   558,   550,   658,   659,
       0,     0,   595,   127,   128,     0,   567,     0,   482,     0,
     489,     0,    20,     0,   529,   207,   134,   140,   151,   146,
     145,   153,   131,   144,   154,   141,   135,   155,   129,   156,
     149,   143,   150,   147,   148,   130,   132,   142,   152,   157,
     136,   321,   133,   206,     0,     0,     0,     0,    70,    69,
      71,   207,     0,     0,   249,     0,     0,     0,     0,   500,
       0,   174,    40,     0,   302,     0,   301,   725,   619,   616,
     617,   618,   496,   561,   726,     7,   207,   331,   331,   426,
     171,   421,   172,   422,   423,   427,   428,   170,   429,   430,
     419,   525,     0,   336,   337,   339,     0,   420,   524,   341,
     512,     0,   207,   207,   175,   643,   634,   656,   635,     0,
       0,    43,     0,   565,     0,   207,    44,   559,     0,   207,
     280,    48,   284,   281,   284,   563,     0,   727,   729,   629,
     724,   723,     0,    74,    78,     0,     0,   207,     0,     0,
     538,   630,     0,     6,   308,     0,   195,   207,   309,     0,
      49,     0,     9,    71,    50,    53,     0,    60,     0,   207,
      72,   207,     0,   494,   193,     0,   680,   311,   653,   202,
     213,     0,   210,   211,   212,   207,   207,   207,   209,   570,
     578,     0,   251,     0,   207,     0,   307,     0,   443,   444,
     442,   357,   447,   452,   455,   446,   450,   449,   451,   454,
       0,   435,   437,   441,   440,   432,   433,   436,   439,     0,
     486,     0,   483,     0,     0,   638,    34,   620,   207,   207,
     207,   207,   207,   207,   728,   207,   207,   207,   207,   207,
       0,   628,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   388,   395,
     396,   397,   392,   394,   398,   399,   390,   393,   391,   389,
     207,   401,   400,   207,   207,   207,   207,   207,   207,     0,
       0,   210,   207,   207,     0,   599,     0,    36,    24,    37,
       0,    25,   207,    38,   547,     0,   542,   543,   546,     0,
       0,     0,   240,     0,   388,   236,   207,     0,     0,    23,
     207,    26,     0,   504,   506,     0,   523,   207,   509,   207,
       0,   173,    39,   207,   304,     0,   207,   207,    41,     0,
       0,   182,   180,   424,   425,     0,   420,   445,   419,     0,
     527,   207,   121,   654,   655,   347,     0,   176,     0,    42,
     207,     0,   291,     0,   282,   283,    27,    76,    75,    79,
     207,   688,     0,   207,   207,   676,   674,     8,   496,   207,
     207,   208,    56,   207,    54,     0,    65,     0,   119,     0,
     114,     0,    86,     0,   207,   207,   190,   311,     0,   198,
     204,     0,   201,     0,     0,     0,     0,     0,   579,     0,
       0,   207,     0,   529,     0,     0,     0,   254,     0,   252,
     278,   312,     0,     0,   320,     0,   329,     0,   322,   453,
     448,   517,     0,   519,   456,   438,   434,   406,   134,   384,
     140,   382,   146,   145,   144,   141,   135,   386,   155,   156,
     143,   147,   130,   142,   157,   381,   363,   366,   364,   365,
     387,   376,   367,   380,   372,   370,   383,   371,   369,   374,
     379,   368,   373,   377,   378,   375,   385,   207,   360,     0,
     406,   361,   406,   358,   207,   485,     0,   480,   499,   498,
     720,   719,   722,   731,   730,   735,   734,   716,   713,   714,
     715,   632,   703,   137,     0,   670,   671,   139,   669,   668,
     626,   707,   718,   712,   710,   721,   711,   709,   701,   706,
     708,   717,   700,   704,   705,   702,   627,     0,     0,     0,
       0,     0,     0,   733,   732,   737,   736,   207,   207,     0,
       0,     0,   278,     0,   605,   606,   612,   611,   613,   608,
     614,   610,   597,     0,   600,     0,     0,     0,     0,   530,
     207,   555,   207,   207,   207,   239,     0,   235,     0,     0,
       0,     0,   554,     0,     0,   507,     0,   522,   521,   207,
     502,   625,   501,   303,   300,     0,     0,     0,     0,   667,
     527,   342,   338,     0,   207,   528,   509,   340,   207,     0,
     460,   346,     0,     0,     0,   556,     0,   285,   689,   633,
     675,   539,   538,   677,   540,     0,     0,     0,    57,    58,
       0,    61,    63,    67,    66,   207,    97,     0,     0,   207,
      92,   207,   207,   207,    64,   207,   363,   207,   364,   365,
     207,   207,   375,     0,   402,   403,    81,    80,    91,     0,
       0,   313,     0,     0,   207,     0,   216,   215,   207,   580,
       0,     0,   207,   207,   536,     0,   531,     0,   534,   692,
     533,   207,   207,   696,     0,   224,     0,     0,   207,   222,
       0,     0,     0,   275,   207,     0,   331,   319,   326,   207,
     324,   318,   515,   514,   513,   516,   457,   463,   419,   356,
       0,   362,     0,   352,   353,   487,   484,     0,     0,   125,
     124,   123,   122,   665,   666,     0,   207,   690,   207,   207,
     207,   207,   598,   607,   609,   207,   596,   158,   165,   164,
     163,   160,   167,   168,   162,   166,   161,   169,     0,   207,
     207,   495,   545,   544,   248,   247,     0,   242,     0,   207,
     238,     0,   244,     0,   277,   505,   508,   509,   510,   511,
     299,     0,     0,     0,   509,   207,   456,   527,   526,   207,
     415,   413,   348,   461,   293,     0,   207,   292,   295,   557,
       0,   286,   289,     0,   207,     0,   207,   207,    59,   120,
     117,    98,   109,   104,   103,   102,   111,    99,   112,   107,
     101,   108,   105,   106,   100,   110,   113,   207,   116,   115,
      88,    87,     0,     0,   207,   191,   207,   192,   313,   331,
       0,     0,     0,   207,     0,   232,     0,     0,     0,   691,
     532,   230,   581,   537,   694,     0,   207,   693,   698,   697,
       0,   574,   225,   207,   573,     0,     0,   223,   253,   250,
       0,   306,   181,   179,   305,   331,   331,   314,   330,     0,
       0,     0,   207,   350,   411,     0,   408,   412,   359,   406,
     406,   672,   673,   277,     0,     0,   207,     0,     0,     0,
     601,   509,   657,   657,   241,   237,   586,   243,     0,    73,
     274,   503,   207,   207,   497,   416,   414,   343,   344,   509,
     405,   297,     0,     0,     0,   207,   287,     0,     0,     0,
       0,    62,     0,   207,   207,   207,    93,    95,    84,    83,
      82,   331,   199,   205,   203,   214,     0,   234,   233,     0,
       0,     0,     0,   695,   231,   207,     0,     0,   220,     0,
       0,   218,     0,   279,   331,   316,   315,   325,     0,   477,
     478,     0,   479,   465,   468,     0,   464,     0,   419,   407,
     354,   355,   207,   207,     0,   207,   207,   279,   602,     0,
     246,     0,     0,   418,   417,     0,   296,     0,   290,     0,
       0,   118,    89,     0,     0,   207,   200,   217,   207,   228,
     583,     0,   207,   226,   582,     0,   699,     0,   207,   572,
     221,   207,   571,   219,   276,   317,   328,   476,   207,     0,
       0,     0,     0,     0,     0,     0,     0,   458,   351,   459,
     409,   207,   207,   207,   207,   207,   245,   621,   622,   298,
     288,    94,    96,    85,     0,   229,     0,   227,   577,     0,
       0,   467,   469,   470,   473,   474,   475,   471,   472,   466,
       0,     0,     0,     0,   623,   624,     0,     0,     0,     0,
     207,   207,   207,   207,   585,   584,   576,   575
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -871,  -871,  -871,     3,   -57,   816,  -871,   376,   760,  -871,
    -871,   392,  -389,  -106,  -871,   298,  -871,  -871,  -104,  -871,
    -871,    18,   631,  -871,  -536,  2971,   509,  -572,  -871,  -799,
    -871,  -871,  -871,    63,  -871,  -871,  -871,   855,  -871,   217,
    -176,  -871,  -871,  -465,   736,  -871,  -851,  -871,  -871,   174,
    -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,  -871,     5,
    -871,  -156,   522,  -198,  -871,   496,  -871,  -871,  -871,    19,
    1510,  -871,  -871,  -871,  -871,   558,  -871,   -76,  -871,  -871,
    -871,  -871,   373,  -871,  -871,  -871,   -98,  -871,  -348,  -795,
    -871,  -871,  -871,  -871,  -871,  -604,  -871,  -871,  -871,  -871,
     357,  -871,   130,  -871,  -871,  -871,  -871,   519,  -871,  -871,
    -871,  -871,     8,  -401,  -177,  -782,  -870,  -653,  -871,  -871,
    -247,  -861,   605,   -48,  -871,  -871,   365,   -53,  -871,  -239,
    -871,  -871,  -296,  -288,  -753,  -871,  -871,    70,   239,  -871,
     388,  -193,   786,  -871,  -871,  -153,   396,   155,  -391,  -764,
    -601,  -871,  -871,  -871,  -711,  -513,  -871,  -746,   -24,  -871,
    -871,  -569,    54,  -871,  -255,  -490,  -559,    -2,  -871,  -871,
    -871,  -871,  -871,  -871,    98,  -871,   897,  -533,  -871,     6,
    -871,  -871,   129,   471,  -871,  -871,  -871,  -871,  2535,  -871,
     -50,   516,  -871,  1159,  1323,  -871,  -871,  -871,  -871,  -871,
    -871,  -871,  -871,  -871,  -471
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   344,   103,   104,   105,   106,   107,   361,
     362,   108,   109,   110,   111,   367,   574,   810,   811,   112,
     113,   114,   372,   115,   116,   117,   345,   838,   581,   839,
     118,   119,   998,   578,   579,   120,   121,   215,   250,   267,
     122,   252,   123,   301,   528,   124,   125,   288,   287,   126,
     127,   128,   129,   130,   590,   131,   591,   842,   132,   200,
     134,   387,   388,   599,   135,   256,   136,   137,   608,   609,
     201,   139,   140,   141,   142,   554,   797,   973,   143,   144,
     793,   968,   274,   275,   145,   146,   147,   148,   395,   876,
     149,   150,   617,  1049,  1050,   618,   151,   152,   302,   303,
     304,   956,   153,   316,   546,   788,   154,   155,  1147,   156,
     157,   669,   670,   834,   835,   836,   960,   889,  1054,  1055,
    1056,   305,   306,   536,   419,   401,   410,   887,  1208,   791,
     158,  1053,  1143,  1144,  1145,   159,   160,   421,   422,   675,
     161,   162,   163,   219,   529,   268,   269,   514,   515,   770,
     308,   885,   623,   624,   518,   309,   785,   786,   346,   601,
     602,   856,   348,   349,   495,   496,   497,   202,   165,   166,
     389,   167,   390,   168,   203,   170,   211,   212,   743,   744,
     745,   171,   204,   205,   174,   175,   176,   317,   177,   379,
     498,   207,   179,   208,   209,   182,   183,   184,   185,   604,
     186,   187,   188,   189,   190
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     164,   474,   223,   365,   300,   102,   508,   133,   615,   424,
     790,   878,   833,   307,   812,   747,   319,   893,   671,   894,
     763,   884,   961,   164,  1003,  1047,   490,  1057,   787,   588,
     133,   530,   377,   860,   954,   336,   999,   621,   516,  -327,
     380,   311,   792,   485,   483,  -272,  1009,   852,   381,   494,
     350,  -273,   767,   260,   354,   947,   382,   383,   384,   533,
     501,  1198,  -345,   693,   619,     3,   851,   857,  -518,   519,
     551,   164,   866,   870,   210,   164,   164,   380,   133,  1139,
     506,   369,   133,   133,  1085,   494,   213,   270,   864,   210,
    1199,   210,   516,   481,   383,   384,  -518,   164,   -70,  1051,
     169,   164,   534,   191,   133,   270,   862,   620,   133,   564,
     494,   789,   789,   270,   195,  -294,   270,   -70,   210,   370,
     358,   764,  -255,   169,   765,   492,   363,   359,   210,   850,
    1140,   172,   218,   385,   863,   565,   -70,   214,  -520,   906,
    -518,  1127,    81,   493,  1119,   853,  1169,   371,   783,   494,
     766,   545,  -518,   434,   172,  1052,  -294,   883,  1047,   225,
    -520,   440,   697,   854,   -70,   261,   871,   520,  1141,   380,
     359,   169,   391,   855,   210,   169,   169,   381,  1142,  -323,
     622,   517,  -520,  1081,  -256,   382,   383,   384,   386,   605,
    1086,   210,   872,   933,  1135,  1136,   315,   169,  -345,  -518,
     523,   169,   172,  -323,  -518,   570,   172,   172,  -520,   164,
    -294,  1089,   486,  1111,  -520,   315,   133,  1071,   974,  1163,
     721,   722,  1108,   359,   359,   482,   492,   210,   172,   475,
     315,   476,   172,   683,   685,   517,  -520,   853,   905,   840,
    1139,   359,   672,  1207,  -262,   251,   398,   359,   359,   359,
    1002,   965,   359,   625,  -520,   854,   915,   263,   264,   265,
    1047,   913,   385,   164,   699,   907,   399,   -69,   164,  -261,
     133,   958,   857,   866,   870,   133,   400,   359,   587,   723,
     725,   220,   916,  1195,  1020,   730,   -69,  1057,   752,   210,
     225,  1140,   966,   765,   380,   794,   626,   278,   765,   494,
     494,   769,  1165,   977,   914,   -69,  1174,  1158,   253,   169,
     382,   383,   384,   492,   944,   873,   492,   569,   430,   945,
     676,  -664,   874,  -664,   957,  1164,   752,   959,  -270,   978,
     693,   542,   492,   -69,   494,   752,   778,   748,   677,  1142,
     172,    23,  -260,   164,   975,   752,   967,   694,   559,   854,
     133,  -257,  1063,   261,  1087,   315,   279,   164,  1117,   859,
     695,  1133,   600,   169,   133,   396,   164,  -604,   169,  -603,
     611,  1157,   806,   133,   494,   280,   431,   420,   560,   164,
     432,   195,    37,   520,  1040,  -331,   133,   385,  1197,   696,
     281,   315,    55,  -604,   172,  -603,   815,   494,   843,   172,
     258,   679,   561,   259,    64,  -331,  1150,  1151,    53,   880,
     819,   262,   289,   480,   816,  -331,   315,   673,   812,   520,
     718,  1069,  1004,   719,   720,  1102,   434,    83,   820,   697,
    -264,   438,   291,   881,   440,   698,   293,   931,   443,  -268,
     589,  1004,   386,   169,   747,  1137,  1171,  1233,  1234,  1235,
    1236,  1237,  1238,   330,   333,   315,   402,   169,   729,  1005,
     294,   531,   532,   295,   520,    57,   169,  1023,  1004,   331,
     331,   499,   500,   173,   172,   296,   403,  1024,  -265,   169,
     404,   300,  1084,   513,  1120,  1122,  1106,  1046,   172,   285,
     307,   671,   411,   298,   191,  1200,   173,   172,   299,   164,
     557,   164,  1129,  1132,   405,   286,   133,   406,   133,   365,
     172,   676,   412,   802,   802,   520,   413,  -259,   178,   407,
    1004,  1023,  1201,   540,  1202,  1203,  1023,  1204,  1205,  1113,
     430,  1027,  1173,  1114,  1196,   477,  1123,   408,  1221,   478,
     414,   178,   409,   415,   173,   805,   930,   191,   173,   173,
     478,   430,   494,   813,   324,   416,  -410,  -742,  -743,  -744,
    -263,  -742,  -743,  -744,  1120,  1122,  1129,  1132,   325,  -661,
     173,  -661,   251,   417,   173,   164,   494,   858,   418,   434,
     559,  -739,   133,  -269,   438,  -739,  -271,   440,   431,   178,
    -258,   443,   432,   178,   178,  -663,  -662,  -663,  -662,   169,
     315,   169,  -266,   315,   315,   315,   315,     7,   607,   431,
     610,   790,   368,   432,   616,   178,   393,   801,   804,   178,
    1046,   897,   366,   898,   397,   953,   394,  -589,   425,   484,
     172,   359,   172,   370,   541,  -588,   668,   426,   434,  -592,
     315,   980,   699,   438,  -593,   439,   440,  -591,  -590,   488,
     443,   491,   509,   510,  1017,   430,  1134,   450,   537,   434,
     435,  1138,   436,   437,   438,   456,   439,   440,   441,   538,
    1035,   443,   526,   527,   547,   169,   550,   449,   450,   566,
     315,   453,   173,   563,   454,   455,   456,   567,   556,   572,
    1011,   573,   583,   315,   315,   457,   594,   612,   494,   674,
     932,   732,   678,   315,   440,   717,   172,   728,  1064,   750,
     752,   751,  1067,   431,   753,  1060,   754,   432,   756,   949,
     761,   759,  1046,   762,   769,   779,   784,   178,   796,   513,
     795,   683,   723,   844,   841,   494,   173,   848,   877,   492,
     879,   173,   774,   928,   886,   888,  -137,  -158,   891,   903,
    -165,  -164,   780,  -163,   164,  -160,   513,   164,  -138,   289,
    -167,   133,  -168,   434,   133,   904,  -162,  -166,   438,  -139,
     439,   440,  -161,  -169,   908,   443,  1097,   892,  1099,   291,
     272,   178,   450,   293,   899,   912,   178,   164,   454,   455,
     456,   900,   963,   911,   133,   901,   902,   936,   938,  1004,
     824,   494,   955,   964,   321,  1007,   969,   294,  1013,  1008,
     295,   845,   352,  1026,  1061,  1041,   173,  1016,  1025,  1045,
    1062,  1078,   296,   289,  1014,  1079,  1080,  1161,  1162,  1082,
     173,   300,   616,  1095,  1083,  1107,  1148,  1149,  1152,   173,
     298,   222,  1155,   291,  1167,   299,  1159,   293,   164,  1181,
    1185,  1194,   173,   164,   169,   133,  1217,   169,   315,  1222,
     133,   178,   164,  1218,  1244,   164,  1245,  1199,   360,   133,
     808,   294,   133,  1101,   295,   178,    57,   577,   979,   221,
     315,   595,   315,   571,   178,   172,   296,   169,   172,   326,
    1038,   507,   555,   420,  1094,   782,   773,   178,   592,   315,
    1059,  1210,   781,  1088,   298,   535,   494,   494,  1209,   299,
    1239,  1232,  1010,   775,   374,   896,   772,   217,   172,   353,
     946,  1070,     0,     0,     0,     0,     0,  1072,  1073,     0,
       0,     0,     0,     0,   164,     0,   164,     0,     0,   164,
     365,   133,  1093,   133,     0,     0,   133,     0,   169,   506,
       0,     0,  1045,   169,     0,     0,     0,     0,     0,     0,
       0,     0,   169,   164,     0,   169,     0,     0,     0,     0,
    1092,     0,   173,   164,   173,     0,     0,     0,   943,   172,
     133,     0,   513,     0,   172,     0,     0,     0,     0,     0,
       0,     0,   950,   172,     0,     0,   172,     0,     0,     0,
       0,     0,     0,     0,   315,     0,     0,     0,     0,   522,
       0,   524,     0,   972,   164,     0,     0,   178,     0,   178,
       0,   133,     0,   164,     0,     0,     0,   487,   365,     0,
     133,     0,   365,     0,   169,     0,   169,     0,     0,   169,
       0,   315,     0,     0,     0,     0,     0,   506,   173,   506,
       0,     0,   506,   506,  1045,     0,     0,     0,   549,   420,
       0,     0,     0,   169,     0,   172,     0,   172,     0,     0,
     172,     0,     0,   169,     0,     0,   164,     0,     0,     0,
       0,   511,     0,   133,     0,     0,   521,     0,   607,     0,
       0,   300,     0,   178,   172,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   172,     0,     0,   315,     0,   668,
       0,     0,     0,     0,   169,     0,     0,   164,     0,   164,
       0,     0,     0,   169,   133,   164,   133,     0,   164,     0,
       0,     0,   133,     0,     0,   133,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   172,     0,     0,     0,     0,
       0,     0,     0,     0,   172,     0,     0,   164,     0,     0,
       0,   180,     0,   164,   133,     0,     0,   164,   164,     0,
     133,     0,     0,     0,   133,   133,   169,     0,     0,   164,
       0,     0,     0,   164,   180,   164,   133,   972,     0,     0,
     133,     0,   133,     0,     0,     0,     0,   593,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   172,     0,     0,
       0,     0,   315,   315,     0,     0,     0,   169,     0,   169,
       0,     0,     0,     0,     0,   169,     0,   173,   169,     0,
     173,     0,   180,     0,     0,     0,   180,   180,     0,     0,
       0,     0,     0,     0,   164,   164,   164,   164,   172,     0,
     172,   133,   133,   133,   133,     0,   172,   169,   180,   172,
     173,     0,   180,   169,     0,     0,   616,   169,   169,     0,
       0,     0,   178,     0,     0,   178,     0,     0,     0,   169,
       0,     0,     0,   169,     0,   169,     0,     0,   172,     0,
       0,     0,     0,     0,   172,     0,     0,     0,   172,   172,
       0,     0,     0,     0,     0,   178,     0,     0,     0,     0,
     172,     0,     0,     0,   172,     0,   172,   755,     0,   757,
       0,   173,     0,     0,     0,   181,   173,     0,     0,   430,
       0,   846,   847,     0,     0,   173,     0,     0,   173,     0,
       0,   865,   869,     0,   169,   169,   169,   169,   181,     0,
       0,     0,     0,     0,     0,   616,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   178,     0,     0,     0,
     180,   178,     0,     0,     0,   172,   172,   172,   172,     0,
     178,     0,     0,   178,     0,     0,     0,   431,     0,     0,
       0,   432,     0,     0,     0,     0,   181,     0,     0,     0,
     181,   181,     0,     0,     0,     0,     0,   173,     0,   173,
       0,     0,   173,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   181,     0,   180,     0,   181,     0,     0,   180,
       0,     0,     0,     0,     0,     0,   173,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   173,     0,     0,   443,
       0,     0,   178,     0,   178,   449,   450,   178,     0,   453,
       0,     0,   454,   455,   456,     0,     0,     0,     0,   423,
       0,   226,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   178,     0,     0,     0,   227,   228,   173,   229,     0,
       0,   178,     0,   230,     0,     0,   173,     0,     0,     0,
       0,   231,     0,     0,   180,     0,     0,   232,     0,     0,
       0,     0,   138,   233,     0,     0,     0,   234,   180,     0,
     235,     0,   236,     0,     0,     0,     0,   180,     0,     0,
     237,     0,   178,     0,   181,   138,     0,   238,   239,     0,
     180,   178,   865,   869,     0,   240,     0,     0,     0,   173,
       0,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,   243,     0,   244,     0,
     245,     0,   937,   246,     0,   940,     0,   247,     0,   248,
       0,     0,   249,   138,     0,     0,     0,     0,   181,     0,
     173,     0,   173,   181,   178,     0,     0,     0,   173,     0,
     693,   173,  1032,     0,     0,     0,  1037,     0,     0,   138,
       0,     0,     0,   138,   917,     0,     0,   918,     0,     0,
       0,     0,   919,     0,     0,     0,     0,     0,     0,     0,
     173,     0,   193,     0,     0,   178,   173,   178,   266,     0,
     173,   173,   920,   178,     0,     0,   178,     0,     0,   921,
       0,   195,   173,     0,     0,     0,   173,     0,   173,   922,
     180,     0,   180,     0,     0,     0,  1015,   923,   181,     0,
       0,  1021,     0,     0,     0,   178,     0,     0,     0,     0,
     521,   178,   181,   521,   924,   178,   178,     0,     0,     0,
       0,   181,     0,     0,     0,   925,     0,   178,     0,   697,
       0,   178,     0,   178,   181,     0,   926,     0,     0,     0,
       0,   927,     0,     7,     0,     0,     0,   173,   173,   173,
     173,   138,     0,     0,     0,     0,     0,     0,     0,     0,
     354,     0,     0,     0,    22,    23,   180,     0,     0,     0,
       0,     0,     0,   355,     0,    29,   356,     0,     0,     0,
       0,    32,  1074,     0,  1075,     0,     0,  1077,    37,     0,
       0,     0,   178,   178,   178,   178,     0,     0,     0,     0,
       0,  1128,  1131,     0,     0,   138,     0,     0,     0,   357,
     138,  1091,     0,     0,    53,     0,    55,     0,    57,     0,
    1042,  1021,     0,  1043,     0,     0,   358,     0,    64,     0,
       0,     0,     0,     0,     0,     0,   613,     0,   226,     0,
       7,     0,     0,     0,     0,     0,    79,     0,    81,     0,
       0,    83,   227,   228,   181,   229,   181,     0,     0,  1166,
     230,     0,  1118,     0,     0,  1128,  1131,     0,   231,     0,
       0,  1124,     0,     0,   232,     0,     0,     0,     0,     0,
     233,     0,  1177,     0,   234,   138,     0,   235,     0,   236,
       0,     0,     0,     0,     0,  1190,     0,   237,  1193,   138,
       0,    97,     0,     0,   238,   239,     0,  1044,   138,     0,
       0,     0,   240,     0,     0,     0,     0,     0,     0,     0,
       0,   138,   241,     0,  1160,     0,     0,     0,     0,     0,
     181,   242,     0,   243,     0,   244,   428,   245,     0,     0,
     246,     0,     0,   429,   247,   180,   248,     0,   180,   249,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
     584,     0,     0,     0,     0,  1179,     0,  1183,     0,     0,
       0,     0,     0,   521,     0,     0,   521,     0,   180,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   470,   471,   472,     0,     0,   585,     0,     0,     0,
       0,     0,     0,     0,   614,  1216,     0,     0,     0,     0,
       0,  1219,     0,   431,     0,  1179,  1183,   432,     0,     0,
       0,     0,     0,   473,     0,     0,  -661,  1225,  -661,     0,
       0,  1227,     0,   757,     0,     0,     0,     0,     0,   180,
       0,   138,     0,   138,   180,     0,   433,     0,     0,     0,
       0,     0,     0,   180,     0,     0,   180,     0,     0,     0,
       0,     0,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,   586,   454,   455,
     456,     0,   757,   757,   757,   757,     0,     0,     0,   457,
       6,     0,     8,     9,    10,    11,    12,     0,     0,   181,
      13,     0,   181,     0,    17,     0,     0,   138,   192,     0,
    -207,     0,     0,   289,     0,   180,     0,   180,     0,     0,
     180,   193,     0,     0,     0,     0,  -207,  -207,  -207,     0,
     194,     0,   181,   291,     0,     0,     0,   293,     0,     0,
       0,     0,   196,     0,   180,    43,     0,     0,     0,     0,
      47,     0,    48,    49,   180,     0,     0,     0,     0,     0,
       0,   294,     0,    54,   295,    56,    57,    58,     0,     0,
       0,     0,    61,     0,   197,     0,   296,     0,     0,     0,
       0,     0,    69,    70,     0,    72,     0,     0,     0,     0,
       0,     0,     0,   181,   298,   180,     0,    82,   181,   299,
       0,     0,     0,     0,   180,     0,     0,   181,    86,     0,
     181,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       7,     0,    98,     0,   199,     0,   100,   180,  -207,     0,
       0,   101,     0,     0,     0,     0,     0,   354,     0,     0,
       0,    22,    23,     0,     0,     0,     0,     0,     0,   181,
     355,   181,    29,   356,   181,     0,   138,     0,    32,   138,
       0,     0,     0,     0,     0,    37,     0,     0,   180,     0,
     180,     0,     0,     0,     0,     0,   180,     0,   181,   180,
       0,     0,     0,     0,     0,     0,   357,     0,   181,   138,
       0,    53,     0,    55,     0,   352,     0,  1042,     0,     0,
    1043,     0,     0,   358,     0,    64,     0,     0,   180,     0,
       0,     0,     0,     0,   180,     0,     0,     0,   180,   180,
       0,     0,     0,    79,     0,    81,     0,     0,    83,   181,
     180,     0,     0,     0,   180,   875,   180,     0,   181,  -313,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     138,     0,     0,     0,     0,   138,  -313,     0,     0,     0,
    -313,  -313,     0,     0,   138,     0,     0,   138,     0,  -313,
       0,  -313,  -313,     0,     0,     0,     0,  -313,    97,     0,
       0,     0,     0,     0,  -313,     0,     0,  -313,     0,     0,
       0,   181,     0,     0,     0,   180,   180,   180,   180,     0,
       0,     0,     0,     0,     0,  -313,     0,  -313,     0,     0,
    -313,     0,  -313,     0,  -313,     0,  -313,  -313,     0,  -313,
       0,     0,  -313,     0,  -313,     0,     0,     0,     0,     0,
       0,     0,   181,     0,   181,     0,   138,     0,   138,     0,
     181,   138,  -313,   181,  -313,     0,     0,  -313,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   354,     0,
       0,     0,    22,    23,     0,   138,     0,     0,     0,     0,
       0,   355,   181,    29,   356,   138,     0,     0,   181,    32,
       0,     0,   181,   181,     0,     0,    37,     0,     0,  -331,
       0,     0,     0,    41,   181,     0,     0,  -313,   181,     0,
     181,   -68,     0,  -313,     0,     0,     0,   357,     0,  -331,
       0,     0,    53,     0,    55,     0,   138,     0,    59,  -331,
     -68,    60,     0,     0,   358,   138,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   206,     0,
       0,     0,   216,     0,    79,     0,    81,     0,     0,    83,
       0,   224,     0,     0,     0,     0,     0,     0,   257,   181,
     181,   181,   181,   428,   821,     0,     0,     0,     0,   273,
     429,   276,   277,     0,     0,     0,     0,   282,   138,   283,
       0,   284,   430,     0,     0,     0,   310,   822,     0,     0,
     224,   318,   320,   322,   323,     0,     0,     0,   327,    97,
     328,     0,     0,     0,   359,   430,     0,   335,     0,     0,
     318,     0,   337,     0,   338,     0,     0,   339,   340,   138,
     341,   138,     0,   823,   224,   318,   351,   138,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,   375,     0,   378,     0,     0,     0,   138,
       0,     0,     0,   431,     0,   138,     0,   432,     0,   138,
     138,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,   138,     0,     0,     0,   138,     0,   138,     0,     0,
     434,   435,     0,   436,   437,   438,   427,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   434,   435,   454,   455,   456,   438,   479,
     439,   440,     0,   327,   224,   443,   457,     0,     0,     0,
       0,     0,   450,     0,     0,     0,     0,     0,   454,   455,
     456,     0,     0,     0,     0,     0,   138,   138,   138,   138,
     318,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   733,     0,     5,     6,
       0,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,   318,   192,     0,  -207,
      21,     0,   734,     0,     0,     0,     0,     0,    27,     0,
     193,     0,     0,     0,    31,  -207,  -207,  -207,     0,   194,
      34,   318,   735,     0,    36,     0,   736,    38,     0,   195,
       0,   196,   327,     0,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,   543,   544,     0,
     737,     0,    54,   738,    56,     0,    58,     0,     0,     0,
     318,    61,    62,   197,   553,   739,     0,     0,     0,     0,
       0,    69,    70,    71,    72,   198,     0,    74,     0,     0,
      76,     0,   562,   740,    80,     0,    82,     0,   741,    84,
       0,     0,   568,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,   580,     0,   582,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
     596,   603,   606,    92,     0,     0,     0,     0,     0,   224,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   199,     0,   100,   742,  -207,     0,     0,
     101,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   680,   681,   682,   684,   686,   687,     0,
     688,   689,   690,   691,   692,     0,   700,   701,   702,   703,
     704,   705,   706,   707,   708,   709,   710,   711,   712,   713,
     714,   715,   716,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   318,     0,     0,   318,   318,
     318,   318,   724,   726,     0,   271,     0,   603,   731,     0,
       0,   746,     0,     0,     0,     0,     0,   749,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   758,   271,
       0,   760,     0,     0,     0,   318,     0,     0,   332,   334,
       0,     0,   768,     0,   771,     0,     0,     0,   276,     0,
       0,   776,   777,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   310,     0,     0,   310,     0,     0,   364,
       0,     0,     0,     0,     0,   318,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   749,     0,     0,   318,   318,
       0,     0,     0,     0,   224,   807,     0,   392,   318,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   837,
     837,     0,     0,     5,     6,     0,     8,     9,    10,    11,
      12,     0,     0,   849,    13,     0,   224,    16,    17,     0,
      18,     0,   192,     0,  -207,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   193,     0,   310,     0,    31,
    -207,  -207,  -207,     0,   194,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,   489,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,   890,     0,     0,     0,    61,    62,   197,   895,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,     0,   502,   505,    80,
       0,    82,     0,     0,    84,     7,     0,     0,     0,     0,
       0,     0,     0,     0,   271,     0,   271,     0,     0,     0,
       0,     0,   354,   271,     0,     0,    22,    23,     0,    87,
       0,    89,   758,   318,     0,   355,     0,    29,   356,     0,
       0,     0,     0,    32,     0,     0,     0,     0,     0,     0,
      37,     0,    96,     0,     0,   318,     0,   318,   934,   935,
     100,     0,  -207,   271,     0,     0,     0,     0,     0,     0,
       0,   357,     0,     0,   318,     0,    53,     0,    55,     0,
       0,     0,  1042,     0,     0,  1043,     0,     0,   358,   310,
      64,     0,     0,   962,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    79,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
     580,     0,     0,     0,  1000,     0,  1001,   837,   837,     0,
     337,     0,   338,     0,     0,   340,   341,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1012,
       0,     0,     0,   224,     0,     0,     0,  1018,   749,     0,
    1022,     0,     0,    97,     0,     7,  1028,  1030,     0,  1031,
       0,     0,  1034,  1036,     0,     0,     0,     0,     0,   318,
       0,     0,   354,     0,  1048,     0,    22,    23,     0,     0,
       0,     0,     0,     0,     0,   355,     0,    29,   356,     0,
       0,     0,     0,    32,     0,     0,     0,     0,     0,     0,
      37,  1065,  1022,  1031,  1034,  1068,   318,     0,     0,     0,
     746,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   357,     0,     0,   684,   724,    53,     0,    55,     0,
      57,     0,  1042,     0,  1076,  1043,     0,     0,   358,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     310,     0,     0,     0,  1090,     0,     0,     0,    79,     0,
      81,   224,     0,    83,     0,     0,     0,     0,     0,  1098,
       0,  1100,   318,     0,   226,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   271,     0,     0,     0,   227,   228,
       0,   229,   837,     0,     0,     0,   230,     0,     0,  1109,
       0,  1110,     0,     0,   231,     0,     0,     0,  1116,     0,
     232,     0,     0,    97,     0,     0,   233,     0,     0,  1112,
     234,   749,     0,   235,     0,   236,   271,   271,  1126,     0,
       0,     0,     0,   237,     0,     0,   271,   271,     0,     0,
     238,   239,     0,     0,     0,     0,     0,  1146,   240,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   241,     0,
       0,  1154,     0,     0,     0,     0,     0,   242,     0,   243,
       0,   244,     0,   245,     0,     0,   246,   318,   318,     0,
     247,     0,   248,     0,     0,   249,     0,     0,     0,     0,
    1168,     7,     0,     0,     0,     0,     0,     0,  1172,   837,
     837,     0,     0,     0,     0,     0,     0,     0,   354,     0,
       0,     0,    22,    23,  1180,     0,  1184,     0,     0,     0,
    1186,   355,  1189,    29,   356,  1192,     0,     0,     0,    32,
       0,     0,     0,     0,     0,     0,    37,     0,     0,   359,
       0,   970,   428,     0,     0,     0,     0,  1180,  1184,   429,
    1189,  1192,     0,     0,     0,     0,     0,   357,     0,     0,
       0,   430,    53,     0,    55,     0,    57,     0,  1042,     0,
    1223,  1043,     0,  1224,   358,     0,    64,  1226,     0,     0,
       0,     0,  1228,  1229,     0,     0,  1230,     0,     0,     0,
       0,   942,     0,  1231,    79,     0,    81,     0,     0,    83,
       0,     0,     0,     0,     0,     0,  1240,  1241,  1228,  1242,
    1243,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,   432,     0,     0,     0,   971,     0,     0,
       0,   941,     0,     0,     0,     0,     0,   271,   271,     0,
       0,  1254,  1255,  1256,  1257,  1254,  1255,  1256,  1257,    97,
       0,     0,   433,     0,     0,  1176,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,     0,   454,   455,   456,   271,   359,     0,
       0,   271,     0,  1039,     0,   457,     0,     0,     0,     0,
       0,    -2,     4,     0,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -207,    21,    22,    23,    24,
      25,    26,     0,  -207,    27,     0,    28,     0,    29,    30,
      31,  -207,  -207,  -207,    32,    33,    34,   -68,     0,    35,
      36,    37,     0,    38,  -331,    39,     0,    40,    41,    42,
      43,     0,    44,    45,    46,    47,   -68,    48,    49,     0,
      50,    51,    52,   430,  -331,     0,     0,    53,    54,    55,
      56,    57,    58,    59,  -331,   -68,    60,    61,    62,    63,
       0,    64,    65,    66,  1096,    67,    68,    69,    70,    71,
      72,    73,     0,    74,    75,     0,    76,    77,    78,    79,
      80,    81,    82,   -68,    83,    84,     0,     0,  -207,     0,
      85,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,   431,     0,     0,     0,   432,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,   271,   271,    93,   428,
       0,    94,    95,    96,    97,     0,   429,    98,     0,    99,
       0,   100,     0,  -207,     0,     0,   101,     0,   430,     0,
       0,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,     0,   447,   448,   449,
     450,   451,     0,   453,     0,     0,   454,   455,   456,     0,
       0,     0,     0,     0,   271,     0,     0,   457,     0,     0,
     271,   271,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   431,   271,     0,     0,
     432,     0,     0,     0,     0,     0,     0,   505,   503,     0,
     271,     0,     0,   271,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     504,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,     0,   471,   472,     0,   434,   435,  1220,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,     0,
       0,   454,   455,   456,     0,   359,     0,     0,     0,     0,
       0,     0,   457,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   505,     0,   505,     0,     0,
     505,   505,     4,     0,     5,     6,     7,     8,     9,    10,
      11,    12,     0,  -690,     0,    13,    14,    15,    16,    17,
    -690,    18,     0,    19,    20,  -207,    21,    22,    23,    24,
      25,    26,  -690,  -207,    27,  -690,    28,     0,    29,    30,
      31,  -207,  -207,  -207,    32,    33,    34,   -68,     0,    35,
      36,    37,     0,    38,  -331,    39,     0,    40,    41,    42,
      43,     0,    44,    45,    46,    47,   -68,    48,    49,     0,
      50,    51,    52,     0,  -331,     0,     0,    53,    54,    55,
      56,     0,    58,    59,  -331,   -68,    60,    61,    62,    63,
    -690,    64,    65,    66,  -690,    67,    68,    69,    70,    71,
      72,    73,     0,    74,    75,     0,    76,    77,    78,    79,
      80,    81,    82,   -68,    83,    84,     0,     0,  -207,     0,
      85,     0,     0,  -690,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,  -690,    89,  -690,  -690,  -690,  -690,  -690,  -690,  -690,
       0,  -690,  -690,  -690,  -690,  -690,  -690,  -690,  -690,  -690,
    -690,  -690,  -690,    96,    97,  -690,  -690,  -690,     0,    99,
    -690,   100,     0,  -207,     0,   342,  -690,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -207,    21,
      22,    23,    24,    25,    26,     0,  -207,    27,     0,    28,
       0,    29,    30,    31,  -207,  -207,  -207,    32,    33,    34,
     -68,     0,    35,    36,    37,     0,    38,  -331,    39,     0,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -68,
      48,    49,     0,    50,    51,    52,     0,  -331,     0,     0,
      53,    54,    55,    56,    57,    58,    59,  -331,   -68,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,     0,    74,    75,     0,    76,
      77,    78,    79,    80,    81,    82,   -68,    83,    84,     0,
       0,  -207,     0,    85,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,    97,     0,     0,
      98,     0,    99,   343,   100,     0,  -207,     0,     4,   101,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,  -207,    21,    22,    23,    24,    25,    26,     0,  -207,
      27,     0,    28,     0,    29,    30,    31,  -207,  -207,  -207,
      32,    33,    34,   -68,     0,    35,    36,    37,     0,    38,
    -331,    39,     0,    40,    41,    42,    43,     0,    44,    45,
      46,    47,   -68,    48,    49,     0,    50,    51,    52,     0,
    -331,     0,     0,    53,    54,    55,    56,    57,    58,    59,
    -331,   -68,    60,    61,    62,    63,     0,    64,    65,    66,
       0,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,    81,    82,   -68,
      83,    84,     0,     0,  -207,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,   558,   100,     0,  -207,
       0,   575,   101,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -207,    21,    22,    23,    24,    25,
      26,     0,  -207,    27,     0,    28,     0,    29,    30,    31,
    -207,  -207,  -207,    32,    33,    34,   -68,     0,    35,    36,
      37,     0,    38,  -331,    39,     0,    40,    41,    42,    43,
       0,    44,    45,    46,    47,   -68,    48,    49,     0,    50,
      51,    52,     0,  -331,     0,     0,    53,    54,    55,    56,
      57,    58,    59,  -331,   -68,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
      81,    82,   -68,    83,    84,     0,     0,  -207,     0,    85,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,    97,     0,     0,    98,     0,    99,   576,
     100,     0,  -207,     0,     4,   101,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,  -207,    21,    22,
      23,    24,    25,    26,     0,  -207,    27,     0,    28,     0,
      29,    30,    31,  -207,  -207,  -207,    32,    33,    34,   -68,
       0,    35,    36,    37,     0,    38,  -331,    39,     0,    40,
      41,    42,    43,     0,    44,    45,    46,    47,   -68,    48,
      49,     0,    50,    51,    52,     0,  -331,     0,     0,    53,
      54,    55,    56,    57,    58,    59,  -331,   -68,    60,    61,
      62,    63,     0,    64,    65,    66,     0,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,    81,    82,   -68,    83,    84,     0,     0,
    -207,     0,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,    97,     0,     0,    98,
       0,    99,   814,   100,     0,  -207,     0,     4,   101,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -207,    21,    22,    23,    24,    25,    26,     0,  -207,    27,
       0,    28,     0,    29,    30,    31,  -207,  -207,  -207,    32,
      33,    34,   -68,     0,    35,    36,    37,     0,    38,  -331,
      39,     0,    40,    41,    42,    43,     0,    44,    45,    46,
      47,   -68,    48,    49,     0,    50,    51,    52,     0,  -331,
       0,     0,    53,    54,    55,    56,   352,    58,    59,  -331,
     -68,    60,    61,    62,    63,     0,    64,    65,    66,     0,
      67,    68,    69,    70,    71,    72,    73,     0,    74,    75,
       0,    76,    77,    78,    79,    80,    81,    82,   -68,    83,
      84,     0,     0,  -207,     0,    85,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,    97,
       0,     0,    98,     0,    99,     0,   100,     0,  -207,     0,
       4,   101,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -207,    21,    22,    23,    24,    25,    26,
       0,  -207,    27,     0,    28,     0,    29,    30,    31,  -207,
    -207,  -207,    32,    33,    34,   -68,     0,    35,    36,    37,
       0,    38,  -331,    39,     0,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -68,    48,    49,     0,    50,    51,
      52,     0,  -331,     0,     0,    53,    54,    55,    56,    57,
      58,    59,  -331,   -68,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
       0,    74,    75,     0,    76,    77,    78,    79,    80,    81,
      82,   -68,    83,    84,     0,     0,  -207,     0,    85,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,    97,     0,     0,    98,     0,    99,     0,   100,
       0,  -207,     0,     4,   101,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,  -207,    21,    22,    23,
      24,    25,    26,     0,  -207,    27,     0,    28,     0,    29,
      30,    31,  -207,  -207,  -207,    32,    33,    34,   -68,     0,
      35,    36,    37,     0,    38,  -331,    39,     0,    40,    41,
      42,    43,     0,    44,    45,    46,    47,   -68,    48,    49,
       0,    50,    51,    52,     0,  -331,     0,     0,    53,    54,
      55,    56,     0,    58,    59,  -331,   -68,    60,    61,    62,
      63,     0,    64,    65,    66,     0,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,    81,    82,   -68,    83,    84,     0,     0,  -207,
       0,    85,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,    97,     0,     0,    98,     0,
      99,     0,   100,     0,  -207,     0,     4,   101,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -207,
      21,    22,    23,    24,    25,    26,     0,  -207,    27,     0,
      28,     0,    29,    30,    31,  -207,  -207,  -207,    32,  1033,
      34,   -68,     0,    35,    36,    37,     0,    38,  -331,    39,
       0,    40,    41,    42,    43,     0,    44,    45,    46,    47,
     -68,    48,    49,     0,    50,    51,    52,     0,  -331,     0,
       0,    53,    54,    55,    56,     0,    58,    59,  -331,   -68,
      60,    61,    62,    63,     0,    64,    65,    66,     0,    67,
      68,    69,    70,    71,    72,    73,     0,    74,    75,     0,
      76,    77,    78,    79,    80,    81,    82,   -68,    83,    84,
       0,     0,  -207,     0,    85,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,    97,     0,
       0,    98,     0,    99,     0,   100,     0,  -207,     0,     4,
     101,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,  -207,    21,    22,    23,    24,    25,    26,     0,
    -207,    27,     0,    28,     0,    29,    30,    31,  -207,  -207,
    -207,    32,  1178,    34,   -68,     0,    35,    36,    37,     0,
      38,  -331,    39,     0,    40,    41,    42,    43,     0,    44,
      45,    46,    47,   -68,    48,    49,     0,    50,    51,    52,
       0,  -331,     0,     0,    53,    54,    55,    56,     0,    58,
      59,  -331,   -68,    60,    61,    62,    63,     0,    64,    65,
      66,     0,    67,    68,    69,    70,    71,    72,    73,     0,
      74,    75,     0,    76,    77,    78,    79,    80,    81,    82,
     -68,    83,    84,     0,     0,  -207,     0,    85,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,    97,     0,     0,    98,     0,    99,     0,   100,     0,
    -207,     0,     4,   101,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -207,    21,    22,    23,    24,
      25,    26,     0,  -207,    27,     0,    28,     0,    29,    30,
      31,  -207,  -207,  -207,    32,  1182,    34,   -68,     0,    35,
      36,    37,     0,    38,  -331,    39,     0,    40,    41,    42,
      43,     0,    44,    45,    46,    47,   -68,    48,    49,     0,
      50,    51,    52,     0,  -331,     0,     0,    53,    54,    55,
      56,     0,    58,    59,  -331,   -68,    60,    61,    62,    63,
       0,    64,    65,    66,     0,    67,    68,    69,    70,    71,
      72,    73,     0,    74,    75,     0,    76,    77,    78,    79,
      80,    81,    82,   -68,    83,    84,     0,     0,  -207,     0,
      85,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,    97,     0,     0,    98,     0,    99,
       0,   100,     0,  -207,     0,     4,   101,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -207,    21,
      22,    23,    24,    25,    26,     0,  -207,    27,     0,    28,
       0,    29,    30,    31,  -207,  -207,  -207,    32,  1188,    34,
     -68,     0,    35,    36,    37,     0,    38,  -331,    39,     0,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -68,
      48,    49,     0,    50,    51,    52,     0,  -331,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -331,   -68,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,     0,    74,    75,     0,    76,
      77,    78,    79,    80,    81,    82,   -68,    83,    84,     0,
       0,  -207,     0,    85,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,    97,     0,     0,
      98,     0,    99,     0,   100,     0,  -207,     0,     4,   101,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,  -207,    21,    22,    23,    24,    25,    26,     0,  -207,
      27,     0,    28,     0,    29,    30,    31,  -207,  -207,  -207,
      32,  1191,    34,   -68,     0,    35,    36,    37,     0,    38,
    -331,    39,     0,    40,    41,    42,    43,     0,    44,    45,
      46,    47,   -68,    48,    49,     0,    50,    51,    52,     0,
    -331,     0,     0,    53,    54,    55,    56,     0,    58,    59,
    -331,   -68,    60,    61,    62,    63,     0,    64,    65,    66,
       0,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,    81,    82,   -68,
      83,    84,     0,     0,  -207,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,     0,   100,   882,  -207,
       5,     6,   101,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   192,
       0,  -207,    21,     0,     0,     0,     0,     0,     0,     0,
     290,     0,   193,     0,     0,     0,    31,  -207,  -207,  -207,
       0,   194,    34,     0,     0,     0,   292,     0,     0,    38,
       0,   195,     0,   196,     0,     0,    43,     0,     0,    45,
       0,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,    62,   197,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,   297,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,   199,     0,   100,     0,  -207,
       5,     6,   101,     8,     9,    10,    11,    12,     0,   629,
       0,    13,     0,     0,    16,    17,   631,    18,     0,   192,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   193,     0,     0,     0,    31,     0,     0,     0,
       0,   194,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   195,   637,   196,     0,     0,    43,     0,     0,    45,
       0,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,    62,   197,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,   198,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    86,
     504,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,     0,   471,   472,     0,   825,   826,   827,   828,
     829,   650,     0,   651,     0,    92,     0,   652,   653,   654,
     655,   656,   657,   658,   830,   660,   661,    94,   831,    96,
     -90,   663,   664,   832,   666,   199,     0,   100,     0,     0,
       5,     6,   101,     8,     9,    10,    11,    12,     0,   629,
       0,    13,     0,     0,    16,    17,   631,    18,     0,   192,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   193,     0,     0,     0,    31,     0,     0,     0,
       0,   194,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   195,   637,   196,     0,     0,    43,     0,     0,    45,
       0,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,    62,   197,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,   198,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    86,
     504,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,     0,   471,   472,     0,   825,   826,   827,   828,
     829,   650,     0,   651,     0,    92,     0,   652,   653,   654,
     655,   656,   657,   658,   830,   660,   661,    94,   831,    96,
       0,   663,   664,   832,   666,   199,     0,   100,     0,     0,
       0,     0,   101,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,     0,    16,    17,     0,
      18,     0,   192,    20,     0,    21,     0,     0,     0,     0,
      26,     0,     0,    27,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,    33,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,    44,    45,    46,    47,     0,    48,    49,     0,    50,
      51,   329,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,     0,     0,     0,    80,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,    99,     0,
     100,     0,     0,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,     0,    21,     0,   734,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,   194,    34,     0,   735,     0,    36,
       0,   736,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,   737,     0,    54,   738,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
     739,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,     0,     0,   740,    80,
       0,    82,     0,   741,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   199,     0,
     100,     0,     0,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,     0,    21,   373,    23,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,   194,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,   357,     0,     0,     0,     0,     0,    54,    55,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
      64,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,    82,     0,    83,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   199,     0,
     100,     0,     0,     5,     6,   101,     8,     9,    10,   376,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,    19,     0,  -207,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
    -207,  -207,  -207,     0,   194,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,    63,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,     0,     0,     0,    80,
      81,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   199,     0,
     100,     0,  -207,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,  -207,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
    -207,  -207,  -207,     0,   194,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   199,     0,
     100,     0,  -207,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,   194,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,  -738,
       0,     0,     0,  -738,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,  -373,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,  -373,     0,     0,    98,     0,   199,  -373,
     100,     0,     0,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,   194,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,   312,   313,     0,    80,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,   314,     0,     0,    98,     0,   199,     0,
     100,     0,     0,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,   194,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,   312,   313,     0,    80,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,   347,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   199,     0,
     100,     0,     0,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,   194,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,   312,   313,     0,    80,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   199,     0,
     100,  -541,     0,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,   194,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,   312,   313,     0,    80,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   199,     0,
     100,     0,     0,  -541,     0,   101,     5,     6,     0,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   193,     0,
       0,     0,    31,     0,     0,     0,     0,   194,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   195,     0,   196,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   197,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,   198,     0,    74,     0,     0,    76,   312,
     313,     0,    80,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   199,     0,   100,   800,     0,     5,     6,   101,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   193,     0,
       0,     0,    31,     0,     0,     0,     0,   194,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   195,     0,   196,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   197,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,   198,     0,    74,     0,     0,    76,   312,
     313,     0,    80,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   199,     0,   100,   803,     0,     5,     6,   101,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   193,     0,
       0,     0,    31,     0,     0,     0,     0,   194,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   195,     0,   196,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   197,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,   198,     0,    74,     0,     0,    76,   312,
     313,     0,    80,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   199,     0,   100,   809,     0,     5,     6,   101,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   193,     0,
       0,     0,    31,     0,     0,     0,     0,   194,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   195,     0,   196,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,   948,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   197,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,   198,     0,    74,     0,     0,    76,   312,
     313,     0,    80,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   199,     0,   100,     0,     0,     5,     6,   101,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   193,     0,
       0,     0,    31,     0,     0,     0,     0,   194,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   195,     0,   196,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   197,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,   198,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,  -381,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,  -381,     0,     0,    98,
       0,   199,  -381,   100,     0,     0,     5,     6,   101,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   193,     0,
       0,     0,    31,     0,     0,     0,     0,   194,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   195,     0,   196,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   197,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,   198,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,  -366,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,  -366,     0,     0,    98,
       0,   199,  -366,   100,     0,     0,     5,     6,   101,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   193,     0,
       0,     0,    31,     0,     0,     0,     0,   194,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   195,     0,   196,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   197,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,   198,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,  -374,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,  -374,     0,     0,    98,
       0,   199,  -374,   100,     0,     0,     5,     6,   101,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   192,     0,     0,    21,     0,
     254,     0,     0,     0,     0,     0,    27,     0,   193,     0,
       0,     0,    31,     0,     0,     0,     0,   194,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   195,     0,   196,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   197,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,   198,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,    82,     0,   255,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   199,     0,   100,     0,     0,     5,     6,   101,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   193,     0,
       0,     0,    31,     0,     0,     0,     0,   194,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   195,     0,   196,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   197,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,   198,     0,    74,     0,     0,    76,   312,
     313,     0,    80,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   199,     0,   100,     0,     0,     5,     6,   101,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   193,     0,
       0,     0,    31,     0,     0,     0,     0,   194,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   195,     0,   196,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   197,  -738,     0,     0,     0,  -738,     0,     0,    69,
      70,    71,    72,   198,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   199,     0,   100,     0,     0,     5,     6,   101,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   193,     0,
       0,     0,    31,     0,     0,     0,     0,   194,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   195,     0,   196,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   197,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,   198,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,   597,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   199,     0,   100,     0,     0,   598,     0,   101,     5,
       6,     0,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   192,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   193,     0,     0,     0,    31,     0,     0,     0,     0,
     194,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     195,     0,   196,     0,     0,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   197,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,   198,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,   854,
       0,     0,    98,     0,   199,     0,   100,     0,     0,  1019,
       0,   101,     5,     6,     0,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   192,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   193,     0,     0,     0,    31,     0,
       0,     0,     0,   194,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   195,     0,   196,     0,     0,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   197,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,   198,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,  -404,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,   199,     0,   100,
    -404,     0,     5,     6,   101,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   192,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   193,     0,     0,     0,    31,     0,
       0,     0,     0,   194,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   195,     0,   196,     0,     0,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   197,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,   198,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,  -535,     0,     0,    98,     0,   199,     0,   100,
       0,     0,  -535,     0,   101,     5,     6,     0,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   192,     0,     0,    21,     0,     0,
       0,     0,     0,     0,   270,    27,     0,   193,     0,     0,
       0,    31,     0,     0,     0,     0,   194,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   195,     0,   196,     0,
       0,    43,     0,     0,    45,     0,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,    62,
     197,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,   198,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
      99,     0,   100,     0,     0,     5,     6,   101,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   192,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   193,     0,     0,
       0,    31,     0,     0,     0,     0,   194,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   195,     0,   196,     0,
       0,    43,     0,     0,    45,     0,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,    62,
     197,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,   198,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,   597,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     199,     0,   100,     0,     0,     5,     6,   101,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   192,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   193,     0,     0,
       0,    31,     0,     0,     0,     0,   194,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   195,     0,   196,     0,
       0,    43,     0,     0,    45,     0,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,    62,
     197,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,   198,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     199,     0,   100,     0,     0,   598,     0,   101,     5,     6,
       0,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   192,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     193,     0,     0,     0,    31,     0,     0,     0,     0,   194,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   195,
       0,   196,     0,     0,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   197,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,   198,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   199,   798,   100,     0,     0,     5,     6,
     101,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   192,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     193,     0,     0,     0,    31,     0,     0,     0,     0,   194,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   195,
       0,   196,     0,     0,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   197,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,   198,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   199,     0,   100,     0,     0,  1029,     0,
     101,     5,     6,     0,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     192,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   193,     0,     0,     0,    31,     0,     0,
       0,     0,   194,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   195,     0,   196,     0,     0,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   197,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,   198,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   199,     0,   100,     0,
       0,     5,     6,   101,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     192,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,   290,     0,   193,     0,     0,     0,    31,     0,     0,
       0,     0,   194,    34,     0,     0,     0,   292,     0,     0,
      38,     0,   195,     0,   196,     0,     0,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   197,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,   297,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   199,     0,   100,     0,
       0,     5,     6,   101,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     192,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   193,     0,     0,     0,    31,     0,     0,
       0,     0,  1066,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   195,     0,   196,     0,     0,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   197,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,   198,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   199,     0,   100,     0,
       0,     5,     6,   101,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     192,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   193,     0,     0,     0,    31,     0,     0,
       0,     0,  1211,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   195,     0,   196,     0,     0,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   197,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,   198,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   199,     0,   100,     0,
       0,     5,     6,   101,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     192,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   193,     0,     0,     0,    31,     0,     0,
       0,     0,  1212,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   195,     0,   196,     0,     0,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   197,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,   198,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   199,     0,   100,     0,
       0,     5,     6,   101,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     192,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   193,     0,     0,     0,    31,     0,     0,
       0,     0,  1214,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   195,     0,   196,     0,     0,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   197,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,   198,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,     0,     0,     0,    98,     0,   199,     0,   100,     0,
       0,     5,     6,   101,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     192,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   193,     0,     0,     0,    31,     0,     0,
       0,     0,  1215,    34,     0,     0,     0,    36,     0,     0,
      38,     0,   195,     0,   196,     0,     0,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   197,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,   198,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     6,     0,     8,
       9,    10,    11,    12,     0,    93,     0,    13,    94,    95,
      96,    17,     0,     0,    98,   192,   199,  -207,   100,     0,
     289,     0,     0,   101,     0,     0,     0,     0,   193,     0,
       0,     0,     0,  -207,  -207,  -207,     0,   194,     0,     0,
     291,     0,     0,     0,   293,     0,     0,     0,     0,   196,
       0,     0,    43,     0,     0,     0,     0,    47,     0,    48,
      49,     0,     0,     0,     0,     0,     0,     0,   294,     0,
      54,   295,    56,    57,    58,     0,     0,     0,     0,    61,
       0,   197,     0,   296,     0,     0,     0,     0,     0,    69,
      70,     0,    72,     0,     0,     0,     0,     0,     0,     0,
       0,   298,     0,     0,    82,     0,   299,     0,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   627,     0,
     628,     0,    87,    88,    89,    90,    91,     0,     0,   629,
       0,    92,     0,     0,   630,   228,   631,   632,     0,     0,
      93,     0,   633,    94,    95,    96,     0,     0,     0,    98,
     231,   199,   193,   100,  -335,  -207,   232,     0,   101,     0,
       0,     0,   634,     0,     0,     0,   234,     0,     0,   635,
       0,   636,   637,     0,     0,     0,     0,     0,     0,   638,
       0,     0,     0,     0,     0,     0,   238,   639,     0,     0,
       0,     0,     0,     0,   240,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   640,     0,     0,     0,     0,     0,
       0,     0,     0,   242,     0,   641,     0,   244,     0,   642,
       0,     0,   246,     0,     0,     0,   643,     0,   248,     0,
       0,   644,     0,     0,     0,     0,     0,     0,     0,     0,
     504,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,     0,   471,   472,     0,   645,   646,   647,   648,
     649,   650,     0,   651,     0,     0,     0,   652,   653,   654,
     655,   656,   657,   658,   659,   660,   661,     0,   662,     0,
       0,   663,   664,   665,   666,     5,     6,   667,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   192,     0,  -207,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   193,     0,     0,
       0,    31,  -207,  -207,  -207,     0,   194,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   195,     0,   196,     0,
       0,    43,     0,     0,    45,     0,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,    62,
     197,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,   198,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,     0,    95,    96,     0,     0,     0,    98,     0,
     199,     0,   100,     0,  -207,     5,     6,     0,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   192,     0,  -207,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   193,     0,     0,
       0,    31,  -207,  -207,  -207,     0,   194,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   195,     0,   196,     0,
       0,    43,     0,     0,    45,     0,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,    62,
     197,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,   198,     0,    74,     0,     0,    76,     0,   226,
       0,    80,     0,    82,     0,   430,    84,     0,     0,     0,
       0,     0,     0,   227,   228,     0,   229,     0,     0,     0,
       0,   230,     0,     0,     0,     0,     0,     0,     0,   231,
       0,    87,     0,    89,     0,   232,     0,     0,     0,     0,
       0,   233,     0,     0,     0,   234,     0,     0,   235,     0,
     236,     0,     0,     0,    96,     0,     0,   226,   237,     0,
     199,     0,   100,   431,  -207,   238,   239,   432,     0,     0,
       0,   227,   228,   240,   229,     0,     0,     0,     0,   230,
       0,     0,     0,   241,     0,     0,     0,   231,     0,     0,
       0,     0,   242,   232,   243,     0,   244,     0,   245,   233,
       0,   246,     0,   234,     0,   247,   235,   248,   236,     0,
     249,     0,     0,   434,   435,     0,   237,   437,   438,     0,
     439,   440,     0,   238,   239,   443,     0,     0,     0,     0,
       0,   240,   450,     0,     0,     0,     0,     0,   454,   455,
     456,   241,     0,     0,     0,     0,     0,   226,     0,     0,
     242,     0,   243,     0,   244,     0,   245,     0,     0,   246,
       0,   227,   228,   247,   229,   248,   266,     0,   249,   230,
       0,     0,     0,     0,     0,     0,     0,   231,     0,     0,
       0,     0,     0,   232,     0,     0,     0,     0,     0,   233,
     430,     0,     0,   234,     0,     0,   235,     0,   236,     0,
       0,     0,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,   238,   239,     0,     0,     0,     0,     0,
       0,   240,     0,     0,   512,     0,     0,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,   243,     0,   244,   693,   245,     0,   431,   246,
       0,     0,   432,   247,   629,   248,     0,     0,   249,   981,
     982,   631,   983,     0,     0,     0,     0,   984,     0,     0,
       0,     0,     0,     0,     0,   695,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   985,     0,     0,
       0,   986,     0,     0,   987,     0,   195,   637,   434,   435,
       0,   436,   437,   438,   696,   439,   440,     0,     0,     0,
     443,     0,   988,     0,   539,     0,     0,   450,     0,   989,
       0,     0,     0,   454,   455,   456,     0,     0,     0,   990,
       0,     0,     0,     0,     0,     0,     0,     0,   991,     0,
     992,     0,   993,     0,   697,     0,     0,     0,     0,     0,
     698,   994,     0,   995,     0,     0,   996,     0,     0,     0,
       0,     0,     0,     0,     0,   504,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,     0,   471,   472,
       0,   645,   646,   647,   648,   649,   650,     0,   651,     0,
       0,     0,   652,   653,   654,   655,   656,   657,   658,   659,
     660,   661,   226,   662,     0,     0,   663,   664,   665,   666,
     997,   629,     0,     0,     0,     0,   227,   228,   631,   229,
       0,     0,     0,     0,   230,     0,     0,     0,     0,     0,
       0,     0,   231,     0,     0,     0,     0,     0,   232,     0,
       0,     0,     0,     0,   233,     0,     0,     0,   234,     0,
       0,   235,     0,   236,   637,     0,     0,     0,     0,     0,
       0,   237,     0,     0,     0,     0,     0,     0,   238,   239,
       0,     0,     0,     0,     0,     0,   240,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,   428,     0,   242,     0,   243,     0,   244,
     429,   245,     0,     0,   246,     0,     0,     0,   247,     0,
     248,     0,   430,   249,     0,     0,     0,     0,     0,     0,
       0,     0,   504,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   468,   469,     0,   471,   472,     0,   645,   646,
     647,   648,   649,   650,     0,   651,     0,     0,     0,   652,
     653,   654,   655,   656,   657,   658,   659,   660,   661,     0,
     662,     0,     0,   663,   664,   665,   666,     0,     0,     0,
     431,   428,     0,     0,   432,     0,     0,     0,   429,     0,
       0,     0,  1187,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,   504,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   468,   469,     0,   471,   472,     0,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,     0,   454,   455,   456,   431,   359,
       0,   428,   432,     0,     0,     0,   457,     0,   429,     0,
    1246,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,   504,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   468,   469,     0,   471,   472,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,     0,   454,   455,   456,     0,   359,   431,   428,
       0,     0,   432,     0,   457,     0,   429,     0,     0,     0,
    1247,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,   504,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   468,   469,     0,   471,   472,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,     0,   454,   455,   456,   431,   359,     0,   428,
     432,     0,     0,     0,   457,     0,   429,     0,  1248,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     504,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,     0,   471,   472,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,     0,
       0,   454,   455,   456,     0,   359,   431,     0,     0,     0,
     432,     0,   457,     0,     0,     0,     0,   428,  1249,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,   867,     0,   433,
     504,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   868,   471,   472,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,     0,
       0,   454,   455,   456,     0,   359,     0,     0,     0,     0,
       0,     0,   457,     0,   431,   428,     0,     0,   432,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,   270,     0,     0,     0,     0,
       0,     0,     0,   210,     0,     0,     0,   433,     0,     0,
     976,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,     0,     0,   454,
     455,   456,   431,   359,     0,   428,   432,     0,     0,     0,
     457,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,  1130,     0,     0,     0,     0,
       0,   210,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,     0,   454,   455,   456,
       0,   359,   431,   428,     0,     0,   432,     0,   457,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,   270,     0,     0,     0,     0,     0,     0,
       0,   210,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,     0,   454,   455,   456,
     431,   359,     0,   428,   432,     0,     0,     0,   457,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,   270,     0,     0,     0,     0,     0,   210,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,     0,   454,   455,   456,     0,   359,
     431,   428,     0,     0,   432,     0,   457,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,   909,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,   910,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,     0,   454,   455,   456,   431,   359,
       0,     0,   432,     0,     0,     0,   457,     0,     0,     0,
       0,   428,     0,     0,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,   433,     0,     0,     0,  1104,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,  1105,     0,   454,   455,   456,     0,     0,     0,     0,
       0,     0,     0,     0,   457,     0,     0,     0,   431,   428,
       0,     0,   432,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,     0,   454,   455,   456,   431,     0,   428,     0,
     432,     0,     0,     0,   457,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,     0,     0,     0,     0,   210,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,     0,
       0,   454,   455,   456,     0,   431,   428,   525,     0,   432,
    1121,     0,   457,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
       0,     0,     0,     0,   210,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,   428,     0,     0,     0,
       0,     0,     0,   429,     0,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,   430,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,     0,     0,
     454,   455,   456,   431,     0,     0,   428,   432,     0,  1170,
       0,   457,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,   431,     0,     0,     0,   432,     0,     0,
       0,     0,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   433,     0,   454,   455,
     456,     0,     0,   431,   428,     0,     0,   432,     0,   457,
       0,   429,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,   430,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   433,     0,   454,   455,
     456,     0,   548,     0,   428,     0,     0,     0,     0,   457,
       0,   429,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,   430,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,   552,   454,   455,
     456,   431,     0,     0,     0,   432,     0,     0,     0,   457,
       0,     0,     0,   727,     0,     0,   428,   817,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   430,     0,     0,     0,     0,
       0,   431,     0,     0,     0,   432,     0,     0,     0,     0,
       0,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   433,     0,   454,   455,   456,     0,
       0,     0,     0,     0,     0,     0,     0,   457,     0,     0,
       0,   434,   435,   431,   436,   437,   438,   432,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   428,     0,   454,   455,   456,     0,
       0,   429,     0,   799,     0,     0,   433,   457,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   818,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   428,     0,   454,   455,
     456,     0,     0,   429,     0,     0,     0,     0,     0,   457,
       0,     0,     0,     0,     0,   430,     0,     0,   939,     0,
       0,   431,     0,     0,     0,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   428,     0,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   861,   433,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   434,   435,   431,   436,   437,   438,   432,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,     0,     0,   454,   455,   456,     0,
       0,     0,     0,     0,     0,     0,   433,   457,     0,     0,
       0,     0,     0,   431,     0,     0,     0,   432,     0,     0,
       0,     0,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   433,     0,   454,   455,
     456,     0,     0,     0,   428,     0,     0,     0,     0,   457,
       0,   429,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,   430,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,     0,   454,   455,
     456,     0,     0,     0,     0,   951,     0,     0,     0,   457,
       0,     0,   428,  1006,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,     0,     0,     0,   432,     0,     0,     0,     0,
       0,     0,   428,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,     0,     0,   433,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,   434,   435,   432,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,     0,     0,   454,   455,   456,     0,
       0,     0,   433,   952,     0,     0,     0,   457,     0,   431,
       0,     0,     0,   432,     0,     0,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   433,     0,   454,   455,   456,     0,     0,     0,
     428,  1103,     0,     0,     0,   457,     0,   429,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,   430,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,     0,   454,   455,   456,     0,     0,     0,
     428,  1058,     0,     0,     0,   457,     0,   429,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   431,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
     433,     0,     0,     0,     0,     0,     0,   431,     0,     0,
       0,   432,     0,     0,     0,     0,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     433,     0,   454,   455,   456,     0,     0,     0,     0,     0,
       0,     0,     0,   457,     0,   431,     0,   434,   435,   432,
     436,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,  1115,   454,   455,   456,   428,     0,  1125,   433,     0,
       0,     0,   429,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,     0,     0,
     454,   455,   456,   428,     0,     0,     0,     0,     0,     0,
     429,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,  1156,     0,     0,     0,     0,     0,     0,
       0,     0,   431,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,   428,  1175,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,     0,   434,   435,   432,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,     0,   454,   455,   456,
       0,     0,     0,   433,     0,     0,  1153,     0,   457,     0,
     431,   428,     0,     0,   432,     0,     0,     0,   429,     0,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     430,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   433,     0,   454,   455,   456,     0,     0,
       0,   428,     0,     0,     0,     0,   457,     0,   429,  1206,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     430,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,     0,   454,   455,   456,   431,     0,
       0,   428,   432,     0,     0,     0,   457,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,   431,     0,
       0,     0,   432,     0,     0,     0,     0,     0,   434,   435,
    1213,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   433,     0,   454,   455,   456,     0,     0,   431,   428,
       0,     0,   432,     0,   457,     0,   429,     0,   434,   435,
    1250,   436,   437,   438,     0,   439,   440,   441,   430,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   433,     0,   454,   455,   456,     0,     0,     0,   428,
       0,     0,     0,     0,   457,     0,   429,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,   430,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,     0,   454,   455,   456,   431,     0,     0,   428,
     432,     0,     0,     0,   457,     0,   429,     0,  1251,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
     432,     0,     0,     0,     0,     0,   434,   435,  1252,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   433,
       0,   454,   455,   456,     0,     0,   431,   428,     0,     0,
     432,     0,   457,     0,   429,     0,   434,   435,  1253,   436,
     437,   438,     0,   439,   440,   441,   430,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   433,
       0,   454,   455,   456,     0,     0,     0,   428,     0,     0,
       0,     0,   457,     0,   429,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,   430,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,     0,
       0,   454,   455,   456,   431,     0,     0,     0,   432,     0,
       0,     0,   457,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,   929,     0,     0,     0,   432,     0,
       0,     0,     0,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   433,   430,   454,
     455,   456,     0,     0,   431,     0,     0,     0,   432,     0,
     457,     0,     0,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   433,   430,   454,
     455,   456,     0,     0,     0,     0,     0,     0,     0,     0,
     457,     0,     0,     0,   434,   435,   431,   436,   437,   438,
     432,   439,   440,   441,     0,   442,   443,   444,   445,     0,
     447,   448,   449,   450,   451,   452,   453,     0,     0,   454,
     455,   456,     0,     0,     0,     0,     0,     0,     0,   433,
     457,     0,     0,     0,     0,     0,   431,     0,     0,     0,
     432,     0,     0,     0,     0,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,     0,   447,   448,   449,   450,   451,     0,   453,     0,
       0,   454,   455,   456,     0,     0,     0,     0,     0,     0,
       0,     0,   457,     0,     0,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,     0,     0,   443,   444,
     445,   226,   447,   448,   449,   450,     0,     0,   453,     0,
       0,   454,   455,   456,     0,   227,   228,     0,   229,     0,
       0,     0,   457,   230,     0,    23,     0,     0,     0,     0,
       0,   231,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,     0,   233,     0,     0,     0,   234,     0,     0,
     235,     0,   236,     0,     0,     0,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,   238,   239,     0,
       0,     0,     0,     0,     0,   240,    55,     0,     0,     0,
       0,     0,     0,     0,     0,   241,     0,     0,    64,     0,
       0,   226,     0,     7,   242,     0,   243,     0,   244,     0,
     245,     0,     0,   246,     0,   227,   228,   247,   229,   248,
       0,    83,   249,   230,     0,     0,     0,     0,     0,     0,
       0,   231,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,     0,   233,     0,     0,     0,   234,     0,     0,
     235,     0,   236,     0,     0,     0,     0,     0,     0,  -308,
     237,     0,     0,     0,     0,     0,     0,   238,   239,     0,
       0,     0,     0,  -308,  -308,   240,  -308,     0,     0,     0,
       0,  -308,     0,     0,     0,   241,     0,     0,     0,  -308,
       0,     0,     0,     0,   242,  -308,   243,     0,   244,     0,
     245,  -308,     0,   246,     0,  -308,     0,   247,  -308,   248,
    -308,     0,   249,     0,     0,     0,     0,   226,  -308,     0,
       0,     0,     0,     0,     0,  -308,  -308,     0,     0,     0,
       0,   227,   228,  -308,   229,     0,     0,     0,     0,   230,
       0,     0,     0,  -308,     0,     0,     0,   231,     0,     0,
       0,     0,  -308,   232,  -308,     0,  -308,     0,  -308,   233,
       0,  -308,     0,   234,     0,  -308,   235,  -308,   236,     0,
    -308,     0,     0,     0,     0,  -309,   237,     0,     0,     0,
       0,     0,     0,   238,   239,     0,     0,     0,     0,  -309,
    -309,   240,  -309,     0,     0,     0,     0,  -309,     0,     0,
       0,   241,     0,     0,     0,  -309,     0,     0,     0,     0,
     242,  -309,   243,     0,   244,     0,   245,  -309,     0,   246,
       0,  -309,     0,   247,  -309,   248,  -309,     0,   249,     0,
       0,     0,     0,     0,  -309,   354,     0,     0,     0,    22,
      23,  -309,  -309,     0,     0,     0,     0,     0,   355,  -309,
      29,   356,     0,     0,     0,     0,    32,     0,     0,  -309,
       0,     0,     0,    37,     0,     0,     0,     0,  -309,     0,
    -309,     0,  -309,     0,  -309,     0,     0,  -309,     0,     0,
       0,  -309,     0,  -309,   357,     0,  -309,     0,     0,    53,
       0,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   358,     0,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    79,     0,    81,     0,     0,    83
};

static const yytype_int16 yycheck[] =
{
       2,   178,    26,   109,    61,     2,   261,     2,   397,   162,
     546,   615,   584,    61,   573,   486,    66,   670,   419,   672,
     510,   622,   786,    25,   823,   876,   219,   888,   541,   377,
      25,   286,   130,   602,   780,    85,   818,     1,     1,     5,
      24,    65,     1,     1,   200,    56,   841,    47,    32,   225,
     100,    56,   517,    35,    22,   766,    40,    41,    42,    47,
     101,   116,    32,     3,    47,     0,   599,   600,    32,    47,
     325,    73,   605,   606,   118,    77,    78,    24,    73,     3,
     257,    46,    77,    78,   954,   261,     3,    32,    32,   118,
     145,   118,     1,    40,    41,    42,    60,    99,    46,    60,
       2,   103,    90,   163,    99,    32,   145,    90,   103,   145,
     286,    32,    32,    32,    54,    74,    32,    65,   118,    84,
      88,   512,    56,    25,   145,   145,   108,   168,   118,   173,
      54,     2,   108,   117,   173,   171,    84,    54,    47,    47,
     104,    32,   110,   163,   173,   145,   173,   112,   539,   325,
     171,   121,   116,   139,    25,   116,   115,   622,  1009,   170,
     123,   147,   102,   163,   112,   170,   145,   145,    92,    24,
     168,    73,   170,   173,   118,    77,    78,    32,   102,   145,
     144,   144,   145,   947,    56,    40,    41,    42,   172,   387,
     954,   118,   171,   752,  1045,  1046,    66,    99,   168,   163,
     145,   103,    73,   169,   168,   361,    77,    78,   171,   211,
     169,   957,   170,  1008,   123,    85,   211,   928,    47,  1089,
     475,   476,  1004,   168,   168,   172,   145,   118,    99,   170,
     100,   172,   103,   431,   432,   144,   145,   145,   728,   587,
       3,   168,   419,   163,    56,    28,    53,   168,   168,   168,
     822,    74,   168,    47,   163,   163,   145,    40,    41,    42,
    1111,    47,   117,   265,   440,   173,    73,    46,   270,    56,
     265,   784,   805,   806,   807,   270,    83,   168,   376,   477,
     478,    90,   171,  1134,   853,   483,    65,  1148,   145,   118,
     170,    54,   115,   145,    24,   550,    90,    18,   145,   475,
     476,   123,    32,   145,    90,    84,  1105,  1071,    11,   211,
      40,    41,    42,   145,   171,   163,   145,   172,    31,   171,
     145,   170,   170,   172,   171,  1089,   145,   149,    56,   171,
       3,   163,   145,   112,   510,   145,   529,   490,   163,   102,
     211,    27,    56,   345,   173,   145,   169,    20,   345,   163,
     345,    56,   171,   170,   955,   225,    77,   359,   171,   173,
      33,   171,   386,   265,   359,   148,   368,   145,   270,   145,
     394,   171,   570,   368,   550,    96,    89,   160,   145,   381,
      93,    54,    50,   145,   874,    53,   381,   117,  1141,    62,
     111,   261,    78,   171,   265,   171,   145,   573,   591,   270,
      82,   163,   169,    85,    90,    73,  1059,  1060,    76,   145,
     145,   102,    27,   196,   163,    83,   286,   419,   977,   145,
     470,   911,   145,   473,   474,   997,   139,   113,   163,   102,
      56,   144,    47,   169,   147,   108,    51,   163,   151,    56,
     108,   145,   172,   345,   915,  1049,   169,  1200,  1201,  1202,
    1203,  1204,  1205,    77,    78,   325,    27,   359,   482,   163,
      75,   287,   288,    78,   145,    80,   368,   163,   145,    77,
      78,   254,   255,     2,   345,    90,    47,   173,    56,   381,
      51,   538,   163,   266,  1017,  1018,   163,   876,   359,     9,
     538,   892,    27,   108,   163,   123,    25,   368,   113,   501,
     169,   503,  1035,  1036,    75,   170,   501,    78,   503,   615,
     381,   145,    47,   563,   564,   145,    51,    56,     2,    90,
     145,   163,   150,   306,   152,   153,   163,   155,   156,   163,
      31,   173,  1104,   163,  1138,    89,   173,   108,   163,    93,
      75,    25,   113,    78,    73,   569,    89,   163,    77,    78,
      93,    31,   728,   169,    99,    90,   171,    89,    89,    89,
      56,    93,    93,    93,  1097,  1098,  1099,  1100,   170,   170,
      99,   172,   355,   108,   103,   577,   752,   601,   113,   139,
     577,    89,   577,    56,   144,    93,    56,   147,    89,    73,
      56,   151,    93,    77,    78,   170,   170,   172,   172,   501,
     470,   503,    56,   473,   474,   475,   476,     5,   391,    89,
     393,  1147,   168,    93,   397,    99,    56,   563,   564,   103,
    1009,   170,   147,   172,   168,   778,   144,   163,   162,    56,
     501,   168,   503,    84,   144,   163,   419,   163,   139,   163,
     510,   817,   818,   144,   163,   146,   147,   163,   163,   163,
     151,   163,   163,   170,   852,    31,  1045,   158,   171,   139,
     140,  1050,   142,   143,   144,   166,   146,   147,   148,   145,
     868,   151,   170,   170,   163,   577,   170,   157,   158,   171,
     550,   161,   211,   145,   164,   165,   166,     9,   163,     3,
     843,   170,    65,   563,   564,   175,    78,   168,   874,   123,
     750,   484,   163,   573,   147,    68,   577,   170,   906,   123,
     145,   171,   910,    89,   123,   892,   123,    93,    34,   769,
      65,    34,  1111,   171,   123,   171,   144,   211,    21,   512,
     171,   929,   930,   117,   168,   911,   265,   170,   169,   145,
     123,   270,   525,   745,   104,   170,   147,   147,   139,   171,
     147,   147,   535,   147,   756,   147,   539,   759,   147,    27,
     147,   756,   147,   139,   759,   173,   147,   147,   144,   147,
     146,   147,   147,   147,    32,   151,   974,   147,   976,    47,
      44,   265,   158,    51,   163,   171,   270,   789,   164,   165,
     166,   163,   789,   170,   789,   163,   163,    34,    34,   145,
     583,   977,   144,   169,    68,   163,   171,    75,    47,   168,
      78,   594,    80,   145,   171,   169,   345,   173,   173,   876,
     173,    34,    90,    27,   848,   163,   163,  1082,  1083,   170,
     359,   888,   615,   144,   170,   163,   145,   171,   173,   368,
     108,    25,    32,    47,   171,   113,    34,    51,   850,   173,
     173,   163,   381,   855,   756,   850,   171,   759,   728,   163,
     855,   345,   864,   171,   162,   867,   162,   145,   108,   864,
     572,    75,   867,   977,    78,   359,    80,   368,   815,    24,
     750,   385,   752,   361,   368,   756,    90,   789,   759,    73,
     871,   260,   334,   676,   970,   538,   523,   381,   379,   769,
     892,  1148,   537,   956,   108,   300,  1082,  1083,  1147,   113,
    1206,  1199,   842,   525,   128,   676,   520,    20,   789,   103,
     765,   915,    -1,    -1,    -1,    -1,    -1,   929,   930,    -1,
      -1,    -1,    -1,    -1,   936,    -1,   938,    -1,    -1,   941,
    1046,   936,   966,   938,    -1,    -1,   941,    -1,   850,  1126,
      -1,    -1,  1009,   855,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   864,   965,    -1,   867,    -1,    -1,    -1,    -1,
     965,    -1,   501,   975,   503,    -1,    -1,    -1,   761,   850,
     975,    -1,   765,    -1,   855,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   775,   864,    -1,    -1,   867,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   874,    -1,    -1,    -1,    -1,   273,
      -1,   275,    -1,   796,  1016,    -1,    -1,   501,    -1,   503,
      -1,  1016,    -1,  1025,    -1,    -1,    -1,   211,  1134,    -1,
    1025,    -1,  1138,    -1,   936,    -1,   938,    -1,    -1,   941,
      -1,   911,    -1,    -1,    -1,    -1,    -1,  1224,   577,  1226,
      -1,    -1,  1229,  1230,  1111,    -1,    -1,    -1,   322,   842,
      -1,    -1,    -1,   965,    -1,   936,    -1,   938,    -1,    -1,
     941,    -1,    -1,   975,    -1,    -1,  1078,    -1,    -1,    -1,
      -1,   265,    -1,  1078,    -1,    -1,   270,    -1,   871,    -1,
      -1,  1148,    -1,   577,   965,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   975,    -1,    -1,   977,    -1,   892,
      -1,    -1,    -1,    -1,  1016,    -1,    -1,  1119,    -1,  1121,
      -1,    -1,    -1,  1025,  1119,  1127,  1121,    -1,  1130,    -1,
      -1,    -1,  1127,    -1,    -1,  1130,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1016,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1025,    -1,    -1,  1159,    -1,    -1,
      -1,     2,    -1,  1165,  1159,    -1,    -1,  1169,  1170,    -1,
    1165,    -1,    -1,    -1,  1169,  1170,  1078,    -1,    -1,  1181,
      -1,    -1,    -1,  1185,    25,  1187,  1181,   970,    -1,    -1,
    1185,    -1,  1187,    -1,    -1,    -1,    -1,   381,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1078,    -1,    -1,
      -1,    -1,  1082,  1083,    -1,    -1,    -1,  1119,    -1,  1121,
      -1,    -1,    -1,    -1,    -1,  1127,    -1,   756,  1130,    -1,
     759,    -1,    73,    -1,    -1,    -1,    77,    78,    -1,    -1,
      -1,    -1,    -1,    -1,  1246,  1247,  1248,  1249,  1119,    -1,
    1121,  1246,  1247,  1248,  1249,    -1,  1127,  1159,    99,  1130,
     789,    -1,   103,  1165,    -1,    -1,  1049,  1169,  1170,    -1,
      -1,    -1,   756,    -1,    -1,   759,    -1,    -1,    -1,  1181,
      -1,    -1,    -1,  1185,    -1,  1187,    -1,    -1,  1159,    -1,
      -1,    -1,    -1,    -1,  1165,    -1,    -1,    -1,  1169,  1170,
      -1,    -1,    -1,    -1,    -1,   789,    -1,    -1,    -1,    -1,
    1181,    -1,    -1,    -1,  1185,    -1,  1187,   501,    -1,   503,
      -1,   850,    -1,    -1,    -1,     2,   855,    -1,    -1,    31,
      -1,   595,   596,    -1,    -1,   864,    -1,    -1,   867,    -1,
      -1,   605,   606,    -1,  1246,  1247,  1248,  1249,    25,    -1,
      -1,    -1,    -1,    -1,    -1,  1138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   850,    -1,    -1,    -1,
     211,   855,    -1,    -1,    -1,  1246,  1247,  1248,  1249,    -1,
     864,    -1,    -1,   867,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,
      77,    78,    -1,    -1,    -1,    -1,    -1,   936,    -1,   938,
      -1,    -1,   941,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   265,    -1,   103,    -1,    -1,   270,
      -1,    -1,    -1,    -1,    -1,    -1,   965,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   975,    -1,    -1,   151,
      -1,    -1,   936,    -1,   938,   157,   158,   941,    -1,   161,
      -1,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,     1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   965,    -1,    -1,    -1,    17,    18,  1016,    20,    -1,
      -1,   975,    -1,    25,    -1,    -1,  1025,    -1,    -1,    -1,
      -1,    33,    -1,    -1,   345,    -1,    -1,    39,    -1,    -1,
      -1,    -1,     2,    45,    -1,    -1,    -1,    49,   359,    -1,
      52,    -1,    54,    -1,    -1,    -1,    -1,   368,    -1,    -1,
      62,    -1,  1016,    -1,   211,    25,    -1,    69,    70,    -1,
     381,  1025,   806,   807,    -1,    77,    -1,    -1,    -1,  1078,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    98,    -1,   100,    -1,
     102,    -1,   756,   105,    -1,   759,    -1,   109,    -1,   111,
      -1,    -1,   114,    73,    -1,    -1,    -1,    -1,   265,    -1,
    1119,    -1,  1121,   270,  1078,    -1,    -1,    -1,  1127,    -1,
       3,  1130,   866,    -1,    -1,    -1,   870,    -1,    -1,    99,
      -1,    -1,    -1,   103,    17,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1159,    -1,    35,    -1,    -1,  1119,  1165,  1121,   170,    -1,
    1169,  1170,    45,  1127,    -1,    -1,  1130,    -1,    -1,    52,
      -1,    54,  1181,    -1,    -1,    -1,  1185,    -1,  1187,    62,
     501,    -1,   503,    -1,    -1,    -1,   850,    70,   345,    -1,
      -1,   855,    -1,    -1,    -1,  1159,    -1,    -1,    -1,    -1,
     864,  1165,   359,   867,    87,  1169,  1170,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    98,    -1,  1181,    -1,   102,
      -1,  1185,    -1,  1187,   381,    -1,   109,    -1,    -1,    -1,
      -1,   114,    -1,     5,    -1,    -1,    -1,  1246,  1247,  1248,
    1249,   211,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    26,    27,   577,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,
      -1,    43,   936,    -1,   938,    -1,    -1,   941,    50,    -1,
      -1,    -1,  1246,  1247,  1248,  1249,    -1,    -1,    -1,    -1,
      -1,  1035,  1036,    -1,    -1,   265,    -1,    -1,    -1,    71,
     270,   965,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      82,   975,    -1,    85,    -1,    -1,    88,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,
       5,    -1,    -1,    -1,    -1,    -1,   108,    -1,   110,    -1,
      -1,   113,    17,    18,   501,    20,   503,    -1,    -1,  1093,
      25,    -1,  1016,    -1,    -1,  1099,  1100,    -1,    33,    -1,
      -1,  1025,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,  1116,    -1,    49,   345,    -1,    52,    -1,    54,
      -1,    -1,    -1,    -1,    -1,  1129,    -1,    62,  1132,   359,
      -1,   163,    -1,    -1,    69,    70,    -1,   169,   368,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   381,    87,    -1,  1078,    -1,    -1,    -1,    -1,    -1,
     577,    96,    -1,    98,    -1,   100,    12,   102,    -1,    -1,
     105,    -1,    -1,    19,   109,   756,   111,    -1,   759,   114,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    -1,  1119,    -1,  1121,    -1,    -1,
      -1,    -1,    -1,  1127,    -1,    -1,  1130,    -1,   789,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,  1159,    -1,    -1,    -1,    -1,
      -1,  1165,    -1,    89,    -1,  1169,  1170,    93,    -1,    -1,
      -1,    -1,    -1,   167,    -1,    -1,   170,  1181,   172,    -1,
      -1,  1185,    -1,  1187,    -1,    -1,    -1,    -1,    -1,   850,
      -1,   501,    -1,   503,   855,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,   864,    -1,    -1,   867,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,   163,   164,   165,
     166,    -1,  1246,  1247,  1248,  1249,    -1,    -1,    -1,   175,
       4,    -1,     6,     7,     8,     9,    10,    -1,    -1,   756,
      14,    -1,   759,    -1,    18,    -1,    -1,   577,    22,    -1,
      24,    -1,    -1,    27,    -1,   936,    -1,   938,    -1,    -1,
     941,    35,    -1,    -1,    -1,    -1,    40,    41,    42,    -1,
      44,    -1,   789,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    56,    -1,   965,    59,    -1,    -1,    -1,    -1,
      64,    -1,    66,    67,   975,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    78,    79,    80,    81,    -1,    -1,
      -1,    -1,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   850,   108,  1016,    -1,   111,   855,   113,
      -1,    -1,    -1,    -1,  1025,    -1,    -1,   864,   122,    -1,
     867,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
       5,    -1,   166,    -1,   168,    -1,   170,  1078,   172,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,   936,
      35,   938,    37,    38,   941,    -1,   756,    -1,    43,   759,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,  1119,    -1,
    1121,    -1,    -1,    -1,    -1,    -1,  1127,    -1,   965,  1130,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,   975,   789,
      -1,    76,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      85,    -1,    -1,    88,    -1,    90,    -1,    -1,  1159,    -1,
      -1,    -1,    -1,    -1,  1165,    -1,    -1,    -1,  1169,  1170,
      -1,    -1,    -1,   108,    -1,   110,    -1,    -1,   113,  1016,
    1181,    -1,    -1,    -1,  1185,     1,  1187,    -1,  1025,     5,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     850,    -1,    -1,    -1,    -1,   855,    22,    -1,    -1,    -1,
      26,    27,    -1,    -1,   864,    -1,    -1,   867,    -1,    35,
      -1,    37,    38,    -1,    -1,    -1,    -1,    43,   163,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      -1,  1078,    -1,    -1,    -1,  1246,  1247,  1248,  1249,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    82,    83,    -1,    85,
      -1,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1119,    -1,  1121,    -1,   936,    -1,   938,    -1,
    1127,   941,   108,  1130,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    26,    27,    -1,   965,    -1,    -1,    -1,    -1,
      -1,    35,  1159,    37,    38,   975,    -1,    -1,  1165,    43,
      -1,    -1,  1169,  1170,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,    57,  1181,    -1,    -1,   163,  1185,    -1,
    1187,    65,    -1,   169,    -1,    -1,    -1,    71,    -1,    73,
      -1,    -1,    76,    -1,    78,    -1,  1016,    -1,    82,    83,
      84,    85,    -1,    -1,    88,  1025,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,
      -1,    -1,    17,    -1,   108,    -1,   110,    -1,    -1,   113,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    33,  1246,
    1247,  1248,  1249,    12,    13,    -1,    -1,    -1,    -1,    44,
      19,    46,    47,    -1,    -1,    -1,    -1,    52,  1078,    54,
      -1,    56,    31,    -1,    -1,    -1,    61,    36,    -1,    -1,
      65,    66,    67,    68,    69,    -1,    -1,    -1,    73,   163,
      75,    -1,    -1,    -1,   168,    31,    -1,    82,    -1,    -1,
      85,    -1,    87,    -1,    89,    -1,    -1,    92,    93,  1119,
      95,  1121,    -1,    72,    99,   100,   101,  1127,    -1,    -1,
    1130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,    -1,   130,    -1,    -1,    -1,  1159,
      -1,    -1,    -1,    89,    -1,  1165,    -1,    93,    -1,  1169,
    1170,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1181,    -1,    -1,    -1,  1185,    -1,  1187,    -1,    -1,
     139,   140,    -1,   142,   143,   144,   171,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   139,   140,   164,   165,   166,   144,   194,
     146,   147,    -1,   198,   199,   151,   175,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,    -1,    -1,    -1,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,  1246,  1247,  1248,  1249,
     225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,
      -1,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,   261,    22,    -1,    24,
      25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    42,    -1,    44,
      45,   286,    47,    -1,    49,    -1,    51,    52,    -1,    54,
      -1,    56,   297,    -1,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,   312,   313,    -1,
      75,    -1,    77,    78,    79,    -1,    81,    -1,    -1,    -1,
     325,    86,    87,    88,   329,    90,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,   347,   108,   109,    -1,   111,    -1,   113,   114,
      -1,    -1,   357,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,   369,    -1,   371,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
     385,   386,   387,   148,    -1,    -1,    -1,    -1,    -1,   394,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   428,   429,   430,   431,   432,   433,    -1,
     435,   436,   437,   438,   439,    -1,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   470,    -1,    -1,   473,   474,
     475,   476,   477,   478,    -1,    44,    -1,   482,   483,    -1,
      -1,   486,    -1,    -1,    -1,    -1,    -1,   492,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   503,    68,
      -1,   506,    -1,    -1,    -1,   510,    -1,    -1,    77,    78,
      -1,    -1,   517,    -1,   519,    -1,    -1,    -1,   523,    -1,
      -1,   526,   527,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   538,    -1,    -1,   541,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,   550,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   560,    -1,    -1,   563,   564,
      -1,    -1,    -1,    -1,   569,   570,    -1,   136,   573,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   584,
     585,    -1,    -1,     3,     4,    -1,     6,     7,     8,     9,
      10,    -1,    -1,   598,    14,    -1,   601,    17,    18,    -1,
      20,    -1,    22,    -1,    24,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,   622,    -1,    39,
      40,    41,    42,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,   217,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,   667,    -1,    -1,    -1,    86,    87,    88,   674,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,   256,   257,   109,
      -1,   111,    -1,    -1,   114,     5,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   273,    -1,   275,    -1,    -1,    -1,
      -1,    -1,    22,   282,    -1,    -1,    26,    27,    -1,   139,
      -1,   141,   727,   728,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    -1,   162,    -1,    -1,   750,    -1,   752,   753,   754,
     170,    -1,   172,   322,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    -1,   769,    -1,    76,    -1,    78,    -1,
      -1,    -1,    82,    -1,    -1,    85,    -1,    -1,    88,   784,
      90,    -1,    -1,   788,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
     815,    -1,    -1,    -1,   819,    -1,   821,   822,   823,    -1,
     825,    -1,   827,    -1,    -1,   830,   831,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   844,
      -1,    -1,    -1,   848,    -1,    -1,    -1,   852,   853,    -1,
     855,    -1,    -1,   163,    -1,     5,   861,   862,    -1,   864,
      -1,    -1,   867,   868,    -1,    -1,    -1,    -1,    -1,   874,
      -1,    -1,    22,    -1,   879,    -1,    26,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      50,   906,   907,   908,   909,   910,   911,    -1,    -1,    -1,
     915,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    -1,   929,   930,    76,    -1,    78,    -1,
      80,    -1,    82,    -1,   939,    85,    -1,    -1,    88,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     955,    -1,    -1,    -1,   959,    -1,    -1,    -1,   108,    -1,
     110,   966,    -1,   113,    -1,    -1,    -1,    -1,    -1,   974,
      -1,   976,   977,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   553,    -1,    -1,    -1,    17,    18,
      -1,    20,   997,    -1,    -1,    -1,    25,    -1,    -1,  1004,
      -1,  1006,    -1,    -1,    33,    -1,    -1,    -1,  1013,    -1,
      39,    -1,    -1,   163,    -1,    -1,    45,    -1,    -1,   169,
      49,  1026,    -1,    52,    -1,    54,   595,   596,  1033,    -1,
      -1,    -1,    -1,    62,    -1,    -1,   605,   606,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,  1052,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,  1066,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,
      -1,   100,    -1,   102,    -1,    -1,   105,  1082,  1083,    -1,
     109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
    1095,     5,    -1,    -1,    -1,    -1,    -1,    -1,  1103,  1104,
    1105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    26,    27,  1119,    -1,  1121,    -1,    -1,    -1,
    1125,    35,  1127,    37,    38,  1130,    -1,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,   168,
      -1,   170,    12,    -1,    -1,    -1,    -1,  1152,  1153,    19,
    1155,  1156,    -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    31,    76,    -1,    78,    -1,    80,    -1,    82,    -1,
    1175,    85,    -1,  1178,    88,    -1,    90,  1182,    -1,    -1,
      -1,    -1,  1187,  1188,    -1,    -1,  1191,    -1,    -1,    -1,
      -1,   760,    -1,  1198,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,  1211,  1212,  1213,  1214,
    1215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,   796,    -1,    -1,
      -1,   101,    -1,    -1,    -1,    -1,    -1,   806,   807,    -1,
      -1,  1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,   163,
      -1,    -1,   122,    -1,    -1,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,   866,   168,    -1,
      -1,   870,    -1,   872,    -1,   175,    -1,    -1,    -1,    -1,
      -1,     0,     1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    54,    -1,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    31,    73,    -1,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,   973,    94,    95,    96,    97,    98,
      99,   100,    -1,   102,   103,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    -1,    -1,   117,    -1,
     119,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,  1035,  1036,   157,    12,
      -1,   160,   161,   162,   163,    -1,    19,   166,    -1,   168,
      -1,   170,    -1,   172,    -1,    -1,   175,    -1,    31,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,    -1,   155,   156,   157,
     158,   159,    -1,   161,    -1,    -1,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,  1093,    -1,    -1,   175,    -1,    -1,
    1099,  1100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,  1116,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,  1126,   101,    -1,
    1129,    -1,    -1,  1132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,   136,   137,    -1,   139,   140,  1167,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,    -1,   168,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1224,    -1,  1226,    -1,    -1,
    1229,  1230,     1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    18,
      19,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    54,    -1,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    -1,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,    -1,   102,   103,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    -1,    -1,   117,    -1,
     119,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,   168,
     169,   170,    -1,   172,    -1,     1,   175,     3,     4,     5,
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
     166,    -1,   168,   169,   170,    -1,   172,    -1,     1,   175,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    54,    -1,    56,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,    -1,   102,
     103,    -1,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    -1,    -1,   117,    -1,   119,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
     163,    -1,    -1,   166,    -1,   168,   169,   170,    -1,   172,
      -1,     1,   175,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    54,    -1,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,    -1,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,   163,    -1,    -1,   166,    -1,   168,   169,
     170,    -1,   172,    -1,     1,   175,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    49,    50,    -1,    52,    53,    54,    -1,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,    -1,   102,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    -1,    -1,
     117,    -1,   119,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,
      -1,   168,   169,   170,    -1,   172,    -1,     1,   175,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      54,    -1,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    80,    81,    82,    83,
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
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    80,
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
     168,    -1,   170,    -1,   172,    -1,     1,   175,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    49,    50,    -1,    52,    53,    54,
      -1,    56,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    94,
      95,    96,    97,    98,    99,   100,    -1,   102,   103,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      -1,    -1,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,     1,
     175,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    49,    50,    -1,
      52,    53,    54,    -1,    56,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,    -1,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,   117,    -1,   119,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,   163,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
     172,    -1,     1,   175,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    54,    -1,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    -1,    81,    82,    83,    84,    85,    86,    87,    88,
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
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
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
     163,    -1,    -1,   166,    -1,   168,    -1,   170,     1,   172,
       3,     4,   175,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    42,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
      -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,
       3,     4,   175,     6,     7,     8,     9,    10,    -1,    12,
      -1,    14,    -1,    -1,    17,    18,    19,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    55,    56,    -1,    -1,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,   136,   137,    -1,   139,   140,   141,   142,
     143,   144,    -1,   146,    -1,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,    -1,   170,    -1,    -1,
       3,     4,   175,     6,     7,     8,     9,    10,    -1,    12,
      -1,    14,    -1,    -1,    17,    18,    19,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    55,    56,    -1,    -1,    59,    -1,    -1,    62,
      -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,   136,   137,    -1,   139,   140,   141,   142,
     143,   144,    -1,   146,    -1,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,   164,   165,   166,   167,   168,    -1,   170,    -1,    -1,
      -1,    -1,   175,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    -1,    17,    18,    -1,
      20,    -1,    22,    23,    -1,    25,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      -1,    61,    62,    63,    64,    -1,    66,    67,    -1,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
     100,    -1,   102,   103,    -1,   105,    -1,    -1,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,    -1,     3,     4,   175,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,    49,
      -1,    51,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    77,    78,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,   109,
      -1,   111,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,    -1,     3,     4,   175,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    26,    27,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,   111,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,    -1,     3,     4,   175,     6,     7,     8,     9,
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
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,     3,     4,   175,     6,     7,     8,     9,
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
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,    -1,   172,     3,     4,   175,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    89,
      -1,    -1,    -1,    93,    -1,    -1,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,   145,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,   163,    -1,    -1,   166,    -1,   168,   169,
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
     100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,   163,    -1,    -1,   166,    -1,   168,    -1,
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
     100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,   149,
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
     100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,
     170,   171,    -1,     3,     4,   175,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
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
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,   106,
     107,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,   171,    -1,     3,     4,   175,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,   106,
     107,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,   171,    -1,     3,     4,   175,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,   106,
     107,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,   171,    -1,     3,     4,   175,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,   106,
     107,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,    -1,     3,     4,   175,     6,
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
      -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,   145,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,
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
      -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,   145,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,
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
      -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,   145,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,
      -1,   168,   169,   170,    -1,    -1,     3,     4,   175,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,   111,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,    -1,     3,     4,   175,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,   106,
     107,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,    -1,     3,     4,   175,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    -1,    -1,    -1,    93,    -1,    -1,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,    -1,     3,     4,   175,     6,
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
      -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,   120,    -1,   122,    -1,    -1,    -1,    -1,
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
      -1,   105,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,
      -1,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,   173,
      -1,   175,     3,     4,    -1,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,   145,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,
     171,    -1,     3,     4,   175,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,   163,    -1,    -1,   166,    -1,   168,    -1,   170,
      -1,    -1,   173,    -1,   175,     3,     4,    -1,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,    -1,     3,     4,   175,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,   120,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,    -1,     3,     4,   175,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,    -1,   173,    -1,   175,     3,     4,
      -1,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,   169,   170,    -1,    -1,     3,     4,
     175,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,    -1,   173,    -1,
     175,     3,     4,    -1,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
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
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
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
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
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
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
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
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
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
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
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
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,     4,    -1,     6,
       7,     8,     9,    10,    -1,   157,    -1,    14,   160,   161,
     162,    18,    -1,    -1,   166,    22,   168,    24,   170,    -1,
      27,    -1,    -1,   175,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    42,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    59,    -1,    -1,    -1,    -1,    64,    -1,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      77,    78,    79,    80,    81,    -1,    -1,    -1,    -1,    86,
      -1,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,    -1,   139,   140,   141,   142,   143,    -1,    -1,    12,
      -1,   148,    -1,    -1,    17,    18,    19,    20,    -1,    -1,
     157,    -1,    25,   160,   161,   162,    -1,    -1,    -1,   166,
      33,   168,    35,   170,   171,   172,    39,    -1,   175,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    98,    -1,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,   136,   137,    -1,   139,   140,   141,   142,
     143,   144,    -1,   146,    -1,    -1,    -1,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,    -1,   161,    -1,
      -1,   164,   165,   166,   167,     3,     4,   170,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    42,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,    -1,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,   172,     3,     4,    -1,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    42,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,     3,
      -1,   109,    -1,   111,    -1,    31,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,   139,    -1,   141,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    -1,    -1,   162,    -1,    -1,     3,    62,    -1,
     168,    -1,   170,    89,   172,    69,    70,    93,    -1,    -1,
      -1,    17,    18,    77,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    96,    39,    98,    -1,   100,    -1,   102,    45,
      -1,   105,    -1,    49,    -1,   109,    52,   111,    54,    -1,
     114,    -1,    -1,   139,   140,    -1,    62,   143,   144,    -1,
     146,   147,    -1,    69,    70,   151,    -1,    -1,    -1,    -1,
      -1,    77,   158,    -1,    -1,    -1,    -1,    -1,   164,   165,
     166,    87,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,
      96,    -1,    98,    -1,   100,    -1,   102,    -1,    -1,   105,
      -1,    17,    18,   109,    20,   111,   170,    -1,   114,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      31,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    98,    -1,   100,     3,   102,    -1,    89,   105,
      -1,    -1,    93,   109,    12,   111,    -1,    -1,   114,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    55,   139,   140,
      -1,   142,   143,   144,    62,   146,   147,    -1,    -1,    -1,
     151,    -1,    70,    -1,   170,    -1,    -1,   158,    -1,    77,
      -1,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,    87,
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
      -1,    -1,    -1,    12,    -1,    96,    -1,    98,    -1,   100,
      19,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
     111,    -1,    31,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,   136,   137,    -1,   139,   140,
     141,   142,   143,   144,    -1,   146,    -1,    -1,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,    -1,
     161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,    -1,
      89,    12,    -1,    -1,    93,    -1,    -1,    -1,    19,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,   136,   137,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    89,   168,
      -1,    12,    93,    -1,    -1,    -1,   175,    -1,    19,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,   136,   137,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    -1,   168,    89,    12,
      -1,    -1,    93,    -1,   175,    -1,    19,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,   136,   137,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    89,   168,    -1,    12,
      93,    -1,    -1,    -1,   175,    -1,    19,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    -1,   136,   137,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,    -1,   168,    89,    -1,    -1,    -1,
      93,    -1,   175,    -1,    -1,    -1,    -1,    12,   101,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,    47,   136,   137,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,    -1,   168,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,    89,    12,    -1,    -1,    93,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,   122,    -1,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,    89,   168,    -1,    12,    93,    -1,    -1,    -1,
     175,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
      -1,   168,    89,    12,    -1,    -1,    93,    -1,   175,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
      89,   168,    -1,    12,    93,    -1,    -1,    -1,   175,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    -1,   168,
      89,    12,    -1,    -1,    93,    -1,   175,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    89,   168,
      -1,    -1,    93,    -1,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,   122,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    72,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    89,    12,
      -1,    -1,    93,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    89,    -1,    12,    -1,
      93,    -1,    -1,    -1,   175,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,    -1,    89,    12,    13,    -1,    93,
     173,    -1,   175,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    31,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,    89,    -1,    -1,    12,    93,    -1,   173,
      -1,   175,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   122,    -1,   164,   165,
     166,    -1,    -1,    89,    12,    -1,    -1,    93,    -1,   175,
      -1,    19,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    31,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   122,    -1,   164,   165,
     166,    -1,   168,    -1,    12,    -1,    -1,    -1,    -1,   175,
      -1,    19,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    31,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,   163,   164,   165,
     166,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,   175,
      -1,    -1,    -1,   101,    -1,    -1,    12,    13,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    31,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   122,    -1,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,
      -1,   139,   140,    89,   142,   143,   144,    93,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    12,    -1,   164,   165,   166,    -1,
      -1,    19,    -1,   171,    -1,    -1,   122,   175,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    12,    -1,   164,   165,
     166,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,   122,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    89,   142,   143,   144,    93,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   175,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   122,    -1,   164,   165,
     166,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,   175,
      -1,    19,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    31,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,   175,
      -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,   139,   140,    93,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    -1,
      -1,    -1,   122,   171,    -1,    -1,    -1,   175,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   122,    -1,   164,   165,   166,    -1,    -1,    -1,
      12,    13,    -1,    -1,    -1,   175,    -1,    19,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    31,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,
      12,   171,    -1,    -1,    -1,   175,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     122,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    89,    -1,   139,   140,    93,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,   163,   164,   165,   166,    12,    -1,   121,   122,    -1,
      -1,    -1,    19,   175,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,   139,   140,    93,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
      -1,    -1,    -1,   122,    -1,    -1,   173,    -1,   175,    -1,
      89,    12,    -1,    -1,    93,    -1,    -1,    -1,    19,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      31,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   122,    -1,   164,   165,   166,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,   175,    -1,    19,    60,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      31,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    89,    -1,
      -1,    12,    93,    -1,    -1,    -1,   175,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,   139,   140,
     101,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   122,    -1,   164,   165,   166,    -1,    -1,    89,    12,
      -1,    -1,    93,    -1,   175,    -1,    19,    -1,   139,   140,
     101,   142,   143,   144,    -1,   146,   147,   148,    31,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   122,    -1,   164,   165,   166,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,   175,    -1,    19,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    31,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    89,    -1,    -1,    12,
      93,    -1,    -1,    -1,   175,    -1,    19,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,   139,   140,   101,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   122,
      -1,   164,   165,   166,    -1,    -1,    89,    12,    -1,    -1,
      93,    -1,   175,    -1,    19,    -1,   139,   140,   101,   142,
     143,   144,    -1,   146,   147,   148,    31,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   122,
      -1,   164,   165,   166,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,   175,    -1,    19,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    31,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   122,    31,   164,
     165,   166,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
     175,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   122,    31,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,   139,   140,    89,   142,   143,   144,
      93,   146,   147,   148,    -1,   150,   151,   152,   153,    -1,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
     175,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,    -1,   155,   156,   157,   158,   159,    -1,   161,    -1,
      -1,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   175,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,    -1,   151,   152,
     153,     3,   155,   156,   157,   158,    -1,    -1,   161,    -1,
      -1,   164,   165,   166,    -1,    17,    18,    -1,    20,    -1,
      -1,    -1,   175,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    90,    -1,
      -1,     3,    -1,     5,    96,    -1,    98,    -1,   100,    -1,
     102,    -1,    -1,   105,    -1,    17,    18,   109,    20,   111,
      -1,   113,   114,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    17,    18,    77,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    87,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    96,    39,    98,    -1,   100,    -1,
     102,    45,    -1,   105,    -1,    49,    -1,   109,    52,   111,
      54,    -1,   114,    -1,    -1,    -1,    -1,     3,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    17,    18,    77,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    96,    39,    98,    -1,   100,    -1,   102,    45,
      -1,   105,    -1,    49,    -1,   109,    52,   111,    54,    -1,
     114,    -1,    -1,    -1,    -1,     3,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    17,
      18,    77,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      96,    39,    98,    -1,   100,    -1,   102,    45,    -1,   105,
      -1,    49,    -1,   109,    52,   111,    54,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    62,    22,    -1,    -1,    -1,    26,
      27,    69,    70,    -1,    -1,    -1,    -1,    -1,    35,    77,
      37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    87,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    96,    -1,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,   111,    71,    -1,   114,    -1,    -1,    76,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,   110,    -1,    -1,   113
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
     251,   252,   253,   257,   258,   263,   264,   265,   266,   269,
     270,   275,   276,   281,   285,   286,   288,   289,   309,   314,
     315,   319,   320,   321,   346,   347,   348,   350,   352,   353,
     354,   360,   361,   362,   363,   364,   365,   367,   370,   371,
     372,   373,   374,   375,   376,   377,   379,   380,   381,   382,
     383,   163,    22,    35,    44,    54,    56,    88,   100,   168,
     238,   249,   346,   353,   361,   362,   367,   370,   372,   373,
     118,   355,   356,     3,    54,   216,   367,   355,   108,   322,
      90,   216,   184,   337,   367,   170,     3,    17,    18,    20,
      25,    33,    39,    45,    49,    52,    54,    62,    69,    70,
      77,    87,    96,    98,   100,   102,   105,   109,   111,   114,
     217,   218,   220,    11,    27,   113,   244,   367,    82,    85,
     200,   170,   102,   218,   218,   218,   170,   218,   324,   325,
      32,   204,   223,   367,   261,   262,   367,   367,    18,    77,
      96,   111,   367,   367,   367,     9,   170,   227,   226,    27,
      33,    47,    49,    51,    75,    78,    90,   100,   108,   113,
     183,   222,   277,   278,   279,   300,   301,   302,   329,   334,
     367,   337,   106,   107,   163,   281,   282,   366,   367,   369,
     367,   223,   367,   367,    99,   170,   184,   367,   367,    71,
     186,   190,   204,   186,   204,   367,   369,   367,   367,   367,
     367,   367,     1,   169,   182,   205,   337,   149,   341,   342,
     369,   367,    80,   184,    22,    35,    38,    71,    88,   168,
     187,   188,   189,   200,   204,   192,   147,   194,   168,    46,
      84,   112,   201,    26,   321,   367,     9,   265,   367,   368,
      24,    32,    40,    41,    42,   117,   172,   240,   241,   349,
     351,   170,   204,    56,   144,   267,   218,   168,    53,    73,
      83,   304,    27,    47,    51,    75,    78,    90,   108,   113,
     305,    27,    47,    51,    75,    78,    90,   108,   113,   303,
     218,   316,   317,     1,   324,   162,   163,   367,    12,    19,
      31,    89,    93,   122,   139,   140,   142,   143,   144,   146,
     147,   148,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   164,   165,   166,   175,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   167,   293,   170,   172,    89,    93,   367,
     218,    40,   172,   240,    56,     1,   170,   184,   163,   204,
     320,   163,   145,   163,   219,   343,   344,   345,   369,   218,
     218,   101,   204,   101,   123,   204,   293,   201,   343,   163,
     170,   184,   170,   218,   326,   327,     1,   144,   333,    47,
     145,   184,   223,   145,   223,    13,   170,   170,   223,   323,
     343,   228,   228,    47,    90,   301,   302,   171,   145,   170,
     218,   144,   163,   367,   367,   121,   283,   163,   168,   223,
     170,   343,   163,   367,   254,   254,   163,   169,   169,   182,
     145,   169,   367,   145,   145,   171,   171,     9,   367,   172,
     240,   241,     3,   170,   195,     1,   169,   205,   212,   213,
     367,   207,   367,    65,    36,    72,   163,   265,   267,   108,
     233,   235,   286,   184,    78,   244,   367,   120,   173,   242,
     337,   338,   339,   367,   378,   242,   367,   218,   247,   248,
     218,   337,   168,     1,   169,   191,   218,   271,   274,    47,
      90,     1,   144,   331,   332,    47,    90,     1,     3,    12,
      17,    19,    20,    25,    45,    52,    54,    55,    62,    70,
      87,    98,   102,   109,   114,   139,   140,   141,   142,   143,
     144,   146,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   161,   164,   165,   166,   167,   170,   218,   290,
     291,   292,   293,   346,   123,   318,   145,   163,   163,   163,
     367,   367,   367,   242,   367,   242,   367,   367,   367,   367,
     367,   367,   367,     3,    20,    33,    62,   102,   108,   219,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,    68,   369,   369,
     369,   343,   343,   242,   367,   242,   367,   101,   170,   337,
     242,   367,   218,     1,    27,    47,    51,    75,    78,    90,
     108,   113,   171,   357,   358,   359,   367,   383,   324,   367,
     123,   171,   145,   123,   123,   184,    34,   184,   367,    34,
     367,    65,   171,   344,   327,   145,   171,   222,   367,   123,
     328,   367,   325,   261,   218,   319,   367,   367,   320,   171,
     218,   305,   279,   327,   144,   335,   336,   334,   284,    32,
     203,   308,     1,   259,   343,   171,    21,   255,   169,   171,
     171,   341,   369,   171,   341,   337,   242,   367,   194,   171,
     196,   197,   345,   169,   169,   145,   163,    13,   147,   145,
     163,    13,    36,    72,   218,   139,   140,   141,   142,   143,
     157,   161,   166,   206,   292,   293,   294,   367,   206,   208,
     267,   168,   236,   320,   117,   218,   223,   223,   170,   367,
     173,   356,    47,   145,   163,   173,   340,   356,   337,   173,
     340,   121,   145,   173,    32,   223,   356,    32,    47,   223,
     356,   145,   171,   163,   170,     1,   268,   169,   274,   123,
     145,   169,     1,   222,   329,   330,   104,   306,   170,   296,
     367,   139,   147,   296,   296,   367,   317,   170,   172,   163,
     163,   163,   163,   171,   173,   344,    47,   173,    32,    32,
      47,   170,   171,    47,    90,   145,   171,    17,    20,    25,
      45,    52,    62,    70,    87,    98,   109,   114,   346,    89,
      89,   163,   369,   345,   367,   367,    34,   184,    34,    34,
     184,   101,   204,   218,   171,   171,   326,   333,    68,   369,
     218,   171,   171,   324,   336,   144,   280,   171,   334,   149,
     295,   328,   367,   182,   169,    74,   115,   169,   260,   171,
     170,   204,   218,   256,    47,   173,    47,   145,   171,   212,
     219,    17,    18,    20,    25,    45,    49,    52,    70,    77,
      87,    96,    98,   100,   109,   111,   114,   168,   211,   294,
     367,   367,   206,   208,   145,   163,    13,   163,   168,   268,
     316,   324,   367,    47,   337,   184,   173,   242,   367,   173,
     340,   184,   367,   163,   173,   173,   145,   173,   367,   173,
     367,   367,   223,    44,   367,   242,   367,   223,   248,   204,
     344,   169,    82,    85,   169,   183,   191,   225,   367,   272,
     273,    60,   116,   310,   297,   298,   299,   300,   171,   291,
     293,   171,   173,   171,   242,   367,    44,   242,   367,   344,
     358,   333,   346,   346,   184,   184,   367,   184,    34,   163,
     163,   328,   170,   170,   163,   295,   328,   329,   306,   336,
     367,   184,   238,   337,   256,   144,   204,   242,   367,   242,
     367,   197,   206,    13,    36,    72,   163,   163,   294,   367,
     367,   268,   169,   163,   163,   163,   367,   171,   184,   173,
     356,   173,   356,   173,   184,   121,   367,    32,   223,   356,
      32,   223,   356,   171,   191,   225,   225,   274,   191,     3,
      54,    92,   102,   311,   312,   313,   367,   287,   145,   171,
     296,   296,   173,   173,   367,    32,    32,   171,   328,    34,
     184,   343,   343,   295,   328,    32,   223,   171,   367,   173,
     173,   169,   367,   206,   208,    13,   169,   223,    44,   184,
     367,   173,    44,   184,   367,   173,   367,   101,    44,   367,
     223,    44,   367,   223,   163,   225,   274,   313,   116,   145,
     123,   150,   152,   153,   155,   156,    60,   163,   307,   308,
     299,    44,    44,   101,    44,    44,   184,   171,   171,   184,
     204,   163,   163,   367,   367,   184,   367,   184,   367,   367,
     367,   367,   312,   313,   313,   313,   313,   313,   313,   311,
     367,   367,   367,   367,   162,   162,   101,   101,   101,   101,
     101,   101,   101,   101,   367,   367,   367,   367
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   179,   180,   181,   181,   182,   182,   183,   183,   184,
     184,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   187,   187,   187,   188,   189,
     189,   190,   191,   191,   192,   193,   194,   194,   195,   195,
     195,   196,   196,   197,   198,   199,   199,   199,   200,   200,
     200,   201,   201,   202,   203,   203,   203,   204,   205,   205,
     206,   206,   206,   206,   206,   206,   207,   207,   207,   207,
     208,   208,   209,   209,   209,   209,   209,   210,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   212,   212,   212,   212,   212,   213,
     213,   214,   215,   215,   215,   215,   216,   216,   216,   217,
     217,   217,   217,   217,   218,   218,   218,   219,   219,   219,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     222,   222,   222,   223,   223,   224,   224,   225,   225,   226,
     225,   227,   225,   228,   228,   228,   228,   228,   228,   228,
     229,   229,   229,   229,   230,   231,   231,   232,   233,   234,
     234,   234,   235,   234,   236,   234,   237,   238,   239,   239,
     240,   240,   240,   240,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   242,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   244,   244,   245,
     246,   246,   247,   247,   248,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   250,   250,
     250,   250,   250,   250,   251,   251,   251,   252,   252,   252,
     253,   253,   253,   253,   254,   254,   255,   255,   255,   256,
     256,   257,   258,   258,   259,   259,   260,   260,   260,   261,
     261,   261,   262,   262,   263,   264,   264,   265,   266,   266,
     266,   267,   267,   268,   268,   268,   268,   268,   269,   269,
     269,   270,   271,   271,   272,   271,   271,   273,   271,   274,
     274,   275,   276,   276,   276,   277,   277,   278,   278,   279,
     279,   279,   280,   280,   281,   283,   282,   284,   282,   285,
     287,   286,   288,   288,   288,   288,   288,   289,   290,   290,
     291,   291,   291,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   293,   293,
     293,   293,   293,   293,   293,   293,   293,   293,   293,   293,
     293,   293,   294,   294,   295,   295,   296,   296,   297,   297,
     298,   298,   299,   300,   300,   300,   300,   300,   300,   301,
     301,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   304,   304,   304,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   306,   306,   307,   307,
     308,   308,   309,   310,   310,   310,   310,   310,   311,   311,
     312,   312,   312,   312,   312,   312,   312,   313,   313,   313,
     314,   315,   315,   316,   316,   317,   318,   318,   319,   319,
     319,   319,   319,   320,   322,   321,   323,   321,   321,   321,
     324,   324,   325,   325,   326,   326,   327,   327,   327,   328,
     328,   328,   329,   330,   330,   330,   331,   331,   332,   332,
     333,   333,   333,   333,   334,   334,   335,   336,   336,   337,
     337,   338,   338,   338,   339,   339,   340,   340,   341,   342,
     342,   343,   343,   344,   344,   345,   345,   346,   346,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     348,   349,   349,   349,   349,   349,   349,   349,   350,   351,
     351,   351,   351,   351,   351,   351,   352,   353,   354,   354,
     354,   354,   354,   354,   355,   355,   356,   356,   356,   356,
     357,   357,   358,   358,   358,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   360,   360,   360,   360,   360,
     361,   361,   361,   361,   361,   362,   363,   363,   363,   363,
     363,   363,   364,   365,   366,   366,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   368,   368,   369,   369,   369,   370,   370,   370,
     370,   371,   371,   371,   371,   372,   372,   372,   373,   373,
     373,   373,   373,   373,   374,   374,   374,   374,   375,   375,
     376,   376,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   378,   378,
     379,   379,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   380,   380,   380,   380,   380,   380,   380,
     381,   381,   381,   381,   382,   382,   382,   382,   383,   383,
     383,   383,   383,   383,   383
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
       3,     2,     1,     2,     0,     4,     2,     0,     5,     1,
       3,     0,     1,     1,     1,     0,     1,     1,     3,     1,
       3,     1,     0,     2,     6,     0,     3,     0,     4,     1,
       0,     7,     4,     4,     6,     6,     4,     2,     1,     3,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     0,     3,     1,     3,
       0,     1,     1,     4,     5,     4,     5,     6,     6,     0,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     0,     1,     1,     2,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     0,     1,     1,     2,     1,
       1,     1,     1,     2,     1,     1,     0,     1,     1,     1,
       1,     2,     1,     0,     2,     2,     4,     4,     1,     3,
       3,     3,     3,     3,     3,     3,     2,     1,     1,     1,
       3,     1,     2,     1,     3,     2,     0,     2,     1,     2,
       1,     1,     1,     1,     0,     5,     0,     6,     3,     3,
       1,     3,     3,     5,     1,     3,     1,     2,     3,     0,
       2,     2,     1,     1,     1,     1,     2,     1,     0,     1,
       0,     2,     2,     1,     1,     1,     2,     0,     1,     1,
       3,     2,     3,     2,     2,     3,     1,     2,     1,     3,
       3,     0,     1,     1,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     4,     5,     2,     2,
       1,     2,     1,     2,     1,     2,     1,     2,     1,     1,
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
#line 691 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 5871 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 695 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 5877 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 696 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 5883 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 703 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 5891 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 711 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 5899 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 715 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 5908 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 725 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 5916 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 730 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 5924 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 742 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)); }
#line 5930 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 744 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5936 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 745 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5942 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 746 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5948 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 747 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5954 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 748 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5960 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 749 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5966 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 750 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5972 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: TDEFER stmt  */
#line 752 "chpl.ypp"
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
#line 5993 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: try_stmt  */
#line 768 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 5999 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: return_stmt  */
#line 769 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6005 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: TINIT TTHIS TSEMI  */
#line 771 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr));
    auto node = Init::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6017 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 779 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6030 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 788 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6043 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 797 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6051 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 801 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6062 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 808 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6071 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 815 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6077 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 816 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6083 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 817 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6089 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 818 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6095 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 819 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6101 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 820 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6107 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 821 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6113 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 823 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6119 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 825 "chpl.ypp"
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
#line 6142 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 844 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6154 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 852 "chpl.ypp"
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
#line 6174 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL do_stmt  */
#line 868 "chpl.ypp"
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
#line 6193 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TON expr do_stmt  */
#line 883 "chpl.ypp"
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
#line 6213 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 899 "chpl.ypp"
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
#line 6233 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL do_stmt  */
#line 915 "chpl.ypp"
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
#line 6252 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSYNC stmt  */
#line 930 "chpl.ypp"
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
#line 6273 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 955 "chpl.ypp"
  {
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 6281 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 976 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6289 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 983 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6298 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 990 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6306 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 994 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6317 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 1004 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6323 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 1006 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6331 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 1010 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6339 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 1017 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6348 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 1022 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6358 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 1035 "chpl.ypp"
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
#line 6380 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 1056 "chpl.ypp"
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
#line 6402 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 1074 "chpl.ypp"
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
#line 6424 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1092 "chpl.ypp"
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
#line 6444 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1110 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6450 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1111 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6457 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1113 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6464 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1118 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6470 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1119 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6477 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1125 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr), LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6487 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1143 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6496 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1148 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6505 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1153 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6518 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1165 "chpl.ypp"
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
#line 6541 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1187 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6547 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1188 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6553 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1193 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6561 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1197 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6569 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1201 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6578 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1206 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6586 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1210 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6594 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1214 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6603 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1222 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6612 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1227 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6622 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1233 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6631 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1238 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6642 "bison-chpl-lib.cpp"
    break;

  case 90: /* opt_only_ls: %empty  */
#line 1246 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6648 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: renames_ls  */
#line 1247 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6654 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1252 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6663 "bison-chpl-lib.cpp"
    break;

  case 93: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1257 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6675 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1265 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6688 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1274 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6700 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1282 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6713 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1293 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6722 "bison-chpl-lib.cpp"
    break;

  case 114: /* import_expr: expr  */
#line 1328 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6730 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr TDOT all_op_name  */
#line 1332 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6739 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1337 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6748 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TAS ident_use  */
#line 1342 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6758 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1348 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6768 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_ls: import_expr  */
#line 1356 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6774 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_ls TCOMMA import_expr  */
#line 1357 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6780 "bison-chpl-lib.cpp"
    break;

  case 121: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1362 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = makeCommentsAndStmt(comments, node.release());
  }
#line 6790 "bison-chpl-lib.cpp"
    break;

  case 122: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1371 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6798 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1375 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6806 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1379 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6814 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1383 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6822 "bison-chpl-lib.cpp"
    break;

  case 126: /* opt_label_ident: %empty  */
#line 1391 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6828 "bison-chpl-lib.cpp"
    break;

  case 127: /* opt_label_ident: TIDENT  */
#line 1392 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6834 "bison-chpl-lib.cpp"
    break;

  case 128: /* opt_label_ident: TINIT  */
#line 1393 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6840 "bison-chpl-lib.cpp"
    break;

  case 137: /* ident_use: TIDENT  */
#line 1419 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6846 "bison-chpl-lib.cpp"
    break;

  case 138: /* ident_use: TINIT  */
#line 1420 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6852 "bison-chpl-lib.cpp"
    break;

  case 139: /* ident_use: TTHIS  */
#line 1421 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6858 "bison-chpl-lib.cpp"
    break;

  case 158: /* scalar_type: TBOOL  */
#line 1447 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6864 "bison-chpl-lib.cpp"
    break;

  case 159: /* scalar_type: TENUM  */
#line 1448 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6870 "bison-chpl-lib.cpp"
    break;

  case 160: /* scalar_type: TINT  */
#line 1449 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6876 "bison-chpl-lib.cpp"
    break;

  case 161: /* scalar_type: TUINT  */
#line 1450 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6882 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TREAL  */
#line 1451 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6888 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TIMAG  */
#line 1452 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6894 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TCOMPLEX  */
#line 1453 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6900 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TBYTES  */
#line 1454 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6906 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TSTRING  */
#line 1455 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6912 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TLOCALE  */
#line 1456 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6918 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TNOTHING  */
#line 1457 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6924 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TVOID  */
#line 1458 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6930 "bison-chpl-lib.cpp"
    break;

  case 173: /* do_stmt: TDO stmt  */
#line 1471 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 6936 "bison-chpl-lib.cpp"
    break;

  case 174: /* do_stmt: block_stmt  */
#line 1472 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 6942 "bison-chpl-lib.cpp"
    break;

  case 175: /* return_stmt: TRETURN TSEMI  */
#line 1477 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6953 "bison-chpl-lib.cpp"
    break;

  case 176: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1484 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6964 "bison-chpl-lib.cpp"
    break;

  case 177: /* class_level_stmt: TSEMI  */
#line 1494 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 6972 "bison-chpl-lib.cpp"
    break;

  case 178: /* class_level_stmt: inner_class_level_stmt  */
#line 1498 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 6982 "bison-chpl-lib.cpp"
    break;

  case 179: /* $@1: %empty  */
#line 1503 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 6989 "bison-chpl-lib.cpp"
    break;

  case 180: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1505 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->adjustLocation((yyval.commentsAndStmt), (yylsp[-2]), (yylsp[0]));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 6999 "bison-chpl-lib.cpp"
    break;

  case 181: /* $@2: %empty  */
#line 1510 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7006 "bison-chpl-lib.cpp"
    break;

  case 182: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1512 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->adjustLocation((yyval.commentsAndStmt), (yylsp[-2]), (yylsp[0]));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7016 "bison-chpl-lib.cpp"
    break;

  case 190: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1531 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7025 "bison-chpl-lib.cpp"
    break;

  case 191: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1536 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7034 "bison-chpl-lib.cpp"
    break;

  case 192: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1541 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7043 "bison-chpl-lib.cpp"
    break;

  case 193: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1546 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7051 "bison-chpl-lib.cpp"
    break;

  case 194: /* forwarding_decl_start: TFORWARDING  */
#line 1553 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7060 "bison-chpl-lib.cpp"
    break;

  case 195: /* extern_or_export: TEXTERN  */
#line 1560 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7066 "bison-chpl-lib.cpp"
    break;

  case 196: /* extern_or_export: TEXPORT  */
#line 1561 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7072 "bison-chpl-lib.cpp"
    break;

  case 197: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1566 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7082 "bison-chpl-lib.cpp"
    break;

  case 198: /* extern_export_kind_inner: TTYPE  */
#line 1579 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7091 "bison-chpl-lib.cpp"
    break;

  case 199: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1588 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7102 "bison-chpl-lib.cpp"
    break;

  case 200: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1596 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7115 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1605 "chpl.ypp"
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
#line 7137 "bison-chpl-lib.cpp"
    break;

  case 202: /* $@3: %empty  */
#line 1623 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7145 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 1627 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7154 "bison-chpl-lib.cpp"
    break;

  case 204: /* $@4: %empty  */
#line 1632 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7163 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr extern_export_kind_inner $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1637 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-1])), (yyvsp[-3].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7172 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1645 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yyloc), (yyvsp[0].sizedStr));
  }
#line 7180 "bison-chpl-lib.cpp"
    break;

  case 207: /* no_loop_attributes: %empty  */
#line 1651 "chpl.ypp"
  {
    // Even though there are no attributes, there could be loop pragmas.
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 7189 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1657 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7195 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1658 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7201 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1675 "chpl.ypp"
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
#line 7227 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1697 "chpl.ypp"
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
#line 7249 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1715 "chpl.ypp"
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
#line 7271 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1733 "chpl.ypp"
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
#line 7299 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1757 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7308 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: for_loop_kw expr TIN expr task_intent_clause do_stmt  */
#line 1762 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7317 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1767 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7326 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator task_intent_clause do_stmt  */
#line 1772 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7335 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1777 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7344 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: for_loop_kw expr task_intent_clause do_stmt  */
#line 1782 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7353 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1787 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7362 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw zippered_iterator task_intent_clause do_stmt  */
#line 1792 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7371 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1797 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7380 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TLSBR expr_ls TIN expr task_intent_clause TRSBR stmt  */
#line 1802 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7389 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1807 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7398 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator task_intent_clause TRSBR stmt  */
#line 1812 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7407 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1817 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7416 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TLSBR expr_ls task_intent_clause TRSBR stmt  */
#line 1822 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7425 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1827 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7435 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR zippered_iterator task_intent_clause TRSBR stmt  */
#line 1833 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7445 "bison-chpl-lib.cpp"
    break;

  case 234: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1842 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7455 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF expr TTHEN stmt  */
#line 1851 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7463 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF expr block_stmt  */
#line 1855 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7471 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1859 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7479 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1863 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7487 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1867 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7495 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF ifvar block_stmt  */
#line 1871 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7503 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1875 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7511 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1879 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7519 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1883 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7528 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1888 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7537 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1893 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7546 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1898 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7555 "bison-chpl-lib.cpp"
    break;

  case 247: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1906 "chpl.ypp"
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
#line 7574 "bison-chpl-lib.cpp"
    break;

  case 248: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1921 "chpl.ypp"
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
#line 7593 "bison-chpl-lib.cpp"
    break;

  case 249: /* interface_start: TINTERFACE ident_def  */
#line 1939 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Interface);
  }
#line 7601 "bison-chpl-lib.cpp"
    break;

  case 250: /* interface_stmt: interface_start TLP ifc_formal_ls TRP block_stmt  */
#line 1946 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-4].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), parts, (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC((yyvsp[0].commentsAndStmt).curlyLoc));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7612 "bison-chpl-lib.cpp"
    break;

  case 251: /* interface_stmt: interface_start block_stmt  */
#line 1953 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-1].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), parts, nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC((yyvsp[0].commentsAndStmt).curlyLoc));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7623 "bison-chpl-lib.cpp"
    break;

  case 252: /* ifc_formal_ls: ifc_formal  */
#line 1962 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7629 "bison-chpl-lib.cpp"
    break;

  case 253: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1963 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7635 "bison-chpl-lib.cpp"
    break;

  case 254: /* ifc_formal: ident_def  */
#line 1968 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7641 "bison-chpl-lib.cpp"
    break;

  case 274: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 2011 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7649 "bison-chpl-lib.cpp"
    break;

  case 275: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 2015 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7657 "bison-chpl-lib.cpp"
    break;

  case 276: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 2019 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7665 "bison-chpl-lib.cpp"
    break;

  case 277: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2026 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7673 "bison-chpl-lib.cpp"
    break;

  case 278: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 2030 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7681 "bison-chpl-lib.cpp"
    break;

  case 279: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2034 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7689 "bison-chpl-lib.cpp"
    break;

  case 280: /* try_stmt: TTRY tryable_stmt  */
#line 2041 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7697 "bison-chpl-lib.cpp"
    break;

  case 281: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2045 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7705 "bison-chpl-lib.cpp"
    break;

  case 282: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2049 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7713 "bison-chpl-lib.cpp"
    break;

  case 283: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2053 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7721 "bison-chpl-lib.cpp"
    break;

  case 284: /* catch_expr_ls: %empty  */
#line 2059 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7727 "bison-chpl-lib.cpp"
    break;

  case 285: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2060 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7733 "bison-chpl-lib.cpp"
    break;

  case 286: /* catch_expr: TCATCH block_stmt  */
#line 2065 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false, TextLocation::create());
  }
#line 7741 "bison-chpl-lib.cpp"
    break;

  case 287: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2069 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false, TextLocation::create());
  }
#line 7749 "bison-chpl-lib.cpp"
    break;

  case 288: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2073 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true, context->makeSpannedLocation((yylsp[-3]), (yylsp[-1])));
  }
#line 7757 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr_inner: ident_def  */
#line 2080 "chpl.ypp"
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
#line 7776 "bison-chpl-lib.cpp"
    break;

  case 290: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2095 "chpl.ypp"
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
#line 7795 "bison-chpl-lib.cpp"
    break;

  case 291: /* throw_stmt: TTHROW expr TSEMI  */
#line 2113 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = makeCommentsAndStmt(comments, node.release());
  }
#line 7805 "bison-chpl-lib.cpp"
    break;

  case 292: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2121 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7814 "bison-chpl-lib.cpp"
    break;

  case 293: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2126 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = makeCommentsAndStmt(comments, node.release());
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7825 "bison-chpl-lib.cpp"
    break;

  case 294: /* when_stmt_ls: %empty  */
#line 2135 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7831 "bison-chpl-lib.cpp"
    break;

  case 295: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2136 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7837 "bison-chpl-lib.cpp"
    break;

  case 296: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2141 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7845 "bison-chpl-lib.cpp"
    break;

  case 297: /* when_stmt: TOTHERWISE stmt  */
#line 2145 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7854 "bison-chpl-lib.cpp"
    break;

  case 298: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2150 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 7864 "bison-chpl-lib.cpp"
    break;

  case 299: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2159 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 7873 "bison-chpl-lib.cpp"
    break;

  case 300: /* manager_expr: expr TAS ident_def  */
#line 2164 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 7881 "bison-chpl-lib.cpp"
    break;

  case 301: /* manager_expr: expr  */
#line 2168 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 7889 "bison-chpl-lib.cpp"
    break;

  case 302: /* manager_expr_ls: manager_expr  */
#line 2174 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7895 "bison-chpl-lib.cpp"
    break;

  case 303: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2175 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7901 "bison-chpl-lib.cpp"
    break;

  case 304: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2180 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 7910 "bison-chpl-lib.cpp"
    break;

  case 305: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2190 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 7921 "bison-chpl-lib.cpp"
    break;

  case 306: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2197 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 7934 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_start: class_tag ident_def  */
#line 2210 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 7942 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_tag: TCLASS  */
#line 2216 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 7948 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_tag: TRECORD  */
#line 2217 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 7954 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_tag: TUNION  */
#line 2218 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 7960 "bison-chpl-lib.cpp"
    break;

  case 311: /* opt_inherit: %empty  */
#line 2222 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7966 "bison-chpl-lib.cpp"
    break;

  case 312: /* opt_inherit: TCOLON expr_ls  */
#line 2223 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7972 "bison-chpl-lib.cpp"
    break;

  case 313: /* class_level_stmt_ls: %empty  */
#line 2227 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 7981 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2232 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 7989 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2236 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 7997 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2240 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8005 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2244 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8013 "bison-chpl-lib.cpp"
    break;

  case 318: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
#line 2252 "chpl.ypp"
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
#line 8037 "bison-chpl-lib.cpp"
    break;

  case 319: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
#line 2272 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
    auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    CommentsAndStmt cs = makeCommentsAndStmt(parts.comments, err.release());
    (yyval.commentsAndStmt) = cs;
    context->exitScope(asttags::Enum, parts.name);
    context->resetDeclState();
    context->clearComments();
  }
#line 8051 "bison-chpl-lib.cpp"
    break;

  case 320: /* enum_decl_stmt: enum_header TLCBR TRCBR  */
#line 2282 "chpl.ypp"
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
#line 8066 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_header: TENUM ident_def  */
#line 2296 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8074 "bison-chpl-lib.cpp"
    break;

  case 322: /* enum_ls: enum_item  */
#line 2303 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8083 "bison-chpl-lib.cpp"
    break;

  case 323: /* enum_ls: enum_ls TCOMMA  */
#line 2308 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8093 "bison-chpl-lib.cpp"
    break;

  case 324: /* $@5: %empty  */
#line 2314 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8102 "bison-chpl-lib.cpp"
    break;

  case 325: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2319 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8111 "bison-chpl-lib.cpp"
    break;

  case 326: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2324 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8120 "bison-chpl-lib.cpp"
    break;

  case 327: /* $@6: %empty  */
#line 2329 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8129 "bison-chpl-lib.cpp"
    break;

  case 328: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2334 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8138 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_item: ident_def  */
#line 2344 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[0])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8150 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_item: ident_def TASSIGN expr  */
#line 2352 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[-2])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8164 "bison-chpl-lib.cpp"
    break;

  case 332: /* linkage_spec: linkage_spec_empty  */
#line 2367 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8170 "bison-chpl-lib.cpp"
    break;

  case 333: /* linkage_spec: TINLINE  */
#line 2368 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8177 "bison-chpl-lib.cpp"
    break;

  case 334: /* linkage_spec: TOVERRIDE  */
#line 2370 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8184 "bison-chpl-lib.cpp"
    break;

  case 335: /* opt_fn_type_formal_ls: %empty  */
#line 2375 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8190 "bison-chpl-lib.cpp"
    break;

  case 336: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2376 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8196 "bison-chpl-lib.cpp"
    break;

  case 337: /* fn_type_formal_ls: fn_type_formal  */
#line 2380 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8202 "bison-chpl-lib.cpp"
    break;

  case 338: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2381 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8208 "bison-chpl-lib.cpp"
    break;

  case 339: /* fn_type_formal: named_formal  */
#line 2386 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8214 "bison-chpl-lib.cpp"
    break;

  case 340: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2389 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8220 "bison-chpl-lib.cpp"
    break;

  case 341: /* fn_type_formal: formal_type  */
#line 2391 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8226 "bison-chpl-lib.cpp"
    break;

  case 342: /* opt_fn_type_ret_type: %empty  */
#line 2395 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8232 "bison-chpl-lib.cpp"
    break;

  case 343: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2396 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8238 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2409 "chpl.ypp"
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
#line 8258 "bison-chpl-lib.cpp"
    break;

  case 345: /* $@7: %empty  */
#line 2428 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8268 "bison-chpl-lib.cpp"
    break;

  case 346: /* fn_expr: fn_type $@7 function_body_stmt  */
#line 2434 "chpl.ypp"
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
#line 8285 "bison-chpl-lib.cpp"
    break;

  case 347: /* $@8: %empty  */
#line 2447 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8295 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_expr: fn_type TALIAS $@8 expr  */
#line 2453 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8308 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2465 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8316 "bison-chpl-lib.cpp"
    break;

  case 350: /* $@9: %empty  */
#line 2474 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8324 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@9 opt_function_body_stmt  */
#line 2478 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-6].functionParts);
    fp.returnIntent = (yyvsp[-5].returnTag);
    fp.returnIntentLoc = (yylsp[-5]);
    fp.returnType = (yyvsp[-4].expr);
    fp.throws = ((yyvsp[-3].throwsTag) == ThrowsTag_THROWS);
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
#line 8351 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2504 "chpl.ypp"
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
#line 8368 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2517 "chpl.ypp"
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
#line 8385 "bison-chpl-lib.cpp"
    break;

  case 354: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2530 "chpl.ypp"
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
#line 8403 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2544 "chpl.ypp"
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
#line 8421 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2558 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      // store location of the header to handle weird bison location issues
      // with empty rules
      fp.headerLoc =
        context->makeSpannedLocation((yylsp[-3]), (yyvsp[0].exprList) != context->parenlessMarker ? (yylsp[0]) : (yylsp[-1]));
      (yyval.functionParts) = fp;
    }
#line 8435 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
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
#line 8451 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2587 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 8460 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_ident: ident_def TBANG  */
#line 2597 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8470 "bison-chpl-lib.cpp"
    break;

  case 404: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2656 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8476 "bison-chpl-lib.cpp"
    break;

  case 405: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2657 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8482 "bison-chpl-lib.cpp"
    break;

  case 406: /* opt_formal_ls: %empty  */
#line 2661 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; CHPL_NULL_YYLLOC((yyloc)); }
#line 8488 "bison-chpl-lib.cpp"
    break;

  case 407: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2662 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8494 "bison-chpl-lib.cpp"
    break;

  case 408: /* formal_ls_inner: formal  */
#line 2666 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8500 "bison-chpl-lib.cpp"
    break;

  case 409: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2667 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8506 "bison-chpl-lib.cpp"
    break;

  case 410: /* formal_ls: %empty  */
#line 2671 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8512 "bison-chpl-lib.cpp"
    break;

  case 411: /* formal_ls: formal_ls_inner  */
#line 2672 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8518 "bison-chpl-lib.cpp"
    break;

  case 413: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2681 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8526 "bison-chpl-lib.cpp"
    break;

  case 414: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2686 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8534 "bison-chpl-lib.cpp"
    break;

  case 415: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2691 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8542 "bison-chpl-lib.cpp"
    break;

  case 416: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2696 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8550 "bison-chpl-lib.cpp"
    break;

  case 417: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2701 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yylsp[-5]), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8558 "bison-chpl-lib.cpp"
    break;

  case 418: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2706 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8566 "bison-chpl-lib.cpp"
    break;

  case 419: /* opt_formal_intent_tag: %empty  */
#line 2712 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);
  }
#line 8575 "bison-chpl-lib.cpp"
    break;

  case 420: /* opt_formal_intent_tag: required_intent_tag  */
#line 2717 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8584 "bison-chpl-lib.cpp"
    break;

  case 421: /* required_intent_tag: TIN  */
#line 2724 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::IN);            }
#line 8590 "bison-chpl-lib.cpp"
    break;

  case 422: /* required_intent_tag: TINOUT  */
#line 2725 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::INOUT);         }
#line 8596 "bison-chpl-lib.cpp"
    break;

  case 423: /* required_intent_tag: TOUT  */
#line 2726 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::OUT);           }
#line 8602 "bison-chpl-lib.cpp"
    break;

  case 424: /* required_intent_tag: TCONST TIN  */
#line 2727 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_IN);      }
#line 8608 "bison-chpl-lib.cpp"
    break;

  case 425: /* required_intent_tag: TCONST TREF  */
#line 2728 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);     }
#line 8614 "bison-chpl-lib.cpp"
    break;

  case 426: /* required_intent_tag: TCONST  */
#line 2729 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);         }
#line 8620 "bison-chpl-lib.cpp"
    break;

  case 427: /* required_intent_tag: TPARAM  */
#line 2730 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);         }
#line 8626 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TREF  */
#line 2731 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);           }
#line 8632 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TTYPE  */
#line 2732 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);          }
#line 8638 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TVAR  */
#line 2733 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR); }
#line 8644 "bison-chpl-lib.cpp"
    break;

  case 431: /* opt_this_intent_tag: %empty  */
#line 2737 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);     }
#line 8650 "bison-chpl-lib.cpp"
    break;

  case 432: /* opt_this_intent_tag: TPARAM  */
#line 2738 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);              }
#line 8656 "bison-chpl-lib.cpp"
    break;

  case 433: /* opt_this_intent_tag: TREF  */
#line 2739 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);                }
#line 8662 "bison-chpl-lib.cpp"
    break;

  case 434: /* opt_this_intent_tag: TCONST TREF  */
#line 2740 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);          }
#line 8668 "bison-chpl-lib.cpp"
    break;

  case 435: /* opt_this_intent_tag: TCONST  */
#line 2741 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);              }
#line 8674 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_this_intent_tag: TTYPE  */
#line 2742 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);               }
#line 8680 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: TIN  */
#line 2743 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::IN);       }
#line 8686 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TCONST TIN  */
#line 2744 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::CONST_IN); }
#line 8692 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TVAR  */
#line 2745 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR);      }
#line 8698 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TOUT  */
#line 2746 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::OUT);      }
#line 8704 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TINOUT  */
#line 2747 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::INOUT);    }
#line 8710 "bison-chpl-lib.cpp"
    break;

  case 442: /* proc_iter_or_op: TPROC  */
#line 2751 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8716 "bison-chpl-lib.cpp"
    break;

  case 443: /* proc_iter_or_op: TITER  */
#line 2752 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8722 "bison-chpl-lib.cpp"
    break;

  case 444: /* proc_iter_or_op: TOPERATOR  */
#line 2753 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8728 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_ret_tag: %empty  */
#line 2757 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::DEFAULT_RETURN_INTENT); }
#line 8734 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_ret_tag: TOUT  */
#line 2758 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::OUT);                   }
#line 8740 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_ret_tag: TCONST  */
#line 2759 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST);                 }
#line 8746 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_ret_tag: TCONST TREF  */
#line 2760 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST_REF);             }
#line 8752 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: TREF  */
#line 2761 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::REF);                   }
#line 8758 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TPARAM  */
#line 2762 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::PARAM);                 }
#line 8764 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TTYPE  */
#line 2763 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::TYPE);                  }
#line 8770 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TIN  */
#line 2764 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::IN);            }
#line 8776 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_ret_tag: TCONST TIN  */
#line 2765 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::CONST_IN);      }
#line 8782 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: TVAR  */
#line 2766 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::VAR);           }
#line 8788 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: TINOUT  */
#line 2767 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::INOUT);         }
#line 8794 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_throws_error: %empty  */
#line 2771 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8800 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_throws_error: TTHROWS  */
#line 2772 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8806 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_function_body_stmt: TSEMI  */
#line 2775 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8812 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_function_body_stmt: function_body_stmt  */
#line 2776 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8818 "bison-chpl-lib.cpp"
    break;

  case 460: /* function_body_stmt: block_stmt_body  */
#line 2780 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8824 "bison-chpl-lib.cpp"
    break;

  case 461: /* function_body_stmt: TDO toplevel_stmt  */
#line 2781 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yylsp[0]), (yyvsp[0].commentsAndStmt)); }
#line 8830 "bison-chpl-lib.cpp"
    break;

  case 462: /* query_expr: TQUERIEDIDENT  */
#line 2785 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8836 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_lifetime_where: %empty  */
#line 2790 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8842 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_lifetime_where: TWHERE expr  */
#line 2792 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8848 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2794 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8854 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2796 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 8860 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2798 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 8866 "bison-chpl-lib.cpp"
    break;

  case 468: /* lifetime_components_expr: lifetime_expr  */
#line 2803 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8872 "bison-chpl-lib.cpp"
    break;

  case 469: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2805 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8878 "bison-chpl-lib.cpp"
    break;

  case 470: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2810 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8884 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2812 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8890 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2814 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8896 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2816 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8902 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2818 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8908 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2820 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8914 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2822 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 8920 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_ident: TIDENT  */
#line 2826 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8926 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_ident: TINIT  */
#line 2827 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8932 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_ident: TTHIS  */
#line 2828 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8938 "bison-chpl-lib.cpp"
    break;

  case 480: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2833 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 8947 "bison-chpl-lib.cpp"
    break;

  case 481: /* type_alias_decl_stmt_start: TTYPE  */
#line 2842 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 8957 "bison-chpl-lib.cpp"
    break;

  case 482: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2848 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true, (yylsp[-1]));
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 8968 "bison-chpl-lib.cpp"
    break;

  case 483: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2858 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 8976 "bison-chpl-lib.cpp"
    break;

  case 484: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2862 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8984 "bison-chpl-lib.cpp"
    break;

  case 485: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2869 "chpl.ypp"
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
#line 9006 "bison-chpl-lib.cpp"
    break;

  case 486: /* opt_init_type: %empty  */
#line 2889 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9012 "bison-chpl-lib.cpp"
    break;

  case 487: /* opt_init_type: TASSIGN expr  */
#line 2891 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9018 "bison-chpl-lib.cpp"
    break;

  case 488: /* var_decl_type: TPARAM  */
#line 2895 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9024 "bison-chpl-lib.cpp"
    break;

  case 489: /* var_decl_type: TCONST TREF  */
#line 2896 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 490: /* var_decl_type: TREF  */
#line 2897 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9036 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_type: TCONST  */
#line 2898 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9042 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_type: TVAR  */
#line 2899 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9048 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_start: var_decl_type  */
#line 2904 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9057 "bison-chpl-lib.cpp"
    break;

  case 494: /* $@10: %empty  */
#line 2912 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true, (yylsp[0]));
  }
#line 9066 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_stmt: TCONFIG $@10 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2916 "chpl.ypp"
                                              {
    CHPL_ASSERT(context->isVarDeclConfig);
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9076 "bison-chpl-lib.cpp"
    break;

  case 496: /* $@11: %empty  */
#line 2922 "chpl.ypp"
  {
    // Use a mid-rule action to preserve the 'on' expr
    context->noteVarDestinationExpr((yyvsp[0].expr));
  }
#line 9085 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_stmt: TON expr $@11 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2927 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9094 "bison-chpl-lib.cpp"
    break;

  case 498: /* var_decl_stmt: var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2932 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9103 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt: var_decl_start error TSEMI  */
#line 2937 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = makeCommentsAndStmt(nullptr, ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release());
    delete (yyvsp[-2].attributeGroup);
    context->resetDeclStateOnError();
  }
#line 9113 "bison-chpl-lib.cpp"
    break;

  case 500: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2946 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9121 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2950 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9129 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2957 "chpl.ypp"
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
#line 9151 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2975 "chpl.ypp"
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
#line 9171 "bison-chpl-lib.cpp"
    break;

  case 504: /* tuple_var_decl_component: ident_def  */
#line 2994 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9179 "bison-chpl-lib.cpp"
    break;

  case 505: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2998 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9187 "bison-chpl-lib.cpp"
    break;

  case 506: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 3005 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9193 "bison-chpl-lib.cpp"
    break;

  case 507: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 3007 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9199 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 3009 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9205 "bison-chpl-lib.cpp"
    break;

  case 509: /* opt_init_expr: %empty  */
#line 3015 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9211 "bison-chpl-lib.cpp"
    break;

  case 510: /* opt_init_expr: TASSIGN TNOINIT  */
#line 3016 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9217 "bison-chpl-lib.cpp"
    break;

  case 511: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 3017 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9223 "bison-chpl-lib.cpp"
    break;

  case 512: /* formal_or_ret_type_expr: expr  */
#line 3021 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9229 "bison-chpl-lib.cpp"
    break;

  case 513: /* ret_type: formal_or_ret_type_expr  */
#line 3025 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9235 "bison-chpl-lib.cpp"
    break;

  case 514: /* ret_type: reserved_type_ident_use  */
#line 3026 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9241 "bison-chpl-lib.cpp"
    break;

  case 515: /* ret_type: error  */
#line 3027 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9247 "bison-chpl-lib.cpp"
    break;

  case 516: /* colon_ret_type: TCOLON ret_type  */
#line 3031 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9253 "bison-chpl-lib.cpp"
    break;

  case 517: /* colon_ret_type: error  */
#line 3032 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9259 "bison-chpl-lib.cpp"
    break;

  case 518: /* opt_ret_type: %empty  */
#line 3036 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9265 "bison-chpl-lib.cpp"
    break;

  case 520: /* opt_type: %empty  */
#line 3041 "chpl.ypp"
                                 { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9271 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_type: TCOLON expr  */
#line 3042 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9277 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_type: TCOLON reserved_type_ident_use  */
#line 3043 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9283 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_type: error  */
#line 3044 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9289 "bison-chpl-lib.cpp"
    break;

  case 524: /* formal_type: formal_or_ret_type_expr  */
#line 3048 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9295 "bison-chpl-lib.cpp"
    break;

  case 525: /* formal_type: reserved_type_ident_use  */
#line 3049 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9301 "bison-chpl-lib.cpp"
    break;

  case 526: /* colon_formal_type: TCOLON formal_type  */
#line 3053 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9307 "bison-chpl-lib.cpp"
    break;

  case 527: /* opt_colon_formal_type: %empty  */
#line 3057 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9313 "bison-chpl-lib.cpp"
    break;

  case 528: /* opt_colon_formal_type: colon_formal_type  */
#line 3058 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9319 "bison-chpl-lib.cpp"
    break;

  case 529: /* expr_ls: expr  */
#line 3064 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9325 "bison-chpl-lib.cpp"
    break;

  case 530: /* expr_ls: expr_ls TCOMMA expr  */
#line 3065 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9331 "bison-chpl-lib.cpp"
    break;

  case 531: /* expr_ls_end_semi: expr_ls semicolon_list  */
#line 3069 "chpl.ypp"
                         {
    (yyval.ndArrayList) = context->makeNDArrayList();
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElement((yyvsp[-1].exprList), (yylsp[-1])));
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9341 "bison-chpl-lib.cpp"
    break;

  case 532: /* expr_ls_end_semi: expr_ls TCOMMA semicolon_list  */
#line 3074 "chpl.ypp"
                                {
    (yyval.ndArrayList) = context->makeNDArrayList();
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElement((yyvsp[-2].exprList), (yylsp[-2])));
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9351 "bison-chpl-lib.cpp"
    break;

  case 533: /* expr_ls_end_semi: expr_ls_semi semicolon_list  */
#line 3080 "chpl.ypp"
  {
    (yyval.ndArrayList) = (yyvsp[-1].ndArrayList);
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9360 "bison-chpl-lib.cpp"
    break;

  case 534: /* expr_ls_semi: expr_ls_end_semi expr_ls  */
#line 3087 "chpl.ypp"
                           {
    (yyval.ndArrayList) = context->appendNDArrayList((yyvsp[-1].ndArrayList), makeNDArrayElement((yyvsp[0].exprList), (yylsp[0])));
  }
#line 9368 "bison-chpl-lib.cpp"
    break;

  case 535: /* expr_ls_semi: expr_ls_end_semi expr_ls TCOMMA  */
#line 3090 "chpl.ypp"
                                  {
    (yyval.ndArrayList) = context->appendNDArrayList((yyvsp[-2].ndArrayList), makeNDArrayElement((yyvsp[-1].exprList), (yylsp[-1])));
  }
#line 9376 "bison-chpl-lib.cpp"
    break;

  case 536: /* semicolon_list: TSEMI  */
#line 3096 "chpl.ypp"
                       { (yyval.counter) = 1; }
#line 9382 "bison-chpl-lib.cpp"
    break;

  case 537: /* semicolon_list: semicolon_list TSEMI  */
#line 3097 "chpl.ypp"
                       { (yyval.counter) = (yyvsp[-1].counter) + 1; }
#line 9388 "bison-chpl-lib.cpp"
    break;

  case 538: /* tuple_component: opt_try_expr  */
#line 3103 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9394 "bison-chpl-lib.cpp"
    break;

  case 539: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3108 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9402 "bison-chpl-lib.cpp"
    break;

  case 540: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3112 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9410 "bison-chpl-lib.cpp"
    break;

  case 541: /* opt_actual_ls: %empty  */
#line 3118 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9416 "bison-chpl-lib.cpp"
    break;

  case 542: /* opt_actual_ls: actual_ls  */
#line 3119 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9422 "bison-chpl-lib.cpp"
    break;

  case 543: /* actual_ls: actual_expr  */
#line 3124 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9431 "bison-chpl-lib.cpp"
    break;

  case 544: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3129 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9441 "bison-chpl-lib.cpp"
    break;

  case 545: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3137 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9447 "bison-chpl-lib.cpp"
    break;

  case 546: /* actual_expr: opt_try_expr  */
#line 3138 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9453 "bison-chpl-lib.cpp"
    break;

  case 547: /* ident_expr: ident_use  */
#line 3142 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9459 "bison-chpl-lib.cpp"
    break;

  case 548: /* ident_expr: scalar_type  */
#line 3143 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9465 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3156 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9471 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3158 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9477 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3160 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9483 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3162 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9493 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TATOMIC expr  */
#line 3168 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9499 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TSYNC expr  */
#line 3170 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9505 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TOWNED  */
#line 3173 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9511 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TOWNED expr  */
#line 3175 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9517 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TUNMANAGED  */
#line 3177 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9523 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3179 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9529 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TSHARED  */
#line 3181 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9535 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TSHARED expr  */
#line 3183 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9541 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TBORROWED  */
#line 3185 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9547 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TBORROWED expr  */
#line 3187 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9553 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TCLASS  */
#line 3189 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9559 "bison-chpl-lib.cpp"
    break;

  case 569: /* sub_type_level_expr: TRECORD  */
#line 3191 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9565 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr: no_loop_attributes for_expr_base  */
#line 3195 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9571 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3200 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9579 "bison-chpl-lib.cpp"
    break;

  case 572: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3204 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9587 "bison-chpl-lib.cpp"
    break;

  case 573: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3208 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9595 "bison-chpl-lib.cpp"
    break;

  case 574: /* for_expr_base: for_loop_kw zippered_iterator TDO expr  */
#line 3212 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "expression-level loops with 'zip' must have an index.");
  }
#line 9603 "bison-chpl-lib.cpp"
    break;

  case 575: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3216 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9616 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3225 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9629 "bison-chpl-lib.cpp"
    break;

  case 577: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3234 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9642 "bison-chpl-lib.cpp"
    break;

  case 578: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3245 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9648 "bison-chpl-lib.cpp"
    break;

  case 579: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3250 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9656 "bison-chpl-lib.cpp"
    break;

  case 580: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3254 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9664 "bison-chpl-lib.cpp"
    break;

  case 581: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3258 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9672 "bison-chpl-lib.cpp"
    break;

  case 582: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3262 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9680 "bison-chpl-lib.cpp"
    break;

  case 583: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3266 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9688 "bison-chpl-lib.cpp"
    break;

  case 584: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3270 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9696 "bison-chpl-lib.cpp"
    break;

  case 585: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3274 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9704 "bison-chpl-lib.cpp"
    break;

  case 586: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3281 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9718 "bison-chpl-lib.cpp"
    break;

  case 587: /* nil_expr: TNIL  */
#line 3298 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9724 "bison-chpl-lib.cpp"
    break;

  case 594: /* opt_task_intent_ls: %empty  */
#line 3315 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9730 "bison-chpl-lib.cpp"
    break;

  case 595: /* opt_task_intent_ls: task_intent_clause  */
#line 3316 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9736 "bison-chpl-lib.cpp"
    break;

  case 596: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3321 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9744 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_intent_clause: TWITH TLP TRP  */
#line 3325 "chpl.ypp"
  {
    auto err = context->syntax((yyloc), "'with' clause cannot be empty.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-2]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9755 "bison-chpl-lib.cpp"
    break;

  case 598: /* task_intent_clause: TWITH TLP error TRP  */
#line 3332 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[-2]), "invalid intent expression in 'with' clause.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9766 "bison-chpl-lib.cpp"
    break;

  case 599: /* task_intent_clause: TWITH error  */
#line 3339 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[0]), "missing parentheses around 'with' clause intents.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-1]), dummyLoc, dummyLoc, dummyLoc, exprs);
  }
#line 9777 "bison-chpl-lib.cpp"
    break;

  case 600: /* task_intent_ls: intent_expr  */
#line 3348 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9783 "bison-chpl-lib.cpp"
    break;

  case 601: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3349 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9789 "bison-chpl-lib.cpp"
    break;

  case 602: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3354 "chpl.ypp"
  {
    (yyval.expr) = context->buildTaskIntent((yyloc), (yylsp[-2]), nullptr, (yyvsp[-2].expr), (yyvsp[-3].taskIntent), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9797 "bison-chpl-lib.cpp"
    break;

  case 603: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3358 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9805 "bison-chpl-lib.cpp"
    break;

  case 604: /* intent_expr: expr TREDUCE ident_expr  */
#line 3362 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9813 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_var_prefix: TCONST  */
#line 3368 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST);          }
#line 9819 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_var_prefix: TIN  */
#line 3369 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::IN);             }
#line 9825 "bison-chpl-lib.cpp"
    break;

  case 607: /* task_var_prefix: TCONST TIN  */
#line 3370 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_IN);       }
#line 9831 "bison-chpl-lib.cpp"
    break;

  case 608: /* task_var_prefix: TREF  */
#line 3371 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::REF);            }
#line 9837 "bison-chpl-lib.cpp"
    break;

  case 609: /* task_var_prefix: TCONST TREF  */
#line 3372 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_REF);      }
#line 9843 "bison-chpl-lib.cpp"
    break;

  case 610: /* task_var_prefix: TVAR  */
#line 3373 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::VAR);            }
#line 9849 "bison-chpl-lib.cpp"
    break;

  case 611: /* task_var_prefix: TOUT  */
#line 3374 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::OUT);   }
#line 9855 "bison-chpl-lib.cpp"
    break;

  case 612: /* task_var_prefix: TINOUT  */
#line 3375 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::INOUT); }
#line 9861 "bison-chpl-lib.cpp"
    break;

  case 613: /* task_var_prefix: TPARAM  */
#line 3376 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::PARAM); }
#line 9867 "bison-chpl-lib.cpp"
    break;

  case 614: /* task_var_prefix: TTYPE  */
#line 3377 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::TYPE);  }
#line 9873 "bison-chpl-lib.cpp"
    break;

  case 615: /* new_maybe_decorated: TNEW  */
#line 3382 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9879 "bison-chpl-lib.cpp"
    break;

  case 616: /* new_maybe_decorated: TNEW TOWNED  */
#line 3384 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9885 "bison-chpl-lib.cpp"
    break;

  case 617: /* new_maybe_decorated: TNEW TSHARED  */
#line 3386 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9891 "bison-chpl-lib.cpp"
    break;

  case 618: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3388 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9897 "bison-chpl-lib.cpp"
    break;

  case 619: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3390 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9903 "bison-chpl-lib.cpp"
    break;

  case 620: /* new_expr: new_maybe_decorated expr  */
#line 3396 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 9911 "bison-chpl-lib.cpp"
    break;

  case 621: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
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
#line 9928 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
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
#line 9945 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
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
#line 9963 "bison-chpl-lib.cpp"
    break;

  case 624: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
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
#line 9982 "bison-chpl-lib.cpp"
    break;

  case 625: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3461 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9990 "bison-chpl-lib.cpp"
    break;

  case 626: /* range_literal_expr: expr TDOTDOT expr  */
#line 3468 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 9999 "bison-chpl-lib.cpp"
    break;

  case 627: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3473 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10008 "bison-chpl-lib.cpp"
    break;

  case 628: /* range_literal_expr: expr TDOTDOT  */
#line 3478 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10017 "bison-chpl-lib.cpp"
    break;

  case 629: /* range_literal_expr: TDOTDOT expr  */
#line 3483 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10027 "bison-chpl-lib.cpp"
    break;

  case 630: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3489 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10037 "bison-chpl-lib.cpp"
    break;

  case 631: /* range_literal_expr: TDOTDOT  */
#line 3495 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10047 "bison-chpl-lib.cpp"
    break;

  case 632: /* cast_expr: expr TCOLON expr  */
#line 3525 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10055 "bison-chpl-lib.cpp"
    break;

  case 633: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3532 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10063 "bison-chpl-lib.cpp"
    break;

  case 634: /* super_expr: fn_expr  */
#line 3538 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10069 "bison-chpl-lib.cpp"
    break;

  case 638: /* expr: sub_type_level_expr TQUESTION  */
#line 3547 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10075 "bison-chpl-lib.cpp"
    break;

  case 639: /* expr: TQUESTION  */
#line 3549 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10081 "bison-chpl-lib.cpp"
    break;

  case 643: /* expr: fn_type  */
#line 3554 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10087 "bison-chpl-lib.cpp"
    break;

  case 652: /* opt_expr: %empty  */
#line 3567 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10093 "bison-chpl-lib.cpp"
    break;

  case 653: /* opt_expr: expr  */
#line 3568 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10099 "bison-chpl-lib.cpp"
    break;

  case 654: /* opt_try_expr: TTRY expr  */
#line 3572 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10105 "bison-chpl-lib.cpp"
    break;

  case 655: /* opt_try_expr: TTRYBANG expr  */
#line 3573 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10111 "bison-chpl-lib.cpp"
    break;

  case 656: /* opt_try_expr: super_expr  */
#line 3574 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10117 "bison-chpl-lib.cpp"
    break;

  case 662: /* call_base_expr: expr TBANG  */
#line 3591 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10125 "bison-chpl-lib.cpp"
    break;

  case 663: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3594 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10131 "bison-chpl-lib.cpp"
    break;

  case 665: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3600 "chpl.ypp"
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
#line 10147 "bison-chpl-lib.cpp"
    break;

  case 666: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3612 "chpl.ypp"
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
#line 10163 "bison-chpl-lib.cpp"
    break;

  case 667: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3624 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10171 "bison-chpl-lib.cpp"
    break;

  case 668: /* dot_expr: expr TDOT ident_use  */
#line 3631 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10177 "bison-chpl-lib.cpp"
    break;

  case 669: /* dot_expr: expr TDOT TTYPE  */
#line 3633 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10183 "bison-chpl-lib.cpp"
    break;

  case 670: /* dot_expr: expr TDOT TDOMAIN  */
#line 3635 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10189 "bison-chpl-lib.cpp"
    break;

  case 671: /* dot_expr: expr TDOT TLOCALE  */
#line 3637 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10195 "bison-chpl-lib.cpp"
    break;

  case 672: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3639 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10201 "bison-chpl-lib.cpp"
    break;

  case 673: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3641 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10207 "bison-chpl-lib.cpp"
    break;

  case 674: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3650 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10217 "bison-chpl-lib.cpp"
    break;

  case 675: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3656 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10225 "bison-chpl-lib.cpp"
    break;

  case 676: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3660 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10233 "bison-chpl-lib.cpp"
    break;

  case 677: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3664 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10241 "bison-chpl-lib.cpp"
    break;

  case 678: /* bool_literal: TFALSE  */
#line 3670 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10247 "bison-chpl-lib.cpp"
    break;

  case 679: /* bool_literal: TTRUE  */
#line 3671 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10253 "bison-chpl-lib.cpp"
    break;

  case 680: /* str_bytes_literal: STRINGLITERAL  */
#line 3675 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10259 "bison-chpl-lib.cpp"
    break;

  case 681: /* str_bytes_literal: BYTESLITERAL  */
#line 3676 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10265 "bison-chpl-lib.cpp"
    break;

  case 684: /* literal_expr: INTLITERAL  */
#line 3682 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10271 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: REALLITERAL  */
#line 3683 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10277 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: IMAGLITERAL  */
#line 3684 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10283 "bison-chpl-lib.cpp"
    break;

  case 687: /* literal_expr: TNONE  */
#line 3685 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10289 "bison-chpl-lib.cpp"
    break;

  case 688: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3687 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10298 "bison-chpl-lib.cpp"
    break;

  case 689: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3692 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10307 "bison-chpl-lib.cpp"
    break;

  case 690: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3697 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10316 "bison-chpl-lib.cpp"
    break;

  case 691: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3702 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10325 "bison-chpl-lib.cpp"
    break;

  case 692: /* literal_expr: no_loop_attributes TLSBR expr_ls_semi TRSBR  */
#line 3707 "chpl.ypp"
  {
    (yyval.expr) = context->buildNDArray((yyloc), (yyvsp[-1].ndArrayList));
    context->popLoopAttributeGroup();
  }
#line 10334 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: no_loop_attributes TLSBR expr_ls_semi semicolon_list TRSBR  */
#line 3712 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    (yyval.expr) = context->buildNDArray((yyloc), (yyvsp[-2].ndArrayList));
    context->popLoopAttributeGroup();
  }
#line 10346 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: no_loop_attributes TLSBR expr_ls semicolon_list TRSBR  */
#line 3720 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    AstList lst;
    lst.push_back(ArrayRow::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))));
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), std::move(lst)).release();
    context->popLoopAttributeGroup();
  }
#line 10360 "bison-chpl-lib.cpp"
    break;

  case 695: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA semicolon_list TRSBR  */
#line 3730 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    AstList lst;
    lst.push_back(ArrayRow::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-3].exprList))));
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), std::move(lst)).release();
    context->popLoopAttributeGroup();
  }
#line 10374 "bison-chpl-lib.cpp"
    break;

  case 696: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3740 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10383 "bison-chpl-lib.cpp"
    break;

  case 697: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3745 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10392 "bison-chpl-lib.cpp"
    break;

  case 698: /* assoc_expr_ls: expr TALIAS expr  */
#line 3754 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10401 "bison-chpl-lib.cpp"
    break;

  case 699: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3759 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10411 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TPLUS expr  */
#line 3767 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10417 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TMINUS expr  */
#line 3768 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10423 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TSTAR expr  */
#line 3769 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10429 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TDIVIDE expr  */
#line 3770 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10435 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3771 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10441 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3772 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10447 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TMOD expr  */
#line 3773 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10453 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TEQUAL expr  */
#line 3774 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10459 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3775 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10465 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3776 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10471 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3777 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10477 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TLESS expr  */
#line 3778 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10483 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TGREATER expr  */
#line 3779 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10489 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TBAND expr  */
#line 3780 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10495 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TBOR expr  */
#line 3781 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10501 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TBXOR expr  */
#line 3782 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10507 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TAND expr  */
#line 3783 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10513 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TOR expr  */
#line 3784 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10519 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TEXP expr  */
#line 3785 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10525 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TBY expr  */
#line 3786 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10531 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TALIGN expr  */
#line 3787 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10537 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr THASH expr  */
#line 3788 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10543 "bison-chpl-lib.cpp"
    break;

  case 722: /* binary_op_expr: expr TDMAPPED expr  */
#line 3789 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10549 "bison-chpl-lib.cpp"
    break;

  case 723: /* unary_op_expr: TPLUS expr  */
#line 3793 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10555 "bison-chpl-lib.cpp"
    break;

  case 724: /* unary_op_expr: TMINUS expr  */
#line 3794 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10561 "bison-chpl-lib.cpp"
    break;

  case 725: /* unary_op_expr: TMINUSMINUS expr  */
#line 3795 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10567 "bison-chpl-lib.cpp"
    break;

  case 726: /* unary_op_expr: TPLUSPLUS expr  */
#line 3796 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10573 "bison-chpl-lib.cpp"
    break;

  case 727: /* unary_op_expr: TBANG expr  */
#line 3797 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10579 "bison-chpl-lib.cpp"
    break;

  case 728: /* unary_op_expr: expr TBANG  */
#line 3798 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10587 "bison-chpl-lib.cpp"
    break;

  case 729: /* unary_op_expr: TBNOT expr  */
#line 3801 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10593 "bison-chpl-lib.cpp"
    break;

  case 730: /* reduce_expr: expr TREDUCE expr  */
#line 3806 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10601 "bison-chpl-lib.cpp"
    break;

  case 731: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3810 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10609 "bison-chpl-lib.cpp"
    break;

  case 732: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3814 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10617 "bison-chpl-lib.cpp"
    break;

  case 733: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3818 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10625 "bison-chpl-lib.cpp"
    break;

  case 734: /* scan_expr: expr TSCAN expr  */
#line 3825 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10633 "bison-chpl-lib.cpp"
    break;

  case 735: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3829 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10641 "bison-chpl-lib.cpp"
    break;

  case 736: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3833 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10649 "bison-chpl-lib.cpp"
    break;

  case 737: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3837 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10657 "bison-chpl-lib.cpp"
    break;


#line 10661 "bison-chpl-lib.cpp"

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
