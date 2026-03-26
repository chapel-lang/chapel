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
#define YYLAST   17824

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  179
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  205
/* YYNRULES -- Number of rules.  */
#define YYNRULES  745
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1260

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
    1459,  1466,  1467,  1468,  1472,  1473,  1477,  1484,  1494,  1498,
    1504,  1504,  1511,  1511,  1521,  1522,  1523,  1524,  1525,  1526,
    1527,  1531,  1536,  1541,  1546,  1553,  1561,  1562,  1566,  1579,
    1587,  1595,  1605,  1624,  1623,  1633,  1632,  1645,  1652,  1658,
    1659,  1668,  1669,  1670,  1671,  1675,  1697,  1715,  1733,  1757,
    1762,  1767,  1772,  1777,  1782,  1787,  1792,  1797,  1802,  1807,
    1812,  1817,  1822,  1827,  1833,  1842,  1851,  1855,  1859,  1863,
    1867,  1871,  1875,  1879,  1883,  1888,  1893,  1898,  1906,  1921,
    1939,  1946,  1953,  1963,  1964,  1969,  1974,  1975,  1976,  1977,
    1978,  1979,  1980,  1981,  1982,  1983,  1984,  1985,  1990,  1995,
    1996,  1997,  1998,  2006,  2007,  2011,  2015,  2019,  2026,  2030,
    2034,  2041,  2045,  2049,  2053,  2060,  2061,  2065,  2069,  2073,
    2080,  2095,  2113,  2121,  2126,  2136,  2137,  2141,  2145,  2150,
    2159,  2164,  2168,  2175,  2176,  2180,  2190,  2197,  2210,  2217,
    2218,  2219,  2223,  2224,  2228,  2232,  2236,  2240,  2244,  2252,
    2272,  2282,  2296,  2303,  2308,  2315,  2314,  2324,  2330,  2329,
    2344,  2352,  2364,  2368,  2369,  2371,  2376,  2377,  2381,  2382,
    2386,  2389,  2391,  2396,  2397,  2408,  2429,  2428,  2448,  2447,
    2465,  2475,  2472,  2504,  2517,  2530,  2544,  2558,  2571,  2586,
    2587,  2595,  2596,  2597,  2606,  2607,  2608,  2609,  2610,  2611,
    2612,  2613,  2614,  2615,  2616,  2617,  2618,  2619,  2620,  2621,
    2622,  2623,  2624,  2625,  2626,  2627,  2628,  2629,  2630,  2634,
    2635,  2636,  2637,  2638,  2639,  2640,  2641,  2642,  2643,  2644,
    2645,  2646,  2647,  2652,  2653,  2657,  2658,  2662,  2663,  2667,
    2668,  2672,  2673,  2677,  2681,  2685,  2690,  2695,  2700,  2705,
    2713,  2717,  2725,  2726,  2727,  2728,  2729,  2730,  2731,  2732,
    2733,  2734,  2738,  2739,  2740,  2741,  2742,  2743,  2744,  2745,
    2746,  2747,  2748,  2752,  2753,  2754,  2758,  2759,  2760,  2761,
    2762,  2763,  2764,  2765,  2766,  2767,  2768,  2772,  2773,  2776,
    2777,  2781,  2782,  2786,  2791,  2792,  2794,  2796,  2798,  2803,
    2805,  2810,  2812,  2814,  2816,  2818,  2820,  2822,  2827,  2828,
    2829,  2833,  2842,  2848,  2858,  2862,  2869,  2890,  2891,  2896,
    2897,  2898,  2899,  2900,  2904,  2913,  2912,  2923,  2922,  2932,
    2937,  2946,  2950,  2957,  2975,  2994,  2998,  3005,  3007,  3009,
    3016,  3017,  3018,  3022,  3026,  3027,  3028,  3032,  3033,  3037,
    3038,  3042,  3043,  3044,  3045,  3049,  3050,  3054,  3058,  3059,
    3065,  3066,  3070,  3075,  3080,  3088,  3091,  3097,  3098,  3104,
    3108,  3112,  3119,  3120,  3124,  3129,  3138,  3139,  3143,  3144,
    3151,  3152,  3153,  3154,  3155,  3156,  3158,  3160,  3162,  3168,
    3170,  3173,  3175,  3177,  3179,  3181,  3183,  3185,  3187,  3189,
    3191,  3196,  3200,  3204,  3208,  3212,  3216,  3225,  3234,  3246,
    3250,  3254,  3258,  3262,  3266,  3270,  3274,  3281,  3299,  3307,
    3308,  3309,  3310,  3311,  3312,  3316,  3317,  3321,  3325,  3332,
    3339,  3349,  3350,  3354,  3358,  3362,  3369,  3370,  3371,  3372,
    3373,  3374,  3375,  3376,  3377,  3378,  3382,  3384,  3386,  3388,
    3390,  3396,  3403,  3416,  3429,  3443,  3461,  3468,  3473,  3478,
    3483,  3489,  3495,  3525,  3532,  3539,  3540,  3544,  3546,  3547,
    3549,  3551,  3552,  3553,  3554,  3557,  3558,  3559,  3560,  3561,
    3562,  3563,  3564,  3568,  3569,  3573,  3574,  3575,  3579,  3580,
    3581,  3582,  3591,  3592,  3595,  3596,  3600,  3612,  3624,  3631,
    3633,  3635,  3637,  3639,  3641,  3650,  3656,  3660,  3664,  3671,
    3672,  3676,  3677,  3681,  3682,  3683,  3684,  3685,  3686,  3687,
    3692,  3697,  3702,  3707,  3712,  3720,  3730,  3740,  3745,  3754,
    3759,  3768,  3769,  3770,  3771,  3772,  3773,  3774,  3775,  3776,
    3777,  3778,  3779,  3780,  3781,  3782,  3783,  3784,  3785,  3786,
    3787,  3788,  3789,  3790,  3794,  3795,  3796,  3797,  3798,  3799,
    3802,  3806,  3810,  3814,  3818,  3825,  3829,  3833,  3837,  3845,
    3846,  3847,  3848,  3849,  3850,  3851
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

#define YYPACT_NINF (-866)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-746)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -866,    81,  3557,  -866,   -98,    89,  -866,  -866,  -866,  -866,
    -866,  -866,  -866, 11610,    70,   112,   182,  3279,   219, 17536,
      70,   302,   170,   248,   112,  4941, 11610,   204, 17594,  -866,
     379,   401,  -866,  9555,   436,   101,   265,  -866,   450,   423,
   17594, 17594, 17594, 13512, 10754,   464, 11610, 11610,   229,  -866,
     472,   487, 11610,  -866,  3279,  -866, 11610,   488,   381,   203,
     341, 12775,   492, 17652,  -866, 11610,  7682, 11610, 10754,  3279,
     457,   504,   392,  4941,   510, 11610,   514,  6662,  6662,  -866,
     516, 17710,  3279,  -866,   518,  9725,  -866, 11610,  -866, 11610,
    -866,  -866, 13244, 11610,  -866,  9895,  -866,  -866,  -866,  3903,
    7852, 11610,  -866,  4595,  -866,  -866,  -866,  -866, 17260,   560,
    -866,   428,   408,  -866,   206,  -866,  -866,  -866,  -866,  -866,
    -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,  7002,  -866,
    7172,  -866,  -866,    20,  -866,  -866,   235,  -866,   523,  -866,
    -866,  -866,  -866,  -866,  -866,  -866,  -866,   437, 17594,  -866,
     414,  -866,   213,  -866,  -866,  -866,   770,   826,  -866,  -866,
   17594,  -866,  1450,  -866,   420,   424,  -866,  -866,  -866,   422,
     425, 11610,   426,   429,  -866,  -866,  -866, 17079,  2969,   268,
     431,   432,  -866,  -866,   315,  -866,  -866,  -866,  -866,  -866,
     409,  -866,  -866,  -866, 11610,  -866, 17594,  -866, 11610,  -866,
   11610,    65,   531,  -866,  -866,  -866,  -866, 17079,   366,  -866,
    -866,    39,  4941,  -866,  -866,  -866,   434,   174,   438,  -866,
     214,  -866,   445,  -866,   -33, 17079,  8022,  -866,  -866,  -866,
    -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,
    -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,
    -866,  -866,  -866,  -866,  -866, 17594, 17594,   -32, 14194,  -866,
    -866,   512,  8022,   446,   443,  -866,  4941, 13570,    40,     2,
    -866,  4941,  -866,  -866, 15170,  -866,    10, 15604,   283,  -866,
     447,   449,  -866, 15170,   174,   283,  -866,  8022,  2859,  2859,
      23,   -11,  -866,     0,  -866,  -866,  -866,  -866,  7342,  -866,
    -866,   603,  -866,   440,   455,  -866,  -866, 13660,   471,  -866,
    -866, 17079,    16, 11610, 11610,  -866,    21,  -866,  -866, 17079,
     459, 15645,  -866, 15170,   174,   453,  8022,  -866, 17079, 15693,
   11610,  -866,  -866,  -866,  -866,  -866,   174,   461,   250,   250,
    2564,   283,   283,    73,  -866,  -866,  4076,   251, 11610,   473,
     -50,   458,  2564,   590,  -866,  -866, 17594,  -866, 11610,  -866,
    -866,  3903,  -866,    78,  -866,   512,  -866,  -866,   630,   466,
    4249, 11610,  -866, 11610,   566,  -866,  -866, 14844,    36,   437,
   17079,   378,  -866,  4941,   567,  -866,  -866,  9555, 10065, 10924,
    -866,  -866,  -866, 17594,  -866, 17594, 11610,   478,  -866,  1673,
    -866,  -866,  -866,  -866,   171,  -866,  -866,  -866,  -866,  -866,
    -866,  -866,   140,   199,  -866,  -866,  -866,  -866,  -866,  -866,
    -866, 13079,   524,   110,  -866,   495,   239,   372,  -866,   517,
   11610, 11610, 11610, 10924, 10924, 11610,   377, 11610, 11610, 11610,
   11610, 11610,   350, 13244, 11610, 11610, 11610, 11610, 11610, 11610,
   11610, 11610, 11610, 11610, 11610, 11610, 11610, 11610, 11610, 11610,
     595,  -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,
    -866,  -866,  9725,  -866,  -866,  9725,  9725,  8022,  8192, 10924,
   10924, 15795,   498,  -866, 11094, 10924, 17594,  -866,  5976,  -866,
    -866,  -866, 13512,  -866, 11610,  -866,   542,   499,   534,  -866,
    -866,   557,   559,  4941,   650,  4941,  -866,   652, 11610,   622,
     519,  -866,  9725,  -866, 13570,  -866,  -866,    94,  -866, 11780,
     565, 11610, 13512,  -866,  -866, 11610,  -866, 17388, 11610, 11610,
    -866,   214,   521,  -866,  -866,  -866,  -866, 17594,  -866,   770,
   12945, 13570,   545, 11780,  -866, 17079, 17079,  -866,    31,  -866,
      42,  -866,  8022,   525,  -866, 15170,   673,   673,  -866,  -866,
    -866,  -866, 11267,  -866, 15843,  8365,  8535,  -866,  -866,  -866,
   17079, 10924, 10924,  -866,   428,  8705,  -866,   194,  -866,  4422,
    -866,   279, 15888,   287, 14934, 17594,  6489,  6319,  -866,   437,
     527,  -866,  -866,   214,  -866,   580, 17594,    67, 15170,   529,
   13414,   -51,   122, 11610,     4, 15993,   -59,    25, 14676,  -866,
     153,  -866,   -86,   555,  1913,   532,  -866, 17478,   579,   278,
    -866,  -866,  -866,  -866,  6149,  -866,   600,  -866,  -866,   535,
     562,  -866,   568,  -866,   572,   577,   581,   586,   588,  -866,
     589,   591,   592,   593,   596,   599,   604,  -866,  -866,  -866,
    -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,
    -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866, 11610,
     573,   605,   535,  -866,   535,  -866, 11610,  -866, 17594,  -866,
    -866,  -866,  2004,  2004,   370,  -866,   370,  -866,   370, 17039,
     583,   628,  2366,   174,   250,  -866,   380,  -866,  -866,  -866,
    -866,  -866,  2564,  2477,   370,  2101,  2101,  2004,  2101,  2101,
     509,   250,  2477, 17237,   509,   283,   283,   250,  2564,   543,
     544,   550,   551,   571,   564,  -866,   370,  -866,   370, 11610,
    9725,   109,   718, 15250,   587,   582,   201,  -866,  -866,  -866,
    -866,  -866,  -866,  -866,  -866,   155,  -866,  1272, 17119,   418,
     317, 17079,  9725,  -866,  9725, 11610, 11610,   726,  4941,   727,
   16038,  4941, 15210, 17594,  -866,   157,   160, 13570,    40,  -866,
   17079,  8875,  -866, 17079,  -866,  -866,  -866, 17594, 16078, 16196,
   13512,  -866,   545,   618,  -866,   175, 11780,  -866,   193,  -866,
   11610,  4768,  -866,  -866,   601,   186,   594,  -866,  2911,  -866,
    -866,  -866,  -866,  -866,  -866,  -866,  -866,    17,    56, 14754,
    -866,  -866,   185,  -866,  -866,  -866,  -866, 11610,  -866,    63,
   13758, 11610,  -866, 11610,  6489,  6319,  -866,  9045,   430,  9215,
     433,   441,  9385,  7512,   448,   320,  -866,  -866,  -866, 16244,
     624,   610,   608,  -866, 17594, 13512, 11610,   731,  -866,  -866,
   11610, 17079,  4941,   607, 10924, 10238,  -866,  3730,   144,   609,
     632,  -866,   198, 11610, 11437,  -866,  4941,  -866,    67,  5114,
   10924,  -866,    67, 17594,   438,  -866,  9725,   612, 14070,  -866,
    -866, 11610,    46,  -866,  -866,  -866,  -866,  -866,  -866,    13,
     272,  -866, 16284,  -866, 13915,  -866,  -866, 17079,  -866,   613,
     614,  -866,  -866,  -866,  -866,  -866,  -866,   222, 10924, 13414,
   11610, 11950, 10924,  9725,  -866,  -866,  -866,  6832,  -866,  -866,
    -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,
      37, 10924, 10924,  -866,  -866,  -866, 17079, 17079,  4941,  -866,
    4941, 11610,  -866,  4941,   749,   625,   636,  -866,  -866,   565,
    -866,  -866,  -866,   615,   621,   326,   193, 11610,   600,   545,
    -866, 10411,  -866,  -866, 17079,  -866,  -866,  4941, 11610,  -866,
    -866,  -866, 17594,  -866,   651,   438, 10924,  4941, 10924,  9725,
    -866,  -866,  -866,  -866,  -866,   380,  -866,  -866,  -866,  -866,
    -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,  6489,
    -866,  -866, 16402, 15400,   331,   638,  6489,  -866, 11610,  -866,
    -866, 14097,   332,   333, 16442, 11610,   228,  -866,  4941,   -22,
   15445,  -866,   282,  -866, 17079,  -866,  -866,  4941, 10581,  -866,
   17079,  -866, 16490, 17079,  -866,  9555, 17079,    62, 15012,  -866,
    -866,  -866,   237,  -866,  -866,  -866,  -866,  2956,  1795,  -866,
   17079, 17594,   560,   220, 11610,  -866,   659,   634,  -866,  -866,
    -866,   535,   535,  -866,  -866,  -866,   633, 16597, 11610,   777,
   16645,   246,  -866,   565,   247,   262,  -866,  -866, 17079,   776,
    4941,  -866,  -866,  -866,  8022,  8022,  -866,  -866,  -866,  -866,
    -866,   193, 17079,  -866,   108,    53,   640, 11610,  -866,   -15,
   15490,    56, 15092,  -866,   291, 11610,  6489,  6319,  -866,  -866,
    -866, 16685, 17079, 14166,  -866,  -866,  -866,  -866, 15170,  -866,
    -866,  5287,   639,  5460,   641,  -866,  -866, 11610, 14272,  5633,
    -866,    67,  5806,  -866,    67,   653,  1795,  -866,  -866,  -866,
   17478,  -866,  -866,    71,  -866,   -36,  -866,   314, 16763,    66,
     771,  -866,  -866,  -866, 12120, 12290, 16803, 12460, 12630,  -866,
    -866,  4941,  -866,   648,   654,  -866,  -866,  4941,  -866,   438,
   17079,  4941,  4941,  -866, 17079,   355,   660, 11610,  -866,  -866,
    9555,  -866, 17079,  4941,  9555,  -866, 17079,  4941, 17079,  4941,
    9555, 17079,  -866,  9555, 17079,  -866,  -866,  -866,  -866,  -866,
   11610,   220,    71,    71,    71,    71,    71,    71,   220,  -866,
    -866,  -866,  -866, 11610, 11610, 11610, 11610, 11610,  -866,   666,
     668,   580,  -866,  -866,  -866, 17079, 14352,  -866, 14430,  -866,
   16038, 14510, 14588, 17079,  -866,  -866,  -866,  -866,  -866,  -866,
    -866,   686, 16843, 16921, 16961, 17001,  -866,  -866,  4941,  4941,
    4941,  4941, 11610, 11610, 11610, 11610, 16038, 16038, 16038, 16038
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   137,   463,    55,   685,   686,
     687,   681,   682,   208,   595,   126,   158,   567,   166,   569,
     595,   165,   495,   492,   126,     0,   208,   273,   159,   197,
     196,   679,   195,   208,   164,    68,   274,   334,   161,   138,
       0,     0,     0,     0,   208,   168,   208,   208,   616,   588,
     688,   169,   208,   335,   561,   489,   208,     0,     0,   182,
     180,   420,   163,   570,   491,   208,   208,   208,   208,   565,
       0,   167,     0,     0,   139,   208,   680,   208,   208,   482,
     162,   160,   563,   493,   170,   208,   741,   208,   743,   208,
     744,   745,   632,   208,   742,   208,   640,   178,   740,     0,
     208,   208,     4,     0,     5,    10,    11,    12,    48,    51,
      52,    56,     0,    45,    71,    13,    77,    14,    15,    16,
      17,    29,   548,   549,    22,    46,   179,   189,   208,   198,
     653,   190,    18,     0,    31,    30,     0,    47,     0,   268,
      19,   649,    21,    35,    32,    33,   188,   312,     0,   186,
       0,   333,     0,   644,   184,   350,   446,   432,   642,   187,
       0,   494,     0,   185,   658,   638,   637,   641,   552,   550,
       0,   208,   647,   648,   652,   651,   650,     0,   551,     0,
     659,   660,   661,   683,   684,   643,   554,   553,   645,   646,
       0,    28,   569,   159,   208,   138,     0,   570,   208,   160,
     208,     0,     0,   658,   550,   647,   648,   559,   551,   659,
     660,     0,     0,   596,   127,   128,     0,   568,     0,   483,
       0,   490,     0,    20,     0,   530,   208,   134,   140,   151,
     146,   145,   153,   131,   144,   154,   141,   135,   155,   129,
     156,   149,   143,   150,   147,   148,   130,   132,   142,   152,
     157,   136,   322,   133,   207,     0,     0,     0,     0,    70,
      69,    71,   208,     0,     0,   250,     0,     0,     0,     0,
     501,     0,   175,    40,     0,   303,     0,   302,   726,   620,
     617,   618,   619,   497,   562,   727,     7,   208,   332,   332,
     427,   172,   422,   173,   423,   424,   428,   429,   171,   430,
     431,   420,   526,     0,   337,   338,   340,     0,   421,   525,
     342,   513,     0,   208,   208,   176,   644,   635,   657,   636,
       0,     0,    43,     0,   566,     0,   208,    44,   560,     0,
     208,   281,    48,   285,   282,   285,   564,     0,   728,   730,
     630,   725,   724,     0,    74,    78,     0,     0,   208,     0,
       0,   539,   631,     0,     6,   309,     0,   196,   208,   310,
     311,     0,    49,     0,     9,    71,    50,    53,     0,    60,
       0,   208,    72,   208,     0,   495,   194,     0,   681,   312,
     654,   203,   214,     0,   211,   212,   213,   208,   208,   208,
     210,   571,   579,     0,   252,     0,   208,     0,   308,     0,
     444,   445,   443,   358,   448,   453,   456,   447,   451,   450,
     452,   455,     0,   436,   438,   442,   441,   433,   434,   437,
     440,     0,   487,     0,   484,     0,     0,   639,    34,   621,
     208,   208,   208,   208,   208,   208,   729,   208,   208,   208,
     208,   208,     0,   629,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     389,   396,   397,   398,   393,   395,   399,   400,   391,   394,
     392,   390,   208,   402,   401,   208,   208,   208,   208,   208,
     208,     0,     0,   211,   208,   208,     0,   600,     0,    36,
      24,    37,     0,    25,   208,    38,   548,     0,   543,   544,
     547,     0,     0,     0,   241,     0,   389,   237,   208,     0,
       0,    23,   208,    26,     0,   505,   507,     0,   524,   208,
     510,   208,     0,   174,    39,   208,   305,     0,   208,   208,
      41,     0,     0,   183,   181,   425,   426,     0,   421,   446,
     420,     0,   528,   208,   121,   655,   656,   348,     0,   177,
       0,    42,   208,     0,   292,     0,   283,   284,    27,    76,
      75,    79,   208,   689,     0,   208,   208,   677,   675,     8,
     497,   208,   208,   209,    56,   208,    54,     0,    65,     0,
     119,     0,   114,     0,    86,     0,   208,   208,   191,   312,
       0,   199,   205,     0,   202,     0,     0,     0,     0,     0,
     580,     0,     0,   208,     0,   530,     0,     0,     0,   255,
       0,   253,   279,   313,     0,     0,   321,     0,   330,     0,
     323,   454,   449,   518,     0,   520,   457,   439,   435,   407,
     134,   385,   140,   383,   146,   145,   144,   141,   135,   387,
     155,   156,   143,   147,   130,   142,   157,   382,   364,   367,
     365,   366,   388,   377,   368,   381,   373,   371,   384,   372,
     370,   375,   380,   369,   374,   378,   379,   376,   386,   208,
     361,     0,   407,   362,   407,   359,   208,   486,     0,   481,
     500,   499,   721,   720,   723,   732,   731,   736,   735,   717,
     714,   715,   716,   633,   704,   137,     0,   671,   672,   139,
     670,   669,   627,   708,   719,   713,   711,   722,   712,   710,
     702,   707,   709,   718,   701,   705,   706,   703,   628,     0,
       0,     0,     0,     0,     0,   734,   733,   738,   737,   208,
     208,     0,     0,     0,   279,     0,   606,   607,   613,   612,
     614,   609,   615,   611,   598,     0,   601,     0,     0,     0,
       0,   531,   208,   556,   208,   208,   208,   240,     0,   236,
       0,     0,     0,     0,   555,     0,     0,   508,     0,   523,
     522,   208,   503,   626,   502,   304,   301,     0,     0,     0,
       0,   668,   528,   343,   339,     0,   208,   529,   510,   341,
     208,     0,   461,   347,     0,     0,     0,   557,     0,   286,
     690,   634,   676,   540,   539,   678,   541,     0,     0,     0,
      57,    58,     0,    61,    63,    67,    66,   208,    97,     0,
       0,   208,    92,   208,   208,   208,    64,   208,   364,   208,
     365,   366,   208,   208,   376,     0,   403,   404,    81,    80,
      91,     0,     0,   314,     0,     0,   208,     0,   217,   216,
     208,   581,     0,     0,   208,   208,   537,     0,   532,     0,
     535,   693,   534,   208,   208,   697,     0,   225,     0,     0,
     208,   223,     0,     0,     0,   276,   208,     0,   332,   320,
     327,   208,   325,   319,   516,   515,   514,   517,   458,   464,
     420,   357,     0,   363,     0,   353,   354,   488,   485,     0,
       0,   125,   124,   123,   122,   666,   667,     0,   208,   691,
     208,   208,   208,   208,   599,   608,   610,   208,   597,   158,
     166,   165,   164,   161,   168,   169,   163,   167,   162,   170,
       0,   208,   208,   496,   546,   545,   249,   248,     0,   243,
       0,   208,   239,     0,   245,     0,   278,   506,   509,   510,
     511,   512,   300,     0,     0,     0,   510,   208,   457,   528,
     527,   208,   416,   414,   349,   462,   294,     0,   208,   293,
     296,   558,     0,   287,   290,     0,   208,     0,   208,   208,
      59,   120,   117,    98,   109,   104,   103,   102,   111,    99,
     112,   107,   101,   108,   105,   106,   100,   110,   113,   208,
     116,   115,    88,    87,     0,     0,   208,   192,   208,   193,
     314,   332,     0,     0,     0,   208,     0,   233,     0,     0,
       0,   692,   533,   231,   582,   538,   695,     0,   208,   694,
     699,   698,     0,   575,   226,   208,   574,     0,     0,   224,
     254,   251,     0,   307,   182,   180,   306,   332,   332,   315,
     331,     0,     0,     0,   208,   351,   412,     0,   409,   413,
     360,   407,   407,   673,   674,   278,     0,     0,   208,     0,
       0,     0,   602,   510,   658,   658,   242,   238,   587,   244,
       0,    73,   275,   504,   208,   208,   498,   417,   415,   344,
     345,   510,   406,   298,     0,     0,     0,   208,   288,     0,
       0,     0,     0,    62,     0,   208,   208,   208,    93,    95,
      84,    83,    82,   332,   200,   206,   204,   215,     0,   235,
     234,     0,     0,     0,     0,   696,   232,   208,     0,     0,
     221,     0,     0,   219,     0,   280,   332,   317,   316,   326,
       0,   478,   479,     0,   480,   466,   469,     0,   465,     0,
     420,   408,   355,   356,   208,   208,     0,   208,   208,   280,
     603,     0,   247,     0,     0,   419,   418,     0,   297,     0,
     291,     0,     0,   118,    89,     0,     0,   208,   201,   218,
     208,   229,   584,     0,   208,   227,   583,     0,   700,     0,
     208,   573,   222,   208,   572,   220,   277,   318,   329,   477,
     208,     0,     0,     0,     0,     0,     0,     0,     0,   459,
     352,   460,   410,   208,   208,   208,   208,   208,   246,   622,
     623,   299,   289,    94,    96,    85,     0,   230,     0,   228,
     578,     0,     0,   468,   470,   471,   474,   475,   476,   472,
     473,   467,     0,     0,     0,     0,   624,   625,     0,     0,
       0,     0,   208,   208,   208,   208,   586,   585,   577,   576
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -866,  -866,  -866,    -1,   -58,  1825,  -866,   271,   728,  -866,
    -866,   477,  -390,  -101,  -866,   261,  -866,  -866,  -141,  -866,
    -866,    44,   578,  -866,  -542,   841,   474,  -573,  -866,  -798,
    -866,  -866,  -866,    24,  -866,  -866,  -866,   818,  -866,   217,
    -215,  -866,  -866,  -460,   395,  -866,  -843,  -866,  -866,   270,
    -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,  -866,     5,
    -866,  -151,   480,  -341,  -866,   460,  -866,  -866,  -866,   -21,
    1362,  -866,  -866,  -866,  -866,   528,  -866,  -118,  -866,  -866,
    -866,  -866,   340,  -866,  -866,  -866,   -99,  -866,  -346,  -797,
    -866,  -866,  -866,  -866,  -866,  -605,  -866,  -866,  -866,  -866,
     318,  -866,  1726,  -866,  -866,  -866,  -866,   485,  -866,  -866,
    -866,  -866,   -26,  -403,  -168,  -784,  -865,  -652,  -866,  -866,
    -278,  -862,   574,   -47,  -866,  -866,   337,   -78,  -866,  -268,
    -866,  -866,  -326,  -315,  -563,  -866,  -866,    47,   210,  -866,
     365,  -195,   765,  -866,  -866,  -160,   374,   127,  -388,  -762,
    -603,  -866,  -866,  -866,  -714,  -509,  -866,  -745,   -10,  -866,
    -866,  -572,     3,  -866,  -258,  -497,  -558,    -2,  -866,  -866,
    -866,  -866,  -866,  -866,    98,  -866,   877,  -474,  -866,   -19,
    -866,  -866,   129,   454,  -866,  -866,  -866,  -866,  2301,  -866,
     -61,   730,  -866,  1006,  1331,  -866,  -866,  -866,  -866,  -866,
    -866,  -866,  -866,  -866,  -469
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   345,   103,   104,   105,   106,   107,   363,
     364,   108,   109,   110,   111,   369,   576,   812,   813,   112,
     113,   114,   374,   115,   116,   117,   346,   840,   583,   841,
     118,   119,  1000,   580,   581,   120,   121,   216,   251,   268,
     122,   253,   123,   302,   530,   124,   125,   289,   288,   126,
     127,   128,   129,   130,   592,   131,   593,   844,   132,   201,
     134,   389,   390,   601,   135,   257,   136,   137,   610,   611,
     202,   139,   140,   141,   142,   556,   799,   975,   143,   144,
     795,   970,   275,   276,   145,   146,   147,   148,   397,   878,
     149,   150,   619,  1051,  1052,   620,   151,   152,   303,   304,
     305,   958,   153,   317,   548,   790,   154,   155,  1149,   156,
     157,   671,   672,   836,   837,   838,   962,   891,  1056,  1057,
    1058,   306,   307,   538,   421,   403,   412,   889,  1210,   793,
     158,  1055,  1145,  1146,  1147,   159,   160,   423,   424,   677,
     161,   162,   163,   220,   531,   269,   270,   516,   517,   772,
     309,   887,   625,   626,   520,   310,   787,   788,   347,   603,
     604,   858,   349,   350,   497,   498,   499,   203,   165,   166,
     391,   167,   392,   168,   204,   170,   212,   213,   745,   746,
     747,   171,   205,   206,   174,   175,   176,   318,   177,   381,
     500,   208,   179,   209,   210,   182,   183,   184,   185,   606,
     186,   187,   188,   189,   190
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     164,   102,   426,   301,   510,   320,   792,   133,   367,   617,
     476,   496,   880,   835,   308,   765,   224,   814,   673,   749,
     895,   886,   896,   164,   337,   492,   963,  1005,  1059,   532,
     133,   379,   862,   590,   789,  1049,  1001,   956,   518,   351,
     487,   518,   271,   794,   382,  -273,  1011,   496,   607,   521,
     485,  -328,   383,  -346,   949,   312,  -274,   866,   355,   769,
     384,   385,   386,   791,   976,   191,   695,   211,   553,   503,
     535,   164,   496,  1053,  1141,   164,   164,   875,   133,   261,
    1200,     3,   133,   133,   876,   271,   864,  -521,   271,   382,
     508,  1087,   685,   687,  1129,   566,   211,   164,   791,   271,
     169,   164,   382,   211,   133,   483,   385,   386,   133,  1201,
     383,   496,   494,   536,   865,   214,  -295,   195,   384,   385,
     386,   567,   852,   169,   359,  1142,   766,   853,   859,  1054,
     495,   172,   382,   868,   872,   211,   361,   387,   725,   727,
    1167,   623,   547,   211,   732,  -256,   360,   522,   384,   385,
     386,  1121,   365,   785,   172,   525,   908,  -295,  1171,   226,
    -521,   494,   494,  -521,   885,   699,   215,   856,  1049,   854,
     262,   169,  -519,  1144,   211,   169,   169,   861,   361,   544,
     211,   519,  -521,   259,   519,  -521,   260,  1083,   211,  -346,
     977,  -324,   388,   361,  1088,   387,   935,   169,   494,   361,
    -519,   169,   172,  -521,  1137,  1138,   172,   172,  -521,   488,
     164,  -295,   572,  1113,  1091,  -324,  1073,   133,   621,   723,
     724,   361,  1110,  1141,   361,   387,  1165,   701,   172,  1209,
     361,   808,   172,   907,   361,   361,   191,   484,  -257,   767,
     211,    23,   559,   842,  -519,   252,   627,   279,   915,   -70,
     571,  1004,   371,   674,   855,   678,  -519,   264,   265,   266,
     967,   622,   496,   496,   164,   768,   400,   855,   -70,   164,
    1049,   133,   856,   679,  1142,  -263,   133,   960,   219,   589,
     388,   432,   909,  1022,   624,   856,   401,   -70,  1059,   628,
     372,   916,    55,  1197,   796,   857,   402,   496,   873,   290,
     917,   968,   754,  -519,    64,   767,   280,  1025,  -519,  1176,
     169,  1160,  1143,   436,   432,   -70,   771,  1026,   373,   292,
     767,   442,  1144,   294,   874,   281,   918,    83,   946,  1166,
     979,   947,   750,   859,   868,   872,   780,   496,   221,   433,
     282,   172,   961,   434,   164,   561,   959,   295,   331,   334,
     296,   133,    57,   695,  1089,   969,   980,   191,  -262,   164,
     496,  1025,   297,   815,   169,   398,   133,   754,   164,   169,
     696,  1029,   433,   494,   226,   133,   434,   422,   602,  1042,
     299,   164,   754,   697,   522,   300,   613,   -69,   133,   436,
     254,   754,  -605,  1065,   440,   172,   562,   442,   845,  1119,
     172,   445,   681,   361,   195,   393,   -69,  -604,  1135,  1152,
    1153,   720,   698,   482,   721,   722,  1071,  1159,  -605,   675,
     563,   814,   436,   882,   817,   -69,  1104,   440,    37,   441,
     442,  -332,   821,  -604,   445,   262,  1006,  1202,   477,   273,
     478,   452,   818,  -411,   169,  1025,  1139,   883,   749,   458,
     822,  -332,   699,   -69,    53,  1125,   173,  -271,   700,   169,
    1173,  -332,   522,   322,  1203,  1006,  1204,  1205,   169,  1206,
    1207,   522,   501,   502,   731,   172,  1006,   678,   522,   173,
     933,   169,   301,  1007,   515,  -665,   591,  -665,  1048,  1086,
     172,   673,  -261,   308,  1108,  1115,  1116,   286,   479,   172,
    1006,   164,   480,   164,   804,   804,  -258,   932,   133,   436,
     133,   480,   172,  1019,   440,   496,   367,   442,  1223,  -743,
    -265,   445,  -744,  -743,   542,   263,  -744,   173,  -269,  1037,
    -745,   173,   173,  1175,  -745,  1198,  -662,  -740,  -662,   496,
     432,  -740,  -664,  -266,  -664,  1122,  1124,  -663,  -260,  -663,
     899,   287,   900,   173,   332,   332,   325,   173,   533,   534,
    -264,   807,   326,  1131,  1134,     7,  -270,  1066,   803,   806,
    -272,  1069,  -259,   252,  -267,   368,   370,   164,   561,   395,
    1199,   396,   399,  -590,   133,  -589,   427,   486,   428,  -593,
     685,   725,  -594,   860,  -592,  -591,   372,   490,   433,   569,
     540,   169,   434,   169,   982,   701,   361,   792,   493,   511,
     609,   539,   612,   512,   432,   543,   618,   528,   565,   529,
     955,  1048,   549,   552,   558,  1122,  1124,  1131,  1134,   568,
     290,   585,   172,   574,   172,  1099,   575,  1101,   670,  1235,
    1236,  1237,  1238,  1239,  1240,   596,   614,   676,   436,   437,
     292,   438,   439,   440,   294,   441,   442,  1136,   680,   432,
     445,   496,  1140,   719,   442,   752,   173,   452,   730,   524,
     753,   526,   433,   456,   457,   458,   434,   169,   295,   754,
     755,   296,   756,   353,   758,  1013,   761,   763,   771,   786,
     764,   934,   781,   297,   798,   843,   797,   846,   496,   850,
     494,   879,   881,   734,   888,   890,   901,   902,   172,  -137,
     951,   299,   893,   903,   904,  -158,   300,   433,   551,  -166,
     173,   434,   436,  1048,  -165,   173,  1062,   440,  -164,   441,
     442,   515,   178,  -161,   445,  -138,  -168,   906,  -169,  -163,
    -167,   452,   905,  -139,   776,   930,  -162,   456,   457,   458,
     910,  -170,   894,   914,   782,   178,   164,   913,   515,   164,
     938,   940,   957,   133,   496,   971,   133,   436,   437,  1006,
     966,   439,   440,  1009,   441,   442,  1010,  1028,  1015,   445,
    1018,  1043,  1027,  1080,  1063,  1084,   452,  1064,  1081,   164,
     965,  1085,   456,   457,   458,  1097,   133,   404,   290,  1082,
     173,  1109,   826,   178,  1150,  1151,  1154,   178,   178,  1157,
    1161,  1169,  1183,   847,  1187,   173,  1196,   405,   292,  1219,
    1047,   406,   294,  1224,   173,  1220,  1163,  1164,  1246,   178,
    1247,  1201,   301,   178,   618,   810,   362,   173,  1103,   509,
    1016,   981,   222,   573,   579,   407,   295,   597,   408,   296,
     164,    57,  1040,   413,  1096,   164,   169,   133,   784,   169,
     409,   297,   133,   557,   164,   775,   594,   164,  1061,   496,
     496,   133,  1212,   414,   133,   537,   783,   415,   410,   299,
    1090,  1211,  1241,   411,   300,   272,  1234,   172,   898,   169,
     172,  1012,   777,   376,   948,   422,   774,   218,  1072,     0,
       0,   416,     0,     0,   417,     0,     0,     0,     0,   272,
       0,     0,     0,     0,     0,     0,   418,     0,   333,   335,
     172,     0,     0,     0,     0,     0,     0,     0,     0,  1074,
    1075,     0,     0,     0,   419,     0,   164,     0,   164,   420,
       0,   164,   178,   133,     0,   133,     0,   367,   133,   366,
     169,     0,     0,  1047,     0,   169,     0,   173,  1095,   173,
     508,     0,     0,     0,   169,   164,     0,   169,     0,     0,
       0,     0,  1094,     0,     0,   164,     0,   394,     0,     0,
     945,   172,   133,     0,   515,     0,   172,     0,     0,     0,
       0,     0,   848,   849,   952,   172,   178,     0,   172,     0,
       0,   178,   867,   871,     0,     0,     0,     0,   180,     0,
       0,     0,     0,     0,     0,   974,   164,     0,     0,     0,
       0,     0,     0,   133,     0,   164,     0,     0,     0,     0,
       0,   180,   133,   173,     0,   367,   169,     0,   169,   367,
       0,   169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1047,     0,     0,   508,   491,
     508,   422,     0,   508,   508,   169,     0,   172,     0,   172,
       0,     0,   172,     0,     0,   169,   178,     0,   164,   180,
       0,     0,     0,   180,   180,   133,     0,     0,     0,     0,
     609,   178,   301,     0,     0,     0,   172,     0,   504,   507,
     178,     0,     0,     0,     0,   180,   172,     0,     0,   180,
       0,   670,     0,   178,     0,   272,   169,   272,     0,   164,
       0,   164,     0,     0,   272,   169,   133,   164,   133,     0,
     164,     0,     0,     0,   133,     0,     0,   133,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   172,     0,     0,
       0,     0,     0,     0,     0,     0,   172,     0,     0,   164,
       0,     0,     0,     0,   272,   164,   133,     0,     0,   164,
     164,     0,   133,     0,     0,     0,   133,   133,   169,     0,
       0,   164,     0,     0,     0,   164,     0,   164,   133,   974,
       0,     0,   133,     0,   133,     0,     0,     0,     0,     0,
       0,     0,     0,   867,   871,     0,     0,     0,     0,   172,
       0,     0,   173,     0,     0,   173,     0,     0,   180,   169,
       0,   169,     0,     0,     0,     0,     0,   169,     0,     0,
     169,     0,     0,   178,     0,   178,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   173,   164,   164,   164,   164,
     172,     0,   172,   133,   133,   133,   133,     0,   172,   169,
       0,   172,     0,  1034,     0,   169,     0,  1039,   618,   169,
     169,     0,   180,     0,     0,   695,     0,   180,     0,     0,
       0,   169,     0,     0,     0,   169,     0,   169,     0,   919,
     172,     0,   920,     0,     0,     0,   172,   921,     0,     0,
     172,   172,     0,     0,     0,     0,   173,   193,     0,   178,
       0,   173,   172,     0,     0,     0,   172,   922,   172,     0,
     173,     0,     0,   173,   923,     0,   195,     0,     0,     0,
       0,     0,     0,   181,   924,     0,     0,     0,     0,     0,
       0,     0,   925,     0,     0,     0,   169,   169,   169,   169,
       0,     0,   180,     0,     0,     0,   181,   618,     0,   926,
       0,     0,     0,     0,   138,     0,     0,   180,     0,     0,
     927,     0,     0,     0,   699,     0,   180,   172,   172,   172,
     172,   928,   199,     0,     0,     0,   929,   138,     0,   180,
       0,     0,   173,     0,   173,     0,   272,   173,     0,     0,
       0,     0,     0,     0,   181,     0,     0,     0,   181,   181,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   173,     0,     0,     0,     0,     0,     0,     0,     0,
     181,   173,  1130,  1133,   181,   138,     0,     0,   272,   272,
       0,     0,     0,     0,     0,     0,     0,     0,   272,   272,
       0,   425,     0,   227,     0,     0,     0,     0,     0,     0,
       0,   138,     0,     0,     0,   138,     0,   228,   229,     0,
     230,     0,   173,     0,     0,   231,     0,     0,     0,     0,
       0,   173,     0,   232,     0,     0,     0,     0,   178,   233,
    1168,   178,     0,     0,     0,   234,  1130,  1133,     0,   235,
       0,     0,   236,     0,   237,     0,     0,     0,     0,   180,
       0,   180,   238,  1179,     0,     0,     0,     0,     0,   239,
     240,   178,     0,     0,     0,     0,  1192,   241,     0,  1195,
       0,     0,     0,     0,   173,     0,     0,   242,     0,     0,
       0,     0,     0,   181,     0,     0,   243,     0,   244,     0,
     245,     0,   246,     0,     0,   247,     0,     0,     0,   248,
       0,   249,     0,     0,   250,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   138,   173,     0,   173,     0,     0,
       0,     0,   178,   173,     0,   180,   173,   178,     0,     0,
       0,     0,     0,     0,     0,     0,   178,   181,     0,   178,
       0,     0,   181,   944,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   173,     0,     0,     0,     0,
     267,   173,     0,     0,     0,   173,   173,     0,   138,     0,
       0,     0,     0,   138,     0,     0,     0,   173,     0,   973,
       0,   173,     0,   173,     0,     0,     0,     0,     0,   272,
     272,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   178,     0,
     178,     0,     0,   178,   615,     0,   227,   181,     7,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     228,   229,   181,   230,     0,     0,     0,   178,   231,     0,
       0,   181,   173,   173,   173,   173,   232,   178,   138,   272,
       0,     0,   233,   272,   181,  1041,     0,     0,   234,     0,
       0,     0,   235,   138,     0,   236,     0,   237,     0,     0,
       0,     0,   138,     0,     0,   238,     0,     0,     0,     0,
       0,     0,   239,   240,     0,   138,     0,     0,   178,     0,
     241,     0,     0,     0,     0,     0,     0,   178,     0,     0,
     242,     0,     0,     0,   180,     0,     0,   180,     0,   243,
       0,   244,     0,   245,     0,   246,     0,     0,   247,     0,
       0,     0,   248,     0,   249,     0,     0,   250,     0,     0,
       0,     0,   316,     0,     0,     0,     0,   180,     0,     0,
       7,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     178,   316,     0,     0,     0,     0,  1098,   355,     0,     0,
       0,    22,    23,     0,     0,     0,   316,     0,     0,     0,
     356,     0,    29,   357,   181,     0,   181,     0,    32,     0,
       0,     0,   616,     0,     0,    37,     0,     0,     0,     0,
     223,   178,     0,   178,     0,     0,     0,     0,   180,   178,
       0,     0,   178,   180,     0,   138,   358,   138,     0,     0,
       0,    53,   180,    55,     0,   180,     0,  1044,   272,   272,
    1045,     0,     0,   359,     0,    64,     0,     0,     0,     0,
       0,   178,     0,     0,     0,     0,     0,   178,   327,     0,
       0,   178,   178,    79,     0,   360,     0,     0,    83,     0,
     181,     0,     0,   178,   877,     0,     0,   178,  -314,   178,
       0,     0,     0,     0,     0,     0,     0,     0,   354,     0,
       0,     0,     0,     0,     0,  -314,   272,     0,     0,  -314,
    -314,   138,   272,   272,   180,     0,   180,     0,  -314,   180,
    -314,  -314,   316,     0,     0,     0,  -314,     0,    97,   272,
       0,     0,     0,  -314,     0,     0,  -314,     0,     0,   507,
       0,     0,   272,   180,     0,   272,     0,     0,   178,   178,
     178,   178,     0,   180,  -314,     0,  -314,     0,   316,  -314,
       0,  -314,     0,  -314,     0,  -314,  -314,     0,  -314,     0,
       0,  -314,     0,  -314,     0,     0,     0,     0,     0,     0,
    1222,     0,     0,   316,     0,     0,     0,     0,     0,     0,
       0,  -314,     0,  -314,   180,     0,  -314,     0,     0,     0,
       0,     0,     0,   180,     0,   432,     0,   489,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   507,     0,   507,
       0,     0,   507,   507,     0,     0,  -314,     0,     0,     0,
       0,     0,  -314,     0,     0,     0,   180,     0,     0,   181,
       0,   513,   181,   433,     0,     0,   523,   434,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     138,     0,   181,   138,     0,     0,   435,   180,     0,   180,
       0,     0,   432,     0,     0,   180,     0,     0,   180,     0,
       0,     0,     0,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   138,   444,   445,   446,   447,     0,   449,
     450,   451,   452,   453,   454,   455,     0,   180,   456,   457,
     458,     0,     0,   180,     0,     0,     0,   180,   180,   459,
       0,     0,     0,   181,     0,     0,     0,     0,   181,   180,
     433,     0,     0,   180,   434,   180,     0,   181,   316,     0,
     181,   316,   316,   316,   316,     0,     0,     0,   595,     0,
       0,     0,     0,     0,   138,     0,     0,     0,     0,   138,
       0,     0,     0,     0,     0,     0,     0,     0,   138,     0,
       0,   138,     0,     0,     0,     0,     0,     0,   316,     0,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
       0,     0,   445,     0,   180,   180,   180,   180,   451,   452,
       0,     0,   455,     0,     0,   456,   457,   458,     0,   181,
       0,   181,     0,     0,   181,     0,   459,     0,   316,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   316,   316,     0,     0,     0,     0,     0,   181,     0,
     138,   316,   138,     0,     0,   138,     0,     0,   181,     0,
       0,     0,     0,     0,   207,     0,     0,     0,   217,     0,
       0,     0,     0,     0,     0,     0,     0,   225,   757,   138,
     759,     0,     0,     0,   258,     0,     0,     0,     0,   138,
       0,     0,     0,     0,     0,   274,     0,   277,   278,   181,
       0,     0,     0,   283,     0,   284,     0,   285,   181,     0,
       0,     0,   311,     0,     0,     0,   225,   319,   321,   323,
     324,     0,     0,     0,   328,     0,   329,     0,     0,     0,
     138,     0,     0,   336,     0,     0,   319,     0,   338,   138,
     339,     0,     0,   340,   341,     0,   342,   432,     0,     0,
     225,   319,   352,     0,     0,     0,     0,     0,     0,     0,
       0,   181,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   377,
       0,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   138,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   181,     0,   181,   433,   316,     0,     0,   434,
     181,     0,     0,   181,     0,     0,     0,     0,     0,     0,
       0,     0,   429,     0,     0,     0,     0,     0,   316,     0,
     316,     0,     0,   138,     0,   138,     0,     0,     0,     0,
       0,   138,   181,     0,   138,   481,     0,   316,   181,   328,
       0,   225,   181,   181,     0,   436,   437,     0,   432,     0,
     440,     0,   441,   442,   181,     0,     0,   445,   181,     0,
     181,     0,     0,   138,   452,     0,     0,   319,     0,   138,
     456,   457,   458,   138,   138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   138,     0,     0,     0,   138,
       0,   138,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,     0,   433,     0,     0,     0,
     434,     0,     0,     0,     0,     0,     0,     0,     0,   181,
     181,   181,   181,   939,     0,     0,   942,     0,   319,     0,
       0,     0,     0,     0,     0,   432,     0,     0,     0,   328,
       0,     0,   316,     0,     0,     0,     0,     0,     0,     0,
     138,   138,   138,   138,   545,   546,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,     0,   319,   445,   446,
     447,   555,   449,   450,   451,   452,     0,     0,   455,   316,
       0,   456,   457,   458,     0,     0,     0,     0,     0,   564,
       0,     0,   459,   433,     0,     0,     0,   434,     0,   570,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   582,     0,   584,     0,     0,  1017,     0,     0,
       0,     0,  1023,     0,     0,     0,     0,     0,   598,   605,
     608,   523,     0,     0,   523,     0,     0,   225,     0,     0,
       0,     0,     0,   436,   437,   316,   438,   439,   440,     0,
     441,   442,     0,     0,     0,   445,     0,     0,     0,     0,
       0,   451,   452,     0,     0,   455,     0,     0,   456,   457,
     458,   682,   683,   684,   686,   688,   689,     0,   690,   691,
     692,   693,   694,     0,   702,   703,   704,   705,   706,   707,
     708,   709,   710,   711,   712,   713,   714,   715,   716,   717,
     718,     0,     0,  1076,     0,  1077,     0,     0,  1079,     0,
       0,     0,     0,   319,     0,     0,   319,   319,   319,   319,
     726,   728,     0,     0,     0,   605,   733,     0,     0,   748,
       0,     0,  1093,     0,     0,   751,     0,     0,     0,     0,
       0,     0,  1023,     0,     0,     0,   760,     0,     0,   762,
     316,   316,     0,   319,     0,     0,     0,     0,     0,     0,
     770,     0,   773,     0,     0,     0,   277,     0,     0,   778,
     779,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,     0,  1120,   311,     0,     0,     0,     0,     0,
       0,     0,  1126,   319,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   751,     0,     0,   319,   319,     0,     0,
       0,     0,   225,   809,     0,     0,   319,     0,     0,     0,
       0,   355,     0,     0,     0,    22,    23,   839,   839,     0,
       0,     0,     0,     0,   356,     0,    29,   357,     0,     0,
       0,   851,    32,     0,   225,  1162,     0,     0,     0,    37,
       0,     0,     0,     0,   227,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   311,     0,     0,   228,   229,
     358,   230,     0,     0,     0,    53,   231,    55,     0,     0,
       0,     0,     0,     0,   232,     0,  1181,   359,  1185,    64,
     233,     0,     0,     0,   523,     0,   234,   523,     0,     0,
     235,     7,     0,   236,     0,   237,     0,    79,     0,   360,
     892,     0,    83,   238,     0,     0,     0,   897,   355,     0,
     239,   240,    22,    23,     0,     0,  1218,     0,   241,     0,
       0,   356,  1221,    29,   357,     0,  1181,  1185,   242,    32,
       0,     0,     0,     0,     0,     0,    37,   243,  1227,   244,
       0,   245,  1229,   246,   759,     0,   247,     0,     0,     0,
     248,     0,   249,     0,     0,   250,     0,   358,     0,     0,
     760,   319,    53,     0,    55,     0,   353,     0,  1044,     0,
       0,  1045,     0,     0,   359,     0,    64,     0,     0,     0,
       0,     0,     0,   319,     0,   319,   936,   937,     0,     0,
       0,     0,     0,     0,    79,     0,   360,     0,     0,    83,
       0,     0,   319,   759,   759,   759,   759,     0,     0,   361,
       0,   972,     0,     0,     0,     0,     0,   311,     0,     0,
       0,   964,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   474,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   582,    97,
       0,     0,  1002,     0,  1003,   839,   839,     0,   338,     0,
     339,     0,     0,   341,   342,     0,   475,     0,     0,  -662,
       0,  -662,     0,     0,     0,     0,     0,  1014,     0,     0,
       0,   225,     0,     0,     0,  1020,   751,     0,  1024,     0,
       0,     0,     0,     0,  1030,  1032,     0,  1033,     0,     0,
    1036,  1038,     0,     0,     0,     0,     0,   319,     0,     0,
       0,     0,  1050,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1067,
    1024,  1033,  1036,  1070,   319,     0,     0,     0,   748,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   686,   726,     0,     0,     0,     0,     0,     0,
       0,     0,  1078,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   311,     0,
       0,     0,  1092,     0,     0,     0,     0,     0,     0,   225,
       0,     0,     0,     0,     0,     0,     0,  1100,     0,  1102,
     319,     0,     5,     6,     0,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
     839,   192,     0,  -208,    21,     0,     0,  1111,     0,  1112,
       0,     0,    27,     0,   193,     0,  1118,     0,    31,  -208,
    -208,  -208,     0,   194,    34,     0,     0,     0,    36,   751,
       0,    38,     0,   195,     0,   196,  1128,     0,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,  1148,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   197,     0,  1156,
       0,     0,     0,     0,     0,    69,    70,    71,    72,   198,
       0,    74,     0,     0,    76,   319,   319,     0,    80,   199,
      82,     0,     0,    84,     0,     0,     0,     0,  1170,     0,
       0,     0,     0,     0,     0,     0,  1174,   839,   839,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,     0,
      89,     0,  1182,     0,  1186,     0,     0,     0,  1188,     0,
    1191,     0,     0,  1194,     0,     0,     0,     0,     0,     0,
       0,    96,     0,     0,     0,     0,     0,   200,     0,   100,
       0,  -208,     0,     0,     0,  1182,  1186,     0,  1191,  1194,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1225,     0,
       0,  1226,     0,     0,     0,  1228,     0,     0,     0,     0,
    1230,  1231,     0,     0,  1232,     0,     0,     0,     0,     0,
       0,  1233,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1242,  1243,  1230,  1244,  1245,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1256,
    1257,  1258,  1259,  1256,  1257,  1258,  1259,    -2,     4,     0,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,  -208,    21,    22,    23,    24,    25,    26,     0,  -208,
      27,     0,    28,     0,    29,    30,    31,  -208,  -208,  -208,
      32,    33,    34,   -68,     0,    35,    36,    37,     0,    38,
    -332,    39,     0,    40,    41,    42,    43,     0,    44,    45,
      46,    47,   -68,    48,    49,     0,    50,    51,    52,     0,
    -332,     0,     0,    53,    54,    55,    56,    57,    58,    59,
    -332,   -68,    60,    61,    62,    63,     0,    64,    65,    66,
       0,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,    81,    82,   -68,
      83,    84,     0,     0,  -208,     0,    85,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
      97,     0,     0,    98,     0,    99,     0,   100,     0,  -208,
       0,     4,   101,     5,     6,     7,     8,     9,    10,    11,
      12,     0,  -691,     0,    13,    14,    15,    16,    17,  -691,
      18,     0,    19,    20,  -208,    21,    22,    23,    24,    25,
      26,  -691,  -208,    27,  -691,    28,     0,    29,    30,    31,
    -208,  -208,  -208,    32,    33,    34,   -68,     0,    35,    36,
      37,     0,    38,  -332,    39,     0,    40,    41,    42,    43,
       0,    44,    45,    46,    47,   -68,    48,    49,     0,    50,
      51,    52,     0,  -332,     0,     0,    53,    54,    55,    56,
       0,    58,    59,  -332,   -68,    60,    61,    62,    63,  -691,
      64,    65,    66,  -691,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
      81,    82,   -68,    83,    84,     0,     0,  -208,     0,    85,
       0,     0,  -691,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
    -691,    89,  -691,  -691,  -691,  -691,  -691,  -691,  -691,     0,
    -691,  -691,  -691,  -691,  -691,  -691,  -691,  -691,  -691,  -691,
    -691,  -691,    96,    97,  -691,  -691,  -691,     0,    99,  -691,
     100,     0,  -208,     0,   343,  -691,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,  -208,    21,    22,
      23,    24,    25,    26,     0,  -208,    27,     0,    28,     0,
      29,    30,    31,  -208,  -208,  -208,    32,    33,    34,   -68,
       0,    35,    36,    37,     0,    38,  -332,    39,     0,    40,
      41,    42,    43,     0,    44,    45,    46,    47,   -68,    48,
      49,     0,    50,    51,    52,     0,  -332,     0,     0,    53,
      54,    55,    56,    57,    58,    59,  -332,   -68,    60,    61,
      62,    63,     0,    64,    65,    66,     0,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,    81,    82,   -68,    83,    84,     0,     0,
    -208,     0,    85,     0,     0,    86,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,    88,    89,    90,    91,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,    94,    95,    96,    97,     0,     0,    98,
       0,    99,   344,   100,     0,  -208,     0,     4,   101,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
    -208,    21,    22,    23,    24,    25,    26,     0,  -208,    27,
       0,    28,     0,    29,    30,    31,  -208,  -208,  -208,    32,
      33,    34,   -68,     0,    35,    36,    37,     0,    38,  -332,
      39,     0,    40,    41,    42,    43,     0,    44,    45,    46,
      47,   -68,    48,    49,     0,    50,    51,    52,     0,  -332,
       0,     0,    53,    54,    55,    56,    57,    58,    59,  -332,
     -68,    60,    61,    62,    63,     0,    64,    65,    66,     0,
      67,    68,    69,    70,    71,    72,    73,     0,    74,    75,
       0,    76,    77,    78,    79,    80,    81,    82,   -68,    83,
      84,     0,     0,  -208,     0,    85,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,    97,
       0,     0,    98,     0,    99,   560,   100,     0,  -208,     0,
     577,   101,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -208,    21,    22,    23,    24,    25,    26,
       0,  -208,    27,     0,    28,     0,    29,    30,    31,  -208,
    -208,  -208,    32,    33,    34,   -68,     0,    35,    36,    37,
       0,    38,  -332,    39,     0,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -68,    48,    49,     0,    50,    51,
      52,     0,  -332,     0,     0,    53,    54,    55,    56,    57,
      58,    59,  -332,   -68,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
       0,    74,    75,     0,    76,    77,    78,    79,    80,    81,
      82,   -68,    83,    84,     0,     0,  -208,     0,    85,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,    97,     0,     0,    98,     0,    99,   578,   100,
       0,  -208,     0,     4,   101,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,  -208,    21,    22,    23,
      24,    25,    26,     0,  -208,    27,     0,    28,     0,    29,
      30,    31,  -208,  -208,  -208,    32,    33,    34,   -68,     0,
      35,    36,    37,     0,    38,  -332,    39,     0,    40,    41,
      42,    43,     0,    44,    45,    46,    47,   -68,    48,    49,
       0,    50,    51,    52,     0,  -332,     0,     0,    53,    54,
      55,    56,    57,    58,    59,  -332,   -68,    60,    61,    62,
      63,     0,    64,    65,    66,     0,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,    81,    82,   -68,    83,    84,     0,     0,  -208,
       0,    85,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,    97,     0,     0,    98,     0,
      99,   816,   100,     0,  -208,     0,     4,   101,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,  -208,
      21,    22,    23,    24,    25,    26,     0,  -208,    27,     0,
      28,     0,    29,    30,    31,  -208,  -208,  -208,    32,    33,
      34,   -68,     0,    35,    36,    37,     0,    38,  -332,    39,
       0,    40,    41,    42,    43,     0,    44,    45,    46,    47,
     -68,    48,    49,     0,    50,    51,    52,     0,  -332,     0,
       0,    53,    54,    55,    56,   353,    58,    59,  -332,   -68,
      60,    61,    62,    63,     0,    64,    65,    66,     0,    67,
      68,    69,    70,    71,    72,    73,     0,    74,    75,     0,
      76,    77,    78,    79,    80,    81,    82,   -68,    83,    84,
       0,     0,  -208,     0,    85,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,    97,     0,
       0,    98,     0,    99,     0,   100,     0,  -208,     0,     4,
     101,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,  -208,    21,    22,    23,    24,    25,    26,     0,
    -208,    27,     0,    28,     0,    29,    30,    31,  -208,  -208,
    -208,    32,    33,    34,   -68,     0,    35,    36,    37,     0,
      38,  -332,    39,     0,    40,    41,    42,    43,     0,    44,
      45,    46,    47,   -68,    48,    49,     0,    50,    51,    52,
       0,  -332,     0,     0,    53,    54,    55,    56,    57,    58,
      59,  -332,   -68,    60,    61,    62,    63,     0,    64,    65,
      66,     0,    67,    68,    69,    70,    71,    72,    73,     0,
      74,    75,     0,    76,    77,    78,    79,    80,    81,    82,
     -68,    83,    84,     0,     0,  -208,     0,    85,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,    97,     0,     0,    98,     0,    99,     0,   100,     0,
    -208,     0,     4,   101,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,  -208,    21,    22,    23,    24,
      25,    26,     0,  -208,    27,     0,    28,     0,    29,    30,
      31,  -208,  -208,  -208,    32,    33,    34,   -68,     0,    35,
      36,    37,     0,    38,  -332,    39,     0,    40,    41,    42,
      43,     0,    44,    45,    46,    47,   -68,    48,    49,     0,
      50,    51,    52,     0,  -332,     0,     0,    53,    54,    55,
      56,     0,    58,    59,  -332,   -68,    60,    61,    62,    63,
       0,    64,    65,    66,     0,    67,    68,    69,    70,    71,
      72,    73,     0,    74,    75,     0,    76,    77,    78,    79,
      80,    81,    82,   -68,    83,    84,     0,     0,  -208,     0,
      85,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,    97,     0,     0,    98,     0,    99,
       0,   100,     0,  -208,     0,     4,   101,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -208,    21,
      22,    23,    24,    25,    26,     0,  -208,    27,     0,    28,
       0,    29,    30,    31,  -208,  -208,  -208,    32,  1035,    34,
     -68,     0,    35,    36,    37,     0,    38,  -332,    39,     0,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -68,
      48,    49,     0,    50,    51,    52,     0,  -332,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -332,   -68,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,     0,    74,    75,     0,    76,
      77,    78,    79,    80,    81,    82,   -68,    83,    84,     0,
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
      32,  1180,    34,   -68,     0,    35,    36,    37,     0,    38,
    -332,    39,     0,    40,    41,    42,    43,     0,    44,    45,
      46,    47,   -68,    48,    49,     0,    50,    51,    52,     0,
    -332,     0,     0,    53,    54,    55,    56,     0,    58,    59,
    -332,   -68,    60,    61,    62,    63,     0,    64,    65,    66,
       0,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,    81,    82,   -68,
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
    -208,  -208,  -208,    32,  1184,    34,   -68,     0,    35,    36,
      37,     0,    38,  -332,    39,     0,    40,    41,    42,    43,
       0,    44,    45,    46,    47,   -68,    48,    49,     0,    50,
      51,    52,     0,  -332,     0,     0,    53,    54,    55,    56,
       0,    58,    59,  -332,   -68,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
      81,    82,   -68,    83,    84,     0,     0,  -208,     0,    85,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,    97,     0,     0,    98,     0,    99,     0,
     100,     0,  -208,     0,     4,   101,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,  -208,    21,    22,
      23,    24,    25,    26,     0,  -208,    27,     0,    28,     0,
      29,    30,    31,  -208,  -208,  -208,    32,  1190,    34,   -68,
       0,    35,    36,    37,     0,    38,  -332,    39,     0,    40,
      41,    42,    43,     0,    44,    45,    46,    47,   -68,    48,
      49,     0,    50,    51,    52,     0,  -332,     0,     0,    53,
      54,    55,    56,     0,    58,    59,  -332,   -68,    60,    61,
      62,    63,     0,    64,    65,    66,     0,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,    81,    82,   -68,    83,    84,     0,     0,
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
    1193,    34,   -68,     0,    35,    36,    37,     0,    38,  -332,
      39,     0,    40,    41,    42,    43,     0,    44,    45,    46,
      47,   -68,    48,    49,     0,    50,    51,    52,     0,  -332,
       0,     0,    53,    54,    55,    56,     0,    58,    59,  -332,
     -68,    60,    61,    62,    63,     0,    64,    65,    66,     0,
      67,    68,    69,    70,    71,    72,    73,     0,    74,    75,
       0,    76,    77,    78,    79,    80,    81,    82,   -68,    83,
      84,     0,     0,  -208,     0,    85,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,    97,
       0,     0,    98,     0,    99,     0,   100,   735,  -208,     5,
       6,   101,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   192,     0,
    -208,    21,     0,   736,     0,     0,     0,     0,     0,    27,
       0,   193,     0,     0,     0,    31,  -208,  -208,  -208,     0,
     194,    34,     0,   737,     0,    36,     0,   738,    38,     0,
     195,     0,   196,     0,     0,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,   739,     0,    54,   740,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   197,     0,   741,     0,     0,     0,
       0,     0,    69,    70,    71,    72,   198,     0,    74,     0,
       0,    76,     0,     0,   742,    80,   199,    82,     0,   743,
      84,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    87,    88,    89,    90,    91,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,    94,    95,    96,     0,
       0,     0,    98,     0,   200,     0,   100,   744,  -208,     0,
     884,   101,     5,     6,     0,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   192,     0,  -208,    21,     0,     0,     0,     0,     0,
       0,     0,   291,     0,   193,     0,     0,     0,    31,  -208,
    -208,  -208,     0,   194,    34,     0,     0,     0,   293,     0,
       0,    38,     0,   195,     0,   196,     0,     0,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   197,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,   298,
       0,    74,     0,     0,    76,     0,     0,     0,    80,   199,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,    94,
      95,    96,     0,     0,     0,    98,     0,   200,     0,   100,
       0,  -208,     5,     6,   101,     8,     9,    10,    11,    12,
       0,   631,     0,    13,     0,     0,    16,    17,   633,    18,
       0,   192,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   193,     0,     0,     0,    31,     0,
       0,     0,     0,   194,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   195,   639,   196,     0,     0,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   197,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,   198,
       0,    74,     0,     0,    76,     0,     0,     0,    80,   199,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    86,   506,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,     0,   473,   474,     0,   827,   828,
     829,   830,   831,   652,     0,   653,     0,    92,     0,   654,
     655,   656,   657,   658,   659,   660,   832,   662,   663,    94,
     833,    96,   -90,   665,   666,   834,   668,   200,     0,   100,
       0,     0,     5,     6,   101,     8,     9,    10,    11,    12,
       0,   631,     0,    13,     0,     0,    16,    17,   633,    18,
       0,   192,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   193,     0,     0,     0,    31,     0,
       0,     0,     0,   194,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   195,   639,   196,     0,     0,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   197,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,   198,
       0,    74,     0,     0,    76,     0,     0,     0,    80,   199,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    86,   506,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,     0,   473,   474,     0,   827,   828,
     829,   830,   831,   652,     0,   653,     0,    92,     0,   654,
     655,   656,   657,   658,   659,   660,   832,   662,   663,    94,
     833,    96,     0,   665,   666,   834,   668,   200,     0,   100,
       0,     0,     0,     0,   101,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,     0,    16,
      17,     0,    18,     0,   192,    20,     0,    21,     0,     0,
       0,     0,    26,     0,     0,    27,     0,   193,     0,     0,
       0,    31,     0,     0,     0,     0,    33,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   195,     0,   196,     0,
       0,    43,     0,    44,    45,    46,    47,     0,    48,    49,
       0,    50,    51,   330,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,    62,
     197,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,     0,     0,
       0,    80,   199,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
      99,     0,   100,     0,     0,     5,     6,   101,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   192,     0,     0,    21,     0,   736,
       0,     0,     0,     0,     0,    27,     0,   193,     0,     0,
       0,    31,     0,     0,     0,     0,   194,    34,     0,   737,
       0,    36,     0,   738,    38,     0,   195,     0,   196,     0,
       0,    43,     0,     0,    45,     0,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,   739,     0,    54,
     740,    56,     0,    58,     0,     0,     0,     0,    61,    62,
     197,     0,   741,     0,     0,     0,     0,     0,    69,    70,
      71,    72,   198,     0,    74,     0,     0,    76,     0,     0,
     742,    80,   199,    82,     0,   743,    84,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     200,     0,   100,     0,     0,     5,     6,   101,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   192,     0,     0,    21,   375,    23,
       0,     0,     0,     0,     0,    27,     0,   193,     0,     0,
       0,    31,     0,     0,     0,     0,   194,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   195,     0,   196,     0,
       0,    43,     0,     0,    45,     0,    47,     0,    48,    49,
       0,    50,    51,   358,     0,     0,     0,     0,     0,    54,
      55,    56,     0,    58,     0,     0,     0,     0,    61,    62,
     197,     0,    64,     0,     0,     0,     0,     0,    69,    70,
      71,    72,   198,     0,    74,     0,     0,    76,     0,     0,
       0,    80,   199,    82,     0,    83,    84,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     200,     0,   100,     0,     0,     5,     6,   101,     8,     9,
      10,   378,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,    19,     0,  -208,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   193,     0,     0,
       0,    31,  -208,  -208,  -208,     0,   194,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   195,     0,   196,     0,
       0,    43,     0,     0,    45,     0,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,    62,
      63,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,   198,     0,    74,     0,     0,    76,     0,     0,
       0,    80,    81,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     200,     0,   100,     0,  -208,     5,     6,   101,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   192,     0,  -208,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   193,     0,     0,
       0,    31,  -208,  -208,  -208,     0,   194,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   195,     0,   196,     0,
       0,    43,     0,     0,    45,     0,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,    62,
     197,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,   198,     0,    74,     0,     0,    76,     0,     0,
       0,    80,   199,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     200,     0,   100,     0,  -208,     5,     6,   101,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   192,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   193,     0,     0,
       0,    31,     0,     0,     0,     0,   194,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   195,     0,   196,     0,
       0,    43,     0,     0,    45,     0,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,    62,
     197,  -739,     0,     0,     0,  -739,     0,     0,    69,    70,
      71,    72,   198,     0,    74,     0,     0,    76,     0,     0,
       0,    80,   199,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,  -374,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,  -374,     0,     0,    98,     0,
     200,  -374,   100,     0,     0,     5,     6,   101,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   192,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   193,     0,     0,
       0,    31,     0,     0,     0,     0,   194,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   195,     0,   196,     0,
       0,    43,     0,     0,    45,     0,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,    62,
     197,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,   198,     0,    74,     0,     0,    76,   313,   314,
       0,    80,   199,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,   315,     0,     0,    98,     0,
     200,     0,   100,     0,     0,     5,     6,   101,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   192,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   193,     0,     0,
       0,    31,     0,     0,     0,     0,   194,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   195,     0,   196,     0,
       0,    43,     0,     0,    45,     0,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,    62,
     197,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,   198,     0,    74,     0,     0,    76,   313,   314,
       0,    80,   199,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,   348,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     200,     0,   100,     0,     0,     5,     6,   101,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   192,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   193,     0,     0,
       0,    31,     0,     0,     0,     0,   194,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   195,     0,   196,     0,
       0,    43,     0,     0,    45,     0,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,    62,
     197,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,   198,     0,    74,     0,     0,    76,   313,   314,
       0,    80,   199,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     200,     0,   100,  -542,     0,     5,     6,   101,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   192,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    27,     0,   193,     0,     0,
       0,    31,     0,     0,     0,     0,   194,    34,     0,     0,
       0,    36,     0,     0,    38,     0,   195,     0,   196,     0,
       0,    43,     0,     0,    45,     0,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,    62,
     197,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,   198,     0,    74,     0,     0,    76,   313,   314,
       0,    80,   199,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,    88,    89,    90,    91,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,    94,    95,    96,     0,     0,     0,    98,     0,
     200,     0,   100,     0,     0,  -542,     0,   101,     5,     6,
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
      76,   313,   314,     0,    80,   199,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   200,     0,   100,   802,     0,     5,     6,
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
      76,   313,   314,     0,    80,   199,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   200,     0,   100,   805,     0,     5,     6,
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
      76,   313,   314,     0,    80,   199,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   200,     0,   100,   811,     0,     5,     6,
     101,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   192,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     193,     0,     0,     0,    31,     0,     0,     0,     0,   194,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   195,
       0,   196,     0,     0,    43,     0,     0,    45,     0,    47,
       0,    48,    49,   950,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   197,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,   198,     0,    74,     0,     0,
      76,   313,   314,     0,    80,   199,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   200,     0,   100,     0,     0,     5,     6,
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
      76,     0,     0,     0,    80,   199,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
    -382,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,  -382,     0,
       0,    98,     0,   200,  -382,   100,     0,     0,     5,     6,
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
      76,     0,     0,     0,    80,   199,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
    -367,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,  -367,     0,
       0,    98,     0,   200,  -367,   100,     0,     0,     5,     6,
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
      76,     0,     0,     0,    80,   199,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
    -375,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,  -375,     0,
       0,    98,     0,   200,  -375,   100,     0,     0,     5,     6,
     101,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   192,     0,     0,
      21,     0,   255,     0,     0,     0,     0,     0,    27,     0,
     193,     0,     0,     0,    31,     0,     0,     0,     0,   194,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   195,
       0,   196,     0,     0,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   197,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,   198,     0,    74,     0,     0,
      76,     0,     0,     0,    80,   199,    82,     0,   256,    84,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   200,     0,   100,     0,     0,     5,     6,
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
      76,   313,   314,     0,    80,   199,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   200,     0,   100,     0,     0,     5,     6,
     101,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   192,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    27,     0,
     193,     0,     0,     0,    31,     0,     0,     0,     0,   194,
      34,     0,     0,     0,    36,     0,     0,    38,     0,   195,
       0,   196,     0,     0,    43,     0,     0,    45,     0,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,    62,   197,  -739,     0,     0,     0,  -739,     0,
       0,    69,    70,    71,    72,   198,     0,    74,     0,     0,
      76,     0,     0,     0,    80,   199,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   200,     0,   100,     0,     0,     5,     6,
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
      76,     0,     0,     0,    80,   199,    82,     0,     0,    84,
       0,     0,     0,     0,     0,   599,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   200,     0,   100,     0,     0,   600,     0,
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
      74,     0,     0,    76,     0,     0,     0,    80,   199,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    88,    89,
      90,    91,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,    94,    95,
      96,   856,     0,     0,    98,     0,   200,     0,   100,     0,
       0,  1021,     0,   101,     5,     6,     0,     8,     9,    10,
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
      80,   199,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,  -405,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,     0,     0,     0,    98,     0,   200,
       0,   100,  -405,     0,     5,     6,   101,     8,     9,    10,
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
      80,   199,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,    88,    89,    90,    91,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,    94,    95,    96,  -536,     0,     0,    98,     0,   200,
       0,   100,     0,     0,  -536,     0,   101,     5,     6,     0,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   192,     0,     0,    21,
       0,     0,     0,     0,     0,     0,   271,    27,     0,   193,
       0,     0,     0,    31,     0,     0,     0,     0,   194,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   195,     0,
     196,     0,     0,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   197,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,   198,     0,    74,     0,     0,    76,
       0,     0,     0,    80,   199,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,    99,     0,   100,     0,     0,     5,     6,   101,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   192,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   193,
       0,     0,     0,    31,     0,     0,     0,     0,   194,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   195,     0,
     196,     0,     0,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   197,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,   198,     0,    74,     0,     0,    76,
       0,     0,     0,    80,   199,    82,     0,     0,    84,     0,
       0,     0,     0,     0,   599,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,   200,     0,   100,     0,     0,     5,     6,   101,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   192,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   193,
       0,     0,     0,    31,     0,     0,     0,     0,   194,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   195,     0,
     196,     0,     0,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   197,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,   198,     0,    74,     0,     0,    76,
       0,     0,     0,    80,   199,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,    94,    95,    96,     0,     0,     0,
      98,     0,   200,     0,   100,     0,     0,   600,     0,   101,
       5,     6,     0,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   192,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   193,     0,     0,     0,    31,     0,     0,     0,
       0,   194,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   195,     0,   196,     0,     0,    43,     0,     0,    45,
       0,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,    62,   197,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,   198,     0,    74,
       0,     0,    76,     0,     0,     0,    80,   199,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,   200,   800,   100,     0,     0,
       5,     6,   101,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   192,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      27,     0,   193,     0,     0,     0,    31,     0,     0,     0,
       0,   194,    34,     0,     0,     0,    36,     0,     0,    38,
       0,   195,     0,   196,     0,     0,    43,     0,     0,    45,
       0,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,    62,   197,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,   198,     0,    74,
       0,     0,    76,     0,     0,     0,    80,   199,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,    88,    89,    90,
      91,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,    95,    96,
       0,     0,     0,    98,     0,   200,     0,   100,     0,     0,
    1031,     0,   101,     5,     6,     0,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,   194,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,     0,     0,     0,    80,
     199,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   200,     0,
     100,     0,     0,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   291,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,   194,    34,     0,     0,     0,   293,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     298,     0,    74,     0,     0,    76,     0,     0,     0,    80,
     199,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   200,     0,
     100,     0,     0,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,  1068,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,     0,     0,     0,    80,
     199,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   200,     0,
     100,     0,     0,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,  1213,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,     0,     0,     0,    80,
     199,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   200,     0,
     100,     0,     0,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,  1214,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,     0,     0,     0,    80,
     199,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   200,     0,
     100,     0,     0,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,  1216,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,     0,     0,     0,    80,
     199,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
      94,    95,    96,     0,     0,     0,    98,     0,   200,     0,
     100,     0,     0,     5,     6,   101,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   192,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    27,     0,   193,     0,     0,     0,    31,
       0,     0,     0,     0,  1217,    34,     0,     0,     0,    36,
       0,     0,    38,     0,   195,     0,   196,     0,     0,    43,
       0,     0,    45,     0,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,    62,   197,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
     198,     0,    74,     0,     0,    76,     0,     0,     0,    80,
     199,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,    89,    90,    91,     0,     0,     0,     0,    92,     6,
       0,     8,     9,    10,    11,    12,     0,    93,     0,    13,
      94,    95,    96,    17,     0,     0,    98,   192,   200,  -208,
     100,     0,   290,     0,     0,   101,     0,     0,     0,     0,
     193,     0,     0,     0,     0,  -208,  -208,  -208,     0,   194,
       0,     0,   292,     0,     0,     0,   294,     0,     0,     0,
       0,   196,     0,     0,    43,     0,     0,     0,     0,    47,
       0,    48,    49,     0,     0,     0,     0,     0,     0,     0,
     295,     0,    54,   296,    56,    57,    58,     0,     0,     0,
       0,    61,     0,   197,     0,   297,     0,     0,     0,     0,
       0,    69,    70,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,   299,     0,   199,    82,     0,   300,     0,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    88,    89,    90,    91,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,    94,    95,    96,     0,     0,
       0,    98,     0,   200,     0,   100,  -336,  -208,     0,     6,
     101,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,     0,    17,     0,     0,     0,   192,     0,  -208,
       0,     0,   290,     0,     0,     0,     0,     0,     0,     0,
     193,     0,     0,     0,     0,  -208,  -208,  -208,     0,   194,
       0,     0,   292,     0,     0,     0,   294,     0,     0,     0,
       0,   196,     0,     0,    43,     0,     0,     0,     0,    47,
       0,    48,    49,     0,     0,     0,     0,     0,     0,     0,
     295,     0,    54,   296,    56,    57,    58,     0,     0,     0,
       0,    61,     0,   197,     0,   297,     0,     0,     0,     0,
       0,    69,    70,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,   299,     0,   199,    82,     0,   300,     0,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     629,     0,   630,     0,    87,    88,    89,    90,    91,     0,
       0,   631,     0,    92,     0,     0,   632,   229,   633,   634,
       0,     0,    93,     0,   635,    94,    95,    96,     0,     0,
       0,    98,   232,   200,   193,   100,     0,  -208,   233,     0,
     101,     0,     0,     0,   636,     0,     0,     0,   235,     0,
       0,   637,     0,   638,   639,     0,     0,     0,     0,     0,
       0,   640,     0,     0,     0,     0,     0,     0,   239,   641,
       0,     0,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   642,     0,     0,     0,
       0,     0,     0,     0,     0,   243,     0,   643,     0,   245,
       0,   644,     0,     0,   247,     0,     0,     0,   645,   199,
     249,     0,     0,   646,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,     0,   473,   474,     0,   647,   648,
     649,   650,   651,   652,     0,   653,     0,     0,     0,   654,
     655,   656,   657,   658,   659,   660,   661,   662,   663,     0,
     664,     0,     0,   665,   666,   667,   668,     5,     6,   669,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   192,     0,  -208,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   193,
       0,     0,     0,    31,  -208,  -208,  -208,     0,   194,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   195,     0,
     196,     0,     0,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   197,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,   198,     0,    74,     0,     0,    76,
       0,     0,     0,    80,   199,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    89,    90,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,    95,    96,     0,     0,     0,
      98,     0,   200,     0,   100,     0,  -208,     5,     6,     0,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   192,     0,  -208,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   193,
       0,     0,     0,    31,  -208,  -208,  -208,     0,   194,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   195,     0,
     196,     0,     0,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   197,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,   198,   227,    74,     0,     0,    76,
       0,     0,     0,    80,   199,    82,     0,     0,    84,   228,
     229,     0,   230,     0,     0,     0,     0,   231,     0,     0,
       0,     0,     0,     0,     0,   232,     0,     0,     0,     0,
       0,   233,     0,    87,     0,    89,     0,   234,     0,     0,
       0,   235,     0,     0,   236,     0,   237,     0,     0,     0,
       0,     0,     0,   227,   238,     0,    96,     0,     0,     0,
       0,   239,   240,     0,   100,     0,  -208,   228,   229,   241,
     230,     0,     0,     0,     0,   231,     0,     0,     0,   242,
       0,     0,     0,   232,     0,     0,     0,     0,   243,   233,
     244,     0,   245,     0,   246,   234,     0,   247,     0,   235,
       0,   248,   236,   249,   237,     0,   250,     0,     0,     0,
       0,     0,   238,     0,     0,     0,     0,     0,     0,   239,
     240,     0,     0,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   242,     0,     0,
       0,     0,     0,   227,     0,     0,   243,     0,   244,     0,
     245,     0,   246,     0,     0,   247,     0,   228,   229,   248,
     230,   249,   267,     0,   250,   231,     0,     0,     0,     0,
       0,     0,     0,   232,     0,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,   234,     0,     0,     0,   235,
       0,     0,   236,     0,   237,     0,     0,     0,     0,     0,
       0,     0,   238,     0,     0,     0,     0,     0,     0,   239,
     240,     0,     0,     0,     0,     0,     0,   241,     0,     0,
     514,     0,     0,     0,     0,     0,     0,   242,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,   244,     0,
     245,   695,   246,     0,     0,   247,     0,     0,     0,   248,
     631,   249,     0,     0,   250,   983,   984,   633,   985,     0,
       0,     0,     0,   986,     0,     0,     0,     0,     0,     0,
       0,   697,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   987,     0,     0,     0,   988,     0,     0,
     989,     0,   195,   639,     0,     0,     0,     0,     0,     0,
     698,     0,     0,     0,     0,     0,     0,     0,   990,     0,
     541,     0,     0,     0,     0,   991,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   992,     0,     0,     0,     0,
       0,     0,     0,     0,   993,     0,   994,     0,   995,     0,
     699,     0,     0,     0,     0,     0,   700,   996,     0,   997,
       0,     0,   998,     0,     0,     0,     0,     0,     0,     0,
       0,   506,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   470,   471,     0,   473,   474,     0,   647,   648,   649,
     650,   651,   652,     0,   653,     0,     0,     0,   654,   655,
     656,   657,   658,   659,   660,   661,   662,   663,   227,   664,
       0,     0,   665,   666,   667,   668,   999,   631,     0,     0,
       0,     0,   228,   229,   633,   230,     0,     0,     0,     0,
     231,     0,     0,     0,     0,     0,     0,     0,   232,     0,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
     234,     0,     0,     0,   235,     0,     0,   236,     0,   237,
     639,     0,     0,     0,     0,     0,     0,   238,     0,     0,
       0,     0,     0,     0,   239,   240,     0,     0,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   242,     0,     0,     0,     0,     0,     0,     0,
       0,   243,     0,   244,     0,   245,     0,   246,     0,     0,
     247,     0,     0,     0,   248,     0,   249,     0,     0,   250,
       0,     0,     0,     0,     0,     0,     0,     0,   506,   461,
     462,   463,   464,   465,   466,   467,   468,   469,   470,   471,
       0,   473,   474,     0,   647,   648,   649,   650,   651,   652,
       0,   653,     0,     0,     0,   654,   655,   656,   657,   658,
     659,   660,   661,   662,   663,     7,   664,     0,     0,   665,
     666,   667,   668,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   355,     0,     0,     0,    22,    23,     0,     0,
       0,     0,     7,     0,     0,   356,     0,    29,   357,     0,
       0,     0,     0,    32,     0,     0,     0,     0,     0,   355,
      37,     0,     0,    22,    23,     0,     0,     0,     0,     0,
       0,     0,   356,     0,    29,   357,     0,     0,     0,     0,
      32,   358,     0,     0,     0,     0,    53,    37,    55,     0,
      57,     0,  1044,     0,     0,  1045,     0,     0,   359,     0,
      64,     0,     0,     0,     0,     0,     0,     0,   358,     0,
       0,     7,     0,    53,     0,    55,     0,    57,    79,  1044,
     360,     0,  1045,    83,     0,   359,     0,    64,   355,     0,
       0,     0,    22,    23,     0,     0,     0,     0,     0,     0,
       0,   356,     0,    29,   357,    79,   430,   360,     0,    32,
      83,     0,     0,   431,     0,     0,    37,     0,     0,     0,
       0,     0,     0,     0,     0,   432,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,     0,   358,     0,  1046,
       0,     0,    53,     0,    55,     0,    57,     0,  1044,     0,
       0,  1045,     0,     0,   359,     0,    64,     0,     0,     0,
      97,     0,     0,     0,     0,     0,  1114,     0,     0,     0,
       0,     0,     0,     0,    79,     0,   360,     0,     0,    83,
       0,     0,     0,   433,   430,     0,     0,   434,     0,     0,
       0,   431,     0,     0,     0,   505,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   435,   506,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,    97,
     473,   474,     0,   436,   437,  1178,   438,   439,   440,     0,
     441,   442,   443,     0,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,     0,     0,   456,   457,
     458,   433,   361,     0,   430,   434,     0,     0,     0,   459,
       0,   431,     0,  1189,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,   506,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,     0,   473,   474,
       0,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,     0,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,     0,     0,   456,   457,   458,     0,
     361,   433,   430,     0,     0,   434,     0,   459,     0,   431,
       0,     0,     0,  1248,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   435,   506,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,     0,   473,   474,
       0,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,     0,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,     0,     0,   456,   457,   458,   433,
     361,     0,   430,   434,     0,     0,     0,   459,     0,   431,
       0,  1249,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   435,   506,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,     0,   473,   474,     0,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,     0,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,     0,     0,   456,   457,   458,     0,   361,   433,
     430,     0,     0,   434,     0,   459,     0,   431,     0,     0,
       0,  1250,     0,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   435,   506,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,     0,   473,   474,     0,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,     0,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,     0,     0,   456,   457,   458,   433,   361,     0,
       0,   434,     0,     0,     0,   459,     0,     0,   430,  1251,
       0,     0,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   432,   869,     0,
     435,   506,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   470,   471,   870,   473,   474,     0,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,     0,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
       0,     0,   456,   457,   458,     0,   361,     0,     0,     0,
       0,     0,     0,   459,     0,   433,   430,     0,     0,   434,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   432,   271,     0,     0,     0,
       0,     0,     0,     0,   211,     0,     0,     0,   435,     0,
       0,   978,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,   437,     0,   438,   439,
     440,     0,   441,   442,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,     0,     0,
     456,   457,   458,   433,   361,     0,     0,   434,     0,     0,
       0,   459,     0,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,     0,   211,     0,     0,   432,   435,     0,     0,     0,
     586,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,     0,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   587,     0,   456,   457,
     458,     0,   361,     0,     0,     0,     0,     0,     0,   459,
       0,     0,     0,   433,     0,     0,     0,   434,     0,     0,
       0,     0,     0,     0,     0,     0,   430,   823,     0,     0,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   432,   435,     0,     0,     0,
     824,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,     0,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   825,   588,   456,   457,
     458,     0,     0,     0,     0,     0,     0,     0,     0,   459,
       0,     0,     0,   433,   430,     0,     0,   434,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,  1132,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,     0,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,     0,     0,   456,   457,
     458,   433,     0,     0,   430,   434,     0,     0,     0,   459,
       0,   431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,   271,     0,     0,     0,     0,     0,
     211,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,     0,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,     0,     0,   456,   457,   458,     0,
     361,   433,   430,     0,     0,   434,     0,   459,     0,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,   271,     0,     0,     0,     0,     0,     0,     0,
     211,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,   431,
       0,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,   432,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,     0,     0,   456,   457,   458,   433,
     361,     0,   430,   434,     0,     0,     0,   459,     0,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,   911,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   435,     0,     0,     0,     0,   912,     0,   433,
       0,     0,     0,   434,     0,     0,     0,     0,     0,   436,
     437,   943,   438,   439,   440,     0,   441,   442,   443,     0,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   435,     0,   456,   457,   458,     0,   361,   433,
       0,     0,     0,   434,     0,   459,     0,     0,     0,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,     0,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   435,     0,   456,   457,   458,     0,   361,     0,
       0,     0,     0,     0,     0,   459,     0,     0,     0,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,     0,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   430,     0,   456,   457,   458,     0,     0,   431,
       0,     0,     0,     0,     0,   459,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,  1106,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,     0,     0,     0,   431,     0,     0,     0,     0,     0,
       0,     0,  1107,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,   434,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,   434,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,     0,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,     0,   211,   456,   457,   458,   435,     0,     0,
       0,     0,     0,     0,     0,   459,     0,     0,     0,   433,
       0,     0,     0,   434,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,     0,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,     0,   211,   456,
     457,   458,   435,     0,     0,     0,   430,   527,  1123,     0,
     459,     0,     0,   431,     0,     0,     0,     0,     0,   436,
     437,     0,   438,   439,   440,   432,   441,   442,   443,     0,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,     0,     0,   456,   457,   458,   430,     0,     0,
       0,     0,     0,  1172,   431,   459,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,   434,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,     0,   435,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,   434,     0,
       0,     0,     0,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,     0,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,     0,   435,   456,   457,
     458,     0,     0,     0,     0,     0,     0,     0,     0,   459,
       0,     0,   433,     0,   436,   437,   434,   438,   439,   440,
       0,   441,   442,   443,     0,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   430,     0,   456,
     457,   458,     0,   550,   431,   435,     0,     0,     0,     0,
     459,     0,     0,     0,     0,     0,   432,     0,     0,     0,
       0,     0,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,     0,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   430,   554,   456,   457,   458,
       0,     0,   431,     0,     0,     0,     0,     0,   459,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,   434,     0,
       0,     0,     0,     0,     0,     0,   729,     0,     0,     0,
     430,   819,     0,     0,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   435,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,     0,   436,   437,   434,   438,   439,   440,
       0,   441,   442,   443,     0,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,     0,     0,   456,
     457,   458,     0,     0,     0,   435,     0,     0,     0,     0,
     459,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,   434,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,     0,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   430,     0,   456,   457,   458,
     435,     0,   431,     0,   801,     0,     0,     0,   459,     0,
       0,     0,     0,     0,   432,     0,     0,   436,   437,     0,
     438,   439,   440,     0,   441,   820,   443,     0,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     430,     0,   456,   457,   458,     0,     0,   431,     0,     0,
       0,     0,     0,   459,     0,     0,     0,     0,     0,   432,
       0,     0,   941,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,   434,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,   863,   435,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,   434,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,     0,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,     0,     0,   456,   457,   458,
     435,     0,     0,     0,     0,     0,     0,   433,   459,     0,
       0,   434,     0,     0,     0,     0,     0,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,     0,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     435,     0,   456,   457,   458,     0,     0,     0,   430,     0,
       0,     0,     0,   459,     0,   431,     0,   436,   437,     0,
     438,   439,   440,     0,   441,   442,   443,   432,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
       0,     0,   456,   457,   458,     0,     0,     0,     0,   953,
       0,     0,     0,   459,     0,     0,   430,  1008,     0,     0,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,   434,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   432,     0,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,   436,   437,   434,   438,   439,
     440,     0,   441,   442,   443,     0,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,     0,     0,
     456,   457,   458,     0,     0,     0,   435,   954,     0,     0,
       0,   459,     0,   433,     0,     0,     0,   434,     0,     0,
       0,     0,     0,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,     0,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   435,     0,   456,   457,
     458,     0,     0,     0,   430,  1105,     0,     0,     0,   459,
       0,   431,     0,   436,   437,     0,   438,   439,   440,     0,
     441,   442,   443,   432,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,     0,     0,   456,   457,
     458,     0,     0,     0,   430,  1060,     0,     0,     0,   459,
       0,   431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,   434,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   432,     0,     0,   435,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,   434,     0,     0,     0,     0,
       0,   436,   437,     0,   438,   439,   440,     0,   441,   442,
     443,     0,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   435,     0,   456,   457,   458,     0,
       0,     0,     0,     0,     0,     0,     0,   459,     0,   433,
       0,   436,   437,   434,   438,   439,   440,     0,   441,   442,
     443,     0,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,     0,  1117,   456,   457,   458,   430,
       0,  1127,   435,     0,     0,     0,   431,   459,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   432,   436,
     437,     0,   438,   439,   440,     0,   441,   442,   443,     0,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,     0,     0,   456,   457,   458,   430,     0,     0,
       0,     0,     0,     0,   431,   459,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   432,  1158,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
     434,     0,     0,     0,     0,     0,     0,   430,  1177,     0,
       0,     0,     0,     0,   431,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   432,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,     0,   436,   437,   434,   438,
     439,   440,     0,   441,   442,   443,     0,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,     0,
       0,   456,   457,   458,     0,     0,     0,   435,     0,     0,
    1155,     0,   459,     0,   433,   430,     0,     0,   434,     0,
       0,     0,   431,     0,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   432,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   435,     0,   456,
     457,   458,     0,     0,     0,   430,     0,     0,     0,     0,
     459,     0,   431,  1208,   436,   437,     0,   438,   439,   440,
       0,   441,   442,   443,   432,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,     0,     0,   456,
     457,   458,   433,     0,     0,   430,   434,     0,     0,     0,
     459,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   435,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,   434,     0,     0,     0,
       0,     0,   436,   437,  1215,   438,   439,   440,     0,   441,
     442,   443,     0,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   435,     0,   456,   457,   458,
       0,     0,   433,   430,     0,     0,   434,     0,   459,     0,
     431,     0,   436,   437,  1252,   438,   439,   440,     0,   441,
     442,   443,   432,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   435,     0,   456,   457,   458,
       0,     0,     0,   430,     0,     0,     0,     0,   459,     0,
     431,     0,   436,   437,     0,   438,   439,   440,     0,   441,
     442,   443,   432,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,     0,     0,   456,   457,   458,
     433,     0,     0,   430,   434,     0,     0,     0,   459,     0,
     431,     0,  1253,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   435,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,   434,     0,     0,     0,     0,     0,
     436,   437,  1254,   438,   439,   440,     0,   441,   442,   443,
     432,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   435,     0,   456,   457,   458,     0,     0,
     433,   430,     0,     0,   434,     0,   459,     0,   431,     0,
     436,   437,  1255,   438,   439,   440,     0,   441,   442,   443,
     432,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   435,     0,   456,   457,   458,   433,     0,
       0,   430,   434,     0,     0,     0,   459,     0,   431,     0,
     436,   437,     0,   438,   439,   440,     0,   441,   442,   443,
     432,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,     0,     0,   456,   457,   458,   433,     0,
       0,     0,   434,     0,     0,     0,   459,     0,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,     0,   444,
     445,   446,   447,     0,   449,   450,   451,   452,   453,     0,
     455,   435,     0,   456,   457,   458,     0,     0,   931,     0,
       0,     0,   434,     0,   459,     0,     0,     0,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,     0,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   435,     0,   456,   457,   458,     0,     0,     0,     0,
       0,     0,     0,     0,   459,     0,     0,     0,   436,   437,
       0,   438,   439,   440,     0,   441,   442,   443,   432,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,     0,   355,   456,   457,   458,    22,    23,     0,     0,
       0,     0,     0,     0,   459,   356,     0,    29,   357,     0,
       0,     0,     0,    32,     0,     0,     0,     0,     0,     0,
      37,     0,     0,  -332,     0,     0,     0,    41,     0,     0,
       0,     0,     0,     0,     0,   -68,   433,     0,     0,     0,
     434,   358,     0,  -332,     0,     0,    53,     0,    55,     0,
       0,     0,    59,  -332,   -68,    60,     0,     0,   359,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,    79,     0,
     360,     0,     0,    83,     0,     0,   436,   437,     0,   438,
     439,   440,     0,   441,   442,   443,     0,   444,   445,   446,
     447,   227,   449,   450,   451,   452,   453,     0,   455,     0,
       0,   456,   457,   458,     0,   228,   229,     0,   230,     0,
       0,     0,   459,   231,     0,    23,     0,     0,     0,     0,
       0,   232,     0,    97,     0,     0,     0,   233,   361,     0,
       0,     0,     0,   234,     0,     0,     0,   235,     0,     0,
     236,     0,   237,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,     0,     0,     0,     0,   239,   240,     0,
       0,     0,     0,     0,     0,   241,    55,     0,     0,     0,
       0,     0,     0,     0,     0,   242,     0,     0,    64,     0,
       0,   227,     0,     7,   243,     0,   244,     0,   245,     0,
     246,     0,     0,   247,     0,   228,   229,   248,   230,   249,
       0,    83,   250,   231,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   234,     0,     0,     0,   235,     0,     0,
     236,     0,   237,     0,     0,     0,     0,     0,     0,  -309,
     238,     0,     0,     0,     0,     0,     0,   239,   240,     0,
       0,     0,     0,  -309,  -309,   241,  -309,     0,     0,     0,
       0,  -309,     0,     0,     0,   242,     0,     0,     0,  -309,
       0,     0,     0,     0,   243,  -309,   244,     0,   245,     0,
     246,  -309,     0,   247,     0,  -309,     0,   248,  -309,   249,
    -309,     0,   250,     0,     0,     0,     0,   227,  -309,     0,
       0,     0,     0,     0,     0,  -309,  -309,     0,     0,     0,
       0,   228,   229,  -309,   230,     0,     0,     0,     0,   231,
       0,     0,     0,  -309,     0,     0,     0,   232,     0,     0,
       0,     0,  -309,   233,  -309,     0,  -309,     0,  -309,   234,
       0,  -309,     0,   235,     0,  -309,   236,  -309,   237,     0,
    -309,     0,     0,     0,     0,  -310,   238,     0,     0,     0,
       0,     0,     0,   239,   240,     0,     0,     0,     0,  -310,
    -310,   241,  -310,     0,     0,     0,     0,  -310,     0,     0,
       0,   242,     0,     0,     0,  -310,     0,     0,     0,     0,
     243,  -310,   244,     0,   245,     0,   246,  -310,     0,   247,
       0,  -310,     0,   248,  -310,   249,  -310,     0,   250,     0,
       0,     0,     0,  -311,  -310,     0,     0,     0,     0,     0,
       0,  -310,  -310,     0,     0,     0,     0,  -311,  -311,  -310,
    -311,     0,     0,     0,     0,  -311,     0,     0,     0,  -310,
       0,     0,     0,  -311,     0,     0,     0,     0,  -310,  -311,
    -310,     0,  -310,     0,  -310,  -311,     0,  -310,     0,  -311,
       0,  -310,  -311,  -310,  -311,     0,  -310,     0,     0,     0,
       0,     0,  -311,     0,     0,     0,     0,     0,     0,  -311,
    -311,     0,     0,     0,     0,     0,     0,  -311,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -311,     0,     0,
       0,     0,     0,     0,     0,     0,  -311,     0,  -311,     0,
    -311,     0,  -311,     0,     0,  -311,     0,     0,     0,  -311,
       0,  -311,     0,     0,  -311
};

static const yytype_int16 yycheck[] =
{
       2,     2,   162,    61,   262,    66,   548,     2,   109,   399,
     178,   226,   617,   586,    61,   512,    26,   575,   421,   488,
     672,   624,   674,    25,    85,   220,   788,   825,   890,   287,
      25,   130,   604,   379,   543,   878,   820,   782,     1,   100,
       1,     1,    32,     1,    24,    56,   843,   262,   389,    47,
     201,     5,    32,    32,   768,    65,    56,    32,    22,   519,
      40,    41,    42,    32,    47,   163,     3,   118,   326,   101,
      47,    73,   287,    60,     3,    77,    78,   163,    73,    35,
     116,     0,    77,    78,   170,    32,   145,    47,    32,    24,
     258,   956,   433,   434,    32,   145,   118,    99,    32,    32,
       2,   103,    24,   118,    99,    40,    41,    42,   103,   145,
      32,   326,   145,    90,   173,     3,    74,    54,    40,    41,
      42,   171,   173,    25,    88,    54,   514,   601,   602,   116,
     163,     2,    24,   607,   608,   118,   168,   117,   479,   480,
      32,     1,   121,   118,   485,    56,   110,   145,    40,    41,
      42,   173,   108,   541,    25,   145,    47,   115,   173,   170,
     123,   145,   145,   123,   624,   102,    54,   163,  1011,    47,
     170,    73,    32,   102,   118,    77,    78,   173,   168,   163,
     118,   144,   145,    82,   144,   145,    85,   949,   118,   168,
     173,   145,   172,   168,   956,   117,   754,    99,   145,   168,
      60,   103,    73,   163,  1047,  1048,    77,    78,   171,   170,
     212,   169,   363,  1010,   959,   169,   930,   212,    47,   477,
     478,   168,  1006,     3,   168,   117,  1091,   442,    99,   163,
     168,   572,   103,   730,   168,   168,   163,   172,    56,   145,
     118,    27,   169,   589,   104,    28,    47,    18,    47,    46,
     172,   824,    46,   421,   145,   145,   116,    40,    41,    42,
      74,    90,   477,   478,   266,   171,    53,   145,    65,   271,
    1113,   266,   163,   163,    54,    56,   271,   786,   108,   378,
     172,    31,   173,   855,   144,   163,    73,    84,  1150,    90,
      84,    90,    78,  1136,   552,   173,    83,   512,   145,    27,
     145,   115,   145,   163,    90,   145,    77,   163,   168,  1107,
     212,  1073,    92,   139,    31,   112,   123,   173,   112,    47,
     145,   147,   102,    51,   171,    96,   171,   113,   171,  1091,
     145,   171,   492,   807,   808,   809,   531,   552,    90,    89,
     111,   212,   149,    93,   346,   346,   171,    75,    77,    78,
      78,   346,    80,     3,   957,   169,   171,   163,    56,   361,
     575,   163,    90,   169,   266,   148,   361,   145,   370,   271,
      20,   173,    89,   145,   170,   370,    93,   160,   388,   876,
     108,   383,   145,    33,   145,   113,   396,    46,   383,   139,
      11,   145,   145,   171,   144,   266,   145,   147,   593,   171,
     271,   151,   163,   168,    54,   170,    65,   145,   171,  1061,
    1062,   472,    62,   196,   475,   476,   913,   171,   171,   421,
     169,   979,   139,   145,   145,    84,   999,   144,    50,   146,
     147,    53,   145,   171,   151,   170,   145,   123,   170,    44,
     172,   158,   163,   171,   346,   163,  1051,   169,   917,   166,
     163,    73,   102,   112,    76,   173,     2,    56,   108,   361,
     169,    83,   145,    68,   150,   145,   152,   153,   370,   155,
     156,   145,   255,   256,   484,   346,   145,   145,   145,    25,
     163,   383,   540,   163,   267,   170,   108,   172,   878,   163,
     361,   894,    56,   540,   163,   163,   163,     9,    89,   370,
     145,   503,    93,   505,   565,   566,    56,    89,   503,   139,
     505,    93,   383,   854,   144,   730,   617,   147,   163,    89,
      56,   151,    89,    93,   307,   102,    93,    73,    56,   870,
      89,    77,    78,  1106,    93,  1140,   170,    89,   172,   754,
      31,    93,   170,    56,   172,  1019,  1020,   170,    56,   172,
     170,   170,   172,    99,    77,    78,    99,   103,   288,   289,
      56,   571,   170,  1037,  1038,     5,    56,   908,   565,   566,
      56,   912,    56,   356,    56,   147,   168,   579,   579,    56,
    1143,   144,   168,   163,   579,   163,   162,    56,   163,   163,
     931,   932,   163,   603,   163,   163,    84,   163,    89,     9,
     145,   503,    93,   505,   819,   820,   168,  1149,   163,   163,
     393,   171,   395,   170,    31,   144,   399,   170,   145,   170,
     780,  1011,   163,   170,   163,  1099,  1100,  1101,  1102,   171,
      27,    65,   503,     3,   505,   976,   170,   978,   421,  1202,
    1203,  1204,  1205,  1206,  1207,    78,   168,   123,   139,   140,
      47,   142,   143,   144,    51,   146,   147,  1047,   163,    31,
     151,   876,  1052,    68,   147,   123,   212,   158,   170,   274,
     171,   276,    89,   164,   165,   166,    93,   579,    75,   145,
     123,    78,   123,    80,    34,   845,    34,    65,   123,   144,
     171,   752,   171,    90,    21,   168,   171,   117,   913,   170,
     145,   169,   123,   486,   104,   170,   163,   163,   579,   147,
     771,   108,   139,   163,   163,   147,   113,    89,   323,   147,
     266,    93,   139,  1113,   147,   271,   894,   144,   147,   146,
     147,   514,     2,   147,   151,   147,   147,   173,   147,   147,
     147,   158,   171,   147,   527,   747,   147,   164,   165,   166,
      32,   147,   147,   171,   537,    25,   758,   170,   541,   761,
      34,    34,   144,   758,   979,   171,   761,   139,   140,   145,
     169,   143,   144,   163,   146,   147,   168,   145,    47,   151,
     173,   169,   173,    34,   171,   170,   158,   173,   163,   791,
     791,   170,   164,   165,   166,   144,   791,    27,    27,   163,
     346,   163,   585,    73,   145,   171,   173,    77,    78,    32,
      34,   171,   173,   596,   173,   361,   163,    47,    47,   171,
     878,    51,    51,   163,   370,   171,  1084,  1085,   162,    99,
     162,   145,   890,   103,   617,   574,   108,   383,   979,   261,
     850,   817,    24,   363,   370,    75,    75,   387,    78,    78,
     852,    80,   873,    27,   972,   857,   758,   852,   540,   761,
      90,    90,   857,   335,   866,   525,   381,   869,   894,  1084,
    1085,   866,  1150,    47,   869,   301,   539,    51,   108,   108,
     958,  1149,  1208,   113,   113,    44,  1201,   758,   678,   791,
     761,   844,   527,   128,   767,   678,   522,    20,   917,    -1,
      -1,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    77,    78,
     791,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   931,
     932,    -1,    -1,    -1,   108,    -1,   938,    -1,   940,   113,
      -1,   943,   212,   938,    -1,   940,    -1,  1048,   943,   108,
     852,    -1,    -1,  1011,    -1,   857,    -1,   503,   968,   505,
    1128,    -1,    -1,    -1,   866,   967,    -1,   869,    -1,    -1,
      -1,    -1,   967,    -1,    -1,   977,    -1,   136,    -1,    -1,
     763,   852,   977,    -1,   767,    -1,   857,    -1,    -1,    -1,
      -1,    -1,   597,   598,   777,   866,   266,    -1,   869,    -1,
      -1,   271,   607,   608,    -1,    -1,    -1,    -1,     2,    -1,
      -1,    -1,    -1,    -1,    -1,   798,  1018,    -1,    -1,    -1,
      -1,    -1,    -1,  1018,    -1,  1027,    -1,    -1,    -1,    -1,
      -1,    25,  1027,   579,    -1,  1136,   938,    -1,   940,  1140,
      -1,   943,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1113,    -1,    -1,  1226,   218,
    1228,   844,    -1,  1231,  1232,   967,    -1,   938,    -1,   940,
      -1,    -1,   943,    -1,    -1,   977,   346,    -1,  1080,    73,
      -1,    -1,    -1,    77,    78,  1080,    -1,    -1,    -1,    -1,
     873,   361,  1150,    -1,    -1,    -1,   967,    -1,   257,   258,
     370,    -1,    -1,    -1,    -1,    99,   977,    -1,    -1,   103,
      -1,   894,    -1,   383,    -1,   274,  1018,   276,    -1,  1121,
      -1,  1123,    -1,    -1,   283,  1027,  1121,  1129,  1123,    -1,
    1132,    -1,    -1,    -1,  1129,    -1,    -1,  1132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1018,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1027,    -1,    -1,  1161,
      -1,    -1,    -1,    -1,   323,  1167,  1161,    -1,    -1,  1171,
    1172,    -1,  1167,    -1,    -1,    -1,  1171,  1172,  1080,    -1,
      -1,  1183,    -1,    -1,    -1,  1187,    -1,  1189,  1183,   972,
      -1,    -1,  1187,    -1,  1189,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   808,   809,    -1,    -1,    -1,    -1,  1080,
      -1,    -1,   758,    -1,    -1,   761,    -1,    -1,   212,  1121,
      -1,  1123,    -1,    -1,    -1,    -1,    -1,  1129,    -1,    -1,
    1132,    -1,    -1,   503,    -1,   505,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   791,  1248,  1249,  1250,  1251,
    1121,    -1,  1123,  1248,  1249,  1250,  1251,    -1,  1129,  1161,
      -1,  1132,    -1,   868,    -1,  1167,    -1,   872,  1051,  1171,
    1172,    -1,   266,    -1,    -1,     3,    -1,   271,    -1,    -1,
      -1,  1183,    -1,    -1,    -1,  1187,    -1,  1189,    -1,    17,
    1161,    -1,    20,    -1,    -1,    -1,  1167,    25,    -1,    -1,
    1171,  1172,    -1,    -1,    -1,    -1,   852,    35,    -1,   579,
      -1,   857,  1183,    -1,    -1,    -1,  1187,    45,  1189,    -1,
     866,    -1,    -1,   869,    52,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,     2,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,  1248,  1249,  1250,  1251,
      -1,    -1,   346,    -1,    -1,    -1,    25,  1140,    -1,    87,
      -1,    -1,    -1,    -1,     2,    -1,    -1,   361,    -1,    -1,
      98,    -1,    -1,    -1,   102,    -1,   370,  1248,  1249,  1250,
    1251,   109,   110,    -1,    -1,    -1,   114,    25,    -1,   383,
      -1,    -1,   938,    -1,   940,    -1,   555,   943,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   967,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   977,  1037,  1038,   103,    73,    -1,    -1,   597,   598,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   607,   608,
      -1,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,    -1,    -1,   103,    -1,    17,    18,    -1,
      20,    -1,  1018,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,  1027,    -1,    33,    -1,    -1,    -1,    -1,   758,    39,
    1095,   761,    -1,    -1,    -1,    45,  1101,  1102,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    -1,    -1,    -1,   503,
      -1,   505,    62,  1118,    -1,    -1,    -1,    -1,    -1,    69,
      70,   791,    -1,    -1,    -1,    -1,  1131,    77,    -1,  1134,
      -1,    -1,    -1,    -1,  1080,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,   212,    -1,    -1,    96,    -1,    98,    -1,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   212,  1121,    -1,  1123,    -1,    -1,
      -1,    -1,   852,  1129,    -1,   579,  1132,   857,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   866,   266,    -1,   869,
      -1,    -1,   271,   762,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1161,    -1,    -1,    -1,    -1,
     170,  1167,    -1,    -1,    -1,  1171,  1172,    -1,   266,    -1,
      -1,    -1,    -1,   271,    -1,    -1,    -1,  1183,    -1,   798,
      -1,  1187,    -1,  1189,    -1,    -1,    -1,    -1,    -1,   808,
     809,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   938,    -1,
     940,    -1,    -1,   943,     1,    -1,     3,   346,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,   361,    20,    -1,    -1,    -1,   967,    25,    -1,
      -1,   370,  1248,  1249,  1250,  1251,    33,   977,   346,   868,
      -1,    -1,    39,   872,   383,   874,    -1,    -1,    45,    -1,
      -1,    -1,    49,   361,    -1,    52,    -1,    54,    -1,    -1,
      -1,    -1,   370,    -1,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,   383,    -1,    -1,  1018,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,  1027,    -1,    -1,
      87,    -1,    -1,    -1,   758,    -1,    -1,   761,    -1,    96,
      -1,    98,    -1,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    -1,    -1,   791,    -1,    -1,
       5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1080,    85,    -1,    -1,    -1,    -1,   975,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,   100,    -1,    -1,    -1,
      35,    -1,    37,    38,   503,    -1,   505,    -1,    43,    -1,
      -1,    -1,   169,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      25,  1121,    -1,  1123,    -1,    -1,    -1,    -1,   852,  1129,
      -1,    -1,  1132,   857,    -1,   503,    71,   505,    -1,    -1,
      -1,    76,   866,    78,    -1,   869,    -1,    82,  1037,  1038,
      85,    -1,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,
      -1,  1161,    -1,    -1,    -1,    -1,    -1,  1167,    73,    -1,
      -1,  1171,  1172,   108,    -1,   110,    -1,    -1,   113,    -1,
     579,    -1,    -1,  1183,     1,    -1,    -1,  1187,     5,  1189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    22,  1095,    -1,    -1,    26,
      27,   579,  1101,  1102,   938,    -1,   940,    -1,    35,   943,
      37,    38,   226,    -1,    -1,    -1,    43,    -1,   163,  1118,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,  1128,
      -1,    -1,  1131,   967,    -1,  1134,    -1,    -1,  1248,  1249,
    1250,  1251,    -1,   977,    71,    -1,    73,    -1,   262,    76,
      -1,    78,    -1,    80,    -1,    82,    83,    -1,    85,    -1,
      -1,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
    1169,    -1,    -1,   287,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,   110,  1018,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,  1027,    -1,    31,    -1,   212,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   326,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1226,    -1,  1228,
      -1,    -1,  1231,  1232,    -1,    -1,   163,    -1,    -1,    -1,
      -1,    -1,   169,    -1,    -1,    -1,  1080,    -1,    -1,   758,
      -1,   266,   761,    89,    -1,    -1,   271,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     758,    -1,   791,   761,    -1,    -1,   122,  1121,    -1,  1123,
      -1,    -1,    31,    -1,    -1,  1129,    -1,    -1,  1132,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,   791,   150,   151,   152,   153,    -1,   155,
     156,   157,   158,   159,   160,   161,    -1,  1161,   164,   165,
     166,    -1,    -1,  1167,    -1,    -1,    -1,  1171,  1172,   175,
      -1,    -1,    -1,   852,    -1,    -1,    -1,    -1,   857,  1183,
      89,    -1,    -1,  1187,    93,  1189,    -1,   866,   472,    -1,
     869,   475,   476,   477,   478,    -1,    -1,    -1,   383,    -1,
      -1,    -1,    -1,    -1,   852,    -1,    -1,    -1,    -1,   857,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   866,    -1,
      -1,   869,    -1,    -1,    -1,    -1,    -1,    -1,   512,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,    -1,   151,    -1,  1248,  1249,  1250,  1251,   157,   158,
      -1,    -1,   161,    -1,    -1,   164,   165,   166,    -1,   938,
      -1,   940,    -1,    -1,   943,    -1,   175,    -1,   552,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   565,   566,    -1,    -1,    -1,    -1,    -1,   967,    -1,
     938,   575,   940,    -1,    -1,   943,    -1,    -1,   977,    -1,
      -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,   503,   967,
     505,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,   977,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    46,    47,  1018,
      -1,    -1,    -1,    52,    -1,    54,    -1,    56,  1027,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    65,    66,    67,    68,
      69,    -1,    -1,    -1,    73,    -1,    75,    -1,    -1,    -1,
    1018,    -1,    -1,    82,    -1,    -1,    85,    -1,    87,  1027,
      89,    -1,    -1,    92,    93,    -1,    95,    31,    -1,    -1,
      99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1080,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1080,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1121,    -1,  1123,    89,   730,    -1,    -1,    93,
    1129,    -1,    -1,  1132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,   752,    -1,
     754,    -1,    -1,  1121,    -1,  1123,    -1,    -1,    -1,    -1,
      -1,  1129,  1161,    -1,  1132,   194,    -1,   771,  1167,   198,
      -1,   200,  1171,  1172,    -1,   139,   140,    -1,    31,    -1,
     144,    -1,   146,   147,  1183,    -1,    -1,   151,  1187,    -1,
    1189,    -1,    -1,  1161,   158,    -1,    -1,   226,    -1,  1167,
     164,   165,   166,  1171,  1172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1183,    -1,    -1,    -1,  1187,
      -1,  1189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   262,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1248,
    1249,  1250,  1251,   758,    -1,    -1,   761,    -1,   287,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,   298,
      -1,    -1,   876,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1248,  1249,  1250,  1251,   313,   314,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   326,   151,   152,
     153,   330,   155,   156,   157,   158,    -1,    -1,   161,   913,
      -1,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,   348,
      -1,    -1,   175,    89,    -1,    -1,    -1,    93,    -1,   358,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   371,    -1,   373,    -1,    -1,   852,    -1,    -1,
      -1,    -1,   857,    -1,    -1,    -1,    -1,    -1,   387,   388,
     389,   866,    -1,    -1,   869,    -1,    -1,   396,    -1,    -1,
      -1,    -1,    -1,   139,   140,   979,   142,   143,   144,    -1,
     146,   147,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,   157,   158,    -1,    -1,   161,    -1,    -1,   164,   165,
     166,   430,   431,   432,   433,   434,   435,    -1,   437,   438,
     439,   440,   441,    -1,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,    -1,    -1,   938,    -1,   940,    -1,    -1,   943,    -1,
      -1,    -1,    -1,   472,    -1,    -1,   475,   476,   477,   478,
     479,   480,    -1,    -1,    -1,   484,   485,    -1,    -1,   488,
      -1,    -1,   967,    -1,    -1,   494,    -1,    -1,    -1,    -1,
      -1,    -1,   977,    -1,    -1,    -1,   505,    -1,    -1,   508,
    1084,  1085,    -1,   512,    -1,    -1,    -1,    -1,    -1,    -1,
     519,    -1,   521,    -1,    -1,    -1,   525,    -1,    -1,   528,
     529,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   540,    -1,  1018,   543,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1027,   552,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   562,    -1,    -1,   565,   566,    -1,    -1,
      -1,    -1,   571,   572,    -1,    -1,   575,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    26,    27,   586,   587,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,
      -1,   600,    43,    -1,   603,  1080,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   624,    -1,    -1,    17,    18,
      71,    20,    -1,    -1,    -1,    76,    25,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,  1121,    88,  1123,    90,
      39,    -1,    -1,    -1,  1129,    -1,    45,  1132,    -1,    -1,
      49,     5,    -1,    52,    -1,    54,    -1,   108,    -1,   110,
     669,    -1,   113,    62,    -1,    -1,    -1,   676,    22,    -1,
      69,    70,    26,    27,    -1,    -1,  1161,    -1,    77,    -1,
      -1,    35,  1167,    37,    38,    -1,  1171,  1172,    87,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    96,  1183,    98,
      -1,   100,  1187,   102,  1189,    -1,   105,    -1,    -1,    -1,
     109,    -1,   111,    -1,    -1,   114,    -1,    71,    -1,    -1,
     729,   730,    76,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    85,    -1,    -1,    88,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,   752,    -1,   754,   755,   756,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,   771,  1248,  1249,  1250,  1251,    -1,    -1,   168,
      -1,   170,    -1,    -1,    -1,    -1,    -1,   786,    -1,    -1,
      -1,   790,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   817,   163,
      -1,    -1,   821,    -1,   823,   824,   825,    -1,   827,    -1,
     829,    -1,    -1,   832,   833,    -1,   167,    -1,    -1,   170,
      -1,   172,    -1,    -1,    -1,    -1,    -1,   846,    -1,    -1,
      -1,   850,    -1,    -1,    -1,   854,   855,    -1,   857,    -1,
      -1,    -1,    -1,    -1,   863,   864,    -1,   866,    -1,    -1,
     869,   870,    -1,    -1,    -1,    -1,    -1,   876,    -1,    -1,
      -1,    -1,   881,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   908,
     909,   910,   911,   912,   913,    -1,    -1,    -1,   917,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   931,   932,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   941,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   957,    -1,
      -1,    -1,   961,    -1,    -1,    -1,    -1,    -1,    -1,   968,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   976,    -1,   978,
     979,    -1,     3,     4,    -1,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
     999,    22,    -1,    24,    25,    -1,    -1,  1006,    -1,  1008,
      -1,    -1,    33,    -1,    35,    -1,  1015,    -1,    39,    40,
      41,    42,    -1,    44,    45,    -1,    -1,    -1,    49,  1028,
      -1,    52,    -1,    54,    -1,    56,  1035,    -1,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,  1054,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,  1068,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,  1084,  1085,    -1,   109,   110,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,  1097,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1105,  1106,  1107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
     141,    -1,  1121,    -1,  1123,    -1,    -1,    -1,  1127,    -1,
    1129,    -1,    -1,  1132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   162,    -1,    -1,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,    -1,  1154,  1155,    -1,  1157,  1158,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1177,    -1,
      -1,  1180,    -1,    -1,    -1,  1184,    -1,    -1,    -1,    -1,
    1189,  1190,    -1,    -1,  1193,    -1,    -1,    -1,    -1,    -1,
      -1,  1200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1213,  1214,  1215,  1216,  1217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1248,
    1249,  1250,  1251,  1252,  1253,  1254,  1255,     0,     1,    -1,
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
     163,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,   172,
      -1,     1,   175,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    19,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    49,
      50,    -1,    52,    53,    54,    -1,    56,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,    -1,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    -1,    -1,   117,    -1,   119,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,   168,   169,
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
      -1,   166,    -1,   168,    -1,   170,    -1,   172,    -1,     1,
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
      -1,    -1,   166,    -1,   168,    -1,   170,     1,   172,     3,
       4,   175,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      24,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    42,    -1,
      44,    45,    -1,    47,    -1,    49,    -1,    51,    52,    -1,
      54,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    77,    78,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,   108,   109,   110,   111,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   166,    -1,   168,    -1,   170,   171,   172,    -1,
       1,   175,     3,     4,    -1,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    24,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    42,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    -1,    56,    -1,    -1,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,   110,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,
     161,   162,    -1,    -1,    -1,   166,    -1,   168,    -1,   170,
      -1,   172,     3,     4,   175,     6,     7,     8,     9,    10,
      -1,    12,    -1,    14,    -1,    -1,    17,    18,    19,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    55,    56,    -1,    -1,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,   110,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,   136,   137,    -1,   139,   140,
     141,   142,   143,   144,    -1,   146,    -1,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,    -1,   170,
      -1,    -1,     3,     4,   175,     6,     7,     8,     9,    10,
      -1,    12,    -1,    14,    -1,    -1,    17,    18,    19,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    55,    56,    -1,    -1,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,   110,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,   136,   137,    -1,   139,   140,
     141,   142,   143,   144,    -1,   146,    -1,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,    -1,   164,   165,   166,   167,   168,    -1,   170,
      -1,    -1,    -1,    -1,   175,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    -1,    17,
      18,    -1,    20,    -1,    22,    23,    -1,    25,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    -1,    61,    62,    63,    64,    -1,    66,    67,
      -1,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,   100,    -1,   102,   103,    -1,   105,    -1,    -1,
      -1,   109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,    -1,     3,     4,   175,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
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
     168,    -1,   170,    -1,    -1,     3,     4,   175,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    26,    27,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,   110,   111,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,    -1,     3,     4,   175,     6,     7,
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
      -1,   109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,   172,     3,     4,   175,     6,     7,
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
      -1,   109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   166,    -1,
     168,    -1,   170,    -1,   172,     3,     4,   175,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    89,    -1,    -1,    -1,    93,    -1,    -1,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,   145,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,
     168,   169,   170,    -1,    -1,     3,     4,   175,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    56,    -1,
      -1,    59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,
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
      98,    99,   100,    -1,   102,    -1,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
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
      98,    99,   100,    -1,   102,    -1,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
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
      98,    99,   100,    -1,   102,    -1,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,
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
     105,   106,   107,    -1,   109,   110,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,   171,    -1,     3,     4,
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
     105,   106,   107,    -1,   109,   110,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,   171,    -1,     3,     4,
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
     105,   106,   107,    -1,   109,   110,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,   171,    -1,     3,     4,
     175,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,   106,   107,    -1,   109,   110,   111,    -1,    -1,   114,
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
     105,    -1,    -1,    -1,   109,   110,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
     145,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,    -1,
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
     105,    -1,    -1,    -1,   109,   110,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
     145,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,    -1,
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
     105,    -1,    -1,    -1,   109,   110,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
     145,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,   163,    -1,
      -1,   166,    -1,   168,   169,   170,    -1,    -1,     3,     4,
     175,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,
      -1,    56,    -1,    -1,    59,    -1,    -1,    62,    -1,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,   110,   111,    -1,   113,   114,
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
     105,   106,   107,    -1,   109,   110,   111,    -1,    -1,   114,
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
      -1,    86,    87,    88,    89,    -1,    -1,    -1,    93,    -1,
      -1,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,   110,   111,    -1,    -1,   114,
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
     105,    -1,    -1,    -1,   109,   110,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,    -1,
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
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,   110,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,
     162,   163,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,
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
     109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,   145,    -1,    -1,   148,
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
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,   110,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,   160,   161,   162,   163,    -1,    -1,   166,    -1,   168,
      -1,   170,    -1,    -1,   173,    -1,   175,     3,     4,    -1,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    35,
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
      -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,    -1,     3,     4,   175,
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
      -1,    -1,    -1,    -1,   120,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,    -1,     3,     4,   175,
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
      -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
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
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,    -1,    -1,   160,   161,   162,
      -1,    -1,    -1,   166,    -1,   168,   169,   170,    -1,    -1,
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
      -1,    -1,    -1,   166,    -1,   168,    -1,   170,    -1,    -1,
     173,    -1,   175,     3,     4,    -1,     6,     7,     8,     9,
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
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,     4,
      -1,     6,     7,     8,     9,    10,    -1,   157,    -1,    14,
     160,   161,   162,    18,    -1,    -1,   166,    22,   168,    24,
     170,    -1,    27,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,    64,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    78,    79,    80,    81,    -1,    -1,    -1,
      -1,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,   110,   111,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,    -1,    -1,   160,   161,   162,    -1,    -1,
      -1,   166,    -1,   168,    -1,   170,   171,   172,    -1,     4,
     175,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    22,    -1,    24,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    42,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,    64,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    78,    79,    80,    81,    -1,    -1,    -1,
      -1,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,   110,   111,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,     3,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    12,    -1,   148,    -1,    -1,    17,    18,    19,    20,
      -1,    -1,   157,    -1,    25,   160,   161,   162,    -1,    -1,
      -1,   166,    33,   168,    35,   170,    -1,   172,    39,    -1,
     175,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    -1,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,   110,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    -1,   136,   137,    -1,   139,   140,
     141,   142,   143,   144,    -1,   146,    -1,    -1,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,    -1,
     161,    -1,    -1,   164,   165,   166,   167,     3,     4,   170,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    24,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    42,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,   110,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,    -1,   161,   162,    -1,    -1,    -1,
     166,    -1,   168,    -1,   170,    -1,   172,     3,     4,    -1,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    24,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    42,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    -1,
      56,    -1,    -1,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,     3,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,   110,   111,    -1,    -1,   114,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,   139,    -1,   141,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    62,    -1,   162,    -1,    -1,    -1,
      -1,    69,    70,    -1,   170,    -1,   172,    17,    18,    77,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    96,    39,
      98,    -1,   100,    -1,   102,    45,    -1,   105,    -1,    49,
      -1,   109,    52,   111,    54,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,     3,    -1,    -1,    96,    -1,    98,    -1,
     100,    -1,   102,    -1,    -1,   105,    -1,    17,    18,   109,
      20,   111,   170,    -1,   114,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,
      -1,    -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    -1,
     100,     3,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      12,   111,    -1,    -1,   114,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,
     170,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    98,    -1,   100,    -1,
     102,    -1,    -1,    -1,    -1,    -1,   108,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,   136,   137,    -1,   139,   140,   141,
     142,   143,   144,    -1,   146,    -1,    -1,    -1,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,     3,   161,
      -1,    -1,   164,   165,   166,   167,   168,    12,    -1,    -1,
      -1,    -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
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
     155,   156,   157,   158,   159,     5,   161,    -1,    -1,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,
      -1,    -1,     5,    -1,    -1,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    22,
      50,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,    -1,
      43,    71,    -1,    -1,    -1,    -1,    76,    50,    78,    -1,
      80,    -1,    82,    -1,    -1,    85,    -1,    -1,    88,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      -1,     5,    -1,    76,    -1,    78,    -1,    80,   108,    82,
     110,    -1,    85,   113,    -1,    88,    -1,    90,    22,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    37,    38,   108,    12,   110,    -1,    43,
     113,    -1,    -1,    19,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,    -1,    -1,    71,    -1,   169,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    85,    -1,    -1,    88,    -1,    90,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    89,    12,    -1,    -1,    93,    -1,    -1,
      -1,    19,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   163,
     136,   137,    -1,   139,   140,   169,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    89,   168,    -1,    12,    93,    -1,    -1,    -1,   175,
      -1,    19,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,   136,   137,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    -1,
     168,    89,    12,    -1,    -1,    93,    -1,   175,    -1,    19,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,   136,   137,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
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
      -1,    93,    -1,    -1,    -1,   175,    -1,    -1,    12,   101,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    47,   136,   137,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,    -1,   168,    -1,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    89,    12,    -1,    -1,    93,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,   122,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,    89,   168,    -1,    -1,    93,    -1,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    31,   122,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    72,    -1,   164,   165,
     166,    -1,   168,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,   122,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    72,   163,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    -1,    89,    12,    -1,    -1,    93,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,   175,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    -1,
     168,    89,    12,    -1,    -1,    93,    -1,   175,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    31,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,    89,
     168,    -1,    12,    93,    -1,    -1,    -1,   175,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    47,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,   139,
     140,   101,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   122,    -1,   164,   165,   166,    -1,   168,    89,
      -1,    -1,    -1,    93,    -1,   175,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   122,    -1,   164,   165,   166,    -1,   168,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    12,    -1,   164,   165,   166,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,   118,   164,   165,   166,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,   118,   164,
     165,   166,   122,    -1,    -1,    -1,    12,    13,   173,    -1,
     175,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    31,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,    12,    -1,    -1,
      -1,    -1,    -1,   173,    19,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,   122,   164,   165,
     166,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,
      -1,    -1,    89,    -1,   139,   140,    93,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    12,    -1,   164,
     165,   166,    -1,   168,    19,   122,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    12,   163,   164,   165,   166,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,
      12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,   139,   140,    93,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    12,    -1,   164,   165,   166,
     122,    -1,    19,    -1,   171,    -1,    -1,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      12,    -1,   164,   165,   166,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,   121,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    89,   175,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     122,    -1,   164,   165,   166,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,   175,    -1,    19,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    31,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,   175,    -1,    -1,    12,    13,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,   139,   140,    93,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,    -1,    -1,    -1,   122,   171,    -1,    -1,
      -1,   175,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   122,    -1,   164,   165,
     166,    -1,    -1,    -1,    12,    13,    -1,    -1,    -1,   175,
      -1,    19,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    31,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,    -1,    -1,    -1,    12,   171,    -1,    -1,    -1,   175,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   122,    -1,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   175,    -1,    89,
      -1,   139,   140,    93,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,   164,   165,   166,    12,
      -1,   121,   122,    -1,    -1,    -1,    19,   175,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,   139,   140,    93,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,    -1,    -1,    -1,   122,    -1,    -1,
     173,    -1,   175,    -1,    89,    12,    -1,    -1,    93,    -1,
      -1,    -1,    19,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    31,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   122,    -1,   164,
     165,   166,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
     175,    -1,    19,    60,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    31,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,
     175,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     139,   140,   101,   142,   143,   144,    -1,   146,   147,   148,
      31,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   122,    -1,   164,   165,   166,    -1,    -1,
      89,    12,    -1,    -1,    93,    -1,   175,    -1,    19,    -1,
     139,   140,   101,   142,   143,   144,    -1,   146,   147,   148,
      31,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   122,    -1,   164,   165,   166,    89,    -1,
      -1,    12,    93,    -1,    -1,    -1,   175,    -1,    19,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      31,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,   175,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,    -1,   155,   156,   157,   158,   159,    -1,
     161,   122,    -1,   164,   165,   166,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,   175,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   122,    -1,   164,   165,   166,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    31,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,    22,   164,   165,   166,    26,    27,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    35,    -1,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    89,    -1,    -1,    -1,
      93,    71,    -1,    73,    -1,    -1,    76,    -1,    78,    -1,
      -1,    -1,    82,    83,    84,    85,    -1,    -1,    88,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,     3,   155,   156,   157,   158,   159,    -1,   161,    -1,
      -1,   164,   165,   166,    -1,    17,    18,    -1,    20,    -1,
      -1,    -1,   175,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,   163,    -1,    -1,    -1,    39,   168,    -1,
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
      -1,    -1,    -1,     3,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    17,    18,    77,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    96,    39,
      98,    -1,   100,    -1,   102,    45,    -1,   105,    -1,    49,
      -1,   109,    52,   111,    54,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    -1,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,   111,    -1,    -1,   114
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
     383,   163,    22,    35,    44,    54,    56,    88,   100,   110,
     168,   238,   249,   346,   353,   361,   362,   367,   370,   372,
     373,   118,   355,   356,     3,    54,   216,   367,   355,   108,
     322,    90,   216,   184,   337,   367,   170,     3,    17,    18,
      20,    25,    33,    39,    45,    49,    52,    54,    62,    69,
      70,    77,    87,    96,    98,   100,   102,   105,   109,   111,
     114,   217,   218,   220,    11,    27,   113,   244,   367,    82,
      85,   200,   170,   102,   218,   218,   218,   170,   218,   324,
     325,    32,   204,   223,   367,   261,   262,   367,   367,    18,
      77,    96,   111,   367,   367,   367,     9,   170,   227,   226,
      27,    33,    47,    49,    51,    75,    78,    90,   100,   108,
     113,   183,   222,   277,   278,   279,   300,   301,   302,   329,
     334,   367,   337,   106,   107,   163,   281,   282,   366,   367,
     369,   367,   223,   367,   367,    99,   170,   184,   367,   367,
      71,   186,   190,   204,   186,   204,   367,   369,   367,   367,
     367,   367,   367,     1,   169,   182,   205,   337,   149,   341,
     342,   369,   367,    80,   184,    22,    35,    38,    71,    88,
     110,   168,   187,   188,   189,   200,   204,   192,   147,   194,
     168,    46,    84,   112,   201,    26,   321,   367,     9,   265,
     367,   368,    24,    32,    40,    41,    42,   117,   172,   240,
     241,   349,   351,   170,   204,    56,   144,   267,   218,   168,
      53,    73,    83,   304,    27,    47,    51,    75,    78,    90,
     108,   113,   305,    27,    47,    51,    75,    78,    90,   108,
     113,   303,   218,   316,   317,     1,   324,   162,   163,   367,
      12,    19,    31,    89,    93,   122,   139,   140,   142,   143,
     144,   146,   147,   148,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   164,   165,   166,   175,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   167,   293,   170,   172,    89,
      93,   367,   218,    40,   172,   240,    56,     1,   170,   184,
     163,   204,   320,   163,   145,   163,   219,   343,   344,   345,
     369,   218,   218,   101,   204,   101,   123,   204,   293,   201,
     343,   163,   170,   184,   170,   218,   326,   327,     1,   144,
     333,    47,   145,   184,   223,   145,   223,    13,   170,   170,
     223,   323,   343,   228,   228,    47,    90,   301,   302,   171,
     145,   170,   218,   144,   163,   367,   367,   121,   283,   163,
     168,   223,   170,   343,   163,   367,   254,   254,   163,   169,
     169,   182,   145,   169,   367,   145,   145,   171,   171,     9,
     367,   172,   240,   241,     3,   170,   195,     1,   169,   205,
     212,   213,   367,   207,   367,    65,    36,    72,   163,   265,
     267,   108,   233,   235,   286,   184,    78,   244,   367,   120,
     173,   242,   337,   338,   339,   367,   378,   242,   367,   218,
     247,   248,   218,   337,   168,     1,   169,   191,   218,   271,
     274,    47,    90,     1,   144,   331,   332,    47,    90,     1,
       3,    12,    17,    19,    20,    25,    45,    52,    54,    55,
      62,    70,    87,    98,   102,   109,   114,   139,   140,   141,
     142,   143,   144,   146,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   161,   164,   165,   166,   167,   170,
     218,   290,   291,   292,   293,   346,   123,   318,   145,   163,
     163,   163,   367,   367,   367,   242,   367,   242,   367,   367,
     367,   367,   367,   367,   367,     3,    20,    33,    62,   102,
     108,   219,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,    68,
     369,   369,   369,   343,   343,   242,   367,   242,   367,   101,
     170,   337,   242,   367,   218,     1,    27,    47,    51,    75,
      78,    90,   108,   113,   171,   357,   358,   359,   367,   383,
     324,   367,   123,   171,   145,   123,   123,   184,    34,   184,
     367,    34,   367,    65,   171,   344,   327,   145,   171,   222,
     367,   123,   328,   367,   325,   261,   218,   319,   367,   367,
     320,   171,   218,   305,   279,   327,   144,   335,   336,   334,
     284,    32,   203,   308,     1,   259,   343,   171,    21,   255,
     169,   171,   171,   341,   369,   171,   341,   337,   242,   367,
     194,   171,   196,   197,   345,   169,   169,   145,   163,    13,
     147,   145,   163,    13,    36,    72,   218,   139,   140,   141,
     142,   143,   157,   161,   166,   206,   292,   293,   294,   367,
     206,   208,   267,   168,   236,   320,   117,   218,   223,   223,
     170,   367,   173,   356,    47,   145,   163,   173,   340,   356,
     337,   173,   340,   121,   145,   173,    32,   223,   356,    32,
      47,   223,   356,   145,   171,   163,   170,     1,   268,   169,
     274,   123,   145,   169,     1,   222,   329,   330,   104,   306,
     170,   296,   367,   139,   147,   296,   296,   367,   317,   170,
     172,   163,   163,   163,   163,   171,   173,   344,    47,   173,
      32,    32,    47,   170,   171,    47,    90,   145,   171,    17,
      20,    25,    45,    52,    62,    70,    87,    98,   109,   114,
     346,    89,    89,   163,   369,   345,   367,   367,    34,   184,
      34,    34,   184,   101,   204,   218,   171,   171,   326,   333,
      68,   369,   218,   171,   171,   324,   336,   144,   280,   171,
     334,   149,   295,   328,   367,   182,   169,    74,   115,   169,
     260,   171,   170,   204,   218,   256,    47,   173,    47,   145,
     171,   212,   219,    17,    18,    20,    25,    45,    49,    52,
      70,    77,    87,    96,    98,   100,   109,   111,   114,   168,
     211,   294,   367,   367,   206,   208,   145,   163,    13,   163,
     168,   268,   316,   324,   367,    47,   337,   184,   173,   242,
     367,   173,   340,   184,   367,   163,   173,   173,   145,   173,
     367,   173,   367,   367,   223,    44,   367,   242,   367,   223,
     248,   204,   344,   169,    82,    85,   169,   183,   191,   225,
     367,   272,   273,    60,   116,   310,   297,   298,   299,   300,
     171,   291,   293,   171,   173,   171,   242,   367,    44,   242,
     367,   344,   358,   333,   346,   346,   184,   184,   367,   184,
      34,   163,   163,   328,   170,   170,   163,   295,   328,   329,
     306,   336,   367,   184,   238,   337,   256,   144,   204,   242,
     367,   242,   367,   197,   206,    13,    36,    72,   163,   163,
     294,   367,   367,   268,   169,   163,   163,   163,   367,   171,
     184,   173,   356,   173,   356,   173,   184,   121,   367,    32,
     223,   356,    32,   223,   356,   171,   191,   225,   225,   274,
     191,     3,    54,    92,   102,   311,   312,   313,   367,   287,
     145,   171,   296,   296,   173,   173,   367,    32,    32,   171,
     328,    34,   184,   343,   343,   295,   328,    32,   223,   171,
     367,   173,   173,   169,   367,   206,   208,    13,   169,   223,
      44,   184,   367,   173,    44,   184,   367,   173,   367,   101,
      44,   367,   223,    44,   367,   223,   163,   225,   274,   313,
     116,   145,   123,   150,   152,   153,   155,   156,    60,   163,
     307,   308,   299,    44,    44,   101,    44,    44,   184,   171,
     171,   184,   204,   163,   163,   367,   367,   184,   367,   184,
     367,   367,   367,   367,   312,   313,   313,   313,   313,   313,
     313,   311,   367,   367,   367,   367,   162,   162,   101,   101,
     101,   101,   101,   101,   101,   101,   367,   367,   367,   367
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
     261,   261,   261,   262,   262,   263,   264,   264,   265,   266,
     266,   266,   267,   267,   268,   268,   268,   268,   268,   269,
     269,   269,   270,   271,   271,   272,   271,   271,   273,   271,
     274,   274,   275,   276,   276,   276,   277,   277,   278,   278,
     279,   279,   279,   280,   280,   281,   283,   282,   284,   282,
     285,   287,   286,   288,   288,   288,   288,   288,   289,   290,
     290,   291,   291,   291,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   293,
     293,   293,   293,   293,   293,   293,   293,   293,   293,   293,
     293,   293,   293,   294,   294,   295,   295,   296,   296,   297,
     297,   298,   298,   299,   300,   300,   300,   300,   300,   300,
     301,   301,   302,   302,   302,   302,   302,   302,   302,   302,
     302,   302,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   304,   304,   304,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   305,   306,   306,   307,
     307,   308,   308,   309,   310,   310,   310,   310,   310,   311,
     311,   312,   312,   312,   312,   312,   312,   312,   313,   313,
     313,   314,   315,   315,   316,   316,   317,   318,   318,   319,
     319,   319,   319,   319,   320,   322,   321,   323,   321,   321,
     321,   324,   324,   325,   325,   326,   326,   327,   327,   327,
     328,   328,   328,   329,   330,   330,   330,   331,   331,   332,
     332,   333,   333,   333,   333,   334,   334,   335,   336,   336,
     337,   337,   338,   338,   338,   339,   339,   340,   340,   341,
     342,   342,   343,   343,   344,   344,   345,   345,   346,   346,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   348,   349,   349,   349,   349,   349,   349,   349,   350,
     351,   351,   351,   351,   351,   351,   351,   352,   353,   354,
     354,   354,   354,   354,   354,   355,   355,   356,   356,   356,
     356,   357,   357,   358,   358,   358,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   360,   360,   360,   360,
     360,   361,   361,   361,   361,   361,   362,   363,   363,   363,
     363,   363,   363,   364,   365,   366,   366,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   368,   368,   369,   369,   369,   370,   370,
     370,   370,   371,   371,   371,   371,   372,   372,   372,   373,
     373,   373,   373,   373,   373,   374,   374,   374,   374,   375,
     375,   376,   376,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   378,
     378,   379,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   379,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   379,   380,   380,   380,   380,   380,   380,
     380,   381,   381,   381,   381,   382,   382,   382,   382,   383,
     383,   383,   383,   383,   383,   383
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
       4,     3,     1,     1,     3,     3,     5,     5,     2,     1,
       1,     1,     0,     2,     0,     2,     3,     3,     4,     4,
       4,     3,     2,     1,     2,     0,     4,     2,     0,     5,
       1,     3,     0,     1,     1,     1,     0,     1,     1,     3,
       1,     3,     1,     0,     2,     6,     0,     3,     0,     4,
       1,     0,     7,     4,     4,     6,     6,     4,     2,     1,
       3,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     0,     3,     1,
       3,     0,     1,     1,     4,     5,     4,     5,     6,     6,
       0,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     0,     1,     1,     2,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     0,     1,     1,     2,
       1,     1,     1,     1,     2,     1,     1,     0,     1,     1,
       1,     1,     2,     1,     0,     2,     2,     4,     4,     1,
       3,     3,     3,     3,     3,     3,     3,     2,     1,     1,
       1,     3,     1,     2,     1,     3,     2,     0,     2,     1,
       2,     1,     1,     1,     1,     0,     5,     0,     6,     3,
       3,     1,     3,     3,     5,     1,     3,     1,     2,     3,
       0,     2,     2,     1,     1,     1,     1,     2,     1,     0,
       1,     0,     2,     2,     1,     1,     1,     2,     0,     1,
       1,     3,     2,     3,     2,     2,     3,     1,     2,     1,
       3,     3,     0,     1,     1,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     4,     5,     2,
       2,     1,     2,     1,     2,     1,     2,     1,     2,     1,
       1,     2,     6,     6,     4,     4,     9,     9,     7,     2,
       2,     3,     4,     6,     6,     9,     9,     6,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     4,     3,     4,
       2,     1,     3,     4,     3,     3,     1,     1,     2,     1,
       2,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     2,     8,     8,     9,     9,     4,     3,     3,     2,
       2,     2,     1,     3,     4,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     4,     4,     4,     3,
       3,     3,     3,     5,     5,     3,     4,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       4,     4,     5,     4,     5,     5,     6,     4,     5,     3,
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
#line 691 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 5949 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 695 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 5955 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 696 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 5961 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 703 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 5969 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 711 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 5977 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 715 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 5986 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 725 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 5994 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 730 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6002 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 742 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)); }
#line 6008 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 744 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6014 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 745 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6020 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 746 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6026 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 747 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6032 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 748 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6038 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 749 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6044 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 750 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6050 "bison-chpl-lib.cpp"
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
#line 6071 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: try_stmt  */
#line 768 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6077 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: return_stmt  */
#line 769 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6083 "bison-chpl-lib.cpp"
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
#line 6095 "bison-chpl-lib.cpp"
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
#line 6108 "bison-chpl-lib.cpp"
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
#line 6121 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 797 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6129 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 801 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6140 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 808 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6149 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 815 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6155 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 816 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6161 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 817 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6167 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 818 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6173 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 819 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6179 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 820 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6185 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 821 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6191 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 823 "chpl.ypp"
  { (yyval.commentsAndStmt) = context->buildBeginStmt((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].withClause), (yyvsp[0].commentsAndStmt)); }
#line 6197 "bison-chpl-lib.cpp"
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
#line 6220 "bison-chpl-lib.cpp"
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
#line 6232 "bison-chpl-lib.cpp"
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
#line 6252 "bison-chpl-lib.cpp"
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
#line 6271 "bison-chpl-lib.cpp"
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
#line 6291 "bison-chpl-lib.cpp"
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
#line 6311 "bison-chpl-lib.cpp"
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
#line 6330 "bison-chpl-lib.cpp"
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
#line 6351 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 955 "chpl.ypp"
  {
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 6359 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 976 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6367 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_begin: TATTRIBUTEIDENT  */
#line 983 "chpl.ypp"
  {
    auto ident = context->buildAttributeIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6376 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_attribute_decl_toolspace: %empty  */
#line 990 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6384 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 994 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6395 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_actuals: TLP TRP  */
#line 1004 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6401 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 1006 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6409 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: %empty  */
#line 1010 "chpl.ypp"
  {
    (yyval.maybeNamedActualList)=new MaybeNamedActualList();
  }
#line 6417 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 1017 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6426 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 1022 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6436 "bison-chpl-lib.cpp"
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
#line 6458 "bison-chpl-lib.cpp"
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
#line 6480 "bison-chpl-lib.cpp"
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
#line 6502 "bison-chpl-lib.cpp"
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
#line 6522 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1110 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6528 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1111 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6535 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1113 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6542 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1118 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6548 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1119 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6555 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1125 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr), LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6565 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1143 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-1]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6574 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1148 "chpl.ypp"
  {
    context->noteCurlyBraces((yylsp[-2]), (yylsp[0]));
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6583 "bison-chpl-lib.cpp"
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
#line 6596 "bison-chpl-lib.cpp"
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
#line 6619 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1187 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6625 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1188 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6631 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1193 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6639 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1197 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6647 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1201 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6656 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1206 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6664 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1210 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6672 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1214 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6681 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1222 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6690 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1227 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6700 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1233 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6709 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1238 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6720 "bison-chpl-lib.cpp"
    break;

  case 90: /* opt_only_ls: %empty  */
#line 1246 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6726 "bison-chpl-lib.cpp"
    break;

  case 91: /* opt_only_ls: renames_ls  */
#line 1247 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6732 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1252 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6741 "bison-chpl-lib.cpp"
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
#line 6753 "bison-chpl-lib.cpp"
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
#line 6766 "bison-chpl-lib.cpp"
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
#line 6778 "bison-chpl-lib.cpp"
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
#line 6791 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1293 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6800 "bison-chpl-lib.cpp"
    break;

  case 114: /* import_expr: expr  */
#line 1328 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6808 "bison-chpl-lib.cpp"
    break;

  case 115: /* import_expr: expr TDOT all_op_name  */
#line 1332 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6817 "bison-chpl-lib.cpp"
    break;

  case 116: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1337 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6826 "bison-chpl-lib.cpp"
    break;

  case 117: /* import_expr: expr TAS ident_use  */
#line 1342 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6836 "bison-chpl-lib.cpp"
    break;

  case 118: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1348 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6846 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_ls: import_expr  */
#line 1356 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6852 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_ls: import_ls TCOMMA import_expr  */
#line 1357 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6858 "bison-chpl-lib.cpp"
    break;

  case 121: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1362 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = makeCommentsAndStmt(comments, node.release());
  }
#line 6868 "bison-chpl-lib.cpp"
    break;

  case 122: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1371 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6876 "bison-chpl-lib.cpp"
    break;

  case 123: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1375 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6884 "bison-chpl-lib.cpp"
    break;

  case 124: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1379 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6892 "bison-chpl-lib.cpp"
    break;

  case 125: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1383 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6900 "bison-chpl-lib.cpp"
    break;

  case 126: /* opt_label_ident: %empty  */
#line 1391 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6906 "bison-chpl-lib.cpp"
    break;

  case 127: /* opt_label_ident: TIDENT  */
#line 1392 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6912 "bison-chpl-lib.cpp"
    break;

  case 128: /* opt_label_ident: TINIT  */
#line 1393 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6918 "bison-chpl-lib.cpp"
    break;

  case 137: /* ident_use: TIDENT  */
#line 1419 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6924 "bison-chpl-lib.cpp"
    break;

  case 138: /* ident_use: TINIT  */
#line 1420 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6930 "bison-chpl-lib.cpp"
    break;

  case 139: /* ident_use: TTHIS  */
#line 1421 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6936 "bison-chpl-lib.cpp"
    break;

  case 158: /* scalar_type: TBOOL  */
#line 1447 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6942 "bison-chpl-lib.cpp"
    break;

  case 159: /* scalar_type: TENUM  */
#line 1448 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6948 "bison-chpl-lib.cpp"
    break;

  case 160: /* scalar_type: TUNION  */
#line 1449 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6954 "bison-chpl-lib.cpp"
    break;

  case 161: /* scalar_type: TINT  */
#line 1450 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6960 "bison-chpl-lib.cpp"
    break;

  case 162: /* scalar_type: TUINT  */
#line 1451 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6966 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TREAL  */
#line 1452 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6972 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TIMAG  */
#line 1453 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6978 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TCOMPLEX  */
#line 1454 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6984 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TBYTES  */
#line 1455 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6990 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TSTRING  */
#line 1456 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6996 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TLOCALE  */
#line 1457 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7002 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TNOTHING  */
#line 1458 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7008 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TVOID  */
#line 1459 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7014 "bison-chpl-lib.cpp"
    break;

  case 174: /* do_stmt: TDO stmt  */
#line 1472 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7020 "bison-chpl-lib.cpp"
    break;

  case 175: /* do_stmt: block_stmt  */
#line 1473 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7026 "bison-chpl-lib.cpp"
    break;

  case 176: /* return_stmt: TRETURN TSEMI  */
#line 1478 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7037 "bison-chpl-lib.cpp"
    break;

  case 177: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1485 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7048 "bison-chpl-lib.cpp"
    break;

  case 178: /* class_level_stmt: TSEMI  */
#line 1495 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7056 "bison-chpl-lib.cpp"
    break;

  case 179: /* class_level_stmt: inner_class_level_stmt  */
#line 1499 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7066 "bison-chpl-lib.cpp"
    break;

  case 180: /* $@1: %empty  */
#line 1504 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7073 "bison-chpl-lib.cpp"
    break;

  case 181: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1506 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->adjustLocation((yyval.commentsAndStmt), (yylsp[-2]), (yylsp[0]));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7083 "bison-chpl-lib.cpp"
    break;

  case 182: /* $@2: %empty  */
#line 1511 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7090 "bison-chpl-lib.cpp"
    break;

  case 183: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1513 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->adjustLocation((yyval.commentsAndStmt), (yylsp[-2]), (yylsp[0]));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7100 "bison-chpl-lib.cpp"
    break;

  case 191: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1532 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7109 "bison-chpl-lib.cpp"
    break;

  case 192: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1537 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7118 "bison-chpl-lib.cpp"
    break;

  case 193: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1542 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7127 "bison-chpl-lib.cpp"
    break;

  case 194: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1547 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7135 "bison-chpl-lib.cpp"
    break;

  case 195: /* forwarding_decl_start: TFORWARDING  */
#line 1554 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7144 "bison-chpl-lib.cpp"
    break;

  case 196: /* extern_or_export: TEXTERN  */
#line 1561 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7150 "bison-chpl-lib.cpp"
    break;

  case 197: /* extern_or_export: TEXPORT  */
#line 1562 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7156 "bison-chpl-lib.cpp"
    break;

  case 198: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1567 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7166 "bison-chpl-lib.cpp"
    break;

  case 199: /* extern_export_kind_inner: TTYPE  */
#line 1580 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7175 "bison-chpl-lib.cpp"
    break;

  case 200: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1589 "chpl.ypp"
  {
    auto headerLoc = context->makeSpannedLocation((yylsp[-5]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7186 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1597 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    auto headerLoc = context->makeSpannedLocation((yylsp[-6]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7199 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1606 "chpl.ypp"
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
#line 7221 "bison-chpl-lib.cpp"
    break;

  case 203: /* $@3: %empty  */
#line 1624 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7229 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 1628 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7238 "bison-chpl-lib.cpp"
    break;

  case 205: /* $@4: %empty  */
#line 1633 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7247 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr extern_export_kind_inner $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1638 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-1])), (yyvsp[-3].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7256 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1646 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yyloc), (yyvsp[0].sizedStr));
  }
#line 7264 "bison-chpl-lib.cpp"
    break;

  case 208: /* no_loop_attributes: %empty  */
#line 1652 "chpl.ypp"
  {
    // Even though there are no attributes, there could be loop pragmas.
    context->buildAndPushAttributeGroup((yyloc));
  }
#line 7273 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1658 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7279 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1659 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7285 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1676 "chpl.ypp"
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
#line 7311 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1698 "chpl.ypp"
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
#line 7333 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1716 "chpl.ypp"
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
#line 7355 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1734 "chpl.ypp"
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
#line 7383 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: for_loop_kw expr TIN expr do_stmt  */
#line 1758 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7392 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: for_loop_kw expr TIN expr task_intent_clause do_stmt  */
#line 1763 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7401 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator do_stmt  */
#line 1768 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7410 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: for_loop_kw expr TIN zippered_iterator task_intent_clause do_stmt  */
#line 1773 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-4]), locHeader, (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7419 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: for_loop_kw expr do_stmt  */
#line 1778 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7428 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: for_loop_kw expr task_intent_clause do_stmt  */
#line 1783 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7437 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: for_loop_kw zippered_iterator do_stmt  */
#line 1788 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-2]), locHeader, (yylsp[0]), (yyvsp[-2].uniqueStr), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7446 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: for_loop_kw zippered_iterator task_intent_clause do_stmt  */
#line 1793 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildGeneralLoopStmt((yyloc), (yylsp[-3]), locHeader, (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7455 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1798 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7464 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TLSBR expr_ls TIN expr task_intent_clause TRSBR stmt  */
#line 1803 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7473 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1808 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-4]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7482 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator task_intent_clause TRSBR stmt  */
#line 1813 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-5]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7491 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1818 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7500 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TLSBR expr_ls task_intent_clause TRSBR stmt  */
#line 1823 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7509 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1828 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-2]));
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7519 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TLSBR zippered_iterator task_intent_clause TRSBR stmt  */
#line 1834 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-3]), (yylsp[-3]));
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yyloc), locHeader, (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7529 "bison-chpl-lib.cpp"
    break;

  case 235: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1843 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7539 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF expr TTHEN stmt  */
#line 1852 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7547 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr block_stmt  */
#line 1856 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7555 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1860 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7563 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1864 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7571 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1868 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7579 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar block_stmt  */
#line 1872 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7587 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1876 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7595 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1880 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7603 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1884 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-1]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7612 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1889 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[0]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7621 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1894 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7630 "bison-chpl-lib.cpp"
    break;

  case 247: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1899 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yyloc), (yylsp[-2]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7639 "bison-chpl-lib.cpp"
    break;

  case 248: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1907 "chpl.ypp"
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
#line 7658 "bison-chpl-lib.cpp"
    break;

  case 249: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1922 "chpl.ypp"
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
#line 7677 "bison-chpl-lib.cpp"
    break;

  case 250: /* interface_start: TINTERFACE ident_def  */
#line 1940 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Interface);
  }
#line 7685 "bison-chpl-lib.cpp"
    break;

  case 251: /* interface_stmt: interface_start TLP ifc_formal_ls TRP block_stmt  */
#line 1947 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-4].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-4]), parts, (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC((yyvsp[0].commentsAndStmt).curlyLoc));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7696 "bison-chpl-lib.cpp"
    break;

  case 252: /* interface_stmt: interface_start block_stmt  */
#line 1954 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-1].typeDeclParts);
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yylsp[-1]), parts, nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC((yyvsp[0].commentsAndStmt).curlyLoc));
    context->exitScope(asttags::Interface, parts.name);
  }
#line 7707 "bison-chpl-lib.cpp"
    break;

  case 253: /* ifc_formal_ls: ifc_formal  */
#line 1963 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7713 "bison-chpl-lib.cpp"
    break;

  case 254: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1964 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7719 "bison-chpl-lib.cpp"
    break;

  case 255: /* ifc_formal: ident_def  */
#line 1969 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7725 "bison-chpl-lib.cpp"
    break;

  case 275: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 2012 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7733 "bison-chpl-lib.cpp"
    break;

  case 276: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 2016 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7741 "bison-chpl-lib.cpp"
    break;

  case 277: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 2020 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7749 "bison-chpl-lib.cpp"
    break;

  case 278: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2027 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7757 "bison-chpl-lib.cpp"
    break;

  case 279: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 2031 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7765 "bison-chpl-lib.cpp"
    break;

  case 280: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2035 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7773 "bison-chpl-lib.cpp"
    break;

  case 281: /* try_stmt: TTRY tryable_stmt  */
#line 2042 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7781 "bison-chpl-lib.cpp"
    break;

  case 282: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2046 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7789 "bison-chpl-lib.cpp"
    break;

  case 283: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2050 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7797 "bison-chpl-lib.cpp"
    break;

  case 284: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2054 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7805 "bison-chpl-lib.cpp"
    break;

  case 285: /* catch_expr_ls: %empty  */
#line 2060 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7811 "bison-chpl-lib.cpp"
    break;

  case 286: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2061 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7817 "bison-chpl-lib.cpp"
    break;

  case 287: /* catch_expr: TCATCH block_stmt  */
#line 2066 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false, TextLocation::create());
  }
#line 7825 "bison-chpl-lib.cpp"
    break;

  case 288: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2070 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false, TextLocation::create());
  }
#line 7833 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2074 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true, context->makeSpannedLocation((yylsp[-3]), (yylsp[-1])));
  }
#line 7841 "bison-chpl-lib.cpp"
    break;

  case 290: /* catch_expr_inner: ident_def  */
#line 2081 "chpl.ypp"
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
#line 7860 "bison-chpl-lib.cpp"
    break;

  case 291: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2096 "chpl.ypp"
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
#line 7879 "bison-chpl-lib.cpp"
    break;

  case 292: /* throw_stmt: TTHROW expr TSEMI  */
#line 2114 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = makeCommentsAndStmt(comments, node.release());
  }
#line 7889 "bison-chpl-lib.cpp"
    break;

  case 293: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2122 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), context->makeSpannedLocation((yylsp[-4]), (yylsp[-3])), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7898 "bison-chpl-lib.cpp"
    break;

  case 294: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2127 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = makeCommentsAndStmt(comments, node.release());
    BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
  }
#line 7909 "bison-chpl-lib.cpp"
    break;

  case 295: /* when_stmt_ls: %empty  */
#line 2136 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 7915 "bison-chpl-lib.cpp"
    break;

  case 296: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2137 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7921 "bison-chpl-lib.cpp"
    break;

  case 297: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2142 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), context->makeSpannedLocation((yylsp[-2]), (yylsp[-1])), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 7929 "bison-chpl-lib.cpp"
    break;

  case 298: /* when_stmt: TOTHERWISE stmt  */
#line 2146 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-1]), nullptr, blockOrDo);
  }
#line 7938 "bison-chpl-lib.cpp"
    break;

  case 299: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2151 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yylsp[-3]), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 7948 "bison-chpl-lib.cpp"
    break;

  case 300: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2160 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 7957 "bison-chpl-lib.cpp"
    break;

  case 301: /* manager_expr: expr TAS ident_def  */
#line 2165 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 7965 "bison-chpl-lib.cpp"
    break;

  case 302: /* manager_expr: expr  */
#line 2169 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 7973 "bison-chpl-lib.cpp"
    break;

  case 303: /* manager_expr_ls: manager_expr  */
#line 2175 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7979 "bison-chpl-lib.cpp"
    break;

  case 304: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2176 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7985 "bison-chpl-lib.cpp"
    break;

  case 305: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2181 "chpl.ypp"
  {
    auto locHeader = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), locHeader, (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 7994 "bison-chpl-lib.cpp"
    break;

  case 306: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2191 "chpl.ypp"
    {
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8005 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2198 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto headerLoc = context->makeSpannedLocation((yylsp[-4]), (yyvsp[-3].exprList) ? (yylsp[-3]) : (yylsp[-4]));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), headerLoc, (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      BUILDER->noteCurlyBracesLocation((yyval.commentsAndStmt).stmt, LOC2((yylsp[-2]), (yylsp[0])));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8018 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_start: class_tag ident_def  */
#line 2211 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8026 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_tag: TCLASS  */
#line 2217 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8032 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_tag: TRECORD  */
#line 2218 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8038 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_tag: TUNION  */
#line 2219 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8044 "bison-chpl-lib.cpp"
    break;

  case 312: /* opt_inherit: %empty  */
#line 2223 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8050 "bison-chpl-lib.cpp"
    break;

  case 313: /* opt_inherit: TCOLON expr_ls  */
#line 2224 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8056 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_level_stmt_ls: %empty  */
#line 2228 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8065 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2233 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8073 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2237 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8081 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2241 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8089 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2245 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8097 "bison-chpl-lib.cpp"
    break;

  case 319: /* enum_decl_stmt: enum_header TLCBR enum_ls TRCBR  */
#line 2253 "chpl.ypp"
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
#line 8121 "bison-chpl-lib.cpp"
    break;

  case 320: /* enum_decl_stmt: enum_header TLCBR error TRCBR  */
#line 2273 "chpl.ypp"
  {
    TypeDeclParts parts = (yyvsp[-3].typeDeclParts);
    auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    CommentsAndStmt cs = makeCommentsAndStmt(parts.comments, err.release());
    (yyval.commentsAndStmt) = cs;
    context->exitScope(asttags::Enum, parts.name);
    context->resetDeclState();
    context->clearComments();
  }
#line 8135 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_decl_stmt: enum_header TLCBR TRCBR  */
#line 2283 "chpl.ypp"
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
#line 8150 "bison-chpl-lib.cpp"
    break;

  case 322: /* enum_header: TENUM ident_def  */
#line 2297 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yylsp[0]), (yyvsp[0].uniqueStr), asttags::Enum);
  }
#line 8158 "bison-chpl-lib.cpp"
    break;

  case 323: /* enum_ls: enum_item  */
#line 2304 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8167 "bison-chpl-lib.cpp"
    break;

  case 324: /* enum_ls: enum_ls TCOMMA  */
#line 2309 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8177 "bison-chpl-lib.cpp"
    break;

  case 325: /* $@5: %empty  */
#line 2315 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8186 "bison-chpl-lib.cpp"
    break;

  case 326: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2320 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8195 "bison-chpl-lib.cpp"
    break;

  case 327: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2325 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8204 "bison-chpl-lib.cpp"
    break;

  case 328: /* $@6: %empty  */
#line 2330 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8213 "bison-chpl-lib.cpp"
    break;

  case 329: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2335 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8222 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_item: ident_def  */
#line 2345 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[0])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8234 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_item: ident_def TASSIGN expr  */
#line 2353 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      BUILDER->noteDeclNameLocation(decl.get(), LOC((yylsp[-2])));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8248 "bison-chpl-lib.cpp"
    break;

  case 333: /* linkage_spec: linkage_spec_empty  */
#line 2368 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8254 "bison-chpl-lib.cpp"
    break;

  case 334: /* linkage_spec: TINLINE  */
#line 2369 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8261 "bison-chpl-lib.cpp"
    break;

  case 335: /* linkage_spec: TOVERRIDE  */
#line 2371 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8268 "bison-chpl-lib.cpp"
    break;

  case 336: /* opt_fn_type_formal_ls: %empty  */
#line 2376 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8274 "bison-chpl-lib.cpp"
    break;

  case 337: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2377 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8280 "bison-chpl-lib.cpp"
    break;

  case 338: /* fn_type_formal_ls: fn_type_formal  */
#line 2381 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8286 "bison-chpl-lib.cpp"
    break;

  case 339: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2382 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8292 "bison-chpl-lib.cpp"
    break;

  case 340: /* fn_type_formal: named_formal  */
#line 2387 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8298 "bison-chpl-lib.cpp"
    break;

  case 341: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2390 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8304 "bison-chpl-lib.cpp"
    break;

  case 342: /* fn_type_formal: formal_type  */
#line 2392 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8310 "bison-chpl-lib.cpp"
    break;

  case 343: /* opt_fn_type_ret_type: %empty  */
#line 2396 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8316 "bison-chpl-lib.cpp"
    break;

  case 344: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2397 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8322 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2410 "chpl.ypp"
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
#line 8342 "bison-chpl-lib.cpp"
    break;

  case 346: /* $@7: %empty  */
#line 2429 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8352 "bison-chpl-lib.cpp"
    break;

  case 347: /* fn_expr: fn_type $@7 function_body_stmt  */
#line 2435 "chpl.ypp"
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
#line 8369 "bison-chpl-lib.cpp"
    break;

  case 348: /* $@8: %empty  */
#line 2448 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8379 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_expr: fn_type TALIAS $@8 expr  */
#line 2454 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8392 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2466 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8400 "bison-chpl-lib.cpp"
    break;

  case 351: /* $@9: %empty  */
#line 2475 "chpl.ypp"
  {
    context->enterScopeForFunctionDecl((yyvsp[-4].functionParts), (yyvsp[-2].expr));
  }
#line 8408 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@9 opt_function_body_stmt  */
#line 2479 "chpl.ypp"
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
#line 8435 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2505 "chpl.ypp"
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
#line 8452 "bison-chpl-lib.cpp"
    break;

  case 354: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2518 "chpl.ypp"
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
#line 8469 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2531 "chpl.ypp"
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
#line 8487 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2545 "chpl.ypp"
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
#line 8505 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2559 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      // store location of the header to handle weird bison location issues
      // with empty rules
      fp.headerLoc =
        context->makeSpannedLocation((yylsp[-3]), (yyvsp[0].exprList) != context->parenlessMarker ? (yylsp[0]) : (yylsp[-1]));
      (yyval.functionParts) = fp;
    }
#line 8519 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2572 "chpl.ypp"
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
#line 8535 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2588 "chpl.ypp"
  {
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 8544 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_ident: ident_def TBANG  */
#line 2598 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8554 "bison-chpl-lib.cpp"
    break;

  case 405: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2657 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8560 "bison-chpl-lib.cpp"
    break;

  case 406: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2658 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8566 "bison-chpl-lib.cpp"
    break;

  case 407: /* opt_formal_ls: %empty  */
#line 2662 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; CHPL_NULL_YYLLOC((yyloc)); }
#line 8572 "bison-chpl-lib.cpp"
    break;

  case 408: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2663 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8578 "bison-chpl-lib.cpp"
    break;

  case 409: /* formal_ls_inner: formal  */
#line 2667 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8584 "bison-chpl-lib.cpp"
    break;

  case 410: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2668 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8590 "bison-chpl-lib.cpp"
    break;

  case 411: /* formal_ls: %empty  */
#line 2672 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8596 "bison-chpl-lib.cpp"
    break;

  case 412: /* formal_ls: formal_ls_inner  */
#line 2673 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8602 "bison-chpl-lib.cpp"
    break;

  case 414: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2682 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8610 "bison-chpl-lib.cpp"
    break;

  case 415: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2687 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yylsp[-2]), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8618 "bison-chpl-lib.cpp"
    break;

  case 416: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2692 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8626 "bison-chpl-lib.cpp"
    break;

  case 417: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2697 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yylsp[-3]), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yylsp[-2]), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8634 "bison-chpl-lib.cpp"
    break;

  case 418: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2702 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yylsp[-5]), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8642 "bison-chpl-lib.cpp"
    break;

  case 419: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2707 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8650 "bison-chpl-lib.cpp"
    break;

  case 420: /* opt_formal_intent_tag: %empty  */
#line 2713 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);
  }
#line 8659 "bison-chpl-lib.cpp"
    break;

  case 421: /* opt_formal_intent_tag: required_intent_tag  */
#line 2718 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8668 "bison-chpl-lib.cpp"
    break;

  case 422: /* required_intent_tag: TIN  */
#line 2725 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::IN);            }
#line 8674 "bison-chpl-lib.cpp"
    break;

  case 423: /* required_intent_tag: TINOUT  */
#line 2726 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::INOUT);         }
#line 8680 "bison-chpl-lib.cpp"
    break;

  case 424: /* required_intent_tag: TOUT  */
#line 2727 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::OUT);           }
#line 8686 "bison-chpl-lib.cpp"
    break;

  case 425: /* required_intent_tag: TCONST TIN  */
#line 2728 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_IN);      }
#line 8692 "bison-chpl-lib.cpp"
    break;

  case 426: /* required_intent_tag: TCONST TREF  */
#line 2729 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);     }
#line 8698 "bison-chpl-lib.cpp"
    break;

  case 427: /* required_intent_tag: TCONST  */
#line 2730 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);         }
#line 8704 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TPARAM  */
#line 2731 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);         }
#line 8710 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TREF  */
#line 2732 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);           }
#line 8716 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TTYPE  */
#line 2733 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);          }
#line 8722 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TVAR  */
#line 2734 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR); }
#line 8728 "bison-chpl-lib.cpp"
    break;

  case 432: /* opt_this_intent_tag: %empty  */
#line 2738 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::DEFAULT_INTENT);     }
#line 8734 "bison-chpl-lib.cpp"
    break;

  case 433: /* opt_this_intent_tag: TPARAM  */
#line 2739 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::PARAM);              }
#line 8740 "bison-chpl-lib.cpp"
    break;

  case 434: /* opt_this_intent_tag: TREF  */
#line 2740 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::REF);                }
#line 8746 "bison-chpl-lib.cpp"
    break;

  case 435: /* opt_this_intent_tag: TCONST TREF  */
#line 2741 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST_REF);          }
#line 8752 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_this_intent_tag: TCONST  */
#line 2742 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::CONST);              }
#line 8758 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: TTYPE  */
#line 2743 "chpl.ypp"
              { (yyval.intentTag) = makeIntent(Formal::TYPE);               }
#line 8764 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TIN  */
#line 2744 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::IN);       }
#line 8770 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TCONST TIN  */
#line 2745 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::CONST_IN); }
#line 8776 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TVAR  */
#line 2746 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::VAR);      }
#line 8782 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TOUT  */
#line 2747 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::OUT);      }
#line 8788 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TINOUT  */
#line 2748 "chpl.ypp"
              { (yyval.intentTag) = makeInvalidIntent(Qualifier::INOUT);    }
#line 8794 "bison-chpl-lib.cpp"
    break;

  case 443: /* proc_iter_or_op: TPROC  */
#line 2752 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8800 "bison-chpl-lib.cpp"
    break;

  case 444: /* proc_iter_or_op: TITER  */
#line 2753 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8806 "bison-chpl-lib.cpp"
    break;

  case 445: /* proc_iter_or_op: TOPERATOR  */
#line 2754 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8812 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_ret_tag: %empty  */
#line 2758 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::DEFAULT_RETURN_INTENT); }
#line 8818 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_ret_tag: TOUT  */
#line 2759 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::OUT);                   }
#line 8824 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_ret_tag: TCONST  */
#line 2760 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST);                 }
#line 8830 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: TCONST TREF  */
#line 2761 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::CONST_REF);             }
#line 8836 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TREF  */
#line 2762 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::REF);                   }
#line 8842 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TPARAM  */
#line 2763 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::PARAM);                 }
#line 8848 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TTYPE  */
#line 2764 "chpl.ypp"
                  { (yyval.returnTag) = makeIntent(Function::TYPE);                  }
#line 8854 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_ret_tag: TIN  */
#line 2765 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::IN);            }
#line 8860 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_ret_tag: TCONST TIN  */
#line 2766 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::CONST_IN);      }
#line 8866 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: TVAR  */
#line 2767 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::VAR);           }
#line 8872 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: TINOUT  */
#line 2768 "chpl.ypp"
                  { (yyval.returnTag) = makeInvalidIntent(Qualifier::INOUT);         }
#line 8878 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_throws_error: %empty  */
#line 2772 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8884 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_throws_error: TTHROWS  */
#line 2773 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8890 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_function_body_stmt: TSEMI  */
#line 2776 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8896 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_function_body_stmt: function_body_stmt  */
#line 2777 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8902 "bison-chpl-lib.cpp"
    break;

  case 461: /* function_body_stmt: block_stmt_body  */
#line 2781 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8908 "bison-chpl-lib.cpp"
    break;

  case 462: /* function_body_stmt: TDO toplevel_stmt  */
#line 2782 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yylsp[0]), (yyvsp[0].commentsAndStmt)); }
#line 8914 "bison-chpl-lib.cpp"
    break;

  case 463: /* query_expr: TQUERIEDIDENT  */
#line 2786 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 8920 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_lifetime_where: %empty  */
#line 2791 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8926 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_lifetime_where: TWHERE expr  */
#line 2793 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8932 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2795 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8938 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2797 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 8944 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2799 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 8950 "bison-chpl-lib.cpp"
    break;

  case 469: /* lifetime_components_expr: lifetime_expr  */
#line 2804 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8956 "bison-chpl-lib.cpp"
    break;

  case 470: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2806 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8962 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2811 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8968 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2813 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8974 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2815 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8980 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2817 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8986 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2819 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8992 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2821 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8998 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2823 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9004 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_ident: TIDENT  */
#line 2827 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9010 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_ident: TINIT  */
#line 2828 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9016 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_ident: TTHIS  */
#line 2829 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9022 "bison-chpl-lib.cpp"
    break;

  case 481: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2834 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9031 "bison-chpl-lib.cpp"
    break;

  case 482: /* type_alias_decl_stmt_start: TTYPE  */
#line 2843 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9041 "bison-chpl-lib.cpp"
    break;

  case 483: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2849 "chpl.ypp"
  {
    context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true, (yylsp[-1]));
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9052 "bison-chpl-lib.cpp"
    break;

  case 484: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2859 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9060 "bison-chpl-lib.cpp"
    break;

  case 485: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2863 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9068 "bison-chpl-lib.cpp"
    break;

  case 486: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2870 "chpl.ypp"
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
#line 9090 "bison-chpl-lib.cpp"
    break;

  case 487: /* opt_init_type: %empty  */
#line 2890 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9096 "bison-chpl-lib.cpp"
    break;

  case 488: /* opt_init_type: TASSIGN expr  */
#line 2892 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9102 "bison-chpl-lib.cpp"
    break;

  case 489: /* var_decl_type: TPARAM  */
#line 2896 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9108 "bison-chpl-lib.cpp"
    break;

  case 490: /* var_decl_type: TCONST TREF  */
#line 2897 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9114 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_type: TREF  */
#line 2898 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9120 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_type: TCONST  */
#line 2899 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9126 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_type: TVAR  */
#line 2900 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9132 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_start: var_decl_type  */
#line 2905 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 9141 "bison-chpl-lib.cpp"
    break;

  case 495: /* $@10: %empty  */
#line 2913 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true, (yylsp[0]));
  }
#line 9150 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_stmt: TCONFIG $@10 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2917 "chpl.ypp"
                                              {
    CHPL_ASSERT(context->isVarDeclConfig);
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9160 "bison-chpl-lib.cpp"
    break;

  case 497: /* $@11: %empty  */
#line 2923 "chpl.ypp"
  {
    // Use a mid-rule action to preserve the 'on' expr
    context->noteVarDestinationExpr((yyvsp[0].expr));
  }
#line 9169 "bison-chpl-lib.cpp"
    break;

  case 498: /* var_decl_stmt: TON expr $@11 var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2928 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-5]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9178 "bison-chpl-lib.cpp"
    break;

  case 499: /* var_decl_stmt: var_decl_start var_decl_stmt_inner_ls TSEMI  */
#line 2933 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt(YLOC2((yylsp[-2]), (yylsp[-1])), (yyvsp[-2].attributeGroup), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9187 "bison-chpl-lib.cpp"
    break;

  case 500: /* var_decl_stmt: var_decl_start error TSEMI  */
#line 2938 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = makeCommentsAndStmt(nullptr, ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release());
    delete (yyvsp[-2].attributeGroup);
    context->resetDeclStateOnError();
  }
#line 9197 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2947 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9205 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2951 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9213 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2958 "chpl.ypp"
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
#line 9235 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2976 "chpl.ypp"
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

  case 505: /* tuple_var_decl_component: ident_def  */
#line 2995 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9263 "bison-chpl-lib.cpp"
    break;

  case 506: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2999 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9271 "bison-chpl-lib.cpp"
    break;

  case 507: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 3006 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9277 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 3008 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9283 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 3010 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9289 "bison-chpl-lib.cpp"
    break;

  case 510: /* opt_init_expr: %empty  */
#line 3016 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9295 "bison-chpl-lib.cpp"
    break;

  case 511: /* opt_init_expr: TASSIGN TNOINIT  */
#line 3017 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9301 "bison-chpl-lib.cpp"
    break;

  case 512: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 3018 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9307 "bison-chpl-lib.cpp"
    break;

  case 513: /* formal_or_ret_type_expr: expr  */
#line 3022 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9313 "bison-chpl-lib.cpp"
    break;

  case 514: /* ret_type: formal_or_ret_type_expr  */
#line 3026 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9319 "bison-chpl-lib.cpp"
    break;

  case 515: /* ret_type: reserved_type_ident_use  */
#line 3027 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9325 "bison-chpl-lib.cpp"
    break;

  case 516: /* ret_type: error  */
#line 3028 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9331 "bison-chpl-lib.cpp"
    break;

  case 517: /* colon_ret_type: TCOLON ret_type  */
#line 3032 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9337 "bison-chpl-lib.cpp"
    break;

  case 518: /* colon_ret_type: error  */
#line 3033 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9343 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_ret_type: %empty  */
#line 3037 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9349 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_type: %empty  */
#line 3042 "chpl.ypp"
                                 { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9355 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_type: TCOLON expr  */
#line 3043 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9361 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_type: TCOLON reserved_type_ident_use  */
#line 3044 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9367 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_type: error  */
#line 3045 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9373 "bison-chpl-lib.cpp"
    break;

  case 525: /* formal_type: formal_or_ret_type_expr  */
#line 3049 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9379 "bison-chpl-lib.cpp"
    break;

  case 526: /* formal_type: reserved_type_ident_use  */
#line 3050 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9385 "bison-chpl-lib.cpp"
    break;

  case 527: /* colon_formal_type: TCOLON formal_type  */
#line 3054 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9391 "bison-chpl-lib.cpp"
    break;

  case 528: /* opt_colon_formal_type: %empty  */
#line 3058 "chpl.ypp"
                        { (yyval.expr) = nullptr; CHPL_NULL_YYLLOC((yyloc)); }
#line 9397 "bison-chpl-lib.cpp"
    break;

  case 529: /* opt_colon_formal_type: colon_formal_type  */
#line 3059 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9403 "bison-chpl-lib.cpp"
    break;

  case 530: /* expr_ls: expr  */
#line 3065 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9409 "bison-chpl-lib.cpp"
    break;

  case 531: /* expr_ls: expr_ls TCOMMA expr  */
#line 3066 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9415 "bison-chpl-lib.cpp"
    break;

  case 532: /* expr_ls_end_semi: expr_ls semicolon_list  */
#line 3070 "chpl.ypp"
                         {
    (yyval.ndArrayList) = context->makeNDArrayList();
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElement((yyvsp[-1].exprList), (yylsp[-1])));
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9425 "bison-chpl-lib.cpp"
    break;

  case 533: /* expr_ls_end_semi: expr_ls TCOMMA semicolon_list  */
#line 3075 "chpl.ypp"
                                {
    (yyval.ndArrayList) = context->makeNDArrayList();
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElement((yyvsp[-2].exprList), (yylsp[-2])));
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9435 "bison-chpl-lib.cpp"
    break;

  case 534: /* expr_ls_end_semi: expr_ls_semi semicolon_list  */
#line 3081 "chpl.ypp"
  {
    (yyval.ndArrayList) = (yyvsp[-1].ndArrayList);
    (yyval.ndArrayList) = context->appendNDArrayList((yyval.ndArrayList), makeNDArrayElementSep((yyvsp[0].counter), (yylsp[0])));
  }
#line 9444 "bison-chpl-lib.cpp"
    break;

  case 535: /* expr_ls_semi: expr_ls_end_semi expr_ls  */
#line 3088 "chpl.ypp"
                           {
    (yyval.ndArrayList) = context->appendNDArrayList((yyvsp[-1].ndArrayList), makeNDArrayElement((yyvsp[0].exprList), (yylsp[0])));
  }
#line 9452 "bison-chpl-lib.cpp"
    break;

  case 536: /* expr_ls_semi: expr_ls_end_semi expr_ls TCOMMA  */
#line 3091 "chpl.ypp"
                                  {
    (yyval.ndArrayList) = context->appendNDArrayList((yyvsp[-2].ndArrayList), makeNDArrayElement((yyvsp[-1].exprList), (yylsp[-1])));
  }
#line 9460 "bison-chpl-lib.cpp"
    break;

  case 537: /* semicolon_list: TSEMI  */
#line 3097 "chpl.ypp"
                       { (yyval.counter) = 1; }
#line 9466 "bison-chpl-lib.cpp"
    break;

  case 538: /* semicolon_list: semicolon_list TSEMI  */
#line 3098 "chpl.ypp"
                       { (yyval.counter) = (yyvsp[-1].counter) + 1; }
#line 9472 "bison-chpl-lib.cpp"
    break;

  case 539: /* tuple_component: opt_try_expr  */
#line 3104 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9478 "bison-chpl-lib.cpp"
    break;

  case 540: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3109 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9486 "bison-chpl-lib.cpp"
    break;

  case 541: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3113 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9494 "bison-chpl-lib.cpp"
    break;

  case 542: /* opt_actual_ls: %empty  */
#line 3119 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9500 "bison-chpl-lib.cpp"
    break;

  case 543: /* opt_actual_ls: actual_ls  */
#line 3120 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9506 "bison-chpl-lib.cpp"
    break;

  case 544: /* actual_ls: actual_expr  */
#line 3125 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9515 "bison-chpl-lib.cpp"
    break;

  case 545: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3130 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9525 "bison-chpl-lib.cpp"
    break;

  case 546: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3138 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9531 "bison-chpl-lib.cpp"
    break;

  case 547: /* actual_expr: opt_try_expr  */
#line 3139 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9537 "bison-chpl-lib.cpp"
    break;

  case 548: /* ident_expr: ident_use  */
#line 3143 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9543 "bison-chpl-lib.cpp"
    break;

  case 549: /* ident_expr: scalar_type  */
#line 3144 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9549 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3157 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9555 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3159 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9561 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3161 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9567 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3163 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9577 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TATOMIC expr  */
#line 3169 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9583 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TSYNC expr  */
#line 3171 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9589 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TOWNED  */
#line 3174 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9595 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TOWNED expr  */
#line 3176 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9601 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TUNMANAGED  */
#line 3178 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9607 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3180 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9613 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TSHARED  */
#line 3182 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9619 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TSHARED expr  */
#line 3184 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9625 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TBORROWED  */
#line 3186 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9631 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TBORROWED expr  */
#line 3188 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9637 "bison-chpl-lib.cpp"
    break;

  case 569: /* sub_type_level_expr: TCLASS  */
#line 3190 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9643 "bison-chpl-lib.cpp"
    break;

  case 570: /* sub_type_level_expr: TRECORD  */
#line 3192 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9649 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr: no_loop_attributes for_expr_base  */
#line 3196 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 9655 "bison-chpl-lib.cpp"
    break;

  case 572: /* for_expr_base: for_loop_kw expr TIN expr TDO expr  */
#line 3201 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9663 "bison-chpl-lib.cpp"
    break;

  case 573: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO expr  */
#line 3205 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-4]), (yylsp[0]), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9671 "bison-chpl-lib.cpp"
    break;

  case 574: /* for_expr_base: for_loop_kw expr TDO expr  */
#line 3209 "chpl.ypp"
  {
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-2]), (yylsp[0]), (yyvsp[-3].uniqueStr), nullptr, (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9679 "bison-chpl-lib.cpp"
    break;

  case 575: /* for_expr_base: for_loop_kw zippered_iterator TDO expr  */
#line 3213 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "expression-level loops with 'zip' must have an index.");
  }
#line 9687 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr_base: for_loop_kw expr TIN expr TDO TIF expr TTHEN expr  */
#line 3217 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9700 "bison-chpl-lib.cpp"
    break;

  case 577: /* for_expr_base: for_loop_kw expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3226 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-7]), ifLoc, (yyvsp[-8].uniqueStr), (yyvsp[-7].expr), (yyvsp[-5].expr), ifExpr.release());
  }
#line 9713 "bison-chpl-lib.cpp"
    break;

  case 578: /* for_expr_base: for_loop_kw expr TDO TIF expr TTHEN expr  */
#line 3235 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = context->buildGeneralLoopExpr((yyloc), (yylsp[-5]), ifLoc, (yyvsp[-6].uniqueStr), nullptr, (yyvsp[-5].expr), ifExpr.release());
  }
#line 9726 "bison-chpl-lib.cpp"
    break;

  case 579: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3246 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 9732 "bison-chpl-lib.cpp"
    break;

  case 580: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3251 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9740 "bison-chpl-lib.cpp"
    break;

  case 581: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3255 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9748 "bison-chpl-lib.cpp"
    break;

  case 582: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3259 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9756 "bison-chpl-lib.cpp"
    break;

  case 583: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3263 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9764 "bison-chpl-lib.cpp"
    break;

  case 584: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3267 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9772 "bison-chpl-lib.cpp"
    break;

  case 585: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3271 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9780 "bison-chpl-lib.cpp"
    break;

  case 586: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3275 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9788 "bison-chpl-lib.cpp"
    break;

  case 587: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3282 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9802 "bison-chpl-lib.cpp"
    break;

  case 588: /* nil_expr: TNIL  */
#line 3299 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9808 "bison-chpl-lib.cpp"
    break;

  case 595: /* opt_task_intent_ls: %empty  */
#line 3316 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9814 "bison-chpl-lib.cpp"
    break;

  case 596: /* opt_task_intent_ls: task_intent_clause  */
#line 3317 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9820 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3322 "chpl.ypp"
  {
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 9828 "bison-chpl-lib.cpp"
    break;

  case 598: /* task_intent_clause: TWITH TLP TRP  */
#line 3326 "chpl.ypp"
  {
    auto err = context->syntax((yyloc), "'with' clause cannot be empty.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-2]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9839 "bison-chpl-lib.cpp"
    break;

  case 599: /* task_intent_clause: TWITH TLP error TRP  */
#line 3333 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[-2]), "invalid intent expression in 'with' clause.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-3]), dummyLoc, dummyLoc, dummyLoc, exprs);
}
#line 9850 "bison-chpl-lib.cpp"
    break;

  case 600: /* task_intent_clause: TWITH error  */
#line 3340 "chpl.ypp"
  {
    auto err = context->syntax((yylsp[0]), "missing parentheses around 'with' clause intents.");
    auto exprs = context->makeList(err);
    auto dummyLoc = TextLocation::create();
    (yyval.withClause) = context->buildWithClause((yyloc), (yylsp[-1]), dummyLoc, dummyLoc, dummyLoc, exprs);
  }
#line 9861 "bison-chpl-lib.cpp"
    break;

  case 601: /* task_intent_ls: intent_expr  */
#line 3349 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9867 "bison-chpl-lib.cpp"
    break;

  case 602: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3350 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9873 "bison-chpl-lib.cpp"
    break;

  case 603: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3355 "chpl.ypp"
  {
    (yyval.expr) = context->buildTaskIntent((yyloc), (yylsp[-2]), nullptr, (yyvsp[-2].expr), (yyvsp[-3].taskIntent), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9881 "bison-chpl-lib.cpp"
    break;

  case 604: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3359 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9889 "bison-chpl-lib.cpp"
    break;

  case 605: /* intent_expr: expr TREDUCE ident_expr  */
#line 3363 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr), (yylsp[0]));
  }
#line 9897 "bison-chpl-lib.cpp"
    break;

  case 606: /* task_var_prefix: TCONST  */
#line 3369 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST);          }
#line 9903 "bison-chpl-lib.cpp"
    break;

  case 607: /* task_var_prefix: TIN  */
#line 3370 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::IN);             }
#line 9909 "bison-chpl-lib.cpp"
    break;

  case 608: /* task_var_prefix: TCONST TIN  */
#line 3371 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_IN);       }
#line 9915 "bison-chpl-lib.cpp"
    break;

  case 609: /* task_var_prefix: TREF  */
#line 3372 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::REF);            }
#line 9921 "bison-chpl-lib.cpp"
    break;

  case 610: /* task_var_prefix: TCONST TREF  */
#line 3373 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::CONST_REF);      }
#line 9927 "bison-chpl-lib.cpp"
    break;

  case 611: /* task_var_prefix: TVAR  */
#line 3374 "chpl.ypp"
               { (yyval.taskIntent) = makeIntent(TaskVar::VAR);            }
#line 9933 "bison-chpl-lib.cpp"
    break;

  case 612: /* task_var_prefix: TOUT  */
#line 3375 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::OUT);   }
#line 9939 "bison-chpl-lib.cpp"
    break;

  case 613: /* task_var_prefix: TINOUT  */
#line 3376 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::INOUT); }
#line 9945 "bison-chpl-lib.cpp"
    break;

  case 614: /* task_var_prefix: TPARAM  */
#line 3377 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::PARAM); }
#line 9951 "bison-chpl-lib.cpp"
    break;

  case 615: /* task_var_prefix: TTYPE  */
#line 3378 "chpl.ypp"
               { (yyval.taskIntent) = makeInvalidIntent(Qualifier::TYPE);  }
#line 9957 "bison-chpl-lib.cpp"
    break;

  case 616: /* new_maybe_decorated: TNEW  */
#line 3383 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9963 "bison-chpl-lib.cpp"
    break;

  case 617: /* new_maybe_decorated: TNEW TOWNED  */
#line 3385 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9969 "bison-chpl-lib.cpp"
    break;

  case 618: /* new_maybe_decorated: TNEW TSHARED  */
#line 3387 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9975 "bison-chpl-lib.cpp"
    break;

  case 619: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3389 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9981 "bison-chpl-lib.cpp"
    break;

  case 620: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3391 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9987 "bison-chpl-lib.cpp"
    break;

  case 621: /* new_expr: new_maybe_decorated expr  */
#line 3397 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 9995 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3404 "chpl.ypp"
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
#line 10012 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3417 "chpl.ypp"
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
#line 10029 "bison-chpl-lib.cpp"
    break;

  case 624: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3430 "chpl.ypp"
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
#line 10047 "bison-chpl-lib.cpp"
    break;

  case 625: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3444 "chpl.ypp"
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
#line 10066 "bison-chpl-lib.cpp"
    break;

  case 626: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3462 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10074 "bison-chpl-lib.cpp"
    break;

  case 627: /* range_literal_expr: expr TDOTDOT expr  */
#line 3469 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10083 "bison-chpl-lib.cpp"
    break;

  case 628: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3474 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10092 "bison-chpl-lib.cpp"
    break;

  case 629: /* range_literal_expr: expr TDOTDOT  */
#line 3479 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10101 "bison-chpl-lib.cpp"
    break;

  case 630: /* range_literal_expr: TDOTDOT expr  */
#line 3484 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10111 "bison-chpl-lib.cpp"
    break;

  case 631: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3490 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10121 "bison-chpl-lib.cpp"
    break;

  case 632: /* range_literal_expr: TDOTDOT  */
#line 3496 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10131 "bison-chpl-lib.cpp"
    break;

  case 633: /* cast_expr: expr TCOLON expr  */
#line 3526 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10139 "bison-chpl-lib.cpp"
    break;

  case 634: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3533 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10147 "bison-chpl-lib.cpp"
    break;

  case 635: /* super_expr: fn_expr  */
#line 3539 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10153 "bison-chpl-lib.cpp"
    break;

  case 639: /* expr: sub_type_level_expr TQUESTION  */
#line 3548 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10159 "bison-chpl-lib.cpp"
    break;

  case 640: /* expr: TQUESTION  */
#line 3550 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10165 "bison-chpl-lib.cpp"
    break;

  case 644: /* expr: fn_type  */
#line 3555 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10171 "bison-chpl-lib.cpp"
    break;

  case 653: /* opt_expr: %empty  */
#line 3568 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10177 "bison-chpl-lib.cpp"
    break;

  case 654: /* opt_expr: expr  */
#line 3569 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10183 "bison-chpl-lib.cpp"
    break;

  case 655: /* opt_try_expr: TTRY expr  */
#line 3573 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10189 "bison-chpl-lib.cpp"
    break;

  case 656: /* opt_try_expr: TTRYBANG expr  */
#line 3574 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10195 "bison-chpl-lib.cpp"
    break;

  case 657: /* opt_try_expr: super_expr  */
#line 3575 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10201 "bison-chpl-lib.cpp"
    break;

  case 663: /* call_base_expr: expr TBANG  */
#line 3592 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10209 "bison-chpl-lib.cpp"
    break;

  case 664: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3595 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10215 "bison-chpl-lib.cpp"
    break;

  case 666: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3601 "chpl.ypp"
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
#line 10231 "bison-chpl-lib.cpp"
    break;

  case 667: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3613 "chpl.ypp"
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
#line 10247 "bison-chpl-lib.cpp"
    break;

  case 668: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3625 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10255 "bison-chpl-lib.cpp"
    break;

  case 669: /* dot_expr: expr TDOT ident_use  */
#line 3632 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10261 "bison-chpl-lib.cpp"
    break;

  case 670: /* dot_expr: expr TDOT TTYPE  */
#line 3634 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10267 "bison-chpl-lib.cpp"
    break;

  case 671: /* dot_expr: expr TDOT TDOMAIN  */
#line 3636 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10273 "bison-chpl-lib.cpp"
    break;

  case 672: /* dot_expr: expr TDOT TLOCALE  */
#line 3638 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-2]), (yylsp[-1]), (yylsp[0]), (yyvsp[-2].expr), (yyvsp[0].uniqueStr)); }
#line 10279 "bison-chpl-lib.cpp"
    break;

  case 673: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3640 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10285 "bison-chpl-lib.cpp"
    break;

  case 674: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3642 "chpl.ypp"
    { (yyval.expr) = context->buildDot((yyloc), (yylsp[-4]), (yylsp[-3]), (yylsp[-2]), (yyvsp[-4].expr), (yyvsp[-2].uniqueStr), true); }
#line 10291 "bison-chpl-lib.cpp"
    break;

  case 675: /* parenthesized_expr: TLP opt_try_expr TRP  */
#line 3651 "chpl.ypp"
  {
    // Use 'tryNote' here in case an expression like ((x)) comes along.
    BUILDER->tryNoteExprParenLocation((yyvsp[-1].expr), LOC((yyloc)));
    (yyval.expr) = (yyvsp[-1].expr);
  }
#line 10301 "bison-chpl-lib.cpp"
    break;

  case 676: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3657 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10309 "bison-chpl-lib.cpp"
    break;

  case 677: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3661 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10317 "bison-chpl-lib.cpp"
    break;

  case 678: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3665 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10325 "bison-chpl-lib.cpp"
    break;

  case 679: /* bool_literal: TFALSE  */
#line 3671 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10331 "bison-chpl-lib.cpp"
    break;

  case 680: /* bool_literal: TTRUE  */
#line 3672 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10337 "bison-chpl-lib.cpp"
    break;

  case 681: /* str_bytes_literal: STRINGLITERAL  */
#line 3676 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10343 "bison-chpl-lib.cpp"
    break;

  case 682: /* str_bytes_literal: BYTESLITERAL  */
#line 3677 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10349 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: INTLITERAL  */
#line 3683 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10355 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: REALLITERAL  */
#line 3684 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10361 "bison-chpl-lib.cpp"
    break;

  case 687: /* literal_expr: IMAGLITERAL  */
#line 3685 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10367 "bison-chpl-lib.cpp"
    break;

  case 688: /* literal_expr: TNONE  */
#line 3686 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10373 "bison-chpl-lib.cpp"
    break;

  case 689: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3688 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10382 "bison-chpl-lib.cpp"
    break;

  case 690: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3693 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10391 "bison-chpl-lib.cpp"
    break;

  case 691: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3698 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 10400 "bison-chpl-lib.cpp"
    break;

  case 692: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3703 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 10409 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: no_loop_attributes TLSBR expr_ls_semi TRSBR  */
#line 3708 "chpl.ypp"
  {
    (yyval.expr) = context->buildNDArray((yyloc), (yyvsp[-1].ndArrayList));
    context->popLoopAttributeGroup();
  }
#line 10418 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: no_loop_attributes TLSBR expr_ls_semi semicolon_list TRSBR  */
#line 3713 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    (yyval.expr) = context->buildNDArray((yyloc), (yyvsp[-2].ndArrayList));
    context->popLoopAttributeGroup();
  }
#line 10430 "bison-chpl-lib.cpp"
    break;

  case 695: /* literal_expr: no_loop_attributes TLSBR expr_ls semicolon_list TRSBR  */
#line 3721 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    AstList lst;
    lst.push_back(ArrayRow::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))));
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), std::move(lst)).release();
    context->popLoopAttributeGroup();
  }
#line 10444 "bison-chpl-lib.cpp"
    break;

  case 696: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA semicolon_list TRSBR  */
#line 3731 "chpl.ypp"
  {
    if ((yyvsp[-1].counter) > 1) {
      context->syntax((yylsp[-1]), "multiple trailing semicolons in array literal");
    }
    AstList lst;
    lst.push_back(ArrayRow::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-3].exprList))));
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), std::move(lst)).release();
    context->popLoopAttributeGroup();
  }
#line 10458 "bison-chpl-lib.cpp"
    break;

  case 697: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3741 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 10467 "bison-chpl-lib.cpp"
    break;

  case 698: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3746 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 10476 "bison-chpl-lib.cpp"
    break;

  case 699: /* assoc_expr_ls: expr TALIAS expr  */
#line 3755 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10485 "bison-chpl-lib.cpp"
    break;

  case 700: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3760 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10495 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TPLUS expr  */
#line 3768 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10501 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TMINUS expr  */
#line 3769 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10507 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TSTAR expr  */
#line 3770 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10513 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TDIVIDE expr  */
#line 3771 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10519 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3772 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10525 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3773 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10531 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TMOD expr  */
#line 3774 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10537 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TEQUAL expr  */
#line 3775 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10543 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3776 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10549 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3777 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10555 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3778 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10561 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TLESS expr  */
#line 3779 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10567 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TGREATER expr  */
#line 3780 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10573 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TBAND expr  */
#line 3781 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10579 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TBOR expr  */
#line 3782 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10585 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TBXOR expr  */
#line 3783 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10591 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TAND expr  */
#line 3784 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10597 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TOR expr  */
#line 3785 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10603 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TEXP expr  */
#line 3786 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10609 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TBY expr  */
#line 3787 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10615 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr TALIGN expr  */
#line 3788 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10621 "bison-chpl-lib.cpp"
    break;

  case 722: /* binary_op_expr: expr THASH expr  */
#line 3789 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10627 "bison-chpl-lib.cpp"
    break;

  case 723: /* binary_op_expr: expr TDMAPPED expr  */
#line 3790 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10633 "bison-chpl-lib.cpp"
    break;

  case 724: /* unary_op_expr: TPLUS expr  */
#line 3794 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10639 "bison-chpl-lib.cpp"
    break;

  case 725: /* unary_op_expr: TMINUS expr  */
#line 3795 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10645 "bison-chpl-lib.cpp"
    break;

  case 726: /* unary_op_expr: TMINUSMINUS expr  */
#line 3796 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10651 "bison-chpl-lib.cpp"
    break;

  case 727: /* unary_op_expr: TPLUSPLUS expr  */
#line 3797 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10657 "bison-chpl-lib.cpp"
    break;

  case 728: /* unary_op_expr: TBANG expr  */
#line 3798 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10663 "bison-chpl-lib.cpp"
    break;

  case 729: /* unary_op_expr: expr TBANG  */
#line 3799 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10671 "bison-chpl-lib.cpp"
    break;

  case 730: /* unary_op_expr: TBNOT expr  */
#line 3802 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10677 "bison-chpl-lib.cpp"
    break;

  case 731: /* reduce_expr: expr TREDUCE expr  */
#line 3807 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10685 "bison-chpl-lib.cpp"
    break;

  case 732: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3811 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10693 "bison-chpl-lib.cpp"
    break;

  case 733: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3815 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10701 "bison-chpl-lib.cpp"
    break;

  case 734: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3819 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10709 "bison-chpl-lib.cpp"
    break;

  case 735: /* scan_expr: expr TSCAN expr  */
#line 3826 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10717 "bison-chpl-lib.cpp"
    break;

  case 736: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3830 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10725 "bison-chpl-lib.cpp"
    break;

  case 737: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3834 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10733 "bison-chpl-lib.cpp"
    break;

  case 738: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3838 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10741 "bison-chpl-lib.cpp"
    break;


#line 10745 "bison-chpl-lib.cpp"

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
