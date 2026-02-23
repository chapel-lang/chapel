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
#define YYLAST   17559

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  179
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  205
/* YYNRULES -- Number of rules.  */
#define YYNRULES  743
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1257

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
    2284,  2291,  2296,  2303,  2302,  2312,  2318,  2317,  2332,  2340,
    2352,  2356,  2357,  2359,  2364,  2365,  2369,  2370,  2374,  2377,
    2379,  2384,  2385,  2396,  2417,  2416,  2436,  2435,  2453,  2463,
    2460,  2492,  2505,  2518,  2532,  2546,  2559,  2574,  2575,  2583,
    2584,  2585,  2594,  2595,  2596,  2597,  2598,  2599,  2600,  2601,
    2602,  2603,  2604,  2605,  2606,  2607,  2608,  2609,  2610,  2611,
    2612,  2613,  2614,  2615,  2616,  2617,  2618,  2622,  2623,  2624,
    2625,  2626,  2627,  2628,  2629,  2630,  2631,  2632,  2633,  2634,
    2635,  2640,  2641,  2645,  2646,  2650,  2651,  2655,  2656,  2660,
    2661,  2665,  2669,  2673,  2678,  2683,  2688,  2693,  2701,  2705,
    2713,  2714,  2715,  2716,  2717,  2718,  2719,  2720,  2721,  2722,
    2726,  2727,  2728,  2729,  2730,  2731,  2732,  2733,  2734,  2735,
    2736,  2740,  2741,  2742,  2746,  2747,  2748,  2749,  2750,  2751,
    2752,  2753,  2754,  2755,  2756,  2760,  2761,  2764,  2765,  2769,
    2770,  2774,  2779,  2780,  2782,  2784,  2786,  2791,  2793,  2798,
    2800,  2802,  2804,  2806,  2808,  2810,  2815,  2816,  2817,  2821,
    2830,  2836,  2846,  2850,  2857,  2878,  2879,  2884,  2885,  2886,
    2887,  2888,  2892,  2901,  2900,  2911,  2910,  2920,  2925,  2934,
    2938,  2945,  2963,  2982,  2986,  2993,  2995,  2997,  3004,  3005,
    3006,  3010,  3014,  3015,  3016,  3020,  3021,  3025,  3026,  3030,
    3031,  3032,  3033,  3037,  3038,  3042,  3046,  3047,  3053,  3054,
    3058,  3063,  3068,  3076,  3079,  3085,  3086,  3092,  3096,  3100,
    3107,  3108,  3112,  3117,  3126,  3127,  3131,  3132,  3139,  3140,
    3141,  3142,  3143,  3144,  3146,  3148,  3150,  3156,  3158,  3161,
    3163,  3165,  3167,  3169,  3171,  3173,  3175,  3177,  3179,  3184,
    3188,  3192,  3196,  3200,  3204,  3213,  3222,  3234,  3238,  3242,
    3246,  3250,  3254,  3258,  3262,  3269,  3287,  3295,  3296,  3297,
    3298,  3299,  3300,  3304,  3305,  3309,  3313,  3320,  3327,  3337,
    3338,  3342,  3346,  3350,  3357,  3358,  3359,  3360,  3361,  3362,
    3363,  3364,  3365,  3366,  3370,  3372,  3374,  3376,  3378,  3384,
    3391,  3404,  3417,  3431,  3449,  3456,  3461,  3466,  3471,  3477,
    3483,  3513,  3520,  3527,  3528,  3532,  3534,  3535,  3537,  3539,
    3540,  3541,  3542,  3545,  3546,  3547,  3548,  3549,  3550,  3551,
    3552,  3556,  3557,  3561,  3562,  3563,  3567,  3568,  3569,  3570,
    3579,  3580,  3583,  3584,  3588,  3600,  3612,  3619,  3621,  3623,
    3625,  3627,  3629,  3638,  3644,  3648,  3652,  3659,  3660,  3664,
    3665,  3669,  3670,  3671,  3672,  3673,  3674,  3675,  3680,  3685,
    3690,  3695,  3700,  3708,  3718,  3728,  3733,  3742,  3747,  3756,
    3757,  3758,  3759,  3760,  3761,  3762,  3763,  3764,  3765,  3766,
    3767,  3768,  3769,  3770,  3771,  3772,  3773,  3774,  3775,  3776,
    3777,  3778,  3782,  3783,  3784,  3785,  3786,  3787,  3790,  3794,
    3798,  3802,  3806,  3813,  3817,  3821,  3825,  3833,  3834,  3835,
    3836,  3837,  3838,  3839
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

#define YYPACT_NINF (-912)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-744)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -912,   103,  3841,  -912,   -38,    84,  -912,  -912,  -912,  -912,
    -912,  -912,  -912, 11721,    37,    76,   105, 13525,   138, 17329,
      37,   173,    66,   164,    76,  5225, 11721,    96, 17387,  -912,
     260,   249,  -912,  9666,   295,   466,   158,  -912,   326,   233,
   17387, 17387, 17387,  2300, 10865,   349, 11721, 11721,   243,  -912,
     365,   369, 11721,  -912, 13525,  -912, 11721,   378,   188,   262,
     272, 12886,   374, 17445,  -912, 11721,  7793, 11721, 10865, 13525,
     366,   417,   342,  5225,   463, 11721,   506,  6773,  6773,  -912,
     512,  -912, 13525,  -912,   522,  9836,  -912, 11721,  -912, 11721,
    -912,  -912, 13355, 11721,  -912, 10006,  -912,  -912,  -912,  4187,
    7963, 11721,  -912,  4879,  -912,  -912,  -912,  -912, 16979,   479,
    -912,   399,   413,  -912,    15,  -912,  -912,  -912,  -912,  -912,
    -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,  7113,  -912,
    7283,  -912,  -912,   104,  -912,  -912,   -78,  -912,   530,  -912,
    -912,  -912,  -912,  -912,  -912,  -912,  -912,   445, 17387,  -912,
     424,  -912,   242,  -912,  -912,  -912,   314,   518,  -912,  -912,
   17387,  -912,  1413,  -912,   434,   477,  -912,  -912,  -912,   452,
     462, 11721,   475,   478,  -912,  -912,  -912, 16798,   776,   348,
     480,   483,  -912,  -912,   401,  -912,  -912,  -912,  -912,  -912,
     321,  -912,  -912,  -912, 11721,  -912, 17387,  -912, 11721, 11721,
      93,   584,  -912,  -912,  -912,  -912, 16798,   440,  -912,  -912,
      42,  5225,  -912,  -912,  -912,   491,   120,   474,  -912,   160,
    -912,   492,  -912,   101, 16798,  8133,  -912,  -912,  -912,  -912,
    -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,
    -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,
    -912,  -912,  -912,  -912, 17387, 17387,   -21, 14003,  -912,  -912,
     573,  8133,   495,   489,  -912,  5225,  2740,    92,    27,  -912,
    5225,  -912,  -912, 14889,  -912,    23, 15323,  1130,  -912,   497,
     498,  -912, 14889,   120,  1130,  -912,  8133,  2972,  2972,    19,
      -8,  -912,    11,  -912,  -912,  -912,  -912,  7453,  -912,  -912,
     510,  -912,   501,   516,  -912,  -912,  3069,   533,  -912,  -912,
   16798,   318, 11721, 11721,  -912,    20,  -912,  -912, 16798,   511,
   15364,  -912, 14889,   120,   503,  8133,  -912, 16798, 15412, 11721,
    -912,  -912,  -912,  -912,  -912,   120,   515,   208,   208,   604,
    1130,  1130,   144,  -912,  -912,  4360,   -43, 11721,   534,   -64,
     513,   604,   672,  -912,  -912, 17387,  -912, 11721,  -912,  4187,
    -912,   124,  -912,   573,  -912,  -912,   680,   517,  4533, 11721,
    -912, 11721,   621,  -912,  -912,  1392,    50,   445, 16798,   325,
    -912,  5225,   611,  -912,  -912,  9666, 10176, 11035,  -912,  -912,
    -912, 17387,  -912, 17387, 11721,   523,  -912, 17123,  -912,  -912,
    -912,  -912,   184,  -912,  -912,  -912,  -912,  -912,  -912,  -912,
     118,   195,  -912,  -912,  -912,  -912,  -912,  -912,  -912, 13190,
     567,   322,  -912,   531,   327,   444,  -912,   548, 11721, 11721,
   11721, 11035, 11035, 11721,   449, 11721, 11721, 11721, 11721, 11721,
     269, 13355, 11721, 11721, 11721, 11721, 11721, 11721, 11721, 11721,
   11721, 11721, 11721, 11721, 11721, 11721, 11721, 11721,   628,  -912,
    -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,
    9836,  -912,  -912,  9836,  9836,  8133,  8303, 11035, 11035, 15514,
     535,  -912, 11205, 11035, 17387,  -912,  3500,  -912,  -912,  -912,
    2300,  -912, 11721,  -912,   576,   537,   555,  -912,  -912,   578,
     581,  5225,   675,  5225,  -912,   676, 11721,   646,   541,  -912,
    9836,  -912,  2740,  -912,  -912,   146,  -912, 11891,   590, 11721,
    2300,  -912,  -912, 11721,  -912, 17181, 11721, 11721,  -912,   160,
     544,  -912,  -912,  -912,  -912, 17387,  -912,   314, 13056,  2740,
     577, 11891,  -912, 16798, 16798,  -912,    30,  -912,    44,  -912,
    8133,   546,  -912, 14889,   699,   699,  -912,  -912,  -912,  -912,
   11378,  -912, 15562,  8476,  8646,  -912,  -912,  -912, 16798, 11035,
   11035,  -912,   399,  8816,  -912,   324,  -912,  4706,  -912,   331,
   15607,   332, 14653, 17387,  6600,  6430,  -912,   445,   557,  -912,
    -912,   160,  -912,   610, 17387,    32, 14889,   559, 13666,   -41,
       6, 11721,   163, 15712,   -59,    24, 14485,  -912,   171,  -912,
     -74,   585,  1565,   562, 17271,   609,   279,  -912,  -912,  -912,
    -912,  6260,  -912,   631,  -912,  -912,   566,   592,  -912,   593,
    -912,   594,   598,   602,   605,   607,  -912,   612,   617,   619,
     620,   624,   626,   627,  -912,  -912,  -912,  -912,  -912,  -912,
    -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,
    -912,  -912,  -912,  -912,  -912,  -912, 11721,   618,   629,   566,
    -912,   566,  -912, 11721,  -912, 17387,  -912,  -912,  -912,  3628,
    3628,   433,  -912,   433,  -912,   433, 16758,   843,  1931,   645,
     120,   208,  -912,   450,  -912,  -912,  -912,  -912,  -912,   604,
    3300,   433,   505,   505,  3628,   505,   505,  1567,   208,  3300,
   16956,  1567,  1130,  1130,   208,   604,   574,   595,   614,   615,
     601,   606,  -912,   433,  -912,   433, 11721,  9836,     7,   743,
   14969,   613,   623,   213,  -912,  -912,  -912,  -912,  -912,  -912,
    -912,  -912,   174,  -912,  2207, 16838,   428,   337, 16798,  9836,
    -912,  9836, 11721, 11721,   746,  5225,   747, 15757,  5225, 14929,
   17387,  -912,   189,   222,  2740,    92,  -912, 16798,  8986,  -912,
   16798,  -912,  -912,  -912, 17387, 15797, 15915,  2300,  -912,   577,
     638,  -912,   224, 11891,  -912,   -36,  -912, 11721,  5052,  -912,
    -912,   630,    14,   633,  -912,  1930,  -912,  -912,  -912,  -912,
    -912,  -912,  -912,  -912,   176,    25, 14563,  -912,  -912,   225,
    -912,  -912,  -912,  -912, 11721,  -912,    56, 13764, 11721,  -912,
   11721,  6600,  6430,  -912,  9156,   471,  9326,   481,   490,  9496,
    7623,   502,   341,  -912,  -912,  -912, 15963,   652,   637,   622,
    -912, 17387,  2300, 11721,   754,  -912,  -912, 11721, 16798,  5225,
     632, 11035, 10349,  -912,  4014,   297,   634,   657,  -912,   371,
   11721, 11548,  -912,  5225,  -912,    32,  5398, 11035,  -912,    32,
   17387,   474,  -912,  9836,   639,  1730,  -912,  -912, 11721,    41,
    -912,  -912,  -912,  -912,  -912,  -912,   167,   356,  -912, 16003,
    -912, 13921,  -912,  -912, 16798,  -912,   635,   640,  -912,  -912,
    -912,  -912,  -912,  -912,   231, 11035, 13666, 11721, 12061, 11035,
    9836,  -912,  -912,  -912,  6943,  -912,  -912,  -912,  -912,  -912,
    -912,  -912,  -912,  -912,  -912,  -912,  -912,    40, 11035, 11035,
    -912,  -912,  -912, 16798, 16798,  5225,  -912,  5225, 11721,  -912,
    5225,   780,   653,   655,  -912,  -912,   590,  -912,  -912,  -912,
     650,   656,   351,   -36, 11721,   631,   577,  -912, 10522,  -912,
    -912, 16798,  -912,  -912,  5225, 11721,  -912,  -912,  -912, 17387,
    -912,   678,   474, 11035,  5225, 11035,  9836,  -912,  -912,  -912,
    -912,  -912,   450,  -912,  -912,  -912,  -912,  -912,  -912,  -912,
    -912,  -912,  -912,  -912,  -912,  -912,  6600,  -912,  -912, 16121,
   15119,   353,   660,  6600,  -912, 11721,  -912,  -912,  3323,   362,
     379, 16161, 11721,   252,  -912,  5225,   125, 15164,  -912,   386,
    -912, 16798,  -912,  -912,  5225, 10692,  -912, 16798,  -912, 16209,
   16798,  -912,  9666, 16798,    31, 14731,  -912,  -912,  -912,   255,
    -912,  -912,  -912,  -912,  1824, 15142,  -912, 16798, 17387,   479,
     246, 11721,  -912,   682,   659,  -912,  -912,  -912,   566,   566,
    -912,  -912,  -912,   662, 16316, 11721,   796, 16364,   261,  -912,
     590,   266,   274,  -912,  -912, 16798,   803,  5225,  -912,  -912,
    -912,  8133,  8133,  -912,  -912,  -912,  -912,  -912,   -36, 16798,
    -912,   216,    28,   668, 11721,  -912,   126, 15209,    25, 14811,
    -912,   302, 11721,  6600,  6430,  -912,  -912,  -912, 16404, 16798,
    3647,  -912,  -912,  -912,  -912, 14889,  -912,  -912,  5571,   667,
    5744,   671,  -912,  -912, 11721, 14081,  5917,  -912,    32,  6090,
    -912,    32,   686, 15142,  -912,  -912,  -912, 17271,  -912,  -912,
     230,  -912,   -25,  -912,   306, 16482,    62,   768,  -912,  -912,
    -912, 12231, 12401, 16522, 12571, 12741,  -912,  -912,  5225,  -912,
     674,   679,  -912,  -912,  5225,  -912,   474, 16798,  5225,  5225,
    -912, 16798,   387,   688, 11721,  -912,  -912,  9666,  -912, 16798,
    5225,  9666,  -912, 16798,  5225, 16798,  5225,  9666, 16798,  -912,
    9666, 16798,  -912,  -912,  -912,  -912,  -912, 11721,   246,   230,
     230,   230,   230,   230,   230,   246,  -912,  -912,  -912,  -912,
   11721, 11721, 11721, 11721, 11721,  -912,   693,   695,   610,  -912,
    -912,  -912, 16798, 14161,  -912, 14239,  -912, 15757, 14319, 14397,
   16798,  -912,  -912,  -912,  -912,  -912,  -912,  -912,   715, 16562,
   16640, 16680, 16720,  -912,  -912,  5225,  5225,  5225,  5225, 11721,
   11721, 11721, 11721, 15757, 15757, 15757, 15757
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   137,   461,    55,   683,   684,
     685,   679,   680,   207,   593,   126,   158,   565,   165,   567,
     593,   164,   493,   490,   126,     0,   207,   272,   159,   196,
     195,   677,   194,   207,   163,    68,   273,   332,   160,   138,
       0,     0,     0,     0,   207,   167,   207,   207,   614,   586,
     686,   168,   207,   333,   559,   487,   207,     0,     0,   181,
     179,   418,   162,   568,   489,   207,   207,   207,   207,   563,
       0,   166,     0,     0,   139,   207,   678,   207,   207,   480,
     161,   310,   561,   491,   169,   207,   739,   207,   741,   207,
     742,   743,   630,   207,   740,   207,   638,   177,   738,     0,
     207,   207,     4,     0,     5,    10,    11,    12,    48,    51,
      52,    56,     0,    45,    71,    13,    77,    14,    15,    16,
      17,    29,   546,   547,    22,    46,   178,   188,   207,   197,
     651,   189,    18,     0,    31,    30,     0,    47,     0,   267,
      19,   647,    21,    35,    32,    33,   187,   311,     0,   185,
       0,   331,     0,   642,   183,   348,   444,   430,   640,   186,
       0,   492,     0,   184,   656,   636,   635,   639,   550,   548,
       0,   207,   645,   646,   650,   649,   648,     0,   549,     0,
     657,   658,   659,   681,   682,   641,   552,   551,   643,   644,
       0,    28,   567,   159,   207,   138,     0,   568,   207,   207,
       0,     0,   656,   548,   645,   646,   557,   549,   657,   658,
       0,     0,   594,   127,   128,     0,   566,     0,   481,     0,
     488,     0,    20,     0,   528,   207,   134,   140,   151,   146,
     145,   153,   131,   144,   154,   141,   135,   155,   129,   156,
     149,   143,   150,   147,   148,   130,   132,   142,   152,   157,
     136,   320,   133,   206,     0,     0,     0,     0,    70,    69,
      71,   207,     0,     0,   249,     0,     0,     0,     0,   499,
       0,   174,    40,     0,   302,     0,   301,   724,   618,   615,
     616,   617,   495,   560,   725,     7,   207,   330,   330,   425,
     171,   420,   172,   421,   422,   426,   427,   170,   428,   429,
     418,   524,     0,   335,   336,   338,     0,   419,   523,   340,
     511,     0,   207,   207,   175,   642,   633,   655,   634,     0,
       0,    43,     0,   564,     0,   207,    44,   558,     0,   207,
     280,    48,   284,   281,   284,   562,     0,   726,   728,   628,
     723,   722,     0,    74,    78,     0,     0,   207,     0,     0,
     537,   629,     0,     6,   308,     0,   195,   207,   309,     0,
      49,     0,     9,    71,    50,    53,     0,    60,     0,   207,
      72,   207,     0,   493,   193,     0,   679,   311,   652,   202,
     213,     0,   210,   211,   212,   207,   207,   207,   209,   569,
     577,     0,   251,     0,   207,     0,   307,     0,   442,   443,
     441,   356,   446,   451,   454,   445,   449,   448,   450,   453,
       0,   434,   436,   440,   439,   431,   432,   435,   438,     0,
     485,     0,   482,     0,     0,   637,    34,   619,   207,   207,
     207,   207,   207,   207,   727,   207,   207,   207,   207,   207,
       0,   627,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   387,   394,
     395,   396,   391,   393,   397,   398,   389,   392,   390,   388,
     207,   400,   399,   207,   207,   207,   207,   207,   207,     0,
       0,   210,   207,   207,     0,   598,     0,    36,    24,    37,
       0,    25,   207,    38,   546,     0,   541,   542,   545,     0,
       0,     0,   240,     0,   387,   236,   207,     0,     0,    23,
     207,    26,     0,   503,   505,     0,   522,   207,   508,   207,
       0,   173,    39,   207,   304,     0,   207,   207,    41,     0,
       0,   182,   180,   423,   424,     0,   419,   444,   418,     0,
     526,   207,   121,   653,   654,   346,     0,   176,     0,    42,
     207,     0,   291,     0,   282,   283,    27,    76,    75,    79,
     207,   687,     0,   207,   207,   675,   673,     8,   495,   207,
     207,   208,    56,   207,    54,     0,    65,     0,   119,     0,
     114,     0,    86,     0,   207,   207,   190,   311,     0,   198,
     204,     0,   201,     0,     0,     0,     0,     0,   578,     0,
       0,   207,     0,   528,     0,     0,     0,   254,     0,   252,
     278,   312,     0,     0,     0,   328,     0,   321,   452,   447,
     516,     0,   518,   455,   437,   433,   405,   134,   383,   140,
     381,   146,   145,   144,   141,   135,   385,   155,   156,   143,
     147,   130,   142,   157,   380,   362,   365,   363,   364,   386,
     375,   366,   379,   371,   369,   382,   370,   368,   373,   378,
     367,   372,   376,   377,   374,   384,   207,   359,     0,   405,
     360,   405,   357,   207,   484,     0,   479,   498,   497,   719,
     718,   721,   730,   729,   734,   733,   715,   712,   713,   714,
     631,   702,   137,     0,   669,   670,   139,   668,   667,   625,
     706,   717,   711,   709,   720,   710,   708,   700,   705,   707,
     716,   699,   703,   704,   701,   626,     0,     0,     0,     0,
       0,     0,   732,   731,   736,   735,   207,   207,     0,     0,
       0,   278,     0,   604,   605,   611,   610,   612,   607,   613,
     609,   596,     0,   599,     0,     0,     0,     0,   529,   207,
     554,   207,   207,   207,   239,     0,   235,     0,     0,     0,
       0,   553,     0,     0,   506,     0,   521,   520,   207,   501,
     624,   500,   303,   300,     0,     0,     0,     0,   666,   526,
     341,   337,     0,   207,   527,   508,   339,   207,     0,   459,
     345,     0,     0,     0,   555,     0,   285,   688,   632,   674,
     538,   537,   676,   539,     0,     0,     0,    57,    58,     0,
      61,    63,    67,    66,   207,    97,     0,     0,   207,    92,
     207,   207,   207,    64,   207,   362,   207,   363,   364,   207,
     207,   374,     0,   401,   402,    81,    80,    91,     0,     0,
     313,     0,     0,   207,     0,   216,   215,   207,   579,     0,
       0,   207,   207,   535,     0,   530,     0,   533,   691,   532,
     207,   207,   695,     0,   224,     0,     0,   207,   222,     0,
       0,     0,   275,   207,     0,   330,   319,   325,   207,   323,
     318,   514,   513,   512,   515,   456,   462,   418,   355,     0,
     361,     0,   351,   352,   486,   483,     0,     0,   125,   124,
     123,   122,   664,   665,     0,   207,   689,   207,   207,   207,
     207,   597,   606,   608,   207,   595,   158,   165,   164,   163,
     160,   167,   168,   162,   166,   161,   169,     0,   207,   207,
     494,   544,   543,   248,   247,     0,   242,     0,   207,   238,
       0,   244,     0,   277,   504,   507,   508,   509,   510,   299,
       0,     0,     0,   508,   207,   455,   526,   525,   207,   414,
     412,   347,   460,   293,     0,   207,   292,   295,   556,     0,
     286,   289,     0,   207,     0,   207,   207,    59,   120,   117,
      98,   109,   104,   103,   102,   111,    99,   112,   107,   101,
     108,   105,   106,   100,   110,   113,   207,   116,   115,    88,
      87,     0,     0,   207,   191,   207,   192,   313,   330,     0,
       0,     0,   207,     0,   232,     0,     0,     0,   690,   531,
     230,   580,   536,   693,     0,   207,   692,   697,   696,     0,
     573,   225,   207,   572,     0,     0,   223,   253,   250,     0,
     306,   181,   179,   305,   330,   330,   314,   329,     0,     0,
       0,   207,   349,   410,     0,   407,   411,   358,   405,   405,
     671,   672,   277,     0,     0,   207,     0,     0,     0,   600,
     508,   656,   656,   241,   237,   585,   243,     0,    73,   274,
     502,   207,   207,   496,   415,   413,   342,   343,   508,   404,
     297,     0,     0,     0,   207,   287,     0,     0,     0,     0,
      62,     0,   207,   207,   207,    93,    95,    84,    83,    82,
     330,   199,   205,   203,   214,     0,   234,   233,     0,     0,
       0,     0,   694,   231,   207,     0,     0,   220,     0,     0,
     218,     0,   279,   330,   316,   315,   324,     0,   476,   477,
       0,   478,   464,   467,     0,   463,     0,   418,   406,   353,
     354,   207,   207,     0,   207,   207,   279,   601,     0,   246,
       0,     0,   417,   416,     0,   296,     0,   290,     0,     0,
     118,    89,     0,     0,   207,   200,   217,   207,   228,   582,
       0,   207,   226,   581,     0,   698,     0,   207,   571,   221,
     207,   570,   219,   276,   317,   327,   475,   207,     0,     0,
       0,     0,     0,     0,     0,     0,   457,   350,   458,   408,
     207,   207,   207,   207,   207,   245,   620,   621,   298,   288,
      94,    96,    85,     0,   229,     0,   227,   576,     0,     0,
     466,   468,   469,   472,   473,   474,   470,   471,   465,     0,
       0,     0,     0,   622,   623,     0,     0,     0,     0,   207,
     207,   207,   207,   584,   583,   575,   574
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -912,  -912,  -912,    -1,   -58,  1657,  -912,   476,   755,  -912,
    -912,   529,  -384,  -101,  -912,   290,  -912,  -912,  -111,  -912,
    -912,    49,   636,  -912,  -537,  2611,   499,  -568,  -912,  -790,
    -912,  -912,  -912,    52,  -912,  -912,  -912,   845,  -912,  2664,
    -188,  -912,  -912,  -468,   236,  -912,  -841,  -912,  -912,   346,
    -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,  -912,     5,
    -912,  -156,   509,  -362,  -912,   487,  -912,  -912,  -912,     3,
    1299,  -912,  -912,  -912,  -912,   543,  -912,   -91,  -912,  -912,
    -912,  -912,   357,  -912,  -912,  -912,   -97,  -912,  -342,  -793,
    -912,  -912,  -912,  -912,  -912,  -604,  -912,  -912,  -912,  -912,
     344,  -912,   -35,  -912,  -912,  -912,  -912,   500,  -912,  -912,
    -912,  -912,    -6,  -408,  -172,  -779,  -911,  -643,  -912,  -912,
    -264,  -865,   588,   -46,  -912,  -912,   352,   -63,  -912,  -255,
    -912,  -912,  -308,  -300,  -750,  -912,  -912,    74,   239,  -912,
     391,  -200,   790,  -912,  -912,  -160,   400,   155,  -417,  -764,
    -601,  -912,  -912,  -912,  -707,  -505,  -912,  -739,   -14,  -912,
    -912,  -575,    73,  -912,  -257,  -492,  -556,    -2,  -912,  -912,
    -912,  -912,  -912,  -912,    98,  -912,   901,  -494,  -912,     8,
    -912,  -912,   129,   453,  -912,  -912,  -912,  -912,  2130,  -912,
     -61,   625,  -912,  1003,  1127,  -912,  -912,  -912,  -912,  -912,
    -912,  -912,  -912,  -912,  -472
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   344,   103,   104,   105,   106,   107,   361,
     362,   108,   109,   110,   111,   367,   574,   809,   810,   112,
     113,   114,   372,   115,   116,   117,   345,   837,   581,   838,
     118,   119,   997,   578,   579,   120,   121,   215,   250,   267,
     122,   252,   123,   301,   528,   124,   125,   288,   287,   126,
     127,   128,   129,   130,   590,   131,   591,   841,   132,   200,
     134,   387,   388,   599,   135,   256,   136,   137,   608,   609,
     201,   139,   140,   141,   142,   554,   796,   972,   143,   144,
     792,   967,   274,   275,   145,   146,   147,   148,   395,   875,
     149,   150,   616,  1048,  1049,   617,   151,   152,   302,   303,
     304,   955,   153,   316,   546,   787,   154,   155,  1146,   156,
     157,   668,   669,   833,   834,   835,   959,   888,  1053,  1054,
    1055,   305,   306,   536,   419,   401,   410,   886,  1207,   790,
     158,  1052,  1142,  1143,  1144,   159,   160,   421,   422,   674,
     161,   162,   163,   219,   529,   268,   269,   514,   515,   769,
     308,   884,   622,   623,   518,   309,   784,   785,   346,   601,
     602,   855,   348,   349,   495,   496,   497,   202,   165,   166,
     389,   167,   390,   168,   203,   170,   211,   212,   742,   743,
     744,   171,   204,   205,   174,   175,   176,   317,   177,   379,
     498,   207,   179,   208,   209,   182,   183,   184,   185,   604,
     186,   187,   188,   189,   190
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     164,   102,   424,   300,   508,   319,   474,   133,   365,   789,
     877,   670,   223,   614,   746,   307,   832,   811,   762,   490,
     883,   960,  1056,   164,   336,   605,   892,   859,   893,   530,
     133,   315,  1002,   377,  1046,   588,   786,   494,   998,   350,
     953,   516,  1084,   485,   483,   791,  -326,  1008,  -272,   766,
     315,   311,  -344,   851,   905,   270,   863,   270,   946,   692,
     270,   369,   788,  1126,   270,   315,   533,  -273,   551,   682,
     684,   164,   354,   494,   519,   164,   164,   210,   133,   213,
     501,   564,   133,   133,   260,   506,   861,   768,   964,   872,
     359,  1197,   391,   516,   788,   763,   873,   164,   494,   370,
     169,   164,   560,     3,   133,   850,   856,   565,   133,   534,
     195,   865,   869,   958,   862,   722,   724,   380,  -294,   620,
    1198,   729,   782,   169,   210,   191,   561,   371,   380,   965,
     214,   172,   849,   481,   383,   384,   381,   494,   358,  -519,
    -255,   545,   210,   210,   382,   383,   384,   359,   380,   210,
    -517,   852,   852,   882,   172,   210,   381,   363,   696,  -294,
      81,  -256,   225,  -519,   382,   383,   384,  1046,   523,   853,
     853,   169,   520,   492,   218,   169,   169,  1162,  -517,   854,
     906,   261,  1080,   966,   517,  -519,  -322,    23,  -344,  1085,
     315,   359,   359,   359,  -262,   932,   359,   169,   359,   359,
     359,   169,   172,  1134,  1135,   570,   172,   172,   805,   164,
    -322,  -519,   486,  -294,  1110,  -519,   133,  1088,   720,   721,
    1070,   385,  -517,   973,  1107,  1206,   315,  1050,   172,  -261,
     359,   618,   172,  1138,  -517,   904,   517,  -519,    55,   430,
     380,   385,   624,   210,   210,   839,   492,   671,  1164,  1138,
      64,   315,   698,  1001,   220,  -519,   382,   383,   384,   434,
     912,   278,   621,   164,   493,   482,   225,   440,   164,  1046,
     133,   253,   692,    83,   619,   133,   386,  1019,   957,   587,
     272,  -517,  1056,  1051,  1139,   625,  -517,   494,   494,   693,
     315,   764,  1194,   793,   210,   398,   569,   431,  1118,  1168,
    1139,   432,   694,   913,   321,  -270,  1157,   191,   -70,   169,
     856,   865,   869,   557,  1173,   399,   870,   765,   -69,   914,
     279,   492,   494,   195,  1163,   400,   853,   -70,   261,   777,
     747,   695,  1141,   385,   751,   262,   858,   -69,  1140,   280,
     172,   402,   871,   164,   559,   915,   -70,   434,  1141,   974,
     133,  -260,   438,  1086,   281,   440,   -69,   164,   286,   443,
     943,   403,   494,   169,   133,   404,   164,   764,   169,   764,
     976,   696,   600,   133,   -70,    37,   751,   697,  -330,   164,
     611,  1039,  -257,   289,   -69,   494,   133,   285,   386,   405,
    1196,   842,   406,   944,   172,   956,   977,   492,  -330,   172,
     751,    53,  1062,   291,   407,  -264,   751,   293,  -330,   717,
     477,  -603,   718,   719,   478,  1149,  1150,   672,  1068,  -602,
     811,  -268,   408,  1116,   879,  -265,  1132,   409,  1101,  1199,
    -259,   294,  1156,   589,   295,   315,    57,  -603,   315,   315,
     315,   315,   746,   169,  1136,  -602,   296,  1003,   880,  1232,
    1233,  1234,  1235,  1236,  1237,   173,  1200,   169,  1201,  1202,
    1022,  1203,  1204,   492,   298,   324,   169,   675,   728,   299,
    1023,  1170,   520,  -263,   172,   315,   814,   818,   173,   169,
     300,   542,   520,   670,     7,   676,  1003,   191,   172,  1016,
     678,  1045,   307,   812,   815,   819,   520,   172,  1003,   164,
     930,   164,   801,   801,  1004,  1034,   133,   675,   133,   522,
     172,   524,   325,   365,  1083,   315,  1105,   929,   475,  -269,
     476,   478,  1119,  1121,   520,  1112,   173,  -409,   315,   315,
     173,   173,  1003,  1195,  1022,  1172,   430,   289,   315,   494,
    1128,  1131,  1113,  1063,  1026,   411,   366,  1066,   258,  1022,
    1220,   259,   173,   330,   333,   804,   173,   291,   549,  1122,
    -741,   293,  -271,   494,  -741,   412,   682,   722,  -258,   413,
    -742,  -663,   434,  -663,  -742,   164,   559,   438,  -266,  -743,
     440,   368,   133,  -743,   443,   294,   393,   857,   295,   394,
     352,  -738,   397,   414,   431,  -738,   415,  -588,   432,   169,
     296,   169,  1119,  1121,  1128,  1131,   331,   331,   416,   789,
    -660,  1096,  -660,  1098,  -662,  -587,  -662,   952,   298,  -661,
     896,  -661,   897,   299,  1045,   426,   417,   178,   979,   698,
     172,   418,   172,   531,   532,   430,   800,   803,  -591,   425,
     484,  -592,   359,  -590,   434,   435,  -589,   436,   437,   438,
     178,   439,   440,   441,   488,   491,   443,   370,   509,   510,
    1133,   538,   449,   450,   173,  1137,   453,   526,   527,   454,
     455,   456,   537,   550,   547,   169,   430,   541,   556,   563,
     457,   567,  1010,   572,   566,   494,   583,   573,   931,   594,
     673,   612,   315,   431,   677,   440,   716,   432,   178,   749,
     751,   752,   178,   178,   753,   727,   172,   948,   750,   755,
     758,   760,   761,   768,   315,   778,   315,   794,   173,  1059,
     795,   783,   494,   173,   178,   840,  1045,   843,   178,   847,
     492,   876,   878,   315,   431,   885,   887,   898,   432,  -137,
    -158,  -165,   927,   434,   435,  -164,   436,   437,   438,  -163,
     439,   440,  -160,   164,  -138,   443,   164,   890,   899,  -167,
     133,   449,   450,   133,  -168,   453,  -162,  -166,   454,   455,
     456,  -139,   902,  -161,  -169,   907,   891,   900,   901,   903,
     935,   937,   954,   910,   434,   435,   164,   962,   494,   438,
    1007,   439,   440,   133,   911,   289,   443,  1003,   173,   963,
    1006,  1012,  1025,   450,   968,  1015,  1060,  1024,  1040,   454,
     455,   456,   173,  1061,  1077,   291,  1078,  1044,  1079,   293,
    1081,   173,  1094,  1106,  1160,  1161,  1082,  1147,  1154,   300,
    1148,   845,   846,  1013,   173,  1151,   178,  1158,   315,  1166,
    1180,   864,   868,   294,  1184,  1216,   295,   164,    57,  1193,
    1217,  1221,   164,   169,   133,  1243,   169,  1244,   296,   133,
    1198,   164,   807,   360,   164,  1100,   978,   577,   133,   221,
     571,   133,   595,  1037,   430,   315,   298,   555,  1093,   592,
     772,   299,   781,  1209,   172,  1058,   169,   172,   535,   780,
     178,  1208,  1087,   494,   494,   178,   507,  1238,  1231,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   470,   471,   472,   895,  1009,   774,   172,   374,   945,
     771,   217,  1069,     0,     0,     0,  1071,  1072,     0,     0,
       0,     0,   431,   164,     0,   164,   432,     0,   164,     0,
     133,   315,   133,   473,   365,   133,  -660,   169,  -660,     0,
    1044,  1092,   169,   506,   173,     0,   173,     0,     0,     0,
       0,   169,   164,     0,   169,     0,     0,     0,     0,  1091,
     178,     0,   164,     0,     0,     0,     0,     0,   172,   133,
       0,     0,   434,   172,   178,     0,     0,   438,     0,   439,
     440,     0,   172,   178,   443,   172,     0,     0,     0,     0,
       0,   450,     0,     0,     0,   180,   178,   454,   455,   456,
       0,     0,     0,   164,     0,     0,     0,     0,     0,     0,
     133,     0,   164,     0,     0,     0,     0,     0,   180,   133,
     173,     0,   365,   169,     0,   169,   365,     0,   169,     0,
       0,   864,   868,     0,     0,     0,   315,   315,     0,     0,
       0,   506,  1044,   506,     0,     0,   506,   506,     0,     0,
       0,     0,   169,     0,   172,     0,   172,     0,     0,   172,
       0,     0,   169,     0,     0,   164,   180,     0,     0,     0,
     180,   180,   133,     0,     0,     0,     0,     0,     0,   300,
       0,     0,     0,   172,     0,     0,     0,     0,     0,     0,
       0,  1031,   180,   172,     0,  1036,   180,     0,     0,     0,
       0,     0,     0,   169,     0,     0,   164,     0,   164,     0,
       0,     0,   169,   133,   164,   133,   178,   164,   178,   181,
       0,   133,     0,     0,   133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   172,     0,     0,     0,     0,     0,
       0,     0,   181,   172,     0,     0,   164,     0,     0,     0,
       0,   430,   164,   133,     0,     0,   164,   164,     0,   133,
       0,     0,     0,   133,   133,   169,     0,     0,   164,     0,
       0,     0,   164,     0,   164,   133,     0,     0,     0,   133,
       0,   133,     0,     0,     0,     0,     0,     0,     0,     0,
     181,     0,   178,     0,   181,   181,   172,     0,   173,     0,
       0,   173,     0,     0,   180,     0,   169,     0,   169,   431,
       0,     0,     0,   432,   169,     0,   181,   169,     0,     0,
     181,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   173,     0,   164,   164,   164,   164,   172,     0,   172,
     133,   133,   133,   133,     0,   172,   169,     0,   172,     0,
       0,     0,   169,     0,     0,     0,   169,   169,   180,   434,
    1127,  1130,     0,   180,   438,     0,   439,   440,   169,     0,
       0,   443,   169,     0,   169,     0,     0,   172,   450,     0,
       0,     0,     0,   172,     0,     0,   456,   172,   172,     0,
       0,   138,   173,     0,     0,     0,     0,   173,     0,   172,
       0,     0,     0,   172,     0,   172,   173,     0,     0,   173,
       0,     0,     0,     0,   138,     0,     0,     0,  1165,     0,
       0,     0,     0,     0,  1127,  1130,     0,     0,   181,     0,
       0,     0,     0,   169,   169,   169,   169,     0,   180,     0,
       0,  1176,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   180,     0,  1189,     0,     0,  1192,     0,     0,
       0,   180,   138,     0,   172,   172,   172,   172,     0,     0,
     178,     0,     0,   178,   180,     0,     0,     0,   173,     0,
     173,     0,   181,   173,     0,     0,     0,   181,   138,     0,
       0,     0,   138,     0,   428,     0,     0,     0,     0,     0,
       0,   429,     0,   178,   423,     0,   226,   173,     0,     0,
       0,     0,     0,   430,     0,     0,     0,   173,   584,     0,
     227,   228,     0,   229,     0,     0,     0,     0,   230,     0,
       0,     0,     0,     0,     0,     0,   231,     0,     0,     0,
       0,     0,   232,     0,     0,     0,     0,     0,   233,     0,
       0,     0,   234,     0,   585,   235,     0,   236,   173,     0,
       0,     0,   181,     0,   178,   237,     0,   173,     0,   178,
       0,   431,   238,   239,     0,   432,   181,     0,   178,     0,
     240,   178,     0,     0,     0,   181,     0,     0,     0,     0,
     241,     0,     0,     0,   180,     0,   180,     0,   181,   242,
     138,   243,     0,   244,   433,   245,     0,     0,   246,     0,
       0,     0,   247,     0,   248,     0,     0,   249,     0,     0,
     173,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,     0,   586,   454,   455,   456,     0,
     178,     0,   178,     0,   138,   178,   874,   457,     0,   138,
    -313,   173,     0,   173,     0,     0,     0,     0,     0,   173,
     180,     0,   173,   266,     0,     0,     0,  -313,     0,   178,
       0,  -313,  -313,     0,     0,     0,     0,     0,   430,   178,
    -313,     0,  -313,  -313,     0,     0,     0,     0,  -313,     0,
       0,   173,     0,     0,     0,  -313,     0,   173,  -313,     0,
       0,   173,   173,     0,     0,     0,     0,     0,   181,     0,
     181,     0,     0,   173,     0,     0,  -313,   173,  -313,   173,
     178,  -313,     0,  -313,   138,  -313,     0,  -313,  -313,   178,
    -313,     0,     0,  -313,     0,  -313,   431,     0,   138,     0,
     432,     0,     0,     0,     0,     0,     0,   138,     0,     0,
       0,     0,     0,  -313,     0,  -313,     0,     0,  -313,     0,
     138,     0,   222,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   173,   173,
     173,   173,   178,     0,   181,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,     0,     0,     0,   443,     0,
       0,     0,     0,     0,     0,   450,     0,     0,  -313,     0,
     326,   454,   455,   456,  -313,     7,     0,     0,     0,     0,
       0,     0,     0,   178,     0,   178,     0,     0,     0,     0,
       0,   178,   354,     0,   178,     0,    22,    23,   180,     0,
     353,   180,     0,     0,     0,   355,     0,    29,   356,     0,
       0,     0,     0,    32,     0,     0,     0,     0,     0,     0,
      37,     0,     0,   178,     0,     0,     0,     0,     0,   178,
       0,   180,     0,   178,   178,     0,     0,     0,     0,     0,
     138,   357,   138,     0,     0,   178,    53,     0,    55,   178,
      57,   178,  1041,     0,     0,  1042,     0,     0,   358,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       0,     0,     0,     0,     0,     0,     0,     0,    79,     0,
      81,     0,     0,    83,     0,     0,   354,     0,     0,     0,
      22,    23,   180,     0,     0,     0,     0,   180,     0,   355,
       0,    29,   356,     0,     0,     0,   180,    32,   487,   180,
     178,   178,   178,   178,    37,     0,   138,     0,     0,     0,
       0,     0,   181,     0,     0,   181,     0,     0,     0,     0,
       0,     0,     0,    97,     0,   357,     0,     0,     0,  1043,
      53,     0,    55,     0,   352,     0,  1041,     0,     0,  1042,
       0,     0,   358,     0,    64,   181,     0,     0,     0,     0,
       0,     0,   511,     0,     0,     0,     0,   521,     0,     0,
       0,     0,    79,   226,    81,     0,     0,    83,   180,     0,
     180,     0,     0,   180,     0,     0,     0,   227,   228,     0,
     229,     0,     0,     0,     0,   230,     0,     0,     0,     0,
       0,     0,   430,   231,     0,     0,     0,   180,     0,   232,
       0,     0,     0,     0,     0,   233,   181,   180,     0,   234,
       0,   181,   235,     0,   236,     0,     0,    97,     0,     0,
     181,     0,   237,   181,     0,     0,     0,     0,     0,   238,
     239,     0,     0,     0,     0,     0,     0,   240,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   241,   180,     0,
     431,     0,     0,     0,   432,     0,   242,   180,   243,     0,
     244,     0,   245,     0,     0,   246,     0,     0,   593,   247,
       0,   248,     0,     0,   249,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   138,     0,     0,   138,     0,     0,
       0,     0,   181,     0,   181,     0,     0,   181,     0,     0,
     434,   435,     0,     0,   437,   438,     0,   439,   440,     0,
     180,     0,   443,     0,     0,     0,     0,   138,     0,   450,
       0,   181,     0,     0,     0,   454,   455,   456,   359,     0,
     969,   181,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   180,     0,   180,     0,     0,     0,     0,     0,   180,
       0,     0,   180,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   181,   206,     0,     0,     0,   216,   138,     0,
       0,   181,     0,   138,     0,     0,   224,     0,   754,     0,
     756,   180,   138,   257,     0,   138,     0,   180,     0,     0,
       0,   180,   180,     0,   273,     0,   276,   277,     0,     0,
       0,     0,   282,   180,   283,     0,   284,   180,     0,   180,
       0,   310,     0,     0,     0,   224,   318,   320,   322,   323,
       0,     0,     0,   327,   181,   328,     0,     0,     0,     0,
     692,     0,   335,     0,     0,   318,     0,   337,     0,   338,
       0,     0,   339,   340,   916,   341,     0,   917,     0,   224,
     318,   351,   918,     0,   138,     0,   138,     0,     0,   138,
       0,     0,   193,     0,     0,   181,     0,   181,   180,   180,
     180,   180,   919,   181,     0,     0,   181,     0,   375,   920,
     378,   195,     0,   138,     0,     0,     0,     0,     0,   921,
       0,     0,     0,   138,     0,     0,     0,   922,     0,     0,
       0,     0,     0,     0,     0,   181,     0,     0,     0,     0,
       0,   181,     0,     0,   923,   181,   181,     0,     0,     0,
       0,   427,     0,   226,     0,   924,     0,   181,     0,   696,
       0,   181,     0,   181,   138,     0,   925,   227,   228,     0,
     229,   926,     0,   138,   479,   230,     0,     0,   327,   224,
       0,     0,     0,   231,     0,     0,     0,     0,     0,   232,
       0,     0,     0,     0,     0,   233,     0,     0,     0,   234,
       0,     0,   235,     0,   236,   318,     0,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,     0,   238,
     239,     0,   181,   181,   181,   181,   138,   240,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   241,     0,     0,
       0,   318,     0,     0,     0,     0,   242,     0,   243,     0,
     244,     0,   245,     0,     0,   246,     0,     0,     0,   247,
       0,   248,   936,     0,   249,   939,   318,   138,     0,   138,
       0,     0,     0,     0,     0,   138,     0,   327,   138,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   543,   544,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   318,     0,   138,     0,   553,
       0,     0,     0,   138,     0,     0,     0,   138,   138,     0,
     266,     0,     0,     0,     0,     0,     0,   562,     0,   138,
       0,     0,     0,   138,     0,   138,     0,   568,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   580,
       0,   582,     0,     0,     0,     0,  1014,     0,     0,     0,
       0,  1020,     0,     0,     0,   596,   603,   606,     0,     0,
     521,     0,     0,   521,   224,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   138,   138,   138,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   679,   680,
     681,   683,   685,   686,     0,   687,   688,   689,   690,   691,
       0,   699,   700,   701,   702,   703,   704,   705,   706,   707,
     708,   709,   710,   711,   712,   713,   714,   715,     0,     0,
       0,     0,  1073,     0,  1074,     0,     0,  1076,     0,     0,
     318,     0,     0,   318,   318,   318,   318,   723,   725,     0,
       0,     0,   603,   730,     0,     0,   745,     0,     0,     0,
       0,  1090,   748,     0,     0,     0,     0,     0,     0,     0,
       0,  1020,     0,   757,     0,     0,   759,     0,     0,     0,
     318,     0,     0,     0,     0,     0,     0,   767,     0,   770,
       0,     0,     0,   276,     0,   271,   775,   776,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   310,     0,
       0,   310,  1117,     0,     0,     0,     0,     0,     0,   271,
     318,  1123,     0,     0,     0,     0,     0,     0,   332,   334,
     748,     0,   251,   318,   318,     0,     0,     0,     0,   224,
     806,     0,     0,   318,   263,   264,   265,     0,     0,     0,
       0,     0,     0,     0,   836,   836,     0,     0,     0,   364,
       0,     0,     0,     0,     0,     0,     0,     0,   848,     0,
       0,   224,     0,     0,  1159,     0,     0,     0,     0,     0,
       0,     0,     0,   226,     0,     0,     0,   392,     0,     0,
       0,   310,     0,     0,     0,     0,     0,   227,   228,     0,
     229,     0,     0,     0,     0,   230,     0,     0,     0,     0,
       0,     0,     0,   231,     0,  1178,     0,  1182,     0,   232,
       0,     0,     0,   521,     0,   233,   521,     0,     0,   234,
       0,     0,   235,     0,   236,     0,   889,     0,     0,     0,
       0,     0,   237,   894,     0,     0,     0,     0,     0,   238,
     239,     0,   396,     0,     0,  1215,     0,   240,     0,     0,
       0,  1218,     0,     0,   420,  1178,  1182,   241,   489,     0,
       0,     0,     0,     0,     0,     0,   242,  1224,   243,     0,
     244,  1226,   245,   756,     0,   246,     0,     0,     0,   247,
       0,   248,     0,     0,   249,     0,   757,   318,     0,     0,
     480,     0,     0,     0,     0,     0,     0,   502,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   318,
       0,   318,   933,   934,   271,     0,   271,     0,     0,     0,
       0,     0,     0,   271,     0,     0,     0,     0,   318,     0,
       0,     0,   756,   756,   756,   756,     0,     0,     0,     0,
     512,     0,     0,   310,     0,     0,     0,   961,   499,   500,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     513,     0,     0,   271,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   580,     0,     0,     0,   999,     0,
    1000,   836,   836,     0,   337,     0,   338,     0,     0,   340,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     540,     0,     0,  1011,     0,     0,     0,   224,     0,     0,
       0,  1017,   748,     0,  1021,     0,     0,     0,     0,     0,
    1027,  1029,     0,  1030,   354,     0,  1033,  1035,    22,    23,
       0,     0,     0,   318,     0,     0,     0,   355,  1047,    29,
     356,     0,     0,     0,     0,    32,     0,     0,     0,   251,
       0,     0,    37,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1064,  1021,  1030,  1033,  1067,
     318,     0,     0,   357,   745,     0,     0,     0,    53,     0,
      55,     0,     0,     0,     0,   607,     0,   610,   683,   723,
     358,   615,    64,     0,     0,     0,     0,     0,  1075,     0,
       0,     0,   226,     0,     0,     0,     0,     0,     0,     0,
      79,     0,    81,   667,   310,    83,   227,   228,  1089,   229,
       0,     0,     0,     0,   230,   224,     0,     0,     0,     0,
       0,     0,   231,  1097,     0,  1099,   318,     0,   232,     0,
       0,     0,     0,     0,   233,     0,     0,     0,   234,     0,
       0,   235,     0,   236,     0,     0,   836,     0,     0,     0,
       0,   237,     0,  1108,     0,  1109,     0,     0,   238,   239,
       0,     0,  1115,     0,     0,     0,   240,     0,   731,     0,
       0,     0,     0,     0,     0,   748,   241,     0,     0,     0,
       0,     0,  1125,     0,   271,   242,     0,   243,     0,   244,
       0,   245,     0,     0,   246,     0,   513,     0,   247,     0,
     248,  1145,     0,   249,     0,     0,     0,     0,     0,   773,
       0,     0,     0,     0,     0,  1153,     0,     0,     0,   779,
       0,     0,     0,   513,     0,     0,   271,   271,     0,     0,
       0,   318,   318,     0,     0,     0,   271,   271,     0,     0,
       0,     0,     0,     0,  1167,     0,     0,     0,     0,     0,
       0,     0,  1171,   836,   836,     0,     0,     0,     0,   539,
       0,     0,     0,     0,     0,     0,     0,   823,  1179,     0,
    1183,     0,     0,     0,  1185,     0,  1188,     0,   844,  1191,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   615,     0,
       0,  1179,  1183,     0,  1188,  1191,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1222,     0,     0,  1223,     0,     0,
       0,  1225,     0,     0,     0,     0,  1227,  1228,     0,     0,
    1229,     0,     0,     0,     0,     0,     0,  1230,     7,     0,
       0,   430,     0,     0,     0,     0,     0,     0,     0,   420,
    1239,  1240,  1227,  1241,  1242,   354,     0,     0,     0,    22,
      23,     0,     0,     0,     0,     0,     0,     0,   355,     0,
      29,   356,     0,     0,     0,     0,    32,     0,     0,     0,
     941,     0,     0,    37,     0,  1253,  1254,  1255,  1256,  1253,
    1254,  1255,  1256,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,   432,   357,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,  1041,   970,     0,  1042,     0,
       0,   358,     0,    64,     0,     0,   271,   271,     0,     0,
       0,     0,     0,     0,   942,     0,     0,     0,   513,     0,
       0,    79,     0,    81,     0,     0,    83,     0,   949,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
       0,   443,   444,   445,     0,   447,   448,   449,   450,   971,
       0,   453,     0,     0,   454,   455,   456,     0,     0,     0,
       0,     0,     0,     0,     0,   457,   271,     0,     0,     0,
     271,     0,  1038,     0,     0,     0,    97,     0,     0,     0,
       0,     0,  1111,     0,     0,     0,     0,     0,     0,     0,
       0,   732,     0,     5,     6,   420,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,  -207,    21,     0,   733,     0,     0,
       0,     0,     0,    27,   607,   193,     0,     0,     0,    31,
    -207,  -207,  -207,     0,   194,    34,     0,   734,     0,    36,
       0,   735,    38,     0,   195,   667,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,   736,     0,    54,   737,    56,
       0,    58,     0,  1095,     0,     0,    61,    62,   197,     0,
     738,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,     0,     0,   739,    80,
       0,    82,     0,   740,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   971,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,   271,   271,     0,    92,     0,
       0,     0,     7,     0,     0,     0,     0,    93,     0,   430,
      94,    95,    96,     0,     0,     0,    98,     0,   199,   354,
     100,   741,  -207,    22,    23,   101,     0,     0,     0,     0,
       0,     0,   355,     0,    29,   356,     0,     0,     0,     0,
      32,     0,     0,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,   271,     0,     0,     0,     0,     0,   271,
     271,     0,   615,     0,     0,     0,     0,   431,   357,     0,
       0,   432,     0,    53,     0,    55,   271,    57,     0,  1041,
       0,     0,  1042,     0,     0,   358,   505,    64,     0,   271,
       0,     0,   271,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,    79,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,  1219,   442,   443,
     444,   445,     0,   447,   448,   449,   450,   451,   452,   453,
       0,     0,   454,   455,   456,     0,     0,     0,     0,     0,
       0,   615,     0,   457,     0,     0,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,  1175,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   505,     0,   505,     0,     0,   505,
     505,    -2,     4,     0,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -207,    21,    22,    23,    24,
      25,    26,     0,  -207,    27,     0,    28,     0,    29,    30,
      31,  -207,  -207,  -207,    32,    33,    34,   -68,     0,    35,
      36,    37,     0,    38,  -330,    39,     0,    40,    41,    42,
      43,     0,    44,    45,    46,    47,   -68,    48,    49,     0,
      50,    51,    52,     0,  -330,     0,     0,    53,    54,    55,
      56,    57,    58,    59,  -330,   -68,    60,    61,    62,    63,
       0,    64,    65,    66,     0,    67,    68,    69,    70,    71,
      72,    73,     0,    74,    75,     0,    76,    77,    78,    79,
      80,    81,    82,   -68,    83,    84,     0,     0,  -207,     0,
      85,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,    97,     0,     0,    98,     0,    99,
       0,   100,     0,  -207,     0,     4,   101,     5,     6,     7,
       8,     9,    10,    11,    12,     0,  -689,     0,    13,    14,
      15,    16,    17,  -689,    18,     0,    19,    20,  -207,    21,
      22,    23,    24,    25,    26,  -689,  -207,    27,  -689,    28,
       0,    29,    30,    31,  -207,  -207,  -207,    32,    33,    34,
     -68,     0,    35,    36,    37,     0,    38,  -330,    39,     0,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -68,
      48,    49,     0,    50,    51,    52,     0,  -330,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -330,   -68,    60,
      61,    62,    63,  -689,    64,    65,    66,  -689,    67,    68,
      69,    70,    71,    72,    73,     0,    74,    75,     0,    76,
      77,    78,    79,    80,    81,    82,   -68,    83,    84,     0,
       0,  -207,     0,    85,     0,     0,  -689,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,  -689,    89,  -689,  -689,  -689,  -689,
    -689,  -689,  -689,     0,  -689,  -689,  -689,  -689,  -689,  -689,
    -689,  -689,  -689,  -689,  -689,  -689,    96,    97,  -689,  -689,
    -689,     0,    99,  -689,   100,     0,  -207,     0,   342,  -689,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,  -207,    21,    22,    23,    24,    25,    26,     0,  -207,
      27,     0,    28,     0,    29,    30,    31,  -207,  -207,  -207,
      32,    33,    34,   -68,     0,    35,    36,    37,     0,    38,
    -330,    39,     0,    40,    41,    42,    43,     0,    44,    45,
      46,    47,   -68,    48,    49,     0,    50,    51,    52,     0,
    -330,     0,     0,    53,    54,    55,    56,    57,    58,    59,
    -330,   -68,    60,    61,    62,    63,     0,    64,    65,    66,
       0,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,    81,    82,   -68,
      83,    84,     0,     0,  -207,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,   343,   100,     0,  -207,
       0,     4,   101,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -207,    21,    22,    23,    24,    25,
      26,     0,  -207,    27,     0,    28,     0,    29,    30,    31,
    -207,  -207,  -207,    32,    33,    34,   -68,     0,    35,    36,
      37,     0,    38,  -330,    39,     0,    40,    41,    42,    43,
       0,    44,    45,    46,    47,   -68,    48,    49,     0,    50,
      51,    52,     0,  -330,     0,     0,    53,    54,    55,    56,
      57,    58,    59,  -330,   -68,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
      81,    82,   -68,    83,    84,     0,     0,  -207,     0,    85,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,    97,     0,     0,    98,     0,    99,   558,
     100,     0,  -207,     0,   575,   101,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,  -207,    21,    22,
      23,    24,    25,    26,     0,  -207,    27,     0,    28,     0,
      29,    30,    31,  -207,  -207,  -207,    32,    33,    34,   -68,
       0,    35,    36,    37,     0,    38,  -330,    39,     0,    40,
      41,    42,    43,     0,    44,    45,    46,    47,   -68,    48,
      49,     0,    50,    51,    52,     0,  -330,     0,     0,    53,
      54,    55,    56,    57,    58,    59,  -330,   -68,    60,    61,
      62,    63,     0,    64,    65,    66,     0,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,    81,    82,   -68,    83,    84,     0,     0,
    -207,     0,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,    97,     0,     0,    98,
       0,    99,   576,   100,     0,  -207,     0,     4,   101,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -207,    21,    22,    23,    24,    25,    26,     0,  -207,    27,
       0,    28,     0,    29,    30,    31,  -207,  -207,  -207,    32,
      33,    34,   -68,     0,    35,    36,    37,     0,    38,  -330,
      39,     0,    40,    41,    42,    43,     0,    44,    45,    46,
      47,   -68,    48,    49,     0,    50,    51,    52,     0,  -330,
       0,     0,    53,    54,    55,    56,    57,    58,    59,  -330,
     -68,    60,    61,    62,    63,     0,    64,    65,    66,     0,
      67,    68,    69,    70,    71,    72,    73,     0,    74,    75,
       0,    76,    77,    78,    79,    80,    81,    82,   -68,    83,
      84,     0,     0,  -207,     0,    85,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,    97,
       0,     0,    98,     0,    99,   813,   100,     0,  -207,     0,
       4,   101,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -207,    21,    22,    23,    24,    25,    26,
       0,  -207,    27,     0,    28,     0,    29,    30,    31,  -207,
    -207,  -207,    32,    33,    34,   -68,     0,    35,    36,    37,
       0,    38,  -330,    39,     0,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -68,    48,    49,     0,    50,    51,
      52,     0,  -330,     0,     0,    53,    54,    55,    56,   352,
      58,    59,  -330,   -68,    60,    61,    62,    63,     0,    64,
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
      35,    36,    37,     0,    38,  -330,    39,     0,    40,    41,
      42,    43,     0,    44,    45,    46,    47,   -68,    48,    49,
       0,    50,    51,    52,     0,  -330,     0,     0,    53,    54,
      55,    56,    57,    58,    59,  -330,   -68,    60,    61,    62,
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
      28,     0,    29,    30,    31,  -207,  -207,  -207,    32,    33,
      34,   -68,     0,    35,    36,    37,     0,    38,  -330,    39,
       0,    40,    41,    42,    43,     0,    44,    45,    46,    47,
     -68,    48,    49,     0,    50,    51,    52,     0,  -330,     0,
       0,    53,    54,    55,    56,     0,    58,    59,  -330,   -68,
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
    -207,    32,  1032,    34,   -68,     0,    35,    36,    37,     0,
      38,  -330,    39,     0,    40,    41,    42,    43,     0,    44,
      45,    46,    47,   -68,    48,    49,     0,    50,    51,    52,
       0,  -330,     0,     0,    53,    54,    55,    56,     0,    58,
      59,  -330,   -68,    60,    61,    62,    63,     0,    64,    65,
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
      31,  -207,  -207,  -207,    32,  1177,    34,   -68,     0,    35,
      36,    37,     0,    38,  -330,    39,     0,    40,    41,    42,
      43,     0,    44,    45,    46,    47,   -68,    48,    49,     0,
      50,    51,    52,     0,  -330,     0,     0,    53,    54,    55,
      56,     0,    58,    59,  -330,   -68,    60,    61,    62,    63,
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
       0,    29,    30,    31,  -207,  -207,  -207,    32,  1181,    34,
     -68,     0,    35,    36,    37,     0,    38,  -330,    39,     0,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -68,
      48,    49,     0,    50,    51,    52,     0,  -330,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -330,   -68,    60,
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
      32,  1187,    34,   -68,     0,    35,    36,    37,     0,    38,
    -330,    39,     0,    40,    41,    42,    43,     0,    44,    45,
      46,    47,   -68,    48,    49,     0,    50,    51,    52,     0,
    -330,     0,     0,    53,    54,    55,    56,     0,    58,    59,
    -330,   -68,    60,    61,    62,    63,     0,    64,    65,    66,
       0,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,    81,    82,   -68,
      83,    84,     0,     0,  -207,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,     0,   100,     0,  -207,
       0,     4,   101,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,  -207,    21,    22,    23,    24,    25,
      26,     0,  -207,    27,     0,    28,     0,    29,    30,    31,
    -207,  -207,  -207,    32,  1190,    34,   -68,     0,    35,    36,
      37,     0,    38,  -330,    39,     0,    40,    41,    42,    43,
       0,    44,    45,    46,    47,   -68,    48,    49,     0,    50,
      51,    52,     0,  -330,     0,     0,    53,    54,    55,    56,
       0,    58,    59,  -330,   -68,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
      81,    82,   -68,    83,    84,     0,     0,  -207,     0,    85,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,    97,     0,     0,    98,     0,    99,     0,
     100,   881,  -207,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,  -207,    21,     0,     0,     0,     0,
       0,     0,     0,   290,     0,   193,     0,     0,     0,    31,
    -207,  -207,  -207,     0,   194,    34,     0,     0,     0,   292,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     297,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   199,     0,
     100,     0,  -207,     5,     6,   101,     8,     9,    10,    11,
      12,     0,   628,     0,    13,     0,     0,    16,    17,   630,
      18,     0,   192,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,   194,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,   636,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,   504,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,     0,   471,   472,     0,   824,
     825,   826,   827,   828,   649,     0,   650,     0,    92,     0,
     651,   652,   653,   654,   655,   656,   657,   829,   659,   660,
      94,   830,    96,   -90,   662,   663,   831,   665,   199,     0,
     100,     0,     0,     5,     6,   101,     8,     9,    10,    11,
      12,     0,   628,     0,    13,     0,     0,    16,    17,   630,
      18,     0,   192,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,   194,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,   636,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,   504,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,     0,   471,   472,     0,   824,
     825,   826,   827,   828,   649,     0,   650,     0,    92,     0,
     651,   652,   653,   654,   655,   656,   657,   829,   659,   660,
      94,   830,    96,     0,   662,   663,   831,   665,   199,     0,
     100,     0,     0,     0,     0,   101,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,     0,
      16,    17,     0,    18,     0,   192,    20,     0,    21,     0,
       0,     0,     0,    26,     0,     0,    27,     0,   193,     0,
       0,     0,    31,     0,     0,     0,     0,    33,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   195,     0,   196,
       0,     0,    43,     0,    44,    45,    46,    47,     0,    48,
      49,     0,    50,    51,   329,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   197,     0,     0,     0,     0,     0,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,     0,
       0,     0,    80,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,    99,     0,   100,     0,     0,     5,     6,   101,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   192,     0,     0,    21,     0,
     733,     0,     0,     0,     0,     0,    27,     0,   193,     0,
       0,     0,    31,     0,     0,     0,     0,   194,    34,     0,
     734,     0,    36,     0,   735,    38,     0,   195,     0,   196,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,   736,     0,
      54,   737,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   197,     0,   738,     0,     0,     0,     0,     0,    69,
      70,    71,    72,   198,     0,    74,     0,     0,    76,     0,
       0,   739,    80,     0,    82,     0,   740,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   199,     0,   100,     0,     0,     5,     6,   101,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   192,     0,     0,    21,   373,
      23,     0,     0,     0,     0,     0,    27,     0,   193,     0,
       0,     0,    31,     0,     0,     0,     0,   194,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   195,     0,   196,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,   357,     0,     0,     0,     0,     0,
      54,    55,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   197,     0,    64,     0,     0,     0,     0,     0,    69,
      70,    71,    72,   198,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,    82,     0,    83,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   199,     0,   100,     0,     0,     5,     6,   101,     8,
       9,    10,   376,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,    19,     0,  -207,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   193,     0,
       0,     0,    31,  -207,  -207,  -207,     0,   194,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   195,     0,   196,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,    63,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,   198,     0,    74,     0,     0,    76,     0,
       0,     0,    80,    81,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   199,     0,   100,     0,  -207,     5,     6,   101,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   192,     0,  -207,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   193,     0,
       0,     0,    31,  -207,  -207,  -207,     0,   194,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   195,     0,   196,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   197,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,   198,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,     0,     0,     0,    98,
       0,   199,     0,   100,     0,  -207,     5,     6,   101,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    27,     0,   193,     0,
       0,     0,    31,     0,     0,     0,     0,   194,    34,     0,
       0,     0,    36,     0,     0,    38,     0,   195,     0,   196,
       0,     0,    43,     0,     0,    45,     0,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
      62,   197,  -737,     0,     0,     0,  -737,     0,     0,    69,
      70,    71,    72,   198,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,  -372,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,  -372,     0,     0,    98,
       0,   199,  -372,   100,     0,     0,     5,     6,   101,     8,
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
      93,     0,     0,    94,    95,    96,   314,     0,     0,    98,
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
       0,    92,   347,     0,     0,     0,     0,     0,     0,     0,
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
       0,   199,     0,   100,  -540,     0,     5,     6,   101,     8,
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
       0,   199,     0,   100,     0,     0,  -540,     0,   101,     5,
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
       0,    76,   312,   313,     0,    80,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   199,     0,   100,   799,     0,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   192,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   193,     0,     0,     0,    31,     0,     0,     0,     0,
     194,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     195,     0,   196,     0,     0,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   197,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,   198,     0,    74,     0,
       0,    76,   312,   313,     0,    80,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   199,     0,   100,   802,     0,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   192,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   193,     0,     0,     0,    31,     0,     0,     0,     0,
     194,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     195,     0,   196,     0,     0,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   197,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,   198,     0,    74,     0,
       0,    76,   312,   313,     0,    80,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   199,     0,   100,   808,     0,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   192,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   193,     0,     0,     0,    31,     0,     0,     0,     0,
     194,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     195,     0,   196,     0,     0,    43,     0,     0,    45,     0,
      47,     0,    48,    49,   947,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   197,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,   198,     0,    74,     0,
       0,    76,   312,   313,     0,    80,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   199,     0,   100,     0,     0,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
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
       0,  -380,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,  -380,
       0,     0,    98,     0,   199,  -380,   100,     0,     0,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
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
       0,  -365,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,  -365,
       0,     0,    98,     0,   199,  -365,   100,     0,     0,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
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
       0,  -373,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,  -373,
       0,     0,    98,     0,   199,  -373,   100,     0,     0,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   192,     0,
       0,    21,     0,   254,     0,     0,     0,     0,     0,    27,
       0,   193,     0,     0,     0,    31,     0,     0,     0,     0,
     194,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     195,     0,   196,     0,     0,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   197,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,   198,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,    82,     0,   255,
      84,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   199,     0,   100,     0,     0,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   192,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   193,     0,     0,     0,    31,     0,     0,     0,     0,
     194,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     195,     0,   196,     0,     0,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   197,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,   198,     0,    74,     0,
       0,    76,   312,   313,     0,    80,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   199,     0,   100,     0,     0,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   192,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   193,     0,     0,     0,    31,     0,     0,     0,     0,
     194,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     195,     0,   196,     0,     0,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   197,  -737,     0,     0,     0,  -737,
       0,     0,    69,    70,    71,    72,   198,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   199,     0,   100,     0,     0,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
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
      84,     0,     0,     0,     0,     0,   597,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   199,     0,   100,     0,     0,   598,
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
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,   853,     0,     0,    98,     0,   199,     0,   100,
       0,     0,  1018,     0,   101,     5,     6,     0,     8,     9,
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
       0,    87,    88,    89,    90,    91,     0,  -403,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     199,     0,   100,  -403,     0,     5,     6,   101,     8,     9,
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
       0,     0,    94,    95,    96,  -534,     0,     0,    98,     0,
     199,     0,   100,     0,     0,  -534,     0,   101,     5,     6,
       0,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   192,     0,     0,
      21,     0,     0,     0,     0,     0,     0,   270,    27,     0,
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
       0,    98,     0,    99,     0,   100,     0,     0,     5,     6,
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
       0,     0,     0,     0,     0,   597,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   199,     0,   100,     0,     0,     5,     6,
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
       0,    98,     0,   199,     0,   100,     0,     0,   598,     0,
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
      96,     0,     0,     0,    98,     0,   199,   797,   100,     0,
       0,     5,     6,   101,     8,     9,    10,    11,    12,     0,
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
       0,  1028,     0,   101,     5,     6,     0,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   192,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   193,     0,     0,     0,
      31,     0,     0,     0,     0,   194,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   195,     0,   196,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   197,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   198,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   199,
       0,   100,     0,     0,     5,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   192,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,   290,     0,   193,     0,     0,     0,
      31,     0,     0,     0,     0,   194,    34,     0,     0,     0,
     292,     0,     0,    38,     0,   195,     0,   196,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   197,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   297,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   199,
       0,   100,     0,     0,     5,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   192,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   193,     0,     0,     0,
      31,     0,     0,     0,     0,  1065,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   195,     0,   196,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   197,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   198,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   199,
       0,   100,     0,     0,     5,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   192,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   193,     0,     0,     0,
      31,     0,     0,     0,     0,  1210,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   195,     0,   196,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   197,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   198,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   199,
       0,   100,     0,     0,     5,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   192,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   193,     0,     0,     0,
      31,     0,     0,     0,     0,  1211,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   195,     0,   196,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   197,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   198,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   199,
       0,   100,     0,     0,     5,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   192,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   193,     0,     0,     0,
      31,     0,     0,     0,     0,  1213,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   195,     0,   196,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   197,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   198,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   199,
       0,   100,     0,     0,     5,     6,   101,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   192,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    27,     0,   193,     0,     0,     0,
      31,     0,     0,     0,     0,  1214,    34,     0,     0,     0,
      36,     0,     0,    38,     0,   195,     0,   196,     0,     0,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   197,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,   198,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       6,     0,     8,     9,    10,    11,    12,     0,    93,     0,
      13,    94,    95,    96,    17,     0,     0,    98,   192,   199,
    -207,   100,     0,   289,     0,     0,   101,     0,     0,     0,
       0,   193,     0,     0,     0,     0,  -207,  -207,  -207,     0,
     194,     0,     0,   291,     0,     0,     0,   293,     0,     0,
       0,     0,   196,     0,     0,    43,     0,     0,     0,     0,
      47,     0,    48,    49,     0,     0,     0,     0,     0,     0,
       0,   294,     0,    54,   295,    56,    57,    58,     0,     0,
       0,     0,    61,     0,   197,     0,   296,     0,     0,     0,
       0,     0,    69,    70,     0,    72,     0,     0,     0,     0,
       0,     0,     0,     0,   298,     0,     0,    82,     0,   299,
       0,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   199,     0,   100,  -334,  -207,     0,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,     0,    17,     0,     0,     0,   192,     0,
    -207,     0,     0,   289,     0,     0,     0,     0,     0,     0,
       0,   193,     0,     0,     0,     0,  -207,  -207,  -207,     0,
     194,     0,     0,   291,     0,     0,     0,   293,     0,     0,
       0,     0,   196,     0,     0,    43,     0,     0,     0,     0,
      47,     0,    48,    49,     0,     0,     0,     0,     0,     0,
       0,   294,     0,    54,   295,    56,    57,    58,     0,     0,
       0,     0,    61,     0,   197,     0,   296,     0,     0,     0,
       0,     0,    69,    70,     0,    72,     0,     0,     0,     0,
       0,     0,     0,     0,   298,     0,     0,    82,     0,   299,
       0,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   626,     0,   627,     0,    87,    88,    89,    90,    91,
       0,     0,   628,     0,    92,     0,     0,   629,   228,   630,
     631,     0,     0,    93,     0,   632,    94,    95,    96,     0,
       0,     0,    98,   231,   199,   193,   100,     0,  -207,   232,
       0,   101,     0,     0,     0,   633,     0,     0,     0,   234,
       0,     0,   634,     0,   635,   636,     0,     0,     0,     0,
       0,     0,   637,     0,     0,     0,     0,     0,     0,   238,
     638,     0,     0,     0,     0,     0,     0,   240,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   639,     0,     0,
       0,     0,     0,     0,     0,     0,   242,     0,   640,     0,
     244,     0,   641,     0,     0,   246,     0,     0,     0,   642,
       0,   248,     0,     0,   643,     0,     0,     0,     0,     0,
       0,     0,     0,   504,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,     0,   471,   472,     0,   644,
     645,   646,   647,   648,   649,     0,   650,     0,     0,     0,
     651,   652,   653,   654,   655,   656,   657,   658,   659,   660,
       0,   661,     0,     0,   662,   663,   664,   665,     5,     6,
     666,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   192,     0,  -207,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     193,     0,     0,     0,    31,  -207,  -207,  -207,     0,   194,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   195,
       0,   196,     0,     0,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   197,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,   198,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,    95,    96,     0,     0,
       0,    98,     0,   199,     0,   100,     0,  -207,     5,     6,
       0,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   192,     0,  -207,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     193,     0,     0,     0,    31,  -207,  -207,  -207,     0,   194,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   195,
       0,   196,     0,     0,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   197,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,   198,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,     0,    89,     0,     0,     5,
       6,     0,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,    96,   192,     0,
    -207,    21,     0,   199,     0,   100,     0,  -207,     0,    27,
       0,   193,     0,     0,     0,    31,  -207,  -207,  -207,     0,
     194,    34,     0,     0,     0,    36,     0,     0,    38,     0,
     195,     0,   196,     0,     0,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   197,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,   198,   692,    74,     0,
       0,    76,     0,     0,     0,    80,   628,    82,     0,     0,
      84,   980,   981,   630,   982,     0,     0,     0,     0,   983,
       0,     0,     0,     0,     0,     0,     0,   694,     0,     0,
       0,     0,     0,     0,     0,    87,     0,    89,     0,   984,
       0,     0,     0,   985,     0,     0,   986,     0,   195,   636,
       0,     0,     0,     0,     0,     0,   695,     0,    96,     0,
       0,     0,     0,     0,   987,     0,   100,     0,  -207,     0,
       0,   988,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   989,     0,     0,     0,     0,     0,     0,     0,     0,
     990,     0,   991,     0,   992,     0,   696,     0,     0,     0,
       0,     0,   697,   993,     0,   994,     0,     0,   995,     0,
       0,     0,     0,     0,     0,     0,     0,   504,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   468,   469,     0,
     471,   472,     0,   644,   645,   646,   647,   648,   649,     0,
     650,     0,     0,     0,   651,   652,   653,   654,   655,   656,
     657,   658,   659,   660,   226,   661,     0,     0,   662,   663,
     664,   665,   996,   628,     0,     0,     0,     0,   227,   228,
     630,   229,     0,     0,     0,     0,   230,     0,     0,     0,
       0,     0,     0,     0,   231,     0,     0,     0,     0,     0,
     232,     0,     0,     0,     0,     0,   233,     0,     0,     0,
     234,     0,     0,   235,     0,   236,   636,     0,     0,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
     238,   239,     0,     0,     0,     0,     0,     0,   240,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   241,     0,
       0,     0,     0,     0,     0,   428,     0,   242,     0,   243,
       0,   244,   429,   245,     0,     0,   246,     0,     0,     0,
     247,     0,   248,     0,   430,   249,     0,     0,     0,     0,
       0,     0,     0,     0,   504,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   468,   469,     0,   471,   472,     0,
     644,   645,   646,   647,   648,   649,     0,   650,     0,     0,
       0,   651,   652,   653,   654,   655,   656,   657,   658,   659,
     660,     0,   661,     0,     0,   662,   663,   664,   665,     0,
       0,     0,   431,   428,     0,     0,   432,     0,     0,     0,
     429,     0,     0,     0,   503,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   504,   459,   460,   461,
     462,   463,   464,   465,   466,   467,   468,   469,     0,   471,
     472,     0,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,     0,   454,   455,   456,
     431,   359,     0,   428,   432,     0,     0,     0,   457,     0,
     429,     0,  1186,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,   504,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   468,   469,     0,   471,   472,     0,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,     0,   454,   455,   456,     0,   359,
     431,   428,     0,     0,   432,     0,   457,     0,   429,     0,
       0,     0,  1245,     0,     0,     0,     0,     0,     0,     0,
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
     453,     0,     0,   454,   455,   456,   431,   359,     0,     0,
     432,     0,     0,     0,   457,     0,     0,   428,  1248,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,   866,     0,   433,
     504,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   867,   471,   472,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,     0,
       0,   454,   455,   456,     0,   359,     0,     0,     0,     0,
       0,     0,   457,     0,   431,   428,     0,     0,   432,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,   270,     0,     0,     0,     0,
       0,     0,     0,   210,     0,     0,     0,   433,     0,     0,
     975,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,     0,     0,   454,
     455,   456,   431,   359,     0,     0,   432,     0,     0,     0,
     457,     0,     0,     0,     0,   428,   820,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,   210,     0,     0,   430,   433,     0,     0,     0,   821,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   822,     0,   454,   455,   456,
       0,   359,     0,     0,     0,     0,     0,     0,   457,     0,
       0,     0,   431,   428,     0,     0,   432,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,  1129,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,     0,   454,   455,   456,
     431,     0,     0,   428,   432,     0,     0,     0,   457,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,   270,     0,     0,     0,     0,     0,   210,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,     0,   454,   455,   456,     0,   359,
     431,   428,     0,     0,   432,     0,   457,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,   270,     0,     0,     0,     0,     0,     0,     0,   210,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,   428,     0,     0,     0,     0,     0,     0,   429,     0,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
     430,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,     0,   454,   455,   456,   431,   359,
       0,   428,   432,     0,     0,     0,   457,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,   908,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,   909,     0,   431,     0,
       0,     0,   432,     0,     0,     0,     0,     0,   434,   435,
     940,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   433,     0,   454,   455,   456,     0,   359,   431,     0,
       0,     0,   432,     0,   457,     0,     0,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   433,     0,   454,   455,   456,     0,   359,     0,     0,
       0,     0,     0,     0,   457,     0,     0,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   428,     0,   454,   455,   456,     0,     0,   429,     0,
       0,     0,     0,     0,   457,     0,     0,     7,     0,     0,
     430,     0,     0,     0,     0,  1103,     0,     0,     0,     0,
       0,     0,     0,     0,   354,     0,     0,     0,    22,    23,
       0,     0,     0,     0,     0,     0,   428,   355,     0,    29,
     356,     0,     0,   429,     0,    32,     0,     0,     0,     0,
       0,  1104,    37,     0,     0,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431,     0,
       0,     0,   432,   357,     0,     0,     0,     0,    53,     0,
      55,   428,     0,     0,  1041,     0,     0,  1042,   429,     0,
     358,     0,    64,     0,     0,     0,     0,     0,     0,     0,
     430,   433,     0,     0,     0,     0,     0,     0,     0,     0,
      79,     0,    81,   431,     0,    83,     0,   432,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,   210,   454,   455,   456,   433,     0,     0,     0,
       0,     0,     0,     0,   457,     0,     0,     0,   431,     0,
       0,     0,   432,   434,   435,    97,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,   210,   454,   455,
     456,   433,     0,     0,     0,   428,   525,  1120,     0,   457,
       0,     0,   429,     0,     0,     0,     0,     0,   434,   435,
       0,   436,   437,   438,   430,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,     0,   454,   455,   456,   428,     0,     0,     0,
       0,     0,  1169,   429,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   431,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,   428,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,     0,   433,     0,     0,     0,     0,
       0,     0,     0,   431,     0,     0,     0,   432,     0,     0,
       0,     0,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,   433,   454,   455,   456,
       0,     0,     0,     0,     0,     0,     0,     0,   457,     0,
       0,   431,     0,   434,   435,   432,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   428,     0,   454,   455,
     456,     0,   548,   429,   433,     0,     0,     0,     0,   457,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
       0,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   428,   552,   454,   455,   456,     0,
       0,   429,     0,     0,     0,     0,     0,   457,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   431,     0,     0,     0,   432,     0,     0,
       0,     0,     0,     0,     0,   726,     0,     0,     0,   428,
     816,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,     0,   434,   435,   432,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,     0,   454,   455,
     456,     0,     0,     0,   433,     0,     0,     0,     0,   457,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
     432,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   428,     0,   454,   455,   456,   433,
       0,   429,     0,   798,     0,     0,     0,   457,     0,     0,
       0,     0,     0,   430,     0,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   817,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   428,
       0,   454,   455,   456,     0,     0,   429,     0,     0,     0,
       0,     0,   457,     0,     0,     0,     0,     0,   430,     0,
       0,   938,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,     0,     0,     0,   432,     0,     0,     0,   428,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,   860,   433,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   431,     0,     0,     0,
     432,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,     0,     0,   454,   455,   456,   433,
       0,     0,     0,     0,     0,     0,   431,   457,     0,     0,
     432,     0,     0,     0,     0,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   433,
       0,   454,   455,   456,     0,     0,     0,   428,     0,     0,
       0,     0,   457,     0,   429,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,   430,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,     0,
       0,   454,   455,   456,     0,     0,     0,     0,   950,     0,
       0,     0,   457,     0,     0,   428,  1005,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   431,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,   428,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   431,     0,   434,   435,   432,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,     0,     0,   454,
     455,   456,     0,     0,     0,   433,   951,     0,     0,     0,
     457,     0,   431,     0,     0,     0,   432,     0,     0,     0,
       0,     0,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   433,     0,   454,   455,   456,
       0,     0,     0,   428,  1102,     0,     0,     0,   457,     0,
     429,     0,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,   430,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,     0,   454,   455,   456,
       0,     0,     0,   428,  1057,     0,     0,     0,   457,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,   428,     0,     0,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,   433,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,   432,     0,     0,     0,     0,     0,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   433,     0,   454,   455,   456,     0,     0,
       0,     0,     0,     0,     0,     0,   457,     0,   431,     0,
     434,   435,   432,   436,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,  1114,   454,   455,   456,   428,     0,
    1124,   433,     0,     0,     0,   429,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,     0,   454,   455,   456,   428,     0,     0,     0,
       0,     0,     0,   429,   457,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   430,  1155,     0,     0,     0,
       0,     0,     0,     0,     0,   431,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,   428,  1174,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   431,     0,   434,   435,   432,   436,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,     0,     0,
     454,   455,   456,     0,     0,     0,   433,     0,     0,  1152,
       0,   457,     0,   431,   428,     0,     0,   432,     0,     0,
       0,   429,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,   430,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   433,     0,   454,   455,
     456,     0,     0,     0,   428,     0,     0,     0,     0,   457,
       0,   429,  1205,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,   430,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,     0,   454,   455,
     456,   431,     0,     0,   428,   432,     0,     0,     0,   457,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,     0,     0,
       0,   431,     0,     0,     0,   432,     0,     0,     0,     0,
       0,   434,   435,  1212,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   433,     0,   454,   455,   456,     0,
       0,   431,   428,     0,     0,   432,     0,   457,     0,   429,
       0,   434,   435,  1249,   436,   437,   438,     0,   439,   440,
     441,   430,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   433,     0,   454,   455,   456,     0,
       0,     0,   428,     0,     0,     0,     0,   457,     0,   429,
       0,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,   430,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,     0,     0,   454,   455,   456,   431,
       0,     0,   428,   432,     0,     0,     0,   457,     0,   429,
       0,  1250,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,   432,     0,     0,     0,     0,     0,   434,
     435,  1251,   436,   437,   438,     0,   439,   440,   441,   430,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   433,     0,   454,   455,   456,     0,     0,   431,
     428,     0,     0,   432,     0,   457,     0,   429,     0,   434,
     435,  1252,   436,   437,   438,     0,   439,   440,   441,   430,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   433,     0,   454,   455,   456,   431,     0,     0,
     428,   432,     0,     0,     0,   457,     0,   429,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,   430,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,     0,   454,   455,   456,   431,     0,     0,
       0,   432,     0,     0,     0,   457,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   445,     0,   447,   448,   449,   450,   451,     0,   453,
     433,     0,   454,   455,   456,     0,     0,   928,     0,     0,
       0,   432,     0,   457,     0,     0,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     433,     0,   454,   455,   456,     0,     0,     0,     0,     0,
       0,     0,     0,   457,     0,     0,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,   430,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,   354,   454,   455,   456,    22,    23,     0,     0,     0,
       0,     0,     0,   457,   355,     0,    29,   356,     0,     0,
       0,     0,    32,     0,     0,     0,     0,     0,     0,    37,
       0,     0,  -330,     0,     0,     0,    41,     0,     0,     0,
       0,     0,     0,     0,   -68,   431,     0,     0,     0,   432,
     357,     0,  -330,     0,     0,    53,     0,    55,     0,     0,
       0,    59,  -330,   -68,    60,     0,     0,   358,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,    79,     0,    81,
       0,     0,    83,     0,     0,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,   445,
       0,   447,   448,   449,   450,   451,     0,   453,     0,     0,
     454,   455,   456,     0,   613,     0,   226,     0,     7,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
     227,   228,    97,   229,     0,     0,     0,   359,   230,     0,
       0,     0,     0,     0,     0,     0,   231,     0,     0,     0,
       0,     0,   232,     0,     0,     0,     0,     0,   233,     0,
       0,     0,   234,     0,     0,   235,     0,   236,     0,     0,
       0,     0,     0,     0,   226,   237,     0,     0,     0,     0,
       0,     0,   238,   239,     0,     0,     0,     0,   227,   228,
     240,   229,     0,     0,     0,     0,   230,     0,    23,     0,
     241,     0,     0,     0,   231,     0,     0,     0,     0,   242,
     232,   243,     0,   244,     0,   245,   233,     0,   246,     0,
     234,     0,   247,   235,   248,   236,     0,   249,     0,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
     238,   239,     0,     0,     0,     0,     0,     0,   240,    55,
       0,     0,     0,     0,     0,     0,     0,     0,   241,     0,
       0,    64,     0,     0,   226,     0,     7,   242,     0,   243,
       0,   244,     0,   245,     0,     0,   246,     0,   227,   228,
     247,   229,   248,     0,    83,   249,   230,     0,     0,     0,
       0,     0,     0,     0,   231,     0,     0,     0,     0,     0,
     232,     0,     0,     0,     0,     0,   233,     0,     0,     0,
     234,     0,     0,   235,     0,   236,     0,     0,     0,     0,
       0,     0,  -308,   237,     0,     0,     0,     0,     0,     0,
     238,   239,     0,     0,     0,     0,  -308,  -308,   240,  -308,
       0,     0,     0,     0,  -308,     0,     0,     0,   241,     0,
       0,     0,  -308,     0,     0,     0,     0,   242,  -308,   243,
       0,   244,     0,   245,  -308,     0,   246,     0,  -308,     0,
     247,  -308,   248,  -308,     0,   249,     0,     0,     0,     0,
     226,  -308,     0,     0,     0,     0,     0,     0,  -308,  -308,
       0,     0,     0,     0,   227,   228,  -308,   229,     0,     0,
       0,     0,   230,     0,     0,     0,  -308,     0,     0,     0,
     231,     0,     0,     0,     0,  -308,   232,  -308,     0,  -308,
       0,  -308,   233,     0,  -308,     0,   234,     0,  -308,   235,
    -308,   236,     0,  -308,     0,     0,     0,     0,  -309,   237,
       0,     0,     0,     0,     0,     0,   238,   239,     0,     0,
       0,     0,  -309,  -309,   240,  -309,     0,     0,     0,     0,
    -309,     0,     0,     0,   241,     0,     0,     0,  -309,     0,
       0,     0,     0,   242,  -309,   243,     0,   244,     0,   245,
    -309,     0,   246,     0,  -309,     0,   247,  -309,   248,  -309,
       0,   249,     0,     0,     0,     0,     0,  -309,     0,     0,
       0,     0,     0,     0,  -309,  -309,     0,     0,     0,     0,
       0,     0,  -309,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -309,     0,     0,     0,     0,     0,     0,     0,
       0,  -309,     0,  -309,     0,  -309,     0,  -309,     0,     0,
    -309,     0,     0,     0,  -309,     0,  -309,     0,     0,  -309
};

static const yytype_int16 yycheck[] =
{
       2,     2,   162,    61,   261,    66,   178,     2,   109,   546,
     614,   419,    26,   397,   486,    61,   584,   573,   510,   219,
     621,   785,   887,    25,    85,   387,   669,   602,   671,   286,
      25,    66,   822,   130,   875,   377,   541,   225,   817,   100,
     779,     1,   953,     1,   200,     1,     5,   840,    56,   517,
      85,    65,    32,    47,    47,    32,    32,    32,   765,     3,
      32,    46,    32,    32,    32,   100,    47,    56,   325,   431,
     432,    73,    22,   261,    47,    77,    78,   118,    73,     3,
     101,   145,    77,    78,    35,   257,   145,   123,    74,   163,
     168,   116,   170,     1,    32,   512,   170,    99,   286,    84,
       2,   103,   145,     0,    99,   599,   600,   171,   103,    90,
      54,   605,   606,   149,   173,   477,   478,    24,    74,     1,
     145,   483,   539,    25,   118,   163,   169,   112,    24,   115,
      54,     2,   173,    40,    41,    42,    32,   325,    88,    47,
      56,   121,   118,   118,    40,    41,    42,   168,    24,   118,
      32,   145,   145,   621,    25,   118,    32,   108,   102,   115,
     110,    56,   170,   123,    40,    41,    42,  1008,   145,   163,
     163,    73,   145,   145,   108,    77,    78,  1088,    60,   173,
     173,   170,   946,   169,   144,   145,   145,    27,   168,   953,
     225,   168,   168,   168,    56,   751,   168,    99,   168,   168,
     168,   103,    73,  1044,  1045,   361,    77,    78,   570,   211,
     169,   171,   170,   169,  1007,   123,   211,   956,   475,   476,
     927,   117,   104,    47,  1003,   163,   261,    60,    99,    56,
     168,    47,   103,     3,   116,   727,   144,   145,    78,    31,
      24,   117,    47,   118,   118,   587,   145,   419,    32,     3,
      90,   286,   440,   821,    90,   163,    40,    41,    42,   139,
      47,    18,   144,   265,   163,   172,   170,   147,   270,  1110,
     265,    11,     3,   113,    90,   270,   172,   852,   783,   376,
      44,   163,  1147,   116,    54,    90,   168,   475,   476,    20,
     325,   145,  1133,   550,   118,    53,   172,    89,   173,   173,
      54,    93,    33,    90,    68,    56,  1070,   163,    46,   211,
     804,   805,   806,   169,  1104,    73,   145,   171,    46,   145,
      77,   145,   510,    54,  1088,    83,   163,    65,   170,   529,
     490,    62,   102,   117,   145,   102,   173,    65,    92,    96,
     211,    27,   171,   345,   345,   171,    84,   139,   102,   173,
     345,    56,   144,   954,   111,   147,    84,   359,   170,   151,
     171,    47,   550,   265,   359,    51,   368,   145,   270,   145,
     145,   102,   386,   368,   112,    50,   145,   108,    53,   381,
     394,   873,    56,    27,   112,   573,   381,     9,   172,    75,
    1140,   591,    78,   171,   265,   171,   171,   145,    73,   270,
     145,    76,   171,    47,    90,    56,   145,    51,    83,   470,
      89,   145,   473,   474,    93,  1058,  1059,   419,   910,   145,
     976,    56,   108,   171,   145,    56,   171,   113,   996,   123,
      56,    75,   171,   108,    78,   470,    80,   171,   473,   474,
     475,   476,   914,   345,  1048,   171,    90,   145,   169,  1199,
    1200,  1201,  1202,  1203,  1204,     2,   150,   359,   152,   153,
     163,   155,   156,   145,   108,    99,   368,   145,   482,   113,
     173,   169,   145,    56,   345,   510,   145,   145,    25,   381,
     538,   163,   145,   891,     5,   163,   145,   163,   359,   851,
     163,   875,   538,   169,   163,   163,   145,   368,   145,   501,
     163,   503,   563,   564,   163,   867,   501,   145,   503,   273,
     381,   275,   170,   614,   163,   550,   163,    89,   170,    56,
     172,    93,  1016,  1017,   145,   163,    73,   171,   563,   564,
      77,    78,   145,  1137,   163,  1103,    31,    27,   573,   727,
    1034,  1035,   163,   905,   173,    27,   147,   909,    82,   163,
     163,    85,    99,    77,    78,   569,   103,    47,   322,   173,
      89,    51,    56,   751,    93,    47,   928,   929,    56,    51,
      89,   170,   139,   172,    93,   577,   577,   144,    56,    89,
     147,   168,   577,    93,   151,    75,    56,   601,    78,   144,
      80,    89,   168,    75,    89,    93,    78,   163,    93,   501,
      90,   503,  1096,  1097,  1098,  1099,    77,    78,    90,  1146,
     170,   973,   172,   975,   170,   163,   172,   777,   108,   170,
     170,   172,   172,   113,  1008,   163,   108,     2,   816,   817,
     501,   113,   503,   287,   288,    31,   563,   564,   163,   162,
      56,   163,   168,   163,   139,   140,   163,   142,   143,   144,
      25,   146,   147,   148,   163,   163,   151,    84,   163,   170,
    1044,   145,   157,   158,   211,  1049,   161,   170,   170,   164,
     165,   166,   171,   170,   163,   577,    31,   144,   163,   145,
     175,     9,   842,     3,   171,   873,    65,   170,   749,    78,
     123,   168,   727,    89,   163,   147,    68,    93,    73,   123,
     145,   123,    77,    78,   123,   170,   577,   768,   171,    34,
      34,    65,   171,   123,   749,   171,   751,   171,   265,   891,
      21,   144,   910,   270,    99,   168,  1110,   117,   103,   170,
     145,   169,   123,   768,    89,   104,   170,   163,    93,   147,
     147,   147,   744,   139,   140,   147,   142,   143,   144,   147,
     146,   147,   147,   755,   147,   151,   758,   139,   163,   147,
     755,   157,   158,   758,   147,   161,   147,   147,   164,   165,
     166,   147,   171,   147,   147,    32,   147,   163,   163,   173,
      34,    34,   144,   170,   139,   140,   788,   788,   976,   144,
     168,   146,   147,   788,   171,    27,   151,   145,   345,   169,
     163,    47,   145,   158,   171,   173,   171,   173,   169,   164,
     165,   166,   359,   173,    34,    47,   163,   875,   163,    51,
     170,   368,   144,   163,  1081,  1082,   170,   145,    32,   887,
     171,   595,   596,   847,   381,   173,   211,    34,   873,   171,
     173,   605,   606,    75,   173,   171,    78,   849,    80,   163,
     171,   163,   854,   755,   849,   162,   758,   162,    90,   854,
     145,   863,   572,   108,   866,   976,   814,   368,   863,    24,
     361,   866,   385,   870,    31,   910,   108,   334,   969,   379,
     523,   113,   538,  1147,   755,   891,   788,   758,   300,   537,
     265,  1146,   955,  1081,  1082,   270,   260,  1205,  1198,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   675,   841,   525,   788,   128,   764,
     520,    20,   914,    -1,    -1,    -1,   928,   929,    -1,    -1,
      -1,    -1,    89,   935,    -1,   937,    93,    -1,   940,    -1,
     935,   976,   937,   167,  1045,   940,   170,   849,   172,    -1,
    1008,   965,   854,  1125,   501,    -1,   503,    -1,    -1,    -1,
      -1,   863,   964,    -1,   866,    -1,    -1,    -1,    -1,   964,
     345,    -1,   974,    -1,    -1,    -1,    -1,    -1,   849,   974,
      -1,    -1,   139,   854,   359,    -1,    -1,   144,    -1,   146,
     147,    -1,   863,   368,   151,   866,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,    -1,     2,   381,   164,   165,   166,
      -1,    -1,    -1,  1015,    -1,    -1,    -1,    -1,    -1,    -1,
    1015,    -1,  1024,    -1,    -1,    -1,    -1,    -1,    25,  1024,
     577,    -1,  1133,   935,    -1,   937,  1137,    -1,   940,    -1,
      -1,   805,   806,    -1,    -1,    -1,  1081,  1082,    -1,    -1,
      -1,  1223,  1110,  1225,    -1,    -1,  1228,  1229,    -1,    -1,
      -1,    -1,   964,    -1,   935,    -1,   937,    -1,    -1,   940,
      -1,    -1,   974,    -1,    -1,  1077,    73,    -1,    -1,    -1,
      77,    78,  1077,    -1,    -1,    -1,    -1,    -1,    -1,  1147,
      -1,    -1,    -1,   964,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   865,    99,   974,    -1,   869,   103,    -1,    -1,    -1,
      -1,    -1,    -1,  1015,    -1,    -1,  1118,    -1,  1120,    -1,
      -1,    -1,  1024,  1118,  1126,  1120,   501,  1129,   503,     2,
      -1,  1126,    -1,    -1,  1129,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1015,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,  1024,    -1,    -1,  1158,    -1,    -1,    -1,
      -1,    31,  1164,  1158,    -1,    -1,  1168,  1169,    -1,  1164,
      -1,    -1,    -1,  1168,  1169,  1077,    -1,    -1,  1180,    -1,
      -1,    -1,  1184,    -1,  1186,  1180,    -1,    -1,    -1,  1184,
      -1,  1186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    -1,   577,    -1,    77,    78,  1077,    -1,   755,    -1,
      -1,   758,    -1,    -1,   211,    -1,  1118,    -1,  1120,    89,
      -1,    -1,    -1,    93,  1126,    -1,    99,  1129,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   788,    -1,  1245,  1246,  1247,  1248,  1118,    -1,  1120,
    1245,  1246,  1247,  1248,    -1,  1126,  1158,    -1,  1129,    -1,
      -1,    -1,  1164,    -1,    -1,    -1,  1168,  1169,   265,   139,
    1034,  1035,    -1,   270,   144,    -1,   146,   147,  1180,    -1,
      -1,   151,  1184,    -1,  1186,    -1,    -1,  1158,   158,    -1,
      -1,    -1,    -1,  1164,    -1,    -1,   166,  1168,  1169,    -1,
      -1,     2,   849,    -1,    -1,    -1,    -1,   854,    -1,  1180,
      -1,    -1,    -1,  1184,    -1,  1186,   863,    -1,    -1,   866,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,  1092,    -1,
      -1,    -1,    -1,    -1,  1098,  1099,    -1,    -1,   211,    -1,
      -1,    -1,    -1,  1245,  1246,  1247,  1248,    -1,   345,    -1,
      -1,  1115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   359,    -1,  1128,    -1,    -1,  1131,    -1,    -1,
      -1,   368,    73,    -1,  1245,  1246,  1247,  1248,    -1,    -1,
     755,    -1,    -1,   758,   381,    -1,    -1,    -1,   935,    -1,
     937,    -1,   265,   940,    -1,    -1,    -1,   270,    99,    -1,
      -1,    -1,   103,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,   788,     1,    -1,     3,   964,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,   974,    36,    -1,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    72,    52,    -1,    54,  1015,    -1,
      -1,    -1,   345,    -1,   849,    62,    -1,  1024,    -1,   854,
      -1,    89,    69,    70,    -1,    93,   359,    -1,   863,    -1,
      77,   866,    -1,    -1,    -1,   368,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,   501,    -1,   503,    -1,   381,    96,
     211,    98,    -1,   100,   122,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
    1077,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,   164,   165,   166,    -1,
     935,    -1,   937,    -1,   265,   940,     1,   175,    -1,   270,
       5,  1118,    -1,  1120,    -1,    -1,    -1,    -1,    -1,  1126,
     577,    -1,  1129,   170,    -1,    -1,    -1,    22,    -1,   964,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    31,   974,
      35,    -1,    37,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,  1158,    -1,    -1,    -1,    50,    -1,  1164,    53,    -1,
      -1,  1168,  1169,    -1,    -1,    -1,    -1,    -1,   501,    -1,
     503,    -1,    -1,  1180,    -1,    -1,    71,  1184,    73,  1186,
    1015,    76,    -1,    78,   345,    80,    -1,    82,    83,  1024,
      85,    -1,    -1,    88,    -1,    90,    89,    -1,   359,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,   368,    -1,    -1,
      -1,    -1,    -1,   108,    -1,   110,    -1,    -1,   113,    -1,
     381,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1245,  1246,
    1247,  1248,  1077,    -1,   577,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   163,    -1,
      73,   164,   165,   166,   169,     5,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1118,    -1,  1120,    -1,    -1,    -1,    -1,
      -1,  1126,    22,    -1,  1129,    -1,    26,    27,   755,    -1,
     103,   758,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    -1,  1158,    -1,    -1,    -1,    -1,    -1,  1164,
      -1,   788,    -1,  1168,  1169,    -1,    -1,    -1,    -1,    -1,
     501,    71,   503,    -1,    -1,  1180,    76,    -1,    78,  1184,
      80,  1186,    82,    -1,    -1,    85,    -1,    -1,    88,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,    22,    -1,    -1,    -1,
      26,    27,   849,    -1,    -1,    -1,    -1,   854,    -1,    35,
      -1,    37,    38,    -1,    -1,    -1,   863,    43,   211,   866,
    1245,  1246,  1247,  1248,    50,    -1,   577,    -1,    -1,    -1,
      -1,    -1,   755,    -1,    -1,   758,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,    71,    -1,    -1,    -1,   169,
      76,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    85,
      -1,    -1,    88,    -1,    90,   788,    -1,    -1,    -1,    -1,
      -1,    -1,   265,    -1,    -1,    -1,    -1,   270,    -1,    -1,
      -1,    -1,   108,     3,   110,    -1,    -1,   113,   935,    -1,
     937,    -1,    -1,   940,    -1,    -1,    -1,    17,    18,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    33,    -1,    -1,    -1,   964,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,   849,   974,    -1,    49,
      -1,   854,    52,    -1,    54,    -1,    -1,   163,    -1,    -1,
     863,    -1,    62,   866,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,  1015,    -1,
      89,    -1,    -1,    -1,    93,    -1,    96,  1024,    98,    -1,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,   381,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   755,    -1,    -1,   758,    -1,    -1,
      -1,    -1,   935,    -1,   937,    -1,    -1,   940,    -1,    -1,
     139,   140,    -1,    -1,   143,   144,    -1,   146,   147,    -1,
    1077,    -1,   151,    -1,    -1,    -1,    -1,   788,    -1,   158,
      -1,   964,    -1,    -1,    -1,   164,   165,   166,   168,    -1,
     170,   974,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1118,    -1,  1120,    -1,    -1,    -1,    -1,    -1,  1126,
      -1,    -1,  1129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1015,    13,    -1,    -1,    -1,    17,   849,    -1,
      -1,  1024,    -1,   854,    -1,    -1,    26,    -1,   501,    -1,
     503,  1158,   863,    33,    -1,   866,    -1,  1164,    -1,    -1,
      -1,  1168,  1169,    -1,    44,    -1,    46,    47,    -1,    -1,
      -1,    -1,    52,  1180,    54,    -1,    56,  1184,    -1,  1186,
      -1,    61,    -1,    -1,    -1,    65,    66,    67,    68,    69,
      -1,    -1,    -1,    73,  1077,    75,    -1,    -1,    -1,    -1,
       3,    -1,    82,    -1,    -1,    85,    -1,    87,    -1,    89,
      -1,    -1,    92,    93,    17,    95,    -1,    20,    -1,    99,
     100,   101,    25,    -1,   935,    -1,   937,    -1,    -1,   940,
      -1,    -1,    35,    -1,    -1,  1118,    -1,  1120,  1245,  1246,
    1247,  1248,    45,  1126,    -1,    -1,  1129,    -1,   128,    52,
     130,    54,    -1,   964,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    -1,    -1,   974,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1158,    -1,    -1,    -1,    -1,
      -1,  1164,    -1,    -1,    87,  1168,  1169,    -1,    -1,    -1,
      -1,   171,    -1,     3,    -1,    98,    -1,  1180,    -1,   102,
      -1,  1184,    -1,  1186,  1015,    -1,   109,    17,    18,    -1,
      20,   114,    -1,  1024,   194,    25,    -1,    -1,   198,   199,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,   225,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,  1245,  1246,  1247,  1248,  1077,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,   261,    -1,    -1,    -1,    -1,    96,    -1,    98,    -1,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,   111,   755,    -1,   114,   758,   286,  1118,    -1,  1120,
      -1,    -1,    -1,    -1,    -1,  1126,    -1,   297,  1129,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   312,   313,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   325,    -1,  1158,    -1,   329,
      -1,    -1,    -1,  1164,    -1,    -1,    -1,  1168,  1169,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,   347,    -1,  1180,
      -1,    -1,    -1,  1184,    -1,  1186,    -1,   357,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   369,
      -1,   371,    -1,    -1,    -1,    -1,   849,    -1,    -1,    -1,
      -1,   854,    -1,    -1,    -1,   385,   386,   387,    -1,    -1,
     863,    -1,    -1,   866,   394,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1245,  1246,  1247,  1248,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   428,   429,
     430,   431,   432,   433,    -1,   435,   436,   437,   438,   439,
      -1,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,    -1,    -1,
      -1,    -1,   935,    -1,   937,    -1,    -1,   940,    -1,    -1,
     470,    -1,    -1,   473,   474,   475,   476,   477,   478,    -1,
      -1,    -1,   482,   483,    -1,    -1,   486,    -1,    -1,    -1,
      -1,   964,   492,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   974,    -1,   503,    -1,    -1,   506,    -1,    -1,    -1,
     510,    -1,    -1,    -1,    -1,    -1,    -1,   517,    -1,   519,
      -1,    -1,    -1,   523,    -1,    44,   526,   527,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   538,    -1,
      -1,   541,  1015,    -1,    -1,    -1,    -1,    -1,    -1,    68,
     550,  1024,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,
     560,    -1,    28,   563,   564,    -1,    -1,    -1,    -1,   569,
     570,    -1,    -1,   573,    40,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   584,   585,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   598,    -1,
      -1,   601,    -1,    -1,  1077,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    -1,    -1,    -1,   136,    -1,    -1,
      -1,   621,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,  1118,    -1,  1120,    -1,    39,
      -1,    -1,    -1,  1126,    -1,    45,  1129,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,   666,    -1,    -1,    -1,
      -1,    -1,    62,   673,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,   148,    -1,    -1,  1158,    -1,    77,    -1,    -1,
      -1,  1164,    -1,    -1,   160,  1168,  1169,    87,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,  1180,    98,    -1,
     100,  1184,   102,  1186,    -1,   105,    -1,    -1,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,   726,   727,    -1,    -1,
     196,    -1,    -1,    -1,    -1,    -1,    -1,   256,   257,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   749,
      -1,   751,   752,   753,   273,    -1,   275,    -1,    -1,    -1,
      -1,    -1,    -1,   282,    -1,    -1,    -1,    -1,   768,    -1,
      -1,    -1,  1245,  1246,  1247,  1248,    -1,    -1,    -1,    -1,
     170,    -1,    -1,   783,    -1,    -1,    -1,   787,   254,   255,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     266,    -1,    -1,   322,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   814,    -1,    -1,    -1,   818,    -1,
     820,   821,   822,    -1,   824,    -1,   826,    -1,    -1,   829,
     830,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     306,    -1,    -1,   843,    -1,    -1,    -1,   847,    -1,    -1,
      -1,   851,   852,    -1,   854,    -1,    -1,    -1,    -1,    -1,
     860,   861,    -1,   863,    22,    -1,   866,   867,    26,    27,
      -1,    -1,    -1,   873,    -1,    -1,    -1,    35,   878,    37,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,   355,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   905,   906,   907,   908,   909,
     910,    -1,    -1,    71,   914,    -1,    -1,    -1,    76,    -1,
      78,    -1,    -1,    -1,    -1,   391,    -1,   393,   928,   929,
      88,   397,    90,    -1,    -1,    -1,    -1,    -1,   938,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,   110,   419,   954,   113,    17,    18,   958,    20,
      -1,    -1,    -1,    -1,    25,   965,    -1,    -1,    -1,    -1,
      -1,    -1,    33,   973,    -1,   975,   976,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    -1,   996,    -1,    -1,    -1,
      -1,    62,    -1,  1003,    -1,  1005,    -1,    -1,    69,    70,
      -1,    -1,  1012,    -1,    -1,    -1,    77,    -1,   484,    -1,
      -1,    -1,    -1,    -1,    -1,  1025,    87,    -1,    -1,    -1,
      -1,    -1,  1032,    -1,   553,    96,    -1,    98,    -1,   100,
      -1,   102,    -1,    -1,   105,    -1,   512,    -1,   109,    -1,
     111,  1051,    -1,   114,    -1,    -1,    -1,    -1,    -1,   525,
      -1,    -1,    -1,    -1,    -1,  1065,    -1,    -1,    -1,   535,
      -1,    -1,    -1,   539,    -1,    -1,   595,   596,    -1,    -1,
      -1,  1081,  1082,    -1,    -1,    -1,   605,   606,    -1,    -1,
      -1,    -1,    -1,    -1,  1094,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1102,  1103,  1104,    -1,    -1,    -1,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   583,  1118,    -1,
    1120,    -1,    -1,    -1,  1124,    -1,  1126,    -1,   594,  1129,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   614,    -1,
      -1,  1151,  1152,    -1,  1154,  1155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1174,    -1,    -1,  1177,    -1,    -1,
      -1,  1181,    -1,    -1,    -1,    -1,  1186,  1187,    -1,    -1,
    1190,    -1,    -1,    -1,    -1,    -1,    -1,  1197,     5,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   675,
    1210,  1211,  1212,  1213,  1214,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,
     759,    -1,    -1,    50,    -1,  1245,  1246,  1247,  1248,  1249,
    1250,  1251,  1252,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    71,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    80,    -1,    82,   795,    -1,    85,    -1,
      -1,    88,    -1,    90,    -1,    -1,   805,   806,    -1,    -1,
      -1,    -1,    -1,    -1,   760,    -1,    -1,    -1,   764,    -1,
      -1,   108,    -1,   110,    -1,    -1,   113,    -1,   774,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
      -1,   151,   152,   153,    -1,   155,   156,   157,   158,   795,
      -1,   161,    -1,    -1,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,   865,    -1,    -1,    -1,
     869,    -1,   871,    -1,    -1,    -1,   163,    -1,    -1,    -1,
      -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,     3,     4,   841,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    24,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,   870,    35,    -1,    -1,    -1,    39,
      40,    41,    42,    -1,    44,    45,    -1,    47,    -1,    49,
      -1,    51,    52,    -1,    54,   891,    56,    -1,    -1,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    77,    78,    79,
      -1,    81,    -1,   972,    -1,    -1,    86,    87,    88,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,   108,   109,
      -1,   111,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   969,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,  1034,  1035,    -1,   148,    -1,
      -1,    -1,     5,    -1,    -1,    -1,    -1,   157,    -1,    31,
     160,   161,   162,    -1,    -1,    -1,   166,    -1,   168,    22,
     170,   171,   172,    26,    27,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,  1092,    -1,    -1,    -1,    -1,    -1,  1098,
    1099,    -1,  1048,    -1,    -1,    -1,    -1,    89,    71,    -1,
      -1,    93,    -1,    76,    -1,    78,  1115,    80,    -1,    82,
      -1,    -1,    85,    -1,    -1,    88,  1125,    90,    -1,  1128,
      -1,    -1,  1131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,   108,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,  1166,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,  1137,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1223,    -1,  1225,    -1,    -1,  1228,
    1229,     0,     1,    -1,     3,     4,     5,     6,     7,     8,
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
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    18,    19,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    54,    -1,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,    -1,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    -1,
      -1,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
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
      -1,    -1,   166,    -1,   168,   169,   170,    -1,   172,    -1,
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
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
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
     170,     1,   172,     3,     4,   175,     6,     7,     8,     9,
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
      10,    -1,    12,    -1,    14,    -1,    -1,    17,    18,    19,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    55,    56,    -1,    -1,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,   137,    -1,   139,
     140,   141,   142,   143,   144,    -1,   146,    -1,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
     170,    -1,    -1,     3,     4,   175,     6,     7,     8,     9,
      10,    -1,    12,    -1,    14,    -1,    -1,    17,    18,    19,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    55,    56,    -1,    -1,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,   137,    -1,   139,
     140,   141,   142,   143,   144,    -1,   146,    -1,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,   164,   165,   166,   167,   168,    -1,
     170,    -1,    -1,    -1,    -1,   175,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    -1,
      17,    18,    -1,    20,    -1,    22,    23,    -1,    25,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    -1,    61,    62,    63,    64,    -1,    66,
      67,    -1,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,   100,    -1,   102,   103,    -1,   105,    -1,
      -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,    -1,     3,     4,   175,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      47,    -1,    49,    -1,    51,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      77,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,   108,   109,    -1,   111,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,    -1,     3,     4,   175,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    26,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,   111,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,    -1,     3,     4,   175,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    24,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    42,    -1,    44,    45,    -1,
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
      -1,   168,    -1,   170,    -1,   172,     3,     4,   175,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    24,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    42,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,
      -1,    -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,
      -1,   168,    -1,   170,    -1,   172,     3,     4,   175,     6,
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
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,   106,
     107,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,
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
      -1,   148,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   105,   106,   107,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,   171,    -1,     3,
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
      -1,   105,   106,   107,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,   171,    -1,     3,
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
      -1,   105,   106,   107,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,   171,    -1,     3,
       4,   175,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,   106,   107,    -1,   109,    -1,   111,    -1,    -1,
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
      -1,   105,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,   145,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,
      -1,    -1,   166,    -1,   168,   169,   170,    -1,    -1,     3,
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
      -1,   105,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,   145,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,
      -1,    -1,   166,    -1,   168,   169,   170,    -1,    -1,     3,
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
      -1,   105,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,   145,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,
      -1,    -1,   166,    -1,   168,   169,   170,    -1,    -1,     3,
       4,   175,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,   111,    -1,   113,
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
      -1,   105,   106,   107,    -1,   109,    -1,   111,    -1,    -1,
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
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    -1,    93,
      -1,    -1,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,
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
      -1,   105,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
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
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,   163,    -1,    -1,   166,    -1,   168,    -1,   170,
      -1,    -1,   173,    -1,   175,     3,     4,    -1,     6,     7,
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
      -1,   139,   140,   141,   142,   143,    -1,   145,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,   171,    -1,     3,     4,   175,     6,     7,
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
      -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,    -1,   173,    -1,   175,     3,     4,
      -1,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
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
      -1,   166,    -1,   168,    -1,   170,    -1,    -1,     3,     4,
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
      -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,    -1,     3,     4,
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
     162,    -1,    -1,    -1,   166,    -1,   168,   169,   170,    -1,
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
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
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
     109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
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
     109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
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
     109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
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
     109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
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
     109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
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
     109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
       4,    -1,     6,     7,     8,     9,    10,    -1,   157,    -1,
      14,   160,   161,   162,    18,    -1,    -1,   166,    22,   168,
      24,   170,    -1,    27,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      64,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    78,    79,    80,    81,    -1,    -1,
      -1,    -1,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,   171,   172,    -1,
       4,   175,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    18,    -1,    -1,    -1,    22,    -1,
      24,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    42,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      64,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    77,    78,    79,    80,    81,    -1,    -1,
      -1,    -1,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,     3,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    12,    -1,   148,    -1,    -1,    17,    18,    19,
      20,    -1,    -1,   157,    -1,    25,   160,   161,   162,    -1,
      -1,    -1,   166,    33,   168,    35,   170,    -1,   172,    39,
      -1,   175,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    -1,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,   136,   137,    -1,   139,
     140,   141,   142,   143,   144,    -1,   146,    -1,    -1,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
      -1,   161,    -1,    -1,   164,   165,   166,   167,     3,     4,
     170,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    24,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    42,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,    -1,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,    -1,   172,     3,     4,
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
     105,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,   141,    -1,    -1,     3,
       4,    -1,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,   162,    22,    -1,
      24,    25,    -1,   168,    -1,   170,    -1,   172,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    42,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,     3,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    12,   111,    -1,    -1,
     114,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,    -1,   141,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,   162,    -1,
      -1,    -1,    -1,    -1,    70,    -1,   170,    -1,   172,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    98,    -1,   100,    -1,   102,    -1,    -1,    -1,
      -1,    -1,   108,   109,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
     136,   137,    -1,   139,   140,   141,   142,   143,   144,    -1,
     146,    -1,    -1,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,     3,   161,    -1,    -1,   164,   165,
     166,   167,   168,    12,    -1,    -1,    -1,    -1,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    96,    -1,    98,
      -1,   100,    19,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,   111,    -1,    31,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,   136,   137,    -1,
     139,   140,   141,   142,   143,   144,    -1,   146,    -1,    -1,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,    -1,   161,    -1,    -1,   164,   165,   166,   167,    -1,
      -1,    -1,    89,    12,    -1,    -1,    93,    -1,    -1,    -1,
      19,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,    -1,   136,
     137,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
      89,   168,    -1,    12,    93,    -1,    -1,    -1,   175,    -1,
      19,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    -1,   136,   137,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    -1,   168,
      89,    12,    -1,    -1,    93,    -1,   175,    -1,    19,    -1,
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
     161,    -1,    -1,   164,   165,   166,    89,   168,    -1,    -1,
      93,    -1,    -1,    -1,   175,    -1,    -1,    12,   101,    -1,
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
     165,   166,    89,   168,    -1,    -1,    93,    -1,    -1,    -1,
     175,    -1,    -1,    -1,    -1,    12,    13,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    31,   122,    -1,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    72,    -1,   164,   165,   166,
      -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
      -1,    -1,    89,    12,    -1,    -1,    93,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
      89,    -1,    -1,    12,    93,    -1,    -1,    -1,   175,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    -1,   168,
      89,    12,    -1,    -1,    93,    -1,   175,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      31,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    89,   168,
      -1,    12,    93,    -1,    -1,    -1,   175,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    47,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,   139,   140,
     101,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   122,    -1,   164,   165,   166,    -1,   168,    89,    -1,
      -1,    -1,    93,    -1,   175,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   122,    -1,   164,   165,   166,    -1,   168,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    12,    -1,   164,   165,   166,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,     5,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    35,    -1,    37,
      38,    -1,    -1,    19,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    72,    50,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    71,    -1,    -1,    -1,    -1,    76,    -1,
      78,    12,    -1,    -1,    82,    -1,    -1,    85,    19,    -1,
      88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,   110,    89,    -1,   113,    -1,    93,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,   118,   164,   165,   166,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,   139,   140,   163,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,   118,   164,   165,
     166,   122,    -1,    -1,    -1,    12,    13,   173,    -1,   175,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    31,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    12,    -1,    -1,    -1,
      -1,    -1,   173,    19,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,   122,   164,   165,   166,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,
      -1,    89,    -1,   139,   140,    93,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    12,    -1,   164,   165,
     166,    -1,   168,    19,   122,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    12,   163,   164,   165,   166,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,    12,
      13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,   139,   140,    93,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    12,    -1,   164,   165,   166,   122,
      -1,    19,    -1,   171,    -1,    -1,    -1,   175,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    12,
      -1,   164,   165,   166,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,   121,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    89,   175,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   122,
      -1,   164,   165,   166,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,   175,    -1,    19,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    31,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,   175,    -1,    -1,    12,    13,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,   139,   140,    93,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,    -1,    -1,    -1,   122,   171,    -1,    -1,    -1,
     175,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   122,    -1,   164,   165,   166,
      -1,    -1,    -1,    12,    13,    -1,    -1,    -1,   175,    -1,
      19,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    31,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
      -1,    -1,    -1,    12,   171,    -1,    -1,    -1,   175,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   122,    -1,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    89,    -1,
     139,   140,    93,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,   163,   164,   165,   166,    12,    -1,
     121,   122,    -1,    -1,    -1,    19,   175,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,   139,   140,    93,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,    -1,    -1,    -1,   122,    -1,    -1,   173,
      -1,   175,    -1,    89,    12,    -1,    -1,    93,    -1,    -1,
      -1,    19,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    31,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   122,    -1,   164,   165,
     166,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,   175,
      -1,    19,    60,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    31,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,   175,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,   139,   140,   101,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   122,    -1,   164,   165,   166,    -1,
      -1,    89,    12,    -1,    -1,    93,    -1,   175,    -1,    19,
      -1,   139,   140,   101,   142,   143,   144,    -1,   146,   147,
     148,    31,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   122,    -1,   164,   165,   166,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,   175,    -1,    19,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    31,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    89,
      -1,    -1,    12,    93,    -1,    -1,    -1,   175,    -1,    19,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,   139,
     140,   101,   142,   143,   144,    -1,   146,   147,   148,    31,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   122,    -1,   164,   165,   166,    -1,    -1,    89,
      12,    -1,    -1,    93,    -1,   175,    -1,    19,    -1,   139,
     140,   101,   142,   143,   144,    -1,   146,   147,   148,    31,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   122,    -1,   164,   165,   166,    89,    -1,    -1,
      12,    93,    -1,    -1,    -1,   175,    -1,    19,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    31,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,   175,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,    -1,   155,   156,   157,   158,   159,    -1,   161,
     122,    -1,   164,   165,   166,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,   175,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     122,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    31,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,    22,   164,   165,   166,    26,    27,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    35,    -1,    37,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    89,    -1,    -1,    -1,    93,
      71,    -1,    73,    -1,    -1,    76,    -1,    78,    -1,    -1,
      -1,    82,    83,    84,    85,    -1,    -1,    88,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
      -1,   155,   156,   157,   158,   159,    -1,   161,    -1,    -1,
     164,   165,   166,    -1,     1,    -1,     3,    -1,     5,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,   163,    20,    -1,    -1,    -1,   168,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,     3,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    17,    18,
      77,    20,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      87,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    96,
      39,    98,    -1,   100,    -1,   102,    45,    -1,   105,    -1,
      49,    -1,   109,    52,   111,    54,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    90,    -1,    -1,     3,    -1,     5,    96,    -1,    98,
      -1,   100,    -1,   102,    -1,    -1,   105,    -1,    17,    18,
     109,    20,   111,    -1,   113,   114,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,     3,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    17,    18,    77,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    96,    39,    98,
      -1,   100,    -1,   102,    45,    -1,   105,    -1,    49,    -1,
     109,    52,   111,    54,    -1,   114,    -1,    -1,    -1,    -1,
       3,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    17,    18,    77,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    96,    39,    98,    -1,   100,
      -1,   102,    45,    -1,   105,    -1,    49,    -1,   109,    52,
     111,    54,    -1,   114,    -1,    -1,    -1,    -1,     3,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    17,    18,    77,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    87,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    96,    39,    98,    -1,   100,    -1,   102,
      45,    -1,   105,    -1,    49,    -1,   109,    52,   111,    54,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    98,    -1,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,   111,    -1,    -1,   114
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
     218,   337,   168,     1,   191,   218,   271,   274,    47,    90,
       1,   144,   331,   332,    47,    90,     1,     3,    12,    17,
      19,    20,    25,    45,    52,    54,    55,    62,    70,    87,
      98,   102,   109,   114,   139,   140,   141,   142,   143,   144,
     146,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   161,   164,   165,   166,   167,   170,   218,   290,   291,
     292,   293,   346,   123,   318,   145,   163,   163,   163,   367,
     367,   367,   242,   367,   242,   367,   367,   367,   367,   367,
     367,   367,     3,    20,    33,    62,   102,   108,   219,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,    68,   369,   369,   369,
     343,   343,   242,   367,   242,   367,   101,   170,   337,   242,
     367,   218,     1,    27,    47,    51,    75,    78,    90,   108,
     113,   171,   357,   358,   359,   367,   383,   324,   367,   123,
     171,   145,   123,   123,   184,    34,   184,   367,    34,   367,
      65,   171,   344,   327,   145,   171,   222,   367,   123,   328,
     367,   325,   261,   218,   319,   367,   367,   320,   171,   218,
     305,   279,   327,   144,   335,   336,   334,   284,    32,   203,
     308,     1,   259,   343,   171,    21,   255,   169,   171,   171,
     341,   369,   171,   341,   337,   242,   367,   194,   171,   196,
     197,   345,   169,   169,   145,   163,    13,   147,   145,   163,
      13,    36,    72,   218,   139,   140,   141,   142,   143,   157,
     161,   166,   206,   292,   293,   294,   367,   206,   208,   267,
     168,   236,   320,   117,   218,   223,   223,   170,   367,   173,
     356,    47,   145,   163,   173,   340,   356,   337,   173,   340,
     121,   145,   173,    32,   223,   356,    32,    47,   223,   356,
     145,   171,   163,   170,     1,   268,   169,   274,   123,   145,
     169,     1,   222,   329,   330,   104,   306,   170,   296,   367,
     139,   147,   296,   296,   367,   317,   170,   172,   163,   163,
     163,   163,   171,   173,   344,    47,   173,    32,    32,    47,
     170,   171,    47,    90,   145,   171,    17,    20,    25,    45,
      52,    62,    70,    87,    98,   109,   114,   346,    89,    89,
     163,   369,   345,   367,   367,    34,   184,    34,    34,   184,
     101,   204,   218,   171,   171,   326,   333,    68,   369,   218,
     171,   171,   324,   336,   144,   280,   171,   334,   149,   295,
     328,   367,   182,   169,    74,   115,   169,   260,   171,   170,
     204,   218,   256,    47,   173,    47,   145,   171,   212,   219,
      17,    18,    20,    25,    45,    49,    52,    70,    77,    87,
      96,    98,   100,   109,   111,   114,   168,   211,   294,   367,
     367,   206,   208,   145,   163,    13,   163,   168,   268,   316,
     324,   367,    47,   337,   184,   173,   242,   367,   173,   340,
     184,   367,   163,   173,   173,   145,   173,   367,   173,   367,
     367,   223,    44,   367,   242,   367,   223,   248,   204,   344,
     169,    82,    85,   169,   183,   191,   225,   367,   272,   273,
      60,   116,   310,   297,   298,   299,   300,   171,   291,   293,
     171,   173,   171,   242,   367,    44,   242,   367,   344,   358,
     333,   346,   346,   184,   184,   367,   184,    34,   163,   163,
     328,   170,   170,   163,   295,   328,   329,   306,   336,   367,
     184,   238,   337,   256,   144,   204,   242,   367,   242,   367,
     197,   206,    13,    36,    72,   163,   163,   294,   367,   367,
     268,   169,   163,   163,   163,   367,   171,   184,   173,   356,
     173,   356,   173,   184,   121,   367,    32,   223,   356,    32,
     223,   356,   171,   191,   225,   225,   274,   191,     3,    54,
      92,   102,   311,   312,   313,   367,   287,   145,   171,   296,
     296,   173,   173,   367,    32,    32,   171,   328,    34,   184,
     343,   343,   295,   328,    32,   223,   171,   367,   173,   173,
     169,   367,   206,   208,    13,   169,   223,    44,   184,   367,
     173,    44,   184,   367,   173,   367,   101,    44,   367,   223,
      44,   367,   223,   163,   225,   274,   313,   116,   145,   123,
     150,   152,   153,   155,   156,    60,   163,   307,   308,   299,
      44,    44,   101,    44,    44,   184,   171,   171,   184,   204,
     163,   163,   367,   367,   184,   367,   184,   367,   367,   367,
     367,   312,   313,   313,   313,   313,   313,   313,   311,   367,
     367,   367,   367,   162,   162,   101,   101,   101,   101,   101,
     101,   101,   101,   367,   367,   367,   367
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
     270,   271,   271,   272,   271,   271,   273,   271,   274,   274,
     275,   276,   276,   276,   277,   277,   278,   278,   279,   279,
     279,   280,   280,   281,   283,   282,   284,   282,   285,   287,
     286,   288,   288,   288,   288,   288,   289,   290,   290,   291,
     291,   291,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   293,   293,   293,
     293,   293,   293,   293,   293,   293,   293,   293,   293,   293,
     293,   294,   294,   295,   295,   296,   296,   297,   297,   298,
     298,   299,   300,   300,   300,   300,   300,   300,   301,   301,
     302,   302,   302,   302,   302,   302,   302,   302,   302,   302,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   304,   304,   304,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   306,   306,   307,   307,   308,
     308,   309,   310,   310,   310,   310,   310,   311,   311,   312,
     312,   312,   312,   312,   312,   312,   313,   313,   313,   314,
     315,   315,   316,   316,   317,   318,   318,   319,   319,   319,
     319,   319,   320,   322,   321,   323,   321,   321,   321,   324,
     324,   325,   325,   326,   326,   327,   327,   327,   328,   328,
     328,   329,   330,   330,   330,   331,   331,   332,   332,   333,
     333,   333,   333,   334,   334,   335,   336,   336,   337,   337,
     338,   338,   338,   339,   339,   340,   340,   341,   342,   342,
     343,   343,   344,   344,   345,   345,   346,   346,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   348,
     349,   349,   349,   349,   349,   349,   349,   350,   351,   351,
     351,   351,   351,   351,   351,   352,   353,   354,   354,   354,
     354,   354,   354,   355,   355,   356,   356,   356,   356,   357,
     357,   358,   358,   358,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   360,   360,   360,   360,   360,   361,
     361,   361,   361,   361,   362,   363,   363,   363,   363,   363,
     363,   364,   365,   366,   366,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   368,   368,   369,   369,   369,   370,   370,   370,   370,
     371,   371,   371,   371,   372,   372,   372,   373,   373,   373,
     373,   373,   373,   374,   374,   374,   374,   375,   375,   376,
     376,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   378,   378,   379,
     379,   379,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   380,   380,   380,   380,   380,   380,   380,   381,
     381,   381,   381,   382,   382,   382,   382,   383,   383,   383,
     383,   383,   383,   383
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
       2,     1,     2,     1,     2,     1,     2,     1,     1,     2,
       6,     6,     4,     4,     9,     9,     7,     2,     2,     3,
       4,     6,     6,     9,     9,     6,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     4,     3,     4,     2,     1,
       3,     4,     3,     3,     1,     1,     2,     1,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     2,
       8,     8,     9,     9,     4,     3,     3,     2,     2,     2,
       1,     3,     4,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     2,     2,     1,     4,     4,     4,     3,     3,     3,
       3,     5,     5,     3,     4,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     4,     4,
       5,     4,     5,     5,     6,     4,     5,     3,     5,     3,
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
#line 691 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 5895 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 695 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 5901 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 696 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 5907 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 703 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 5915 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 711 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 5923 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 715 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 5932 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 725 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 5940 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 730 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 5948 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 742 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)); }
#line 5954 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 744 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5960 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 745 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5966 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 746 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5972 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 747 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5978 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 748 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5984 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 749 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5990 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 750 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 5996 "bison-chpl-lib.cpp"
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
#line 6017 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: try_stmt  */
#line 768 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6023 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: return_stmt  */
#line 769 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6029 "bison-chpl-lib.cpp"
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
#line 6041 "bison-chpl-lib.cpp"
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
#line 6054 "bison-chpl-lib.cpp"
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
#line 6067 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 797 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6075 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 801 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6086 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 808 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6095 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 815 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6101 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 816 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6107 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 817 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6113 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 818 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6119 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 819 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6125 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 820 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6131 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 821 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6137 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 823 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6143 "bison-chpl-lib.cpp"
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
#line 6166 "bison-chpl-lib.cpp"
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
#line 6178 "bison-chpl-lib.cpp"
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
#line 6198 "bison-chpl-lib.cpp"
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
#line 6217 "bison-chpl-lib.cpp"
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
#line 6237 "bison-chpl-lib.cpp"
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
#line 6257 "bison-chpl-lib.cpp"
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
#line 6276 "bison-chpl-lib.cpp"
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
#line 6297 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 955 "chpl.ypp"
  {
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 6305 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 976 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6313 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 983 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6322 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 990 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6330 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 994 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6341 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 1004 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6347 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 1006 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6355 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 1010 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6363 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 1017 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6372 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 1022 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6382 "bison-chpl-lib.cpp"
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
#line 6404 "bison-chpl-lib.cpp"
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
#line 6426 "bison-chpl-lib.cpp"
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
#line 6448 "bison-chpl-lib.cpp"
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
#line 6468 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1110 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6474 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1111 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6481 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1113 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6488 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1118 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6494 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1119 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6501 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1125 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr), LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6511 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1143 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6520 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1148 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6529 "bison-chpl-lib.cpp"
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
#line 6542 "bison-chpl-lib.cpp"
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
#line 6565 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1187 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6571 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1188 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6577 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1193 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6585 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1197 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6593 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1201 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6602 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1206 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6610 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1210 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6618 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1214 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6627 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1222 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6636 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1227 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6646 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1233 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6655 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1238 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6666 "bison-chpl-lib.cpp"
    break;

  case 90: /* opt_only_ls: %empty  */
#line 1246 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6672 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: renames_ls  */
#line 1247 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6678 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1252 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6687 "bison-chpl-lib.cpp"
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
#line 6699 "bison-chpl-lib.cpp"
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
#line 6712 "bison-chpl-lib.cpp"
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
#line 6724 "bison-chpl-lib.cpp"
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
#line 6737 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1293 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6746 "bison-chpl-lib.cpp"
    break;

  case 114: /* import_expr: expr  */
#line 1328 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6754 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr TDOT all_op_name  */
#line 1332 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6763 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1337 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6772 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TAS ident_use  */
#line 1342 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6782 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1348 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6792 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_ls: import_expr  */
#line 1356 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6798 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_ls TCOMMA import_expr  */
#line 1357 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6804 "bison-chpl-lib.cpp"
    break;

  case 121: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1362 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = makeCommentsAndStmt(comments, node.release());
  }
#line 6814 "bison-chpl-lib.cpp"
    break;

  case 122: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1371 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6822 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1375 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6830 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1379 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6838 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1383 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6846 "bison-chpl-lib.cpp"
    break;

  case 126: /* opt_label_ident: %empty  */
#line 1391 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6852 "bison-chpl-lib.cpp"
    break;

  case 127: /* opt_label_ident: TIDENT  */
#line 1392 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6858 "bison-chpl-lib.cpp"
    break;

  case 128: /* opt_label_ident: TINIT  */
#line 1393 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6864 "bison-chpl-lib.cpp"
    break;

  case 137: /* ident_use: TIDENT  */
#line 1419 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6870 "bison-chpl-lib.cpp"
    break;

  case 138: /* ident_use: TINIT  */
#line 1420 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6876 "bison-chpl-lib.cpp"
    break;

  case 139: /* ident_use: TTHIS  */
#line 1421 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6882 "bison-chpl-lib.cpp"
    break;

  case 158: /* scalar_type: TBOOL  */
#line 1447 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6888 "bison-chpl-lib.cpp"
    break;

  case 159: /* scalar_type: TENUM  */
#line 1448 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6894 "bison-chpl-lib.cpp"
    break;

  case 160: /* scalar_type: TINT  */
#line 1449 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6900 "bison-chpl-lib.cpp"
    break;

  case 161: /* scalar_type: TUINT  */
#line 1450 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6906 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TREAL  */
#line 1451 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6912 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TIMAG  */
#line 1452 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6918 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TCOMPLEX  */
#line 1453 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6924 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TBYTES  */
#line 1454 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6930 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TSTRING  */
#line 1455 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6936 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TLOCALE  */
#line 1456 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6942 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TNOTHING  */
#line 1457 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6948 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TVOID  */
#line 1458 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6954 "bison-chpl-lib.cpp"
    break;

  case 173: /* do_stmt: TDO stmt  */
#line 1471 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 6960 "bison-chpl-lib.cpp"
    break;

  case 174: /* do_stmt: block_stmt  */
#line 1472 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 6966 "bison-chpl-lib.cpp"
    break;

  case 175: /* return_stmt: TRETURN TSEMI  */
#line 1477 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6977 "bison-chpl-lib.cpp"
    break;

  case 176: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1484 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6988 "bison-chpl-lib.cpp"
    break;

  case 177: /* class_level_stmt: TSEMI  */
#line 1494 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 6996 "bison-chpl-lib.cpp"
    break;

  case 178: /* class_level_stmt: inner_class_level_stmt  */
#line 1498 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7006 "bison-chpl-lib.cpp"
    break;

  case 179: /* $@1: %empty  */
#line 1503 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7013 "bison-chpl-lib.cpp"
    break;

  case 180: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1505 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->adjustLocation((yyval.commentsAndStmt), (yylsp[-2]), (yylsp[0]));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7023 "bison-chpl-lib.cpp"
    break;

  case 181: /* $@2: %empty  */
#line 1510 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7030 "bison-chpl-lib.cpp"
    break;

  case 182: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1512 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->adjustLocation((yyval.commentsAndStmt), (yylsp[-2]), (yylsp[0]));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7040 "bison-chpl-lib.cpp"
    break;

  case 190: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1531 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7049 "bison-chpl-lib.cpp"
    break;

  case 191: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1536 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7058 "bison-chpl-lib.cpp"
    break;

  case 192: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1541 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7067 "bison-chpl-lib.cpp"
    break;

  case 193: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1546 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7075 "bison-chpl-lib.cpp"
    break;

  case 194: /* forwarding_decl_start: TFORWARDING  */
#line 1553 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7084 "bison-chpl-lib.cpp"
    break;

  case 195: /* extern_or_export: TEXTERN  */
#line 1560 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7090 "bison-chpl-lib.cpp"
    break;

  case 196: /* extern_or_export: TEXPORT  */
#line 1561 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7096 "bison-chpl-lib.cpp"
    break;

  case 197: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1566 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7106 "bison-chpl-lib.cpp"
    break;

  case 198: /* extern_export_kind_inner: TTYPE  */
#line 1579 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7115 "bison-chpl-lib.cpp"
    break;

  case 199: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1588 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7126 "bison-chpl-lib.cpp"
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
#line 7139 "bison-chpl-lib.cpp"
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
#line 7161 "bison-chpl-lib.cpp"
    break;

  case 202: /* $@3: %empty  */
#line 1623 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7169 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 1627 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7178 "bison-chpl-lib.cpp"
    break;

  case 204: /* $@4: %empty  */
#line 1632 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7187 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr extern_export_kind_inner $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1637 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-1])), (yyvsp[-3].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7196 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1645 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yyloc), (yyvsp[0].sizedStr));
  }
#line 7204 "bison-chpl-lib.cpp"
    break;

  case 207: /* no_loop_attributes: %empty  */
#line 1651 "chpl.ypp"
  {
    // Even though there are no attributes, there could be loop pragmas.
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 7213 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1657 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7219 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1658 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7225 "bison-chpl-lib.cpp"
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
#line 7251 "bison-chpl-lib.cpp"
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
#line 7273 "bison-chpl-lib.cpp"
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
#line 7295 "bison-chpl-lib.cpp"
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
#line 7323 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1757 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7332 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: for_loop_kw expr TIN expr task_intent_clause do_stmt  */
#line 1762 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7341 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1767 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7350 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator task_intent_clause do_stmt  */
#line 1772 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7359 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1777 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7368 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: for_loop_kw expr task_intent_clause do_stmt  */
#line 1782 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7377 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1787 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7386 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw zippered_iterator task_intent_clause do_stmt  */
#line 1792 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7395 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1797 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7404 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TLSBR expr_ls TIN expr task_intent_clause TRSBR stmt  */
#line 1802 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7413 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1807 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7422 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator task_intent_clause TRSBR stmt  */
#line 1812 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7431 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1817 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7440 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TLSBR expr_ls task_intent_clause TRSBR stmt  */
#line 1822 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7449 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1827 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7459 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR zippered_iterator task_intent_clause TRSBR stmt  */
#line 1833 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7469 "bison-chpl-lib.cpp"
    break;

  case 234: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1842 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7479 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF expr TTHEN stmt  */
#line 1851 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7487 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF expr block_stmt  */
#line 1855 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7495 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1859 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7503 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1863 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7511 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1867 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7519 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF ifvar block_stmt  */
#line 1871 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7527 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1875 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7535 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1879 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7543 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1883 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7552 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1888 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7561 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1893 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7570 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1898 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7579 "bison-chpl-lib.cpp"
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
#line 7598 "bison-chpl-lib.cpp"
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
#line 7617 "bison-chpl-lib.cpp"
    break;

  case 249: /* interface_start: TINTERFACE ident_def  */
#line 1939 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Interface);
  }
#line 7625 "bison-chpl-lib.cpp"
    break;

  case 250: /* interface_stmt: interface_start TLP ifc_formal_ls TRP block_stmt  */
#line 1946 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-4].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), parts, (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC((yyvsp[0].commentsAndStmt).curlyLoc));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7636 "bison-chpl-lib.cpp"
    break;

  case 251: /* interface_stmt: interface_start block_stmt  */
#line 1953 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-1].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), parts, nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC((yyvsp[0].commentsAndStmt).curlyLoc));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7647 "bison-chpl-lib.cpp"
    break;

  case 252: /* ifc_formal_ls: ifc_formal  */
#line 1962 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7653 "bison-chpl-lib.cpp"
    break;

  case 253: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1963 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7659 "bison-chpl-lib.cpp"
    break;

  case 254: /* ifc_formal: ident_def  */
#line 1968 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7665 "bison-chpl-lib.cpp"
    break;

  case 274: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 2011 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7673 "bison-chpl-lib.cpp"
    break;

  case 275: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 2015 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7681 "bison-chpl-lib.cpp"
    break;

  case 276: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 2019 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7689 "bison-chpl-lib.cpp"
    break;

  case 277: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2026 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7697 "bison-chpl-lib.cpp"
    break;

  case 278: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 2030 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7705 "bison-chpl-lib.cpp"
    break;

  case 279: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2034 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7713 "bison-chpl-lib.cpp"
    break;

  case 280: /* try_stmt: TTRY tryable_stmt  */
#line 2041 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7721 "bison-chpl-lib.cpp"
    break;

  case 281: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2045 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7729 "bison-chpl-lib.cpp"
    break;

  case 282: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2049 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7737 "bison-chpl-lib.cpp"
    break;

  case 283: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2053 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7745 "bison-chpl-lib.cpp"
    break;

  case 284: /* catch_expr_ls: %empty  */
#line 2059 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7751 "bison-chpl-lib.cpp"
    break;

  case 285: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2060 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7757 "bison-chpl-lib.cpp"
    break;

  case 286: /* catch_expr: TCATCH block_stmt  */
#line 2065 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false, TextLocation::create());
  }
#line 7765 "bison-chpl-lib.cpp"
    break;

  case 287: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2069 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false, TextLocation::create());
  }
#line 7773 "bison-chpl-lib.cpp"
    break;

  case 288: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2073 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true, context->makeSpannedLocation((yylsp[-3]), (yylsp[-1])));
  }
#line 7781 "bison-chpl-lib.cpp"
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
#line 7800 "bison-chpl-lib.cpp"
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
#line 7819 "bison-chpl-lib.cpp"
    break;

  case 291: /* throw_stmt: TTHROW expr TSEMI  */
#line 2113 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = makeCommentsAndStmt(comments, node.release());
  }
#line 7829 "bison-chpl-lib.cpp"
    break;

  case 292: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2121 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7838 "bison-chpl-lib.cpp"
    break;

  case 293: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2126 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = makeCommentsAndStmt(comments, node.release());
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7849 "bison-chpl-lib.cpp"
    break;

  case 294: /* when_stmt_ls: %empty  */
#line 2135 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7855 "bison-chpl-lib.cpp"
    break;

  case 295: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2136 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7861 "bison-chpl-lib.cpp"
    break;

  case 296: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2141 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7869 "bison-chpl-lib.cpp"
    break;

  case 297: /* when_stmt: TOTHERWISE stmt  */
#line 2145 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7878 "bison-chpl-lib.cpp"
    break;

  case 298: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2150 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 7888 "bison-chpl-lib.cpp"
    break;

  case 299: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2159 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 7897 "bison-chpl-lib.cpp"
    break;

  case 300: /* manager_expr: expr TAS ident_def  */
#line 2164 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 7905 "bison-chpl-lib.cpp"
    break;

  case 301: /* manager_expr: expr  */
#line 2168 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 7913 "bison-chpl-lib.cpp"
    break;

  case 302: /* manager_expr_ls: manager_expr  */
#line 2174 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7919 "bison-chpl-lib.cpp"
    break;

  case 303: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2175 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7925 "bison-chpl-lib.cpp"
    break;

  case 304: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2180 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 7934 "bison-chpl-lib.cpp"
    break;

  case 305: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2190 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 7945 "bison-chpl-lib.cpp"
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
#line 7958 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_start: class_tag ident_def  */
#line 2210 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 7966 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_tag: TCLASS  */
#line 2216 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 7972 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_tag: TRECORD  */
#line 2217 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 7978 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_tag: TUNION  */
#line 2218 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 7984 "bison-chpl-lib.cpp"
    break;

  case 311: /* opt_inherit: %empty  */
#line 2222 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7990 "bison-chpl-lib.cpp"
    break;

  case 312: /* opt_inherit: TCOLON expr_ls  */
#line 2223 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7996 "bison-chpl-lib.cpp"
    break;

  case 313: /* class_level_stmt_ls: %empty  */
#line 2227 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8005 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2232 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8013 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2236 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8021 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2240 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8029 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2244 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8037 "bison-chpl-lib.cpp"
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
#line 8061 "bison-chpl-lib.cpp"
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
#line 8075 "bison-chpl-lib.cpp"
    break;

  case 320: /* enum_header: TENUM ident_def  */
#line 2285 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8083 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_ls: enum_item  */
#line 2292 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8092 "bison-chpl-lib.cpp"
    break;

  case 322: /* enum_ls: enum_ls TCOMMA  */
#line 2297 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8102 "bison-chpl-lib.cpp"
    break;

  case 323: /* $@5: %empty  */
#line 2303 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8111 "bison-chpl-lib.cpp"
    break;

  case 324: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2308 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8120 "bison-chpl-lib.cpp"
    break;

  case 325: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2313 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8129 "bison-chpl-lib.cpp"
    break;

  case 326: /* $@6: %empty  */
#line 2318 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8138 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2323 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8147 "bison-chpl-lib.cpp"
    break;

  case 328: /* enum_item: ident_def  */
#line 2333 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[0])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8159 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_item: ident_def TASSIGN expr  */
#line 2341 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[-2])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8173 "bison-chpl-lib.cpp"
    break;

  case 331: /* linkage_spec: linkage_spec_empty  */
#line 2356 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8179 "bison-chpl-lib.cpp"
    break;

  case 332: /* linkage_spec: TINLINE  */
#line 2357 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8186 "bison-chpl-lib.cpp"
    break;

  case 333: /* linkage_spec: TOVERRIDE  */
#line 2359 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8193 "bison-chpl-lib.cpp"
    break;

  case 334: /* opt_fn_type_formal_ls: %empty  */
#line 2364 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8199 "bison-chpl-lib.cpp"
    break;

  case 335: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2365 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8205 "bison-chpl-lib.cpp"
    break;

  case 336: /* fn_type_formal_ls: fn_type_formal  */
#line 2369 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8211 "bison-chpl-lib.cpp"
    break;

  case 337: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2370 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8217 "bison-chpl-lib.cpp"
    break;

  case 338: /* fn_type_formal: named_formal  */
#line 2375 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8223 "bison-chpl-lib.cpp"
    break;

  case 339: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2378 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8229 "bison-chpl-lib.cpp"
    break;

  case 340: /* fn_type_formal: formal_type  */
#line 2380 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8235 "bison-chpl-lib.cpp"
    break;

  case 341: /* opt_fn_type_ret_type: %empty  */
#line 2384 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8241 "bison-chpl-lib.cpp"
    break;

  case 342: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2385 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8247 "bison-chpl-lib.cpp"
    break;

  case 343: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2398 "chpl.ypp"
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
#line 8267 "bison-chpl-lib.cpp"
    break;

  case 344: /* $@7: %empty  */
#line 2417 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8277 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_expr: fn_type $@7 function_body_stmt  */
#line 2423 "chpl.ypp"
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
#line 8294 "bison-chpl-lib.cpp"
    break;

  case 346: /* $@8: %empty  */
#line 2436 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8304 "bison-chpl-lib.cpp"
    break;

  case 347: /* fn_expr: fn_type TALIAS $@8 expr  */
#line 2442 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8317 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2454 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8325 "bison-chpl-lib.cpp"
    break;

  case 349: /* $@9: %empty  */
#line 2463 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8333 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@9 opt_function_body_stmt  */
#line 2467 "chpl.ypp"
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
#line 8360 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2493 "chpl.ypp"
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
#line 8377 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2506 "chpl.ypp"
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
#line 8394 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2519 "chpl.ypp"
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
#line 8412 "bison-chpl-lib.cpp"
    break;

  case 354: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2533 "chpl.ypp"
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
#line 8430 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2547 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      // store location of the header to handle weird bison location issues
      // with empty rules
      fp.headerLoc =
        context->makeSpannedLocation((yylsp[-3]), (yyvsp[0].exprList) != context->parenlessMarker ? (yylsp[0]) : (yylsp[-1]));
      (yyval.functionParts) = fp;
    }
#line 8444 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2560 "chpl.ypp"
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
#line 8460 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2576 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 8469 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_ident: ident_def TBANG  */
#line 2586 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8479 "bison-chpl-lib.cpp"
    break;

  case 403: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2645 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8485 "bison-chpl-lib.cpp"
    break;

  case 404: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2646 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8491 "bison-chpl-lib.cpp"
    break;

  case 405: /* opt_formal_ls: %empty  */
#line 2650 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; CHPL_NULL_YYLLOC((yyloc)); }
#line 8497 "bison-chpl-lib.cpp"
    break;

  case 406: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2651 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8503 "bison-chpl-lib.cpp"
    break;

  case 407: /* formal_ls_inner: formal  */
#line 2655 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8509 "bison-chpl-lib.cpp"
    break;

  case 408: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2656 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8515 "bison-chpl-lib.cpp"
    break;

  case 409: /* formal_ls: %empty  */
#line 2660 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8521 "bison-chpl-lib.cpp"
    break;

  case 410: /* formal_ls: formal_ls_inner  */
#line 2661 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8527 "bison-chpl-lib.cpp"
    break;

  case 412: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2670 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8535 "bison-chpl-lib.cpp"
    break;

  case 413: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2675 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8543 "bison-chpl-lib.cpp"
    break;

  case 414: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2680 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8551 "bison-chpl-lib.cpp"
    break;

  case 415: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2685 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8559 "bison-chpl-lib.cpp"
    break;

  case 416: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2690 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yylsp[-5]), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8567 "bison-chpl-lib.cpp"
    break;

  case 417: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2695 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8575 "bison-chpl-lib.cpp"
    break;

  case 418: /* opt_formal_intent_tag: %empty  */
#line 2701 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);
  }
#line 8584 "bison-chpl-lib.cpp"
    break;

  case 419: /* opt_formal_intent_tag: required_intent_tag  */
#line 2706 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8593 "bison-chpl-lib.cpp"
    break;

  case 420: /* required_intent_tag: TIN  */
#line 2713 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::IN);            }
#line 8599 "bison-chpl-lib.cpp"
    break;

  case 421: /* required_intent_tag: TINOUT  */
#line 2714 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::INOUT);         }
#line 8605 "bison-chpl-lib.cpp"
    break;

  case 422: /* required_intent_tag: TOUT  */
#line 2715 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::OUT);           }
#line 8611 "bison-chpl-lib.cpp"
    break;

  case 423: /* required_intent_tag: TCONST TIN  */
#line 2716 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_IN);      }
#line 8617 "bison-chpl-lib.cpp"
    break;

  case 424: /* required_intent_tag: TCONST TREF  */
#line 2717 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);     }
#line 8623 "bison-chpl-lib.cpp"
    break;

  case 425: /* required_intent_tag: TCONST  */
#line 2718 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);         }
#line 8629 "bison-chpl-lib.cpp"
    break;

  case 426: /* required_intent_tag: TPARAM  */
#line 2719 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);         }
#line 8635 "bison-chpl-lib.cpp"
    break;

  case 427: /* required_intent_tag: TREF  */
#line 2720 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);           }
#line 8641 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TTYPE  */
#line 2721 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);          }
#line 8647 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TVAR  */
#line 2722 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR); }
#line 8653 "bison-chpl-lib.cpp"
    break;

  case 430: /* opt_this_intent_tag: %empty  */
#line 2726 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);     }
#line 8659 "bison-chpl-lib.cpp"
    break;

  case 431: /* opt_this_intent_tag: TPARAM  */
#line 2727 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);              }
#line 8665 "bison-chpl-lib.cpp"
    break;

  case 432: /* opt_this_intent_tag: TREF  */
#line 2728 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);                }
#line 8671 "bison-chpl-lib.cpp"
    break;

  case 433: /* opt_this_intent_tag: TCONST TREF  */
#line 2729 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);          }
#line 8677 "bison-chpl-lib.cpp"
    break;

  case 434: /* opt_this_intent_tag: TCONST  */
#line 2730 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);              }
#line 8683 "bison-chpl-lib.cpp"
    break;

  case 435: /* opt_this_intent_tag: TTYPE  */
#line 2731 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);               }
#line 8689 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_this_intent_tag: TIN  */
#line 2732 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::IN);       }
#line 8695 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: TCONST TIN  */
#line 2733 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::CONST_IN); }
#line 8701 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TVAR  */
#line 2734 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR);      }
#line 8707 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TOUT  */
#line 2735 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::OUT);      }
#line 8713 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TINOUT  */
#line 2736 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::INOUT);    }
#line 8719 "bison-chpl-lib.cpp"
    break;

  case 441: /* proc_iter_or_op: TPROC  */
#line 2740 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8725 "bison-chpl-lib.cpp"
    break;

  case 442: /* proc_iter_or_op: TITER  */
#line 2741 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8731 "bison-chpl-lib.cpp"
    break;

  case 443: /* proc_iter_or_op: TOPERATOR  */
#line 2742 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8737 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_ret_tag: %empty  */
#line 2746 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::DEFAULT_RETURN_INTENT); }
#line 8743 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_ret_tag: TOUT  */
#line 2747 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::OUT);                   }
#line 8749 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_ret_tag: TCONST  */
#line 2748 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST);                 }
#line 8755 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_ret_tag: TCONST TREF  */
#line 2749 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST_REF);             }
#line 8761 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_ret_tag: TREF  */
#line 2750 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::REF);                   }
#line 8767 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: TPARAM  */
#line 2751 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::PARAM);                 }
#line 8773 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TTYPE  */
#line 2752 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::TYPE);                  }
#line 8779 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TIN  */
#line 2753 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::IN);            }
#line 8785 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TCONST TIN  */
#line 2754 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::CONST_IN);      }
#line 8791 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_ret_tag: TVAR  */
#line 2755 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::VAR);           }
#line 8797 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: TINOUT  */
#line 2756 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::INOUT);         }
#line 8803 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_throws_error: %empty  */
#line 2760 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8809 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_throws_error: TTHROWS  */
#line 2761 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8815 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_function_body_stmt: TSEMI  */
#line 2764 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8821 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_function_body_stmt: function_body_stmt  */
#line 2765 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8827 "bison-chpl-lib.cpp"
    break;

  case 459: /* function_body_stmt: block_stmt_body  */
#line 2769 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8833 "bison-chpl-lib.cpp"
    break;

  case 460: /* function_body_stmt: TDO toplevel_stmt  */
#line 2770 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yylsp[0]), (yyvsp[0].commentsAndStmt)); }
#line 8839 "bison-chpl-lib.cpp"
    break;

  case 461: /* query_expr: TQUERIEDIDENT  */
#line 2774 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8845 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_lifetime_where: %empty  */
#line 2779 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8851 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_lifetime_where: TWHERE expr  */
#line 2781 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8857 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2783 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8863 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2785 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 8869 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2787 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 8875 "bison-chpl-lib.cpp"
    break;

  case 467: /* lifetime_components_expr: lifetime_expr  */
#line 2792 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8881 "bison-chpl-lib.cpp"
    break;

  case 468: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2794 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8887 "bison-chpl-lib.cpp"
    break;

  case 469: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2799 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8893 "bison-chpl-lib.cpp"
    break;

  case 470: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2801 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8899 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2803 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8905 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2805 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8911 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2807 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8917 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2809 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8923 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2811 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 8929 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_ident: TIDENT  */
#line 2815 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8935 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_ident: TINIT  */
#line 2816 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8941 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_ident: TTHIS  */
#line 2817 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8947 "bison-chpl-lib.cpp"
    break;

  case 479: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2822 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 8956 "bison-chpl-lib.cpp"
    break;

  case 480: /* type_alias_decl_stmt_start: TTYPE  */
#line 2831 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 8966 "bison-chpl-lib.cpp"
    break;

  case 481: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2837 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true, (yylsp[-1]));
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 8977 "bison-chpl-lib.cpp"
    break;

  case 482: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2847 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 8985 "bison-chpl-lib.cpp"
    break;

  case 483: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2851 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8993 "bison-chpl-lib.cpp"
    break;

  case 484: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2858 "chpl.ypp"
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
#line 9015 "bison-chpl-lib.cpp"
    break;

  case 485: /* opt_init_type: %empty  */
#line 2878 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9021 "bison-chpl-lib.cpp"
    break;

  case 486: /* opt_init_type: TASSIGN expr  */
#line 2880 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9027 "bison-chpl-lib.cpp"
    break;

  case 487: /* var_decl_type: TPARAM  */
#line 2884 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9033 "bison-chpl-lib.cpp"
    break;

  case 488: /* var_decl_type: TCONST TREF  */
#line 2885 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9039 "bison-chpl-lib.cpp"
    break;

  case 489: /* var_decl_type: TREF  */
#line 2886 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9045 "bison-chpl-lib.cpp"
    break;

  case 490: /* var_decl_type: TCONST  */
#line 2887 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9051 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_type: TVAR  */
#line 2888 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9057 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_start: var_decl_type  */
#line 2893 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9066 "bison-chpl-lib.cpp"
    break;

  case 493: /* $@10: %empty  */
#line 2901 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true, (yylsp[0]));
  }
#line 9075 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_stmt: TCONFIG $@10 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2905 "chpl.ypp"
                                              {
    CHPL_ASSERT(context->isVarDeclConfig);
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9085 "bison-chpl-lib.cpp"
    break;

  case 495: /* $@11: %empty  */
#line 2911 "chpl.ypp"
  {
    // Use a mid-rule action to preserve the 'on' expr
    context->noteVarDestinationExpr((yyvsp[0].expr));
  }
#line 9094 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_stmt: TON expr $@11 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2916 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9103 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_stmt: var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2921 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9112 "bison-chpl-lib.cpp"
    break;

  case 498: /* var_decl_stmt: var_decl_start error TSEMI  */
#line 2926 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = makeCommentsAndStmt(nullptr, ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release());
    delete (yyvsp[-2].attributeGroup);
    context->resetDeclStateOnError();
  }
#line 9122 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2935 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9130 "bison-chpl-lib.cpp"
    break;

  case 500: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2939 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9138 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2946 "chpl.ypp"
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
#line 9160 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2964 "chpl.ypp"
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
#line 9180 "bison-chpl-lib.cpp"
    break;

  case 503: /* tuple_var_decl_component: ident_def  */
#line 2983 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9188 "bison-chpl-lib.cpp"
    break;

  case 504: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2987 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9196 "bison-chpl-lib.cpp"
    break;

  case 505: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2994 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9202 "bison-chpl-lib.cpp"
    break;

  case 506: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2996 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9208 "bison-chpl-lib.cpp"
    break;

  case 507: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2998 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9214 "bison-chpl-lib.cpp"
    break;

  case 508: /* opt_init_expr: %empty  */
#line 3004 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9220 "bison-chpl-lib.cpp"
    break;

  case 509: /* opt_init_expr: TASSIGN TNOINIT  */
#line 3005 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9226 "bison-chpl-lib.cpp"
    break;

  case 510: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 3006 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9232 "bison-chpl-lib.cpp"
    break;

  case 511: /* formal_or_ret_type_expr: expr  */
#line 3010 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9238 "bison-chpl-lib.cpp"
    break;

  case 512: /* ret_type: formal_or_ret_type_expr  */
#line 3014 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9244 "bison-chpl-lib.cpp"
    break;

  case 513: /* ret_type: reserved_type_ident_use  */
#line 3015 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9250 "bison-chpl-lib.cpp"
    break;

  case 514: /* ret_type: error  */
#line 3016 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9256 "bison-chpl-lib.cpp"
    break;

  case 515: /* colon_ret_type: TCOLON ret_type  */
#line 3020 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9262 "bison-chpl-lib.cpp"
    break;

  case 516: /* colon_ret_type: error  */
#line 3021 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9268 "bison-chpl-lib.cpp"
    break;

  case 517: /* opt_ret_type: %empty  */
#line 3025 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9274 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_type: %empty  */
#line 3030 "chpl.ypp"
                                 { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9280 "bison-chpl-lib.cpp"
    break;

  case 520: /* opt_type: TCOLON expr  */
#line 3031 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9286 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_type: TCOLON reserved_type_ident_use  */
#line 3032 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9292 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_type: error  */
#line 3033 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9298 "bison-chpl-lib.cpp"
    break;

  case 523: /* formal_type: formal_or_ret_type_expr  */
#line 3037 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9304 "bison-chpl-lib.cpp"
    break;

  case 524: /* formal_type: reserved_type_ident_use  */
#line 3038 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9310 "bison-chpl-lib.cpp"
    break;

  case 525: /* colon_formal_type: TCOLON formal_type  */
#line 3042 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9316 "bison-chpl-lib.cpp"
    break;

  case 526: /* opt_colon_formal_type: %empty  */
#line 3046 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9322 "bison-chpl-lib.cpp"
    break;

  case 527: /* opt_colon_formal_type: colon_formal_type  */
#line 3047 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9328 "bison-chpl-lib.cpp"
    break;

  case 528: /* expr_ls: expr  */
#line 3053 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9334 "bison-chpl-lib.cpp"
    break;

  case 529: /* expr_ls: expr_ls TCOMMA expr  */
#line 3054 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9340 "bison-chpl-lib.cpp"
    break;

  case 530: /* expr_ls_end_semi: expr_ls semicolon_list  */
#line 3058 "chpl.ypp"
                         {
    (yyval.ndArrayList) = context->makeNDArrayList();
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElement((yyvsp[-1].exprList), (yylsp[-1])));
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9350 "bison-chpl-lib.cpp"
    break;

  case 531: /* expr_ls_end_semi: expr_ls TCOMMA semicolon_list  */
#line 3063 "chpl.ypp"
                                {
    (yyval.ndArrayList) = context->makeNDArrayList();
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElement((yyvsp[-2].exprList), (yylsp[-2])));
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9360 "bison-chpl-lib.cpp"
    break;

  case 532: /* expr_ls_end_semi: expr_ls_semi semicolon_list  */
#line 3069 "chpl.ypp"
  {
    (yyval.ndArrayList) = (yyvsp[-1].ndArrayList);
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9369 "bison-chpl-lib.cpp"
    break;

  case 533: /* expr_ls_semi: expr_ls_end_semi expr_ls  */
#line 3076 "chpl.ypp"
                           {
    (yyval.ndArrayList) = context->appendNDArrayList((yyvsp[-1].ndArrayList), makeNDArrayElement((yyvsp[0].exprList), (yylsp[0])));
  }
#line 9377 "bison-chpl-lib.cpp"
    break;

  case 534: /* expr_ls_semi: expr_ls_end_semi expr_ls TCOMMA  */
#line 3079 "chpl.ypp"
                                  {
    (yyval.ndArrayList) = context->appendNDArrayList((yyvsp[-2].ndArrayList), makeNDArrayElement((yyvsp[-1].exprList), (yylsp[-1])));
  }
#line 9385 "bison-chpl-lib.cpp"
    break;

  case 535: /* semicolon_list: TSEMI  */
#line 3085 "chpl.ypp"
                       { (yyval.counter) = 1; }
#line 9391 "bison-chpl-lib.cpp"
    break;

  case 536: /* semicolon_list: semicolon_list TSEMI  */
#line 3086 "chpl.ypp"
                       { (yyval.counter) = (yyvsp[-1].counter) + 1; }
#line 9397 "bison-chpl-lib.cpp"
    break;

  case 537: /* tuple_component: opt_try_expr  */
#line 3092 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9403 "bison-chpl-lib.cpp"
    break;

  case 538: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3097 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9411 "bison-chpl-lib.cpp"
    break;

  case 539: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3101 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9419 "bison-chpl-lib.cpp"
    break;

  case 540: /* opt_actual_ls: %empty  */
#line 3107 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9425 "bison-chpl-lib.cpp"
    break;

  case 541: /* opt_actual_ls: actual_ls  */
#line 3108 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9431 "bison-chpl-lib.cpp"
    break;

  case 542: /* actual_ls: actual_expr  */
#line 3113 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9440 "bison-chpl-lib.cpp"
    break;

  case 543: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3118 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9450 "bison-chpl-lib.cpp"
    break;

  case 544: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3126 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9456 "bison-chpl-lib.cpp"
    break;

  case 545: /* actual_expr: opt_try_expr  */
#line 3127 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9462 "bison-chpl-lib.cpp"
    break;

  case 546: /* ident_expr: ident_use  */
#line 3131 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9468 "bison-chpl-lib.cpp"
    break;

  case 547: /* ident_expr: scalar_type  */
#line 3132 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9474 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3145 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9480 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3147 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9486 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3149 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9492 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3151 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9502 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TATOMIC expr  */
#line 3157 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9508 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TSYNC expr  */
#line 3159 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9514 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TOWNED  */
#line 3162 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9520 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TOWNED expr  */
#line 3164 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9526 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TUNMANAGED  */
#line 3166 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9532 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3168 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9538 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TSHARED  */
#line 3170 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9544 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TSHARED expr  */
#line 3172 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9550 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TBORROWED  */
#line 3174 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9556 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TBORROWED expr  */
#line 3176 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9562 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TCLASS  */
#line 3178 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9568 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TRECORD  */
#line 3180 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9574 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr: no_loop_attributes for_expr_base  */
#line 3184 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9580 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3189 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9588 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3193 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9596 "bison-chpl-lib.cpp"
    break;

  case 572: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3197 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9604 "bison-chpl-lib.cpp"
    break;

  case 573: /* for_expr_base: for_loop_kw zippered_iterator TDO expr  */
#line 3201 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "expression-level loops with 'zip' must have an index.");
  }
#line 9612 "bison-chpl-lib.cpp"
    break;

  case 574: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3205 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9625 "bison-chpl-lib.cpp"
    break;

  case 575: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3214 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9638 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3223 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9651 "bison-chpl-lib.cpp"
    break;

  case 577: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3234 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9657 "bison-chpl-lib.cpp"
    break;

  case 578: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3239 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9665 "bison-chpl-lib.cpp"
    break;

  case 579: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3243 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9673 "bison-chpl-lib.cpp"
    break;

  case 580: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3247 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9681 "bison-chpl-lib.cpp"
    break;

  case 581: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3251 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9689 "bison-chpl-lib.cpp"
    break;

  case 582: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3255 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9697 "bison-chpl-lib.cpp"
    break;

  case 583: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3259 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9705 "bison-chpl-lib.cpp"
    break;

  case 584: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3263 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9713 "bison-chpl-lib.cpp"
    break;

  case 585: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3270 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9727 "bison-chpl-lib.cpp"
    break;

  case 586: /* nil_expr: TNIL  */
#line 3287 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9733 "bison-chpl-lib.cpp"
    break;

  case 593: /* opt_task_intent_ls: %empty  */
#line 3304 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9739 "bison-chpl-lib.cpp"
    break;

  case 594: /* opt_task_intent_ls: task_intent_clause  */
#line 3305 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9745 "bison-chpl-lib.cpp"
    break;

  case 595: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3310 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9753 "bison-chpl-lib.cpp"
    break;

  case 596: /* task_intent_clause: TWITH TLP TRP  */
#line 3314 "chpl.ypp"
  {
    auto err = context->syntax((yyloc), "'with' clause cannot be empty.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-2]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9764 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_intent_clause: TWITH TLP error TRP  */
#line 3321 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[-2]), "invalid intent expression in 'with' clause.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9775 "bison-chpl-lib.cpp"
    break;

  case 598: /* task_intent_clause: TWITH error  */
#line 3328 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[0]), "missing parentheses around 'with' clause intents.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-1]), dummyLoc, dummyLoc, dummyLoc, exprs);
  }
#line 9786 "bison-chpl-lib.cpp"
    break;

  case 599: /* task_intent_ls: intent_expr  */
#line 3337 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9792 "bison-chpl-lib.cpp"
    break;

  case 600: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3338 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9798 "bison-chpl-lib.cpp"
    break;

  case 601: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3343 "chpl.ypp"
  {
    (yyval.expr) = context->buildTaskIntent((yyloc), (yylsp[-2]), nullptr, (yyvsp[-2].expr), (yyvsp[-3].taskIntent), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9806 "bison-chpl-lib.cpp"
    break;

  case 602: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3347 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9814 "bison-chpl-lib.cpp"
    break;

  case 603: /* intent_expr: expr TREDUCE ident_expr  */
#line 3351 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9822 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_var_prefix: TCONST  */
#line 3357 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST);          }
#line 9828 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_var_prefix: TIN  */
#line 3358 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::IN);             }
#line 9834 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_var_prefix: TCONST TIN  */
#line 3359 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_IN);       }
#line 9840 "bison-chpl-lib.cpp"
    break;

  case 607: /* task_var_prefix: TREF  */
#line 3360 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::REF);            }
#line 9846 "bison-chpl-lib.cpp"
    break;

  case 608: /* task_var_prefix: TCONST TREF  */
#line 3361 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_REF);      }
#line 9852 "bison-chpl-lib.cpp"
    break;

  case 609: /* task_var_prefix: TVAR  */
#line 3362 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::VAR);            }
#line 9858 "bison-chpl-lib.cpp"
    break;

  case 610: /* task_var_prefix: TOUT  */
#line 3363 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::OUT);   }
#line 9864 "bison-chpl-lib.cpp"
    break;

  case 611: /* task_var_prefix: TINOUT  */
#line 3364 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::INOUT); }
#line 9870 "bison-chpl-lib.cpp"
    break;

  case 612: /* task_var_prefix: TPARAM  */
#line 3365 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::PARAM); }
#line 9876 "bison-chpl-lib.cpp"
    break;

  case 613: /* task_var_prefix: TTYPE  */
#line 3366 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::TYPE);  }
#line 9882 "bison-chpl-lib.cpp"
    break;

  case 614: /* new_maybe_decorated: TNEW  */
#line 3371 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9888 "bison-chpl-lib.cpp"
    break;

  case 615: /* new_maybe_decorated: TNEW TOWNED  */
#line 3373 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9894 "bison-chpl-lib.cpp"
    break;

  case 616: /* new_maybe_decorated: TNEW TSHARED  */
#line 3375 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9900 "bison-chpl-lib.cpp"
    break;

  case 617: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3377 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9906 "bison-chpl-lib.cpp"
    break;

  case 618: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3379 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9912 "bison-chpl-lib.cpp"
    break;

  case 619: /* new_expr: new_maybe_decorated expr  */
#line 3385 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 9920 "bison-chpl-lib.cpp"
    break;

  case 620: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3392 "chpl.ypp"
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
#line 9937 "bison-chpl-lib.cpp"
    break;

  case 621: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3405 "chpl.ypp"
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
#line 9954 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3418 "chpl.ypp"
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
#line 9972 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3432 "chpl.ypp"
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
#line 9991 "bison-chpl-lib.cpp"
    break;

  case 624: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3450 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9999 "bison-chpl-lib.cpp"
    break;

  case 625: /* range_literal_expr: expr TDOTDOT expr  */
#line 3457 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10008 "bison-chpl-lib.cpp"
    break;

  case 626: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3462 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10017 "bison-chpl-lib.cpp"
    break;

  case 627: /* range_literal_expr: expr TDOTDOT  */
#line 3467 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10026 "bison-chpl-lib.cpp"
    break;

  case 628: /* range_literal_expr: TDOTDOT expr  */
#line 3472 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10036 "bison-chpl-lib.cpp"
    break;

  case 629: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3478 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10046 "bison-chpl-lib.cpp"
    break;

  case 630: /* range_literal_expr: TDOTDOT  */
#line 3484 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10056 "bison-chpl-lib.cpp"
    break;

  case 631: /* cast_expr: expr TCOLON expr  */
#line 3514 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10064 "bison-chpl-lib.cpp"
    break;

  case 632: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3521 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10072 "bison-chpl-lib.cpp"
    break;

  case 633: /* super_expr: fn_expr  */
#line 3527 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10078 "bison-chpl-lib.cpp"
    break;

  case 637: /* expr: sub_type_level_expr TQUESTION  */
#line 3536 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10084 "bison-chpl-lib.cpp"
    break;

  case 638: /* expr: TQUESTION  */
#line 3538 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 642: /* expr: fn_type  */
#line 3543 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10096 "bison-chpl-lib.cpp"
    break;

  case 651: /* opt_expr: %empty  */
#line 3556 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10102 "bison-chpl-lib.cpp"
    break;

  case 652: /* opt_expr: expr  */
#line 3557 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10108 "bison-chpl-lib.cpp"
    break;

  case 653: /* opt_try_expr: TTRY expr  */
#line 3561 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10114 "bison-chpl-lib.cpp"
    break;

  case 654: /* opt_try_expr: TTRYBANG expr  */
#line 3562 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10120 "bison-chpl-lib.cpp"
    break;

  case 655: /* opt_try_expr: super_expr  */
#line 3563 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10126 "bison-chpl-lib.cpp"
    break;

  case 661: /* call_base_expr: expr TBANG  */
#line 3580 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10134 "bison-chpl-lib.cpp"
    break;

  case 662: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3583 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10140 "bison-chpl-lib.cpp"
    break;

  case 664: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3589 "chpl.ypp"
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
#line 10156 "bison-chpl-lib.cpp"
    break;

  case 665: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3601 "chpl.ypp"
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
#line 10172 "bison-chpl-lib.cpp"
    break;

  case 666: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3613 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10180 "bison-chpl-lib.cpp"
    break;

  case 667: /* dot_expr: expr TDOT ident_use  */
#line 3620 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10186 "bison-chpl-lib.cpp"
    break;

  case 668: /* dot_expr: expr TDOT TTYPE  */
#line 3622 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10192 "bison-chpl-lib.cpp"
    break;

  case 669: /* dot_expr: expr TDOT TDOMAIN  */
#line 3624 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10198 "bison-chpl-lib.cpp"
    break;

  case 670: /* dot_expr: expr TDOT TLOCALE  */
#line 3626 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10204 "bison-chpl-lib.cpp"
    break;

  case 671: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3628 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10210 "bison-chpl-lib.cpp"
    break;

  case 672: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3630 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10216 "bison-chpl-lib.cpp"
    break;

  case 673: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3639 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10226 "bison-chpl-lib.cpp"
    break;

  case 674: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3645 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10234 "bison-chpl-lib.cpp"
    break;

  case 675: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3649 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10242 "bison-chpl-lib.cpp"
    break;

  case 676: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3653 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10250 "bison-chpl-lib.cpp"
    break;

  case 677: /* bool_literal: TFALSE  */
#line 3659 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10256 "bison-chpl-lib.cpp"
    break;

  case 678: /* bool_literal: TTRUE  */
#line 3660 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10262 "bison-chpl-lib.cpp"
    break;

  case 679: /* str_bytes_literal: STRINGLITERAL  */
#line 3664 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10268 "bison-chpl-lib.cpp"
    break;

  case 680: /* str_bytes_literal: BYTESLITERAL  */
#line 3665 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10274 "bison-chpl-lib.cpp"
    break;

  case 683: /* literal_expr: INTLITERAL  */
#line 3671 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10280 "bison-chpl-lib.cpp"
    break;

  case 684: /* literal_expr: REALLITERAL  */
#line 3672 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10286 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: IMAGLITERAL  */
#line 3673 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10292 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: TNONE  */
#line 3674 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10298 "bison-chpl-lib.cpp"
    break;

  case 687: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3676 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10307 "bison-chpl-lib.cpp"
    break;

  case 688: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3681 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10316 "bison-chpl-lib.cpp"
    break;

  case 689: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3686 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10325 "bison-chpl-lib.cpp"
    break;

  case 690: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3691 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10334 "bison-chpl-lib.cpp"
    break;

  case 691: /* literal_expr: no_loop_attributes TLSBR expr_ls_semi TRSBR  */
#line 3696 "chpl.ypp"
  {
    (yyval.expr) = context->buildNDArray((yyloc), (yyvsp[-1].ndArrayList));
    context->popLoopAttributeGroup();
  }
#line 10343 "bison-chpl-lib.cpp"
    break;

  case 692: /* literal_expr: no_loop_attributes TLSBR expr_ls_semi semicolon_list TRSBR  */
#line 3701 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    (yyval.expr) = context->buildNDArray((yyloc), (yyvsp[-2].ndArrayList));
    context->popLoopAttributeGroup();
  }
#line 10355 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: no_loop_attributes TLSBR expr_ls semicolon_list TRSBR  */
#line 3709 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    AstList lst;
    lst.push_back(ArrayRow::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))));
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), std::move(lst)).release();
    context->popLoopAttributeGroup();
  }
#line 10369 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA semicolon_list TRSBR  */
#line 3719 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    AstList lst;
    lst.push_back(ArrayRow::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-3].exprList))));
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), std::move(lst)).release();
    context->popLoopAttributeGroup();
  }
#line 10383 "bison-chpl-lib.cpp"
    break;

  case 695: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3729 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10392 "bison-chpl-lib.cpp"
    break;

  case 696: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3734 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10401 "bison-chpl-lib.cpp"
    break;

  case 697: /* assoc_expr_ls: expr TALIAS expr  */
#line 3743 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10410 "bison-chpl-lib.cpp"
    break;

  case 698: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3748 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10420 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TPLUS expr  */
#line 3756 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10426 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TMINUS expr  */
#line 3757 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10432 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TSTAR expr  */
#line 3758 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10438 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TDIVIDE expr  */
#line 3759 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10444 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3760 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10450 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3761 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10456 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TMOD expr  */
#line 3762 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10462 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TEQUAL expr  */
#line 3763 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10468 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3764 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10474 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3765 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10480 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3766 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10486 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TLESS expr  */
#line 3767 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10492 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TGREATER expr  */
#line 3768 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10498 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TBAND expr  */
#line 3769 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10504 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TBOR expr  */
#line 3770 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10510 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TBXOR expr  */
#line 3771 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10516 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TAND expr  */
#line 3772 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10522 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TOR expr  */
#line 3773 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10528 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TEXP expr  */
#line 3774 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10534 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TBY expr  */
#line 3775 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10540 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TALIGN expr  */
#line 3776 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10546 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr THASH expr  */
#line 3777 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10552 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr TDMAPPED expr  */
#line 3778 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10558 "bison-chpl-lib.cpp"
    break;

  case 722: /* unary_op_expr: TPLUS expr  */
#line 3782 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10564 "bison-chpl-lib.cpp"
    break;

  case 723: /* unary_op_expr: TMINUS expr  */
#line 3783 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10570 "bison-chpl-lib.cpp"
    break;

  case 724: /* unary_op_expr: TMINUSMINUS expr  */
#line 3784 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10576 "bison-chpl-lib.cpp"
    break;

  case 725: /* unary_op_expr: TPLUSPLUS expr  */
#line 3785 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10582 "bison-chpl-lib.cpp"
    break;

  case 726: /* unary_op_expr: TBANG expr  */
#line 3786 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10588 "bison-chpl-lib.cpp"
    break;

  case 727: /* unary_op_expr: expr TBANG  */
#line 3787 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10596 "bison-chpl-lib.cpp"
    break;

  case 728: /* unary_op_expr: TBNOT expr  */
#line 3790 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10602 "bison-chpl-lib.cpp"
    break;

  case 729: /* reduce_expr: expr TREDUCE expr  */
#line 3795 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10610 "bison-chpl-lib.cpp"
    break;

  case 730: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3799 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10618 "bison-chpl-lib.cpp"
    break;

  case 731: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3803 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10626 "bison-chpl-lib.cpp"
    break;

  case 732: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3807 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10634 "bison-chpl-lib.cpp"
    break;

  case 733: /* scan_expr: expr TSCAN expr  */
#line 3814 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10642 "bison-chpl-lib.cpp"
    break;

  case 734: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3818 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10650 "bison-chpl-lib.cpp"
    break;

  case 735: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3822 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10658 "bison-chpl-lib.cpp"
    break;

  case 736: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3826 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10666 "bison-chpl-lib.cpp"
    break;


#line 10670 "bison-chpl-lib.cpp"

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
