/* A Bison parser, made by GNU Bison 3.7.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.7.2"

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
  YYSYMBOL_TDEPRECATED = 31,               /* TDEPRECATED  */
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
  YYSYMBOL_TINITEQUALS = 55,               /* TINITEQUALS  */
  YYSYMBOL_TIMPLEMENTS = 56,               /* TIMPLEMENTS  */
  YYSYMBOL_TINTERFACE = 57,                /* TINTERFACE  */
  YYSYMBOL_TLABEL = 58,                    /* TLABEL  */
  YYSYMBOL_TLAMBDA = 59,                   /* TLAMBDA  */
  YYSYMBOL_TLET = 60,                      /* TLET  */
  YYSYMBOL_TLIFETIME = 61,                 /* TLIFETIME  */
  YYSYMBOL_TLOCAL = 62,                    /* TLOCAL  */
  YYSYMBOL_TLOCALE = 63,                   /* TLOCALE  */
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
  YYSYMBOL_TREAL = 86,                     /* TREAL  */
  YYSYMBOL_TRECORD = 87,                   /* TRECORD  */
  YYSYMBOL_TREDUCE = 88,                   /* TREDUCE  */
  YYSYMBOL_TREF = 89,                      /* TREF  */
  YYSYMBOL_TREQUIRE = 90,                  /* TREQUIRE  */
  YYSYMBOL_TRETURN = 91,                   /* TRETURN  */
  YYSYMBOL_TSCAN = 92,                     /* TSCAN  */
  YYSYMBOL_TSELECT = 93,                   /* TSELECT  */
  YYSYMBOL_TSERIAL = 94,                   /* TSERIAL  */
  YYSYMBOL_TSHARED = 95,                   /* TSHARED  */
  YYSYMBOL_TSINGLE = 96,                   /* TSINGLE  */
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
  YYSYMBOL_TUNDERSCORE = 110,              /* TUNDERSCORE  */
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
  YYSYMBOL_TIO = 155,                      /* TIO  */
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
  YYSYMBOL_deprecated_decl_stmt = 186,     /* deprecated_decl_stmt  */
  YYSYMBOL_187_1 = 187,                    /* $@1  */
  YYSYMBOL_188_2 = 188,                    /* $@2  */
  YYSYMBOL_deprecated_decl_base = 189,     /* deprecated_decl_base  */
  YYSYMBOL_module_decl_start = 190,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 191,         /* module_decl_stmt  */
  YYSYMBOL_access_control = 192,           /* access_control  */
  YYSYMBOL_opt_prototype = 193,            /* opt_prototype  */
  YYSYMBOL_include_module_stmt = 194,      /* include_module_stmt  */
  YYSYMBOL_block_stmt_body = 195,          /* block_stmt_body  */
  YYSYMBOL_block_stmt = 196,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 197,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 198,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 199,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 200,              /* opt_only_ls  */
  YYSYMBOL_use_stmt = 201,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 202,              /* import_stmt  */
  YYSYMBOL_import_expr = 203,              /* import_expr  */
  YYSYMBOL_import_ls = 204,                /* import_ls  */
  YYSYMBOL_require_stmt = 205,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 206,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 207,          /* opt_label_ident  */
  YYSYMBOL_ident_fn_def = 208,             /* ident_fn_def  */
  YYSYMBOL_ident_def = 209,                /* ident_def  */
  YYSYMBOL_ident_use = 210,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 211,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 212,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 213,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 214,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 215,              /* return_stmt  */
  YYSYMBOL_deprecated_class_level_stmt = 216, /* deprecated_class_level_stmt  */
  YYSYMBOL_217_3 = 217,                    /* $@3  */
  YYSYMBOL_218_4 = 218,                    /* $@4  */
  YYSYMBOL_class_level_stmt = 219,         /* class_level_stmt  */
  YYSYMBOL_220_5 = 220,                    /* $@5  */
  YYSYMBOL_221_6 = 221,                    /* $@6  */
  YYSYMBOL_inner_class_level_stmt = 222,   /* inner_class_level_stmt  */
  YYSYMBOL_forwarding_decl_stmt = 223,     /* forwarding_decl_stmt  */
  YYSYMBOL_forwarding_decl_start = 224,    /* forwarding_decl_start  */
  YYSYMBOL_extern_or_export = 225,         /* extern_or_export  */
  YYSYMBOL_extern_export_decl_stmt_start = 226, /* extern_export_decl_stmt_start  */
  YYSYMBOL_extern_export_decl_stmt = 227,  /* extern_export_decl_stmt  */
  YYSYMBOL_228_7 = 228,                    /* $@7  */
  YYSYMBOL_extern_block_stmt = 229,        /* extern_block_stmt  */
  YYSYMBOL_loop_stmt = 230,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 231,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 232,                  /* if_stmt  */
  YYSYMBOL_ifvar = 233,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 234,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 235,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 236,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 237,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 238, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 239,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 240,           /* ifc_constraint  */
  YYSYMBOL_try_stmt = 241,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 242,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 243,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 244,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 245,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 246,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 247,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 248,                /* when_stmt  */
  YYSYMBOL_class_decl_stmt = 249,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 250,              /* class_start  */
  YYSYMBOL_class_tag = 251,                /* class_tag  */
  YYSYMBOL_opt_inherit = 252,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 253,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 254,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header_lcbr = 255,         /* enum_header_lcbr  */
  YYSYMBOL_enum_ls = 256,                  /* enum_ls  */
  YYSYMBOL_257_8 = 257,                    /* $@8  */
  YYSYMBOL_deprecated_enum_item = 258,     /* deprecated_enum_item  */
  YYSYMBOL_259_9 = 259,                    /* $@9  */
  YYSYMBOL_260_10 = 260,                   /* $@10  */
  YYSYMBOL_enum_item = 261,                /* enum_item  */
  YYSYMBOL_lambda_decl_expr = 262,         /* lambda_decl_expr  */
  YYSYMBOL_linkage_spec_empty = 263,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 264,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt_complete = 265,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 266,             /* fn_decl_stmt  */
  YYSYMBOL_267_11 = 267,                   /* $@11  */
  YYSYMBOL_fn_decl_stmt_inner = 268,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 269,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 270,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 271,                 /* fn_ident  */
  YYSYMBOL_op_ident = 272,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 273,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 274,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 275,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 276,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 277,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 278,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 279,                /* formal_ls  */
  YYSYMBOL_formal = 280,                   /* formal  */
  YYSYMBOL_opt_formal_intent_tag = 281,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 282,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 283,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 284,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 285,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 286,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 287,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 288,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 289,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 290,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 291, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 292,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 293,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 294,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 295, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 296, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 297, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 298,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 299,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 300,            /* var_decl_stmt  */
  YYSYMBOL_301_12 = 301,                   /* $@12  */
  YYSYMBOL_var_decl_stmt_inner_ls = 302,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 303,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 304, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 305, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 306,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 307,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 308,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 309,                 /* opt_type  */
  YYSYMBOL_array_type = 310,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 311, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 312,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 313,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 314,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 315,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 316,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 317,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 318,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 319,                /* actual_ls  */
  YYSYMBOL_actual_expr = 320,              /* actual_expr  */
  YYSYMBOL_ident_expr = 321,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 322,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 323,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 324,                 /* for_expr  */
  YYSYMBOL_cond_expr = 325,                /* cond_expr  */
  YYSYMBOL_nil_expr = 326,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 327,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 328,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 329,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 330,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 331,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 332,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 333,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 334,          /* task_var_prefix  */
  YYSYMBOL_io_expr = 335,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 336,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 337,                 /* new_expr  */
  YYSYMBOL_let_expr = 338,                 /* let_expr  */
  YYSYMBOL_expr = 339,                     /* expr  */
  YYSYMBOL_opt_expr = 340,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 341,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 342,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 343,           /* call_base_expr  */
  YYSYMBOL_call_expr = 344,                /* call_expr  */
  YYSYMBOL_dot_expr = 345,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 346,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 347,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 348,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 349,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 350,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 351,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 352,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 353,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 354,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 355       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 362 "chpl.ypp"

  // include the definition of methods for ParserContext
  #include "ParserContextImpl.h"
  // include other helper functions for the parser
  #include "parser-help.h"


#line 478 "bison-chpl-lib.cpp"

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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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
#define YYLAST   20369

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  180
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  176
/* YYNRULES -- Number of rules.  */
#define YYNRULES  712
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1264

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
       0,   639,   639,   643,   644,   649,   650,   658,   662,   670,
     671,   672,   673,   674,   675,   676,   677,   678,   679,   680,
     681,   682,   683,   696,   697,   698,   699,   700,   701,   715,
     724,   739,   748,   756,   779,   791,   802,   814,   826,   837,
     850,   857,   864,   866,   865,   874,   873,   884,   885,   889,
     909,   924,   939,   955,   956,   958,   963,   964,   969,   986,
     990,   994,  1005,  1024,  1025,  1029,  1033,  1037,  1042,  1046,
    1050,  1060,  1065,  1071,  1078,  1083,  1090,  1101,  1102,  1106,
    1110,  1117,  1125,  1134,  1141,  1149,  1161,  1168,  1172,  1179,
    1185,  1194,  1195,  1199,  1208,  1212,  1216,  1220,  1224,  1228,
    1237,  1238,  1242,  1243,  1244,  1245,  1246,  1247,  1250,  1251,
    1252,  1253,  1254,  1255,  1267,  1268,  1279,  1280,  1281,  1282,
    1283,  1284,  1285,  1286,  1287,  1288,  1289,  1290,  1291,  1292,
    1293,  1294,  1295,  1296,  1297,  1301,  1302,  1303,  1304,  1305,
    1306,  1307,  1308,  1309,  1310,  1311,  1312,  1319,  1320,  1321,
    1322,  1326,  1327,  1331,  1338,  1348,  1350,  1349,  1358,  1357,
    1368,  1372,  1378,  1378,  1384,  1384,  1393,  1394,  1395,  1396,
    1397,  1398,  1399,  1403,  1408,  1413,  1418,  1425,  1433,  1434,
    1438,  1447,  1453,  1461,  1481,  1480,  1493,  1500,  1518,  1531,
    1544,  1548,  1552,  1556,  1560,  1564,  1568,  1572,  1590,  1594,
    1598,  1602,  1606,  1610,  1614,  1618,  1622,  1626,  1630,  1634,
    1638,  1642,  1646,  1650,  1654,  1658,  1662,  1666,  1670,  1674,
    1678,  1683,  1691,  1700,  1704,  1708,  1712,  1716,  1720,  1724,
    1728,  1732,  1737,  1742,  1747,  1755,  1770,  1788,  1790,  1795,
    1796,  1801,  1806,  1807,  1808,  1809,  1810,  1811,  1812,  1813,
    1814,  1815,  1816,  1817,  1818,  1830,  1831,  1832,  1833,  1842,
    1843,  1847,  1849,  1851,  1856,  1858,  1860,  1865,  1869,  1873,
    1877,  1881,  1885,  1892,  1893,  1897,  1901,  1905,  1912,  1925,
    1941,  1949,  1953,  1962,  1963,  1967,  1971,  1976,  1986,  1991,
    2002,  2009,  2010,  2011,  2015,  2016,  2020,  2024,  2028,  2035,
    2052,  2065,  2072,  2077,  2084,  2083,  2096,  2098,  2097,  2106,
    2105,  2116,  2123,  2135,  2143,  2147,  2148,  2150,  2155,  2165,
    2162,  2190,  2198,  2206,  2217,  2228,  2237,  2250,  2251,  2255,
    2256,  2257,  2266,  2267,  2268,  2269,  2270,  2271,  2272,  2273,
    2274,  2275,  2276,  2277,  2278,  2279,  2280,  2281,  2282,  2283,
    2284,  2285,  2286,  2287,  2288,  2289,  2290,  2291,  2295,  2296,
    2297,  2298,  2299,  2300,  2301,  2302,  2303,  2304,  2305,  2306,
    2311,  2312,  2316,  2317,  2318,  2322,  2323,  2327,  2331,  2332,
    2336,  2337,  2341,  2350,  2361,  2371,  2383,  2395,  2403,  2407,
    2415,  2416,  2417,  2418,  2419,  2420,  2421,  2422,  2423,  2427,
    2428,  2429,  2430,  2431,  2432,  2436,  2437,  2438,  2442,  2443,
    2444,  2445,  2446,  2447,  2451,  2452,  2455,  2456,  2460,  2461,
    2465,  2470,  2471,  2473,  2475,  2477,  2482,  2484,  2489,  2491,
    2493,  2495,  2497,  2499,  2501,  2506,  2507,  2511,  2519,  2523,
    2528,  2536,  2540,  2547,  2568,  2569,  2571,  2579,  2580,  2581,
    2582,  2583,  2588,  2587,  2596,  2604,  2608,  2615,  2631,  2648,
    2652,  2656,  2663,  2665,  2667,  2674,  2675,  2676,  2681,  2685,
    2689,  2693,  2697,  2701,  2705,  2712,  2713,  2714,  2715,  2716,
    2721,  2722,  2723,  2724,  2725,  2745,  2749,  2753,  2757,  2764,
    2765,  2766,  2770,  2775,  2783,  2788,  2792,  2799,  2800,  2801,
    2802,  2803,  2809,  2810,  2811,  2812,  2816,  2817,  2821,  2822,
    2823,  2827,  2831,  2838,  2839,  2843,  2848,  2857,  2858,  2859,
    2860,  2864,  2865,  2876,  2878,  2880,  2886,  2887,  2888,  2889,
    2890,  2891,  2893,  2895,  2897,  2899,  2905,  2907,  2910,  2912,
    2914,  2916,  2918,  2920,  2922,  2924,  2927,  2929,  2934,  2943,
    2952,  2960,  2974,  2988,  3002,  3011,  3020,  3028,  3042,  3056,
    3070,  3086,  3095,  3104,  3119,  3137,  3155,  3163,  3164,  3165,
    3166,  3167,  3168,  3169,  3170,  3175,  3176,  3180,  3189,  3190,
    3194,  3203,  3204,  3208,  3223,  3227,  3234,  3235,  3236,  3237,
    3238,  3239,  3243,  3244,  3249,  3251,  3253,  3255,  3257,  3263,
    3280,  3284,  3288,  3292,  3299,  3307,  3308,  3309,  3310,  3311,
    3312,  3313,  3314,  3315,  3316,  3320,  3324,  3329,  3334,  3339,
    3345,  3351,  3383,  3384,  3388,  3389,  3390,  3394,  3395,  3396,
    3397,  3406,  3407,  3408,  3409,  3410,  3414,  3426,  3438,  3445,
    3447,  3449,  3451,  3453,  3459,  3472,  3473,  3477,  3481,  3488,
    3489,  3493,  3494,  3498,  3499,  3500,  3501,  3502,  3503,  3504,
    3505,  3509,  3513,  3517,  3521,  3525,  3534,  3539,  3548,  3549,
    3550,  3551,  3552,  3553,  3554,  3555,  3556,  3557,  3558,  3559,
    3560,  3561,  3562,  3563,  3564,  3565,  3566,  3567,  3568,  3569,
    3570,  3574,  3575,  3576,  3577,  3578,  3579,  3582,  3586,  3590,
    3594,  3598,  3605,  3609,  3613,  3617,  3625,  3626,  3627,  3628,
    3629,  3630,  3631
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
  "TCONFIG", "TCONST", "TCONTINUE", "TDEFER", "TDELETE", "TDEPRECATED",
  "TDMAPPED", "TDO", "TDOMAIN", "TELSE", "TENUM", "TEXCEPT", "TEXPORT",
  "TEXTERN", "TFALSE", "TFOR", "TFORALL", "TFOREACH", "TFORWARDING", "TIF",
  "TIMAG", "TIMPORT", "TIN", "TINCLUDE", "TINDEX", "TINLINE", "TINOUT",
  "TINT", "TITER", "TINITEQUALS", "TIMPLEMENTS", "TINTERFACE", "TLABEL",
  "TLAMBDA", "TLET", "TLIFETIME", "TLOCAL", "TLOCALE", "TMINUSMINUS",
  "TMODULE", "TNEW", "TNIL", "TNOINIT", "TNONE", "TNOTHING", "TON",
  "TONLY", "TOPERATOR", "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED",
  "TPARAM", "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC",
  "TPROTOTYPE", "TPUBLIC", "TREAL", "TRECORD", "TREDUCE", "TREF",
  "TREQUIRE", "TRETURN", "TSCAN", "TSELECT", "TSERIAL", "TSHARED",
  "TSINGLE", "TSPARSE", "TSTRING", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS",
  "TTHROW", "TTHROWS", "TTRUE", "TTRY", "TTRYBANG", "TTYPE", "TUINT",
  "TUNDERSCORE", "TUNION", "TUNMANAGED", "TUSE", "TVAR", "TVOID", "TWHEN",
  "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND",
  "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
  "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL",
  "TASSIGNSR", "TBANG", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON",
  "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP",
  "TGREATER", "TGREATEREQUAL", "THASH", "TIO", "TLESS", "TLESSEQUAL",
  "TMINUS", "TMOD", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI",
  "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP",
  "TRP", "TLSBR", "TRSBR", "TNOELSE", "TDOTDOTOPENHIGH", "TUPLUS",
  "TUMINUS", "TLNOT", "$accept", "program", "toplevel_stmt_ls",
  "toplevel_stmt", "pragma_ls", "stmt", "deprecated_decl_stmt", "$@1",
  "$@2", "deprecated_decl_base", "module_decl_start", "module_decl_stmt",
  "access_control", "opt_prototype", "include_module_stmt",
  "block_stmt_body", "block_stmt", "stmt_ls", "renames_ls",
  "use_renames_ls", "opt_only_ls", "use_stmt", "import_stmt",
  "import_expr", "import_ls", "require_stmt", "assignment_stmt",
  "opt_label_ident", "ident_fn_def", "ident_def", "ident_use",
  "internal_type_ident_def", "scalar_type", "reserved_type_ident_use",
  "do_stmt", "return_stmt", "deprecated_class_level_stmt", "$@3", "$@4",
  "class_level_stmt", "$@5", "$@6", "inner_class_level_stmt",
  "forwarding_decl_stmt", "forwarding_decl_start", "extern_or_export",
  "extern_export_decl_stmt_start", "extern_export_decl_stmt", "$@7",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "try_stmt", "catch_expr_ls",
  "catch_expr", "catch_expr_inner", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_start",
  "class_tag", "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_header_lcbr", "enum_ls", "$@8", "deprecated_enum_item", "$@9",
  "$@10", "enum_item", "lambda_decl_expr", "linkage_spec_empty",
  "linkage_spec", "fn_decl_stmt_complete", "fn_decl_stmt", "$@11",
  "fn_decl_stmt_inner", "fn_decl_stmt_start", "fn_decl_receiver_expr",
  "fn_ident", "op_ident", "assignop_ident", "all_op_name",
  "formal_var_arg_expr", "opt_formal_ls", "req_formal_ls",
  "formal_ls_inner", "formal_ls", "formal", "opt_formal_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_iter_or_op",
  "opt_ret_tag", "opt_throws_error", "opt_function_body_stmt",
  "function_body_stmt", "query_expr", "opt_lifetime_where",
  "lifetime_components_expr", "lifetime_expr", "lifetime_ident",
  "type_alias_decl_stmt", "type_alias_decl_stmt_start",
  "type_alias_decl_stmt_inner_ls", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_type", "var_decl_stmt", "$@12",
  "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
  "tuple_var_decl_component", "tuple_var_decl_stmt_inner_ls",
  "opt_init_expr", "ret_array_type", "opt_ret_type", "opt_type",
  "array_type", "opt_formal_array_elt_type", "formal_array_type",
  "opt_formal_type", "expr_ls", "simple_expr_ls", "tuple_component",
  "tuple_expr_ls", "opt_actual_ls", "actual_ls", "actual_expr",
  "ident_expr", "type_level_expr", "sub_type_level_expr", "for_expr",
  "cond_expr", "nil_expr", "stmt_level_expr", "opt_task_intent_ls",
  "task_intent_clause", "task_intent_ls", "forall_intent_clause",
  "forall_intent_ls", "intent_expr", "task_var_prefix", "io_expr",
  "new_maybe_decorated", "new_expr", "let_expr", "expr", "opt_expr",
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

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434
};
#endif

#define YYPACT_NINF (-1116)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-713)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1116,   133,  3608, -1116,     2,   117, -1116, -1116, -1116, -1116,
   -1116, -1116,  5000,   105,   239,   249, 14631,   384, 20134,   105,
   11754,   424,   279,   421,   239,  5000, 11754,   496,  5000,   345,
   20220, -1116,    93,   482,  8642, 10026, 10026, -1116,  8814,   483,
     387,   383, -1116,   521, 20220, 20220, 20220,   407,  2152, 10198,
     529, 11754,   246, -1116,   539,   545, 11754, -1116, 14631, -1116,
   11754,   537,   440,   416,   565,   560, 20254, -1116, 10372,  8124,
   11754, 10198, 14631, 11754,   546,   594,   490,  5000,   604, 11754,
     610, 11926, 11926, -1116,   618, -1116, 14631, -1116,   628,  8814,
   11754, -1116, 11754, -1116, 11754, -1116, -1116, 14289, 11754, -1116,
   11754, -1116, -1116, -1116,  3956,  7260,  8988, 11754, -1116,  4826,
   -1116, -1116, -1116,   524, -1116,   248, -1116, -1116, -1116, -1116,
   -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116,  7432,
   -1116, 10544, -1116, -1116, -1116, -1116, -1116,   634, -1116, -1116,
   -1116, -1116, -1116, -1116, -1116,   550, 20220, -1116, 20100,   499,
   -1116,   201, -1116, -1116,   388,   412, -1116, 20220,  2152, -1116,
     534, -1116,   547, -1116, -1116,   538,   543,   566, 11754,   556,
     561, 19892, 14282,   289,   567,   570, -1116, -1116,   429, -1116,
   -1116, -1116, -1116, -1116,   390, -1116, -1116, 19892,   569,  5000,
   -1116, -1116,   579, 11754, -1116, -1116, 11754, 11754, 11754, 20220,
   -1116, 11754, 10372, 10372,   681,   516, -1116, -1116, -1116, -1116,
     138,   526, -1116, -1116,   581, 16445, -1116,   266, -1116,   589,
   -1116,   -36, 19892, -1116,  2491,   646,  8298, -1116, -1116, -1116,
   -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116,
   -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116,
   -1116,   596, -1116, -1116, -1116, 20220,   595,    40, 16197,    38,
   16032,    38, 16114, 20220, 20220,   179, 15285, -1116, -1116,   683,
    8298,   598,   532,  5000,   122,   388,  1325,   102,    92, -1116,
    5000, -1116, -1116, 16527,  1497, -1116,   599,   602, -1116, 16527,
     138,  1497, -1116,  8298,  2112,  2112, -1116, -1116,   209, 19892,
   11754, 11754, -1116, 19892,   617, 17103, -1116, 16527,   138, 19892,
     611,  8298, -1116, 19892, 17186, -1116, -1116, 17268, 14624, -1116,
   -1116, 17349,   138,    40, 16527, 17430,   405,   405,  1027,  1497,
    1497,   158, -1116, -1116,  4130,   -29, -1116, 11754, -1116,   -37,
     104, -1116,   -32,   -14, 17511,   124,  1027,   775, -1116,  4304,
   11754, -1116, 11754,   719, -1116, -1116, 16280,   222,   550, 19892,
     603, 20220, 10372,   621, -1116,   622,   779,   667,   127, -1116,
   -1116, -1116, -1116, -1116, -1116,   705, -1116, -1116, -1116,    54,
     706, -1116, -1116, -1116, 14122,   678,   640,   662,   370,   535,
   -1116, 11754,   664, 11754, 11754, 11754, 10026, 10026, 11754,   555,
   11754, 11754, 11754, 11754, 11754,   219, 14289, 11754, 11754, 11754,
   11754, 11754, 11754, 11754, 11754, 11754, 11754, 11754, 11754, 11754,
   11754, 11754, 11754,   744, -1116, -1116, -1116, -1116, -1116,  9160,
    9160, -1116, -1116, -1116, -1116,  9160, -1116, -1116,  9160,  9160,
    8298,  8298, 10026, 10026,  7952, -1116, -1116, 16610, 16692, 17592,
     648,    42, 20220,  4478, -1116, 10026,    40,  2152, -1116, 11754,
   -1116,  2491, -1116, 20220,   707, -1116, -1116,   683, 11754,   693,
   -1116,   651,   680, -1116, -1116, -1116,   780, 10372, -1116,  5174,
   10026, -1116,   656, -1116,    40,  5348, 10026, -1116,    40, -1116,
      40, 10026, -1116,    40,   710,   711,  5000,   801,  5000, -1116,
     804, 11754,   782,   668,  8298, 20220, -1116, -1116,    62, -1116,
   -1116, -1116, -1116, -1116, -1116,  1517,   700,   682, -1116, 14997,
   -1116,    71, -1116,  1325, -1116, -1116,   181, -1116, 12098,   733,
   11754,  2152, -1116, -1116, 11754, 11754, -1116,   689, -1116, -1116,
   10372, -1116, 19892, 19892, -1116,    49, -1116,  8298,   690, -1116,
     842, -1116,   842, -1116, -1116, -1116, -1116, -1116, -1116, -1116,
    9334, -1116, 17673,  7606, -1116,  7780, -1116,  5000,   691, 10026,
    9508,  3782,   692, 11754, 10716, -1116, -1116,   252, -1116,  4652,
   -1116,   415, 17754,   420,   475, 20220,  7086,  7086, -1116,   550,
     698,   266, -1116,   -42,   725,  1275, -1116, -1116, 20220, 11754,
     189, -1116, -1116, -1116, 12270,   767, -1116,   701,   140, -1116,
     726, -1116,   730,   732,   741,   734,   736, -1116,   737,   746,
     739,   743,   749,   228,   750,   751,   752, -1116, -1116, -1116,
   -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116,
   -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116,
   11754, -1116,   761,   762,   755,   701, -1116,   701, -1116, 12442,
   -1116, -1116, 20220, -1116, 17835,  3220,  3220,   544, -1116,   544,
   -1116,   544, 19203,   612,   709,  1315,   138,   405, -1116,   573,
   -1116, -1116, -1116, -1116, -1116,  1027,  1127,   544,  2555,  2555,
    3220,  2555,  2555,  1344,   405,  1127, 16733,  1344,  1497,  1497,
     405,  1027,   724,   727,   740,   748,   753,   754,   735,   745,
   -1116,   544, -1116,   544,    91, -1116, -1116, -1116,   184, -1116,
    1876, 19933,   433, 12614, 10026, 12786, 10026, 11754,  8298, 10026,
   14911,   756,   105, 17916, -1116,   425, 19892, -1116, 17997,  8298,
   -1116,  8298, 11754,   185,  8814, 19892,    66, 16773,  7952, -1116,
    8814, 19892,    56,  2886, -1116, -1116,    38, 16362, -1116, 11754,
   11754,   873,  5000,   875, 18078,  5000, 16855, 20220, -1116,   190,
   -1116,   194, -1116, -1116, -1116, 20220,  1619, -1116,  1325,   769,
      35,   198,  1325,   102,    -2,    12, 11754, 11754,  6914, -1116,
   -1116,   200,  8470, -1116, 19892, -1116, 18159, 18240, -1116, -1116,
   19892,   758,   134,   757, -1116,  1514, -1116, -1116, -1116, -1116,
   -1116, -1116, -1116, -1116,  5000,   -13, 16938, -1116, -1116, 19892,
    5000, 19892, -1116, 18321, -1116, -1116, 11754, -1116,    76, 15045,
   11754, -1116, 10888,  7086,  7086, -1116, 11754,   444, 11754,   498,
     585, 11754,  9680,   593,   427, -1116, -1116, -1116, 18402,   770,
     766,   765, -1116,  2152, -1116,  8298,   768,  1736, 20220, -1116,
   19892, 19704,  6740, -1116, -1116,   199, -1116,    35,   122, -1116,
   18483, -1116, 15203, -1116, -1116, -1116,   443, -1116,   759,   763,
   -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116,
    7952, -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116, -1116,
   -1116, -1116, -1116,    48, 10026, 10026, 11754,   893, 18564, 11754,
     902, 18645,   312,   771, 18726,  8298,    40,    40, -1116, -1116,
   -1116, -1116, -1116, 16527, -1116, 15368,  5522, -1116,  5696, -1116,
     313, -1116, 15451,  5870, -1116,    40,  6044, -1116,    40, -1116,
      40, -1116,    40, 19892, 19892,  5000, -1116,  5000, 11754, -1116,
    5000,   904,   776,   777, 20220,   581,   769, -1116,   319, 11062,
     310,   152, 11754,   180, -1116, -1116,   733,   772,    82, -1116,
   -1116,   773,   781, -1116,  6218, 10372, -1116, -1116, -1116, 20220,
   -1116,   798,   581, -1116,  6392,   774,  6566,   783, -1116, 11754,
   -1116, -1116,  7086, -1116, 18807,    81, 17021,   449,   789,  7086,
   -1116, 11754, -1116, -1116,  2702,   479,   330, -1116,   935, -1116,
   -1116, -1116, 19494, -1116, -1116, -1116, -1116,   785, 13990,    88,
   -1116,   784, -1116,   808,   815,   701,   701, -1116, -1116, -1116,
     733,   331,   361, 18889, 12958, 13130, 18970, 13302, 13474, -1116,
   13646, 13818,   368, -1116, -1116, -1116,  5000,  8814, 19892,  8814,
   19892,  7952, -1116,  5000,  8814, 19892, -1116,  8814, 19892, -1116,
   -1116, -1116, -1116, -1116, 19892,   925,  5000, -1116, -1116, -1116,
   -1116,   310,   769,  9854, -1116, -1116, -1116,   -24, 10372, -1116,
   -1116, -1116,    80, -1116,     9, -1116,   374, 19064, -1116, -1116,
   -1116, -1116, -1116, 10026, 14771,  8298,  8298,  5000, -1116,    65,
     790, 11754, -1116,  8814, -1116, 19892,  5000,  8814, -1116, 19892,
    5000, 19892,   372, 11234,  7086,  7086,  7086,  7086, -1116, -1116,
   -1116, 19121, 19892,  3052, -1116, -1116,   800, -1116,  1665, -1116,
   -1116, -1116,   489,  3417,   182, -1116, -1116, -1116, -1116, 11754,
   11754, 11754, 11754, 11754, 11754, 11754, 11754, -1116, 18078, 15534,
   15617, -1116, 18078, 15700, 15783,  5000, -1116, -1116, -1116,   310,
   11408,    96, -1116, 19892, -1116, 11754,   152,    80,    80,    80,
      80,    80,    80,   152, 19161, -1116,   644,   793,   794,   646,
   -1116,   581, 19892, 15866, -1116, 15949, -1116, -1116, -1116, 19892,
     501,   803,   507,   807, 11754, -1116, -1116,  1665, -1116, -1116,
     742, -1116, -1116, -1116, 19312, 19352, 19503, 19543, 19694, 19734,
    5000,  5000,  5000,  5000, -1116, -1116, -1116, -1116, -1116, -1116,
     114, 10026, 14460, 19892, -1116, -1116, -1116, -1116, -1116, -1116,
   -1116,   823, 13818,   812,   818, -1116,  5000,  5000, -1116, -1116,
   -1116, -1116, 19892, -1116, 11754, 11754, 11754, 11754, 11754, 11754,
   18078, 18078, 18078, 18078, 19775, -1116, -1116,   834, -1116, -1116,
   18078, 18078, 11582, -1116
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   114,   655,   656,   657,   651,
     652,   658,     0,   575,   100,   135,   544,   142,   546,   575,
       0,   141,   452,   450,   100,     0,     0,    45,     0,   259,
     136,   179,   178,   649,     0,     0,     0,   177,     0,   140,
      53,   260,   316,   137,     0,     0,     0,     0,     0,     0,
     144,     0,   594,   566,   659,   145,     0,   317,   538,   447,
       0,     0,     0,   164,   162,   139,   547,   449,     0,     0,
       0,     0,   542,     0,     0,   143,     0,     0,   115,     0,
     650,     0,     0,   438,   138,   293,   540,   451,   146,     0,
       0,   708,     0,   710,     0,   711,   712,   621,     0,   709,
     706,   525,   160,   707,     0,     0,     0,     0,     4,     0,
       5,     9,    42,     0,    47,    56,    10,    62,    11,    12,
      13,    14,    15,   521,   522,    25,    48,   161,   171,     0,
     180,   622,   172,    16,    20,    17,    19,     0,   254,    18,
     613,    23,    24,    21,   170,   294,     0,   168,     0,   610,
     315,     0,   166,   318,   408,   399,   169,     0,     0,   167,
     627,   606,   523,   607,   528,   526,     0,     0,     0,   611,
     612,     0,   527,     0,   628,   629,   630,   653,   654,   605,
     530,   529,   608,   609,     0,    41,    27,   536,     0,     0,
     576,   101,     0,     0,   546,   136,     0,     0,     0,     0,
     547,     0,     0,     0,     0,   610,   627,   526,   611,   612,
     545,   527,   628,   629,     0,   575,   439,     0,   448,     0,
      22,     0,   506,    43,   314,     0,   513,   108,   116,   128,
     122,   121,   130,   111,   120,   131,   117,   132,   109,   133,
     126,   119,   127,   125,   123,   124,   110,   112,   118,   129,
     134,     0,   113,   186,   440,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,    54,    56,
     513,     0,     0,     0,   388,   408,     0,     0,     0,   455,
       0,   152,    35,     0,   693,   598,   595,   596,   597,     0,
     539,   694,     7,   513,   314,   314,   420,   503,     0,   502,
       0,     0,   153,   626,     0,     0,    38,     0,   543,   531,
       0,   513,    39,   537,     0,   273,   269,     0,   527,   273,
     270,     0,   541,     0,     0,     0,   695,   697,   619,   692,
     691,     0,    59,    63,     0,     0,   508,     0,   510,     0,
       0,   509,     0,     0,   502,     0,   620,     0,     6,     0,
       0,    57,     0,     0,   452,   176,     0,   651,   294,   623,
     184,     0,     0,     0,   290,     0,   309,   311,     0,   302,
     306,   406,   407,   405,   326,   409,   412,   411,   413,     0,
     403,   400,   401,   404,     0,   444,     0,   441,     0,   524,
      26,     0,   599,     0,     0,     0,     0,     0,     0,   696,
       0,     0,     0,     0,     0,     0,   618,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   358,   365,   366,   367,   362,   364,     0,
       0,   360,   363,   361,   359,     0,   369,   368,     0,     0,
     513,   513,     0,     0,     0,    28,    29,     0,     0,     0,
       0,     0,     0,     0,    30,     0,     0,     0,    31,     0,
      32,   314,   291,     0,   178,   292,    46,    56,     0,   521,
     519,     0,   514,   515,   520,   301,     0,     0,   196,     0,
       0,   195,     0,   204,     0,     0,     0,   202,     0,   212,
       0,     0,   210,     0,     0,     0,     0,   228,     0,   358,
     224,     0,     0,     0,     0,     0,   238,    33,   395,   390,
     391,   392,   396,   397,   398,   388,   381,     0,   378,     0,
     389,     0,   459,     0,   460,   462,     0,   484,     0,   465,
       0,     0,   151,    34,     0,     0,    36,     0,   165,   163,
       0,    93,   624,   625,   154,     0,    37,     0,     0,   280,
     271,   267,   272,   268,   189,   188,    40,    61,    60,    64,
       0,   660,     0,     0,   645,     0,   647,     0,     0,     0,
       0,     0,     0,     0,     0,   664,     8,     0,    50,     0,
      91,     0,    87,     0,    71,     0,     0,    77,   173,   294,
       0,     0,   183,   265,   295,     0,   300,   307,     0,     0,
     304,   299,   410,   479,     0,   414,   402,   375,   102,   353,
     116,   351,   122,   121,   105,   120,   117,   356,   132,   103,
     133,   119,   123,   104,   106,   118,   134,   350,   332,   335,
     333,   334,   357,   345,   336,   349,   341,   339,   352,   355,
     340,   338,   343,   348,   337,   342,   346,   347,   344,   354,
       0,   329,     0,   107,     0,   375,   330,   375,   327,     0,
     443,   437,     0,   454,   574,   688,   687,   690,   699,   698,
     703,   702,   684,   681,   682,   683,   615,   671,   114,     0,
     641,   642,   115,   640,   639,   616,   675,   686,   680,   678,
     689,   679,   677,   669,   674,   676,   685,   668,   672,   673,
     670,   617,     0,     0,     0,     0,     0,     0,     0,     0,
     701,   700,   705,   704,   586,   587,   589,   591,     0,   578,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     662,   265,   575,   575,   192,     0,   507,    44,     0,     0,
     533,     0,     0,     0,     0,   550,     0,     0,     0,   205,
       0,   556,     0,     0,   203,   213,     0,     0,   211,     0,
       0,   227,     0,   223,     0,     0,     0,     0,   532,     0,
     241,     0,   239,   393,   394,     0,   388,   377,     0,   497,
     421,     0,   463,     0,   149,   150,   148,   147,     0,   483,
     482,   606,     0,   457,   604,   456,     0,     0,   638,   505,
     504,     0,     0,     0,   534,     0,   274,   661,   614,   646,
     511,   648,   512,   220,     0,     0,     0,   663,   218,   560,
       0,   666,   665,     0,    52,    51,     0,    86,     0,     0,
       0,    79,     0,     0,    77,    49,   350,   332,   335,   333,
     334,   343,   342,   344,     0,   370,   371,    66,    65,    78,
       0,     0,   296,     0,   262,     0,     0,   314,     0,   310,
     312,     0,     0,   478,   477,   606,   415,   421,   388,   325,
       0,   331,     0,   321,   322,   446,   606,   442,     0,     0,
      99,    97,    98,    96,    95,    94,   636,   637,   588,   590,
       0,   577,   135,   142,   141,   140,   137,   144,   145,   139,
     143,   138,   146,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   453,   187,
     517,   518,   516,     0,   222,     0,     0,   194,     0,   193,
       0,   581,     0,     0,   200,     0,     0,   198,     0,   208,
       0,   206,     0,   236,   235,     0,   230,     0,     0,   226,
       0,   232,     0,   264,     0,     0,   497,   379,     0,     0,
     465,     0,     0,     0,   461,   464,   465,     0,     0,   466,
     467,     0,     0,   282,     0,     0,   281,   284,   535,     0,
     275,   278,     0,   221,     0,     0,     0,     0,   219,     0,
      92,    89,     0,    88,    74,    73,    72,     0,     0,     0,
     174,     0,   175,   296,   314,     0,     0,   289,   158,   164,
     162,   288,   314,   297,   155,   308,   305,     0,   469,     0,
     319,     0,   328,   102,   104,   375,   375,   643,   644,   579,
     465,   627,   627,     0,     0,     0,     0,     0,     0,   264,
       0,     0,     0,   191,   190,   197,     0,     0,   549,     0,
     548,     0,   580,     0,     0,   555,   201,     0,   554,   199,
     209,   207,   229,   225,   565,   231,     0,    58,   261,   240,
     237,   465,   497,     0,   500,   499,   501,   606,   372,   384,
     382,   435,     0,   436,   423,   426,     0,   422,   418,   419,
     313,   458,   488,     0,   662,   513,   513,     0,   286,     0,
       0,     0,   276,     0,   216,   562,     0,     0,   214,   561,
       0,   667,     0,     0,     0,    77,     0,    77,    80,    83,
      69,    68,    67,   314,   181,   185,   266,   156,   314,   298,
     474,   472,   606,   662,     0,   376,   323,   324,   583,     0,
       0,     0,     0,     0,     0,     0,     0,   266,   553,     0,
       0,   582,   559,     0,     0,     0,   234,   385,   383,   465,
     489,     0,   374,   373,   434,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   486,   606,     0,     0,   287,
     285,     0,   279,     0,   217,     0,   215,    90,    76,    75,
       0,     0,     0,     0,     0,   182,   263,   314,   159,   473,
     606,   416,   320,   417,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   233,   387,   386,   491,   492,   494,
     606,     0,   662,   425,   427,   428,   431,   432,   433,   429,
     430,   424,     0,   600,   601,   277,     0,     0,    82,    85,
      81,    84,    70,   157,     0,     0,     0,     0,     0,     0,
     552,   551,   558,   557,     0,   493,   495,   606,   602,   603,
     564,   563,   489,   496
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1116, -1116, -1116,     3,  -265,  2292, -1116, -1116, -1116,  -196,
   -1116, -1116,   -28,   713, -1116,  -922,  2402,   629,  -550, -1116,
    -809, -1116, -1116,   159, -1116, -1116,   237,   960, -1116,  2666,
    -173,  -377, -1116,  -588,    53,  -921,   -26, -1116, -1116,  -776,
   -1116, -1116,   125, -1116, -1116, -1116, -1116, -1116, -1116, -1116,
   -1116,    28, -1116,   901, -1116, -1116,    43,   971, -1116, -1116,
   -1116, -1116,   673, -1116,    15, -1116, -1116, -1116, -1116, -1116,
    -100, -1116,  -337,  -786, -1116, -1116, -1116, -1116,   139, -1116,
   -1116,  -580,  1178, -1116, -1116, -1116,   641, -1116, -1116, -1116,
   -1116,   130,  -373,  -139,  -778,  -995,  -612, -1116, -1116,   136,
     229,   493, -1116, -1116, -1116,   738, -1116, -1116,  -125,    99,
     145,  -156,  -150,  -717, -1116, -1116,   356, -1116, -1116,  -198,
     890, -1116,  -157,   500,   238,  -503,  -928,  -934, -1116,  -491,
    -646, -1115, -1059,  -904,   -66, -1116,   192, -1116,  -264,  -477,
    -512,   571,  -513, -1116, -1116, -1116,  1313, -1116,    -9, -1116,
   -1116,  -148, -1116,  -731, -1116, -1116, -1116,  1524,  1857,   -12,
   -1116,     6,   760, -1116,  2036,  2355, -1116, -1116, -1116, -1116,
   -1116, -1116, -1116, -1116, -1116,  -441
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   333,   109,   763,   111,   461,   224,   112,
     113,   114,   115,   353,   116,   117,   118,   334,   849,   583,
     850,   119,   120,   580,   581,   121,   122,   192,   651,   277,
     123,   252,   124,   789,   282,   125,  1013,  1197,  1128,   126,
     295,   294,   127,   128,   129,   130,   131,   132,   591,   133,
     134,   913,   135,   265,   136,   771,   772,   204,   138,   139,
     140,   141,   550,   806,   982,   142,   143,   802,   977,   144,
     145,   146,   363,   857,   147,   148,   368,   861,   369,   858,
     598,   370,   205,   150,   151,   152,   153,  1134,   154,   155,
     654,   655,   845,   846,   847,  1079,   869,   275,   516,   517,
     518,   519,   520,   384,   374,   379,   867,  1202,  1090,   470,
     963,  1084,  1085,  1086,   156,   157,   386,   387,   660,   158,
     159,   217,   278,   279,   525,   526,   793,   864,   605,   529,
     790,  1218,  1076,   960,   335,   221,   339,   340,   471,   472,
     473,   206,   161,   162,   163,   164,   207,   166,   189,   190,
     718,   484,   930,   719,   720,   167,   168,   208,   209,   171,
     360,   474,   211,   173,   212,   213,   176,   177,   178,   179,
     345,   180,   181,   182,   183,   184
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     187,   388,   298,   722,   210,   108,   503,   653,   215,   515,
     214,   656,   269,   875,   222,   791,   863,   931,   859,   457,
     781,   590,   258,   260,   262,   998,   266,   769,   466,   537,
     780,   358,  1080,   439,   569,   803,   844,   283,  1091,   284,
     343,  1088,  1089,   873,   289,   874,   290,   548,   291,   527,
     801,   993,  1071,   469,  -259,   603,   299,   303,   305,   307,
     308,   309,   257,   259,   261,   313,  1004,   314,  -260,   317,
     321,   280,   527,   280,   322,   304,  1157,   324,   325,   678,
     326,  1014,   327,  1081,  1131,   328,   329,   482,   330,   933,
     729,   865,   299,   303,   344,   346,   961,   469,   280,   926,
    -498,  1219,  1138,   527,   253,   482,   482,  1255,   563,   459,
     773,   341,   488,   490,   493,  -475,   560,   356,  1114,   359,
     469,  -498,   854,  -283,   306,  -498,  1165,   501,   460,   855,
    1093,   570,  -480,     3,   342,   564,   729,   451,   469,   888,
     530,   561,   567,  1158,  1221,  -475,   876,  1263,  -498,   508,
    -480,   774,   962,  1115,  1166,  1081,   392,   482,  -475,  1029,
     571,   984,  -480,  1256,  1215,  -283,   185,   297,  1159,   226,
     509,  -475,  -480,  -242,   510,   482,   708,   709,   682,   439,
     889,   187,  1083,   270,   447,   448,   449,   570,  -480,   313,
     299,   344,   528,  -480,   568,   572,   467,   511,   604,  1199,
     512,   254,    61,   297,   338,   297,   456,   453,   974,   453,
     540,   513,  1088,  1089,   303,   528,   730,  1123,  -475,  -283,
    -480,  1120,   678,  -475,   188,   453,  -480,   570,  1014,   922,
     514,  1216,   684,   570,   453,   453,  1014,   531,  -490,   679,
    -480,   570,   191,  1082,   462,   657,   528,  -480,  -481,   565,
     975,   912,   851,   680,  1083,   371,  1094,   589,   303,  -490,
    -476,  -481,  1133,  -490,   285,   737,  -480,   469,   469,   574,
    1222,    69,   600,    69,   372,   958,   566,   399,  1015,  -108,
     496,   303,   681,   997,   373,   405,  -490,  -114,   542,   543,
    -476,  -481,   966,    23,  -380,   350,   594,   601,   575,   303,
     735,   297,   297,  -476,   976,  -243,  1191,   722,  1193,   465,
     478,   481,   483,   487,   489,   492,  -476,  -481,   316,   320,
    1151,   682,   185,   286,  -481,   562,   782,   683,   557,   890,
     540,   469,   351,    85,  -303,   741,   533,   559,   582,   954,
     584,   287,   536,   782,    59,  -481,  1201,  1014,   453,   453,
     299,   453,  1198,   783,   540,    67,   891,   924,   288,  -303,
     546,   352,   953,  -476,  -481,  1164,   955,  -110,  -476,  -481,
     964,  1074,  -481,   541,   469,  -115,   554,   555,  1006,   664,
      87,   665,   666,   667,   669,   671,   672,   216,   673,   674,
     675,   676,   677,   853,   685,   686,   687,   688,   689,   690,
     691,   692,   693,   694,   695,   696,   697,   698,   699,   700,
     701,   743,  1030,  1136,  1137,   375,   185,   303,   303,   538,
     539,  1243,   824,   303,   668,   670,   303,   303,   303,   303,
     711,   713,   721,   467,   792,   703,   704,   395,  1042,   380,
    -249,   705,  1112,   733,   706,   707,  1077,   736,  1175,   722,
    1225,  1226,  1227,  1228,  1229,  1230,   738,   741,  1051,  1078,
     440,   297,   441,   -55,   782,   299,   376,   745,   747,   267,
     710,   712,   268,   751,   753,   741,  -585,   377,   442,   757,
    -248,   -55,   443,   732,  1039,  1052,   764,   393,   832,   766,
     381,  1072,   303,   396,   394,   653,   378,   397,  1167,   656,
     -55,   382,  1126,  -585,   223,  1132,  -584,   395,   746,   734,
     218,   515,   833,   741,   752,   531,   226,   999,   794,   756,
     383,   905,   796,   797,  1168,   443,  1169,  1170,   800,   -55,
    1171,  1172,  -710,  -584,   663,   303,  -710,   749,  -257,  -247,
    1147,   754,  1187,   755,   399,   292,   758,   834,   800,   403,
    -468,   303,   405,   303,   270,   469,   408,   816,   800,   819,
     826,   821,   823,   396,  1190,   830,  1192,   397,   469,   341,
     531,   341,   999,   160,   848,   848,   297,  -244,   274,   827,
    -468,  1176,   559,   160,   831,  -251,  -711,   860,  -445,   918,
    -711,  1000,  1012,  -468,   999,  -255,   160,   815,   398,   160,
    -635,  -252,  -635,   515,   935,   938,  -468,  -445,   940,   942,
     722,   293,   -54,  1118,   399,   400,  -246,   401,   402,   403,
    1200,   404,   405,   406,   531,   407,   408,   409,   410,   411,
     -54,   412,   413,   414,   415,   416,   417,   418,   870,   799,
     419,   420,   421,  1125,   395,   310,   999,  1220,   160,   -54,
    -250,   422,   999,  -468,    42,   991,   684,  -314,  -468,   799,
    -256,   311,   338,  -571,   338,  1238,  -258,   985,   987,   799,
    -634,  1240,  -634,  -712,  -245,   160,  -314,  -712,   -54,    57,
     160,  -707,   469,   399,  -253,  -707,  -314,  -634,   403,  -634,
     361,   405,  -485,   349,   362,   408,  1005,  -631,  -568,  -631,
     396,   453,  -567,   505,   397,  -485,  -633,   390,  -633,  1220,
     389,   745,   908,   751,   911,   764,   303,   914,   819,  1257,
    -572,   391,   968,   916,   917,  -573,  -632,   303,  -632,   303,
     923,  -570,   925,  1026,  -569,  -485,   721,   452,   932,  1012,
     444,   395,   469,   446,   878,   921,   879,   943,   944,  1220,
     453,   399,   907,   458,   910,   810,   403,   812,   404,   405,
     160,  -485,   172,   408,   468,   475,   477,   351,  -485,   504,
     534,   415,   172,   535,   309,   313,   344,   419,   420,   421,
     303,   544,   547,   576,   585,   172,   501,   597,   172,  -485,
     595,   599,   596,   501,   602,   606,  1019,   396,   970,   927,
     929,   397,   659,  -470,   661,   934,   937,   662,  -485,   939,
     941,   405,   702,  -485,   582,   254,  -485,   739,   994,   728,
     996,   848,   848,   740,   326,   741,   327,   748,   742,   329,
     330,  1177,  1178,  -470,   759,   760,   762,   172,   920,   765,
     768,   318,   318,   303,   160,   776,  -470,   767,   399,   400,
     344,   160,   402,   403,   777,   404,   405,   792,  1012,  -470,
     408,   798,   804,   805,   172,   814,   820,   852,   415,   172,
     540,   866,   868,  -135,   419,   420,   421,  -142,   721,  -141,
    -111,  -140,  -487,  -137,  -144,  -109,  -145,   297,   880,  -112,
    -139,   881,   669,   711,  1033,  -487,  -143,  1036,  -138,  -146,
     871,  -113,   872,   303,   882,   160,  -470,   886,   945,  1099,
     947,  -470,   883,   959,  1048,   999,  1050,   884,   885,   887,
     160,  1055,   469,   469,  1058,  -487,  1034,   915,   973,   978,
    1002,  1027,   668,   710,  1003,  1037,  1064,  1028,  1007,  1066,
    1067,  1068,  1101,  1127,  1095,  1040,  1092,  -108,  1106,   172,
    1087,  -487,  1096,  1119,  -110,   658,  1135,  1110,  -487,  1130,
    1155,   297,  1181,   299,  1196,  1233,  1234,  1239,  1166,  1043,
    1044,  1241,  1105,   137,  1109,  1258,  1045,  1111,   579,  -487,
     848,  1259,   502,   137,   219,   990,  1129,  1121,  1056,  1122,
     323,  1059,   552,  1060,  1100,  1061,   137,  1069,  -487,   137,
    1016,   592,  1025,  -487,  1021,   957,  -487,  1161,   775,  1203,
     501,   501,  1020,   521,   501,   501,  1224,  1231,   877,   355,
     965,     0,  1048,  1050,   160,  1055,  1058,     0,  1105,  1109,
       0,   795,     0,   172,  1148,  1149,     0,  1150,     0,   721,
     172,  1152,  1153,     0,   501,  1154,   501,     0,   137,     0,
     160,     0,     0,     0,     0,     0,   160,     0,  1075,   395,
       0,   344,     0,     0,     0,     0,  1163,   160,     0,   160,
       0,     0,     0,     0,   297,   137,     0,     0,     0,     0,
     137,  1174,   819,   303,   303,     0,     0,     0,     0,  1182,
       0,  1183,     0,     0,   172,  1185,     0,     0,     0,     0,
       0,  1189,   848,   848,   848,   848,     0,     0,     0,   172,
       0,     0,     0,     0,     0,   396,     0,     0,     0,   397,
       0,   819,     0,     0,     0,     0,     0,  1148,  1204,  1205,
    1152,  1206,  1207,  1208,  1209,     0,     0,     0,   160,     0,
       0,     0,   160,     0,     0,     0,     0,     0,     0,     0,
     160,     0,  1180,  1223,     0,     0,     0,     0,     0,   395,
     137,     0,     0,     0,     0,     0,   399,   400,     0,   401,
     402,   403,   297,   404,   405,     0,     0,  1162,   408,     0,
     149,     0,  1242,     0,     0,   414,   415,     0,     0,   418,
     149,     0,   419,   420,   421,     0,     0,     0,  1250,  1251,
    1252,  1253,     0,   149,     0,     0,   149,     0,     0,  1254,
     819,     0,     0,   172,     0,   396,     0,     0,     0,   397,
    1109,     0,     0,     0,  1260,  1261,     0,     0,     0,     0,
       0,     0,  1250,  1251,  1252,  1253,  1260,  1261,     0,   172,
       0,     0,     0,     0,   137,   172,     0,     0,     0,     0,
    1109,   137,     0,     0,     0,   149,   172,     0,   172,  1217,
       0,     0,     0,     0,     0,     0,   399,   400,     0,   401,
     402,   403,     0,   404,   405,   406,   856,     0,   408,   409,
     410,     0,   149,   412,   413,   414,   415,   149,     0,   418,
       0,   903,   419,   420,   421,     0,     0,  -296,     0,     0,
       0,  -296,  -296,   422,     0,   137,  -296,     0,     0,     0,
       0,  -296,     0,  -296,  -296,   165,     0,     0,     0,  -296,
     137,  1217,     0,     0,     0,   165,  -296,   172,   227,  -296,
       0,   172,     0,   160,     0,     0,   160,     0,   165,   172,
       0,   165,   228,   229,     0,   230,     0,   395,  -296,     0,
     231,  -296,     0,  -296,     0,  -296,     0,  -296,  -296,   232,
    -296,  1217,  -296,     0,  -296,   233,     0,   149,     0,     0,
       0,   234,     0,     0,     0,   235,   395,     0,   236,     0,
       0,     0,     0,  -296,     0,   160,  -296,     0,   237,  -296,
     165,   160,     0,     0,   238,   239,     0,     0,     0,     0,
       0,     0,   240,   396,     0,     0,     0,   397,     0,     0,
       0,   241,     0,     0,     0,     0,     0,   165,     0,     0,
     242,   243,   165,   244,   137,   245,     0,   246,     0,     0,
     247,     0,   396,     0,   248,   522,   397,   249,     0,  -296,
     250,     0,     0,     0,     0,  -296,     0,     0,     0,     0,
     137,   149,     0,     0,   399,   400,   137,     0,   149,   403,
       0,   404,   405,     0,     0,     0,   408,   137,     0,   137,
       0,     0,     0,     0,   415,  1031,  1032,     0,     0,     0,
     419,   420,   421,   399,   400,     0,   401,   402,   403,     0,
     404,   405,     0,     0,     0,   408,   523,   160,     0,   160,
       0,     0,   165,   415,   160,     0,     0,   160,     0,   419,
     420,   421,   149,     0,     0,     0,   160,   227,   160,     0,
       0,   160,   172,     0,     0,   172,   169,   149,     0,   395,
       0,   228,   229,     0,   230,     0,   169,     0,   137,   231,
       0,     0,   137,     0,   508,   160,     0,     0,   232,   169,
     137,     0,   169,     0,   233,   160,     0,   160,     0,     0,
     234,     0,     0,     0,   235,   509,     0,   236,     0,   510,
       0,     0,     0,     0,   172,     0,     0,   237,     0,     0,
     172,     0,     0,   238,   239,   396,   165,     0,     0,   397,
       0,   240,   511,   165,     0,   512,     0,   347,     0,     0,
     241,   169,     0,     0,     0,     0,   513,     0,     0,   242,
     243,     0,   244,     0,   245,     0,   246,   160,     0,   247,
       0,     0,     0,   248,   160,   514,   249,     0,   169,   250,
       0,   149,     0,   169,     0,     0,   399,   160,     0,     0,
       0,   403,     0,   404,   405,     0,   508,   165,   408,     0,
       0,     0,     0,     0,     0,     0,   415,   149,     0,     0,
       0,     0,   165,   149,   421,     0,     0,   509,   160,     0,
       0,   510,     0,     0,   149,     0,   149,   160,     0,     0,
       0,   160,     0,   453,     0,   979,   172,   462,   172,     0,
       0,    22,    23,   172,   511,     0,   172,   512,     0,    61,
       0,   463,     0,    31,   464,   172,     0,   172,   513,    37,
     172,     0,     0,   169,     0,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   160,   514,     0,     0,
       0,     0,     0,   137,   172,     0,   137,     0,     0,     0,
       0,    57,     0,    59,   172,   149,   172,  1009,     0,   149,
    1010,     0,   465,     0,    67,     0,     0,   149,   462,     0,
       0,     0,    22,    23,     0,     0,   165,  1008,     0,     0,
       0,     0,   463,    83,    31,   464,    85,     0,     0,    87,
      37,   160,   160,   160,   160,   137,     0,    42,     0,     0,
       0,   137,   165,     0,     0,     0,     0,   169,   165,     0,
       0,     0,     0,     0,   169,     0,   172,   160,   160,   165,
       0,   165,    57,   172,    59,     0,    61,     0,  1009,     0,
       0,  1010,     0,   465,     0,    67,   172,     0,     0,   102,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,     0,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,   172,   169,   170,
       0,     0,     0,     0,     0,     0,   172,     0,     0,   170,
     172,     0,     0,   169,     0,     0,     0,     0,     0,   678,
     165,     0,   170,     0,   165,   170,     0,     0,     0,     0,
       0,     0,   165,   892,     0,     0,   893,   137,     0,   137,
     102,   894,     0,     0,   137,     0,  1011,   137,     0,     0,
       0,     0,   195,     0,     0,   172,   137,     0,   137,     0,
       0,   137,   895,     0,     0,     0,     0,     0,     0,   896,
       0,     0,     0,     0,   170,     0,     0,     0,     0,   897,
     149,     0,     0,   149,     0,   137,   898,     0,     0,     0,
       0,     0,     0,     0,     0,   137,     0,   137,     0,     0,
       0,   170,   899,     0,     0,     0,   170,     0,     0,     0,
     172,   172,   172,   172,   900,     0,     0,   169,   682,     0,
       0,     0,     0,     0,     0,   901,     0,     0,     0,     0,
       0,   902,   149,     0,     0,     0,   172,   172,   149,     0,
       0,     0,     0,   169,     0,     0,     0,     0,     0,   169,
       0,     0,     0,     0,     0,     0,     0,   137,     0,     0,
     169,     0,   169,     0,   137,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   137,   174,     0,
       0,     0,     0,     0,     0,     0,   170,     0,   174,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   174,     0,     0,   174,     0,     0,     0,   137,     0,
       0,     0,     0,     0,     0,   165,     0,   137,   165,     0,
       0,   137,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   169,     0,     0,     0,   169,     0,     0,     0,     0,
       0,     0,     0,   169,   149,     0,   149,     0,     0,     0,
       0,   149,     0,   174,   149,     0,     0,     0,     0,     0,
       0,     0,     0,   149,     0,   149,   137,   165,   149,     0,
     170,     0,     0,   165,   462,     0,     0,   170,    22,    23,
     174,     0,     0,     0,     0,   174,     0,     0,   463,     0,
      31,   464,   149,     0,     0,   227,    37,     0,     0,     0,
       0,     0,   149,    42,   149,     0,     0,     0,     0,   228,
     229,     0,   230,     0,     0,     0,     0,   231,     0,     0,
       0,   137,   137,   137,   137,     0,   232,     0,    57,     0,
      59,   170,   233,     0,     0,     0,     0,     0,   234,   465,
       0,    67,   235,     0,     0,   236,   170,   137,   137,     0,
       0,     0,     0,     0,     0,   237,     0,     0,     0,     0,
      83,   238,   239,    85,   149,   174,    87,     0,     0,   240,
       0,   149,     0,     0,     0,     0,     0,     0,   241,   165,
       0,   165,     0,     0,   149,     0,   165,   242,   243,   165,
     244,     0,   245,     0,   246,     0,     0,   247,   165,     0,
     165,   248,     0,   165,   249,     0,     0,   250,     0,     0,
       0,     0,     0,     0,     0,   149,     0,     0,     0,     0,
       0,     0,     0,     0,   149,     0,   169,   165,   149,   169,
       0,     0,     0,     0,   110,     0,     0,   165,     0,   165,
       0,     0,     0,     0,   186,     0,     0,     0,     0,   174,
     170,     0,     0,     0,     0,     0,   174,   220,     0,     0,
     225,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   149,     0,     0,   170,     0,   169,     0,
       0,     0,   170,     0,   169,     0,     0,     0,     0,     0,
       0,     0,     0,   170,     0,   170,     0,   175,     0,   165,
       0,     0,     0,     0,     0,     0,   165,   175,     0,   312,
     174,     0,     0,     0,     0,     0,     0,     0,     0,   165,
     175,     0,     0,   175,     0,   174,     0,     0,   149,   149,
     149,   149,     0,     0,     0,     0,   110,     0,     0,     0,
       0,   348,     0,     0,     0,     0,     0,     0,     0,     0,
     165,     0,     0,     0,   149,   149,     0,     0,     0,   165,
       0,     0,     0,   165,   170,     0,     0,     0,   170,     0,
       0,     0,   175,     0,     0,     0,   170,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     169,   281,   169,     0,     0,     0,     0,   169,     0,   175,
     169,     0,     0,     0,   175,     0,     0,     0,   165,   169,
       0,   169,     0,   281,   169,     0,     0,     0,     0,     0,
       0,   445,     0,   315,   319,     0,     0,     0,     0,   174,
       0,     0,     0,     0,     0,     0,     0,     0,   169,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   169,     0,
     169,     0,     0,   462,     0,   174,     0,    22,    23,     0,
       0,   174,     0,   165,   165,   165,   165,   463,     0,    31,
     464,     0,   174,     0,   174,    37,     0,     0,     0,     0,
       0,     0,    42,     0,   175,     0,     0,     0,     0,   165,
     165,     0,     0,     0,     0,     0,   -53,     0,     0,     0,
       0,     0,     0,     0,     0,   507,     0,    57,     0,    59,
     169,     0,   532,    63,     0,   -53,    64,   169,   465,     0,
      67,     0,     0,     0,     0,     0,     0,   395,     0,     0,
     169,     0,     0,     0,     0,     0,     0,     0,     0,    83,
       0,     0,    85,   174,     0,    87,     0,   174,     0,     0,
       0,     0,     0,     0,     0,   174,   454,     0,     0,   170,
       0,   169,   170,     0,     0,     0,   110,     0,   175,     0,
     169,     0,     0,     0,   169,   175,     0,     0,     0,     0,
       0,   110,     0,   396,     0,     0,     0,   397,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,     0,   281,
     281,   281,   281,   281,   281,     0,     0,   497,   500,     0,
       0,   170,     0,     0,   506,     0,     0,   170,     0,   169,
       0,     0,     0,     0,     0,   281,     0,     0,     0,   175,
       0,   281,     0,     0,   399,   400,   251,   401,   402,   403,
       0,   404,   405,   406,   175,     0,   408,     0,     0,   281,
     271,   272,   273,   414,   415,     0,     0,   418,     0,     0,
     419,   420,   421,     0,   462,   281,   281,     0,    22,    23,
       0,   422,     0,  1008,   169,   169,   169,   169,   463,     0,
      31,   464,     0,     0,     0,   110,    37,     0,     0,     0,
       0,     0,     0,    42,     0,     0,     0,     0,     0,     0,
     169,   169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   532,     0,     0,     0,     0,     0,   532,    57,     0,
      59,     0,    61,   170,  1009,   170,     0,  1010,   761,   465,
     170,    67,     0,   170,     0,     0,     0,     0,   174,     0,
       0,   174,   170,     0,   170,     0,     0,   170,   175,     0,
      83,     0,   364,    85,   367,     0,    87,     0,     0,     0,
       0,     0,     0,   385,     0,     0,     0,     0,     0,     0,
       0,   170,     0,     0,   175,     0,     0,     0,     0,     0,
     175,   170,     0,   170,     0,     0,     0,     0,     0,     0,
     174,   175,     0,   175,     0,     0,   174,     0,   281,   813,
       0,     0,     0,   818,     0,   450,   102,     0,     0,     0,
       0,   110,  1124,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   281,     0,     0,     0,
     281,     0,   281,     0,     0,   281,     0,     0,   393,     0,
       0,     0,     0,   170,     0,   394,     0,     0,     0,     0,
     170,     0,     0,     0,     0,     0,     0,     0,   395,   936,
       0,   476,   175,   170,     0,     0,   175,     0,     0,   494,
     495,     0,     0,     0,   175,     0,     0,     0,     0,     0,
       0,     0,   524,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   170,     0,     0,     0,     0,     0,
       0,     0,   174,   170,   174,     0,     0,   170,     0,   174,
       0,     0,   174,     0,   396,     0,     0,     0,   397,     0,
       0,   174,     0,   174,     0,     0,   174,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   482,     0,     0,     0,   398,
     174,     0,   170,     0,     0,     0,     0,     0,     0,     0,
     174,     0,   174,     0,     0,   399,   400,   593,   401,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,     0,
     652,   419,   420,   421,   946,   453,     0,   949,     0,     0,
       0,     0,   422,     0,     0,     0,     0,   170,   170,   170,
     170,     0,     0,     0,   462,     0,     0,     0,    22,    23,
       0,     0,   174,  1008,     0,     0,     0,     0,   463,   174,
      31,   464,     0,   170,   170,     0,    37,     0,     0,     0,
       0,     0,   174,    42,     0,     0,   983,     0,     0,     0,
       0,     0,   988,     0,     0,     0,     0,   175,   731,     0,
     175,     0,     0,     0,     0,     0,     0,     0,    57,   251,
      59,     0,    61,   174,  1009,     0,     0,  1010,     0,   465,
       0,    67,   174,     0,     0,     0,   174,     0,   281,   281,
       0,     0,     0,     0,   281,   281,     0,     0,   281,   281,
      83,     0,     0,    85,     0,     0,    87,     0,   951,   175,
       0,   770,     0,     0,     0,   175,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   779,     0,     0,     0,   524,
       0,   174,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   980,     0,     0,
       0,     0,     0,     0,     0,     0,   102,     0,   532,     0,
     532,     0,  1195,     0,     0,   532,     0,     0,   532,     0,
       0,     0,     0,     0,     0,     0,     0,  1062,     0,  1063,
       0,     0,  1065,     0,     0,     0,   174,   174,   174,   174,
       0,   835,   395,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   367,     0,  1098,     0,     0,     0,
       0,     0,   174,   174,     0,     0,  1104,     0,  1108,     0,
       0,   175,     0,   175,     0,     0,     0,     0,   175,     0,
       0,   175,     0,     0,     0,     0,     0,     0,     0,     0,
     175,     0,   175,     0,     0,   175,     0,     0,   396,     0,
       0,     0,   397,     0,     0,     0,     0,     0,   281,   281,
       0,     0,     0,     0,     0,   281,     0,   500,   385,   175,
       0,     0,     0,     0,   500,     0,     0,   281,     0,   175,
     281,   175,   281,   398,   281,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1070,  1156,   399,
     400,     0,   401,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,   410,     0,     0,   412,   413,   414,   415,
     416,   417,   418,     0,  1102,   419,   420,   421,     0,  1179,
       0,     0,     0,     0,     0,     0,   422,     0,  1184,     0,
       0,   175,  1186,     0,     0,     0,     0,     0,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,   175,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   193,     0,   952,    15,    16,     0,    17,     0,   194,
       0,   956,    21,     0,   524,     0,     0,  1214,   524,     0,
       0,    29,   175,   195,     0,     0,     0,    33,   196,   197,
       0,   175,   198,    39,     0,   175,     0,    41,     0,     0,
      43,   981,     0,   199,     0,     0,    47,    48,  -471,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,   281,     0,    65,   200,     0,     0,     0,  -471,     0,
     175,     0,    72,    73,    74,    75,    76,   201,     0,    78,
       0,  -471,    80,     0,   367,     0,    84,   367,     0,    86,
       0,     0,    88,     0,  -471,     0,     0,     0,   652,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   500,   500,     0,     0,   500,   500,     0,    94,     0,
       0,     0,     0,     0,     0,   175,   175,   175,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     101,  -471,     0,  1235,     0,   500,  -471,   500,   105,     0,
     862,   175,   175,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    -2,     4,
       0,     5,     0,     6,     7,     8,     9,    10,    11,     0,
     770,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,   981,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -53,     0,    40,    41,    42,
       0,    43,  -314,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -53,    52,    53,     0,    54,    55,    56,
       0,  -314,     0,     0,    57,    58,    59,    60,    61,    62,
      63,  -314,   -53,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -53,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,     0,   105,
       0,   106,     0,     4,   107,     5,     0,     6,     7,     8,
       9,    10,    11,     0,  -662,     0,    12,    13,    14,    15,
      16,  -662,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,  -662,    28,    29,  -662,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -53,
       0,    40,    41,    42,     0,    43,  -314,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -53,    52,    53,
       0,    54,    55,    56,     0,  -314,     0,     0,    57,    58,
      59,    60,     0,    62,    63,  -314,   -53,    64,    65,    66,
    -662,    67,    68,    69,  -662,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,     0,    80,    81,    82,
      83,    84,     0,    85,    86,   -53,    87,    88,     0,     0,
      89,     0,    90,     0,     0,  -662,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -662,  -662,    94,  -662,  -662,  -662,  -662,  -662,  -662,
    -662,     0,  -662,  -662,  -662,  -662,  -662,     0,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,   102,  -662,  -662,  -662,
       0,   104,  -662,   105,     0,   106,     0,   331,  -662,     5,
     296,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -53,     0,    40,    41,    42,     0,    43,
    -314,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -53,    52,    53,     0,    54,    55,    56,     0,  -314,
       0,     0,    57,    58,    59,    60,    61,    62,    63,  -314,
     -53,    64,    65,    66,     0,    67,    68,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
       0,    80,    81,    82,    83,    84,     0,    85,    86,   -53,
      87,    88,     0,     0,    89,     0,    90,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
     102,     0,     0,   103,     0,   104,   332,   105,     0,   106,
       0,     4,   107,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -53,     0,    40,
      41,    42,     0,    43,  -314,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -53,    52,    53,     0,    54,
      55,    56,     0,  -314,     0,     0,    57,    58,    59,    60,
      61,    62,    63,  -314,   -53,    64,    65,    66,     0,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -53,    87,    88,     0,     0,    89,     0,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
     558,   105,     0,   106,     0,   577,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -53,     0,    40,    41,    42,     0,    43,  -314,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -53,
      52,    53,     0,    54,    55,    56,     0,  -314,     0,     0,
      57,    58,    59,    60,    61,    62,    63,  -314,   -53,    64,
      65,    66,     0,    67,    68,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -53,    87,    88,
       0,     0,    89,     0,    90,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,   102,     0,
       0,   103,     0,   104,   578,   105,     0,   106,     0,   331,
     107,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -53,     0,    40,    41,    42,
       0,    43,  -314,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -53,    52,    53,     0,    54,    55,    56,
       0,  -314,     0,     0,    57,    58,    59,    60,    61,    62,
      63,  -314,   -53,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -53,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,   332,   105,
       0,   106,     0,     4,   107,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -53,
       0,    40,    41,    42,     0,    43,  -314,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -53,    52,    53,
       0,    54,    55,    56,     0,  -314,     0,     0,    57,    58,
      59,    60,    61,    62,    63,  -314,   -53,    64,    65,    66,
       0,    67,    68,    69,     0,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,     0,    80,    81,    82,
      83,    84,     0,    85,    86,   -53,    87,    88,     0,     0,
      89,     0,    90,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,   102,     0,     0,   103,
       0,   104,   825,   105,     0,   106,     0,     4,   107,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -53,     0,    40,    41,    42,     0,    43,
    -314,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -53,    52,    53,     0,    54,    55,    56,     0,  -314,
       0,     0,    57,    58,    59,    60,   347,    62,    63,  -314,
     -53,    64,    65,    66,     0,    67,    68,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
       0,    80,    81,    82,    83,    84,     0,    85,    86,   -53,
      87,    88,     0,     0,    89,     0,    90,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
     102,     0,     0,   103,     0,   104,     0,   105,     0,   106,
       0,     4,   107,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -53,     0,    40,
      41,    42,     0,    43,  -314,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -53,    52,    53,     0,    54,
      55,    56,     0,  -314,     0,     0,    57,    58,    59,    60,
       0,    62,    63,  -314,   -53,    64,    65,    66,     0,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -53,    87,    88,     0,     0,    89,     0,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
       0,   105,     0,   106,     0,     4,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,   744,
      39,   -53,     0,    40,    41,    42,     0,    43,  -314,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -53,
      52,    53,     0,    54,    55,    56,     0,  -314,     0,     0,
      57,    58,    59,    60,     0,    62,    63,  -314,   -53,    64,
      65,    66,     0,    67,    68,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -53,    87,    88,
       0,     0,    89,     0,    90,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,   102,     0,
       0,   103,     0,   104,     0,   105,     0,   106,     0,     4,
     107,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,   750,    39,   -53,     0,    40,    41,    42,
       0,    43,  -314,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -53,    52,    53,     0,    54,    55,    56,
       0,  -314,     0,     0,    57,    58,    59,    60,     0,    62,
      63,  -314,   -53,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -53,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,     0,   105,
       0,   106,     0,     4,   107,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,  1047,    39,   -53,
       0,    40,    41,    42,     0,    43,  -314,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -53,    52,    53,
       0,    54,    55,    56,     0,  -314,     0,     0,    57,    58,
      59,    60,     0,    62,    63,  -314,   -53,    64,    65,    66,
       0,    67,    68,    69,     0,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,     0,    80,    81,    82,
      83,    84,     0,    85,    86,   -53,    87,    88,     0,     0,
      89,     0,    90,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,   102,     0,     0,   103,
       0,   104,     0,   105,     0,   106,     0,     4,   107,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,  1049,    39,   -53,     0,    40,    41,    42,     0,    43,
    -314,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -53,    52,    53,     0,    54,    55,    56,     0,  -314,
       0,     0,    57,    58,    59,    60,     0,    62,    63,  -314,
     -53,    64,    65,    66,     0,    67,    68,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
       0,    80,    81,    82,    83,    84,     0,    85,    86,   -53,
      87,    88,     0,     0,    89,     0,    90,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
     102,     0,     0,   103,     0,   104,     0,   105,     0,   106,
       0,     4,   107,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,  1054,    39,   -53,     0,    40,
      41,    42,     0,    43,  -314,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -53,    52,    53,     0,    54,
      55,    56,     0,  -314,     0,     0,    57,    58,    59,    60,
       0,    62,    63,  -314,   -53,    64,    65,    66,     0,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -53,    87,    88,     0,     0,    89,     0,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
       0,   105,     0,   106,     0,     4,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,  1057,
      39,   -53,     0,    40,    41,    42,     0,    43,  -314,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -53,
      52,    53,     0,    54,    55,    56,     0,  -314,     0,     0,
      57,    58,    59,    60,     0,    62,    63,  -314,   -53,    64,
      65,    66,     0,    67,    68,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -53,    87,    88,
       0,     0,    89,     0,    90,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,   102,     0,
       0,   103,     0,   104,     0,   105,     0,   106,     0,     4,
     107,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,  1097,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -53,     0,    40,    41,    42,
       0,    43,  -314,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -53,    52,    53,     0,    54,    55,    56,
       0,  -314,     0,     0,    57,    58,    59,    60,     0,    62,
      63,  -314,   -53,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -53,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,     0,   105,
       0,   106,     0,     4,   107,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,  1103,    39,   -53,
       0,    40,    41,    42,     0,    43,  -314,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -53,    52,    53,
       0,    54,    55,    56,     0,  -314,     0,     0,    57,    58,
      59,    60,     0,    62,    63,  -314,   -53,    64,    65,    66,
       0,    67,    68,    69,     0,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,     0,    80,    81,    82,
      83,    84,     0,    85,    86,   -53,    87,    88,     0,     0,
      89,     0,    90,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,   102,     0,     0,   103,
       0,   104,     0,   105,     0,   106,     0,     4,   107,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,  1107,    39,   -53,     0,    40,    41,    42,     0,    43,
    -314,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -53,    52,    53,     0,    54,    55,    56,     0,  -314,
       0,     0,    57,    58,    59,    60,     0,    62,    63,  -314,
     -53,    64,    65,    66,     0,    67,    68,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
       0,    80,    81,    82,    83,    84,     0,    85,    86,   -53,
      87,    88,     0,     0,    89,     0,    90,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
     102,     0,     0,   103,     0,   104,     0,   105,     0,   106,
       0,  1017,   107,     5,   296,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,     0,   203,  1018,   967,   107,     5,   296,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,   198,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   201,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,   203,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,   609,     0,
     193,     0,     0,    15,    16,   611,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   198,    39,     0,     0,     0,    41,     0,     0,    43,
       0,   617,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
     499,   424,   425,   426,   427,   428,     0,     0,   431,   432,
     433,   434,     0,   436,   437,   836,   837,   838,   839,   840,
     632,     0,   633,     0,    97,     0,   634,   635,   636,   637,
     638,   639,   640,   641,   841,   643,   644,    99,   842,   101,
       0,   646,   647,   843,   649,   202,     0,   105,     0,   203,
       0,     0,   107,     5,   296,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,   300,   301,     0,    84,
     336,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,   337,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,     0,   203,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,   354,    23,
       0,     0,     0,     0,     0,     0,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   198,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
      59,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,    67,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,    87,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,     0,   203,     0,     0,   107,     5,
     296,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   198,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
       0,    80,   300,   301,     0,    84,   336,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   202,     0,   105,   809,   203,
       0,     0,   107,     5,   296,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,   300,   301,     0,    84,
     336,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,   811,   203,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,     0,   714,
       0,     0,     0,     0,     0,     0,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   198,    39,     0,
     715,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,   716,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,   717,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,     0,   203,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,   198,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   201,     0,    78,     0,     0,    80,
     300,   301,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,   302,     0,
       0,   103,     0,   202,     0,   105,     0,   203,     0,     0,
     107,     5,   296,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,   300,   301,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   203,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,   969,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,   300,   301,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,     0,   203,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   198,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
     255,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,   256,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,     0,   203,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,   263,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,   198,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   201,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,   264,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,   203,     0,     0,
     107,     5,   296,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,   256,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   203,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,   300,   301,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,     0,   203,     0,     0,   107,     5,   296,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,   198,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   201,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,   807,   105,     0,   203,     0,     0,
     107,     5,   296,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   203,   817,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,  -706,     0,
       0,     0,  -706,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,     0,   203,     0,     0,   107,     5,   296,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,   198,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   201,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,   203,  1160,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   198,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,   256,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   202,     0,   105,     0,   203,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,   280,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   104,     0,   105,
       0,   203,     0,     0,   107,     5,   296,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   198,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,     0,   203,     0,     5,   107,     6,
       7,     8,   357,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,    18,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,   198,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,    66,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   201,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,    85,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,   203,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   198,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   202,     0,   105,     0,   203,
     822,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,   995,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   203,     0,     0,   107,     5,   296,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,   784,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   198,    39,     0,
       0,     0,   785,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,   786,    74,
      75,    76,   787,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,     0,  1073,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,   198,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   201,     0,    78,     0,     0,    80,
       0,     0,     0,    84,  1188,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,   203,     0,     0,
     107,     5,   296,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,  1073,     0,     0,   107,     5,   296,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,  1146,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,     0,   203,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,   198,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   201,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,   203,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   198,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   104,     0,   105,     0,   203,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,   784,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,   785,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,   786,    74,    75,    76,   787,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   788,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,   784,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,     0,     0,
     785,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,   786,    74,    75,    76,
     787,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,     0,   862,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   198,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,     0,   788,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,   906,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   201,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,   203,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   909,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   202,     0,   105,     0,   203,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,  1140,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   203,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,  1141,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,     0,   203,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,  1143,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,     0,   203,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,  1144,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   201,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,   203,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,  1145,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   202,     0,   105,     0,   203,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,  1146,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   203,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   607,     0,   608,     0,     0,     0,    92,
      93,    94,    95,    96,   609,     0,     0,     0,    97,   610,
     229,   611,   612,     0,     0,     0,     0,   613,    98,     0,
       0,    99,   100,   101,     0,     0,   232,   103,   195,     0,
       0,   105,   614,   862,     0,     0,   107,     0,   615,     0,
       0,     0,   235,     0,     0,   616,     0,   617,     0,     0,
       0,     0,     0,     0,     0,   618,     0,     0,     0,     0,
       0,   619,   620,     0,     0,     0,     0,     0,     0,   240,
       0,     0,     0,     0,     0,     0,     0,     0,   621,     0,
       0,     0,     0,     0,     0,     0,     0,   242,   243,     0,
     622,     0,   245,     0,   623,     0,     0,   624,     0,     0,
       0,   625,     0,     0,   249,     0,     0,   626,     0,     0,
       0,     0,     0,     0,     0,     0,   499,   424,   425,   426,
     427,   428,     0,     0,   431,   432,   433,   434,     0,   436,
     437,   627,   628,   629,   630,   631,   632,     0,   633,     0,
       0,     0,   634,   635,   636,   637,   638,   639,   640,   641,
     642,   643,   644,     0,   645,     0,     0,   646,   647,   648,
     649,     0,     5,   650,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   193,     0,     0,    15,    16,     0,    17,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   195,     0,     0,     0,    33,
     196,   197,     0,     0,   198,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   199,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   200,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   201,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,  -592,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
     438,   100,   101,  -631,     0,  -631,   103,     0,   202,     0,
     105,     0,   203,     5,   296,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,  -489,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,  -489,     0,     0,     0,  -489,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,   202,
       0,   105,  -489,  1073,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   193,     0,     0,    15,    16,
       0,    17,     0,   194,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   195,     0,     0,
       0,    33,   196,   197,     0,     0,   198,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   199,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   200,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   201,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,     0,     0,     0,     0,     0,     0,     0,
      92,     0,    94,     0,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   193,     0,     0,    15,    16,
       0,    17,   438,   194,   101,  -631,    21,  -631,     0,     0,
     202,     0,   105,     0,   203,    29,     0,   195,     0,     0,
       0,    33,   196,   197,     0,     0,   198,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   199,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   200,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   201,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   193,     0,     0,    15,    16,
       0,    17,     0,   194,   101,     0,    21,     0,     0,     0,
     202,     0,   105,     0,   788,    29,     0,   195,     0,     0,
       0,    33,   196,   197,     0,     0,   198,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   199,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   200,     0,
     227,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   201,     0,    78,   228,   229,    80,   230,     0,     0,
      84,     0,   231,    86,     0,     0,    88,     0,     0,     0,
       0,   232,     0,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   234,     0,     0,     0,   235,   678,     0,
     236,     0,    94,     0,     0,     0,     0,   609,     0,     0,
     237,     0,     0,     0,   611,   679,   238,   239,     0,     0,
       0,     0,     0,     0,   240,     0,     0,     0,     0,   680,
       0,     0,   105,   241,   203,     0,     0,     0,     0,     0,
       0,     0,   242,   243,     0,   244,     0,   245,     0,   246,
     617,     0,   247,     0,     0,     0,   248,     0,   681,   249,
       0,     0,   250,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   682,     0,     0,
       0,     0,     0,   683,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   778,   499,
     424,   425,   426,   427,   428,     0,     0,   431,   432,   433,
     434,     0,   436,   437,   627,   628,   629,   630,   631,   632,
       0,   633,     0,     0,     0,   634,   635,   636,   637,   638,
     639,   640,   641,   642,   643,   644,  1023,   645,     0,     0,
     646,   647,   648,   649,   992,   609,     0,     0,     0,     0,
     228,   229,   611,   230,     0,     0,     0,     0,   231,     0,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,     0,   614,     0,     0,     0,     0,     0,   234,
       0,     0,     0,   235,     0,     0,   236,     0,   617,     0,
       0,     0,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,   619,   239,     0,     0,     0,     0,     0,     0,
     240,     0,     0,     0,     0,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,   393,   242,   243,
       0,   244,     0,   245,   394,  1024,     0,     0,   624,     0,
       0,     0,   248,     0,     0,   249,     0,   395,   250,     0,
       0,     0,     0,     0,     0,     0,     0,   499,   424,   425,
     426,   427,   428,     0,     0,   431,   432,   433,   434,     0,
     436,   437,   627,   628,   629,   630,   631,   632,     0,   633,
       0,     0,     0,   634,   635,   636,   637,   638,   639,   640,
     641,   642,   643,   644,     0,   645,     0,     0,   646,   647,
     648,   649,     0,   396,     0,     0,     0,   397,     0,     0,
     393,     0,     0,     0,     0,     0,   498,   394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     395,     0,     0,     0,     0,     0,     0,     0,   398,   499,
     424,   425,   426,   427,   428,     0,     0,   431,   432,   433,
     434,     0,   436,   437,   399,   400,     0,   401,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,     0,     0,
     419,   420,   421,     0,   453,     0,   396,     0,     0,     0,
     397,   422,     0,   393,     0,     0,     0,     0,     0,  1046,
     394,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
       0,   398,   499,   424,   425,   426,   427,   428,     0,     0,
     431,   432,   433,   434,     0,   436,   437,   399,   400,     0,
     401,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,   410,   411,     0,   412,   413,   414,   415,   416,   417,
     418,     0,     0,   419,   420,   421,     0,   453,     0,   396,
       0,     0,     0,   397,   422,     0,   393,     0,     0,     0,
       0,     0,  1053,   394,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   395,     0,     0,     0,
       0,     0,     0,     0,   398,   499,   424,   425,   426,   427,
     428,     0,     0,   431,   432,   433,   434,     0,   436,   437,
     399,   400,     0,   401,   402,   403,     0,   404,   405,   406,
       0,   407,   408,   409,   410,   411,     0,   412,   413,   414,
     415,   416,   417,   418,     0,     0,   419,   420,   421,     0,
     453,     0,   396,     0,     0,     0,   397,   422,     0,   393,
       0,     0,     0,     0,     0,  1210,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
       0,     0,     0,     0,     0,     0,     0,   398,   499,   424,
     425,   426,   427,   428,     0,     0,   431,   432,   433,   434,
       0,   436,   437,   399,   400,     0,   401,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,     0,     0,   419,
     420,   421,     0,   453,     0,   396,     0,     0,     0,   397,
     422,     0,   393,     0,     0,     0,     0,     0,  1211,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,     0,     0,     0,     0,     0,     0,     0,
     398,   499,   424,   425,   426,   427,   428,     0,     0,   431,
     432,   433,   434,     0,   436,   437,   399,   400,     0,   401,
     402,   403,     0,   404,   405,   406,     0,   407,   408,   409,
     410,   411,     0,   412,   413,   414,   415,   416,   417,   418,
       0,     0,   419,   420,   421,     0,   453,     0,   396,     0,
       0,     0,   397,   422,     0,   393,     0,     0,     0,     0,
       0,  1212,   394,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,   398,   499,   424,   425,   426,   427,   428,
       0,     0,   431,   432,   433,   434,     0,   436,   437,   399,
     400,     0,   401,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,   410,   411,     0,   412,   413,   414,   415,
     416,   417,   418,     0,     0,   419,   420,   421,     0,   453,
       0,   396,     0,     0,     0,   397,   422,     0,   393,     0,
       0,     0,     0,     0,  1213,   394,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   395,     0,
       0,     0,     0,     0,     0,     0,   398,   499,   424,   425,
     426,   427,   428,     0,     0,   431,   432,   433,   434,     0,
     436,   437,   399,   400,     0,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,     0,     0,   419,   420,
     421,     0,   453,     0,   396,     0,     0,     0,   397,   422,
       0,   393,     0,     0,     0,     0,     0,  1236,   394,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,   398,
     499,   424,   425,   426,   427,   428,     0,     0,   431,   432,
     433,   434,     0,   436,   437,   399,   400,     0,   401,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,     0,
       0,   419,   420,   421,     0,   453,     0,   396,     0,     0,
       0,   397,   422,     0,   393,     0,     0,     0,     0,     0,
    1237,   394,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   395,   485,     0,     0,     0,     0,
       0,     0,   398,   499,   424,   425,   426,   427,   428,     0,
     486,   431,   432,   433,   434,     0,   436,   437,   399,   400,
       0,   401,   402,   403,     0,   404,   405,   406,     0,   407,
     408,   409,   410,   411,     0,   412,   413,   414,   415,   416,
     417,   418,     0,     0,   419,   420,   421,     0,   453,     0,
     396,     0,     0,     0,   397,   422,   393,     0,     0,     0,
       0,     0,     0,   394,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   395,   280,     0,     0,
       0,   482,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,   491,     0,     0,     0,     0,     0,     0,     0,
       0,   399,   400,     0,   401,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   417,   418,     0,     0,   419,   420,   421,
       0,   453,   396,     0,     0,     0,   397,     0,   422,   393,
       0,     0,     0,     0,     0,     0,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
     479,     0,     0,   482,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,     0,     0,   399,   400,     0,   401,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,     0,     0,   419,
     420,   421,     0,   453,     0,   396,     0,     0,     0,   397,
     422,     0,   393,     0,     0,     0,     0,     0,     0,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,     0,     0,     0,     0,   586,     0,     0,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,   400,     0,   401,
     402,   403,     0,   404,   405,   406,     0,   407,   408,   409,
     410,   411,   587,   412,   413,   414,   415,   416,   417,   418,
       0,     0,   419,   420,   421,     0,   453,     0,   396,     0,
       0,     0,   397,   422,   393,     0,     0,     0,     0,     0,
       0,   394,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   395,   280,     0,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
     400,     0,   401,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,   410,   411,     0,   412,   413,   414,   415,
     416,   417,   418,     0,   588,   419,   420,   421,     0,     0,
     396,     0,     0,     0,   397,     0,   422,   393,     0,     0,
       0,     0,     0,     0,   394,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   395,     0,     0,
       0,   482,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,   455,     0,     0,     0,     0,     0,     0,
       0,   399,   400,     0,   401,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   417,   418,     0,     0,   419,   420,   421,
       0,   453,     0,   396,     0,     0,     0,   397,   422,   393,
       0,     0,     0,     0,     0,     0,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
     280,     0,     0,     0,   188,     0,     0,     0,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,   400,     0,   401,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,     0,     0,
     419,   420,   421,     0,     0,   396,     0,     0,     0,   397,
       0,   422,   393,     0,     0,     0,     0,     0,     0,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,   723,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,     0,   724,     0,
       0,     0,     0,     0,     0,     0,   399,   400,     0,   401,
     402,   403,     0,   404,   405,   406,     0,   407,   408,   409,
     410,   411,     0,   412,   413,   414,   415,   416,   417,   418,
       0,     0,   419,   420,   421,     0,   453,     0,   396,     0,
       0,     0,   397,   422,   393,     0,     0,     0,     0,     0,
       0,   394,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   395,   725,     0,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
     726,     0,     0,     0,     0,     0,     0,     0,     0,   399,
     400,     0,   401,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,   410,   411,   395,   412,   413,   414,   415,
     416,   417,   418,     0,     0,   419,   420,   421,     0,     0,
     396,     0,     0,     0,   397,   393,   422,     0,     0,     0,
       0,     0,   394,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   395,   928,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,   396,     0,     0,     0,   397,     0,     0,     0,     0,
       0,   399,   400,     0,   401,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   417,   418,     0,   398,   419,   420,   421,
       0,   396,     0,     0,     0,   397,     0,   393,   422,     0,
       0,     0,   399,   400,   394,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   395,     0,   412,
     413,   414,   415,   416,     0,   418,   398,     0,   419,   420,
     421,     0,     0,     0,     0,     0,     0,     0,     0,   422,
       0,     0,   399,   400,     0,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,     0,     0,   419,   420,
     421,     0,   453,   396,     0,     0,     0,   397,     0,   422,
     393,     0,     0,     0,     0,     0,   950,   394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     395,     0,     0,     0,     0,     0,     0,     0,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,   400,     0,   401,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,     0,     0,
     419,   420,   421,     0,   453,     0,   396,     0,     0,     0,
     397,   422,     0,   393,     0,     0,     0,     0,     0,     0,
     394,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   395,     0,     0,     0,   482,  1116,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   400,     0,
     401,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,   410,   411,  1117,   412,   413,   414,   415,   416,   417,
     418,     0,     0,   419,   420,   421,     0,     0,     0,   396,
       0,     0,   986,   397,   422,   393,     0,     0,     0,     0,
       0,     0,   394,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,   400,     0,   401,   402,   403,     0,   404,   405,   406,
       0,   407,   408,   409,   410,   411,     0,   412,   413,   414,
     415,   416,   417,   418,     0,     0,   419,   420,   421,     0,
       0,   396,     0,     0,     0,   397,     0,   422,   393,     0,
       0,     0,     0,     0,     0,   394,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   395,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,   400,     0,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,     0,     0,   419,   420,
     421,     0,   545,     0,   396,     0,     0,     0,   397,   422,
     393,     0,     0,     0,     0,     0,     0,   394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     395,     0,     0,     0,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   399,   400,     0,   401,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,     0,
     549,   419,   420,   421,     0,     0,   396,     0,     0,     0,
     397,   393,   422,     0,     0,     0,     0,     0,   394,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   400,     0,
     401,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,   410,   411,     0,   412,   413,   414,   415,   416,   417,
     418,     0,   551,   419,   420,   421,     0,   396,     0,     0,
       0,   397,   393,     0,   422,     0,     0,     0,     0,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,   400,
       0,   401,   402,   403,     0,   404,   405,   406,     0,   407,
     408,   409,   410,   411,     0,   412,   413,   414,   415,   416,
     417,   418,     0,   553,   419,   420,   421,     0,   396,     0,
       0,     0,   397,   393,     0,   422,     0,     0,     0,     0,
     394,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
     400,     0,   401,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,   410,   411,     0,   412,   413,   414,   415,
     416,   417,   418,     0,   556,   419,   420,   421,     0,   396,
       0,     0,     0,   397,   393,     0,   422,     0,     0,     0,
       0,   394,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   395,     0,     0,     0,     0,     0,
       0,     0,     0,   573,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,   400,     0,   401,   402,   403,     0,   404,   405,   406,
       0,   407,   408,   409,   410,   411,     0,   412,   413,   414,
     415,   416,   417,   418,     0,     0,   419,   420,   421,     0,
     396,     0,     0,     0,   397,   393,     0,   422,     0,     0,
       0,     0,   394,   727,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   399,   400,     0,   401,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   417,   418,     0,     0,   419,   420,   421,
       0,   396,     0,     0,     0,   397,   393,   828,   422,     0,
       0,     0,     0,   394,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   395,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,   400,     0,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,     0,     0,   419,   420,
     421,     0,   396,     0,     0,   808,   397,   393,     0,   422,
       0,     0,     0,     0,   394,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   395,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   400,     0,   401,   402,   403,     0,
     404,   829,   406,     0,   407,   408,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,     0,     0,   419,
     420,   421,     0,   396,     0,     0,     0,   397,   393,     0,
     422,     0,     0,     0,     0,   394,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   395,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,   400,     0,   401,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,   410,   411,
    -593,   412,   413,   414,   415,   416,   417,   418,     0,     0,
     419,   420,   421,     0,   396,     0,     0,     0,   397,   393,
       0,   422,     0,     0,     0,     0,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
       0,     0,     0,     0,     0,   188,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   399,   400,     0,   401,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,     0,
       0,   419,   420,   421,     0,   396,     0,     0,     0,   397,
     393,     0,   422,     0,     0,     0,     0,   394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     395,     0,     0,   948,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,   400,     0,   401,
     402,   403,     0,   404,   405,   406,     0,   407,   408,   409,
     410,   411,     0,   412,   413,   414,   415,   416,   417,   418,
       0,   919,   419,   420,   421,     0,   396,     0,     0,     0,
     397,   393,     0,   422,     0,     0,     0,     0,   394,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   400,     0,
     401,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,   410,   411,     0,   412,   413,   414,   415,   416,   417,
     418,     0,     0,   419,   420,   421,     0,   396,     0,     0,
       0,   397,   393,     0,   422,     0,     0,     0,     0,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,   400,
       0,   401,   402,   403,     0,   404,   405,   406,     0,   407,
     408,   409,   410,   411,     0,   412,   413,   414,   415,   416,
     417,   418,     0,     0,   419,   420,   421,     0,   396,     0,
       0,   971,   397,   393,     0,   422,     0,     0,     0,     0,
     394,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
     400,     0,   401,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,   410,   411,     0,   412,   413,   414,   415,
     416,   417,   418,     0,     0,   419,   420,   421,     0,   396,
       0,     0,   972,   397,   393,  1001,   422,     0,     0,     0,
       0,   394,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   395,     0,     0,     0,     0,     0,
       0,     0,     0,   989,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,   400,     0,   401,   402,   403,     0,   404,   405,   406,
       0,   407,   408,   409,   410,   411,     0,   412,   413,   414,
     415,   416,   417,   418,     0,     0,   419,   420,   421,     0,
     396,     0,     0,     0,   397,   393,     0,   422,     0,     0,
       0,     0,   394,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   399,   400,     0,   401,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   417,   418,     0,     0,   419,   420,   421,
       0,   396,     0,     0,     0,   397,   393,     0,   422,     0,
       0,     0,     0,   394,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   395,  1035,     0,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,   400,     0,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,     0,     0,   419,   420,
     421,     0,   396,     0,     0,  1022,   397,   393,     0,   422,
       0,     0,     0,     0,   394,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   395,  1038,     0,
       0,     0,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   400,     0,   401,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,     0,     0,   419,
     420,   421,     0,   396,     0,     0,     0,   397,   393,     0,
     422,     0,     0,     0,     0,   394,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   395,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,   400,     0,   401,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,     0,     0,
     419,   420,   421,     0,   396,     0,     0,     0,   397,   393,
    1113,   422,     0,     0,     0,     0,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   399,   400,     0,   401,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,     0,
       0,   419,   420,   421,     0,   396,     0,     0,     0,   397,
    1041,   393,   422,     0,     0,     0,     0,     0,   394,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,   400,     0,   401,
     402,   403,     0,   404,   405,   406,     0,   407,   408,   409,
     410,   411,     0,   412,   413,   414,   415,   416,   417,   418,
       0,     0,   419,   420,   421,     0,     0,   396,     0,     0,
       0,   397,   393,   422,     0,     0,     0,     0,     0,   394,
    1139,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,   400,
       0,   401,   402,   403,     0,   404,   405,   406,     0,   407,
     408,   409,   410,   411,     0,   412,   413,   414,   415,   416,
     417,   418,     0,     0,   419,   420,   421,     0,   396,     0,
       0,     0,   397,     0,     0,   422,     0,     0,     0,     0,
       0,  1142,     0,     0,     0,     0,   393,     0,     0,     0,
       0,     0,     0,   394,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   398,     0,     0,   395,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
     400,     0,   401,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,   410,   411,  1173,   412,   413,   414,   415,
     416,   417,   418,   393,  1194,   419,   420,   421,     0,     0,
     394,     0,     0,     0,     0,     0,   422,     0,     0,     0,
       0,     0,   396,   395,     0,     0,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   393,     0,     0,     0,     0,     0,     0,
     394,     0,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   400,     0,   401,   402,   403,   396,
     404,   405,   406,   397,   407,   408,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,     0,     0,   419,
     420,   421,     0,     0,     0,   395,     0,     0,     0,     0,
     422,     0,     0,     0,   398,     0,     0,     0,     0,   396,
       0,     0,     0,   397,     0,     0,     0,     0,     0,     0,
     399,   400,     0,   401,   402,   403,     0,   404,   405,   406,
       0,   407,   408,   409,   410,   411,     0,   412,   413,   414,
     415,   416,   417,   418,   398,     0,   419,   420,   421,     0,
       0,   396,     0,     0,     0,   397,     0,   422,     0,     0,
     399,   400,     0,   401,   402,   403,     0,   404,   405,   406,
       0,   407,   408,   409,   410,   411,     0,   412,   413,   414,
     415,   416,   417,   418,   393,     0,   419,   420,   421,     0,
       0,   394,     0,     0,     0,  1232,     0,   422,     0,     0,
       0,     0,   399,   400,   395,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,     0,     0,   412,
     413,   414,   415,   416,   393,   418,     0,     0,   419,   420,
     421,   394,     0,     0,     0,     0,     0,     0,     0,   422,
       0,     0,     0,     0,   395,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,   397,     0,     0,     0,     0,     0,
       0,     0,     0,  1244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
     396,     0,     0,     0,   397,     0,     0,     0,     0,     0,
       0,   399,   400,  1245,   401,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   417,   418,   398,     0,   419,   420,   421,
       0,     0,     0,     0,     0,     0,     0,     0,   422,     0,
       0,   399,   400,     0,   401,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   417,   418,   393,   462,   419,   420,   421,
      22,    23,   394,     0,     0,  1008,     0,     0,   422,     0,
     463,     0,    31,   464,     0,   395,     0,     0,    37,     0,
       0,     0,     0,     0,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   393,     0,     0,     0,     0,
       0,     0,   394,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,   347,   395,  1009,     0,     0,  1010,
       0,   465,     0,    67,     0,     0,     0,     0,     0,     0,
       0,   396,     0,     0,     0,   397,     0,     0,     0,     0,
       0,     0,    83,     0,  1246,    85,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,     0,
       0,   396,     0,     0,     0,   397,     0,     0,     0,     0,
       0,     0,   399,   400,  1247,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   411,   102,   412,
     413,   414,   415,   416,   417,   418,   398,     0,   419,   420,
     421,     0,     0,     0,     0,     0,     0,     0,     0,   422,
       0,     0,   399,   400,     0,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,   393,   227,   419,   420,
     421,     0,     0,   394,     0,     0,     0,     0,     0,   422,
       0,   228,   229,     0,   230,     0,   395,     0,     0,   231,
       0,     0,     0,     0,     0,   366,     0,     0,   232,     0,
       0,     0,     0,     0,   233,     0,   393,     0,     0,     0,
     234,     0,     0,   394,   235,     0,     0,   236,     0,     0,
       0,     0,     0,     0,     0,     0,   395,   237,     0,     0,
       0,     0,     0,   238,   239,     0,     0,     0,     0,     0,
       0,   240,   396,     0,     0,     0,   397,   393,     0,     0,
     241,     0,     0,     0,   394,  1248,     0,     0,     0,   242,
     243,     0,   244,     0,   245,     0,   246,   395,     0,   247,
       0,     0,     0,   248,     0,     0,   249,   398,     0,   250,
       0,     0,   396,     0,     0,     0,   397,     0,     0,     0,
       0,     0,     0,   399,   400,  1249,   401,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,   398,     0,   419,
     420,   421,     0,   396,     0,     0,     0,   397,     0,     0,
     422,     0,     0,   399,   400,     0,   401,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,     0,   398,   419,
     420,   421,     0,     0,   393,     0,     0,     0,     0,     0,
     422,   394,     0,     0,   399,   400,     0,   401,   402,   403,
       0,   404,   405,   406,   395,   407,   408,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,     0,     0,
     419,   420,   421,     0,     0,   393,     0,     0,     0,  1262,
       0,   422,   394,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,   397,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,   904,     0,     0,     0,   397,     0,     0,     0,     0,
       0,   399,   400,     0,   401,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   417,   418,     0,   398,   419,   420,   421,
       0,     0,     0,     0,     0,     0,     0,     0,   422,     0,
       0,     0,   399,   400,     0,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,     0,     0,   419,   420,
     421,   365,     0,   227,     0,     0,     0,     0,     0,   422,
       0,     0,     0,     0,     0,     0,     0,   228,   229,     0,
     230,     0,     0,     0,     0,   231,     0,     0,     0,     0,
       0,   366,     0,     0,   232,     0,     0,  -291,     0,     0,
     233,     0,     0,     0,     0,     0,   234,     0,     0,     0,
     235,  -291,  -291,   236,  -291,     0,     0,     0,     0,  -291,
       0,     0,     0,   237,     0,     0,     0,     0,  -291,   238,
     239,     0,     0,     0,  -291,     0,     0,   240,     0,     0,
    -291,     0,     0,     0,  -291,     0,   241,  -291,     0,     0,
       0,     0,     0,     0,     0,   242,   243,  -291,   244,     0,
     245,     0,   246,  -291,  -291,   247,     0,     0,     0,   248,
       0,  -291,   249,     0,     0,   250,     0,     0,     0,     0,
    -291,     0,     0,   227,     0,     0,     0,     0,     0,  -291,
    -291,     0,  -291,     0,  -291,     0,  -291,   228,   229,  -291,
     230,     0,     0,  -291,     0,   231,  -291,     0,     0,  -291,
       0,     0,     0,     0,   232,     0,     0,  -292,     0,     0,
     233,     0,     0,     0,     0,     0,   234,     0,     0,     0,
     235,  -292,  -292,   236,  -292,     0,     0,     0,     0,  -292,
       0,     0,     0,   237,     0,     0,     0,     0,  -292,   238,
     239,     0,     0,     0,  -292,     0,     0,   240,     0,     0,
    -292,     0,     0,     0,  -292,     0,   241,  -292,     0,     0,
       0,     0,     0,     0,     0,   242,   243,  -292,   244,     0,
     245,     0,   246,  -292,  -292,   247,     0,     0,     0,   248,
       0,  -292,   249,     0,     0,   250,     0,     0,     0,     0,
    -292,     0,     0,     0,     0,     0,     0,     0,     0,  -292,
    -292,     0,  -292,     0,  -292,     0,  -292,     0,     0,  -292,
       0,     0,     0,  -292,     0,     0,  -292,     0,     0,  -292
};

static const yytype_int16 yycheck[] =
{
      12,   158,    68,   444,    16,     2,   270,   384,    20,   274,
      19,   384,    40,   659,    26,   528,   604,   748,   598,   217,
     523,   358,    34,    35,    36,   834,    38,   504,   224,   293,
     521,   131,   960,   172,    48,   547,   586,    49,   966,    51,
     106,   963,   963,   655,    56,   657,    58,   311,    60,     1,
       1,   829,   956,   226,    56,     1,    68,    69,    70,    71,
      72,    73,    34,    35,    36,    77,   852,    79,    56,    81,
      82,    33,     1,    33,    86,    69,  1071,    89,    90,     3,
      92,   857,    94,     3,  1018,    97,    98,   119,   100,    33,
      48,   604,   104,   105,   106,   107,    61,   270,    33,    33,
     124,  1160,  1030,     1,    11,   119,   119,  1222,   145,   145,
      48,   105,   260,   261,   262,    61,   145,   129,    37,   131,
     293,   145,   164,    74,    71,   149,   117,   266,   164,   171,
      48,   145,    61,     0,   106,   172,    48,   203,   311,    48,
      48,   170,   174,  1071,    48,    91,   659,  1262,   172,    27,
      48,    89,   117,    72,   145,     3,   168,   119,   104,   890,
     174,   174,    91,  1222,  1159,   116,   164,    68,  1072,   171,
      48,   117,   124,    56,    52,   119,   440,   441,   102,   318,
      89,   193,   102,   171,   196,   197,   198,   145,   117,   201,
     202,   203,   144,   145,   342,   343,   224,    75,   144,  1133,
      78,   108,    80,   104,   105,   106,   215,   169,    74,   169,
     145,    89,  1134,  1134,   226,   144,   174,  1003,   164,   170,
     172,   999,     3,   169,   119,   169,   124,   145,  1004,   741,
     108,  1159,   405,   145,   169,   169,  1012,   145,   124,    20,
     169,   145,     3,    91,    22,   384,   144,   145,    48,   145,
     116,   728,   589,    34,   102,    54,   174,   357,   270,   145,
      61,    61,   174,   149,    18,   461,   164,   440,   441,   145,
     174,    91,   145,    91,    73,   778,   172,   139,   858,   139,
     101,   293,    63,   833,    83,   147,   172,   147,   300,   301,
      91,    91,   783,    27,   172,    47,   362,   170,   174,   311,
     457,   202,   203,   104,   170,    56,  1115,   748,  1117,    87,
     257,   258,   259,   260,   261,   262,   117,   117,    81,    82,
    1051,   102,   164,    77,   124,   337,   145,   108,   170,   145,
     145,   504,    84,   111,   145,   145,   283,   334,   350,   145,
     352,    95,   289,   145,    78,   145,   164,  1123,   169,   169,
     362,   169,  1128,   172,   145,    89,   172,   172,   112,   170,
     307,   113,   172,   164,   164,  1082,   172,   139,   169,   169,
     172,   959,   172,   164,   547,   147,   323,   324,   855,   391,
     114,   393,   394,   395,   396,   397,   398,   108,   400,   401,
     402,   403,   404,   591,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   477,   903,  1025,  1026,    27,   164,   429,   430,   294,
     295,  1197,   170,   435,   396,   397,   438,   439,   440,   441,
     442,   443,   444,   461,   124,   429,   430,    32,   915,    27,
      56,   435,   992,   455,   438,   439,   959,   459,  1094,   890,
    1167,  1168,  1169,  1170,  1171,  1172,   468,   145,   145,   149,
     171,   362,   173,    47,   145,   477,    78,   479,   480,    82,
     442,   443,    85,   485,   486,   145,   145,    89,    88,   491,
      56,    65,    92,   455,   172,   172,   498,    12,    13,   501,
      78,   172,   504,    88,    19,   872,   108,    92,   124,   872,
      84,    89,   172,   172,     8,  1018,   145,    32,   480,   456,
      89,   776,    37,   145,   486,   145,   171,   145,   530,   491,
     108,    88,   534,   535,   150,    92,   152,   153,   540,   113,
     156,   157,    88,   172,   164,   547,    92,   484,    56,    56,
     172,   488,   170,   490,   139,     8,   493,    72,   560,   144,
      61,   563,   147,   565,   171,   728,   151,   569,   570,   571,
     145,   573,   574,    88,  1114,   145,  1116,    92,   741,   563,
     145,   565,   145,     2,   586,   587,   477,    56,   171,   164,
      91,  1094,   579,    12,   164,    56,    88,   599,   145,   164,
      92,   164,   857,   104,   145,    56,    25,   569,   123,    28,
     171,    56,   173,   868,   752,   753,   117,   164,   756,   757,
    1051,   171,    47,   164,   139,   140,    56,   142,   143,   144,
    1133,   146,   147,   148,   145,   150,   151,   152,   153,   154,
      65,   156,   157,   158,   159,   160,   161,   162,   650,   540,
     165,   166,   167,   164,    32,    99,   145,  1160,    77,    84,
      56,   176,   145,   164,    51,   828,   829,    54,   169,   560,
      56,   171,   563,   164,   565,   164,    56,   815,   816,   570,
     171,   164,   173,    88,    56,   104,    73,    92,   113,    76,
     109,    88,   855,   139,    56,    92,    83,   171,   144,   173,
      56,   147,    48,   169,   144,   151,   853,   171,   164,   173,
      88,   169,   164,   171,    92,    61,   171,   164,   173,  1222,
     163,   723,   724,   725,   726,   727,   728,   729,   730,  1232,
     164,   155,   788,   732,   733,   164,   171,   739,   173,   741,
     742,   164,   744,   872,   164,    91,   748,    56,   750,  1004,
     171,    32,   915,   164,   171,   739,   173,   759,   760,  1262,
     169,   139,   724,   164,   726,   563,   144,   565,   146,   147,
     189,   117,     2,   151,   118,   169,   171,    84,   124,   171,
     171,   159,    12,   171,   786,   787,   788,   165,   166,   167,
     792,   164,   171,     8,    65,    25,   925,     8,    28,   145,
     169,   124,   170,   932,    89,    89,   862,    88,   792,   746,
     747,    92,   124,    61,   164,   752,   753,   145,   164,   756,
     757,   147,    68,   169,   826,   108,   172,   124,   830,   171,
     832,   833,   834,   172,   836,   145,   838,   171,    48,   841,
     842,  1095,  1096,    91,   124,   124,    35,    77,   739,    35,
     172,    81,    82,   855,   273,   145,   104,    65,   139,   140,
     862,   280,   143,   144,   172,   146,   147,   124,  1123,   117,
     151,   172,   172,    21,   104,   174,   174,   169,   159,   109,
     145,   104,   171,   147,   165,   166,   167,   147,   890,   147,
     139,   147,    48,   147,   147,   139,   147,   788,   164,   139,
     147,   164,   904,   905,   906,    61,   147,   909,   147,   147,
     139,   139,   147,   915,   164,   334,   164,   172,    35,   975,
      35,   169,   164,   144,   926,   145,   928,   164,   164,   174,
     349,   933,  1095,  1096,   936,    91,    33,   171,   170,   172,
     164,   172,   904,   905,   169,    33,   948,   174,   170,    35,
     164,   164,   144,     8,   171,   174,   174,   139,   174,   189,
     962,   117,   171,   164,   139,   384,   172,   174,   124,   174,
      35,   862,   172,   975,   164,   172,   172,   164,   145,   916,
     917,   164,   984,     2,   986,   163,   923,   989,   349,   145,
     992,   163,   269,    12,    24,   826,  1012,   999,   935,  1001,
      89,   938,   319,   940,   979,   942,    25,   954,   164,    28,
     861,   360,   872,   169,   868,   776,   172,  1073,   515,  1134,
    1149,  1150,   867,   275,  1153,  1154,  1166,  1173,   662,   129,
     782,    -1,  1034,  1035,   453,  1037,  1038,    -1,  1040,  1041,
      -1,   531,    -1,   273,  1046,  1047,    -1,  1049,    -1,  1051,
     280,  1053,  1054,    -1,  1183,  1057,  1185,    -1,    77,    -1,
     479,    -1,    -1,    -1,    -1,    -1,   485,    -1,   959,    32,
      -1,  1073,    -1,    -1,    -1,    -1,  1078,   496,    -1,   498,
      -1,    -1,    -1,    -1,   975,   104,    -1,    -1,    -1,    -1,
     109,  1093,  1094,  1095,  1096,    -1,    -1,    -1,    -1,  1101,
      -1,  1103,    -1,    -1,   334,  1107,    -1,    -1,    -1,    -1,
      -1,  1113,  1114,  1115,  1116,  1117,    -1,    -1,    -1,   349,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    92,
      -1,  1133,    -1,    -1,    -1,    -1,    -1,  1139,  1140,  1141,
    1142,  1143,  1144,  1145,  1146,    -1,    -1,    -1,   567,    -1,
      -1,    -1,   571,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     579,    -1,  1099,  1165,    -1,    -1,    -1,    -1,    -1,    32,
     189,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,  1073,   146,   147,    -1,    -1,  1078,   151,    -1,
       2,    -1,  1194,    -1,    -1,   158,   159,    -1,    -1,   162,
      12,    -1,   165,   166,   167,    -1,    -1,    -1,  1210,  1211,
    1212,  1213,    -1,    25,    -1,    -1,    28,    -1,    -1,  1221,
    1222,    -1,    -1,   453,    -1,    88,    -1,    -1,    -1,    92,
    1232,    -1,    -1,    -1,  1236,  1237,    -1,    -1,    -1,    -1,
      -1,    -1,  1244,  1245,  1246,  1247,  1248,  1249,    -1,   479,
      -1,    -1,    -1,    -1,   273,   485,    -1,    -1,    -1,    -1,
    1262,   280,    -1,    -1,    -1,    77,   496,    -1,   498,  1160,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,     1,    -1,   151,   152,
     153,    -1,   104,   156,   157,   158,   159,   109,    -1,   162,
      -1,   720,   165,   166,   167,    -1,    -1,    22,    -1,    -1,
      -1,    26,    27,   176,    -1,   334,    31,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,     2,    -1,    -1,    -1,    44,
     349,  1222,    -1,    -1,    -1,    12,    51,   567,     3,    54,
      -1,   571,    -1,   762,    -1,    -1,   765,    -1,    25,   579,
      -1,    28,    17,    18,    -1,    20,    -1,    32,    73,    -1,
      25,    76,    -1,    78,    -1,    80,    -1,    82,    83,    34,
      85,  1262,    87,    -1,    89,    40,    -1,   189,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    32,    -1,    53,    -1,
      -1,    -1,    -1,   108,    -1,   814,   111,    -1,    63,   114,
      77,   820,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    88,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      95,    96,   109,    98,   453,   100,    -1,   102,    -1,    -1,
     105,    -1,    88,    -1,   109,   110,    92,   112,    -1,   164,
     115,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,
     479,   273,    -1,    -1,   139,   140,   485,    -1,   280,   144,
      -1,   146,   147,    -1,    -1,    -1,   151,   496,    -1,   498,
      -1,    -1,    -1,    -1,   159,   904,   905,    -1,    -1,    -1,
     165,   166,   167,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,    -1,    -1,    -1,   151,   171,   926,    -1,   928,
      -1,    -1,   189,   159,   933,    -1,    -1,   936,    -1,   165,
     166,   167,   334,    -1,    -1,    -1,   945,     3,   947,    -1,
      -1,   950,   762,    -1,    -1,   765,     2,   349,    -1,    32,
      -1,    17,    18,    -1,    20,    -1,    12,    -1,   567,    25,
      -1,    -1,   571,    -1,    27,   974,    -1,    -1,    34,    25,
     579,    -1,    28,    -1,    40,   984,    -1,   986,    -1,    -1,
      46,    -1,    -1,    -1,    50,    48,    -1,    53,    -1,    52,
      -1,    -1,    -1,    -1,   814,    -1,    -1,    63,    -1,    -1,
     820,    -1,    -1,    69,    70,    88,   273,    -1,    -1,    92,
      -1,    77,    75,   280,    -1,    78,    -1,    80,    -1,    -1,
      86,    77,    -1,    -1,    -1,    -1,    89,    -1,    -1,    95,
      96,    -1,    98,    -1,   100,    -1,   102,  1046,    -1,   105,
      -1,    -1,    -1,   109,  1053,   108,   112,    -1,   104,   115,
      -1,   453,    -1,   109,    -1,    -1,   139,  1066,    -1,    -1,
      -1,   144,    -1,   146,   147,    -1,    27,   334,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,   479,    -1,    -1,
      -1,    -1,   349,   485,   167,    -1,    -1,    48,  1097,    -1,
      -1,    52,    -1,    -1,   496,    -1,   498,  1106,    -1,    -1,
      -1,  1110,    -1,   169,    -1,   171,   926,    22,   928,    -1,
      -1,    26,    27,   933,    75,    -1,   936,    78,    -1,    80,
      -1,    36,    -1,    38,    39,   945,    -1,   947,    89,    44,
     950,    -1,    -1,   189,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1155,   108,    -1,    -1,
      -1,    -1,    -1,   762,   974,    -1,   765,    -1,    -1,    -1,
      -1,    76,    -1,    78,   984,   567,   986,    82,    -1,   571,
      85,    -1,    87,    -1,    89,    -1,    -1,   579,    22,    -1,
      -1,    -1,    26,    27,    -1,    -1,   453,    31,    -1,    -1,
      -1,    -1,    36,   108,    38,    39,   111,    -1,    -1,   114,
      44,  1210,  1211,  1212,  1213,   814,    -1,    51,    -1,    -1,
      -1,   820,   479,    -1,    -1,    -1,    -1,   273,   485,    -1,
      -1,    -1,    -1,    -1,   280,    -1,  1046,  1236,  1237,   496,
      -1,   498,    76,  1053,    78,    -1,    80,    -1,    82,    -1,
      -1,    85,    -1,    87,    -1,    89,  1066,    -1,    -1,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,  1097,   334,     2,
      -1,    -1,    -1,    -1,    -1,    -1,  1106,    -1,    -1,    12,
    1110,    -1,    -1,   349,    -1,    -1,    -1,    -1,    -1,     3,
     567,    -1,    25,    -1,   571,    28,    -1,    -1,    -1,    -1,
      -1,    -1,   579,    17,    -1,    -1,    20,   926,    -1,   928,
     164,    25,    -1,    -1,   933,    -1,   170,   936,    -1,    -1,
      -1,    -1,    36,    -1,    -1,  1155,   945,    -1,   947,    -1,
      -1,   950,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    63,
     762,    -1,    -1,   765,    -1,   974,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   984,    -1,   986,    -1,    -1,
      -1,   104,    86,    -1,    -1,    -1,   109,    -1,    -1,    -1,
    1210,  1211,  1212,  1213,    98,    -1,    -1,   453,   102,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,   115,   814,    -1,    -1,    -1,  1236,  1237,   820,    -1,
      -1,    -1,    -1,   479,    -1,    -1,    -1,    -1,    -1,   485,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1046,    -1,    -1,
     496,    -1,   498,    -1,  1053,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1066,     2,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    28,    -1,    -1,    -1,  1097,    -1,
      -1,    -1,    -1,    -1,    -1,   762,    -1,  1106,   765,    -1,
      -1,  1110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   567,    -1,    -1,    -1,   571,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   579,   926,    -1,   928,    -1,    -1,    -1,
      -1,   933,    -1,    77,   936,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   945,    -1,   947,  1155,   814,   950,    -1,
     273,    -1,    -1,   820,    22,    -1,    -1,   280,    26,    27,
     104,    -1,    -1,    -1,    -1,   109,    -1,    -1,    36,    -1,
      38,    39,   974,    -1,    -1,     3,    44,    -1,    -1,    -1,
      -1,    -1,   984,    51,   986,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,  1210,  1211,  1212,  1213,    -1,    34,    -1,    76,    -1,
      78,   334,    40,    -1,    -1,    -1,    -1,    -1,    46,    87,
      -1,    89,    50,    -1,    -1,    53,   349,  1236,  1237,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
     108,    69,    70,   111,  1046,   189,   114,    -1,    -1,    77,
      -1,  1053,    -1,    -1,    -1,    -1,    -1,    -1,    86,   926,
      -1,   928,    -1,    -1,  1066,    -1,   933,    95,    96,   936,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,   945,    -1,
     947,   109,    -1,   950,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1097,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1106,    -1,   762,   974,  1110,   765,
      -1,    -1,    -1,    -1,     2,    -1,    -1,   984,    -1,   986,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,   273,
     453,    -1,    -1,    -1,    -1,    -1,   280,    25,    -1,    -1,
      28,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1155,    -1,    -1,   479,    -1,   814,    -1,
      -1,    -1,   485,    -1,   820,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   496,    -1,   498,    -1,     2,    -1,  1046,
      -1,    -1,    -1,    -1,    -1,    -1,  1053,    12,    -1,    77,
     334,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1066,
      25,    -1,    -1,    28,    -1,   349,    -1,    -1,  1210,  1211,
    1212,  1213,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1097,    -1,    -1,    -1,  1236,  1237,    -1,    -1,    -1,  1106,
      -1,    -1,    -1,  1110,   567,    -1,    -1,    -1,   571,    -1,
      -1,    -1,    77,    -1,    -1,    -1,   579,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     926,    49,   928,    -1,    -1,    -1,    -1,   933,    -1,   104,
     936,    -1,    -1,    -1,   109,    -1,    -1,    -1,  1155,   945,
      -1,   947,    -1,    71,   950,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,    81,    82,    -1,    -1,    -1,    -1,   453,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   974,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   984,    -1,
     986,    -1,    -1,    22,    -1,   479,    -1,    26,    27,    -1,
      -1,   485,    -1,  1210,  1211,  1212,  1213,    36,    -1,    38,
      39,    -1,   496,    -1,   498,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,   189,    -1,    -1,    -1,    -1,  1236,
    1237,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   273,    -1,    76,    -1,    78,
    1046,    -1,   280,    82,    -1,    84,    85,  1053,    87,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
    1066,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,   111,   567,    -1,   114,    -1,   571,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   579,   214,    -1,    -1,   762,
      -1,  1097,   765,    -1,    -1,    -1,   334,    -1,   273,    -1,
    1106,    -1,    -1,    -1,  1110,   280,    -1,    -1,    -1,    -1,
      -1,   349,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,   257,
     258,   259,   260,   261,   262,    -1,    -1,   265,   266,    -1,
      -1,   814,    -1,    -1,   272,    -1,    -1,   820,    -1,  1155,
      -1,    -1,    -1,    -1,    -1,   283,    -1,    -1,    -1,   334,
      -1,   289,    -1,    -1,   139,   140,    30,   142,   143,   144,
      -1,   146,   147,   148,   349,    -1,   151,    -1,    -1,   307,
      44,    45,    46,   158,   159,    -1,    -1,   162,    -1,    -1,
     165,   166,   167,    -1,    22,   323,   324,    -1,    26,    27,
      -1,   176,    -1,    31,  1210,  1211,  1212,  1213,    36,    -1,
      38,    39,    -1,    -1,    -1,   453,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
    1236,  1237,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   479,    -1,    -1,    -1,    -1,    -1,   485,    76,    -1,
      78,    -1,    80,   926,    82,   928,    -1,    85,   496,    87,
     933,    89,    -1,   936,    -1,    -1,    -1,    -1,   762,    -1,
      -1,   765,   945,    -1,   947,    -1,    -1,   950,   453,    -1,
     108,    -1,   146,   111,   148,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   974,    -1,    -1,   479,    -1,    -1,    -1,    -1,    -1,
     485,   984,    -1,   986,    -1,    -1,    -1,    -1,    -1,    -1,
     814,   496,    -1,   498,    -1,    -1,   820,    -1,   456,   567,
      -1,    -1,    -1,   571,    -1,   199,   164,    -1,    -1,    -1,
      -1,   579,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   484,    -1,    -1,    -1,
     488,    -1,   490,    -1,    -1,   493,    -1,    -1,    12,    -1,
      -1,    -1,    -1,  1046,    -1,    19,    -1,    -1,    -1,    -1,
    1053,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,   255,   567,  1066,    -1,    -1,   571,    -1,    -1,   263,
     264,    -1,    -1,    -1,   579,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   276,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1097,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   926,  1106,   928,    -1,    -1,  1110,    -1,   933,
      -1,    -1,   936,    -1,    88,    -1,    -1,    -1,    92,    -1,
      -1,   945,    -1,   947,    -1,    -1,   950,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,   123,
     974,    -1,  1155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     984,    -1,   986,    -1,    -1,   139,   140,   361,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
     384,   165,   166,   167,   762,   169,    -1,   765,    -1,    -1,
      -1,    -1,   176,    -1,    -1,    -1,    -1,  1210,  1211,  1212,
    1213,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      -1,    -1,  1046,    31,    -1,    -1,    -1,    -1,    36,  1053,
      38,    39,    -1,  1236,  1237,    -1,    44,    -1,    -1,    -1,
      -1,    -1,  1066,    51,    -1,    -1,   814,    -1,    -1,    -1,
      -1,    -1,   820,    -1,    -1,    -1,    -1,   762,   452,    -1,
     765,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,   463,
      78,    -1,    80,  1097,    82,    -1,    -1,    85,    -1,    87,
      -1,    89,  1106,    -1,    -1,    -1,  1110,    -1,   746,   747,
      -1,    -1,    -1,    -1,   752,   753,    -1,    -1,   756,   757,
     108,    -1,    -1,   111,    -1,    -1,   114,    -1,   766,   814,
      -1,   505,    -1,    -1,    -1,   820,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   519,    -1,    -1,    -1,   523,
      -1,  1155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   805,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,   926,    -1,
     928,    -1,   170,    -1,    -1,   933,    -1,    -1,   936,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   945,    -1,   947,
      -1,    -1,   950,    -1,    -1,    -1,  1210,  1211,  1212,  1213,
      -1,   585,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   598,    -1,   974,    -1,    -1,    -1,
      -1,    -1,  1236,  1237,    -1,    -1,   984,    -1,   986,    -1,
      -1,   926,    -1,   928,    -1,    -1,    -1,    -1,   933,    -1,
      -1,   936,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     945,    -1,   947,    -1,    -1,   950,    -1,    -1,    88,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,   916,   917,
      -1,    -1,    -1,    -1,    -1,   923,    -1,   925,   662,   974,
      -1,    -1,    -1,    -1,   932,    -1,    -1,   935,    -1,   984,
     938,   986,   940,   123,   942,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   955,  1066,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,    -1,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,   982,   165,   166,   167,    -1,  1097,
      -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,  1106,    -1,
      -1,  1046,  1110,    -1,    -1,    -1,    -1,    -1,  1053,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,  1066,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,   767,    17,    18,    -1,    20,    -1,    22,
      -1,   775,    25,    -1,   778,    -1,    -1,  1155,   782,    -1,
      -1,    34,  1097,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,  1106,    45,    46,    -1,  1110,    -1,    50,    -1,    -1,
      53,   805,    -1,    56,    -1,    -1,    59,    60,    61,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,  1099,    -1,    86,    87,    -1,    -1,    -1,    91,    -1,
    1155,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,   104,   105,    -1,   858,    -1,   109,   861,    -1,   112,
      -1,    -1,   115,    -1,   117,    -1,    -1,    -1,   872,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1149,  1150,    -1,    -1,  1153,  1154,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,  1210,  1211,  1212,  1213,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,   164,    -1,  1181,    -1,  1183,   169,  1185,   171,    -1,
     173,  1236,  1237,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,     1,
      -1,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
     954,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,   979,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    93,    94,    95,    96,    97,    98,    99,   100,    -1,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     1,   176,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,    -1,   102,   103,    -1,   105,   106,   107,
     108,   109,    -1,   111,   112,   113,   114,   115,    -1,    -1,
     118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      -1,   169,   170,   171,    -1,   173,    -1,     1,   176,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    91,    -1,    93,
      94,    95,    96,    97,    98,    99,   100,    -1,   102,   103,
      -1,   105,   106,   107,   108,   109,    -1,   111,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
     164,    -1,    -1,   167,    -1,   169,   170,   171,    -1,   173,
      -1,     1,   176,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    89,
      90,    91,    -1,    93,    94,    95,    96,    97,    98,    99,
     100,    -1,   102,   103,    -1,   105,   106,   107,   108,   109,
      -1,   111,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,   169,
     170,   171,    -1,   173,    -1,     1,   176,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    -1,    89,    90,    91,    -1,    93,    94,    95,
      96,    97,    98,    99,   100,    -1,   102,   103,    -1,   105,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,
      -1,   167,    -1,   169,   170,   171,    -1,   173,    -1,     1,
     176,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    93,    94,    95,    96,    97,    98,    99,   100,    -1,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,   164,    -1,    -1,   167,    -1,   169,   170,   171,
      -1,   173,    -1,     1,   176,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      -1,    89,    90,    91,    -1,    93,    94,    95,    96,    97,
      98,    99,   100,    -1,   102,   103,    -1,   105,   106,   107,
     108,   109,    -1,   111,   112,   113,   114,   115,    -1,    -1,
     118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,
      -1,   169,   170,   171,    -1,   173,    -1,     1,   176,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    91,    -1,    93,
      94,    95,    96,    97,    98,    99,   100,    -1,   102,   103,
      -1,   105,   106,   107,   108,   109,    -1,   111,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
     164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     1,   176,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    -1,    89,
      90,    91,    -1,    93,    94,    95,    96,    97,    98,    99,
     100,    -1,   102,   103,    -1,   105,   106,   107,   108,   109,
      -1,   111,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     1,   176,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    -1,    89,    90,    91,    -1,    93,    94,    95,
      96,    97,    98,    99,   100,    -1,   102,   103,    -1,   105,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,     1,
     176,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    93,    94,    95,    96,    97,    98,    99,   100,    -1,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     1,   176,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      -1,    89,    90,    91,    -1,    93,    94,    95,    96,    97,
      98,    99,   100,    -1,   102,   103,    -1,   105,   106,   107,
     108,   109,    -1,   111,   112,   113,   114,   115,    -1,    -1,
     118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     1,   176,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    91,    -1,    93,
      94,    95,    96,    97,    98,    99,   100,    -1,   102,   103,
      -1,   105,   106,   107,   108,   109,    -1,   111,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
     164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     1,   176,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    -1,    89,
      90,    91,    -1,    93,    94,    95,    96,    97,    98,    99,
     100,    -1,   102,   103,    -1,   105,   106,   107,   108,   109,
      -1,   111,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     1,   176,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    -1,    89,    90,    91,    -1,    93,    94,    95,
      96,    97,    98,    99,   100,    -1,   102,   103,    -1,   105,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,     1,
     176,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    93,    94,    95,    96,    97,    98,    99,   100,    -1,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     1,   176,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      -1,    89,    90,    91,    -1,    93,    94,    95,    96,    97,
      98,    99,   100,    -1,   102,   103,    -1,   105,   106,   107,
     108,   109,    -1,   111,   112,   113,   114,   115,    -1,    -1,
     118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     1,   176,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    91,    -1,    93,
      94,    95,    96,    97,    98,    99,   100,    -1,   102,   103,
      -1,   105,   106,   107,   108,   109,    -1,   111,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
     164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     1,   176,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,   174,     1,   176,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,
     176,     5,     6,     7,     8,     9,    10,    -1,    12,    -1,
      14,    -1,    -1,    17,    18,    19,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
     124,   125,   126,   127,   128,   129,    -1,    -1,   132,   133,
     134,   135,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,    -1,   146,    -1,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,   165,   166,   167,   168,   169,    -1,   171,    -1,   173,
      -1,    -1,   176,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,
     110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     3,   176,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,   114,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,    -1,   176,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,   106,   107,    -1,   109,   110,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,   172,   173,
      -1,    -1,   176,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,
     110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,   172,   173,    -1,     3,   176,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      48,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,   114,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
     106,   107,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,
     176,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,   106,   107,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     3,   176,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     3,   176,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,   114,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,
     176,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,   121,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     3,   176,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,    -1,   176,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,   170,   171,    -1,   173,    -1,    -1,
     176,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,   174,     3,   176,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,
      -1,    -1,    92,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,    -1,   176,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,   174,     3,
     176,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     3,   176,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,    -1,   176,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,   111,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,
     176,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
     174,     3,   176,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,    -1,   176,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,   110,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,
     176,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,    -1,   176,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,
     176,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     3,   176,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     3,   176,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     3,   176,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,
     176,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     3,   176,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     3,   176,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     3,   176,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,
     176,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     3,   176,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     3,   176,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    12,    -1,    -1,    -1,   148,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    34,   167,    36,    -1,
      -1,   171,    40,   173,    -1,    -1,   176,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    -1,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,    -1,    -1,   132,   133,   134,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,    -1,   146,    -1,
      -1,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,    -1,   162,    -1,    -1,   165,   166,   167,
     168,    -1,     3,   171,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   155,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     168,   162,   163,   171,    -1,   173,   167,    -1,   169,    -1,
     171,    -1,   173,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,    -1,    -1,   145,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,   169,
      -1,   171,   172,   173,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,    -1,   141,    -1,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,   168,    22,   163,   171,    25,   173,    -1,    -1,
     169,    -1,   171,    -1,   173,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,   163,    -1,    25,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    17,    18,   105,    20,    -1,    -1,
     109,    -1,    25,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,     3,    -1,
      53,    -1,   141,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      63,    -1,    -1,    -1,    19,    20,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    34,
      -1,    -1,   171,    86,   173,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    -1,    98,    -1,   100,    -1,   102,
      55,    -1,   105,    -1,    -1,    -1,   109,    -1,    63,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,   124,
     125,   126,   127,   128,   129,    -1,    -1,   132,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
      -1,   146,    -1,    -1,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,     3,   162,    -1,    -1,
     165,   166,   167,   168,   169,    12,    -1,    -1,    -1,    -1,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    95,    96,
      -1,    98,    -1,   100,    19,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    32,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,    -1,    -1,   132,   133,   134,   135,    -1,
     137,   138,   139,   140,   141,   142,   143,   144,    -1,   146,
      -1,    -1,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,   162,    -1,    -1,   165,   166,
     167,   168,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,   101,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,
     125,   126,   127,   128,   129,    -1,    -1,   132,   133,   134,
     135,    -1,   137,   138,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,
      92,   176,    -1,    12,    -1,    -1,    -1,    -1,    -1,   101,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,   124,   125,   126,   127,   128,   129,    -1,    -1,
     132,   133,   134,   135,    -1,   137,   138,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,   169,    -1,    88,
      -1,    -1,    -1,    92,   176,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   101,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,   128,
     129,    -1,    -1,   132,   133,   134,   135,    -1,   137,   138,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
     169,    -1,    88,    -1,    -1,    -1,    92,   176,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   101,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,   129,    -1,    -1,   132,   133,   134,   135,
      -1,   137,   138,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,
     176,    -1,    12,    -1,    -1,    -1,    -1,    -1,   101,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,   129,    -1,    -1,   132,
     133,   134,   135,    -1,   137,   138,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,
      -1,    -1,    92,   176,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   101,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,   132,   133,   134,   135,    -1,   137,   138,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    88,    -1,    -1,    -1,    92,   176,    -1,    12,    -1,
      -1,    -1,    -1,    -1,   101,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,
     127,   128,   129,    -1,    -1,   132,   133,   134,   135,    -1,
     137,   138,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   101,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
     124,   125,   126,   127,   128,   129,    -1,    -1,   132,   133,
     134,   135,    -1,   137,   138,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,    -1,
      -1,    92,   176,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     101,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,   125,   126,   127,   128,   129,    -1,
      48,   132,   133,   134,   135,    -1,   137,   138,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,   169,    -1,
      88,    -1,    -1,    -1,    92,   176,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,   119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    88,    -1,    -1,    -1,    92,    -1,   176,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,
     176,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    37,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    72,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,
      -1,    -1,    92,   176,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,   164,   165,   166,   167,    -1,    -1,
      88,    -1,    -1,    -1,    92,    -1,   176,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,   119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    -1,    88,    -1,    -1,    -1,    92,
      -1,   176,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,
      -1,    -1,    92,   176,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    32,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    -1,
      88,    -1,    -1,    -1,    92,    12,   176,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,   123,   165,   166,   167,
      -1,    88,    -1,    -1,    -1,    92,    -1,    12,   176,    -1,
      -1,    -1,   139,   140,    19,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,    32,    -1,   156,
     157,   158,   159,   160,    -1,   162,   123,    -1,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,   169,    88,    -1,    -1,    -1,    92,    -1,   176,
      12,    -1,    -1,    -1,    -1,    -1,   101,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,
      92,   176,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,   119,    37,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    72,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    -1,    -1,    88,
      -1,    -1,   174,    92,   176,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      -1,    88,    -1,    -1,    -1,    92,    -1,   176,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
     164,   165,   166,   167,    -1,    -1,    88,    -1,    -1,    -1,
      92,    12,   176,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,   164,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,   164,   165,   166,   167,    -1,    88,    -1,
      -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,   164,   165,   166,   167,    -1,    88,
      -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,
      -1,    -1,    19,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    88,    -1,    -1,    -1,    92,    12,    13,   176,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    88,    -1,    -1,   172,    92,    12,    -1,   176,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,
     176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,
      -1,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,
      12,    -1,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,   164,   165,   166,   167,    -1,    88,    -1,    -1,    -1,
      92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,
      -1,   172,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    88,
      -1,    -1,   172,    92,    12,    13,   176,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    88,    -1,    -1,    -1,    92,    12,    -1,   176,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    88,    -1,    -1,   172,    92,    12,    -1,   176,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,
     176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,
      13,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,
     174,    12,   176,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    -1,    88,    -1,    -1,
      -1,    92,    12,   176,    -1,    -1,    -1,    -1,    -1,    19,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,
      -1,    -1,    92,    -1,    -1,   176,    -1,    -1,    -1,    -1,
      -1,   101,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    61,   156,   157,   158,   159,
     160,   161,   162,    12,    13,   165,   166,   167,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,    88,    32,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    88,
     146,   147,   148,    92,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
     176,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   123,    -1,   165,   166,   167,    -1,
      -1,    88,    -1,    -1,    -1,    92,    -1,   176,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    12,    -1,   165,   166,   167,    -1,
      -1,    19,    -1,    -1,    -1,   174,    -1,   176,    -1,    -1,
      -1,    -1,   139,   140,    32,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,    -1,    -1,   156,
     157,   158,   159,   160,    12,   162,    -1,    -1,   165,   166,
     167,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   101,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   123,    -1,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    12,    22,   165,   166,   167,
      26,    27,    19,    -1,    -1,    31,    -1,    -1,   176,    -1,
      36,    -1,    38,    39,    -1,    32,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    32,    82,    -1,    -1,    85,
      -1,    87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,   101,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   101,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   164,   156,
     157,   158,   159,   160,   161,   162,   123,    -1,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    12,     3,   165,   166,
     167,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    17,    18,    -1,    20,    -1,    32,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    12,    -1,    -1,    -1,
      46,    -1,    -1,    19,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    63,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    88,    -1,    -1,    -1,    92,    12,    -1,    -1,
      86,    -1,    -1,    -1,    19,   101,    -1,    -1,    -1,    95,
      96,    -1,    98,    -1,   100,    -1,   102,    32,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,   123,    -1,   115,
      -1,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   101,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   123,    -1,   165,
     166,   167,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,
     176,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,   123,   165,
     166,   167,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     176,    19,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    32,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    -1,    12,    -1,    -1,    -1,   174,
      -1,   176,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,   123,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    34,    -1,    -1,     3,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    17,    18,    53,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    34,    69,
      70,    -1,    -1,    -1,    40,    -1,    -1,    77,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    86,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    63,    98,    -1,
     100,    -1,   102,    69,    70,   105,    -1,    -1,    -1,   109,
      -1,    77,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      86,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    95,
      96,    -1,    98,    -1,   100,    -1,   102,    17,    18,   105,
      20,    -1,    -1,   109,    -1,    25,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    34,    -1,    -1,     3,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    17,    18,    53,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    34,    69,
      70,    -1,    -1,    -1,    40,    -1,    -1,    77,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    86,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    63,    98,    -1,
     100,    -1,   102,    69,    70,   105,    -1,    -1,    -1,   109,
      -1,    77,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    -1,    98,    -1,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   181,   182,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    33,    34,
      36,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      49,    50,    51,    53,    56,    57,    58,    59,    60,    62,
      63,    64,    66,    67,    69,    70,    71,    76,    77,    78,
      79,    80,    81,    82,    85,    86,    87,    89,    90,    91,
      93,    94,    95,    96,    97,    98,    99,   100,   102,   103,
     105,   106,   107,   108,   109,   111,   112,   114,   115,   118,
     120,   123,   139,   140,   141,   142,   143,   148,   158,   161,
     162,   163,   164,   167,   169,   171,   173,   176,   183,   184,
     185,   186,   189,   190,   191,   192,   194,   195,   196,   201,
     202,   205,   206,   210,   212,   215,   219,   222,   223,   224,
     225,   226,   227,   229,   230,   232,   234,   237,   238,   239,
     240,   241,   245,   246,   249,   250,   251,   254,   255,   262,
     263,   264,   265,   266,   268,   269,   294,   295,   299,   300,
     321,   322,   323,   324,   325,   326,   327,   335,   336,   337,
     338,   339,   342,   343,   344,   345,   346,   347,   348,   349,
     351,   352,   353,   354,   355,   164,   185,   339,   119,   328,
     329,     3,   207,    14,    22,    36,    41,    42,    45,    56,
      87,   100,   169,   173,   237,   262,   321,   326,   337,   338,
     339,   342,   344,   345,   328,   339,   108,   301,    89,   207,
     185,   315,   339,     8,   188,   185,   171,     3,    17,    18,
      20,    25,    34,    40,    46,    50,    53,    63,    69,    70,
      77,    86,    95,    96,    98,   100,   102,   105,   109,   112,
     115,   209,   211,    11,   108,    78,   121,   231,   339,   231,
     339,   231,   339,    27,   114,   233,   339,    82,    85,   192,
     171,   209,   209,   209,   171,   277,   171,   209,   302,   303,
      33,   196,   214,   339,   339,    18,    77,    95,   112,   339,
     339,   339,     8,   171,   221,   220,     4,   289,   314,   339,
     106,   107,   164,   339,   341,   339,   214,   339,   339,   339,
      99,   171,   185,   339,   339,   196,   206,   339,   342,   196,
     206,   339,   339,   233,   339,   339,   339,   339,   339,   339,
     339,     1,   170,   183,   197,   314,   110,   149,   289,   316,
     317,   341,   231,   314,   339,   350,   339,    80,   185,   169,
      47,    84,   113,   193,    26,   300,   339,     8,   250,   339,
     340,    56,   144,   252,   209,     1,    31,   209,   256,   258,
     261,    54,    73,    83,   284,    27,    78,    89,   108,   285,
      27,    78,    89,   108,   283,   209,   296,   297,   302,   163,
     164,   155,   339,    12,    19,    32,    88,    92,   123,   139,
     140,   142,   143,   144,   146,   147,   148,   150,   151,   152,
     153,   154,   156,   157,   158,   159,   160,   161,   162,   165,
     166,   167,   176,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   168,   273,
     171,   173,    88,    92,   171,   185,   164,   339,   339,   339,
     209,   314,    56,   169,   196,    48,   328,   299,   164,   145,
     164,   187,    22,    36,    39,    87,   189,   192,   118,   210,
     289,   318,   319,   320,   341,   169,   209,   171,   214,    33,
      48,   214,   119,   214,   331,    33,    48,   214,   331,   214,
     331,    48,   214,   331,   209,   209,   101,   196,   101,   124,
     196,   273,   193,   318,   171,   171,   196,   185,    27,    48,
      52,    75,    78,    89,   108,   184,   278,   279,   280,   281,
     282,   285,   110,   171,   209,   304,   305,     1,   144,   309,
      48,   145,   185,   214,   171,   171,   214,   318,   222,   222,
     145,   164,   339,   339,   164,   169,   214,   171,   318,   164,
     242,   164,   242,   164,   214,   214,   164,   170,   170,   183,
     145,   170,   339,   145,   172,   145,   172,   174,   331,    48,
     145,   174,   331,   122,   145,   174,     8,     1,   170,   197,
     203,   204,   339,   199,   339,    65,    37,    72,   164,   250,
     252,   228,   266,   209,   314,   169,   170,     8,   260,   124,
     145,   170,    89,     1,   144,   308,    89,     1,     3,    12,
      17,    19,    20,    25,    40,    46,    53,    55,    63,    69,
      70,    86,    98,   102,   105,   109,   115,   139,   140,   141,
     142,   143,   144,   146,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   162,   165,   166,   167,   168,
     171,   208,   209,   211,   270,   271,   272,   273,   321,   124,
     298,   164,   145,   164,   339,   339,   339,   339,   231,   339,
     231,   339,   339,   339,   339,   339,   339,   339,     3,    20,
      34,    63,   102,   108,   210,   339,   339,   339,   339,   339,
     339,   339,   339,   339,   339,   339,   339,   339,   339,   339,
     339,   339,    68,   341,   341,   341,   341,   341,   318,   318,
     231,   339,   231,   339,    27,    48,    89,   114,   330,   333,
     334,   339,   355,    33,    48,    33,    48,   101,   171,    48,
     174,   209,   231,   339,   214,   302,   339,   189,   339,   124,
     172,   145,    48,   314,    45,   339,   231,   339,   171,   214,
      45,   339,   231,   339,   214,   214,   231,   339,   214,   124,
     124,   185,    35,   185,   339,    35,   339,    65,   172,   319,
     209,   235,   236,    48,    89,   281,   145,   172,   171,   209,
     309,   305,   145,   172,    34,    50,    96,   100,   173,   213,
     310,   322,   124,   306,   339,   303,   339,   339,   172,   289,
     339,     1,   247,   320,   172,    21,   243,   170,   172,   172,
     316,   172,   316,   185,   174,   231,   339,   174,   185,   339,
     174,   339,   174,   339,   170,   170,   145,   164,    13,   147,
     145,   164,    13,    37,    72,   209,   139,   140,   141,   142,
     143,   158,   162,   167,   198,   272,   273,   274,   339,   198,
     200,   252,   169,   299,   164,   171,     1,   253,   259,   261,
     339,   257,   173,   213,   307,   322,   104,   286,   171,   276,
     339,   139,   147,   276,   276,   310,   322,   296,   171,   173,
     164,   164,   164,   164,   164,   164,   172,   174,    48,    89,
     145,   172,    17,    20,    25,    46,    53,    63,    70,    86,
      98,   109,   115,   321,    88,    88,    45,   231,   339,    45,
     231,   339,   319,   231,   339,   171,   328,   328,   164,   164,
     289,   341,   320,   339,   172,   339,    33,   214,    33,   214,
     332,   333,   339,    33,   214,   331,    33,   214,   331,   214,
     331,   214,   331,   339,   339,    35,   185,    35,    35,   185,
     101,   196,   209,   172,   145,   172,   209,   280,   305,   144,
     313,    61,   117,   290,   172,   304,   309,     1,   314,    68,
     341,   172,   172,   170,    74,   116,   170,   248,   172,   171,
     196,   209,   244,   185,   174,   331,   174,   331,   185,   122,
     203,   210,   169,   274,   339,   110,   339,   198,   200,   145,
     164,    13,   164,   169,   253,   302,   319,   170,    31,    82,
      85,   170,   184,   216,   219,   261,   258,     1,   174,   314,
     290,   279,   172,     3,   102,   271,   273,   172,   174,   333,
     309,   321,   321,   339,    33,    33,   339,    33,    33,   172,
     174,   174,   319,   214,   214,   214,   101,    45,   339,    45,
     339,   145,   172,   101,    45,   339,   214,    45,   339,   214,
     214,   214,   185,   185,   339,   185,    35,   164,   164,   236,
     196,   313,   172,   173,   213,   289,   312,   322,   149,   275,
     306,     3,    91,   102,   291,   292,   293,   339,   195,   215,
     288,   306,   174,    48,   174,   171,   171,    33,   185,   314,
     244,   144,   196,    45,   185,   339,   174,    45,   185,   339,
     174,   339,   198,    13,    37,    72,    37,    72,   164,   164,
     274,   339,   339,   253,   170,   164,   172,     8,   218,   216,
     174,   307,   322,   174,   267,   172,   276,   276,   306,   101,
      45,    45,   101,    45,    45,    45,    45,   172,   339,   339,
     339,   333,   339,   339,   339,    35,   185,   275,   306,   313,
     174,   314,   289,   339,   293,   117,   145,   124,   150,   152,
     153,   156,   157,    61,   339,   310,   322,   318,   318,   185,
     214,   172,   339,   339,   185,   339,   185,   170,   110,   339,
     198,   200,   198,   200,    13,   170,   164,   217,   219,   307,
     322,   164,   287,   288,   339,   339,   339,   339,   339,   339,
     101,   101,   101,   101,   185,   275,   306,   289,   311,   312,
     322,    48,   174,   339,   292,   293,   293,   293,   293,   293,
     293,   291,   174,   172,   172,   196,   101,   101,   164,   164,
     164,   164,   339,   219,   101,   101,   101,   101,   101,   101,
     339,   339,   339,   339,   339,   311,   312,   322,   163,   163,
     339,   339,   174,   311
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   180,   181,   182,   182,   183,   183,   184,   184,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   186,   187,   186,   188,   186,   189,   189,   190,
     191,   191,   191,   192,   192,   192,   193,   193,   194,   195,
     195,   195,   196,   197,   197,   198,   198,   198,   198,   198,
     198,   199,   199,   199,   199,   199,   199,   200,   200,   201,
     201,   201,   201,   201,   201,   201,   202,   203,   203,   203,
     203,   204,   204,   205,   206,   206,   206,   206,   206,   206,
     207,   207,   208,   208,   208,   208,   208,   208,   209,   209,
     209,   209,   209,   209,   210,   210,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   213,   213,   213,
     213,   214,   214,   215,   215,   216,   217,   216,   218,   216,
     219,   219,   220,   219,   221,   219,   222,   222,   222,   222,
     222,   222,   222,   223,   223,   223,   223,   224,   225,   225,
     226,   227,   227,   227,   228,   227,   229,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   231,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   233,   233,   234,   234,   235,
     235,   236,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   238,   238,   238,   238,   238,
     238,   239,   239,   239,   240,   240,   240,   241,   241,   241,
     241,   241,   241,   242,   242,   243,   243,   243,   244,   244,
     245,   246,   246,   247,   247,   248,   248,   248,   249,   249,
     250,   251,   251,   251,   252,   252,   253,   253,   253,   254,
     254,   255,   256,   256,   257,   256,   258,   259,   258,   260,
     258,   261,   261,   262,   263,   264,   264,   264,   265,   267,
     266,   268,   268,   268,   268,   268,   269,   270,   270,   271,
     271,   271,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     274,   274,   275,   275,   275,   276,   276,   277,   278,   278,
     279,   279,   280,   280,   280,   280,   280,   280,   281,   281,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   283,
     283,   283,   283,   283,   283,   284,   284,   284,   285,   285,
     285,   285,   285,   285,   286,   286,   287,   287,   288,   288,
     289,   290,   290,   290,   290,   290,   291,   291,   292,   292,
     292,   292,   292,   292,   292,   293,   293,   294,   295,   295,
     295,   296,   296,   297,   298,   298,   298,   299,   299,   299,
     299,   299,   301,   300,   300,   302,   302,   303,   303,   304,
     304,   304,   305,   305,   305,   306,   306,   306,   307,   307,
     307,   307,   307,   307,   307,   308,   308,   308,   308,   308,
     309,   309,   309,   309,   309,   310,   310,   310,   310,   311,
     311,   311,   312,   312,   312,   312,   312,   313,   313,   313,
     313,   313,   314,   314,   314,   314,   315,   315,   316,   316,
     316,   317,   317,   318,   318,   319,   319,   320,   320,   320,
     320,   321,   321,   322,   322,   322,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   325,   326,   327,   327,   327,
     327,   327,   327,   327,   327,   328,   328,   329,   330,   330,
     331,   332,   332,   333,   333,   333,   334,   334,   334,   334,
     334,   334,   335,   335,   336,   336,   336,   336,   336,   337,
     337,   337,   337,   337,   338,   339,   339,   339,   339,   339,
     339,   339,   339,   339,   339,   339,   339,   339,   339,   339,
     339,   339,   340,   340,   341,   341,   341,   342,   342,   342,
     342,   343,   343,   343,   343,   343,   344,   344,   344,   345,
     345,   345,   345,   345,   345,   346,   346,   346,   346,   347,
     347,   348,   348,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   350,   350,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   352,   352,   352,   352,   352,   352,   352,   353,   353,
     353,   353,   354,   354,   354,   354,   355,   355,   355,   355,
     355,   355,   355
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     2,     3,     3,     2,     2,
       3,     2,     1,     0,     4,     0,     3,     1,     1,     4,
       3,     4,     4,     0,     1,     1,     0,     1,     6,     2,
       3,     3,     1,     1,     2,     1,     1,     3,     3,     3,
       5,     1,     3,     3,     3,     5,     5,     0,     1,     4,
       6,     8,     8,     6,     8,     8,     4,     1,     3,     3,
       5,     1,     3,     3,     4,     4,     4,     4,     4,     4,
       0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     2,     3,     1,     0,     4,     0,     3,
       1,     1,     0,     3,     0,     3,     1,     1,     1,     1,
       1,     1,     1,     3,     5,     5,     2,     1,     1,     1,
       1,     6,     7,     3,     0,     6,     2,     5,     3,     3,
       6,     6,     4,     5,     5,     3,     3,     6,     5,     6,
       5,     6,     3,     4,     3,     4,     5,     6,     5,     6,
       3,     4,     3,     4,     6,     7,     6,     7,     4,     5,
       4,     5,     4,     4,     3,     6,     5,     4,     3,     6,
       5,     6,     5,     8,     7,     4,     4,     6,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     6,     4,     7,     5,     3,     6,     3,     3,     2,
       2,     3,     3,     0,     2,     2,     3,     5,     1,     3,
       3,     5,     5,     0,     2,     3,     2,     3,     5,     5,
       2,     1,     1,     1,     0,     2,     0,     2,     3,     3,
       3,     3,     1,     2,     0,     4,     1,     0,     4,     0,
       3,     1,     3,     6,     0,     1,     1,     1,     1,     0,
       7,     4,     4,     6,     6,     4,     2,     1,     3,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     0,     3,     3,     1,     3,
       0,     1,     4,     5,     4,     5,     6,     6,     0,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     0,
       1,     1,     2,     1,     1,     1,     1,     1,     0,     1,
       2,     1,     1,     1,     0,     1,     1,     1,     1,     1,
       1,     0,     2,     2,     4,     4,     1,     3,     3,     3,
       3,     3,     3,     3,     2,     1,     1,     3,     1,     2,
       2,     1,     3,     2,     0,     2,     2,     1,     2,     1,
       1,     1,     0,     5,     3,     1,     3,     3,     5,     1,
       1,     3,     1,     2,     3,     0,     2,     2,     3,     2,
       4,     3,     3,     4,     3,     0,     2,     2,     2,     1,
       0,     2,     2,     2,     1,     4,     4,     6,     3,     0,
       1,     1,     3,     4,     3,     4,     6,     0,     2,     2,
       2,     2,     1,     1,     3,     3,     1,     3,     1,     1,
       1,     3,     3,     0,     1,     1,     3,     3,     3,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     2,     4,     4,     4,     5,     2,     2,     1,     2,
       1,     2,     1,     2,     1,     2,     1,     1,     6,     6,
       4,     9,     9,     7,     6,     6,     4,     9,     9,     7,
       4,     6,     6,     9,     9,     6,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     0,     1,     4,     1,     3,
       4,     1,     3,     4,     3,     3,     1,     1,     2,     1,
       2,     1,     1,     3,     1,     2,     2,     2,     2,     2,
       8,     8,     9,     9,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     3,     3,     3,     2,     2,
       2,     1,     0,     1,     2,     2,     1,     1,     1,     1,
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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YY_LOCATION_PRINT
#  if defined YYCHPL_LTYPE_IS_TRIVIAL && YYCHPL_LTYPE_IS_TRIVIAL

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

#   define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

#  else
#   define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined YY_LOCATION_PRINT */


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
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (context);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
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

  YY_LOCATION_PRINT (yyo, *yylocationp);
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
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (context);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
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
    case 2:
      yypstate_clear (yyps);
      goto case_0;

    case_0:
    case 0:
      yyn = yypact[yystate];
      goto yyread_pushed_token;
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
    goto yyexhaustedlab;
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
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
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
#line 639 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6400 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 643 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6406 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 644 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6412 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 651 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6420 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 659 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6428 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 663 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6437 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: deprecated_decl_stmt  */
#line 670 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6443 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: include_module_stmt  */
#line 671 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6449 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: block_stmt  */
#line 672 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6455 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: use_stmt  */
#line 673 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6461 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: import_stmt  */
#line 674 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6467 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: require_stmt  */
#line 675 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6473 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: assignment_stmt  */
#line 676 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6479 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: extern_block_stmt  */
#line 677 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6485 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: if_stmt  */
#line 678 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6491 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: implements_stmt  */
#line 679 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6497 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt: interface_stmt  */
#line 680 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6503 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt: loop_stmt  */
#line 681 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6509 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: select_stmt  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6515 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt: TDEFER stmt  */
#line 684 "chpl.ypp"
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
#line 6532 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt: try_stmt  */
#line 696 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6538 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt: throw_stmt  */
#line 697 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6544 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: return_stmt  */
#line 698 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6550 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: stmt_level_expr TSEMI  */
#line 699 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6556 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt: TATOMIC stmt  */
#line 700 "chpl.ypp"
                            { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6562 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt: TBEGIN opt_task_intent_ls stmt  */
#line 702 "chpl.ypp"
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
#line 6580 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 716 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6593 "bison-chpl-lib.cpp"
    break;

  case 30: /* stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 725 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    YYLTYPE locBodyAnchor = context->makeLocationAtLast((yylsp[-1]));
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]),
                               false, locBodyAnchor, (yyvsp[0].commentsAndStmt));
    assert(blockStyle == BlockStyle::EXPLICIT);
    auto taskBodies = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Cobegin::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].withClause)),
                               std::move(taskBodies));
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6612 "bison-chpl-lib.cpp"
    break;

  case 31: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 740 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6625 "bison-chpl-lib.cpp"
    break;

  case 32: /* stmt: TDELETE simple_expr_ls TSEMI  */
#line 749 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6637 "bison-chpl-lib.cpp"
    break;

  case 33: /* stmt: TLABEL ident_def stmt  */
#line 757 "chpl.ypp"
  {
    if ((yyvsp[0].commentsAndStmt).stmt->isFor() || (yyvsp[0].commentsAndStmt).stmt->isWhile() || (yyvsp[0].commentsAndStmt).stmt->isDoWhile()) {
      auto exprLst = context->makeList((yyvsp[0].commentsAndStmt));
      auto comments = context->gatherCommentsFromList(exprLst, (yylsp[-2]));
      auto astLst = context->consumeList(exprLst);
      Loop* loop = nullptr;
      for (auto& ast : astLst) {
        if (ast->isLoop()) {
          loop = ast.release()->toLoop();
          break;
        }
      }
      assert(loop);
      auto node = Label::build(BUILDER, LOC((yyloc)), (yyvsp[-1].uniqueStr), toOwned(loop));
      (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    } else {
      const char* msg = "can only label for-, while-do- "
                        "and do-while-statements";
      auto err = context->raiseError((yyloc), msg);
      (yyval.commentsAndStmt) = context->finishStmt(err);
    }
  }
#line 6664 "bison-chpl-lib.cpp"
    break;

  case 34: /* stmt: TLOCAL expr do_stmt  */
#line 780 "chpl.ypp"
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
#line 6680 "bison-chpl-lib.cpp"
    break;

  case 35: /* stmt: TLOCAL do_stmt  */
#line 792 "chpl.ypp"
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
#line 6695 "bison-chpl-lib.cpp"
    break;

  case 36: /* stmt: TON expr do_stmt  */
#line 803 "chpl.ypp"
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
#line 6711 "bison-chpl-lib.cpp"
    break;

  case 37: /* stmt: TSERIAL expr do_stmt  */
#line 815 "chpl.ypp"
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
#line 6727 "bison-chpl-lib.cpp"
    break;

  case 38: /* stmt: TSERIAL do_stmt  */
#line 827 "chpl.ypp"
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
#line 6742 "bison-chpl-lib.cpp"
    break;

  case 39: /* stmt: TSYNC stmt  */
#line 838 "chpl.ypp"
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
#line 6759 "bison-chpl-lib.cpp"
    break;

  case 40: /* stmt: TYIELD expr TSEMI  */
#line 851 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6770 "bison-chpl-lib.cpp"
    break;

  case 41: /* stmt: error TSEMI  */
#line 858 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
  }
#line 6778 "bison-chpl-lib.cpp"
    break;

  case 43: /* $@1: %empty  */
#line 866 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 6786 "bison-chpl-lib.cpp"
    break;

  case 44: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL $@1 deprecated_decl_base  */
#line 870 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6794 "bison-chpl-lib.cpp"
    break;

  case 45: /* $@2: %empty  */
#line 874 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 6802 "bison-chpl-lib.cpp"
    break;

  case 46: /* deprecated_decl_stmt: TDEPRECATED $@2 deprecated_decl_base  */
#line 878 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6810 "bison-chpl-lib.cpp"
    break;

  case 49: /* module_decl_start: access_control opt_prototype TMODULE ident_def  */
#line 890 "chpl.ypp"
    {
      // take into account location of public/private, if any
      auto loc = context->declStartLoc((yylsp[-1]));

      ModuleParts parts = {
        .comments=context->gatherComments(loc),
        .attributes=context->buildAttributes((yyloc)).release(),
        .visibility=(yyvsp[-3].visibilityTag),
        .kind=(yyvsp[-2].moduleKind),
        .name=(yyvsp[0].uniqueStr)
      };

      (yyval.moduleParts) = parts;
      context->resetDeclState();
      context->clearComments();
    }
#line 6831 "bison-chpl-lib.cpp"
    break;

  case 50: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 910 "chpl.ypp"
    {
      context->clearCommentsBefore((yylsp[-1]));

      ModuleParts parts = (yyvsp[-2].moduleParts);
      ParserExprList* body = context->makeList();
      context->appendList(body, context->gatherComments((yylsp[0])));
      auto mod = Module::build(BUILDER, LOC((yylsp[-2])), toOwned(parts.attributes),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6850 "bison-chpl-lib.cpp"
    break;

  case 51: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 925 "chpl.ypp"
    {
      context->clearCommentsBefore((yylsp[-2]));

      ModuleParts parts = (yyvsp[-3].moduleParts);
      ParserExprList* body = (yyvsp[-1].exprList);
      context->appendList(body, context->gatherComments((yylsp[0])));
      auto mod = Module::build(BUILDER, LOC((yylsp[-3])), toOwned(parts.attributes),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6869 "bison-chpl-lib.cpp"
    break;

  case 52: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 940 "chpl.ypp"
    {
      ModuleParts parts = (yyvsp[-3].moduleParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      ParserExprList* body = context->makeList(std::move(err));
      auto mod = Module::build(BUILDER, LOC((yylsp[-3])), toOwned(parts.attributes),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6886 "bison-chpl-lib.cpp"
    break;

  case 53: /* access_control: %empty  */
#line 955 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6892 "bison-chpl-lib.cpp"
    break;

  case 54: /* access_control: TPUBLIC  */
#line 956 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6899 "bison-chpl-lib.cpp"
    break;

  case 55: /* access_control: TPRIVATE  */
#line 958 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6906 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_prototype: %empty  */
#line 963 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6912 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_prototype: TPROTOTYPE  */
#line 964 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6919 "bison-chpl-lib.cpp"
    break;

  case 58: /* include_module_stmt: TINCLUDE access_control opt_prototype TMODULE ident_def TSEMI  */
#line 970 "chpl.ypp"
   {
     (yyval.commentsAndStmt) = TODOSTMT((yyloc));
     context->visibility = Decl::DEFAULT_VISIBILITY;
   }
#line 6928 "bison-chpl-lib.cpp"
    break;

  case 59: /* block_stmt_body: TLCBR TRCBR  */
#line 987 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6936 "bison-chpl-lib.cpp"
    break;

  case 60: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 991 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6944 "bison-chpl-lib.cpp"
    break;

  case 61: /* block_stmt_body: TLCBR error TRCBR  */
#line 995 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6956 "bison-chpl-lib.cpp"
    break;

  case 62: /* block_stmt: block_stmt_body  */
#line 1006 "chpl.ypp"
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
#line 6975 "bison-chpl-lib.cpp"
    break;

  case 63: /* stmt_ls: toplevel_stmt  */
#line 1024 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6981 "bison-chpl-lib.cpp"
    break;

  case 64: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1025 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6987 "bison-chpl-lib.cpp"
    break;

  case 65: /* renames_ls: expr  */
#line 1030 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6995 "bison-chpl-lib.cpp"
    break;

  case 66: /* renames_ls: all_op_name  */
#line 1034 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7003 "bison-chpl-lib.cpp"
    break;

  case 67: /* renames_ls: expr TAS expr  */
#line 1038 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7012 "bison-chpl-lib.cpp"
    break;

  case 68: /* renames_ls: renames_ls TCOMMA expr  */
#line 1043 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7020 "bison-chpl-lib.cpp"
    break;

  case 69: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1047 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7028 "bison-chpl-lib.cpp"
    break;

  case 70: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1051 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7037 "bison-chpl-lib.cpp"
    break;

  case 71: /* use_renames_ls: expr  */
#line 1061 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(node);
  }
#line 7046 "bison-chpl-lib.cpp"
    break;

  case 72: /* use_renames_ls: expr TAS expr  */
#line 1066 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as));
    (yyval.exprList) = context->makeList(node);
  }
#line 7056 "bison-chpl-lib.cpp"
    break;

  case 73: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1072 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as));
    (yyval.exprList) = context->makeList(node);
  }
#line 7067 "bison-chpl-lib.cpp"
    break;

  case 74: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1079 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7076 "bison-chpl-lib.cpp"
    break;

  case 75: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1084 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7087 "bison-chpl-lib.cpp"
    break;

  case 76: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1091 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7099 "bison-chpl-lib.cpp"
    break;

  case 77: /* opt_only_ls: %empty  */
#line 1101 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7105 "bison-chpl-lib.cpp"
    break;

  case 78: /* opt_only_ls: renames_ls  */
#line 1102 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7111 "bison-chpl-lib.cpp"
    break;

  case 79: /* use_stmt: access_control TUSE use_renames_ls TSEMI  */
#line 1107 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7119 "bison-chpl-lib.cpp"
    break;

  case 80: /* use_stmt: access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1111 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7130 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_stmt: access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1118 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7142 "bison-chpl-lib.cpp"
    break;

  case 82: /* use_stmt: access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1126 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7155 "bison-chpl-lib.cpp"
    break;

  case 83: /* use_stmt: access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1135 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7166 "bison-chpl-lib.cpp"
    break;

  case 84: /* use_stmt: access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1142 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7178 "bison-chpl-lib.cpp"
    break;

  case 85: /* use_stmt: access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1150 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7191 "bison-chpl-lib.cpp"
    break;

  case 86: /* import_stmt: access_control TIMPORT import_ls TSEMI  */
#line 1162 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7199 "bison-chpl-lib.cpp"
    break;

  case 87: /* import_expr: expr  */
#line 1169 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)));
  }
#line 7207 "bison-chpl-lib.cpp"
    break;

  case 88: /* import_expr: expr TDOT all_op_name  */
#line 1173 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-2].expr)),
                                        VisibilityClause::NONE,
                                        context->consume(ident));
  }
#line 7218 "bison-chpl-lib.cpp"
    break;

  case 89: /* import_expr: expr TAS ident_use  */
#line 1180 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as));
  }
#line 7228 "bison-chpl-lib.cpp"
    break;

  case 90: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1186 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)));
  }
#line 7238 "bison-chpl-lib.cpp"
    break;

  case 91: /* import_ls: import_expr  */
#line 1194 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7244 "bison-chpl-lib.cpp"
    break;

  case 92: /* import_ls: import_ls TCOMMA import_expr  */
#line 1195 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7250 "bison-chpl-lib.cpp"
    break;

  case 93: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1200 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7260 "bison-chpl-lib.cpp"
    break;

  case 94: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1209 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7268 "bison-chpl-lib.cpp"
    break;

  case 95: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1213 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7276 "bison-chpl-lib.cpp"
    break;

  case 96: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1217 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7284 "bison-chpl-lib.cpp"
    break;

  case 97: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1221 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7292 "bison-chpl-lib.cpp"
    break;

  case 98: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1225 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7300 "bison-chpl-lib.cpp"
    break;

  case 99: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1229 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7308 "bison-chpl-lib.cpp"
    break;

  case 100: /* opt_label_ident: %empty  */
#line 1237 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7314 "bison-chpl-lib.cpp"
    break;

  case 101: /* opt_label_ident: TIDENT  */
#line 1238 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7320 "bison-chpl-lib.cpp"
    break;

  case 102: /* ident_fn_def: TIDENT  */
#line 1242 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7326 "bison-chpl-lib.cpp"
    break;

  case 103: /* ident_fn_def: TNONE  */
#line 1243 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'none'"); }
#line 7332 "bison-chpl-lib.cpp"
    break;

  case 104: /* ident_fn_def: TTHIS  */
#line 1244 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7338 "bison-chpl-lib.cpp"
    break;

  case 105: /* ident_fn_def: TFALSE  */
#line 1245 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'false'"); }
#line 7344 "bison-chpl-lib.cpp"
    break;

  case 106: /* ident_fn_def: TTRUE  */
#line 1246 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'true'"); }
#line 7350 "bison-chpl-lib.cpp"
    break;

  case 107: /* ident_fn_def: internal_type_ident_def  */
#line 1247 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word"); }
#line 7356 "bison-chpl-lib.cpp"
    break;

  case 108: /* ident_def: TIDENT  */
#line 1250 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7362 "bison-chpl-lib.cpp"
    break;

  case 109: /* ident_def: TNONE  */
#line 1251 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'none'"); }
#line 7368 "bison-chpl-lib.cpp"
    break;

  case 110: /* ident_def: TTHIS  */
#line 1252 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'this'"); }
#line 7374 "bison-chpl-lib.cpp"
    break;

  case 111: /* ident_def: TFALSE  */
#line 1253 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'false'"); }
#line 7380 "bison-chpl-lib.cpp"
    break;

  case 112: /* ident_def: TTRUE  */
#line 1254 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'true'"); }
#line 7386 "bison-chpl-lib.cpp"
    break;

  case 113: /* ident_def: internal_type_ident_def  */
#line 1255 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word"); }
#line 7392 "bison-chpl-lib.cpp"
    break;

  case 114: /* ident_use: TIDENT  */
#line 1267 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7398 "bison-chpl-lib.cpp"
    break;

  case 115: /* ident_use: TTHIS  */
#line 1268 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7404 "bison-chpl-lib.cpp"
    break;

  case 135: /* scalar_type: TBOOL  */
#line 1301 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7410 "bison-chpl-lib.cpp"
    break;

  case 136: /* scalar_type: TENUM  */
#line 1302 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7416 "bison-chpl-lib.cpp"
    break;

  case 137: /* scalar_type: TINT  */
#line 1303 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7422 "bison-chpl-lib.cpp"
    break;

  case 138: /* scalar_type: TUINT  */
#line 1304 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7428 "bison-chpl-lib.cpp"
    break;

  case 139: /* scalar_type: TREAL  */
#line 1305 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7434 "bison-chpl-lib.cpp"
    break;

  case 140: /* scalar_type: TIMAG  */
#line 1306 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7440 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TCOMPLEX  */
#line 1307 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7446 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TBYTES  */
#line 1308 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7452 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TSTRING  */
#line 1309 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7458 "bison-chpl-lib.cpp"
    break;

  case 144: /* scalar_type: TLOCALE  */
#line 1310 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7464 "bison-chpl-lib.cpp"
    break;

  case 145: /* scalar_type: TNOTHING  */
#line 1311 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7470 "bison-chpl-lib.cpp"
    break;

  case 146: /* scalar_type: TVOID  */
#line 1312 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7476 "bison-chpl-lib.cpp"
    break;

  case 151: /* do_stmt: TDO stmt  */
#line 1326 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7482 "bison-chpl-lib.cpp"
    break;

  case 152: /* do_stmt: block_stmt  */
#line 1327 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7488 "bison-chpl-lib.cpp"
    break;

  case 153: /* return_stmt: TRETURN TSEMI  */
#line 1332 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7499 "bison-chpl-lib.cpp"
    break;

  case 154: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1339 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7510 "bison-chpl-lib.cpp"
    break;

  case 156: /* $@3: %empty  */
#line 1350 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 7518 "bison-chpl-lib.cpp"
    break;

  case 157: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL $@3 class_level_stmt  */
#line 1354 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7526 "bison-chpl-lib.cpp"
    break;

  case 158: /* $@4: %empty  */
#line 1358 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 7534 "bison-chpl-lib.cpp"
    break;

  case 159: /* deprecated_class_level_stmt: TDEPRECATED $@4 class_level_stmt  */
#line 1362 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7542 "bison-chpl-lib.cpp"
    break;

  case 160: /* class_level_stmt: TSEMI  */
#line 1369 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), nullptr);
    }
#line 7550 "bison-chpl-lib.cpp"
    break;

  case 161: /* class_level_stmt: inner_class_level_stmt  */
#line 1373 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7560 "bison-chpl-lib.cpp"
    break;

  case 162: /* $@5: %empty  */
#line 1378 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7567 "bison-chpl-lib.cpp"
    break;

  case 163: /* class_level_stmt: TPUBLIC $@5 inner_class_level_stmt  */
#line 1380 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7576 "bison-chpl-lib.cpp"
    break;

  case 164: /* $@6: %empty  */
#line 1384 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7583 "bison-chpl-lib.cpp"
    break;

  case 165: /* class_level_stmt: TPRIVATE $@6 inner_class_level_stmt  */
#line 1386 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7592 "bison-chpl-lib.cpp"
    break;

  case 173: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1404 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attribute)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7601 "bison-chpl-lib.cpp"
    break;

  case 174: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1409 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7610 "bison-chpl-lib.cpp"
    break;

  case 175: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1414 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7619 "bison-chpl-lib.cpp"
    break;

  case 176: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1419 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attribute)), (yyvsp[0].commentsAndStmt));
    }
#line 7627 "bison-chpl-lib.cpp"
    break;

  case 177: /* forwarding_decl_start: TFORWARDING  */
#line 1426 "chpl.ypp"
  {
    (yyval.attribute) = context->buildAttributes((yyloc)).release();
    context->resetAttributePartsState();
  }
#line 7636 "bison-chpl-lib.cpp"
    break;

  case 178: /* extern_or_export: TEXTERN  */
#line 1433 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7642 "bison-chpl-lib.cpp"
    break;

  case 179: /* extern_or_export: TEXPORT  */
#line 1434 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7648 "bison-chpl-lib.cpp"
    break;

  case 180: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1439 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7658 "bison-chpl-lib.cpp"
    break;

  case 181: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1449 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7667 "bison-chpl-lib.cpp"
    break;

  case 182: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1455 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7678 "bison-chpl-lib.cpp"
    break;

  case 183: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1462 "chpl.ypp"
  {
    auto loc = context->declStartLoc((yylsp[-2]));

    Expression* optExpr = (yyvsp[-1].expr);
    FunctionParts fp = (yyvsp[0].functionParts);

    auto commentList = context->appendList(context->makeList(),
                                           fp.comments);

    auto comments = context->gatherCommentsFromList(commentList, loc);
    delete commentList;

    fp.linkage = (yyvsp[-2].linkageTag);
    fp.comments = comments;
    fp.linkageNameExpr = optExpr;

    (yyval.commentsAndStmt) = context->buildFunctionDecl((yyloc), fp);
  }
#line 7701 "bison-chpl-lib.cpp"
    break;

  case 184: /* $@7: %empty  */
#line 1481 "chpl.ypp"
  {
    // This will be consumed when building the first variable in the list.
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7710 "bison-chpl-lib.cpp"
    break;

  case 185: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@7 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1486 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7719 "bison-chpl-lib.cpp"
    break;

  case 186: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1494 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7727 "bison-chpl-lib.cpp"
    break;

  case 187: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1501 "chpl.ypp"
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
#line 7749 "bison-chpl-lib.cpp"
    break;

  case 188: /* loop_stmt: TWHILE expr do_stmt  */
#line 1519 "chpl.ypp"
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
#line 7766 "bison-chpl-lib.cpp"
    break;

  case 189: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1532 "chpl.ypp"
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
#line 7783 "bison-chpl-lib.cpp"
    break;

  case 190: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1545 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7791 "bison-chpl-lib.cpp"
    break;

  case 191: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1549 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7799 "bison-chpl-lib.cpp"
    break;

  case 192: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1553 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7807 "bison-chpl-lib.cpp"
    break;

  case 193: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1557 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7815 "bison-chpl-lib.cpp"
    break;

  case 194: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1561 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7823 "bison-chpl-lib.cpp"
    break;

  case 195: /* loop_stmt: TFOR expr do_stmt  */
#line 1565 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7831 "bison-chpl-lib.cpp"
    break;

  case 196: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1569 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7839 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1573 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-5]), (yylsp[0]), (yyvsp[0].blockOrDo));
    Expression* ident = context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr));
    auto index = context->buildLoopIndexDecl((yylsp[-3]), toOwned(ident));
    auto body = context->consumeToBlock((yylsp[0]), exprLst);
    auto node = For::build(BUILDER, LOC((yylsp[-5])), std::move(index),
                           toOwned((yyvsp[-1].expr)),
                           blockStyle,
                           std::move(body),
                           /*isExpressionLevel*/ false,
                           /*isParam*/ true);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7861 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1591 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7869 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1595 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7877 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1599 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7885 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1603 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7893 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFORALL expr do_stmt  */
#line 1607 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7901 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1611 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7909 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1615 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7917 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1619 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7925 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1623 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7933 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1627 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7941 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1631 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7949 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1635 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7957 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1639 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7965 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1643 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7973 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7981 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1651 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7989 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1655 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7997 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1659 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8005 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1663 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8013 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1667 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8021 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1671 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8029 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1675 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8037 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1679 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8046 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1684 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8055 "bison-chpl-lib.cpp"
    break;

  case 222: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1692 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8065 "bison-chpl-lib.cpp"
    break;

  case 223: /* if_stmt: TIF expr TTHEN stmt  */
#line 1701 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8073 "bison-chpl-lib.cpp"
    break;

  case 224: /* if_stmt: TIF expr block_stmt  */
#line 1705 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8081 "bison-chpl-lib.cpp"
    break;

  case 225: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1709 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8089 "bison-chpl-lib.cpp"
    break;

  case 226: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1713 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8097 "bison-chpl-lib.cpp"
    break;

  case 227: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1717 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8105 "bison-chpl-lib.cpp"
    break;

  case 228: /* if_stmt: TIF ifvar block_stmt  */
#line 1721 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8113 "bison-chpl-lib.cpp"
    break;

  case 229: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1725 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8121 "bison-chpl-lib.cpp"
    break;

  case 230: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1729 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8129 "bison-chpl-lib.cpp"
    break;

  case 231: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1733 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8138 "bison-chpl-lib.cpp"
    break;

  case 232: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1738 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8147 "bison-chpl-lib.cpp"
    break;

  case 233: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1743 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8156 "bison-chpl-lib.cpp"
    break;

  case 234: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1748 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8165 "bison-chpl-lib.cpp"
    break;

  case 235: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1756 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                   /*attributes*/ nullptr,
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
#line 8184 "bison-chpl-lib.cpp"
    break;

  case 236: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1771 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                   /*attributes*/ nullptr,
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
#line 8203 "bison-chpl-lib.cpp"
    break;

  case 237: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1789 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8209 "bison-chpl-lib.cpp"
    break;

  case 238: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1791 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8215 "bison-chpl-lib.cpp"
    break;

  case 239: /* ifc_formal_ls: ifc_formal  */
#line 1795 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8221 "bison-chpl-lib.cpp"
    break;

  case 240: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1796 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8227 "bison-chpl-lib.cpp"
    break;

  case 241: /* ifc_formal: ident_def  */
#line 1801 "chpl.ypp"
             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8233 "bison-chpl-lib.cpp"
    break;

  case 254: /* implements_type_ident: implements_type_error_ident  */
#line 1819 "chpl.ypp"
  {
    std::string s = "type ";
    s += "'"; s += (yyvsp[0].uniqueStr).c_str(); s += "'";
    s += " not allowed to implement an interface";
    context->noteError((yyloc), s);
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8245 "bison-chpl-lib.cpp"
    break;

  case 261: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1848 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8251 "bison-chpl-lib.cpp"
    break;

  case 262: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1850 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8257 "bison-chpl-lib.cpp"
    break;

  case 263: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1852 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8263 "bison-chpl-lib.cpp"
    break;

  case 264: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1857 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8269 "bison-chpl-lib.cpp"
    break;

  case 265: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1859 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8275 "bison-chpl-lib.cpp"
    break;

  case 266: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1861 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8281 "bison-chpl-lib.cpp"
    break;

  case 267: /* try_stmt: TTRY expr TSEMI  */
#line 1866 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[-1].expr), false);
  }
#line 8289 "bison-chpl-lib.cpp"
    break;

  case 268: /* try_stmt: TTRYBANG expr TSEMI  */
#line 1870 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[-1].expr), true);
  }
#line 8297 "bison-chpl-lib.cpp"
    break;

  case 269: /* try_stmt: TTRY assignment_stmt  */
#line 1874 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8305 "bison-chpl-lib.cpp"
    break;

  case 270: /* try_stmt: TTRYBANG assignment_stmt  */
#line 1878 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8313 "bison-chpl-lib.cpp"
    break;

  case 271: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1882 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8321 "bison-chpl-lib.cpp"
    break;

  case 272: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1886 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8329 "bison-chpl-lib.cpp"
    break;

  case 273: /* catch_expr_ls: %empty  */
#line 1892 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8335 "bison-chpl-lib.cpp"
    break;

  case 274: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1893 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8341 "bison-chpl-lib.cpp"
    break;

  case 275: /* catch_expr: TCATCH block_stmt  */
#line 1898 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8349 "bison-chpl-lib.cpp"
    break;

  case 276: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1902 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8357 "bison-chpl-lib.cpp"
    break;

  case 277: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1906 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8365 "bison-chpl-lib.cpp"
    break;

  case 278: /* catch_expr_inner: ident_def  */
#line 1913 "chpl.ypp"
  {
    (yyval.expr) = Variable::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
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
#line 8382 "bison-chpl-lib.cpp"
    break;

  case 279: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1926 "chpl.ypp"
  {
    (yyval.expr) = Variable::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
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
#line 8399 "bison-chpl-lib.cpp"
    break;

  case 280: /* throw_stmt: TTHROW expr TSEMI  */
#line 1942 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8409 "bison-chpl-lib.cpp"
    break;

  case 281: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1950 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8417 "bison-chpl-lib.cpp"
    break;

  case 282: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1954 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8427 "bison-chpl-lib.cpp"
    break;

  case 283: /* when_stmt_ls: %empty  */
#line 1962 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8433 "bison-chpl-lib.cpp"
    break;

  case 284: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1963 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8439 "bison-chpl-lib.cpp"
    break;

  case 285: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1968 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8447 "bison-chpl-lib.cpp"
    break;

  case 286: /* when_stmt: TOTHERWISE stmt  */
#line 1972 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8456 "bison-chpl-lib.cpp"
    break;

  case 287: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1977 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8465 "bison-chpl-lib.cpp"
    break;

  case 288: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1987 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8474 "bison-chpl-lib.cpp"
    break;

  case 289: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 1992 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8485 "bison-chpl-lib.cpp"
    break;

  case 290: /* class_start: class_tag ident_def  */
#line 2003 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8493 "bison-chpl-lib.cpp"
    break;

  case 291: /* class_tag: TCLASS  */
#line 2009 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8499 "bison-chpl-lib.cpp"
    break;

  case 292: /* class_tag: TRECORD  */
#line 2010 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8505 "bison-chpl-lib.cpp"
    break;

  case 293: /* class_tag: TUNION  */
#line 2011 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8511 "bison-chpl-lib.cpp"
    break;

  case 294: /* opt_inherit: %empty  */
#line 2015 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8517 "bison-chpl-lib.cpp"
    break;

  case 295: /* opt_inherit: TCOLON expr_ls  */
#line 2016 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8523 "bison-chpl-lib.cpp"
    break;

  case 296: /* class_level_stmt_ls: %empty  */
#line 2020 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8532 "bison-chpl-lib.cpp"
    break;

  case 297: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 2025 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8540 "bison-chpl-lib.cpp"
    break;

  case 298: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 2029 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8548 "bison-chpl-lib.cpp"
    break;

  case 299: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2036 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      ParserExprList* list = (yyvsp[-1].exprList);
      // get any comments after the last element but before the closing brace
      context->appendList(list, context->gatherComments((yylsp[0])));

      auto decl = Enum::build(BUILDER, LOC((yyloc)), toOwned(parts.attributes),
                              parts.visibility,
                              parts.name,
                              context->consumeList(list));
      CommentsAndStmt cs = {parts.comments, decl.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8569 "bison-chpl-lib.cpp"
    break;

  case 300: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2053 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8583 "bison-chpl-lib.cpp"
    break;

  case 301: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2066 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8591 "bison-chpl-lib.cpp"
    break;

  case 302: /* enum_ls: deprecated_enum_item  */
#line 2073 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8600 "bison-chpl-lib.cpp"
    break;

  case 303: /* enum_ls: enum_ls TCOMMA  */
#line 2078 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8610 "bison-chpl-lib.cpp"
    break;

  case 304: /* $@8: %empty  */
#line 2084 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8619 "bison-chpl-lib.cpp"
    break;

  case 305: /* enum_ls: enum_ls TCOMMA $@8 deprecated_enum_item  */
#line 2089 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8628 "bison-chpl-lib.cpp"
    break;

  case 307: /* $@9: %empty  */
#line 2098 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 8636 "bison-chpl-lib.cpp"
    break;

  case 308: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL $@9 enum_item  */
#line 2102 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8644 "bison-chpl-lib.cpp"
    break;

  case 309: /* $@10: %empty  */
#line 2106 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 8652 "bison-chpl-lib.cpp"
    break;

  case 310: /* deprecated_enum_item: TDEPRECATED $@10 enum_item  */
#line 2110 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8660 "bison-chpl-lib.cpp"
    break;

  case 311: /* enum_item: ident_def  */
#line 2117 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8671 "bison-chpl-lib.cpp"
    break;

  case 312: /* enum_item: ident_def TASSIGN expr  */
#line 2124 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8684 "bison-chpl-lib.cpp"
    break;

  case 313: /* lambda_decl_expr: TLAMBDA req_formal_ls opt_ret_tag opt_type opt_lifetime_where function_body_stmt  */
#line 2136 "chpl.ypp"
  {
    // TODO (dlongnecke): Leave this unimplemented for now? Since we will
    // be addressing FCFs in 1.26...
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 8694 "bison-chpl-lib.cpp"
    break;

  case 315: /* linkage_spec: linkage_spec_empty  */
#line 2147 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8700 "bison-chpl-lib.cpp"
    break;

  case 316: /* linkage_spec: TINLINE  */
#line 2148 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8707 "bison-chpl-lib.cpp"
    break;

  case 317: /* linkage_spec: TOVERRIDE  */
#line 2150 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8714 "bison-chpl-lib.cpp"
    break;

  case 318: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2156 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8722 "bison-chpl-lib.cpp"
    break;

  case 319: /* $@11: %empty  */
#line 2165 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name);
    }
#line 8732 "bison-chpl-lib.cpp"
    break;

  case 320: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@11 opt_function_body_stmt  */
#line 2171 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-6].functionParts);
      fp.returnIntent = (yyvsp[-5].returnTag);
      fp.returnType = (yyvsp[-4].expr);
      fp.throws = ((yyvsp[-3].throwsTag) == ThrowsTag_THROWS) ? true : false;
      WhereAndLifetime wl = (yyvsp[-2].lifetimeAndWhere);
      fp.body = (yyvsp[0].exprList);

      fp.lifetime = wl.lifetime;
      fp.where = wl.where;

      context->clearComments();
      context->exitScope(asttags::Function, (yyvsp[-6].functionParts).name);

      (yyval.functionParts) = fp;
    }
#line 8753 "bison-chpl-lib.cpp"
    break;

  case 321: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2191 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8765 "bison-chpl-lib.cpp"
    break;

  case 322: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2199 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8777 "bison-chpl-lib.cpp"
    break;

  case 323: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2207 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.receiver = Formal::build(BUILDER, LOC((yylsp[-3])), /*attributes*/ nullptr,
                                  STR("this"), (yyvsp[-4].intentTag), toOwned((yyvsp[-3].expr)),
                                  nullptr).release();
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8792 "bison-chpl-lib.cpp"
    break;

  case 324: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2218 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.receiver = Formal::build(BUILDER, LOC((yylsp[-3])), /*attributes*/ nullptr,
                                  STR("this"), (yyvsp[-4].intentTag), toOwned((yyvsp[-3].expr)),
                                  nullptr).release();
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8807 "bison-chpl-lib.cpp"
    break;

  case 325: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2229 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8817 "bison-chpl-lib.cpp"
    break;

  case 326: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2238 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-1].functionParts);
      auto loc = context->declStartLoc((yylsp[0]));
      fp.comments = context->gatherComments(loc);
      fp.attributes = context->buildAttributes((yyloc)).release();
      context->resetAttributePartsState();
      fp.kind = (yyvsp[0].functionKind);
      (yyval.functionParts) = fp;
    }
#line 8831 "bison-chpl-lib.cpp"
    break;

  case 328: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2251 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8837 "bison-chpl-lib.cpp"
    break;

  case 331: /* fn_ident: ident_def TBANG  */
#line 2258 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8847 "bison-chpl-lib.cpp"
    break;

  case 372: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2316 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8853 "bison-chpl-lib.cpp"
    break;

  case 373: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2317 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8859 "bison-chpl-lib.cpp"
    break;

  case 374: /* formal_var_arg_expr: TDOTDOTDOT query_expr  */
#line 2318 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8865 "bison-chpl-lib.cpp"
    break;

  case 375: /* opt_formal_ls: %empty  */
#line 2322 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8871 "bison-chpl-lib.cpp"
    break;

  case 376: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2323 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8877 "bison-chpl-lib.cpp"
    break;

  case 377: /* req_formal_ls: TLP formal_ls TRP  */
#line 2327 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8883 "bison-chpl-lib.cpp"
    break;

  case 378: /* formal_ls_inner: formal  */
#line 2331 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8889 "bison-chpl-lib.cpp"
    break;

  case 379: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2332 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8895 "bison-chpl-lib.cpp"
    break;

  case 380: /* formal_ls: %empty  */
#line 2336 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8901 "bison-chpl-lib.cpp"
    break;

  case 381: /* formal_ls: formal_ls_inner  */
#line 2337 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8907 "bison-chpl-lib.cpp"
    break;

  case 382: /* formal: opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2342 "chpl.ypp"
  {
    (yyval.expr) = Formal::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
                       /*name*/ (yyvsp[-2].uniqueStr),
                       /*intent*/ (yyvsp[-3].intentTag),
                       toOwned((yyvsp[-1].expr)),
                       toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 8920 "bison-chpl-lib.cpp"
    break;

  case 383: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2351 "chpl.ypp"
  {
    auto attributes = context->buildAttributes((yyloc));
    (yyval.expr) = Formal::build(BUILDER, LOC((yyloc)), std::move(attributes),
                       /*name*/ (yyvsp[-2].uniqueStr),
                       /*intent*/ (yyvsp[-3].intentTag),
                       toOwned((yyvsp[-1].expr)),
                       toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
    context->resetAttributePartsState();
  }
#line 8935 "bison-chpl-lib.cpp"
    break;

  case 384: /* formal: opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
#line 2362 "chpl.ypp"
  {
    (yyval.expr) = VarArgFormal::build(BUILDER, LOC((yyloc)),
                             /*attributes*/ nullptr,
                             /*name*/ (yyvsp[-2].uniqueStr),
                             /*intent*/ (yyvsp[-3].intentTag),
                             toOwned((yyvsp[-1].expr)),
                             toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 8949 "bison-chpl-lib.cpp"
    break;

  case 385: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
#line 2372 "chpl.ypp"
  {
    auto attributes = context->buildAttributes((yyloc));
    (yyval.expr) = VarArgFormal::build(BUILDER, LOC((yyloc)),
                             std::move(attributes),
                             /*name*/ (yyvsp[-2].uniqueStr),
                             /*intent*/ (yyvsp[-3].intentTag),
                             toOwned((yyvsp[-1].expr)),
                             toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
    context->resetAttributePartsState();
  }
#line 8965 "bison-chpl-lib.cpp"
    break;

  case 386: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 2385 "chpl.ypp"
  {
    (yyval.expr) = TupleDecl::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
                          context->visibility,
                          context->linkage,
                          ((TupleDecl::IntentOrKind) (yyvsp[-5].intentTag)),
                          context->consumeList((yyvsp[-3].exprList)),
                          toOwned((yyvsp[-1].expr)),
                          toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 8980 "bison-chpl-lib.cpp"
    break;

  case 387: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type formal_var_arg_expr  */
#line 2397 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "variable-length argument may not be grouped in a tuple");
  }
#line 8988 "bison-chpl-lib.cpp"
    break;

  case 388: /* opt_formal_intent_tag: %empty  */
#line 2403 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8997 "bison-chpl-lib.cpp"
    break;

  case 389: /* opt_formal_intent_tag: required_intent_tag  */
#line 2408 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9006 "bison-chpl-lib.cpp"
    break;

  case 390: /* required_intent_tag: TIN  */
#line 2415 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9012 "bison-chpl-lib.cpp"
    break;

  case 391: /* required_intent_tag: TINOUT  */
#line 2416 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9018 "bison-chpl-lib.cpp"
    break;

  case 392: /* required_intent_tag: TOUT  */
#line 2417 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9024 "bison-chpl-lib.cpp"
    break;

  case 393: /* required_intent_tag: TCONST TIN  */
#line 2418 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 394: /* required_intent_tag: TCONST TREF  */
#line 2419 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9036 "bison-chpl-lib.cpp"
    break;

  case 395: /* required_intent_tag: TCONST  */
#line 2420 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9042 "bison-chpl-lib.cpp"
    break;

  case 396: /* required_intent_tag: TPARAM  */
#line 2421 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9048 "bison-chpl-lib.cpp"
    break;

  case 397: /* required_intent_tag: TREF  */
#line 2422 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9054 "bison-chpl-lib.cpp"
    break;

  case 398: /* required_intent_tag: TTYPE  */
#line 2423 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9060 "bison-chpl-lib.cpp"
    break;

  case 399: /* opt_this_intent_tag: %empty  */
#line 2427 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9066 "bison-chpl-lib.cpp"
    break;

  case 400: /* opt_this_intent_tag: TPARAM  */
#line 2428 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9072 "bison-chpl-lib.cpp"
    break;

  case 401: /* opt_this_intent_tag: TREF  */
#line 2429 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9078 "bison-chpl-lib.cpp"
    break;

  case 402: /* opt_this_intent_tag: TCONST TREF  */
#line 2430 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9084 "bison-chpl-lib.cpp"
    break;

  case 403: /* opt_this_intent_tag: TCONST  */
#line 2431 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9090 "bison-chpl-lib.cpp"
    break;

  case 404: /* opt_this_intent_tag: TTYPE  */
#line 2432 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9096 "bison-chpl-lib.cpp"
    break;

  case 405: /* proc_iter_or_op: TPROC  */
#line 2436 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9102 "bison-chpl-lib.cpp"
    break;

  case 406: /* proc_iter_or_op: TITER  */
#line 2437 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9108 "bison-chpl-lib.cpp"
    break;

  case 407: /* proc_iter_or_op: TOPERATOR  */
#line 2438 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9114 "bison-chpl-lib.cpp"
    break;

  case 408: /* opt_ret_tag: %empty  */
#line 2442 "chpl.ypp"
              { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9120 "bison-chpl-lib.cpp"
    break;

  case 409: /* opt_ret_tag: TCONST  */
#line 2443 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST; }
#line 9126 "bison-chpl-lib.cpp"
    break;

  case 410: /* opt_ret_tag: TCONST TREF  */
#line 2444 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST_REF; }
#line 9132 "bison-chpl-lib.cpp"
    break;

  case 411: /* opt_ret_tag: TREF  */
#line 2445 "chpl.ypp"
              { (yyval.returnTag) = Function::REF; }
#line 9138 "bison-chpl-lib.cpp"
    break;

  case 412: /* opt_ret_tag: TPARAM  */
#line 2446 "chpl.ypp"
              { (yyval.returnTag) = Function::PARAM; }
#line 9144 "bison-chpl-lib.cpp"
    break;

  case 413: /* opt_ret_tag: TTYPE  */
#line 2447 "chpl.ypp"
              { (yyval.returnTag) = Function::TYPE; }
#line 9150 "bison-chpl-lib.cpp"
    break;

  case 414: /* opt_throws_error: %empty  */
#line 2451 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9156 "bison-chpl-lib.cpp"
    break;

  case 415: /* opt_throws_error: TTHROWS  */
#line 2452 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9162 "bison-chpl-lib.cpp"
    break;

  case 416: /* opt_function_body_stmt: TSEMI  */
#line 2455 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9168 "bison-chpl-lib.cpp"
    break;

  case 417: /* opt_function_body_stmt: function_body_stmt  */
#line 2456 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9174 "bison-chpl-lib.cpp"
    break;

  case 418: /* function_body_stmt: block_stmt_body  */
#line 2460 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9180 "bison-chpl-lib.cpp"
    break;

  case 419: /* function_body_stmt: return_stmt  */
#line 2461 "chpl.ypp"
                    { context->clearComments(); (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 9186 "bison-chpl-lib.cpp"
    break;

  case 420: /* query_expr: TQUERIEDIDENT  */
#line 2465 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9192 "bison-chpl-lib.cpp"
    break;

  case 421: /* opt_lifetime_where: %empty  */
#line 2470 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9198 "bison-chpl-lib.cpp"
    break;

  case 422: /* opt_lifetime_where: TWHERE expr  */
#line 2472 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9204 "bison-chpl-lib.cpp"
    break;

  case 423: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2474 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9210 "bison-chpl-lib.cpp"
    break;

  case 424: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2476 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9216 "bison-chpl-lib.cpp"
    break;

  case 425: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2478 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9222 "bison-chpl-lib.cpp"
    break;

  case 426: /* lifetime_components_expr: lifetime_expr  */
#line 2483 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9228 "bison-chpl-lib.cpp"
    break;

  case 427: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2485 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9234 "bison-chpl-lib.cpp"
    break;

  case 428: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2490 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9240 "bison-chpl-lib.cpp"
    break;

  case 429: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2492 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9246 "bison-chpl-lib.cpp"
    break;

  case 430: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2494 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9252 "bison-chpl-lib.cpp"
    break;

  case 431: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2496 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9258 "bison-chpl-lib.cpp"
    break;

  case 432: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2498 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9264 "bison-chpl-lib.cpp"
    break;

  case 433: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2500 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9270 "bison-chpl-lib.cpp"
    break;

  case 434: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2502 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9276 "bison-chpl-lib.cpp"
    break;

  case 435: /* lifetime_ident: TIDENT  */
#line 2506 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9282 "bison-chpl-lib.cpp"
    break;

  case 436: /* lifetime_ident: TTHIS  */
#line 2507 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9288 "bison-chpl-lib.cpp"
    break;

  case 437: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2512 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9297 "bison-chpl-lib.cpp"
    break;

  case 438: /* type_alias_decl_stmt_start: TTYPE  */
#line 2520 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9305 "bison-chpl-lib.cpp"
    break;

  case 439: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2524 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9314 "bison-chpl-lib.cpp"
    break;

  case 440: /* type_alias_decl_stmt_start: TEXTERN TTYPE  */
#line 2529 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteLinkage(Decl::EXTERN);
  }
#line 9323 "bison-chpl-lib.cpp"
    break;

  case 441: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2537 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9331 "bison-chpl-lib.cpp"
    break;

  case 442: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner TCOMMA type_alias_decl_stmt_inner_ls  */
#line 2541 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[0].exprList), (yyvsp[-2].commentsAndStmt));
  }
#line 9339 "bison-chpl-lib.cpp"
    break;

  case 443: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2548 "chpl.ypp"
  {
    // TODO (dlongnecke-cray): Add a helper to build this and var_decl_stmt.
    auto node = Variable::build(BUILDER, LOC((yyloc)),
                                context->buildAttributes((yyloc)),
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
#line 9361 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_init_type: %empty  */
#line 2568 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9367 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_init_type: TASSIGN type_level_expr  */
#line 2570 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9373 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_init_type: TASSIGN array_type  */
#line 2572 "chpl.ypp"
  {
    // Cannot be a type_level_expr as expr inherits from type_level_expr.
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 9382 "bison-chpl-lib.cpp"
    break;

  case 447: /* var_decl_type: TPARAM  */
#line 2579 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9388 "bison-chpl-lib.cpp"
    break;

  case 448: /* var_decl_type: TCONST TREF  */
#line 2580 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9394 "bison-chpl-lib.cpp"
    break;

  case 449: /* var_decl_type: TREF  */
#line 2581 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9400 "bison-chpl-lib.cpp"
    break;

  case 450: /* var_decl_type: TCONST  */
#line 2582 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9406 "bison-chpl-lib.cpp"
    break;

  case 451: /* var_decl_type: TVAR  */
#line 2583 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9412 "bison-chpl-lib.cpp"
    break;

  case 452: /* $@12: %empty  */
#line 2588 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9421 "bison-chpl-lib.cpp"
    break;

  case 453: /* var_decl_stmt: TCONFIG $@12 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2592 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9430 "bison-chpl-lib.cpp"
    break;

  case 454: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2597 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9439 "bison-chpl-lib.cpp"
    break;

  case 455: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2605 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9447 "bison-chpl-lib.cpp"
    break;

  case 456: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2609 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9455 "bison-chpl-lib.cpp"
    break;

  case 457: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2616 "chpl.ypp"
    {
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
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
#line 9475 "bison-chpl-lib.cpp"
    break;

  case 458: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2632 "chpl.ypp"
    {
      auto intentOrKind = (TupleDecl::IntentOrKind) context->varDeclKind;
      auto tupleDecl = TupleDecl::build(BUILDER, LOC((yyloc)),
                                        context->buildAttributes((yyloc)),
                                        context->visibility,
                                        context->linkage,
                                        intentOrKind,
                                        context->consumeList((yyvsp[-3].exprList)),
                                        toOwned((yyvsp[-1].expr)),
                                        toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yylsp[-4]), tupleDecl.release());
      context->clearComments();
    }
#line 9493 "bison-chpl-lib.cpp"
    break;

  case 459: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2649 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9501 "bison-chpl-lib.cpp"
    break;

  case 460: /* tuple_var_decl_component: ident_def  */
#line 2653 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9509 "bison-chpl-lib.cpp"
    break;

  case 461: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2657 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9517 "bison-chpl-lib.cpp"
    break;

  case 462: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2664 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9523 "bison-chpl-lib.cpp"
    break;

  case 463: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2666 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9529 "bison-chpl-lib.cpp"
    break;

  case 464: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2668 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9535 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_init_expr: %empty  */
#line 2674 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9541 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2675 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9547 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2676 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9553 "bison-chpl-lib.cpp"
    break;

  case 468: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 2682 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9561 "bison-chpl-lib.cpp"
    break;

  case 469: /* ret_array_type: TLSBR TRSBR  */
#line 2686 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), nullptr, nullptr);
  }
#line 9569 "bison-chpl-lib.cpp"
    break;

  case 470: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2690 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9577 "bison-chpl-lib.cpp"
    break;

  case 471: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 2694 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), (yyvsp[-1].exprList), /*typeExpr*/ nullptr);
  }
#line 9585 "bison-chpl-lib.cpp"
    break;

  case 472: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 2698 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9593 "bison-chpl-lib.cpp"
    break;

  case 473: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 2702 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9601 "bison-chpl-lib.cpp"
    break;

  case 474: /* ret_array_type: TLSBR error TRSBR  */
#line 2706 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "invalid expression for domain of array return type");
  }
#line 9609 "bison-chpl-lib.cpp"
    break;

  case 475: /* opt_ret_type: %empty  */
#line 2712 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9615 "bison-chpl-lib.cpp"
    break;

  case 476: /* opt_ret_type: TCOLON type_level_expr  */
#line 2713 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9621 "bison-chpl-lib.cpp"
    break;

  case 477: /* opt_ret_type: TCOLON ret_array_type  */
#line 2714 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9627 "bison-chpl-lib.cpp"
    break;

  case 478: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 2715 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9633 "bison-chpl-lib.cpp"
    break;

  case 479: /* opt_ret_type: error  */
#line 2716 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9639 "bison-chpl-lib.cpp"
    break;

  case 480: /* opt_type: %empty  */
#line 2721 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9645 "bison-chpl-lib.cpp"
    break;

  case 481: /* opt_type: TCOLON type_level_expr  */
#line 2722 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9651 "bison-chpl-lib.cpp"
    break;

  case 482: /* opt_type: TCOLON array_type  */
#line 2723 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9657 "bison-chpl-lib.cpp"
    break;

  case 483: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2724 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9663 "bison-chpl-lib.cpp"
    break;

  case 484: /* opt_type: error  */
#line 2725 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9669 "bison-chpl-lib.cpp"
    break;

  case 485: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2746 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9677 "bison-chpl-lib.cpp"
    break;

  case 486: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 2750 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9685 "bison-chpl-lib.cpp"
    break;

  case 487: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 2754 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9693 "bison-chpl-lib.cpp"
    break;

  case 488: /* array_type: TLSBR error TRSBR  */
#line 2758 "chpl.ypp"
  {
    (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release();
  }
#line 9701 "bison-chpl-lib.cpp"
    break;

  case 489: /* opt_formal_array_elt_type: %empty  */
#line 2764 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9707 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_formal_array_elt_type: type_level_expr  */
#line 2765 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9713 "bison-chpl-lib.cpp"
    break;

  case 491: /* opt_formal_array_elt_type: query_expr  */
#line 2766 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9719 "bison-chpl-lib.cpp"
    break;

  case 492: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 2771 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9728 "bison-chpl-lib.cpp"
    break;

  case 493: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 2776 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9736 "bison-chpl-lib.cpp"
    break;

  case 494: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 2784 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9745 "bison-chpl-lib.cpp"
    break;

  case 495: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 2789 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9753 "bison-chpl-lib.cpp"
    break;

  case 496: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 2793 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9761 "bison-chpl-lib.cpp"
    break;

  case 497: /* opt_formal_type: %empty  */
#line 2799 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9767 "bison-chpl-lib.cpp"
    break;

  case 498: /* opt_formal_type: TCOLON type_level_expr  */
#line 2800 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9773 "bison-chpl-lib.cpp"
    break;

  case 499: /* opt_formal_type: TCOLON query_expr  */
#line 2801 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9779 "bison-chpl-lib.cpp"
    break;

  case 500: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 2802 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9785 "bison-chpl-lib.cpp"
    break;

  case 501: /* opt_formal_type: TCOLON formal_array_type  */
#line 2803 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9791 "bison-chpl-lib.cpp"
    break;

  case 502: /* expr_ls: expr  */
#line 2809 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9797 "bison-chpl-lib.cpp"
    break;

  case 503: /* expr_ls: query_expr  */
#line 2810 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9803 "bison-chpl-lib.cpp"
    break;

  case 504: /* expr_ls: expr_ls TCOMMA expr  */
#line 2811 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9809 "bison-chpl-lib.cpp"
    break;

  case 505: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 2812 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9815 "bison-chpl-lib.cpp"
    break;

  case 506: /* simple_expr_ls: expr  */
#line 2816 "chpl.ypp"
                                   { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9821 "bison-chpl-lib.cpp"
    break;

  case 507: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2817 "chpl.ypp"
                                   { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9827 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_component: TUNDERSCORE  */
#line 2821 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9833 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_component: opt_try_expr  */
#line 2822 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9839 "bison-chpl-lib.cpp"
    break;

  case 510: /* tuple_component: query_expr  */
#line 2823 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9845 "bison-chpl-lib.cpp"
    break;

  case 511: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2828 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9853 "bison-chpl-lib.cpp"
    break;

  case 512: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2832 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9861 "bison-chpl-lib.cpp"
    break;

  case 513: /* opt_actual_ls: %empty  */
#line 2838 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9867 "bison-chpl-lib.cpp"
    break;

  case 514: /* opt_actual_ls: actual_ls  */
#line 2839 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9873 "bison-chpl-lib.cpp"
    break;

  case 515: /* actual_ls: actual_expr  */
#line 2844 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9882 "bison-chpl-lib.cpp"
    break;

  case 516: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2849 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9892 "bison-chpl-lib.cpp"
    break;

  case 517: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2857 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9898 "bison-chpl-lib.cpp"
    break;

  case 518: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2858 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9904 "bison-chpl-lib.cpp"
    break;

  case 519: /* actual_expr: query_expr  */
#line 2859 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9910 "bison-chpl-lib.cpp"
    break;

  case 520: /* actual_expr: opt_try_expr  */
#line 2860 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9916 "bison-chpl-lib.cpp"
    break;

  case 521: /* ident_expr: ident_use  */
#line 2864 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9922 "bison-chpl-lib.cpp"
    break;

  case 522: /* ident_expr: scalar_type  */
#line 2865 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9928 "bison-chpl-lib.cpp"
    break;

  case 523: /* type_level_expr: sub_type_level_expr  */
#line 2877 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9934 "bison-chpl-lib.cpp"
    break;

  case 524: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2879 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 9940 "bison-chpl-lib.cpp"
    break;

  case 525: /* type_level_expr: TQUESTION  */
#line 2881 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 9946 "bison-chpl-lib.cpp"
    break;

  case 531: /* sub_type_level_expr: TSINGLE expr  */
#line 2892 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9952 "bison-chpl-lib.cpp"
    break;

  case 532: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2894 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9958 "bison-chpl-lib.cpp"
    break;

  case 533: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2896 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9964 "bison-chpl-lib.cpp"
    break;

  case 534: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2898 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9970 "bison-chpl-lib.cpp"
    break;

  case 535: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2900 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9980 "bison-chpl-lib.cpp"
    break;

  case 536: /* sub_type_level_expr: TATOMIC expr  */
#line 2906 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9986 "bison-chpl-lib.cpp"
    break;

  case 537: /* sub_type_level_expr: TSYNC expr  */
#line 2908 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9992 "bison-chpl-lib.cpp"
    break;

  case 538: /* sub_type_level_expr: TOWNED  */
#line 2911 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9998 "bison-chpl-lib.cpp"
    break;

  case 539: /* sub_type_level_expr: TOWNED expr  */
#line 2913 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10004 "bison-chpl-lib.cpp"
    break;

  case 540: /* sub_type_level_expr: TUNMANAGED  */
#line 2915 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10010 "bison-chpl-lib.cpp"
    break;

  case 541: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2917 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10016 "bison-chpl-lib.cpp"
    break;

  case 542: /* sub_type_level_expr: TSHARED  */
#line 2919 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10022 "bison-chpl-lib.cpp"
    break;

  case 543: /* sub_type_level_expr: TSHARED expr  */
#line 2921 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10028 "bison-chpl-lib.cpp"
    break;

  case 544: /* sub_type_level_expr: TBORROWED  */
#line 2923 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10034 "bison-chpl-lib.cpp"
    break;

  case 545: /* sub_type_level_expr: TBORROWED expr  */
#line 2925 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10040 "bison-chpl-lib.cpp"
    break;

  case 546: /* sub_type_level_expr: TCLASS  */
#line 2928 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10046 "bison-chpl-lib.cpp"
    break;

  case 547: /* sub_type_level_expr: TRECORD  */
#line 2930 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10052 "bison-chpl-lib.cpp"
    break;

  case 548: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2935 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10065 "bison-chpl-lib.cpp"
    break;

  case 549: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2944 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10078 "bison-chpl-lib.cpp"
    break;

  case 550: /* for_expr: TFOR expr TDO expr  */
#line 2953 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[0].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 551: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 2961 "chpl.ypp"
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
                    /*isParam*/ false).release();
  }
#line 10108 "bison-chpl-lib.cpp"
    break;

  case 552: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2975 "chpl.ypp"
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
                    /*isParam*/ false).release();
  }
#line 10126 "bison-chpl-lib.cpp"
    break;

  case 553: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 2989 "chpl.ypp"
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
                    /*isParam*/ false).release();
  }
#line 10144 "bison-chpl-lib.cpp"
    break;

  case 554: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3003 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10157 "bison-chpl-lib.cpp"
    break;

  case 555: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3012 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10170 "bison-chpl-lib.cpp"
    break;

  case 556: /* for_expr: TFORALL expr TDO expr  */
#line 3021 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[0].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10182 "bison-chpl-lib.cpp"
    break;

  case 557: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3029 "chpl.ypp"
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
#line 10200 "bison-chpl-lib.cpp"
    break;

  case 558: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3043 "chpl.ypp"
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
#line 10218 "bison-chpl-lib.cpp"
    break;

  case 559: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3057 "chpl.ypp"
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
#line 10236 "bison-chpl-lib.cpp"
    break;

  case 560: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 3071 "chpl.ypp"
  {
    if ((yyvsp[-2].exprList)->size() > 1) {
      const char* msg = "Invalid iterand expression";
      (yyval.expr) = context->raiseError((yylsp[-2]), msg);
    } else {
      auto iterandExpr = context->consumeList((yyvsp[-2].exprList))[0].release();
      assert(iterandExpr);
      (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), /*index*/ nullptr,
                              toOwned((yyvsp[0].expr)),
                              /*withClause*/ nullptr,
                              BlockStyle::IMPLICIT,
                              context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                              /*isExpressionLevel*/ true).release();
    }
  }
#line 10256 "bison-chpl-lib.cpp"
    break;

  case 561: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3087 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10269 "bison-chpl-lib.cpp"
    break;

  case 562: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3096 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10282 "bison-chpl-lib.cpp"
    break;

  case 563: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3105 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-7]), (yyvsp[-7].exprList));
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index),
                            toOwned((yyvsp[-5].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock(ifLoc, ifExpr.release()),
                            /*isExpressionLevel*/ true).release();
  }
#line 10301 "bison-chpl-lib.cpp"
    break;

  case 564: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3120 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-7]), (yyvsp[-7].exprList));
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index),
                            toOwned((yyvsp[-5].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock(ifLoc, ifExpr.release()),
                            /*isExpressionLevel*/ true).release();
  }
#line 10320 "bison-chpl-lib.cpp"
    break;

  case 565: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3138 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10334 "bison-chpl-lib.cpp"
    break;

  case 566: /* nil_expr: TNIL  */
#line 3155 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10340 "bison-chpl-lib.cpp"
    break;

  case 574: /* stmt_level_expr: io_expr TIO expr  */
#line 3171 "chpl.ypp"
  { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10346 "bison-chpl-lib.cpp"
    break;

  case 575: /* opt_task_intent_ls: %empty  */
#line 3175 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10352 "bison-chpl-lib.cpp"
    break;

  case 576: /* opt_task_intent_ls: task_intent_clause  */
#line 3176 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10358 "bison-chpl-lib.cpp"
    break;

  case 577: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3181 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10368 "bison-chpl-lib.cpp"
    break;

  case 578: /* task_intent_ls: intent_expr  */
#line 3189 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10374 "bison-chpl-lib.cpp"
    break;

  case 579: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3190 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10380 "bison-chpl-lib.cpp"
    break;

  case 580: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3195 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10390 "bison-chpl-lib.cpp"
    break;

  case 581: /* forall_intent_ls: intent_expr  */
#line 3203 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10396 "bison-chpl-lib.cpp"
    break;

  case 582: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3204 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10402 "bison-chpl-lib.cpp"
    break;

  case 583: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3209 "chpl.ypp"
  {
    if (auto ident = (yyvsp[-2].expr)->toIdentifier()) {
      auto name = ident->name();
      auto node = TaskVar::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
                                 name,
                                 /*intent*/ (yyvsp[-3].taskIntent),
                                 toOwned((yyvsp[-1].expr)),
                                 toOwned((yyvsp[0].expr)));
      (yyval.expr) = node.release();
    } else {
      const char* msg = "Expected identifier for task variable name";
      (yyval.expr) = context->raiseError((yyloc), msg);
    }
  }
#line 10421 "bison-chpl-lib.cpp"
    break;

  case 584: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3224 "chpl.ypp"
  {
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 10429 "bison-chpl-lib.cpp"
    break;

  case 585: /* intent_expr: expr TREDUCE ident_expr  */
#line 3228 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10437 "bison-chpl-lib.cpp"
    break;

  case 586: /* task_var_prefix: TCONST  */
#line 3234 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10443 "bison-chpl-lib.cpp"
    break;

  case 587: /* task_var_prefix: TIN  */
#line 3235 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10449 "bison-chpl-lib.cpp"
    break;

  case 588: /* task_var_prefix: TCONST TIN  */
#line 3236 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10455 "bison-chpl-lib.cpp"
    break;

  case 589: /* task_var_prefix: TREF  */
#line 3237 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10461 "bison-chpl-lib.cpp"
    break;

  case 590: /* task_var_prefix: TCONST TREF  */
#line 3238 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10467 "bison-chpl-lib.cpp"
    break;

  case 591: /* task_var_prefix: TVAR  */
#line 3239 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10473 "bison-chpl-lib.cpp"
    break;

  case 593: /* io_expr: io_expr TIO expr  */
#line 3245 "chpl.ypp"
  { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10479 "bison-chpl-lib.cpp"
    break;

  case 594: /* new_maybe_decorated: TNEW  */
#line 3250 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10485 "bison-chpl-lib.cpp"
    break;

  case 595: /* new_maybe_decorated: TNEW TOWNED  */
#line 3252 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10491 "bison-chpl-lib.cpp"
    break;

  case 596: /* new_maybe_decorated: TNEW TSHARED  */
#line 3254 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10497 "bison-chpl-lib.cpp"
    break;

  case 597: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3256 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10503 "bison-chpl-lib.cpp"
    break;

  case 598: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3258 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10509 "bison-chpl-lib.cpp"
    break;

  case 599: /* new_expr: new_maybe_decorated expr  */
#line 3264 "chpl.ypp"
  {
    if (FnCall* fnCall = (yyvsp[0].expr)->toFnCall()) {
      (yyval.expr) = context->wrapCalledExpressionInNew((yyloc), (yyvsp[-1].newManagement), fnCall);
    } else {

      // It's an error for one reason or another. TODO: Specialize these
      // errors later (e.g. 'new a.field' would require parens around
      // the expression 'a.field'; 'new foo' would require an argument
      // list for 'foo'; and something like 'new __primitive()' just
      // doesn't make any sense...
      (yyval.expr) = context->raiseError((yyloc), "Invalid form for new expression");
    }
  }
#line 10527 "bison-chpl-lib.cpp"
    break;

  case 600: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3281 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10535 "bison-chpl-lib.cpp"
    break;

  case 601: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3285 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10543 "bison-chpl-lib.cpp"
    break;

  case 602: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3289 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10551 "bison-chpl-lib.cpp"
    break;

  case 603: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3293 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10559 "bison-chpl-lib.cpp"
    break;

  case 604: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3300 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10565 "bison-chpl-lib.cpp"
    break;

  case 614: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 3317 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10573 "bison-chpl-lib.cpp"
    break;

  case 615: /* expr: expr TCOLON expr  */
#line 3321 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10581 "bison-chpl-lib.cpp"
    break;

  case 616: /* expr: expr TDOTDOT expr  */
#line 3325 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10590 "bison-chpl-lib.cpp"
    break;

  case 617: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 3330 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10599 "bison-chpl-lib.cpp"
    break;

  case 618: /* expr: expr TDOTDOT  */
#line 3335 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10608 "bison-chpl-lib.cpp"
    break;

  case 619: /* expr: TDOTDOT expr  */
#line 3340 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10618 "bison-chpl-lib.cpp"
    break;

  case 620: /* expr: TDOTDOTOPENHIGH expr  */
#line 3346 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10628 "bison-chpl-lib.cpp"
    break;

  case 621: /* expr: TDOTDOT  */
#line 3352 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10638 "bison-chpl-lib.cpp"
    break;

  case 622: /* opt_expr: %empty  */
#line 3383 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10644 "bison-chpl-lib.cpp"
    break;

  case 623: /* opt_expr: expr  */
#line 3384 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10650 "bison-chpl-lib.cpp"
    break;

  case 624: /* opt_try_expr: TTRY expr  */
#line 3388 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10656 "bison-chpl-lib.cpp"
    break;

  case 625: /* opt_try_expr: TTRYBANG expr  */
#line 3389 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10662 "bison-chpl-lib.cpp"
    break;

  case 626: /* opt_try_expr: expr  */
#line 3390 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10668 "bison-chpl-lib.cpp"
    break;

  case 632: /* call_base_expr: expr TBANG  */
#line 3407 "chpl.ypp"
                                { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10674 "bison-chpl-lib.cpp"
    break;

  case 633: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3408 "chpl.ypp"
                                { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10680 "bison-chpl-lib.cpp"
    break;

  case 636: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3415 "chpl.ypp"
    {
      ASTList actuals;
      std::vector<UniqueString> actualNames;
      context->consumeNamedActuals((yyvsp[-1].maybeNamedActualList), actuals, actualNames);
      auto fnCall = FnCall::build(BUILDER, LOC((yyloc)),
                                  toOwned((yyvsp[-3].expr)),
                                  std::move(actuals),
                                  std::move(actualNames),
                                  /* square */ false);
      (yyval.expr) = fnCall.release();
    }
#line 10696 "bison-chpl-lib.cpp"
    break;

  case 637: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3427 "chpl.ypp"
    {
      ASTList actuals;
      std::vector<UniqueString> actualNames;
      context->consumeNamedActuals((yyvsp[-1].maybeNamedActualList), actuals, actualNames);
      auto fnCall = FnCall::build(BUILDER, LOC((yyloc)),
                                  toOwned((yyvsp[-3].expr)),
                                  std::move(actuals),
                                  std::move(actualNames),
                                  /* square */ true);
      (yyval.expr) = fnCall.release();
    }
#line 10712 "bison-chpl-lib.cpp"
    break;

  case 638: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3439 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10720 "bison-chpl-lib.cpp"
    break;

  case 639: /* dot_expr: expr TDOT ident_use  */
#line 3446 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10726 "bison-chpl-lib.cpp"
    break;

  case 640: /* dot_expr: expr TDOT TTYPE  */
#line 3448 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10732 "bison-chpl-lib.cpp"
    break;

  case 641: /* dot_expr: expr TDOT TDOMAIN  */
#line 3450 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10738 "bison-chpl-lib.cpp"
    break;

  case 642: /* dot_expr: expr TDOT TLOCALE  */
#line 3452 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10744 "bison-chpl-lib.cpp"
    break;

  case 643: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3454 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10754 "bison-chpl-lib.cpp"
    break;

  case 644: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3460 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10764 "bison-chpl-lib.cpp"
    break;

  case 645: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3472 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10770 "bison-chpl-lib.cpp"
    break;

  case 646: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3474 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10778 "bison-chpl-lib.cpp"
    break;

  case 647: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3478 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10786 "bison-chpl-lib.cpp"
    break;

  case 648: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3482 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10794 "bison-chpl-lib.cpp"
    break;

  case 649: /* bool_literal: TFALSE  */
#line 3488 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10800 "bison-chpl-lib.cpp"
    break;

  case 650: /* bool_literal: TTRUE  */
#line 3489 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10806 "bison-chpl-lib.cpp"
    break;

  case 651: /* str_bytes_literal: STRINGLITERAL  */
#line 3493 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10812 "bison-chpl-lib.cpp"
    break;

  case 652: /* str_bytes_literal: BYTESLITERAL  */
#line 3494 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10818 "bison-chpl-lib.cpp"
    break;

  case 655: /* literal_expr: INTLITERAL  */
#line 3500 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10824 "bison-chpl-lib.cpp"
    break;

  case 656: /* literal_expr: REALLITERAL  */
#line 3501 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10830 "bison-chpl-lib.cpp"
    break;

  case 657: /* literal_expr: IMAGLITERAL  */
#line 3502 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10836 "bison-chpl-lib.cpp"
    break;

  case 658: /* literal_expr: CSTRINGLITERAL  */
#line 3503 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10842 "bison-chpl-lib.cpp"
    break;

  case 659: /* literal_expr: TNONE  */
#line 3504 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10848 "bison-chpl-lib.cpp"
    break;

  case 660: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3506 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10856 "bison-chpl-lib.cpp"
    break;

  case 661: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3510 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10864 "bison-chpl-lib.cpp"
    break;

  case 662: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3514 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10872 "bison-chpl-lib.cpp"
    break;

  case 663: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3518 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10880 "bison-chpl-lib.cpp"
    break;

  case 664: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3522 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10888 "bison-chpl-lib.cpp"
    break;

  case 665: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3526 "chpl.ypp"
  {
    // TODO (dlongnecke): Record trailing comma?
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10897 "bison-chpl-lib.cpp"
    break;

  case 666: /* assoc_expr_ls: expr TALIAS expr  */
#line 3535 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10906 "bison-chpl-lib.cpp"
    break;

  case 667: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3540 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10916 "bison-chpl-lib.cpp"
    break;

  case 668: /* binary_op_expr: expr TPLUS expr  */
#line 3548 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10922 "bison-chpl-lib.cpp"
    break;

  case 669: /* binary_op_expr: expr TMINUS expr  */
#line 3549 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10928 "bison-chpl-lib.cpp"
    break;

  case 670: /* binary_op_expr: expr TSTAR expr  */
#line 3550 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10934 "bison-chpl-lib.cpp"
    break;

  case 671: /* binary_op_expr: expr TDIVIDE expr  */
#line 3551 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10940 "bison-chpl-lib.cpp"
    break;

  case 672: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3552 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10946 "bison-chpl-lib.cpp"
    break;

  case 673: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3553 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10952 "bison-chpl-lib.cpp"
    break;

  case 674: /* binary_op_expr: expr TMOD expr  */
#line 3554 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10958 "bison-chpl-lib.cpp"
    break;

  case 675: /* binary_op_expr: expr TEQUAL expr  */
#line 3555 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10964 "bison-chpl-lib.cpp"
    break;

  case 676: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3556 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10970 "bison-chpl-lib.cpp"
    break;

  case 677: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3557 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10976 "bison-chpl-lib.cpp"
    break;

  case 678: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3558 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10982 "bison-chpl-lib.cpp"
    break;

  case 679: /* binary_op_expr: expr TLESS expr  */
#line 3559 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10988 "bison-chpl-lib.cpp"
    break;

  case 680: /* binary_op_expr: expr TGREATER expr  */
#line 3560 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10994 "bison-chpl-lib.cpp"
    break;

  case 681: /* binary_op_expr: expr TBAND expr  */
#line 3561 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11000 "bison-chpl-lib.cpp"
    break;

  case 682: /* binary_op_expr: expr TBOR expr  */
#line 3562 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11006 "bison-chpl-lib.cpp"
    break;

  case 683: /* binary_op_expr: expr TBXOR expr  */
#line 3563 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11012 "bison-chpl-lib.cpp"
    break;

  case 684: /* binary_op_expr: expr TAND expr  */
#line 3564 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11018 "bison-chpl-lib.cpp"
    break;

  case 685: /* binary_op_expr: expr TOR expr  */
#line 3565 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11024 "bison-chpl-lib.cpp"
    break;

  case 686: /* binary_op_expr: expr TEXP expr  */
#line 3566 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11030 "bison-chpl-lib.cpp"
    break;

  case 687: /* binary_op_expr: expr TBY expr  */
#line 3567 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11036 "bison-chpl-lib.cpp"
    break;

  case 688: /* binary_op_expr: expr TALIGN expr  */
#line 3568 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11042 "bison-chpl-lib.cpp"
    break;

  case 689: /* binary_op_expr: expr THASH expr  */
#line 3569 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11048 "bison-chpl-lib.cpp"
    break;

  case 690: /* binary_op_expr: expr TDMAPPED expr  */
#line 3570 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11054 "bison-chpl-lib.cpp"
    break;

  case 691: /* unary_op_expr: TPLUS expr  */
#line 3574 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11060 "bison-chpl-lib.cpp"
    break;

  case 692: /* unary_op_expr: TMINUS expr  */
#line 3575 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11066 "bison-chpl-lib.cpp"
    break;

  case 693: /* unary_op_expr: TMINUSMINUS expr  */
#line 3576 "chpl.ypp"
                                 { (yyval.expr) = TODOEXPR((yyloc)); /* warn */ }
#line 11072 "bison-chpl-lib.cpp"
    break;

  case 694: /* unary_op_expr: TPLUSPLUS expr  */
#line 3577 "chpl.ypp"
                                 { (yyval.expr) = TODOEXPR((yyloc)); /* warn */ }
#line 11078 "bison-chpl-lib.cpp"
    break;

  case 695: /* unary_op_expr: TBANG expr  */
#line 3578 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11084 "bison-chpl-lib.cpp"
    break;

  case 696: /* unary_op_expr: expr TBANG  */
#line 3579 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11092 "bison-chpl-lib.cpp"
    break;

  case 697: /* unary_op_expr: TBNOT expr  */
#line 3582 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11098 "bison-chpl-lib.cpp"
    break;

  case 698: /* reduce_expr: expr TREDUCE expr  */
#line 3587 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11106 "bison-chpl-lib.cpp"
    break;

  case 699: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3591 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11114 "bison-chpl-lib.cpp"
    break;

  case 700: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3595 "chpl.ypp"
  {
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11122 "bison-chpl-lib.cpp"
    break;

  case 701: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3599 "chpl.ypp"
  {
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11130 "bison-chpl-lib.cpp"
    break;

  case 702: /* scan_expr: expr TSCAN expr  */
#line 3606 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11138 "bison-chpl-lib.cpp"
    break;

  case 703: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3610 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11146 "bison-chpl-lib.cpp"
    break;

  case 704: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3614 "chpl.ypp"
  {
    (yyval.expr) = Scan::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11154 "bison-chpl-lib.cpp"
    break;

  case 705: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3618 "chpl.ypp"
  {
    (yyval.expr) = Scan::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11162 "bison-chpl-lib.cpp"
    break;


#line 11166 "bison-chpl-lib.cpp"

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
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, context, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
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
