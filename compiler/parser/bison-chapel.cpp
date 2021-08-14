/* A Bison parser, made by GNU Bison 3.7.6.  */

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
#define YYBISON 30706

/* Bison version string.  */
#define YYBISON_VERSION "3.7.6"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 1

/* Pull parsers.  */
#define YYPULL 0





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

#include "bison-chapel.h"
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
  YYSYMBOL_TMANAGE = 64,                   /* TMANAGE  */
  YYSYMBOL_TMINUSMINUS = 65,               /* TMINUSMINUS  */
  YYSYMBOL_TMODULE = 66,                   /* TMODULE  */
  YYSYMBOL_TNEW = 67,                      /* TNEW  */
  YYSYMBOL_TNIL = 68,                      /* TNIL  */
  YYSYMBOL_TNOINIT = 69,                   /* TNOINIT  */
  YYSYMBOL_TNONE = 70,                     /* TNONE  */
  YYSYMBOL_TNOTHING = 71,                  /* TNOTHING  */
  YYSYMBOL_TON = 72,                       /* TON  */
  YYSYMBOL_TONLY = 73,                     /* TONLY  */
  YYSYMBOL_TOPERATOR = 74,                 /* TOPERATOR  */
  YYSYMBOL_TOTHERWISE = 75,                /* TOTHERWISE  */
  YYSYMBOL_TOUT = 76,                      /* TOUT  */
  YYSYMBOL_TOVERRIDE = 77,                 /* TOVERRIDE  */
  YYSYMBOL_TOWNED = 78,                    /* TOWNED  */
  YYSYMBOL_TPARAM = 79,                    /* TPARAM  */
  YYSYMBOL_TPLUSPLUS = 80,                 /* TPLUSPLUS  */
  YYSYMBOL_TPRAGMA = 81,                   /* TPRAGMA  */
  YYSYMBOL_TPRIMITIVE = 82,                /* TPRIMITIVE  */
  YYSYMBOL_TPRIVATE = 83,                  /* TPRIVATE  */
  YYSYMBOL_TPROC = 84,                     /* TPROC  */
  YYSYMBOL_TPROTOTYPE = 85,                /* TPROTOTYPE  */
  YYSYMBOL_TPUBLIC = 86,                   /* TPUBLIC  */
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
  YYSYMBOL_TBANG = 140,                    /* TBANG  */
  YYSYMBOL_TBAND = 141,                    /* TBAND  */
  YYSYMBOL_TBNOT = 142,                    /* TBNOT  */
  YYSYMBOL_TBOR = 143,                     /* TBOR  */
  YYSYMBOL_TBXOR = 144,                    /* TBXOR  */
  YYSYMBOL_TCOLON = 145,                   /* TCOLON  */
  YYSYMBOL_TCOMMA = 146,                   /* TCOMMA  */
  YYSYMBOL_TDIVIDE = 147,                  /* TDIVIDE  */
  YYSYMBOL_TDOT = 148,                     /* TDOT  */
  YYSYMBOL_TDOTDOT = 149,                  /* TDOTDOT  */
  YYSYMBOL_TDOTDOTDOT = 150,               /* TDOTDOTDOT  */
  YYSYMBOL_TEQUAL = 151,                   /* TEQUAL  */
  YYSYMBOL_TEXP = 152,                     /* TEXP  */
  YYSYMBOL_TGREATER = 153,                 /* TGREATER  */
  YYSYMBOL_TGREATEREQUAL = 154,            /* TGREATEREQUAL  */
  YYSYMBOL_THASH = 155,                    /* THASH  */
  YYSYMBOL_TIO = 156,                      /* TIO  */
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
  YYSYMBOL_deprecated_decl_stmt = 187,     /* deprecated_decl_stmt  */
  YYSYMBOL_deprecated_decl_base = 188,     /* deprecated_decl_base  */
  YYSYMBOL_module_decl_start = 189,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 190,         /* module_decl_stmt  */
  YYSYMBOL_access_control = 191,           /* access_control  */
  YYSYMBOL_opt_prototype = 192,            /* opt_prototype  */
  YYSYMBOL_include_access_control = 193,   /* include_access_control  */
  YYSYMBOL_include_module_stmt = 194,      /* include_module_stmt  */
  YYSYMBOL_195_1 = 195,                    /* $@1  */
  YYSYMBOL_block_stmt = 196,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 197,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 198,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 199,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 200,              /* opt_only_ls  */
  YYSYMBOL_except_ls = 201,                /* except_ls  */
  YYSYMBOL_use_access_control = 202,       /* use_access_control  */
  YYSYMBOL_use_stmt = 203,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 204,              /* import_stmt  */
  YYSYMBOL_import_expr = 205,              /* import_expr  */
  YYSYMBOL_import_ls = 206,                /* import_ls  */
  YYSYMBOL_require_stmt = 207,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 208,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 209,          /* opt_label_ident  */
  YYSYMBOL_ident_fn_def = 210,             /* ident_fn_def  */
  YYSYMBOL_ident_def = 211,                /* ident_def  */
  YYSYMBOL_ident_use = 212,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 213,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 214,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 215,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 216,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 217,              /* return_stmt  */
  YYSYMBOL_manager_expr = 218,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 219,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 220,              /* manage_stmt  */
  YYSYMBOL_deprecated_class_level_stmt = 221, /* deprecated_class_level_stmt  */
  YYSYMBOL_class_level_stmt = 222,         /* class_level_stmt  */
  YYSYMBOL_223_2 = 223,                    /* @2  */
  YYSYMBOL_private_decl = 224,             /* private_decl  */
  YYSYMBOL_forwarding_stmt = 225,          /* forwarding_stmt  */
  YYSYMBOL_extern_export_decl_stmt = 226,  /* extern_export_decl_stmt  */
  YYSYMBOL_227_3 = 227,                    /* $@3  */
  YYSYMBOL_228_4 = 228,                    /* $@4  */
  YYSYMBOL_229_5 = 229,                    /* $@5  */
  YYSYMBOL_230_6 = 230,                    /* $@6  */
  YYSYMBOL_231_7 = 231,                    /* $@7  */
  YYSYMBOL_232_8 = 232,                    /* $@8  */
  YYSYMBOL_extern_block_stmt = 233,        /* extern_block_stmt  */
  YYSYMBOL_loop_stmt = 234,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 235,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 236,                  /* if_stmt  */
  YYSYMBOL_ifvar = 237,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 238,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 239,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 240,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 241,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 242, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 243,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 244,           /* ifc_constraint  */
  YYSYMBOL_defer_stmt = 245,               /* defer_stmt  */
  YYSYMBOL_try_stmt = 246,                 /* try_stmt  */
  YYSYMBOL_catch_stmt_ls = 247,            /* catch_stmt_ls  */
  YYSYMBOL_catch_stmt = 248,               /* catch_stmt  */
  YYSYMBOL_catch_expr = 249,               /* catch_expr  */
  YYSYMBOL_throw_stmt = 250,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 251,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 252,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 253,                /* when_stmt  */
  YYSYMBOL_class_decl_stmt = 254,          /* class_decl_stmt  */
  YYSYMBOL_class_tag = 255,                /* class_tag  */
  YYSYMBOL_opt_inherit = 256,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 257,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 258,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header = 259,              /* enum_header  */
  YYSYMBOL_enum_ls = 260,                  /* enum_ls  */
  YYSYMBOL_deprecated_enum_item = 261,     /* deprecated_enum_item  */
  YYSYMBOL_enum_item = 262,                /* enum_item  */
  YYSYMBOL_lambda_decl_expr = 263,         /* lambda_decl_expr  */
  YYSYMBOL_264_9 = 264,                    /* $@9  */
  YYSYMBOL_265_10 = 265,                   /* $@10  */
  YYSYMBOL_linkage_spec = 266,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt = 267,             /* fn_decl_stmt  */
  YYSYMBOL_268_11 = 268,                   /* $@11  */
  YYSYMBOL_269_12 = 269,                   /* $@12  */
  YYSYMBOL_fn_decl_stmt_inner = 270,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_receiver_expr = 271,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 272,                 /* fn_ident  */
  YYSYMBOL_assignop_ident = 273,           /* assignop_ident  */
  YYSYMBOL_opt_formal_ls = 274,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 275,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 276,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 277,                /* formal_ls  */
  YYSYMBOL_formal = 278,                   /* formal  */
  YYSYMBOL_opt_intent_tag = 279,           /* opt_intent_tag  */
  YYSYMBOL_required_intent_tag = 280,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 281,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 282,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 283,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 284,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 285,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 286,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 287,               /* query_expr  */
  YYSYMBOL_var_arg_expr = 288,             /* var_arg_expr  */
  YYSYMBOL_opt_lifetime_where = 289,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 290, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 291,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 292,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 293,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_inner = 294, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 295,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 296,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 297,            /* var_decl_stmt  */
  YYSYMBOL_var_decl_stmt_inner_ls = 298,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 299,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 300, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 301, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 302,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 303,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 304,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 305,                 /* opt_type  */
  YYSYMBOL_array_type = 306,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 307, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 308,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 309,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 310,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 311,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 312,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 313,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 314,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 315,                /* actual_ls  */
  YYSYMBOL_actual_expr = 316,              /* actual_expr  */
  YYSYMBOL_ident_expr = 317,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 318,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 319,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 320,                 /* for_expr  */
  YYSYMBOL_cond_expr = 321,                /* cond_expr  */
  YYSYMBOL_nil_expr = 322,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 323,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 324,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 325,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 326,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 327,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 328,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 329,              /* intent_expr  */
  YYSYMBOL_shadow_var_prefix = 330,        /* shadow_var_prefix  */
  YYSYMBOL_io_expr = 331,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 332,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 333,                 /* new_expr  */
  YYSYMBOL_let_expr = 334,                 /* let_expr  */
  YYSYMBOL_expr = 335,                     /* expr  */
  YYSYMBOL_opt_expr = 336,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 337,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 338,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 339,           /* call_base_expr  */
  YYSYMBOL_call_expr = 340,                /* call_expr  */
  YYSYMBOL_dot_expr = 341,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 342,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 343,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 344,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 345,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 346,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 347,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 348,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 349,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 350,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 351       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 39 "chapel.ypp"

  #include <string>
  int         captureTokens;
  std::string captureString;
  bool        parsingPrivate=false;
#line 214 "chapel.ypp"

  #include "build.h"
  #include "CatchStmt.h"
  #include "DeferStmt.h"
  #include "DoWhileStmt.h"
  #include "driver.h"
  #include "flex-chapel.h"
  #include "ForallStmt.h"
  #include "ForLoop.h"
  #include "IfExpr.h"
  #include "misc.h"
  #include "parser.h"
  #include "stmt.h"
  #include "stringutil.h"
  #include "TryStmt.h"
  #include "vec.h"
  #include "WhileDoStmt.h"

  #include <cstdio>
  #include <cstdlib>
  #include <cstring>
  #include <stdint.h>

  #define YYLLOC_DEFAULT(Current, Rhs, N)                                 \
    if (N) {                                                              \
      (Current).first_line   = (Rhs)[1].first_line;                       \
      (Current).first_column = (Rhs)[1].first_column;                     \
      (Current).last_line    = (Rhs)[N].last_line;                        \
      (Current).last_column  = (Rhs)[N].last_column;                      \
      (Current).comment      = NULL;                                      \
                                                                          \
      if ((Current).first_line)                                           \
        yystartlineno = (Current).first_line;                             \
                                                                          \
    } else  {                                                             \
      (Current) = yylloc;                                                 \
    }

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str) {

    // like USR_FATAL_CONT
    setupError("parser", __FILE__, __LINE__, 3);

    // TODO -- should this begin with error:
    if (!chplParseString) {
      const char* yyText = yyget_text(context->scanner);
      fprintf(stderr, "%s:%d: %s", yyfilename, chplLineno, str);

      if (strlen(yyText) > 0) {
        fprintf(stderr, ": near '%s'", yyText);
      }
    } else {
      fprintf(stderr, "%s: %s", yyfilename, str);

      if (chplParseStringMsg && (strlen(chplParseStringMsg) > 0)) {
        fprintf(stderr, " %s", chplParseStringMsg);
      }
    }

    fprintf(stderr, "\n");
  }

#line 526 "bison-chapel.cpp"

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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

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
#define YYLAST   21054

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  171
/* YYNRULES -- Number of rules.  */
#define YYNRULES  712
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1283

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

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   518,   518,   523,   524,   530,   531,   536,   537,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
     573,   574,   575,   579,   580,   582,   587,   588,   592,   605,
     610,   615,   623,   624,   625,   629,   630,   634,   635,   636,
     641,   640,   661,   662,   663,   668,   669,   674,   679,   684,
     688,   697,   702,   707,   712,   716,   720,   728,   733,   737,
     742,   746,   747,   748,   752,   753,   754,   755,   756,   757,
     758,   762,   767,   768,   769,   773,   774,   778,   782,   784,
     786,   788,   790,   792,   799,   800,   804,   805,   806,   807,
     808,   809,   812,   813,   814,   815,   816,   817,   829,   830,
     841,   842,   843,   844,   845,   846,   847,   848,   849,   850,
     851,   852,   853,   854,   855,   856,   857,   858,   859,   863,
     864,   865,   866,   867,   868,   869,   870,   871,   872,   873,
     874,   881,   882,   883,   884,   888,   889,   893,   894,   898,
     899,   900,   904,   905,   909,   913,   914,   916,   921,   922,
     923,   933,   933,   938,   939,   940,   941,   942,   943,   944,
     948,   949,   950,   951,   956,   955,   972,   971,   989,   988,
    1005,  1004,  1022,  1021,  1037,  1036,  1052,  1056,  1061,  1069,
    1080,  1087,  1088,  1089,  1090,  1091,  1092,  1093,  1094,  1095,
    1096,  1097,  1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,
    1106,  1107,  1108,  1109,  1110,  1116,  1122,  1128,  1134,  1141,
    1148,  1152,  1159,  1163,  1164,  1165,  1166,  1168,  1169,  1170,
    1171,  1173,  1175,  1177,  1179,  1184,  1185,  1189,  1191,  1199,
    1200,  1205,  1210,  1211,  1212,  1213,  1214,  1215,  1216,  1217,
    1218,  1219,  1220,  1221,  1222,  1229,  1230,  1231,  1232,  1241,
    1242,  1246,  1248,  1251,  1257,  1259,  1262,  1268,  1271,  1272,
    1273,  1274,  1275,  1276,  1280,  1281,  1285,  1286,  1287,  1291,
    1292,  1296,  1299,  1301,  1306,  1307,  1311,  1313,  1315,  1322,
    1332,  1346,  1351,  1356,  1364,  1365,  1370,  1371,  1373,  1378,
    1394,  1401,  1410,  1418,  1422,  1429,  1430,  1432,  1437,  1438,
    1443,  1448,  1442,  1475,  1478,  1482,  1490,  1500,  1489,  1539,
    1543,  1548,  1552,  1557,  1564,  1565,  1569,  1570,  1571,  1572,
    1573,  1574,  1575,  1576,  1577,  1578,  1579,  1580,  1581,  1582,
    1583,  1584,  1585,  1586,  1587,  1588,  1589,  1590,  1591,  1592,
    1593,  1594,  1595,  1596,  1600,  1601,  1602,  1603,  1604,  1605,
    1606,  1607,  1608,  1609,  1610,  1611,  1615,  1616,  1620,  1624,
    1625,  1629,  1630,  1634,  1636,  1638,  1640,  1642,  1644,  1649,
    1650,  1654,  1655,  1656,  1657,  1658,  1659,  1660,  1661,  1662,
    1666,  1667,  1668,  1669,  1670,  1671,  1675,  1676,  1677,  1681,
    1682,  1683,  1684,  1685,  1686,  1690,  1691,  1694,  1695,  1699,
    1700,  1704,  1708,  1709,  1710,  1718,  1719,  1721,  1723,  1725,
    1730,  1732,  1737,  1738,  1739,  1740,  1741,  1742,  1743,  1747,
    1749,  1754,  1756,  1758,  1763,  1776,  1793,  1794,  1796,  1801,
    1802,  1803,  1804,  1805,  1809,  1815,  1823,  1824,  1832,  1834,
    1839,  1841,  1843,  1848,  1850,  1852,  1859,  1860,  1861,  1866,
    1868,  1870,  1874,  1878,  1880,  1884,  1892,  1893,  1894,  1895,
    1896,  1901,  1902,  1903,  1904,  1905,  1925,  1929,  1933,  1941,
    1948,  1949,  1950,  1954,  1956,  1962,  1964,  1966,  1971,  1972,
    1973,  1974,  1975,  1981,  1982,  1983,  1984,  1988,  1989,  1993,
    1994,  1995,  1999,  2000,  2004,  2005,  2009,  2010,  2014,  2015,
    2016,  2017,  2021,  2022,  2033,  2035,  2037,  2043,  2044,  2045,
    2046,  2047,  2048,  2050,  2052,  2054,  2056,  2058,  2060,  2063,
    2065,  2067,  2069,  2071,  2073,  2075,  2077,  2080,  2082,  2087,
    2089,  2091,  2093,  2095,  2097,  2099,  2101,  2103,  2105,  2107,
    2109,  2111,  2118,  2124,  2130,  2136,  2145,  2155,  2163,  2164,
    2165,  2166,  2167,  2168,  2169,  2170,  2175,  2176,  2180,  2184,
    2185,  2189,  2193,  2194,  2198,  2202,  2206,  2213,  2214,  2215,
    2216,  2217,  2218,  2222,  2223,  2228,  2230,  2234,  2238,  2242,
    2250,  2255,  2261,  2267,  2274,  2284,  2292,  2293,  2294,  2295,
    2296,  2297,  2298,  2299,  2300,  2301,  2303,  2305,  2307,  2322,
    2324,  2326,  2328,  2333,  2334,  2338,  2339,  2340,  2344,  2345,
    2346,  2347,  2356,  2357,  2358,  2359,  2360,  2364,  2365,  2366,
    2370,  2371,  2372,  2373,  2374,  2382,  2383,  2384,  2385,  2389,
    2390,  2394,  2395,  2399,  2400,  2401,  2402,  2403,  2404,  2405,
    2406,  2408,  2410,  2411,  2412,  2416,  2424,  2425,  2429,  2430,
    2431,  2432,  2433,  2434,  2435,  2436,  2437,  2438,  2439,  2440,
    2441,  2442,  2443,  2444,  2445,  2446,  2447,  2448,  2449,  2450,
    2451,  2456,  2457,  2458,  2459,  2460,  2461,  2462,  2466,  2467,
    2468,  2469,  2473,  2474,  2475,  2476,  2481,  2482,  2483,  2484,
    2485,  2486,  2487
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
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
  "TLAMBDA", "TLET", "TLIFETIME", "TLOCAL", "TLOCALE", "TMANAGE",
  "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT", "TNONE", "TNOTHING",
  "TON", "TONLY", "TOPERATOR", "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED",
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
  "toplevel_stmt", "pragma_ls", "stmt", "deprecated_decl_stmt",
  "deprecated_decl_base", "module_decl_start", "module_decl_stmt",
  "access_control", "opt_prototype", "include_access_control",
  "include_module_stmt", "$@1", "block_stmt", "stmt_ls", "renames_ls",
  "use_renames_ls", "opt_only_ls", "except_ls", "use_access_control",
  "use_stmt", "import_stmt", "import_expr", "import_ls", "require_stmt",
  "assignment_stmt", "opt_label_ident", "ident_fn_def", "ident_def",
  "ident_use", "internal_type_ident_def", "scalar_type",
  "reserved_type_ident_use", "do_stmt", "return_stmt", "manager_expr",
  "manager_expr_ls", "manage_stmt", "deprecated_class_level_stmt",
  "class_level_stmt", "@2", "private_decl", "forwarding_stmt",
  "extern_export_decl_stmt", "$@3", "$@4", "$@5", "$@6", "$@7", "$@8",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "defer_stmt", "try_stmt",
  "catch_stmt_ls", "catch_stmt", "catch_expr", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt", "enum_header",
  "enum_ls", "deprecated_enum_item", "enum_item", "lambda_decl_expr",
  "$@9", "$@10", "linkage_spec", "fn_decl_stmt", "$@11", "$@12",
  "fn_decl_stmt_inner", "fn_decl_receiver_expr", "fn_ident",
  "assignop_ident", "opt_formal_ls", "req_formal_ls", "formal_ls_inner",
  "formal_ls", "formal", "opt_intent_tag", "required_intent_tag",
  "opt_this_intent_tag", "proc_iter_or_op", "opt_ret_tag",
  "opt_throws_error", "opt_function_body_stmt", "function_body_stmt",
  "query_expr", "var_arg_expr", "opt_lifetime_where",
  "lifetime_components_expr", "lifetime_expr", "lifetime_ident",
  "type_alias_decl_stmt", "type_alias_decl_stmt_inner", "opt_init_type",
  "var_decl_type", "var_decl_stmt", "var_decl_stmt_inner_ls",
  "var_decl_stmt_inner", "tuple_var_decl_component",
  "tuple_var_decl_stmt_inner_ls", "opt_init_expr", "ret_array_type",
  "opt_ret_type", "opt_type", "array_type", "opt_formal_array_elt_type",
  "formal_array_type", "opt_formal_type", "expr_ls", "simple_expr_ls",
  "tuple_component", "tuple_expr_ls", "opt_actual_ls", "actual_ls",
  "actual_expr", "ident_expr", "type_level_expr", "sub_type_level_expr",
  "for_expr", "cond_expr", "nil_expr", "stmt_level_expr",
  "opt_task_intent_ls", "task_intent_clause", "task_intent_ls",
  "forall_intent_clause", "forall_intent_ls", "intent_expr",
  "shadow_var_prefix", "io_expr", "new_maybe_decorated", "new_expr",
  "let_expr", "expr", "opt_expr", "opt_try_expr", "lhs_expr",
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
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435
};
#endif

#define YYPACT_NINF (-1146)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-663)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1146,   127,  3449, -1146,   -48,   104, -1146, -1146, -1146, -1146,
   -1146, -1146,  4849,    66,   243,   200, 14850,   252, 20503,    66,
   11469,   303,   459,   272,   243,  4849, 11469,  2283,  4849,   247,
   20590, 11642,  7818,   316,  8512,  9904,  9904,  6947,  8685,   373,
   -1146,   282, -1146,   420, 20677, 20677, 20677, -1146,  2852, 10077,
     490, 11469, 11469,    81, -1146,   494,   508, 11469, -1146, 14850,
   -1146, 11469,   479,   415,   359,  1912,   540, 20764, -1146, 10252,
    7991, 11469, 10077, 14850, 11469,   460,   543,   435,  4849,   554,
   11469,   555, 11815, 11815, 20677,   559, -1146, 14850, -1146,   560,
    8685, 11469, -1146, 11469, -1146, 11469, -1146, -1146, 14365, 11469,
   -1146, 11469, -1146, -1146, -1146,  3799,  7122,  8860, 11469, -1146,
    4674, -1146, -1146, -1146,   397, -1146,   534, -1146, -1146,    35,
   -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146,
   -1146, -1146, -1146, -1146, -1146, -1146,   564, -1146, -1146, -1146,
   -1146, -1146, -1146, -1146, -1146, 20677, -1146, 20677,   189,   181,
   -1146, -1146,  2852, -1146,   458, -1146,   487, -1146, -1146,   474,
     492,   507, 11469,   501,   513, 19769, 14358,   426,   514,   518,
   -1146, -1146,   431, -1146, -1146, -1146, -1146, -1146,   350, -1146,
   -1146, 19769,   452,  4849, -1146, -1146,   519, 11469, -1146, -1146,
   11469, 11469, 11469, 20677, -1146, 11469, 10252, 10252,   616,   468,
   -1146, -1146, -1146, -1146,   -33,   475, -1146, -1146,   515, 16549,
   20677,  2852, -1146,   521, -1146,   161, 19769,  2547, -1146, -1146,
    9033,    44,  2480, -1146, -1146,   569,  8166,   601, 20851, 19769,
     414,   312, -1146, 20938, 20677, -1146,   414, 20677,   520,    32,
   16120,     2, 16080,    32, 16160,   393, -1146, 16312, 20677, 20677,
       3, 15320,   171,  8166, -1146, -1146, -1146, -1146, -1146, -1146,
   -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146,
   -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146,   523, -1146,
     434,  4849,   524,   233,   117,    28, -1146,  4849, -1146, -1146,
   16589, -1146,    36, 17098,   704, -1146,   528,   529, -1146, 16589,
     -33,   704, -1146,  8166,  2721, -1146, -1146, -1146,   280, 19769,
   11469, 11469, -1146, 19769,   525, 17205, -1146, 16589,   -33, 19769,
     531,  8166, -1146, 19769, 17250, -1146, -1146, 17290, 14708, -1146,
   -1146, 17442,   566,   541,   -33,    32, 16589, 17482,   308,   308,
    1229,   704,   704,   319, -1146, -1146,  3974,   -23, -1146, 11469,
   -1146,    93,    98, -1146,   -36,   156, 17528,   -38,  1229,   703,
   -1146,  4149, -1146,   646, 11469, 11469, 20677,   570,   549, -1146,
   -1146, -1146, -1146,   304,   478, -1146, 11469,   572, 11469, 11469,
   11469,  9904,  9904, 11469,   482, 11469, 11469, 11469, 11469, 11469,
     432, 14365, 11469, 11469, 11469, 11469, 11469, 11469, 11469, 11469,
   11469, 11469, 11469, 11469, 11469, 11469, 11469, 11469,   652, -1146,
   -1146, -1146, -1146, -1146,  9206,  9206, -1146, -1146, -1146, -1146,
    9206, -1146, -1146,  9206,  9206,  8166,  8166,  9904,  9904,  7645,
   -1146, -1146, 16629, 16781, 17634,   550,    73, 20677,  4324, -1146,
    9904,    32,   561,   332, -1146, 11469, -1146, -1146, 11469,   599,
   -1146,   552,   581, -1146, -1146, -1146, 20677, -1146,  2852, -1146,
   -1146, 20677,   573, 20677, -1146,  2852,   686, 10252, -1146,  5024,
    9904, -1146,   565, -1146,    32,  5199,  9904, -1146,    32, -1146,
    9904, -1146, 11988, 11469, -1146,   614,   615,  4849,   707,  4849,
   -1146,   710, 11469, -1146, -1146,   534,   575,  8166, 20677, -1146,
   -1146,   503, -1146, -1146,   233, -1146,   597,   576, -1146, 12161,
     621, 11469,  2852, -1146, -1146, 11469, -1146, 20228, 11469, 11469,
   -1146,   579, -1146, 10252, -1146, 19769, 19769, -1146,    45, -1146,
    8166,   580, -1146,   734, -1146,   734, -1146, 12334,   611, -1146,
   -1146, -1146, -1146, -1146, -1146, -1146,  9381, -1146, 17680,  7297,
   -1146,  7472, -1146,  4849,   584,  9904,  9556,  3624,   588, 11469,
   10425, -1146, -1146,   406, -1146,  4499, 20677, -1146,   333, 17720,
     408, 16352,   147, 10252,   595, 20127,   410, -1146, 17872,  3241,
    3241,   388, -1146,   388, -1146,   388, 19047,  1030,  1339,   823,
     -33,   308, -1146,   596, -1146, -1146, -1146, -1146, -1146,  1229,
   19579,   388, 14529, 14529,  3241, 14529, 14529,   793,   308, 19579,
   17834,   793,   704,   704,   308,  1229,   605,   606,   608,   619,
     622,   629,   613,   604, -1146,   388, -1146,   388,    63, -1146,
   -1146, -1146,   160, -1146,  1623, 19845,   399, 12507,  9904, 12680,
    9904, 11469,  8166,  9904, 15132,   609,    66, 17917, -1146, -1146,
   -1146, 19769, 17957,  8166, -1146,  8166, 20677,   570,   416, 20677,
   20677,   570, -1146,   570,   424, 11469,   178,  8685, 19769,    39,
   16821,  7645, -1146,  8685, 19769,    21, 16397, -1146,    32, 16589,
     633,   655,   637, 18114,   655,   642, 11469, 11469,   775,  4849,
     781, 18154,  4849, 16861,   752, -1146,   250, -1146,   286, -1146,
     185, -1146, -1146, -1146, -1146, -1146, -1146,   751,   677,   651,
   -1146,  3257, -1146,   504,   656,   233,   117,    24,    50, 11469,
   11469,  6774, -1146, -1146,   610,  8339, -1146, 19769, -1146, -1146,
   -1146, 20677, 18194, 18346, -1146, -1146, 19769,   657,   -25,   661,
   -1146,  1991, -1146, -1146,   430, 20677, -1146, -1146, -1146, -1146,
   -1146, -1146, -1146,  4849,   -30, 17013, -1146, -1146, 19769,  4849,
   19769, -1146, 18387, -1146, -1146, -1146, 11469, -1146,    53,    94,
   11469, -1146, 10598, 11988, 11469, -1146,  8166,   690,  1513,   666,
   20329,   713,    88, -1146, -1146,   749, -1146, -1146, -1146, -1146,
   14197,   672, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146,
   -1146, -1146,  7645, -1146, -1146, -1146, -1146, -1146, -1146, -1146,
   -1146, -1146, -1146, -1146, -1146,    42,  9904,  9904, 11469,   820,
   18428, 11469,   821, 18580,   288,   683, 18620,  8166,    32,    32,
   -1146, -1146, -1146, -1146,   570,   689, -1146,   570,   570,   691,
     692, -1146, 16589, -1146, 15396,  5374, -1146,  5549, -1146,   294,
   -1146, 15472,  5724, -1146,    32,  5899, -1146,    32, -1146, -1146,
   11469, -1146, 11469, -1146, 19769, 19769,  4849, -1146,  4849, 11469,
   -1146,  4849,   828, 20677,   700, 20677,   515, -1146, -1146, 20677,
    1110, -1146,   233,   721,   777, -1146, -1146, -1146,   100, -1146,
   -1146,   621,   693,    85, -1146, -1146, -1146,   705,   709, -1146,
    6074, 10252, -1146, -1146, -1146, 20677, -1146,   730,   515, -1146,
   -1146,  6249,   712,  6424,   715, -1146, 11469, -1146, -1146, 11469,
   18665,    65, 17053,   714,   718,   328,   723,  3140, -1146, 20677,
   -1146, 11469, 20416, -1146, -1146,   504,   724,   -24, -1146,   732,
   -1146,   747,   750,   757,   754,   755, -1146,   760,   765,   761,
     763,   766,   129,   773,   767,   769, -1146, -1146, -1146, -1146,
   -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146,
   -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, 11469,
   -1146,   778,   780,   774,   724,   724, -1146, -1146, -1146,   621,
     336,   348, 18772, 12853, 13026, 18848, 13199, 13372, -1146, 13545,
   13718,   386, -1146, -1146,   753, -1146,   756,   758, -1146, -1146,
   -1146,  4849,  8685, 19769,  8685, 19769,  7645, -1146,  4849,  8685,
   19769, -1146,  8685, 19769, -1146, 18933, 19769, -1146, -1146, 19769,
     865,  4849,   759, -1146, -1146, -1146,   721, -1146,   748, 10773,
     118, -1146,    58, -1146, -1146,  9904, 14991,  8166,  8166,  4849,
   -1146,    67,   771, 11469, -1146,  8685, -1146, 19769,  4849,  8685,
   -1146, 19769,  4849, 19769,   170, 10946, 11988, 11469, 11988, 11469,
   -1146, -1146,   762, -1146,  1780, -1146,  2721, -1146, 18337, -1146,
   -1146, -1146, 19769, -1146,   357,   503, -1146, 19009, -1146, 15237,
   -1146, -1146, -1146, 11469, 11469, 11469, 11469, 11469, 11469, 11469,
   11469, -1146, -1146, 17453, -1146, -1146, 18584, 19788, 18154, 15548,
   15624, -1146, 18154, 15700, 15776, 11469,  4849, -1146, -1146,   118,
     721,  9731, -1146, -1146, -1146,   155, 10252, -1146, -1146,    52,
   11469,   -18, 19085, -1146,   649,   782,   783,   569, -1146,   515,
   19769, 15852, -1146, 15928, -1146, -1146, -1146, 19769,   789,   792,
     797,   800, -1146,  2643, -1146, -1146, -1146, 13891,   824,   785,
   -1146,   791,   806,   724,   724, 19161, 19237, 19313, 19389, 19465,
   19541, 19940, -1146, 20007, 20076, -1146, -1146,  4849,  4849,  4849,
    4849, 19769, -1146, -1146, -1146,   118, 11121,   103, -1146, 19769,
   -1146,    61, -1146,   -15, -1146,   511, 19617, -1146, -1146, -1146,
   13718,   805,   808, -1146,  4849,  4849, -1146, -1146, -1146, -1146,
   -1146,  6599, -1146, -1146,   612, -1146,    58, -1146, -1146, -1146,
   11469, 11469, 11469, 11469, 11469, 11469, -1146, -1146, -1146, 18154,
   18154, 18154, 18154, -1146, -1146, -1146, -1146, -1146,   472,  9904,
   14537, -1146, 11469,    52,    61,    61,    61,    61,    61,    61,
      52,  1124, -1146, -1146, 18154, 18154,   798, 14064,   106,   120,
   19693, -1146, -1146, 19769, -1146, -1146, -1146, -1146, -1146, -1146,
   -1146,   830, -1146, -1146,   626, 14709, -1146, -1146, -1146, 11296,
   -1146,   708, -1146
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   118,   655,   656,   657,   651,
     652,   658,     0,   576,   104,   139,   545,   146,   547,   576,
       0,   145,     0,   450,   104,     0,     0,   323,     0,   269,
     140,   623,   623,   649,     0,     0,     0,     0,     0,   144,
      60,   270,   324,   141,     0,     0,     0,   320,     0,     0,
     148,     0,     0,   595,   567,   659,   149,     0,   325,   539,
     449,     0,     0,     0,   171,   323,   143,   548,   451,     0,
       0,     0,     0,   543,     0,     0,   147,     0,     0,   119,
       0,   650,     0,     0,     0,   142,   303,   541,   453,   150,
       0,     0,   708,     0,   710,     0,   711,   712,   622,     0,
     709,   706,   526,   168,   707,     0,     0,     0,     0,     4,
       0,     5,     9,    43,     0,    46,    55,    10,    11,     0,
      12,    13,    14,    15,   522,   523,    25,    26,    47,   169,
     178,   179,    16,    20,    17,    19,     0,   264,    18,   614,
      22,    23,    24,    21,   177,     0,   175,     0,   611,     0,
     173,   176,     0,   174,   628,   607,   524,   608,   529,   527,
       0,     0,     0,   612,   613,     0,   528,     0,   629,   630,
     631,   653,   654,   606,   531,   530,   609,   610,     0,    42,
      28,   537,     0,     0,   577,   105,     0,     0,   547,   140,
       0,     0,     0,     0,   548,     0,     0,     0,     0,   611,
     628,   527,   612,   613,   546,   528,   629,   630,     0,   576,
       0,     0,   452,     0,   277,     0,   507,   323,   301,   311,
     623,   171,   323,   302,    45,     0,   514,   651,   548,   624,
     323,   651,   200,   548,     0,   188,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   183,     0,     0,     0,
       0,     0,    57,   514,   112,   120,   132,   126,   125,   134,
     115,   124,   135,   121,   136,   113,   137,   130,   123,   131,
     129,   127,   128,   114,   116,   122,   133,   138,     0,   117,
       0,     0,     0,     0,     0,     0,   456,     0,   156,    36,
       0,   162,     0,   161,   693,   599,   596,   597,   598,     0,
     540,   694,     7,   514,   323,   170,   421,   504,     0,   503,
       0,     0,   157,   627,     0,     0,    39,     0,   544,   532,
       0,   514,    40,   538,     0,   284,   280,     0,   528,   284,
     281,     0,   446,     0,   542,     0,     0,     0,   695,   697,
     620,   692,   691,     0,    62,    65,     0,     0,   509,     0,
     511,     0,     0,   510,     0,     0,   503,     0,   621,     0,
       6,     0,    56,     0,     0,     0,     0,   304,     0,   407,
     408,   406,   326,     0,   525,    27,     0,   600,     0,     0,
       0,     0,     0,     0,   696,     0,     0,     0,     0,     0,
       0,   619,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   364,   371,
     372,   373,   368,   370,     0,     0,   366,   369,   367,   365,
       0,   375,   374,     0,     0,   514,   514,     0,     0,     0,
      29,    30,     0,     0,     0,     0,     0,     0,     0,    31,
       0,     0,     0,     0,    32,     0,    33,    44,     0,   522,
     520,     0,   515,   516,   521,   194,     0,   197,     0,   186,
     190,     0,     0,     0,   196,     0,     0,     0,   210,     0,
       0,   209,     0,   218,     0,     0,     0,   216,     0,   223,
       0,   222,     0,    77,   180,     0,     0,     0,   238,     0,
     364,   234,     0,    59,    58,    55,     0,     0,     0,   248,
      34,   389,   321,   460,     0,   461,   463,     0,   485,     0,
     466,     0,     0,   155,    35,     0,   164,     0,     0,     0,
      37,     0,   172,     0,    97,   625,   626,   158,     0,    38,
       0,     0,   291,   282,   278,   283,   279,     0,   444,   441,
     203,   202,    41,    64,    63,    66,     0,   660,     0,     0,
     645,     0,   647,     0,     0,     0,     0,     0,     0,     0,
       0,   664,     8,     0,    49,     0,     0,    95,     0,    92,
       0,    71,   275,     0,     0,     0,   400,   455,   575,   688,
     687,   690,   699,   698,   703,   702,   684,   681,   682,   683,
     616,   671,   118,     0,   642,   643,   119,   641,   640,   617,
     675,   686,   680,   678,   689,   679,   677,   669,   674,   676,
     685,   668,   672,   673,   670,   618,     0,     0,     0,     0,
       0,     0,     0,     0,   701,   700,   705,   704,   587,   588,
     590,   592,     0,   579,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   662,   275,   576,   576,   206,   442,
     454,   508,     0,     0,   534,     0,     0,   304,     0,     0,
       0,   304,   443,   304,     0,     0,     0,     0,   551,     0,
       0,     0,   219,     0,   557,     0,     0,   217,     0,     0,
     707,    80,     0,    67,    78,     0,     0,     0,   237,     0,
     233,     0,     0,     0,     0,   533,     0,   251,     0,   249,
     394,   391,   392,   393,   397,   398,   399,   389,   382,     0,
     379,     0,   390,   409,     0,   464,     0,   153,   154,   152,
     151,     0,   484,   483,   607,     0,   458,   605,   457,   163,
     160,     0,     0,     0,   639,   506,   505,     0,     0,     0,
     535,     0,   285,   448,   607,     0,   661,   615,   646,   512,
     648,   513,   230,     0,     0,     0,   663,   228,   561,     0,
     666,   665,     0,    51,    50,    48,     0,    91,     0,     0,
       0,    84,     0,     0,    77,   272,     0,   305,     0,     0,
       0,   318,     0,   312,   315,   404,   401,   402,   405,   327,
       0,     0,   103,   101,   102,   100,    99,    98,   637,   638,
     589,   591,     0,   578,   139,   146,   145,   144,   141,   148,
     149,   143,   147,   142,   150,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     201,   518,   519,   517,   304,     0,   199,   304,   304,     0,
       0,   198,     0,   232,     0,     0,   208,     0,   207,     0,
     582,     0,     0,   214,     0,     0,   212,     0,   221,   220,
       0,   181,     0,   182,   246,   245,     0,   240,     0,     0,
     236,     0,   242,     0,   274,     0,     0,   395,   396,     0,
     389,   378,     0,   498,   410,   413,   412,   414,     0,   462,
     465,   466,     0,     0,   467,   468,   159,     0,     0,   293,
       0,     0,   292,   295,   536,     0,   286,   289,     0,   445,
     231,     0,     0,     0,     0,   229,     0,    96,    93,     0,
      74,    73,    72,     0,     0,     0,     0,   323,   310,     0,
     317,     0,   313,   309,   403,   409,   376,   106,   358,   120,
     356,   126,   125,   109,   124,   121,   361,   136,   107,   137,
     123,   127,   108,   110,   122,   138,   355,   337,   340,   338,
     339,   362,   350,   341,   354,   346,   344,   357,   360,   345,
     343,   348,   353,   342,   347,   351,   352,   349,   359,     0,
     336,     0,   111,     0,   376,   376,   334,   644,   580,   466,
     628,   628,     0,     0,     0,     0,     0,     0,   274,     0,
       0,     0,   205,   204,     0,   306,     0,     0,   306,   306,
     211,     0,     0,   550,     0,   549,     0,   581,     0,     0,
     556,   215,     0,   555,   213,    69,    68,   239,   235,   566,
     241,     0,     0,   271,   250,   247,   498,   380,     0,     0,
     466,   411,   425,   459,   489,     0,   662,   514,   514,     0,
     297,     0,     0,     0,   287,     0,   226,   563,     0,     0,
     224,   562,     0,   667,     0,     0,     0,    77,     0,    77,
      85,    88,   276,   300,   323,   171,   323,   299,   323,   307,
     165,   316,   319,   314,     0,   389,   333,     0,   363,     0,
     329,   330,   584,     0,     0,     0,     0,     0,     0,     0,
       0,   276,   306,   323,   306,   306,   323,   323,   554,     0,
       0,   583,   560,     0,     0,     0,     0,   244,    61,   466,
     498,     0,   501,   500,   502,   607,   422,   385,   383,     0,
       0,     0,     0,   487,   607,     0,     0,   298,   296,     0,
     290,     0,   227,     0,   225,    94,    76,    75,     0,     0,
       0,     0,   273,   323,   167,   308,   480,     0,   415,     0,
     335,   106,   108,   376,   376,     0,     0,     0,     0,     0,
       0,   323,   193,   323,   323,   185,   189,     0,     0,     0,
       0,    70,   243,   386,   384,   466,   490,     0,   424,   423,
     439,     0,   440,   427,   430,     0,   426,   419,   420,   322,
       0,   601,   602,   288,     0,     0,    87,    90,    86,    89,
     166,     0,   479,   478,   607,   416,   425,   377,   331,   332,
       0,     0,     0,     0,     0,     0,   195,   187,   191,   553,
     552,   559,   558,   388,   387,   492,   493,   495,   607,     0,
     662,   438,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   607,   603,   604,   565,   564,     0,   470,     0,     0,
       0,   494,   496,   429,   431,   432,   435,   436,   437,   433,
     434,   428,   475,   473,   607,   662,   417,   328,   418,   490,
     474,   607,   497
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1146, -1146, -1146,     1,     4,  2275, -1146,     0, -1146, -1146,
   -1146,   483, -1146, -1146, -1146,   273,   618,  -481, -1146,  -746,
    -741, -1146, -1146, -1146,   208, -1146, -1146,   235,   953, -1146,
    2397,   395,  -774, -1146,  -988,  2361, -1093,   465, -1146, -1146,
     -92,  -879, -1146,   -64, -1146, -1146, -1146, -1146, -1146, -1146,
   -1146, -1146, -1146, -1146,   253, -1146,   908, -1146, -1146,   126,
    1001, -1146, -1146, -1146, -1146, -1146,   676, -1146,   110, -1146,
   -1146, -1146, -1146, -1146, -1146,  -627,  -695, -1146, -1146, -1146,
      76,  -728,  1234, -1146, -1146, -1146,   168, -1146, -1146, -1146,
   -1146,   -80,  -157,  -943, -1146, -1146,   -74,   136,   310, -1146,
   -1146, -1146,    83, -1146, -1146,  -240,    29, -1015,  -196,  -228,
    -222,  -252, -1146,  -203, -1146,    -7,   986,  -123,   512, -1146,
    -483,  -847, -1145, -1146,  -683,  -526,  -979,  -956,  -973,   -16,
   -1146,    95, -1146,  -236,  -479,  -457,   589,  -369, -1146, -1146,
   -1146,  1275, -1146,    -9, -1146, -1146,  -229, -1146,  -659, -1146,
   -1146, -1146,  1321,  1637,   -12,   994,     7,   764, -1146,  1869,
    2236, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146, -1146,
    -424
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   345,  1078,   690,   112,   113,   114,   115,
     116,   363,   495,   117,   252,   118,   346,   681,   570,   685,
     682,   119,   120,   121,   567,   568,   122,   123,   186,   980,
     284,   124,   279,   125,   722,   289,   126,   291,   292,   127,
    1079,   128,   304,   129,   130,   131,   461,   659,   463,   660,
     456,   656,   132,   133,   825,   134,   250,   135,   698,   699,
     198,   137,   138,   139,   140,   141,   533,   742,   908,   142,
     143,   738,   903,   144,   145,   574,   927,   146,   147,   782,
     783,   784,   199,   282,   713,   149,   150,   576,   935,   789,
     983,   984,   492,  1086,   502,   708,   709,   710,   711,   712,
     790,   372,   888,  1216,  1277,  1199,   450,  1127,  1131,  1193,
    1194,  1195,   151,   333,   538,   152,   153,   285,   286,   506,
     507,   726,  1213,  1158,   510,   723,  1236,  1124,  1040,   347,
     215,   351,   352,   451,   452,   453,   200,   155,   156,   157,
     158,   201,   160,   183,   184,   632,   474,   849,   633,   634,
     161,   162,   202,   203,   165,   236,   454,   205,   167,   206,
     207,   170,   171,   172,   173,   357,   174,   175,   176,   177,
     178
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     181,   305,   684,   109,   204,   636,   110,   442,   209,   424,
     208,   743,   850,   478,   216,   211,   982,   496,   696,   229,
     229,   714,   240,   242,   244,   247,   251,   224,   924,   373,
     835,   462,   923,   891,   839,   287,   840,   290,  1198,   293,
     294,  1090,  1091,   508,  1043,   299,   737,   300,  1080,   301,
     900,  1122,   930,   308,   852,  1190,   592,   309,   313,   315,
     317,   318,   319,  1119,  1190,   287,   323,   521,   324,   287,
     327,   331,   845,   739,    70,   334,   511,   314,   336,   337,
    -269,   338,   364,   339,   472,   531,   340,   341,   443,   342,
     472,   355,   901,   309,   313,   356,   358,   592,   307,   295,
     287,   508,  1066,  1242,  1183,   487,  -270,   384,   560,   110,
     -54,   800,  1273,   353,   593,   390,  -112,   179,   508,  1129,
    -294,   643,   472,   546,  -118,   554,   558,     3,   594,   -54,
    1280,  1243,   989,  1045,   307,   350,   307,   561,  1067,   553,
     724,   472,  1092,   988,  1191,   911,   902,  1185,   547,   365,
     377,  1239,   438,   801,   643,  1192,   596,   595,   305,   296,
    -252,  -481,  -294,   824,  1192,  -481,  1198,  -481,   744,  1212,
    1233,   424,   438,   438,   512,   181,  1130,   297,   432,   433,
     434,   436,   515,   323,   309,   356,   182,   509,  -481,   622,
     623,   438,  -481,  1128,   298,  1154,   226,   596,   833,  1080,
     441,  1081,   438,   597,   555,  1042,   438,  1004,   229,   438,
    1006,  1007,    70,   523,   313,  -481,  -294,   447,  -481,   556,
    1218,  1219,   253,   458,  1080,   307,   307,  1080,  1080,   465,
    1237,   556,   890,   877,   932,   369,   254,   438,   211,   549,
     522,   313,  -481,   725,   551,   509,   185,   636,   644,   556,
     255,   256,   556,   257,   493,   370,  -253,   494,   258,   933,
    1046,  1261,   509,  -481,   919,   371,   550,   259,  1126,  -114,
    -481,   552,  1184,   260,  1210,   878,   472,  -119,  1240,   261,
    -499,  1275,  -481,   262,  1262,  1276,   263,   239,   241,   243,
     438,   313,  1080,   684,  1080,  1080,   264,   925,   525,   526,
    1282,  -499,   556,   265,   266,  -499,   802,   445,  -259,   313,
    1103,   267,   775,  1106,  1107,   982,   860,   326,   330,   776,
     268,  1149,   288,  1151,   523,  1148,   446,  1150,  -499,   269,
     270,   557,   271,   803,   272,   658,   273,   548,  1234,   274,
     380,  1145,   664,   275,   503,   288,   276,   545,  1001,   277,
     110,   843,   569,   571,  -572,   325,   329,  1111,  1156,  -258,
     354,  -635,   212,  -635,   578,   110,   579,   580,   581,   583,
     585,   586,  -267,   587,   588,   589,   590,   591,   636,   599,
     600,   601,   602,   603,   604,   605,   606,   607,   608,   609,
     610,   611,   612,   613,   614,   615,   655,   381,   457,  1038,
     459,   382,   313,   313,   464,   504,   -83,  1171,   313,  1173,
    1174,   313,   313,   313,   313,   625,   627,   635,  -476,   226,
      23,   617,   618,   874,   460,   -54,   523,   619,   647,  -257,
     620,   621,   875,   651,   655,   592,   652,   785,  1064,   427,
    1016,    23,   110,   428,   -54,   524,   854,   857,   384,  -476,
     512,   666,   593,   388,   253,   309,   390,   668,   670,   876,
     393,   998,  -476,   674,   676,    42,   594,  1017,   679,   577,
     683,   683,    60,   -83,   655,  -476,  -254,   691,   512,   766,
     693,   439,  -586,    68,   179,   313,    23,   302,   817,   786,
     543,    58,   428,    60,  -585,   595,   307,   650,   767,   727,
     787,  1072,  1157,   293,    68,   707,   732,   733,    88,  -586,
     731,   736,   288,   288,   288,   288,   288,   288,   313,   788,
    1133,  -585,  -476,   488,   491,   912,   914,  -476,   384,    88,
     700,   884,   655,   388,   736,   596,   390,   313,    60,   313,
     393,   597,   909,   755,   736,   758,  -261,   760,   762,    68,
    -265,   701,   735,   499,   770,   702,   353,   777,   353,  1101,
     320,   309,   512,   288,  -262,   288,   545,   361,   210,   110,
     512,   179,   288,   771,    88,   735,  -447,   763,   350,   703,
     350,   836,   704,   885,    62,   735,   684,   303,   684,   841,
     288,   154,   636,   705,   886,  -447,  -256,  -491,   425,  -260,
     426,   154,   307,  -636,   438,  -636,   498,   321,   288,   288,
    -266,  -268,   706,   887,   154,  -255,  -263,   154,  -491,   362,
     366,   449,  -491,  -569,   429,   668,   820,   674,   823,   691,
     313,   826,   758,   985,   582,   584,  1244,   828,   829,  -568,
    -635,   313,  -635,   313,   749,  -491,   751,  -632,   449,  -632,
    -634,   374,  -634,   842,  -633,   844,  -633,   375,  -482,   635,
     832,   851,  1245,   376,  1246,  1247,  -573,   154,  1248,  1249,
    1125,  -482,   437,  -477,   864,   865,  -381,  1134,  -574,  -571,
     624,   626,   831,  -570,   431,   438,   444,  -469,   448,   455,
     527,   537,   467,   646,   154,   497,   501,  -486,   449,   154,
     518,   519,  -482,   530,  -477,   893,   539,   319,   323,   356,
    -486,   562,   566,   313,   288,   573,   449,  -477,  -469,   575,
     390,   616,   642,   669,   653,   654,   649,   655,  -482,   675,
    -477,  -469,   895,   678,   665,  -482,   380,   671,   662,   686,
     687,  -486,   689,   715,  -469,   692,   725,   288,   695,   716,
     307,   288,   734,   740,   569,   741,  -482,   745,   920,   753,
     922,   683,   683,   759,   313,   778,   166,  -486,   791,  -471,
     792,   793,   154,   794,  -486,  -482,   166,  -477,   700,   799,
    -482,   827,  -477,  -482,   795,   598,   798,   796,  1214,   166,
     635,  -469,   166,   381,   797,  -486,  -469,   382,   -79,   701,
    -471,   860,   861,   702,   583,   625,   992,   863,   754,   995,
     866,  1135,  1136,  -471,  -486,   313,   868,  1238,   873,  -486,
     449,   449,  -486,   880,   881,   380,  -471,   703,   899,   889,
     704,  1251,   359,  1013,   904,  1015,   523,   928,   931,   934,
    1020,   705,   166,  1023,   384,   987,   328,   328,  1025,   388,
    1026,   389,   390,   993,   996,   380,   393,  1029,   999,  1005,
     706,  1008,  1009,  1031,   400,  1033,  1039,  1041,  1044,   166,
     154,  1238,   406,  -471,   166,  1053,   154,  1047,  -471,  1070,
    -139,  1048,   381,  1071,   707,  1051,   382,  1058,  1274,   309,
    1062,   819,   449,   822,  1073,  -146,  1085,  -115,  -145,  1057,
    1116,  1061,  -144,  -141,  1063,  -113,  1281,   683,  -148,  -149,
    1238,  -143,   381,  -116,  -147,  -142,   382,  -150,  1088,  1082,
    -117,  1120,  1089,  1102,  1118,   449,  1104,  1152,  1105,  1215,
     307,  -112,  1164,   384,   385,   154,   386,   387,   388,  1241,
     389,   390,   288,   288,  1139,   393,  -114,   166,   288,   288,
     154,   288,   288,   400,  1206,  1201,  1202,  1207,  1217,   404,
     405,   406,  1208,   384,   385,  1209,   872,  1087,   388,  1252,
     389,   390,  1253,  1272,   917,   393,  1243,   213,   694,   565,
     729,  1013,  1015,   400,  1020,  1023,  1155,  1057,  1061,   404,
     405,   406,  1265,  1266,  1267,  1268,  1269,  1270,   335,  1108,
    1109,  1034,  1110,   136,   635,   535,  1112,  1113,  1083,  1163,
    1114,  1159,   305,   136,   906,  1052,  1037,   879,  1084,  1278,
    1259,  1264,  1271,   246,   728,   230,   136,   154,     0,   136,
       0,     0,     0,  1132,   758,   313,   313,   449,     0,     0,
       0,  1140,     0,  1141,     0,   166,     0,  1143,     0,     0,
     449,   166,     0,  1147,   683,   683,   683,   683,   154,     0,
       0,     0,   380,     0,   154,     0,     0,     0,  1123,   582,
     624,     0,     0,     0,     0,     0,   154,     0,   154,   136,
       0,  1108,  1165,  1166,  1112,  1167,  1168,  1169,  1170,   707,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   288,   288,  1181,     0,  1187,   136,     0,     0,   356,
     166,   136,     0,     0,  1189,   288,     0,   491,  1196,   381,
       0,     0,     0,   382,   491,   166,     0,   288,     0,     0,
     288,     0,     0,     0,     0,     0,     0,   700,     0,     0,
       0,     0,   154,     0,     0,     0,   154,     0,     0,  1035,
     307,     0,     0,     0,   154,  1188,     0,     0,   701,     0,
       0,     0,   702,   918,   598,  1229,  1230,  1231,  1232,     0,
     384,   449,  -488,     0,     0,   388,     0,   389,   390,     0,
       0,  1054,   393,     0,   136,  -488,   703,     0,  1061,   704,
     400,    62,  1254,  1255,     0,  1258,   404,   405,   406,   356,
     705,     0,   166,     0,     0,     0,     0,     0,  1229,  1230,
    1231,  1232,  1254,  1255,     0,  1235,  -488,     0,     0,   706,
       0,     0,   449,   815,     0,     0,     0,  1260,   758,     0,
    1263,     0,     0,   166,     0,     0,   148,     0,     0,   166,
     307,     0,  -488,     0,     0,     0,   148,     0,     0,  -488,
       0,   166,     0,   166,     0,     0,     0,     0,     0,   148,
       0,   380,   148,   758,     0,     0,     0,  1061,     0,  1235,
    -488,     0,     0,     0,     0,     0,     0,   159,   154,     0,
       0,   154,   136,     0,     0,     0,     0,   159,   136,  -488,
       0,     0,     0,     0,  -488,     0,     0,  -488,     0,     0,
     159,     0,     0,   159,     0,     0,     0,     0,  1235,     0,
       0,     0,   148,     0,     0,     0,     0,   166,   381,     0,
       0,   166,   382,   163,   288,     0,     0,     0,     0,   166,
       0,     0,     0,   163,     0,     0,     0,     0,     0,   148,
       0,     0,   154,     0,   148,     0,   163,   136,   154,   163,
       0,     0,     0,   159,     0,     0,     0,     0,     0,     0,
       0,     0,   136,     0,     0,     0,     0,     0,     0,   384,
     385,   380,   386,   387,   388,     0,   389,   390,     0,   986,
     159,   393,   491,   491,     0,   159,   491,   491,   399,   400,
       0,     0,   403,     0,     0,   404,   405,   406,     0,   163,
       0,     0,     0,     0,  1197,   990,   991,     0,     0,     0,
       0,     0,  1203,     0,   491,     0,   491,   148,     0,     0,
       0,     0,     0,     0,     0,     0,   163,     0,   381,     0,
       0,   163,   382,     0,   154,     0,   154,     0,     0,   136,
       0,   154,   449,   449,   154,     0,     0,     0,     0,     0,
       0,     0,     0,   166,     0,   154,   166,   154,   159,     0,
     154,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     136,     0,     0,     0,     0,     0,   136,     0,     0,   384,
     385,     0,     0,   387,   388,     0,   389,   390,   136,   154,
     136,   393,     0,     0,     0,     0,     0,     0,     0,   400,
     154,     0,   154,     0,   163,   404,   405,   406,     0,     0,
       0,     0,     0,     0,   926,   148,     0,   166,     0,     0,
       0,   148,     0,   166,     0,     0,     0,     0,     0,     0,
       0,     0,  1197,     0,     0,  -306,     0,     0,     0,  -306,
    -306,     0,     0,     0,  -306,     0,     0,     0,     0,  -306,
       0,  -306,  -306,     0,   136,     0,   159,  -306,   136,     0,
       0,     0,   159,     0,  -306,     0,   136,  -306,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     148,     0,     0,     0,     0,     0,     0,  -306,     0,     0,
    -306,     0,  -306,     0,  -306,   148,  -306,  -306,     0,  -306,
     154,  -306,   163,  -306,     0,     0,     0,   154,   163,   166,
       0,   166,     0,     0,     0,     0,   166,     0,     0,   166,
     154,   159,  -306,     0,     0,  -306,   592,     0,  -306,     0,
     166,     0,   166,     0,     0,   166,   159,     0,   154,   164,
     804,     0,     0,   805,     0,     0,     0,   154,   806,   164,
       0,   154,     0,     0,     0,     0,     0,     0,     0,   189,
       0,     0,   164,     0,   166,   164,     0,   163,     0,   807,
       0,     0,   148,     0,     0,   166,   808,   166,  -306,     0,
       0,     0,   163,     0,  -306,     0,   809,     0,     0,     0,
     136,     0,     0,   136,   810,     0,     0,     0,     0,     0,
       0,     0,     0,   148,     0,   154,     0,     0,     0,   148,
     811,     0,     0,   159,     0,   164,     0,     0,     0,     0,
       0,   148,   812,   148,     0,     0,   596,     0,     0,     0,
       0,     0,     0,   813,     0,     0,     0,     0,     0,   814,
       0,     0,   164,     0,   159,     0,     0,   164,     0,     0,
     159,     0,     0,     0,   136,     0,     0,     0,     0,   163,
     136,     0,   159,     0,   159,     0,   154,   154,   154,   154,
       0,     0,     0,     0,     0,   166,     0,     0,     0,     0,
       0,     0,   166,     0,     0,     0,     0,   148,  1153,     0,
     163,   148,     0,   154,   154,   166,   163,     0,     0,   148,
       0,     0,   218,     0,     0,     0,    22,    23,   163,     0,
     163,     0,     0,   166,     0,     0,   219,     0,    31,   220,
     164,     0,   166,     0,    37,     0,   166,     0,   159,     0,
       0,    42,   159,     0,     0,     0,     0,     0,     0,     0,
     159,     0,     0,     0,     0,     0,   136,     0,   136,     0,
       0,     0,     0,   136,     0,     0,   136,    58,     0,    60,
       0,     0,     0,  1075,     0,     0,  1076,   136,   223,   136,
      68,   168,   136,     0,   163,     0,     0,     0,   163,     0,
     166,   168,     0,     0,     0,     0,   163,     0,     0,    84,
       0,     0,    86,     0,   168,    88,     0,   168,     0,     0,
       0,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   136,     0,   136,     0,     0,     0,   164,     0,
       0,     0,     0,   148,   164,     0,   148,     0,     0,     0,
       0,     0,     0,     0,   218,     0,     0,     0,    22,    23,
       0,   166,   166,   166,   166,   103,     0,   168,   219,     0,
      31,   220,     0,     0,     0,     0,    37,     0,     0,   -82,
       0,     0,     0,    42,   159,     0,     0,   159,   166,   166,
       0,     0,     0,     0,   168,     0,     0,     0,   -53,   168,
       0,     0,     0,   164,     0,     0,     0,   148,     0,    58,
       0,    60,     0,   148,   254,     0,     0,   -53,   164,     0,
     223,     0,    68,     0,     0,     0,     0,     0,   255,   256,
     163,   257,   136,   163,     0,     0,   258,     0,     0,   136,
       0,    84,     0,     0,    86,   259,   -82,    88,   159,     0,
       0,   260,   136,     0,   159,     0,     0,   261,     0,     0,
       0,   262,     0,     0,   263,     0,     0,     0,     0,     0,
     136,     0,   168,     0,   264,     0,     0,     0,     0,   136,
       0,   265,   266,   136,     0,     0,     0,     0,     0,   267,
       0,     0,     0,     0,   163,   164,     0,     0,   268,   148,
     163,   148,     0,     0,     0,     0,   148,   269,   270,   148,
     271,     0,   272,     0,   273,     0,     0,   274,     0,     0,
     148,   275,   148,     0,   276,   148,   164,   277,     0,     0,
       0,     0,   164,     0,     0,     0,     0,   136,     0,     0,
     159,     0,   159,     0,   164,     0,   164,   159,     0,     0,
     159,     0,     0,     0,   148,     0,     0,     0,     0,     0,
       0,   159,     0,   159,     0,   148,   159,   148,     0,     0,
     168,     0,     0,     0,     0,     0,   168,     0,     0,     0,
       0,   438,     0,   905,     0,     0,   163,     0,   163,     0,
       0,     0,     0,   163,     0,   159,   163,     0,   136,   136,
     136,   136,     0,     0,     0,     0,   159,   163,   159,   163,
     164,     0,   163,     0,   164,     0,     0,     0,     0,     0,
       0,     0,   164,     0,     0,   136,   136,     0,     0,     0,
       0,     0,     0,     0,     0,   168,     0,     0,     0,     0,
       0,   163,     0,     0,     0,     0,     0,     0,     0,     0,
     168,     0,   163,     0,   163,     0,     0,     0,   169,     0,
       0,     0,     0,     0,     0,   148,     0,     0,   169,     0,
       0,     0,   148,     0,     0,     0,     0,     0,     0,     0,
       0,   169,     0,     0,   169,   148,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   111,     0,     0,
       0,     0,     0,   148,     0,     0,   159,   180,     0,     0,
       0,   217,   148,   159,     0,     0,   148,     0,     0,     0,
     214,     0,     0,   225,     0,   218,   159,   168,     0,    22,
      23,     0,     0,     0,   169,     0,     0,     0,     0,   219,
       0,    31,   220,     0,   159,     0,   164,    37,     0,   164,
       0,     0,   163,   159,    42,     0,     0,   159,   168,   163,
       0,   169,     0,     0,   168,     0,   169,     0,     0,   -52,
     148,     0,   163,   322,     0,     0,   168,     0,   168,     0,
      58,     0,    60,     0,     0,     0,   221,     0,   -52,   222,
     163,   223,     0,    68,     0,     0,     0,     0,     0,   163,
     111,     0,     0,   163,     0,   360,     0,     0,     0,     0,
     164,   159,    84,     0,     0,    86,   164,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   148,   148,   148,   148,     0,     0,     0,     0,   169,
       0,     0,   168,     0,     0,     0,   168,     0,     0,     0,
       0,     0,     0,   316,   168,     0,     0,   163,   148,   148,
       0,   278,   280,   281,     0,     0,     0,     0,   103,     0,
       0,     0,   159,   159,   159,   159,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   159,
     159,   332,   164,     0,   164,     0,     0,     0,     0,   164,
       0,     0,   164,     0,     0,     0,     0,     0,   163,   163,
     163,   163,   218,   164,     0,   164,    22,    23,   164,     0,
       0,     0,     0,     0,     0,     0,   219,   169,    31,   220,
       0,     0,     0,   169,    37,   163,   163,     0,     0,     0,
       0,    42,     0,     0,     0,     0,     0,   164,     0,     0,
       0,     0,   367,     0,   368,     0,   -53,     0,   164,     0,
     164,     0,     0,     0,     0,     0,   500,    58,   168,    60,
       0,   168,   513,     0,     0,   -53,     0,     0,   223,   218,
      68,     0,     0,    22,    23,     0,     0,     0,     0,     0,
       0,     0,   169,   219,     0,    31,   220,     0,     0,    84,
     435,    37,    86,     0,     0,    88,     0,   169,    42,     0,
     468,   471,   473,   477,   479,   481,     0,   332,     0,     0,
       0,     0,     0,   -52,     0,     0,     0,     0,     0,     0,
       0,   111,   168,     0,    58,     0,    60,     0,   168,     0,
     221,   332,   -52,   222,   466,   223,   111,    68,     0,     0,
       0,     0,     0,     0,     0,   485,   486,     0,   164,     0,
       0,   514,     0,   516,     0,   164,    84,     0,     0,    86,
     520,     0,    88,     0,     0,   218,     0,     0,   164,    22,
      23,     0,     0,     0,   169,     0,     0,     0,   529,   219,
     505,    31,   220,     0,     0,     0,   164,    37,     0,     0,
       0,     0,     0,     0,    42,   164,   540,   541,     0,   164,
       0,     0,     0,     0,     0,   169,     0,     0,     0,     0,
       0,   169,   103,   111,   168,     0,   168,     0,     0,     0,
      58,   168,    60,   169,   168,   169,  1075,     0,     0,  1076,
       0,   223,     0,    68,     0,   168,     0,   168,     0,     0,
     168,     0,     0,   218,   513,     0,     0,    22,    23,     0,
     513,     0,    84,   164,     0,    86,     0,   219,    88,    31,
     220,     0,   688,   572,     0,    37,     0,     0,     0,   168,
       0,     0,    42,     0,     0,     0,     0,     0,     0,     0,
     168,     0,   168,     0,     0,     0,     0,     0,     0,   169,
       0,     0,     0,   169,     0,     0,     0,     0,    58,     0,
      60,   169,   648,     0,     0,     0,     0,     0,   103,   223,
       0,    68,     0,     0,   164,   164,   164,   164,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   752,     0,
      84,     0,   757,    86,   645,   672,    88,     0,     0,   677,
     111,   164,   164,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   657,     0,   254,     0,     0,   661,     0,
     663,     0,     0,     0,     0,     0,     0,     0,     0,   255,
     256,     0,   257,     0,     0,     0,     0,   258,     0,     0,
     168,     0,     0,     0,     0,     0,   259,   168,     0,     0,
       0,     0,   260,     0,     0,   697,     0,     0,   261,     0,
     168,   505,   262,     0,     0,   263,     0,     0,     0,     0,
       0,     0,     0,     0,   730,   264,     0,     0,   168,     0,
       0,     0,   265,   266,     0,   169,     0,   168,   169,     0,
     267,   168,     0,     0,     0,     0,     0,     0,     0,   268,
       0,     0,     0,     0,     0,     0,     0,     0,   269,   270,
       0,   271,     0,   272,     0,   273,     0,     0,   274,     0,
       0,     0,   275,   765,   867,   276,     0,   870,   277,     0,
       0,     0,   781,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   168,     0,     0,     0,   169,
       0,     0,     0,     0,     0,   169,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   283,     0,     0,     0,   910,     0,
     846,   848,     0,     0,   915,     0,   853,   856,     0,   858,
     859,     0,     0,     0,     0,     0,   168,   168,   168,   168,
       0,     0,     0,   834,     0,     0,   837,   838,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   168,   168,     0,     0,     0,     0,     0,
       0,   169,     0,   169,     0,     0,     0,     0,   169,     0,
       0,   169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   169,     0,   169,     0,     0,   169,   883,     0,
       0,     0,   505,     0,     0,     0,     0,     0,     0,     0,
     513,     0,   513,     0,     0,     0,     0,   513,   896,     0,
     513,     0,     0,     0,     0,     0,   169,     0,   907,     0,
       0,  1027,   332,  1028,     0,     0,  1030,   169,     0,   169,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   218,     0,     0,     0,    22,    23,     0,     0,
       0,  1074,     0,     0,     0,  1050,   219,   781,    31,   220,
       0,     0,     0,     0,    37,     0,  1056,   981,  1060,  1002,
    1003,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1010,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1021,     0,    58,  1024,    60,
       0,    62,     0,  1075,     0,     0,  1076,     0,   223,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   169,     0,    84,
       0,     0,    86,     0,   169,    88,     0,     0,     0,     0,
     254,     0,     0,     0,     0,     0,     0,   169,     0,     0,
    1032,     0,   697,   380,   255,   256,  1036,   257,     0,   505,
       0,     0,   258,     0,     0,   169,     0,     0,     0,     0,
       0,   259,     0,     0,   169,     0,     0,   260,   169,     0,
       0,     0,   907,   261,     0,   103,  1117,   262,     0,     0,
     263,  1077,     0,     0,     0,     0,     0,     0,     0,     0,
     264,     0,     0,     0,  1137,     0,   781,   265,   266,   781,
     381,     0,     0,  1142,   382,   267,     0,  1144,     0,     0,
       0,     0,     0,     0,   268,     0,     0,     0,     0,     0,
       0,     0,   169,   269,   270,     0,   271,     0,   272,     0,
     273,     0,     0,   274,     0,   383,     0,   275,     0,     0,
     276,     0,     0,   277,     0,     0,     0,     0,     0,     0,
       0,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,  1182,   392,   393,   394,   395,     0,     0,   397,   398,
     399,   400,   401,   402,   403,     0,     0,   404,   405,   406,
       0,     0,  1138,   169,   169,   169,   169,     0,   407,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   882,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     169,   169,     0,     0,     0,     0,     0,     0,     0,    -2,
       4,     0,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,   981,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -81,     0,    40,    41,
      42,     0,    43,  -323,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -323,     0,     0,    58,    59,    60,    61,
      62,    63,    64,  -323,   -52,    65,    66,    67,     0,    68,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,    84,    85,
       0,    86,    87,   -81,    88,    89,     0,     0,    90,     0,
      91,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,   103,     0,     0,   104,     0,   105,
       0,   106,     0,   107,     0,     4,   108,     5,     0,     6,
       7,     8,     9,    10,    11,     0,  -662,     0,    12,    13,
      14,    15,    16,  -662,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,  -662,    28,    29,  -662,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -81,     0,    40,    41,    42,     0,    43,  -323,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -323,     0,
       0,    58,    59,    60,    61,     0,    63,    64,  -323,   -52,
      65,    66,    67,  -662,    68,    69,    70,  -662,    71,    72,
      73,    74,    75,    76,    77,    78,     0,    79,    80,     0,
      81,    82,    83,    84,    85,     0,    86,    87,   -81,    88,
      89,     0,     0,    90,     0,    91,     0,     0,  -662,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -662,  -662,    95,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,     0,  -662,  -662,  -662,  -662,  -662,
       0,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,   103,
    -662,  -662,  -662,     0,   105,  -662,   106,     0,   107,     0,
     343,  -662,     5,   306,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -81,     0,    40,    41,
      42,     0,    43,  -323,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -323,     0,     0,    58,    59,    60,    61,
      62,    63,    64,  -323,   -52,    65,    66,    67,     0,    68,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,    84,    85,
       0,    86,    87,   -81,    88,    89,     0,     0,    90,     0,
      91,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,   103,     0,     0,   104,     0,   105,
     344,   106,     0,   107,     0,     4,   108,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -81,     0,    40,    41,    42,     0,    43,  -323,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -323,     0,
       0,    58,    59,    60,    61,    62,    63,    64,  -323,   -52,
      65,    66,    67,     0,    68,    69,    70,     0,    71,    72,
      73,    74,    75,    76,    77,    78,     0,    79,    80,     0,
      81,    82,    83,    84,    85,     0,    86,    87,   -81,    88,
      89,     0,     0,    90,     0,    91,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,   103,
       0,     0,   104,     0,   105,   544,   106,     0,   107,     0,
     563,   108,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -81,     0,    40,    41,
      42,     0,    43,  -323,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -323,     0,     0,    58,    59,    60,    61,
      62,    63,    64,  -323,   -52,    65,    66,    67,     0,    68,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,    84,    85,
       0,    86,    87,   -81,    88,    89,     0,     0,    90,     0,
      91,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,   103,     0,     0,   104,     0,   105,
     564,   106,     0,   107,     0,   343,   108,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -81,     0,    40,    41,    42,     0,    43,  -323,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -323,     0,
       0,    58,    59,    60,    61,    62,    63,    64,  -323,   -52,
      65,    66,    67,     0,    68,    69,    70,     0,    71,    72,
      73,    74,    75,    76,    77,    78,     0,    79,    80,     0,
      81,    82,    83,    84,    85,     0,    86,    87,   -81,    88,
      89,     0,     0,    90,     0,    91,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,   103,
       0,     0,   104,     0,   105,   344,   106,     0,   107,     0,
       4,   108,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -81,     0,    40,    41,
      42,     0,    43,  -323,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -323,     0,     0,    58,    59,    60,    61,
      62,    63,    64,  -323,   -52,    65,    66,    67,     0,    68,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,    84,    85,
       0,    86,    87,   -81,    88,    89,     0,     0,    90,     0,
      91,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,   103,     0,     0,   104,     0,   105,
     764,   106,     0,   107,     0,     4,   108,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -81,     0,    40,    41,    42,     0,    43,  -323,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -323,     0,
       0,    58,    59,    60,    61,   359,    63,    64,  -323,   -52,
      65,    66,    67,     0,    68,    69,    70,     0,    71,    72,
      73,    74,    75,    76,    77,    78,     0,    79,    80,     0,
      81,    82,    83,    84,    85,     0,    86,    87,   -81,    88,
      89,     0,     0,    90,     0,    91,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,   103,
       0,     0,   104,     0,   105,     0,   106,     0,   107,     0,
       4,   108,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -81,     0,    40,    41,
      42,     0,    43,  -323,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -323,     0,     0,    58,    59,    60,    61,
       0,    63,    64,  -323,   -52,    65,    66,    67,     0,    68,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,    84,    85,
       0,    86,    87,   -81,    88,    89,     0,     0,    90,     0,
      91,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,   103,     0,     0,   104,     0,   105,
       0,   106,     0,   107,     0,     4,   108,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,   667,
      39,   -81,     0,    40,    41,    42,     0,    43,  -323,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -323,     0,
       0,    58,    59,    60,    61,     0,    63,    64,  -323,   -52,
      65,    66,    67,     0,    68,    69,    70,     0,    71,    72,
      73,    74,    75,    76,    77,    78,     0,    79,    80,     0,
      81,    82,    83,    84,    85,     0,    86,    87,   -81,    88,
      89,     0,     0,    90,     0,    91,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,   103,
       0,     0,   104,     0,   105,     0,   106,     0,   107,     0,
       4,   108,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,   673,    39,   -81,     0,    40,    41,
      42,     0,    43,  -323,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -323,     0,     0,    58,    59,    60,    61,
       0,    63,    64,  -323,   -52,    65,    66,    67,     0,    68,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,    84,    85,
       0,    86,    87,   -81,    88,    89,     0,     0,    90,     0,
      91,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,   103,     0,     0,   104,     0,   105,
       0,   106,     0,   107,     0,     4,   108,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,  1012,
      39,   -81,     0,    40,    41,    42,     0,    43,  -323,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -323,     0,
       0,    58,    59,    60,    61,     0,    63,    64,  -323,   -52,
      65,    66,    67,     0,    68,    69,    70,     0,    71,    72,
      73,    74,    75,    76,    77,    78,     0,    79,    80,     0,
      81,    82,    83,    84,    85,     0,    86,    87,   -81,    88,
      89,     0,     0,    90,     0,    91,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,   103,
       0,     0,   104,     0,   105,     0,   106,     0,   107,     0,
       4,   108,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,  1014,    39,   -81,     0,    40,    41,
      42,     0,    43,  -323,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -323,     0,     0,    58,    59,    60,    61,
       0,    63,    64,  -323,   -52,    65,    66,    67,     0,    68,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,    84,    85,
       0,    86,    87,   -81,    88,    89,     0,     0,    90,     0,
      91,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,   103,     0,     0,   104,     0,   105,
       0,   106,     0,   107,     0,     4,   108,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,  1019,
      39,   -81,     0,    40,    41,    42,     0,    43,  -323,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -323,     0,
       0,    58,    59,    60,    61,     0,    63,    64,  -323,   -52,
      65,    66,    67,     0,    68,    69,    70,     0,    71,    72,
      73,    74,    75,    76,    77,    78,     0,    79,    80,     0,
      81,    82,    83,    84,    85,     0,    86,    87,   -81,    88,
      89,     0,     0,    90,     0,    91,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,   103,
       0,     0,   104,     0,   105,     0,   106,     0,   107,     0,
       4,   108,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,  1022,    39,   -81,     0,    40,    41,
      42,     0,    43,  -323,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -323,     0,     0,    58,    59,    60,    61,
       0,    63,    64,  -323,   -52,    65,    66,    67,     0,    68,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,    84,    85,
       0,    86,    87,   -81,    88,    89,     0,     0,    90,     0,
      91,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,   103,     0,     0,   104,     0,   105,
       0,   106,     0,   107,     0,     4,   108,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,  1049,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -81,     0,    40,    41,    42,     0,    43,  -323,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -323,     0,
       0,    58,    59,    60,    61,     0,    63,    64,  -323,   -52,
      65,    66,    67,     0,    68,    69,    70,     0,    71,    72,
      73,    74,    75,    76,    77,    78,     0,    79,    80,     0,
      81,    82,    83,    84,    85,     0,    86,    87,   -81,    88,
      89,     0,     0,    90,     0,    91,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,   103,
       0,     0,   104,     0,   105,     0,   106,     0,   107,     0,
       4,   108,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,  1055,    39,   -81,     0,    40,    41,
      42,     0,    43,  -323,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -52,    53,    54,     0,    55,
      56,    57,     0,  -323,     0,     0,    58,    59,    60,    61,
       0,    63,    64,  -323,   -52,    65,    66,    67,     0,    68,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,    84,    85,
       0,    86,    87,   -81,    88,    89,     0,     0,    90,     0,
      91,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,   103,     0,     0,   104,     0,   105,
       0,   106,     0,   107,     0,     4,   108,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,  1059,
      39,   -81,     0,    40,    41,    42,     0,    43,  -323,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,    52,
     -52,    53,    54,     0,    55,    56,    57,     0,  -323,     0,
       0,    58,    59,    60,    61,     0,    63,    64,  -323,   -52,
      65,    66,    67,     0,    68,    69,    70,     0,    71,    72,
      73,    74,    75,    76,    77,    78,     0,    79,    80,     0,
      81,    82,    83,    84,    85,     0,    86,    87,   -81,    88,
      89,     0,     0,    90,     0,    91,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,   103,
       0,     0,   104,     0,   105,     0,   106,     0,   107,     0,
    1256,   108,     5,   306,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   187,     0,     0,    15,    16,     0,    17,
       0,   188,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   189,     0,     0,     0,    33,
     190,   191,     0,     0,   192,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   193,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   194,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     195,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   196,
       0,   106,     0,   197,  1257,   892,   108,     5,   306,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   187,     0,
       0,    15,    16,     0,    17,     0,   188,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     189,     0,     0,     0,    33,   190,   191,     0,     0,   192,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     193,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   194,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   195,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   196,     0,   106,     0,   197,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   187,     0,     0,    15,    16,     0,    17,     0,   188,
       0,     0,    21,   245,    23,     0,     0,     0,     0,     0,
       0,    29,     0,   189,     0,     0,     0,    33,   190,   191,
       0,     0,   192,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   193,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,    60,    61,     0,    63,
       0,     0,     0,     0,    66,   194,     0,    68,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   195,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,    88,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   196,     0,   106,
       0,   197,     0,     0,   108,     5,   306,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   187,     0,     0,    15,
      16,     0,    17,     0,   188,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   189,     0,
       0,     0,    33,   190,   191,     0,     0,   192,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   193,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     194,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   195,     0,    79,     0,     0,    81,   310,
     311,     0,    85,   348,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,   349,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   196,     0,   106,     0,   197,     0,     0,   108,
       5,   306,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   187,     0,     0,    15,    16,     0,    17,     0,   188,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   189,     0,     0,     0,    33,   190,   191,
       0,     0,   192,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   193,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   194,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   195,     0,
      79,     0,     0,    81,   310,   311,     0,    85,   348,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   196,     0,   106,
     748,   197,     0,     0,   108,     5,   306,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   187,     0,     0,    15,
      16,     0,    17,     0,   188,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   189,     0,
       0,     0,    33,   190,   191,     0,     0,   192,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   193,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     194,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   195,     0,    79,     0,     0,    81,   310,
     311,     0,    85,   348,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   196,     0,   106,   750,   197,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   187,
       0,     0,    15,    16,     0,    17,     0,   188,     0,     0,
      21,     0,   628,     0,     0,     0,     0,     0,     0,    29,
       0,   189,     0,     0,     0,    33,   190,   191,     0,     0,
     192,    39,     0,   629,     0,    41,     0,     0,    43,     0,
       0,   193,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   194,     0,   630,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   195,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
     631,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   196,     0,   106,     0,   197,
       0,     5,   108,     6,     7,     8,   231,    10,    11,   232,
       0,     0,   187,     0,     0,    15,    16,     0,    17,     0,
     188,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   189,     0,     0,     0,    33,   190,
     191,     0,     0,   192,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   193,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   233,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   195,
       0,    79,     0,     0,    81,     0,     0,   234,    85,     0,
     235,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   196,     0,
     106,     0,   197,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   187,     0,     0,    15,    16,
       0,    17,     0,   188,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   189,     0,     0,
       0,    33,   190,   191,     0,     0,   192,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   193,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   194,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   195,     0,    79,     0,     0,    81,   310,   311,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   312,     0,     0,   104,
       0,   196,     0,   106,     0,   197,     0,     0,   108,     5,
     306,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     187,     0,     0,    15,    16,     0,    17,     0,   188,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   189,     0,     0,     0,    33,   190,   191,     0,
       0,   192,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   193,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   194,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   195,     0,    79,
       0,     0,    81,   310,   311,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   196,     0,   106,     0,
     197,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   187,     0,     0,    15,    16,     0,    17,
       0,   188,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   189,     0,     0,     0,    33,
     190,   191,     0,     0,   192,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   193,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,   894,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   194,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     195,     0,    79,     0,     0,    81,   310,   311,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   196,
       0,   106,     0,   197,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   187,     0,     0,    15,
      16,     0,    17,     0,   188,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   189,     0,
       0,     0,    33,   190,   191,     0,     0,   192,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   193,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,   237,    61,     0,    63,     0,     0,     0,     0,    66,
     194,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   195,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,   238,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   196,     0,   106,     0,   197,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   187,
       0,     0,    15,    16,     0,    17,     0,   188,     0,     0,
      21,     0,   248,     0,     0,     0,     0,     0,     0,    29,
       0,   189,     0,     0,     0,    33,   190,   191,     0,     0,
     192,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   193,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   194,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   195,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
     249,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   196,     0,   106,     0,   197,
       0,     0,   108,     5,   306,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   187,     0,     0,    15,    16,     0,
      17,     0,   188,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   189,     0,     0,     0,
      33,   190,   191,     0,     0,   192,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   193,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   194,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   195,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,   238,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     196,     0,   106,     0,   197,     0,     5,   108,     6,     7,
       8,   231,    10,    11,     0,     0,     0,   187,     0,     0,
      15,    16,     0,    17,     0,   188,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   189,
       0,     0,     0,    33,   190,   191,     0,     0,   192,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   193,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   233,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   195,     0,    79,     0,     0,    81,
       0,     0,   234,    85,     0,   235,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   196,     0,   106,     0,   197,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     187,     0,     0,    15,    16,     0,    17,     0,   188,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   189,     0,     0,     0,    33,   190,   191,     0,
       0,   192,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   193,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   194,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   195,     0,    79,
       0,     0,    81,   310,   311,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   196,     0,   106,     0,
     197,     0,     0,   108,     5,   306,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   187,     0,     0,    15,    16,
       0,    17,     0,   188,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   189,     0,     0,
       0,    33,   190,   191,     0,     0,   192,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   193,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   194,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   195,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   196,   746,   106,     0,   197,     0,     0,   108,     5,
     306,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     187,     0,     0,    15,    16,     0,    17,     0,   188,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   189,     0,     0,     0,    33,   190,   191,     0,
       0,   192,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   193,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   194,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   195,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   196,     0,   106,     0,
     197,   756,     0,   108,     5,   306,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   187,     0,     0,    15,    16,
       0,    17,     0,   188,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   189,     0,     0,
       0,    33,   190,   191,     0,     0,   192,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   193,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   194,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   195,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   196,     0,   106,     0,   197,  1186,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   187,     0,
       0,    15,    16,     0,    17,     0,   188,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     189,     0,     0,     0,    33,   190,   191,     0,     0,   192,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     193,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   194,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   195,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,   238,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   196,     0,   106,     0,   197,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   187,     0,     0,    15,    16,     0,    17,     0,   188,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
     287,    29,     0,   189,     0,     0,     0,    33,   190,   191,
       0,     0,   192,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   193,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   194,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   195,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   105,     0,   106,
       0,   197,     0,     0,   108,     5,   306,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   187,     0,     0,    15,
      16,     0,    17,     0,   188,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   189,     0,
       0,     0,    33,   190,   191,     0,     0,   192,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   193,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     194,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   195,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   196,     0,   106,     0,   197,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   187,
       0,     0,    15,    16,     0,    17,     0,   188,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   189,     0,     0,     0,    33,   190,   191,     0,     0,
     192,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   193,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   194,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   195,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   196,     0,   106,     0,   197,
     761,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   187,     0,     0,    15,    16,     0,    17,     0,
     188,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   189,     0,     0,     0,    33,   190,
     191,     0,     0,   192,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   193,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   194,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   195,
       0,    79,     0,     0,    81,     0,     0,     0,    85,   921,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   196,     0,
     106,     0,   197,     0,     0,   108,     5,   306,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   187,     0,     0,
      15,    16,     0,    17,     0,   188,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,   717,     0,   189,
       0,     0,     0,    33,   190,   191,     0,     0,   192,    39,
       0,     0,     0,   718,     0,     0,    43,     0,     0,   193,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   194,     0,     0,     0,     0,     0,     0,     0,    73,
     719,    75,    76,    77,   720,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   196,     0,   106,     0,  1121,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     187,     0,     0,    15,    16,     0,    17,     0,   188,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   189,     0,     0,     0,    33,   190,   191,     0,
       0,   192,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   193,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   194,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   195,     0,    79,
       0,     0,    81,     0,     0,     0,    85,  1146,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   196,     0,   106,     0,
     197,     0,     0,   108,     5,   306,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   187,     0,     0,    15,    16,
       0,    17,     0,   188,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   189,     0,     0,
       0,    33,   190,   191,     0,     0,   192,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   193,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   194,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   195,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   196,     0,   106,     0,  1121,     0,     0,   108,     5,
     306,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     187,     0,     0,    15,    16,     0,    17,     0,   188,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   189,     0,     0,     0,    33,   190,   191,     0,
       0,  1100,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   193,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   194,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   195,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   196,     0,   106,     0,
     197,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   187,     0,     0,    15,    16,     0,    17,
       0,   188,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   189,     0,     0,     0,    33,
     190,   191,     0,     0,   192,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   193,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   194,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     195,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   196,
       0,   106,     0,   197,     0,     5,   108,     6,     7,     8,
     227,    10,    11,     0,     0,     0,   187,     0,     0,    15,
      16,     0,    17,     0,   188,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   189,     0,
       0,     0,    33,   190,   191,     0,     0,   192,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   193,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     228,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   195,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   196,     0,   106,     0,   197,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   187,
       0,     0,    15,    16,     0,    17,     0,   188,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   189,     0,     0,     0,    33,   190,   191,     0,     0,
     192,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   193,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   194,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   195,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   105,     0,   106,     0,   197,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   187,     0,     0,    15,    16,     0,    17,     0,
     188,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   189,     0,     0,     0,    33,   190,
     191,     0,     0,   192,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   193,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   194,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   195,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   680,     0,   196,     0,
     106,     0,   197,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   187,     0,     0,    15,    16,
       0,    17,     0,   188,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,   717,     0,   189,     0,     0,
       0,    33,   190,   191,     0,     0,   192,    39,     0,     0,
       0,   718,     0,     0,    43,     0,     0,   193,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   194,
       0,     0,     0,     0,     0,     0,     0,    73,   719,    75,
      76,    77,   720,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   196,     0,   106,     0,   721,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   187,     0,
       0,    15,    16,     0,    17,     0,   188,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     189,     0,     0,     0,    33,   190,   191,     0,     0,   192,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     193,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   194,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   195,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   196,     0,   106,     0,   721,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   187,     0,     0,    15,    16,     0,    17,     0,   188,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   189,     0,     0,     0,    33,   190,   191,
       0,     0,   818,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   193,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   194,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   195,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   196,     0,   106,
       0,   197,     0,     5,   108,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   187,     0,     0,    15,    16,     0,
      17,     0,   188,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   189,     0,     0,     0,
      33,   190,   191,     0,     0,   821,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   193,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   194,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   195,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     196,     0,   106,     0,   197,     0,     5,   108,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   187,     0,     0,
      15,    16,     0,    17,     0,   188,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   189,
       0,     0,     0,    33,   190,   191,     0,     0,  1094,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   193,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   194,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   195,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   196,     0,   106,     0,   197,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     187,     0,     0,    15,    16,     0,    17,     0,   188,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   189,     0,     0,     0,    33,   190,   191,     0,
       0,  1095,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   193,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   194,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   195,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   196,     0,   106,     0,
     197,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   187,     0,     0,    15,    16,     0,    17,
       0,   188,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   189,     0,     0,     0,    33,
     190,   191,     0,     0,  1097,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   193,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   194,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     195,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   196,
       0,   106,     0,   197,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   187,     0,     0,    15,
      16,     0,    17,     0,   188,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   189,     0,
       0,     0,    33,   190,   191,     0,     0,  1098,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   193,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     194,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   195,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   196,     0,   106,     0,   197,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   187,
       0,     0,    15,    16,     0,    17,     0,   188,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   189,     0,     0,     0,    33,   190,   191,     0,     0,
    1099,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   193,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   194,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   195,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   196,     0,   106,     0,   197,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   187,     0,     0,    15,    16,     0,    17,     0,
     188,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   189,     0,     0,     0,    33,   190,
     191,     0,     0,  1100,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   193,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   194,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   195,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   196,     0,
     106,     0,   197,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   187,     0,     0,    15,    16,
       0,    17,     0,   188,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,   717,     0,   189,     0,     0,
       0,    33,   190,   191,     0,     0,   192,    39,     0,     0,
       0,   718,     0,     0,    43,     0,     0,   193,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   194,
       0,     0,     0,     0,     0,     0,     0,    73,   719,    75,
      76,    77,   720,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   196,     0,   106,     0,  1211,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   187,     0,
       0,    15,    16,     0,    17,     0,   188,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     189,     0,     0,     0,    33,   190,   191,     0,     0,   192,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     193,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   194,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   195,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   936,     0,
     937,     0,     0,     0,    93,    94,    95,    96,    97,   938,
       0,     0,     0,    98,   939,   256,   940,   941,     0,     0,
       0,     0,   942,    99,     0,     0,   100,   101,   102,     0,
       0,   259,   104,   189,     0,     0,   106,   943,  1211,     0,
       0,   108,     0,   944,     0,     0,     0,   262,     0,     0,
     945,     0,   946,     0,     0,     0,     0,     0,     0,     0,
     947,     0,     0,     0,     0,     0,     0,   948,   949,     0,
       0,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,   950,     0,     0,     0,     0,     0,
       0,     0,     0,   269,   270,     0,   951,     0,   272,     0,
     952,     0,     0,   953,     0,     0,     0,   954,     0,     0,
     276,     0,     0,   955,     0,     0,     0,     0,     0,     0,
       0,     0,   490,   409,   410,   411,   412,   413,     0,     0,
     416,   417,   418,   419,     0,   421,   422,   956,   957,   958,
     959,   960,   961,     0,   962,     0,     0,     0,   963,   964,
     965,   966,   967,   968,   969,   970,   971,   972,   973,     0,
     974,     0,     0,   975,   976,   977,   978,     0,     5,   979,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   187,
       0,     0,    15,    16,     0,    17,     0,   188,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   189,     0,     0,     0,    33,   190,   191,     0,     0,
     192,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   193,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   194,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   195,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,  -593,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   423,   101,   102,
    -632,     0,  -632,   104,     0,   196,     0,   106,     0,   197,
       5,   306,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   187,     0,     0,    15,    16,     0,    17,     0,   188,
       0,   380,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   189,     0,     0,     0,    33,   190,   191,
       0,     0,   192,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   193,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,   381,    63,
       0,     0,   382,     0,    66,   194,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   195,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,     0,  -490,     0,     0,     0,     0,     0,     0,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,    95,
       0,   393,     0,  -490,     0,     0,     0,  -490,   399,   400,
       0,     0,   403,     0,     0,   404,   405,   406,     0,     0,
       0,   102,     0,     0,     0,     0,   407,   196,     0,   106,
    -490,  1121,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   187,     0,     0,    15,    16,     0,    17,
       0,   188,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   189,     0,     0,     0,    33,
     190,   191,     0,     0,   192,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   193,     0,     0,    47,    48,
    -472,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   194,     0,     0,
       0,  -472,     0,     0,     0,    73,    74,    75,    76,    77,
     195,     0,    79,     0,  -472,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,  -472,     0,     0,
       0,     0,     0,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,     0,     0,
       0,    95,     0,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   187,     0,     0,    15,    16,     0,
      17,     0,   188,   102,  -472,    21,     0,   423,     0,  -472,
    -632,   106,  -632,  1211,    29,     0,   189,     0,     0,     0,
      33,   190,   191,     0,     0,   192,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   193,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   194,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   195,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,     0,    95,     0,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   187,     0,     0,    15,    16,
       0,    17,     0,   188,   102,     0,    21,     0,     0,     0,
     196,     0,   106,     0,   197,    29,     0,   189,     0,     0,
       0,    33,   190,   191,     0,     0,   192,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   193,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   194,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   195,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   187,     0,     0,    15,
      16,     0,    17,     0,   188,   102,     0,    21,     0,     0,
       0,   196,     0,   106,     0,   721,    29,     0,   189,     0,
       0,     0,    33,   190,   191,     0,     0,   192,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   193,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     194,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   195,     0,    79,     0,     0,    81,     0,
    1161,     0,    85,     0,     0,    87,     0,     0,    89,   938,
       0,     0,     0,     0,   255,   256,   940,   257,     0,     0,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,   259,     0,     0,    95,     0,     0,   943,     0,     0,
       0,     0,     0,   261,     0,     0,     0,   262,     0,     0,
     263,     0,   946,     0,     0,     0,     0,     0,     0,     0,
     264,     0,     0,     0,   106,     0,   197,   948,   266,     0,
       0,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,   268,     0,     0,     0,     0,     0,
       0,     0,   378,   269,   270,     0,   271,     0,   272,   379,
    1162,     0,     0,   953,     0,     0,     0,   275,     0,     0,
     276,     0,   380,   277,     0,     0,     0,     0,     0,     0,
       0,     0,   490,   409,   410,   411,   412,   413,     0,     0,
     416,   417,   418,   419,     0,   421,   422,   956,   957,   958,
     959,   960,   961,     0,   962,     0,     0,     0,   963,   964,
     965,   966,   967,   968,   969,   970,   971,   972,   973,     0,
     974,     0,     0,   975,   976,   977,   978,     0,   378,   381,
       0,     0,     0,   382,     0,   379,     0,     0,     0,     0,
       0,     0,   489,     0,     0,     0,     0,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   383,   490,   409,   410,   411,   412,
     413,     0,     0,   416,   417,   418,   419,     0,   421,   422,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,   378,   381,   404,   405,   406,   382,
     438,   379,     0,     0,     0,     0,     0,   407,  1011,     0,
       0,     0,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     383,   490,   409,   410,   411,   412,   413,     0,     0,   416,
     417,   418,   419,     0,   421,   422,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,     0,   392,   393,   394,
     395,   396,     0,   397,   398,   399,   400,   401,   402,   403,
     378,   381,   404,   405,   406,   382,   438,   379,     0,     0,
       0,     0,     0,   407,  1018,     0,     0,     0,     0,     0,
     380,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   383,   490,   409,   410,
     411,   412,   413,     0,     0,   416,   417,   418,   419,     0,
     421,   422,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,     0,   392,   393,   394,   395,   396,     0,   397,
     398,   399,   400,   401,   402,   403,   378,   381,   404,   405,
     406,   382,   438,   379,     0,     0,     0,     0,     0,   407,
    1177,     0,     0,     0,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   383,   490,   409,   410,   411,   412,   413,     0,
       0,   416,   417,   418,   419,     0,   421,   422,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,   378,   381,   404,   405,   406,   382,   438,   379,
       0,     0,     0,     0,     0,   407,  1178,     0,     0,     0,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   383,   490,
     409,   410,   411,   412,   413,     0,     0,   416,   417,   418,
     419,     0,   421,   422,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,     0,   392,   393,   394,   395,   396,
       0,   397,   398,   399,   400,   401,   402,   403,   378,   381,
     404,   405,   406,   382,   438,   379,     0,     0,     0,     0,
       0,   407,  1179,     0,     0,     0,     0,     0,   380,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   383,   490,   409,   410,   411,   412,
     413,     0,     0,   416,   417,   418,   419,     0,   421,   422,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,   378,   381,   404,   405,   406,   382,
     438,   379,     0,     0,     0,     0,     0,   407,  1180,     0,
       0,     0,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     383,   490,   409,   410,   411,   412,   413,     0,     0,   416,
     417,   418,   419,     0,   421,   422,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,     0,   392,   393,   394,
     395,   396,     0,   397,   398,   399,   400,   401,   402,   403,
     378,   381,   404,   405,   406,   382,   438,   379,     0,     0,
       0,     0,     0,   407,  1204,     0,     0,     0,     0,     0,
     380,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   383,   490,   409,   410,
     411,   412,   413,     0,     0,   416,   417,   418,   419,     0,
     421,   422,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,     0,   392,   393,   394,   395,   396,     0,   397,
     398,   399,   400,   401,   402,   403,     0,   381,   404,   405,
     406,   382,   438,     0,     0,     0,     0,     0,     0,   407,
    1205,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   383,   490,   409,   410,   411,   412,   413,     0,
       0,   416,   417,   418,   419,     0,   421,   422,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,   378,     0,   404,   405,   406,     0,   438,   379,
       0,     0,     0,     0,     0,   407,     0,     0,     0,     0,
       0,     0,   380,   475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   476,     0,
       0,     0,   378,     0,     0,     0,     0,     0,     0,   379,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   380,   469,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   470,   381,
       0,     0,   378,   382,     0,     0,     0,     0,     0,   379,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   380,   287,     0,     0,     0,     0,     0,     0,
     472,     0,     0,     0,   383,     0,     0,     0,   480,   381,
       0,     0,     0,   382,     0,     0,     0,     0,     0,     0,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,   383,     0,   404,   405,   406,   381,
     438,     0,     0,   382,     0,     0,     0,   407,     0,     0,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,   383,     0,   404,   405,   406,     0,
     438,     0,     0,     0,     0,     0,     0,   407,     0,     0,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,   378,     0,   404,   405,   406,     0,
     438,   379,     0,     0,     0,     0,     0,   407,     0,     0,
       0,     0,     0,     0,   380,     0,     0,     0,     0,   482,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,   772,     0,     0,     0,     0,
       0,   379,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   380,   483,     0,     0,     0,   773,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   381,     0,     0,     0,   382,     0,     0,     0,   378,
       0,     0,     0,     0,     0,     0,   379,     0,     0,     0,
       0,     0,     0,     0,     0,   774,     0,     0,     0,   380,
     855,     0,     0,     0,     0,     0,   383,     0,     0,     0,
       0,   381,     0,     0,     0,   382,     0,     0,     0,     0,
       0,     0,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,     0,   392,   393,   394,   395,   396,     0,   397,
     398,   399,   400,   401,   402,   403,   383,   484,   404,   405,
     406,     0,     0,     0,     0,     0,   381,     0,     0,   407,
     382,     0,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,     0,   392,   393,   394,   395,   396,     0,   397,
     398,   399,   400,   401,   402,   403,     0,   472,   404,   405,
     406,   383,     0,     0,     0,     0,     0,     0,     0,   407,
       0,     0,     0,     0,     0,     0,     0,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,     0,   392,   393,
     394,   395,   396,     0,   397,   398,   399,   400,   401,   402,
     403,   378,     0,   404,   405,   406,     0,   438,   379,     0,
       0,     0,     0,     0,   407,     0,     0,     0,     0,     0,
       0,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   440,     0,     0,
       0,   378,     0,     0,     0,     0,     0,     0,   379,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   380,   287,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   381,     0,
       0,   378,   382,     0,     0,     0,     0,     0,   379,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   380,   637,     0,     0,     0,     0,     0,     0,   182,
       0,     0,     0,   383,     0,     0,     0,   638,   381,     0,
       0,     0,   382,     0,     0,     0,     0,     0,     0,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,     0,
     392,   393,   394,   395,   396,     0,   397,   398,   399,   400,
     401,   402,   403,   383,     0,   404,   405,   406,   381,     0,
       0,     0,   382,     0,     0,     0,   407,     0,     0,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,     0,
     392,   393,   394,   395,   396,     0,   397,   398,   399,   400,
     401,   402,   403,   383,     0,   404,   405,   406,     0,   438,
       0,     0,     0,     0,     0,     0,   407,     0,     0,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,     0,
     392,   393,   394,   395,   396,     0,   397,   398,   399,   400,
     401,   402,   403,   378,     0,   404,   405,   406,     0,     0,
     379,     0,     0,     0,     0,     0,   407,     0,     0,     0,
       0,     0,     0,   380,   639,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   640,
       0,     0,     0,   378,     0,     0,     0,     0,     0,     0,
     379,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   380,   847,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     381,     0,     0,   378,   382,     0,     0,     0,     0,     0,
     379,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   383,     0,     0,     0,     0,
     381,     0,     0,     0,   382,     0,     0,     0,     0,     0,
       0,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,     0,   392,   393,   394,   395,   396,     0,   397,   398,
     399,   400,   401,   402,   403,   383,     0,   404,   405,   406,
     381,     0,     0,     0,   382,     0,     0,     0,   407,     0,
       0,   384,   385,   871,   386,   387,   388,     0,   389,   390,
     391,     0,   392,   393,   394,   395,   396,     0,   397,   398,
     399,   400,   401,   402,   403,   383,     0,   404,   405,   406,
       0,   438,     0,     0,     0,     0,     0,     0,   407,     0,
       0,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,     0,   392,   393,   394,   395,   396,     0,   397,   398,
     399,   400,   401,   402,   403,   378,     0,   404,   405,   406,
       0,   438,   379,     0,     0,     0,     0,     0,   407,     0,
       0,     0,     0,     0,     0,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   378,     0,     0,     0,     0,
       0,     0,   379,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   380,     0,     0,     0,     0,
    1068,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   381,     0,     0,     0,   382,     0,     0,     0,
     378,   517,     0,     0,     0,     0,     0,   379,     0,     0,
       0,     0,     0,     0,     0,     0,  1069,     0,     0,     0,
     380,     0,     0,   472,     0,     0,     0,   383,     0,     0,
       0,     0,   381,     0,     0,     0,   382,     0,     0,     0,
       0,     0,     0,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,     0,   392,   393,   394,   395,   396,     0,
     397,   398,   399,   400,   401,   402,   403,   383,     0,   404,
     405,   406,     0,     0,     0,     0,     0,   381,   913,     0,
     407,   382,     0,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,     0,   392,   393,   394,   395,   396,     0,
     397,   398,   399,   400,   401,   402,   403,   378,     0,   404,
     405,   406,   383,     0,   379,     0,     0,     0,     0,     0,
     407,     0,     0,     0,     0,     0,     0,   380,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,   378,     0,   404,   405,   406,     0,     0,   379,
       0,     0,     0,     0,     0,   407,     0,     0,     0,     0,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   381,     0,     0,     0,   382,     0,
       0,     0,   378,     0,     0,     0,     0,     0,     0,   379,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   380,     0,     0,     0,     0,     0,     0,   383,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   381,
       0,     0,     0,   382,     0,   384,   385,     0,   386,   387,
     388,     0,   389,   390,   391,     0,   392,   393,   394,   395,
     396,     0,   397,   398,   399,   400,   401,   402,   403,     0,
       0,   404,   405,   406,   383,   528,     0,     0,     0,   381,
       0,     0,   407,   382,     0,     0,     0,     0,     0,     0,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,   383,   532,   404,   405,   406,     0,
       0,     0,     0,     0,     0,     0,     0,   407,     0,     0,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,   378,   534,   404,   405,   406,     0,
       0,   379,     0,     0,     0,     0,     0,   407,     0,     0,
       0,     0,     0,     0,   380,   218,     0,     0,     0,    22,
      23,     0,     0,     0,  1074,     0,     0,     0,     0,   219,
       0,    31,   220,     0,   378,     0,     0,    37,     0,     0,
       0,   379,     0,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,   381,    60,     0,    62,   382,  1075,     0,     0,  1076,
     378,   223,     0,    68,     0,     0,     0,   379,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     380,     0,    84,     0,     0,    86,   383,     0,    88,     0,
       0,   381,     0,     0,     0,   382,     0,     0,     0,     0,
       0,     0,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,     0,   392,   393,   394,   395,   396,     0,   397,
     398,   399,   400,   401,   402,   403,   383,   536,   404,   405,
     406,     0,     0,     0,     0,     0,     0,   381,   103,   407,
       0,   382,   384,   385,  1172,   386,   387,   388,     0,   389,
     390,   391,     0,   392,   393,   394,   395,   396,     0,   397,
     398,   399,   400,   401,   402,   403,   378,   542,   404,   405,
     406,   559,   383,   379,     0,     0,     0,     0,     0,   407,
       0,     0,     0,     0,     0,     0,   380,     0,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,   378,     0,   404,   405,   406,     0,     0,   379,
       0,     0,     0,     0,     0,   407,     0,     0,     0,     0,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   381,     0,     0,     0,   382,     0,     0,
       0,     0,   378,   768,     0,     0,   641,     0,     0,   379,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   380,     0,     0,     0,     0,     0,   383,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   381,
       0,     0,     0,   382,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,     0,   392,   393,   394,   395,   396,
       0,   397,   398,   399,   400,   401,   402,   403,     0,     0,
     404,   405,   406,     0,   383,     0,     0,     0,     0,   381,
       0,   407,     0,   382,     0,     0,     0,     0,     0,     0,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,   383,     0,   404,   405,   406,     0,
       0,     0,     0,   747,     0,     0,     0,   407,     0,     0,
     384,   385,     0,   386,   387,   388,   380,   389,   769,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,   378,     0,   404,   405,   406,     0,
       0,   379,     0,     0,     0,     0,     0,   407,     0,     0,
       0,     0,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   381,     0,     0,     0,   382,     0,   378,
       0,     0,     0,     0,     0,     0,   379,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   380,
       0,     0,     0,     0,     0,     0,     0,     0,   383,     0,
       0,   381,     0,     0,     0,   382,     0,     0,     0,   378,
       0,     0,     0,     0,   384,   385,   379,   386,   387,   388,
       0,   389,   390,   391,     0,   392,   393,   394,   395,   380,
       0,   397,   398,   399,   400,   401,   383,   403,     0,     0,
     404,   405,   406,     0,     0,     0,   381,     0,     0,     0,
     382,   407,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,     0,   392,   393,   394,   395,   396,  -594,   397,
     398,   399,   400,   401,   402,   403,     0,   182,   404,   405,
     406,   383,     0,     0,     0,     0,   381,     0,     0,   407,
     382,     0,     0,     0,     0,     0,     0,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,     0,   392,   393,
     394,   395,   396,     0,   397,   398,   399,   400,   401,   402,
     403,   383,     0,   404,   405,   406,     0,     0,     0,     0,
       0,     0,     0,     0,   407,     0,     0,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,     0,   392,   393,
     394,   395,   396,     0,   397,   398,   399,   400,   401,   402,
     403,     0,   830,   404,   405,   406,   378,   862,     0,     0,
       0,     0,     0,   379,   407,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   378,     0,     0,     0,
       0,     0,     0,   379,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   380,     0,     0,   869,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   381,     0,     0,   378,   382,     0,     0,
       0,     0,     0,   379,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   383,     0,
       0,     0,     0,   381,     0,     0,     0,   382,     0,     0,
       0,     0,     0,     0,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,     0,   392,   393,   394,   395,   396,
       0,   397,   398,   399,   400,   401,   402,   403,   383,     0,
     404,   405,   406,   381,     0,     0,     0,   382,     0,     0,
       0,   407,     0,     0,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,     0,   392,   393,   394,   395,   396,
       0,   397,   398,   399,   400,   401,   402,   403,   383,     0,
     404,   405,   406,     0,     0,     0,     0,     0,     0,     0,
       0,   407,     0,     0,   384,   385,     0,   386,   387,   388,
       0,   389,   390,   391,     0,   392,   393,   394,   395,   396,
       0,   397,   398,   399,   400,   401,   402,   403,   378,   218,
     404,   405,   406,    22,    23,   379,     0,   897,  1074,     0,
       0,   407,     0,   219,     0,    31,   220,     0,   380,     0,
       0,    37,     0,     0,     0,     0,     0,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   378,
       0,     0,     0,     0,     0,     0,   379,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,   359,   380,
    1075,     0,     0,  1076,     0,   223,     0,    68,     0,     0,
       0,     0,     0,     0,     0,   381,     0,     0,     0,   382,
     378,     0,     0,     0,     0,     0,    84,   379,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
     380,   994,     0,     0,     0,     0,     0,     0,     0,     0,
     383,     0,     0,     0,     0,     0,   381,     0,     0,     0,
     382,     0,     0,     0,     0,     0,   384,   385,     0,   386,
     387,   388,     0,   389,   390,   391,     0,   392,   393,   394,
     395,   396,   103,   397,   398,   399,   400,   401,   402,   403,
     916,   383,   404,   405,   406,     0,     0,   381,     0,   898,
       0,   382,     0,   407,     0,     0,     0,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,     0,   392,   393,
     394,   395,   396,     0,   397,   398,   399,   400,   401,   402,
     403,     0,   383,   404,   405,   406,     0,     0,     0,     0,
       0,     0,     0,     0,   407,     0,     0,     0,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,   378,     0,   404,   405,   406,     0,     0,   379,
       0,     0,     0,     0,     0,   407,   218,     0,     0,     0,
      22,    23,   380,   997,     0,  1074,     0,     0,     0,     0,
     219,     0,    31,   220,     0,     0,     0,     0,    37,     0,
       0,     0,   378,     0,     0,    42,     0,     0,     0,   379,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   380,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,  1075,     0,   381,
    1076,     0,   223,   382,    68,     0,     0,   378,  1065,     0,
       0,     0,     0,     0,   379,     0,     0,     0,     0,     0,
       0,     0,     0,    84,     0,     0,    86,   380,     0,    88,
       0,     0,     0,     0,   383,     0,     0,     0,     0,   381,
       0,     0,     0,   382,     0,     0,     0,     0,     0,     0,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,   383,     0,   404,   405,   406,   103,
       0,     0,     0,     0,   381,  1175,     0,   407,   382,     0,
     384,   385,     0,   386,   387,   388,     0,   389,   390,   391,
       0,   392,   393,   394,   395,   396,     0,   397,   398,   399,
     400,   401,   402,   403,   378,     0,   404,   405,   406,   383,
       0,   379,     0,     0,     0,  1000,     0,   407,     0,     0,
       0,     0,     0,     0,   380,   384,   385,     0,   386,   387,
     388,     0,   389,   390,   391,     0,   392,   393,   394,   395,
     396,     0,   397,   398,   399,   400,   401,   402,   403,     0,
       0,   404,   405,   406,     0,     0,     0,     0,     0,     0,
       0,     0,   407,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     378,   381,     0,     0,     0,   382,     0,   379,     0,     0,
       0,     0,     0,     0,  1093,     0,     0,     0,     0,     0,
     380,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   383,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   384,   385,     0,   386,   387,   388,     0,   389,
     390,   391,     0,   392,   393,   394,   395,   396,     0,   397,
     398,   399,   400,   401,   402,   403,     0,   381,   404,   405,
     406,   382,     0,     0,     0,   378,  1115,     0,     0,   407,
    1096,     0,   379,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   380,     0,     0,     0,     0,
       0,     0,   383,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   384,   385,
       0,   386,   387,   388,     0,   389,   390,   391,     0,   392,
     393,   394,   395,   396,     0,   397,   398,   399,   400,   401,
     402,   403,     0,     0,   404,   405,   406,     0,     0,     0,
       0,   378,   381,     0,     0,   407,   382,     0,   379,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   383,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   384,   385,     0,   386,   387,   388,   380,
     389,   390,   391,     0,   392,   393,   394,   395,   396,     0,
     397,   398,   399,   400,   401,   402,   403,   378,   381,   404,
     405,   406,   382,     0,   379,     0,     0,     0,     0,     0,
     407,     0,     0,     0,     0,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   383,     0,     0,   381,     0,     0,     0,
     382,     0,     0,     0,     0,     0,     0,     0,     0,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,     0,
     392,   393,   394,   395,   396,     0,   397,   398,   399,   400,
     401,   402,   403,   378,   381,   404,   405,   406,   382,     0,
     379,     0,  1160,     0,     0,     0,   407,   384,   385,     0,
     386,   387,   388,   380,   389,   390,   391,     0,   392,   393,
     394,   395,     0,     0,   397,   398,   399,   400,   401,   383,
     403,     0,     0,   404,   405,   406,     0,     0,     0,     0,
       0,     0,     0,     0,   407,   384,   385,     0,   386,   387,
     388,     0,   389,   390,   391,     0,   392,   393,   394,   395,
     396,     0,   397,   398,   399,   400,   401,   402,   403,   378,
     381,   404,   405,   406,   382,     0,   379,     0,     0,     0,
    1200,     0,   407,  1220,     0,     0,     0,     0,     0,   380,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   383,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,     0,   392,   393,   394,   395,   396,     0,   397,   398,
     399,   400,   401,   402,   403,   378,   381,   404,   405,   406,
     382,     0,   379,     0,     0,     0,     0,     0,   407,  1221,
       0,     0,     0,     0,     0,   380,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   383,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,     0,   392,   393,
     394,   395,   396,     0,   397,   398,   399,   400,   401,   402,
     403,   378,   381,   404,   405,   406,   382,     0,   379,     0,
       0,     0,     0,     0,   407,  1222,     0,     0,     0,     0,
       0,   380,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   383,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   384,   385,     0,   386,   387,   388,     0,
     389,   390,   391,     0,   392,   393,   394,   395,   396,     0,
     397,   398,   399,   400,   401,   402,   403,   378,   381,   404,
     405,   406,   382,     0,   379,     0,     0,     0,     0,     0,
     407,  1223,     0,     0,     0,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   383,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,     0,
     392,   393,   394,   395,   396,     0,   397,   398,   399,   400,
     401,   402,   403,   378,   381,   404,   405,   406,   382,     0,
     379,     0,     0,     0,     0,     0,   407,  1224,     0,     0,
       0,     0,     0,   380,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   383,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   384,   385,     0,   386,   387,
     388,   380,   389,   390,   391,     0,   392,   393,   394,   395,
     396,     0,   397,   398,   399,   400,   401,   402,   403,   378,
     381,   404,   405,   406,   382,     0,   379,     0,     0,     0,
       0,     0,   407,  1225,     0,     0,     0,     0,     0,   380,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   383,     0,     0,   381,     0,
       0,     0,   382,     0,     0,     0,     0,     0,  1250,     0,
       0,   384,   385,     0,   386,   387,   388,     0,   389,   390,
     391,     0,   392,   393,   394,   395,   396,     0,   397,   398,
     399,   400,   401,   402,   403,   378,   381,   404,   405,   406,
     382,     0,   379,     0,     0,     0,     0,     0,   407,   384,
     385,     0,   386,   387,   388,   380,   389,   390,   391,     0,
       0,   393,   394,   395,     0,     0,   397,   398,   399,   400,
       0,   383,   403,     0,     0,   404,   405,   406,     0,     0,
       0,     0,     0,     0,     0,     0,   407,   384,   385,     0,
     386,   387,   388,     0,   389,   390,   391,     0,   392,   393,
     394,   395,   396,     0,   397,   398,   399,   400,   401,   402,
     403,   378,   381,   404,   405,   406,   382,     0,   379,     0,
       0,     0,     0,     0,   407,     0,     0,     0,     0,     0,
       0,   380,     0,     0,     0,     0,     0,     0,     0,     0,
     218,     0,     0,     0,    22,    23,     0,   383,     0,  1074,
       0,     0,     0,     0,   219,     0,    31,   220,     0,     0,
       0,     0,    37,   384,   385,     0,   386,   387,   388,    42,
     389,   390,   391,     0,   392,   393,   394,   395,   396,     0,
     397,   398,   399,   400,   401,   402,   403,   378,   381,   404,
     405,   406,   382,     0,   379,    58,     0,    60,  1279,    62,
     407,  1075,     0,     0,  1076,     0,   223,   380,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   383,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,   384,
     385,     0,   386,   387,   388,     0,   389,   390,   391,     0,
     392,   393,   394,   395,   396,     0,   397,   398,   399,   400,
     401,   402,   403,     0,   816,   404,   405,   406,   382,     0,
       0,     0,     0,     0,     0,     0,   407,     0,     0,     0,
       0,     0,     0,   103,     0,     0,     0,     0,     0,  1176,
       0,     0,   218,     0,     0,     0,    22,    23,     0,   383,
       0,  1074,     0,     0,     0,     0,   219,     0,    31,   220,
       0,     0,     0,     0,    37,   384,   385,     0,   386,   387,
     388,    42,   389,   390,   391,     0,   392,   393,   394,   395,
     396,     0,   397,   398,   399,   400,   401,   402,   403,     0,
       0,   404,   405,   406,     0,     0,     0,    58,     0,    60,
       0,    62,   407,  1075,     0,     0,  1076,     0,   223,   218,
      68,     0,     0,    22,    23,     0,     0,     0,  1074,     0,
       0,     0,     0,   219,     0,    31,   220,     0,     0,    84,
       0,    37,    86,     0,     0,    88,     0,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
    1075,     0,     0,  1076,     0,   223,     0,    68,   218,     0,
       0,     0,    22,    23,     0,   103,     0,  1074,     0,     0,
       0,  1226,   219,     0,    31,   220,    84,     0,     0,    86,
      37,     0,    88,     0,     0,     0,     0,    42,   779,     0,
     254,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   255,   256,     0,   257,     0,     0,
       0,     0,   258,    58,     0,    60,     0,    62,   780,  1075,
       0,   259,  1076,     0,   223,     0,    68,   260,     0,     0,
       0,     0,   103,   261,     0,     0,     0,   262,  1227,     0,
     263,     0,     0,     0,     0,    84,     0,     0,    86,     0,
     264,    88,     0,     0,     0,     0,     0,   265,   266,     0,
       0,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,   268,     0,     0,     0,     0,     0,
       0,     0,     0,   269,   270,     0,   271,     0,   272,     0,
     273,   254,     0,   274,     0,     0,     0,   275,     0,     0,
     276,   103,     0,   277,     0,   255,   256,  1228,   257,     0,
       0,     0,     0,   258,     0,    23,     0,     0,     0,     0,
       0,     0,   259,     0,     0,     0,     0,     0,   260,     0,
       0,     0,     0,     0,   261,     0,     0,     0,   262,     0,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,     0,     0,     0,     0,     0,     0,   265,   266,
       0,     0,     0,     0,     0,     0,   267,    60,     0,     0,
       0,     0,     0,     0,     0,   268,     0,     0,    68,     0,
       0,     0,     0,     0,   269,   270,     0,   271,     0,   272,
       0,   273,   254,     0,   274,     0,     0,   929,   275,     0,
       0,   276,     0,    88,   277,     0,   255,   256,     0,   257,
       0,     0,     0,     0,   258,     0,     0,     0,     0,     0,
       0,     0,     0,   259,     0,     0,     0,     0,     0,   260,
       0,     0,     0,     0,     0,   261,     0,     0,     0,   262,
       0,     0,   263,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   264,     0,     0,     0,     0,     0,     0,   265,
     266,     0,     0,     0,     0,     0,     0,   267,     0,     0,
       0,     0,     0,     0,     0,     0,   268,     0,     0,   254,
       0,     0,     0,     0,     0,   269,   270,     0,   271,     0,
     272,     0,   273,   255,   256,   274,   257,     0,     0,   275,
       0,   258,   276,     0,     0,   277,     0,   780,     0,     0,
     259,     0,     0,     0,     0,     0,   260,     0,     0,     0,
       0,     0,   261,     0,     0,     0,   262,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
       0,     0,     0,     0,     0,     0,   265,   266,     0,     0,
       0,     0,     0,     0,   267,     0,     0,     0,     0,     0,
       0,     0,     0,   268,     0,     0,  -301,     0,     0,     0,
       0,     0,   269,   270,     0,   271,     0,   272,     0,   273,
    -301,  -301,   274,  -301,     0,     0,   275,     0,  -301,   276,
       0,     0,   277,     0,     0,     0,     0,  -301,     0,     0,
       0,     0,     0,  -301,     0,     0,     0,     0,     0,  -301,
       0,     0,     0,  -301,     0,     0,  -301,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -301,     0,     0,     0,
       0,     0,     0,  -301,  -301,     0,     0,     0,     0,     0,
       0,  -301,     0,     0,     0,     0,     0,     0,     0,     0,
    -301,     0,     0,  -311,     0,     0,     0,     0,     0,  -301,
    -301,     0,  -301,     0,  -301,     0,  -301,  -311,  -311,  -301,
    -311,     0,     0,  -301,     0,  -311,  -301,     0,     0,  -301,
       0,     0,     0,     0,  -311,     0,     0,     0,     0,     0,
    -311,     0,     0,     0,     0,     0,  -311,     0,     0,     0,
    -311,     0,     0,  -311,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -311,     0,     0,     0,     0,     0,     0,
    -311,  -311,     0,     0,     0,     0,     0,     0,  -311,     0,
       0,     0,     0,     0,     0,     0,     0,  -311,     0,     0,
     254,     0,     0,     0,     0,     0,  -311,  -311,     0,  -311,
       0,  -311,     0,  -311,   255,   256,  -311,   257,     0,     0,
    -311,     0,   258,  -311,     0,     0,  -311,     0,     0,     0,
       0,   259,     0,     0,     0,     0,     0,   260,     0,     0,
       0,     0,     0,   261,     0,     0,     0,   262,     0,     0,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,     0,     0,     0,     0,     0,     0,   265,   266,     0,
       0,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,   268,     0,     0,  -302,     0,     0,
       0,     0,     0,   269,   270,     0,   271,     0,   272,     0,
     273,  -302,  -302,   274,  -302,     0,     0,   275,     0,  -302,
     276,     0,     0,   277,     0,     0,     0,     0,  -302,     0,
       0,     0,     0,     0,  -302,     0,     0,     0,     0,     0,
    -302,     0,     0,     0,  -302,     0,     0,  -302,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -302,     0,     0,
       0,     0,     0,     0,  -302,  -302,     0,     0,     0,     0,
       0,     0,  -302,     0,     0,     0,     0,     0,     0,     0,
       0,  -302,     0,     0,  -192,     0,     0,     0,     0,     0,
    -302,  -302,     0,  -302,     0,  -302,     0,  -302,  -192,  -192,
    -302,  -192,     0,     0,  -302,     0,  -192,  -302,     0,     0,
    -302,     0,     0,     0,     0,  -192,     0,     0,     0,     0,
       0,  -192,     0,     0,     0,     0,     0,  -192,     0,     0,
       0,  -192,     0,     0,  -192,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -192,     0,     0,     0,     0,     0,
       0,  -192,  -192,     0,     0,     0,     0,     0,     0,  -192,
       0,     0,     0,     0,     0,     0,     0,     0,  -192,     0,
       0,  -184,     0,     0,     0,     0,     0,  -192,  -192,     0,
    -192,     0,  -192,     0,  -192,  -184,  -184,  -192,  -184,     0,
       0,  -192,     0,  -184,  -192,     0,     0,  -192,     0,     0,
       0,     0,  -184,     0,     0,     0,     0,     0,  -184,     0,
       0,     0,     0,     0,  -184,     0,     0,     0,  -184,     0,
       0,  -184,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -184,     0,     0,     0,     0,     0,     0,  -184,  -184,
       0,     0,     0,     0,     0,     0,  -184,     0,     0,     0,
       0,     0,     0,     0,     0,  -184,     0,     0,     0,     0,
       0,     0,     0,     0,  -184,  -184,     0,  -184,     0,  -184,
       0,  -184,     0,     0,  -184,     0,     0,     0,  -184,     0,
       0,  -184,     0,     0,  -184
};

static const yytype_int16 yycheck[] =
{
      12,    65,   483,     2,    16,   429,     2,   210,    20,   166,
      19,   537,   671,   242,    26,    22,   790,   253,   497,    31,
      32,   504,    34,    35,    36,    37,    38,    27,   774,   152,
     657,   234,   773,   716,   661,    33,   663,    49,  1131,    51,
      52,   984,   985,     1,   891,    57,     1,    59,   927,    61,
      75,  1039,   780,    69,    33,     3,     3,    69,    70,    71,
      72,    73,    74,  1036,     3,    33,    78,   303,    80,    33,
      82,    83,    33,   530,    92,    87,    48,    70,    90,    91,
      56,    93,    47,    95,   120,   321,    98,    99,   211,   101,
     120,   107,   117,   105,   106,   107,   108,     3,    69,    18,
      33,     1,    37,   118,  1119,   102,    56,   140,   146,   105,
      66,    48,  1257,   106,    20,   148,   140,   165,     1,    61,
      75,    48,   120,   146,   148,   354,   355,     0,    34,    85,
    1275,   146,   815,    48,   105,   106,   107,   175,    73,   175,
     509,   120,   989,   802,    92,   175,   171,  1120,   171,   114,
     162,    48,   170,    90,    48,   103,   103,    63,   222,    78,
      56,    61,   117,   642,   103,    48,  1259,   125,   537,  1157,
    1185,   328,   170,   170,   146,   187,   118,    96,   190,   191,
     192,   197,   146,   195,   196,   197,   120,   145,   146,   425,
     426,   170,    92,  1040,   113,  1074,   172,   103,   655,  1078,
     209,   929,   170,   109,    48,   888,   170,   834,   220,   170,
     837,   838,    92,   146,   226,   173,   171,   217,   118,   146,
    1163,  1164,   172,   230,  1103,   196,   197,  1106,  1107,   236,
    1186,   146,   715,    48,   146,    54,     3,   170,   245,   146,
     304,   253,   125,   125,   146,   145,     3,   671,   175,   146,
      17,    18,   146,    20,    83,    74,    56,    86,    25,   171,
     175,  1240,   145,   146,   170,    84,   173,    34,   150,   140,
     170,   173,  1119,    40,  1153,    90,   120,   148,   175,    46,
     125,   175,   165,    50,  1240,   165,    53,    34,    35,    36,
     170,   303,  1171,   774,  1173,  1174,    63,   776,   310,   311,
    1279,   146,   146,    70,    71,   150,   146,   146,    56,   321,
    1005,    78,   165,  1008,  1009,  1089,   146,    82,    83,   172,
      87,  1067,    49,  1069,   146,  1066,   165,  1068,   173,    96,
      97,   175,    99,   173,   101,   458,   103,   349,  1185,   106,
      32,   171,   465,   110,   111,    72,   113,   346,   827,   116,
     346,   173,   364,   365,   165,    82,    83,  1016,     1,    56,
     107,   172,    90,   174,   376,   361,   378,   379,   380,   381,
     382,   383,    56,   385,   386,   387,   388,   389,   802,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   146,    89,   230,   882,
      88,    93,   414,   415,   236,   172,    47,  1102,   420,  1104,
    1105,   423,   424,   425,   426,   427,   428,   429,    61,   172,
      27,   414,   415,   173,   112,    66,   146,   420,   440,    56,
     423,   424,   146,   445,   146,     3,   448,    27,   919,    89,
     146,    27,   438,    93,    85,   165,   675,   676,   140,    92,
     146,   467,    20,   145,   172,   467,   148,   469,   470,   173,
     152,   173,   105,   475,   476,    51,    34,   173,   480,   165,
     482,   483,    79,   114,   146,   118,    56,   489,   146,   146,
     492,   208,   146,    90,   165,   497,    27,     8,    89,    79,
     171,    77,    93,    79,   146,    63,   467,   165,   165,   511,
      90,   173,   145,   515,    90,   501,   518,   519,   115,   173,
     517,   523,   239,   240,   241,   242,   243,   244,   530,   109,
    1046,   173,   165,   250,   251,   754,   755,   170,   140,   115,
      27,    27,   146,   145,   546,   103,   148,   549,    79,   551,
     152,   109,   745,   555,   556,   557,    56,   559,   560,    90,
      56,    48,   523,   280,   146,    52,   549,   573,   551,   173,
     100,   573,   146,   290,    56,   292,   565,   170,   109,   565,
     146,   165,   299,   165,   115,   546,   146,   171,   549,    76,
     551,   165,    79,    79,    81,   556,  1067,   172,  1069,   165,
     317,     2,  1016,    90,    90,   165,    56,   125,   172,    56,
     174,    12,   573,   172,   170,   174,   172,   172,   335,   336,
      56,    56,   109,   109,    25,    56,    56,    28,   146,    85,
      56,   226,   150,   165,   172,   637,   638,   639,   640,   641,
     642,   643,   644,   790,   381,   382,   125,   646,   647,   165,
     172,   653,   174,   655,   549,   173,   551,   172,   253,   174,
     172,   164,   174,   665,   172,   667,   174,   165,    48,   671,
     653,   673,   151,   156,   153,   154,   165,    78,   157,   158,
    1039,    61,    56,    61,   686,   687,   173,  1046,   165,   165,
     427,   428,   653,   165,   165,   170,   165,    61,   119,    88,
     165,   125,   172,   440,   105,   172,   172,    48,   303,   110,
     172,   172,    92,   172,    92,   721,   165,   719,   720,   721,
      61,     8,    66,   725,   441,   145,   321,   105,    92,   170,
     148,    69,   172,   470,   125,   173,   165,   146,   118,   476,
     118,   105,   725,   480,    48,   125,    32,   172,   165,   125,
     125,    92,    35,   146,   118,    35,   125,   474,   173,   173,
     721,   478,   173,   173,   766,    21,   146,   146,   770,   175,
     772,   773,   774,   175,   776,   170,     2,   118,   172,    61,
     165,   165,   183,   165,   125,   165,    12,   165,    27,   175,
     170,   172,   170,   173,   165,   390,   173,   165,  1157,    25,
     802,   165,    28,    89,   165,   146,   170,    93,   165,    48,
      92,   146,   165,    52,   816,   817,   818,   165,   555,   821,
      35,  1047,  1048,   105,   165,   827,    35,  1186,    66,   170,
     425,   426,   173,   146,   173,    32,   118,    76,   171,   173,
      79,  1200,    81,   845,   173,   847,   146,   171,   125,    90,
     852,    90,    78,   855,   140,   173,    82,    83,   860,   145,
     862,   147,   148,    33,    33,    32,   152,   869,   175,   170,
     109,   170,   170,    35,   160,   165,   145,    90,   175,   105,
     281,  1240,   168,   165,   110,   145,   287,   172,   170,   165,
     148,   172,    89,   165,   880,   901,    93,   175,  1257,   901,
     175,   638,   497,   640,   171,   148,   172,   140,   148,   911,
      35,   913,   148,   148,   916,   140,  1275,   919,   148,   148,
    1279,   148,    89,   140,   148,   148,    93,   148,   140,   931,
     140,   173,   148,   170,   165,   530,   170,   165,   170,   105,
     901,   140,  1089,   140,   141,   346,   143,   144,   145,  1191,
     147,   148,   669,   670,   173,   152,   140,   183,   675,   676,
     361,   678,   679,   160,   165,   173,   173,   165,   173,   166,
     167,   168,   165,   140,   141,   165,   693,   979,   145,   164,
     147,   148,   164,   175,   766,   152,   146,    24,   495,   361,
     515,   993,   994,   160,   996,   997,  1078,   999,  1000,   166,
     167,   168,  1244,  1245,  1246,  1247,  1248,  1249,    90,  1011,
    1012,   875,  1014,     2,  1016,   329,  1018,  1019,   932,  1089,
    1022,  1085,  1076,    12,   741,   905,   880,   707,   935,  1259,
    1216,  1243,  1250,    37,   512,    31,    25,   438,    -1,    28,
      -1,    -1,    -1,  1045,  1046,  1047,  1048,   642,    -1,    -1,
      -1,  1053,    -1,  1055,    -1,   281,    -1,  1059,    -1,    -1,
     655,   287,    -1,  1065,  1066,  1067,  1068,  1069,   469,    -1,
      -1,    -1,    32,    -1,   475,    -1,    -1,    -1,  1039,   816,
     817,    -1,    -1,    -1,    -1,    -1,   487,    -1,   489,    78,
      -1,  1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,  1085,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   828,   829,  1115,    -1,  1121,   105,    -1,    -1,  1121,
     346,   110,    -1,    -1,  1126,   842,    -1,   844,  1130,    89,
      -1,    -1,    -1,    93,   851,   361,    -1,   854,    -1,    -1,
     857,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,   553,    -1,    -1,    -1,   557,    -1,    -1,   876,
    1121,    -1,    -1,    -1,   565,  1126,    -1,    -1,    48,    -1,
      -1,    -1,    52,   768,   769,  1177,  1178,  1179,  1180,    -1,
     140,   776,    48,    -1,    -1,   145,    -1,   147,   148,    -1,
      -1,   908,   152,    -1,   183,    61,    76,    -1,  1200,    79,
     160,    81,  1204,  1205,    -1,  1211,   166,   167,   168,  1211,
      90,    -1,   438,    -1,    -1,    -1,    -1,    -1,  1220,  1221,
    1222,  1223,  1224,  1225,    -1,  1186,    92,    -1,    -1,   109,
      -1,    -1,   827,   634,    -1,    -1,    -1,  1239,  1240,    -1,
    1242,    -1,    -1,   469,    -1,    -1,     2,    -1,    -1,   475,
    1211,    -1,   118,    -1,    -1,    -1,    12,    -1,    -1,   125,
      -1,   487,    -1,   489,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    32,    28,  1275,    -1,    -1,    -1,  1279,    -1,  1240,
     146,    -1,    -1,    -1,    -1,    -1,    -1,     2,   689,    -1,
      -1,   692,   281,    -1,    -1,    -1,    -1,    12,   287,   165,
      -1,    -1,    -1,    -1,   170,    -1,    -1,   173,    -1,    -1,
      25,    -1,    -1,    28,    -1,    -1,    -1,    -1,  1279,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,   553,    89,    -1,
      -1,   557,    93,     2,  1051,    -1,    -1,    -1,    -1,   565,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   105,
      -1,    -1,   753,    -1,   110,    -1,    25,   346,   759,    28,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    32,   143,   144,   145,    -1,   147,   148,    -1,   790,
     105,   152,  1109,  1110,    -1,   110,  1113,  1114,   159,   160,
      -1,    -1,   163,    -1,    -1,   166,   167,   168,    -1,    78,
      -1,    -1,    -1,    -1,  1131,   816,   817,    -1,    -1,    -1,
      -1,    -1,  1139,    -1,  1141,    -1,  1143,   183,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,    89,    -1,
      -1,   110,    93,    -1,   845,    -1,   847,    -1,    -1,   438,
      -1,   852,  1047,  1048,   855,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   689,    -1,   866,   692,   868,   183,    -1,
     871,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     469,    -1,    -1,    -1,    -1,    -1,   475,    -1,    -1,   140,
     141,    -1,    -1,   144,   145,    -1,   147,   148,   487,   900,
     489,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
     911,    -1,   913,    -1,   183,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,     1,   281,    -1,   753,    -1,    -1,
      -1,   287,    -1,   759,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1259,    -1,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    38,    39,    -1,   553,    -1,   281,    44,   557,    -1,
      -1,    -1,   287,    -1,    51,    -1,   565,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     346,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      77,    -1,    79,    -1,    81,   361,    83,    84,    -1,    86,
    1011,    88,   281,    90,    -1,    -1,    -1,  1018,   287,   845,
      -1,   847,    -1,    -1,    -1,    -1,   852,    -1,    -1,   855,
    1031,   346,   109,    -1,    -1,   112,     3,    -1,   115,    -1,
     866,    -1,   868,    -1,    -1,   871,   361,    -1,  1049,     2,
      17,    -1,    -1,    20,    -1,    -1,    -1,  1058,    25,    12,
      -1,  1062,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,
      -1,    -1,    25,    -1,   900,    28,    -1,   346,    -1,    46,
      -1,    -1,   438,    -1,    -1,   911,    53,   913,   165,    -1,
      -1,    -1,   361,    -1,   171,    -1,    63,    -1,    -1,    -1,
     689,    -1,    -1,   692,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   469,    -1,  1116,    -1,    -1,    -1,   475,
      87,    -1,    -1,   438,    -1,    78,    -1,    -1,    -1,    -1,
      -1,   487,    99,   489,    -1,    -1,   103,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,   105,    -1,   469,    -1,    -1,   110,    -1,    -1,
     475,    -1,    -1,    -1,   753,    -1,    -1,    -1,    -1,   438,
     759,    -1,   487,    -1,   489,    -1,  1177,  1178,  1179,  1180,
      -1,    -1,    -1,    -1,    -1,  1011,    -1,    -1,    -1,    -1,
      -1,    -1,  1018,    -1,    -1,    -1,    -1,   553,     8,    -1,
     469,   557,    -1,  1204,  1205,  1031,   475,    -1,    -1,   565,
      -1,    -1,    22,    -1,    -1,    -1,    26,    27,   487,    -1,
     489,    -1,    -1,  1049,    -1,    -1,    36,    -1,    38,    39,
     183,    -1,  1058,    -1,    44,    -1,  1062,    -1,   553,    -1,
      -1,    51,   557,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     565,    -1,    -1,    -1,    -1,    -1,   845,    -1,   847,    -1,
      -1,    -1,    -1,   852,    -1,    -1,   855,    77,    -1,    79,
      -1,    -1,    -1,    83,    -1,    -1,    86,   866,    88,   868,
      90,     2,   871,    -1,   553,    -1,    -1,    -1,   557,    -1,
    1116,    12,    -1,    -1,    -1,    -1,   565,    -1,    -1,   109,
      -1,    -1,   112,    -1,    25,   115,    -1,    28,    -1,    -1,
      -1,   900,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   911,    -1,   913,    -1,    -1,    -1,   281,    -1,
      -1,    -1,    -1,   689,   287,    -1,   692,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      -1,  1177,  1178,  1179,  1180,   165,    -1,    78,    36,    -1,
      38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,
      -1,    -1,    -1,    51,   689,    -1,    -1,   692,  1204,  1205,
      -1,    -1,    -1,    -1,   105,    -1,    -1,    -1,    66,   110,
      -1,    -1,    -1,   346,    -1,    -1,    -1,   753,    -1,    77,
      -1,    79,    -1,   759,     3,    -1,    -1,    85,   361,    -1,
      88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    17,    18,
     689,    20,  1011,   692,    -1,    -1,    25,    -1,    -1,  1018,
      -1,   109,    -1,    -1,   112,    34,   114,   115,   753,    -1,
      -1,    40,  1031,    -1,   759,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
    1049,    -1,   183,    -1,    63,    -1,    -1,    -1,    -1,  1058,
      -1,    70,    71,  1062,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,   753,   438,    -1,    -1,    87,   845,
     759,   847,    -1,    -1,    -1,    -1,   852,    96,    97,   855,
      99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
     866,   110,   868,    -1,   113,   871,   469,   116,    -1,    -1,
      -1,    -1,   475,    -1,    -1,    -1,    -1,  1116,    -1,    -1,
     845,    -1,   847,    -1,   487,    -1,   489,   852,    -1,    -1,
     855,    -1,    -1,    -1,   900,    -1,    -1,    -1,    -1,    -1,
      -1,   866,    -1,   868,    -1,   911,   871,   913,    -1,    -1,
     281,    -1,    -1,    -1,    -1,    -1,   287,    -1,    -1,    -1,
      -1,   170,    -1,   172,    -1,    -1,   845,    -1,   847,    -1,
      -1,    -1,    -1,   852,    -1,   900,   855,    -1,  1177,  1178,
    1179,  1180,    -1,    -1,    -1,    -1,   911,   866,   913,   868,
     553,    -1,   871,    -1,   557,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   565,    -1,    -1,  1204,  1205,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   346,    -1,    -1,    -1,    -1,
      -1,   900,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     361,    -1,   911,    -1,   913,    -1,    -1,    -1,     2,    -1,
      -1,    -1,    -1,    -1,    -1,  1011,    -1,    -1,    12,    -1,
      -1,    -1,  1018,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    28,  1031,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,
      -1,    -1,    -1,  1049,    -1,    -1,  1011,    12,    -1,    -1,
      -1,     8,  1058,  1018,    -1,    -1,  1062,    -1,    -1,    -1,
      25,    -1,    -1,    28,    -1,    22,  1031,   438,    -1,    26,
      27,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    36,
      -1,    38,    39,    -1,  1049,    -1,   689,    44,    -1,   692,
      -1,    -1,  1011,  1058,    51,    -1,    -1,  1062,   469,  1018,
      -1,   105,    -1,    -1,   475,    -1,   110,    -1,    -1,    66,
    1116,    -1,  1031,    78,    -1,    -1,   487,    -1,   489,    -1,
      77,    -1,    79,    -1,    -1,    -1,    83,    -1,    85,    86,
    1049,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,  1058,
     105,    -1,    -1,  1062,    -1,   110,    -1,    -1,    -1,    -1,
     753,  1116,   109,    -1,    -1,   112,   759,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1177,  1178,  1179,  1180,    -1,    -1,    -1,    -1,   183,
      -1,    -1,   553,    -1,    -1,    -1,   557,    -1,    -1,    -1,
      -1,    -1,    -1,    72,   565,    -1,    -1,  1116,  1204,  1205,
      -1,    44,    45,    46,    -1,    -1,    -1,    -1,   165,    -1,
      -1,    -1,  1177,  1178,  1179,  1180,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1204,
    1205,    84,   845,    -1,   847,    -1,    -1,    -1,    -1,   852,
      -1,    -1,   855,    -1,    -1,    -1,    -1,    -1,  1177,  1178,
    1179,  1180,    22,   866,    -1,   868,    26,    27,   871,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    36,   281,    38,    39,
      -1,    -1,    -1,   287,    44,  1204,  1205,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,   900,    -1,    -1,
      -1,    -1,   145,    -1,   147,    -1,    66,    -1,   911,    -1,
     913,    -1,    -1,    -1,    -1,    -1,   281,    77,   689,    79,
      -1,   692,   287,    -1,    -1,    85,    -1,    -1,    88,    22,
      90,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   346,    36,    -1,    38,    39,    -1,    -1,   109,
     193,    44,   112,    -1,    -1,   115,    -1,   361,    51,    -1,
     239,   240,   241,   242,   243,   244,    -1,   210,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   346,   753,    -1,    77,    -1,    79,    -1,   759,    -1,
      83,   234,    85,    86,   237,    88,   361,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   248,   249,    -1,  1011,    -1,
      -1,   290,    -1,   292,    -1,  1018,   109,    -1,    -1,   112,
     299,    -1,   115,    -1,    -1,    22,    -1,    -1,  1031,    26,
      27,    -1,    -1,    -1,   438,    -1,    -1,    -1,   317,    36,
     283,    38,    39,    -1,    -1,    -1,  1049,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    51,  1058,   335,   336,    -1,  1062,
      -1,    -1,    -1,    -1,    -1,   469,    -1,    -1,    -1,    -1,
      -1,   475,   165,   438,   845,    -1,   847,    -1,    -1,    -1,
      77,   852,    79,   487,   855,   489,    83,    -1,    -1,    86,
      -1,    88,    -1,    90,    -1,   866,    -1,   868,    -1,    -1,
     871,    -1,    -1,    22,   469,    -1,    -1,    26,    27,    -1,
     475,    -1,   109,  1116,    -1,   112,    -1,    36,   115,    38,
      39,    -1,   487,   366,    -1,    44,    -1,    -1,    -1,   900,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     911,    -1,   913,    -1,    -1,    -1,    -1,    -1,    -1,   553,
      -1,    -1,    -1,   557,    -1,    -1,    -1,    -1,    77,    -1,
      79,   565,   441,    -1,    -1,    -1,    -1,    -1,   165,    88,
      -1,    90,    -1,    -1,  1177,  1178,  1179,  1180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   553,    -1,
     109,    -1,   557,   112,   437,   474,   115,    -1,    -1,   478,
     565,  1204,  1205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   456,    -1,     3,    -1,    -1,   461,    -1,
     463,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
    1011,    -1,    -1,    -1,    -1,    -1,    34,  1018,    -1,    -1,
      -1,    -1,    40,    -1,    -1,   498,    -1,    -1,    46,    -1,
    1031,   504,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   517,    63,    -1,    -1,  1049,    -1,
      -1,    -1,    70,    71,    -1,   689,    -1,  1058,   692,    -1,
      78,  1062,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,   566,   689,   113,    -1,   692,   116,    -1,
      -1,    -1,   575,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1116,    -1,    -1,    -1,   753,
      -1,    -1,    -1,    -1,    -1,   759,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,   753,    -1,
     669,   670,    -1,    -1,   759,    -1,   675,   676,    -1,   678,
     679,    -1,    -1,    -1,    -1,    -1,  1177,  1178,  1179,  1180,
      -1,    -1,    -1,   656,    -1,    -1,   659,   660,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1204,  1205,    -1,    -1,    -1,    -1,    -1,
      -1,   845,    -1,   847,    -1,    -1,    -1,    -1,   852,    -1,
      -1,   855,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   866,    -1,   868,    -1,    -1,   871,   711,    -1,
      -1,    -1,   715,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     845,    -1,   847,    -1,    -1,    -1,    -1,   852,   731,    -1,
     855,    -1,    -1,    -1,    -1,    -1,   900,    -1,   741,    -1,
      -1,   866,   745,   868,    -1,    -1,   871,   911,    -1,   913,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,   900,    36,   780,    38,    39,
      -1,    -1,    -1,    -1,    44,    -1,   911,   790,   913,   828,
     829,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   842,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   854,    -1,    77,   857,    79,
      -1,    81,    -1,    83,    -1,    -1,    86,    -1,    88,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1011,    -1,   109,
      -1,    -1,   112,    -1,  1018,   115,    -1,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    -1,  1031,    -1,    -1,
     873,    -1,   875,    32,    17,    18,   879,    20,    -1,   882,
      -1,    -1,    25,    -1,    -1,  1049,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    -1,  1058,    -1,    -1,    40,  1062,    -1,
      -1,    -1,   905,    46,    -1,   165,  1031,    50,    -1,    -1,
      53,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,  1049,    -1,   929,    70,    71,   932,
      89,    -1,    -1,  1058,    93,    78,    -1,  1062,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1116,    96,    97,    -1,    99,    -1,   101,    -1,
     103,    -1,    -1,   106,    -1,   124,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,  1116,   151,   152,   153,   154,    -1,    -1,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,    -1,  1051,  1177,  1178,  1179,  1180,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1204,  1205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,
       1,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,  1089,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    18,    19,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    -1,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,    -1,   170,   171,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
     171,   172,    -1,   174,    -1,     1,   177,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,
       1,   177,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
     171,   172,    -1,   174,    -1,     1,   177,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,
       1,   177,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
     171,   172,    -1,   174,    -1,     1,   177,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       1,   177,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      -1,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    -1,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       1,   177,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      -1,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    -1,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       1,   177,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      -1,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    -1,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       1,   177,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      -1,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    -1,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       1,   177,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      -1,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    -1,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,   175,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       3,   177,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,   107,
     108,    -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,   107,   108,    -1,   110,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
     173,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,   107,
     108,    -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,   173,   174,    -1,     3,   177,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    48,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     3,   177,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,   109,   110,    -1,
     112,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     3,   177,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,   107,   108,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,   107,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     3,   177,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    69,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     3,   177,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    79,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     3,   177,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,   109,   110,    -1,   112,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,
     177,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,   107,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,   174,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,   175,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,   175,     3,   177,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       3,   177,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
     175,     3,   177,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,
     177,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     3,   177,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     3,   177,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     3,   177,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     3,   177,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       3,   177,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     3,   177,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     3,   177,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,
     177,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     3,   177,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     3,   177,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     3,   177,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     3,   177,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,    -1,    -1,    -1,   140,   141,   142,   143,   144,    12,
      -1,    -1,    -1,   149,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    25,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    34,   168,    36,    -1,    -1,   172,    40,   174,    -1,
      -1,   177,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,    -1,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,   147,    -1,    -1,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
     163,    -1,    -1,   166,   167,   168,   169,    -1,     3,   172,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   169,   163,   164,
     172,    -1,   174,   168,    -1,   170,    -1,   172,    -1,   174,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    32,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    89,    82,
      -1,    -1,    93,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,   142,
      -1,   152,    -1,   146,    -1,    -1,    -1,   150,   159,   160,
      -1,    -1,   163,    -1,    -1,   166,   167,   168,    -1,    -1,
      -1,   164,    -1,    -1,    -1,    -1,   177,   170,    -1,   172,
     173,   174,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      61,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,   105,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,   118,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,    -1,
      -1,   142,    -1,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,   164,   165,    25,    -1,   169,    -1,   170,
     172,   172,   174,   174,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,   142,    -1,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,   164,    -1,    25,    -1,    -1,    -1,
     170,    -1,   172,    -1,   174,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,   164,    -1,    25,    -1,    -1,
      -1,   170,    -1,   172,    -1,   174,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
       3,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    12,
      -1,    -1,    -1,    -1,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    -1,   142,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,   172,    -1,   174,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    96,    97,    -1,    99,    -1,   101,    19,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    32,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,    -1,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,   147,    -1,    -1,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
     163,    -1,    -1,   166,   167,   168,   169,    -1,    12,    89,
      -1,    -1,    -1,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,    -1,   138,   139,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
     170,    19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,    -1,   138,   139,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    89,   166,   167,   168,    93,   170,    19,    -1,    -1,
      -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,
     102,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    89,   166,   167,   168,    93,   170,    19,
      -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,    -1,   138,   139,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,    89,
     166,   167,   168,    93,   170,    19,    -1,    -1,    -1,    -1,
      -1,   177,   102,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,    -1,   138,   139,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
     170,    19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,    -1,   138,   139,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    89,   166,   167,   168,    93,   170,    19,    -1,    -1,
      -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,    89,   166,   167,
     168,    93,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,   170,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    89,
      -1,    -1,    12,    93,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    48,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    89,
     170,    -1,    -1,    93,    -1,    -1,    -1,   177,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    -1,   166,   167,   168,    -1,
     170,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    73,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   124,   165,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,   177,
      93,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,   120,   166,   167,
     168,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    -1,   166,   167,   168,    -1,   170,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    12,    93,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    48,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,   177,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    -1,   166,   167,   168,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    12,    93,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   124,    -1,   166,   167,   168,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,   177,    -1,
      -1,   140,   141,   102,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   124,    -1,   166,   167,   168,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    12,    -1,   166,   167,   168,
      -1,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,
      32,    -1,    -1,   120,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   124,    -1,   166,
     167,   168,    -1,    -1,    -1,    -1,    -1,    89,   175,    -1,
     177,    93,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    -1,   166,
     167,   168,   124,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    32,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,   124,   170,    -1,    -1,    -1,    89,
      -1,    -1,   177,    93,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,   165,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,   165,   166,   167,   168,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    38,    39,    -1,    12,    -1,    -1,    44,    -1,    -1,
      -1,    19,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    89,    79,    -1,    81,    93,    83,    -1,    -1,    86,
      12,    88,    -1,    90,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,   109,    -1,    -1,   112,   124,    -1,   115,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   124,   165,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    89,   165,   177,
      -1,    93,   140,   141,   171,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,   165,   166,   167,
     168,   123,   124,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    12,    13,    -1,    -1,   102,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,    -1,   124,    -1,    -1,    -1,    -1,    89,
      -1,   177,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,   173,    -1,    -1,    -1,   177,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    32,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    -1,   166,   167,   168,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,   140,   141,    19,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,    32,
      -1,   157,   158,   159,   160,   161,   124,   163,    -1,    -1,
     166,   167,   168,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,   177,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,   120,   166,   167,
     168,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,   177,
      93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    -1,   165,   166,   167,   168,    12,    13,    -1,    -1,
      -1,    -1,    -1,    19,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    12,    93,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,    22,
     166,   167,   168,    26,    27,    19,    -1,   173,    31,    -1,
      -1,   177,    -1,    36,    -1,    38,    39,    -1,    32,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    32,
      83,    -1,    -1,    86,    -1,    88,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      12,    -1,    -1,    -1,    -1,    -1,   109,    19,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   165,   157,   158,   159,   160,   161,   162,   163,
     123,   124,   166,   167,   168,    -1,    -1,    89,    -1,   173,
      -1,    93,    -1,   177,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    -1,   124,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    22,    -1,    -1,    -1,
      26,    27,    32,    33,    -1,    31,    -1,    -1,    -1,    -1,
      36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    12,    -1,    -1,    51,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    83,    -1,    89,
      86,    -1,    88,    93,    90,    -1,    -1,    12,    13,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,   112,    32,    -1,   115,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,   165,
      -1,    -1,    -1,    -1,    89,   171,    -1,   177,    93,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    -1,   166,   167,   168,   124,
      -1,    19,    -1,    -1,    -1,   175,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    89,    -1,    -1,    -1,    93,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,    89,   166,   167,
     168,    93,    -1,    -1,    -1,    12,    13,    -1,    -1,   177,
     102,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    12,    89,    -1,    -1,   177,    93,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    32,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,    -1,
      19,    -1,   173,    -1,    -1,    -1,   177,   140,   141,    -1,
     143,   144,   145,    32,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,   157,   158,   159,   160,   161,   124,
     163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,
     175,    -1,   177,   102,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,   102,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     177,   102,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    32,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,   140,
     141,    -1,   143,   144,   145,    32,   147,   148,   149,    -1,
      -1,   152,   153,   154,    -1,    -1,   157,   158,   159,   160,
      -1,   124,   163,    -1,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    26,    27,    -1,   124,    -1,    31,
      -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,
      -1,    -1,    44,   140,   141,    -1,   143,   144,   145,    51,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,    -1,    19,    77,    -1,    79,   175,    81,
     177,    83,    -1,    -1,    86,    -1,    88,    32,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,    89,   166,   167,   168,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,   124,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,
      -1,    -1,    -1,    -1,    44,   140,   141,    -1,   143,   144,
     145,    51,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,   177,    83,    -1,    -1,    86,    -1,    88,    22,
      90,    -1,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,   109,
      -1,    44,   112,    -1,    -1,   115,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      83,    -1,    -1,    86,    -1,    88,    -1,    90,    22,    -1,
      -1,    -1,    26,    27,    -1,   165,    -1,    31,    -1,    -1,
      -1,   171,    36,    -1,    38,    39,   109,    -1,    -1,   112,
      44,    -1,   115,    -1,    -1,    -1,    -1,    51,     1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,    77,    -1,    79,    -1,    81,    31,    83,
      -1,    34,    86,    -1,    88,    -1,    90,    40,    -1,    -1,
      -1,    -1,   165,    46,    -1,    -1,    -1,    50,   171,    -1,
      53,    -1,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      63,   115,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,
     103,     3,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,   165,    -1,   116,    -1,    17,    18,   171,    20,    -1,
      -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,     3,    -1,   106,    -1,    -1,     8,   110,    -1,
      -1,   113,    -1,   115,   116,    -1,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,
     101,    -1,   103,    17,    18,   106,    20,    -1,    -1,   110,
      -1,    25,   113,    -1,    -1,   116,    -1,    31,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,
      17,    18,   106,    20,    -1,    -1,   110,    -1,    25,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    99,    -1,   101,    -1,   103,    17,    18,   106,
      20,    -1,    -1,   110,    -1,    25,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,
      -1,   101,    -1,   103,    17,    18,   106,    20,    -1,    -1,
     110,    -1,    25,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,
     103,    17,    18,   106,    20,    -1,    -1,   110,    -1,    25,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    99,    -1,   101,    -1,   103,    17,    18,
     106,    20,    -1,    -1,   110,    -1,    25,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      99,    -1,   101,    -1,   103,    17,    18,   106,    20,    -1,
      -1,   110,    -1,    25,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   182,   183,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    33,    34,
      36,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      49,    50,    51,    53,    56,    57,    58,    59,    60,    62,
      63,    64,    65,    67,    68,    70,    71,    72,    77,    78,
      79,    80,    81,    82,    83,    86,    87,    88,    90,    91,
      92,    94,    95,    96,    97,    98,    99,   100,   101,   103,
     104,   106,   107,   108,   109,   110,   112,   113,   115,   116,
     119,   121,   124,   140,   141,   142,   143,   144,   149,   159,
     162,   163,   164,   165,   168,   170,   172,   174,   177,   184,
     185,   186,   187,   188,   189,   190,   191,   194,   196,   202,
     203,   204,   207,   208,   212,   214,   217,   220,   222,   224,
     225,   226,   233,   234,   236,   238,   241,   242,   243,   244,
     245,   246,   250,   251,   254,   255,   258,   259,   263,   266,
     267,   293,   296,   297,   317,   318,   319,   320,   321,   322,
     323,   331,   332,   333,   334,   335,   338,   339,   340,   341,
     342,   343,   344,   345,   347,   348,   349,   350,   351,   165,
     186,   335,   120,   324,   325,     3,   209,    14,    22,    36,
      41,    42,    45,    56,    88,   101,   170,   174,   241,   263,
     317,   322,   333,   334,   335,   338,   340,   341,   324,   335,
     109,   296,    90,   209,   186,   311,   335,     8,    22,    36,
      39,    83,    86,    88,   188,   186,   172,     8,    88,   335,
     336,     8,    11,    88,   109,   112,   336,    79,   122,   235,
     335,   235,   335,   235,   335,    26,   297,   335,    27,   115,
     237,   335,   195,   172,     3,    17,    18,    20,    25,    34,
      40,    46,    50,    53,    63,    70,    71,    78,    87,    96,
      97,    99,   101,   103,   106,   110,   113,   116,   211,   213,
     211,   211,   264,   172,   211,   298,   299,    33,   196,   216,
     335,   218,   219,   335,   335,    18,    78,    96,   113,   335,
     335,   335,     8,   172,   223,   224,     4,   287,   310,   335,
     107,   108,   165,   335,   337,   335,   216,   335,   335,   335,
     100,   172,   186,   335,   335,   196,   208,   335,   338,   196,
     208,   335,   211,   294,   335,   237,   335,   335,   335,   335,
     335,   335,   335,     1,   171,   184,   197,   310,   111,   150,
     287,   312,   313,   337,   235,   310,   335,   346,   335,    81,
     186,   170,    85,   192,    47,   114,    56,   211,   211,    54,
      74,    84,   282,   298,   164,   165,   156,   335,    12,    19,
      32,    89,    93,   124,   140,   141,   143,   144,   145,   147,
     148,   149,   151,   152,   153,   154,   155,   157,   158,   159,
     160,   161,   162,   163,   166,   167,   168,   177,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   169,   273,   172,   174,    89,    93,   172,
     186,   165,   335,   335,   335,   211,   310,    56,   170,   196,
      48,   324,   294,   298,   165,   146,   165,   188,   119,   212,
     287,   314,   315,   316,   337,    88,   231,   267,   296,    88,
     112,   227,   294,   229,   267,   296,   211,   172,   216,    33,
      48,   216,   120,   216,   327,    33,    48,   216,   327,   216,
      48,   216,    37,    73,   165,   211,   211,   102,   196,   102,
     125,   196,   273,    83,    86,   193,   314,   172,   172,   196,
     186,   172,   275,   111,   172,   211,   300,   301,     1,   145,
     305,    48,   146,   186,   216,   146,   216,    13,   172,   172,
     216,   314,   224,   146,   165,   335,   335,   165,   170,   216,
     172,   314,   165,   247,   165,   247,   165,   125,   295,   165,
     216,   216,   165,   171,   171,   184,   146,   171,   335,   146,
     173,   146,   173,   175,   327,    48,   146,   175,   327,   123,
     146,   175,     8,     1,   171,   197,    66,   205,   206,   335,
     199,   335,   211,   145,   256,   170,   268,   165,   335,   335,
     335,   335,   235,   335,   235,   335,   335,   335,   335,   335,
     335,   335,     3,    20,    34,    63,   103,   109,   212,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,    69,   337,   337,   337,
     337,   337,   314,   314,   235,   335,   235,   335,    27,    48,
      90,   115,   326,   329,   330,   335,   351,    33,    48,    33,
      48,   102,   172,    48,   175,   211,   235,   335,   216,   165,
     165,   335,   335,   125,   173,   146,   232,   211,   298,   228,
     230,   211,   165,   211,   298,    48,   310,    45,   335,   235,
     335,   172,   216,    45,   335,   235,   335,   216,   235,   335,
     168,   198,   201,   335,   198,   200,   125,   125,   186,    35,
     186,   335,    35,   335,   192,   173,   315,   211,   239,   240,
      27,    48,    52,    76,    79,    90,   109,   185,   276,   277,
     278,   279,   280,   265,   301,   146,   173,    34,    50,    97,
     101,   174,   215,   306,   318,   125,   302,   335,   299,   218,
     211,   296,   335,   335,   173,   287,   335,     1,   252,   316,
     173,    21,   248,   306,   318,   146,   171,   173,   173,   312,
     173,   312,   186,   175,   235,   335,   175,   186,   335,   175,
     335,   175,   335,   171,   171,   211,   146,   165,    13,   148,
     146,   165,    13,    37,    73,   165,   172,   310,   170,     1,
      31,   211,   260,   261,   262,    27,    79,    90,   109,   270,
     281,   172,   165,   165,   165,   165,   165,   165,   173,   175,
      48,    90,   146,   173,    17,    20,    25,    46,    53,    63,
      71,    87,    99,   110,   116,   317,    89,    89,    45,   235,
     335,    45,   235,   335,   315,   235,   335,   172,   324,   324,
     165,   287,   337,   316,   211,   256,   165,   211,   211,   256,
     256,   165,   335,   173,   335,    33,   216,    33,   216,   328,
     329,   335,    33,   216,   327,    33,   216,   327,   216,   216,
     146,   165,    13,   165,   335,   335,    35,   186,    35,    35,
     186,   102,   196,    66,   173,   146,   173,    48,    90,   279,
     146,   173,   172,   211,    27,    79,    90,   109,   283,   173,
     301,   305,     1,   310,    69,   337,   211,   173,   173,   171,
      75,   117,   171,   253,   173,   172,   196,   211,   249,   294,
     186,   175,   327,   175,   327,   186,   123,   205,   212,   170,
     335,   111,   335,   201,   200,   315,     1,   257,   171,     8,
     262,   125,   146,   171,    90,   269,     1,     3,    12,    17,
      19,    20,    25,    40,    46,    53,    55,    63,    70,    71,
      87,    99,   103,   106,   110,   116,   140,   141,   142,   143,
     144,   145,   147,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   163,   166,   167,   168,   169,   172,
     210,   211,   213,   271,   272,   273,   317,   173,   329,   305,
     317,   317,   335,    33,    33,   335,    33,    33,   173,   175,
     175,   315,   216,   216,   256,   170,   256,   256,   170,   170,
     216,   102,    45,   335,    45,   335,   146,   173,   102,    45,
     335,   216,    45,   335,   216,   335,   335,   186,   186,   335,
     186,    35,   211,   165,   240,   196,   211,   278,   301,   145,
     309,    90,   305,   302,   175,    48,   175,   172,   172,    33,
     186,   310,   249,   145,   196,    45,   186,   335,   175,    45,
     186,   335,   175,   335,   198,    13,    37,    73,    37,    73,
     165,   165,   173,   171,    31,    83,    86,   171,   185,   221,
     222,   262,   335,   261,   283,   172,   274,   335,   140,   148,
     274,   274,   302,   102,    45,    45,   102,    45,    45,    45,
      45,   173,   170,   257,   170,   170,   257,   257,   335,   335,
     335,   329,   335,   335,   335,    13,    35,   186,   165,   309,
     173,   174,   215,   287,   308,   318,   150,   288,   302,    61,
     118,   289,   335,   306,   318,   314,   314,   186,   216,   173,
     335,   335,   186,   335,   186,   171,   111,   335,   201,   200,
     201,   200,   165,     8,   222,   221,     1,   145,   304,   277,
     173,     3,   103,   272,   273,   335,   335,   335,   335,   335,
     335,   257,   171,   257,   257,   171,   171,   102,   102,   102,
     102,   335,   186,   288,   302,   309,   175,   310,   287,   335,
       3,    92,   103,   290,   291,   292,   335,   196,   217,   286,
     175,   173,   173,   196,   102,   102,   165,   165,   165,   165,
     222,   174,   215,   303,   318,   105,   284,   173,   274,   274,
     102,   102,   102,   102,   102,   102,   171,   171,   171,   335,
     335,   335,   335,   288,   302,   287,   307,   308,   318,    48,
     175,   292,   118,   146,   125,   151,   153,   154,   157,   158,
      61,   318,   164,   164,   335,   335,     1,   175,   310,   289,
     335,   307,   308,   335,   291,   292,   292,   292,   292,   292,
     292,   290,   175,   303,   318,   175,   165,   285,   286,   175,
     303,   318,   307
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   181,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   187,   187,   187,   188,   188,   189,   190,
     190,   190,   191,   191,   191,   192,   192,   193,   193,   193,
     195,   194,   196,   196,   196,   197,   197,   198,   198,   198,
     198,   199,   199,   199,   199,   199,   199,   200,   200,   201,
     201,   202,   202,   202,   203,   203,   203,   203,   203,   203,
     203,   204,   205,   205,   205,   206,   206,   207,   208,   208,
     208,   208,   208,   208,   209,   209,   210,   210,   210,   210,
     210,   210,   211,   211,   211,   211,   211,   211,   212,   212,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   215,   215,   215,   215,   216,   216,   217,   217,   218,
     218,   218,   219,   219,   220,   221,   221,   221,   222,   222,
     222,   223,   222,   224,   224,   224,   224,   224,   224,   224,
     225,   225,   225,   225,   227,   226,   228,   226,   229,   226,
     230,   226,   231,   226,   232,   226,   226,   226,   226,   226,
     233,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   235,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   237,   237,   238,   238,   239,
     239,   240,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   242,   242,   242,   242,   242,
     242,   243,   243,   243,   244,   244,   244,   245,   246,   246,
     246,   246,   246,   246,   247,   247,   248,   248,   248,   249,
     249,   250,   251,   251,   252,   252,   253,   253,   253,   254,
     254,   255,   255,   255,   256,   256,   257,   257,   257,   258,
     258,   259,   260,   260,   260,   261,   261,   261,   262,   262,
     264,   265,   263,   266,   266,   266,   268,   269,   267,   270,
     270,   270,   270,   270,   271,   271,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   274,   274,   275,   276,
     276,   277,   277,   278,   278,   278,   278,   278,   278,   279,
     279,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     281,   281,   281,   281,   281,   281,   282,   282,   282,   283,
     283,   283,   283,   283,   283,   284,   284,   285,   285,   286,
     286,   287,   288,   288,   288,   289,   289,   289,   289,   289,
     290,   290,   291,   291,   291,   291,   291,   291,   291,   292,
     292,   293,   293,   293,   294,   294,   295,   295,   295,   296,
     296,   296,   296,   296,   297,   297,   298,   298,   299,   299,
     300,   300,   300,   301,   301,   301,   302,   302,   302,   303,
     303,   303,   303,   303,   303,   303,   304,   304,   304,   304,
     304,   305,   305,   305,   305,   305,   306,   306,   306,   306,
     307,   307,   307,   308,   308,   308,   308,   308,   309,   309,
     309,   309,   309,   310,   310,   310,   310,   311,   311,   312,
     312,   312,   313,   313,   314,   314,   315,   315,   316,   316,
     316,   316,   317,   317,   318,   318,   318,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   321,   322,   323,   323,
     323,   323,   323,   323,   323,   323,   324,   324,   325,   326,
     326,   327,   328,   328,   329,   329,   329,   330,   330,   330,
     330,   330,   330,   331,   331,   332,   332,   332,   332,   332,
     333,   333,   333,   333,   333,   334,   335,   335,   335,   335,
     335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
     335,   335,   335,   336,   336,   337,   337,   337,   338,   338,
     338,   338,   339,   339,   339,   339,   339,   340,   340,   340,
     341,   341,   341,   341,   341,   342,   342,   342,   342,   343,
     343,   344,   344,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   346,   346,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   348,   348,   348,   348,   348,   348,   348,   349,   349,
     349,   349,   350,   350,   350,   350,   351,   351,   351,   351,
     351,   351,   351
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     3,     2,
       2,     3,     2,     1,     3,     2,     1,     1,     4,     3,
       4,     4,     0,     1,     1,     0,     1,     0,     1,     1,
       0,     7,     2,     3,     3,     1,     2,     1,     3,     3,
       5,     1,     3,     3,     3,     5,     5,     0,     1,     1,
       1,     0,     1,     1,     4,     6,     8,     8,     6,     8,
       8,     4,     1,     3,     5,     1,     3,     3,     4,     4,
       4,     4,     4,     4,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     2,     3,     4,
       3,     1,     1,     3,     3,     1,     3,     2,     1,     1,
       2,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       3,     5,     5,     2,     0,     8,     0,     9,     0,     8,
       0,     9,     0,     8,     0,     9,     3,     3,     5,     5,
       2,     5,     3,     3,     6,     6,     4,     5,     5,     3,
       3,     6,     5,     6,     5,     6,     3,     4,     3,     4,
       5,     5,     3,     3,     6,     7,     6,     7,     4,     5,
       4,     5,     4,     4,     3,     6,     5,     4,     3,     6,
       5,     6,     5,     8,     7,     4,     4,     6,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     6,     4,     7,     5,     3,     6,     2,     3,     3,
       2,     2,     3,     3,     0,     2,     2,     3,     5,     1,
       3,     3,     5,     5,     0,     2,     3,     2,     3,     6,
       6,     1,     1,     1,     0,     2,     0,     2,     3,     5,
       5,     1,     1,     2,     3,     1,     3,     2,     1,     3,
       0,     0,     8,     0,     1,     1,     0,     0,    10,     3,
       3,     5,     5,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     3,     3,     1,
       3,     0,     1,     4,     5,     4,     5,     6,     6,     0,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       0,     1,     1,     2,     1,     1,     1,     1,     1,     0,
       1,     2,     1,     1,     1,     0,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     0,     2,     2,     4,     4,
       1,     3,     3,     3,     3,     3,     3,     3,     2,     1,
       1,     3,     4,     4,     2,     4,     0,     2,     2,     1,
       1,     1,     2,     1,     4,     3,     1,     3,     3,     5,
       1,     1,     3,     1,     2,     3,     0,     2,     2,     3,
       2,     4,     3,     3,     4,     3,     0,     2,     2,     2,
       1,     0,     2,     2,     2,     1,     4,     4,     6,     3,
       0,     1,     1,     3,     4,     3,     4,     6,     0,     2,
       2,     2,     2,     1,     1,     3,     3,     1,     3,     1,
       1,     1,     3,     3,     0,     1,     1,     3,     3,     3,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     2,     4,     4,     4,     5,     2,     2,     1,
       2,     1,     2,     1,     2,     1,     2,     1,     1,     6,
       6,     4,     9,     9,     7,     6,     6,     4,     9,     9,
       7,     4,     6,     6,     9,     9,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     0,     1,     4,     1,
       3,     4,     1,     3,     4,     3,     3,     1,     1,     2,
       1,     2,     1,     1,     3,     1,     2,     2,     2,     2,
       2,     8,     8,     9,     9,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     3,     3,     3,     2,
       2,     2,     1,     0,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     4,     4,     4,
       3,     3,     3,     3,     5,     3,     4,     3,     4,     1,
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
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
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
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

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
#if YYDEBUG

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
#  if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

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
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (context);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
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
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


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





#define yynerrs yyps->yynerrs
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
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
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

  yychar = YYEMPTY; /* Cause a token to be read.  */
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
  if (yychar == YYEMPTY)
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

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
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
  yychar = YYEMPTY;
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
#line 518 "chapel.ypp"
                                       { yyblock = (yyval.pblockstmt); }
#line 6604 "bison-chapel.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 523 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6610 "bison-chapel.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 524 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6616 "bison-chapel.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 531 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6622 "bison-chapel.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 536 "chapel.ypp"
                                       { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6628 "bison-chapel.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 537 "chapel.ypp"
                                       { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6634 "bison-chapel.cpp"
    break;

  case 27: /* stmt: stmt_level_expr TSEMI  */
#line 560 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6640 "bison-chapel.cpp"
    break;

  case 28: /* stmt: TATOMIC stmt  */
#line 561 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6646 "bison-chapel.cpp"
    break;

  case 29: /* stmt: TBEGIN opt_task_intent_ls stmt  */
#line 562 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6652 "bison-chapel.cpp"
    break;

  case 30: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 563 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6658 "bison-chapel.cpp"
    break;

  case 31: /* stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 564 "chapel.ypp"
                                         { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6664 "bison-chapel.cpp"
    break;

  case 32: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 565 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6670 "bison-chapel.cpp"
    break;

  case 33: /* stmt: TDELETE simple_expr_ls TSEMI  */
#line 566 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6676 "bison-chapel.cpp"
    break;

  case 34: /* stmt: TLABEL ident_def stmt  */
#line 567 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6682 "bison-chapel.cpp"
    break;

  case 35: /* stmt: TLOCAL expr do_stmt  */
#line 568 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6688 "bison-chapel.cpp"
    break;

  case 36: /* stmt: TLOCAL do_stmt  */
#line 569 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6694 "bison-chapel.cpp"
    break;

  case 37: /* stmt: TON expr do_stmt  */
#line 570 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6700 "bison-chapel.cpp"
    break;

  case 38: /* stmt: TSERIAL expr do_stmt  */
#line 571 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6706 "bison-chapel.cpp"
    break;

  case 39: /* stmt: TSERIAL do_stmt  */
#line 572 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6712 "bison-chapel.cpp"
    break;

  case 40: /* stmt: TSYNC stmt  */
#line 573 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6718 "bison-chapel.cpp"
    break;

  case 41: /* stmt: TYIELD expr TSEMI  */
#line 574 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6724 "bison-chapel.cpp"
    break;

  case 42: /* stmt: error TSEMI  */
#line 575 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6730 "bison-chapel.cpp"
    break;

  case 44: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL deprecated_decl_base  */
#line 581 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt), (yyvsp[-1].pch)); }
#line 6736 "bison-chapel.cpp"
    break;

  case 45: /* deprecated_decl_stmt: TDEPRECATED deprecated_decl_base  */
#line 583 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt)); }
#line 6742 "bison-chapel.cpp"
    break;

  case 48: /* module_decl_start: access_control opt_prototype TMODULE ident_def  */
#line 593 "chapel.ypp"
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[0].pch), currentModuleType, NULL, yyfilename, (yyvsp[-3].b), (yyvsp[-2].b), (yylsp[-3]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[0].pch);
    }
#line 6756 "bison-chapel.cpp"
    break;

  case 49: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 606 "chapel.ypp"
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6765 "bison-chapel.cpp"
    break;

  case 50: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 611 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6774 "bison-chapel.cpp"
    break;

  case 51: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 616 "chapel.ypp"
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6783 "bison-chapel.cpp"
    break;

  case 52: /* access_control: %empty  */
#line 623 "chapel.ypp"
        { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6789 "bison-chapel.cpp"
    break;

  case 53: /* access_control: TPUBLIC  */
#line 624 "chapel.ypp"
          { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6795 "bison-chapel.cpp"
    break;

  case 54: /* access_control: TPRIVATE  */
#line 625 "chapel.ypp"
           { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6801 "bison-chapel.cpp"
    break;

  case 55: /* opt_prototype: %empty  */
#line 629 "chapel.ypp"
             { (yyval.b) = false; }
#line 6807 "bison-chapel.cpp"
    break;

  case 56: /* opt_prototype: TPROTOTYPE  */
#line 630 "chapel.ypp"
             { (yyval.b) = true;  }
#line 6813 "bison-chapel.cpp"
    break;

  case 57: /* include_access_control: %empty  */
#line 634 "chapel.ypp"
           { (yyval.b) = false; }
#line 6819 "bison-chapel.cpp"
    break;

  case 58: /* include_access_control: TPUBLIC  */
#line 635 "chapel.ypp"
           { (yyval.b) = false; }
#line 6825 "bison-chapel.cpp"
    break;

  case 59: /* include_access_control: TPRIVATE  */
#line 636 "chapel.ypp"
           { (yyval.b) = true; }
#line 6831 "bison-chapel.cpp"
    break;

  case 60: /* $@1: %empty  */
#line 641 "chapel.ypp"
  {
    (yylsp[0]).comment = context->latestComment;
    context->latestComment = NULL;
  }
#line 6840 "bison-chapel.cpp"
    break;

  case 61: /* include_module_stmt: TINCLUDE $@1 include_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 646 "chapel.ypp"
 {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[-1].pch), (yyvsp[-4].b), (yyvsp[-3].b), (yylsp[-6]).comment);
 }
#line 6848 "bison-chapel.cpp"
    break;

  case 62: /* block_stmt: TLCBR TRCBR  */
#line 661 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); }
#line 6854 "bison-chapel.cpp"
    break;

  case 63: /* block_stmt: TLCBR stmt_ls TRCBR  */
#line 662 "chapel.ypp"
                                       { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6860 "bison-chapel.cpp"
    break;

  case 64: /* block_stmt: TLCBR error TRCBR  */
#line 663 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6866 "bison-chapel.cpp"
    break;

  case 65: /* stmt_ls: toplevel_stmt  */
#line 668 "chapel.ypp"
                                       { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6872 "bison-chapel.cpp"
    break;

  case 66: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 669 "chapel.ypp"
                                       { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6878 "bison-chapel.cpp"
    break;

  case 67: /* renames_ls: expr  */
#line 674 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6888 "bison-chapel.cpp"
    break;

  case 68: /* renames_ls: expr TAS expr  */
#line 679 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6898 "bison-chapel.cpp"
    break;

  case 69: /* renames_ls: renames_ls TCOMMA expr  */
#line 684 "chapel.ypp"
                                          { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6907 "bison-chapel.cpp"
    break;

  case 70: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 688 "chapel.ypp"
                                          { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6916 "bison-chapel.cpp"
    break;

  case 71: /* use_renames_ls: expr  */
#line 697 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6926 "bison-chapel.cpp"
    break;

  case 72: /* use_renames_ls: expr TAS expr  */
#line 702 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6936 "bison-chapel.cpp"
    break;

  case 73: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 707 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6946 "bison-chapel.cpp"
    break;

  case 74: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 712 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6955 "bison-chapel.cpp"
    break;

  case 75: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 716 "chapel.ypp"
                                       { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6964 "bison-chapel.cpp"
    break;

  case 76: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 720 "chapel.ypp"
                                             { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6973 "bison-chapel.cpp"
    break;

  case 77: /* opt_only_ls: %empty  */
#line 728 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6983 "bison-chapel.cpp"
    break;

  case 79: /* except_ls: TSTAR  */
#line 737 "chapel.ypp"
                                       { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6993 "bison-chapel.cpp"
    break;

  case 81: /* use_access_control: %empty  */
#line 746 "chapel.ypp"
           { (yyval.b) = true; }
#line 6999 "bison-chapel.cpp"
    break;

  case 82: /* use_access_control: TPUBLIC  */
#line 747 "chapel.ypp"
           { (yyval.b) = false; }
#line 7005 "bison-chapel.cpp"
    break;

  case 83: /* use_access_control: TPRIVATE  */
#line 748 "chapel.ypp"
           { (yyval.b) = true; }
#line 7011 "bison-chapel.cpp"
    break;

  case 84: /* use_stmt: use_access_control TUSE use_renames_ls TSEMI  */
#line 752 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 7017 "bison-chapel.cpp"
    break;

  case 85: /* use_stmt: use_access_control TUSE expr TEXCEPT except_ls TSEMI  */
#line 753 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 7023 "bison-chapel.cpp"
    break;

  case 86: /* use_stmt: use_access_control TUSE expr TAS expr TEXCEPT except_ls TSEMI  */
#line 754 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7029 "bison-chapel.cpp"
    break;

  case 87: /* use_stmt: use_access_control TUSE expr TAS TUNDERSCORE TEXCEPT except_ls TSEMI  */
#line 755 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 7035 "bison-chapel.cpp"
    break;

  case 88: /* use_stmt: use_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 756 "chapel.ypp"
                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 7041 "bison-chapel.cpp"
    break;

  case 89: /* use_stmt: use_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 757 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7047 "bison-chapel.cpp"
    break;

  case 90: /* use_stmt: use_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 758 "chapel.ypp"
                                                                          { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 7053 "bison-chapel.cpp"
    break;

  case 91: /* import_stmt: use_access_control TIMPORT import_ls TSEMI  */
#line 762 "chapel.ypp"
                                             { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[-3].b));}
#line 7060 "bison-chapel.cpp"
    break;

  case 92: /* import_expr: expr  */
#line 767 "chapel.ypp"
       { (yyval.pimportstmt) = buildImportStmt((yyvsp[0].pexpr)); }
#line 7066 "bison-chapel.cpp"
    break;

  case 93: /* import_expr: expr TAS ident_use  */
#line 768 "chapel.ypp"
                     { (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7072 "bison-chapel.cpp"
    break;

  case 94: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 769 "chapel.ypp"
                                   { (yyval.pimportstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist)); }
#line 7078 "bison-chapel.cpp"
    break;

  case 95: /* import_ls: import_expr  */
#line 773 "chapel.ypp"
              { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pimportstmt)); }
#line 7084 "bison-chapel.cpp"
    break;

  case 96: /* import_ls: import_ls TCOMMA import_expr  */
#line 774 "chapel.ypp"
                               { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pimportstmt)); }
#line 7090 "bison-chapel.cpp"
    break;

  case 97: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 778 "chapel.ypp"
                                       { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 7096 "bison-chapel.cpp"
    break;

  case 98: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 783 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 7102 "bison-chapel.cpp"
    break;

  case 99: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 785 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 7108 "bison-chapel.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 787 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 7114 "bison-chapel.cpp"
    break;

  case 101: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 789 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 7120 "bison-chapel.cpp"
    break;

  case 102: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 791 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 7126 "bison-chapel.cpp"
    break;

  case 103: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 793 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), new SymExpr(gNoInit), "="); }
#line 7132 "bison-chapel.cpp"
    break;

  case 104: /* opt_label_ident: %empty  */
#line 799 "chapel.ypp"
         { (yyval.pch) = NULL; }
#line 7138 "bison-chapel.cpp"
    break;

  case 105: /* opt_label_ident: TIDENT  */
#line 800 "chapel.ypp"
         { (yyval.pch) = (yyvsp[0].pch); }
#line 7144 "bison-chapel.cpp"
    break;

  case 106: /* ident_fn_def: TIDENT  */
#line 804 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7150 "bison-chapel.cpp"
    break;

  case 107: /* ident_fn_def: TNONE  */
#line 805 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7156 "bison-chapel.cpp"
    break;

  case 108: /* ident_fn_def: TTHIS  */
#line 806 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7162 "bison-chapel.cpp"
    break;

  case 109: /* ident_fn_def: TFALSE  */
#line 807 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7168 "bison-chapel.cpp"
    break;

  case 110: /* ident_fn_def: TTRUE  */
#line 808 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7174 "bison-chapel.cpp"
    break;

  case 111: /* ident_fn_def: internal_type_ident_def  */
#line 809 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7180 "bison-chapel.cpp"
    break;

  case 112: /* ident_def: TIDENT  */
#line 812 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7186 "bison-chapel.cpp"
    break;

  case 113: /* ident_def: TNONE  */
#line 813 "chapel.ypp"
                           { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 7192 "bison-chapel.cpp"
    break;

  case 114: /* ident_def: TTHIS  */
#line 814 "chapel.ypp"
                           { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 7198 "bison-chapel.cpp"
    break;

  case 115: /* ident_def: TFALSE  */
#line 815 "chapel.ypp"
                           { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 7204 "bison-chapel.cpp"
    break;

  case 116: /* ident_def: TTRUE  */
#line 816 "chapel.ypp"
                           { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 7210 "bison-chapel.cpp"
    break;

  case 117: /* ident_def: internal_type_ident_def  */
#line 817 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 7216 "bison-chapel.cpp"
    break;

  case 118: /* ident_use: TIDENT  */
#line 829 "chapel.ypp"
                           { (yyval.pch) = (yyvsp[0].pch); }
#line 7222 "bison-chapel.cpp"
    break;

  case 119: /* ident_use: TTHIS  */
#line 830 "chapel.ypp"
                           { (yyval.pch) = "this"; }
#line 7228 "bison-chapel.cpp"
    break;

  case 120: /* internal_type_ident_def: TBOOL  */
#line 841 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 7234 "bison-chapel.cpp"
    break;

  case 121: /* internal_type_ident_def: TINT  */
#line 842 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 7240 "bison-chapel.cpp"
    break;

  case 122: /* internal_type_ident_def: TUINT  */
#line 843 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 7246 "bison-chapel.cpp"
    break;

  case 123: /* internal_type_ident_def: TREAL  */
#line 844 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 7252 "bison-chapel.cpp"
    break;

  case 124: /* internal_type_ident_def: TIMAG  */
#line 845 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 7258 "bison-chapel.cpp"
    break;

  case 125: /* internal_type_ident_def: TCOMPLEX  */
#line 846 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 7264 "bison-chapel.cpp"
    break;

  case 126: /* internal_type_ident_def: TBYTES  */
#line 847 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 7270 "bison-chapel.cpp"
    break;

  case 127: /* internal_type_ident_def: TSTRING  */
#line 848 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 7276 "bison-chapel.cpp"
    break;

  case 128: /* internal_type_ident_def: TSYNC  */
#line 849 "chapel.ypp"
             { (yyval.pch) = "sync"; }
#line 7282 "bison-chapel.cpp"
    break;

  case 129: /* internal_type_ident_def: TSINGLE  */
#line 850 "chapel.ypp"
             { (yyval.pch) = "single"; }
#line 7288 "bison-chapel.cpp"
    break;

  case 130: /* internal_type_ident_def: TOWNED  */
#line 851 "chapel.ypp"
             { (yyval.pch) = "owned"; }
#line 7294 "bison-chapel.cpp"
    break;

  case 131: /* internal_type_ident_def: TSHARED  */
#line 852 "chapel.ypp"
             { (yyval.pch) = "shared"; }
#line 7300 "bison-chapel.cpp"
    break;

  case 132: /* internal_type_ident_def: TBORROWED  */
#line 853 "chapel.ypp"
             { (yyval.pch) = "borrowed"; }
#line 7306 "bison-chapel.cpp"
    break;

  case 133: /* internal_type_ident_def: TUNMANAGED  */
#line 854 "chapel.ypp"
             { (yyval.pch) = "unmanaged"; }
#line 7312 "bison-chapel.cpp"
    break;

  case 134: /* internal_type_ident_def: TDOMAIN  */
#line 855 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 7318 "bison-chapel.cpp"
    break;

  case 135: /* internal_type_ident_def: TINDEX  */
#line 856 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 7324 "bison-chapel.cpp"
    break;

  case 136: /* internal_type_ident_def: TLOCALE  */
#line 857 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 7330 "bison-chapel.cpp"
    break;

  case 137: /* internal_type_ident_def: TNOTHING  */
#line 858 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 7336 "bison-chapel.cpp"
    break;

  case 138: /* internal_type_ident_def: TVOID  */
#line 859 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 7342 "bison-chapel.cpp"
    break;

  case 139: /* scalar_type: TBOOL  */
#line 863 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 7348 "bison-chapel.cpp"
    break;

  case 140: /* scalar_type: TENUM  */
#line 864 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 7354 "bison-chapel.cpp"
    break;

  case 141: /* scalar_type: TINT  */
#line 865 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 7360 "bison-chapel.cpp"
    break;

  case 142: /* scalar_type: TUINT  */
#line 866 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 7366 "bison-chapel.cpp"
    break;

  case 143: /* scalar_type: TREAL  */
#line 867 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7372 "bison-chapel.cpp"
    break;

  case 144: /* scalar_type: TIMAG  */
#line 868 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 7378 "bison-chapel.cpp"
    break;

  case 145: /* scalar_type: TCOMPLEX  */
#line 869 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 7384 "bison-chapel.cpp"
    break;

  case 146: /* scalar_type: TBYTES  */
#line 870 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 7390 "bison-chapel.cpp"
    break;

  case 147: /* scalar_type: TSTRING  */
#line 871 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 7396 "bison-chapel.cpp"
    break;

  case 148: /* scalar_type: TLOCALE  */
#line 872 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 7402 "bison-chapel.cpp"
    break;

  case 149: /* scalar_type: TNOTHING  */
#line 873 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 7408 "bison-chapel.cpp"
    break;

  case 150: /* scalar_type: TVOID  */
#line 874 "chapel.ypp"
           { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 7414 "bison-chapel.cpp"
    break;

  case 151: /* reserved_type_ident_use: TSYNC  */
#line 881 "chapel.ypp"
             { (yyval.pch) = "_syncvar"; }
#line 7420 "bison-chapel.cpp"
    break;

  case 152: /* reserved_type_ident_use: TSINGLE  */
#line 882 "chapel.ypp"
             { (yyval.pch) = "_singlevar"; }
#line 7426 "bison-chapel.cpp"
    break;

  case 153: /* reserved_type_ident_use: TDOMAIN  */
#line 883 "chapel.ypp"
             { (yyval.pch) = "_domain"; }
#line 7432 "bison-chapel.cpp"
    break;

  case 154: /* reserved_type_ident_use: TINDEX  */
#line 884 "chapel.ypp"
             { (yyval.pch) = "_index"; }
#line 7438 "bison-chapel.cpp"
    break;

  case 155: /* do_stmt: TDO stmt  */
#line 888 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7444 "bison-chapel.cpp"
    break;

  case 156: /* do_stmt: block_stmt  */
#line 889 "chapel.ypp"
              { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7450 "bison-chapel.cpp"
    break;

  case 157: /* return_stmt: TRETURN TSEMI  */
#line 893 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 7456 "bison-chapel.cpp"
    break;

  case 158: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 894 "chapel.ypp"
                              { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 7462 "bison-chapel.cpp"
    break;

  case 159: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 898 "chapel.ypp"
                                    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[-3].pexpr), (yyvsp[-1].pflagset), (yyvsp[0].pch)); }
#line 7468 "bison-chapel.cpp"
    break;

  case 160: /* manager_expr: expr TAS ident_def  */
#line 899 "chapel.ypp"
                                    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[-2].pexpr), NULL, (yyvsp[0].pch)); }
#line 7474 "bison-chapel.cpp"
    break;

  case 161: /* manager_expr: expr  */
#line 900 "chapel.ypp"
                                    { (yyval.pblockstmt) = buildManagerBlock((yyvsp[0].pexpr), NULL, NULL); }
#line 7480 "bison-chapel.cpp"
    break;

  case 162: /* manager_expr_ls: manager_expr  */
#line 904 "chapel.ypp"
                                        { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7486 "bison-chapel.cpp"
    break;

  case 163: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 905 "chapel.ypp"
                                        { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7492 "bison-chapel.cpp"
    break;

  case 164: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 909 "chapel.ypp"
                                  { (yyval.pblockstmt) = buildManageStmt((yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7498 "bison-chapel.cpp"
    break;

  case 166: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL class_level_stmt  */
#line 915 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt), (yyvsp[-1].pch)); }
#line 7504 "bison-chapel.cpp"
    break;

  case 167: /* deprecated_class_level_stmt: TDEPRECATED class_level_stmt  */
#line 917 "chapel.ypp"
{ (yyval.pblockstmt) = buildDeprecated((yyvsp[0].pblockstmt)); }
#line 7510 "bison-chapel.cpp"
    break;

  case 168: /* class_level_stmt: TSEMI  */
#line 921 "chapel.ypp"
                        { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 7516 "bison-chapel.cpp"
    break;

  case 170: /* class_level_stmt: TPUBLIC private_decl  */
#line 923 "chapel.ypp"
                        { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7522 "bison-chapel.cpp"
    break;

  case 171: /* @2: %empty  */
#line 933 "chapel.ypp"
           { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 7528 "bison-chapel.cpp"
    break;

  case 172: /* class_level_stmt: TPRIVATE @2 private_decl  */
#line 934 "chapel.ypp"
                 { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7534 "bison-chapel.cpp"
    break;

  case 180: /* forwarding_stmt: TFORWARDING expr TSEMI  */
#line 948 "chapel.ypp"
                          { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 7540 "bison-chapel.cpp"
    break;

  case 181: /* forwarding_stmt: TFORWARDING expr TEXCEPT except_ls TSEMI  */
#line 949 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 7546 "bison-chapel.cpp"
    break;

  case 182: /* forwarding_stmt: TFORWARDING expr TONLY opt_only_ls TSEMI  */
#line 950 "chapel.ypp"
                                            { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 7552 "bison-chapel.cpp"
    break;

  case 183: /* forwarding_stmt: TFORWARDING var_decl_stmt  */
#line 951 "chapel.ypp"
                            { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 7558 "bison-chapel.cpp"
    break;

  case 184: /* $@3: %empty  */
#line 956 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7567 "bison-chapel.cpp"
    break;

  case 185: /* extern_export_decl_stmt: TEXTERN TRECORD $@3 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 961 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7581 "bison-chapel.cpp"
    break;

  case 186: /* $@4: %empty  */
#line 972 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7590 "bison-chapel.cpp"
    break;

  case 187: /* extern_export_decl_stmt: TEXTERN STRINGLITERAL TRECORD $@4 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 977 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7605 "bison-chapel.cpp"
    break;

  case 188: /* $@5: %empty  */
#line 989 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7614 "bison-chapel.cpp"
    break;

  case 189: /* extern_export_decl_stmt: TEXTERN TUNION $@5 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 994 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_UNION,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7628 "bison-chapel.cpp"
    break;

  case 190: /* $@6: %empty  */
#line 1005 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7637 "bison-chapel.cpp"
    break;

  case 191: /* extern_export_decl_stmt: TEXTERN STRINGLITERAL TUNION $@6 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1010 "chapel.ypp"
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_UNION,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7652 "bison-chapel.cpp"
    break;

  case 192: /* $@7: %empty  */
#line 1022 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7661 "bison-chapel.cpp"
    break;

  case 193: /* extern_export_decl_stmt: TEXPORT TRECORD $@7 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1027 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7675 "bison-chapel.cpp"
    break;

  case 194: /* $@8: %empty  */
#line 1037 "chapel.ypp"
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7684 "bison-chapel.cpp"
    break;

  case 195: /* extern_export_decl_stmt: TEXPORT STRINGLITERAL TRECORD $@8 ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1042 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7698 "bison-chapel.cpp"
    break;

  case 196: /* extern_export_decl_stmt: TEXTERN opt_expr fn_decl_stmt  */
#line 1053 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7706 "bison-chapel.cpp"
    break;

  case 197: /* extern_export_decl_stmt: TEXPORT opt_expr fn_decl_stmt  */
#line 1057 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7714 "bison-chapel.cpp"
    break;

  case 198: /* extern_export_decl_stmt: TEXTERN opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1062 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7726 "bison-chapel.cpp"
    break;

  case 199: /* extern_export_decl_stmt: TEXPORT opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1070 "chapel.ypp"
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7738 "bison-chapel.cpp"
    break;

  case 200: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1081 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 7746 "bison-chapel.cpp"
    break;

  case 201: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1087 "chapel.ypp"
                                                { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 7752 "bison-chapel.cpp"
    break;

  case 202: /* loop_stmt: TWHILE expr do_stmt  */
#line 1088 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7758 "bison-chapel.cpp"
    break;

  case 203: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1089 "chapel.ypp"
                                                { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7764 "bison-chapel.cpp"
    break;

  case 204: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1090 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 7770 "bison-chapel.cpp"
    break;

  case 205: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1091 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 7776 "bison-chapel.cpp"
    break;

  case 206: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1092 "chapel.ypp"
                                                                  { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 7782 "bison-chapel.cpp"
    break;

  case 207: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1093 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7788 "bison-chapel.cpp"
    break;

  case 208: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1094 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7794 "bison-chapel.cpp"
    break;

  case 209: /* loop_stmt: TFOR expr do_stmt  */
#line 1095 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7800 "bison-chapel.cpp"
    break;

  case 210: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1096 "chapel.ypp"
                                                { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7806 "bison-chapel.cpp"
    break;

  case 211: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1097 "chapel.ypp"
                                                { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7812 "bison-chapel.cpp"
    break;

  case 212: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1098 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7818 "bison-chapel.cpp"
    break;

  case 213: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1099 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7824 "bison-chapel.cpp"
    break;

  case 214: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1100 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7830 "bison-chapel.cpp"
    break;

  case 215: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1101 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7836 "bison-chapel.cpp"
    break;

  case 216: /* loop_stmt: TFORALL expr do_stmt  */
#line 1102 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7842 "bison-chapel.cpp"
    break;

  case 217: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1103 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7848 "bison-chapel.cpp"
    break;

  case 218: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1104 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7854 "bison-chapel.cpp"
    break;

  case 219: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1105 "chapel.ypp"
                                                                   { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7860 "bison-chapel.cpp"
    break;

  case 220: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1106 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7866 "bison-chapel.cpp"
    break;

  case 221: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1107 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7872 "bison-chapel.cpp"
    break;

  case 222: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1108 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7878 "bison-chapel.cpp"
    break;

  case 223: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1109 "chapel.ypp"
                                                    { (yyval.pblockstmt) = ForLoop::buildForeachLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7884 "bison-chapel.cpp"
    break;

  case 224: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1111 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7894 "bison-chapel.cpp"
    break;

  case 225: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1117 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7904 "bison-chapel.cpp"
    break;

  case 226: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1123 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7914 "bison-chapel.cpp"
    break;

  case 227: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1129 "chapel.ypp"
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7924 "bison-chapel.cpp"
    break;

  case 228: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1135 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7935 "bison-chapel.cpp"
    break;

  case 229: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1142 "chapel.ypp"
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7946 "bison-chapel.cpp"
    break;

  case 230: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1149 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7954 "bison-chapel.cpp"
    break;

  case 231: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1153 "chapel.ypp"
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7962 "bison-chapel.cpp"
    break;

  case 232: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1159 "chapel.ypp"
                            { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 7968 "bison-chapel.cpp"
    break;

  case 233: /* if_stmt: TIF expr TTHEN stmt  */
#line 1163 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7974 "bison-chapel.cpp"
    break;

  case 234: /* if_stmt: TIF expr block_stmt  */
#line 1164 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7980 "bison-chapel.cpp"
    break;

  case 235: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1165 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7986 "bison-chapel.cpp"
    break;

  case 236: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1166 "chapel.ypp"
                                     { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7992 "bison-chapel.cpp"
    break;

  case 237: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1168 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7998 "bison-chapel.cpp"
    break;

  case 238: /* if_stmt: TIF ifvar block_stmt  */
#line 1169 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 8004 "bison-chapel.cpp"
    break;

  case 239: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1170 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8010 "bison-chapel.cpp"
    break;

  case 240: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1171 "chapel.ypp"
                                      { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8016 "bison-chapel.cpp"
    break;

  case 241: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1173 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8023 "bison-chapel.cpp"
    break;

  case 242: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1175 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8030 "bison-chapel.cpp"
    break;

  case 243: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1177 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8037 "bison-chapel.cpp"
    break;

  case 244: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1179 "chapel.ypp"
                                                         {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8044 "bison-chapel.cpp"
    break;

  case 245: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1184 "chapel.ypp"
                                { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), false); }
#line 8050 "bison-chapel.cpp"
    break;

  case 246: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1185 "chapel.ypp"
                                { (yyval.pexpr) = buildIfVar((yyvsp[-2].pch), (yyvsp[0].pexpr), true);  }
#line 8056 "bison-chapel.cpp"
    break;

  case 247: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1190 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), (yyvsp[0].pblockstmt))); }
#line 8062 "bison-chapel.cpp"
    break;

  case 248: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1192 "chapel.ypp"
  { // mimick ifc_formal_ls for a single formal "Self"
    DefExpr*  formal = InterfaceSymbol::buildFormal("Self", INTENT_TYPE);
    CallExpr* ls     = new CallExpr(PRIM_ACTUALS_LIST, formal);
    (yyval.pblockstmt) = buildChapelStmt(InterfaceSymbol::buildDef((yyvsp[-1].pch), ls, (yyvsp[0].pblockstmt))); }
#line 8071 "bison-chapel.cpp"
    break;

  case 249: /* ifc_formal_ls: ifc_formal  */
#line 1199 "chapel.ypp"
                                  { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 8077 "bison-chapel.cpp"
    break;

  case 250: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1200 "chapel.ypp"
                                  { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 8083 "bison-chapel.cpp"
    break;

  case 251: /* ifc_formal: ident_def  */
#line 1205 "chapel.ypp"
             { (yyval.pdefexpr) = InterfaceSymbol::buildFormal((yyvsp[0].pch), INTENT_TYPE); }
#line 8089 "bison-chapel.cpp"
    break;

  case 252: /* implements_type_ident: TIDENT  */
#line 1210 "chapel.ypp"
             { (yyval.pch) = (yyvsp[0].pch); }
#line 8095 "bison-chapel.cpp"
    break;

  case 253: /* implements_type_ident: TBOOL  */
#line 1211 "chapel.ypp"
             { (yyval.pch) = "bool"; }
#line 8101 "bison-chapel.cpp"
    break;

  case 254: /* implements_type_ident: TINT  */
#line 1212 "chapel.ypp"
             { (yyval.pch) = "int"; }
#line 8107 "bison-chapel.cpp"
    break;

  case 255: /* implements_type_ident: TUINT  */
#line 1213 "chapel.ypp"
             { (yyval.pch) = "uint"; }
#line 8113 "bison-chapel.cpp"
    break;

  case 256: /* implements_type_ident: TREAL  */
#line 1214 "chapel.ypp"
             { (yyval.pch) = "real"; }
#line 8119 "bison-chapel.cpp"
    break;

  case 257: /* implements_type_ident: TIMAG  */
#line 1215 "chapel.ypp"
             { (yyval.pch) = "imag"; }
#line 8125 "bison-chapel.cpp"
    break;

  case 258: /* implements_type_ident: TCOMPLEX  */
#line 1216 "chapel.ypp"
             { (yyval.pch) = "complex"; }
#line 8131 "bison-chapel.cpp"
    break;

  case 259: /* implements_type_ident: TBYTES  */
#line 1217 "chapel.ypp"
             { (yyval.pch) = "bytes"; }
#line 8137 "bison-chapel.cpp"
    break;

  case 260: /* implements_type_ident: TSTRING  */
#line 1218 "chapel.ypp"
             { (yyval.pch) = "string"; }
#line 8143 "bison-chapel.cpp"
    break;

  case 261: /* implements_type_ident: TLOCALE  */
#line 1219 "chapel.ypp"
             { (yyval.pch) = "locale"; }
#line 8149 "bison-chapel.cpp"
    break;

  case 262: /* implements_type_ident: TNOTHING  */
#line 1220 "chapel.ypp"
             { (yyval.pch) = "nothing"; }
#line 8155 "bison-chapel.cpp"
    break;

  case 263: /* implements_type_ident: TVOID  */
#line 1221 "chapel.ypp"
             { (yyval.pch) = "void"; }
#line 8161 "bison-chapel.cpp"
    break;

  case 264: /* implements_type_ident: implements_type_error_ident  */
#line 1223 "chapel.ypp"
  { (yyval.pch) = (yyvsp[0].pch);
    USR_FATAL_CONT("'%s' is not allowed to \"implement\" an interface", (yyvsp[0].pch)); }
#line 8168 "bison-chapel.cpp"
    break;

  case 265: /* implements_type_error_ident: TNONE  */
#line 1229 "chapel.ypp"
             { (yyval.pch) = "none"; }
#line 8174 "bison-chapel.cpp"
    break;

  case 266: /* implements_type_error_ident: TTHIS  */
#line 1230 "chapel.ypp"
             { (yyval.pch) = "this"; }
#line 8180 "bison-chapel.cpp"
    break;

  case 267: /* implements_type_error_ident: TFALSE  */
#line 1231 "chapel.ypp"
             { (yyval.pch) = "false"; }
#line 8186 "bison-chapel.cpp"
    break;

  case 268: /* implements_type_error_ident: TTRUE  */
#line 1232 "chapel.ypp"
             { (yyval.pch) = "true"; }
#line 8192 "bison-chapel.cpp"
    break;

  case 269: /* implements_type_error_ident: TDOMAIN  */
#line 1241 "chapel.ypp"
             { (yyval.pch) = "domain"; }
#line 8198 "bison-chapel.cpp"
    break;

  case 270: /* implements_type_error_ident: TINDEX  */
#line 1242 "chapel.ypp"
             { (yyval.pch) = "index"; }
#line 8204 "bison-chapel.cpp"
    break;

  case 271: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1247 "chapel.ypp"
  { (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8210 "bison-chapel.cpp"
    break;

  case 272: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1249 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-3].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-1].pch), act, NULL)); }
#line 8217 "bison-chapel.cpp"
    break;

  case 273: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1252 "chapel.ypp"
  { (yyvsp[-2].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-6].pch)));
    (yyval.pblockstmt) = buildChapelStmt(ImplementsStmt::build((yyvsp[-4].pch), (yyvsp[-2].pcallexpr), NULL)); }
#line 8224 "bison-chapel.cpp"
    break;

  case 274: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1258 "chapel.ypp"
  { (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8230 "bison-chapel.cpp"
    break;

  case 275: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1260 "chapel.ypp"
  { CallExpr* act = new CallExpr(PRIM_ACTUALS_LIST, new UnresolvedSymExpr((yyvsp[-2].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[0].pch), act); }
#line 8237 "bison-chapel.cpp"
    break;

  case 276: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1263 "chapel.ypp"
  { (yyvsp[-1].pcallexpr)->insertAtHead(new UnresolvedSymExpr((yyvsp[-5].pch)));
    (yyval.pexpr) = IfcConstraint::build((yyvsp[-3].pch), (yyvsp[-1].pcallexpr)); }
#line 8244 "bison-chapel.cpp"
    break;

  case 277: /* defer_stmt: TDEFER stmt  */
#line 1268 "chapel.ypp"
                          { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 8250 "bison-chapel.cpp"
    break;

  case 278: /* try_stmt: TTRY expr TSEMI  */
#line 1271 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 8256 "bison-chapel.cpp"
    break;

  case 279: /* try_stmt: TTRYBANG expr TSEMI  */
#line 1272 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 8262 "bison-chapel.cpp"
    break;

  case 280: /* try_stmt: TTRY assignment_stmt  */
#line 1273 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 8268 "bison-chapel.cpp"
    break;

  case 281: /* try_stmt: TTRYBANG assignment_stmt  */
#line 1274 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 8274 "bison-chapel.cpp"
    break;

  case 282: /* try_stmt: TTRY block_stmt catch_stmt_ls  */
#line 1275 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8280 "bison-chapel.cpp"
    break;

  case 283: /* try_stmt: TTRYBANG block_stmt catch_stmt_ls  */
#line 1276 "chapel.ypp"
                                         { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 8286 "bison-chapel.cpp"
    break;

  case 284: /* catch_stmt_ls: %empty  */
#line 1280 "chapel.ypp"
                           { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8292 "bison-chapel.cpp"
    break;

  case 285: /* catch_stmt_ls: catch_stmt_ls catch_stmt  */
#line 1281 "chapel.ypp"
                           { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8298 "bison-chapel.cpp"
    break;

  case 286: /* catch_stmt: TCATCH block_stmt  */
#line 1285 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 8304 "bison-chapel.cpp"
    break;

  case 287: /* catch_stmt: TCATCH catch_expr block_stmt  */
#line 1286 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8310 "bison-chapel.cpp"
    break;

  case 288: /* catch_stmt: TCATCH TLP catch_expr TRP block_stmt  */
#line 1287 "chapel.ypp"
                                       { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 8316 "bison-chapel.cpp"
    break;

  case 289: /* catch_expr: ident_def  */
#line 1291 "chapel.ypp"
                      { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 8322 "bison-chapel.cpp"
    break;

  case 290: /* catch_expr: ident_def TCOLON expr  */
#line 1292 "chapel.ypp"
                        { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 8328 "bison-chapel.cpp"
    break;

  case 291: /* throw_stmt: TTHROW expr TSEMI  */
#line 1296 "chapel.ypp"
                    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 8334 "bison-chapel.cpp"
    break;

  case 292: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1300 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 8340 "bison-chapel.cpp"
    break;

  case 293: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1302 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 8346 "bison-chapel.cpp"
    break;

  case 294: /* when_stmt_ls: %empty  */
#line 1306 "chapel.ypp"
                          { (yyval.pblockstmt) = buildChapelStmt(); }
#line 8352 "bison-chapel.cpp"
    break;

  case 295: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1307 "chapel.ypp"
                          { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 8358 "bison-chapel.cpp"
    break;

  case 296: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1312 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 8364 "bison-chapel.cpp"
    break;

  case 297: /* when_stmt: TOTHERWISE stmt  */
#line 1314 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8370 "bison-chapel.cpp"
    break;

  case 298: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1316 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 8376 "bison-chapel.cpp"
    break;

  case 299: /* class_decl_stmt: class_tag ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1323 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8390 "bison-chapel.cpp"
    break;

  case 300: /* class_decl_stmt: class_tag ident_def opt_inherit TLCBR error TRCBR  */
#line 1333 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 8404 "bison-chapel.cpp"
    break;

  case 301: /* class_tag: TCLASS  */
#line 1346 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8414 "bison-chapel.cpp"
    break;

  case 302: /* class_tag: TRECORD  */
#line 1351 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8424 "bison-chapel.cpp"
    break;

  case 303: /* class_tag: TUNION  */
#line 1356 "chapel.ypp"
           {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 8434 "bison-chapel.cpp"
    break;

  case 304: /* opt_inherit: %empty  */
#line 1364 "chapel.ypp"
                  { (yyval.pcallexpr) = NULL; }
#line 8440 "bison-chapel.cpp"
    break;

  case 305: /* opt_inherit: TCOLON expr_ls  */
#line 1365 "chapel.ypp"
                  { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 8446 "bison-chapel.cpp"
    break;

  case 306: /* class_level_stmt_ls: %empty  */
#line 1370 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 8452 "bison-chapel.cpp"
    break;

  case 307: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 1372 "chapel.ypp"
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 8458 "bison-chapel.cpp"
    break;

  case 308: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 1374 "chapel.ypp"
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 8464 "bison-chapel.cpp"
    break;

  case 309: /* enum_decl_stmt: enum_header ident_def TLCBR enum_ls TRCBR  */
#line 1379 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[-4].penumtype);
      for_vector(DefExpr, ec, *(yyvsp[-1].pvecOfDefs)) {
        ec->sym->type = pdt;
        pdt->constants.insertAtTail(ec);
        if (pdt->defaultValue == NULL) {
          pdt->defaultValue = ec->sym;
        }
      }
      delete (yyvsp[-1].pvecOfDefs);
      pdt->doc = (yylsp[-4]).comment;
      TypeSymbol* pst = new TypeSymbol((yyvsp[-3].pch), pdt);
      (yyvsp[-4].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
#line 8484 "bison-chapel.cpp"
    break;

  case 310: /* enum_decl_stmt: enum_header ident_def TLCBR error TRCBR  */
#line 1395 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 8492 "bison-chapel.cpp"
    break;

  case 311: /* enum_header: TENUM  */
#line 1402 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 8502 "bison-chapel.cpp"
    break;

  case 312: /* enum_ls: deprecated_enum_item  */
#line 1411 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 8514 "bison-chapel.cpp"
    break;

  case 313: /* enum_ls: enum_ls TCOMMA  */
#line 1419 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 8522 "bison-chapel.cpp"
    break;

  case 314: /* enum_ls: enum_ls TCOMMA deprecated_enum_item  */
#line 1423 "chapel.ypp"
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 8530 "bison-chapel.cpp"
    break;

  case 316: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL enum_item  */
#line 1431 "chapel.ypp"
{ (yyval.pdefexpr) = buildDeprecated((yyvsp[0].pdefexpr), (yyvsp[-1].pch)); }
#line 8536 "bison-chapel.cpp"
    break;

  case 317: /* deprecated_enum_item: TDEPRECATED enum_item  */
#line 1433 "chapel.ypp"
{ (yyval.pdefexpr) = buildDeprecated((yyvsp[0].pdefexpr)); }
#line 8542 "bison-chapel.cpp"
    break;

  case 318: /* enum_item: ident_def  */
#line 1437 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 8548 "bison-chapel.cpp"
    break;

  case 319: /* enum_item: ident_def TASSIGN expr  */
#line 1438 "chapel.ypp"
                          { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 8554 "bison-chapel.cpp"
    break;

  case 320: /* $@9: %empty  */
#line 1443 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 8563 "bison-chapel.cpp"
    break;

  case 321: /* $@10: %empty  */
#line 1448 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8572 "bison-chapel.cpp"
    break;

  case 322: /* lambda_decl_expr: TLAMBDA $@9 req_formal_ls $@10 opt_ret_tag opt_type opt_lifetime_where function_body_stmt  */
#line 1453 "chapel.ypp"
    {
      (yyvsp[-5].pfnsymbol)->retTag = (yyvsp[-3].retTag);
      if ((yyvsp[-3].retTag) == RET_REF || (yyvsp[-3].retTag) == RET_CONST_REF)
        USR_FATAL("'ref' return types are not allowed in lambdas");
      if ((yyvsp[-3].retTag) == RET_PARAM)
        USR_FATAL("'param' return types are not allowed in lambdas");
      if ((yyvsp[-3].retTag) == RET_TYPE)
        USR_FATAL("'type' return types are not allowed in lambdas");
      if ((yyvsp[-2].pexpr))
        (yyvsp[-5].pfnsymbol)->retExprType = new BlockStmt((yyvsp[-2].pexpr), BLOCK_SCOPELESS);
      if ((yyvsp[-1].lifetimeAndWhere).where)
        (yyvsp[-5].pfnsymbol)->where = new BlockStmt((yyvsp[-1].lifetimeAndWhere).where);
      if ((yyvsp[-1].lifetimeAndWhere).lifetime)
        (yyvsp[-5].pfnsymbol)->lifetimeConstraints = new BlockStmt((yyvsp[-1].lifetimeAndWhere).lifetime);
      (yyvsp[-5].pfnsymbol)->insertAtTail((yyvsp[0].pblockstmt));
      (yyval.pexpr) = new DefExpr(buildLambda((yyvsp[-5].pfnsymbol)));
    }
#line 8594 "bison-chapel.cpp"
    break;

  case 323: /* linkage_spec: %empty  */
#line 1475 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 8602 "bison-chapel.cpp"
    break;

  case 324: /* linkage_spec: TINLINE  */
#line 1478 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 8611 "bison-chapel.cpp"
    break;

  case 325: /* linkage_spec: TOVERRIDE  */
#line 1482 "chapel.ypp"
                {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 8620 "bison-chapel.cpp"
    break;

  case 326: /* $@11: %empty  */
#line 1490 "chapel.ypp"
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 8634 "bison-chapel.cpp"
    break;

  case 327: /* $@12: %empty  */
#line 1500 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 8645 "bison-chapel.cpp"
    break;

  case 328: /* fn_decl_stmt: linkage_spec proc_iter_or_op $@11 fn_decl_stmt_inner $@12 opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where opt_function_body_stmt  */
#line 1507 "chapel.ypp"
    {
      FnSymbol* fn = (yyvsp[-6].pfnsymbol);
      FnSymbol* linkageFn = (yyvsp[-9].pfnsymbol);

      fn->copyFlags((yyvsp[-9].pfnsymbol));
      if (*linkageFn->name)
        // The user explicitly named this function (controls mangling).
        fn->cname = linkageFn->name;
      else if (linkageFn->numFormals() == 1)
        // cname should be set based upon param
        fn->insertFormalAtTail(linkageFn->getFormal(1));

      if ((yyvsp[-8].procIterOp) == ProcIterOp_ITER)
      {
        if (fn->hasFlag(FLAG_EXTERN))
          USR_FATAL_CONT(fn, "'iter' is not legal with 'extern'");
        fn->addFlag(FLAG_ITERATOR_FN);
      }
      if ((yyvsp[-8].procIterOp) == ProcIterOp_OP) {
        fn->addFlag(FLAG_OPERATOR);
        if (fn->_this != NULL) {
          updateOpThisTagOrErr(fn);
          setupTypeIntentArg(toArgSymbol(fn->_this));
        }
      }

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[-6].pfnsymbol), (yyvsp[-4].retTag), (yyvsp[-3].pexpr), (yyvsp[-2].b), (yyvsp[-1].lifetimeAndWhere).where, (yyvsp[-1].lifetimeAndWhere).lifetime, (yyvsp[0].pblockstmt), (yylsp[-8]).comment);
      context->latestComment = NULL;
    }
#line 8679 "bison-chapel.cpp"
    break;

  case 329: /* fn_decl_stmt_inner: opt_this_intent_tag fn_ident opt_formal_ls  */
#line 1540 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 8687 "bison-chapel.cpp"
    break;

  case 330: /* fn_decl_stmt_inner: opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 1544 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8696 "bison-chapel.cpp"
    break;

  case 331: /* fn_decl_stmt_inner: opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 1549 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 8704 "bison-chapel.cpp"
    break;

  case 332: /* fn_decl_stmt_inner: opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 1553 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 8713 "bison-chapel.cpp"
    break;

  case 333: /* fn_decl_stmt_inner: opt_this_intent_tag error opt_formal_ls  */
#line 1558 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 8721 "bison-chapel.cpp"
    break;

  case 335: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 1565 "chapel.ypp"
                      { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8727 "bison-chapel.cpp"
    break;

  case 336: /* fn_ident: ident_fn_def  */
#line 1569 "chapel.ypp"
                  { (yyval.pch) = (yyvsp[0].pch); }
#line 8733 "bison-chapel.cpp"
    break;

  case 337: /* fn_ident: TBAND  */
#line 1570 "chapel.ypp"
                 { (yyval.pch) = "&"; }
#line 8739 "bison-chapel.cpp"
    break;

  case 338: /* fn_ident: TBOR  */
#line 1571 "chapel.ypp"
                 { (yyval.pch) = "|"; }
#line 8745 "bison-chapel.cpp"
    break;

  case 339: /* fn_ident: TBXOR  */
#line 1572 "chapel.ypp"
                 { (yyval.pch) = "^"; }
#line 8751 "bison-chapel.cpp"
    break;

  case 340: /* fn_ident: TBNOT  */
#line 1573 "chapel.ypp"
                 { (yyval.pch) = "~"; }
#line 8757 "bison-chapel.cpp"
    break;

  case 341: /* fn_ident: TEQUAL  */
#line 1574 "chapel.ypp"
                 { (yyval.pch) = "=="; }
#line 8763 "bison-chapel.cpp"
    break;

  case 342: /* fn_ident: TNOTEQUAL  */
#line 1575 "chapel.ypp"
                 { (yyval.pch) = "!="; }
#line 8769 "bison-chapel.cpp"
    break;

  case 343: /* fn_ident: TLESSEQUAL  */
#line 1576 "chapel.ypp"
                 { (yyval.pch) = "<="; }
#line 8775 "bison-chapel.cpp"
    break;

  case 344: /* fn_ident: TGREATEREQUAL  */
#line 1577 "chapel.ypp"
                 { (yyval.pch) = ">="; }
#line 8781 "bison-chapel.cpp"
    break;

  case 345: /* fn_ident: TLESS  */
#line 1578 "chapel.ypp"
                 { (yyval.pch) = "<"; }
#line 8787 "bison-chapel.cpp"
    break;

  case 346: /* fn_ident: TGREATER  */
#line 1579 "chapel.ypp"
                 { (yyval.pch) = ">"; }
#line 8793 "bison-chapel.cpp"
    break;

  case 347: /* fn_ident: TPLUS  */
#line 1580 "chapel.ypp"
                 { (yyval.pch) = "+"; }
#line 8799 "bison-chapel.cpp"
    break;

  case 348: /* fn_ident: TMINUS  */
#line 1581 "chapel.ypp"
                 { (yyval.pch) = "-"; }
#line 8805 "bison-chapel.cpp"
    break;

  case 349: /* fn_ident: TSTAR  */
#line 1582 "chapel.ypp"
                 { (yyval.pch) = "*"; }
#line 8811 "bison-chapel.cpp"
    break;

  case 350: /* fn_ident: TDIVIDE  */
#line 1583 "chapel.ypp"
                 { (yyval.pch) = "/"; }
#line 8817 "bison-chapel.cpp"
    break;

  case 351: /* fn_ident: TSHIFTLEFT  */
#line 1584 "chapel.ypp"
                 { (yyval.pch) = "<<"; }
#line 8823 "bison-chapel.cpp"
    break;

  case 352: /* fn_ident: TSHIFTRIGHT  */
#line 1585 "chapel.ypp"
                 { (yyval.pch) = ">>"; }
#line 8829 "bison-chapel.cpp"
    break;

  case 353: /* fn_ident: TMOD  */
#line 1586 "chapel.ypp"
                 { (yyval.pch) = "%"; }
#line 8835 "bison-chapel.cpp"
    break;

  case 354: /* fn_ident: TEXP  */
#line 1587 "chapel.ypp"
                 { (yyval.pch) = "**"; }
#line 8841 "bison-chapel.cpp"
    break;

  case 355: /* fn_ident: TBANG  */
#line 1588 "chapel.ypp"
                 { (yyval.pch) = "!"; }
#line 8847 "bison-chapel.cpp"
    break;

  case 356: /* fn_ident: TBY  */
#line 1589 "chapel.ypp"
                 { (yyval.pch) = "chpl_by"; }
#line 8853 "bison-chapel.cpp"
    break;

  case 357: /* fn_ident: THASH  */
#line 1590 "chapel.ypp"
                 { (yyval.pch) = "#"; }
#line 8859 "bison-chapel.cpp"
    break;

  case 358: /* fn_ident: TALIGN  */
#line 1591 "chapel.ypp"
                 { (yyval.pch) = "chpl_align"; }
#line 8865 "bison-chapel.cpp"
    break;

  case 359: /* fn_ident: TSWAP  */
#line 1592 "chapel.ypp"
                 { (yyval.pch) = "<=>"; }
#line 8871 "bison-chapel.cpp"
    break;

  case 360: /* fn_ident: TIO  */
#line 1593 "chapel.ypp"
                 { (yyval.pch) = "<~>"; }
#line 8877 "bison-chapel.cpp"
    break;

  case 361: /* fn_ident: TINITEQUALS  */
#line 1594 "chapel.ypp"
                 { (yyval.pch) = "init="; }
#line 8883 "bison-chapel.cpp"
    break;

  case 362: /* fn_ident: TCOLON  */
#line 1595 "chapel.ypp"
                 { (yyval.pch) = ":"; }
#line 8889 "bison-chapel.cpp"
    break;

  case 363: /* fn_ident: ident_def TBANG  */
#line 1596 "chapel.ypp"
                  { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 8895 "bison-chapel.cpp"
    break;

  case 364: /* assignop_ident: TASSIGN  */
#line 1600 "chapel.ypp"
                 { (yyval.pch) = "="; }
#line 8901 "bison-chapel.cpp"
    break;

  case 365: /* assignop_ident: TASSIGNPLUS  */
#line 1601 "chapel.ypp"
                 { (yyval.pch) = "+="; }
#line 8907 "bison-chapel.cpp"
    break;

  case 366: /* assignop_ident: TASSIGNMINUS  */
#line 1602 "chapel.ypp"
                 { (yyval.pch) = "-="; }
#line 8913 "bison-chapel.cpp"
    break;

  case 367: /* assignop_ident: TASSIGNMULTIPLY  */
#line 1603 "chapel.ypp"
                  { (yyval.pch) = "*="; }
#line 8919 "bison-chapel.cpp"
    break;

  case 368: /* assignop_ident: TASSIGNDIVIDE  */
#line 1604 "chapel.ypp"
                 { (yyval.pch) = "/="; }
#line 8925 "bison-chapel.cpp"
    break;

  case 369: /* assignop_ident: TASSIGNMOD  */
#line 1605 "chapel.ypp"
                 { (yyval.pch) = "%="; }
#line 8931 "bison-chapel.cpp"
    break;

  case 370: /* assignop_ident: TASSIGNEXP  */
#line 1606 "chapel.ypp"
                 { (yyval.pch) = "**="; }
#line 8937 "bison-chapel.cpp"
    break;

  case 371: /* assignop_ident: TASSIGNBAND  */
#line 1607 "chapel.ypp"
                 { (yyval.pch) = "&="; }
#line 8943 "bison-chapel.cpp"
    break;

  case 372: /* assignop_ident: TASSIGNBOR  */
#line 1608 "chapel.ypp"
                 { (yyval.pch) = "|="; }
#line 8949 "bison-chapel.cpp"
    break;

  case 373: /* assignop_ident: TASSIGNBXOR  */
#line 1609 "chapel.ypp"
                 { (yyval.pch) = "^="; }
#line 8955 "bison-chapel.cpp"
    break;

  case 374: /* assignop_ident: TASSIGNSR  */
#line 1610 "chapel.ypp"
                 { (yyval.pch) = ">>="; }
#line 8961 "bison-chapel.cpp"
    break;

  case 375: /* assignop_ident: TASSIGNSL  */
#line 1611 "chapel.ypp"
                 { (yyval.pch) = "<<="; }
#line 8967 "bison-chapel.cpp"
    break;

  case 376: /* opt_formal_ls: %empty  */
#line 1615 "chapel.ypp"
                     { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 8973 "bison-chapel.cpp"
    break;

  case 377: /* opt_formal_ls: TLP formal_ls TRP  */
#line 1616 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8979 "bison-chapel.cpp"
    break;

  case 378: /* req_formal_ls: TLP formal_ls TRP  */
#line 1620 "chapel.ypp"
                     { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8985 "bison-chapel.cpp"
    break;

  case 379: /* formal_ls_inner: formal  */
#line 1624 "chapel.ypp"
                                 { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 8991 "bison-chapel.cpp"
    break;

  case 380: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 1625 "chapel.ypp"
                                 { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 8997 "bison-chapel.cpp"
    break;

  case 381: /* formal_ls: %empty  */
#line 1629 "chapel.ypp"
                           { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 9003 "bison-chapel.cpp"
    break;

  case 382: /* formal_ls: formal_ls_inner  */
#line 1630 "chapel.ypp"
                           { (yyval.pfnsymbol) = (yyvsp[0].pfnsymbol); }
#line 9009 "bison-chapel.cpp"
    break;

  case 383: /* formal: opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1635 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 9015 "bison-chapel.cpp"
    break;

  case 384: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1637 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 9021 "bison-chapel.cpp"
    break;

  case 385: /* formal: opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1639 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 9027 "bison-chapel.cpp"
    break;

  case 386: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1641 "chapel.ypp"
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr))); }
#line 9033 "bison-chapel.cpp"
    break;

  case 387: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 1643 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9039 "bison-chapel.cpp"
    break;

  case 388: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type var_arg_expr  */
#line 1645 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 9045 "bison-chapel.cpp"
    break;

  case 389: /* opt_intent_tag: %empty  */
#line 1649 "chapel.ypp"
                      { (yyval.pt) = INTENT_BLANK; }
#line 9051 "bison-chapel.cpp"
    break;

  case 390: /* opt_intent_tag: required_intent_tag  */
#line 1650 "chapel.ypp"
                      { (yyval.pt) = (yyvsp[0].pt); }
#line 9057 "bison-chapel.cpp"
    break;

  case 391: /* required_intent_tag: TIN  */
#line 1654 "chapel.ypp"
          { (yyval.pt) = INTENT_IN; }
#line 9063 "bison-chapel.cpp"
    break;

  case 392: /* required_intent_tag: TINOUT  */
#line 1655 "chapel.ypp"
          { (yyval.pt) = INTENT_INOUT; }
#line 9069 "bison-chapel.cpp"
    break;

  case 393: /* required_intent_tag: TOUT  */
#line 1656 "chapel.ypp"
          { (yyval.pt) = INTENT_OUT; }
#line 9075 "bison-chapel.cpp"
    break;

  case 394: /* required_intent_tag: TCONST  */
#line 1657 "chapel.ypp"
          { (yyval.pt) = INTENT_CONST; }
#line 9081 "bison-chapel.cpp"
    break;

  case 395: /* required_intent_tag: TCONST TIN  */
#line 1658 "chapel.ypp"
             { (yyval.pt) = INTENT_CONST_IN; }
#line 9087 "bison-chapel.cpp"
    break;

  case 396: /* required_intent_tag: TCONST TREF  */
#line 1659 "chapel.ypp"
              { (yyval.pt) = INTENT_CONST_REF; }
#line 9093 "bison-chapel.cpp"
    break;

  case 397: /* required_intent_tag: TPARAM  */
#line 1660 "chapel.ypp"
          { (yyval.pt) = INTENT_PARAM; }
#line 9099 "bison-chapel.cpp"
    break;

  case 398: /* required_intent_tag: TREF  */
#line 1661 "chapel.ypp"
          { (yyval.pt) = INTENT_REF; }
#line 9105 "bison-chapel.cpp"
    break;

  case 399: /* required_intent_tag: TTYPE  */
#line 1662 "chapel.ypp"
          { (yyval.pt) = INTENT_TYPE; }
#line 9111 "bison-chapel.cpp"
    break;

  case 400: /* opt_this_intent_tag: %empty  */
#line 1666 "chapel.ypp"
         { (yyval.pt) = INTENT_BLANK; }
#line 9117 "bison-chapel.cpp"
    break;

  case 401: /* opt_this_intent_tag: TPARAM  */
#line 1667 "chapel.ypp"
         { (yyval.pt) = INTENT_PARAM; }
#line 9123 "bison-chapel.cpp"
    break;

  case 402: /* opt_this_intent_tag: TREF  */
#line 1668 "chapel.ypp"
         { (yyval.pt) = INTENT_REF;   }
#line 9129 "bison-chapel.cpp"
    break;

  case 403: /* opt_this_intent_tag: TCONST TREF  */
#line 1669 "chapel.ypp"
                { (yyval.pt) = INTENT_CONST_REF;   }
#line 9135 "bison-chapel.cpp"
    break;

  case 404: /* opt_this_intent_tag: TCONST  */
#line 1670 "chapel.ypp"
         { (yyval.pt) = INTENT_CONST;   }
#line 9141 "bison-chapel.cpp"
    break;

  case 405: /* opt_this_intent_tag: TTYPE  */
#line 1671 "chapel.ypp"
         { (yyval.pt) = INTENT_TYPE;  }
#line 9147 "bison-chapel.cpp"
    break;

  case 406: /* proc_iter_or_op: TPROC  */
#line 1675 "chapel.ypp"
         { (yyval.procIterOp) = ProcIterOp_PROC; }
#line 9153 "bison-chapel.cpp"
    break;

  case 407: /* proc_iter_or_op: TITER  */
#line 1676 "chapel.ypp"
         { (yyval.procIterOp) = ProcIterOp_ITER; }
#line 9159 "bison-chapel.cpp"
    break;

  case 408: /* proc_iter_or_op: TOPERATOR  */
#line 1677 "chapel.ypp"
            { (yyval.procIterOp) = ProcIterOp_OP; }
#line 9165 "bison-chapel.cpp"
    break;

  case 409: /* opt_ret_tag: %empty  */
#line 1681 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 9171 "bison-chapel.cpp"
    break;

  case 410: /* opt_ret_tag: TCONST  */
#line 1682 "chapel.ypp"
              { (yyval.retTag) = RET_VALUE; }
#line 9177 "bison-chapel.cpp"
    break;

  case 411: /* opt_ret_tag: TCONST TREF  */
#line 1683 "chapel.ypp"
              { (yyval.retTag) = RET_CONST_REF; }
#line 9183 "bison-chapel.cpp"
    break;

  case 412: /* opt_ret_tag: TREF  */
#line 1684 "chapel.ypp"
              { (yyval.retTag) = RET_REF; }
#line 9189 "bison-chapel.cpp"
    break;

  case 413: /* opt_ret_tag: TPARAM  */
#line 1685 "chapel.ypp"
              { (yyval.retTag) = RET_PARAM; }
#line 9195 "bison-chapel.cpp"
    break;

  case 414: /* opt_ret_tag: TTYPE  */
#line 1686 "chapel.ypp"
              { (yyval.retTag) = RET_TYPE; }
#line 9201 "bison-chapel.cpp"
    break;

  case 415: /* opt_throws_error: %empty  */
#line 1690 "chapel.ypp"
          { (yyval.b) = false; }
#line 9207 "bison-chapel.cpp"
    break;

  case 416: /* opt_throws_error: TTHROWS  */
#line 1691 "chapel.ypp"
          { (yyval.b) = true;  }
#line 9213 "bison-chapel.cpp"
    break;

  case 417: /* opt_function_body_stmt: TSEMI  */
#line 1694 "chapel.ypp"
            { (yyval.pblockstmt) = NULL; }
#line 9219 "bison-chapel.cpp"
    break;

  case 420: /* function_body_stmt: return_stmt  */
#line 1700 "chapel.ypp"
               { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 9225 "bison-chapel.cpp"
    break;

  case 421: /* query_expr: TQUERIEDIDENT  */
#line 1704 "chapel.ypp"
                      { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 9231 "bison-chapel.cpp"
    break;

  case 422: /* var_arg_expr: TDOTDOTDOT  */
#line 1708 "chapel.ypp"
                         { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9237 "bison-chapel.cpp"
    break;

  case 423: /* var_arg_expr: TDOTDOTDOT expr  */
#line 1709 "chapel.ypp"
                         { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9243 "bison-chapel.cpp"
    break;

  case 424: /* var_arg_expr: TDOTDOTDOT query_expr  */
#line 1710 "chapel.ypp"
                         { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 9253 "bison-chapel.cpp"
    break;

  case 425: /* opt_lifetime_where: %empty  */
#line 1718 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 9259 "bison-chapel.cpp"
    break;

  case 426: /* opt_lifetime_where: TWHERE expr  */
#line 1720 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 9265 "bison-chapel.cpp"
    break;

  case 427: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 1722 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 9271 "bison-chapel.cpp"
    break;

  case 428: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 1724 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9277 "bison-chapel.cpp"
    break;

  case 429: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 1726 "chapel.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9283 "bison-chapel.cpp"
    break;

  case 430: /* lifetime_components_expr: lifetime_expr  */
#line 1731 "chapel.ypp"
  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9289 "bison-chapel.cpp"
    break;

  case 431: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 1733 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9295 "bison-chapel.cpp"
    break;

  case 432: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 1737 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9301 "bison-chapel.cpp"
    break;

  case 433: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 1738 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9307 "bison-chapel.cpp"
    break;

  case 434: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 1739 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9313 "bison-chapel.cpp"
    break;

  case 435: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 1740 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9319 "bison-chapel.cpp"
    break;

  case 436: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 1741 "chapel.ypp"
                                             {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9325 "bison-chapel.cpp"
    break;

  case 437: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 1742 "chapel.ypp"
                                              {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 9331 "bison-chapel.cpp"
    break;

  case 438: /* lifetime_expr: TRETURN lifetime_ident  */
#line 1743 "chapel.ypp"
                         { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 9337 "bison-chapel.cpp"
    break;

  case 439: /* lifetime_ident: TIDENT  */
#line 1748 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 9343 "bison-chapel.cpp"
    break;

  case 440: /* lifetime_ident: TTHIS  */
#line 1750 "chapel.ypp"
  { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 9349 "bison-chapel.cpp"
    break;

  case 441: /* type_alias_decl_stmt: TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1755 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 9355 "bison-chapel.cpp"
    break;

  case 442: /* type_alias_decl_stmt: TCONFIG TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1757 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 9361 "bison-chapel.cpp"
    break;

  case 443: /* type_alias_decl_stmt: TEXTERN TTYPE type_alias_decl_stmt_inner TSEMI  */
#line 1759 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 9367 "bison-chapel.cpp"
    break;

  case 444: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 1764 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 9384 "bison-chapel.cpp"
    break;

  case 445: /* type_alias_decl_stmt_inner: ident_def opt_init_type TCOMMA type_alias_decl_stmt_inner  */
#line 1777 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 9402 "bison-chapel.cpp"
    break;

  case 446: /* opt_init_type: %empty  */
#line 1793 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
#line 9408 "bison-chapel.cpp"
    break;

  case 447: /* opt_init_type: TASSIGN type_level_expr  */
#line 1795 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9414 "bison-chapel.cpp"
    break;

  case 448: /* opt_init_type: TASSIGN array_type  */
#line 1797 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 9420 "bison-chapel.cpp"
    break;

  case 449: /* var_decl_type: TPARAM  */
#line 1801 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 9426 "bison-chapel.cpp"
    break;

  case 450: /* var_decl_type: TCONST  */
#line 1802 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 9432 "bison-chapel.cpp"
    break;

  case 451: /* var_decl_type: TREF  */
#line 1803 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 9438 "bison-chapel.cpp"
    break;

  case 452: /* var_decl_type: TCONST TREF  */
#line 1804 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 9444 "bison-chapel.cpp"
    break;

  case 453: /* var_decl_type: TVAR  */
#line 1805 "chapel.ypp"
              { (yyval.pflagset) = buildVarDeclFlags(); }
#line 9450 "bison-chapel.cpp"
    break;

  case 454: /* var_decl_stmt: TCONFIG var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1810 "chapel.ypp"
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9460 "bison-chapel.cpp"
    break;

  case 455: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1816 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 9469 "bison-chapel.cpp"
    break;

  case 457: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 1825 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 9478 "bison-chapel.cpp"
    break;

  case 458: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 1833 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 9484 "bison-chapel.cpp"
    break;

  case 459: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 1835 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 9490 "bison-chapel.cpp"
    break;

  case 460: /* tuple_var_decl_component: TUNDERSCORE  */
#line 1840 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 9496 "bison-chapel.cpp"
    break;

  case 461: /* tuple_var_decl_component: ident_def  */
#line 1842 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 9502 "bison-chapel.cpp"
    break;

  case 462: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 1844 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 9508 "bison-chapel.cpp"
    break;

  case 463: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 1849 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 9514 "bison-chapel.cpp"
    break;

  case 464: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA  */
#line 1851 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 9520 "bison-chapel.cpp"
    break;

  case 465: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA tuple_var_decl_stmt_inner_ls  */
#line 1853 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 9526 "bison-chapel.cpp"
    break;

  case 466: /* opt_init_expr: %empty  */
#line 1859 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9532 "bison-chapel.cpp"
    break;

  case 467: /* opt_init_expr: TASSIGN TNOINIT  */
#line 1860 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 9538 "bison-chapel.cpp"
    break;

  case 468: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 1861 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9544 "bison-chapel.cpp"
    break;

  case 469: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 1867 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9550 "bison-chapel.cpp"
    break;

  case 470: /* ret_array_type: TLSBR TRSBR  */
#line 1869 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 9556 "bison-chapel.cpp"
    break;

  case 471: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 1871 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9564 "bison-chapel.cpp"
    break;

  case 472: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 1875 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 9572 "bison-chapel.cpp"
    break;

  case 473: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 1879 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9578 "bison-chapel.cpp"
    break;

  case 474: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 1881 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9586 "bison-chapel.cpp"
    break;

  case 475: /* ret_array_type: TLSBR error TRSBR  */
#line 1885 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9594 "bison-chapel.cpp"
    break;

  case 476: /* opt_ret_type: %empty  */
#line 1892 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9600 "bison-chapel.cpp"
    break;

  case 477: /* opt_ret_type: TCOLON type_level_expr  */
#line 1893 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9606 "bison-chapel.cpp"
    break;

  case 478: /* opt_ret_type: TCOLON ret_array_type  */
#line 1894 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9612 "bison-chapel.cpp"
    break;

  case 479: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 1895 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9618 "bison-chapel.cpp"
    break;

  case 480: /* opt_ret_type: error  */
#line 1896 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9624 "bison-chapel.cpp"
    break;

  case 481: /* opt_type: %empty  */
#line 1901 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9630 "bison-chapel.cpp"
    break;

  case 482: /* opt_type: TCOLON type_level_expr  */
#line 1902 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9636 "bison-chapel.cpp"
    break;

  case 483: /* opt_type: TCOLON array_type  */
#line 1903 "chapel.ypp"
                          { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 9642 "bison-chapel.cpp"
    break;

  case 484: /* opt_type: TCOLON reserved_type_ident_use  */
#line 1904 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9648 "bison-chapel.cpp"
    break;

  case 485: /* opt_type: error  */
#line 1905 "chapel.ypp"
                          { (yyval.pexpr) = NULL; }
#line 9654 "bison-chapel.cpp"
    break;

  case 486: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 1926 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 9662 "bison-chapel.cpp"
    break;

  case 487: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 1930 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 9670 "bison-chapel.cpp"
    break;

  case 488: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 1934 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 9682 "bison-chapel.cpp"
    break;

  case 489: /* array_type: TLSBR error TRSBR  */
#line 1942 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 9690 "bison-chapel.cpp"
    break;

  case 490: /* opt_formal_array_elt_type: %empty  */
#line 1948 "chapel.ypp"
                        { (yyval.pexpr) = NULL; }
#line 9696 "bison-chapel.cpp"
    break;

  case 491: /* opt_formal_array_elt_type: type_level_expr  */
#line 1949 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9702 "bison-chapel.cpp"
    break;

  case 492: /* opt_formal_array_elt_type: query_expr  */
#line 1950 "chapel.ypp"
                        { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9708 "bison-chapel.cpp"
    break;

  case 493: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 1955 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9714 "bison-chapel.cpp"
    break;

  case 494: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 1957 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9720 "bison-chapel.cpp"
    break;

  case 495: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 1963 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 9726 "bison-chapel.cpp"
    break;

  case 496: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 1965 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 9732 "bison-chapel.cpp"
    break;

  case 497: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 1967 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 9738 "bison-chapel.cpp"
    break;

  case 498: /* opt_formal_type: %empty  */
#line 1971 "chapel.ypp"
                            { (yyval.pexpr) = NULL; }
#line 9744 "bison-chapel.cpp"
    break;

  case 499: /* opt_formal_type: TCOLON type_level_expr  */
#line 1972 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9750 "bison-chapel.cpp"
    break;

  case 500: /* opt_formal_type: TCOLON query_expr  */
#line 1973 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9756 "bison-chapel.cpp"
    break;

  case 501: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 1974 "chapel.ypp"
                                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9762 "bison-chapel.cpp"
    break;

  case 502: /* opt_formal_type: TCOLON formal_array_type  */
#line 1975 "chapel.ypp"
                            { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9768 "bison-chapel.cpp"
    break;

  case 503: /* expr_ls: expr  */
#line 1981 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9774 "bison-chapel.cpp"
    break;

  case 504: /* expr_ls: query_expr  */
#line 1982 "chapel.ypp"
                             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9780 "bison-chapel.cpp"
    break;

  case 505: /* expr_ls: expr_ls TCOMMA expr  */
#line 1983 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9786 "bison-chapel.cpp"
    break;

  case 506: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 1984 "chapel.ypp"
                             { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9792 "bison-chapel.cpp"
    break;

  case 507: /* simple_expr_ls: expr  */
#line 1988 "chapel.ypp"
                                   { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 9798 "bison-chapel.cpp"
    break;

  case 508: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 1989 "chapel.ypp"
                                   { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9804 "bison-chapel.cpp"
    break;

  case 509: /* tuple_component: TUNDERSCORE  */
#line 1993 "chapel.ypp"
                { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 9810 "bison-chapel.cpp"
    break;

  case 510: /* tuple_component: opt_try_expr  */
#line 1994 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9816 "bison-chapel.cpp"
    break;

  case 511: /* tuple_component: query_expr  */
#line 1995 "chapel.ypp"
                { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9822 "bison-chapel.cpp"
    break;

  case 512: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 1999 "chapel.ypp"
                                         { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9828 "bison-chapel.cpp"
    break;

  case 513: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2000 "chapel.ypp"
                                       { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9834 "bison-chapel.cpp"
    break;

  case 514: /* opt_actual_ls: %empty  */
#line 2004 "chapel.ypp"
             { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 9840 "bison-chapel.cpp"
    break;

  case 516: /* actual_ls: actual_expr  */
#line 2009 "chapel.ypp"
                                { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9846 "bison-chapel.cpp"
    break;

  case 517: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2010 "chapel.ypp"
                                { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9852 "bison-chapel.cpp"
    break;

  case 518: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2014 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9858 "bison-chapel.cpp"
    break;

  case 519: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2015 "chapel.ypp"
                                 { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9864 "bison-chapel.cpp"
    break;

  case 520: /* actual_expr: query_expr  */
#line 2016 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9870 "bison-chapel.cpp"
    break;

  case 521: /* actual_expr: opt_try_expr  */
#line 2017 "chapel.ypp"
                             { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9876 "bison-chapel.cpp"
    break;

  case 522: /* ident_expr: ident_use  */
#line 2021 "chapel.ypp"
                 { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9882 "bison-chapel.cpp"
    break;

  case 523: /* ident_expr: scalar_type  */
#line 2022 "chapel.ypp"
                 { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9888 "bison-chapel.cpp"
    break;

  case 524: /* type_level_expr: sub_type_level_expr  */
#line 2034 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9894 "bison-chapel.cpp"
    break;

  case 525: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2036 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 9900 "bison-chapel.cpp"
    break;

  case 526: /* type_level_expr: TQUESTION  */
#line 2038 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9906 "bison-chapel.cpp"
    break;

  case 532: /* sub_type_level_expr: TSINGLE expr  */
#line 2049 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 9912 "bison-chapel.cpp"
    break;

  case 533: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2051 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 9918 "bison-chapel.cpp"
    break;

  case 534: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2053 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 9924 "bison-chapel.cpp"
    break;

  case 535: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2055 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 9930 "bison-chapel.cpp"
    break;

  case 536: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2057 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pexpr)); }
#line 9936 "bison-chapel.cpp"
    break;

  case 537: /* sub_type_level_expr: TATOMIC expr  */
#line 2059 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 9942 "bison-chapel.cpp"
    break;

  case 538: /* sub_type_level_expr: TSYNC expr  */
#line 2061 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 9948 "bison-chapel.cpp"
    break;

  case 539: /* sub_type_level_expr: TOWNED  */
#line 2064 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 9954 "bison-chapel.cpp"
    break;

  case 540: /* sub_type_level_expr: TOWNED expr  */
#line 2066 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 9960 "bison-chapel.cpp"
    break;

  case 541: /* sub_type_level_expr: TUNMANAGED  */
#line 2068 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 9966 "bison-chapel.cpp"
    break;

  case 542: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2070 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9972 "bison-chapel.cpp"
    break;

  case 543: /* sub_type_level_expr: TSHARED  */
#line 2072 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 9978 "bison-chapel.cpp"
    break;

  case 544: /* sub_type_level_expr: TSHARED expr  */
#line 2074 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 9984 "bison-chapel.cpp"
    break;

  case 545: /* sub_type_level_expr: TBORROWED  */
#line 2076 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 9990 "bison-chapel.cpp"
    break;

  case 546: /* sub_type_level_expr: TBORROWED expr  */
#line 2078 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9996 "bison-chapel.cpp"
    break;

  case 547: /* sub_type_level_expr: TCLASS  */
#line 2081 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 10002 "bison-chapel.cpp"
    break;

  case 548: /* sub_type_level_expr: TRECORD  */
#line 2083 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 10008 "bison-chapel.cpp"
    break;

  case 549: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2088 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10014 "bison-chapel.cpp"
    break;

  case 550: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2090 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 10020 "bison-chapel.cpp"
    break;

  case 551: /* for_expr: TFOR expr TDO expr  */
#line 2092 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10026 "bison-chapel.cpp"
    break;

  case 552: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 2094 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10032 "bison-chapel.cpp"
    break;

  case 553: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2096 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 10038 "bison-chapel.cpp"
    break;

  case 554: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 2098 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10044 "bison-chapel.cpp"
    break;

  case 555: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 2100 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10050 "bison-chapel.cpp"
    break;

  case 556: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 2102 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 10056 "bison-chapel.cpp"
    break;

  case 557: /* for_expr: TFORALL expr TDO expr  */
#line 2104 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10062 "bison-chapel.cpp"
    break;

  case 558: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 2106 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10068 "bison-chapel.cpp"
    break;

  case 559: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2108 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 10074 "bison-chapel.cpp"
    break;

  case 560: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 2110 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 10080 "bison-chapel.cpp"
    break;

  case 561: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 2112 "chapel.ypp"
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 10091 "bison-chapel.cpp"
    break;

  case 562: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 2119 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 10101 "bison-chapel.cpp"
    break;

  case 563: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 2125 "chapel.ypp"
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 10111 "bison-chapel.cpp"
    break;

  case 564: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 2131 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10121 "bison-chapel.cpp"
    break;

  case 565: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 2137 "chapel.ypp"
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 10131 "bison-chapel.cpp"
    break;

  case 566: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 2146 "chapel.ypp"
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10137 "bison-chapel.cpp"
    break;

  case 567: /* nil_expr: TNIL  */
#line 2155 "chapel.ypp"
            { (yyval.pexpr) = new SymExpr(gNil); }
#line 10143 "bison-chapel.cpp"
    break;

  case 575: /* stmt_level_expr: io_expr TIO expr  */
#line 2171 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10149 "bison-chapel.cpp"
    break;

  case 576: /* opt_task_intent_ls: %empty  */
#line 2175 "chapel.ypp"
                                { (yyval.pcallexpr) = NULL; }
#line 10155 "bison-chapel.cpp"
    break;

  case 578: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 2180 "chapel.ypp"
                                { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10161 "bison-chapel.cpp"
    break;

  case 579: /* task_intent_ls: intent_expr  */
#line 2184 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10167 "bison-chapel.cpp"
    break;

  case 580: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 2185 "chapel.ypp"
                                                    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10173 "bison-chapel.cpp"
    break;

  case 581: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 2189 "chapel.ypp"
                                  { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 10179 "bison-chapel.cpp"
    break;

  case 582: /* forall_intent_ls: intent_expr  */
#line 2193 "chapel.ypp"
              { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10185 "bison-chapel.cpp"
    break;

  case 583: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 2194 "chapel.ypp"
                                                    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 10191 "bison-chapel.cpp"
    break;

  case 584: /* intent_expr: shadow_var_prefix ident_expr opt_type opt_init_expr  */
#line 2199 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 10199 "bison-chapel.cpp"
    break;

  case 585: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 2203 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10207 "bison-chapel.cpp"
    break;

  case 586: /* intent_expr: expr TREDUCE ident_expr  */
#line 2207 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 10215 "bison-chapel.cpp"
    break;

  case 587: /* shadow_var_prefix: TCONST  */
#line 2213 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 10221 "bison-chapel.cpp"
    break;

  case 588: /* shadow_var_prefix: TIN  */
#line 2214 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_IN;        }
#line 10227 "bison-chapel.cpp"
    break;

  case 589: /* shadow_var_prefix: TCONST TIN  */
#line 2215 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 10233 "bison-chapel.cpp"
    break;

  case 590: /* shadow_var_prefix: TREF  */
#line 2216 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_REF;       }
#line 10239 "bison-chapel.cpp"
    break;

  case 591: /* shadow_var_prefix: TCONST TREF  */
#line 2217 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 10245 "bison-chapel.cpp"
    break;

  case 592: /* shadow_var_prefix: TVAR  */
#line 2218 "chapel.ypp"
               { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 10251 "bison-chapel.cpp"
    break;

  case 594: /* io_expr: io_expr TIO expr  */
#line 2224 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10257 "bison-chapel.cpp"
    break;

  case 595: /* new_maybe_decorated: TNEW  */
#line 2229 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 10263 "bison-chapel.cpp"
    break;

  case 596: /* new_maybe_decorated: TNEW TOWNED  */
#line 2231 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 10271 "bison-chapel.cpp"
    break;

  case 597: /* new_maybe_decorated: TNEW TSHARED  */
#line 2235 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 10279 "bison-chapel.cpp"
    break;

  case 598: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 2239 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 10287 "bison-chapel.cpp"
    break;

  case 599: /* new_maybe_decorated: TNEW TBORROWED  */
#line 2243 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 10295 "bison-chapel.cpp"
    break;

  case 600: /* new_expr: new_maybe_decorated expr  */
#line 2251 "chapel.ypp"
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 10302 "bison-chapel.cpp"
    break;

  case 601: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2256 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10312 "bison-chapel.cpp"
    break;

  case 602: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2262 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 10322 "bison-chapel.cpp"
    break;

  case 603: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2268 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10333 "bison-chapel.cpp"
    break;

  case 604: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2275 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 10344 "bison-chapel.cpp"
    break;

  case 605: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 2285 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 10350 "bison-chapel.cpp"
    break;

  case 615: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 2302 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 10356 "bison-chapel.cpp"
    break;

  case 616: /* expr: expr TCOLON expr  */
#line 2304 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10362 "bison-chapel.cpp"
    break;

  case 617: /* expr: expr TDOTDOT expr  */
#line 2306 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10368 "bison-chapel.cpp"
    break;

  case 618: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 2308 "chapel.ypp"
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr), false, true); }
#line 10374 "bison-chapel.cpp"
    break;

  case 619: /* expr: expr TDOTDOT  */
#line 2323 "chapel.ypp"
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[-1].pexpr)); }
#line 10380 "bison-chapel.cpp"
    break;

  case 620: /* expr: TDOTDOT expr  */
#line 2325 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr)); }
#line 10386 "bison-chapel.cpp"
    break;

  case 621: /* expr: TDOTDOTOPENHIGH expr  */
#line 2327 "chapel.ypp"
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr), true); }
#line 10392 "bison-chapel.cpp"
    break;

  case 622: /* expr: TDOTDOT  */
#line 2329 "chapel.ypp"
    { (yyval.pexpr) = buildUnboundedRange(); }
#line 10398 "bison-chapel.cpp"
    break;

  case 623: /* opt_expr: %empty  */
#line 2333 "chapel.ypp"
                  { (yyval.pexpr) = NULL; }
#line 10404 "bison-chapel.cpp"
    break;

  case 624: /* opt_expr: expr  */
#line 2334 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10410 "bison-chapel.cpp"
    break;

  case 625: /* opt_try_expr: TTRY expr  */
#line 2338 "chapel.ypp"
                  { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 10416 "bison-chapel.cpp"
    break;

  case 626: /* opt_try_expr: TTRYBANG expr  */
#line 2339 "chapel.ypp"
                  { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 10422 "bison-chapel.cpp"
    break;

  case 627: /* opt_try_expr: expr  */
#line 2340 "chapel.ypp"
                  { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10428 "bison-chapel.cpp"
    break;

  case 632: /* call_base_expr: lhs_expr  */
#line 2356 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10434 "bison-chapel.cpp"
    break;

  case 633: /* call_base_expr: expr TBANG  */
#line 2357 "chapel.ypp"
                     { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10440 "bison-chapel.cpp"
    break;

  case 634: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 2358 "chapel.ypp"
                                {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 10446 "bison-chapel.cpp"
    break;

  case 635: /* call_base_expr: lambda_decl_expr  */
#line 2359 "chapel.ypp"
                     { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 10452 "bison-chapel.cpp"
    break;

  case 637: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 2364 "chapel.ypp"
                                              { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10458 "bison-chapel.cpp"
    break;

  case 638: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 2365 "chapel.ypp"
                                              { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 10464 "bison-chapel.cpp"
    break;

  case 639: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 2366 "chapel.ypp"
                                        { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 10470 "bison-chapel.cpp"
    break;

  case 640: /* dot_expr: expr TDOT ident_use  */
#line 2370 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 10476 "bison-chapel.cpp"
    break;

  case 641: /* dot_expr: expr TDOT TTYPE  */
#line 2371 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 10482 "bison-chapel.cpp"
    break;

  case 642: /* dot_expr: expr TDOT TDOMAIN  */
#line 2372 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 10488 "bison-chapel.cpp"
    break;

  case 643: /* dot_expr: expr TDOT TLOCALE  */
#line 2373 "chapel.ypp"
                               { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 10494 "bison-chapel.cpp"
    break;

  case 644: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 2374 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 10500 "bison-chapel.cpp"
    break;

  case 645: /* parenthesized_expr: TLP tuple_component TRP  */
#line 2382 "chapel.ypp"
                                    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 10506 "bison-chapel.cpp"
    break;

  case 646: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 2383 "chapel.ypp"
                                    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 10512 "bison-chapel.cpp"
    break;

  case 647: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 2384 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 10518 "bison-chapel.cpp"
    break;

  case 648: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 2385 "chapel.ypp"
                                    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 10524 "bison-chapel.cpp"
    break;

  case 649: /* bool_literal: TFALSE  */
#line 2389 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gFalse); }
#line 10530 "bison-chapel.cpp"
    break;

  case 650: /* bool_literal: TTRUE  */
#line 2390 "chapel.ypp"
         { (yyval.pexpr) = new SymExpr(gTrue); }
#line 10536 "bison-chapel.cpp"
    break;

  case 651: /* str_bytes_literal: STRINGLITERAL  */
#line 2394 "chapel.ypp"
                  { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 10542 "bison-chapel.cpp"
    break;

  case 652: /* str_bytes_literal: BYTESLITERAL  */
#line 2395 "chapel.ypp"
                  { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 10548 "bison-chapel.cpp"
    break;

  case 655: /* literal_expr: INTLITERAL  */
#line 2401 "chapel.ypp"
                        { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 10554 "bison-chapel.cpp"
    break;

  case 656: /* literal_expr: REALLITERAL  */
#line 2402 "chapel.ypp"
                        { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 10560 "bison-chapel.cpp"
    break;

  case 657: /* literal_expr: IMAGLITERAL  */
#line 2403 "chapel.ypp"
                        { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 10566 "bison-chapel.cpp"
    break;

  case 658: /* literal_expr: CSTRINGLITERAL  */
#line 2404 "chapel.ypp"
                        { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 10572 "bison-chapel.cpp"
    break;

  case 659: /* literal_expr: TNONE  */
#line 2405 "chapel.ypp"
                        { (yyval.pexpr) = new SymExpr(gNone); }
#line 10578 "bison-chapel.cpp"
    break;

  case 660: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 2406 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr),
                                            new SymExpr(gTrue)); }
#line 10585 "bison-chapel.cpp"
    break;

  case 661: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 2408 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr),
                                                   new SymExpr(gTrue)); }
#line 10592 "bison-chapel.cpp"
    break;

  case 662: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 2410 "chapel.ypp"
                        { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 10598 "bison-chapel.cpp"
    break;

  case 663: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 2411 "chapel.ypp"
                               { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 10604 "bison-chapel.cpp"
    break;

  case 664: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 2413 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 10612 "bison-chapel.cpp"
    break;

  case 665: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 2417 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 10620 "bison-chapel.cpp"
    break;

  case 666: /* assoc_expr_ls: expr TALIAS expr  */
#line 2424 "chapel.ypp"
                                        { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10626 "bison-chapel.cpp"
    break;

  case 667: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 2425 "chapel.ypp"
                                        { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 10632 "bison-chapel.cpp"
    break;

  case 668: /* binary_op_expr: expr TPLUS expr  */
#line 2429 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10638 "bison-chapel.cpp"
    break;

  case 669: /* binary_op_expr: expr TMINUS expr  */
#line 2430 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10644 "bison-chapel.cpp"
    break;

  case 670: /* binary_op_expr: expr TSTAR expr  */
#line 2431 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10650 "bison-chapel.cpp"
    break;

  case 671: /* binary_op_expr: expr TDIVIDE expr  */
#line 2432 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10656 "bison-chapel.cpp"
    break;

  case 672: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 2433 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10662 "bison-chapel.cpp"
    break;

  case 673: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 2434 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10668 "bison-chapel.cpp"
    break;

  case 674: /* binary_op_expr: expr TMOD expr  */
#line 2435 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10674 "bison-chapel.cpp"
    break;

  case 675: /* binary_op_expr: expr TEQUAL expr  */
#line 2436 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10680 "bison-chapel.cpp"
    break;

  case 676: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 2437 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10686 "bison-chapel.cpp"
    break;

  case 677: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 2438 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10692 "bison-chapel.cpp"
    break;

  case 678: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 2439 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10698 "bison-chapel.cpp"
    break;

  case 679: /* binary_op_expr: expr TLESS expr  */
#line 2440 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10704 "bison-chapel.cpp"
    break;

  case 680: /* binary_op_expr: expr TGREATER expr  */
#line 2441 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10710 "bison-chapel.cpp"
    break;

  case 681: /* binary_op_expr: expr TBAND expr  */
#line 2442 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10716 "bison-chapel.cpp"
    break;

  case 682: /* binary_op_expr: expr TBOR expr  */
#line 2443 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10722 "bison-chapel.cpp"
    break;

  case 683: /* binary_op_expr: expr TBXOR expr  */
#line 2444 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10728 "bison-chapel.cpp"
    break;

  case 684: /* binary_op_expr: expr TAND expr  */
#line 2445 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10734 "bison-chapel.cpp"
    break;

  case 685: /* binary_op_expr: expr TOR expr  */
#line 2446 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10740 "bison-chapel.cpp"
    break;

  case 686: /* binary_op_expr: expr TEXP expr  */
#line 2447 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10746 "bison-chapel.cpp"
    break;

  case 687: /* binary_op_expr: expr TBY expr  */
#line 2448 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10752 "bison-chapel.cpp"
    break;

  case 688: /* binary_op_expr: expr TALIGN expr  */
#line 2449 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10758 "bison-chapel.cpp"
    break;

  case 689: /* binary_op_expr: expr THASH expr  */
#line 2450 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10764 "bison-chapel.cpp"
    break;

  case 690: /* binary_op_expr: expr TDMAPPED expr  */
#line 2451 "chapel.ypp"
                           { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr),
                                               new SymExpr(gTrue)); }
#line 10771 "bison-chapel.cpp"
    break;

  case 691: /* unary_op_expr: TPLUS expr  */
#line 2456 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 10777 "bison-chapel.cpp"
    break;

  case 692: /* unary_op_expr: TMINUS expr  */
#line 2457 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 10783 "bison-chapel.cpp"
    break;

  case 693: /* unary_op_expr: TMINUSMINUS expr  */
#line 2458 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 10789 "bison-chapel.cpp"
    break;

  case 694: /* unary_op_expr: TPLUSPLUS expr  */
#line 2459 "chapel.ypp"
                                  { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 10795 "bison-chapel.cpp"
    break;

  case 695: /* unary_op_expr: TBANG expr  */
#line 2460 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 10801 "bison-chapel.cpp"
    break;

  case 696: /* unary_op_expr: expr TBANG  */
#line 2461 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 10807 "bison-chapel.cpp"
    break;

  case 697: /* unary_op_expr: TBNOT expr  */
#line 2462 "chapel.ypp"
                                  { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 10813 "bison-chapel.cpp"
    break;

  case 698: /* reduce_expr: expr TREDUCE expr  */
#line 2466 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10819 "bison-chapel.cpp"
    break;

  case 699: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 2467 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10825 "bison-chapel.cpp"
    break;

  case 700: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 2468 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10831 "bison-chapel.cpp"
    break;

  case 701: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 2469 "chapel.ypp"
                                                 { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10837 "bison-chapel.cpp"
    break;

  case 702: /* scan_expr: expr TSCAN expr  */
#line 2473 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10843 "bison-chapel.cpp"
    break;

  case 703: /* scan_expr: expr TSCAN zippered_iterator  */
#line 2474 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10849 "bison-chapel.cpp"
    break;

  case 704: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 2475 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10855 "bison-chapel.cpp"
    break;

  case 705: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 2476 "chapel.ypp"
                                               { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10861 "bison-chapel.cpp"
    break;

  case 706: /* reduce_scan_op_expr: TPLUS  */
#line 2481 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 10867 "bison-chapel.cpp"
    break;

  case 707: /* reduce_scan_op_expr: TSTAR  */
#line 2482 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 10873 "bison-chapel.cpp"
    break;

  case 708: /* reduce_scan_op_expr: TAND  */
#line 2483 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 10879 "bison-chapel.cpp"
    break;

  case 709: /* reduce_scan_op_expr: TOR  */
#line 2484 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 10885 "bison-chapel.cpp"
    break;

  case 710: /* reduce_scan_op_expr: TBAND  */
#line 2485 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 10891 "bison-chapel.cpp"
    break;

  case 711: /* reduce_scan_op_expr: TBOR  */
#line 2486 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 10897 "bison-chapel.cpp"
    break;

  case 712: /* reduce_scan_op_expr: TBXOR  */
#line 2487 "chapel.ypp"
         { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 10903 "bison-chapel.cpp"
    break;


#line 10907 "bison-chapel.cpp"

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
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

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, context);
          yychar = YYEMPTY;
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
  if (yychar != YYEMPTY)
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
#undef yynerrs
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
